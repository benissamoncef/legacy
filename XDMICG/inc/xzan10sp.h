/*E*/
/*Fichier : $Id: xzan10sp.h,v 1.1 1994/11/10 10:45:44 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/10 10:45:44 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan10sp.h
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

#ifndef xzan10sp
#define xzan10sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

/* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumFicheAnnuaire_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Nom		va_NomInterlocuteur_out
* XDY_Nom		va_Fonction_out
* XDY_Mot		va_NumCompetence_out
* XDY_Phrase		va_Adresse_out
* XDY_Nom		va_ExplicationTel1_out
* XDY_Tel		va_Telephone1_out
* XDY_Nom		va_ExplicationTel2_out
* XDY_Tel		va_Telephone2_out
* XDY_Nom		va_ExplicationTel3_out
* XDY_Tel		va_Telephone3_out
* XDY_Nom		va_ExplicationFax_out
* XDY_Tel		va_Fax_out
* XDY_Nom		va_ExplicationBip_out
* XDY_Tel		va_Bip_out
* XDY_Octet		va_TypeBip_out
* XDY_TexteAnn		va_Commentaire_out
* XDY_Horodate		va_ValiditeCommentaire_out
* XDY_Tel		va_TelephoneCommentaire_out
*
*/

#define XZAN10_Lire_Fiche_Annuaire	XZAN;10

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
