/*E*/
/* Fichier : $Id: ITMA_TBAF.h,v 1.2 2007/11/19 17:21:40 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2007/11/19 17:21:40 $
------------------------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------------------------
* MODULE MTPM * FICHIER  ITMA_TBAF.h
------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Include IHM de commande des NAV.
*
*
------------------------------------------------------------------------
* HISTORIQUE :
*
* Orengo	08 Oct 1996	: Creation						1.1
* Orengo	06 Dec 1996	: Correction affichage boite de dialogue
					consultation nav appel ITMA_TMC (DEM/1228)  	1.2
* Cluchague     02 Dec 2004     : nombre max de modules passe de 22 a 36 et 4 positions par icone 1.4
* JMG   13/11/07        : ajout site de gestion DEM715
--------------------------------------------------------------------- */


/*A Description des constantes generales
 * ------------------------------------- */
DEFINE	C_MODULE			"MTBAF"	' Nom du module
DEFINE	REP_CONFIG			"../fichiers/"
DEFINE	NUM_REPONDEUR		1		' N° du repondeur


/*A Declaration des DEFINES */
/*--------------------------*/

DEFINE	INDETERMINE							"-1"

DEFINE	vg_Config_BAF 							"vg_Config_BAF"

/*A Definition des noms des objets */
/*A ------------------------------ */

DEFINE	OBJET_ETAT_ACTUEL						"Objet_etat_actuel_"
DEFINE	OBJET_NOUVEL_ETAT						"Objet_nouvel_etat_"
DEFINE	OBJET_DESSIN							"Objet_dessin_"

DEFINE	OBJET_IDPL							OBJET_ETAT_ACTUEL++"7"

DEFINE	OFFSET_Y_PILOTAGE						270
DEFINE	COFFICIENT_X_CONSULTATION					0.833

/*A Declaration des structures */
/*-----------------------------*/

FORMAT	TMC_actionFiche
		numero_fiche,
		cle_fiche,
		numero_action,
		cle_action,
		libelle_fiche,
		heure_action,
		etat_action

FORMAT	PA_BAF
		Remarque,						' remarque (50 char.)
		NumEqt,							' equipement
		Explication,						' explication du PA
		Module11,						
		Module12,						
		Module15,						
		Module21,						
		Module22,						
		Module25,						
		Priorite,						' Priorite
		DistanceEvt,						' Distance de l'evenement
		NumEvt,							' N° d'evenement
		CleEvt,							' cle de l'evenement
		Dispo,							' disponibilite
		NumeroAction,						' numero d'action
		NomSite,						' nom du site
		fmc_liee						' libelle FMC


/* Structure de donnees issues du fichier des configuations NAV (Donnees statiques et dynamiques) */
/* ---------------------------------------------------------------------------------------------- */


FORMAT	ST_affichage_et_message
		equipement_present,					' L'equipement est present: Booleen TRUE / FALSE (Donnee statique)
		equipement_barriere,					' L'equipement est une barriere: Booleen TRUE / FALSE (Donnee statique)
		nom_objet_etat_actuel,					' Chaine (Donnee statique)
		nom_objet_nouvel_etat,					' Chaine (Donnee statique)
		etat_actuel_equipement,					' Int compris entre 0 et DIM_MAX_ETAT (Donnee dynamique)
		nouvel_etat_equipement,					' Int compris entre 0 et DIM_MAX_ETAT (Donnee dynamique)
		nom_fichier_pix,					' Table 6 elements: chaine (Donnees statiques)
		message_equipement,					' Table 6 elements: chaine (Donnees statiques)
		etat_equipement_scenario				' Table 22 elements Int compris entre 0 et DIM_MAX_ETAT (Donnees statiques) 


/* Structure de donnes issues du fichier des scenarios NAV (Donnees statiques) */
/* --------------------------------------------------------------------------- */

/* Indice de la structure: l'ordre de lecture dans le fichier scenario */

FORMAT	ST_scenario
		quadrigramme,						' Quadrigramme du scenario: chaine 4 lettres (Donnee statique)
		fin_action						' Ce type de scenario est une fin d'action: Booleen TRUE / FALSE (Donnee statique)cle de l'evenement


/* Structure de donnes issues des fichiers SYBASE (Donnees statiques) */
/* ------------------------------------------------------------------ */

/* Indice de la structure: le numero de NAV */
 
FORMAT	Config_BAF
		NumType,						' numero du type
		Numero,							' numero
		Identifiant,						' Id. du BRA (30 char.)
		nom_machine_LT,						' Nom de la machine dont depend la NAV
		Autoroute,						' Autoroute d'implantation du BRA
		PR,							' PR d'implantation du BRA
		Sens,							' Sens d'implantation du BRA
		NomSite,						' Nom site du BRA
		NumSite,						' Numero du site du BRA
		Disponible,						' Disponibilite de l'equipement TRUE / FALSE
		Type,			' 1 = une barriere / 2 = deux barrieres
		SiteGestion


/* Structure de donnes issues des fichiers SYBASE Scenarios autorises par type de NAV (Donnees statiques) */
/* ------------------------------------------------------------------------------------------------------ */

/* Indice de la table: le type de NAV */

FORMAT	Config_Scenario
		NomScenario						' nom du scenario (4 char.)

/* Structure de donnees du fichier des configurations NAV pour les objets des boites de dialogues */
/* ---------------------------------------------------------------------------------------------- */

/* Les objets sont les panneaux, les barrieres et les motifs de fond de plan */

/* Indice de la table: Ordre de lecture du fichier des configurations NAV ( Ordre sans importance ) */

FORMAT	ST_objet
		nom_objet,						' Nom de l'objet
		indicateur_objet_dessin,				' Objet dessin ( Fond de plan ) TRUE / FALSE
		nom_fichier_pix,					' Nom du fichier associe a l'objet
		x_objet,						' Position en X de l'objet
		y_objet							' Position en Y de l'objet
