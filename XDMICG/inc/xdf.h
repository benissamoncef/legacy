/* Fichier : $Id: xdf.h,v 1.72 2020/11/03 18:03:31 pc2dpdy Exp $	Release : $Revision: 1.72 $        Date : $Date: 2020/11/03 18:03:31 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xdf * FICHIER xdf.h
******************************************************
* DESCRIPTION DU MODULE :
*
*
******************************************************
* HISTORIQUE :
*
* T.Milleville   24 Aou 1994	: Creation
*
* O.MERCIER   	13 Oct 1994	:  V 1.5
*			Ajout de XDF_BILAN_CONFIG
*************
*  O.MERCIER	18 Oct 1994	: V 1.7
*			Ajout de XDF_CONF_DISTRICT
*				XDF_CONF_AUTOROUTE
*				XDF_CONF_PORTION
*				XDF_CONF_NOEUD
*************
*  F.VOLCIC	11 Nov 1994	: V 1.9
*			Modif des noms de fichiers de configuration base
*************
*  F.VOLCIC	11 Nov 1994	: V 1.9
*			Modif des noms de fichiers de configuration base
*************
*  A.FLAIDAR	25 Nov 1994	: V 1.10
*			Ajout :
*			XDF_Config_Lignes
*			XDF_Presence_Equip
*			XDF_COMMENTAIRE
*			XDF_Alarme
*			XDF_PATH_FIC_ALARME
*************
*  D.MISMER	11 Nov 1994	: V 1.13
*			Ajout des fichiers de config d'EQUEXT
*************
* O. MERCIER	28 Nov 1994	: V 1.14
*			Ajout de XDF_TYPE_ASTREINTE
*************
* F.VOLCIC	20 Dec 1994	: V 1.20
*			Ajout de XDF_Dem_Indiv
*************
* O. MERCIER	22 Dec 1994	: V 1.21
*  			Ajout de XDF_BILAN_SVG_BASE
*************
* O. MERCIER    22 Dec 1994     : V 1.22
*  			Ajout de XDF_BILAN_REST_BASE
*************
* O. MERCIER 	22 Dec 1994	: V 1.23
* Rajoue apres disparition de XDF_CHECKLIST
*************
* E. NAGIEL     26 Dec 1994	: V 1.24
*           MAJ XDF_Config_Scenar
*************
* M. CHARLES     28 Dec 1994	: V 1.25
*           Rien
*
* T.Milleville   30 Dec 1994	: Ajout constant XDF_BILAN_CONFIG_TMP
*					et  XDF_BON_LIVRAISON_TMP
*					V 1.26
*************
*
* F.VOLCIC	02 Jan 1995	: V 1.27
*			Ajout de XDF_CALENDRIER_CONTEXTE
*************
*
* NOUAILLE M.  02 Jan 1995      : V 1.28
*			Ajout de XDF_Config_Cstes_AUTOTEL
*
*************
* MERCIER	06 Jan 1995	: V 1.29
* 			Ajout de XDF_CATALOGUE_ARCHIVE
*
*************
* THOMAS	09 Jan 1995	: V 1.30
* 			Ajout de XDF_ASTREINTES_BDC
*************
* MERCIER	10 Jan 1995	: V 1.31
*			Ajout de XDF_BILAN_REST_BASE_HIST
*			et    de XDF_BILAN_SVG_BASE_HIST
*************
* MERCIER	11 Jan 1995	: 1.32
*			Ajout de XDF_BILAN_SAUVEG_HEBDO
*
*************
* MERCIER	11 Jan 1995	: 1.34 identifie
* Rajoue apres disparition de XDF_BON_LIVRAISON_MODELE
* 	XDF_ScenNAV_Connu
* Ajoue de XDF_TYPEFIC_BL_MODELE
*************
* J.P. LECRIVAIN  11 Jan 1995   : V 1.35
*                       Delimitation de zones "non Applix" (2eme fois !)
*************
* MISMER	 23 Jan 1995   : V 1.36
*			Ajout de XDF_Machine_Sup
* MISMER	 24 Jan 1995   : V 1.37
*			Ajout de XDF_ParamLS
* GABORIT	26 Jan 1995	: V 1.38
*			Ajout de XDF_Equation_Evt
* T.Milleville	16 Mar 95	: Ajout constante XDF_TMP_IMPRESSION V1.40
* F.VOLCIC	19 Mai 95	: Ajout constante XDF_Config_Utilisateurs V1.41
* MISMER	30 Mai 95	: Ajout constante XDF_Affichage_Def_PMV V1.42
* MISMER	03 Oct 95	: Ajout constante XDF_Nom_Serveur V1.43
* TORREGROSSA	01 Dec 95	: Ajout constante pour ajout de la config de fax V1.44
* MISMER	21 Oct 96	: Ajout constante XDF_CONF_RADT_VIRTUELLE (DEM/1252)
*           			  Ajout constante XDF_CONF_ALARME_ALERTE (DEM/1232)
*           			  Ajout constante XDF_Config_EQT_GEN_LCR (DEM/1232) 
*           			  Ajout constante XDF_CONF_ALERTE_DAI_TUNNEL (DEM/1309) V1.45
* MISMER	20 Fev 97	: Ajout constante fichier TDP (DEM/1395) (v1.46)
* MISMER	24 Mar 97	: Ajout constante fichier TDP suite (DEM/1396) (v1.47)
* Guilhou	02 jan 98	: ajout XDF_CONF_ZON_PMV (dem/1534) 1.48
* MISMER	19 Jan 98	: Ajout constante fichier PAL  (DEM/1539) (v1.50)
* MISMER	21 Jan 98	: Ajout constante fichier RAU timeout  (DEM/1542) (v1.51)
* JMG		13/09/05	: ajout constantes PMVA BAF BAD 1.52
* JMG		28/09/05	: ajout XDF_CONF_PMVA + BAF + BAD 1.53
* AAZ       30/08/2007  : ajout gtc renovee DEM 662
* JMG		01/10/08	: ajout XDC_CONF_ZON_TUN 1.55 DEM 835
* JPL		07/10/10	: ajout XDF_CONF_ZON_CAM (DEM 948)  1.57
* VR		04/11/11	: Ajout destination 1.58 (DEM/1014)
* VR		25/01/12	: Ajout Regulation 1.326(DEM/1016)
* VR		05/06/12	: Ajout TDP PMVA (DEM/1014 PMA)
* JMG		16/04/16	: ajout SAGA 1.61
* JPL		24/05/16	: Ajout XDF_CONF_PERIPH_SYSTEME (DEM 1165)  1.62
* JPL		31/05/16	: Declaration fonction XDF01_ValeurNom (DEM 1167)  1.63
* JMG		24/06/16	: ajout XDF_ST40	1.64
* LCL		15/11/16	: Ajout XDF_CONF_VIADUC (LOT23) 1.65
* JMG		16/12/16	: ajout zones PMVA et PMV SAGA 1.66 DEM1191
* JMG		26/06/17	: ajout zones urbaines convergence 1.67
* JPL		08/01/17	: Ajout XDF_CONF_STATION_DIT (DEM 1266)  1.69
* LCL		20/02/18	: Ajout XDF_CONF_CFE CTRL_FEUX 1.70 DEM1284
*				  		  ajout de XDF_CONF_FMC_MULTI_AUTO 
* LCL		11/05/18 	: ajout XDF_CONF_CAP GABEB 1.71 DEM1283
* JMG		01/01/2020	: IMU 1.72
* CGR		27/07/2020	: Ajout XDF_CONF_IMU 1.72 DEM-SAE155
* CGR		06/10/2020	: Ajout XDF_CONF_IMU_ZON et XDF_CONF_IMU_MSG DEM-SAE155
* ABE       02/08/22 	: Ajout des niche (NIC) et issue de secour (IS) pour localisation tunnel DEM-SAE403
* GGY		16/02/24	: Ajout des batiments (DEM-483)

******************************************************/
/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
* NOM DU RELECTEUR : 
* DATE             : 
* COMMENTAIRES     :
*
*
*******************************************************/

