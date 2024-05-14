/*E*/
/*Fichier : $Id: xzan17sp.h,v 1.1 1994/11/10 10:45:55 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/10 10:45:55 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan17sp.h
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

#ifndef xzan17sp
#define xzan17sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

/* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumCompetence_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PRdebut_in
* XDY_PR		va_PRfin_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Mot 		va_NumZoneGeo_out
*
*/

#define XZAN17_Supprimer_Zone_Geographique_Competence	XZAN;17

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
