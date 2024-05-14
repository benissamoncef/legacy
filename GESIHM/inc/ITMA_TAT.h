/*E*/
/* Fichier : $Id: ITMA_TAT.h,v 1.1 1994/11/24 09:22:13 lecrivain Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/24 09:22:13 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE ITMA_TAT * FICHIER ITMA_TAT.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Fiche appel telephonique
*
*   Cf. DCG 2.4.11
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Lecrivain	03 Nov 1994	: Creation
---------------------------------------------------------------------------- */


/*A
 * Valeurs de resultats d'appel
 * ----------------------------
 */

DEFINE	TAT_RES_NUL		0
DEFINE	TAT_RES_SUCCES		1
DEFINE	TAT_RES_ECHEC_COMPLET	2
DEFINE	TAT_RES_DEJA_PREVENU	3

DEFINE	TAT_LIBELLES_RESULTATS	"", "Succès", "Echec complet", "Déjà prévenu"
