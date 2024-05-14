/*E*/
/* Fichier : $Id: xzat78sp.h,v 1.1 2018/04/12 11:50:49 devgfi Exp $        $Revision: 1.1 $        $Date: 2018/04/12 11:50:49 $
--------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
--------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
--------------------------------------------------------------------------------
* MODULE XZAT * FICHIER xzat78sp.h
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* JPL	28/03/18 : Creation (DEM 1278)  1.1
------------------------------------------------------------------------------*/


/*X-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Données de mise au neutre programmee d'un PMVA SAGA donné a l'horodate courante.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

DEFINE	XZAT78_Neutre_Programme_PMVA_SAGA	"XZAT78"



FORMAT	XZATT_Neutre_PMVA_SAGA
	numero_pmva,
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
	ligne_4,
	ligne_5,
	ligne_1_alternat,
	ligne_2_alternat,
	ligne_3_alternat,
	ligne_4_alternat,
	ligne_5_alternat,
	mode_affichage
