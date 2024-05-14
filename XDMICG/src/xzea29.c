/*E*/
/* Fichier : @(#)xzea29.c	1.6      Release : 1.6        Date : 12/13/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea29.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux donnees des equipements externes >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	13 Oct 1994	: Creation
* Mismer.D	version 1.2	14 Oct 1994	:
* Mismer.D	version 1.3	26 Oct 1994	: Ajout static version
* Mismer.D	version 1.4	28 Jan 1995	:
* Fontaine.C	version 1.5	24 Mai 1995	: Reprise historique
* Mismer.D	version 1.6	29 Nov 1995	: Correction condition fermeture datagroup d'init
* LCL		20/12/21	: MOVIS P2 ems SAE-312
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea29.c	1.6 12/13/95 : xzea29" ;
XDY_Fonction		pg_FonctUtil_XZEA_29 = NULL;
T_CB			vg_CB_XZEA29         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_Etat_JourNuit ( 
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
*  XDM_ETAT_JourNuit 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea29.c	1.6 12/13/95 : XZEA_Etat_JourNuit ";
   XZEAT_JourNuit	vl_Etat;
#ifndef _TIBCO_EMS
    T_INT2		vl_JourNuit;
#else
    XDY_Mot		vl_JourNuit;
#endif
   int			vl_Resultat = XDC_OK;
   
		  
   /* Recuperation des parametres du message XDM_ETAT_JourNuit */
   if ( !TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_INT2,        &vl_JourNuit,
	       NULL) ) 
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_JourNuit dans XZEA_Etat_JourNuit ");
   }
   else
   {	       
      XZST_03EcritureTrace ( XZSTC_FONCTION, "IN  : XZEA_Etat_JourNuit JourNuit:%d .", vl_JourNuit );

      /*A Renseignement de la structure XZEAT_JourNuit */
      vl_Etat.JourNuit	= vl_JourNuit;
	       
      /*A Appel de la fonction utilsateur avec la structure XZEAT_JourNuit en argument */
      pg_FonctUtil_XZEA_29 ( vl_Etat, &vl_Resultat );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_JourNuit %d", vl_Resultat );
   }
}
/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Abonnement a l'etat Jour ou Nuit >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	XZEA29_Abt_Etat_JourNuit (	XDY_Booleen	va_Abonnement,
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
*   Ce service est demande pour s'abonner a l'etat Jour ou Nuit d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
static char *version = "@(#)xzea29.c	1.6 12/13/95 : XZEA29_Abt_Etat_JourNuit ";
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  : XZEA29_Abt_Etat_JourNuit Abt:%d DGInit:%s Fonct:%ld NomSite:%s.",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomSite );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_29 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_ETAT_JourNuit */
	if ( vg_CB_XZEA29 == NULL )
	{
	   if ( ( vg_CB_XZEA29 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_ETAT_JourNuit),
							      	XZEA_Etat_JourNuit,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_JourNuit." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ETAT_JourNuit." );
	   }
	}
	
	

	/*A Abonnement au Datagroup XDG_ETAT_JOURNUIT<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_JOURNUIT, pa_NomSite );
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
	   /* Envoi du message de demande d'init XDM_EInit_IHM_JourNuit */
           /*A Construction du DatGroup XDG_EInit_IHM_JOURNUIT_<NomSite> */
           XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_JOURNUIT, pa_NomSite );
   	
           /*A Envoi du message XDM_EInit_IHM_JourNuit en fournissant les valeur recues  */
   
   	   if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  		XDM_IdentMsg(XDM_EInit_IHM_JourNuit),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_STR,  pa_DGInit,
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_JourNuit non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_JourNuit.");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_EInit_IHM_JourNuit */    	
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_JourNuit sur %s.", pl_NomDG );
	   }
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA29 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_ETAT_JourNuit */
	   if ( TutCbDestroy ( vg_CB_XZEA29 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_JourNuit ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_ETAT_JourNuit ." );
	      vg_CB_XZEA29 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_ETAT_JOURNUIT<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_JOURNUIT, pa_NomSite );
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
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA29_Abt_Etat_JourNuit Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


