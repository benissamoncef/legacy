/*E*/
/*Fichier : $Id: xzat060.h,v 1.1 2012/07/04 11:02:36 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:02:36 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat060.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
*  VR	21/12/11	: Creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzat060
#define xzat060

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZATC_XZAT060_RPC_NAME "XZAT060"

#define XZATC_XZAT060_NB_PARM_RETURN 25

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAT060_Utilisation_PRV ( 	XDY_Horodate	va_Horodate_in,
				XDY_Mot		,
				XDY_Octet 			*,
				XDY_Bandeau 		*,
				XDY_VitesseRegul 	*,
				XDY_Entier 			*,
				XDY_Octet 			*,
				XDY_Octet 			*,
				XDY_Entier 			* );
				


#endif
