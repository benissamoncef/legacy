/*E*/
/* Fichier : $Id: ITMA_TCPT.h,v 1.2 2020/06/22 06:00:41 devgfi Exp $        $Revision: 1.2 $        $Date: 2020/06/22 06:00:41 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE MTCPT * FICHIER ITMA_TCPT.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Fonctions "Coup de poing" au tunnel de Toulon.
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	07/03/2018 : Creation (DEM 1277)  1.1
* JPL	06/03/2020 : Ajout du type de commande aux données Action (pour le domaine GTC) (SAE-114)  1.2
-------------------------------------------------------------------------------
*/


/*A
** Definition des noms des variables globales de configuration
** -----------------------------------------------------------
*/

DEFINE	vg_mode_cdp_toulon	"vg_mode_cdp_toulon"		' Definit le mode reel ou trace seulement

DEFINE	vg_cdp_toulon		"vg_cdp_toulon_s"		' Configuration Coup de Poing Toulon (par sens)

DEFINE	vg_cdp_toulon_eqts	"vg_cdp_toulon_eqts_s"		' Liste des equipements (par sens)



/*A
** Description des donnees des equipements configures
** --------------------------------------------------
*/

FORMAT	TCPT_Equipement
	type,						' Identifiant du type d'equipement
	numero,						' Identifiant de l'equipement
	nom,						' Nom de l'equipement
	sens,						' Sens de localisation de l'equipement
	echangeur,					' Echangeur de localisation de l'equipement
	site_gestion					' Site habilite a commander l'equipement



/*A
** Description des commandes sur ces equipements
** ---------------------------------------------
*/

FORMAT	TCPT_Action
	idx_eqt,					' Index (rang) de l'equipement dans la configuration
	num_eqt,					' Numero identifiant l'equipement (redondant)
	type_cmd,					' Type de commande (ex. domaine GTC)
	cmd_phase1					' Commande a executer en phase 1



/*A
** Description des donnees des niches et listes d'actions pour ces niches
** ----------------------------------------------------------------------
*/

DEFINE	TCPT_NOM_NICHE		0

FORMAT	TCPT_Niche
	nom,						' Identifiant unique de la niche (son nom)
	conditions,					' Conditions d'application de la sequence (bouchon...)
	sens,						' Sens de localisation de la niche
	PR,						' PR de localisation de la niche (sur l'A50)
	ech_deviation,					' Numero d'echangeur de deviation
	FORMAT TCPT_Action actions			' Liste des commandes (signalisation ou neutralisation)