/* Non Applix - debut */
#ifndef xdf
#define xdf
/* Non Applix - fin */

/* fichiers inclus */

/* definitions de constantes exportees */
/* Prefixe du  nom du fichier de configuration de supervision */
/* de chaque machine */
/* ce prefixe est concatene au nom de machine                */
#define	XDF_NSUP				"nsup"

/* nom du fichier d'administration, utilise pour configurer */
/* l'environement du site : utilisation XZSA                */
#define XDF_NSUP_ADMINIS         		"nsup_adminis"
#define XDF_NSUP_LSITE				"nsup_adminis"
#define XDF_NSUP_LMACHINE			"nsup_adminis"
#define XDF_NSUP_LSSYSTEME			"nsup_adminis"
#define XDF_NSUP_LTACHE				"nsup_adminis"
#define XDF_NSUP_LTIMER				"nsup_timer"
#define XDF_NSUP_LENTITE			"nsup_adminis"
#define XDF_NSUP_JOURADM			"nsup_jouradm"
#define XDF_NSUP_JOURADM_TMP			"nsup_jouradm_tmp"
#define XDF_NSUP_JOURADM_IMP			"nsup_jouradm_imp"
#define XDF_NSUP_JOURADM_TMP_IMP		"nsup_jouradm_tmp_imp"
#define XDF_NSUP_VERSLOG			"nsup_verslog"
#define XDF_TMP_IMPRESSION			"tmp_impression"

