/* Fichier : xzat81.h
------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR / MAGISTRA
------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------------------------------
* MODULE XZAT * FICHIER xzat81.h
------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Ecriture en base de l'etat d'un Controleur de Feux.
*
------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	10/07/23 : Creation (SAE-440)
----------------------------------------------------------------------------*/


#ifndef xzat81
#define xzat81

	/* Include applicatif Migrazur */
	#include "xdy.h"
	#include "xdc.h"

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZATC_XZAT81_RPC_NAME		"XZAT81"

#define XZATC_XZAT81_NB_PARM_RETURN	0


/* definitions de types exportes */


/* definition de macro exportees */


/* declarations de donnees globales */


/* delaration de fonctions externes */


extern int XZAT81_Ecrire_Etat_CFE (	XDY_Eqt		va_NumEqt_in,
					XDY_Horodate	va_Horodate_in,
					XDY_Octet	va_Fonctionnement_in,
					XDY_Octet	va_Plan_Feux_in);

#endif
