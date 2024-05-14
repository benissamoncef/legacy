/*E*/
/* Fichier : $Id: ITMA_TFC.h,v 1.4 2014/11/12 10:41:11 devgfi Exp $      Release : $Revision: 1.4 $        Date : $Date: 2014/11/12 10:41:11 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE ITMA_TFC * FICHIER ITMA_TFC.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Fiche competence
*
*   Cf. DCG 2.4.28
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Lecrivain	20 Oct 1994	: Creation
* Guilhou	15 nov 1996	: refonte complete de la gestion des astreintes (DEM/1306) 1.3
* JPL		07/10/2014	: Ajout du sens aux zones geographiques (DEM 1100) 1.4
---------------------------------------------------------------------------- */


/*A
 * Types des donnees de competence
 * -------------------------------
 */

DEFINE	TFC_Numero_Competence		SYB#INT2_

DEFINE	TFC_Numero_Zone_Geographique	SYB#INT2_

DEFINE	TFC_Sous_Type_Astreinte		SYB#CHAR_

/*A
 * Format des informations de competence
 * -------------------------------------
 */

FORMAT	TFC_Zone_Geographique
	numero,
	num_autoroute,
	PR_debut,
	PR_fin,
	sens


FORMAT	TFC_Competence
	num_type_astreinte,
	num_district_astreinte,
	sous_type,
	site_responsable,
	FORMAT TFC_Zone_Geographique l_zones_geo