#define XDF_NSUP_ACTMESG			"nsup_liste_des_messages"
#define XDF_TXTMSG_FR				"txtmsg_fr"

/* fichier du bon de livraison (CONFIG & SUPERV) */
#define XDF_BON_LIVRAISON			"bon_livraison"
#define XDF_BON_LIVRAISON_TMP			"bon_livraison_tmp"
#define XDF_BON_LIVRAISON_MODELE		"bon_livraison.modele"
#define XDF_TYPEFIC_BL_MODELE			"typefich.modele"

/* fichier de checklist  (CONFIG ) */
#define XDF_CHECKLIST				"checklist_conf.priv"

/* fichier de bilan de configuration CONFIG */
#define XDF_BILAN_CONFIG			"bilan_config"
#define XDF_BILAN_CONFIG_TMP			"bilan_config_tmp"

/* Fichier contenant le catalogue des archivages effectues sur la base his*/
#define XDF_CATALOGUE_ARCHIVE			"catalogue_archive.priv"

/* fichier de bilan de la duplication de Base */

#define XDF_BILAN_SVG_BASE			"bilan_SVG_base_CFG"
#define XDF_BILAN_SVG_BASE_HIST			"bilan_SVG_base_HISTO_SVG"
#define XDF_BILAN_SAUVEG_HEBDO			"bilan_SVG_hebdomadaire"

#define XDF_BILAN_REST_BASE			"bilan_REST_base_CFG_ds_CFT"
#define XDF_BILAN_REST_BASE_HIST		"bilan_REST_base_HIST_ds_SVG"

/* fichier des coef des donnees 6 min glissantes par defaut pour CONFIG */
#define XDF_CONF_COEF_6_MIN_GLISS		"coef_6min_gliss.txt"

/* fichier des coef des donnees classe de Distances Inter Vehiculaire pour CONFIG */
#define XDF_CONF_DIST_INTER_VEHI		"dist_inter_vehi.txt"


/* Non Applix - debut */

/* fichier des seuils trafic pour alerte DAI par defaut  pour CONFIG  */
#define XDF_CONF_SEUIL_TRAFIC_DAI_DEFAUT	"seuils_trafic_dai_defaut.txt"

/* Non Applix - fin */

/* fichier des LT avec ST 40 ports*/
#define XDF_ST40	"st40.txt"

/* fichier du calendrier des contextes pour les seuils de trafic */
#define XDF_CALENDRIER_CONTEXTE		"calendrier_contexte.txt"

/* fichier des seuils trafic pour alerte DAI pour CONFIG  */
#define XDF_CONF_SEUIL_TRAFIC_DAI		"seuils_trafic_dai.txt"

/* fichier de configuration des scenarios NAV par defaut pour CONFIG  */
#define XDF_CONF_SCENARIO_NAV_DEFAUT		"scenario_nav_defaut.txt"

/* fichier de configuration des scenarios NAV pour CONFIG  */
#define XDF_CONF_SCENARIO_NAV		"scenario_nav.txt"

/* fichier de configuration des equations d alertes par defaut pour CONFIG */
#define XDF_CONF_EQU_ALERTE_DEFAUT		"equation_alerte_defaut.txt"

/* fichier de configuration des equations d alertes  */
#define XDF_CONF_EQU_ALERTE		"equation_alerte.txt"

