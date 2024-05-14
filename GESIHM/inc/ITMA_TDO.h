/*E*/
/* Fichier : $Id: ITMA_TDO.h,v 1.116 2020/04/02 13:49:10 devgtie Exp $        Release : $Revision: 1.116 $        Date : $Date: 2020/04/02 13:49:10 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE ITMA_TDO * FICHIER ITMA_TDO.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de donnees globales a la tache IHM textuelle TITMA
*
*   Cf. DCG 2.4.22
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Lecrivain	25 Aug 1994 : Creation
* Lecrivain	10 Oct 1994 : Ajout var. globale "FMC ouverte"
* Lecrivain	11 Oct 1994 : Ajout var. globale numero de poste   	V1.23
* Lecrivain	17 Oct 1994 : Ajout var. types et categ. astreinte
*				  Suppression cav. liste_sites         	V1.26
* Noel 		24 Oct 1994 : Ajout de vg_numero_operateur		V1.28
* Noel		03 Nov 1994 : definition d'une gare de peage 
*						avec sa vg associee	V1.31
* Guilhou	09 nov 1994 : definition icones des fav			V1.32
*				  ajout format horodate_eclatee
*				  ajout vg_les_points_car
* Lecrivain	10 Nov 1994 : Ajout var. flags d'ouverture de fenetres V1.33
* Lecrivain	15 Nov 1994 : Ajout var. listes aires, peages, ...     V1.35
* Lecrivain	16 Nov 1994 : Modifs types et variables       V1.36 -> V1.39
* Guilhou	01 dec 1994 : ajout vg_les_pix_fav_inv,vg_les_pix_bau_inv
									V1.40
* Guilhou	01 dec 1994 : correction icones config des voies 
* Guilhou	15 dec 1994 : ajout numero de display			V1.43
*						  num et cle de la fiche operateur
* Guilhou	23 dec 1994 : ajout type evenement cause			V1.44
* Lecrivain	06 Jan 1995 : Ajout var. vg_les_pau_en_garde              V1.45
* Guilhou	17 jan 1995 : Ajout var de formation			  V1.48
* Guilhou	24 jan 1995: ajout var vg_liste_reveils                   V1.49
* Lecrivain  15 Mar 1995 : Ajout var. vg_repertoire_migrazur           V1.53
* Lecrivain  16 Mar 1995 : Ajout var. vg_nom_fichier_ss                V1.54
* Lecrivain  14 Avr 1995 : Ajout var. vg_pa_en_cours                   V1.55
* Lecrivain  24 Avr 1995 : Supp. vg_pa_en_cours, ajout vg_requete_en_cours V1.56
* Lecrivain  31 Jul 1995 : Ajout format et var. vg_le_pau_en_ecoute    V1.59
* Noel		21 Sep 1995 : Ajout du type d'imprimante		V1.61
* Torregrossa  12 Oct 1995 : Ajout var. vg_les_peages                   V1.62
* Torregrossa  24 Oct 1995 : modif format des donnees des autoroutes(A57)    V1.63
* Torregrossa  24 Jan 1996 : modif format des donnees des aires    V1.64
* Guilhou	22 aut 1996 : ajout variables globales RADT V1.65
* Mismer	26 Aou 1996 : ajout variables globales type PMV (DEM/1162) V1.66
* Verdier	30 Aou 1996 : Ajout variables globales texte copier/coller (DEM1169) V1.67
* Mismer	30 Aou 1996 : ajout variables globales type PMV (DEM/1162) V1.68
* Guilhou	03 Sep 1996 : modif vg d'etat des PC simplifies V1.69
* Orengo	17 Sep 1996 : Ajout defines Cannet et St Maximin LTVideo V1.70
* Guilhou	24 sep 1996 : ajout vg_canal_SGBD_distant (ANA/33) V1.71
* Niepceron 1 oct 1996 : ajout vg_F_T_gauche et vg_F_T_droite (ANA8) V1.72
* Niepceron 4 nov 1996 : ajout vg_niv_diff (dem/1248) V1.73
* Torregrossa 20 nov 1996 : ajout pix pou bau etroite (dem/1283) V1.74
* Guilhou 22 nov 1996 : ajout variables globales de la gestion des astreintes 1.75
* Niepceron     12 Dec 1996     : Ajout d'un libelle parametrable associe au niveau TFM (dem/1328) v1.76
* Guilhou 13 jan 1997 : ajout droit d'enrichir (DEM/1358) 1.77
* Guilhou 	12 fev 1997 : ajout vg pour DEM/1394 1N164 1.78
* Mismer	05 Mar 1997 : ajout vg_Destination_TDP (DEM/1417) V1.79
* Guilhou	12 nov 1997 : ajout vg pour DEM/1502 1.80
* Torregrossa 19 Dec 1997 : ajout TDO_TYPE_DISTRICT (1531) 1.81
* Guilhou	21 jan 1997 : ajout vg_requete_xzae148_en_cours 1.82
* Torregrossa 09 Fev 1998 : ajout TDO_DIS_RE1, TDO_DIS_REC2, vg_info_district_local (1536) 1.83-1.84
* Mismer	16 fev 1998 : modif pour video CI  (DEM/1541) V1.86
* Torregrossa 16 Mar 1998 : ajout format pour info_district_local (1536) 1.87
* Niepceron     04 Jun 1998 : ajout de vg_ pour la diff TFM 1.88
* Niepceron     04 Jul 1998 : ajout vg_canal_SGBD_his (dem/1699) 1.89
* Niepceron     08 Jan 1999 : ajout vg_datex dem/1723 1.90
* JMG		13/08/05	: ajout vg_type_PMVA 1.91
* JMG		28/09/05	: re-ajout de vg_datex_propriete_* 1.92
* JPL		08/02/2007 : Ajout de vg_autoroutes_district 1.93
* JPL		05/04/2007 : Ajout du format TDO_Libelle_Predefini 1.94
* JPL		20/06/2007 : Ajout format TDO_ITPC et vg_itpc_district 1.95
* JPL		20/06/2007 : Ajout format TDO_Canton et vg_cantons_district (DEM 657) 1.96
* JBL		Nov 2007 : DEM 665
* ABE	    05/06/2008: intégration des spécificités du tunnel MONACO
* JPL		26/11/2008 : Ajout vg_mail_rtfm_asf (DEM 823) 1.100
* JPL		06/05/2009 : Ajout vg_niveau_trace 1.101
* JPL		11/06/2009 : Ajout vg_destinataires_fax et format TDO_Dictionnaire (pour DEM 888) 1.102
* JPL		15/09/2009 : Simple renommage champ format TDO_Dictionnaire 1.103
* JPL		20/11/2009 : Ajout vg_types_bouchon et vg_types_bouchon_valide 1.104
* JPL		24/09/2010 : Ajout vg_types_travaux et vg_types_travaux_valide (pour DEM 942) 1.105
* JPL		01/10/2010 : Ajout vg_les_bifurcations, format associe, PR limites autoroutes (DEM 930) 1.106
* JPL		25/10/2010 : Ajout vg_peages_district pour completude  1.107
* JPL		29/10/2010 : Ajout format TDO_Equipement pour tous equipements, ici cameras (DEM 949)  1.108
* JPL		08/11/2011 : Ajout vg_cond_xxx (types conditions accident) (DEM 1006)  1.109
* JPL		13/01/2012 : Ajout vg_origines_FMC (DEM 1012)  1.110
* LCL		21/03/2012 : Ajout vg_type_PRV (DEM/1016) 1.111
* JPL		27/09/2013 : Ajout sortie et bifurcation aux donnees echangeurs (DEM 1083)  1.112
* NDE		04/08/2015 : Nouvelle variable globale liste des reveils (DEM 1138) 1.113
* JMG		07/06/2016 : ajout vg_type_SAGA 1.114
* ABE		14/02/2020 : Gestion incendie dans sens opposé pour ouvrage toulon
---------------------------------------------------------------------------- */

