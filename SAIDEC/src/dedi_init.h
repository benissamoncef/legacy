/*E*/
/* Fichier : $Id: dedi_init.h,v 1.16 2005/01/11 16:39:06 gesconf Exp $      Release : $Revision: 1.16 $       Date : $Date: 2005/01/11 16:39:06 $ 
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  SAIDEC
-------------------------------------------------------------------------------
* MODULE dedi * FICHIER dedi_init.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de donnees globales a la tache IHM de SAIDEC
*
*
-------------------------------------------------------------------------------
* HISTORIQUE :
* JM Guilhou	28/11/1994					creation
* Thomas	17/10/1995	: ajout de la constante AIDE_PATH (1.?)
* Thomas	15/11/1995	: modif IE_PATH_BDC (1.7)
* Thomas	15/11/1995	: modif AX_PATH (1.8)
* Thomas	20/11/1995	: modif format dedi_evenement dedi_evt dedi_perturbation (1.9)
* Thomas	12/12/1995	: ajout IE_MODELE_PERTU_PMV (1.9)
* Guilhou	22/05/1996	: modif constante dedi_numeroPosInterl (1.11) DEM/1125
* Guilhou	07/12/1996	: temps de parcours  1.12
* Guilhou	10/12/1997	: ajout nature avec bifurcation (1.13) dem/1534
* Niepceron	15/01/1999	: modif intergest (1.14) dem/1725
* ???		???		: ??? 1.15
* JMG		30/03/04	: ajout picto_bifurcation et bandeau_bifurcation 1.16
---------------------------------------------------------------------------- */

/*A ***************************************************************************
 **                                                                          **
 **                    Declaration des constantes			             **
 **                                                                          **
 ******************************************************************************/


/*A
 * Definition des parametres de connexion a la base de donnees
 * -----------------------------------------------------------
 */

DEFINE	COM_NOM_SGBD	"Sybase"
DEFINE	COM_USER		"MIGRAZUR"
DEFINE	COM_MOT_PASSE	"MIGRAZUR"
DEFINE	COM_BASE		"PRC"
DEFINE	COM_SERVEUR	"SQL_CI"				' auquel ajouter le site


/*A caractere de separation des champs dans les fichiers de config*/
DEFINE 	DEDI_CAR_SEPAR		"|"
DEFINE 	DEDI_CAR_VAR		"$"

/*A caractere valides*/
DEFINE	DEDI_CAR_VALIDES	"azertyuiopqsdfghjklmwxcvbn,!?1234567890)&é'(-è_çà)/*-+$£ù%µ§.;:?<>ABCDEFGHIJKLMNOPQRSTUVWXYZ"

/*A definition des fichiers de config*/
DEFINE	DEDI_MENU_AIDE	"../fichiers/aid/dedi_menu_aide" 'menu des fenetres d'aide 
DEFINE	DEDI_MU		"dedi_mu"		'manuel utilisateur de DEDI
DEFINE	AIDE_PATH	"../fichiers/aid/"
DEFINE	AX_PATH		"../modif/fichiers/die/"
DEFINE	IE_PATH		"../fichiers/mod/"
DEFINE	IE_PATH_BDC	"../modif/fichiers/bdc/"

DEFINE	IE_FILE_TYPES_FMC	"dedi_types_fmc.ie" 

DEFINE	IE_FILE_FORMUL_AUTOROUTE "dedi_ec5_autoroute.ie"
DEFINE	IE_FILE_FORMUL_CONSEIL	"dedi_ec5_conseil.ie"


