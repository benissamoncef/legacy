/*E*/
/*Fichier : @(#)xzea31.c	1.4      Release : 1.4        Date : 12/13/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea31.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux donnees des equipements externes >
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	12 Fev 1995	: Creation
* Volcic.F	version 1.2 	16 Mar 1995	:
* Fontaine.C	version 1.3	24 Mai 1995	: Reprise entete
* Mismer.D	version 1.4	29 Nov 1995	: Correction condition fermeture datagroup d'init
* LCL		20/12/21	: MOVIS P2 ems SAE-312
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea31.c	1.4 12/13/95 : xzea31" ;
XDY_Fonction		pg_FonctUtil_XZEA_31 = NULL;
T_CB			vg_CB_XZEA31         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Fonction appel�e lors de la r�ception du message XDM_ETAT_LIAISON
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_Etat_Liaison ( 
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
*  XDM_ETAT_LIAISON 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea31.c	1.4 12/13/95 : XZEA_Etat_Liaison ";
   XZEAT_Etat_Liaison	vl_Liaison;
#ifndef _TIBCO_EMS
   T_INT2		vl_Eqt, vl_TypeEqt,vl_Etat;
#else
   XDY_Mot		vl_Eqt, vl_TypeEqt,vl_Etat;
#endif
   int			vl_Resultat = XDC_OK;
   
		  
   /* Recuperation des parametres du message XDM_ETAT_LIAISON */
   if (!TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_INT2,        &vl_Eqt,
               T_IPC_FT_INT2,        &vl_TypeEqt,
               T_IPC_FT_INT2,	     &vl_Etat,
	       NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_LIAISON dans XZEA_Etat_Liaison ");
   }
   else
   { 
      XZST_03EcritureTrace ( XZSTC_FONCTION, 
      "IN  : %s : Eqt:%d Type_Eqt:%d Etat_Liaison:%d ",version, vl_Eqt, vl_TypeEqt, vl_Etat );

      /*A Renseignement de la structure XZEAT_Etat_Liaison */
      vl_Liaison.Eqt		= vl_Eqt;
      vl_Liaison.TypeEqt	= vl_TypeEqt;
      vl_Liaison.Etat		= vl_Etat;
	       
      /*A Appel de la fonction utilsateur avec la structure XZEAT_Etat_Liaison en argument */
      pg_FonctUtil_XZEA_31 ( vl_Liaison, &vl_Resultat );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : %s : %d", version, vl_Resultat );
      
   }
}

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Ce service est demande pour s'abonner aux etats des Liaison d'une Machine
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEA31_Abt_Etat_Liaison (	XDY_Booleen	va_Abonnement,
					XDY_Datagroup	pa_DGInit,
					XDY_Fonction	pa_FonctUtilisateur,
					XDY_NomMachine	pa_NomMachine,
					int *		pa_Resultat )

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen		va_Abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup		pa_DGInit		: Nom du DataGroup d'Init
* XDY_Fonction		pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XDY_NomMachine	pa_NomMachine		: Nom de la Machine demandee
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etats des Liaisons d'une Machine
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
static char *version = "@(#)xzea31.c	1.4 12/13/95 : XZEA31_Abt_Etat_Liaison";
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  : XZEA31_Abt_Etat_Liaison Abt:%d DGInit:%s Fonct:%ld NomMachine:%s.",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomMachine );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_31 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_ETAT_LIAISON */
	if ( vg_CB_XZEA31 == NULL )
	{
	   if ( ( vg_CB_XZEA31 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_ETAT_LIAISON),
							      	XZEA_Etat_Liaison,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_LIAISON." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ETAT_LIAISON." );
	   }
	}
	
	

	/*A Abonnement au Datagroup XDG_ETAT_LIAISON<NomMachine> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_LIAISON, pa_NomMachine );
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
	   /* Envoi du message de demande d'init XDM_EInit_IHM_Liaison */
           /*A Construction du DatGroup XDG_EInit_IHM_Liaison_<NomMachine> */
           XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_Liaison, pa_NomMachine );
   	
           /*A Envoi du message XDM_EInit_IHM_Liaison en fournissant les valeur recues  */
   
   	   if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  		XDM_IdentMsg(XDM_EInit_IHM_Liaison),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_STR,  pa_DGInit,
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_Liaison non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_Liaison.");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_Liaison */    	
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_Liaison sur %s.", pl_NomDG );
	   }
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA31 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_ETAT_LIAISON */
	   if ( TutCbDestroy ( vg_CB_XZEA31 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_LIAISON ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_ETAT_LIAISON ." );
	      vg_CB_XZEA31 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_ETAT_LIAISON<NomMachine> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_LIAISON, pa_NomMachine );
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
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA31_Abt_Etat_Liaison Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
			
}