/* fichier de configuration autoroute pour CONFIG */
#define XDF_CONF_AUTOROUTE			"autoroute.txt"

/* fichier de configuration portion pour CONFIG */
#define XDF_CONF_PORTION			"portion.txt"

/* fichier de configuration district pour CONFIG */
#define XDF_CONF_DISTRICT			"district.txt"

/* fichier de configuration noeud pour CONFIG */
#define XDF_CONF_NOEUD				"noeud.txt"

/* fichier de configuration segment pour CONFIG */
#define XDF_CONF_SEGMENT		"segment.txt"

/* fichier de configuration pente pour CONFIG */
#define XDF_CONF_PENTE		"pente.txt"

/* fichier de configuration echangeur pour CONFIG */
#define XDF_CONF_ECHANGEUR		"echangeur.txt"

/* fichier de configuration aire pour CONFIG */
#define XDF_CONF_AIRE			"aire.txt"

/* fichier de configuration viaduc pour CONFIG (LOT23) */
#define XDF_CONF_VIADUC			"viaduc.txt"

/* fichier de configuration peage pour CONFIG */
#define XDF_CONF_PEAGE			"peage.txt"

/* fichier de configuration Passage Superieur pour CONFIG */
#define XDF_CONF_PS			"ps.txt"

/* fichier de configuration reseau pour CONFIG */
#define XDF_CONF_RESEAU			"reseau.txt"

/* fichier de configuration ville pour CONFIG */
#define XDF_CONF_VILLE			"ville.txt"

/* fichier de configuration panneau de police pour CONFIG */
#define XDF_CONF_PAN_POLICE		"pan_police.txt"

/* fichier de configuration des interruptions de terre-plein central pour CONFIG */
#define XDF_CONF_ITPC		"itpc.txt"

/* fichier de configuration des portails pour CONFIG */
#define XDF_CONF_PORTAIL		"portail.txt"

/* fichier de configuration des machines pour CONFIG */
#define XDF_CONF_MACHINE		"machine.txt"

/* fichier de configuration des suppleants pour CONFIG */
#define XDF_CONF_SUPPLEANT		"suppleant.txt"

/* fichier de configuration des serveurs de terminaux pour CONFIG */
#define XDF_CONF_SERV_TERM		"serveur_terminaux.txt"
#define XDF_CONF_SERV_TERM		"serveur_terminaux.txt"

/* fichier de configuration des types de PMV pour CONFIG */
#define XDF_CONF_TYPE_PMV		"type_PMV.txt"

#define XDF_CONF_TYPE_PMV		"type_PMV.txt"

/* fichier de configuration des types de pictos pour CONFIG */
#define XDF_CONF_TYPE_PICTO		"type_picto.txt"

/* fichier de configuration des scenarios pour CONFIG */
#define XDF_CONF_SCENARIO		"scenario_NAV.txt"

/* fichier de configuration des stations RADT pour CONFIG */
#define XDF_CONF_STATION_RADT		"radt.txt"

/* fichier de configuration des stations DIT */
#define XDF_CONF_STATION_DIT		"stations_DIT.txt"

/* fichier de configuration des pictos PMV pour CONFIG */
#define XDF_CONF_PMV_PICTO		"PMV_picto.txt"

/* fichier de configuration des zones d'influence PMV pour CONFIG*/
#define XDF_CONF_ZON_PMV		"PMV_zones.txt"
#define XDF_CONF_ZON_PMV_SAGA		"PMV_zones_saga.txt"

/* fichier de configuration des zones d'influence TUNNEL pour CONFIG*/
#define XDF_CONF_ZON_TUN                "TUN_zones.txt"

/* fichier de configuration des Mini GTC pour CONFIG */
#define XDF_CONF_MINI_GTC		"mini_gtc.txt"

/* fichier de configuration des DAI pour CONFIG */
#define XDF_CONF_DAI		"dai.txt"

/* fichier de configuration des systemes video pour CONFIG */
#define XDF_CONF_SYS_VIDEO		"syst_video.txt"

/* fichier de configuration des MAA pour CONFIG */
#define XDF_CONF_MAA			"maa.txt"

/* fichier de configuration des PIRAU pour CONFIG */
#define XDF_CONF_PIRAU			"pirau.txt"