DEFINE	IE_FILE_OBJ_PMV		"dedi_ec27.ie"		'objectifs d information pmv
DEFINE	IE_FILE_PERT_PMV	"dedi_ec29.ie"		'perturbations
DEFINE	IE_FILE_PERT_PMV_DATEX	"dedi_ec29_datex.ie"		'perturbations datex
DEFINE	IE_FILE_PRED_PMV		"dedi_ec33.ie"		'messages PMV predefinis
DEFINE	IE_FILE_PRED_PMV_DATEX		"dedi_ec33_datex.ie"	'messages PMV predefinis datex
DEFINE	IE_FILE_PRED_BAND_PMV	"dedi_ec33_band.ie"	'bandeau predefini
DEFINE	IE_FILE_EVT_PMV		"dedi_ec38.ie"		'evenements
DEFINE	IE_FILE_SEUIL_EVT_PMV		"dedi_ec38_seuil.ie"		'seuil fin evt
DEFINE	IE_FILE_LOCAL_PMV	"dedi_ec35_local.ie"	'localisation
DEFINE	IE_FILE_PROBA_PMV	"dedi_ec35_proba.ie"	'probabilite
DEFINE	IE_FILE_DUREE_PMV	"dedi_ec35_duree.ie"	'duree
DEFINE	IE_FILE_DIRECTION_PMV	"dedi_ec35_direction.ie" 'direction
DEFINE	IE_FILE_BOUCHON_PMV	"dedi_ec35_longbouchon.ie"
DEFINE	IE_FILE_ALTERNAT_PMV 	"dedi_ec37.ie"		'alternat
DEFINE	IE_FILE_NATURE_PMV 	"dedi_ec35_nature.ie"	'nature
DEFINE	IE_FILE_TDP_PMV		"dedi_ec80.ie"		' msg temps de parcours


DEFINE	IE_FILE_EVT_TFM		"dedi_ec1.ie"		'evts<->message tfm
DEFINE	IE_FILE_MEV_TFM		"dedi_ec1b.ie"		'mev des textes travaux
DEFINE	IE_FILE_OBJ_TFM		"dedi_ec2.ie"		'objectifs d'info tfm
DEFINE	IE_FILE_CAUSE_TFM	"dedi_ec5_cause.ie"	'elt cause dans message tfm
DEFINE	IE_FILE_CHAUSSEE_TFM	"dedi_ec5_chaussee.ie"	'elt chaussee dans msg tfm
DEFINE	IE_FILE_NATUREGN_TFM	"dedi_ec5_naturegn.ie"	'elt nature dans msg tfm
DEFINE	IE_FILE_NATUREGV_TFM	"dedi_ec5_naturegv.ie"	'elt nature dans msg tfm
DEFINE	IE_FILE_FIN_TFM		"dedi_ec5_fin.ie"	'elt fin dans message tfm
DEFINE	IE_FILE_CONSEIL_TFM	"dedi_ec5_conseil.ie"	'elt conseil dans message tfm
DEFINE	IE_FILE_GRAVITE_TFM	"dedi_ec5_gravite.ie"	'elt gravite dans message tfm
DEFINE	IE_FILE_NEUTRAL_TFM	"dedi_ec5_neutral.ie"	'elt voies neutralisees
DEFINE	IE_FILE_PERIODE_TFM	"dedi_ec5_periode.ie"	'elt periode dans message tfm
DEFINE	IE_FILE_LOCAL_TFM	"dedi_ec5_local.ie"	'elt localisation
DEFINE	IE_FILE_BOUCHON_TFM	"dedi_ec5_bouchon.ie"	'elt bouchon
DEFINE	IE_FILE_VISIBILITE_TFM	"dedi_ec5_visibilite.ie" 'elt visibilite
DEFINE	IE_FILE_RETOUR_TFM	"dedi_ec5_retour.ie"	'elt retour
DEFINE	IE_FILE_DEBUT_TFM	"dedi_ec5_debut.ie"	'elt debut
DEFINE	IE_FILE_DUREE_TFM	"dedi_ec5_duree.ie"	'elt duree
DEFINE	IE_FILE_VEHICULES_TFM	"dedi_ec5_vehicules.ie"	'elt vehicules
DEFINE	IE_FILE_VICTIMES_TFM	"dedi_ec5_victimes.ie"	'elt victimes
DEFINE	IE_FILE_BLOQUEES_TFM	"dedi_ec5_bloquees.ie"	'elt voies bloquees
DEFINE	IE_FILE_SECOURS_TFM	"dedi_ec5_secours.ie"	'elt secours
DEFINE	IE_FILE_TDP_PERTURBE_TFM	"dedi_ec5_tdp_perturbe.ie"	'msg tfm de tdp perturbe
DEFINE	IE_FILE_TDP_NORMAL_TFM	"dedi_ec5_tdp_normal.ie"	' msg tfm de retour a la normale du tdp
DEFINE	IE_FILE_ENTETE_DATEX_TFM "dedi_ec5_entete_datex.ie"	' msg recuperer de datex


