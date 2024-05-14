/*E*/
/*Fichier : $Id: xzan11sp.h,v 1.1 1994/11/10 10:45:46 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/10 10:45:46 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan11sp.h
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

#ifndef xzan11sp
#define xzan11sp

/* fichiers inclus */

#include "../../XDMICG/inc/xdc.h"

/* definitions de constantes exportees */

/* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumFicheAnnuaire_in
* XDY_Nom		va_NomInterlocuteur_in
* XDY_Nom		va_Fonction_in
* XDY_Mot		va_NumCompetence_in
* XDY_Phrase		va_Adresse_in
* XDY_Nom		va_ExplicationTel1_in
* XDY_Tel		va_Telephone1_in
* XDY_Nom		va_ExplicationTel2_in
* XDY_Tel		va_Telephone2_in
* XDY_Nom		va_ExplicationTel3_in
* XDY_Tel		va_Telephone3_in
* XDY_Nom		va_ExplicationFax_in
* XDY_Tel		va_Fax_in
* XDY_Nom		va_ExplicationBip_in
* XDY_Tel		va_Bip_in
* XDY_Octet		va_TypeBip_in
* XDY_TexteAnn		va_Commentaire_in
* XDY_Horodate		va_ValiditeCommentaire_in
* XDY_Tel		va_TelephoneCommentaire_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Octet		va_resultat_out (OK fiche ajiner
*					 NOK homonymie - non ajiner)
*/

#define XZAN11_modifier_Fiche_Annuaire	XZAN;11

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
