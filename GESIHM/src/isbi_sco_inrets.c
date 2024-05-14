/*E*/
/*Fichier : $Id: isbi_sco_inrets.c,v 1.2 2012/05/29 10:09:29 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2012/05/29 10:09:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE MSCO * FICHIER isbi_sco_inrets.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module permettant l'affichage de courbes inrets au sein du synoptique
*
------------------------------------------------------
* HISTORIQUE :
*

* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/ 

/* fichiers inclus */

#include "isbi_sco.h"
#include "isbi_sra_rafobj.h"

/* definitions de constantes */
#define CM_SEUIL_MAX 32767

#define CM_TAILLE_TABLE_DE_SEUIL 4
#define CM_TAILLE_TABLE_DE_SEUIL_BOUCHON (CM_TAILLE_TABLE_DE_SEUIL + CM_NB_NIVEAU)
#define CM_TAILLE_TABLE_DE_SEUIL_BOUCHON_SEUL (CM_NB_NIVEAU+1)

#define CM_60secondes      60
#define CM_6heures (3600/60/6)

//#define CM_6HEURES (3600*6)

#define CM_V_MAX 200
#define CM_Q_MAX 8000
#define CM_TO_MAX 30

#define CM_MAX_SLOT 1120

#define CM_INRETS  	XDC_GR_INRETS
#define CM_INRETS_BOUCH XDC_GR_INRETS_BOUCHON
#define CM_BOUCH        XDC_GR_BOUCHON


#define CM_NB_SLOT_V 200
#define CM_NB_SLOT_Q 800
#define CM_NB_SLOT_TO 30

#define CM_SYMBOLE 'O'
#define CM_OBJ_ATTENTE "msg_attente"
#define CM_OBJ_DONNEES "donnees"

#define CM_BUFF_SOCK  600
#define CM_NB_FENTRES 2


               
/* definition de macro locales */

/* declaration de variables locales */
static char *version = "@(#)isyn_sco_inrets.c	1.23 08/18/08 : isyn_sco" ;
char vl_msg[CM_BUFF_SOCK];

static XDY_Horodate vm_HorodateSecInit[CM_NB_FENTRES];
static XDY_Horodate vm_HorodateSecDeb[CM_NB_MAX_FENETRES];
static float  vm_duree=24.0;
static float  vm_rapport=1.0;

static XDY_Octet vm_type_courbe[CM_NB_MAX_FENETRES]={0,0,0};
static XDY_Entier vm_taille_tra[CM_NB_MAX_FENETRES]={0,0,0};

static T_CB vm_callback[CM_NB_MAX_FENETRES];
static T_CB vm_callback_pr[CM_NB_MAX_FENETRES];
static T_CB vm_callback_tra[CM_NB_MAX_FENETRES];

static XZIST_Ligne_INRETS_PR *vm_donnees_inrets_PR[CM_NB_MAX_FENETRES]={NULL,NULL,NULL};
static XZIST_Ligne_INRETS_Trafic *vm_donnees_inrets_TRA[CM_NB_MAX_FENETRES]={NULL,NULL,NULL};
static XDY_Octet *vm_donnees_inrets_Bouchons[CM_NB_MAX_FENETRES]={NULL,NULL,NULL};
static int vm_nb_Donnees[CM_NB_MAX_FENETRES]={0,0,0};
static XDY_PR vm_prdepart[CM_NB_MAX_FENETRES]={0,0,0},vm_prdeb[CM_NB_MAX_FENETRES]={0,0,0},vm_distance[CM_NB_MAX_FENETRES]={0,0,0};
static XDY_Entier vm_prfin[CM_NB_MAX_FENETRES];
static int vm_precision[CM_NB_MAX_FENETRES]={100,100,100};


/* declaration de variables externes */
extern char c_correspFenetre[];

/* declaration de fonctions internes */
void MSCO_Recup_donnees_INRETS_Bouchons();
void MSCO_Recup_donnees_INRETS_TRA();
void MSCO_Recup_donnees_INRETS_PR();
/* definition de fonctions externes */







/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Initialisation de la courbe INRETS
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO04_init_courbe_INRETS(
                    va_horodeb,
                    va_prdebut,
                    va_prfin,
                    va_type_courbe,
                    va_fenetre,
                    va_no_autoroute,
                    va_nom_autoroute,
                    va_plage,
                    va_Sens)

                    char      * va_horodeb;
        	    XDY_Entier	va_prdebut;
                    XDY_Entier	va_prfin;
                    XDY_Octet   va_type_courbe;
                    XDY_Octet   va_fenetre;
		    XDY_Autoroute va_no_autoroute;
		    char      * va_nom_autoroute;
		    XDY_Octet	va_plage;
		    XDY_Sens	va_Sens;