DEFINE	IE_FILE_MOYENS_CI	"dedi_ec7_ci.ie"	'moyens d'appel CI
DEFINE	IE_FILE_MOYENS_PC	"dedi_ec7_pc.ie"	'moyens d'appel PC
DEFINE	IE_FILE_APPELS_CI	"dedi_ec13_ci.ie"
DEFINE	IE_FILE_APPELS_PC	"dedi_ec13_pc.ie"
DEFINE	IE_FILE_APPELS_DIFFERES_CI "dedi_ec13_differes_ci.ie"
DEFINE	IE_FILE_APPELS_DIFFERES_PC "dedi_ec13_differes_pc.ie"
DEFINE	IE_FILE_APPELS_CI_DATEX	"dedi_ec13_ci_datex.ie"
DEFINE	IE_FILE_APPELS_PC_DATEX	"dedi_ec13_pc_datex.ie"
DEFINE	IE_FILE_APPELS_DIFFERES_CI_DATEX "dedi_ec13_differes_ci_datex.ie"
DEFINE	IE_FILE_APPELS_DIFFERES_PC_DATEX "dedi_ec13_differes_pc_datex.ie"


DEFINE	IE_FILE_PRIORITE_PC	"dedi_ec43_pc.ie"	'priorite actions PC
DEFINE	IE_FILE_PRIORITE_CI	"dedi_ec43_ci.ie"	'priorite actions CI


DEFINE	IE_FILE_MEV_FAX		"dedi_ec21.ie"		'mev des textes travaux
DEFINE	IE_FILE_STRUCT_FAX	"dedi_ec25.ie"		'structure d'un fax

DEFINE	IE_FILE_OBJET_FAX	"dedi_ec22_objet.ie"	'rubrique objet du fax
DEFINE	IE_FILE_BLOQUE_FAX	"dedi_ec23_bloque.ie"	'rubrique voies bloquees
DEFINE	IE_FILE_SECTION_FAX	"dedi_ec23_section.ie"
DEFINE	IE_FILE_INTITULE_FAX	"dedi_ec22.ie"		'liste des intitules
DEFINE	IE_FILE_INFORMATION_FAX	"dedi_ec21.ie"		'liste des informations
DEFINE	IE_FILE_AUTOROUTE_FAX	"dedi_ec23_autoroute.ie"	'rub autoroute
DEFINE	IE_FILE_CIRCULATION_FAX	"dedi_ec23_circulation.ie"	'rub circulation
DEFINE	IE_FILE_DEBUT_FAX	"dedi_ec23_debut.ie"	'rub heure de debut
DEFINE	IE_FILE_FIN_FAX	"dedi_ec23_fin.ie"	'rub heure de fin
DEFINE	IE_FILE_MATIERE_FAX	"dedi_ec23_matiere.ie"	'rub matieres dangereuses
DEFINE	IE_FILE_PR_FAX		"dedi_ec23_pr.ie"	'rub pr
DEFINE	IE_FILE_GRAVITE_FAX	"dedi_ec23_gravite.ie"	'rub gravite
DEFINE	IE_FILE_NEUTRAL_FAX	"dedi_ec23_neutral.ie"	'rub voies neutral
DEFINE	IE_FILE_NBVEH_FAX	"dedi_ec23_nbveh.ie"	'rub nb de vehicules
DEFINE	IE_FILE_POLICE_FAX	"dedi_ec23_police.ie"	' fax police
DEFINE	IE_FILE_DEPANNEUR_ARRETE_FAX	"dedi_ec23_depanneur_arrete.ie"		' fax depanneur vehicule arrete
DEFINE	IE_FILE_DEPANNEUR_FEU_FAX	"dedi_ec23_depanneur_feu.ie"		' fax depanneur vehicule en feu
DEFINE	IE_FILE_DEPANNEUR_PANNE_FAX	"dedi_ec23_depanneur_panne.ie"		' fax depanneur vehicule en panne
DEFINE	IE_FILE_DEPANNEUR_ACCIDENT_FAX	"dedi_ec23_depanneur_accident.ie"	' fax depanneur accident 

DEFINE	IE_FILE_STRATEGIE	"dedi_ec36.ie"		'strategie sorties
DEFINE	IE_FILE_FONCT_NAV	"dedi_ec42.ie"		'mode de fonct de la nav

