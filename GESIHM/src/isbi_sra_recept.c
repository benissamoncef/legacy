/*E*/
/*Fichier :  $Id: isbi_sra_recept.c,v 1.8 2020/11/17 15:44:24 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2020/11/17 15:44:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE MSRA * FICHIER isbi_sra_recept.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Creation et Mise a jour des Variables d'animations 
*   graphiques : Reception des messages de raf 
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	12 Sep 1994	: Creation
* VR	29/05/12 : PRV DEM 1016
* JMG	15/05/013 : projeter
* JMG   20/03/17       SAGA DEM1191
* JMG	08/06/20 : EMS 1.7 
* GR    10/07/20 : EMS 1.8
* JMG	10/06/21 : tisse brique API 1.9
* LCL	20/12/21 : MOVIS P2 ems SAE-312
* GGY	24/11/23 : Ajout MSRA16_raf_vag_PIC (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

#include "isbi_sra_recept.h"
#include "isbi_sco.h"
#include "isbi_sra.h"
#include "isbi_sop.h"
#include "xzis.h"

/* definitions de constantes */
#define CM_NB_VOIES   10
#define CM_DECAL_NIV (0.1)
#define CM_NB_NIV 6


/* definition de macro locales */

/* declaration de variables locales */
static int Fin_Init_Done = 0;
static T_CB vm_callback_finInit;

#ifndef _TIBCO_EMS
T_STR	vm_sender;
T_STR	vm_dest;
#else
XDY_Nom vm_sender;
XDY_Nom vm_dest;
#endif


static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : isbi_sra_recept" ;

extern XZSCT_NomSite vm_NomSite;
extern int vm_TypeMachine;
extern XZSCT_NomMachine vm_NomMachine;
extern XDY_Booleen vg_pas_tisse;

#include "isbi_svu_hashtypes.h"

/* declaration de fonctions internes */
void MSRA_Recept_Etat_Alerte();
void MSRA_Recept_Etat_Alerte_Localisee();
void MSRA_Recept_Etat_Eqt();
void MSRA_Recept_Alarme();
void MSRA_Recept_Bouchon();
void MSRA_Recept_Bouchon_Voie();
void MSRA_Recept_voie_neutralisee();
void MSRA_Recept_voie_radt_inverse();
void MSRA_Recept_basculement();
void MSRA_Recept_objet_mobile();
void MSRA_Recept_objet_mobile_echang();
void MSRA_Recept_aff_voies();
void MSRA_Recept_RazMob();
void MSRA_Recept_strada();
void MSRA_Reinit();
void MSRA_Debut();
void MSRA_Fin();
void MSRA_Recept_PMVDATEX();
void MSRA_Recept_Masque();
void MSRA_Recept_Masque_Projeter();
void MSRA_Recept_Vue();
void MSRA_Recept_Graphe();
void MSRA_Recept_PlanSecours();
void MSRA_init_POMagistra();

/* definition de fonctions externes */
extern void MSRA01_raf_vag_PMV_Message(); 
extern void MSRA02_raf_vag_PMV_Picto();
extern void MSRA03_raf_vag_TRA();
extern void MSRA03_raf_vag_DIV();
extern void MSRA03_raf_vag_NivTRA();
extern void MSRA04_raf_vag_NAV();
extern void MSRA05_raf_vag_DAI();
extern void MSRA06_raf_vag_PAU();
extern void MSRA07_raf_vag_CAMERA();
extern void MSRA10_raf_vag_FAV();
extern void MSRA11_raf_vag_Panneaux();
extern void MSRA17_raf_vag_EqtSigGTCRenovee();
extern void MSRA12_raf_vag_PMVA();
extern void MSRA13_raf_vag_BAF();
extern void MSRA14_raf_vag_BAD();
extern void MSRA15_raf_vag_PRV();
extern void MSRA16_raf_vag_PIC();
extern void MSRA_raf_journuit();
extern void MSRA_raf_SeqTunnel();
extern void MSRA_raf_SeqEch();
extern void MSRA_raf_SeqGTCRenovee();
extern int MSRA14_raf_vag_DIS();
extern int MSRA18_raf_zones_mobiles();
extern int MSRA18_raf_objets_mobiles();
extern int MSRA18_raf_objets_mobiles_echang();
extern int MSRA_raf_Aff_Voies_Prep();
extern int MSRA_Detruire_mobiles();
extern void MSOP_Affiche_elt_fop();
extern void MSVU_RazEtatEqt();
extern int  MSRA_Maj_mobiles();
extern void MSRA09_raf_STRADA();
extern void MSRA01_raf_vag_PMVDATEX_Message();

extern int MSCO01_demarer_courbe_ft();
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Init de la reception des message de rafraichissement 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSRA_Init_Recept()
          
     
/*
* ARGUMENTS EN ENTREE :
*  aucun
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Init_Recept" ;
   XZSCT_Datagroup vl_Datagroup;
   char vl_message[10];
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Init_Recept *");
   
   /*A Abonnements aux datagroups de rafraichissement*/
   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IPOSIT_INDIC_ALARME");
   XDG_EncodeDG2(vl_Datagroup , XDG_IPOSIT_INDIC_ALARME , vm_NomSite );
   if (! XDG_JoindreDG(vl_Datagroup) )
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
      return XDC_NOK;
      }	

    XZST_03EcritureTrace( XZSTC_WARNING,"MSRA_Init_Recept: Abonnement XDG_IETAT_DISPO_EQT %s", vm_NomSite);
    XDG_EncodeDG2(vl_Datagroup , XDG_IETAT_DISPO_EQT , vm_NomSite );
    if (! XDG_JoindreDG(vl_Datagroup) )
         {	
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING, 
				   "mon Abonnement sur %s est non effectuee. \n " ,vl_Datagroup);
         return XDC_NOK;
         }	
    if (!vg_pas_tisse)
    {
    	XZST_03EcritureTrace( XZSTC_WARNING,"MSRA_Init_Recept: Abonnement XDG_API %s", vm_NomSite);
    	XDG_EncodeDG2(vl_Datagroup , XDG_API , vm_NomSite );
    	if (! XDG_JoindreDG(vl_Datagroup) )
        {	
        	 /* Ecriture Trace */
         	XZST_03EcritureTrace( XZSTC_WARNING, 
				   "mon Abonnement sur %s est non effectuee. \n " ,vl_Datagroup);
         	return XDC_NOK;
         }

	 /* on indique le debut de l'initialisation a Magistra */
        strcpy(vl_message, "|0|1");
        TipcSrvMsgWrite(vl_Datagroup,
                XDM_IdentMsg(XDM_API_SYNOS),
                XDM_FLG_SRVMSGWRITE,
                T_IPC_FT_STR, vl_message,
                T_IPC_FT_STR, vm_NomSite,
                NULL);

    }

   /*A Abonnements aux datagroups de rafraichissement*/
   if ( !strcmp( vm_NomSite , XDC_NOM_SITE_CI )  )
      {
      XDG_EncodeDG2(vl_Datagroup , XDG_IETAT_DISPO_EQT , XDC_NOM_SITE_VC );
      if (! XDG_JoindreDG(vl_Datagroup) )
         {	
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s toujours non effectuee. \n " ,vl_Datagroup);
         return XDC_NOK;
         }	
      XDG_EncodeDG2(vl_Datagroup , XDG_IETAT_DISPO_EQT , XDC_NOM_SITE_DP );
      if (! XDG_JoindreDG(vl_Datagroup) )
         {	
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee2. \n " ,vl_Datagroup);
         return XDC_NOK;
         }	
      XDG_EncodeDG2(vl_Datagroup , XDG_IETAT_DISPO_EQT , XDC_NOM_SITE_CA );
      if (! XDG_JoindreDG(vl_Datagroup) )
         {	
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee4. \n " ,vl_Datagroup);
         return XDC_NOK;
         }	
      }

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IETAT_INIEQT");
   XDG_EncodeDG2( vl_Datagroup , XDG_IETAT_INIEQT , vm_NomMachine );
   if (! XDG_JoindreDG(vl_Datagroup) )
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
      return XDC_NOK;
      }	
    
