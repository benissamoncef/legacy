/*E*/
/*  Fichier : $Id: ITMA_TCV.h,v 1.3 2007/08/02 17:16:03 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2007/08/02 17:16:03 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE ITMA_TCV * FICHIER ITMA_TCV.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Fiche de definition d'une courbe a visualiser.
*
*   Cf. DCG 2.4.21
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Lecrivain	19 Dec 1994 : Creation
* Torregrossa	06 Jui 1995 : Ajout TCV_Donnees_Station (V 1.2)
* JPL		02/08/07 : Ajout types de visualisation des courbes (DEM 673) 1.3
---------------------------------------------------------------------------- */


/*A
 * Valeurs de types de courbes a visualiser
 * ----------------------------------------
 */

DEFINE	TCV_COURBE_TRAFIC	"TRAFIC"
DEFINE	TCV_COURBE_Q_V_TO	"Q_V_TO"
DEFINE	TCV_COURBE_RETARD	"RETARD"
DEFINE	TCV_COURBE_F_T		"F_T"


/*A
 * Types de visualisation
 * ----------------------
 */

DEFINE	TCV_TYPE_SYNOPTIQUE	0
DEFINE	TCV_TYPE_SYN_AVEC_REF	1
DEFINE	TCV_TYPE_BUREAUTIQUE	2


/*A
 * Format des points de mesure pour une station
 * --------------------------------------------
 */

FORMAT TCV_Donnees_Station
	sens,			' Sens du point de mesure
	voie			' Voie possedant le point de mesure
