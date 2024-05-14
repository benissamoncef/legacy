/*E*/
/* Fichier : $Id: ITMA_TMC.h,v 1.33 2020/06/02 21:20:00 devgfi Exp $        $Revision: 1.33 $        $Date: 2020/06/02 21:20:00 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE MTMC * FICHIER ITMA_TMC.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Fiche Main Courante
*
*   Cf. DCG 2.4.52
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
*   Lecrivain    06 Jun 1995 : Creation                                   V1.1
*   Torregrossa  12 Jun 1995 : Ajout des positions des pixmap de config de 
*                              voies                                      V1.2
*   Torregrossa  05 Jul 1995 : Ajout les indices des types de vehicules   V1.3
*   Torregrossa  27 Nov 1995 : Ajout les indices des types de vehicules   V1.4
*   Torregrossa  25 Sep 1996 : Ajout degats domaine dans TMC_Fiche V1.5 (FMC10)
*                              Ajout format TMC_veh_lent (FMC6)
*                              Retypage FMC ajout type_precedent (FMC3) 
*   Torregrossa  14 Oct 1996 : Ajout des formats des differentes classes de fmc (IHM7) (V1.6)
*   Torregrossa  04 Nov 1996 : Ajout fausse alerte (FMC18) (V1.7)
*   Torregrossa  14 Nov 1996 : Ajout bau etroite dans TMC_fmc (SAD4) (V1.8-1.9)
*   Torregrossa  11 Dec 1996 : Ajout longueur dans TMC_fmc (FMC7) (V1.10)
* Guilhou 	11 fev 1997 : neutralisations de voies dans FMC travaux (DEM/1394) ANA64 1.11
*   Torregrossa  15 Dec 1997 : Ajout degrade, num_fmc_init, cle_fmc_init dans TMC_fmc (1531) (V1.12)
*   Torregrossa  08 Jan 1998 : Ajout site_alerte dans TMC_fmc (1532) (V1.13)
*   Torregrossa  26 Jan 1998 : correction inverse district_loc et site_alerte dans TMC_fmc (1532) (V1.14)
*   Torregrossa  03 Mar 1998 : ajout com_fmc_init dans TMC_fmc (1583) (V1.15)
*   Niepceron    08 Jan 1999 : Ajout du champ datex,trafic,gravite,bloquant,duree dans TMC_fmc (1723) V1.16
*   Hilmarcher   18 Jul 2005 : Ajout format TMC_nature DEM487 v1.17
*   Lecrivain    23 Jan 2007 : Suppression positions pixmaps de FAV suite à reorg. fiche FMC (V1.18)
*   Lecrivain    28 Mar 2007 : Nouveau format TMC_Fiche_Vehicule conforme a XZAE95 et plus XZAE45 (V1.19)
*   Lecrivain    14 Mai 2007 : Ajout format TMC_chaussee pour nouvelles FMCs (DEM 644,646) V1.20
*   Lecrivain    31 Oct 2007 : Ajout de constantes de mode (Creation/modif/Clos) (lie a DEM 722) 1.21
*   PNI		12 Aout 2008 : Correction TMC_accident incoherent avec la déclaration dans ITMA_TMC_Fiche_Main_Courante.am 1.22 DEM795
*   JPL		10/02/2009 : Ajout du champ 'feu_maitrise' aux donnees vehicules (DEM 845) 1.23
*   LCL		20/07/2011 : Ajout des champs pour le rappel client DEM 1000  1.24
*   JPL		09/11/2011 : Ajout champs de conditions accident pour EVEREST (DEM 1006)  1.25
*   LCL		23/11/2011 : Ajout vitesse et vitesse opposee (DEM/1015) 1.25
*   LCL		02/12/2011 : Ajout vitesse et vitesse opposee (DEM 1015)
*   JMG		10/04/2012 : ajout regulation DEM/1016 1.26
*   JPL		03/09/2012 : Ajout format TMC_incendie (DEM 1037)  1.28
* JMG		29/11/2016 : ajout majeur lot 23 1.29
* JMG		08/03/18	: ajout personnel travaux 1.30
				: ajout config multi auto 
* JPL		29/11/19 : Ajout de l'indicateur d'écoulement aux données FMC (DEM 1322)  1.31
* JPL		05/09/19 : Ajout du code de nature aux données spécifiques incendie (DEM 1352)  1.32
* JPL		24/09/19 : Ajout de l'indicateur de bouchon mobile aux données travaux (DEM 1346)  1.33
-------------------------------------------------------------------------------
*/


/*
** Modes d'ouverture et de traitement de la fiche
** ----------------------------------------------
*/
DEFINE	TMC_MODE_CREATION	0
DEFINE	TMC_MODE_MODIF		1
DEFINE	TMC_MODE_CLOS		2


/*
** Ecart entre les pixmaps de config de voies
** ------------------------------------------
*/
DEFINE	fav_delta		30


/*
** Indices des types de vehicules dans le menu 
** (fenetre FMC et description de vehicule)
** -------------------------------------------
*/
DEFINE TMC_veh_PL	0
DEFINE TMC_veh_VL	1
DEFINE TMC_veh_Car	2
DEFINE TMC_veh_Moto	3
DEFINE TMC_veh_Remorque	4

DEFINE TMC_Val_Type_trafic	0, XZAEC_BOU_Dense, XZAEC_BOU_Ralenti, XZAEC_BOU_Bouche, XZAEC_BOU_Bloque
DEFINE TMC_Lib_Type_trafic	XDC_CHAINE_VIDE , XZAEC_BOU_LIB_Dense, XZAEC_BOU_LIB_Ralenti, XZAEC_BOU_LIB_Bouche, XZAEC_BOU_Bloque