/*
* ARGUMENTS EN ENTREE :
*  va_horodeb : horodate de debut
*  va_prdebut : pr de debut
*  va_prfin : pr de fin
*  va_type_courbe: Type de courbe : INRETS sans bouchon INRETS avec bouchon
*  va_fenetre : fenetre d'affichage de la courbe droite ou gauche CM_FENETRE_DROITE , CM_FENETRE_GAUCHE
* va_no_autoroute : no d'autoroute
* va_nom_autoroute : nom d'autoroute
* va_plage : plage horaire demandee (ente 6 et 24 heures)
* va_Sens : sens

* ARGUMENTS EN SORTIE :
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
{static char *version = "@(#)isyn_sco_inrets.c	1.23 08/18/08 : MSCO04_init_courbe_INRETS " ;
char vl_titre[100];
char vl_chaine_fenetre[10];
int i;
int    vl_JourSemaine;
XDY_Horodate vl_HorodateFin, vl_HorodateDeb;
XZSCT_Datagroup vl_datagroup_init ;
T_BOOL vl_etat;
int code_retour=0;
int vl_x, vl_y;
XDY_Octet     vl_duree=24;
int vl_nb_slots;

XZST_03EcritureTrace(XZSTC_FONCTION,"IN  : MSCO04_init_courbe_INRETS: va_horodeb:%s,va_prdebut:%d,va_prfin:%d, va_type_courbe:%d,va_fenetre:%d,va_plage:%d" ,
						va_horodeb,  va_prdebut, va_prfin, va_type_courbe, va_fenetre,va_plage);

	vl_duree=va_plage;

	if ( XZSM_13ConversionHorodateSec( va_horodeb , &vl_HorodateDeb) != XDC_OK )
	   {
	   XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la conversion de %s en Sec", va_horodeb );
	   return XDC_NOK;
	   }
	/*   vl_HorodateFin = vl_HorodateDeb + CM_6HEURES;*/
	vl_HorodateFin = vl_HorodateDeb + vl_duree*3600;
	vm_HorodateSecInit[va_fenetre-1] = vl_HorodateDeb;
	vm_duree = (float) vl_duree;

	/*A Teste la validite du tyoe de courbe */
	if (( va_type_courbe != CM_INRETS ) && ( va_type_courbe != CM_INRETS_BOUCH ) && ( va_type_courbe != CM_BOUCH ))
	   {
	   XZST_03EcritureTrace(   XZSTC_WARNING , "Type de courbe invalide: %d, Courbe inrets par default", va_type_courbe  );
	   vm_type_courbe[va_fenetre-1] = CM_INRETS;
	   }
	else
		vm_type_courbe[va_fenetre-1] = va_type_courbe;

	vm_prdeb[va_fenetre-1]=va_prdebut;
	vm_prfin[va_fenetre-1] = va_prfin;
	if ( va_prdebut > va_prfin )
		vm_prdepart[va_fenetre-1] = va_prfin;
	else
		vm_prdepart[va_fenetre-1]=va_prdebut;

	vm_distance[va_fenetre-1]=abs(va_prfin  - va_prdebut);

	if ( vm_distance[va_fenetre-1] < 5000)
		vm_precision[va_fenetre-1] = 100;
	else
		vm_precision[va_fenetre-1] = vm_distance[va_fenetre-1]/210;

	vl_nb_slots = (vl_HorodateFin - vl_HorodateDeb ) / CM_60secondes ;
	if ( vl_nb_slots > CM_MAX_SLOT )
	  {
	  vm_rapport = ((float) vl_nb_slots)/((float) CM_MAX_SLOT);
	  XZST_03EcritureTrace(   XZSTC_DEBUG1, "vm_rapport = %lf",vm_rapport);
	  vl_nb_slots= CM_MAX_SLOT;
	  }
	else
	  vm_rapport=1.0;

	switch ( vm_type_courbe[va_fenetre-1] )
	{
		case CM_INRETS_BOUCH :
			sprintf( vl_titre , "Graphe Carto. densite + Bouchon %s pr %d,pr %d,%d", va_nom_autoroute , va_prdebut , va_prfin , va_Sens );
			break;
		case CM_INRETS :
			sprintf( vl_titre , "Graphe Cartographie densite  %s pr %d, pr %d, %d" ,va_nom_autoroute, va_prdebut , va_prfin , va_Sens );
			break;
		case CM_BOUCH :
			sprintf( vl_titre , "Graphe Bouchon %s pr %d, pr %d, %d" ,va_nom_autoroute, va_prdebut , va_prfin , va_Sens );
			break;
	}

	vl_x=(6*60+1);
	vl_y=((vm_distance[va_fenetre-1]/vm_precision[va_fenetre-1])+1);

	sprintf(vl_msg,"%d|%d|%c|%d!%s!%d!%d!%f!%f!%d!%d",CMD_FLASH_INIT_COURBE, CM_COURBE_INRETS, c_correspFenetre[va_fenetre-1], vm_type_courbe[va_fenetre-1], vl_titre, va_prdebut, va_prfin, vl_HorodateDeb, vl_HorodateFin, vl_x, vl_y);
	 if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
	 {
	    XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
	    return ;
	 }


	vm_taille_tra[va_fenetre-1] = 0;

	//recup des donn��es
	if ( vm_type_courbe[va_fenetre-1] == CM_BOUCH )
	  {
		 sprintf( vl_chaine_fenetre , "%d" , va_fenetre );
		 XDG_EncodeDG3(vl_datagroup_init , XDG_IDONNEES_BOUCHONS , vl_chaine_fenetre , vm_NomMachine );

			XZST_03EcritureTrace( XZSTC_WARNING,
					  "DG = %s " , vl_datagroup_init);


		 /*A Abonnement XDG_IDONNEES_BOUCHONS+vm_NomMachine pour le callback si ce n'est deja fait */
		 TipcSrvDgGetRecv( vl_datagroup_init , &vl_etat );
		 if ( vl_etat == FALSE )
			{
			if (! XDG_JoindreDG(vl_datagroup_init) )
			   {
			   /* Ecriture Trace */
			   XZST_03EcritureTrace( XZSTC_WARNING,
				   "Abonnement sur %s non effectuee. \n " ,vl_datagroup_init);
			   return XDC_NOK;
			   }
			}
		 /*A  Armement Callback sur Reception XDM_IDONNEES_BOUCHONS */
		 if ( (vm_callback[va_fenetre-1] = TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IDONNEES_BOUCHONS ),
				   MSCO_Recup_donnees_INRETS_Bouchons,(T_CB_ARG) va_fenetre )) == NULL)
			{
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING,
					  "Callback sur %s non declenchee. \n " , XDM_IDONNEES_BOUCHONS);
			return XDC_NOK;
			}
	  /*A Appel a XZAS... pour demander la recuperation des donnees des courbes */
	  if ( XZAS03_Bouchon( vl_datagroup_init , va_no_autoroute , va_prdebut, va_prfin, vl_HorodateDeb ) != XDC_OK )
		   {
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING,
					  "Probleme a l'appel de XZAS03_Bouchon");
			return XDC_NOK;
			}
	  }
	else
	  {
		 sprintf( vl_chaine_fenetre , "%d" , va_fenetre );
		 XDG_EncodeDG3(vl_datagroup_init , XDG_IDONNEES_INRETS_TRA , vl_chaine_fenetre , vm_NomMachine );
		 /*A Abonnement XDG_IDONNEES_INRETS_TRA+vm_NomMachine pour le callback si ce n'est deja fait */
		 TipcSrvDgGetRecv( vl_datagroup_init , &vl_etat );
		 if ( vl_etat == FALSE )
			{
			if (! XDG_JoindreDG(vl_datagroup_init) )
			   {
			   /* Ecriture Trace */
			   XZST_03EcritureTrace( XZSTC_WARNING,
				   "Abonnement sur %s non effectuee. \n " ,vl_datagroup_init);
			   return XDC_NOK;
			   }
			}
		 /*A  Armement Callback sur Reception XDM_IDONNEES_INRETS_TRA */
		 if ( (vm_callback_tra[va_fenetre-1] = TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IDONNEES_INRETS_TRA ),
				   MSCO_Recup_donnees_INRETS_TRA,(T_CB_ARG) va_fenetre )) == NULL)
			{
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING,
					  "Callback sur XDM_IDONNEES_INRETS_TRA non declenchee. %d\n " , vm_callback_tra[va_fenetre-1]);
			return XDC_NOK;
			}
		 /*A  Armement Callback sur Reception XDM_IDONNEES_INRETS_PR */
		 if ( (vm_callback_pr[va_fenetre-1] = TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IDONNEES_INRETS_PR ),
				   MSCO_Recup_donnees_INRETS_PR,(T_CB_ARG) va_fenetre )) == NULL)
			{
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING,
					  "Callback sur XDM_IDONNEES_INRETS_PR non declenchee. %d\n " , vm_callback_pr[va_fenetre-1] );
			return XDC_NOK;
			}
	  /*A Appel a XZAS... pour demander la recuperation des donnees des courbes */
	  if ( XZAS02_Inrets( vl_datagroup_init , va_no_autoroute , va_prdebut, va_prfin, vl_HorodateDeb , (XDY_Octet) vl_duree) != XDC_OK )
		   {
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING,
					  "Probleme a l'appel de XZAS02_Inrets");
			return XDC_NOK;
			}
	   }

XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : MSCO04_init_courbe_INRETS");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Arret des Courbes INRETS
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void MSCO04_arret_courbe_INRETS(va_fenetre)
				int va_fenetre;


/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une callback
*  pa_caract : Caracteristiques de la courbe.
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
*   Creation de la table de hash
*
------------------------------------------------------*/
{
	vm_type_courbe[va_fenetre-1] = 0;

	if (vm_callback[va_fenetre-1] !=  NULL){
		/*A  DesArmement Callback sur Reception  */
		if ( TutCbDestroy( vm_callback[va_fenetre-1] ) == FALSE )
		{
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING,
					   "Desarmement de la Callback sur %s non effectuee. \n " ,XDM_IDONNEES_BOUCHONS);
			return;
		}
		vm_callback[va_fenetre-1] =  NULL;
	}

	if (vm_callback_pr[va_fenetre-1] !=  NULL){
		/*A  DesArmement Callback sur Reception  */
		if ( TutCbDestroy( vm_callback_pr[va_fenetre-1] ) == FALSE )
		{
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING,
					   "Desarmement de la Callback sur %s non effectuee. \n " ,XDM_IDONNEES_BOUCHONS);
			return;
		}
		vm_callback_pr[va_fenetre-1] =  NULL;
	}

	if (vm_callback_tra[va_fenetre-1] !=  NULL){
		/*A  DesArmement Callback sur Reception  */
		if ( TutCbDestroy( vm_callback_tra[va_fenetre-1] ) == FALSE )
		{
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING,
					   "Desarmement de la Callback sur %s non effectuee. \n " ,XDM_IDONNEES_BOUCHONS);
			return;
		}
		vm_callback_tra[va_fenetre-1] =  NULL;
	}
   sprintf(vl_msg, "|%d|%d|%c", CMD_FLASH_ARRET_COURBE, CM_COURBE_INRETS, c_correspFenetre[va_fenetre-1]);
   if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
	   {
	   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
	   return ;
	   }

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Recuperation des donnees pour les courbes a partir de la bd
   pour ft.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSCO_Recup_donnees_INRETS_Bouchons(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_arg)

/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une callback
*  pa_caract : Caracteristiques de la courbe.
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
*   Creation de la table de hash
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sco_inrets.c	1.23 08/18/08 : MSCO_Recup_donnees_INRETS_Bouchons " ;
/* DEM804 */
T_REAL8 TabHorodeb [2000];
T_REAL8 TabHorofin [2000];
T_INT4 Tabprdeb [2000];
T_INT4 Tabprfin [2000];
T_INT2 Tabplniv [2000];

 T_INT4 *pl_prdeb_f;
 T_INT4 *pl_prfin_f;
 T_INT2 *pl_niv_f;
 T_REAL8 *pl_horodeb_f;
 T_REAL8 *pl_horofin_f;

#ifdef _TIBCO_EMS
 T_INT2 vl_Autoroute;
 T_INT4 vl_prdeb;
 T_INT4 vl_prfin;
 T_REAL8 vl_HorodateDebut;
 T_INT4 *pl_numfmc;
 T_REAL8 *pl_horodeb;
 T_REAL8 *pl_horofin;
 T_INT4 *pl_prdeb;
 T_INT4 *pl_prfin;
 T_INT2 *pl_niv;
 T_INT4  vl_nb_Donnees_horodeb,vl_nb_Donnees_horofin,
         vl_nb_Donnees_prdeb,vl_nb_Donnees_numfmc, vl_nb_Donnees_prfin,vl_nb_Donnees_niv;
#else
 XDY_Mot 	vl_Autoroute;
 XDY_Entier 	vl_prdeb;
 XDY_Entier 	vl_prfin;
 XDY_Horodate 	vl_HorodateDebut;
 XDY_Entier 	pl_numfmc[2000];
 XDY_Horodate   pl_horodeb[2000];
 XDY_Horodate   pl_horofin[2000];
 XDY_Entier 	pl_prdeb[2000];
 XDY_Entier 	pl_prfin[2000];
 XDY_Mot 	pl_niv[2000];
 XDY_Entier  	vl_nb_Donnees_horodeb,vl_nb_Donnees_horofin,
         	vl_nb_Donnees_prdeb,vl_nb_Donnees_numfmc, vl_nb_Donnees_prfin,vl_nb_Donnees_niv;
