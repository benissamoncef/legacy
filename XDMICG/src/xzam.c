/*E*/
/*Fichier : $Id: xzam.c,v 1.8 2017/03/09 17:56:21 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2017/03/09 17:56:21 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE AMCL * FICHIER xzam.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier xzam.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	24 Oct 1994	: Creation
***********************
* MERCIER	09 Nov 1994	: Modification commentaires	V 1.2
***********************
* MERCIER	17 Fev 1995	: Correction de l envoie de msg	V 1.3
* RtWks : on n utilise pas TipcMsgCreate
*********************************************************************
* B.G.		02 oct 1995	: ajout d'un argument nom machine � xzam01 pour acknowledge (1.4)
* JMG   31/10/07        : ajout site de gestion
* JBL        17/10/2008	: modification de XDM_AM_alarme int2 au lieu de char pour le type d'alarme
* JMG        29/09/09   : SECTO  DEM 887
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.8
------------------------------------------------------*/

/* fichiers inclus */

#include "xzam.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzam.c,v 1.8 2017/03/09 17:56:21 pc2dpdy Exp $ : xzam" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	XZAM01_Ajout_Alarme permet de creer une nouvelle fiche
* 	alarme ou met a jour celle qui existait deja.
*	Algo : Recoit les parametres, 
*	constitue un msg RTwks et
* 	envoie le message RTWks XDM_AM_alarme.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAM01_Ajout_Alarme (	XDY_Horodate 	va_Horodate_double_in,
				XDY_TypeEqt	va_TypeEqt_in,
				XDY_Eqt		va_Eqt_in,
				XDY_TypeAlarme	va_TypeAlarme_in,
				XDY_Booleen	va_Etat_in,
				XDY_NomMachine	va_Nom_Machine_in,
				XDY_District	va_SiteGestion_in) 
/*
* ARGUMENTS EN ENTREE :
*	va_Horodate_double_in 
*   	va_TypeEqt_in
*	va_Eqt_in
*	va_TypeAlarme_in
*	va_Etat_in
* 	va_Nom_Machine_in
*	constituent le contenu du message a envoyer sur le DG XDG_AM_NOMSITE.
*
* ARGUMENTS EN SORTIE : aucun
* CODE RETOUR : 
*  XDC_OK si tout s est bien passe
*  sinon renvoie XDC_NOK.
*
* CONDITION D'UTILISATION
*
*	Il faut etre connecte au RTserveur local.
*	
*
* FONCTION 
*	Fonction permettant d envoyer le message XDM_AM_alarme.
*	Le message RTwks doit contenir un XDY_Horodate XDY_TypeEqt XDY_Eqt XDY_TypeAlerte XDY_Booleen.
*	Le message declenchera XZAM;01 une SP n effectuant pas de select et ne prenant pas
*	de parametre de retour.
* 	
------------------------------------------------------*/
{
	static char *	version = "@(#)xzam.c	1.4 10/02/95 : XZAM01_Ajout_Alarme" ;
	
	XDY_Datagroup   vl_datagroup;
	XDY_NomSite 	pl_NomSite;
        T_BOOL		vl_EtatDg = FALSE;
        int		vl_retcode = XDC_NOK;
        T_IPC_MSG 	pl_Msg;
        
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN  : XZAM01_Ajout_Alarme : Debut d execution\n a recu  Horodate = %lf\n TypeEqt = %d\n Eqt =%d\n TypeAlarme = %d\n Etat = %d\nSiteGestion = %d\n",va_Horodate_double_in,va_TypeEqt_in,va_Eqt_in,va_TypeAlarme_in,va_Etat_in,va_SiteGestion_in);
	
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_retcode = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		/*B 
		** Ecriture Trace 
		*/
		XZST_03EcritureTrace( XZSTC_WARNING, "xzam01 : Recuperation du Nom du site impossible .");
		return( XDC_NOK);
	} 	
	/* 
	** Affichage Trace 
	*/
	XZST_03EcritureTrace( XZSTC_DEBUG1, "xzam01 : Recuperation du Nom du site %s.",pl_NomSite);
	
	/*A
	** Constitution du nom Data Group a partir du nom du site
	*/
	
    switch (va_SiteGestion_in) {
     case (XDC_CI) : XDG_EncodeDG2(vl_datagroup,XDG_AM,XDC_NOM_SITE_CI);
                     break;
     case (XDC_VC) : XDG_EncodeDG2(vl_datagroup,XDG_AM,XDC_NOM_SITE_VC);
                     break;
     case (XDC_DP) : XDG_EncodeDG2(vl_datagroup,XDG_AM,XDC_NOM_SITE_DP);
                     break;
     case (XDC_CA) : XDG_EncodeDG2(vl_datagroup,XDG_AM,XDC_NOM_SITE_CA);
                     break;
	default : XDG_EncodeDG2(vl_datagroup,XDG_AM,pl_NomSite);
			break;
    }
	
	XZST_03EcritureTrace ( XZSTC_DEBUG1, "xzam01 : Data group a joindre : %s",vl_datagroup);
	/*A
	** Envoi du message XDM_AM_alarme 
	*/ 
	if (TipcSrvMsgWrite(vl_datagroup,XDM_IdentMsg(XDM_AM_alarme) ,
				XDM_FLG_SRVMSGWRITE,
				T_IPC_FT_REAL8, va_Horodate_double_in,
				T_IPC_FT_CHAR, va_TypeEqt_in,  
				T_IPC_FT_INT2, va_Eqt_in,
				T_IPC_FT_INT2, va_TypeAlarme_in,
				T_IPC_FT_CHAR, va_Etat_in,
				T_IPC_FT_STR, va_Nom_Machine_in,
				NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"xzam01 a reussi a envoyer le msg XDM_AM_alarme" );	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"XZAM01_Ajout_Alarme : TipcSrvMsgWrite a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAM01_Ajout_Alarme a echoue : Sortie");
		return(XDC_NOK);	 
	}
	/*A
	** Fin de l interface xzam01
	*/
	XZST_03EcritureTrace(XZSTC_INTERFACE,"OUT : XZAM01_Ajout_Alarme : Fin d execution");
	return(XDC_OK);
}