/*A ***************************************************************************
 **                                                                          **
 **                    Declaration des formats de donnees                    **
 **                                                                          **
 ******************************************************************************/

/*A
 * Format des donnees autoroutes
 * -----------------------------
 */

DEFINE	TDO_NUMERO_AUTOROUTE	0
DEFINE	TDO_NOM_AUTOROUTE	1
DEFINE	TDO_SENS_AUTOROUTE	2

FORMAT	TDO_Autoroute
	numero,					' Identifiant de l'autoroute
	nom,					' Nom de l'autoroute (4 car)
	sens,					' Sens des PR (croissant : 1, decroissant : -1)
	escota,					' Appartenance réseau (vrai : 1, faux : 0)
	PR_debut,				' Pr de debut de l'autoroute
	PR_fin,					' Pr de fin de l'autoroute
	debut_concession,			' Pr de debut dans la concession
	fin_concession				' Pr de fin dans la concession



/*A
 * Format des donnees de bifurcations d'autoroutes
 * -----------------------------------------------
 */

DEFINE	TDO_AUTO1_BIFURCATION	0
DEFINE	TDO_PR1_BIFURCATION	1

FORMAT	TDO_Bifurcation
	autoroute1,				' Identifiant de la premiere autoroute
	PR1,					' PR de la bifurcation sur la premiere autoroute
	autoroute2,				' Identifiant de la deuxieme autoroute
	PR2					' PR de la bifurcation sur la deuxieme autoroute



