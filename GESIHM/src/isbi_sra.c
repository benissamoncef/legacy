/*E*/
/*Fichier :  $Id: isbi_sra.c,v 1.10 2021/01/06 09:19:35 akkaakka Exp $     Release : $Revision: 1.10 $        Date : $Date: 2021/01/06 09:19:35 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR  
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE MSRA * FICHIER isbi_sra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Creation et Mise a jour des Variables d'animations 
*   graphiques
*   
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	12 Sep 1994	: Creation
* VR  29/05/12 : PRV DEM 1016
* JMG 15/03/13 : filtre
* JMG 20/03/17 : saga DEM1191
* LCL 17/12/17 : optim tisup-tigmot
* JMG 07/03/20 : EMS 1.8
* PNI 11/03/20 : Abt Panneaux sur DP pour TOULON SAE-113
* LCL 20/12/21 : MOVIS P2 ems SAE-312
* LCL 11/05/23 : MAGISTRA Ajout mise en cache
* LCL 27/09/23 : Adaptations CNA DEM-483
* GGY 23/11/23 : Ajout MSRA15_raf_vag_PIC (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

#include "xdm.h"
#include "xzsa.h"
#include "xzsi.h"
#include "isbi_sra.h"
#include "../inc/uthash.h"

/* definitions de constantes */

#define CM_TEXTE 0
#define CM_TEMP  1
#define CM_HEURE  2


#define CM_LIGNE_BASCOTE 1
#define CM_LIGNE_BAU     2
#define CM_LIGNE_LENTE   3
#define CM_LIGNE_MEDIANE1 4
#define CM_LIGNE_MEDIANE2 5
#define CM_LIGNE_RAPIDE  6
#define CM_LIGNE_TPC     7

#define CM_LIGNE_BAU_NORD     3
#define CM_LIGNE_BAU_SUD      4
#define CM_LIGNE_CHAUSSEE_NORD     1
#define CM_LIGNE_CHAUSSEE_SUD      2

#define CM_INVIS 0
#define CM_TRAIT 1
#define CM_FLECH 2
#define CM_NEUT 3

#define CM_MAX_SEUIL 32767

#define CM_BUFF_SOCK 600

/* definitions de types locaux */
    
    
/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)isbi_sra.c	1.54 07/11/07 : isbi_sra" ;

/* Variables memorisant les etats de Syntheses Machines
*       et Reseau */
int     vs_MachSynt             = 0xffff;
int     vs_ResSynt              = 0xffff;


struct data_struct {
	char key[50];             /* key (string is WITHIN the structure) */
	char	value[10];
	UT_hash_handle hh;         /* makes this structure hashable */
};
struct data_struct *datas = NULL;

void add_data(char *key, char *value)
{
        struct data_struct *s;
        s=(struct data_struct*)malloc(sizeof(struct data_struct));
        strncpy(s->key, key,50);
        strncpy(s->value, value, 10);
        HASH_ADD_STR( datas, key, s );
}

void delete_data(struct data_struct *message)
{
        HASH_DEL(datas, message);
        free(message);
}

void get_data(char *key, char *value, struct data_struct *outMessage)
{
	HASH_FIND_STR( datas, key, outMessage);
	if (outMessage == NULL)
 	{
		XZST_03EcritureTrace(XZSTC_DEBUG1, "cle %s jamais enregistree",key);
		strcpy(value,"");
		return;
	}
	XZST_03EcritureTrace(XZSTC_DEBUG1, "valeur trouvee %s",outMessage->value);
	strcpy(value , outMessage->value);
}

int hash_data(char *key, int value)
{
  int valeur;
  char vl_str[10];
  char vl_value[10];
  struct data_struct *mess;

  sprintf(vl_str,"%d",value);
  get_data(key, vl_value, mess);
  HASH_FIND_STR( datas, key, mess);
  if (mess == NULL)
  {
	XZST_03EcritureTrace(XZSTC_DEBUG1, "cle %s jamais enregistree",key);
	add_data(key,vl_str);
        return(1);
  }
  XZST_03EcritureTrace(XZSTC_DEBUG1, "valeur trouvee %s",mess->value);
  if (!strcmp(vl_str,mess->value))
    return(0);
  XZST_03EcritureTrace(XZSTC_DEBUG1, "avant delete");
  delete_data(mess);
  XZST_03EcritureTrace(XZSTC_DEBUG1, "apres delete");
  add_data(key, vl_str);
  XZST_03EcritureTrace(XZSTC_DEBUG1, "apres add");

  return(1);
}

tm_radtanim	*pm_radtanim;
static int 		vm_nb_radtanim;

static int vm_MSRA_init = XDC_NON;
static    float  vm_high_range;
static    float  vm_low_range;
XDY_Booleen vm_cx;
int vg_nb_abtradt=0;
char vl_msg[CM_BUFF_SOCK];

XDY_Booleen vm_traitement_bloc=XDC_FAUX;

/* declaration de fonctions internes */
int   MSRA_Creer_thash_mob();

void 	MSRA_Abonnements_radt();
void 	MSRA_Abonnements_fav();


int	MSRA_LectureRADRAnim();

void cb_null(T_IPC_CONN , T_IPC_CONN_PROCESS_CB_DATA      , T_CB_ARG                        );

FILE	*vg_tracefile=NULL;

/* definition de fonctions externes */
extern void MSRA01_raf_vag_PMV_Message(); /* Rafraichissement des vag d'un PMV message */
extern void MSRA02_raf_vag_PMV_Picto();
extern void MSRA03_raf_vag_TRA();
extern void MSRA03_raf_vag_DIV();
extern void MSRA03_raf_vag_NivTRA();
extern void MSRA03_raf_RADT();
extern void MSRA04_raf_vag_NAV();
extern void MSRA05_raf_vag_DAI();
extern void MSRA06_raf_vag_PAU();
extern void MSRA07_raf_vag_CAMERA();
extern void MSRA10_raf_vag_FAV();
extern void MSRA11_raf_vag_Panneaux();
extern void MSRA17_raf_vag_EqtSigGTCRenovee();
extern void MSRA13_raf_vag_Pan_Alerte();
extern void MSRA12_raf_vag_PMVA();
extern void MSRA13_raf_vag_BAF();
extern void MSRA14_raf_vag_BAD();
extern void MSRA15_raf_vag_PRV();
extern void MSRA15_raf_vag_SAGA();
extern void MSRA16_raf_vag_PIC();
extern void MSRA_raf_journuit();
extern void MSRA_raf_SeqTunnel();
extern void MSRA_raf_SeqEch();
extern void MSRA_raf_SeqGTCRenovee();
extern void  MSRA12_raf_vag_STRADA();
extern void  MSRA16_raf_vag_ZDP();

extern int   MSRA_init_tabordobj();
extern int   MSRA_Tri_obj();

extern int   MSOP_Inserer_fop();
extern int   MSOP_Detruire_fop();
extern int MSOP_init_trt_actions_operateur();
extern void MSMA_arret();

extern int MSRA_Init_Recept();

extern void MSOP_Affiche_elt_fop();

extern int send_message();
extern int canWeSend(char *);

extern void MajAdm_Mach_Synt(
        XZSCT_NomMachine        va_NomMachine,
        int                     va_EtatMach);

extern void MajAdm_Ent_Synt(
        XZSCT_NomEntite         va_NomEntite,
        int                     va_EtatEnt,
        int                     va_TypeEnt);

extern void MajAdm_Res_Synt(
        XZSCT_NomMachine        va_NomMachine,
        int                     va_EtatMach);

extern void MajSynt_Mach_Synop(
        XZSCT_NomMachine pa_NomMachine,
        int              va_NumOrdre,
        void             **pa_DonneesMsg);
extern void MajSynt_Ent_Synop(
        XZSCT_NomEntite pa_NomEntite,
        int              va_NumOrdre,
        void             **pa_DonneesMsg);
extern void MajSynt_Res_Synop(
        XZSCT_NomMachine pa_NomMachine,
        int              va_NumOrdre,
        void             **pa_DonneesMsg);

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Initialisation du module MSRA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSRA_init_rafraichissement_synopt(
                    argc, argv)

int	argc;
char	**argv;