#endif

   int i=0,j=0,k=0,a=0,b=0;
   int h=0;
 XDY_Octet vl_fenetre;
 int vl_rang=0;int vl_compteur_bouchon=0;
   XZSMT_Horodate vl_horodated;
   XZSMT_Horodate vl_horodatef;

   XZSMT_Horodate vl_horodated_f;
   XZSMT_Horodate vl_horodatef_f;

   float vl_pente_fin=0.0;
   float vl_pente_deb=0.0;
   int vl_modulo = 0;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Recup_donnees_INRETS_Bouchons" );

   vl_fenetre = (XDY_Octet) pa_arg;
   /*A RECUPERE LES DONNEES et bufferiser*/

   /* Recuperation des donnees du message */
   if (TipcMsgRead(pa_Data->msg
			,T_IPC_FT_INT2,&vl_Autoroute
			,T_IPC_FT_INT4,&vl_prdeb
			,T_IPC_FT_INT4,&vl_prfin
			,T_IPC_FT_REAL8,&vl_HorodateDebut
			,T_IPC_FT_INT4_ARRAY, &pl_numfmc , &vl_nb_Donnees_numfmc
			,T_IPC_FT_REAL8_ARRAY, &pl_horodeb , &vl_nb_Donnees_horodeb
			,T_IPC_FT_REAL8_ARRAY, &pl_horofin , &vl_nb_Donnees_horofin
			,T_IPC_FT_INT4_ARRAY, &pl_prdeb , &vl_nb_Donnees_prdeb
			,T_IPC_FT_INT4_ARRAY, &pl_prfin , &vl_nb_Donnees_prfin
			,T_IPC_FT_INT2_ARRAY, &pl_niv , &vl_nb_Donnees_niv, NULL) )
      {
      if ( ! (  ( vl_nb_Donnees_horodeb == vl_nb_Donnees_horofin )
              &&( vl_nb_Donnees_prdeb ==  vl_nb_Donnees_horodeb )
              &&( vl_nb_Donnees_prdeb ==  vl_nb_Donnees_prfin ) ) )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSCO_Recup_donnees_INRETS_Bouchons: Le nombre de donnees n'est pas coherent entre elles");
         return;
         }
      }
   else
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSCO_Recup_donnees_INRETS_Bouchons: Impossible de recuperer le message");
      return;
      }

   /*A  DesArmement Callback sur Reception  */
   if ( TutCbDestroy( vm_callback[vl_fenetre-1] ) == FALSE )
      {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING,
				   "Desarmement de la Callback sur %s non effectuee. \n " ,XDM_IDONNEES_BOUCHONS);
      return;
      }

   vm_callback[vl_fenetre-1]=NULL;

   /*A Allocation pour le tableau de donnees */
   vm_donnees_inrets_Bouchons[vl_fenetre-1] =
      (XDY_Octet *) malloc( sizeof(XDY_Octet)*(6*60+1)*((vm_distance[vl_fenetre-1]/vm_precision[vl_fenetre-1])+1)+1);

   memset( vm_donnees_inrets_Bouchons[vl_fenetre-1] , 0 , sizeof(XDY_Octet)*(6*60+1)*((vm_distance[vl_fenetre-1]/vm_precision[vl_fenetre-1])+1)+1 );
   /*A Recomposition du tableau dans l'ordre du tableau des HORODATE */
   b=0;
   vl_compteur_bouchon=0;