/*   if ( strcmp( vm_NomSite , XDC_NOM_SITE_CI ) != NULL )*/
      {
      XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_ICREER_ALERTE");
      XDG_EncodeDG2(vl_Datagroup , XDG_ICREER_ALERTE , vm_NomSite );
      if (! XDG_JoindreDG(vl_Datagroup) )
         {	
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
         return XDC_NOK;
         }
      }	

      XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Recept_Etat_Alerte_Localisee");
      if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_ICREER_ALERTE_LOCALISEE ),
                                 MSRA_Recept_Etat_Alerte_Localisee,(T_CB_ARG) NULL ) == NULL)
         {
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING,
                                   "Callback sur %s non declenchee. \n " ,XDM_IETAT_DISPO_EQT  );
         return XDC_NOK;
        }

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IRAF_BOUCHON_CHAUSSEE_PORTION");
      XDG_JoindreDG(XDG_IRAF_BOUCHON_CHAUSSEE_PORTION );
   if ( vm_TypeMachine == XZSCC_TYPEM_PCS )
      strcpy(vl_Datagroup , XDG_IRAF_BOUCHON_CHAUSSEE_PORTION );
  /* else
      if ( !strcmp( vm_NomSite , XDC_NOM_SITE_CI ) )
      {
         strcpy(vl_Datagroup , XDG_IRAF_BOUCHON_CHAUSSEE_PORTION );
         if (! XDG_JoindreDG(vl_Datagroup) )
         {	
            XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
             return XDC_NOK;
          }	
         XDG_EncodeDG2(vl_Datagroup , XDG_IRAF_BOUCHON_CHAUSSEE_PORTION , vm_NomSite);
      }
   */
   else
      XDG_EncodeDG2(vl_Datagroup , XDG_IRAF_BOUCHON_CHAUSSEE_PORTION , vm_NomSite);
      
   if (! XDG_JoindreDG(vl_Datagroup) )
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
      return XDC_NOK;
      }	


   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IRAF_VOIE_NEUTRALISEE");
      XDG_JoindreDG(XDG_IRAF_VOIE_NEUTRALISEE );
   if ( vm_TypeMachine == XZSCC_TYPEM_PCS )
   	strcpy(vl_Datagroup , XDG_IRAF_VOIE_NEUTRALISEE );
   /*
   else
      if ( !strcmp( vm_NomSite , XDC_NOM_SITE_CI )  )
      {
   	strcpy(vl_Datagroup , XDG_IRAF_VOIE_NEUTRALISEE );
         if (! XDG_JoindreDG(vl_Datagroup) )
         {	
            XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
             return XDC_NOK;
          }	
   	XDG_EncodeDG2(vl_Datagroup , XDG_IRAF_VOIE_NEUTRALISEE , vm_NomSite );
      }
   */
   else
   	XDG_EncodeDG2(vl_Datagroup , XDG_IRAF_VOIE_NEUTRALISEE , vm_NomSite );
   	
   if (! XDG_JoindreDG(vl_Datagroup) )
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
      return XDC_NOK;
      }	

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IRAF_VOIE_RADT_INVERSE");
      XDG_JoindreDG(XDG_IRAF_VOIE_RADT_INVERSE );
   if ( vm_TypeMachine == XZSCC_TYPEM_PCS )
   	strcpy(vl_Datagroup , XDG_IRAF_VOIE_RADT_INVERSE );
   /*
   else
      if ( !strcmp( vm_NomSite , XDC_NOM_SITE_CI )  )
      {
   	strcpy(vl_Datagroup , XDG_IRAF_VOIE_RADT_INVERSE );
         if (! XDG_JoindreDG(vl_Datagroup) )
         {	
            XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
             return XDC_NOK;
          }	
   	XDG_EncodeDG2(vl_Datagroup , XDG_IRAF_VOIE_RADT_INVERSE , vm_NomSite );
      }
   */
   else
   	XDG_EncodeDG2(vl_Datagroup , XDG_IRAF_VOIE_RADT_INVERSE , vm_NomSite );
   if (! XDG_JoindreDG(vl_Datagroup) )
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
      return XDC_NOK;
      }	

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IRAF_BASCULEMENT_BOUCHON_VOIE");
      XDG_JoindreDG(XDG_IRAF_BASCULEMENT_BOUCHON_VOIE );
   if ( vm_TypeMachine == XZSCC_TYPEM_PCS )
   	strcpy(vl_Datagroup , XDG_IRAF_BASCULEMENT_BOUCHON_VOIE );
   /*
   else
      if ( !strcmp( vm_NomSite , XDC_NOM_SITE_CI )  )
      {
   	strcpy(vl_Datagroup , XDG_IRAF_BASCULEMENT_BOUCHON_VOIE );
         if (! XDG_JoindreDG(vl_Datagroup) )
         {	
            XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
             return XDC_NOK;
          }	
   	XDG_EncodeDG2(vl_Datagroup , XDG_IRAF_BASCULEMENT_BOUCHON_VOIE , vm_NomSite );
      }
   */
   else
   	XDG_EncodeDG2(vl_Datagroup , XDG_IRAF_BASCULEMENT_BOUCHON_VOIE , vm_NomSite );
   if (! XDG_JoindreDG(vl_Datagroup) )
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
      return XDC_NOK;
      }	

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IRAF_OBJET_MOBILE_PORTION");
      XDG_JoindreDG(XDG_IRAF_OBJET_MOBILE_PORTION );
   if ( vm_TypeMachine == XZSCC_TYPEM_PCS )
   	strcpy(vl_Datagroup , XDG_IRAF_OBJET_MOBILE_PORTION );
   /*
   else
      if ( !strcmp( vm_NomSite , XDC_NOM_SITE_CI )  )
      {
   	strcpy(vl_Datagroup , XDG_IRAF_OBJET_MOBILE_PORTION );
   	XDG_EncodeDG2(vl_Datagroup , XDG_IRAF_OBJET_MOBILE_PORTION , vm_NomSite );
         if (! XDG_JoindreDG(vl_Datagroup) )
         {	
            XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
             return XDC_NOK;
          }	
   	XDG_EncodeDG2(vl_Datagroup , XDG_IRAF_OBJET_MOBILE_PORTION , vm_NomSite );
      }
   */
   else {
   	XDG_EncodeDG2(vl_Datagroup , XDG_IRAF_OBJET_MOBILE_PORTION , vm_NomSite );
   if (! XDG_JoindreDG(vl_Datagroup) )
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
      return XDC_NOK;
      }	
   }

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IRAF_OBJET_MOBILE_ECHANG");
      XDG_JoindreDG(XDG_IRAF_OBJET_MOBILE_ECHANG );
   if ( vm_TypeMachine == XZSCC_TYPEM_PCS )
   	strcpy(vl_Datagroup , XDG_IRAF_OBJET_MOBILE_ECHANG );
   /*
   else
      if ( !strcmp( vm_NomSite , XDC_NOM_SITE_CI )  )
      {
   	strcpy(vl_Datagroup , XDG_IRAF_OBJET_MOBILE_ECHANG );
         if (! XDG_JoindreDG(vl_Datagroup) )
         {	
            XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
             return XDC_NOK;
          }	
   	XDG_EncodeDG2(vl_Datagroup , XDG_IRAF_OBJET_MOBILE_ECHANG , vm_NomSite );
      }
   */
   else
   	XDG_EncodeDG2(vl_Datagroup , XDG_IRAF_OBJET_MOBILE_ECHANG , vm_NomSite );
   if (! XDG_JoindreDG(vl_Datagroup) )
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
      return XDC_NOK;
      }	

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IRAF_PORT_AFF_VOIES");
      XDG_JoindreDG(XDG_IRAF_PORT_AFF_VOIES );
   if ( vm_TypeMachine == XZSCC_TYPEM_PCS )
   	strcpy(vl_Datagroup , XDG_IRAF_PORT_AFF_VOIES );
   /*
   else
      if ( !strcmp( vm_NomSite , XDC_NOM_SITE_CI )  )
      {
   	strcpy(vl_Datagroup , XDG_IRAF_PORT_AFF_VOIES );
         if (! XDG_JoindreDG(vl_Datagroup) )
         {	
            XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
             return XDC_NOK;
          }	
   	XDG_EncodeDG2(vl_Datagroup , XDG_IRAF_PORT_AFF_VOIES , vm_NomSite );
      }
   */
   else
   	XDG_EncodeDG2(vl_Datagroup , XDG_IRAF_PORT_AFF_VOIES , vm_NomSite );
   if (! XDG_JoindreDG(vl_Datagroup) )
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
      return XDC_NOK;
      }	

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IREI_EQT");
      XDG_JoindreDG(XDG_IREI_EQT );
   XDG_EncodeDG2(vl_Datagroup , XDG_IREI_EQT , vm_NomSite );
   if (! XDG_JoindreDG(vl_Datagroup) )
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
      return XDC_NOK;
      }	
   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IRAZ_MOB");
   XDG_EncodeDG2(vl_Datagroup , XDG_IRAZ_MOB , vm_NomSite );
   if (! XDG_JoindreDG(vl_Datagroup) )
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
      return XDC_NOK;
      }	
   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IDONNEES_TRAFIC_STRADA");
   if (! XDG_JoindreDG(XDG_IDONNEES_TRAFIC_STRADA))
   {
	/* Ecriture Trace */
	XZST_03EcritureTrace( XZSTC_WARNING,
				"Abonnement sur %s non effectuee. \n " ,XDG_IDONNEES_TRAFIC_STRADA);
	return XDC_NOK;
   }
   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IPMV_DATEX");
   if (! XDG_JoindreDG(XDG_IPMV_DATEX))
   {
	/* Ecriture Trace */
	XZST_03EcritureTrace( XZSTC_WARNING,
				"Abonnement sur %s non effectuee. \n " ,XDG_IPMV_DATEX);
	return XDC_NOK;
   }
//   if ( ( strcmp( vm_NomSite , XDC_NOM_SITE_CI ) != NULL ) || ( vm_TypeMachine == XZSCC_TYPEM_PCS ))
//   	{
   	XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IRAF_DEBUT");
   	strcpy(vl_Datagroup , XDG_IRAF_DEBUT );
   	if (! XDG_JoindreDG(vl_Datagroup) )
     	 {	
      	/* Ecriture Trace */
      	XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
      	return XDC_NOK;
      	 }	
   	XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IRAF_FIN");
      XDG_EncodeDG2( vl_Datagroup , XDG_IRAF_FIN , "LX" );
   	if (! XDG_JoindreDG(vl_Datagroup) )
      		{	
      		/* Ecriture Trace */
      		XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
      		return XDC_NOK;
      		}	
//   	}

   /*SDU*/
//      XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IMASQUE");
      XDG_EncodeDG2( vl_Datagroup , XDG_IMASQUE , vm_NomMachine );
      XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement %s", vl_Datagroup);
      if (! XDG_JoindreDG(vl_Datagroup) )
         {
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING,
   				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
         return XDC_NOK;
         }

//      XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement XDG_IVUE");
      XDG_EncodeDG2( vl_Datagroup , XDG_IVUE , vm_NomMachine );
      XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement %s",vl_Datagroup);
      if (! XDG_JoindreDG(vl_Datagroup) )
         {
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING,
   				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
         return XDC_NOK;
         }

      XDG_EncodeDG2( vl_Datagroup , XDG_IGRAPHE , vm_NomMachine );
      XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement %s",vl_Datagroup);
      if (! XDG_JoindreDG(vl_Datagroup) )
         {
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING,
   				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
         return XDC_NOK;
         }
      XDG_EncodeDG2( vl_Datagroup , XDG_IPLANSECOURS , vm_NomMachine );
      XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Abonnement %s", vl_Datagroup);
      if (! XDG_JoindreDG(vl_Datagroup) )
         {
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING,
   				   "Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
         return XDC_NOK;
         }
   /*SDU*/


   /*A Armement des Procedures CallBack */

   if (!vg_pas_tisse)
   {
   	XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Init_POMagistra");
   	if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_INIT_TISSE ),
				 MSRA_init_POMagistra,(T_CB_ARG) NULL ) == NULL) 
      	{	
      		XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Callback sur %s non declenchee. \n " ,XDM_INIT_TISSE  );
      		return XDC_NOK;
      	}	
   }

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Recept_Alarme");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IPOSIT_INDIC_ALARME ),
				 MSRA_Recept_Alarme,(T_CB_ARG) NULL ) == NULL) 
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Callback sur %s non declenchee. \n " ,XDM_IETAT_DISPO_EQT  );
      return XDC_NOK;
      }	
      
   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Recept_Etat_Eqt");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IETAT_DISPO_EQT ),
				 MSRA_Recept_Etat_Eqt,(T_CB_ARG) NULL ) == NULL) 
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Callback sur %s non declenchee. \n " ,XDM_IETAT_DISPO_EQT  );
      return XDC_NOK;
      }	

