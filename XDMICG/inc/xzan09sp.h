/*E*/
/*Fichier : $Id: xzan09sp.h,v 1.1 1994/11/10 10:45:43 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/10 10:45:43 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan09sp.h
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

#ifndef xzan09sp
#define xzan09sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

/* PARAMETRES EN ENTREE :
*
* XDY_Nom		va_NomInterlocuteur_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Mot		va_NumFicheAnnuaire_in
* XDY_Octet		va_resultat_out (OK fiche ajouter
*					 NOK homogenie - non ajouter)
*
*/

#define XZAN09_Creer_Fiche_Annuaire	XZAN;09

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
