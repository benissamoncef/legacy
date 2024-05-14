/*E*/
/*Fichier : $Id: xzan21sp.h,v 1.1 1994/11/10 10:45:59 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/10 10:45:59 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan21sp.h
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

#ifndef xzan21sp
#define xzan21sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

/* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumFicheAnnuaire_in
* XDY_TexteAnn		va_NouveauCommentaire_in
* XDY_Horodate		va_NouvelleValiditeCommentaire_in
* XDY_Tel		va_NouveauTelephoneCommentaire_in
*
* PARAMETRES EN SORTIE :
*
*/

#define XZAN21_Commenter_Fiche_Annuaire		XZAN;21

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