/* fichier de configuration des types NAV pour CONFIG */
#define XDF_CONF_TYPE_NAV		"type_NAV.txt"

/* fichier de configuration des PAL pour CONFIG */
#define XDF_CONF_PAL			"PAL.txt"

/* fichier de configuration de GTC de Nice pour CONFIG */
#define XDF_CONF_GTC_NICE		"gtc_nice.txt"

/* fichier de configuration de sequence tube pour CONFIG */
#define XDF_CONF_SEQ_TUBE		"seq_tube.txt"

/* fichier de configuration de sequence echangeur pour CONFIG */
#define XDF_CONF_SEQ_ECHANGEUR		"seq_echangeur.txt"

/* fichier de configuration de chaussee pour CONFIG */
#define XDF_CONF_CHAUSSEE		"chaussee.txt"

/* fichier de configuration des analyseurs pour CONFIG */
#define XDF_CONF_ANALYSEUR		"analyseur.txt"

/* fichier de configuration des LTVideos pour CONFIG */
#define XDF_CONF_LTVIDEO		"ltvideo.txt"

/* fichier de configuration des Moniteurs pour CONFIG */
#define XDF_CONF_MONITEUR		"moniteur.txt"

/* fichier de configuration des magnetoscopes pour CONFIG */
#define XDF_CONF_MAGNETOSCOPE		"magnetoscope.txt"

/* fichier de configuration des pau pour CONFIG */
#define XDF_CONF_PAU		"pau.txt"

/* fichier de configuration des scenari connus pour CONFIG */
#define XDF_CONF_SCENARIO_CONNU	"scenario_connu.txt"

/* fichier de configuration des demi-echangeurs pour CONFIG */
#define XDF_CONF_DEMIECHANGEUR	"demi_echangeur.txt"

/* fichier de configuration des tubes  pour CONFIG */
#define XDF_CONF_TUBE		"tube.txt"

#define XDF_CONF_FMC_MULTI_AUTO	"multi_fmc.txt"

/* fichier de configuration des batiments */
#define XDF_CONF_BATIMENT				"batiment.txt"

/* AAZ */
#define XDF_CONF_OUV                    "ouvrage.txt"
#define XDF_CONF_SIG                    "signalisation.txt"
#define XDF_CONF_ECL                    "eclairage.txt"
#define XDF_CONF_VEN                    "ventilation.txt"
#define XDF_CONF_ENR                    "energie.txt"
#define XDF_CONF_INC                    "protect_incendie.txt"
#define XDF_CONF_EXP                    "exploitation.txt"
#define XDF_CONF_SYS                    "systeme.txt"
#define XDF_CONF_LTN                    "locaux_techniques.txt"
#define XDF_CONF_SEQ_EXP                "seq_exp.txt"
#define XDF_CONF_SEQ_SIG                "seq_sig.txt"
#define XDF_CONF_SEQ_ECL                "seq_ecl.txt"
#define XDF_CONF_SEQ_VEN                "seq_ven.txt"
#define XDF_CONF_SEQ_ENR                "seq_enr.txt"
#define XDF_CONF_SEQ_INC                "seq_inc.txt"
#define XDF_CONF_SEQ_AUT_EXP            "seq_aut_exp.txt"
#define XDF_CONF_SEQ_AUT_SIG            "seq_aut_sig.txt"
#define XDF_CONF_SEQ_RET_SIG            "sqr_sig.txt"
#define XDF_CONF_SEQ_AUT_ECL            "seq_aut_ecl.txt"
#define XDF_CONF_SEQ_AUT_VEN            "seq_aut_ven.txt"
#define XDF_CONF_SEQ_AUT_ENR            "seq_aut_enr.txt"
#define XDF_CONF_SEQ_AUT_INC            "seq_aut_inc.txt"
#define XDF_CONF_EQT_VEN                "ventilation_equipements.txt"
#define XDF_CONF_EQT_ECL                "eclairage_equipements.txt"
#define XDF_CONF_EQT_SIG                "signalisation_equipements.txt"
#define XDF_CONF_EQT_ENR                "energie_equipements.txt"
#define XDF_CONF_EQT_EXP                "exploitation_equipements.txt"
#define XDF_CONF_EQT_INC                "incendie_equipements.txt"
#define XDF_CONF_EQT_SYS                "systeme_equipements.txt"
#define XDF_CONF_EQT_LTN                "locaux_techniques_equipements.txt"
#define XDF_Alarme_GTCNiceRenovee       "Config_Alarme_GTCNiceRenovee"
#define XDF_Alerte_GTCNiceRenovee       "Config_Alerte_GTCNiceRenovee"
#define XDF_Equipement_GTCNiceRenovee   "Equipement_GTCNiceRenovee"
#define XDF_Config_GTCNiceRenovee       "Config_GTCNiceRenovee"
#define XDF_Ouvrage_GTCNiceRenovee      "Ouvrage_GTCNiceRenovee"
#define XDF_CONF_GTC_NICE_RENOVEE       "gtc_nice_renovee.txt"
#define XDF_Sequence_GTCNiceRenovee		"Config_Seq_GTCNiceRenovee"	

