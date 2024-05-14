/*E*/
/* Fichier : $Id: xzat68sp.h,v 1.3 2019/02/08 16:50:00 devgfi Exp $        $Revision: 1.3 $        $Date: 2019/02/08 16:50:00 $
--------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
--------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
--------------------------------------------------------------------------------
* MODULE XZAT * FICHIER xzat68sp.h
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* JPL	25/03/13 : Creation (DEM 1061)  1.1
* JPL	28/08/17 : Ajout du champ de mode d'affichage (clignotant / alterne) (DEM 1230)  1.2
* JPL	12/12/18 : Suppression des données panneau et dates FMC et programmation (DEM 1312)  1.3
------------------------------------------------------------------------------*/


/*X-----------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Donneés de mise au neutre programmee d'un PMVA donné a l'horodate courante.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

DEFINE	XZAT68_Neutre_Programme_PMVA	"XZAT68"



FORMAT	XZATT_Neutre_PMVA
	numero_fmc,
	cle_fmc,
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