/*   if ( strcmp( vm_NomSite , XDC_NOM_SITE_CI ) != NULL )*/
      {
      XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Recept_Etat_Alerte");
      if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_ICREER_ALERTE ),
				 MSRA_Recept_Etat_Alerte,(T_CB_ARG) NULL ) == NULL) 
         {	
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Callback sur %s non declenchee. \n " ,XDM_IETAT_DISPO_EQT  );
         return XDC_NOK;
         }
      }	

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Recept_Bouchon");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IRAF_BOUCHON_CHAUSSEE_PORTION ),
				 MSRA_Recept_Bouchon,(T_CB_ARG) NULL ) == NULL) 
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Callback sur %s non declenchee. \n " , XDM_IRAF_BOUCHON_CHAUSSEE_PORTION  );
      return XDC_NOK;
      }	
   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Recept_Bouchon");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IRAF_BOUCHON_CHAUSSEE_VOIE ),
				 MSRA_Recept_Bouchon_Voie,(T_CB_ARG) NULL ) == NULL) 
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Callback sur %s non declenchee. \n " , XDM_IRAF_BOUCHON_CHAUSSEE_VOIE  );
      return XDC_NOK;
      }	

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Recept_voie_neutralisee");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IRAF_VOIE_NEUTRALISEE ),
				 MSRA_Recept_voie_neutralisee,(T_CB_ARG) NULL ) == NULL) 
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Callback sur %s non declenchee. \n " , XDM_IRAF_VOIE_NEUTRALISEE  );
      return XDC_NOK;
      }	

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Recept_voie_radt_inverse");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IRAF_VOIE_RADT_INVERSE ),
				 MSRA_Recept_voie_radt_inverse,(T_CB_ARG) NULL ) == NULL) 
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Callback sur %s non declenchee. \n " , XDM_IRAF_VOIE_RADT_INVERSE  );
      return XDC_NOK;
      }	

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Recept_basculement");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IRAF_BASCULEMENT_BOUCHON_VOIE ),
				 MSRA_Recept_basculement,(T_CB_ARG) NULL ) == NULL) 
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Callback sur %s non declenchee. \n " , XDM_IRAF_BASCULEMENT_BOUCHON_VOIE  );
      return XDC_NOK;
      }	

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Recept_objet_mobile");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IRAF_OBJET_MOBILE_PORTION ),
				 MSRA_Recept_objet_mobile,(T_CB_ARG) NULL ) == NULL) 
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Callback sur %s non declenchee. \n " ,  XDM_IRAF_OBJET_MOBILE_PORTION );
      return XDC_NOK;
      }	

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Recept_objet_mobile_echang");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IRAF_OBJET_MOBILE_ECHANG ),
				 MSRA_Recept_objet_mobile_echang,(T_CB_ARG) NULL ) == NULL) 
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Callback sur %s non declenchee. \n " ,  XDM_IRAF_OBJET_MOBILE_ECHANG );
      return XDC_NOK;
      }	

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Recept_aff_voies");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IRAF_PORT_AFF_VOIES ),
				 MSRA_Recept_aff_voies,(T_CB_ARG) NULL ) == NULL) 
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Callback sur %s non declenchee. \n " , XDM_IRAF_PORT_AFF_VOIES  );
      return XDC_NOK;
      }	

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Recept_RazMob");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IRAZ_MOB ),
				 MSRA_Recept_RazMob,(T_CB_ARG) NULL ) == NULL) 
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Callback sur %s non declenchee. \n " , XDM_IRAZ_MOB  );
      return XDC_NOK;
      }	

   
   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Debut");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IRAF_DEBUT ),
				 MSRA_Debut,(T_CB_ARG) NULL ) == NULL) 
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Callback sur %s non declenchee. \n " , XDM_IRAZ_MOB  );
      return XDC_NOK;
      }	

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Fin");
   if ( (vm_callback_finInit = TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IRAF_FIN ),
				 MSRA_Fin,(T_CB_ARG) NULL )) == NULL)
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Callback sur %s non declenchee. \n " , XDM_IRAZ_MOB  );
      return XDC_NOK;
      }	

   
   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback MSRA_Reinit");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IREI_EQT ),
				 MSRA_Reinit,(T_CB_ARG) NULL ) == NULL) 
      {	
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, 
				   "Callback sur %s non declenchee. \n " , XDM_IREI_EQT );
      return XDC_NOK;
      }	

   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback XDM_IDONNEES_TRAFIC_STRADA");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IDONNEES_TRAFIC_STRADA),
				MSRA_Recept_strada,(T_CB_ARG) NULL ) == NULL)
   {
	/* Ecriture Trace */
	XZST_03EcritureTrace( XZSTC_WARNING,
				"Callback sur %s non declenchee. \n " , XDM_IDONNEES_TRAFIC_STRADA);
        return XDC_NOK;
   }
   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback XDM_IPMV_DATEX");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IPMV_DATEX),
				MSRA_Recept_PMVDATEX,(T_CB_ARG) NULL ) == NULL)
   {
	/* Ecriture Trace */
	XZST_03EcritureTrace( XZSTC_WARNING,
				"Callback sur %s non declenchee. \n " , XDM_IPMV_DATEX);
        return XDC_NOK;
   }
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Init_Recept" );

/*SDU*/
   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback XDM_IMASQUE");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IMASQUE ),
				 MSRA_Recept_Masque,(T_CB_ARG) NULL ) == NULL)
      {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING,
				   "Callback sur %s non declenchee. \n " ,XDM_IMASQUE  );
      return XDC_NOK;
      }
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IMASQUE_PROJETER ),
				 MSRA_Recept_Masque_Projeter,(T_CB_ARG) NULL ) == NULL)
      {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING,
				   "Callback sur %s non declenchee. \n " ,XDM_IMASQUE  );
      return XDC_NOK;
      }
   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback XDM_IVUE");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IVUE ),
				 MSRA_Recept_Vue,(T_CB_ARG) NULL ) == NULL)
      {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING,
				   "Callback sur %s non declenchee. \n " ,XDM_IVUE  );
      return XDC_NOK;
      }
   XZST_03EcritureTrace( XZSTC_DEBUG1,"MSRA_Init_Recept: Armement callback XDM_IGRAPHE");
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IGRAPHE ),
				 MSRA_Recept_Graphe,(T_CB_ARG) NULL ) == NULL)
      {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING,
				   "Callback sur %s non declenchee. \n " ,XDM_IVUE  );
      return XDC_NOK;
      }
   if ( TipcSrvProcessCbCreate(XDM_IdentMsg( XDM_IPLANSECOURS ),
		   	   MSRA_Recept_PlanSecours,(T_CB_ARG) NULL ) == NULL)
      {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING,
				   "Callback sur %s non declenchee. \n " ,XDM_IVUE  );
      return XDC_NOK;
      }

/*SDU*/


   return XDC_OK;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception du message de raf des  mesures strada
*   Callback sur XDM_IDONNEES_TRAFIC_STRADA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_strada(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
          
     
/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Recept_strada" ;
  
#ifdef _TIBCO_EMS
   XDY_Entier	vl_station;
   XDY_Mot	vl_sens;
   XDY_Mot	vl_voie;
   XDY_Horodate	vl_horodate;
   XDY_Entier	vl_Q;
   XDY_Entier	vl_V;
   XDY_Entier	vl_TO;
   XDY_Mot	vl_Q_correct;
   XDY_Mot	vl_V_correct;
   XDY_Mot	vl_TO_correct;
#else
   T_INT4	vl_station;
   T_INT2	vl_sens;
   T_INT2	vl_voie;
   T_REAL8	vl_horodate;
   T_INT4	vl_Q;
   T_INT4	vl_V;
   T_INT4	vl_TO;
   T_INT2	vl_Q_correct;
   T_INT2	vl_V_correct;
   T_INT2	vl_TO_correct;
#endif

   XDY_EMes_RADT	vl_mesure;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Recept_strada");
   
   /*A Recuperation des donnees du message */
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4,&vl_station,
                                T_IPC_FT_INT2,&vl_sens,
                                T_IPC_FT_INT2,&vl_voie, 
				T_IPC_FT_REAL8,&vl_horodate,
				T_IPC_FT_INT4,&vl_Q,
				T_IPC_FT_INT2,&vl_Q_correct,
				T_IPC_FT_INT4,&vl_V,
				T_IPC_FT_INT2,&vl_V_correct,
				T_IPC_FT_INT4,&vl_TO,
				T_IPC_FT_INT2,&vl_TO_correct,NULL) )
      {
      /*A Rafraichissement de la station */
      vl_mesure.Horodate=vl_horodate;
      vl_mesure.Numero=vl_station;
      vl_mesure.Sens=vl_sens;
      vl_mesure.Voie=vl_voie;
      vl_mesure.Mesures.ValQ.Val=vl_Q;
      vl_mesure.Mesures.ValQ.Indic=vl_Q_correct;
      vl_mesure.Mesures.ValV.Val=vl_V;
      vl_mesure.Mesures.ValV.Indic=vl_V_correct;
      vl_mesure.Mesures.ValTO.Val=vl_TO;
      vl_mesure.Mesures.ValTO.Indic=vl_TO_correct;
      MSRA09_raf_STRADA(vl_mesure);
      }
   else 
      {
      XZST_03EcritureTrace(XZSTC_WARNING,
		"MSRA_Recept_strada: Impossible de recuperer le message");
      return;
      }

   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Recept_strada" );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception du message de raf des  PMV DATEX
*   Callback sur XDM_IPMV_DATEX
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_PMVDATEX(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
          
     
/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Recept_" ;
   
#ifndef _TIBCO_EMS
   T_STR  pl_nom;
   T_STR  pl_ligne1;
   T_STR  pl_ligne2;
   T_STR  pl_ligne3;
   T_STR  pl_ligne4;
#else
   XDY_Phrase  pl_nom;
   XDY_Ligne_PMV  pl_ligne1;
   XDY_Ligne_PMV  pl_ligne2;
   XDY_Ligne_PMV  pl_ligne3;
   XDY_Ligne_PMV  pl_ligne4;
#endif

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Recept_PMVDATEX");
   
   /*A Recuperation des donnees du message */
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_STR,&pl_nom,
                                T_IPC_FT_STR,&pl_ligne1,
                                T_IPC_FT_STR,&pl_ligne2,
                                T_IPC_FT_STR,&pl_ligne3,
                                T_IPC_FT_STR,&pl_ligne4,NULL) )
      {
      /*A Rafraichissement du pmv */
      MSRA01_raf_vag_PMVDATEX_Message(pl_nom,pl_ligne1,pl_ligne2,pl_ligne3,pl_ligne4);
      }
   else 
      {
      XZST_03EcritureTrace(XZSTC_WARNING,
		"MSRA_Recept_PMVDATEX: Impossible de recuperer le message");
      return;
      }

   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Recept_PMVDATEX" );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception du message de raf des alarme
