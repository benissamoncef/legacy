/*E*/
/*Fichier : $Id: xzea70.c,v 1.2 2020/11/03 17:57:49 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/11/03 17:57:49 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea70.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux donnees des equipements externes >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		15 Fev 2012	: Creation	 (DEM/1016)
* LCL		20/12/21	: MOVIS P2 ems SAE-312
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzea.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)xzea70.c	1.8 09/09/96 : xzea70" ;
XDY_Fonction		pg_FonctUtil_XZEA_70 = NULL;
T_CB			vg_CB_XZEA70         = NULL;


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZEA_Etat_PRV ( 
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
*  XDM_ETAT_PMV 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version = "@(#)xzea70.c	1.8 09/09/96 : XZEA_Etat_PMV ";
   XZEAT_Etat_PRV	vl_Etat;

#ifndef _TIBCO_EMS
   T_STR		vl_vitesse,vl_bandeau;
   T_INT2		vl_Numero;
#else
  XDY_Ligne_PMV		vl_vitesse,vl_bandeau;
   XDY_Mot		vl_Numero;
#endif
   int			vl_Resultat = XDC_OK;
   XDY_NomMachine vl_machine="";

   /* Recuperation des parametres du message XDM_ETAT_PRV */
   if ( !TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_INT2,        &vl_Numero,
               T_IPC_FT_STR,         &vl_bandeau,
              T_IPC_FT_STR,         &vl_vitesse,
 	       NULL) )
   {
      XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_PRV dans la fonction XZEA_Etat_PRV ");
   }
   else
   {	       
      XZST_03EcritureTrace ( XZSTC_WARNING,
      "IN  : XZEA_Etat_PRV  Numero:%d vitesse:%s bandeau:%s",vl_Numero,
     				vl_vitesse,vl_bandeau );

      /*A Renseignement de la structure XZEAT_Etat_PRV */
      vl_Etat.Numero		= vl_Numero;
      strcpy ( vl_Etat.Vitesse, vl_vitesse );
      strcpy ( vl_Etat.Bandeau, vl_bandeau );

      XZST_03EcritureTrace ( XZSTC_WARNING,
	    "IN  : XZEA_Etat_PRV  Numero:%d vitesse:%s bandeau:%s",
			vl_Etat.Numero, vl_Etat.Vitesse, vl_Etat.Bandeau);

	  if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"XZEA_Etat_PRV : probleme dans recuperation machine destinataire du message");
	  }

      /*A Appel de la fonction utilsateur avec la structure XZEAT_Etat_PRV en argument */
      pg_FonctUtil_XZEA_70 ( vl_Etat, &vl_Resultat, vl_machine );
      XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_PRV %d", vl_Resultat );
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

int	XZEA70_Abt_Etat_PRV (		XDY_Booleen	va_Abonnement,
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
*   Ce service est demande pour s'abonner aux etats des PRV d'un site
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
     "IN  : XZEA70_Abt_Etat_PRV Abt:%d DGInit:%s Fonct:%ld NomSite:%s.",
     		va_Abonnement,
     		pa_DGInit,
     		pa_FonctUtilisateur,
     		pa_NomSite );
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZEA_70 = pa_FonctUtilisateur;
     
     /*A Si demande d'abonnement */
     if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_ETAT_PRV */
			if ( vg_CB_XZEA70 == NULL )
			{
			   if ( ( vg_CB_XZEA70 = TipcSrvProcessCbCreate (	XDM_IdentMsg(XDM_ETAT_PRV),
											XZEA_Etat_PRV,
										(T_CB_ARG)NULL ) ) == NULL )
			   {
				  /*B Ecriture Trace */
				  XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_PRV." );
				  vl_ValRet = XDC_NOK;
			   }
			   else
			   {
				  /*B Ecriture Trace */
				  XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ETAT_PRV." );
			   }
			}

	

			/*A Abonnement au Datagroup XDG_ETAT_PRV<NomSite> */
			/*A Construction du nom du DataGroup */
			sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_PRV, pa_NomSite );
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
					  XZST_03EcritureTrace ( XZSTC_WARNING, "Abonnement au Datagroup %s ", pl_NomDG );
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
					  XZST_03EcritureTrace ( XZSTC_WARNING, "Abonnement au Datagroup %s ", pa_DGInit );
			   }
			}

					/* Envoi du message de demande d'init XDM_EInit_IHM_PRV */
				   /*A Construction du DatGroup XDG_EInit_IHM_PRV_<NomSite> */
				   XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_PRV, pa_NomSite );

				   /*A Envoi du message XDM_EInit_IHM_PRV en fournissant les valeur recues  */
			   if (!TipcSrvMsgWrite ( 	pl_NomDG,
								XDM_IdentMsg(XDM_EInit_IHM_PRV),
							XDM_FLG_SRVMSGWRITE,
										T_IPC_FT_STR,  pa_DGInit,
										NULL))
				   {
				/*A Trace envoi message XDM_EInit_IHM_PRV non effectue */
					XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_PRV.");
			   }
			   else
				   {
				/*A Trace envoi message XDM_EInit_IHM_PRV */
					XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_PRV sur %s.", pl_NomDG );
			   }
     }
     else
     {
        if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA70 != NULL ) )
			{
			   /*A  Desarmement Callback sur Reception XDM_ETAT_PRV */
		   if ( TutCbDestroy ( vg_CB_XZEA70 ) == FALSE )
		   {
			  /*B Ecriture Trace */
			  XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_PRV ." );
			  vl_ValRet = XDC_NOK;
		   }
		   else
		   {
			  XZST_03EcritureTrace ( XZSTC_WARNING, "Desarmement Callback sur XDM_ETAT_PRV ." );
			  vg_CB_XZEA70 = NULL;
		   }
		}

		/*A Desabonnement au Datagroup XDG_ETAT_PRV<NomSite> */
		/*A Construction du nom du DataGroup */
		sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_PRV, pa_NomSite );
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
				  XZST_03EcritureTrace ( XZSTC_WARNING, "Desabonnement au Datagroup %s ", pl_NomDG );
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
				  XZST_03EcritureTrace ( XZSTC_WARNING, "Desabonnement au Datagroup %s ", pa_DGInit );
		   }
		}
     }
     
   XZST_03EcritureTrace ( XZSTC_WARNING, "OUT : XZEA70_Abt_Etat_PRV Retour:%d.", vl_ValRet );
   *pa_Resultat = vl_ValRet;
   return ( vl_ValRet );				
				
}