/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   Creation des tables de hash VAG et pour les mobiles
*   et recuperation du nom du site
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra.c	1.54 07/11/07 : MSRA_init_rafraichissement_synopt";
   int code_retour;
   XZSCT_Datagroup         vl_datagroup_init ;
   int i=0;
   char vl_chaine[20];
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_init_rafraichissement_synopt" );
   
   /*A Initialisation si cela n'a pas deja ete fait */
   if ( vm_MSRA_init == XDC_NON )
      {
      vm_MSRA_init = XDC_OUI;
      
      vm_traitement_bloc=XDC_FAUX;
      
      /*A Initialisation du module MSOP */
      /*
      if ( MSOP_init_trt_actions_operateur() == XDC_NOK )
         {
         XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation du module MSOP" );
         return XDC_NOK;
         }
        */

      /*A Creation de la table de hash mob */
      //MSRA_Creer_thash_mob( &vm_thash_mob );
   
      /*A Init du tableau de ordres des objets */
      /*MSRA_init_tabordobj(); fait dans MSOP */
      
      /*A Init du tableau des tstaions a animer si PC simplifie */
         if ( vm_TypeMachine == XZSCC_TYPEM_PCS )
            {
      		MSRA_LectureRADRAnim();
      	    }
      
      if ( vm_cx == XDC_OUI )
         {
         /*A Abonnement aux Messages de rafraichissement des Objets */
         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_PMV , vm_NomMachine );
         
         XZEA08_Abt_Etat_PMV ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA01_raf_vag_PMV_Message , XDC_NOM_SITE_VC , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnment XZEA08_Abt_Etat_PMV : %d " , code_retour );
            return code_retour;
            }
    
         XZEA08_Abt_Etat_PMV ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA01_raf_vag_PMV_Message , XDC_NOM_SITE_DP , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnment XZEA08_Abt_Etat_PMV : %d " , code_retour );
            return code_retour;
            }

         XZEA08_Abt_Etat_PMV ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA01_raf_vag_PMV_Message , XDC_NOM_SITE_CA , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnment XZEA08_Abt_Etat_PMV : %d " , code_retour );
            return code_retour;
            }

    
         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_PICTO , vm_NomMachine );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA09_Abt_Etat_Picto " );
         XZEA09_Abt_Etat_Picto ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA02_raf_vag_PMV_Picto , XDC_NOM_SITE_VC , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out XZEA09_Abt_Etat_Picto : %d " , code_retour );
         if ( code_retour != XDC_OK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return code_retour;
            }       
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA09_Abt_Etat_Picto " );
         XZEA09_Abt_Etat_Picto ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA02_raf_vag_PMV_Picto , XDC_NOM_SITE_DP , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out XZEA09_Abt_Etat_Picto : %d " , code_retour );
         if ( code_retour != XDC_OK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return code_retour;
            }
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA09_Abt_Etat_Picto " );
         XZEA09_Abt_Etat_Picto ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA02_raf_vag_PMV_Picto , XDC_NOM_SITE_CA , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out XZEA09_Abt_Etat_Picto : %d " , code_retour );
         if ( code_retour != XDC_OK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return code_retour;
            }
       
         XZEA080_Abt_Etat_PMVA ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA12_raf_vag_PMVA, XDC_NOM_SITE_VC , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnment XZEA080_Abt_Etat_PMVA : %d " , code_retour );
            return ;
            }
         XZEA080_Abt_Etat_PMVA ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA12_raf_vag_PMVA, XDC_NOM_SITE_DP , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnment XZEA080_Abt_Etat_PMVA : %d " , code_retour );
            return ;
            }
         XZEA080_Abt_Etat_PMVA ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA12_raf_vag_PMVA, XDC_NOM_SITE_CA , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnment XZEA080_Abt_Etat_PMVA : %d " , code_retour );
            return ;
            }
 
	      XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_BAF , vm_NomMachine );
         XZEA40_Abt_Etat_BAF    ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA13_raf_vag_BAF , XDC_NOM_SITE_VC , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
          XZEA40_Abt_Etat_BAF    ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA13_raf_vag_BAF , XDC_NOM_SITE_DP , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         XZEA40_Abt_Etat_BAF    ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA13_raf_vag_BAF , XDC_NOM_SITE_CA , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }

         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_BAD , vm_NomMachine );
         XZEA41_Abt_Etat_BAD    ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA14_raf_vag_BAD , XDC_NOM_SITE_VC , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         XZEA41_Abt_Etat_BAD    ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA14_raf_vag_BAD , XDC_NOM_SITE_DP , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         XZEA41_Abt_Etat_BAD    ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA14_raf_vag_BAD , XDC_NOM_SITE_CA , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }

         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_PRV , vm_NomMachine );
         XZEA70_Abt_Etat_PRV    ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA15_raf_vag_PRV , XDC_NOM_SITE_VC , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         XZEA70_Abt_Etat_PRV    ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA15_raf_vag_PRV , XDC_NOM_SITE_DP , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         XZEA70_Abt_Etat_PRV    ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA15_raf_vag_PRV , XDC_NOM_SITE_CA , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }

		XDG_EncodeDG2 (vl_datagroup_init, XDG_IABTINIT_PIC, vm_NomMachine);
		XZEA71_Abt_Etat_PIC (XZEAC_DEBUT_ABONNEMENT, vl_datagroup_init, MSRA16_raf_vag_PIC, XDC_NOM_SITE_VC, &code_retour);
		if ( code_retour != XDC_OK )
		{
			XZST_03EcritureTrace (XZSTC_WARNING, " Probleme lors de l'abonnement : %d.", code_retour);
			return ;
		}
		XZEA71_Abt_Etat_PIC (XZEAC_DEBUT_ABONNEMENT, vl_datagroup_init, MSRA16_raf_vag_PIC, XDC_NOM_SITE_DP, &code_retour);
		if ( code_retour != XDC_OK )
		{
			XZST_03EcritureTrace (XZSTC_WARNING, " Probleme lors de l'abonnement  : %d.", code_retour);
			return ;
		}
		XZEA71_Abt_Etat_PIC (XZEAC_DEBUT_ABONNEMENT, vl_datagroup_init, MSRA16_raf_vag_PIC, XDC_NOM_SITE_CA, &code_retour);
		if ( code_retour != XDC_OK )
		{
			XZST_03EcritureTrace (XZSTC_WARNING, " Probleme lors de l'abonnement : %d ", code_retour );
			return ;
		}

         /*A Abonnement pour toutes les radt si pas PC simplifie */
         if ( vm_TypeMachine != XZSCC_TYPEM_PCS )
            {
          XZEA02_Abt_Mesures_6gl_Chaussee ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA03_raf_vag_TRA , (char *)  vm_NomSite ,&code_retour );
            }
         else
            {
            for (i=0;i < vm_nb_radtanim; i++)
               {
               /*A Abonnement  XZEA32_Abt_Mesures_6mn_parVoie si PC simplife */
               XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_RADT , vm_NomMachine );
               XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA32_Abt_Mesures_6mn_parVoie " );
	            XZST_03EcritureTrace(   XZSTC_WARNING , "  %d: numero=%d sens=%d", i,pm_radtanim[i].Numero, pm_radtanim[i].Sens);
               XZEA32_Abt_Mesures_6mn_parVoie( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA03_raf_RADT , pm_radtanim[i].Numero , pm_radtanim[i].Sens, 0 , &code_retour );
               XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA32_Abt_Mesures_6mn_parVoie %d " , code_retour );
               if ( code_retour != XDC_OK )
                  {
                  XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
                  return code_retour;
                  }
               }
            }
      

         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_NIV , vm_NomMachine );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA05_Abt_Niveaux_Service " );
         XZEA05_Abt_Niveaux_Service ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA03_raf_vag_NivTRA , XDC_NOM_SITE_VC , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA05_Abt_Niveaux_Service %d " , code_retour );
         if ( code_retour != XDC_OK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return code_retour;
            }
        XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA05_Abt_Niveaux_Service " );
         XZEA05_Abt_Niveaux_Service ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA03_raf_vag_NivTRA , XDC_NOM_SITE_DP , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA05_Abt_Niveaux_Service %d " , code_retour );
         if ( code_retour != XDC_OK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return code_retour;
            }
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA05_Abt_Niveaux_Service " );
         XZEA05_Abt_Niveaux_Service ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA03_raf_vag_NivTRA , XDC_NOM_SITE_CA , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA05_Abt_Niveaux_Service %d " , code_retour );
         if ( code_retour != XDC_OK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return code_retour;
            }
         
         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_NAV , vm_NomMachine );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA10_Abt_Etat_NAV " );
         XZEA10_Abt_Etat_NAV    ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA04_raf_vag_NAV , XDC_NOM_SITE_VC , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  :XZEA10_Abt_Etat_NAV  %d " , code_retour );
         if ( code_retour != XDC_OK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return code_retour;
            }         
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA10_Abt_Etat_NAV " );
         XZEA10_Abt_Etat_NAV    ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA04_raf_vag_NAV , XDC_NOM_SITE_DP , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  :XZEA10_Abt_Etat_NAV  %d " , code_retour );
         if ( code_retour != XDC_OK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return code_retour;
            }
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA10_Abt_Etat_NAV " );
         XZEA10_Abt_Etat_NAV    ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA04_raf_vag_NAV , XDC_NOM_SITE_CA , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  :XZEA10_Abt_Etat_NAV  %d " , code_retour );
         if ( code_retour != XDC_OK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return code_retour;
            }
         
         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_PAL , vm_NomMachine );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA38_Abt_Etat_PAL " );
         XZEA38_Abt_Etat_PAL    ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA13_raf_vag_Pan_Alerte , XDC_NOM_SITE_VC , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  :XZEA38_Abt_Etat_PAL  %d " , code_retour );
         if ( code_retour != XDC_OK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return code_retour;
            }
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA38_Abt_Etat_PAL " );
         XZEA38_Abt_Etat_PAL    ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA13_raf_vag_Pan_Alerte , XDC_NOM_SITE_DP , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  :XZEA38_Abt_Etat_PAL  %d " , code_retour );
         if ( code_retour != XDC_OK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return code_retour;
            }
          /*if ( ( strcmp( vm_NomSite , XDC_NOM_SITE_CI ) != NULL ) && ( vm_TypeMachine != XZSCC_TYPEM_PCS ))*/
         if ( vm_TypeMachine != XZSCC_TYPEM_PCS )
            {
            XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_RAU , vm_NomMachine );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA14_Abt_Etat_RAU " );
            XZEA14_Abt_Etat_RAU ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA06_raf_vag_PAU , vm_NomSite , &code_retour );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA14_Abt_Etat_RAU %d " , code_retour );
            if ( code_retour != XDC_OK )
               {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return code_retour;
               }
            }
         
         /*A on s'abonne a jour&nuit, tube, ech */
         
         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_JOURNUIT , vm_NomMachine );
      
         /*A Si on est au CI on s'abt pour jour nuit aux 2 district, sinon seult au district d'appartenance*/
         if ( strcmp( vm_NomSite , XDC_NOM_SITE_CI ) == NULL )
            {
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA29_Abt_Etat_JourNuit " );
            XZEA29_Abt_Etat_JourNuit ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA_raf_journuit, XDC_NOM_SITE_VC , &code_retour );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA29_Abt_Etat_JourNuit %d " , code_retour );
            if ( code_retour != XDC_OK )
               {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return code_retour;
               }
            }
         else
            {
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA29_Abt_Etat_JourNuit " );
            XZEA29_Abt_Etat_JourNuit ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA_raf_journuit, vm_NomSite , &code_retour );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA29_Abt_Etat_JourNuit %d " , code_retour );
            if ( code_retour != XDC_OK )
               {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return code_retour;
               }
            }
            
         /*A si PCS on ne s'abonne pas a  tube, ech */
         if ( vm_TypeMachine != XZSCC_TYPEM_PCS )
	      {
            XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_TUB , vm_NomMachine );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA23_Abt_Etat_Sequence_Tube " );
            XZEA23_Abt_Etat_Sequence_Tube ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA_raf_SeqTunnel,XDC_NOM_SITE_VC , XDC_EQTALL , &code_retour );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA23_Abt_Etat_Sequence_Tube %d " , code_retour );
            if ( code_retour != XDC_OK )
            {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return code_retour;
            }
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA23_Abt_Etat_Sequence_Tube " );
            XZEA23_Abt_Etat_Sequence_Tube ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA_raf_SeqTunnel,XDC_NOM_SITE_DP , XDC_EQTALL , &code_retour );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA23_Abt_Etat_Sequence_Tube %d " , code_retour );
            if ( code_retour != XDC_OK )
            {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return code_retour;
            }
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA23_Abt_Etat_Sequence_Tube " );
            XZEA23_Abt_Etat_Sequence_Tube ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA_raf_SeqTunnel,XDC_NOM_SITE_CA , XDC_EQTALL , &code_retour );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA23_Abt_Etat_Sequence_Tube %d " , code_retour );
            if ( code_retour != XDC_OK )
            {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return code_retour;
            }
            
            XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_ECH , vm_NomMachine );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA24_Abt_Etat_Sequence_Ech " );
            XZEA24_Abt_Etat_Sequence_Ech ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA_raf_SeqEch, XDC_NOM_SITE_VC , XDC_EQTALL , &code_retour );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA24_Abt_Etat_Sequence_Ech %d " , code_retour );
            if ( code_retour != XDC_OK )
            {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return code_retour;
            }
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA24_Abt_Etat_Sequence_Ech " );
            XZEA24_Abt_Etat_Sequence_Ech ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA_raf_SeqEch, XDC_NOM_SITE_DP , XDC_EQTALL , &code_retour );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA24_Abt_Etat_Sequence_Ech %d " , code_retour );
            if ( code_retour != XDC_OK )
            {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return code_retour;
            }
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA24_Abt_Etat_Sequence_Ech " );
            XZEA24_Abt_Etat_Sequence_Ech ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA_raf_SeqEch, XDC_NOM_SITE_CA , XDC_EQTALL , &code_retour );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA24_Abt_Etat_Sequence_Ech %d " , code_retour );
            if ( code_retour != XDC_OK )
            {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return code_retour;
            }
           
            /* Abonnement aux sequences signalisation renovee */
            XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_TUB , vm_NomMachine );
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Abonnement aux sequences GTC Renovee - Tube, site DP");
            XZEA50_Abt_Etat_Sequence_SIG ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA_raf_SeqGTCRenovee, XDC_NOM_SITE_DP , XDC_EQT_TUB, XDC_EQTALL , &code_retour );
            if ( code_retour != XDC_OK )
            {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return;
            }
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Abonnement aux sequences GTC Renovee - Tube, site VC");
            XZEA50_Abt_Etat_Sequence_SIG ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA_raf_SeqGTCRenovee, XDC_NOM_SITE_VC , XDC_EQT_TUB, XDC_EQTALL , &code_retour );
            if ( code_retour != XDC_OK )
            {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return;
            }
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Abonnement aux sequences GTC Renovee - Tube, site CA");
            XZEA50_Abt_Etat_Sequence_SIG ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA_raf_SeqGTCRenovee, XDC_NOM_SITE_CA , XDC_EQT_TUB, XDC_EQTALL , &code_retour );
            if ( code_retour != XDC_OK )
            {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return;
            }

            XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_ECH , vm_NomMachine );
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Abonnement aux sequences GTC Renovee - Echangeur, site DP");
            XZEA50_Abt_Etat_Sequence_SIG ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA_raf_SeqGTCRenovee, XDC_NOM_SITE_DP , XDC_EQT_ECH, XDC_EQTALL , &code_retour );
            if ( code_retour != XDC_OK )
            {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return;
            }
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Abonnement aux sequences GTC Renovee - Echangeur, site VC");
            XZEA50_Abt_Etat_Sequence_SIG ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA_raf_SeqGTCRenovee, XDC_NOM_SITE_VC , XDC_EQT_ECH, XDC_EQTALL , &code_retour );
            if ( code_retour != XDC_OK )
            {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return;
            }
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Abonnement aux sequences GTC Renovee - Echangeur, site CA");
            XZEA50_Abt_Etat_Sequence_SIG ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA_raf_SeqGTCRenovee, XDC_NOM_SITE_CA , XDC_EQT_ECH, XDC_EQTALL , &code_retour );
            if ( code_retour != XDC_OK )
            {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return;
            }

		   
           if ( (strcmp( vm_NomSite , XDC_NOM_SITE_CI)==NULL) || 
                (strcmp( vm_NomSite , XDC_NOM_SITE_DP)==NULL) ||
                (strcmp( vm_NomSite , XDC_NOM_SITE_VC)==NULL) ||
                (strcmp( vm_NomSite , XDC_NOM_SITE_CA)==NULL) )
            {
               XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_PAN , vm_NomMachine );
               XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA18_Abt_Etat_Panneaux, site DP " );
               XZEA18_Abt_Etat_Panneaux ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA11_raf_vag_Panneaux , XDC_NOM_SITE_DP , XDC_EQT_TUBE , XDC_EQTALL,&code_retour );
               if ( code_retour != XDC_OK )
               {
                  XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
                  return code_retour;
               }
               XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA18_Abt_Etat_Panneaux, site VC " );
               XZEA18_Abt_Etat_Panneaux ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA11_raf_vag_Panneaux , XDC_NOM_SITE_VC , XDC_EQT_TUBE , XDC_EQTALL,&code_retour );
               if ( code_retour != XDC_OK )
               {
                  XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
                  return code_retour;
               }
               XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA18_Abt_Etat_Panneaux, site CA " );
               XZEA18_Abt_Etat_Panneaux ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA11_raf_vag_Panneaux , XDC_NOM_SITE_CA , XDC_EQT_TUBE , XDC_EQTALL,&code_retour );
               if ( code_retour != XDC_OK )
               {
                  XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
                  return code_retour;
               }
      
               /* Abonnement aux equipements de la GTC renovee */
               XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA42_Abt_Etat_EqtSIG, site DP " );
               XZEA42_Abt_Etat_EqtSIG ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA17_raf_vag_EqtSigGTCRenovee , XDC_NOM_SITE_DP , XDC_EQT_SIG, XDC_EQT_TUBE, XDC_EQTALL,&code_retour );								
               XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA42_Abt_Etat_EqtSIG %d " , code_retour );
               if ( code_retour != XDC_OK )
               {
                  XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
                  return code_retour;
               }
               XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA42_Abt_Etat_EqtSIG, site VC " );
               XZEA42_Abt_Etat_EqtSIG ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA17_raf_vag_EqtSigGTCRenovee , XDC_NOM_SITE_VC , XDC_EQT_SIG, XDC_EQT_TUBE, XDC_EQTALL,&code_retour );								
               XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA42_Abt_Etat_EqtSIG %d " , code_retour );
               if ( code_retour != XDC_OK )
               {
                  XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
                  return code_retour;
               }
               XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA42_Abt_Etat_EqtSIG, site CA " );
               XZEA42_Abt_Etat_EqtSIG ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA17_raf_vag_EqtSigGTCRenovee , XDC_NOM_SITE_CA , XDC_EQT_SIG, XDC_EQT_TUBE, XDC_EQTALL,&code_retour );								
               XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA42_Abt_Etat_EqtSIG %d " , code_retour );
               if ( code_retour != XDC_OK )
               {
                  XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
                  return code_retour;
               }
            }
               
               
            /*A
            ** Abonnement aux donnees de mesures Meteo.
            */

            if ( ( XZEA36_Abt_TdP_6mn (MSRA16_raf_vag_ZDP, XDC_NOM_SITE_VC, &code_retour)) != XDC_OK)
            {
               XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZEA36_Abt_TdP_6mn a echoue.",version);
            } 
            else
            {
               XZST_03EcritureTrace(XZSTC_INFO,"%s : abonnement aux donnees TDP 6mn reussi, site VC",version);
            }
            if ( ( XZEA36_Abt_TdP_6mn (MSRA16_raf_vag_ZDP, XDC_NOM_SITE_DP, &code_retour)) != XDC_OK)
            {
               XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZEA36_Abt_TdP_6mn a echoue.",version);
            } 
            else
            {
               XZST_03EcritureTrace(XZSTC_INFO,"%s : abonnement aux donnees TDP 6mn reussi, site DP",version);
            }
            if ( ( XZEA36_Abt_TdP_6mn (MSRA16_raf_vag_ZDP, XDC_NOM_SITE_CA, &code_retour)) != XDC_OK)
            {
               XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZEA36_Abt_TdP_6mn a echoue.",version);
            } 
            else
            {
               XZST_03EcritureTrace(XZSTC_INFO,"%s : abonnement aux donnees TDP 6mn reussi, site CA",version);
            }
                
            
            }	
            
            /*cb pour ne pas etre pollue par les messages 6 minutes voie*/
            TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EMES_6mn_Voie),	cb_null,(T_CB_ARG)NULL);

            if ( MSRA_Init_Recept() != XDC_OK )
            {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'init de la reception des messages de rafraichissement" );
               return XDC_NOK;
            }

         }    
      
   }
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_init_rafraichissement_synopt" );
   return XDC_OK;
}

void cb_null(T_IPC_CONN va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
	T_CB_ARG                        pa_arg){
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Abonnement aux radt 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Abonnements_radt(
          pa_cle,
          va_valeur,
          pl_result)
          
          char *pa_cle;
          int  *va_valeur;
          int *pl_result;

/*
* ARGUMENTS EN ENTREE :
*   
*
*
* ARGUMENTS EN SORTIE :
*   
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*  Creation de la table de hash des mobiles
*
------------------------------------------------------*/
{
   static char *version = "@(#)isbi_sra.c	1.54 07/11/07 :MSRA_Abonnements_radt "; int vl_no;
  int vl_no_auto;
  int code_retour;
  char pl_no[256];
  XZSCT_Datagroup vl_datagroup_init ;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Abonnements_radt" );

   /*A Recupere le no de station a partir du nom de l'objet */
   if ( ( strcmp( pa_cle , "radt" ) != NULL ) &&
        ( strcmp( &pa_cle[strlen(pa_cle)-3] , "txt") != NULL)  )
   {
   sscanf( pa_cle , "radt_%d_A%d" , &vl_no , &vl_no_auto );
   sprintf( pl_no , "%d" , vl_no );
   pl_no[strlen(pl_no)-1]='\000';
   vl_no = atoi( pl_no );
   XZST_03EcritureTrace(   XZSTC_DEBUG1 , " Numero Station %d" , vl_no );
   
   /*A Abonnement XZEA02_Abt_Mesures_6gl_Chaussee et XZEA27_Abt_Classe_DIV */
   XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_RADT , vm_NomMachine );
   XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA02_Abt_Mesures_6gl_Chaussee%d " );
   }
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Abonnements_radt" );
  *pl_result = XDC_OK;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Abonnement aux fav 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Abonnements_fav(
          pa_cle,
          va_valeur,
          pl_result)
          
          char *pa_cle;
          int  *va_valeur;
          int *pl_result;

/*
* ARGUMENTS EN ENTREE :
*   
*
*
* ARGUMENTS EN SORTIE :
*   
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*  Creation de la table de hash des mobiles
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra.c	1.54 07/11/07 :MSRA_Abonnements_fav "; int vl_no;
  int vl_no_auto,vl_pr,vl_sens,vl_voie;
  int code_retour;
  char pl_no[256];
  XZSCT_Datagroup vl_datagroup_init ;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Abonnements_fav" );

   /*A Recupere le no de station a partir du nom de l'objet */
   sscanf( pa_cle , "fav_%d_pr%d_%d_voie%d_%d" , &vl_no , &vl_pr, &vl_sens , &vl_voie , &vl_no_auto );
   
   XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_PAN , vm_NomMachine );
   XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA18_Abt_Etat_Panneaux, site DP, equipement %d ", vl_no );
   XZEA18_Abt_Etat_Panneaux ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA11_raf_vag_Panneaux , XDC_NOM_SITE_DP , XDC_EQT_PAN , (XDY_Eqt) vl_no,&code_retour );
   if ( code_retour != XDC_OK )
   {
      XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
      return code_retour;
   }
   XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA18_Abt_Etat_Panneaux, site VC, equipement %d ", vl_no );
   XZEA18_Abt_Etat_Panneaux ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA11_raf_vag_Panneaux , XDC_NOM_SITE_VC , XDC_EQT_PAN , (XDY_Eqt) vl_no,&code_retour );
   if ( code_retour != XDC_OK )
   {
      XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
      return code_retour;
   }
   XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA18_Abt_Etat_Panneaux, site CA, equipement %d ", vl_no );
   XZEA18_Abt_Etat_Panneaux ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA11_raf_vag_Panneaux , XDC_NOM_SITE_CA , XDC_EQT_PAN , (XDY_Eqt) vl_no,&code_retour );
   if ( code_retour != XDC_OK )
   {
      XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
      return code_retour;
   }
	/* Abonnement aux equipements de la GTC renovee */
   XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA42_Abt_Etat_EqtSIG, site DP " );
   XZEA42_Abt_Etat_EqtSIG ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA17_raf_vag_EqtSigGTCRenovee , XDC_NOM_SITE_DP , XDC_EQT_SIG, XDC_EQT_TUBE, XDC_EQTALL,&code_retour );
   XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA42_Abt_Etat_EqtSIG %d " , code_retour );
   if ( code_retour != XDC_OK )
   {
      XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
      return code_retour;
   }
   XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA42_Abt_Etat_EqtSIG, site VC " );
   XZEA42_Abt_Etat_EqtSIG ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA17_raf_vag_EqtSigGTCRenovee , XDC_NOM_SITE_VC , XDC_EQT_SIG, XDC_EQT_TUBE, XDC_EQTALL,&code_retour );								
   XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA42_Abt_Etat_EqtSIG %d " , code_retour );
   if ( code_retour != XDC_OK )
   {
      XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
      return code_retour;
   }
   XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA42_Abt_Etat_EqtSIG, site CA " );
   XZEA42_Abt_Etat_EqtSIG ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA17_raf_vag_EqtSigGTCRenovee , XDC_NOM_SITE_CA , XDC_EQT_SIG, XDC_EQT_TUBE, XDC_EQTALL,&code_retour );								
   XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA42_Abt_Etat_EqtSIG %d " , code_retour );
   if ( code_retour != XDC_OK )
   {
      XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
      return code_retour;
   }
         
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Abonnements_fav" );
  *pl_result = XDC_OK;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un mobile de basculement de voie