*   Callback sur XDM_IPOSIT_INDIC_ALARME
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_Alarme(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
          
     
/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Recept_Alarme" ;
 float vl_un=1;   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Recept_Alarme");
   
   /*A Recuperation des donnees du message */
   if (TipcMsgRead(pa_Data->msg, NULL) )
      {
      /*A Met un dans la variable du bouton alarme */
      /*
      if ( MSVU_Affecte_vag( "alarme" , (ADDRESS) &vl_un , V_F_TYPE ) == XDC_NOK )
         {
         XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'affecte %f a la vag %s" , vl_un , "etat" );
         return ;
         }
      */
      }
   else 
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Etat_Eq: Impossible de recuperer le message");
      return;
      }

   /*MSVU_maj_vue();*/
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Recept_Alarme" );
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception du message de raf des etat des eqt
*   Callback sur XDM_IETAT_DISPO_EQT
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_Etat_Eqt(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
          
     
/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Recept_Etat_Eq" ;
#ifdef _TIBCO_EMS
   XDY_Entier vl_etat;
   XDY_Mot vl_no;
   XDY_Mot vl_type;
#else
   T_INT4 vl_etat;
   T_INT2 vl_no;
   T_INT2 vl_type;
#endif
   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA_Recept_Etat_Eq");
   
   /*A Recuperation des donnees du message */
   TipcMsgGetSender(pa_Data->msg,&vm_sender);
    XZST_03EcritureTrace(XZSTC_DEBUG1,"sender = '%s'",vm_sender);

   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT2,&vl_type,
                                T_IPC_FT_INT2,&vl_no,
                                T_IPC_FT_INT4,&vl_etat, NULL) )
      {
      XZST_03EcritureTrace(   XZSTC_DEBUG1, "MSRA14 type = %d, num=%d, etat=%d",
				vl_type,vl_no,vl_etat);
      /*A Rafraichissement de l'etat de dispo de l'eqt */
      if ( MSRA14_raf_vag_DIS( (XDY_TypeEqt) vl_type ,
                               (XDY_Eqt) vl_no , 
                               (XDY_Etat_Eqt) vl_etat ) == XDC_NOK )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Etat_Eq: Proble lors du ref de dispo de l'eqt no %d" ,vl_no);
         return;
         }
      }
   else 
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Etat_Eq: Impossible de recuperer le message");
      return;
      }

   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : MSRA_Recept_Etat_Eq" );
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception du message de raf des etat des alerte
*   Callback sur XDM_ICREER_ALERTE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_Etat_Alerte(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
          
     
/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Recept_Etat_Alerte" ;
#ifndef _TIBCO_EMS
   T_STR  pl_alerte;
   XDY_Entier vl_sendpoke;
   XDY_Entier vl_canal_MTLL;
#else
   char  pl_alerte[255];
   T_INT4 vl_sendpoke;
   T_INT4 vl_canal_MTLL;
#endif
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Recept_Etat_Alerte");
   
   /*A Recuperation des donnees du message */
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4,&vl_sendpoke,
                                T_IPC_FT_INT4,&vl_canal_MTLL,
                                T_IPC_FT_STR,&pl_alerte, NULL) )
      {
      /*A Rafraichissement de l'etat de dispo de l'eqt */
      if ( MSRA15_raf_vag_ALERTE( (char*) pl_alerte ) == XDC_NOK )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Etat_Alerte : Proble lors du ref de dispo de l'eqt no " );
         return;
         }
      }
   else 
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Etat_Alerte: Impossible de recuperer le message");
      return;
      }

   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Recept_Etat_Alerte" );
}







/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception du message de raf des bouchons
*   Callback sur XDM_IRAF_BOUCHON_CHAUSSEE_PORTION
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_Bouchon(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
          
     
/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
*   Declanchee sur message  XDM_IRAF_BOUCHON_CHAUSSEE_PORTION
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Recept_Bouchon" ;
#ifdef _TIBCO_EMS
   XDY_Mot 	vl_type_raf;
   XDY_Mot      vl_idportion;
   XDY_Entier  	vl_numero;
   XDY_Mot  	vl_cle;
   XDY_Evt 	vl_fmc;
   XDY_Mot     	vl_sens;
   XDY_Entier   vl_trans,vl_exten;
   XDY_Mot    	vl_niv;
#else
   T_INT2 vl_type_raf;
   T_INT2      vl_idportion;
   T_INT4  vl_numero;
   T_INT2  vl_cle;
   XDY_Evt vl_fmc;
   T_INT2     vl_sens;
   T_INT4   vl_trans,vl_exten;
   T_INT2    vl_niv;
#endif

   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA_Recept_Bouchon");
   
   /* Recuperation des donnees du message */
   TipcMsgGetSender(pa_Data->msg,&vm_sender);
   TipcMsgGetDest(pa_Data->msg,&vm_dest);
    XZST_03EcritureTrace(XZSTC_DEBUG1,"sender = '%s', dest = '%s'",vm_sender,vm_dest);
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4,&vl_numero,
                                T_IPC_FT_INT2,&vl_cle,
                                T_IPC_FT_INT2,&vl_idportion,
                                T_IPC_FT_INT2,&vl_type_raf,
                                T_IPC_FT_INT2,&vl_sens,
                                T_IPC_FT_INT4,&vl_trans,
                                T_IPC_FT_INT4,&vl_exten,
                                T_IPC_FT_INT2,&vl_niv, NULL) )
      {
      XZST_03EcritureTrace(   XZSTC_DEBUG1, "vl_trans %d, vl_exten %d",vl_trans,vl_exten);
      vl_fmc.numero = (XDY_Entier) vl_numero;
      vl_fmc.cle = (XDY_Octet) vl_cle;
       if ( MSRA18_raf_zones_mobiles(CM_BOUCHON,
                                    (XDY_Octet) vl_type_raf,
                                     vl_fmc,
                                     vl_fmc,
                                    (XDY_Mot) vl_idportion,
                                    (XDY_Entier) NULL,
                                    (XDY_Sens) vl_sens,
                                    (XDY_Voie) NULL,
                                    (float) vl_trans,
                                    (float) vl_exten,
                                    ((float) vl_niv/CM_NB_NIV) - CM_DECAL_NIV,
                                    NULL) == XDC_NOK )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Bouchon: Proble lors du raf bouchon " );
         return;
         }
      }
   else 
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Bouchon: Impossible de recuperer le message");
      return;
      }

   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT :MSRA_Recept_Bouchon " );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception du message de raf des bouchons
*   Callback sur XDM_IRAF_BOUCHON_CHAUSSEE_VOIE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_Bouchon_Voie(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
          
     
/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
*   Declanchee sur message  XDM_IRAF_BOUCHON_CHAUSSEE_VOIE
*
* FONCTION 
*   
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Recept_Bouchon_Voie" ;
#ifdef _TIBCO_EMS
   XDY_Mot 	vl_type_raf;
   XDY_Mot      vl_idportion;
   XDY_Entier  	vl_numero;
   XDY_Mot  	vl_cle;
   XDY_Entier  	vl_pr;
   XDY_Mot  	vl_voie;
   XDY_Evt 	vl_fmc;
   XDY_Mot     	vl_sens;
   XDY_Entier   vl_trans,vl_exten;
   XDY_Mot    	vl_niv;
#else
   T_INT2 vl_type_raf;
   T_INT2      vl_idportion;
   T_INT4  vl_numero;
   T_INT2  vl_cle;
   T_INT4  vl_pr;
   T_INT2  vl_voie;
   XDY_Evt vl_fmc;
   T_INT2     vl_sens;
   T_INT4   vl_trans,vl_exten;
   T_INT2    vl_niv;
#endif

   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA_Recept_Bouchon_Voie");
   
   /* Recuperation des donnees du message */
   TipcMsgGetSender(pa_Data->msg,&vm_sender);
   TipcMsgGetDest(pa_Data->msg,&vm_dest);
    XZST_03EcritureTrace(XZSTC_WARNING,"sender = '%s', dest = '%s'",vm_sender,vm_dest);
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4,&vl_numero,
                                T_IPC_FT_INT2,&vl_cle,
                                T_IPC_FT_INT2,&vl_idportion,
                                T_IPC_FT_INT2,&vl_type_raf,
                                T_IPC_FT_INT4,&vl_pr,
                                T_IPC_FT_INT2,&vl_sens,
                                T_IPC_FT_INT2,&vl_voie,
                                T_IPC_FT_INT4,&vl_trans,
                                T_IPC_FT_INT4,&vl_exten,
                                T_IPC_FT_INT2,&vl_niv, NULL) )
      {
      vl_fmc.numero = (XDY_Entier) vl_numero;
      vl_fmc.cle = (XDY_Octet) vl_cle;
       if ( MSRA18_raf_zones_mobiles(CM_BOUCHONVOIE,
                                    (XDY_Octet) vl_type_raf,
                                     vl_fmc,
                                     vl_fmc,
                                    (XDY_Mot) vl_idportion,
                                    (XDY_Entier) vl_pr,
                                    (XDY_Sens) vl_sens,
                                    (XDY_Voie) vl_voie,
                                    (float) vl_trans,
                                    (float) vl_exten,
                                    ((float) vl_niv/CM_NB_NIV) - CM_DECAL_NIV,
                                    NULL) == XDC_NOK )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Bouchon_Voie: Proble lors du raf bouchon " );
         return;
         }
      }
   else 
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Bouchon_Voie: Impossible de recuperer le message");
      return;
      }

   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT :MSRA_Recept_Bouchon_Voie" );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception du message de raf des voies neutralisees
*   Callback sur XDM_IRAF_VOIE_NEUTRALISEE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_voie_neutralisee(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
          
     
/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 :MSRA_Recept_voie_neutralisee " ;
#ifdef _TIBCO_EMS
   XDY_Entier   vl_numero;
   XDY_Mot  	vl_cle;
   XDY_Mot 	vl_type_raf;
   XDY_Mot      vl_idportion;
   XDY_Evt 	vl_fmc;
   XDY_Mot     	vl_sens;
   XDY_Mot     	vl_voie;
   XDY_Entier   vl_trans,vl_exten;
   XDY_Entier   vl_pr;
#else
   T_INT4  vl_numero;
   T_INT2  vl_cle;
   T_INT2 vl_type_raf;
   T_INT2      vl_idportion;
   XDY_Evt vl_fmc;
   T_INT2     vl_sens;
   T_INT2     vl_voie;
   T_INT4   vl_trans,vl_exten;
   T_INT4   vl_pr;
#endif
   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA_Recept_voie_neutralisee");
   
   /* Recuperation des donnees du message */
   TipcMsgGetSender(pa_Data->msg,&vm_sender);
   TipcMsgGetDest(pa_Data->msg,&vm_dest);
    XZST_03EcritureTrace(XZSTC_WARNING,"sender = '%s', dest = '%s'",vm_sender,vm_dest);
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4,&vl_numero,
                                T_IPC_FT_INT2,&vl_cle,
                                T_IPC_FT_INT2,&vl_idportion,
                                T_IPC_FT_INT4,&vl_pr,
                                T_IPC_FT_INT2,&vl_sens,
                                T_IPC_FT_INT2,&vl_voie,
                                T_IPC_FT_INT2,&vl_type_raf,
                                T_IPC_FT_INT4,&vl_trans,
                                T_IPC_FT_INT4,&vl_exten, NULL) )
      {
      vl_fmc.numero = (XDY_Entier) vl_numero;
      vl_fmc.cle = (XDY_Octet) vl_cle;
      if ( MSRA18_raf_zones_mobiles(CM_NEUTRA,
                                    (XDY_Octet) vl_type_raf,
                                    vl_fmc,
                                    vl_fmc,
                                    (XDY_Mot) vl_idportion,
                                    (XDY_Entier) vl_pr,
                                    (XDY_Sens) vl_sens,
                                    (XDY_Voie) vl_voie,
                                    (float) vl_trans,
                                    (float) vl_exten,
                                    (float) 0,
                                    NULL) == XDC_NOK )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_voie_neutralisee: Proble lors du raf de la voie neutraliser sur potion %d" ,vl_idportion);
         return;
         }
      }
   else 
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_voie_neutralisee: Impossible de recuperer le message");
      return;
      }

   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT :MSRA_Recept_voie_neutralisee " );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception du message de raf des voies radt inverse