/*A
 * Format des donnees "sens circulation"
 * -------------------------------------
 */

DEFINE	TDO_NUMERO_SENS		0
DEFINE	TDO_NOM_SENS		1

FORMAT	TDO_Sens_Circulation
	numero,					' Identifiant du sens
	nom					' Nom sens circulation (11 car)



/*A
 * Format des donnees districts
 * ----------------------------
 */

DEFINE	TDO_NUMERO_DISTRICT	0
DEFINE	TDO_NOM_DISTRICT	1
DEFINE	TDO_CODE_DISTRICT	2
DEFINE	TDO_TYPE_DISTRICT	3

FORMAT	TDO_District
	numero,					' Identifiant du district
	nom,					' Nom du district (25 car)
	code,					' Code du district (2 car)
	type,
	serveur,				' Nom du serveur (6 car)
	machine


FORMAT	TDO_District_local
	code,					' Code du district (2 car)
	numero,					' Identifiant du district
	type,
	nom,					' Nom du district (25 car)
	machine,
	district_rec1,				' district de recouvrement 1
	district_rec2,				' district de recouvrement 2
	district_rec3				' district de recouvrement 3



/*A
 * Format des donnees echangeurs
 * -----------------------------
 */

DEFINE	TDO_NUMERO_ECHANGEUR	0
DEFINE	TDO_NOM_ECHANGEUR	1

DEFINE	TDO_NUMERO_ECH		0
DEFINE	TDO_NOM_ECH		1
DEFINE	TDO_AUTO_ECH		3
DEFINE	TDO_PR_ECH		4
DEFINE	TDO_SORTIE_ECH		15

FORMAT	TDO_Echangeur
	numero,					' Identifiant de l'echangeur
	nom,					' Nom echangeur (25 car)
	abbrev,					' Nom abrege (15 car)
	autoroute,
	PR,
	PR_entree_sens1,
	PR_sortie_sens1,
	PR_entree_sens2,
	PR_sortie_sens2,
	district,				' Identifiant du district
	supprime,				' Indicateur de suppression
	voies_entree_sens1,
	voies_sortie_sens1,
	voies_entree_sens2,
	voies_sortie_sens2,
	sortie,					' Denomination de la sortie
	bifurcation				' Indicateur de type


/*A
 * Format des donnees gares de peage et peages
 * -------------------------------------------
 */

DEFINE	TDO_NUMERO_PEAGE	0
DEFINE	TDO_NOM_PEAGE		1

DEFINE	TDO_NUMERO_GARE		0
DEFINE	TDO_NOM_GARE		1

FORMAT	TDO_Peage
	numero,					' Identifiant du peage
	nom,
	abbrev,
	autoroute,
	PR_sens1,
	PR_sens2,
	district,				' Identifiant du district
	supprime				' Indicateur de suppression


