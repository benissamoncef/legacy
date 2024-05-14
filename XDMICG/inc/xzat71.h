/*E*/
/*  Fichier : 	@(#)xzat71.h	1.2  Release : 1.2 Date : 05/03/12
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat71.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	12/01/18 creation GABEB 1.1 DEM1283
------------------------------------------------------ */

#ifndef xzat71
#define xzat71

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZATC_XZAT71_RPC_NAME "XZAT71"

#define XZATC_XZAT71_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */


int XZAT71_Etat_CAB  ( 	XDY_Horodate	va_Horodate_in,
				XDY_Mot		va_NumEqt_in,
				XDY_Octet 	*va_Dispo_out);


#endif
