/*E*/
/*Fichier : $Id: xzeg01.c,v 1.1 2012/07/04 10:50:09 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:50:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE XZEG * FICHIER xzeg01.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   cf xzeg01.h
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	15 Fevrier 2012	: Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */
#include "xzeg01.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "@(#)xzeg01.c	1.3 01/11/95 : xzeg01" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  prise en compte d'une nouvelle base de connaissances
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEG01_informer_regulation( XDY_Entier    	va_Num_FMC_in,
				XDY_Entier     	va_Cle_FMC_in,
				XDY_Entier			va_zone_in,
				XDY_Entier			va_scenario_in,
				XDY_Entier		va_Fin_FMC_in,
				char		*va_site_in)


/*
* ARGUMENTS EN ENTREE :
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
*	Envoie le message XDM_EREG_FMC sur le datagroup XDG_EREG_FMC_<Secteur>
*
------------------------------------------------------*/
{
	XDY_Datagroup dg;
	XDY_NomSite	NomSite;

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"IN : XZEG01_informer_regulation, va_Num_FMC_in = %d, va_Cle_FMC_in = %d, va_scenario_in = %d, va_Fin_FMC_in = %d, va_zone_in=%d site=%s",
		va_Num_FMC_in, va_Cle_FMC_in, va_scenario_in, va_Fin_FMC_in, va_zone_in,va_site_in);
	
	sprintf(dg,"%s_%s",XDG_EREG_FMC,va_site_in);
	/*A
	**  ecriture d'un message de type XDM_EREG_FMC sur le datagroup choisi
	*/

	XZST_03EcritureTrace(XZSTC_WARNING,
		"envoi d'un message de type XDM_EREG_FMC sur le datagroup %s", dg);
	if (TipcSrvMsgWrite(dg,
		XDM_IdentMsg(XDM_EREG_FMC),
		XDM_FLG_SRVMSGWRITE,
		T_IPC_FT_INT4, va_Num_FMC_in,
		T_IPC_FT_INT4, va_Cle_FMC_in,
		T_IPC_FT_INT4, va_zone_in,
		T_IPC_FT_INT4, va_scenario_in,
		T_IPC_FT_INT4, va_Fin_FMC_in,
		NULL) != TRUE)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "echec dans l'ecriture du message XDM_EREG_FMC sur le datagroup %s", dg);
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

	XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZEG01_informer_regulation retourne %d", XDC_OK);
	return(XDC_OK);
}


