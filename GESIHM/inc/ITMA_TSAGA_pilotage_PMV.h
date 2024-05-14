/*E Fichier : $Id: ITMA_TSAGA_pilotage_PMV.h,v 1.1 2017/03/31 18:10:57 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2017/03/31 18:10:57 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME gesihm
------------------------------------------------------
* MODULE ITMA_TPM_pilotage_PMV * FICHIER ITMA_TPM_pilotage_PMV.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* cornu	13 Sep 1994	: Creation
* Mismer  	25 Mar 1997 	: Ajout constantes pour TDP (DEM/1417) V1.2
* JPL		21/09/10	: Ajout constantes pour tous types d'affichage (lie a DEM 945) 1.3
------------------------------------------------------ */


/* fichiers inclus */

/* definitions de constantes exportees */
DEFINE	SAGA_CM_AFF_TEXTE	0
DEFINE	SAGA_CM_AFF_NEUTRE	1
DEFINE	SAGA_CM_AFF_HEURE	2
DEFINE	SAGA_CM_AFF_ETEINT	3
DEFINE	SAGA_CM_AFF_TDP	4				

DEFINE	CM_NO_SEUIL		0				
DEFINE	CM_FRCH_SEUIL		1				

DEFINE	CM_AFF_TEXTE_TDP	"A    MN"				
DEFINE	CM_AFF_PICTO_TDP	"SATU"				

/* definitions de types exportes */
FORMAT T_commande
	horodate,
	numero_evt,
	cle_evt,
	operateur,
	numero_eqt,
	site,
	ligne1,
	ligne2,
	ligne3,
	ligne4,
	ligne5,
	alternat1,
	alternat2,
	alternat3,
	alternat4,
	alternat5,
	clignotement,
	picto,
	luminosite,
	bandeau,
	barriere,
	scenario,
	priorite,
	distance,
	site_origine,
	fmc_liee,
	dispo,
	numeroaction,
	typeaffichage,
	identifiant,
	lancement,
	lancement_fin,
	succes,
	echec,
	fin,
	type_saga,
	remarque,
	explication

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

