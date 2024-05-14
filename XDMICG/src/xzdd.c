/*E*/
/*Fichier : $Id: xzdd.c,v 1.4 2011/04/04 15:34:08 pc2dpdy Exp $	      Release : $Revision: 1.4 $        Date : $Date: 2011/04/04 15:34:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZDD * FICHIER xzdd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   cf xzdd.h
*
------------------------------------------------------
* HISTORIQUE :
*
* IT	09 Sep 1994	: Creation
* IT	14 Dec 1994	: Ajout du service XZDD01	V 1.2
* IT	11 Jan 1995	: Mise a jour entetes		V 1.3
* JMG	21/12/10	: message part sur le serveur si appel depuis PO Linux V1.4
------------------------------------------------------*/

/* fichiers inclus */
#include "xzdd.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "$Id: xzdd.c,v 1.4 2011/04/04 15:34:08 pc2dpdy Exp $ : xzdd" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  prise en compte d'une nouvelle base de connaissances
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern XDY_Booleen XZDD01_PrendreEnCompteBDC(NomSite)
XDY_NomSite	NomSite;

/*
* ARGUMENTS EN ENTREE :
*   XDY_NomSite	NomSite; (XZDDC_DIFF_SITE_DEV pour le poste de developpement)
*
* ARGUMENTS EN SORTIE :
*   aucun.
*
* CODE RETOUR : 
*   XDC_OK / XDC_NOK 
*
* CONDITION D'UTILISATION
*   a appeler une fois la nouvelle base de connaissances diffusee.
*
* FONCTION 
*   envoi d'un message de type XDM_DIN_BDC au Rtie concerne
*
------------------------------------------------------*/
{
	char	dg[XDC_Lon_Datagroup];

	XZST_03EcritureTrace(XZSTC_INTERFACE, "IN : XZDD01_PrendreEnCompteBDC, NomSite = %s", NomSite);
	
	/*A
	**  construction du datagroup en fonction du site 
	*/
#ifdef _HPUX_SOURCE
	strcpy(dg, "XDG_IE_");
	strcat(dg, NomSite);
#else
	strcpy(dg, "XDG_LX_IE_");
	strcat(dg, NomSite);
#endif
	/*A
	**  ecriture d'un message de type XDM_DIN_BDC sur le datagroup choisi
	*/
	XZST_03EcritureTrace(XZSTC_WARNING, "envoi d'un message de type XDM_DIN_BDC sur le datagroup %s", dg);
	if (TipcSrvMsgWrite(	dg,
				XDM_IdentMsg(XDM_DIN_BDC),
				XDM_FLG_SRVMSGWRITE,
				NULL) != TRUE)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "echec dans l'ecriture du message XDM_DIN_BDC sur le datagroup %s", dg);
		return(XDC_NOK);
	}

	/*A
	**  appel de TipcSrvFlush()
	*/
	if (TipcSrvFlush() != TRUE)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "TipcSrvFlush retourne %d", XDC_NOK);
		return(XDC_NOK);
	}

	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZDD01_PrendreEnCompteBDC retourne %d", XDC_OK);
	return(XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  service de demande de calcul d'un plan d'actions
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern XDY_Booleen XZDD02_CalculerPA(Horodate, NumPosteOperateur, NomPosteOperateur, NumEvt, TypeEvt, TypePA, Mode)
XDY_Horodate	Horodate;
XDY_Eqt		NumPosteOperateur;
XDY_NomMachine	NomPosteOperateur;
XDY_Evt		NumEvt;		
XDY_Octet	TypeEvt;	
XDY_Octet	TypePA;
XDY_Octet	Mode;

/*
* ARGUMENTS EN ENTREE :
*	XDY_Horodate	Horodate;
*	XDY_Eqt		NumPosteOperateur	poste operateur pour lequel le PA est calcule;
*	XDY_NomMachine	NomPosteOperateur;
*	XDY_Evt		NumEvt			numero de l'evt	pour lequel le PA est calcule;
*	XDY_Octet	TypeEvt			type de l'evt pour lequel le PA est calcule;
*	XDY_Octet	TypePA			type de PA : avec ou sans calcul du message TFM;
*	XDY_Octet	Mode			mode operationnel ou mode test;
*
* ARGUMENTS EN SORTIE :
*	aucun.
*
* CODE RETOUR : 
*	XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*	appeler une fois la fiche main courante stockee en base.
*
* FONCTION 
*	choix du RTie a joindre en fonction du mode
*	envoi d'un message de type XDM_DIN_PA au RTie choisi.
*
------------------------------------------------------*/
{
	char		dg[20];
	XDY_NomSite	NomSite;

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"IN : XZDD02_CalculerPA, Horodate = %lf, NumPosteOperateur = %d, NomPosteOperateur = %s, NumEvt = %d, NumEvtCle = %d, TypeEvt = %d, TypePA = %d, Mode = %d",
		Horodate, NumPosteOperateur, NomPosteOperateur, NumEvt.numero, NumEvt.cle, TypeEvt, TypePA, Mode);

	/*A
	**  choix du datagroup en fonction du mode :
	*/
	switch (Mode) {

	case XZDDC_MODE_TEST :
		/*B
		**  XDG_IE_DEV en mode test
		*/
		strcpy(dg, "XDG_IE_DEV");
		break;

	case XZDDC_MODE_OPERATIONNEL :
		/*B
		**  XDG_IE_NomSite en mode operationnel
		*/
#ifdef _HPUX_SOURCE
	strcpy(dg, "XDG_IE_");
#else
	strcpy(dg, "XDG_LX_IE_");
#endif
		if (XZSC_04NomSite(NomSite) == XDC_NOK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "Nom site inconnu");
			return(XDC_NOK);
		}
		strcat(dg, NomSite);
		break;

	default :
		XZST_03EcritureTrace(XZSTC_WARNING, "Mode non prevu : %d", Mode);
		return(XDC_NOK);
	}

	/*A
	**  ecriture d'un message de type XDM_DIN_PA sur le datagroup choisi
	*/
	XZST_03EcritureTrace(XZSTC_WARNING,
		"envoi d'un message de type XDM_DIN_PA sur le datagroup %s", dg);
	if (TipcSrvMsgWrite(dg,
		XDM_IdentMsg(XDM_DIN_PA),
		XDM_FLG_SRVMSGWRITE,
		T_IPC_FT_REAL8, Horodate,
		T_IPC_FT_INT2, NumPosteOperateur,
		T_IPC_FT_STR, NomPosteOperateur,
		T_IPC_FT_INT4, NumEvt.numero,
		T_IPC_FT_CHAR, (char) NumEvt.cle,
		T_IPC_FT_CHAR, (char) TypeEvt,
		T_IPC_FT_CHAR, (char) TypePA,
		NULL) != TRUE)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "echec dans l'ecriture du message XDM_DIN_PA");
		return(XDC_NOK);
	}

	/*A
	**  appel de TipcSrvFlush()
	*/
	if (TipcSrvFlush() != TRUE)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "TipcSrvFlush retourne %d", XDC_NOK);
		return(XDC_NOK);
	}

	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZDD02_CalculerPA retourne %d", XDC_OK);
	return(XDC_OK);
}


