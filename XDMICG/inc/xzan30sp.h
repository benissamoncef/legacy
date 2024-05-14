/*E*/
/*Fichier : $Id: xzan30sp.h,v 1.1 1994/11/10 10:54:46 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/10 10:54:46 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan30sp.h
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

#ifndef xzan30sp
#define xzan30sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

/* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumCompetence_in
* XDY_Horodate		va_Horodate_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Mot		va_NumFicheAnnuaire_out
* XDY_Horodate		va_HoroDebut_out
* XDY_Horodate		va_HoroFin_out
* 
*
*/

#define XZAN30_Rechercher_Fiche_Astreinte		XZAN;30

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
