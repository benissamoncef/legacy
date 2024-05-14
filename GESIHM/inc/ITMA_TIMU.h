/*E*/
/* Fichier : $Id: ITMA_TIMU.h,v 1.1 2021/05/03 13:48:12 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2021/05/03 13:48:12 $
------------------------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------------------------
* MODULE MTPM * FICHIER  ITMA_TIMU.h
------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Include IHM de commande des IMU.
*
*
------------------------------------------------------------------------
* HISTORIQUE :
*
* CGR	12/10/2020		Creation IMU 1.1 DEM-SAE155
--------------------------------------------------------------------- */


/*A Description des constantes generales
 * ------------------------------------- */
DEFINE	C_MODULE			"MTIMU"	/* Nom du module */
DEFINE	NUM_REPONDEUR		1		/* Num du repondeur */


/*A Declaration des DEFINES */
/*--------------------------*/

DEFINE	INDETERMINE							"-1"

/*A Declaration des defines sur les champs du Fichier de Configuration PAL (FC) */



/*A Declaration des mots du Fichier des scenarios PAL (FS) */

DEFINE	CLE_OUI								"OUI"
DEFINE	CLE_NON								"NON"

DEFINE	CLE_NEUTRE							"NEUTRE"
 
DEFINE	CLE_ETEINT							"ETEINT"
DEFINE	CLE_RENTREE							"RENTREE"

DEFINE	CLE_POSITION_1							"POS1"

DEFINE	CLE_POSITION_2							"POS2"

DEFINE	CLE_POSITION_3							"POS3"

DEFINE	CLE_POSITION_4							"POS4"


/*A Declaration des defines sur les champs du Fichier de Configuration IMU (FC) */

DEFINE	CHAMP_FC_NUMERO_EQUIPEMENT					0

DEFINE	CHAMP_FC_OBJET_IMU						4

DEFINE	CHAMP_FC_NOMBRE_REPETITION_X					6
DEFINE	CHAMP_FC_TAILLE_OBJET						7

DEFINE	CHAMP_FC_NOM_FICHIER_PIX_NEUTRE					5
DEFINE	CHAMP_FC_NOM_FICHIER_PIX_ETEINT					7
DEFINE	CHAMP_FC_NOM_FICHIER_PIX_L70					9
DEFINE	CHAMP_FC_NOM_FICHIER_PIX_L90					11
DEFINE	CHAMP_FC_NOM_FICHIER_PIX_L110					13
DEFINE	CHAMP_FC_NOM_FICHIER_PIX_L130					15
DEFINE	CHAMP_FC_NOM_FICHIER_PIX_EN_COURS				17


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

DEFINE	DIM_MAX_ST_affichage_et_message					22							

/*A Definition des noms des variables globales de Config */

DEFINE	vg_Config_IMU 							"vg_Config_IMU"

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

FORMAT	PA_IMU
		NumEqt,						' equipement
		Identifiant,					' Identifiant
		Priorite,					' Priorite
		DistanceEvt,					' Distance de l''evenement
		NumEvt,						' Num d''evenement
		CleEvt,						' cle de l''evenement
		NumeroAction,					' numero d''action
		NomSite,					' nom du site
		fmc_liee,					' libelle FMC
		Dispo,						' Disponibilite IMU
		Operateur,					' Operateur'
		Chaine,						' Chaine de commande
		Intervalle,					' Intervalle de diffusion
		Cycles,						' Cycles de diffusion
		Duree,						' Duree de diffusion
		Zones,						' Zones rattachees au BICE
		IdZones,					' Id de toutes les zones rattachees au BICE
		ZonesDiff,					' Zones en cours de diffusion sur l equipement
		IdZonesDiff,					' Id des zones en cours de diffusion sur l equipement
		Message						' Message en cours de diffusion sur l equipement


/* Structure de donnes issues des fichiers SYBASE (Donnees statiques) */
/* ------------------------------------------------------------------ */

/* Indice de la structure: le numero de PAL */
 
FORMAT	Config_IMU
NumEqt,			' Numero de l'equipement
Identifiant,		' Id. du IMU (30 char.)
NomSite,		' Nom site du IMU
NumSite, 		' Numero du site du IMU
SiteGestion,		' numero du site de gestion du IMU
NumType,
autoroute,
PR,
sens,
nom_machine_lt,
disponible

/* Structure de donnes issues des fichiers SYBASE Scenarios autorises par type de PAL (Donnees statiques) */
/* ------------------------------------------------------------------------------------------------------ */

/* Structure de donnees du fichier des configurations PAL pour les objets des boites de dialogues */
/* ---------------------------------------------------------------------------------------------- */

/* Les objets sont les panneaux, les barrieres et les motifs de fond de plan */

/* Indice de la table: Ordre de lecture du fichier des configurations PAL ( Ordre sans importance ) */