*   Callback sur XDM_IRAF_VOIE_RADT_INVERSE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_voie_radt_inverse(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
          
     
/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 :MSRA_Recept_voie_radt_inverse " ;
#ifdef _TIBCO_EMS
   XDY_Entier  	vl_numero;
   XDY_Mot  	vl_cle;
   XDY_Mot     	vl_idportion;
   XDY_Entier  	vl_pr;
   XDY_Mot     	vl_sens;
   XDY_Mot     	vl_voie;
   XDY_Mot 	vl_type_raf;
   XDY_Entier  	vl_trans,vl_exten;
   XDY_Mot    	vl_idstation;
   XDY_Mot   	vl_sens_station;
#else
   T_INT4  	vl_numero;
   T_INT2  	vl_cle;
   T_INT2      	vl_idportion;
   T_INT4   	vl_pr;
   T_INT2     	vl_sens;
   T_INT2     	vl_voie;
   T_INT2 	vl_type_raf;
   T_INT4   	vl_trans,vl_exten;
   T_INT2    	vl_idstation;
   T_INT2   	vl_sens_station;
#endif
   XDY_Evt vl_fmc;
   XDY_Entier    vl_idradt;
   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  :MSRA_Recept_voie_radt_inverse ");
   
   /* Recuperation des donnees du message */
   TipcMsgGetSender(pa_Data->msg,&vm_sender);
   TipcMsgGetDest(pa_Data->msg,&vm_dest);
    XZST_03EcritureTrace(XZSTC_WARNING,"sender = '%s', dest = '%s'",vm_sender,vm_dest);
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4,&vl_numero,
                                T_IPC_FT_INT2,&vl_cle,
                                T_IPC_FT_INT2,&vl_idportion,
                                T_IPC_FT_INT4,&vl_pr,
                                T_IPC_FT_INT2,&vl_sens,
                                T_IPC_FT_INT2,&vl_voie,
                                T_IPC_FT_INT2,&vl_type_raf,
                                T_IPC_FT_INT4,&vl_trans,
                                T_IPC_FT_INT4,&vl_exten,
                                T_IPC_FT_INT2,&vl_idstation,
                                T_IPC_FT_INT2,&vl_sens_station, NULL) )
      {
      /*A Construction du no de radt */
      vl_idradt = (XDY_Entier) (vl_idstation*10 + vl_sens_station);
      vl_fmc.numero = (XDY_Entier) vl_numero;
      vl_fmc.cle = (XDY_Octet) vl_cle;
      /*A Raf de la voie radt inverse */
      if ( MSRA18_raf_zones_mobiles(CM_INVERSE,
                                    (XDY_Octet) vl_type_raf,
                                    vl_fmc,
                                    vl_fmc,
                                    (XDY_Mot) vl_idportion,
                                    (XDY_Entier) vl_pr,
                                    (XDY_Sens) vl_sens,
                                    (XDY_Voie) vl_voie,
                                    (float) vl_trans,
                                    (float) vl_exten,
                                    (float) 0,
                                    vl_idradt) == XDC_NOK )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_voie_radt_inverse: Proble lors du raf de la voie radt inverse sur portion %d" ,vl_idportion);
         return;
         }
      }
   else 
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_voie_radt_inverse: Impossible de recuperer le message");
      return;
      }

   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT :MSRA_Recept_voie_radt_inverse " );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception du message de raf des basculements de bouchon 
*   Callback sur XDM_IRAF_BASCULEMENT_BOUCHON
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_basculement(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
          
     
/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Recept_basculement" ;
#ifdef _TIBCO_EMS
   XDY_Entier vl_numero;
   XDY_Mot vl_cle;
   XDY_Mot vl_numerob;
   XDY_Mot vl_cleb;
   XDY_Mot vl_idportion;
   XDY_Entier vl_pr;
   XDY_Mot vl_sens;
   XDY_Mot vl_voie;
   XDY_Mot vl_type_raf;
   XDY_Entier vl_trans,vl_exten;
   XDY_Mot vl_niv;
#else
   T_INT4 vl_numero;
   T_INT2 vl_cle;
   T_INT4 vl_numerob;
   T_INT2 vl_cleb;
   T_INT2 vl_idportion;
   T_INT4 vl_pr;
   T_INT2 vl_sens;
   T_INT2 vl_voie;
   T_INT2 vl_type_raf;
   T_INT4 vl_trans,vl_exten;
   T_INT2 vl_niv;
#endif
   XDY_Evt vl_fmc;
   XDY_Evt vl_fmc_bou;
   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Recept_basculement" );
   
   /* Recuperation des donnees du message */
   TipcMsgGetSender(pa_Data->msg,&vm_sender);
   TipcMsgGetDest(pa_Data->msg,&vm_dest);
    XZST_03EcritureTrace(XZSTC_WARNING,"sender = '%s', dest = '%s'",vm_sender,vm_dest);
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4,&vl_numero,
                                T_IPC_FT_INT2,&vl_cle,
                                T_IPC_FT_INT4,&vl_numerob,
                                T_IPC_FT_INT2,&vl_cleb,
                                T_IPC_FT_INT2,&vl_idportion,
                                T_IPC_FT_INT4,&vl_pr,
                                T_IPC_FT_INT2,&vl_sens,
                                T_IPC_FT_INT2,&vl_voie,
                                T_IPC_FT_INT2,&vl_type_raf,
                                T_IPC_FT_INT4,&vl_trans,
                                T_IPC_FT_INT4,&vl_exten,
                                T_IPC_FT_INT2,&vl_niv, NULL) )
      {
      vl_fmc.numero = (XDY_Entier) vl_numero;
      vl_fmc.cle = (XDY_Octet) vl_cle;
      vl_fmc_bou.numero = (XDY_Entier) vl_numerob;
      vl_fmc_bou.cle = (XDY_Octet) vl_cleb;
      if ( MSRA18_raf_zones_mobiles(CM_BASCBOU,
                                    (XDY_Octet) vl_type_raf,
                                    vl_fmc,
                                    vl_fmc_bou,
                                   (XDY_Mot) vl_idportion,
                                    (XDY_Entier) vl_pr,
                                    (XDY_Sens) vl_sens,
                                    (XDY_Voie) vl_voie,
                                    (float) vl_trans,
                                    (float) vl_exten,
                                    ((float) vl_niv/CM_NB_NIV) - CM_DECAL_NIV,
                                    NULL) == XDC_NOK )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_basculement: Proble lors du raf du basculement sur portion %d" ,vl_idportion);
         return;
         }
      }
   else 
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_basculement: Impossible de recuperer le message");
      return;
      }

   
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT :MSRA_Recept_basculement " );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception du message de raf des objets mobiles
*   Callback sur XDM_IRAF_OBJET_MOBILE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_objet_mobile(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
          
     
/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 :MSRA_Recept_objet_mobile " ;
#ifdef _TIBCO_EMS
   XDY_Entier  vl_numero;
   XDY_Mot  vl_cle;
   XDY_Entier  vl_numeroa;
   XDY_Mot  vl_clea;
   XDY_Mot vl_type_raf;
   XDY_Mot vl_type_obj;
   XDY_Mot      vl_idportion;
   XDY_Mot     vl_sens;
   XDY_Mot     vl_voie;
   XDY_Entier   vl_trans;
#else
   T_INT4  vl_numero;
   T_INT2  vl_cle;
   T_INT4  vl_numeroa;
   T_INT2  vl_clea;
   T_INT2 vl_type_raf;
   T_INT2 vl_type_obj;
   T_INT2      vl_idportion;
   T_INT2     vl_sens;
   T_INT2     vl_voie;
   T_INT4   vl_trans;
#endif
   XDY_Evt vl_fmc;
   XDY_Evt vl_act;
   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA_Recept_objet_mobile");
   
   /* Recuperation des donnees du message */
   TipcMsgGetSender(pa_Data->msg,&vm_sender);
   TipcMsgGetDest(pa_Data->msg,&vm_dest);
    XZST_03EcritureTrace(XZSTC_WARNING,"sender = '%s', dest = '%s'",vm_sender,vm_dest);
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4,&vl_numero,
                                T_IPC_FT_INT2,&vl_cle,
                                T_IPC_FT_INT4,&vl_numeroa,
                                T_IPC_FT_INT2,&vl_clea,
                                T_IPC_FT_INT2,&vl_idportion,
                                T_IPC_FT_INT2,&vl_type_raf,
                                T_IPC_FT_INT2,&vl_type_obj,
                                T_IPC_FT_INT2,&vl_sens,
                                T_IPC_FT_INT2,&vl_voie,
                                T_IPC_FT_INT4,&vl_trans, NULL) )
      {
      vl_fmc.numero = (XDY_Entier) vl_numero;
      vl_fmc.cle = (XDY_Octet) vl_cle;
      vl_act.numero = (XDY_Entier) vl_numeroa;
      vl_act.cle = (XDY_Octet) vl_clea;
      XZST_03EcritureTrace(   XZSTC_DEBUG1, "TRANS %d(%f)",vl_trans,(float)vl_trans);
      if ( MSRA18_raf_objets_mobiles((XDY_Octet) vl_type_obj,
                                    (XDY_Octet) vl_type_raf,
                                    vl_fmc,
                                    vl_act,
                                    (XDY_Mot) vl_idportion,
                                    (XDY_Sens) vl_sens,
                                    (XDY_Voie) vl_voie,
                                    vl_trans) == XDC_NOK )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_objet_mobile: Proble lors du raf de l'objet %d sur portion %d" ,vl_type_obj,vl_idportion);
         return;
         }
      }
   else 
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_objet_mobile: Impossible de recuperer le message");
      return;
      }

/*      VThttraverse(    vm_thash_fop , MSOP_Affiche_elt_fop , (ADDRESS) NULL );
      VThtstats( vm_thash_fop );*/
   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT :MSRA_Recept_objet_mobile " );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception du message de raf des objets mobiles
