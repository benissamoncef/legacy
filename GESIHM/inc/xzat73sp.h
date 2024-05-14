/*E*/
/* Fichier : $Id: xzat73sp.h,v 1.1 2018/04/12 11:50:43 devgfi Exp $        $Revision: 1.1 $        $Date: 2018/04/12 11:50:43 $
--------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
--------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
--------------------------------------------------------------------------------
* MODULE XZAT * FICHIER xzat73sp.h
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* JPL	28/03/18 : Creation (DEM 1278)  1.1
------------------------------------------------------------------------------*/


/*X-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Données de mise au neutre programmee d'un PMV SAGA donné a l'horodate courante.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

DEFINE	XZAT73_Neutre_Programme_PMV_SAGA	"XZAT73"



FORMAT	XZATT_Neutre_PMV_SAGA
	numero_pmv,
	numero_action,

	numero_fmc,
	cle_fmc,
	type_fmc,
	debut_fmc,
	fin_fmc,

	debut_prog,
	fin_prog,
	ligne_1,
	ligne_2,
	ligne_3,
	ligne_1_alternat,
	ligne_2_alternat,
	ligne_3_alternat,
	pictogramme,
	bandeau,
	mode_affichage
