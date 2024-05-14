/*E*/
/*Fichier : $Id: xzam07sp.h,v 1.1 1994/11/10 10:45:13 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/10 10:45:13 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAM * FICHIER xzam07sp.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alarmes >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	04  Nov 1994	: Creation
------------------------------------------------------ */

#ifndef xzam07sp
#define xzam07sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

#define XZAMC_MANUELLE_MINEURE		5
#define XZAMC_MANUELLE_MAJEURE		6
#define XZAMC_MANUELLE_CRITIQUE		7


/* PARAMETRES EN ENTREE :
*
* XDY_Horodate		va_Horodate_in
* XDY_TypeEqt		va_TypeEqt_in
* XDY_Eqt		va_Eqt_in
* XDY_TypeAlarme	va_Type_in
* XDY_Booleen		va_Etat_in
*
* PARAMETRES EN SORTIE :
*
*/

#define XZAM07_Ajout_Alarme	XZAM;07

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
