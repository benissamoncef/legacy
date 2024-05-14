/*E*/
/* Fichier : $Id: ITMA_TGF.h,v 1.2 2011/12/20 17:58:42 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2011/12/20 17:58:42 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE ITMA_TGF * FICHIER ITMA_TGF.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Gestion des fichiers de bureautique
*
*   Non decrit dans le DCG
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Lecrivain	22 Mar 1995 : Creation                                    V1.1
---------------------------------------------------------------------------- */


/*A
 * Liste des sites et liste des machines associees
 * stockant les repertoires des fichiers publics
 * --------------------------------------------------
 */

/*B RESPECTER LA CORRESPONDANCE DANS LES DEUX LISTES */

DEFINE	TGF_LISTE_SITES		XDC_CI, XDC_DN, XDC_CA
DEFINE	TGF_LISTE_POSTES	"POGT", "POCDDN", "POCDDM"


/*A
 * Tailles maximales (en Ko) des repertoires local et public
 * ---------------------------------------------------------
 */

DEFINE	TGF_TAILLE_MAX_LOCAL	5000
DEFINE	TGF_TAILLE_MAX_PUBLIC	3000