DEFINE	IE_FILE_EXPORT_INTERGEST "dedi_ec44.ie"	' export intergestionnaire

/*modeles rtie*/
DEFINE	IE_MODELE_ENTETE	"bdc_entete"		'fichier d'entete

DEFINE	IE_MODELE_EVT		"fmc"			'def. des classes d'evt

DEFINE	IE_MODELE_EVT_TFM		"msg_evt_exclus"	'evts a ne pas decrire a TFM
DEFINE	IE_MODELE_MEV_TFM		"msg_mev_travaux"	'mev des textes travux
DEFINE	IE_MODELE_C1_EVT_TFM	"msg_evt_exclus_c1"	'partie condition de msg_evt1
DEFINE	IE_MODELE_C2_EVT_TFM	"msg_evt_exclus_c2"	'partie condition de msg_evt2
DEFINE	IE_MODELE_DEF_TFM		"msg_def"		'structure des messages TFM
DEFINE	IE_MODELE_CONC_DEF_TFM	"msg_def_conc"		'partie conclusion de msg_def
DEFINE	IE_MODELE_GN_TFM		"msg_elt_gn"		'elt gn, cas particuliers
DEFINE	IE_MODELE_GV_TFM		"msg_elt_gv"		'elt gv, cas particuliers
DEFINE	IE_MODELE_FIN_TFM		"msg_elt_fin"		'elt fin, cas particuliers
DEFINE	IE_MODELE_AUTOROUTE		"autoroute"		'instances de la classe autoroute
DEFINE	IE_MODELE_BOUCHON_TFM	"msg_elt_bouchon"	'texte de l'elt bouchon
DEFINE	IE_MODELE_CONSEIL_TFM	"msg_elt_conseil"	'texte de l'elt conseil
DEFINE	IE_MODELE_DEBUT_TFM		"msg_elt_debut"	'texte de l'elt heure de debut
DEFINE	IE_MODELE_DUREE_TFM		"msg_elt_duree"	'texte de l'elt duree
DEFINE	IE_MODELE_GLISSE_TFM	"msg_elt_glisse"	'texte de l'elt glisse
DEFINE	IE_MODELE_GRAVITE_TFM	"msg_elt_gravite"	'texte de l'elt gravite
DEFINE	IE_MODELE_LOCAL_TFM		"msg_elt_local"	'texte de l'elt localisation
DEFINE	IE_MODELE_NEUTRAL_TFM	"msg_elt_neutral"	'texte de l'elt neutralisations
DEFINE	IE_MODELE_PERIODE_TFM	"msg_elt_periode"	'texte de l'elt periode
DEFINE	IE_MODELE_RETOUR_TFM	"msg_elt_retour"	'texte de l'elt retour a la normale
DEFINE	IE_MODELE_SECOURS_TFM	"msg_elt_secours"	'texte de l'elt secours
DEFINE	IE_MODELE_VEHICULES_TFM	"msg_elt_vehicules"	'texte de l'elt veh. impliques
DEFINE	IE_MODELE_VICTIMES_TFM	"msg_elt_victimes"	'texte de l'elt victimes
DEFINE	IE_MODELE_VISIBILITE_TFM	"msg_elt_visibilite" 'texte de l'elt visibilite
DEFINE	IE_MODELE_BLOQUE_TFM	"msg_elt_bloque"	'texte de l'elt voies bloquees 
DEFINE	IE_MODELE_TDP_TFM	"msg_elt_tdp"			'msg tdp 
DEFINE	IE_MODELE_ENTETE_TFM	"msg_elt_datex"			'msg datex 

