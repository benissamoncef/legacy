/*E*/
/* Fichier : $Id: ITMA_TCM.h,v 1.2 1995/01/10 15:26:55 lecrivain Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/10 15:26:55 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE ITMA_TCM * FICHIER ITMA_TCM.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Creation d'une alarme
*
*   Cf. DCG 2.4.18
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Lecrivain	08 Nov 1994	: Creation
---------------------------------------------------------------------------- */


/*A
 * Valeurs d'etats des alarmes
 * ---------------------------
 */

DEFINE	TCM_ETAT_ACTIVE		XDC_VRAI
DEFINE	TCM_ETAT_INACTIVE	XDC_FAUX

DEFINE	TCM_LIB_ETAT_ACTIVE	"Active"
DEFINE	TCM_LIB_ETAT_INACTIVE	"Non active"


/*A
 * Type d'un identifiant de type d'alarme
 * --------------------------------------
 */

DEFINE	TCM_Identifiant_Type_Alarme	SYB#INT2_
