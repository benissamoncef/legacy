/*E*/
/*Fichier : $Id: xzea083.c,v 1.2 2020/11/03 17:57:33 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/11/03 17:57:33 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea083.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux donnees des equipements externes >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	16/05/18	creation GABEB 1.1 DEM1283 
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* LCL		02/06/23	: Correctif parametre vl_machine sur appel pg_FonctUtil_XZEA_XX
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea083.c	1.2 1.2 03/26/07 : xzea083" ;
XDY_Fonction		pg_FonctUtil_XZEA_083 = NULL;
T_CB			vg_CB_XZEA083         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_Etat_CAP ( 
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
*  XDM_ETAT_CAP 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea083.c	1.8 09/09/96 : XZEA_Etat_CAP ";
   XZEAT_Etat_CAP	vl_Etat;
   int			vl_Resultat = XDC_OK;
#ifndef _TIBCO_EMS
   T_INT2		vl_Numero, vl_NoCmd;
   T_INT4		vl_PlanDeFeux;
   T_STR                vl_LibellePlanDeFeux;
#else
   XDY_Mot		vl_Numero, vl_NoCmd;
   XDY_Entier		vl_PlanDeFeux;
   XDY_Phrase           vl_LibellePlanDeFeux;
#endif
   XDY_NomMachine vl_machine="";
   
		  
   /* Recuperation des parametres du message XDM_ETAT_CAP */

   if ( !TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_INT2,        &vl_Numero,
               T_IPC_FT_INT4,         &vl_PlanDeFeux,
               T_IPC_FT_STR,        &vl_LibellePlanDeFeux,
               NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_CAP dans XZEA_Etat_CAP ");
   }
   else
   {	       
      XZST_03EcritureTrace ( XZSTC_WARNING, 
      "IN  : XZEA_Etat_CAP  Numero:%d PlanDeFeux:%d \n",
      	  	  	  	  	  vl_Numero,
      	  	  	  	  	  vl_PlanDeFeux
      	  	  	  	  	  );

      /*A Renseignement de la structure XZEAT_Etat_CAP */
      vl_Etat.Numero		= vl_Numero;
      vl_Etat.NoPlanDeFeux = vl_PlanDeFeux;
      strcpy(vl_Etat.LibellePlanDeFeux, "");

	  if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
	   XZST_03EcritureTrace(XZSTC_WARNING,"XZEA_Etat_CAP : probleme dans recuperation machine destinataire du message");
	  }

      /*A Appel de la fonction utilsateur avec la structure XZEAT_Etat_CAP en argument */
      pg_FonctUtil_XZEA_083 ( vl_Etat, &vl_Resultat, vl_machine );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_CAP %d", vl_Resultat );
   }
}
/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEA083_Abt_Etat_CAP (		XDY_Booleen	va_Abonnement,
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
*   Ce service est demande pour s'abonner aux etats des CAP d'un site
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
     "IN  : XZEA083_Abt_Etat_CAP Abt:%d DGInit:%s Fonct:%ld NomSite:%s.",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomSite );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_083 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_ETAT_CAP */
	if ( vg_CB_XZEA083 == NULL )
	{
	   if ( ( vg_CB_XZEA083 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_ETAT_CAP),
							      	XZEA_Etat_CAP,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_CAP." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ETAT_CAP." );
	   }
	}
	
	

	/*A Abonnement au Datagroup XDG_ETAT_CAP<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_CAP, pa_NomSite );
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
	   /* Envoi du message de demande d'init XDM_EInit_IHM_CAP */
           /*A Construction du DatGroup XDG_EInit_IHM_CAP_<NomSite> */
           XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_CAP, pa_NomSite );
   	
           /*A Envoi du message XDM_EInit_IHM_CAP en fournissant les valeur recues  */
   
   	   if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  		XDM_IdentMsg(XDM_EInit_IHM_CAP),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_STR,  pa_DGInit,
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_CAP non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_CAP.");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_CAP */    	
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_CAP sur %s.", pl_NomDG );
	   }
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA083 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_ETAT_CAP */
	   if ( TutCbDestroy ( vg_CB_XZEA083 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_CAP ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_ETAT_CAP ." );
	      vg_CB_XZEA083 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_ETAT_CAP<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_CAP, pa_NomSite );
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
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA083_Abt_Etat_CAP Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


