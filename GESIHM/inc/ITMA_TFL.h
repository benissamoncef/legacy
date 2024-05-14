/*E*/
/* Fichier : $Id: ITMA_TFL.h,v 1.9 2012/07/04 18:20:47 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2012/07/04 18:20:47 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE ITMA_TFL * FICHIER ITMA_TFL.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Fiche alerte
*
*   Cf. DCG 2.4.29
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
*   Lecrivain	28 Nov 1994	: Creation
*   Torregrossa	07 Jan 1998	: Ajout sit au format TFL_Donnees_Alerte (1532) V1.2
*   Torregrossa	07 Mar 1998	: Ajout format TFL_Donnees_Alerte_Traitee (1581) V1.3
* JMG	22/09/11	: iphone dem 980
---------------------------------------------------------------------------- */


/*A
 * Types des identifiants d'alerte et de qualificatif d'alerte
 * -----------------------------------------------------------
 */

DEFINE	TFL_Identifiant_Alerte		SYB#INT4_

DEFINE	TFL_Identifiant_Qualif_Alerte	SYB#INT1_



/*A
 * Description du format des donnees alerte
 * ----------------------------------------
 */

FORMAT	TFL_Donnees_Alerte
	numero,					' Numero de l'alerte
	date_debut,				' Date debut
	num_type,				' Numero du type alerte
	nom_type,				' Nom du type alerte
	num_equip,				' Numero de l'equipement
	num_type_equip,			' Numero du type equipement
	nom_equip,				' Nom de l'equipement
	texte,					' Texte de l'alerte
	autoroute,				' )
	PR,					' ) Localisation de l'alerte
	sens_circul,				' )
	creation,
	sit,
        autoroute_alerte,
        PR_alerte,
        sens_alerte


/*A
 * Description du format des donnees alerte traitee
 * ----------------------------------------
 */

FORMAT	TFL_Donnees_Alerte_Traitee
	numero,					' Numero de l'alerte
	date_debut,				' Date debut
	num_type,				' Numero du type alerte
	nom_type,				' Nom du type alerte
	num_type_equip,				' Numero du type equipement
	num_equip,				' Numero de l'equipement
	nom_equip,				' Nom de l'equipement
	texte,					' Texte de l'alerte
	autoroute,				' )
	PR,					' ) Localisation de l'alerte
	sens_circul,				' )
	qualif,
	sit,
        autoroute_alerte,
        PR_alerte,
        sens_alerte



/*A
 * Codes de retour de la fiche alerte
 * ----------------------------------
 */

DEFINE	TFL_RET_DIFFERE		1
DEFINE	TFL_RET_VALIDE		2
DEFINE	TFL_RET_A_TRAITER	3
DEFINE	TFL_RET_DEJA_TRAITER	4