#define XDF_CONF_OUV_LIT                "ouvrage_lit.txt"
#define XDF_CONF_OUV_LTN                "ouvrage_locaux.txt"
#define XDF_CONF_OUV_SYS                "ouvrage_systeme.txt"
#define XDF_CONF_OUV_EHT                "ouvrage_eht.txt"

/* AAZ */

/* fichier de configuration des pts de mesures  pour CONFIG */
#define XDF_CONF_PT_MESURE	"pt_mesure.txt"

/* fichier de configuration des cameras  pour CONFIG */
#define XDF_CONF_CAMERA		"camera.txt"
#define XDF_CONF_ZON_CAM	"camera_zones.txt"

/* fichier de configuration des NAV  pour CONFIG */
#define XDF_CONF_NAV		"NAV.txt"

/* fichier de configuration des sequences autoroutieres de type tube  pour CONFIG */
#define XDF_CONF_SEQ_AUT_TUBE		"seq_aut_tube.txt"

/* fichier de configuration des sequences autoroutieres de type echangeur  pour CONFIG */
#define XDF_CONF_SEQ_AUT_ECHANGEUR	"seq_aut_echangeur.txt"

/* fichier de configuration des capteurs tunnel  pour CONFIG */
#define XDF_CONF_CAPT_TUNNEL	"capteur_tunnel.txt"

/* fichier de configuration des FAV  pour CONFIG */
#define XDF_CONF_FAV		"fav.txt"

/* fichier de configuration des Pn GTC  pour CONFIG */
#define XDF_CONF_PN_GTC		"panneau_GTC.txt"
#define XDF_CONF_PN_GTC		"panneau_GTC.txt"


/* fichier de configuration de influence station pour CONFIG */
#define XDF_CONF_INFLU_STATION	"influence_station.txt"

/* fichier de configuration des troncons pour CONFIG */
#define XDF_CONF_TRONCON		"troncon.txt"

/* fichier de configuration des types d astreintes */
#define XDF_TYPE_ASTREINTE	"astreinte.txt"

/* fichier de configuration des fax */
#define XDF_CONF_FAX	"fax.txt"

/* fichier de configuration des alertes */
#define XDF_CONF_ALERTE	"alerte.txt"

/* fichier de configuration des alarmes */
#define XDF_CONF_ALARME	"alarme.txt"

/* fichier de configuration des equipements generiques LCR */
#define XDF_CONF_EQT_GEN_LCR	"eqt_generique_lcr.txt"

/* fichier de configuration des equipements UGTP (Temps De Parcours) */
#define XDF_CONF_EQT_TDP	"UGTP_tps_parcours.txt"

/* fichier de configuration des equipements ZDT (Zone De Parcours) */
#define XDF_CONF_EQT_ZDP	"zone_tps_parcours.txt"

/* fichier de configuration des destination pour temps de parcours */
#define XDF_CONF_DTP	"destination_tps_parcours.txt"

#define XDF_CONF_PMVA	"PMVA.txt"

#define XDF_CONF_PRV	"PRV.txt"

#define XDF_CONF_PIC	"picto.txt"

#define XDF_CONF_BAF	"baf.txt"

#define XDF_CONF_BAD	"bad.txt"

#define XDF_CONF_ZON_BAD	"BAD_zones.txt"
#define XDF_CONF_ZON_BAF	"BAF_zones.txt"

#define XDF_CONF_DTP_PMV		"destination_pmv.txt"
#define XDF_CONF_DTP_PMA		"destination_pma.txt"