*  et de l'objet associe ayant un coefficient d'extension
------------------------------------------------------
* SEQUENCE D'APPEL :
*/ 

MSRA_raf_Aff_Voies_Prep(
          va_typeraf,
          va_no_fmc,
          va_debutfin,
          va_idportion,
          va_anc_voie1,va_voie1,
          va_anc_voie2,va_voie2,
          va_anc_voie3,va_voie3,
          va_anc_voie4,va_voie4,
          va_anc_voie5,va_voie5,
          va_anc_voie6,va_voie6,
          va_anc_voie7,va_voie7,
          va_anc_voie8,va_voie8,
          va_anc_voie9,va_voie9,
          va_anc_voie10,va_voie10,
          va_coeftrans,
	  va_numordre)
          
          
          int va_typeraf;
          XDY_Evt va_no_fmc;
          XDY_Booleen va_debutfin;
          int va_idportion;
          XDY_Octet va_anc_voie1,va_anc_voie2,va_anc_voie3,va_anc_voie4,va_anc_voie5;
          XDY_Octet va_anc_voie6,va_anc_voie7,va_anc_voie8,va_anc_voie9,va_anc_voie10;
          XDY_Octet va_voie1,va_voie2,va_voie3,va_voie4,va_voie5;
          XDY_Octet va_voie6,va_voie7,va_voie8,va_voie9,va_voie10;
          int va_coeftrans;
	  XDY_Octet va_numordre;

/*
* ARGUMENTS EN ENTREE :
*   va_typeraf : XDC_AJOUT, XDC_MODIF ,XDC_SUPPR : 
*                type de rafraichissement
*   va_no_fmc : No fiche fmc
*   va_debutfin : Bool indiquant basculement de debut ou de fin des tarvaux :
                  XDC_DEBUT_TRAVAUX
                  XDC_FIN_TRAVAUX
*   va_idportion : Identifiant de la portion  valide pour 
*                          va_typeraf=XDC_AJOUT.
*   va_anc_voiex : 0 voie non affectee, 1 voie vers droite, 2 voie ver gauchev
*   va_voiex : 0 voie non affectee, 1 voie vers droite, 2 voie ver gauchev
*   va_lienx : booleen de visibilite du lien entre 2 voies
*   va_coeftrans : Coef de translation [0,1]
*   va_numordre : numero d'ordre du rabattement pour la prisignalisation
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*  Creation d'une zone et de ses VAG
*  Modification des VAG controlant la zone
*  Suppression de la Zone et de ses VAG
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra.c	1.54 07/11/07 : MSRA_raf_Aff_Voies_Prep";
   char vl_nom_mobile[256],
        vl_nom_trans[256],
        vl_cle[256],
        vl_nom_var[256],
        vl_nom_dsv[256],
        vl_nom_dsv_sub[256],
        vl_nom_ligne[256],
        vl_debut_nom_segment[256],
        vl_cle_mob[256];
        
   int i;
   int vl_max=0, vl_min= 10 , vl_max_anc = 0 , vl_min_anc = 10;
   int vl_inv_max=0, vl_inv_min= 10 , vl_inv_max_anc = 0 , vl_inv_min_anc = 10;
   
   int      vl_voie[10];
   int      vl_anc_voie[10];
   int      vl_lien[18];
   int      vl_lien_lin[10];
   
   int      code_retour;
   XDY_Booleen vl_lien_anc=XDC_NON, vl_lien_nv=XDC_NON, vl_lln=XDC_NON, vl_lla=XDC_NON;
   
   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA_raf_Aff_Voies_Prep  ");
   
   
   /*A On saute l'etape de calcule du raf des fleches lors d'une suppression*/
   if ( va_typeraf != XDC_SUPPR )
   {
         vl_voie[0] =  va_voie1 ;
         vl_voie[1] =  va_voie2 ;
         vl_voie[2] =  va_voie3 ;
         vl_voie[3] =  va_voie4 ;
         vl_voie[4] =  va_voie5 ;
         vl_voie[5] =  va_voie6 ;
         vl_voie[6] =  va_voie7 ;
         vl_voie[7] =  va_voie8 ;
         vl_voie[8] =  va_voie9 ;
         vl_voie[9] =  va_voie10 ;
         
         vl_anc_voie[0] =  va_anc_voie1 ;
         vl_anc_voie[1] =  va_anc_voie2 ;
         vl_anc_voie[2] =  va_anc_voie3 ;
         vl_anc_voie[3] =  va_anc_voie4 ;
         vl_anc_voie[4] =  va_anc_voie5 ;
         vl_anc_voie[5] =  va_anc_voie6 ;
         vl_anc_voie[6] =  va_anc_voie7 ;
         vl_anc_voie[7] =  va_anc_voie8 ;
         vl_anc_voie[8] =  va_anc_voie9 ;
         vl_anc_voie[9] =  va_anc_voie10 ;
         
         for ( i = 0; i < 10 ; i++ )
            {
            vl_lien[i] = 0;
            vl_lien_lin[i] = 0;
            }
         for ( i = 10 ; i < 18 ; i++ )
            {
            vl_lien[i] = 0;
            }
         
 
     for ( i = 0; i < 10 ; i++ )
         {
         vl_lln = XDC_NON;
         vl_lla = XDC_NON;
         switch( vl_voie[i])
            {
            case XDC_VOIE_NEUTRALISEE :
            case XDC_VOIE_INEXISTANTE :
               break;
            case XDC_VOIE_SENS_NORMAL :
            case XDC_VOIE_SENS_INVERSE :
               vl_lien_anc = XDC_OUI;
               break;
            }
         switch( vl_anc_voie[i])
            {
            case XDC_VOIE_NEUTRALISEE :
            case XDC_VOIE_INEXISTANTE :
               break;
            case  XDC_VOIE_SENS_NORMAL :
            case XDC_VOIE_SENS_INVERSE :
               vl_lien_nv = XDC_OUI;
               break;
            }
         if ( ( ( vl_anc_voie[i] ==  XDC_VOIE_SENS_NORMAL ) && ( vl_voie[i] == XDC_VOIE_SENS_NORMAL ) ) || 
              ( ( vl_anc_voie[i] == XDC_VOIE_SENS_INVERSE  ) && ( vl_voie[i] == XDC_VOIE_SENS_INVERSE  ) )   )
            {
            vl_lien_lin[i] = 1;
            }
         }
     

     for ( i = 0 ; i < 5 ; i++)
         {
         switch( vl_voie[i])
            {
            case XDC_VOIE_NEUTRALISEE :
               vl_voie[i] = CM_NEUT;
               break;
            case XDC_VOIE_INEXISTANTE :
               vl_voie[i] = CM_INVIS;
               break;
            case XDC_VOIE_SENS_NORMAL :
               vl_voie[i] = CM_TRAIT;
               if ( i > vl_max ) vl_max = i;
               if ( i < vl_min ) vl_min = i;
               break;
            case XDC_VOIE_SENS_INVERSE :
               vl_voie[i] = CM_FLECH;
               if ( i > vl_inv_max ) vl_inv_max = i;
               if ( i < vl_inv_min ) vl_inv_min = i;
            }
         switch( vl_anc_voie[i])
            {
            case XDC_VOIE_NEUTRALISEE :
               vl_anc_voie[i] = CM_NEUT;
               break;
            case XDC_VOIE_INEXISTANTE :
               vl_anc_voie[i]= CM_INVIS;
               break;
            case  XDC_VOIE_SENS_NORMAL :
               vl_anc_voie[i] = CM_TRAIT;
               if ( i > vl_max_anc ) vl_max_anc = i;
               if ( i < vl_min_anc ) vl_min_anc = i;
               break;
            case XDC_VOIE_SENS_INVERSE :
               vl_anc_voie[i] = CM_FLECH;
               if ( i > vl_inv_max_anc ) vl_inv_max_anc = i;
               if ( i < vl_inv_min_anc ) vl_inv_min_anc = i;
            }
         }
         

    if ( vl_lien_nv == XDC_OUI )
     {
     for ( i = vl_min ; i < vl_max ; i++)
        {
        vl_lien[i] = 1;
        }
     for ( i = vl_inv_min ; i < vl_inv_max ; i++)
        {
        vl_lien[i] = 1;
        }
     }
     
    if ( vl_lien_anc == XDC_OUI )
     {
     for ( i = vl_min_anc ; i < vl_max_anc ; i++)
        {
        vl_lien[i+9] = 1;
        }
     for ( i = vl_inv_min_anc ; i < vl_inv_max_anc ; i++)
        {
        vl_lien[i+9] = 1;
        }
     }
        
    
     vl_max=0; vl_min= 10 ; vl_max_anc = 0; vl_min_anc = 10; 
     vl_inv_max=0, vl_inv_min= 10 , vl_inv_max_anc = 0 , vl_inv_min_anc = 10;
             
     for ( i = 5 ; i < 10 ; i++)
         {
         switch( vl_voie[i])
            {
            case XDC_VOIE_NEUTRALISEE :
               vl_voie[i] = CM_NEUT;
               break;
            case XDC_VOIE_INEXISTANTE :
               vl_voie[i] = CM_INVIS;
               break;
            case XDC_VOIE_SENS_NORMAL :
               vl_voie[i] = CM_TRAIT;
               if ( i > vl_max ) vl_max = i;
               if ( i < vl_min ) vl_min = i;
               break;
            case XDC_VOIE_SENS_INVERSE :
               vl_voie[i] = CM_FLECH;
               if ( i > vl_inv_max ) vl_inv_max = i;
               if ( i < vl_inv_min ) vl_inv_min = i;
            }
         switch( vl_anc_voie[i])
            {
            case XDC_VOIE_NEUTRALISEE :
               vl_anc_voie[i] = CM_NEUT;
               break;
            case XDC_VOIE_INEXISTANTE :
               vl_anc_voie[i]= CM_INVIS;
               break;
            case  XDC_VOIE_SENS_NORMAL :
               vl_anc_voie[i] = CM_TRAIT;
               if ( i > vl_max_anc ) vl_max_anc = i;
               if ( i < vl_min_anc ) vl_min_anc = i;
               break;
            case XDC_VOIE_SENS_INVERSE :
               vl_anc_voie[i] = CM_FLECH;
               if ( i > vl_inv_max_anc ) vl_inv_max_anc = i;
               if ( i < vl_inv_min_anc ) vl_inv_min_anc = i;
            }
         }

    if ( vl_lien_nv == XDC_OUI )
     {
     for ( i = vl_min ; i < vl_max ; i++)
        {
        vl_lien[i] = XDC_OUI;
        }
     for ( i = vl_inv_min ; i < vl_inv_max ; i++)
        {
        vl_lien[i] = XDC_OUI;
        }
     }
    
   
    if ( vl_lien_anc == XDC_OUI )
     {
     for ( i = vl_min_anc ; i < vl_max_anc ; i++)
        {
        vl_lien[i+9] = XDC_OUI;
        }
     for ( i = vl_inv_min_anc ; i < vl_inv_max_anc ; i++)
        {
        vl_lien[i+9] = XDC_OUI;
        }
     }

    for (i = 0; i < 10 ; i++)
      {
      if ( ( vl_lien[i] == XDC_OUI ) && ( vl_lien[i+9] == XDC_OUI ) )
        vl_lien[i+9] = XDC_NON;
      }
      
    /* Liens croises */ 
    if ( ( ( vl_voie[4] == XDC_VOIE_SENS_NORMAL ) &&   ( vl_anc_voie[5] ==  XDC_VOIE_SENS_INVERSE ) ) ||
         ( ( vl_voie[4] == XDC_VOIE_SENS_INVERSE ) && ( vl_anc_voie[5] ==  XDC_VOIE_SENS_NORMAL ) ) )
       {
       vl_lien[ 4 ]  = XDC_OUI;
       }
       
    if ( ( ( vl_voie[5] == XDC_VOIE_SENS_NORMAL ) && ( vl_anc_voie[4] == XDC_VOIE_SENS_INVERSE ) ) || 
         ( ( vl_voie[5] == XDC_VOIE_SENS_INVERSE ) && ( vl_anc_voie[4] == XDC_VOIE_SENS_NORMAL ) ) )
       {
       vl_lien[ 13 ]  = XDC_OUI;
       }
        
   }
   
   
   /*A Appel de la fonction de raf et de creation de l'affectation de voies*/        
   code_retour = (MSRA_raf_Aff_Voies(
                 va_typeraf,
                 va_no_fmc,
                 va_debutfin,
                 va_idportion,
                 vl_anc_voie[0],vl_voie[0],
                 vl_anc_voie[1],vl_voie[1],
                 vl_anc_voie[2],vl_voie[2],
                 vl_anc_voie[3],vl_voie[3],
                 vl_anc_voie[4],vl_voie[4],
                 vl_anc_voie[5],vl_voie[5],
                 vl_anc_voie[6],vl_voie[6],
                 vl_anc_voie[7],vl_voie[7],
                 vl_anc_voie[8],vl_voie[8],
                 vl_anc_voie[9],vl_voie[9],
                 vl_lien[0],
                 vl_lien[1],
                 vl_lien[2],
                 vl_lien[3],
                 vl_lien[4],
                 vl_lien[5],
                 vl_lien[6],
                 vl_lien[7],
                 vl_lien[8],
                 vl_lien[9],
                 vl_lien[10],
                 vl_lien[11],
                 vl_lien[12],
                 vl_lien[13],
                 vl_lien[14],
                 vl_lien[15],
                 vl_lien[16],
                 vl_lien[17],
                 vl_lien_lin[0],
                 vl_lien_lin[1],
                 vl_lien_lin[2],
                 vl_lien_lin[3],
                 vl_lien_lin[4],
                 vl_lien_lin[5],
                 vl_lien_lin[6],
                 vl_lien_lin[7],
                 vl_lien_lin[8],
                 vl_lien_lin[9],
                 va_coeftrans,
		 va_numordre )) ;
          
          
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : MSRA_raf_Aff_Voies_Prep  ");
   
   return code_retour;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un mobile de basculement de voie
*  et de l'objet associe ayant un coefficient d'extension
------------------------------------------------------
* SEQUENCE D'APPEL :
*/ 

