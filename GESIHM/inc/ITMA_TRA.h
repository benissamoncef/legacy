/*E*/
/* Fichier : $Id: ITMA_TRA.h,v 1.4 2021/06/03 07:15:37 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2021/06/03 07:15:37 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE ITMA_TRA * FICHIER ITMA_TRA.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Communication RAU.
*
*   Cf. DCG 2.4.60, 2.4.63
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Lecrivain	18 Jan 1995	: Creation                                V1.1
* CGR		02/04/2021	: Ajout TRA_Etat_SONO_RAU DEM-SAE244 V1.4
---------------------------------------------------------------------------- */

/*A
 * Declaration des DEFINES
 * ---------------------------------------
 */

DEFINE 	C_MODULE		"MTSONO"
DEFINE  TRA_MESSAGE_REPOS       "REPOS"


/*A
 * Libelles des etats de communication RAU
 * ---------------------------------------
 */

DEFINE	TRA_LIB_ETAT_REPOS	"- Libre -"
DEFINE	TRA_LIB_ETAT_APPEL	"- En Appel -"
DEFINE	TRA_LIB_ETAT_COMM	"- En Communication -"
DEFINE	TRA_LIB_ETAT_GARDE	"- En Garde -"
DEFINE	TRA_LIB_ETAT_DEFAUT	"- Défaut -"
DEFINE	TRA_LIB_ETAT_EN_TEST	"- En cours de Test -"



/*A
 * Format des donnees d'etat recues des equipements PAU
 * ----------------------------------------------------
 */

FORMAT	TRA_Etat_RAU
	num_PAU,
	num_poste_oper,
	commande,
	cpt_rendu,
	etat_comm_RAU,
	etat_comm_GN

FORMAT	TMC_actionFiche
	numero_fiche,
	cle_fiche,
	numero_action,
	cle_action,
	libelle_fiche,
	heure_action,
	etat_action

FORMAT TRA_Etat_SONO_RAU
	NumEvt,
	CleEvt,
	Priorite,
	DistanceEvt,
	Dispo,
	fmc_liee,
	NumeroAction,
	NomSiteAction,
	autoroute,
	sens,
	pr_debut,
	pr_fin,
	message,
	Nb_PAUErreur,
	Nb_PAUPilotes

