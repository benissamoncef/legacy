/*E*/
/*Fichier : $Id: xzea084.c,v 1.1 2021/05/03 14:09:18 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2021/05/03 14:09:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea084.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux donnees des equipements externes >
*
------------------------------------------------------
* HISTORIQUE :
* ABE	16/12/20	creation IMU 1.1 DEM-SAE155
* LCL		20/12/21	: MOVIS P2 ems SAE-312
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea084.c	1.2 1.2 03/26/07 : xzea084" ;
XDY_Fonction		pg_FonctUtil_XZEA_084 = NULL;
T_CB			vg_CB_XZEA084         = NULL;

/* declaration de fonctions internes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_Etat_IMU (
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG )
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appel�e lors de la r�ception du message
*  XDM_ETAT_IMU 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea082.c	1.8 09/09/96 : XZEA_Etat_IMU ";
   XZEAT_Etat_IMU	vl_Etat;
   int			vl_Resultat = XDC_OK;
#ifdef _TIBCO_EMS
   XDY_Mot		vl_Numero;
   char			vl_chaine_imu[255];
   XDY_Entier		vl_Intervalle;
   XDY_Entier		vl_Cycles;
   XDY_Entier		vl_Duree;
   XDY_Entier		vl_NoCmd;
   T_STR		vl_LibellePlanDeFeux;
#else
   T_INT2		vl_Numero;
   T_STR		vl_chaine_imu;
   T_INT4		vl_Intervalle;
   T_INT4		vl_Cycles;
   T_INT4		vl_Duree;
   T_INT4		vl_NoCmd;
   XDY_Phrase		vl_LibellePlanDeFeux;
#endif
   XDY_NomMachine vl_machine="";
   
   /* Recuperation des parametres du message XDM_ETAT_IMU */
   	if ( !TipcMsgRead(pa_Data -> msg,             
		T_IPC_FT_INT2, &vl_Numero,
		T_IPC_FT_STR,  &vl_chaine_imu,
		T_IPC_FT_INT4, &vl_Intervalle,
		T_IPC_FT_INT4, &vl_Cycles,
		T_IPC_FT_INT4, &vl_Duree,
		T_IPC_FT_INT4, &vl_NoCmd,
		NULL))
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_IMU dans XZEA_Etat_IMU ");
   }
   else
   {
	XZST_03EcritureTrace ( XZSTC_WARNING, "chaine=%s",vl_chaine_imu);
      vl_Etat.Numero		= vl_Numero;
      strcpy(vl_Etat.ChaineIMU,vl_chaine_imu);
      vl_Etat.Intervalle	= vl_Intervalle;
      vl_Etat.Cycles 		= vl_Cycles;
      vl_Etat.Duree 		= vl_Duree;
      XZST_03EcritureTrace ( XZSTC_WARNING, 
      "IN  : XZEA_Etat_IMU  Numero:%d ChaineImu:%s Intervalle:%d Cycles:%d Duree:%d NoCmd:%d \n",
      	  	  	  	  	  vl_Etat.Numero,
      	  	  	  	  	  vl_Etat.ChaineIMU,
					  vl_Etat.Intervalle,
					  vl_Etat.Cycles,
					  vl_Etat.Duree,
					  vl_NoCmd);

	  if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"XZEA_Etat_IMU : probleme dans recuperation machine destinataire du message");
	  }

      /*A Appel de la fonction utilsateur avec la structure XZEAT_Etat_IMU en argument */
      pg_FonctUtil_XZEA_084 ( vl_Etat, &vl_Resultat, vl_machine );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_IMU %d", vl_Resultat );
   }
}
/* definition de fonctions externes */


int	XZEA084_Abt_Etat_IMU (		XDY_Booleen	va_Abonnement,
					XDY_Datagroup	pa_DGInit,
					XDY_Fonction	pa_FonctUtilisateur,
					XDY_Nom		pa_NomSite,
					int *		pa_Resultat )

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen	va_Abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup	pa_DGInit		: Nom du DataGroup d'Init
* XDY_Fonction	pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XDY_Nom	pa_NomSite		: Nom du site demande
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etats des IMU d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

     XZST_03EcritureTrace ( XZSTC_WARNING, 
     "IN  : XZEA084_Abt_Etat_IMU Abt:%d DGInit:%s Fonct:%ld NomSite:%s.",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomSite );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_084 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_ETAT_IMU */
	if ( vg_CB_XZEA084 == NULL )
	{
	   if ( ( vg_CB_XZEA084 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_ETAT_IMU),
							      	XZEA_Etat_IMU,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_IMU." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ETAT_IMU." );
	   }
	}
	
	

	/*A Abonnement au Datagroup XDG_ETAT_IMU<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_IMU, pa_NomSite );
   	/*A Joindre ce datagroup s'il n'est pas deja joint */
   	TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
   	if ( !vl_EtatDg )
   	{
   	   if ( !XDG_JoindreDG( pl_NomDG ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pl_NomDG );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Abonnement au Datagroup %s ", pl_NomDG );
	   }
	}
	
	/*A Abonnement au Datagroup DGInit passe en parametre */
   	/*A Joindre ce datagroup s'il n'est pas deja joint */
   	TipcSrvSubjectGetSubscribe ( pa_DGInit, &vl_EtatDg );
   	if ( ( !vl_EtatDg ) && ( pa_DGInit != NULL ) )
   	{
   	   if ( !XDG_JoindreDG( pa_DGInit ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pa_DGInit );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Abonnement au Datagroup %s ", pa_DGInit );
	   }
	}
	   /* Envoi du message de demande d'init XDM_EInit_IHM_IMU */
           /*A Construction du DatGroup XDG_EInit_IHM_IMU_<NomSite> */
           XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_IMU, pa_NomSite );
   	
           /*A Envoi du message XDM_EInit_IHM_IMU en fournissant les valeur recues  */
   
   	   if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  		XDM_IdentMsg(XDM_EInit_IHM_IMU),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_STR,  pa_DGInit,
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_IMU non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_IMU.");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_IMU */    	
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_IMU sur %s.", pl_NomDG );
	   }
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA084 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_ETAT_IMU */
	   if ( TutCbDestroy ( vg_CB_XZEA084 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_IMU ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_ETAT_IMU ." );
	      vg_CB_XZEA084 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_ETAT_IMU<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_IMU, pa_NomSite );
   	/*A Disjoindre ce datagroup s'il est deja joint */
   	TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
   	if ( vl_EtatDg )
   	{
   	   if ( !XDG_QuitterDG( pl_NomDG ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desabonnement au Datagroup %s ", pl_NomDG );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desabonnement au Datagroup %s ", pl_NomDG );
	   }
	}
	
	/*A Desabonnement au Datagroup DGInit passe en parametre */
   	/*A Disjoindre ce datagroup s'il est deja joint */
   	TipcSrvSubjectGetSubscribe ( pa_DGInit, &vl_EtatDg );
   	if ( ( vl_EtatDg ) && ( pa_DGInit != NULL ) )
   	{
   	   if ( !XDG_QuitterDG( pa_DGInit ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desabonnement au Datagroup %s ", pa_DGInit );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desabonnement au Datagroup %s ", pa_DGInit );
	   }
	}
     }
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA084_Abt_Etat_IMU Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