*   Callback sur XDM_IRAF_OBJET_MOBILE_ECHANG
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_objet_mobile_echang(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
          
     
/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 :MSRA_Recept_objet_mobile_echang " ;
#ifndef _TIBCO_EMS
   T_INT4  vl_numero;
   T_INT2  vl_cle;
   T_INT4  vl_numeroa;
   T_INT2  vl_clea;
   T_INT2 vl_type_raf;
   T_INT2 vl_type_obj;
   T_STR  vl_echang;
   T_INT2     vl_sens;
   T_INT2     vl_localisation;
#else
   XDY_Entier  vl_numero;
   XDY_Mot  vl_cle;
   XDY_Entier  vl_numeroa;
   XDY_Mot  vl_clea;
   XDY_Mot vl_type_raf;
   XDY_Mot vl_type_obj;
   XDY_Phrase  vl_echang;
   XDY_Mot     vl_sens;
   XDY_Mot     vl_localisation;
#endif
   XDY_Evt vl_fmc;
   XDY_Evt vl_act;
   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA_Recept_objet_mobile_echang");
   
   /* Recuperation des donnees du message */
   TipcMsgGetSender(pa_Data->msg,&vm_sender);
   TipcMsgGetDest(pa_Data->msg,&vm_dest);
    XZST_03EcritureTrace(XZSTC_WARNING,"sender = '%s', dest = '%s'",vm_sender,vm_dest);
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4,&vl_numero,
                                T_IPC_FT_INT2,&vl_cle,
                                T_IPC_FT_INT4,&vl_numeroa,
                                T_IPC_FT_INT2,&vl_clea,
                                T_IPC_FT_STR,&vl_echang,
                                T_IPC_FT_INT2,&vl_type_raf,
                                T_IPC_FT_INT2,&vl_type_obj,
                                T_IPC_FT_INT2,&vl_sens,
                                T_IPC_FT_INT2,&vl_localisation, NULL) )
      {
      vl_fmc.numero = (XDY_Entier) vl_numero;
      vl_fmc.cle = (XDY_Octet) vl_cle;
      vl_act.numero = (XDY_Entier) vl_numeroa;
      vl_act.cle = (XDY_Octet) vl_clea;
      if ( MSRA18_raf_objets_mobiles_echang((XDY_Octet) vl_type_obj,
                                    (XDY_Octet) vl_type_raf,
                                    vl_fmc,
                                    vl_act,
                                    (char *) vl_echang,
                                    (XDY_Sens) vl_sens,
                                    (XDY_Octet) vl_localisation) == XDC_NOK )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_objet_mobile_echang: Proble lors du raf de l'objet %d sur echangeur %s" ,vl_type_obj,vl_echang);
         return;
         }
      }
   else 
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_objet_mobile_echang: Impossible de recuperer le message");
      return;
      }

   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT :MSRA_Recept_objet_mobile_echang " );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception du message de raf des affectations de voies 
*   Callback sur XDM_IRAF_PORT_AFF_VOIES
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_aff_voies(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
          
     
/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 :MSRA_Recept_aff_voies " ;
   XDY_Evt vl_fmc;
#ifndef _TIBCO_EMS
   T_INT4  vl_numero;
   T_INT2  vl_cle;
   T_INT2     vl_debfin;
   T_INT2 vl_type_raf;
   T_INT2      vl_idportion;
   T_INT4   vl_trans;
   T_INT2   *pl_voie;
   T_INT2     vl_numordre;
   T_INT4   vl_size;
#else
   XDY_Entier  vl_numero;
   XDY_Mot  vl_cle;
   XDY_Mot     vl_debfin;
   XDY_Mot vl_type_raf;
   XDY_Mot      vl_idportion;
   XDY_Entier   vl_trans;
   XDY_Mot   pl_voie[21];
   XDY_Mot     vl_numordre;
   XDY_Entier   vl_size;
#endif
   XDY_Octet vl_voies[CM_NB_VOIES*2];
   int i;
   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  :MSRA_Recept_aff_voies ");
   
   /* Recuperation des donnees du message */
   TipcMsgGetSender(pa_Data->msg,&vm_sender);
   TipcMsgGetDest(pa_Data->msg,&vm_dest);
    XZST_03EcritureTrace(XZSTC_WARNING,"sender = '%s', dest = '%s'",vm_sender,vm_dest);
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4,&vl_numero,
                                T_IPC_FT_INT2,&vl_cle,
                                T_IPC_FT_INT2,&vl_debfin,
                                T_IPC_FT_INT2,&vl_idportion,
                                T_IPC_FT_INT2,&vl_type_raf,
                                T_IPC_FT_INT4,&vl_trans,
                                T_IPC_FT_INT2_ARRAY,&pl_voie,&vl_size,
				T_IPC_FT_INT2,&vl_numordre,
				NULL) )
      {
      if ( vl_size != 20 )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_aff_voies: Le message recu na pas la bonne longueur de donnees pour les voies: %d",vl_size);
         return;
         }
      vl_fmc.numero = (XDY_Entier) vl_numero;
      vl_fmc.cle = (XDY_Octet) vl_cle;
      for ( i=0; i < vl_size; i++)
          vl_voies[i] = (XDY_Octet) pl_voie[i];
          
      vl_fmc.numero = (XDY_Entier) vl_numero;
      vl_fmc.cle = (XDY_Octet) vl_cle;
      
      if ( MSRA_raf_Aff_Voies_Prep((XDY_Octet) vl_type_raf,
                                    vl_fmc,
                                    (XDY_Octet) vl_debfin,
                                    (XDY_Mot) vl_idportion,
                                    vl_voies[0], vl_voies[1],
                                    vl_voies[2], vl_voies[3],
                                    vl_voies[4], vl_voies[5],
                                    vl_voies[6], vl_voies[7],
                                    vl_voies[8], vl_voies[9],
                                    vl_voies[10], vl_voies[11],
                                    vl_voies[12], vl_voies[13],
                                    vl_voies[14], vl_voies[15],
                                    vl_voies[16], vl_voies[17],
                                    vl_voies[18], vl_voies[19],
                                    (int) vl_trans ,
				    (XDY_Octet) vl_numordre) == XDC_NOK )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_aff_voies: Proble lors du raf de la voie neutraliser sur potion %d" ,vl_idportion);
         return;
         }
      }
   else 
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_aff_voies: Impossible de recuperer le message");
      return;
      }

   
   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT :MSRA_Recept_aff_voies " );
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception du message de raz des mobiles
*   Callback sur XDM_IRAZ_MOB
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_RazMob(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
          
     
/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Recept_RazMob" ;

   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA_Recept_RazMob");
   
   /*A Recuperation des donnees du message */
   if (TipcMsgRead(pa_Data->msg, NULL) )
      {
      /*A Destruction de tous les mobiles precedement crees */
      }

   XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : MSRA_Recept_RazMob" );
   return ;
}


/*SDU*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Reception du message de masquage des equipements
*   Callback sur XDM_IMASQUE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_Masque(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)


/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Recept_Masque" ;
#ifdef _TIBCO_EMS
   XDY_Mot vl_type_eq;
#else
   T_INT2 vl_type_eq;
#endif

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Recept_Masque");

   /*A Recuperation des donnees du message */
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT2,&vl_type_eq, NULL) )
      {
      /*A Rafraichissement de l'etat de dispo de l'eqt */
      if ( MSRA18_Masque( (XDY_TypeEqt) vl_type_eq) == XDC_NOK )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Masque : Probleme lors du raf du masque" );
         return;
         }
      }
   else
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Masque: Impossible de recuperer le message");
      return;
      }


   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Recept_Masque" );
}

/*SDU*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Reception du message de masquage des equipements
*   Callback sur XDM_IMASQUE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_PlanSecours(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)


/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Recept_Masque" ;
#ifdef _TIBCO_EMS
   XDY_Entier vl_num_plan;
#else
   T_INT4 vl_num_plan;
#endif

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Recept_PlanSecours");

   /*A Recuperation des donnees du message */
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4,&vl_num_plan, NULL) )
      {
      /*A Rafraichissement de l'etat de dispo de l'eqt */
      if ( MSRA21_PlanSecours( (XDY_Entier) vl_num_plan) == XDC_NOK )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_PlanSecours : Probleme lors du raf du masque" );
         return;
         }
      }
   else
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_PlanSecours: Impossible de recuperer le message");
      return;
      }


   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Recept_PlanSecours" );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Reception du message d'affichage des vues
*   Callback sur XDM_IVUE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_Vue(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)


/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Recept_Vue" ;
#ifndef _TIBCO_EMS
   T_STR  vl_vue;
#else
   XDY_Phrase  vl_vue;
#endif

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Recept_Vue");

   /*A Recuperation des donnees du message */
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_STR,&vl_vue, NULL) )
      {
      /*A Rafraichissement de l'etat de dispo de l'eqt */
      if ( MSRA19_Vue( vl_vue ) == XDC_NOK )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Vue : Probleme lors de l'aff de la vue" );
         return;
         }
      }
   else
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Vue: Impossible de recuperer le message");
      return;
      }


   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Recept_Vue" );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Reception du message d'affichage des vues
*   Callback sur XDM_IGRAPHE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_Graphe(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)


/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Recept_Graphe" ;
#ifndef _TIBCO_EMS
   T_STR  vl_chaine;
   T_INT2 vl_type_graphe;
#else
   char  vl_chaine[512];
   XDY_Mot vl_type_graphe;