MSRA_raf_Aff_Voies(
          va_typeraf,
          va_no_fmc,
          va_debutfin,
          va_idportion,
          va_anc_voie1,va_voie1,
          va_anc_voie2,va_voie2,
          va_anc_voie3,va_voie3,
          va_anc_voie4,va_voie4,
          va_anc_voie5,va_voie5,
          va_anc_voie6,va_voie6,
          va_anc_voie7,va_voie7,
          va_anc_voie8,va_voie8,
          va_anc_voie9,va_voie9,
          va_anc_voie10,va_voie10,
          va_lien1,
          va_lien2,
          va_lien3,
          va_lien4,
          va_lien5,
          va_lien6,
          va_lien7,
          va_lien8,
          va_lien9,
          va_lien10,
          va_lien11,
          va_lien12,
          va_lien13,
          va_lien14,
          va_lien15,
          va_lien16,
          va_lien17,
          va_lien18,
          va_lien_lin1,
          va_lien_lin2,
          va_lien_lin3,
          va_lien_lin4,
          va_lien_lin5,
          va_lien_lin6,
          va_lien_lin7,
          va_lien_lin8,
          va_lien_lin9,
          va_lien_lin10,
          va_coeftrans,
	  va_numordre)
          
          
          int va_typeraf;
          XDY_Evt va_no_fmc;
          XDY_Booleen va_debutfin;
          int va_idportion;
          XDY_Octet va_anc_voie1,va_anc_voie2,va_anc_voie3,va_anc_voie4,va_anc_voie5;
          XDY_Octet va_anc_voie6,va_anc_voie7,va_anc_voie8,va_anc_voie9,va_anc_voie10;
          XDY_Octet va_voie1,va_voie2,va_voie3,va_voie4,va_voie5;
          XDY_Octet va_voie6,va_voie7,va_voie8,va_voie9,va_voie10;
          XDY_Octet va_lien1,va_lien2,va_lien3,va_lien4,va_lien5,va_lien6,va_lien7,va_lien8,va_lien9;
          XDY_Octet va_lien10,va_lien11,va_lien12,va_lien13,va_lien14,va_lien15,va_lien16,va_lien17,va_lien18;
          XDY_Octet va_lien_lin1,va_lien_lin2,va_lien_lin3,va_lien_lin4,va_lien_lin5,va_lien_lin6,va_lien_lin7,va_lien_lin8,va_lien_lin9,va_lien_lin10;
          int va_coeftrans;
          XDY_Octet va_numordre;

/*
* ARGUMENTS EN ENTREE :
*   va_typeraf : XDC_AJOUT, XDC_MODIF ,XDC_SUPPR : 
*                type de rafraichissement
*   va_no_fmc : No fiche fmc
*   va_no_act : No action 
*   va_debutfin : Bool indiquant basculement de debut ou de fin des tarvaux :
                  XDC_DEBUT_TRAVAUX
                  XDC_FIN_TRAVAUX
*   va_idportion : Identifiant de la portion  valide pour 
*                          va_typeraf=XDC_AJOUT.
*   va_anc_voiex : 0 voie non affectee, 1 voie vers droite, 2 voie ver gauchev
*   va_voiex : 0 voie non affectee, 1 voie vers droite, 2 voie ver gauchev
*   va_lienx : booleen de visibilite du lien entre 2 voies
*   va_coeftrans : Coef de translation [0,1]
*   va_numordre : numero d'ordre du rabattement pour la prisignalisation*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*  Creation d'une zone et de ses VAG
*  Modification des VAG controlant la zone
*  Suppression de la Zone et de ses VAG
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra.c	1.54 07/11/07 : MSRA_raf_Aff_Voies";
   char vl_nom_mobile[256],
        vl_nom_trans[256],
        vl_cle[256],
        vl_nom_var[256],
        vl_nom_dsv[256],
        vl_nom_dsv_sub[256],
        vl_nom_ligne[256],
        vl_nom_portion[256],
        vl_debut_nom_segment[256],
        vl_cle_mob[256];
        
   int i;
   
   float      vl_voie[10];
   float      vl_anc_voie[10];
   float      vl_lien[18];
   float      vl_lien_lin[10];
   double     vl_angle;
   //float      vl_coef=0.0;
   int      vl_coef=0;
   XDY_Sens   vl_sens=0;
   int 		vl_result;
   char vl_msg2[CM_BUFF_SOCK];
   char vl_msg3[CM_BUFF_SOCK];
   
   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA_raf_Aff_Voies  ");
             
   sprintf( vl_cle , "!%d!%d!%d!%d!" , va_no_fmc.numero , va_no_fmc.cle  , va_debutfin ,(int)va_numordre);
   sprintf( vl_nom_mobile , "MoAF%s"  , vl_cle );
   sprintf( vl_nom_trans , "MoAF%s_translation"  , vl_cle );
   
   
   /*A Creation de la cle pour la table des mobile */
   sprintf( vl_cle_mob , "%s_AF_%06d_%c_%do", vl_cle , va_idportion , 'x' , 4 );
   if ( strlen( vl_cle_mob ) > CM_LONG_CLEF )
      {
      vl_cle_mob[CM_LONG_CLEF]='\000';
      XZST_03EcritureTrace(   XZSTC_WARNING, " Clef aff voie trop longue: tronquee : %s" , vl_cle_mob );
      }
      
   
   if ( va_typeraf != XDC_SUPPR )
      {
      /* Si le coef est < 0, alors on est sur une autoroute inversee et le coeff et compris
         entre -10001 et -1 , et vrai coef = (coeff+10001)sinon autoroute normale et 0<= coef <= 10000 */
      if ( va_coeftrans < 0 )
         {
         //vl_coef = ((float) ( va_coeftrans+10001)) / CM_COEF_TRANS;
         vl_coef = va_coeftrans+10001;
         vl_sens = XDC_SENS_NORD;
         }
      else
         {
         vl_coef = va_coeftrans;
         vl_sens = XDC_SENS_SUD;
         }
      }
    
         vl_voie[0] = (float) ( va_voie1 * 0.25 ) + 0.125;
         vl_voie[1] = (float) ( va_voie2 * 0.25 ) + 0.125;
         vl_voie[2] = (float) ( va_voie3 * 0.25 ) + 0.125;
         vl_voie[3] = (float) ( va_voie4 * 0.25 ) + 0.125;
         vl_voie[4] = (float) ( va_voie5 * 0.25 ) + 0.125;
         vl_voie[5] = (float) ( va_voie6 * 0.25 ) + 0.125;
         vl_voie[6] = (float) ( va_voie7 * 0.25 ) + 0.125;
         vl_voie[7] = (float) ( va_voie8 * 0.25 ) + 0.125;
         vl_voie[8] = (float) ( va_voie9 * 0.25 ) + 0.125;
         vl_voie[9] = (float) ( va_voie10 * 0.25 ) + 0.125;
         
         vl_anc_voie[0] = (float) ( va_anc_voie1 * 0.25 ) + 0.125;
         vl_anc_voie[1] = (float) ( va_anc_voie2 * 0.25 ) + 0.125;
         vl_anc_voie[2] = (float) ( va_anc_voie3 * 0.25 ) + 0.125;
         vl_anc_voie[3] = (float) ( va_anc_voie4 * 0.25 ) + 0.125;
         vl_anc_voie[4] = (float) ( va_anc_voie5 * 0.25 ) + 0.125;
         vl_anc_voie[5] = (float) ( va_anc_voie6 * 0.25 ) + 0.125;
         vl_anc_voie[6] = (float) ( va_anc_voie7 * 0.25 ) + 0.125;
         vl_anc_voie[7] = (float) ( va_anc_voie8 * 0.25 ) + 0.125;
         vl_anc_voie[8] = (float) ( va_anc_voie9 * 0.25 ) + 0.125;
         vl_anc_voie[9] = (float) ( va_anc_voie10 * 0.25 ) + 0.125;
         
         vl_lien_lin[0] = (float) va_lien_lin1; 
         vl_lien_lin[1] = (float) va_lien_lin2; 
         vl_lien_lin[2] = (float) va_lien_lin3; 
         vl_lien_lin[3] = (float) va_lien_lin4; 
         vl_lien_lin[4] = (float) va_lien_lin5; 
         vl_lien_lin[5] = (float) va_lien_lin6; 
         vl_lien_lin[6] = (float) va_lien_lin7; 
         vl_lien_lin[7] = (float) va_lien_lin8;
         vl_lien_lin[8] = (float) va_lien_lin9;
         vl_lien_lin[9] = (float) va_lien_lin10;

         vl_lien[0] = (float) va_lien1; vl_lien[9] = (float) va_lien10;
         vl_lien[1] = (float) va_lien2; vl_lien[10] = (float) va_lien11;
         vl_lien[2] = (float) va_lien3; vl_lien[11] = (float) va_lien12;
         vl_lien[3] = (float) va_lien4; vl_lien[12] = (float) va_lien13;
         vl_lien[4] = (float) va_lien5; vl_lien[13] = (float) va_lien14;
         vl_lien[5] = (float) va_lien6; vl_lien[14] = (float) va_lien15;
         vl_lien[6] = (float) va_lien7; vl_lien[15] = (float) va_lien16;
         vl_lien[7] = (float) va_lien8; vl_lien[16] = (float) va_lien17;
         vl_lien[8] = (float) va_lien9; vl_lien[17] = (float) va_lien18;
         
    //Preparation du message FLASH
    sprintf( vl_msg , "|9|%d|%d|%d|%d|%d|%d|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|%g|" ,
                    va_no_fmc.numero , 
                    va_no_fmc.cle , 
                    va_debutfin , 
                    va_idportion, 
                    (int)va_numordre, 
                    vl_coef,
                    vl_anc_voie[0], vl_anc_voie[1], vl_anc_voie[2], vl_anc_voie[3], vl_anc_voie[4],
                    vl_anc_voie[5], vl_anc_voie[6], vl_anc_voie[7], vl_anc_voie[8], vl_anc_voie[9],
                    vl_voie[0], vl_voie[1], vl_voie[2], vl_voie[3], vl_voie[4],
                    vl_voie[5], vl_voie[6], vl_voie[7], vl_voie[8], vl_voie[9],
                    vl_lien_lin[0],  vl_lien_lin[1],  vl_lien_lin[2],  vl_lien_lin[3],  vl_lien_lin[4],
                    vl_lien_lin[5],  vl_lien_lin[6],  vl_lien_lin[7],  vl_lien_lin[8],  vl_lien_lin[9],
                    vl_lien[0], vl_lien[1], vl_lien[2], vl_lien[3], vl_lien[4], vl_lien[5],
                    vl_lien[6], vl_lien[7], vl_lien[8], vl_lien[9], vl_lien[10], vl_lien[11],
                    vl_lien[12], vl_lien[13], vl_lien[14], vl_lien[15], vl_lien[16], vl_lien[17]
                    );
   sprintf(vl_msg3,"%s%d|%s",vl_msg,va_typeraf,vl_nom_mobile);
   /*char vl_cmd[500]="";
   sprintf(vl_cmd,"echo \"Add msg type9:%s%d|%s\" >>/tmp/type9",vl_msg3);
   XZST_03EcritureTrace(XZSTC_DEBUG1, "cmd type 9 :%s",vl_cmd);
   system(vl_cmd);*/
      if (canWeSend(vl_msg3)) {
    if (va_typeraf==XDC_MODIF) {
      /*envoyer suppression + ajout*/
      sprintf(vl_msg2,"%s%d|%s",vl_msg,XDC_SUPPR,vl_nom_mobile);
      send_message(vl_msg2, CM_BUFF_SOCK,"");
      sprintf(vl_msg2,"%s%d|%s",vl_msg,XDC_AJOUT,vl_nom_mobile);
      send_message(vl_msg2, CM_BUFF_SOCK,"");
    }
    else {
      sprintf(vl_msg2,"%s%d|%s",vl_msg,va_typeraf,vl_nom_mobile);
    if (send_message(vl_msg2, CM_BUFF_SOCK,"") == -1)
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer la zone mobile %s" , vl_msg2 );
      return XDC_NOK;
      }      
     }
   }
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : MSRA_raf_Aff_Voies" );
   return XDC_OK;
}


int MSRA18_raf_zones_mobiles(
          va_type_mobile,
          va_typeraf,
          va_no_fmc,
          va_no_fmc_basc,
          va_idportion,
          va_pr,
          va_sens,
          va_voie,
          va_coeftrans,
          va_coefext,
          va_niveau,
          va_idradt)

          XDY_Octet va_type_mobile;
          XDY_Octet va_typeraf;
          XDY_Evt va_no_fmc;
          XDY_Evt va_no_fmc_basc;
          XDY_Mot va_idportion;
          XDY_Entier va_pr;
          XDY_Sens va_sens;
          XDY_Voie va_voie;
          float va_coeftrans;
          float va_coefext;
          float va_niveau;
          XDY_Entier va_idradt;

/*
* ARGUMENTS EN ENTREE :
*   va_typeraf : XDC_AJOUT, XDC_MODIF ,XDC_SUPPR : 
*                type de rafraichissement
*   va_no_fmc : No fiche fmc
*   va_no_fmc_basc : No fiche fmc du basculement
*   va_idportion : Identifiant de la portion 
*   va_pr : pr de la zone
*   va_sens : sens du mobile
*   va_voie : voie du mobile
*   va_coeftrans : Coef de translation [0,1]
*   va_coefext : Coef d'extension [0,1]
*   va_niveau : niveau de bouchon [0,1]
*   va_idradt : identifiant de la station radt associee( no station + sens )
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*  Creation d'une zone et de ses VAG
*  Modification des VAG controlant la zone
*  Suppression de la Zone et de ses VAG
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sra.c	1.54 07/11/07 : MSRA18_raf_zones_mobiles";
   char vl_nom_bouch[256],
        vl_nom_trans[256],
        vl_nom_exten[256],
        vl_nom_niv[256],
        vl_nom_var[256],
        vl_nom_portion[256],
        vl_nom_jonction[256],
        vl_nom_objet_jonction[256],
        vl_nom_ligne[256],
        vl_cle_mob[256],
        vl_cle[256];
        
   float      vl_niveau_jonction;
   int vl_result;
   
   char vl_idsegment[256];
   XDY_Sens vl_sens;
   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, 
   "CHI IN  : MSRA18_raf_zones_mobiles  va_type_mobile%d, va_typeraf:%d; va_no_fmc.numero:%d; va_idportion:%d;va_sens:%d; va_coeftrans:%g; va_coefext:%g; va_niv:%g; ",
   va_type_mobile, va_typeraf , va_no_fmc.numero , va_idportion , va_sens , va_coeftrans , va_coefext , va_niveau );
   
   /*A Mise a jour des different nom de vag et de l'objet */ 
   sprintf( vl_idsegment , "%06d%010d%d", va_idportion, va_pr ,va_sens);
   
  vl_result = 1;
  /* la cle est composee du no de segment pour les basculement les radt invers et les voies neutralisees */
   switch (va_type_mobile)
      {
      case CM_BOUCHON :
         sprintf( vl_cle , "!%d!%d!%d!%d!" , va_no_fmc.numero , va_no_fmc.cle , va_idportion , va_sens );
       	 sprintf( vl_nom_bouch , "Mz%03d%s" , va_type_mobile , vl_cle );
         sprintf( vl_msg , "|2|%d|%d|%d|%d|%d|%g|%g|%g|%s" , va_no_fmc.numero , va_no_fmc.cle , va_idportion , va_typeraf, va_sens, va_coeftrans, va_coefext, va_niveau, vl_nom_bouch );
         break;
      case CM_BASCBOU :
         //sprintf( vl_cle , "|%d|%d|%d|%d|%s|%d|%d|" , va_no_fmc.numero , va_no_fmc.cle , va_no_fmc_basc.numero , va_no_fmc_basc.cle , vl_idsegment , va_sens , va_voie);         
	vl_result = 0;
	break;
      
      case CM_BOUCHONVOIE : 
          sprintf( vl_cle , "!%d!%d!%s!%d!%d!" , va_no_fmc.numero , va_no_fmc.cle , vl_idsegment , va_sens , va_voie);
          sprintf( vl_nom_bouch , "Mz%03d%s" , CM_BOUCHON , vl_cle );
          sprintf( vl_msg , "|3|%d|%d|%d|%d|%d|%d|%d|%g|%g|%g|%s" , va_no_fmc.numero , va_no_fmc.cle , va_idportion , va_typeraf, va_pr, va_sens, va_voie, va_coeftrans, va_coefext, va_niveau, vl_nom_bouch );
	 break;
      
    case CM_NEUTRA : 
        sprintf( vl_cle , "!%d!%d!%s!%d!%dv" , va_no_fmc.numero , va_no_fmc.cle , vl_idsegment , va_sens , va_voie);
    	sprintf( vl_nom_bouch , "Mz%03d%s" , va_type_mobile , vl_cle );
        sprintf( vl_msg , "|4|%d|%d|%d|%d|%d|%d|%d|%g|%g|%s" , va_no_fmc.numero , va_no_fmc.cle , va_idportion , va_pr, va_sens, va_voie, va_typeraf, va_coeftrans, va_coefext, vl_nom_bouch);
	 break;
      
    case CM_INVERSE :
    	if (va_sens == XDC_SENS_NORD)
    		vl_sens = XDC_SENS_SUD;
    	else
    		vl_sens = XDC_SENS_NORD;
		sprintf( vl_cle , "!%d!%d!%s!%d!%d!" , va_no_fmc.numero , va_no_fmc.cle , vl_idsegment , va_sens , va_voie);
		sprintf( vl_nom_bouch , "Mz%03d%s" , va_type_mobile , vl_cle );
		sprintf( vl_msg , "|5|%d|%d|%d|%d|%d|%d|%d|%g|%g|%d|%d|%s" , va_no_fmc.numero , va_no_fmc.cle , va_idportion , va_pr, va_sens, va_voie, va_typeraf, va_coeftrans, va_coefext, va_idradt, vl_sens, vl_nom_bouch);
	 break;

     default :
         //sprintf( vl_cle , "|%d|%d|%s|%d|%d|" , va_no_fmc.numero , va_no_fmc.cle , vl_idsegment , va_sens , va_voie);
	XZST_03EcritureTrace(   XZSTC_WARNING , "Type de zone mobile inconnu");
      }

   if (vl_result == 1) {
      /* Pour mise a jour en cache en cas de demande de rafraichissement Magistra */
      canWeSend(vl_msg);
      if (send_message(vl_msg, CM_BUFF_SOCK,"") == -1)
         {
         XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer la zone mobile %s" , vl_msg );
         return XDC_NOK;
         }
  } 
   
   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : MSRA18_raf_zones_mobiles" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Rafraichissement des vag d'un mobile et de l'objet 