for ( i = 0; i <  vl_nb_Donnees_horodeb; i++)
{
	XZSM_11ConversionHorodate( (XDY_Horodate) pl_horodeb[i], &vl_horodated);
      	XZSM_11ConversionHorodate( (XDY_Horodate) pl_horofin[i], &vl_horodatef);
      	XZST_03EcritureTrace( XZSTC_DEBUG1 , "BOUCH: i=%d, num:%d, d:%s = %f ,f:%s = %f ,%d,%d, %d",
	i,(XDY_PR) pl_numfmc[i], vl_horodated, (XDY_Horodate) pl_horodeb[i],vl_horodatef,(XDY_Horodate) pl_horofin[i], (XDY_PR) pl_prdeb[i],(XDY_PR) pl_prfin[i],pl_niv[i] );
}

	pl_horodeb_f = &TabHorodeb[0];
	pl_horofin_f = &TabHorofin[0];
	pl_prdeb_f = &Tabprdeb[0];
	pl_prfin_f = &Tabprfin[0];
	pl_niv_f = &Tabplniv[0];
	for ( i = 0; i <  vl_nb_Donnees_horodeb; i++)
	{
		if (pl_numfmc[i+1] == pl_numfmc[i])
			{
			vl_modulo = (int) ((pl_horofin[i] - pl_horodeb[i]) / 60);
				if (vl_modulo == 0) /*pas de valeurs intermediaires a rajouter */
				{
				pl_horodeb_f[b] = pl_horodeb[i];
				pl_horofin_f[b] =  pl_horofin[i];
				pl_prdeb_f[b] = pl_prdeb[i];
				pl_prfin_f[b] = pl_prfin[i];
				pl_niv_f[b] = pl_niv[i];
				XZST_03EcritureTrace( XZSTC_DEBUG1, "fmc=%d, i=%d, b=%d,d= %f; f= %f; PRd = %d; PRf = %d; niv= %d",
			(XDY_PR) pl_numfmc[i], i,b,(XDY_Horodate) pl_horodeb_f[b], (XDY_Horodate) pl_horofin_f[b], (XDY_PR) pl_prdeb_f[b], (XDY_PR) pl_prfin_f[b], (XDY_PR) pl_niv_f[b]);
					b = b +1;
			}
				else
				{
					for (a = 0; a < (vl_modulo+1) ; a ++)
					{
					pl_niv_f[b] = pl_niv[i];
					if (a ==  vl_modulo) /*derniere insertion intermediaire */
					{
						pl_horodeb_f[b] = (pl_horodeb[i] + (a * 60));
						pl_horofin_f[b] = pl_horofin[i];
					}
					else
					{
						pl_horodeb_f[b] = (pl_horodeb[i] + (a * 60));
						pl_horofin_f[b] = (pl_horodeb_f[b] + 60);
					}
					vl_pente_deb = ((pl_prdeb[i+1] - pl_prdeb[i]) / (pl_horofin[i] - pl_horodeb[i] ));
					vl_pente_fin = ((pl_prfin[i+1] - pl_prfin[i]) / (pl_horofin[i] - pl_horodeb[i] ));
					if (b == 0) /*premiere valeur */
					{
						pl_prdeb_f[b] = pl_prdeb[i];
						pl_prfin_f[b] = pl_prfin[i];
					}
					else if (a == 0) /*premiere valeur de chaque i*/
					{
						pl_prdeb_f[b] = pl_prdeb[i];
						pl_prfin_f[b] = pl_prfin[i];
					}
					else /*pour toutes les autres */
					{
						pl_prdeb_f[b] =  pl_prdeb_f[b-1] + vl_pente_deb * 60;
						pl_prfin_f[b] = pl_prfin_f[b-1] + vl_pente_fin * 60;
					}
					XZST_03EcritureTrace( XZSTC_DEBUG1 , "fmc=%d; i=%d, b=%d,d= %f; f= %f; PRd = %d; PRf = %d; niv= %d",
					(XDY_PR) pl_numfmc[i],i,b,(XDY_Horodate) pl_horodeb_f[b], (XDY_Horodate) pl_horofin_f[b], (XDY_PR) pl_prdeb_f[b], (XDY_PR) pl_prfin_f[b], (XDY_PR) pl_niv_f[b]);
					b = b+1;
				}
				}
		}
		else
			{
			pl_horodeb_f[b] = pl_horodeb[i];
			pl_horofin_f[b] =  pl_horofin[i];
			pl_prdeb_f[b] = pl_prdeb[i];
			pl_prfin_f[b] = pl_prfin[i];
			pl_niv_f[b] = pl_niv[i];
			XZST_03EcritureTrace( XZSTC_DEBUG1 , "fmc=%d; i=%d, b=%d,d= %f; f= %f; PRd = %d; PRf = %d; niv= %d",
			(XDY_PR) pl_numfmc[i], i, b,(XDY_Horodate) pl_horodeb_f[b], (XDY_Horodate) pl_horofin_f[b], (XDY_PR) pl_prdeb_f[b], (XDY_PR) pl_prfin_f[b], (XDY_PR) pl_niv_f[b]);
				b = b +1 ;
			}
	}

	for ( i = 0; i <  (b); i++)
	{
		for( h = (int)(pl_horodeb_f[i]-vm_HorodateSecInit[vl_fenetre-1]); h <= (int)(pl_horofin_f[i]-vm_HorodateSecInit[vl_fenetre-1]); h = h +60)
		{
			if ( h >= 0)
				{
					if ( pl_prdeb_f[i] > pl_prfin_f[i] )
						{
							for ( k =  (pl_prfin_f[i]/vm_precision[vl_fenetre-1])-(vm_prdeb[vl_fenetre-1]/vm_precision[vl_fenetre-1]);
									k <= (pl_prdeb_f[i]/vm_precision[vl_fenetre-1])-(vm_prdeb[vl_fenetre-1]/vm_precision[vl_fenetre-1]);
									k=k+1)
							{
								vl_rang=
									(int)h/(60)*((vm_distance[vl_fenetre-1]/vm_precision[vl_fenetre-1])+1)+k;
//								XZST_03EcritureTrace(   XZSTC_DEBUG1," rang=%d ", vl_rang );
//								XZST_03EcritureTrace(   XZSTC_DEBUG1," h=%d k=%d ", h/(360),k );
								if ( ( vl_rang >= 0 ) && ( vl_rang <= 6*60*((vm_distance[vl_fenetre-1]/vm_precision[vl_fenetre-1])+1) ) )
									{
										/* DEM428 */
										if ( pl_niv_f[i] > 4 ) pl_niv_f[i]=5;
											vm_donnees_inrets_Bouchons[vl_fenetre-1][vl_rang]=(XDY_Octet)(3+pl_niv_f[i]);
									}
							}
					}
					else
					{
						for ( k =  (pl_prdeb_f[i]/vm_precision[vl_fenetre-1])-(vm_prfin[vl_fenetre-1]/vm_precision[vl_fenetre-1]);
								k <= (pl_prfin_f[i]/vm_precision[vl_fenetre-1])-(vm_prfin[vl_fenetre-1]/vm_precision[vl_fenetre-1]);
									k=k+1)
							{
								vl_rang=
								(int)h/(60)*((vm_distance[vl_fenetre-1]/vm_precision[vl_fenetre-1])+1)+k;
//								XZST_03EcritureTrace(   XZSTC_DEBUG1," rang=%d " ,vl_rang );
								if ( ( vl_rang >= 0 ) && ( vl_rang <= 6*60*((vm_distance[vl_fenetre-1]/vm_precision[vl_fenetre-1])+1) ) )
									{
										/* DEM428 */
										if ( pl_niv_f[i] > 4 ) pl_niv_f[i]=5;
										vm_donnees_inrets_Bouchons[vl_fenetre-1][vl_rang]=(XDY_Octet)(3+pl_niv_f[i]);
									}
							}
					}
			}
		}
	}

	/*A Affichage des donnees recues */
   if ( vm_type_courbe[vl_fenetre-1] == CM_INRETS_BOUCH )
      MSCO_Affichage_donnees_inrets_avec_bouchons( vm_donnees_inrets_PR[vl_fenetre-1], vm_donnees_inrets_TRA[vl_fenetre-1],vm_donnees_inrets_Bouchons[vl_fenetre-1], 359,vl_fenetre);
   else
      MSCO_Affichage_donnees_inrets_Bouchons(vm_donnees_inrets_Bouchons[vl_fenetre-1],359,0,vl_fenetre);

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Recup_donnees_INRETS_Bouchons" );
   return ;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Recuperation des donnees pour les courbes a partir de la bd
   pour ft.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSCO_Recup_donnees_INRETS_PR(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_arg)

/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une callback
*  pa_caract : Caracteristiques de la courbe.
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
*   Creation de la table de hash
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_inrets.c	1.23 08/18/08 : MSCO_Recup_donnees_INRETS_PR " ;
#ifdef _TIBCO_EMS
 XDY_Mot 	vl_Autoroute;
 XDY_Entier 	vl_prdeb;
 XDY_Entier 	vl_prfin;
 XDY_Horodate 	vl_HorodateDebut;
 XDY_Entier 	pl_numero[512];
 XDY_Entier 	pl_prdeb[512];
 XDY_Entier 	pl_prfin[512];
 XDY_Entier  	vl_nb_Donnees_numero,
         	vl_nb_Donnees_prdeb,vl_nb_Donnees_prfin;
#else
 T_INT2 vl_Autoroute;
 T_INT4 vl_prdeb;
 T_INT4 vl_prfin;
 T_REAL8 vl_HorodateDebut;
 T_INT4 *pl_numero;
 T_INT4 *pl_prdeb;
 T_INT4 *pl_prfin;
 T_INT4  vl_nb_Donnees_numero,
         vl_nb_Donnees_prdeb,vl_nb_Donnees_prfin;
