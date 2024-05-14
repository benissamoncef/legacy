/*Fichier : $Id: xzea71.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea71.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface pour l'abonnement aux donnees des equipements externes >
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	23/11/23	: Creation (DEM483)
*
------------------------------------------------------*/

/* fichiers inclus */
#include "xzea.h"

/* definitions de constantes */
/* definitions de types locaux */
/* definition de macro locales */
/* declaration de variables locales */

XDY_Fonction	pg_FonctUtil_XZEA_71 = NULL;
T_CB			vg_CB_XZEA71         = NULL;


/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void XZEA_Etat_PIC (	T_IPC_CONN 					va_Cnx,
						T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
						T_CB_ARG					pa_ARG )
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
*  Fonction appelee lors de la reception du message
*  XDM_ETAT_PIC 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
	XZEAT_Etat_PIC	vl_Etat;

#ifndef _TIBCO_EMS
	T_INT2	vl_Numero;
	T_STR	vl_Bandeau;
	T_STR	vl_Flash;
	T_STR	vl_Picto;
#else
	XDY_Eqt			vl_Numero;	
	XDY_Bandeau_PIC	vl_Bandeau;
	XDY_Booleen		vl_Flash;	
	XDY_Picto_PIC	vl_Picto;
#endif

	int				vl_Resultat = XDC_OK;
	XDY_NomMachine	vl_machine = "";

   /* Recuperation des parametres du message XDM_ETAT_PIC */
   if ( !TipcMsgRead(	pa_Data -> msg,
						T_IPC_FT_INT2,	&vl_Numero,
						T_IPC_FT_STR,	&vl_Bandeau,
						T_IPC_FT_STR,	&vl_Flash,
						T_IPC_FT_STR,	&vl_Picto,
						NULL) )
	{
		XZST_03EcritureTrace ( XZSTC_WARNING," Impossible de lire le MSG XDM_ETAT_PIC dans la fonction XZEA_Etat_PIC ");
	}
	else
	{	       
		XZST_03EcritureTrace (	XZSTC_WARNING, "IN  : XZEA_Etat_PIC Numero:%d Bandeau:%s Flash:%s Picto:%s",
								vl_Numero,
								vl_Bandeau,
								vl_Flash,
								vl_Picto );
		
		/*A Renseignement de la structure XZEAT_Etat_PIC */
		vl_Etat.Numero		= vl_Numero;
		strcpy ( vl_Etat.Bandeau, vl_Bandeau );
		strcpy ( vl_Etat.Flash, vl_Flash );
		strcpy ( vl_Etat.Bandeau, vl_Picto );
		
		XZST_03EcritureTrace ( XZSTC_WARNING, "IN : XZEA_Etat_PIC Numero:%d Bandeau:%s Flash:%s Picto:%s",
								vl_Etat.Numero,
								vl_Etat.Bandeau,
								vl_Etat.Flash,
								vl_Etat.Picto);

		if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"XZEA_Etat_PIC : probleme dans recuperation machine destinataire du message");
		}

		/*A Appel de la fonction utilsateur avec la structure XZEA_Etat_PIC en argument */
		pg_FonctUtil_XZEA_71 ( vl_Etat, &vl_Resultat, vl_machine );
		XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZEA_Etat_PIC %d", vl_Resultat );
	}
}

