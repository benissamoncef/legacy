/*E*/
/*Fichier : $Id: xzan05sp.h,v 1.1 1994/11/10 10:45:34 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/10 10:45:34 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan05sp.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* <  >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
------------------------------------------------------ */

#ifndef xzan05sp
#define xzan05sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

/* PARAMETRES EN ENTREE :
*
* XDY_Octet		va_NumAstreinte_in
* XDY_Octet		va_NumCategorie_in
* XDY_Mot		va_DureeAstreinte_in
* XDY_Horodate		va_HeureDebut_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Mot		va_NumCompetence_out
*
*/

#define XZAN05_Creer_Competence	XZAN;05

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