DEFINE	TDO_Gare		TDO_Peage



/*A
 * Format des donnees aires
 * ------------------------
 */

DEFINE	TDO_NUMERO_AIRE		0
DEFINE	TDO_NOM_AIRE		1

FORMAT	TDO_Aire
	numero,					' Identifiant de l'aire
	nom,
	abbrev,
	autoroute,
	PR_entree,
	PR_sortie,
	sens_circul,
	capacite_VL,
	capacite_PL,
	district,				' Identifiant du district
	supprime,				' Indicateur de suppression
	sousconcessionnaire			' identifiant du sous concessionnaire



/*A
 * Format des donnees lits d'arret
 * -------------------------------
 */

DEFINE	TDO_NUMERO_LIT		0
DEFINE	TDO_NOM_LIT		1

FORMAT	TDO_Lit_Arret
	numero,					' Identifiant du lit d'arret
	nom,
	autoroute,
	PR,
	sens_circul,
	district,				' Identifiant du district
	supprime				' Indicateur de suppression



/*A
 * Format des donnees Interruption de Terre-Plein Central
 * ------------------------------------------------------
 */

DEFINE	TDO_ITPC_AUTOROUTE	0
DEFINE	TDO_ITPC_PR		1

FORMAT	TDO_ITPC
	autoroute,				' Numero de l'autoroute
	PR					' Pr de l'ITPC



/*A
 * Format des donnees Cantons
 * --------------------------
 */

DEFINE	TDO_CANTON_AUTO		0
DEFINE	TDO_CANTON_PR_DEB	1

FORMAT	TDO_Canton
	autoroute,				' Numero de l'autoroute
	PR_debut,				' Pr de debut du canton
	sens



/*A
 * Format des types de fiches main courante
 * ----------------------------------------
 */

DEFINE	TDO_NUMERO_FMC		0
DEFINE	TDO_CLASSE_FMC		1
DEFINE	TDO_NOM_FMC		2
DEFINE	TDO_ABBREV_FMC		3

FORMAT	TDO_Type_FMC
	numero,					' Id du type action/event./oper
	classe,					' Classe (pour les evenements)
	nom,					' Nom du type FMC (25 car)
	abbrev,					' Abbreviation (evenements-10 c)
	operation,				' Vrai s'il s'agit d'une oper.
	supprime				' (au moins pour les evenements)



/*A
 * Format d'un libelle fiche main courante
 * ---------------------------------------
 */

DEFINE TDO_NUMERO_FICHE		0
DEFINE TDO_TYPE_FICHE		1
DEFINE TDO_HORODATE		2
DEFINE TDO_LOCALISATION		3

FORMAT TDO_Libelle_Fmc
	numero,		'numero de la fiche
	type,		'type de l'evenement
	horodate,		'horodate debut
	localisation,	'localisation
	enrichisseur	'poste enrichisseur


/*A format d'un evenement lie*/
FORMAT	TDO_Evt_Cause
	numero,					' Numero evenement
	enrichisseur,				' Poste enrichisseur (3 car)
	type,					' Type evenement (25 car)
	date_debut,				' Date debut (16 car)
	localisation,				' Localisation event. (28 car)
	FORMAT COM_Intitule_FMC info_evt,	' Informations evenement
	droit_enrichir				' droit d enrichir



/*A
 * Format des donnees motif d'appel
 * --------------------------------
 */

DEFINE	TDO_NUMERO_MOTIF_APPEL	0
DEFINE	TDO_NOM_MOTIF_APPEL	1

FORMAT	TDO_Motif_Appel
	numero,					' Identifiant du motif d'appel
	nom					' Nom du motif d'appel (25 car)



/*A
 * Format des types d'equipements
 * ------------------------------
 */

DEFINE	TDO_NUMERO_TYPE_EQUIP	0
DEFINE	TDO_NOM_TYPE_EQUIP	1

FORMAT	TDO_Type_Equipement
	numero,					' Numero du type d'equipement
	nom					' Nom du type (30 car)



