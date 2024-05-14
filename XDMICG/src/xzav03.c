/*E*/
/*Fichier : $Id: xzav03.c,v 1.1 2012/07/04 10:49:32 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:49:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZEG * FICHIER xzav03.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   cf xzav03.h
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	16 Fevrier 2012	: Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */
#include "xzav.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "@(#)xzav03.c	1.3 01/11/95 : xzav03" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  service de demande de calcul d'un plan d'actions
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAV03_Demarrer_Simulation(XDY_Horodate Horodate_Debut,XDY_Horodate Horodate_Fin, XDY_Eqt NumPosteOperateur, XDY_NomMachine NomPosteOperateur)
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
*	envoi d'un message de type XDM_REGUL_START_SIMU sur le datagroup XDG_REGUL_START_SIMU
*
------------------------------------------------------*/
{
	char		dg[20];
	XDY_NomSite	NomSite;

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"IN : XZAV03_Demarrer_Simulation, Horodate_Debut = %lf, Horodate_Fin = %lf, NumPosteOperateur = %d, NomPosteOperateur = %s",
		Horodate_Debut, Horodate_Fin, NumPosteOperateur, NomPosteOperateur);

	/*A
	**  ecriture d'un message de type XDM_REGUL_START_SIMU sur le datagroup choisi
	*/
	XZST_03EcritureTrace(XZSTC_WARNING,
		"envoi d'un message de type XDM_REGUL_START_SIMU sur le datagroup %s", dg);
	if (TipcSrvMsgWrite(XDG_REGUL_START_SIMU,
		XDM_IdentMsg(XDM_REGUL_START_SIMU),
		XDM_FLG_SRVMSGWRITE,
		T_IPC_FT_REAL8, Horodate_Debut,
		T_IPC_FT_REAL8, Horodate_Fin,
		T_IPC_FT_INT2, NumPosteOperateur,
		T_IPC_FT_STR, NomPosteOperateur,
		NULL) != TRUE)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "echec dans l'ecriture du message XDM_REGUL_START_SIMU");
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

	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAV03_Demarrer_simulation retourne %d", XDC_OK);
	return(XDC_OK);
}