#endif
 int i=0,k=0;
   XZSCT_Datagroup vl_datagroup_init ;
   T_BOOL vl_etat=0;
 XDY_Octet vl_fenetre;
 char vl_chaine_fenetre[10];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Recup_donnees_INRETS_PR" );

   vl_fenetre = (XDY_Octet) pa_arg;
   /*A RECUPERE LES DONNEES et bufferiser*/
   /* Recuperation des donnees du message */
   if (TipcMsgRead(pa_Data->msg
			,T_IPC_FT_INT2,&vl_Autoroute
			,T_IPC_FT_INT4,&vl_prdeb
			,T_IPC_FT_INT4,&vl_prfin
			,T_IPC_FT_REAL8,&vl_HorodateDebut
			,T_IPC_FT_INT4_ARRAY, &pl_numero , &vl_nb_Donnees_numero
			,T_IPC_FT_INT4_ARRAY, &pl_prdeb , &vl_nb_Donnees_prdeb
			,T_IPC_FT_INT4_ARRAY, &pl_prfin , &vl_nb_Donnees_prfin, NULL) )
      {
      if ( ! (  ( vl_nb_Donnees_numero == vl_nb_Donnees_prdeb )
              &&( vl_nb_Donnees_prdeb ==  vl_nb_Donnees_prfin ) ) )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSCO_Recup_donnees_INRETS: Le nombre de donnees n'est pas coherent entre elles");
         return;
         }
      }
   else
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSCO_Recup_donnees_INRETS: Impossible de recuperer le message");
      return;
      }

   /*A  DesArmement Callback sur Reception  */
   if ( TutCbDestroy( vm_callback_pr[vl_fenetre-1] ) == FALSE )
      {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING,
				   "Desarmement de la Callback sur %s non effectuee. \n " ,XDM_IDONNEES_INRETS_PR);
      return;
      }

   vm_callback_pr[vl_fenetre-1]=NULL;

   /*A Allocation pour le tableau de donnees */
   if ( vl_nb_Donnees_numero > 0 )
      {
      vm_donnees_inrets_PR[vl_fenetre-1] = (XZIST_Ligne_INRETS_PR *) malloc( sizeof(XZIST_Ligne_INRETS_PR)* vl_nb_Donnees_numero);
      memset( vm_donnees_inrets_PR[vl_fenetre-1] , 0 , sizeof(XZIST_Ligne_INRETS_PR)*vl_nb_Donnees_numero );
      }
   else
      vm_donnees_inrets_PR[vl_fenetre-1] = (XZIST_Ligne_INRETS_PR *) NULL;


   /*A Recomposition du tableau dans l'ordre du tableau des numero de trafic */
   for ( i = 0; i < vl_nb_Donnees_numero ; i++)
      {
      XZST_03EcritureTrace( XZSTC_DEBUG1 , "PR: %d,%d,%d", pl_numero[i],pl_prdeb[i],pl_prfin[i] );

      vm_donnees_inrets_PR[vl_fenetre-1][i].numero = (XDY_Entier) pl_numero[i];
      if ( pl_prdeb[i] > pl_prfin[i] )
         {
         vm_donnees_inrets_PR[vl_fenetre-1][i].PRDebut = (XDY_Entier) pl_prfin[i];
         vm_donnees_inrets_PR[vl_fenetre-1][i].PRFin = (XDY_Entier) pl_prdeb[i];
         }
      else
         {
         vm_donnees_inrets_PR[vl_fenetre-1][i].PRDebut = (XDY_Entier) pl_prdeb[i];
         vm_donnees_inrets_PR[vl_fenetre-1][i].PRFin = (XDY_Entier) pl_prfin[i];
         }
      }

   /* Déclenchement Abo Bouchons*/
   if ( vm_type_courbe[vl_fenetre-1] == CM_INRETS_BOUCH )
      {
         sprintf( vl_chaine_fenetre , "%d" , vl_fenetre );
         XDG_EncodeDG3(vl_datagroup_init , XDG_IDONNEES_BOUCHONS , vl_chaine_fenetre , vm_NomMachine );
         /*A Abonnement XDG_IDONNEES_BOUCHONS+vm_NomMachine pour le callback si ce n'est deja fait */
         TipcSrvDgGetRecv( vl_datagroup_init , &vl_etat );
         if ( vl_etat == FALSE )
            {
            if (! XDG_JoindreDG(vl_datagroup_init) )
               {
               /* Ecriture Trace */
               XZST_03EcritureTrace( XZSTC_WARNING,
				   "Abonnement sur %s non effectuee. \n " ,vl_datagroup_init);
               return ;
               }
            }
         /*A  Armement Callback sur Reception XDM_IDONNEES_BOUCHONS */
         if ( (vm_callback[vl_fenetre-1] = TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IDONNEES_BOUCHONS ),
	   			   MSCO_Recup_donnees_INRETS_Bouchons,(T_CB_ARG) vl_fenetre )) == NULL)
            {
            /* Ecriture Trace */
            XZST_03EcritureTrace( XZSTC_WARNING,
				      "Callback sur %s non declenchee. \n " , XDM_IDONNEES_BOUCHONS);
            return ;
            }


         /*A Appel a XZAS... pour demander la recuperation des donnees des courbes */
     if ( XZAS03_Bouchon( vl_datagroup_init , (XDY_Autoroute) vl_Autoroute , vm_prdeb[vl_fenetre-1], vm_prfin[vl_fenetre-1], vm_HorodateSecInit[vl_fenetre-1] ) != XDC_OK )
           {
            /* Ecriture Trace */
            XZST_03EcritureTrace( XZSTC_WARNING,
				      "Probleme a l'appel de XZAS03_Bouchon");
            return ;
            }

       }


   if ( vm_type_courbe[vl_fenetre-1] != CM_INRETS_BOUCH )
      {
      /*A Affichage des donnees recues */
      MSCO_Affichage_donnees_inrets( vm_donnees_inrets_PR[vl_fenetre-1], vm_donnees_inrets_TRA[vl_fenetre-1], vl_nb_Donnees_numero ,vl_fenetre);
      }

   vm_nb_Donnees[vl_fenetre-1]=(int)vl_nb_Donnees_numero;
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Recup_donnees_INRETS_PR" );
   return ;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Recuperation des donnees pour les courbes a partir de la bd
   pour ft.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSCO_Recup_donnees_INRETS_TRA(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_arg)

/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une callback
*  pa_caract : Caracteristiques de la courbe.
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
*   Creation de la table de hash
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco_inrets.c	1.23 08/18/08 : MSCO_Recup_donnees_INRETS_TRA " ;
#ifdef _TIBCO_EMS
 XDY_Mot 	vl_Autoroute;
 XDY_Entier 	vl_prdeb;
 XDY_Entier 	vl_prfin;
 XDY_Horodate 	vl_HorodateDebut;
 XDY_Entier 	pl_numero[512];
 XDY_Horodate 	pl_horo[512];
 XDY_Mot 	pl_niveau[512];
 XDY_Entier  	vl_nb_Donnees_numero,
         	vl_nb_Donnees_horo,vl_nb_Donnees_niveau;