/*A
 * Format des equipements
 * ----------------------
 */

DEFINE	TDO_NUMERO_EQUIP	0
DEFINE	TDO_NOM_EQUIP		1
DEFINE	TDO_AUTO_EQUIP		2
DEFINE	TDO_PR_EQUIP		3
DEFINE	TDO_SENS_EQUIP		4
DEFINE	TDO_SITE_EQUIP		5
DEFINE	TDO_INFO_EQUIP		7

FORMAT	TDO_Equipement
	numero,					' Numero du type d'equipement
	nom,					' Nom de l'equipement (10 car)
	autoroute,
	PR,
	sens,
	site_gestion,				' Site pilotant l'equipement
	dispo,					' Si utile a l'IHM sinon laisser NULL ...
	info					' Infos (ex. code camera, ou structure ...) specifiques au type



/*A
 * Format des postes operateurs
 * ----------------------------
 */

DEFINE TDO_LIBELLE_OPERATEUR	0
DEFINE TDO_NUMERO_POSTE		1
DEFINE TDO_NUMERO_SITE		2

FORMAT TDO_Poste_Operateur
	libelle,	'nom du poste
	numero_poste,	'numero du poste
	numero_site	'numero du site sur lequel il se trouve



/*A
 * Format des types et categories d'astreintes
 * -------------------------------------------
 */

DEFINE	TDO_NUMERO_TYPE_ASTREINTE	0
DEFINE	TDO_NOM_TYPE_ASTREINTE		1

FORMAT	TDO_Type_Astreinte
	numero,
	nom


DEFINE	TDO_NUMERO_CATEG_ASTREINTE	0
DEFINE	TDO_NOM_CATEG_ASTREINTE		1

FORMAT	TDO_Categorie_Astreinte
	numero,
	nom


/*A format de l'horodate eclatee*/
FORMAT TDO_Horo_Eclatee
	jour,
	mois,
	annee,
	heures,
	minutes

/*A format d'un point caracteristique*/
DEFINE 	TDO_NUMERO_POINT_CAR	0
DEFINE	TDO_NOM_POINT_CAR		1
FORMAT TDO_Point_Car
	numero,
	nom



/*A
 * Format des informations PAU en ecoute GN
 * ----------------------------------------
 */

FORMAT TDO_PAU_En_Ecoute
	numero_pau,	' Numero du PAU en ecoute GN
	demande_GN	' Demande eventuelle de secret ou de reprise par le SAE


/*A
 * Format des informations sur Niveau TFM
  * ----------------------------------------
*/
DEFINE	TDO_NIVEAU_TFM 	0
DEFINE  TDO_LIB_NIV	1

FORMAT	TDO_NIV_LIB
	 niveau,	' Numero du niveau
	 libelle	' libelle du niveau


/*A
 * Format des donnees des libelles predefinis
 * ------------------------------------------
 */

DEFINE	TDO_CODE_LIBELLE	1
DEFINE	TDO_TEXTE_LIBELLE	3
DEFINE	TDO_EXPLIC_LIBELLE	4
DEFINE	TDO_VALID_LIBELLE	5

FORMAT	TDO_Libelle_Predefini
	num_type,	' Numero du type de libelle
	code,		' Code du libelle
	position,	' Pour ordonner les libelle dans la liste presentee
	libelle,	' Libelle lui-meme
	explication,	' ... plus detaillees
	valide		' 0 ou 1 selon que le libelle est valide ou non



/*A
 * Format general d'un dictionnaire de donnees
 * -------------------------------------------
 */

DEFINE	TDO_DICO_CLE	0
DEFINE	TDO_DICO_VAL	1

FORMAT	TDO_Dictionnaire
	kle,			' Cle de recherche de l'element
	valeur			' Valeur associee a la cle

/*A
 * Format general des donnees d'un reveil
 * --------------------------------------
 */

FORMAT	TLT_Reveil_Fmc
	num_fmc,
	cle_fmc,
	texte

/*A 
 * Noms des fichiers pixmaps associes aux fav
 */