#define XDF_CONF_CFE	"cfe.txt"
#define XDC_CONF_PLAN_FEUX	"plan_feux.txt"

#define XDF_CONF_CAP	"cap.txt"

/* fichier de configuration des equipements IMU (Insertion de Message d'Urgence) */
#define XDF_CONF_IMU		"imu.txt"
#define XDF_CONF_IMU_ZON	"imu_zones.txt"
#define XDF_CONF_IMU_MSG	"imu_messages.txt"

/* fichie de configuration des niches et issues de secour pour localisation tunnel */
#define XDF_CONF_NIC_IS	"niche_et_issue_de_secour.txt"

/*
 * Nom de fichier de configuration du ssyt EQUEXT 
 */


/* Non Applix - debut */

/* commentaire ds. les fichiers de config. type XDF_Config_Lignes */
#define	XDF_COMMENTAIRE		'#'
#define	XDF_DOLLAR		'$'

/* Non Applix - fin */


/* Nom fichier Alarme */
#define	 XDF_Alarme			"Alarmes"

/* Nom fichier des texte d'Alerte */
#define	 XDF_Texte_Alerte		"Texte_Alerte"

/* Nom fichier des parametres status equipement */
#define	 XDF_Param_Status		"Param_Status"

/* debut nom fichier config. taches utiliusant le Serveur de Terminaux */
#define XDF_Config_Lignes		"Config_Lignes"	

/* constante presence equipement */
#define	XDF_Presence_Equip		"Presence_Equip"
#define	XDF_Machine_Sup			"Machine_Sup"
#define	XDF_ParamLS			"Parametres_LS"

#define XDF_Config_RAU			"Config_RAU"
#define XDF_Config_PAU			"Config_PAU"
#define XDF_Config_EQT_GEN_LCR		"Config_EQTX"
#define XDF_Config_TDP			"Config_UGTP"
#define XDF_Config_AUTOTEL		"Config_AUTOTEL"
#define XDF_Config_Cstes_AUTOTEL	"Config_Cstes_AUTOTEL"	
#define XDF_Config_OPER			"Config_OPER"
#define XDF_Config_VIDEO		"Config_VIDEO"
#define XDF_Config_SYSVIDEO		"Config_SYSVIDEO"
#define XDF_Config_GTCNice		"Config_GTCNice"	
#define XDF_Sequence_GTCNice		"Config_Seq_GTCNice"	
#define	XDF_Capt_Pan_GTCNice		"Config_CptPn_GTCNice"

#define XDF_Config_IMU			"Config_IMU"	
#define XDF_Config_SATIR		"Config_SATIR"	
#define XDF_Config_RADT			"Config_RADT"
#define XDF_Config_Point_Mesure		"Config_Point_Mesure"	
#define XDF_Config_Chaussee		"Config_Chaussee"	
#define XDF_Config_RDT			"Config_RDT"
#define XDF_Config_DAI			"Config_DAI"
#define XDF_Config_Analyseur		"Config_Analyseur"	
#define XDF_Config_Chaussee_DAI		"Config_Chaussee_DAI"	
#define XDF_Config_PMV			"Config_PMV"
#define XDF_Config_Type_PMV		"Config_Type_PMV"	
#define XDF_Config_PICTO		"Config_PICTO"	
#define XDF_Config_Type_PICTO		"Config_Type_PICTO"	
#define XDF_Config_NAV			"Config_NAV"
#define XDF_Config_PAL			"Config_PAL"
#define XDF_Config_Type_NAV		"Config_Type_NAV"	
#define XDF_Config_Scenar       	"ConfScenar"	
#define XDF_Config_GTC			"Config_GTC"
#define XDF_Config_GTC_ETOR		"Config_GTC_ETOR"
#define XDF_Config_Fax			"Config_Fax"
#define XDF_Dem_Indiv			"Fichier_Dem_Indiv"
#define XDF_ScenNAV_Connu    		"ScenNAV_Connu"
#define XDF_Nom_Serveur    		"Nom_Serveur"
#define XDF_Config_Params_TERDT	"Config_Params_TERDT"