*  associe
------------------------------------------------------
* SEQUENCE D'APPEL :
*/ 

int MSRA18_raf_objets_mobiles(
          XDY_Octet va_type_mobile,
          XDY_Octet va_typeraf,
          XDY_Evt va_no_fmc,
          XDY_Evt va_no_act,
          int va_idportion,
          XDY_Sens va_sens,
          XDY_Voie va_voie,
          int va_coeftrans)
          

/*
* ARGUMENTS EN ENTREE :
*   va_typeraf : XDC_AJOUT, XDC_MODIF ,XDC_SUPPR : 
*                type de rafraichissement
*   va_no_fmc :  No de fmc
*   va_no_act :  No d'action
*   va_idportion : Identifiant de la portion , valide pour 
*                          va_typeraf=XDC_AJOUT.
*   va_sens : sens du mobile, 
*   va_voie : voie du mobile
*   va_coeftrans : Coef de translation [0,1]
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   Creation de l'objet et de sa vag de translation
*   Modification de la vag de translation
*   Suppression de l'objet et de sa vag de translation
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra.c	1.54 07/11/07 : MSRA18_raf_objets_mobiles";
   char vl_nom_bouch[256],
        vl_nom_trans[256],
        vl_nom_exten[256],
        vl_nom_ligne[256],
        vl_nom_var[256],
        vl_cle_mob[256],
        vl_cle[256];
        
   int vl_result;
   
   
   XZST_03EcritureTrace(   XZSTC_DEBUG1,
   "IN  : MSRA18_raf_objets_mobiles  va_typeraf:%d; va_no_fmc.numero:%d; va_idportion:%d;va_sens:%d; va_coeftrans:%d;  ", 
    va_typeraf , va_no_fmc.numero , va_idportion , va_sens , va_coeftrans  );
   
    
   /*A Si le sens est inconnu alors on force a sens nord et voie tpc */
   if ( va_sens == XDC_SENS_INCONNU )
      {
      va_sens = (XDY_Sens) XDC_SENS_NORD;
      va_voie = (XDY_Voie) CM_LIGNE_TPC;
      } 
          
   sprintf( vl_cle , "!%d!%d!%d!%d!%d!%d!" , va_no_fmc.numero , va_no_fmc.cle , va_no_act.numero , va_no_act.cle ,(int) va_sens, (int) va_voie);
   sprintf( vl_nom_bouch , "Mo%03d%s" , va_type_mobile , vl_cle );
   sprintf( vl_nom_trans , "Mo%03d%s_translation" , va_type_mobile , vl_cle );
   
   /*A Creation de la cle pour la table des mobile */
   sprintf( vl_cle_mob , "%s_%03d_%06d_%c_%do", vl_cle, va_type_mobile , va_idportion , va_sens + (int) '0', 4 );
   if ( strlen( vl_cle_mob ) > CM_LONG_CLEF )
      {
      vl_cle_mob[CM_LONG_CLEF]='\000';
      XZST_03EcritureTrace(   XZSTC_WARNING, " Clef zone mobile trop longue: tronquee : %s" , vl_cle_mob );
      }
      
   //strcpy( vl_mob.clef , vl_cle_mob );
      
   sprintf( vl_msg , "|7|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%s" , va_no_fmc.numero , va_no_fmc.cle , va_no_act.numero , va_no_act.cle , va_idportion, va_typeraf, va_type_mobile, (int) va_sens, (int) va_voie, va_coeftrans, vl_nom_bouch);
   if (canWeSend(vl_msg)) {
   if (send_message(vl_msg, CM_BUFF_SOCK,"") == -1)
      {
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer l'objet mobile");
      return XDC_NOK;
      }
  } 
      
   /*A Determination du type de rafraichissement : Creation , modification , suppression */
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : MSRA18_raf_objets_mobiles" );
   return XDC_OK;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Renvoie le nom de la ligne servant d'axe de translation au mobile
*  de type zone
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSRA_Ligne_porteuse_zone(
          int va_type_mobile,
          char va_type_vue,
          int va_idportion,
          XDY_Sens va_sens,
          XDY_Voie va_voie,
          char *pa_nom_ligne)
          

/*
* ARGUMENTS EN ENTREE :
*   va_type_mobile : Type du mobile : bouchon, voie neutralisee ...
*   va_type_vue : Type de vue : Generale, District, Troncon
*   va_idportion : Identifiant de la portion ou du segment
*   va_sens : sens pour la cas d'une portion
*   va_voie : voie affectee
*   pa_nom_ligne : Pointeur sur la chaine de caractere contenant le nom dela ligne
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   Construction du nom de la ligne de translation avec
* l'identifiant de segment ou de portion
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra.c	1.54 07/11/07 :MSRA_Ligne_porteuse_zone ";   
   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA_Ligne_porteuse_zone" );
   
   /*A Choix de la ligne suivant le type de zone mobile */
   switch( va_type_mobile )
      {
      /*A Pour un Bouchon, on prend la ligne centrale du segment ou de la voie de la portion*/
      case CM_BOUCHONVOIE :
      case CM_BOUCHON :
         if ( va_type_vue == 'T' )
            {
            sprintf( pa_nom_ligne , "PORTEUSE_%06d_%d%d" , va_idportion , va_sens , va_voie );
            }
         else
            {
            if ( va_sens == XDC_SENS_NORD )
               sprintf( pa_nom_ligne , "Pportion_%06d%d" , va_idportion , CM_LIGNE_CHAUSSEE_NORD );/*! sens de la porteuse portion en dur!*/
            else
               sprintf( pa_nom_ligne , "Pportion_%06d%d" , va_idportion , CM_LIGNE_CHAUSSEE_SUD );/*! sens de la porteuse portion en dur!*/
            }   
            
         break;
            
      /*A Pour un Bouchon bascule, une radt inversee ou une voie neutralise , on prend la ligne de la voie du segment */
      case CM_INVERSE :
      case CM_BASCBOU :
      case CM_NEUTRA :
         if ( va_type_vue == 'T' )
            {
            sprintf( pa_nom_ligne , "PORTEUSE_%06d_%d%d" , va_idportion , va_sens , va_voie);
            }
         else
            {
            strcpy( pa_nom_ligne , "");
            XZST_03EcritureTrace(   XZSTC_WARNING, " On ne gere pas ce type de zone(%d) de voie en vision Portion !." , va_type_mobile );
            return XDC_NOK;
            }
            break;
            
      default :
         break;
      }
   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : MSRA_Ligne_porteuse_zone" );
   return XDC_OK;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Renvoie le nom de la ligne servant d'axe de translation au mobile
*  ponctuel
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSRA_Ligne_porteuse_mobile(
          va_type_mobile,
          va_type_vue,
          va_idportion,
          va_sens,
          va_voie,
          pa_nom_ligne)
          
          int va_type_mobile;
          char va_type_vue;
          int va_idportion;
          XDY_Sens va_sens;
          XDY_Voie va_voie;
          char *pa_nom_ligne;

