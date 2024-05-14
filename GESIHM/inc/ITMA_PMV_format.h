/*E*/
/* Fichier : $Id: ITMA_PMV_format.h,v 1.13 2019/01/07 15:37:12 pc2dpdy Exp $      Release : $Revision: 1.13 $        Date : $Date: 2019/01/07 15:37:12 $
-------------------------------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE ITMA_PMV * FICHIER ITMA_PMV_format.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* format des donnes utilisé pour la gestion des PMV.
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Cornu 	26 Sep 1994	: Creation
* Mismer  	26 Aou 1996 	: Ajout listes type PMV et Picto (DEM/1162)	V1.2
* Mismer  	05 Sep 1996 	: Remplacement type_PMV par  type_PMV_GEN (DEM/1162) V1.3
* Mismer  	25 Mar 1997 	: Modif format pour TDP (DEM/1417) V1.4
* JMG		13/09/05	: ajout PMVA
* JMG   13/11/07        : ajout site de gestion DEM715
* VR		15/12/11	: Ajout PRV (DEM/1016)
* LCL		12/12/11	: Ajout destination noeud a type_PMV_GEN (DEM/1014)
* LCL		31/01/12	: Ajout DestinationNoeud a type PMV_GEN (DEM/1014) v1.7
* VR		05/06/12	: Ajout DTP PMVA (DEM/1014 PMA)
* JMG		26/04/16 	: ajout eqt saga 1.12
* JMG		01/08/18 	: suppression SAGA DEM1306 1.13
---------------------------------------------------------------------------- */


FORMAT	type_PMV_GEN
		Horodate,			' date en entrée (date)
		NumEqt,			' numéro d équipement
		Operateur,		' l'operateur rattaché à l'action
		Identifiant,		' Id. du PMV (30 char.)
		DispoPMV,			' oui / non (booléen)
		DispoPicto,		' oui / non (booléen)
		NumTypePMV,		' type de PMV
        	Message1,			' 1ere ligne du message (15 car)
        	Message2,			' 2eme ligne du message (15 car)
        	Message3,			' 3eme ligne du message (15 car)
		Alternat1,		' 1ere ligne de l'alternat (15 car)
		Alternat2,		' 2eme ligne de l'alternat (15 car)
		Alternat3,		' 3eme ligne de l'alternat (15 car)
        	Flash,			' oui / non (booléen)
        	Clignotement1,		' pour chaque lignes (booléen)
        	Clignotement2,		' pour chaque lignes (booléen)
        	Clignotement3,		' pour chaque lignes (booléen)
		ClignotementPicto,	' pour la notation du picto (booléen)
		TypePicto,		' symbole du pictogramme (4 char.)
		TextePicto,		' notation du pictogramme ( 6 char.)
		NumTypePicto,		' type de picto
		Fmc_liee,			' fmc liee au pmv ( 30 char. )
		Remarque,			' remarque sur le PMV (50 char.)
		Explication,		' explication du PA
		Numero_Evt,		' numéro d'événement
		Cle_Evt,			' clé associé à l événement
		Priorite,			' Priorité du PMV
		DistanceEvt,		' Distance de l événement
		NumeroAction,		' Numero d action
        	Direction,			' Direction (15 car)
        	AltDirection,			' Alterna Direction (15 car)
        	Destination,			' Numero destination TDP
        	AltDestination,			' Alternat Numero destination TDP
		DestinationNoeud,		' Numero Destination Noeud TDP
		TypeAffichage,			' Type d affichage (texte,neutre,TDP,..)
		SiteGestion

FORMAT	Evt
		numero,			' numero de l événement
		cle			' clé associé à l événement

FORMAT	Config_PMV
		NbLigne,		' nombre de ligne
		Identifiant,		' Id. du PMV (30 char.)
		NomSite,		' Nom site du PMV
		NumSite, 		' Numero du site du PMV
		SiteGestion,		' numero du site de gestion du PMV
		NbCaracteres,
		Flash			,
		Picto

FORMAT	Config_Dest_TDP
		NumPMV,			' Numero de PMV
		Sens,			' Sens du PMV
		NumDest,		' Numero de la destination
		Libelle_PMV		' Libelle PMV

FORMAT Config_SAGA
		NumEqt,
		TypeSAGA,
		Identifiant,            
		NomSite,                ' Nom site du PMV
		NumSite,
		SiteGestion,
		Instance,
		felscode,
		eqtcode ,
		codecmdmessage,
		codecmdluminosite,
		codecmdbarriere,
		codecmdpicto,
		codecmdheure,
		autoroute,
		PR,
		sens


FORMAT type_PMVA
		Horodate,
		NumEqt,
		Operateur,
		Identifiant,
		DispoPMV,
		Message1,
		Message2,
		Message3,
		Message4,
		Message5,
		Alternat1,
		Alternat2,
		Alternat3,
		Alternat4,
		Alternat5,
        	Clignotement1,		' pour chaque lignes (booléen)
        	Clignotement2,		' pour chaque lignes (booléen)
        	Clignotement3,		' pour chaque lignes (booléen)
        	Clignotement4,		' pour chaque lignes (booléen)
        	Clignotement5,		' pour chaque lignes (booléen)
		Flash,
		FMC_liee,
		Remarque,
		Explication,
		Numero_evt,
		Cle_evt,
		Priorite,
		distanceEvt,
		NumeroAction,
        	AltDirection,			' Alterna Direction (15 car)
        	/*Destination,			' Numero destination TDP
        	AltDestination,			' Alternat Numero destination TDP*/
        	Dest1_proche,			'Numero destination TDP
        	Dest1_loin,
        	Dest2_proche,
        	Dest2_loin,
        	TypeAffichage,			' Type d affichage (texte,neutre,TDP,..)
		SiteGestion

FORMAT type_SAGA
		Horodate,
		NumEqt,
		Operateur,
		Identifiant,
		DispoPMV,
		Message1,
		Message2,
		Message3,
		Message4,
		Message5,
		Alternat1,
		Alternat2,
		Alternat3,
		Alternat4,
		Alternat5,
        	Clignotement,		' pour chaque lignes (booléen)
		Luminosite,
		Picto,
		Bandeau,
		Barriere,
		Scenario,
		FMC_liee,
		Remarque,
		Explication,
		Numero_evt,
		Cle_evt,
		Priorite,
		distanceEvt,
		NumeroAction,
        	TypeAffichage,			' Type d affichage (texte,neutre,TDP,..)
		SiteGestion,
		FORMAT Config_SAGA Config
