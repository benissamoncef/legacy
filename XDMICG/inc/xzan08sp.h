/*E*/
/*Fichier : $Id: xzan08sp.h,v 1.1 1994/11/10 10:45:41 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/10 10:45:41 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan08sp.h
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

#ifndef xzan08sp
#define xzan08sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

#define XZANC_CAL_NON_VIDE	3

/* PARAMETRES EN ENTREE :
*
* XDY_Octet		va_NumCompetence_in
*
* PARAMETRES EN SORTIE :

* XDY_Entier		va_resultat_out (OK ou calendrier non vide)
*
*/

#define XZAN08_Supprimer_Competence	XZAN;08

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