/*
* ARGUMENTS EN ENTREE :
*   va_type_mobile : Type du mobile : travaux, intervenants ...
*   va_type_vue : Type de vue : Generale, District Troncon
*   va_idportion : Identifiant de la portion ou du segment
*   va_sens : sens pour la cas d'une portion
*   va_voie : voie affectee
*   pa_nom_ligne : Pointeur sur la chaine de caractere contenant le nom dela ligne
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*   Construction du nom de la ligne de translation avec
* l'identifiant de segment ou de portion
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra.c	1.54 07/11/07 :MSRA_Ligne_porteuse_mobile ";   
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Ligne_porteuse_mobile" );
   
   /*A Choix de la ligne suivant le type de mobile */
   switch( va_type_mobile )
      {
            
      /*A Pour des travaux , on prend la ligne BAU du segment ou de la voie de la portion*/
      case XDC_SYN_TRAVAUX_MOBILES_PREVUS :
      case XDC_SYN_TRAVAUX_MOBILES_REELS :
      case XDC_SYN_TRAVAUX_FIXES_PREVUS :
      case XDC_SYN_TRAVAUX_FIXES_REELS :
      case XDC_SYN_TRAVAUX_MOBILES_TETE_PREVUS :
      case XDC_SYN_TRAVAUX_MOBILES_TETE_REELS :
      case XDC_SYN_TRAVAUX_MOBILES_QUEUE_PREVUS :
      case XDC_SYN_TRAVAUX_MOBILES_QUEUE_REELS :
         if ( va_type_vue == 'T' )
            {
            /* ASi la voie est incoonu alors on place le mobile sur la bau */
            if ( va_voie == XZAHC_VoieInconnue ) 
               va_voie = CM_LIGNE_BAU;
            sprintf( pa_nom_ligne , "PORTEUSE_%06d_%d%d" , va_idportion , va_sens , va_voie );
            }
         else
            {
            if ( va_sens == XDC_SENS_NORD )
               sprintf( pa_nom_ligne , "Pportion_%06d%d" , va_idportion , CM_LIGNE_BAU_NORD);/*! sens de la porteuse portion en dur!*/
            else
               sprintf( pa_nom_ligne , "Pportion_%06d%d" , va_idportion , CM_LIGNE_BAU_SUD);/*! sens de la porteuse portion en dur!*/
            }   
            break;
            
      /*A Pour des pmv_mobile, gendarmes ou pompier , on prend la ligne de la voie de droite du segment ou de la voie de la portion*/
      case XDC_SYN_GARE :
      case XDC_SYN_PMV_MOBILE :
      case XDC_SYN_INTERVENANT_GENDARMERIE :
      case XDC_SYN_INTERVENANT_POMPIER :
      case XDC_SYN_INTERVENANT_AUTRE :
         if ( va_type_vue == 'T' )
            {
            /*A Si la voie est incoonu alors on place le mobile sur la ligne lente */
            if ( va_voie == XZAHC_VoieInconnue ) 
               va_voie = CM_LIGNE_LENTE;
            sprintf( pa_nom_ligne , "PORTEUSE_%06d_%d%d" , va_idportion , va_sens, va_voie );
            }
         else
            {
            if ( va_sens == XDC_SENS_NORD )
               sprintf( pa_nom_ligne , "Pportion_%06d%d" , va_idportion  , CM_LIGNE_BAU_NORD);/*! sens de la porteuse portion en dur!*/
            else
               sprintf( pa_nom_ligne , "Pportion_%06d%d" , va_idportion  , CM_LIGNE_BAU_SUD);/*! sens de la porteuse portion en dur!*/
            }   
            break;
       
      /*A Pour des depanneurs, fourgons sis ou pat escota , on prend la ligne BAU du segment ou de la voie de la portion*/
      case XDC_SYN_INTERVENANT_DEPANNEUR :
      case XDC_SYN_INTERVENANT_FOURGON_SIS :
      case XDC_SYN_INTERVENANT_PAT_ESCOTA :
         if ( va_type_vue == 'T' )
            {
            /*A Si la voie est incoonu alors on place le mobile sur le BASCOTE */
            if ( va_voie == XZAHC_VoieInconnue ) 
               va_voie = CM_LIGNE_BASCOTE;
            sprintf( pa_nom_ligne , "PORTEUSE_%06d_%d%d" , va_idportion , va_sens, va_voie );
            }
         else
            {
            if ( va_sens == XDC_SENS_NORD )
               sprintf( pa_nom_ligne , "Pportion_%06d%d" , va_idportion , CM_LIGNE_BAU_NORD);/*! sens de la porteuse portion en dur!*/
            else
               sprintf( pa_nom_ligne , "Pportion_%06d%d" , va_idportion , CM_LIGNE_BAU_SUD);/*! sens de la porteuse portion en dur!*/
            }   
            break;
            
      /*A Pour des evenement , on prend la ligne de la 2eme voie du segment ou de la voie de la portion*/
      case XDC_SYN_FMC_REGULATION_VITESSE:
      case XDC_SYN_FMC_ACCIDENT :
      case XDC_SYN_FMC_VEHICULE_CONTRE_SENS :
      case XDC_SYN_FMC_VEHICULE_ARRETE :
      case XDC_SYN_FMC_VEHICULE_PANNE :
      case XDC_SYN_FMC_VEHICULE_FEU :
      case XDC_SYN_FMC_CIRQUE :
      case XDC_SYN_FMC_CONVOI_EXCEPTIONNEL :
      case XDC_SYN_FMC_CONVOI_MILITAIRE :
      case XDC_SYN_FMC_VEHICULE_LENT :
      case XDC_SYN_FMC_MANIFESTATION :
      case XDC_SYN_FMC_GREVE_ESCOTA :
      case XDC_SYN_FMC_OPERATION_ESCARGOT :
      case XDC_SYN_FMC_INCIDENT_SOUS_CONSC :
      case XDC_SYN_FMC_EBOULEMENT :
      case XDC_SYN_FMC_INCENDIE :
      case XDC_SYN_FMC_INONDATION :
      case XDC_SYN_FMC_OBSTACLE :
      case XDC_SYN_FMC_CHAUSSEE_DETERIOREE :
      case XDC_SYN_FMC_FLAQUE_HUILE :
      case XDC_SYN_FMC_ANIMAL_ERRANT :
      case XDC_SYN_FMC_FRONTIERE_FERMEE :
      case XDC_SYN_FMC_ALTERCATION_USAGERS :
      case XDC_SYN_FMC_AGRESSION :
      case XDC_SYN_FMC_MALAISE_USAGER :
      case XDC_SYN_FMC_HOLD_UP :
      case XDC_SYN_FMC_PIETON_CHAUSSEE :
      case XDC_SYN_FMC_AUTRE :
      case XDC_SYN_FMC_VEHICULE_MARCHE_ARR :
      case XDC_SYN_FMC_TRAITEMENT_CHAUSSEES :
      case XDC_SYN_FMC_ECHANGEUR_DECONSEILLE :
      case XDC_SYN_FMC_CHAUSSEE_GLISSANTE :
      case XDC_SYN_FMC_ACCIDENT_VL 	:
      case XDC_SYN_FMC_ACCIDENT_PL 	:
      case XDC_SYN_FMC_ACCIDENT_VLPL 	:
      case XDC_SYN_FMC_ACCIDENT_CAR 	:
      case XDC_SYN_FMC_ACCIDENT_VLCAR 	:
      case XDC_SYN_FMC_ACCIDENT_PLCAR 	:
      case XDC_SYN_FMC_ACCIDENT_VLPLCAR 	:
      case XDC_SYN_FMC_ACCIDENT_REM 	:
      case XDC_SYN_FMC_ACCIDENT_VLREM 	:
      case XDC_SYN_FMC_ACCIDENT_PLREM 	:
      case XDC_SYN_FMC_ACCIDENT_VLPLREM 	:
      case XDC_SYN_FMC_ACCIDENT_CARREM 	:
      case XDC_SYN_FMC_ACCIDENT_VLCARREM 	:
      case XDC_SYN_FMC_ACCIDENT_PLCARREM 	:
      case XDC_SYN_FMC_ACCIDENT_VLPLCARREM :
      case XDC_SYN_FMC_ACCIDENT_MOTO 	:
      case XDC_SYN_FMC_ACCIDENT_VLMOTO 	:
      case XDC_SYN_FMC_ACCIDENT_PLMOTO 	:
      case XDC_SYN_FMC_ACCIDENT_VLPLMOTO 	:
      case XDC_SYN_FMC_ACCIDENT_CARMOTO 	:
      case XDC_SYN_FMC_ACCIDENT_VLCARMOTO 	:
      case XDC_SYN_FMC_ACCIDENT_PLCARMOTO 	:
      case XDC_SYN_FMC_ACCIDENT_VLPLCARMOTO 	:
      case XDC_SYN_FMC_ACCIDENT_REMMOTO 	:
      case XDC_SYN_FMC_ACCIDENT_VLREMMOTO 	:
      case XDC_SYN_FMC_ACCIDENT_PLREMMOTO 	:
      case XDC_SYN_FMC_ACCIDENT_VLPLREMMOTO 	:
      case XDC_SYN_FMC_ACCIDENT_CARREMMOTO 	:
      case XDC_SYN_FMC_ACCIDENT_VLCARREMMOTO 	:
      case XDC_SYN_FMC_ACCIDENT_PLCARREMMOTO 	:
      case XDC_SYN_FMC_ACCIDENT_VLPLCARREMMOTO 	:
         if ( va_type_vue == 'T' )
            {
            /*A Si la voie est incoonu alors on place le mobile sur la MEDIANE1 */
            if ( va_voie == XZAHC_VoieInconnue ) 
               va_voie = CM_LIGNE_MEDIANE1;
            sprintf( pa_nom_ligne , "PORTEUSE_%06d_%d%d" , va_idportion , va_sens, va_voie);
            }
         else
            {
            if ( va_sens == XDC_SENS_NORD )
               sprintf( pa_nom_ligne , "Pportion_%06d%d" , va_idportion , CM_LIGNE_BAU_NORD);/*! sens de la porteuse portion en dur!*/
            else
               sprintf( pa_nom_ligne , "Pportion_%06d%d" , va_idportion , CM_LIGNE_BAU_SUD);/*! sens de la porteuse portion en dur!*/
            }   
            break;
      default :
         if ( va_type_vue == 'T' )
            {
            /*A Si la voie est incoonu alors on place le mobile sur la MEDIANE1 */
            if ( va_voie == XZAHC_VoieInconnue ) 
               va_voie = CM_LIGNE_MEDIANE1;
            sprintf( pa_nom_ligne , "PORTEUSE_%06d_%d%d" , va_idportion , va_sens, va_voie);
            }
         else
            {
            if ( va_sens == XDC_SENS_NORD )
               sprintf( pa_nom_ligne , "Pportion_%06d%d" , va_idportion , CM_LIGNE_BAU_NORD);/*! sens de la porteuse portion en dur!*/
            else
               sprintf( pa_nom_ligne , "Pportion_%06d%d" , va_idportion , CM_LIGNE_BAU_SUD);/*! sens de la porteuse portion en dur!*/
            }   
         break;
      }
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Ligne_porteuse_mobile" );
   return XDC_OK;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSRA_LectureRADRAnim()

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra.c	1.54 07/11/07 : MSRA_LectureRADRAnim";
   char vl_NomFich[255];
   char vl_ligne[257];
   int  vl_nb_ligne=0,
        vl_taille_max;
   char			*vl_Texte;
   char			vl_Texte_out[255];
   long 		vl_num_ligne = 0;
   int  		vl_retcode = 0;
   FILE			*pl_fic;
   XDY_Booleen		vl_nb_radt_lu=XDC_NON;
   
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_LectureRADRAnim" );
   sprintf( vl_NomFich, "/produits/migrazur/appliSD/fichiers/syn/isbi_RADT_%s.txt", vm_NomSite );

   /*Test d'existance du fichier contenant la liste des stations */
   if ( ( pl_fic = fopen( vl_NomFich , "r")) == NULL ) 
      {
      XZST_03EcritureTrace(   XZSTC_WARNING, "Fichier contenant les stations introuvable : %s", vl_NomFich );
      return XDC_NOK;
      }
      
   vl_taille_max=100;
   vl_nb_radt_lu=XDC_NON;
      
   pm_radtanim = (tm_radtanim *) malloc ( vl_taille_max * sizeof(tm_radtanim) );
   vl_nb_ligne=0;
   
   while ( fgets( vl_ligne, 256 ,pl_fic)  != NULL ) 
      {
      vl_ligne[strlen(vl_ligne)-1]='\0';
      if ( vl_nb_ligne >= vl_taille_max )
         {
         vl_taille_max=vl_taille_max+100;
         pm_radtanim = (tm_radtanim *) realloc ( pm_radtanim, vl_taille_max * sizeof(tm_radtanim) );
         }
      
      /*A
       ** Verification que la ligne n'est pas un commentaire
       ** Presence du caractere #
       */
          
      vl_retcode = strcspn ( vl_ligne, "#" );

      if (vl_retcode != XDC_OK) 

          {
   	     if ( vl_nb_radt_lu == XDC_NON )
   	        {
   	        vl_nb_radt_lu=XDC_OUI;
   	        vg_nb_abtradt=atoi(vl_ligne);
		}
	     else
		{
                /*A
                ** Extraction des champs no station et SurDistrict
                */
    
                vl_Texte = strtok ( vl_ligne, "\t" );
                pm_radtanim[vl_nb_ligne].Numero=(XDY_Eqt) atoi(vl_Texte);
             
                vl_Texte = strtok ( NULL, "\t" );
                pm_radtanim[vl_nb_ligne].Sens=(XDY_Sens) atoi(vl_Texte);

                vl_Texte = strtok ( NULL, "\t" );
                pm_radtanim[vl_nb_ligne].SurDistrict=(XDY_Booleen) atoi(vl_Texte);
	        XZST_03EcritureTrace(   XZSTC_WARNING, "no ligne:%d; %s ; numero=%s sens=%d dist=%d",vl_nb_ligne, vl_ligne, (int) pm_radtanim[vl_nb_ligne].Numero, (int) pm_radtanim[vl_nb_ligne].Sens, (int) pm_radtanim[vl_nb_ligne].SurDistrict );
                vl_nb_ligne++;
		}
          }
      }
      
   if ( ! feof( pl_fic ) )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING, " Probleme lors de la lecture du fichiers contenant la liste des stations.\n");
      fclose( pl_fic );
      return XDC_NOK;
      }
   
   /*A fermeture du fichier de la liste des stations */
   fclose( pl_fic );
   
   vm_nb_radtanim=vl_nb_ligne;
   
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_LectureRADRAnim" );
   return XDC_OK;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  renvoie XDC_Ok,si la station est voisine ou du district
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSRA_RADT_Animee(
          XDY_Eqt va_noEqt,
          XDY_Sens va_Sens)

/*
* ARGUMENTS EN ENTREE :
*   va_noEqt : Numero de station 
*
*
* ARGUMENTS EN SORTIE :
*   aucune
*
*
* CODE RETOUR : 
*   XDC_OK station voisine ou du district
*   XDC_NOK station hors district
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra.c	1.54 07/11/07 : MSRA_RADT_Animee";
   int 			vl_retour=0;
   int  		i = 0;
   
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_RADT_Animee" );
   
   i=0;
   while ( (i < vm_nb_radtanim-1) && ((pm_radtanim[i].Numero != va_noEqt ) || (pm_radtanim[i].Sens != va_Sens )))  i++;
       
   if ( ( pm_radtanim[i].Numero == va_noEqt ) && ( pm_radtanim[i].Sens == va_Sens ))
      {
      vl_retour=XDC_OK;
      }
   else
      {
      vl_retour=XDC_NOK;
      }
   
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_RADT_Animee" );
   return vl_retour;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  renvoie XDC_Ok,si la station est voisine ou du district
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSRA_RADT_AnimeeSurDistrict(
          XDY_Eqt va_noEqt,
          XDY_Sens va_Sens)

/*
* ARGUMENTS EN ENTREE :
*   va_noEqt : Numero de station 
*
*
* ARGUMENTS EN SORTIE :
*   aucune
*
*
* CODE RETOUR : 
*   XDC_OK station sur district
*   XDC_NOK station hors district
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra.c	1.54 07/11/07 : MSRA_RADT_AnimeeSurDistrict";
   int 			vl_retour=0;
   int  		i = 0;
   
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_RADT_AnimeeSurDistrict" );
   
   i=0;
   while ( (i < vm_nb_radtanim-1) && ((pm_radtanim[i].Numero != va_noEqt ) || (pm_radtanim[i].Sens != va_Sens )))  i++;
       
   if ( ( pm_radtanim[i].Numero == va_noEqt ) && ( pm_radtanim[i].Sens == va_Sens ))
      {
      if ( pm_radtanim[i].SurDistrict == XDC_OUI )
         vl_retour=XDC_OK;
      else
         vl_retour=XDC_NOK;
      }
   else
      {
      vl_retour=XDC_NOK;
      }
   
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_RADT_AnimeeSurDistrict" );
   return vl_retour;
}

/**********************modele*********************/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSRA_(
          )

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra.c	1.54 07/11/07 : ";
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : " );
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : " );
   return XDC_OK;
}



/* Fichier : @(#)gmot_adm_dv.c  1.15                    Release : 1.15              Date : 10/09/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction s'execute a la reception d'un message
* XDM_ADM_SSS (rafraichissement d'un synoptique):
* - decodage du message
* - construction du nom de variable de synoptique
* - valorisation de cette variable avec la valeur
*   recue (cette variable se situe dans la table
*   de symboles globale vm_DataSymTable )
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
MajAdmSsSys( T_IPC_CONN                 va_Cnx,
             T_IPC_CONN_PROCESS_CB_DATA pa_Data,
             T_CB_ARG                   pa_Arg )


{
#ifndef _TIBCO_EMS
  T_STR         vl_NomMac ;  /* variables de lecture de message */
  T_STR         vl_NomSSys ;
  T_INT4        vl_EtatSSys;
#else
  XDY_NomMachine vl_NomMac ;  /* variables de lecture de message */
  XDY_NomMachine vl_NomSSys ;
  XDY_Entier     vl_EtatSSys;
#endif
  XZSCT_NomSite vl_NomSite;
  int           vl_cr;
  char          vl_key[50];  /* variables DV-Draw */

  /* decodage du message */
       if (!TipcMsgRead(pa_Data->msg
                ,T_IPC_FT_STR,&vl_NomMac
                ,T_IPC_FT_STR,&vl_NomSSys
                ,T_IPC_FT_INT4,&vl_EtatSSys
                ,NULL))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
                "MajAdmSsSys : Pb lecture message XDM_ADM_SSS" );
    return;
  }
  /* site auquel appartient la machine vl_NomMac ?? */
  vl_cr = XZSA_23SiteMachine( vl_NomMac, vl_NomSite );
  if (vl_cr == XDC_NOK) return ;
  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "SSS_%s_%s_%s_etat", vl_NomSite, vl_NomMac, vl_NomSSys );
  sprintf( vl_key, "%s_%s_etat", vl_NomMac, vl_NomSSys );

  if (hash_data(vl_key, vl_EtatSSys))
  {
	/*valeurs possibles : XZSIC_OK XZSIC_NOK XZSIC_SANS_ETAT*/
	/*transfert au flash*/
	sprintf(vl_msg,"|50|%s_%s|etat|%d",
		vl_NomMac, vl_NomSSys, vl_EtatSSys);
	send_message(vl_msg, CM_BUFF_SOCK,"");
  }
}


/*X*/
/* Fichier : @(#)gmot_adm_dv.c  1.15                    Release : 1.15              Date : 10/09/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction s'execute a la reception d'un message
* XDM_ADM_TAC (rafraichissement d'un synoptique):
* - decodage du message
* - construction du nom de variable de synoptique
* - valorisation de cette variable avec la valeur
*   recue (cette variable se situe dans la table
*   de symboles globale vm_DataSymTable )
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
MajAdmTache( T_IPC_CONN                 va_Cnx,
             T_IPC_CONN_PROCESS_CB_DATA pa_Data,
            T_CB_ARG                   pa_Arg )
{
#ifndef _TIBCO_EMS
  T_STR         vl_NomMac ;  /* variables de lecture de message */
  T_STR         vl_NomTache ;
  T_INT4        vl_EtatTache;
#else
  XDY_NomMachine         vl_NomMac ;  /* variables de lecture de message */
  XDY_NomMachine         vl_NomTache ;
  XDY_Entier        vl_EtatTache;
#endif
  int 		pl_valeur;
  char          vl_key[50];  /* variables DV-Draw */

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: MajAdmTache" );

  /* decodage du message */
       if (!TipcMsgRead(pa_Data->msg
                ,T_IPC_FT_STR,&vl_NomMac
                ,T_IPC_FT_STR,&vl_NomTache
                ,T_IPC_FT_INT4,&vl_EtatTache
                ,NULL))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
                "MajAdmTache : Pb lecture message XDM_ADM_TAC" );
    return;
  }
 /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "TAC_%s_%s_etat", vl_NomMac, vl_NomTache );
  sprintf( vl_key, "%s_%s_etat", vl_NomMac, vl_NomTache );

  if (hash_data(vl_key, vl_EtatTache))
  {
  XZST_03EcritureTrace( XZSTC_DEBUG1,
        " Maj de la tache %s sur la machine %s \n", vl_NomTache,vl_NomMac);
        switch (vl_EtatTache)
        {
        case XZSIC_OK:
                        pl_valeur = 1;
                        break;

        case XZSIC_NOK:
                        pl_valeur = 2;
                        break;

        case XZSIC_SANS_ETAT:
                        pl_valeur = 0;
                        break;
        default:
                        XZST_03EcritureTrace( XZSTC_WARNING,
                                 "MajAdmTache : Mauvais parametre d'entree " );
                        break;
        }
  sprintf(vl_msg,"|51|%s_%s|etat|%d",
		vl_NomMac, vl_NomTache, pl_valeur);
  send_message(vl_msg, CM_BUFF_SOCK,"");
  }
}