#define	XDF_Config_Cligno		"ConfCligno"
#define	XDF_Config_IDPASSW		"ConfIdPassw"
#define	XDF_Config_Utilisateurs		"Login"
#define	XDF_Affichage_Def_PMV		"Affichage_Def_PMV"
#define XDF_CONF_RADT_VIRTUELLE		"Conf_RADT_Virtuelles"
#define XDF_CONF_PARAMS_TERDT		"Conf_PARAMS_TERDT"
#define XDF_CONF_ALERTE_PAU_HS		"Conf_Alerte_PAU_HS"
#define XDF_CONF_SYST_RAU		"Conf_TimeOut_Baculement_RAU"
#define XDF_CONF_ALARME_ALERTE		"Conf_Alarme_Alerte"
#define XDF_CONF_ALERTE_DAI_TUNNEL	"Conf_Alerte_DAI_Tunnel"
#define XDF_CONF_ZONE_UGTP		"Conf_Zone_UGTP"
#define XDF_PARAM_UGTP			"Param_tdp.equ"
#define XDF_ASTREINTES_BDC		"Conf_Astreintes_BDC"

#define XDF_CONF_PERIPH_SYSTEME		"Config_Devices.txt"


/* PMVA BAF BAD*/
#define XDF_CONF_ZON_PMVA	"PMVA_zones.txt"
#define XDF_CONF_ZON_PMVA_SAGA	"PMVA_zones_saga.txt"
#define XDF_CONF_ZON_REG	"zones_regul.txt"
#define XDF_CONF_ZON_URB        "zones_urbaines.txt"
#define XDF_Config_PMVA	"Config_PMVA"
#define XDF_Config_BAF		"Config_BAF"
#define XDF_Config_BAD		"Config_BAD"
#define XDF_Config_PRV		"Config_PRV"
#define XDF_Config_PIC		"Config_PIC"
#define XDF_Affichage_Def_PMVA	"Affichage_Def_PMVA"
#define XDF_Affichage_Def_PRV	"Affichage_Def_PRV"
#define XDF_Affichage_Def_PIC	"Affichage_Def_PIC"

#define XDF_CONF_EQT_SAGA	"eqt_saga.txt"

#define XDF_Config_CFE		"Config_CFE"
#define XDF_Config_CAP		"Config_CAP"


/* definitions de types exportes */


/* definition de macro exportees */

/* Non Applix - debut */

#define XDF_Equation_Evt	{ \
				"Alerte_Accident.txt",\
				"Alerte_Delestage.txt",\
				"Alerte_Deviation.txt",\
				"Alerte_Operateur.txt",\
				"Alerte_Meteo.txt",\
				"Alerte_Contresens.txt",\
				"Alerte_Veh_Arrete.txt",\
				"Alerte_Veh_Panne.txt",\
				"Alerte_Veh_Feu.txt",\
				"Alerte_Cirque.txt",\
				"Alerte_Convoi.txt",\
				"Alerte_Militaire.txt",\
				"Alerte_Veh_Lent.txt",\
				"Alerte_Manifestation.txt",\
				"Alerte_Greve.txt",\
				"Alerte_Escargot.txt",\
				"Alerte_Basculement.txt",\
				"Alerte_Travaux.txt",\
				"Alerte_Ss_Concess.txt",\
				"Alerte_Queue_Bouchon.txt",\
				"Alerte_Echangeur.txt",\
				"Alerte_Eboulement.txt",\
				"Alerte_Incendie_Vegetation.txt",\
				"Alerte_Inondation.txt",\
				"Alerte_Obstacle.txt",\
				"Alerte_Chaussee_Deterioree.txt",\
				"Alerte_Huile.txt",\
				"Alerte_Animal.txt",\
				"Alerte_Frontiere.txt",\
				"Alerte_Altercation.txt",\
				"Alerte_Agression.txt",\
				"Alerte_Malaise.txt",\
				"Alerte_Hors_Reseau.txt",\
				"Alerte_Aire.txt",\
				"Alerte_Salage.txt",\
				"Alerte_Hold_Up.txt",\
				"Alerte_Tete_Bouchon.txt",\
				"Alerte_Pieton.txt",\
				"Alerte_Autre.txt",\
				}
#define XDF_Nb_Equation_Evt	39

#define XDF_EncodeNSUP( p, p1 , p2 ) \
 sprintf((p),"%s_%s",(p1),(p2))

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int	XDF01_ValeurNom ( const char *, const char *, char *, char * );

#endif

/* Non Applix - fin */
