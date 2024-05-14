/*E*/
/*Fichier : @(#)xzea30.c	1.6	      Release : 1.6        Date : 12/13/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea30.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux donnees des equipements externes >
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	version 1.1	04 Jan 1995	: Creation
* Nouaille.M	version 1.2	04 Jan 1995	:
* Nouaille.M	version 1.3	04 Jan 1995	:
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

static char *version = "@(#)xzea30.c	1.4 10/28/94 : xzea30" ;
XDY_Fonction		pg_FonctUtil_XZEA_30 = NULL;
T_CB			vg_CB_XZEA30         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_Etat_OPERATOR ( 
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
*  XDM_ETAT_OPER 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "%w% 12/13/95 : XZEA_Etat_OPERATOR ";
   XZEAT_Etat_Operator	vl_Etat;
   int			vl_Resultat = XDC_OK;
#ifndef _TIBCO_EMS
   T_INT4		vl_Numero, vl_CodeCmd, vl_CrCmd, vl_EtatCom;
   T_STR		vl_NumOperateur, vl_MessageEnvoye;
#else
   XDY_Entier		vl_Numero, vl_CodeCmd, vl_CrCmd, vl_EtatCom;
   XDY_Phrase          vl_NumOperateur, vl_MessageEnvoye;
#endif
   XDY_NomMachine vl_machine="";
   
		  
   /* Recuperation des parametres du message XDM_ETAT_OPER */
   if ( !TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_INT4, &vl_Numero,
               T_IPC_FT_INT4, &vl_CodeCmd,
               T_IPC_FT_STR,  &vl_NumOperateur,
               T_IPC_FT_STR,  &vl_MessageEnvoye,
               T_IPC_FT_INT4, &vl_CrCmd,
               T_IPC_FT_INT4, &vl_EtatCom,
	       NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_OPER dans XZEA_Etat_OPERATOR ");
   }
   else
   {
      XZST_03EcritureTrace ( XZSTC_FONCTION, "IN  : XZEA_Etat_OPERATOR  Numero:%d Code Cmd:%d Telephone:%s Repondeur:%s CR Cmd:%d Etat:%d",
       			     vl_Numero, 
       			     vl_CodeCmd, 
       			     vl_NumOperateur, 
       			     vl_MessageEnvoye,
       			     vl_CrCmd,
       			     vl_EtatCom );
      /*A Renseignement de la structure XZEAT_Etat_OPERATOR */

      vl_Etat.Numero	= vl_Numero;
      vl_Etat.CodeCmd	= vl_CodeCmd;
      strcpy ( vl_Etat.NumOper, vl_NumOperateur );
      strcpy ( vl_Etat.Msg, vl_MessageEnvoye );
      vl_Etat.CptRendu	= vl_CrCmd;
      vl_Etat.EtatCom   = vl_EtatCom;

	  if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"XZEA_Etat_OPERATOR : probleme dans recuperation machine destinataire du message");
	  }
      	       
      /*A Appel de la fonction utilsateur avec la structure XZEAT_Etat_OPERATOR en argument */
      pg_FonctUtil_XZEA_30 ( vl_Etat, &vl_Resultat, vl_machine );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_OPERATOR %d", vl_Resultat );
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

int	XZEA30_Abt_Etat_OPERATOR (	XDY_Booleen	va_Abonnement,
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
*   Ce service est demande pour s'abonner aux etats des liaison avec
*   le service OPERATOR d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
static char *version = "%w% 12/13/95 : XZEA30_Abt_Etat_OPERATOR ";
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  : XZEA30_Abt_Etat_OPERATOR Abt:%d DGInit:%s Fonct:%ld NomSite:%s.",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomSite );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_30 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_ETAT_OPER */
	if ( vg_CB_XZEA30 == NULL )
	{
	   if ( ( vg_CB_XZEA30 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_ETAT_OPER),
							      	XZEA_Etat_OPERATOR,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_OPER." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ETAT_OPER." );
	   }
	}
	
	

	/*A Abonnement au Datagroup XDG_EETAT_OPER<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_EETAT_OPER, pa_NomSite );
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
	   /* Envoi du message de demande d'init XDM_EINIT_ABT */
           /*A Construction du DatGroup XDG_EINIT_ABT_OPER_<NomSite> */
           XDG_EncodeDG2 ( pl_NomDG, XDG_EINIT_ABT_OPER, pa_NomSite );
   	
           /*A Envoi du message XDM_EINIT_ABT en fournissant les valeur recues  */
   
   	   if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  		XDM_IdentMsg(XDM_EINIT_ABT),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_STR,  pa_DGInit,
                          		T_IPC_FT_INT4, XZECC_TYPE_EQT_OPE,
                          		T_IPC_FT_INT4, XZECC_TYPE_EQT_ALL,
                          		NULL))
      	   {
  		/*A Trace envoi message XDM_EINIT_ABT non effectue */    	
         	XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EINIT_ABT.");
	   }
	   else
      	   {
  		/*A Trace envoi message XDM_EINIT_ABT */    	
         	XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EINIT_ABT sur %s.", pl_NomDG );
	   }
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA30 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_ETAT_OPER */
	   if ( TutCbDestroy ( vg_CB_XZEA30 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_OPER ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_ETAT_OPER ." );
	      vg_CB_XZEA30 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_EETAT_OPER<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_EETAT_OPER, pa_NomSite );
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
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA30_Abt_Etat_OPERATOR Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