/* Fichier : @(#)gmot_adm_dv.c  1.15                    Release : 1.15              Date : 10/09/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction s'execute a la reception d'un message
* XDM_ADM_ARC_SER (rafraichissement d'un synoptique):
* - decodage du message
* - construction du nom de variable de synoptique
* - valorisation de cette variable avec la valeur
*   recue (cette variable se situe dans la table
*   de symboles globale vm_DataSymTable )
*
******************************************************/
void
MajAdmServeur( T_IPC_CONN                 va_Cnx,
               T_IPC_CONN_PROCESS_CB_DATA pa_Data,
               T_CB_ARG                   pa_Arg )
{
#ifndef _TIBCO_EMS
  T_STR         vl_NomMac ;  /* variables de lecture de message */
  T_INT4        vl_EtatMac;
#else
  XDY_NomMachine         vl_NomMac ;  /* variables de lecture de message */
  XDY_Entier        vl_EtatMac;
#endif

  char          vl_key[50];  /* variables DV-Draw */
  int           pl_valeur;
  int           vl_TypeMaj_Synt         = XZSIC_ETAT_NACTIF;

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: MajAdmServeur" );

  /* decodage du message */
       if (!TipcMsgRead(pa_Data->msg
                ,T_IPC_FT_STR,&vl_NomMac
                ,T_IPC_FT_INT4,&vl_EtatMac
                ,NULL))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
                "MajAdmServeur : Pb lecture message XDM_ADM_ARC_SER" );
    return;
  }
  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "ARC_%s_etat", vl_NomMac );
  sprintf( vl_key, "%s_etat", vl_NomMac );
	if (hash_data(vl_key, vl_EtatMac))
	{
        switch (vl_EtatMac)
        {
        case XZSIC_ETAT_VEILLE:
                pl_valeur = 4;

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Mach_Synt(vl_NomMac,vl_EtatMac);

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Res_Synt(vl_NomMac,vl_EtatMac);

                break;
        case XZSIC_ETAT_NACTIF:
                pl_valeur = 3;

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Mach_Synt(vl_NomMac,vl_EtatMac);

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Res_Synt(vl_NomMac,vl_EtatMac);

                break;

        case XZSIC_ETAT_NPASSIF:
                pl_valeur = 2;

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Mach_Synt(vl_NomMac,vl_EtatMac);

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Res_Synt(vl_NomMac,vl_EtatMac);

                break;
        case XZSIC_ETAT_DEGRADE:
                pl_valeur = 1;

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Mach_Synt(vl_NomMac,vl_EtatMac);

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Res_Synt(vl_NomMac,vl_EtatMac);

                break;
        case XZSIC_SANS_ETAT:
                pl_valeur = 0;

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Res_Synt(vl_NomMac,vl_EtatMac);

                break;

        default:
                        XZST_03EcritureTrace( XZSTC_WARNING,
                                 "MajAdmServeur : Mauvais parametre d'entree " );
                        break;
        }

  sprintf(vl_msg,"|54|%s|etat|%d",
		vl_NomMac, vl_EtatMac);
  send_message(vl_msg, CM_BUFF_SOCK,"");
  }
}


/*X*/
/* Fichier : @(#)gmot_adm_dv.c  1.15                    Release : 1.15              Date : 10/09/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction s'execute a la reception d'un message
* XDM_ADM_ARC_PER (rafraichissement d'un synoptique):
* - decodage du message
* - construction du nom de variable de synoptique
* - valorisation de cette variable avec la valeur
*   recue (cette variable se situe dans la table
*   de symboles globale vm_DataSymTable )
*
******************************************************/
void
MajAdmPeriph(T_IPC_CONN                 va_Cnx,
             T_IPC_CONN_PROCESS_CB_DATA pa_Data,
             T_CB_ARG                   pa_Arg )

{
#ifndef _TIBCO_EMS
  T_STR         vl_NomPeriph ;  /* variables de lecture de message */
  T_INT4        vl_EtatPeriph   = 0;
  T_INT4        vl_TypeEntite   = 0;
#else
  XDY_NomMachine         vl_NomPeriph ;  /* variables de lecture de message */
  XDY_Entier        vl_EtatPeriph   = 0;
  XDY_Entier        vl_TypeEntite   = 0;
#endif

  char          vl_key[50];  /* variables DV-Draw */
  int           pl_valeur;

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: MajAdmPeriph" );

  /* decodage du message */
       if (!TipcMsgRead(pa_Data->msg
                ,T_IPC_FT_STR,&vl_NomPeriph
                ,T_IPC_FT_INT4,&vl_TypeEntite
                ,T_IPC_FT_INT4,&vl_EtatPeriph
                ,NULL))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
                "MajAdmPeriph : Pb lecture message XDM_ADM_ARC_PER" );
    return;
  }
  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "ARC_%s_etat", vl_NomPeriph );
  sprintf( vl_key, "%s_etat", vl_NomPeriph );
	if (hash_data(vl_key,vl_EtatPeriph))
	{
        switch (vl_EtatPeriph)
        {
     		case XZSIC_OK:
                	pl_valeur = 2;

        	        /*A Mis a jour de la vue de Synthese */
	                MajAdm_Ent_Synt(vl_NomPeriph,vl_EtatPeriph,vl_TypeEntite);
        	        break;

 		case XZSIC_NOK:
                	pl_valeur = 1;

        	        /*A Mis a jour de la vue de Synthese */
	                MajAdm_Ent_Synt(vl_NomPeriph,vl_EtatPeriph,vl_TypeEntite);
        	         break;

	        case XZSIC_SANS_ETAT:
                        	pl_valeur = 0;
                	        break;
       		 default:
                     	   XZST_03EcritureTrace( XZSTC_WARNING,
                        	         "MajAdmPeriph : Mauvais parametre d'entree " );
                	        break;
        	}

  		if (strncmp(vl_NomPeriph, "ST", 2))
      			sprintf(vl_msg, "|55|%s|etat|%d",
               	 	vl_NomPeriph, vl_EtatPeriph);
  		else		
	      		sprintf(vl_msg,"|56|%s|etat|%d",
			vl_NomPeriph, vl_EtatPeriph);
  		send_message(vl_msg, CM_BUFF_SOCK,"");
  	}
}


/*X*/
/* Fichier : @(#)gmot_adm_dv.c  1.15                    Release : 1.15              Date : 10/09/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction s'execute a la reception d'un message
* XDM_ADM_ARC_INT (rafraichissement d'un synoptique):
* - decodage du message
* - construction du nom de variable de synoptique
* - valorisation de cette variable avec la valeur
*   recue (cette variable se situe dans la table
*   de symboles globale vm_DataSymTable )
*
******************************************************/
void
MajAdmInterf(T_IPC_CONN                 va_Cnx,
             T_IPC_CONN_PROCESS_CB_DATA pa_Data,
             T_CB_ARG                   pa_Arg )
{
#ifndef _TIBCO_EMS
  T_STR         vl_NomInterf ;  /* variables de lecture de message */
  T_INT4        vl_EtatInterf;
#else
  XDY_NomMachine         vl_NomInterf ;  /* variables de lecture de message */
  XDY_Entier        vl_EtatInterf;
#endif

  char          vl_key[50];  /* variables DV-Draw */
  int           pl_valeur;

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: MajAdmInterf" );

  /* decodage du message */
       if (!TipcMsgRead(pa_Data->msg
                ,T_IPC_FT_STR,&vl_NomInterf
                ,T_IPC_FT_INT4,&vl_EtatInterf
                ,NULL))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
                "MajAdmInterf : Pb lecture message XDM_ADM_ARC_INT" );
    return;
  }

  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "ARC_%s_etat", vl_NomInterf );
  sprintf( vl_key, "%s_etat", vl_NomInterf );
	if (hash_data(vl_key, vl_EtatInterf))
	{
        switch (vl_EtatInterf)
        {
        case XZSIC_OK:
                pl_valeur = 2;

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Ent_Synt(vl_NomInterf,vl_EtatInterf,XZSAC_TYPE_INTE);

                break;

        case XZSIC_NOK:
                pl_valeur = 1;

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Ent_Synt(vl_NomInterf,vl_EtatInterf,XZSAC_TYPE_INTE);
                break;

        case XZSIC_SANS_ETAT:
                        pl_valeur = 0;
                        break;
        default:
                        XZST_03EcritureTrace( XZSTC_WARNING,
                                 "MajAdmInterf : Mauvais parametre d'entree " );
                        break;
        }

 /*flash */
  sprintf(vl_msg,"|56|%s|etat|%d",
		vl_NomInterf, vl_EtatInterf);
  send_message(vl_msg, CM_BUFF_SOCK,"");
  }
}

/* Fichier : @(#)gmot_adm_dv.c  1.15                    Release : 1.15              Date : 10/09/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction s'execute a la reception d'un message
* XDM_ADM_TRA (rafraichissement d'un synoptique):
* - decodage du message
* - construction du nom de variable de synoptique
* - valorisation de cette variable avec la valeur
*   recue (cette variable se situe dans la table
*   de symboles globale vm_DataSymTable )
*
******************************************************/
void
MajAdmTrace( T_IPC_CONN                 va_Cnx,
             T_IPC_CONN_PROCESS_CB_DATA pa_Data,
             T_CB_ARG                   pa_Arg )
{  
#ifndef _TIBCO_EMS
  T_STR         vl_NomMac ;  /* variables de lecture de message */
  T_STR         vl_NomTache ;
  T_INT4        vl_NivTrace;
  T_INT4        vl_VisuTrace;
#else
  XDY_NomMachine         vl_NomMac ;  /* variables de lecture de message */
  XDY_NomTache         vl_NomTache ;
  XDY_Entier        vl_NivTrace;
  XDY_Entier        vl_VisuTrace;
#endif

  char          vl_key[50];  /* variables DV-Draw */
  int           pl_valeur;

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: MajAdmTrace" );
  /* decodage du message */
       if (!TipcMsgRead(pa_Data->msg
                ,T_IPC_FT_INT4,&vl_NivTrace
                ,T_IPC_FT_INT4,&vl_VisuTrace
                ,T_IPC_FT_STR,&vl_NomTache
                ,T_IPC_FT_STR,&vl_NomMac
                ,NULL))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
                "MajAdmTrace : Pb lecture message XDM_ADM_TRA" );
    return;
  }

  /*---------- VISUALISATION -----------------------*/
  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "TRAV_%s_%s_Ecr", vl_NomMac, vl_NomTache );
  sprintf( vl_key, "%s_%s_ecran", vl_NomMac, vl_NomTache );
  if (hash_data(vl_key,vl_NivTrace))
  {
  sprintf(vl_msg,"|52|%s_%s|ecran|%d",
		vl_NomMac, vl_NomTache, vl_VisuTrace == XZSTC_ECRAN);
  send_message(vl_msg, CM_BUFF_SOCK,"");

  sprintf( vl_key, "%s_%s_fonction",vl_NomMac, vl_NomTache );
  sprintf(vl_msg,"|52|%s_%s|fonction|%d",
		vl_NomMac, vl_NomTache, ( (vl_NivTrace & XZSTC_FONCTION) != 0 ));
  send_message(vl_msg, CM_BUFF_SOCK,"");
  sprintf( vl_key, "%s_%s_message",vl_NomMac, vl_NomTache );
  sprintf(vl_msg,"|52|%s_%s|message|%d",
		vl_NomMac, vl_NomTache, ( (vl_NivTrace & XZSTC_MESSAGE) != 0 ));
  send_message(vl_msg, CM_BUFF_SOCK,"");
  sprintf( vl_key, "%s_%s_info",vl_NomMac, vl_NomTache );
  sprintf(vl_msg,"|52|%s_%s|info|%d",
		vl_NomMac, vl_NomTache, ( (vl_NivTrace & XZSTC_INFO) != 0 ));
  send_message(vl_msg, CM_BUFF_SOCK,"");
  sprintf( vl_key, "%s_%s_sgbd",vl_NomMac, vl_NomTache );
  sprintf(vl_msg,"|52|%s_%s|sgbd|%d",
		vl_NomMac, vl_NomTache, ( (vl_NivTrace & XZSTC_SGBD) != 0 ));
  send_message(vl_msg, CM_BUFF_SOCK,"");
  sprintf( vl_key, "%s_%s_debug1",vl_NomMac, vl_NomTache );
  sprintf(vl_msg,"|52|%s_%s|debug1|%d",
		vl_NomMac, vl_NomTache, ( (vl_NivTrace & XZSTC_DEBUG1) != 0 ));
  send_message(vl_msg, CM_BUFF_SOCK,"");
  sprintf( vl_key, "%s_%s_debug2",vl_NomMac, vl_NomTache );
  sprintf(vl_msg,"|52|%s_%s|debug2|%d",
		vl_NomMac, vl_NomTache, ( (vl_NivTrace & XZSTC_DEBUG2) != 0 ));
  send_message(vl_msg, CM_BUFF_SOCK,"");
  sprintf( vl_key, "%s_%s_debug3",vl_NomMac, vl_NomTache );
  sprintf(vl_msg,"|52|%s_%s|debug3|%d",
		vl_NomMac, vl_NomTache, ( (vl_NivTrace & XZSTC_DEBUG3) != 0 ));
  send_message(vl_msg, CM_BUFF_SOCK,"");
  sprintf( vl_key, "%s_%s_interface",vl_NomMac, vl_NomTache );
  sprintf(vl_msg,"|52|%s_%s|interface|%d",
		vl_NomMac, vl_NomTache, ( (vl_NivTrace & XZSTC_INTERFACE) != 0 ));
  send_message(vl_msg, CM_BUFF_SOCK,"");
  sprintf( vl_key, "%s_%s_regle",vl_NomMac, vl_NomTache );
  sprintf(vl_msg,"|52|%s_%s|regle|%d",
		vl_NomMac, vl_NomTache, ( (vl_NivTrace & XZSTC_REGLE) != 0 ));
  send_message(vl_msg, CM_BUFF_SOCK,"");
  }
}


/* Fichier : @(#)gmot_adm_dv.c  1.15                    Release : 1.15              Date : 10/09/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction s'execute a la reception d'un message
* XDM_ADM_ARC_STA_M (rafraichissement d'un synoptique):
* - decodage du message
* - construction du nom de variable de synoptique
* - valorisation de cette variable avec la valeur
*   recue (cette variable se situe dans la table
*   de symboles globale vm_DataSymTable )
*
******************************************************/
void
MajAdmStatiM( T_IPC_CONN                 va_Cnx,
              T_IPC_CONN_PROCESS_CB_DATA pa_Data,
              T_CB_ARG                   pa_Arg )
{ 
#ifndef _TIBCO_EMS
  T_STR         vl_NomMac ;  /* variables de lecture de message */
  T_INT4        vl_ModeInstr;
#else
  XDY_NomMachine         vl_NomMac ;  /* variables de lecture de message */
  XDY_Entier        vl_ModeInstr;
#endif

  char          vl_key[50];  /* variables DV-Draw */
  int           pl_valeur;

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: MajAdmStatiM " );
  /* decodage du message */
       if (!TipcMsgRead(pa_Data->msg
                ,T_IPC_FT_STR,&vl_NomMac
                ,T_IPC_FT_INT4,&vl_ModeInstr
                ,NULL))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
                "MajAdmStatiM : Pb lecture message XDM_ADM_ARC_STA_M" );
    return;
  }

  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "ARC_%s_modeI", vl_NomMac );
	if (hash_data(vl_key, vl_ModeInstr))
	{
        switch (vl_ModeInstr)
        {
        case XZSIC_MODE_INS:
                        pl_valeur = 1;
                        break;

        case XZSIC_MODE_NOR:
                        pl_valeur = 0;
                        break;
        default:
                        XZST_03EcritureTrace( XZSTC_WARNING,
                                 "MajAdmStatiM : Mauvais parametre d'entree " );
                        break;
        }
/*flash todo*/
        }
}