DEFINE	IE_MODELE_MOYEN_PC_INT	"app_int_pc"	'def. des interlocuteurs PC
DEFINE	IE_MODELE_MOYEN_CI_INT	"app_int_ci"	'def. des interlocuteurs CI
DEFINE	IE_MODELE_CONC_PC		"app_conc_pc"	'conclusion des cas d'appel PC
DEFINE	IE_MODELE_CONC_CI		"app_conc_ci"	'conclusion des cas d'appel CI
DEFINE	IE_MODELE_CONC_PC_DATEX		"app_conc_pc_datex"	'conclusion des cas d'appel PC DATEX
DEFINE	IE_MODELE_CONC_CI_DATEX		"app_conc_ci_datex"	'conclusion des cas d'appel CI DATEX
DEFINE	IE_MODELE_CAS_PC_INT	"app_cas_pc"	'cas d'appel des interlocuteurs PC
DEFINE	IE_MODELE_CAS_CI_INT	"app_cas_ci"	'cas d'appel des interlocuteurs CI
DEFINE	IE_MODELE_DIF_PC_INT	"app_dif_pc"	'cas d'appel differe des int. PC
DEFINE	IE_MODELE_DIF_CI_INT	"app_dif_ci"	'cas d'appel differe des int. CI
DEFINE	IE_MODELE_CAS_PC_INT_DATEX	"app_cas_pc_datex"	'cas d'appel des interlocuteurs PC pour FMC DATEX
DEFINE	IE_MODELE_CAS_CI_INT_DATEX	"app_cas_ci_datex"	'cas d'appel des interlocuteurs CI pour FMC DATEX
DEFINE	IE_MODELE_DIF_PC_INT_DATEX	"app_dif_pc_datex"	'cas d'appel differe des int. PC pour FMC DATEX
DEFINE	IE_MODELE_DIF_CI_INT_DATEX	"app_dif_ci_datex"	'cas d'appel differe des int. CI pour FMC DATEX
DEFINE	IE_MODELE_COND_PC		"app_cond_pc"	'condition des appels differes PC 
DEFINE	IE_MODELE_COND_CI		"app_cond_ci"	'conclusion des appels differes CI
DEFINE	IE_MODELE_COND_PC_DATEX		"app_cond_pc_datex"	'condition des appels differes PC  pour FMC DATEX
DEFINE	IE_MODELE_COND_CI_DATEX		"app_cond_ci_datex"	'conclusion des appels differes CI pour FMC DATEX


DEFINE	IE_MODELE_DEF_PMV	"pmv_def"		'def. objectifs pmv
DEFINE	IE_MODELE_EQT_PMV	"pmv_eqpt"		'choix des eqpt pmv
DEFINE	IE_MODELE_OBJ_PMV	"pmv_obj"		'def. des classes obj
DEFINE	IE_MODELE_BAN_PMV	"pmv_bandeau"		'texte sur bandeau
DEFINE	IE_MODELE_NATURE_PMV	"pmv_elt_nature"	'texte de l'elt nature
DEFINE	IE_MODELE_PROBA_PMV	"pmv_elt_proba"		'texte de l'elt probabilite
DEFINE	IE_MODELE_CONSEIL_PMV	"pmv_elt_conseil"	'texte de l'elt conseil
DEFINE	IE_MODELE_LOCAL_PMV	"pmv_elt_local"		'texte de l'elt localisation
DEFINE	IE_MODELE_DIRECTION_PMV	"pmv_elt_direction"	'texte de l'elt direction
DEFINE	IE_MODELE_DUREE_PMV	"pmv_elt_duree"		'texte de l'elt duree
DEFINE	IE_MODELE_LONGUEUR_PMV	"pmv_elt_longueur"	'texte de l'elt longueur
DEFINE	IE_MODELE_INIT_PMV	"pmv"		'strategie de designation des sorties
DEFINE	IE_MODELE_PERT_PMV	"pmv_pert"	'definition des regles perturbations datex
DEFINE	IE_MODELE_PERT_PMV_DATEX	"pmv_pert_datex"	'definition des regles perturbations
DEFINE	IE_MODELE_PERTU_PMV	"pmv_obj_pert"	'definition des objets perturbations
DEFINE	IE_MODELE_PERTU_PMV_DATEX	"pmv_obj_pert_datex"	'definition des objets perturbations datex
DEFINE	IE_MODELE_PICTO_PMV	"pmv_picto_interdit"	'pictos interdits/objectifs
DEFINE	IE_MODELE_PRED_PMV	"pmv_pred"		'messages PMV predefinis
DEFINE	IE_MODELE_PRED_PMV_DATEX	"pmv_pred_datex"		'messages PMV predefinis datex
DEFINE	IE_MODELE_PRED_BAND_PMV	"pmv_pred_band"	'bandeau messages predefinis
DEFINE	IE_MODELE_PRED_BAND_PMV_DATEX	"pmv_pred_band_datex"	'bandeau messages predefinis datex
DEFINE	IE_MODELE_ALTERNAT_PMV "pmv_alternat"	'alternat
DEFINE	IE_MODELE_PRED_TDP_PMV	"pmv_pred_tdp"	'msg tdp pour fmc operateur