/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int	XZEA71_Abt_Etat_PIC (	XDY_Booleen		va_Abonnement,
							XDY_Datagroup	pa_DGInit,
							XDY_Fonction	pa_FonctUtilisateur,
							XDY_Nom			pa_NomSite,
							int *			pa_Resultat )
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen	va_Abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Datagroup	pa_DGInit			: Nom du DataGroup d'Init
* XDY_Fonction	pa_FonctUtilisateur	: Nom de la fonction utilisateur
* XDY_Nom		pa_NomSite			: Nom du site demande
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etats des PIC d'un site
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre les Datagroups correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre les Datagroups
*
------------------------------------------------------*/
{
	XDY_Datagroup	pl_NomDG;
	int 			vl_ValRet = XDC_OK;
	T_BOOL			vl_EtatDg = FALSE;

	XZST_03EcritureTrace ( XZSTC_WARNING, "IN  : XZEA71_Abt_Etat_PIC Abt:%d DGInit:%s Fonct:%ld NomSite:%s.",
							va_Abonnement,
							pa_DGInit,
							pa_FonctUtilisateur,
							pa_NomSite );
     		
 	/* Memorisation en globale du nom de la fonction utilisateur */		
 	pg_FonctUtil_XZEA_71 = pa_FonctUtilisateur;
 	
 	/* Si demande d'abonnement */
 	if ( va_Abonnement == XZEAC_DEBUT_ABONNEMENT )
 	{
		/* Armement Callback sur Reception XDM_ETAT_PIC */
		if ( vg_CB_XZEA71 == NULL )
		{
			if ((vg_CB_XZEA71 = TipcSrvProcessCbCreate (XDM_IdentMsg (XDM_ETAT_PIC), XZEA_Etat_PIC, (T_CB_ARG)NULL )) == NULL )
			{
				/*B Ecriture Trace */
				XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ETAT_PIC." );
				vl_ValRet = XDC_NOK;
			}
			else
			{
				/*B Ecriture Trace */
				XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ETAT_PIC." );
			}
		}

		/* Abonnement au Datagroup XDG_ETAT_PIC<NomSite> */
		/* Construction du nom du DataGroup */
		sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_PIC, pa_NomSite );
		/* Joindre ce datagroup s'il n'est pas deja joint */
		TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
		if ( !vl_EtatDg )
		{
			if ( !XDG_JoindreDG( pl_NomDG ) )
			{
				/* Ecriture Trace */
				XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pl_NomDG );
				vl_ValRet = XDC_NOK;
			}
			else
			{
				XZST_03EcritureTrace ( XZSTC_WARNING, "Abonnement au Datagroup %s ", pl_NomDG );
			}
		}
	
		/* Abonnement au Datagroup DGInit passe en parametre */
		/* Joindre ce datagroup s'il n'est pas deja joint */
		TipcSrvSubjectGetSubscribe ( pa_DGInit, &vl_EtatDg );
		if ( ( !vl_EtatDg ) && ( pa_DGInit != NULL ) )
		{
			if ( !XDG_JoindreDG( pa_DGInit ) )
			{
				/* Ecriture Trace */
				XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pa_DGInit );
				vl_ValRet = XDC_NOK;
			}
			else
			{
				XZST_03EcritureTrace ( XZSTC_WARNING, "Abonnement au Datagroup %s ", pa_DGInit );
			}
		}

		/* Envoi du message de demande d'init XDM_EInit_IHM_PIC */
		/* Construction du DatGroup XDG_EInit_IHM_PIC_<NomSite> */
		XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_PIC, pa_NomSite );

		/* Envoi du message XDM_EInit_IHM_PIC en fournissant les valeur recues  */
		if (!TipcSrvMsgWrite ( 	pl_NomDG,
								XDM_IdentMsg(XDM_EInit_IHM_PIC),
								XDM_FLG_SRVMSGWRITE,
								T_IPC_FT_STR,  pa_DGInit,
								NULL))
		{
			/* Trace envoi message XDM_EInit_IHM_PIC non effectue */
			XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_EInit_IHM_PIC.");
		}
		else
		{
			/* Trace envoi message XDM_EInit_IHM_PIC */
			XZST_03EcritureTrace(XZSTC_DEBUG1, "Envoi du message XDM_EInit_IHM_PIC sur %s.", pl_NomDG );
		}
	}
	else
	{
		if ( ( va_Abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZEA71 != NULL ) )
		{
		   /*  Desarmement Callback sur Reception XDM_ETAT_PIC */
		   if ( TutCbDestroy ( vg_CB_XZEA71 ) == FALSE )
		   {
			  /*B Ecriture Trace */
			  XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDM_ETAT_PIC ." );
			  vl_ValRet = XDC_NOK;
		   }
		   else
		   {
			  XZST_03EcritureTrace ( XZSTC_WARNING, "Desarmement Callback sur XDM_ETAT_PIC ." );
			  vg_CB_XZEA71 = NULL;
		   }
		}

		/* Desabonnement au Datagroup XDG_ETAT_PIC<NomSite> */
		/* Construction du nom du DataGroup */
		sprintf ( pl_NomDG, "%s_%s", XDG_ETAT_PIC, pa_NomSite );
		/*A Disjoindre ce datagroup s'il est deja joint */
		TipcSrvSubjectGetSubscribe (pl_NomDG, &vl_EtatDg );
		if (vl_EtatDg )
		{
		   if (!XDG_QuitterDG (pl_NomDG))
		   {
				/* Ecriture Trace */
				XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desabonnement au Datagroup %s ", pl_NomDG );
				vl_ValRet = XDC_NOK;
		   }
		   else
		   {
				XZST_03EcritureTrace ( XZSTC_WARNING, "Desabonnement au Datagroup %s ", pl_NomDG );
		   }
		}
		
		/* Desabonnement au Datagroup DGInit passe en parametre */
		/* Disjoindre ce datagroup s'il est deja joint */
		TipcSrvSubjectGetSubscribe (pa_DGInit, &vl_EtatDg);
		if ((vl_EtatDg) && (pa_DGInit != NULL))
		{
			if (!XDG_QuitterDG (pa_DGInit))
			{
				/* Ecriture Trace */
				XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desabonnement au Datagroup %s ", pa_DGInit );
				vl_ValRet = XDC_NOK;
			}
			else
			{
				XZST_03EcritureTrace ( XZSTC_WARNING, "Desabonnement au Datagroup %s ", pa_DGInit );
			}
		}
	}

	XZST_03EcritureTrace ( XZSTC_WARNING, "OUT : XZEA71_Abt_Etat_PIC Retour:%d.", vl_ValRet );
	*pa_Resultat = vl_ValRet;
	return ( vl_ValRet );
}
