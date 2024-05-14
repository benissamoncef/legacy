/*E*/
/*Fichier : $Id: xzan22sp.h,v 1.1 1994/11/10 10:46:01 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/10 10:46:01 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan22sp.h
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

#ifndef xzan22sp
#define xzan22sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

/* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumCompetence_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Mot		va_NumAstreinte_out
* XDY_Mot		va_NumCategorie_out
* XDY_Mot		va_DureeAstreinte_out
* XDY_Horodate		va_HeureDebut
* XZANT_ListeGeo
*
*/

#define XZAN22_Lire_Competence		XZAN;22

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