DEFINE 	TDO_voie_inconnue			""	
DEFINE	TDO_voie_normale			"pix_fav_normale"
DEFINE	TDO_voie_inverse			"pix_fav_inverse"
DEFINE	TDO_voie_bloquee			"pix_fav_bloquee"
DEFINE 	TDO_voie_bau_normale		"pix_fav_bau_normale"
DEFINE	TDO_voie_inexistante		""
DEFINE	TDO_voie_neutralisee		"pix_fav_neutralisee"
DEFINE	TDO_voie_bloq_neutralisee	"pix_fav_bloq_neutralisee"
DEFINE  TDO_voie_inverse_s_inv		"pix_fav_inverse_s_inv"
DEFINE  TDO_voie_normale_s_inv		"pix_fav_normale_s_inv"

DEFINE	TDO_voie_etroite_normale		"pix_fav_etroite_normale"
DEFINE	TDO_voie_etroite_inverse		"pix_fav_etroite_inverse"
DEFINE	TDO_voie_etroite_bloquee		"pix_fav_etroite_bloquee"
DEFINE 	TDO_voie_etroite_bau_normale		"pix_fav_etroite_bau_normale"
DEFINE	TDO_voie_etroite_neutralisee		"pix_fav_etroite_neutralisee"
DEFINE	TDO_voie_etroite_bloq_neutralisee	"pix_fav_etroite_bloq_neutralisee"
DEFINE  TDO_voie_etroite_inverse_s_inv		"pix_fav_etroite_inverse_s_inv"
DEFINE  TDO_voie_etroite_normale_s_inv		"pix_fav_etroite_normale_s_inv"

/*A ***************************************************************************
 **                                                                          **
 **                     Declaration des symboles publics                     **
 **                                                                          **
 ******************************************************************************/

/*A
 * Donnees statiques
 * -----------------
 */

DEFINE	vg_les_autoroutes	"vg_les_autoroutes"
DEFINE	vg_autoroutes_district	"vg_autoroutes_district"
DEFINE  vg_toutes_les_autoroutes "vg_toutes_les_autoroutes"
DEFINE	vg_les_bifurcations	"vg_les_bifurcations"
DEFINE	vg_les_sens_circulation	"vg_les_sens_circulation"
DEFINE	vg_les_districts	"vg_les_districts"
DEFINE	vg_les_echangeurs	"vg_les_echangeurs"
DEFINE	vg_echangeurs_district	"vg_echangeurs_district"
DEFINE	vg_les_gares		"vg_les_gares"
DEFINE	vg_les_peages		"vg_les_peages"
DEFINE	vg_gares_district	"vg_gares_district"
DEFINE	vg_peages_district	"vg_peages_district"
DEFINE	vg_les_lits_arret	"vg_les_lits_arret"
DEFINE	vg_lits_arret_district	"vg_lits_arret_district"
DEFINE	vg_les_aires		"vg_les_aires"
DEFINE	vg_aires_district	"vg_aires_district"
DEFINE	vg_itpc_district	"vg_itpc_district"
DEFINE	vg_cantons_district	"vg_cantons_district"
DEFINE	vg_les_types_FMC	"vg_les_types_FMC"
DEFINE	vg_origines_FMC		"vg_origines_FMC"
DEFINE	vg_types_bouchon	"vg_types_bouchon"
DEFINE	vg_types_bouchon_valide	"vg_types_bouchon_valide"
DEFINE	vg_types_travaux	"vg_types_travaux"
DEFINE	vg_types_travaux_valide	"vg_types_travaux_valide"
DEFINE	vg_cond_meteo		"vg_cond_meteo"
DEFINE	vg_cond_visibilite	"vg_cond_visibilite"
DEFINE	vg_cond_etat_chau	"vg_cond_etat_chau"
DEFINE	vg_cond_chantier	"vg_cond_chantier"
DEFINE	vg_les_motifs_appel	"vg_les_motifs_appel"
DEFINE	vg_les_types_equipement	"vg_les_types_equipement"
DEFINE	vg_liste_postes		"vg_liste_postes"
DEFINE	vg_les_pix_fav		"vg_les_pix_fav"
DEFINE	vg_les_pix_bau		"vg_les_pix_bau"
DEFINE	vg_les_pix_fav_inv	"vg_les_pix_fav_inv"
DEFINE	vg_les_pix_bau_inv	"vg_les_pix_bau_inv"
DEFINE	vg_les_points_car	"vg_les_points_car"
DEFINE	vg_les_types_astreintes	"vg_les_types_astreintes"     ' MTFA, MTFS, ...
DEFINE	vg_les_categ_astreintes	"vg_les_categ_astreintes"     ' MTFA, MTFS, ...
DEFINE	vg_les_cameras		"vg_les_cameras"   
DEFINE	vg_les_magnetoscopes	"vg_les_magnetoscopes"
DEFINE	vg_les_LTV		"vg_les_LTV_"
DEFINE	vg_type_PMV		"vg_type_PMV"
DEFINE 	vg_type_PMVA		"vg_type_PMVA"
DEFINE  vg_type_PRV		"vg_type_PRV"
DEFINE	vg_type_Picto		"vg_type_Picto"
DEFINE	vg_texte_copier_coller	"vg_texte_copier_coller"
DEFINE	vg_Destination_TDP	"vg_Destination_TDP"
DEFINE	vg_info_district_local	"vg_info_district_local"
DEFINE	vg_mail_rtfm_asf	"vg_mail_rtfm_asf"
DEFINE	vg_type_SAGA		"vg_type_SAGA"
DEFINE	vg_eqt_SAGA		"vg_eqt_SAGA"

