/*E*/
/* Fichier : $Id: xzat63sp.h,v 1.3 2019/02/08 16:50:05 devgfi Exp $        $Revision: 1.3 $        $Date: 2019/02/08 16:50:05 $
--------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
--------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
--------------------------------------------------------------------------------
* MODULE XZAT * FICHIER xzat63sp.h
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* JPL	16/03/13 : Creation (DEM 1061)  1.1
* JPL	28/08/17 : Ajout du champ de mode d'affichage (clignotant / alterne) (DEM 1230)  1.2
* JPL	12/12/18 : Suppression des données panneau et dates FMC et programmation (DEM 1312)  1.3
------------------------------------------------------------------------------*/


/*X-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Données de mise au neutre programmee d'un PMV donné a l'horodate courante.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

DEFINE	XZAT63_Neutre_Programme_PMV	"XZAT63"



FORMAT	XZATT_Neutre_PMV
	numero_fmc,
	cle_fmc,
	ligne_1,
	ligne_2,
	ligne_3,
	ligne_1_alternat,
	ligne_2_alternat,
	ligne_3_alternat,
	pictogramme,
	bandeau,
	mode_affichage
