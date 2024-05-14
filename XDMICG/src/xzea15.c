/*E*/
/*Fichier : @(#)xzea15.c	1.7      Release : 1.7        Date : 12/13/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea15.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux donnees des equipements externes >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	12 Sep 1994	: Creation
* Mismer.D      version 1.2	11 Oct 1994	: Modif type XDY_EtatCom
* Mismer.D	version 1.3	14 Oct 1994	:
* Mismer.D	version 1.4	26 Oct 1994	: Ajout static version
* Mismer.D	version 1.5	28 Jan 1995	:
* Fontaine.C	version 1.6	23 Mai 1995	: Reprise historique
* Mismer.D	version 1.7	29 Nov 1995	: Correction condition fermeture datagroup d'init
* LCL		20/12/21	: MOVIS P2 ems SAE-312
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea15.c	1.7 12/13/95 : xzea15" ;
XDY_Fonction		pg_FonctUtil_XZEA_15 = NULL;
T_CB			vg_CB_XZEA15         = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_Etat_Camera ( 
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
*  XDM_ETAT_VIDEO 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea15.c	1.7 12/13/95 : XZEA_Etat_Camera ";
   XZEAT_Etat_Camera	vl_Etat;
#ifndef _TIBCO_EMS
T_INT4		vl_Numero, vl_CodeCmd, vl_CrCmd,
   			vl_NumMoniteur, vl_NumPosition, vl_NumMagneto,
   			vl_EtatCom;
#else
XDY_Entier		vl_Numero, vl_CodeCmd, vl_CrCmd,
   			vl_NumMoniteur, vl_NumPosition, vl_NumMagneto,
   			vl_EtatCom;
#endif
   int			vl_Resultat = XDC_OK;
	XDY_NomMachine vl_machine="";
		  
   /* Recuperation des parametres du message XDM_ETAT_VIDEO */
   if ( !TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_INT4, &vl_Numero,
               T_IPC_FT_INT4, &vl_CodeCmd,
               T_IPC_FT_INT4, &vl_CrCmd,
               T_IPC_FT_INT4, &vl_NumMoniteur,
               T_IPC_FT_INT4, &vl_NumPosition,
               T_IPC_FT_INT4, &vl_NumMagneto,
               T_IPC_FT_INT4, &vl_EtatCom,
	       NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_VIDEO dans XZEA_Etat_Camera ");
   }
   else
   {
      XZST_03EcritureTrace ( XZSTC_FONCTION, "IN  : XZEA_Etat_Camera  Numero:%d Code Cmd:%d CR Cmd:%d Moniteur:%d Position:%d Magn:%d Etat:%d",
       			     vl_Numero, 
       			     vl_CodeCmd, 
       			     vl_CrCmd,
       			     vl_NumMoniteur,
       			     vl_NumPosition,
       			     vl_NumMagneto,
       			     vl_EtatCom);
      /*A Renseignement de la structure XZEAT_Etat_Camera */
      vl_Etat.Numero		= vl_Numero;
      vl_Etat.CodeCmd		= vl_CodeCmd;
      vl_Etat.CrCmd		= vl_CrCmd;
      vl_Etat.NumMoniteur	= vl_NumMoniteur;
      vl_Etat.NumPosition	= vl_NumPosition;
      vl_Etat.NumMagneto	= vl_NumMagneto;
      vl_Etat.EtatCom		= vl_EtatCom;
      	       
  	  if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"XZEA_Etat_Camera : probleme dans recuperation machine destinataire du message");
	  }

      /*A Appel de la fonction utilsateur avec la structure XZEAT_Etat_Camera en argument */
      pg_FonctUtil_XZEA_15 ( vl_Etat, &vl_Resultat, vl_machine );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_Camera %d", vl_Resultat );
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

int	XZEA15_Abt_Etat_Camera (	XDY_Booleen	va_Abonnement,
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
*   Ce service est demande pour s'abonner aux etats des cameras
*   lies au systeme video d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
static char *version = "@(#)xzea15.c	1.7 12/13/95 : XZEA15_Abt_Etat_Camera ";
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  : XZEA15_Abt_Etat_Camera Abt:%d DGInit:%s Fonct:%ld NomSite:%s.",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomSite );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_15 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_ETAT_VIDEO */
	if ( vg_CB_XZEA15 == NULL )
	{
	   if ( ( vg_CB_XZEA15 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_ETAT_VIDEO),
							      	XZEA_Etat_Camera,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_VIDEO." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ETAT_VIDEO." );
	   }
	}
	
	

	/*A Abonnement au Datagroup XDG_EETAT_VIDEO<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_EETAT_VIDEO, pa_NomSite );
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
           /*A Construction du DatGroup XDG_EINIT_ABT_VIDEO_<NomSite> */
           XDG_EncodeDG2 ( pl_NomDG, XDG_EINIT_ABT_VIDEO, pa_NomSite );
   	
           /*A Envoi du message XDM_EINIT_ABT en fournissant les valeur recues  */
   
   	   if (!TipcSrvMsgWrite ( 	pl_NomDG,
      			  		XDM_IdentMsg(XDM_EINIT_ABT),
  			  		XDM_FLG_SRVMSGWRITE,
                          		T_IPC_FT_STR,  pa_DGInit,
                          		T_IPC_FT_INT4, XZECC_TYPE_EQT_VID,
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
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA15 != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDM_ETAT_VIDEO */
	   if ( TutCbDestroy ( vg_CB_XZEA15 ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_VIDEO ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback sur XDM_ETAT_VIDEO ." );
	      vg_CB_XZEA15 = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_EETAT_VIDEO<NomSite> */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s_%s", XDG_EETAT_VIDEO, pa_NomSite );
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
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT : XZEA15_Abt_Etat_Camera Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