DEFINE	vg_zones		"vg_zones"
/*A
 * Donnees session
 * ---------------
 */

DEFINE	vg_repertoire_migrazur	"vg_repertoire_migrazur"

DEFINE	vg_siecle		"vg_siecle"

DEFINE	vg_canal_SGBD		"vg_canal_SGBD"
DEFINE	vg_canal_SGBD_Fiche	"vg_canal_SGBD_Fiche"

DEFINE	vg_nom_site		"vg_nom_site"
DEFINE	vg_site			"vg_site"
DEFINE	vg_numero_poste		"vg_numero_poste"
DEFINE	vg_poste		"vg_poste"
DEFINE	vg_operateur		"vg_operateur"
DEFINE	vg_numero_operateur	"vg_numero_operateur"
DEFINE	vg_profil_operateur	"vg_profil_operateur"
DEFINE	vg_familles		"vg_les_familles"

DEFINE 	vg_numero_display	"vg_numero_display"

DEFINE 	vg_num_fiche_operateur	"vg_num_fiche_operateur"
DEFINE	vg_cle_fiche_operateur	"vg_cle_fiche_operateur"
DEFINE  vg_type_imprimante	"vg_type_imprimante"

DEFINE	vg_type_machine		"vg_type_machine"

/* 14/02/2020 : Gestion incendie dans sens opposé pour ouvrage toulon */
DEFINE	vg_inc_autre_sens	"vg_inc_autre_sens"
/*A
 * Donnees dynamiques
 * ------------------
 */

DEFINE	vg_verrou		"vg_verrou"			' MTFM
DEFINE	vg_requete_en_cours	"vg_requete_en_cours"
DEFINE  vg_requete_xzae148_en_cours	"vg_requete_xzae148_en_cours"

DEFINE	vg_mode_liste_fmc	"vg_mode_liste_fmc"		' MTLT, ...
DEFINE	vg_fmc_ouverte		"vg_fmc_ouverte"		' MTLT, ...
DEFINE	vg_surcharge		"vg_surcharge"
DEFINE	vg_fiche_surcharge	"vg_fiche_surcharge"

DEFINE	vg_liste_reveils	"vg_liste_reveils"
DEFINE	vg_liste_reveils_opr	"vg_liste_reveils_opr"
DEFINE	vg_liste_eqt_indispo	"vg_liste_eqt_indispo"		' MTEI
DEFINE	vg_liste_alarmes	"vg_liste_alarmes"		' MTLM
DEFINE	vg_les_pau_en_garde	"vg_les_pau_en_garde"		' MTRA
DEFINE	vg_le_pau_en_ecoute	"vg_le_pau_en_ecoute"		' MTMT -> MTRA
DEFINE	vg_message_maintenance	"vg_message_maintenance"	' MTMN -> MTAT

