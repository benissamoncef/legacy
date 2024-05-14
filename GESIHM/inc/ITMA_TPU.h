/*E*/
/* Fichier : $Id: ITMA_TPU.h,v 1.2 1995/03/01 15:35:05 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/03/01 15:35:05 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE ITMA_TPU * FICHIER ITMA_TPU.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Profils utilisateurs
*
*   Cf. DCG 2.4.59
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Lecrivain	03 Nov 1994	: Creation
---------------------------------------------------------------------------- */


/*A
 * Type des identifiants de famille de fonctions,
 * d'utilisateur et de profil utilisateur
 * ----------------------------------------------
 */

DEFINE	TPU_Ident_Famille	SYB#INT2_

DEFINE	TPU_Ident_Utilisateur	SYB#INT2_

DEFINE	TPU_Ident_Profil	SYB#INT2_



/*A
 * Format d'une famille de fonctions
 * ---------------------------------
 */

DEFINE	TPU_NUMERO_FAMILLE	0
DEFINE	TPU_NOM_FAMILLE	1

FORMAT	TPU_Famille_Fonctions
	numero,
	nom



/*A
 * Format d'un profil
 * ------------------
 */

DEFINE	TPU_NOM_PROFIL		1

FORMAT	TPU_Profil
	numero,
	nom