DEFINE	IE_MODELE_NAV		"nav_mode"	'modes de fonctionnement nav"

DEFINE	IE_MODELE_PRIORITE_PC	"priorite_pc"	'priorites des actions PC
DEFINE	IE_MODELE_PRIORITE_CI	"priorite_ci"	'priorites des actions CI


DEFINE	IE_MODELE_MEV_FAX		"fax_mev_travaux"	'mev des textes travux
DEFINE	IE_MODELE_SECTION_FAX	"fax_elt_section"	'rubrique section
DEFINE	IE_MODELE_INTITULE_FAX	"fax_elt"			'intitules des rubriques
DEFINE	IE_MODELE_DEF_FAX		"fax_def"		'structure des messages FAX
DEFINE	IE_MODELE_CONC_DEF_FAX	"fax_def_conc"		'partie conclusion de fax_def
DEFINE	IE_MODELE_BLOQUE_FAX	"fax_elt_bloque"	' rubrique voies bloquees
DEFINE	IE_MODELE_AUTOROUTE_FAX	"fax_elt_autoroute"	' rubrique autoroute
DEFINE	IE_MODELE_CIRCULATION_FAX "fax_elt_circul"	' rubrique circulation
DEFINE	IE_MODELE_DEBUT_FAX 	"fax_elt_debut"	' rubrique heure de debut
DEFINE	IE_MODELE_FIN_FAX 	"fax_elt_fin"	' rubrique heure de fin
DEFINE	IE_MODELE_MATIERE_FAX 	"fax_elt_matdang"	' rubrique mat dang
DEFINE	IE_MODELE_PR_FAX 		"fax_elt_pr"		' rubrique PR
DEFINE	IE_MODELE_GRAVITE_FAX 	"fax_elt_gravite"	' rubrique gravite
DEFINE	IE_MODELE_NEUTRAL_FAX 	"fax_elt_neutral"	' rub voies neutral
DEFINE	IE_MODELE_NBVEH_FAX 	"fax_elt_nbveh"	' rub nb de vehicules
DEFINE	IE_MODELE_OBJET_FAX 	"fax_elt_objet"	' rub objet
DEFINE	IE_MODELE_SPE_FAX		"fax_spe"			' fax speciaux
DEFINE	IE_MODELE_CONC_SPE_FAX	"fax_spe_conc"		' fax speciaux

DEFINE	IE_MODELE_EXPORT_INTERGEST	"export"		' export intergestionnaire

/*A suffixes des fichiers rtie */
DEFINE	IE_SUFFIXE_RL		"rl"			'definition des regles
DEFINE	IE_SUFFIXE_CL		"cl"			'definition des classes
DEFINE	IE_SUFFIXE_OB		"ob"			'definition des objets statiques
DEFINE	IE_SUFFIXE_IN		"in"			'initialisations generales
DEFINE	IE_SUFFIXE_LI		"li"			'initialisations de cycle

DEFINE	IE_CONF_INTERLOCUTEURS	"Conf_Astreintes_BDC"	'les interlocuteurs utilises dans BDC


/*A strategies de designation des sorties*/
DEFINE	DEDI_sortie_nom_numero		1
DEFINE	DEDI_lib_sortie_nom_numero	"nom_et_numero"
DEFINE	DEDI_sortie_nom			2
DEFINE	DEDI_lib_sortie_nom		"nom"
DEFINE	DEDI_sortie_numero		3
DEFINE	DEDI_lib_sortie_numero		"numero"
DEFINE 	DEDI_sortie_sans_nom_numero	4
DEFINE	DEDI_lib_sortie_sans_nom_numero	"relative"


/*A mode generation ou impression*/
DEFINE	DEDI_MODE_GENERE		1
DEFINE	DEDI_MODE_IMPRIM		2

DEFINE	DEDI_SUFFIXE_IMPRIM		"_impr"