DEFINE	vg_nom_fichier_ss	"vg_nom_fichier_ss"

DEFINE	vg_formation		"vg_formation"
DEFINE	vg_horodate_formation	"vg_horodate_formation"
DEFINE	vg_numero_scenario	"vg_numero_scenario"

DEFINE	vg_simuler_SQL		"vg_simuler_SQL"
DEFINE	vg_simuler_trace	"vg_simuler_trace"
DEFINE	vg_niveau_trace		"vg_niveau_trace"

DEFINE	vg_canal_SGBD_surcharge	"vg_canal_SGBD_surcharge"
DEFINE	vg_numero_poste_surcharge	"vg_numero_poste_surcharge"
DEFINE	vg_nom_site_surcharge	"vg_nom_site_surcharge"
DEFINE	vg_site_surcharge	"vg_site_surcharge"

DEFINE 	vg_canal_SGBD_pcs	"vg_canal_SGBD_pcs"
DEFINE	vg_canal_SGBD_distant	"vg_canal_SGBD_distant"
DEFINE  vg_canal_SGBD_his	"vg_canal_SGBD_his"

DEFINE	vg_F_T_gauche 	"vg_F_T_gauche"
DEFINE	vg_F_T_droite	"vg_F_T_droite"

DEFINE	vg_niv_diff	"vg_niv_diff"
DEFINE	vg_periode_diffTA "vg_periode_diffTA"
DEFINE  vg_periode_diff	"vg_periode_diff"
DEFINE  vg_zone_diff	"vg_zone_diff"
DEFINE  vg_troncon_TFM	"vg_troncon_TFM"
DEFINE  vg_couleur	"vg_couleur"

DEFINE  vg_datex	"vg_datex"
DEFINE  vg_datex_propriete	"vg_datex_propriete"

DEFINE  vg_datex_propriete_queue	"vg_datex_propriete_queue"
DEFINE	vg_icones_voies	"vg_icones_voies"
DEFINE	vg_codes_voies	"vg_codes_voies"

DEFINE	vg_destinataires_fax	"vg_destinataires_fax"


/*variables globales extension ouest*/
DEFINE	vg_comm_		"vg_comm_"
DEFINE	vg_operateur_		"vg_operateur_"
DEFINE	vg_comm_CI		"vg_comm_CI"
DEFINE	vg_fmc_locales		"vg_fmc_locales"
DEFINE  vg_t_evt_pcs		"vg_t_evt_pcs"
DEFINE	vg_t_info_pcs		"vg_t_info_pcs"
DEFINE	vg_t_picto_pcs		"vg_t_picto_pcs"

/*A
 * datagroups
 */
DEFINE 	vg_dg_mce		"vg_dg_mce"
DEFINE	vg_dg_dial		"vg_dg_dial"


/*A
 * sockets*/
define	vg_socket_envoi		"vg_socket_envoi"
define	vg_socket_recep		"vg_socket_recep"

define	vg_les_donnees_menu_principal	"vg_les_donnees_menu_principal"
define	vg_les_donnees_config_bd_tube	"vg_les_donnees_config_bd_tube"
define	vg_nb_poste_BT					"vg_nb_poste_BT"
define	vg_nb_vue_ventilation			"vg_nb_vue_ventilation"

/*variables globales de la gestion des astreintes*/
DEFINE	vg_debut		"vg_debut"
DEFINE	vg_fin			"vg_fin"
DEFINE	vg_task_id		"vg_task_id"
DEFINE	vg_zoom			"vg_zoom"
DEFINE	vg_appelant		"vg_appelant"
DEFINE	vg_num_comp		"vg_num_comp"
DEFINE	vg_calendrier		"vg_calendrier"
DEFINE	vg_competences		"vg_competences"
DEFINE	vg_interlocuteurs	"vg_interlocuteurs"
DEFINE	vg_toutes_competences	"vg_toutes_competences"