#else
 T_INT2 vl_Autoroute;
 T_INT4 vl_prdeb;
 T_INT4 vl_prfin;
 T_REAL8 vl_HorodateDebut;
 T_INT4 *pl_numero;
 T_REAL8 *pl_horo;
 T_INT2 *pl_niveau;
 T_INT4  vl_nb_Donnees_numero,
         vl_nb_Donnees_horo,vl_nb_Donnees_niveau;
#endif
   int i=0,k=0,j=0,vl_ind_donnees=0;

   XZSCT_Datagroup vl_datagroup_init ;
   T_BOOL vl_etat=0;
   XDY_Octet vl_fenetre;
 char vl_chaine_fenetre[10];
 XDY_Horodate vl_horo=0.0;
   XZSMT_Horodate vl_horodate;


   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Recup_donnees_INRETS_TRA" );

   vl_fenetre = (XDY_Octet) pa_arg;
   /*A RECUPERE LES DONNEES et bufferiser*/

   /* Recuperation des donnees du message */
   if (TipcMsgRead(pa_Data->msg
			,T_IPC_FT_INT2,&vl_Autoroute
			,T_IPC_FT_INT4,&vl_prdeb
			,T_IPC_FT_INT4,&vl_prfin
			,T_IPC_FT_REAL8,&vl_HorodateDebut
			,T_IPC_FT_INT4_ARRAY, &pl_numero , &vl_nb_Donnees_numero
			,T_IPC_FT_REAL8_ARRAY, &pl_horo , &vl_nb_Donnees_horo
			,T_IPC_FT_INT2_ARRAY, &pl_niveau , &vl_nb_Donnees_niveau, NULL) )
      {
      if ( ! (  ( vl_nb_Donnees_numero == vl_nb_Donnees_horo )
              &&( vl_nb_Donnees_horo ==  vl_nb_Donnees_niveau ) ) )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSCO_Recup_donnees_INRETS_TRA: Le nombre de donnees n'est pas coherent entre elles");
         return;
         }
      }
   else
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSCO_Recup_donnees_INRETS_TRA: Impossible de recuperer le message");
      return;
      }

   /*A  DesArmement Callback sur Reception  */
   if ( TutCbDestroy( vm_callback_tra[vl_fenetre-1] ) == FALSE )
      {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING,
				   "Desarmement de la Callback sur %s non effectuee. \n " ,XDM_IDONNEES_INRETS_TRA);
      return;
      }

   vm_callback_tra[vl_fenetre-1]=NULL;

   /*A Allocation pour le tableau de donnees */
   if ( vl_nb_Donnees_numero > 0 )
      {
      vm_donnees_inrets_TRA[vl_fenetre-1] = (XZIST_Ligne_INRETS_Trafic *) malloc( sizeof(XZIST_Ligne_INRETS_Trafic)* vl_nb_Donnees_numero);
      memset( vm_donnees_inrets_TRA[vl_fenetre-1] , 0 , sizeof(XZIST_Ligne_INRETS_Trafic)* vl_nb_Donnees_numero);
      }
  else
      {
      vm_donnees_inrets_TRA[vl_fenetre-1] = (XZIST_Ligne_INRETS_Trafic *) NULL;
      }



   /*A Recomposition du tableau ordonne sur l'horodate en comblant les trous  */
   for ( i = 0 ; i < vl_nb_Donnees_numero ; i++ )
      {

      XZSM_11ConversionHorodate( (XDY_Horodate) pl_horo[i], &vl_horodate);
      XZST_03EcritureTrace( XZSTC_DEBUG1 , "TRA: %d,%s,%d", pl_numero[i], vl_horodate,pl_niveau[i] );

      vm_donnees_inrets_TRA[vl_fenetre-1][i].numero = (XDY_Entier) pl_numero[i];
      vm_donnees_inrets_TRA[vl_fenetre-1][i].Horodate = (XDY_Horodate) pl_horo[i];
      vm_donnees_inrets_TRA[vl_fenetre-1][i].Niveau = (XDY_Octet) pl_niveau[i];
      }

   vm_taille_tra[vl_fenetre-1] = vl_nb_Donnees_numero;
   vm_HorodateSecDeb[vl_fenetre-1]=vl_HorodateDebut;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Recup_donnees_INRETS_TRA" );
   return ;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Affichage_donnees_inrets(
          pa_donnees_inrets_PR,
          pa_donnees_inrets_TRA,
          va_taille,
          va_fenetre)

          XZIST_Ligne_INRETS_PR *pa_donnees_inrets_PR;
          XZIST_Ligne_INRETS_Trafic *pa_donnees_inrets_TRA;
          int va_taille;
          XDY_Octet va_fenetre;

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
{static char *version = "@(#)isyn_sco_inrets.c	1.23 08/18/08 : Affichage_donnees_inrets" ;
   int i,j;
   XDY_Horodate vl_horoprec=0;
   XZSMT_Horodate vl_horodate;
   int vl_pr_deb, vl_pr_fin,vl_valeur, vl_x;


   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Affichage_donnees_inrets va_taille:%d", va_taille );

   vl_horoprec=vm_HorodateSecDeb[va_fenetre-1];

   if ( vm_taille_tra[va_fenetre-1] > 0 )
   {

   vl_horoprec=pa_donnees_inrets_TRA[0].Horodate;
   vl_x = 1;
   /* affichage des donnees */
   for ( i = 0; i < vm_taille_tra[va_fenetre-1] ; i++)
      {
      while ( pa_donnees_inrets_TRA[i].Horodate > vl_horoprec )
         {
         vl_horoprec=vl_horoprec+(60.0*vm_rapport);
         vl_x = vl_x + 1;
         }

      vl_pr_deb = vl_pr_fin = vl_valeur = 0;


      if (pa_donnees_inrets_PR[pa_donnees_inrets_TRA[i].numero-1].PRDebut < vm_prdeb[va_fenetre-1])
    	  vl_pr_deb = 1;
      else
    	  vl_pr_deb = ((pa_donnees_inrets_PR[pa_donnees_inrets_TRA[i].numero-1].PRDebut - vm_prdeb[va_fenetre-1]) / vm_precision[va_fenetre-1])+1;

      if (pa_donnees_inrets_PR[pa_donnees_inrets_TRA[i].numero-1].PRFin > vm_prfin[va_fenetre-1])
    	  vl_pr_fin = (vm_distance[va_fenetre-1]/vm_precision[va_fenetre-1])+1;
      else
    	  vl_pr_fin = ((pa_donnees_inrets_PR[pa_donnees_inrets_TRA[i].numero-1].PRFin - vm_prdeb[va_fenetre-1]) / vm_precision[va_fenetre-1])+1;

      vl_valeur = pa_donnees_inrets_TRA[i].Niveau;

      sprintf(vl_msg,"%d|%d|%c|%d!%d!%d!%d",CMD_FLASH_DONNEES_COURBE, CM_COURBE_INRETS, c_correspFenetre[va_fenetre-1], vl_x,vl_pr_deb, vl_pr_fin, vl_valeur);
		if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
			return ;
		}

      }
   	   	 XZSM_11ConversionHorodate( vl_horoprec, &vl_horodate);
         XZST_03EcritureTrace(   XZSTC_DEBUG3 ," horo %s ", vl_horodate );
   }

   // Dans le cas d'une courbe carto/densite c'est l'affichage Bouchon qui signale la fin
   if (vm_type_courbe[va_fenetre-1] != CM_INRETS_BOUCH)
	   MSCO00_Signaler_Fin_Init(va_fenetre-1, CM_COURBE_INRETS);

XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Affichage_donnees_inrets" );
   return XDC_OK;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Affichage_donnees_inrets_avec_bouchons(
          pa_donnees_inrets_PR,
          pa_donnees_inrets_TRA,
          pa_donnees_inrets_Bouchons,
          va_taille,
          va_fenetre)

          XZIST_Ligne_INRETS_PR *pa_donnees_inrets_PR;
          XZIST_Ligne_INRETS_Trafic *pa_donnees_inrets_TRA;
          XDY_Octet *pa_donnees_inrets_Bouchons;
          int va_taille;
          XDY_Octet va_fenetre;

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
{static char *version = "@(#)isyn_sco_inrets.c	1.23 08/18/08 : Affichage_donnees_inrets_avec_bouchons" ;
   int i,j,k;
   XDY_Horodate vl_horoprec=0;
   int vl_tic=0;
   int vl_pr_deb, vl_pr_fin,vl_valeur, vl_x;

 XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Affichage_donnees_inrets_avec_bouchons va_taille:%d", va_taille );


 if ( vm_taille_tra[va_fenetre-1] > 0 )
   {
	 MSCO_Affichage_donnees_inrets(pa_donnees_inrets_PR,pa_donnees_inrets_TRA,va_taille, va_fenetre);
	 MSCO_Affichage_donnees_inrets_Bouchons(pa_donnees_inrets_Bouchons, va_taille, 3, va_fenetre); // decalage à 0 remplacé par 3
   }
 else
   {
   MSCO_Affichage_donnees_inrets_Bouchons( pa_donnees_inrets_Bouchons, 359, 0 , va_fenetre ); // decalage à 3 remplacé par 0
   }


 XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Affichage_donnees_inrets_avec_bouchons" );
   return XDC_OK;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Affichage_donnees_inrets_Bouchons(
          pa_donnees_inrets_Bouchons,
          va_taille,
          va_decalage,
          va_fenetre)


          XDY_Octet * pa_donnees_inrets_Bouchons;
          int va_taille;
          int va_decalage;
          XDY_Octet va_fenetre;

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
{static char *version = "@(#)isyn_sco_inrets.c	1.23 08/18/08 : Affichage_donnees_inrets_Bouchons" ;
   XDY_Octet vl_valeur;
   int i,j;
   XDY_Horodate vl_horoprec=0;
   int vl_deb, vl_fin;


   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Affichage_donnees_inrets_Bouchons va_taille:%d", va_taille);

   vl_horoprec=vm_HorodateSecInit[va_fenetre-1];

   /*A Traitement des donnees recues */
   for ( i = 0; i < va_taille; i++)
   {/*Boucle sur les horodates*/
	   vl_deb = vl_fin = vl_valeur = 0;
	   for ( j=0; j <= (vm_distance[va_fenetre-1]/vm_precision[va_fenetre-1]) ; j++ )
	   {/* Boucle sur les PR */
		   if (vl_deb == 0)
		   {
			   if (pa_donnees_inrets_Bouchons[i*((vm_distance[va_fenetre-1]/vm_precision[va_fenetre-1])+1)+j] >= 3){
				   vl_valeur = pa_donnees_inrets_Bouchons[i*((vm_distance[va_fenetre-1]/vm_precision[va_fenetre-1])+1)+j]-3+va_decalage;
				   vl_deb = j;
			   }
		   }
		   else if (vl_fin == 0)
			   if (pa_donnees_inrets_Bouchons[i*((vm_distance[va_fenetre-1]/vm_precision[va_fenetre-1])+1)+j]-3+va_decalage != vl_valeur)
				   vl_fin = j-1;

	   	   if (vl_fin > 0)
	   	   {
	            // Envoi donnee
	            sprintf(vl_msg, "%d|%d|%c|%d!%d!%d!%d", CMD_FLASH_DONNEES_COURBE, CM_COURBE_INRETS, c_correspFenetre[va_fenetre-1],i+1, vl_deb+1, vl_fin+1, vl_valeur );
				if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
				{
					XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
					return ;
				}
				vl_fin = 0;
				vl_deb = 0;
				vl_valeur = 0;
			    if (pa_donnees_inrets_Bouchons[i*((vm_distance[va_fenetre-1]/vm_precision[va_fenetre-1])+1)+j] >= 3)
			    {
				   vl_valeur = pa_donnees_inrets_Bouchons[i*((vm_distance[va_fenetre-1]/vm_precision[va_fenetre-1])+1)+j]-3+va_decalage;
				   vl_deb = j;
			    }
	   	   }

	   }
   	   if (vl_deb > 0)
   	   {
            // Envoi donnee
            sprintf(vl_msg, "%d|%d|%c|%d!%d!%d!%d", CMD_FLASH_DONNEES_COURBE, CM_COURBE_INRETS, c_correspFenetre[va_fenetre-1],i+1, vl_deb+1, j, vl_valeur );
			if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
			{
				XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
				return ;
			}
			vl_fin = 0;
			vl_deb = 0;
			vl_valeur = 0;
   	   }
   }

   MSCO00_Signaler_Fin_Init(va_fenetre-1, CM_COURBE_INRETS);

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Affichage_donnees_inrets_Bouchons" );
   return XDC_OK;
}