/*constantes de designation des fichiers a generer*/
DEFINE	DEDI_GENERE_TOUT		1
DEFINE	DEDI_GENERE_TFM			2
DEFINE	DEDI_GENERE_INTERLOCUTEURS	3
DEFINE	DEDI_GENERE_FAX			4
DEFINE 	DEDI_GENERE_PMV			5
DEFINE	DEDI_GENERE_NAV			6
DEFINE	DEDI_GENERE_PRIORITE		7
DEFINE	DEDI_GENERE_EXPORT		8


/*A definition des pokes*/
DEFINE	dedi_canal_fin			1001	'poke de fin


DEFINE	dedi_canal_ec29_valide		1002	'valide la liste des objectifs
DEFINE	dedi_canal_ec29_ajobj		1003	' ajoute un objectif
DEFINE	dedi_canal_ec29_annule		1004	'annule
DEFINE	dedi_canal_ec29_modifobj	1005	'modifie un objectif
DEFINE	dedi_canal_ec29_supobj		1006	'supprime un objectif
DEFINE	dedi_canal_ec29_fin		1012	'supprime un objectif


DEFINE	dedi_canal_ec13_ajinterl	1007	'ajout d'un interlocuteur
DEFINE	dedi_canal_ec13_supinterl	1008	'suppression d'un interlocuteur

DEFINE	dedi_canal_trace		1009
DEFINE	dedi_canal_trace_point		1010
DEFINE	dedi_canal_trace_fin		1011


DEFINE	dedi_canal_ec43_fin		1013
DEFINE	dedi_canal_ec43_ajinterl	1014	'ajout d'un appel
DEFINE	dedi_canal_ec43_supinterl	1015	'suppression d'un appel
DEFINE	dedi_canal_ec43_valide		1016	'valide
DEFINE	dedi_canal_ec43_annule		1017	'annule

DEFINE	dedi_canal_ec13_valide		1018	'valide
DEFINE	dedi_canal_ec13_annule		1019	'annule

DEFINE	dedi_canal_ec37_fin			1020	'ferme



/*A ***************************************************************************
 **                                                                          **
 **                    Declaration des formats de donnees                    **
 **                                                                          **
 ******************************************************************************/

/* variables autorisees par fichier rtie*/
FORMAT dedi_variables
	nomAX,
	nomIE,
	fichier

/* classe Rtie evt */
FORMAT dedi_evt
	fmc,
	classe,
	classe_mere,
	attributs,
	gn_tfm,
	gv_tfm,
	fin_tfm,
	cause_tfm,
	objet_fax,
	nature_pmv,
	nature_fin_pmv,
	proba_signale_pmv,
	proba_confirme_pmv,
	picto_pmv,
	nature_bifurc_pmv

/* objectifs d'information PMV */
FORMAT dedi_objectif
	nom,
	elt1,
	elt2,
	elt3,
	pmv,
	conseil1,
	conseil2,
	conseil3,
	bandeau,
	distance,
	distance_bifurc,
	pictos

define dedi_pos_nom		0
define dedi_pos_elt1	1
define dedi_pos_elt2	2
define dedi_pos_elt3	3
define dedi_pos_pmv		4
define dedi_pos_conseil	5
define dedi_pos_bandeau	8
define dedi_pos_distance	9
define dedi_pos_pictos	11
define dedi_pos_distance_bifurc	10


/* perturbations PMV*/
FORMAT dedi_perturbation
	nom,
	priorite,
	obj1,
	obj2,
	obj3,
	def,
	pmvl1,
	pmvl2,
	pmvl3,
	picto,
	proba1,
	proba2,
	proba3,
	proba4,
	proba5,
	proba6,
	bifurc1,
	bifurc2,
	bifurc3

define dedi_pos_priorite	1
define dedi_pos_obj1	2
define dedi_pos_obj2	3
define dedi_pos_obj3	4
define dedi_pos_def		5
define dedi_pos_l1		6
define dedi_pos_l2		7
define dedi_pos_l3		8
define dedi_pos_picto	9
define dedi_pos_p1		10
define dedi_pos_p2		11
define dedi_pos_p3		12
define dedi_pos_p4		13
define dedi_pos_p5		14
define dedi_pos_p6		15
define dedi_pos_b1		16
define dedi_pos_b2		17
define dedi_pos_b3		18