#endif
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Recept_Graphe");

   /*A Recuperation des donnees du message */
   if (TipcMsgRead(pa_Data->msg,
		   T_IPC_FT_INT2, &vl_type_graphe,
		   T_IPC_FT_STR, &vl_chaine,
		   NULL) )
      {
	 XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Graphe : Type Graphe = %d, chaine = [%s]", vl_type_graphe, vl_chaine );
	  switch(vl_type_graphe)
	  {
	  	  case XZISC_GRAPHE_TDP_MZ:
//			  if ( MSRA20_TdpMZ( vl_chaine ) == XDC_NOK )
			  if ( MSCO04_demarer_courbe_TDP_MZ( vl_chaine ) == XDC_NOK )
				 {
				 XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Graphe : Probleme lors du declenchement de la courbe" );
				 return;
				 }
		  break;
	  	  case XZISC_GRAPHE_F2T:
			  if ( MSCO01_demarer_courbe_ft(CM_COURBE_FT , vl_chaine ) == XDC_NOK )
				 {
				 XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Graphe : Probleme lors du declenchement de la courbe" );
				 return;
				 }
		  break;
	  	  case XZISC_GRAPHE_F2T_REF:
			  if ( MSCO01_demarer_courbe_ft(CM_COURBE_FT_REF, vl_chaine ) == XDC_NOK )
				 {
				 XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Graphe : Probleme lors du declenchement de la courbe" );
				 return;
				 }
		  break;
	  	  case XZISC_GRAPHE_INRETS:
	  		  if ( MSCO04_demarer_courbe_INRETS( vl_chaine ) == XDC_NOK )
	  		  {
	  			  XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Graphe : Probleme lors du declenchement de la courbe" );
	  			  return;
	  		  }
	  	  break;
	  	  default:
				 XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Graphe : Type de Graphe %d Inconnu ", vl_type_graphe );
	  }

      /*A Rafraichissement de l'etat de dispo de l'eqt */
      }
   else
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Graphe: Impossible de recuperer le message");
      return;
      }


   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Recept_Graphe" );
}/*SDU*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  ReInitialisation des equipement
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Reinit(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)



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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Reinit";
   int code_retour;
   XZSCT_Datagroup         vl_datagroup_init ;
   XDY_Booleen	vl_abt;
   int vl_ind=0;
   XDY_Datagroup 	vl_dg;

   XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : MSRA_ReinitRenov" );
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Reinit" );
   
   vl_abt=XZEAC_FERMER_ABONNEMENT;
   if (vg_pas_tisse)
   	strcpy(vl_dg, vm_NomMachine);
   else
   	strcpy(vl_dg, vm_NomSite);

   for (vl_ind=0; vl_ind < 2; vl_ind++)
      {
         /*A Abonnement aux Messages de rafraichissement des Objets */
         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_PMV , vl_dg );
         XZEA08_Abt_Etat_PMV ( vl_abt , vl_datagroup_init , MSRA01_raf_vag_PMV_Message , XDC_NOM_SITE_VC , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnment XZEA08_Abt_Etat_PMV : %d " , code_retour );
            return ;
            }
         XZEA08_Abt_Etat_PMV ( vl_abt , vl_datagroup_init , MSRA01_raf_vag_PMV_Message , XDC_NOM_SITE_DP , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnment XZEA08_Abt_Etat_PMV : %d " , code_retour );
            return ;
            }
         XZEA08_Abt_Etat_PMV ( vl_abt , vl_datagroup_init , MSRA01_raf_vag_PMV_Message , XDC_NOM_SITE_CA , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnment XZEA08_Abt_Etat_PMV : %d " , code_retour );
            return ;
            }
    
         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_PICTO , vl_dg );    
         XZEA09_Abt_Etat_Picto ( vl_abt , vl_datagroup_init , MSRA02_raf_vag_PMV_Picto , XDC_NOM_SITE_VC , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         XZEA09_Abt_Etat_Picto ( vl_abt , vl_datagroup_init , MSRA02_raf_vag_PMV_Picto , XDC_NOM_SITE_DP , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         XZEA09_Abt_Etat_Picto ( vl_abt , vl_datagroup_init , MSRA02_raf_vag_PMV_Picto , XDC_NOM_SITE_CA , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
             
         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_PMVA , vl_dg );
         XZEA080_Abt_Etat_PMVA ( vl_abt , vl_datagroup_init , MSRA12_raf_vag_PMVA, XDC_NOM_SITE_VC , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnment XZEA080_Abt_Etat_PMVA : %d " , code_retour );
            return ;
            }
         XZEA080_Abt_Etat_PMVA ( vl_abt , vl_datagroup_init , MSRA12_raf_vag_PMVA, XDC_NOM_SITE_DP , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnment XZEA080_Abt_Etat_PMVA : %d " , code_retour );
            return ;
            }
         XZEA080_Abt_Etat_PMVA ( vl_abt , vl_datagroup_init , MSRA12_raf_vag_PMVA, XDC_NOM_SITE_CA , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnment XZEA080_Abt_Etat_PMVA : %d " , code_retour );
            return ;
            }

         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_NIV , vl_dg );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA05_Abt_Niveaux_Service " );
         XZEA05_Abt_Niveaux_Service ( vl_abt , vl_datagroup_init , MSRA03_raf_vag_NivTRA , XDC_NOM_SITE_VC , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA05_Abt_Niveaux_Service %d " , code_retour );
         if ( code_retour != XDC_OK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA05_Abt_Niveaux_Service " );
         XZEA05_Abt_Niveaux_Service ( vl_abt , vl_datagroup_init , MSRA03_raf_vag_NivTRA , XDC_NOM_SITE_DP , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA05_Abt_Niveaux_Service %d " , code_retour );
         if ( code_retour != XDC_OK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA05_Abt_Niveaux_Service " );
         XZEA05_Abt_Niveaux_Service ( vl_abt , vl_datagroup_init , MSRA03_raf_vag_NivTRA , XDC_NOM_SITE_CA , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA05_Abt_Niveaux_Service %d " , code_retour );
         if ( code_retour != XDC_OK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         
         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_NAV , vl_dg );
         XZEA10_Abt_Etat_NAV    ( vl_abt , vl_datagroup_init , MSRA04_raf_vag_NAV , XDC_NOM_SITE_VC , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         XZEA10_Abt_Etat_NAV    ( vl_abt , vl_datagroup_init , MSRA04_raf_vag_NAV , XDC_NOM_SITE_DP , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         XZEA10_Abt_Etat_NAV    ( vl_abt , vl_datagroup_init , MSRA04_raf_vag_NAV , XDC_NOM_SITE_CA , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         
         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_BAF , vl_dg );
         XZEA40_Abt_Etat_BAF    ( vl_abt , vl_datagroup_init , MSRA13_raf_vag_BAF , XDC_NOM_SITE_VC , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         XZEA40_Abt_Etat_BAF    ( vl_abt , vl_datagroup_init , MSRA13_raf_vag_BAF , XDC_NOM_SITE_DP , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         XZEA40_Abt_Etat_BAF    ( vl_abt , vl_datagroup_init , MSRA13_raf_vag_BAF , XDC_NOM_SITE_CA , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }

         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_BAD , vl_dg );
         XZEA41_Abt_Etat_BAD    ( vl_abt , vl_datagroup_init , MSRA14_raf_vag_BAD , XDC_NOM_SITE_VC , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         XZEA41_Abt_Etat_BAD    ( vl_abt , vl_datagroup_init , MSRA14_raf_vag_BAD , XDC_NOM_SITE_DP , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }
         XZEA41_Abt_Etat_BAD    ( vl_abt , vl_datagroup_init , MSRA14_raf_vag_BAD , XDC_NOM_SITE_CA , &code_retour );
         if ( code_retour != XDC_OK ) {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
            }

	XZEA70_Abt_Etat_PRV (vl_abt , vl_datagroup_init , MSRA15_raf_vag_PRV, XDC_NOM_SITE_VC , &code_retour );
	if ( code_retour != XDC_OK ) {
	   XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
	   return;
	}
	XZEA70_Abt_Etat_PRV (vl_abt , vl_datagroup_init , MSRA15_raf_vag_PRV, XDC_NOM_SITE_DP , &code_retour );
	if ( code_retour != XDC_OK ) {
	   XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
	   return;
	}
	XZEA70_Abt_Etat_PRV (vl_abt , vl_datagroup_init , MSRA15_raf_vag_PRV, XDC_NOM_SITE_CA , &code_retour );
	if ( code_retour != XDC_OK ) {
	   XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
	   return;
	}

	/* PIC */
	XZEA71_Abt_Etat_PIC (vl_abt , vl_datagroup_init , MSRA16_raf_vag_PIC, XDC_NOM_SITE_VC , &code_retour );
	if (code_retour != XDC_OK )
	{
	   XZST_03EcritureTrace (XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
	   return;
	}
	XZEA71_Abt_Etat_PIC (vl_abt , vl_datagroup_init , MSRA16_raf_vag_PIC, XDC_NOM_SITE_DP , &code_retour );
	if ( code_retour != XDC_OK ) {
	   XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
	   return;
	}
	XZEA71_Abt_Etat_PIC (vl_abt , vl_datagroup_init , MSRA16_raf_vag_PIC, XDC_NOM_SITE_CA , &code_retour );
	if ( code_retour != XDC_OK ) {
	   XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
	   return;
	}
		 
         /*A Si on n'est au CI on s'abt au RAU du site */
         if ( ( !strcmp( vm_NomSite , XDC_NOM_SITE_CI ) ) ||( !strcmp( vm_NomSite , XDC_NOM_SITE_CA ) ) || ( vm_TypeMachine == XZSCC_TYPEM_PCS ))
            {
            XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_RAU , vl_dg );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA14_Abt_Etat_RAU " );
            XZEA14_Abt_Etat_RAU ( XZEAC_DEBUT_ABONNEMENT , vl_datagroup_init , MSRA06_raf_vag_PAU , vm_NomSite , &code_retour );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA14_Abt_Etat_RAU %d " , code_retour );
            if ( code_retour != XDC_OK )
               {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return ;
               }
            }
         
         if (vm_TypeMachine != XZSCC_TYPEM_PCS )
         {
         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_JOURNUIT , vl_dg );
         /*A Si on est au CI on s'abt pour jour nuit aux 2 district, sinon seult au district d'appartenance*/
         if ( !strcmp( vm_NomSite , XDC_NOM_SITE_CI ) )
            {
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA29_Abt_Etat_JourNuit " );
            XZEA29_Abt_Etat_JourNuit ( vl_abt , vl_datagroup_init , MSRA_raf_journuit, XDC_NOM_SITE_DP , &code_retour );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA29_Abt_Etat_JourNuit %d " , code_retour );
            if ( code_retour != XDC_OK )
               {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return ;
               }
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA29_Abt_Etat_JourNuit " );
            XZEA29_Abt_Etat_JourNuit ( vl_abt , vl_datagroup_init , MSRA_raf_journuit, XDC_NOM_SITE_VC , &code_retour );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA29_Abt_Etat_JourNuit %d " , code_retour );
            if ( code_retour != XDC_OK )
               {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return ;
               }
            }
         else
            {
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA29_Abt_Etat_JourNuit " );
            XZEA29_Abt_Etat_JourNuit ( vl_abt , vl_datagroup_init , MSRA_raf_journuit, vm_NomSite , &code_retour );
            XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA29_Abt_Etat_JourNuit %d " , code_retour );
            if ( code_retour != XDC_OK )
               {
               XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
               return ;
               }
            }
            
		 /* Abonnement aux sequences tube */
         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_TUB , vl_dg );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA23_Abt_Etat_Sequence_Tube " );
         XZEA23_Abt_Etat_Sequence_Tube ( vl_abt , vl_datagroup_init , MSRA_raf_SeqTunnel, XDC_NOM_SITE_DP , XDC_EQTALL , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA23_Abt_Etat_Sequence_Tube %d " , code_retour );
         if ( code_retour != XDC_OK )
         {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
         }
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA23_Abt_Etat_Sequence_Tube " );
         XZEA23_Abt_Etat_Sequence_Tube ( vl_abt , vl_datagroup_init , MSRA_raf_SeqTunnel, XDC_NOM_SITE_VC , XDC_EQTALL , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA23_Abt_Etat_Sequence_Tube %d " , code_retour );
         if ( code_retour != XDC_OK )
         {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
         }
          XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA23_Abt_Etat_Sequence_Tube " );
         XZEA23_Abt_Etat_Sequence_Tube ( vl_abt , vl_datagroup_init , MSRA_raf_SeqTunnel, XDC_NOM_SITE_CA , XDC_EQTALL , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA23_Abt_Etat_Sequence_Tube %d " , code_retour );
         if ( code_retour != XDC_OK )
         {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return ;
         }
           
		 /* Abonnement aux sequences echangeur */
         XDG_EncodeDG2(vl_datagroup_init , XDG_IABTINIT_ECH , vl_dg );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA24_Abt_Etat_Sequence_Ech " );
         XZEA24_Abt_Etat_Sequence_Ech ( vl_abt , vl_datagroup_init , MSRA_raf_SeqEch, XDC_NOM_SITE_DP , XDC_EQTALL , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA24_Abt_Etat_Sequence_Ech %d " , code_retour );
         if ( code_retour != XDC_OK )
         {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return;
         }
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA24_Abt_Etat_Sequence_Ech " );
         XZEA24_Abt_Etat_Sequence_Ech ( vl_abt , vl_datagroup_init , MSRA_raf_SeqEch, XDC_NOM_SITE_VC , XDC_EQTALL , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA24_Abt_Etat_Sequence_Ech %d " , code_retour );
         if ( code_retour != XDC_OK )
         {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return;
         }
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  in  : XZEA24_Abt_Etat_Sequence_Ech " );
         XZEA24_Abt_Etat_Sequence_Ech ( vl_abt , vl_datagroup_init , MSRA_raf_SeqEch, XDC_NOM_SITE_CA , XDC_EQTALL , &code_retour );
         XZST_03EcritureTrace(   XZSTC_INTERFACE , "  out  : XZEA24_Abt_Etat_Sequence_Ech %d " , code_retour );
         if ( code_retour != XDC_OK )
         {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return;
         }
		 
		   /* Abonnement aux sequences signalisation renovee */
		   XZST_03EcritureTrace(   XZSTC_WARNING , "  Abonnement aux sequences GTC Renovee - Tube, site DP");
         XZEA50_Abt_Etat_Sequence_SIG ( vl_abt , vl_datagroup_init , MSRA_raf_SeqGTCRenovee, XDC_NOM_SITE_DP , XDC_EQT_TUB, XDC_EQTALL , &code_retour );
		   if ( code_retour != XDC_OK )
         {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return;
         }
		   XZST_03EcritureTrace(   XZSTC_WARNING , "  Abonnement aux sequences GTC Renovee - Tube, site VC");
         XZEA50_Abt_Etat_Sequence_SIG ( vl_abt , vl_datagroup_init , MSRA_raf_SeqGTCRenovee, XDC_NOM_SITE_VC , XDC_EQT_TUB, XDC_EQTALL , &code_retour );
		   if ( code_retour != XDC_OK )
         {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return;
         }
		   XZST_03EcritureTrace(   XZSTC_WARNING , "  Abonnement aux sequences GTC Renovee - Tube, site CA");
         XZEA50_Abt_Etat_Sequence_SIG ( vl_abt , vl_datagroup_init , MSRA_raf_SeqGTCRenovee, XDC_NOM_SITE_CA , XDC_EQT_TUB, XDC_EQTALL , &code_retour );
		   if ( code_retour != XDC_OK )
         {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return;
         }

		   XZST_03EcritureTrace(   XZSTC_WARNING , "  Abonnement aux sequences GTC Renovee - Echangeur, site DP");
         XZEA50_Abt_Etat_Sequence_SIG ( vl_abt , vl_datagroup_init , MSRA_raf_SeqGTCRenovee, XDC_NOM_SITE_DP , XDC_EQT_ECH, XDC_EQTALL , &code_retour );
		   if ( code_retour != XDC_OK )
         {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return;
         }
		   XZST_03EcritureTrace(   XZSTC_WARNING , "  Abonnement aux sequences GTC Renovee - Echangeur, site VC");
         XZEA50_Abt_Etat_Sequence_SIG ( vl_abt , vl_datagroup_init , MSRA_raf_SeqGTCRenovee, XDC_NOM_SITE_VC , XDC_EQT_ECH, XDC_EQTALL , &code_retour );
		   if ( code_retour != XDC_OK )
         {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return;
         }
		   XZST_03EcritureTrace(   XZSTC_WARNING , "  Abonnement aux sequences GTC Renovee - Echangeur, site CA");
         XZEA50_Abt_Etat_Sequence_SIG ( vl_abt , vl_datagroup_init , MSRA_raf_SeqGTCRenovee, XDC_NOM_SITE_CA , XDC_EQT_ECH, XDC_EQTALL , &code_retour );
		   if ( code_retour != XDC_OK )
         {
            XZST_03EcritureTrace(   XZSTC_WARNING , "  Probleme lors de l'abonnement  : %d " , code_retour );
            return;
         }
		 
      }
      vl_abt=XZEAC_DEBUT_ABONNEMENT;
      }
            
      /*A Remise en dispo des eqt avant reinit */
      /*MSVU_RazEtatEqt();*/
      /*A Initialisation de l'IHm avec la Base de Donnee des Eqt*/
      XDG_EncodeDG2( vl_datagroup_init , XDG_IETAT_INIEQT , vl_dg );
      if ( xzat27_Init_Etats_EqtsIhm( XDC_EQT_NAV , 0xFF , vl_datagroup_init ) == XDC_NOK )
         {
         XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation des BAF" );
         }
      if ( xzat27_Init_Etats_EqtsIhm( XDC_EQT_BAF , 0xFF , vl_datagroup_init ) == XDC_NOK )
         {
         XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation des NAV" );
         }
      if ( xzat27_Init_Etats_EqtsIhm( XDC_EQT_BAD , 0xFF , vl_datagroup_init ) == XDC_NOK )
         {
         XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation des BAD" );
         }
      if ( xzat27_Init_Etats_EqtsIhm( XDC_EQT_PRV , 0xFF , vl_datagroup_init ) == XDC_NOK )
         {
         XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation des PRV" );
         }
        	 
      if ( xzat27_Init_Etats_EqtsIhm( XDC_EQT_PMV , 0xFF , vl_datagroup_init ) == XDC_NOK )
         {
         XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation des PMV " );
         }
      if ( xzat27_Init_Etats_EqtsIhm( XDC_EQT_PMVA , 0xFF , vl_datagroup_init ) == XDC_NOK )
         {
         XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation des PMVA " );
         }
         
      if ( xzat27_Init_Etats_EqtsIhm( XDC_EQT_CAM , 0xFF , vl_datagroup_init ) == XDC_NOK )
         {
         XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation des CAM" );
         }

      if (vm_TypeMachine != XZSCC_TYPEM_PCS )
         {
         if ( xzat27_Init_Etats_EqtsIhm( XDC_EQT_ANA , 0xFF , vl_datagroup_init ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation des ANA" );
            }

         if ( xzat27_Init_Etats_EqtsIhm( XDC_EQT_PAU , 0xFF , vl_datagroup_init ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation des PAU" );
            }
         
         if ( xzat27_Init_Etats_EqtsIhm( XDC_EQT_RAD , 0xFF , vl_datagroup_init ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation des RADT" );
            }
         
         if ( xzat27_Init_Etats_EqtsIhm( XDC_EQT_TUB , 0xFF , vl_datagroup_init ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation des TUB" );
            }
         
         if ( xzat27_Init_Etats_EqtsIhm( XDC_EQT_ECH , 0xFF , vl_datagroup_init ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation des PAN" );
            }

         if ( xzat27_Init_Etats_EqtsIhm( XDC_EQT_PAN , 0xFF , vl_datagroup_init ) == XDC_NOK )
            {
            XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation des PAN" );
            }

	  if ( xzat27_Init_Etats_EqtsIhm( XDC_EQT_ZDP, 0xFF , vl_datagroup_init ) == XDC_NOK )
	    {
	      XZST_03EcritureTrace(   XZSTC_WARNING, "Probleme lors de l'initialisation des ZDP");
	    }
	}
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Reinit" );
   return ;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception du message de debut d'envoi des evt et mobiles
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Debut(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
          
     
/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Debut" ;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Debut");
   
   /*A Recuperation des donnees du message */
   if (TipcMsgRead(pa_Data->msg, NULL) )
      {
      /*A Initialise le taritement des evt et mobiles par bloc */
      vm_traitement_bloc = XDC_VRAI;
      }
   else 
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Debut: Impossible de recuperer le message");
      }

   /*MSVU_maj_vue();*/
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Debut" );
   return ;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Reception du message de fin d'envoi des evt et mobiles
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Fin(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
          
     
/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Fin" ;
char vl_msg[50];
   
   if (!Fin_Init_Done){
XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Fin");
	   strcpy(vl_msg, "|0|0");
	   send_message(vl_msg, 50,"");

	   /*A Recuperation des donnees du message */
	   if (TipcMsgRead(pa_Data->msg, NULL) )
		  {
		  /*A Initialise le taritement des evt et mobiles par bloc */
		  vm_traitement_bloc = XDC_FAUX;
		  /*MSRA_Maj_mobiles();*/
		  XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Fin: FIN INIT");
		  }
	   else
		  {
		  XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Fin: Impossible de recuperer le message");
		  }
	   Fin_Init_Done = 1;

XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Fin" );
   }

   /*MSVU_maj_vue();*/
   return ;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Reception du message de raf des etat des alerte
*   Callback sur XDM_ICREER_ALERTE_LOCALISEE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_Etat_Alerte_Localisee(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)

/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{
#ifndef _TIBCO_EMS
   T_INT4 vl_sendpoke;
   T_INT4 vl_canal_MTLL;
   T_STR  pl_alerte;
#else
   XDY_Entier vl_sendpoke;
   XDY_Entier vl_canal_MTLL;
   char  pl_alerte[255];
#endif

   XZST_03EcritureTrace(   XZSTC_DEBUG1, "IN  : MSRA_Recept_Etat_Alerte_Localisee");

   /*A Recuperation des donnees du message */
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4,&vl_sendpoke,
                                T_IPC_FT_INT4,&vl_canal_MTLL,
                                T_IPC_FT_STR,&pl_alerte, NULL) )
      {
      /*A Rafraichissement de l'etat de dispo de l'eqt */
      if ( MSRA15_raf_vag_ALERTE( (char*) pl_alerte ) == XDC_NOK )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Etat_Alerte_Localisee : Proble lors du ref de dispo de l'eqt no " );
         return;
         }
      }
   else
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Etat_Alerte_Localisee: Impossible de recuperer le message");
      return;
      }


   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Recept_Etat_Alerte_Localisee" );
}



/*SDU*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Reception du message de masquage des equipements
*   Callback sur XDM_IMASQUE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void MSRA_Recept_Masque_Projeter(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)


/*
* ARGUMENTS EN ENTREE :
*  Argument d'une callback
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
{static char *version = "@(#)isbi_sra_recept.c	1.31 07/11/07 : MSRA_Recept_Masque" ;
#ifdef _TIBCO_EMS
   XDY_Mot vl_type_eq;
   XDY_Mot vl_etat;
#else
   T_INT2 vl_type_eq;
   T_INT2 vl_etat;
#endif

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSRA_Recept_Masque");

   /*A Recuperation des donnees du message */
   if (TipcMsgRead(pa_Data->msg,T_IPC_FT_INT2,&vl_type_eq, T_IPC_FT_INT2, &vl_etat,NULL) )
      {
      /*A Rafraichissement de l'etat de dispo de l'eqt */
      if ( MSRA18_Masque_Projeter( (XDY_TypeEqt) vl_type_eq, (XDY_TypeEqt) vl_etat) == XDC_NOK )
         {
         XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Masque : Probleme lors du raf du masque" );
         return;
         }
      }
   else
      {
      XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_Recept_Masque: Impossible de recuperer le message");
      return;
      }


   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSRA_Recept_Masque" );
}


/*-----------------------------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction de demande d'initialisation des synoptiques pour un PO Magistra
 ------------------------------------------------------------------------------
 */
void MSRA_init_POMagistra(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
{
char vl_machine[100];

        XZST_03EcritureTrace(XZSTC_FONCTION, "IN : MSRA_init_POMagistra");

        /* Lire le message pour recuperer le nom de la machine qui demande l'initialistation de ses synoptiques */
        if (TipcMsgRead(pa_Data->msg
                ,T_IPC_FT_STR, &vl_machine,NULL) )
        {
        }
        else
        {
                XZST_03EcritureTrace(XZSTC_WARNING,"MSRA_init_POMagistra : Impossible de recuperer le message");
                return;
        }


        /* on envoie toutes les lignes de hashtable qui stocke les messages envoyes */
        iterate_msg( vl_machine );

        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : MSRA_init_POMagistra");

}