/******************************************************
* MODULE gmot_adm_dv
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction s'execute a la reception d'un message
* XDM_ADM_ARC_STA_E (rafraichissement d'un synoptique):
* - decodage du message
* - construction du nom de variable de synoptique
* - valorisation de cette variable avec la valeur
*   recue (cette variable se situe dans la table
*   de symboles globale vm_DataSymTable )
*
******************************************************/
void
MajAdmStatiE( T_IPC_CONN                 va_Cnx,
              T_IPC_CONN_PROCESS_CB_DATA pa_Data,
              T_CB_ARG                   pa_Arg )
{
#ifndef _TIBCO_EMS
  T_STR         vl_NomMac ;  /* variables de lecture de message */
  T_INT4        vl_EtatStation;
#else
  XDY_NomMachine         vl_NomMac ;  /* variables de lecture de message */
  XDY_Entier        vl_EtatStation;
#endif

  char          vl_key[50];  /* variables DV-Draw */
  int           pl_valeur;
  int           vl_TypeMaj_Synt         = XZSIC_ETAT_NACTIF;

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: MajAdmStatiE " );

  /* decodage du message */
       if (!TipcMsgRead(pa_Data->msg
                ,T_IPC_FT_STR,&vl_NomMac
                ,T_IPC_FT_INT4,&vl_EtatStation
                ,NULL))
  {
    XZST_03EcritureTrace( XZSTC_WARNING,
                "MajAdmStatiE : Pb lecture message XDM_ADM_ARC_STA_E" );
    return;
  }
  /*  construction du nom de variable de synoptique */
  sprintf( vl_key, "ARC_%s_etat", vl_NomMac );
  sprintf( vl_key, "%s_etat", vl_NomMac );
	if (hash_data(vl_key,vl_EtatStation))
	{	
        switch (vl_EtatStation)
        {
        case XZSIC_ETAT_VEILLE:
                pl_valeur = 4;

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Mach_Synt(vl_NomMac,vl_EtatStation);

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Res_Synt(vl_NomMac,vl_EtatStation);

                break;
        case XZSIC_ETAT_NACTIF:
                pl_valeur = 3;

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Mach_Synt(vl_NomMac,vl_EtatStation);

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Res_Synt(vl_NomMac,vl_EtatStation);

                break;
        case XZSIC_ETAT_NPASSIF:
                pl_valeur = 2;

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Mach_Synt(vl_NomMac,vl_EtatStation);

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Res_Synt(vl_NomMac,vl_EtatStation);

                break;
        case XZSIC_ETAT_DEGRADE:
                pl_valeur = 1;

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Mach_Synt(vl_NomMac,vl_EtatStation);

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Res_Synt(vl_NomMac,vl_EtatStation);

                break;

        case XZSIC_SANS_ETAT:
                pl_valeur = 0;

                /*A Mis a jour de la vue de Synthese */
                MajAdm_Res_Synt(vl_NomMac,vl_EtatStation);

                break;
        default:
                        XZST_03EcritureTrace( XZSTC_WARNING,
                                 "MajAdmStatiE : Mauvais parametre d'entree " );
                        break;
        }
/*flash*/
  if ((vl_NomMac[0]=='S') && (vl_NomMac[1]=='D'))
    sprintf(vl_msg,"|54|%s|etat|%d",
		vl_NomMac, vl_EtatStation);
  else
    sprintf(vl_msg,"|57|%s|etat|%d",
		vl_NomMac, vl_EtatStation);

  send_message(vl_msg, CM_BUFF_SOCK,"");
  }
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Permet de mettre a jour le synoptique de Synthese de Machine
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void MajAdm_Mach_Synt(
        XZSCT_NomMachine        va_NomMachine,
        int                     va_EtatMach)
{
        XZSCT_NomSite   vl_NomSite      = "";
                /* variables DV-Draw */
        char          vl_key[50]        = "";

        int           vl_valeur    ;
        int             vl_NbElt        = 0;
	struct data_struct *mess;
	int vl_cr;

        XZST_03EcritureTrace(XZSTC_WARNING,
                "MajAdm_Mach_Synt : IN \
                -- Nom Mach %s -- Etat mac %d \n",
                va_NomMachine, va_EtatMach);

        /*A Recuperation du Nom du Site associe a la
        *       machine */
        vl_cr = XZSA_23SiteMachine(va_NomMachine, vl_NomSite);
        if (vl_cr == XDC_NOK) return ;

        /* construction du nom de variable de synoptique */
        sprintf( vl_key, "SYNT_%s_stat_mach", vl_NomSite );
        sprintf( vl_key, "%s_etat_machine", vl_NomSite );

        /*B Teste si au moins une machine degrade */
        if (va_EtatMach == XZSIC_ETAT_DEGRADE)
        {
                /*A MAJ etat de Synthes Machine */
                vs_MachSynt = 0;
        }
        else if (va_EtatMach != XZSEC_ETAT_DEGRADE)
        {
                /*B MAJ de l'etat de Synthese
                * en parcourant etat de toutes
                * les machines du Site */
		vs_MachSynt = 2;
                XZSA_19ListeMachineSite(vl_NomSite,
                           MajSynt_Mach_Synop,&vl_NbElt,NULL);

                /*B MAJ de l'etat de Synthese
                * en parcourant etat de toutes
                * les Peripheriques du Site */
		if (vs_MachSynt != XZSIC_NOK)
                  XZSA_29ListeEntiteSite(vl_NomSite,
                           MajSynt_Ent_Synop,&vl_NbElt,NULL);
        }
        XZST_03EcritureTrace(XZSTC_DEBUG1,
                "Maj Synthese cle %s -- MachSynt %d \n",
                vl_key,vs_MachSynt);


        /*A Affectation de la Valeur Synoptique */
        vl_valeur = vs_MachSynt;
  

        XZST_03EcritureTrace(XZSTC_FONCTION,
                "MajAdm_Mach_Synt : OUT \
        --cle %s -- Valeur SyntMAch %d \n",
                vl_key,vl_valeur);
	/* Patch LCL */
	if (vl_valeur>1) vl_valeur=2;
XZST_03EcritureTrace(XZSTC_WARNING,"MajAdm_Mach_Synt : OUT --cle %s -- Valeur SyntMAch %d ",vl_key,vl_valeur);

	if (hash_data(vl_key,vs_MachSynt))
	{
	 sprintf(vl_msg,"|53|%s|etat_machines|%d",
			vl_NomSite,vl_valeur);
	send_message(vl_msg, CM_BUFF_SOCK,"");
	}
}       /* Fin MajAdm_Mach_Synt */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Permet de mettre a jour le synoptique de Synthese de Machine
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MajAdm_Ent_Synt(
        XZSCT_NomEntite         va_NomEntite,
        int                     va_EtatEnt,
        int                     va_TypeEnt)
{
        XZSCT_NomSite   vl_NomSite      = "";
                /* variables DV-Draw */
        char          vl_key[50]        = "";
        int           vl_valeur        = NULL;
        int             vl_NbElt        = 0;

        XZST_03EcritureTrace(XZSTC_WARNING,
                "MajAdm_Ent_Synt : IN \
                -- Nom Mach %s -- Etat ent %d \n",
                va_NomEntite, va_EtatEnt);

        /*A Recuperation du Nom du Site associe a la
        *       machine */
        XZSA_28SiteEntite(va_NomEntite, va_TypeEnt,vl_NomSite);

        /* construction du nom de variable de synoptique */
        sprintf( vl_key, "SYNT_%s_stat_mach", vl_NomSite );
	sprintf( vl_key, "%s_etat_machine", vl_NomSite );
        /*  valorisation de cette variable avec la valeur */
        /*  recue (cette variable se situe dans la table */
        /*  de symboles globale vm_DataSymTable ) */
	hash_data(vl_key,va_EtatEnt);

        /*B Teste si au moins une machine degrade */
        if (va_EtatEnt == XZSIC_NOK)
        {
                /*A MAJ etat de Synthes Machine */
                vs_MachSynt = 0;
        }
        else
        {
		vs_MachSynt = 2;
                /*B MAJ de l'etat de Synthese
                * en parcourant etat de toutes
                * les Peripheriques du Site */
                XZSA_29ListeEntiteSite(vl_NomSite,
                           MajSynt_Ent_Synop,&vl_NbElt,NULL);

                /*B MAJ de l'etat de Synthese
                * en parcourant etat de toutes
                * les machines du Site */
		if (vs_MachSynt != XZSIC_NOK)
                  XZSA_19ListeMachineSite(vl_NomSite,
                           MajSynt_Mach_Synop,&vl_NbElt,NULL);
        }

        XZST_03EcritureTrace(XZSTC_DEBUG1,
                "Maj Synthese cle %s -- MachSynt %d \n",
                vl_key,vs_MachSynt);


        /*A Affectation de la Valeur Synoptique */
        vl_valeur = vs_MachSynt;
XZST_03EcritureTrace(XZSTC_WARNING,"MajAdm_Ent_Synt : OUT --cle %s -- Valeur SyntMAch %d ",vl_key,vl_valeur);
       XZST_03EcritureTrace(XZSTC_FONCTION,
                "MajAdm_Ent_Synt : OUT \
        --cle %s -- Valeur SyntMAch %d \n",
                vl_key,vl_valeur);

}       /* Fin MajAdm_Ent_Synt */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Permet de mettre a jour le synoptique de Synthese de Reseau
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void MajAdm_Res_Synt(
        XZSCT_NomMachine        va_NomMachine,
        int                     va_EtatMach)
{
        XZSCT_NomSite   vl_NomSite      = "";
                /* variables DV-Draw */
        char          vl_key[50]        = "";
        int           vl_valeur     ;
        int             vl_NbElt        = 0;
	struct data_struct *mess;
	int vl_cr;

        XZST_03EcritureTrace(XZSTC_FONCTION,
                "MajAdm_Res_Synt : IN \
                -- Nom Mach  %s -- Etat mac %d  \n",
                va_NomMachine, va_EtatMach);

        /*A Recuperation du Nom du Site associe a la
        *       machine */
        vl_cr=XZSA_23SiteMachine(va_NomMachine, vl_NomSite);
        if (vl_cr == XDC_NOK) return ;

        /* construction du nom de variable de synoptique */
        sprintf( vl_key, "SYNT_%s_stat_res", vl_NomSite );
  /*  valorisation de cette variable avec la valeur */
  /*  recue (cette variable se situe dans la table */
  /*  de symboles globale vm_DataSymTable ) */
	if (hash_data(vl_key, va_EtatMach))
	{
        /*B Teste si au moins une machine degrade */
        /* Modif PN v1.5 */vs_ResSynt = XZSEC_ETAT_OK;
        if (va_EtatMach == XZSIC_SANS_ETAT)
        {
                /*A MAJ etat de Synthes Machine */
                vs_ResSynt = XZSIC_SANS_ETAT;
        }
        else
        {
                /*B MAJ de l'etat de Synthes en
                * parcourant etat de toutes les
                *       machines du Site */
                XZSA_19ListeMachineSite(vl_NomSite,
                        MajSynt_Res_Synop,&vl_NbElt,NULL);
        }

        /*A Affectation de la Valeur Synoptique */
        vl_valeur = vs_ResSynt;
        XZST_03EcritureTrace(XZSTC_FONCTION,
        "MajAdm_Res_Synt : OUT --cle %s -- SyntRes %d \n",
                vl_key,vl_valeur);
	 sprintf(vl_msg,"|53|%s|etat_reseau|%d",
			vl_NomSite,vs_ResSynt);
	send_message(vl_msg, CM_BUFF_SOCK,"");
	}

}       /* Fin MajAdm_Res_Synt */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Permet de calculer l'etat de Synthese des Machines
*       du site asssocie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void MajSynt_Mach_Synop(
        XZSCT_NomMachine pa_NomMachine,
        int              va_NumOrdre,
        void             **pa_DonneesMsg)
{
        char          vl_key[50]        = "";
        char           vl_valeur[10]      ;
	int vl_valeur_entier;
        int             vl_MASK         = 0x0007;
	struct data_struct *mess;	

	if (vs_MachSynt==0) return;

        XZST_03EcritureTrace(XZSTC_FONCTION,
                " MajSynt_Mach_Synop  : IN -- machine %s \n",
                pa_NomMachine);

        sprintf( vl_key, "%s_etat", pa_NomMachine);

        /*  valorisation de cette variable avec la valeur */
        /*  recue (cette variable se situe dans la table */
        /*  de symboles globale vm_DataSymTable ) */
	 get_data(vl_key,vl_valeur,mess);


        XZST_03EcritureTrace(XZSTC_DEBUG1,
                " Etat de Synthese %s -- %d ",
                vl_valeur,vs_MachSynt);

		sscanf(vl_valeur, "%d", &vl_valeur_entier);

        /*A Memorisation de l'etat de Synthese */
        /*? On effectue un & logique avec la valeur recupere
        * de la machine pour obtenir soit 0 si une machine
        * est en Mode degrade (valeur = 1) ou superieur a
        * 2 si Actif,Passif ou Veille (valeur = 3,2 et 1) */
        /*B On calcule l'etat de Synthese , seulement si
        *       l'etat de la machine est different de
        *       SANS_ETAT */
        if (vl_valeur_entier != XZSIC_SANS_ETAT)
        {
                if (vl_valeur_entier ==XZSEC_ETAT_DEGRADE)
                {
                        /*A Mode Degrade */
                        vs_MachSynt = 0;
                }
		/*
                else
                {
                        vs_MachSynt = vs_MachSynt &
                                ( (vl_valeur) | vl_MASK) ;
                }*/
        }
       XZST_03EcritureTrace(XZSTC_FONCTION,
                " MajSynt_Mach_Synop  : OUT  -- Machine %d  Valeur %d ",
                vs_MachSynt);
       XZST_03EcritureTrace(XZSTC_WARNING,
                " MajSynt_Mach_Synop  : OUT  --  Valeur %d -- MachSynt %d -- Entite %s ", vl_valeur_entier, vs_MachSynt, pa_NomMachine );

}       /* Fin MajSynt_Mach_Synop */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Permet de calculer l'etat de Synthese des Entites
*       du site asssocie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MajSynt_Ent_Synop(
        XZSCT_NomEntite pa_NomEntite,
        int              va_NumOrdre,
        void             **pa_DonneesMsg)
{
        char          vl_key[50]        = "";
        char           vl_valeur[10];
	int vl_valeur_entier;
        int             vl_MASK         = 0x0007;
	struct data_struct *mess;


	if (vs_MachSynt==0) return;

        XZST_03EcritureTrace(XZSTC_FONCTION,
                " MajSynt_Ent_Synop  : IN -- Entite %s ",
                pa_NomEntite);

        sprintf( vl_key, "%s_etat", pa_NomEntite);

        /*  valorisation de cette variable avec la valeur */
        /*  recue (cette variable se situe dans la table */
        /*  de symboles globale vm_DataSymTable ) */
	get_data(vl_key,vl_valeur, mess);
	sscanf(vl_valeur,"%d",&vl_valeur_entier);

       XZST_03EcritureTrace(XZSTC_WARNING,
                " Etat de Synthese %d -- %d ",
                vl_valeur_entier,vs_MachSynt);

       /*A Memorisation de l'etat de Synthese */
        /*? On effectue un & logique avec la valeur recupere
        * de la machine pour obtenir soit 0 si une machine
        * est en Mode degrade (valeur = 1) ou superieur a
        * 2 si Actif,Passif ou Veille (valeur = 3,2 et 1) */
        /*B On calcule l'etat de Synthese , seulement si
        *       l'etat de la machine est different de
        *       SANS_ETAT */
        if (vl_valeur_entier != XZSIC_SANS_ETAT)
        {
                if (vl_valeur_entier == XZSEC_ETAT_DEGRADE)
                {
                        /*A Mode Degrade */
                        vs_MachSynt = 0;
                }
		/*
                else
                {
                        vs_MachSynt = vs_MachSynt &
                                ( (vl_valeur) | vl_MASK) ;
                }*/
        }
       XZST_03EcritureTrace(XZSTC_WARNING,
                " MajSynt_Ent_Synop  : OUT  --  Valeur %d -- MachSynt %d -- Entite %s ", vl_valeur_entier, vs_MachSynt, pa_NomEntite );

}       /* Fin MajSynt_Ent_Synop */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Permet de calculer l'etat de Synthese des Machines
*       du site asssocie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void MajSynt_Res_Synop(
        XZSCT_NomMachine pa_NomMachine,
        int              va_NumOrdre,
        void             **pa_DonneesMsg)
{
        static char *version="@(#)gmot_adm_dv.c 1.15      10/09/07 : MajSynt_Res_Synop " ;
        char          vl_key[50]        = "";

        int           vl_valeur        = 0;
        int             vl_MASK         = 0x0007;
	struct data_struct *mess;


        XZST_03EcritureTrace(XZSTC_FONCTION,
                " MajSynt_Res_Synop  : IN -- machine %s \n",
                pa_NomMachine);

       sprintf( vl_key, "%s_etat", pa_NomMachine);
        /*  valorisation de cette variable avec la valeur */
        /*  recue (cette variable se situe dans la table */
        /*  de symboles globale vm_DataSymTable ) */
	get_data(vl_key,&vl_valeur, mess);

        XZST_03EcritureTrace(XZSTC_DEBUG1,
                " Etat de Synthese %d -- %d \n",
                vl_valeur,vs_ResSynt);
        /*A Mode Degrade */
        vs_ResSynt = vs_ResSynt * ((vl_valeur == 0 ) ? 0 : 1); /* 1.6 */

        XZST_03EcritureTrace(XZSTC_FONCTION,
                " MajSynt_Res_Synop  : OUT  --  Valeur %d \n",
                vs_ResSynt);

}       /* Fin MajSynt_Res_Synop */

