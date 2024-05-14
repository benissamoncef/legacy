/*E*/
/* Fichier : $Id: ITMA_TNA.h,v 1.6 2017/03/31 18:13:18 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2017/03/31 18:13:18 $
------------------------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------------------------
* MODULE MTPM * FICHIER  ITMA_TNA.h
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
DEFINE	C_MODULE			"MTNA"	' Nom du module
DEFINE	REP_CONFIG			"/produits/migrazur/appliSD/fichiers/"
DEFINE	NUM_REPONDEUR		1		' N° du repondeur


/*A Declaration des DEFINES */
/*--------------------------*/

DEFINE	INDETERMINE							"-1"

/*A Declaration du sous path et du nom du Fichiers des scenarios NAV (FS) */

DEFINE	FICHIER_SCENARIOS						"nav/ConfigScenarios.cfg"

/*A Declaration des defines sur les champs du Fichier de Configuration NAV (FC) */


/*A Declaration des defines sur les camps du Fichier de Scenarios NAV (FS) */

DEFINE	CHAMP_FS_LIBELLE_SCENARIO					0
DEFINE	CHAMP_FS_QUADRIGRAMME						1
DEFINE	CHAMP_FS_FIN_ACTION						2
DEFINE	CHAMP_FS_EQUIPEMENT_1						3
DEFINE	CHAMP_FS_EQUIPEMENT_2						4
DEFINE	CHAMP_FS_EQUIPEMENT_3						5
DEFINE	CHAMP_FS_EQUIPEMENT_4						6
DEFINE	CHAMP_FS_EQUIPEMENT_5						7
DEFINE	CHAMP_FS_EQUIPEMENT_6						8
DEFINE	CHAMP_FS_EQUIPEMENT_7						9
DEFINE	CHAMP_FS_EQUIPEMENT_8						10
DEFINE	CHAMP_FS_EQUIPEMENT_9						11
DEFINE	CHAMP_FS_EQUIPEMENT_10						12
DEFINE	CHAMP_FS_EQUIPEMENT_11						13
DEFINE	CHAMP_FS_EQUIPEMENT_12						14
DEFINE	CHAMP_FS_EQUIPEMENT_13						15
DEFINE	CHAMP_FS_EQUIPEMENT_14						16
DEFINE	CHAMP_FS_EQUIPEMENT_15						17
DEFINE	CHAMP_FS_EQUIPEMENT_16						18
DEFINE	CHAMP_FS_EQUIPEMENT_17						19
DEFINE	CHAMP_FS_EQUIPEMENT_18						20
DEFINE	CHAMP_FS_EQUIPEMENT_19						21
DEFINE	CHAMP_FS_EQUIPEMENT_20						22

/*A Declaration des mots du Fichier des scenarios NAV (FS) */

DEFINE	CLE_OUI								"OUI"
DEFINE	CLE_NON								"NON"

DEFINE	CLE_NEUTRE							"NEUTRE"
 
DEFINE	CLE_ETEINT							"ETEINT"
DEFINE	CLE_RENTREE							"RENTREE"

DEFINE	CLE_POSITION_1							"POS1"

DEFINE	CLE_POSITION_2							"POS2"

DEFINE	CLE_POSITION_3							"POS3"

DEFINE	CLE_POSITION_4							"POS4"

/*A Declaration du sous path et du nom du Fichiers des configurations NAV (FC) */

DEFINE	FICHIER_CONFIGURATIONS						"nav/Config_NAV_"
DEFINE	EXTENTION_FICHIER_CONFIGURATIONS				".cfg"

/*A Declaration des defines sur les champs du Fichier de Configuration NAV (FC) */

DEFINE	CHAMP_FC_NUMERO_EQUIPEMENT					0
DEFINE	CHAMP_FC_NUMERO_MODULE						1

DEFINE	CHAMP_FC_X_OBJET						2
DEFINE	CHAMP_FC_Y_OBJET						3

DEFINE	CHAMP_FC_OBJET_NAV						4

DEFINE	CHAMP_FC_NOMBRE_REPETITION_X					6
DEFINE	CHAMP_FC_TAILLE_OBJET						7

DEFINE	CHAMP_FC_NOM_FICHIER_PIX_NEUTRE					5
DEFINE	CHAMP_FC_NOM_FICHIER_PIX_ETEINT					7
DEFINE	CHAMP_FC_NOM_FICHIER_PIX_ALLUME_1				9
DEFINE	CHAMP_FC_NOM_FICHIER_PIX_ALLUME_2				11
DEFINE	CHAMP_FC_NOM_FICHIER_PIX_ALLUME_3				13
DEFINE	CHAMP_FC_NOM_FICHIER_PIX_ALLUME_4				15
DEFINE	CHAMP_FC_NOM_FICHIER_PIX_EN_COURS				17

DEFINE	CHAMP_FC_MESSAGE_EQUIPEMENT_ETEINT				6
DEFINE	CHAMP_FC_MESSAGE_EQUIPEMENT_ALLUME_1				8
DEFINE	CHAMP_FC_MESSAGE_EQUIPEMENT_ALLUME_2				10
DEFINE	CHAMP_FC_MESSAGE_EQUIPEMENT_ALLUME_3				12
DEFINE	CHAMP_FC_MESSAGE_EQUIPEMENT_ALLUME_4				14
DEFINE	CHAMP_FC_MESSAGE_EQUIPEMENT_EN_COURS				16

/*A Declaration des defines sur l'indice des champs de la structure ST_affichage_et_message
	Champs indices:
 			nom_fichier_pix
			message_equipement
*/

DEFINE	CHAMP_NEUTRE							0
DEFINE	CHAMP_ETEINT							1
DEFINE	CHAMP_ALLUME_1							2
DEFINE	CHAMP_ALLUME_2							3
DEFINE	CHAMP_ALLUME_3							4
DEFINE	CHAMP_ALLUME_4							5
DEFINE	CHAMP_EN_COURS							6
DEFINE	DIM_MAX_ETAT							6

DEFINE	MESSAGE_NEUTRE							"__MSG__NEUTRE__"

/*A Definition de la taille maximale de la structure */

/*DEFINE	DIM_MAX_ST_affichage_et_message					22*/
DEFINE	DIM_MAX_ST_affichage_et_message					36

/*A Definition des noms des variables globales de Config */

DEFINE	vg_Config_BRA 							"vg_Config_BRA"
DEFINE	vg_Config_Scenario_BRA 						"vg_Config_Scenario_BRA"

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

FORMAT	PA_NAV
		Remarque,						' remarque (50 char.)
		NumEqt,							' equipement
		Explication,						' explication du PA
		Scenario,						' scenario
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

/* Indice de la structure: le numero de l'equipement dans la NAV */

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
 
FORMAT	Config_BRA
		NumType,						' numero du type
		Identifiant,						' Id. du BRA (30 char.)
		nom_machine_LT,						' Nom de la machine dont depend la NAV
		Autoroute,						' Autoroute d'implantation du BRA
		PR,							' PR d'implantation du BRA
		Sens,							' Sens d'implantation du BRA
		NomSite,						' Nom site du BRA
		NumSite,						' Numero du site du BRA
		Disponible,						' Disponibilite de l'equipement TRUE / FALSE
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
