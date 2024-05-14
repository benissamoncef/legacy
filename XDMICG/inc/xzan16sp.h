/*E*/
/*Fichier : $Id: xzan16sp.h,v 1.1 1994/11/10 10:45:54 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/10 10:45:54 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan16sp.h
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

#ifndef xzan16sp
#define xzan16sp

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

#define XZAN16_Ajouter_Zone_Geographique_Competence	XZAN;16

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