/* message PMV predefini*/
FORMAT dedi_predefini
	nom,
	priorite,
	pmvl1,
	pmvl2,
	pmvl3,
	pmva1,
	pmva2,
	pmva3,
	pmvb1,
	pmvb2,
	pmvb3,
	bandeau,
	eqpt1,
	eqpt2,
	def,
	picto,
	auto1,
	pr1,
	sens1,	
	auto2,	
	pr2,
	sens2,
	dmax1,
	dmax_bifurc1,
	dmax2,
	dmax_bifurc2,
	pmvc1,
	pmvc2,
	pmvc3,
	picto_bifurcation,
	bandeau_bifurcation /*1.16*/

define dedi_pos_nom_pred	0
define dedi_pos_prio_pred	1
define dedi_pos_l1_pred		2
define dedi_pos_l2_pred		3
define dedi_pos_l3_pred		4
define dedi_pos_a1_pred		5
define dedi_pos_a2_pred		6
define dedi_pos_a3_pred		7
define dedi_pos_b1_pred		8
define dedi_pos_b2_pred		9
define dedi_pos_b3_pred		10
define dedi_pos_bandeau_pred	11
define dedi_pos_p1_pred		12
define dedi_pos_p2_pred		13
define dedi_pos_def_pred	14
define dedi_pos_picto_pred	15
define dedi_pos_auto1_pred	16
define dedi_pos_pr1_pred	17
define dedi_pos_sens1_pred	18
define dedi_pos_auto2_pred	19
define dedi_pos_pr2_pred	20
define dedi_pos_sens2_pred	21

/*message PMV de temps de parcours non lie a un evenement*/
FORMAT dedi_tdp_pmv
	pmv,
	picto,
	texte,
	priorite,
	bandeau

/*evenements*/
FORMAT dedi_evenement
	nom,
	picto,
	t1,
	t2,
	t3,
	t4,
	t5,
	t6,
	p1,
	p2,
	p3,
	p4,
	p5,
	p6,
	t7,
	t8,
	t9

/*pictogrammes*/
FORMAT dedi_picto
	trigramme,
	libelle


/*objectif trafic FM*/
FORMAT dedi_objectif_tfm
	nom,
	condition,
	type,
	duree,
	def


/*astreintes*/
FORMAT dedi_astreinte
	nom,
	numero

define dedi_pos_nomAstreinte		0
define dedi_pos_numeroAstreinte	1

/*interlocuteurs */
FORMAT dedi_interl
	nom,
	fax,
	appel,
	astreinte,
	numero

define dedi_pos_nomInterl		0
define dedi_pos_faxInterl		1
define dedi_pos_appelInterl		2
define dedi_pos_numeroInterl		4


/*autoroute*/
FORMAT dedi_autoroute
	nom,
	libelle,
	sens1,
	sens2,	
	numero

/*action*/
FORMAT dedi_action
	nom,
	type,
	identifiant

define	dedi_action_tfm		0
define	dedi_action_appel		1
define	dedi_action_fax		2
define	dedi_action_pmv		3
define	dedi_action_nav		4
define	dedi_action_tunnel		5
define	dedi_action_echangeur	6
define  dedi_action_export	7



/*A ***************************************************************************
 **                                                                          **
 **                     Declaration des symboles publics                     **
 **                                                                          **
 ******************************************************************************/

DEFINE	vg_canal_SGBD				"vg_canal_SGBD"

DEFINE	vg_evts_ie				"vg_evts_ie"

DEFINE	vg_objectifs_pmv			"vg_objectifs_pmv"
DEFINE	vg_perturbations_pmv		"vg_perturbations_pmv"

DEFINE 	vg_strategie_sortie			"vg_strategie_sortie"
DEFINE 	vg_noms_strategie_sortie		"vg_noms_strategie_sortie"

DEFINE	vg_evenements				"vg_evenements"
DEFINE	vg_pictos					"vg_pictos"
DEFINE	vg_types_evts				"vg_types_evts"

DEFINE	vg_astreintes				"vg_astreintes"

DEFINE	vg_objectifs_tfm			"vg_objectifs_tfm"
DEFINE	vg_messagetfm_travaux		"vg_messagetfm_travaux"

DEFINE	vg_presence_ec13			"vg_presence_ec13"
DEFINE	vg_presence_ec29			"vg_presence_ec29"

DEFINE	vg_autoroutes				"vg_autoroutes"
	
DEFINE	vg_mode					"vg_mode"

DEFINE	vg_variables				"vg_variables"

