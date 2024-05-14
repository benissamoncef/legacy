/*E*/
/*Fichier : $Id: xzan35sp.h,v 1.1 1994/11/10 10:54:55 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/10 10:54:55 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan35sp.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
------------------------------------------------------ */

#ifndef xzan35sp
#define xzan35sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

/* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumCategorie_in
* XDY_Octet		va_NumTypeAstreinte_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Horodate		va_DateLimite_in
*
* PARAMETRES EN SORTIE :
*
* XZANT_ListeCal
*
*/

#define XZAN35_Controler_Exhaustivite_Calendrier	XZAN;35

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
