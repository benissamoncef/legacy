/*E*/
/* Fichier : $Id: ITMA_TFA.h,v 1.5 1995/04/07 12:06:13 lecrivain Exp $      Release : $Revision: 1.5 $        Date : $Date: 1995/04/07 12:06:13 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE ITMA_TFA * FICHIER ITMA_TFA.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Fiche annuaire
*
*   Cf. DCG 2.4.27
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Lecrivain  18 Oct 1994 : Creation
* Lecrivain  08 Dec 1994 : Ajout du type TFA_Contact_Interlocuteur V1.2
* Lecrivain  07 Avr 1995 : Ajout de la liste des valeurs de type de bip   V1.5
---------------------------------------------------------------------------- */


/*A
 * Type d'un identifiant de fiche annuaire ;
 * type, valeurs et libelles d'un type de bip
 * ------------------------------------------
 */

DEFINE	TFA_Numero_Fiche_Annuaire	SYB#INT2_

DEFINE	TFA_Identifiant_Type_Bip	SYB#INT1_


/*B ATTENTION : LES NOMS DOIVENT CORRESPONDRE AUX VALEURS 0, 1, ... */

DEFINE	TFA_NOMS_TYPES_BIP		XDC_NOM_BIP_INCONNU, XDC_NOM_BIP_EUROSIGNAL, XDC_NOM_BIP_OPERATOR_80_CAR, XDC_NOM_BIP_OPERATOR_8_CHIFF



/*A
 * Format d'un interlocuteur
 * -------------------------
 */

DEFINE	TFA_NOM_INTERLOCUTEUR	1

FORMAT	TFA_Interlocuteur
	numero,
	nom



/*A
 * Format des donnees interlocuteur retournees au module MTFX
 * ----------------------------------------------------------
 */

FORMAT	TFA_Contact_Interlocuteur
	numero,
	nom,
	numero_appel