/*
** Description du format des donnees communes a tous les types FMC
** ---------------------------------------------------------------
*/

FORMAT	TMC_fmc
	numero,
	cle,
	site,
	type,
	debut_prevu,
	debut,
	fin_prevu,
	fin,
	validation,
	numero_cause,
	cle_cause,
	type_cause,
	numero_alerte,
	horo_alerte,
	type_alerte,
	origine,
	confirme,
	autoroute,
	pr,
	sens,
	point_car,
	nom_point_car,
	vr,
	vm2,
	vm1,
	vl,
	bau,
	vr_i,
	vm2_i,
	vm1_i,
	vl_i,
	bau_i,
	autoroute_cause,
	pr_cause,
	sens_cause,
	horodate_cause,
	position,
	degatsdom,
	type_precedent,
	fausse_alerte,
	bau_etroite,
	bau_i_etroite,
	longueur,
	abrev_point_car,
	num_point_car,
	degrade,
	num_fmc_init,
	cle_fmc_init,
	com_fmc_init,
	type_fmc_init,
	autoroute_fmc_init,
	PR_fmc_init,
	sens_fmc_init,
	debut_fmc_init,
	site_alerte,
	datex,
	trafic,
	gravite,
	bloquant,
	duree,
	majeur,
	ecoulement,
	district_loc


FORMAT	TMC_comment
	numero,
	validation,
	fin,
	texte,
	site

FORMAT TMC_veh_lent
	longueur,
	vitesse,
	hauteur,
	largeur,
	poids,
	contenu

FORMAT TMC_regul
	contexte,
	scenario_courant,
	scenario_cible,
	mesure,
	indicateur_horaire,
	transitoire,
	zone

FORMAT TMC_accident
	vl,
	pl,
	remorques,
	cars,
	motos,
	bl,
	bg,
	morts,
	vehescota,
	matdang,
	homescota,
	perso,
	dummy_degats_dom,	' Pour compatibilite seulement
	cond_meteo,
	cond_visibilite,
	cond_etat_chaus,
	cond_chantier

FORMAT TMC_bascul
	pr,
	longueur,
	vr,
	vm2,
	vm1,
	vl,
	bau,
	vr_i,
	vm2_i,
	vm1_i,
	vl_i,
	bau_i,
	vitesse,
	vitesse_opposee


/*
** Nouveau format des donnees vehicules compatible avec
** les fiches de classe Vehicule, accident et intervention depanneur
** -----------------------------------------------------------------
*/

FORMAT	TMC_Fiche_Vehicule
	type_veh,
	passagers,
	bebe,
	agee,
	animaux,
	escota,
	volume,
	mat_dang,
	depannage,
	site_depan,
	comment,
	marque,
	type_marque,
	couleur,
	immatriculation,
	code_pays,
	nom_pays,
	nom,
	ville,
	adresse,
	code_danger,
	code_panne,
	nature_panne,
	positionnement,
	vehicule_roulant,
	enlev_demande_par,
	num_fiche_appel,
	flag_mat_dang,
	fax_envoye,
	code_marque,
	code_couleur,
	code_demande,
	num_veh,
	site,
	site_creation,
	nb_bebes,
	nb_agees,
	feu_maitrise,
	type_rappel,
	complement_rappel,
	horodate_rappel


FORMAT TMC_bouchon
	type,
	longueur

FORMAT TMC_delestage
	nom_plan,
	numplan,
	hierarchie

FORMAT TMC_echangeur
	entree1,
	entree2,
	sortie1,
	sortie2

FORMAT TMC_incendie
	feu_maitrise,
	horodate_maitrise,
	nature

FORMAT TMC_manif
	nombre,
	perception,
	violence

FORMAT TMC_meteo
	prfin,
	perturbation,
	visibilite,
	longueur

FORMAT TMC_chaussee
	prfin,
	nature,
	visibilite,      /* uniquement impose par l'usage de la table Meteo */
	longueur

FORMAT TMC_nature
	nature

FORMAT TMC_operateur
	nom

FORMAT TMC_sousconce
	nom,
	ferme,
	penurie,
	carburant

FORMAT TMC_travaux
	nature,
	type,
	continu,
	debut_continu,
	fin_continu,
	mobile,
	autoroute,
	prt_mobile,
	prq_mobile,
	sens_mobile,
	prt,
	voiesret,
	longueur,
	longueur_mobile,
	vr,
	vm2,
	vm1,
	vl,
	bau,
	vr_i,
	vm2_i,
	vm1_i,
	vl_i,
	bau_i,
	vitesse,
	personnel,
	bouchon



/*
** Format de données privées au module Main Courante
** -------------------------------------------------
*/

FORMAT TMC_fmc_a_creer
	type,
	cause,
	cle_cause,
	libelle_cause


/* Format de la table des neutralisations de voies */
FORMAT TMC_neutral
	voie,
	PR_debut,
	PR_fin,
	horodate_debut,
	horodate_fin,
	icone,
	code_voie


FORMAT TMC_multi_auto
	autoroute_tete,
	sens_tete,
	autoroute_queue,
	sens_queue,
	autoroute_queue_inter,
	pr_queue_inter,
	sens_queue_inter,
	autoroute_tete_inter,
	pr_tete_inter,
	sens_tete_inter,
	ptcar_tete,
	ptcar_queue
