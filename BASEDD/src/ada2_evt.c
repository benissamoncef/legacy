/*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : ada2_evt.c				*
*************************************************
* DESCRIPTION :
* module externe de la tache passerelle generique
* 
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
*************************************************
* HISTORIQUE :
*
* ABE	31/03/20	Creation DEM SAE130	V1.1
* ABE	16/07/2020	Ajout patrouilleur DEM SAE149 V1.2
* ABE	16/07/2020	modification recette pour patrouilgleur DEM SAE149 V1.3
* ABE	16/07/2020	modification recette pour patrouilleur DEM SAE149 V1.4
* ABE	20/07/2020	correction malloc radio DEM SAE149 V1.5
* ABE	22/07/2020	Suppression des patrouillieurs de tada2 vers tapat DEM-SAE149 V1.6
* ABE	07/09/2020	Ajout Traitement TDP DEM-SAE130 V1.7
* ABE	07/09/2020	Push pre-prod DEM-SAE130 V1.8
* ABE	07/09/2020	Push pre-prod DEM-SAE130 V1.9
* ABE	14/09/2020	Push pre-prod DEM-SAE130 V1.10
* ABE	16/09/2020	Ajout delais une minute entre reception message demande publication coup par coup et getlist mise à jour par tagen pour avoir les modifications DEM-SAE130 V1.11
* ABE	17/09/2020	Ajout test impact lane au test bretelle + fix compteur feedtype overlap la balise suivante DEM-SAE130 V1.15
* ABE   22/09/2020	Exclusion des fmc type basculement, basculement pour travaux et bouchon de la fmc-virtuelle coupure autoroute DEM-SAE130 V1.18
* ABE   28/09/2020	Ajout des Sens Entre/Sortie dans la condition pour déterminer le sens de la balise rpip/rpis DEM-SAE130 V1.19
* ABE   28/09/2020	Exclusion des FMC type autre & fix balise_test_accident_type V1.20
* ABE	30/09/2020	Suppression du timer 1 min sur publication coup par coup DEM-SAE130 V1.21
* ABE	30/09/2020	Fix castage float pour departement DEM-SAE130 V1.22
* ABE	05/10/2020	Si FMC père = FMC_AUTRE, on déshérite ses fils DEM-SAE130 1.23
* ABE	09/10/2020	Correction si PR_Fin==PR_Debut FMC lineaire DEM-SAE130 1.24
* ABE	12/10/2020	Correction boucle infini sur gen_recherche_pere en cpc DEM-SAE130 1.26
* ABE	12/10/2020	Correction pb régression sur lien FMC fils-père DEM-SAE130 1.27
* ABE	15/10/2020	TDP ajout colonne version xzae196 DEM-SAE130 + mise a jour FMC cpc hors mémoire partagée 1.28
* ABE	23/10/2020	Suppression tagen pour traitement coup par coup DEM-SAE130 1.29
* ABE	29/10/2020	Correctif sur les PR dans cpc_mise_a_jour DEM-SAE130 1.30
* PNI	03/10/2020	Traitement du cas XZAEC_CH_GLIS_Autre+ code "D" pour evt dans les 2 sens  DEM-SAE130 1.30
* ABE	17/11/2020	Ajout libération sempahore pour fonction situation recap DEM-SAE130 1.31
* ABE	30/11/2020	Ajout gestion retypage DEM-SAE130 1.32
* ABE	04/12/2020	Forcage du numéro d'ordre des élements dans un regorupement  -1 DEM-SAE130 1.33
* ABE	07/12/2020	Revert de la version 1.30, ajout de la clé dans l'id de la situationRecord DEM-SAE130 1.35
* ABE	10/12/2020	Changement id fournisseur pour trafic DEM-SAE130 1.36
* ABE	08/01/2021	Changement version ZZ pour VNM
			Ajout fonction pour changer type datex de chausse glissante en fonction de la nature DEM-SAE130 1.37
			Filtrage des FMC type 0 retour de xzae202
* ABE	26/01/2021	Correction desheritage fmc coup par coup DEM-SAE130 1.39
* ABE	12/02/2021	Ajout publication coup par coup fin pour FMC coupure SEM-SAE 1.40
* ABE	07/05/2021	Ajout date prévue début et prévu fin 1.41
* ABE	08/05/2021	Ajout tempo au démarrage pour attendre Tagen, ajout vérification sortie XZAE195 1.42
* ABE	28/05/2021	Correction balise related situation si Chantier & evt.num != consequence.num. valeur other pour les XZAEC_FMC_TraitChaussees DEM-SAE296 DEM-SAE297 1.43
* PNI   26/07/2022  déplacement de l'annulation du timer keepalive avant sa programmation SAE-400 & CO_FTP_SCRIPT_RADT_AFFIXE en arrière plan SAE-401
* ZCH   28/03/2023  la saisie des voies impactées sur une FMC opération escargot ne génère pas de coupure d’autoroute SAE_467
* GGY   27/02/2024  Correction plantage tada2 (DE-591)
************************************************/

#include "ada2_evt.h"
#include "xzao000.h"												

/* definition des constantes */
#define C_DATEX2_FMC_NB_TYPE	XZAEC_FMC_NB_TYP + C_DATEX2_FMC_NB_AJOUT
#define C_DATEX2_FMC_NB_AJOUT 5
#define C_FMC_VITESSE		XZAEC_FMC_NB_TYP+1
#define C_FMC_COUPURE_AUTOROUTE	XZAEC_FMC_NB_TYP+2
#define C_TDP_DEBIT		XZAEC_FMC_NB_TYP+3
#define C_TDP_TAUX_OCCUPATION	XZAEC_FMC_NB_TYP+4
#define C_TDP_VITESSE		XZAEC_FMC_NB_TYP+5
#define C_NB_TDP	3


#define C_DATEX2_CONFIG_TEMPLATE_NB_AJOUT 4
#define C_GEN_HEADER_EVT_PERE	C_DATEX2_FMC_NB_TYPE+1
#define C_GEN_HEADER_TDP_GLOBAL	C_DATEX2_FMC_NB_TYPE+2
#define C_GEN_HEADER_EVT_GLOBAL	C_DATEX2_FMC_NB_TYPE+3

#define CO_NB_JOUR_PURGE        3
#define CO_DELAI_PURGE          86400 * CO_NB_JOUR_PURGE
#define CO_NB_CHIFFRE           13

#define MY_ENCODING		"ISO-8859-1"

#define	CO_MAX_CARACTERE	80
#define CO_FIC_DESTINATAIRE     XDC_PATH_ADA "/destinataire.txt"
#define CO_FIC_FREQUENCE        XDC_PATH_ADA "/frequence.txt"
#define CO_REP_EVT		"evts"
#define CO_REP_RADT		"trafic"
#define CO_FTP_SCRIPT_EVTS_AFFIXE	"_ftp_put_evts"
#define CO_FTP_SCRIPT_RADT_AFFIXE	"_ftp_put_trafic &"
#define	C_FREQ_RECAP_EVT	60
#define	C_FREQ_KEEP_ALIVE	600
#define CO_CPT_NOM_FICHIER     XDC_PATH_ADA "/compteur_nom_fichier.txt"
#define CO_DEPARTEMENT			XDC_PATH_ADA "/departement.txt"
#define PATH_MAX_STRING_SIZE 256
#define TAILLE 200
#define C_FREQ_PURGE		86400 * 3
#define C_TAB_SEPAR		"\t"
#define C_CODE_ESCOTA		"ESCOTA"
#define C_DATEX_ESCOTA		"EVE_ESCOTA"
#define C_DATEX_TRAFIC_ESCOTA	"TraficEscota"
#define C_PUBLICATION_RECAPITULATIVE	"snapshot"
#define C_PUBLICATION_COUPPARCOUP	"allElementUpdate"
#define C_PUBLICATION_KEEP_ALIVE	"KeepAlive"
#define C_PUBLICATION_RECAPITULATIVE_FILE_NAME "content.temp"		
#define C_GROUPE_OF_LOCATION_LINEAR	"Linear"
#define C_GROUPE_OF_LOCATION_POINT	"Point"

#define C_PATH 			XDC_PATH_ADA	"/datexII"
#define C_PATH_XML		XDC_PATH_ADA	"/xml"
#define C_PATH_PREPROD	XDC_PATH_ADA	"/datexIIp"
#define C_KEEP_ALIVE_FILE_NAME "KeepAlive.temp"

#define C_MODULE	0
#define C_ATTRIBUT_GEN	1
#define C_ATTRIBUT_SPEC	2

#define C_XML_GEN_PERE		XDC_PATH_ADA	"/xml/balisesGEN_PERE.txt"
#define C_XML_GEN_HEADER	XDC_PATH_ADA	"/xml/balisesGEN_HEADER.txt"
#define C_XML_GEN_EVT_GLOBAL	XDC_PATH_ADA	"/xml/balisesGEN_GLOBAL.txt"
#define C_XML_GEN		XDC_PATH_ADA	"/xml/balisesGEN.txt"
#define C_XML_ACCIDENT		XDC_PATH_ADA	"/xml/balisesACCIDENT.txt"
#define C_XML_AGRESSION		XDC_PATH_ADA	"/xml/balisesAGRESSION.txt"
#define C_XML_AIRE_FERMEE	XDC_PATH_ADA	"/xml/balisesAIRE_FERMEE.txt"
#define C_XML_ALTERC_USAGERS	XDC_PATH_ADA	"/xml/balisesALTERC_USAGERS.txt"
#define C_XML_ANIMAL		XDC_PATH_ADA	"/xml/balisesANIMAL.txt"
#define C_XML_AUTRE		XDC_PATH_ADA	"/xml/balisesAUTRE.txt"
#define C_XML_BASCUL		XDC_PATH_ADA	"/xml/balisesBASCUL.txt"
#define C_XML_BASCUL_TRV	XDC_PATH_ADA	"/xml/balisesBASCUL_TRV.txt"
#define C_XML_BOUCHON		XDC_PATH_ADA	"/xml/balisesBOUCHON.txt"
#define C_XML_CHAUSSEE_DET	XDC_PATH_ADA	"/xml/balisesCHAUSSEE_DET.txt"
#define C_XML_CHAUSSEE_GLISS	XDC_PATH_ADA	"/xml/balisesCHAUSSEE_GLISS.txt"
#define C_XML_CIRQUE		XDC_PATH_ADA	"/xml/balisesCIRQUE.txt"
#define C_XML_CONTRESENS	XDC_PATH_ADA	"/xml/balisesCONTRESENS.txt"
#define C_XML_CONVOI_EX		XDC_PATH_ADA	"/xml/balisesCONVOI_EX.txt"
#define C_XML_CONVOI_MIL	XDC_PATH_ADA	"/xml/balisesCONVOI_MIL.txt"
#define C_XML_DELESTAGE		XDC_PATH_ADA	"/xml/balisesDELESTAGE.txt"
#define C_XML_DEVIATION		XDC_PATH_ADA	"/xml/balisesDEVIATION.txt"
#define C_XML_EBOUL		XDC_PATH_ADA	"/xml/balisesEBOUL.txt"
#define C_XML_ECH_DEC		XDC_PATH_ADA	"/xml/balisesECH_DEC.txt"
#define C_XML_FERM_ECH		XDC_PATH_ADA	"/xml/balisesFERM_ECH.txt"
#define C_XML_FLAQUE_HUILE	XDC_PATH_ADA	"/xml/balisesFLAQUE_HUILE.txt"
#define C_XML_FRONTIERE		XDC_PATH_ADA	"/xml/balisesFRONTIERE.txt"
#define C_XML_GREVE		XDC_PATH_ADA	"/xml/balisesGREVE.txt"
#define C_XML_HOLDUP		XDC_PATH_ADA	"/xml/balisesHOLDUP.txt"
#define C_XML_INCENDIE		XDC_PATH_ADA	"/xml/balisesINCENDIE.txt"
#define C_XML_SOUS_CONC		XDC_PATH_ADA	"/xml/balisesSOUS_CONC.txt"
#define C_XML_INONDATION	XDC_PATH_ADA	"/xml/balisesINONDATION.txt"
#define C_XML_MALAISE		XDC_PATH_ADA	"/xml/balisesMALAISE.txt"
#define C_XML_MANIF		XDC_PATH_ADA	"/xml/balisesMANIF.txt"
#define C_XML_METEO		XDC_PATH_ADA	"/xml/balisesMETEO.txt"
#define C_XML_OBJET		XDC_PATH_ADA	"/xml/balisesOBJET.txt"
#define C_XML_ESCARGOT		XDC_PATH_ADA	"/xml/balisesESCARGOT.txt"
#define C_XML_PIETONS		XDC_PATH_ADA	"/xml/balisesPIETONS.txt"
#define C_XML_SALAGE		XDC_PATH_ADA	"/xml/balisesSALAGE.txt"
#define C_XML_TRAIT_CHAUSS	XDC_PATH_ADA	"/xml/balisesTRAIT_CHAUSS.txt"
#define C_XML_TRAVAUX		XDC_PATH_ADA	"/xml/balisesTRAVAUX.txt"
#define C_XML_VEH_ARRETE	XDC_PATH_ADA	"/xml/balisesVEH_ARRETE.txt"
#define C_XML_VEH_FEU		XDC_PATH_ADA	"/xml/balisesVEH_FEU.txt"
#define C_XML_VEH_PANNE		XDC_PATH_ADA	"/xml/balisesVEH_PANNE.txt"
#define C_XML_VEH_LENT		XDC_PATH_ADA	"/xml/balisesVEH_LENT.txt"
#define C_XML_MARCHE_ARRIERE	XDC_PATH_ADA	"/xml/balisesMARCHE_ARRIERE.txt"
#define C_XML_ZONE_STOCKAGE	XDC_PATH_ADA	"/xml/balisesSTOCKAGE.txt"
#define C_XML_VITESSE	XDC_PATH_ADA	"/xml/balisesVITESSE.txt"
#define C_XML_REGUL	XDC_PATH_ADA	"/xml/balisesREGUL.txt"
#define C_XML_COUPURE	XDC_PATH_ADA	"/xml/balisesCOUPURE.txt"
#define C_XML_GEN_HEADER_TDP_QTV	XDC_PATH_ADA	"/xml/balisesGEN_HEADER_TDP_QTV.txt"
#define	C_XML_GEN_TDP_GLOBAL		XDC_PATH_ADA	"/xml/balisesGEN_TDP_GLOBAL.txt"
#define C_XML_TDP_DEBIT			XDC_PATH_ADA	"/xml/balisesTDP_DEBIT.txt"
#define C_XML_TDP_TAUX_OCCUPATION	XDC_PATH_ADA	"/xml/balisesTDP_TAUX_OCCUPATION.txt"
#define C_XML_TDP_VITESSE		XDC_PATH_ADA	"/xml/balisesTDP_VITESSE.txt"

#define C_BALISE_PREFIXE	"fmc."
#define C_BALISE_DATE_COURANTE	"fmc.date_courante"
#define C_BALISE_ID		"fmc.id"
#define C_BALISE_TYPE		"fmc.type"
#define C_BALISE_ID_EVT		"fmc.id_evt"
#define C_BALISE_ZZ		"fmc.zz"
#define C_BALISE_QUALIF		"fmc.qualif"
#define C_BALISE_QIN		"fmc.qin"
#define C_BALISE_DATE_DEBUT	"fmc.datedebut"
#define C_BALISE_DATE_FIN	"fmc.datefin"
#define C_BALISE_DATE_DEBUT_PREVU	"fmc.date_debut_prevu"
#define C_BALISE_DATE_FIN_PREVU	"fmc.date_fin_prevu"
#define C_BALISE_NB_BLOQ	"fmc.nbBloq"
#define C_BALISE_NB_OUV		"fmc.nbOuv"
#define C_BALISE_COMMENT	"fmc.comment"
#define C_BALISE_L01		"fmc.l01"
#define C_BALISE_LTV		"fmc.ltv"
#define C_BALISE_L05		"fmc.l05"
#define C_BALISE_L07		"fmc.l07"
#define C_BALISE_L04		"fmc.l04"
#define C_BALISE_L06		"fmc.l06"
#define C_BALISE_L11		"fmc.l11"
#define C_BALISE_L09		"fmc.l09"
#define C_BALISE_L10		"fmc.l10"
#define C_BALISE_L12		"fmc.l12"
#define C_BALISE_CANCEL		"fmc.cancel"
#define C_BALISE_END		"fmc.end"
#define C_BALISE_L02		"fmc.l02"
#define C_BALISE_NB_TOT		"fmc.nbTot"
#define C_BALISE_CARRIAGE	"fmc.carriage"
#define C_BALISE_LOCATION	"fmc.location"
#define C_BALISE_CAUSE		"fmc.cause"
#define C_BALISE_LANES		"fmc.lanes"
#define C_BALISE_RPIP		"fmc.rpip"
#define C_BALISE_RNP		"fmc.rnp"
#define C_BALISE_DRP		"fmc.drp"
#define C_BALISE_DFPP		"fmc.dfpp"
#define C_BALISE_RPIS		"fmc.rpis"
#define C_BALISE_RNS		"fmc.rns"
#define C_BALISE_DRS		"fmc.drs"
#define C_BALISE_DFPS		"fmc.dfps"

#define C_BALISE_HORODATE_DEBUT_TACHE	"fmc.horodate_debut_tache"
#define C_BALISE_OPERATING_MODE	"fmc.operating_mode"
#define C_BALISE_PUBLICATION_TYPE	"fmc.publication_type"
#define C_BALISE_ID_REGROUPEMENT	"fmc.id_regroupement"
#define C_BALISE_VERSION_REGROUPEMENT	"fmc.version_regroupement"
#define C_BALISE_GRAVITE	"fmc.gravite"
#define C_BALISE_ID_REGROUPEMENT_CAUSE_CHANTIER	"fmc.id_regroupement_cause_chantier"
#define C_BALISE_VERSION_ID_REGROUPEMENT_CAUSE_CHANTIER	"fmc.version_id_regroupement_cause_chantier"
#define C_BALISE_RELATED_SITUATION_TARGETCLASS	"fmc.related_situation_targetClass"
#define C_BALISE_ID_ELEMENT	"fmc.id_element"
#define C_BALISE_IMPACT_VOIE	"fmc.impact_voie"
#define C_BALISE_LONGUEUR_IMPACT	"fmc.longueur_impact"
#define C_BALISE_SENS	"fmc.sens"
#define C_BALISE_TEST_LANE_RESTRICTED	"fmc.test_lane_restricted"
#define C_BALISE_DATE_VALIDATION	"fmc.date_validation"

#define C_BALISE_GROUPE_OF_LOCATION_TYPE "fmc.groupeOfLocationType"	
#define C_BALISE_TEST_GROUPE_OF_LOCATION_LINEAR	"fmc.test_groupeOfLocationType_linear"
#define C_BALISE_TEST_GROUPE_OF_LOCATION_POINT "fmc.test_groupeOfLocationType_point"

#define C_BALISE_TEST_PUBLICATION_TYPE_RECAP "fmc.test_feedType_recap"
#define C_BALISE_TEST_PUBLICATION_TYPE_COUPPARCOUP "fmc.test_feedType_coupparcoup"

#define C_BALISE_TEST_CAUSE	"fmc.presence_cause"

#define C_BALISE_TEST_DATE_FIN_PREVU	"fmc.test_date_fin_prevu"

/*ACCIDENT*/
#define C_BALISE_TOTAL_PEOPLE	"fmc.total_people"
#define C_BALISE_TOTAL_VEH	"fmc.total_veh"
#define C_BALISE_NB_VL		"fmc.nb_vl"
#define C_BALISE_NB_BUS		"fmc.nb_bus"
#define C_BALISE_NB_PL		"fmc.nb_pl"
#define C_BALISE_NB_REM		"fmc.nb_rem"
#define C_BALISE_NB_MOTOS	"fmc.nb_motos"
#define C_BALISE_BLESSES	"fmc.blesses"
#define C_BALISE_MORTS		"fmc.morts"
#define C_BALISE_CHARGEMENT	"fmc.chargement"
#define C_BALISE_MATIERE	"fmc.matiere"
#define C_BALISE_ENFANTS	"fmc.enfants"
#define C_BALISE_VIP		"fmc.vip"
#define C_BALISE_ESCOTA		"fmc.escota"
#define C_BALISE_TEST_BUS	"fmc.test_bus"
#define C_BALISE_TEST_CHARGEMENT	"fmc.test_chargement"
#define C_BALISE_TEST_PL	"fmc.test_pl"
#define C_BALISE_TEST_MOTOS	"fmc.test_motos"
#define C_BALISE_TEST_BLESSES	"fmc.test_blesses"
#define C_BALISE_TEST_VL	"fmc.test_vl"
#define C_BALISE_TEST_REM	"fmc.test_rem"
#define C_BALISE_TEST_MORTS	"fmc.test_morts"
#define C_BALISE_TEST_BRETELLE	"fmc.test_bretelle"
#define C_BALISE_TEST_CHANTIER	"fmc.test_chantier"
#define C_BALISE_TEST_ACCIDENT	"fmc.test_accident"


/*BASCULEMENT*/
#define C_BALISE_BASCUL_LONGUEUR	"fmc.bascul_longueur"
#define C_BALISE_BASC_VITESSE		"fmc.bascul_vitesse"
#define C_BALISE_BASC_VITESSE_OPPOSEE "fmc.bascul_vitesse_opposee"

/*BOUCHON*/
#define C_BALISE_BOUCHON_LONGUEUR	"fmc.longueurBouchon"
#define C_BALISE_BOUCHON_TYPE		"fmc.typeBouchon"
#define C_BALISE_TEST_BOUCHON_ACCORDEON	"fmc.bouchon_accordeon"

/*VEH LENT*/
#define C_BALISE_VEHL_LONGUEUR		"fmc.vehl_longueur"
#define C_BALISE_VEHL_POIDS		"fmc.vehl_poids"
#define C_BALISE_VEHL_LARGEUR		"fmc.vehl_largeur"
#define C_BALISE_VEHL_HAUTEUR		"fmc.vehl_hauteur"
#define C_BALISE_VEHL_TEST_LONGUEUR	"fmc.test_longueur"
#define C_BALISE_VEHL_TEST_HAUTEUR	"fmc.test_hauteur"
#define C_BALISE_VEHL_TEST_POIDS	"fmc.test_poids"
#define C_BALISE_VEHL_TEST_LARGEUR	"fmc.test_largeur"

/*DELESTAGE*/
#define C_BALISE_ITINERAIRE		"fmc.itineraire"

/*FERMETURE ECHANGEUR*/
#define C_BALISE_FERMETURE		"fmc.fermeture"
#define C_BALISE_FERM_ENTREES		"fmc.fermeture_entrees"
#define C_BALISE_FERM_SORTIES		"fmc.fermeture_sorties"

/*ECHANGEUR DECONSEILLE*/
#define C_BALISE_DECON_ENTREES		"fmc.deconseille_entrees"
#define C_BALISE_DECON_SORTIES		"fmc.deconseille_sorties"

/*INCIDENT SOUS CONCESSIONNAIRE*/
#define C_BALISE_PENURIE		"fmc.penurie"

/*METEO*/
#define C_BALISE_VISIBILITE		"fmc.visi"
#define C_BALISE_METEO_TYPE		"fmc.meteo_type"

/*OBSTACLE*/
#define C_BALISE_OBSTACLE_ANIMALE	"fmc.obstacle_animale"
#define C_COMMENTAIRE_ANIMALE	"ANIMAL DOMESTIQUE ECRASE"
#define C_COMMENTAIRE_ANIMALE2	"ANIMAL SAUVAGE ECRASE"

/*TRAVAUX*/
#define C_BALISE_MOBILE			"fmc.mobile"
#define C_BALISE_TRAVAUX_TYPE		"fmc.travaux_type"
#define C_BALISE_VITESSE		"fmc.vitesse"
#define C_BALISE_TRV_VITESSE	"fmc.travaux_vitesse"

/*ZONE DE STOCKAGE*/
#define C_BALISE_STOCKAGE_DEMARRE	"fmc.stockage_demarre"


/*VEHICULE*/
#define C_BALISE_VEH_MATIERE		"fmc.veh_matiere"
#define C_BALISE_VEH_CHARGEMENT		"fmc.veh_chargement"
#define C_BALISE_VEH_TYPE		"fmc.veh_type"

/*CHAUSSEE GLISSANTE*/
#define C_BALISE_GLISSANTE	"fmc.glissante"
#define C_BALISE_GLISSANTE_METEO	"fmc.glissante_meteo"
#define C_BALISE_GLISSANTE_NON_METEO	"fmc.glissante_non_meteo"

/* TDP */
#define C_BALISE_TDP_SIREDO	"fmc.TDP_siredo"
#define C_BALISE_TDP_VERSION_PMS	"fmc.TDP_version_PMS"
#define C_BALISE_TDP_HORODATE	"fmc.TDP_horodate"
#define	C_BALISE_TDP_DEBIT	"fmc.TDP_Q"
#define	C_BALISE_TDP_TAUX_OCCUPATION	"fmc.TDP_T"
#define	C_BALISE_TDP_VITESSE	"fmc.TDP_V"
#define	C_BALISE_TDP_SENS_STATION	"fmc.TDP_sens_station"

static char *	balise_date_courante();
static char *	balise_id();
static char *	balise_type();
static char *	balise_id_evt();
static char *	balise_zz();
static char *	balise_qualif();
static char *	balise_qin();
static char *	balise_date_debut();
static char *	balise_date_fin();
static char *	balise_date_debut_prevu();
static char *	balise_date_fin_prevu();
static char * 	balise_nb_bloq();
static char * 	balise_nb_ouv();
static char *	balise_comment();
static char *	balise_l01();
static char *	balise_l02();
static char *	balise_ltv();
static char *	balise_l05();
static char *	balise_l07();
static char *	balise_l04();
static char *	balise_l06();
static char *	balise_l11();
static char *	balise_l09();
static char *	balise_l10();
static char *	balise_l12();
static char *	balise_cancel();
static char *	balise_end();
static char *	balise_nb_tot();
static char *	balise_carriage();
static char *	balise_location();
static char *	balise_cause();
static char *	balise_lanes();
static char *	balise_rpip();
static char *	balise_rnp();
static char *	balise_drp();
static char *	balise_dfpp();
static char *	balise_rpis();
static char *	balise_rns();
static char *	balise_drs();
static char *	balise_dfps();

static char * balise_test_cause();
static char * balise_test_true();

static char * balise_test_date_fin_prevu();

/*ACCIDENT*/
static char *	balise_nb_vl();
static char * 	balise_nb_bus();
static char * 	balise_nb_rem();
static char *	balise_nb_pl();
static char *	balise_nb_motos();
static char *	balise_blesses();
static char *	balise_morts();
static char *	balise_chargement();
static char *	balise_matiere();
static char *	balise_vip();
static char *	balise_escota();
static char * 	balise_test_bus();
static char * 	balise_test_chargement();
static char * 	balise_test_pl();
static char * 	balise_test_motos();
static char * 	balise_test_blesses();
static char * 	balise_test_vl();
static char * 	balise_test_rem();
static char * 	balise_test_morts();
static char *	balise_total_people();
static char *	balise_total_veh();
static char *   balise_test_accident();

/*BASCULEMENT*/
static char *	balise_bascul_longueur();
static char *	balise_basc_vitesse();
static char *	balise_basc_vitesse_opposee();
static char *	balise_vitesse();

/*BOUCHON*/
static char *	balise_bouchon_type();
static char *	balise_bouchon_longueur();
static char *	balise_test_bouchon_accordeon();

/*VEH LENT*/
static char *	balise_vehl_longueur();
static char *	balise_vehl_hauteur();
static char *	balise_vehl_poids();
static char *	balise_vehl_largeur();
static char *	balise_vehl_test_poids();
static char *	balise_vehl_test_largeur();
static char *	balise_vehl_test_longueur();
static char *	balise_vehl_test_hauteur();

/*DELESTAGE*/
static char *	balise_itineraire();

/*FERMETURE ECHANGEUR*/
static char *	balise_fermeture();
static char *	balise_entrees_fermeture();
static char *	balise_sorties_fermeture();

/*ECHANGEUR DECONSEILLE*/
static char *	balise_entrees_deconseille();
static char *	balise_sorties_deconseille();

/*INCIDENT SOUS CONCESSIONNAIRE*/
static char *	balise_penurie();

/*METEO*/
static char *	balise_visibilite();
static char *	balise_meteo_type();

/*OBSTACLE*/
static char * balise_test_obstacle_animale();

/*TRAVAUX*/
static char *balise_travaux_type() ;
static char *balise_mobile() ;
static char *balise_trv_vitesse();

/*ZONE DE STOCKAGE*/
static char *balise_stockage_demarre() ;

/*VEHICULE*/
static char *balise_veh_matiere();
static char *balise_veh_chargement();
static char *balise_veh_type();

/*CHAUSSEE GLISSANTE*/
static char *balise_glissante();
static char *balise_test_glissante_meteo();
static char *balise_test_glissante_non_meteo();

/*REGULATION*/
static char *balise_regul_vitesse();

/* Spec TIPI */
static char *balise_horodate_debut_tache();
static char *balise_operating_mode();
static char *balise_publication_type();
static char *balise_id_regroupement();
static char *balise_version_regroupement();
static char *balise_gravite();
static char *balise_test_chantier();
static char *balise_id_regroupement_cause_chantier();
static char *balise_version_id_regroupement_cause_chantier();
static char *balise_related_situation_targetClass();
static char *balise_date_validation();
static char *balise_id_element();
static char *balise_impact_voie();
static char *balise_voie_impactee();
static char *balise_longueur_impact();
static char *balise_test_bretelle();
static char *balise_test_lane_restricted();
static char *balise_sens();
static char *balise_test_feedType_recap();
static char *balise_test_feedType_coupparcoup();
static char *balise_groupe_of_location_type();
static char *balise_test_groupe_of_location_linear();
static char *balise_test_groupe_of_location_point();

/* TDP */
static char *balise_TDP_siredo();
static char *balise_TDP_version_PMS();
static char *balise_TDP_horodate();
static char *balise_TDP_debit();
static char *balise_TDP_taux_occupation();
static char *balise_TDP_vitesse();
static char *balise_TDP_sens_station();

void gen_cumul_voies(XDY_Voie, int *, int *);
XDY_Evt gen_recherche_pere(XZLGT_Liste_FMC *, int);
XZLGT_Liste_FMC gen_recherche_fils(XZLGT_Liste_FMC *, int);
XDY_Booleen gen_a_traiter(XZLGT_Liste_FMC *, int);
int desherite_FMC(XZLGT_Liste_FMC *pa_liste_fmc, int va_indice);
int ada2_evt_genere_commentaires(char *va_texte);
int ada_evt_cpc_mise_a_jour(XZLGT_Liste_FMC * pa_fmc, int va_indice);


#define NBR_PREMIER 	53
#define NBR_ELT_TABLE	59

#define C_l01		"F32"
#define C_l02		"LOC"
#define C_l03		4
#define C_l07_P		"Aix en Provence - Frontiere Italienne"
#define C_l07_N		"Frontiere italienne - Aix en Provence"
#define C_l07_U		"Sens non defini"

#define TAILLE_SUR	350

/* definition des variables globales a l'application */

/* definition des variables locales au module */
XZLGT_Liste_FMC		*tg_fmc;
XZLGT_Liste_FMC		*tg_fmc_courant;

xmlChar *ConvertInput(const char *in, const char *encoding);


static char *tm_l07_p[]={
	"",
	"Aix en Provence - Frontiere Italienne",
	"A8 - Monaco",
	"Marseille - Toulon",
	"Marseille - A52",
	"Aix - Les Alpes",
	"Aix - Aubagne",
	"A52 - Auriol",
	"Le Luc - Toulon"};

static char *tm_l07_n[]={
	"",
	"Frontiere Italienne - Aix en Provence",
	"MonacoA8 - Monaco",
	"Toulon - Marseille",
	"A52 - Marseille",
	"Les Alpes - Aix",
	"Aubagne - Aix",
	"Auriol - A52",
	"Toulon - Le Luc"};
	
static XDY_Entier vm_nb_fmc=0;

T_Type_FMC		tg_types_fmc[]=
{
	{	XZAEC_FMC_Accident,			"ACC",	C_XML_ACCIDENT	,	"Accident"},
	{	XZAEC_FMC_Delestage,		"ROU"	, C_XML_DELESTAGE,	"ReroutingManagement"},
	{	XZAEC_FMC_Deviation,		"ROU"	, C_XML_DEVIATION,	"ReroutingManagement"},
	{	XZAEC_FMC_PosteOperateur,	"OPA"	, "",			""},
	{	XZAEC_FMC_Meteo,			"FOG"	, C_XML_METEO,		"PoorEnvironmentConditions"},
	{	XZAEC_FMC_Contresens,		"INC"	, C_XML_CONTRESENS,	"VehicleObstruction"},
	{	XZAEC_FMC_VehArrete,		"INC"	, C_XML_VEH_ARRETE,	"VehicleObstruction"},
	{	XZAEC_FMC_VehPanne,			"INC"	, C_XML_VEH_PANNE,	"VehicleObstruction"},
	{	XZAEC_FMC_VehFeu,			"INC"	, C_XML_VEH_FEU,	"VehicleObstruction"},
	{	XZAEC_FMC_Cirque,			"INC"	, C_XML_CIRQUE,		"VehicleObstruction"},
	{	XZAEC_FMC_ConvoiExc,		"INC"	, C_XML_CONVOI_EX,	"VehicleObstruction"},
	{	XZAEC_FMC_ConvoiMil,		"INC"	, C_XML_CONVOI_MIL,	"VehicleObstruction"},
	{	XZAEC_FMC_VehLent,			"INC"	, C_XML_VEH_LENT,	"VehicleObstruction"},
	{	XZAEC_FMC_Manifestation,	"ACT"	, C_XML_MANIF,		"DisturbanceActivity"},
	{	XZAEC_FMC_GreveESCOTA,		"INC"	, C_XML_GREVE,		"DisturbanceActivity"},
	{	XZAEC_FMC_Escargot,			"INC"	, C_XML_ESCARGOT,	"DisturbanceActivity"},
	{	XZAEC_FMC_Basculement,		"RMT"	, C_XML_BASCUL,		"RoadOrCarriagewayOrLaneManagement"},
	{	XZAEC_FMC_Travaux,			"RMT"	, C_XML_TRAVAUX,	"MaintenanceWorks"},
	{	XZAEC_FMC_IncSsConces,		"INC"	, C_XML_SOUS_CONC,	"RoadsideServiceDisruption"},
	{	XZAEC_FMC_QueueBouchon,		"LOS"	, C_XML_BOUCHON,	"AbnormalTraffic"},
	{	XZAEC_FMC_FermetureEch,		"RES"	, C_XML_FERM_ECH,	"ReroutingManagement"},
	{	XZAEC_FMC_Eboulement,		"INC"	, C_XML_EBOUL,		"EnvironmentalObstruction"},
	{	XZAEC_FMC_Incendie,			"INC"	, C_XML_INCENDIE,	"EnvironmentalObstruction"},
	{	XZAEC_FMC_Inondation,		"INC"	, C_XML_INONDATION,	"EnvironmentalObstruction"},
	{	XZAEC_FMC_Obstacle,			"OHZ"	, C_XML_OBJET,		"GeneralObstruction"},
	{	XZAEC_FMC_ChausseDeter,		"INC"	, C_XML_CHAUSSEE_DET,	"InfrastructureDamageObstruction"},
	{	XZAEC_FMC_FlaqueHuile,		"SHZ"	, C_XML_FLAQUE_HUILE,	"NonWeatherRelatedRoadConditions"},
	{	XZAEC_FMC_AnimalErrant,		"INC"	, C_XML_ANIMAL,		"AnimalPresenceObstruction"},
	{	XZAEC_FMC_FrontiereFermee,	"INC"	, C_XML_FRONTIERE,	"RoadOrCarriagewayOrLaneManagement"},
	{	XZAEC_FMC_AltercationUsager,"INC"	, C_XML_ALTERC_USAGERS,	"DisturbanceActivity"},
	{	XZAEC_FMC_Agression,		"INC"	, C_XML_AGRESSION,	"DisturbanceActivity"},
	{	XZAEC_FMC_MalaiseUsager,	"INC"	, C_XML_MALAISE,	"DisturbanceActivity"},
	{	XZAEC_FMC_EvtHorsReseau,	"INC"	, C_XML_AUTRE,		"GeneralObstruction"},
	{	XZAEC_FMC_AireFermee,		"INC"	, C_XML_AIRE_FERMEE,	"RoadsideServiceDisruption"},
	{	XZAEC_FMC_Salage,			"INC"	, C_XML_SALAGE,		"MaintenanceWorks"},
	{	XZAEC_FMC_HoldUpVol,		"INC"	, C_XML_HOLDUP,		"DisturbanceActivity"},
	{	XZAEC_FMC_TeteBouchon,		"INC"	, "",			""},
	{	XZAEC_FMC_PietonChaussee,	"INC"	, C_XML_PIETONS,	"GeneralObstruction"},
	{	XZAEC_FMC_Autre,			"INC"	, C_XML_AUTRE,		"GeneralObstruction"},
	{	XZAEC_FMC_VehMarcheArr,		"INC"	, C_XML_MARCHE_ARRIERE,	"GeneralObstruction"},
	{	XZAEC_FMC_TraitChaussees,	"INC"	, C_XML_TRAIT_CHAUSS,	"MaintenanceWorks"},
	{	XZAEC_FMC_EchDeconseille,	"RES"	, C_XML_ECH_DEC,	"ReroutingManagement"},
	{	XZAEC_FMC_ChausseeGliss,	"SHZ"	, C_XML_CHAUSSEE_GLISS,	"WeatherRelatedRoadConditions"},
	{	XZAEC_FMC_BasculTravaux,	"RMT"	, C_XML_BASCUL_TRV,	"RoadOrCarriagewayOrLaneManagement"},
	{	XZAEC_FMC_ZoneStockage,		"RES"	, C_XML_ZONE_STOCKAGE,	"RoadOrCarriagewayOrLaneManagement"},
	{  	XZAEC_FMC_Regulation,       "RES"   , C_XML_REGUL,	"SpeedManagement"},
	{	C_FMC_VITESSE,			"RES"	, C_XML_VITESSE,	"SpeedManagement"},
	{	C_FMC_COUPURE_AUTOROUTE,	"RES"	, C_XML_COUPURE,	"RoadOrCarriagewayOrLaneManagement"},
	{	C_TDP_DEBIT,			"RES"	, C_XML_TDP_DEBIT,	"TDP"},
	{	C_TDP_TAUX_OCCUPATION,		"RES"	, C_XML_TDP_TAUX_OCCUPATION,	"TDP"},
	{	C_TDP_VITESSE,			"RES"	, C_XML_TDP_VITESSE,	"TDP"}
};

static char pm_comment[TAILLE_SUR+1]="";
 
static char *version = "@(#)ada2_evt.c	1.0 31/03/20 : commentaire" ;

static char chemin_export[80];		
static int vm_type;

#define C_PIPE	"\t"
#define C_VALEUR_PREDICAT 	1
#define C_VALEUR_DIRECTE	0

#define DATA_TYPE_EVT 1
#define DATA_TYPE_RADT 2
#define DATA_TYPE_PMV  3
#define DATA_TRANSMISSION_TYPE_FTP 0
#define DATA_TRANSMISSION_TYPE_WEBSERVICE 1

/* structure destinataire */
typedef struct {
	char Nom[32];
	int TypeTransmission;
	char URLWebService_PathFTP[255];
	int DataTypeVoulu[3];	
} DESTINATAIRE;
static int vg_nbrDestinataire;
static DESTINATAIRE vg_destinataire[64];					

static char* vg_publication_type;
static char vg_horodate_debut_tache[50]="";

static T_NOEUD *vg_arbre[C_DATEX2_FMC_NB_TYPE+C_DATEX2_CONFIG_TEMPLATE_NB_AJOUT];
static T_LISTE	*vg_peres=NULL;

/* variable globale pour TDP */
static	int vg_TDP_debit=0;
static float vg_TDP_taux_occupation=0.0;
static float vg_TDP_vitesse=0.0;
static XDY_Horodate vg_TDP_horodate;
static char vg_TDP_code_siredo[15];
static char vg_TDP_version_station[15];

static XDY_Sens vg_TDP_sens_station=0;

XZLGT_Liste_FMC	vm_noeud;

int vg_size_struct_numero_regroupement=0;
int vg_frequence_keep_alive;
int vg_frequence_recap;
T_NUMERO_ELEMENT_REGROUPEMENT *vg_numero_element_regroupement=NULL;


/* Fichier : @(#)ada2_evt.c	1.0        Release : 1.0        Date : 31/03/20
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* purge
******************************************************
* SEQUENCE D'APPEL :
* ada2_evt_purge
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

void ada2_evt_purge()
{
  return ;
}

void ada2_xml_traite_route_coupee(XZLGT_Liste_FMC pa_fmc, XDY_Booleen va_coupure_route_version_precedente,XDY_Booleen va_coupure_route_inverse_version_precedente)
{
	int vl_nb_voies=0;
	int vl_nb_voies_cir=0;
	static char vl_str[255]="";
	int route_coupee=0;
	int route_autre_sens_coupee=0;
	int route_inexistante=0;
	XZLGT_Liste_FMC	vl_noeud;

	/* sens 1 */
	gen_cumul_voies( pa_fmc.VL , &vl_nb_voies , &vl_nb_voies_cir);
	gen_cumul_voies( pa_fmc.VM1 , &vl_nb_voies , &vl_nb_voies_cir);
	gen_cumul_voies( pa_fmc.VM2 , &vl_nb_voies , &vl_nb_voies_cir);
	gen_cumul_voies( pa_fmc.VR, &vl_nb_voies , &vl_nb_voies_cir);
	gen_cumul_voies( pa_fmc.BAU, &vl_nb_voies , &vl_nb_voies_cir);


	/*si les voies éxiste et sont toutes bouchées genere le fichier du fils*/
	if (vl_nb_voies_cir==0 && vl_nb_voies!=0)
	{
		vl_noeud=pa_fmc;
		vl_noeud.Type = C_FMC_COUPURE_AUTOROUTE;
		vl_noeud.Evt.cle = 150+(pa_fmc.Evt.cle);
		ada2_xml_EVT_write(vl_noeud,C_FMC_COUPURE_AUTOROUTE,"");
		/*ajout des fichiers fils à chaque pere*/
		sprintf(vl_str,"%s/format.ksh %d %d %d %d %s",		
			C_PATH_XML,
			pa_fmc.Evt.numero,
			vl_noeud.Evt.cle,
			pa_fmc.Evt_Consequence.numero,
			pa_fmc.Evt_Consequence.cle,
			chemin_export);
		system(vl_str);
		XZST_03EcritureTrace(XZSTC_WARNING,"ajout fils route coupee %s",vl_str);
	} 
	else if (va_coupure_route_version_precedente)
	{
		vl_noeud=pa_fmc;
		vl_noeud.Bloc_Optio.Fin = 'Y';
		vl_noeud.Type = C_FMC_COUPURE_AUTOROUTE;
		vl_noeud.Evt.cle = 150+(pa_fmc.Evt.cle);
		ada2_xml_EVT_write(vl_noeud,C_FMC_COUPURE_AUTOROUTE,"");
		/*ajout des fichiers fils à chaque pere*/
		sprintf(vl_str,"%s/format.ksh %d %d %d %d %s",		
			C_PATH_XML,
			pa_fmc.Evt.numero,
			vl_noeud.Evt.cle,
			pa_fmc.Evt_Consequence.numero,
			pa_fmc.Evt_Consequence.cle,
			chemin_export);
		system(vl_str);
		XZST_03EcritureTrace(XZSTC_WARNING,"ajout fils route coupee fin %s",vl_str);	
	}

	/* sens 2 */		
	vl_nb_voies=0;
	vl_nb_voies_cir=0;
	route_coupee=0;

	gen_cumul_voies( pa_fmc.VL_I , &vl_nb_voies , &vl_nb_voies_cir);
	gen_cumul_voies( pa_fmc.VM1_I , &vl_nb_voies , &vl_nb_voies_cir);
	gen_cumul_voies( pa_fmc.VM2_I , &vl_nb_voies , &vl_nb_voies_cir);
	gen_cumul_voies( pa_fmc.VR_I, &vl_nb_voies , &vl_nb_voies_cir);
	gen_cumul_voies( pa_fmc.BAU_I, &vl_nb_voies , &vl_nb_voies_cir);

	/*si les voies sens inverse éxiste et sont toutes bouchées genere le fichier du fils*/
	if (vl_nb_voies_cir==0 && vl_nb_voies!=0)
	{
		vl_noeud=pa_fmc;
		vl_noeud.Type = C_FMC_COUPURE_AUTOROUTE;
		vl_noeud.Evt.cle = 200+(pa_fmc.Evt.cle);
		if (vl_noeud.Sens==XDC_SENS_SUD)
			vl_noeud.Sens=XDC_SENS_NORD;
		else if (vl_noeud.Sens==XDC_SENS_NORD)
			vl_noeud.Sens=XDC_SENS_SUD;
		ada2_xml_EVT_write(vl_noeud,C_FMC_COUPURE_AUTOROUTE,"");
		/*ajout des fichiers fils à chaque pere*/
		sprintf(vl_str,"%s/format.ksh %d %d %d %d %s",		
			C_PATH_XML,
			pa_fmc.Evt.numero,
			vl_noeud.Evt.cle,
			pa_fmc.Evt_Consequence.numero,
			pa_fmc.Evt_Consequence.cle,
			chemin_export);
			system(vl_str);
		XZST_03EcritureTrace(XZSTC_WARNING,"ajout fils route coupee sens inverse %s",vl_str);
	} 
	else if (va_coupure_route_inverse_version_precedente)
	{
		vl_noeud=pa_fmc;
		vl_noeud.Bloc_Optio.Fin = 'Y';
		vl_noeud.Type = C_FMC_COUPURE_AUTOROUTE;
		vl_noeud.Evt.cle = 200+(pa_fmc.Evt.cle);
		if (vl_noeud.Sens==XDC_SENS_SUD)
			vl_noeud.Sens=XDC_SENS_NORD;
		else if (vl_noeud.Sens==XDC_SENS_NORD)
			vl_noeud.Sens=XDC_SENS_SUD;
		ada2_xml_EVT_write(vl_noeud,C_FMC_COUPURE_AUTOROUTE,"");
		/*ajout des fichiers fils à chaque pere*/
		sprintf(vl_str,"%s/format.ksh %d %d %d %d %s",		
			C_PATH_XML,
			pa_fmc.Evt.numero,
			vl_noeud.Evt.cle,
			pa_fmc.Evt_Consequence.numero,
			pa_fmc.Evt_Consequence.cle,
			chemin_export);
			system(vl_str);
		XZST_03EcritureTrace(XZSTC_WARNING,"ajout fils route coupee sens inverse %s",vl_str);
	}		
}

//Fonction qui genere les fichiers Datex vitesse et vitesse_opposee
void ada2_xml_traite_travaux_basc(XZLGT_Liste_FMC pa_fmc) {
char tl_str[255];
int vl_num;
/*if (is_version_preprod()) {*/
	XZLGT_Liste_FMC	pl_fmc1;

	vm_noeud=pa_fmc;
	if (strcmp(balise_vitesse(),"")) {
	XZST_03EcritureTrace(XZSTC_WARNING,"type %d (%d/%d)",
			pa_fmc.Type,
			pa_fmc.Evt.numero,
			pa_fmc.Evt.cle);
	if (pa_fmc.Type == XZAEC_FMC_Travaux){
	pl_fmc1 = pa_fmc;
	pl_fmc1.Type = C_FMC_VITESSE;
	pl_fmc1.Evt.cle = 50+(pa_fmc.Evt.cle);
	vm_type=XZAEC_FMC_Travaux;
	sprintf(pl_fmc1.Bloc_Optio.Cause,"ID_%d_%d",
			pa_fmc.Evt.numero,
			pa_fmc.Evt.cle);

	ada2_xml_EVT_write(pl_fmc1,pl_fmc1.Type,""); /*genere le fichier xml de speedmanagement*/
XZST_03EcritureTrace(XZSTC_WARNING,"SORTIE TRV");
								/* ajout des fils virtuels*/
								vl_num= pa_fmc.Evt.cle+50;
								sprintf(tl_str,"%s/format.ksh %d %d %d %d %s",
									C_PATH_XML,
									pa_fmc.Evt.numero,
									vl_num,
									pa_fmc.Evt_Consequence.numero,
									pa_fmc.Evt_Consequence.cle,
									chemin_export);							
								system(tl_str);
	XZST_03EcritureTrace(XZSTC_WARNING,"SORTIE TRV 2");
	}
	else
	{
//EN COURS
	//basculement... 
	/*attention si basculement la situation speed limit vitesse opposee*/
	/*est dans le sens inverse*/
	
/* creer pl_fmc1, pl_fmc2
	pl_fmc1 idem travaux speed mngt dans le mm sens que le bascul
	+ generer le fichier xml*/	
		XZLGT_Liste_FMC	pl_fmc1;
		XZLGT_Liste_FMC	pl_fmc2;
	
		vm_type=XZAEC_FMC_Basculement;
// pl_fmc1 sens 1
		pl_fmc1 = pa_fmc;
		pl_fmc1.Type = C_FMC_VITESSE;
		pl_fmc1.Evt.cle = 50+(pa_fmc.Evt.cle);
	sprintf(pl_fmc1.Bloc_Optio.Cause,"ID_%d_%d",
			pa_fmc.Evt.numero,
			pa_fmc.Evt.cle);
		ada2_xml_EVT_write(pl_fmc1,pl_fmc1.Type,""); /*genere le fichier xml*/
// pl_fmc2 sens 2

		pl_fmc2 = pa_fmc;
		pl_fmc2.Type = C_FMC_VITESSE;
		pl_fmc2.Evt.cle = 200+(pa_fmc.Evt.cle);
	sprintf(pl_fmc2.Bloc_Optio.Cause,"ID_%d_%d",
			pa_fmc.Evt.numero,
			pa_fmc.Evt.cle);
			
		strcpy(pl_fmc2.RDS.drs,pa_fmc.RDS.drp);
		strcpy(pl_fmc2.RDS.drp, pa_fmc.RDS.drs);
		
		strcpy(pl_fmc2.RDS.rpis,pa_fmc.RDS.rpip);
		strcpy(pl_fmc2.RDS.rpip, pa_fmc.RDS.rpis);
		
		strcpy(pl_fmc2.RDS.rns,pa_fmc.RDS.rnp);
		strcpy(pl_fmc2.RDS.rnp, pa_fmc.RDS.rns);
		
		strcpy(pl_fmc2.RDS.dfps,pa_fmc.RDS.dfpp);
		strcpy(pl_fmc2.RDS.dfpp, pa_fmc.RDS.dfps);
		
			if (!strcmp(pl_fmc1.RDS.l07, tm_l07_p[pa_fmc.Autoroute]))
			{
				strcpy(pl_fmc2.RDS.l07 , tm_l07_n[pa_fmc.Autoroute]);
			}
			else
			{
				strcpy(pl_fmc2.RDS.l07 , tm_l07_p[pa_fmc.Autoroute]);
			}
			
			//Inverse les valeurs de drp, drs, l05 en positive ou negative (changement du sens de la route)
			if (!strcmp(pa_fmc.RDS.drp, "positive"))
			{
				strcpy(pl_fmc2.RDS.drp,"negative");
			}
			else
			{
				strcpy(pl_fmc2.RDS.drp,"positive");
			}
			
			if (!strcmp(pa_fmc.RDS.drs, "positive"))
			{
				strcpy(pl_fmc2.RDS.drs,"negative");
			}
			else
			{
				strcpy(pl_fmc2.RDS.drs,"positive");
			}
			
			if (!strcmp(pa_fmc.RDS.l05, "positive"))
			{
				strcpy(pl_fmc2.RDS.l05,"negative");
			}
			else
			{
				strcpy(pl_fmc2.RDS.l05,"positive");
			}
			
			//Inverse les valeurs de pl_fmc.RDS.l06/pa_fmc.RDS.l10, l04/l09, l11/l12 (changement du sens de la route)
		strcpy(pl_fmc2.RDS.l06, pa_fmc.RDS.l10);
		strcpy(pl_fmc2.RDS.l10, pa_fmc.RDS.l06);
		
		strcpy(pl_fmc2.RDS.l04, pa_fmc.RDS.l09);
		strcpy(pl_fmc2.RDS.l09, pa_fmc.RDS.l04);
		
		strcpy(pl_fmc2.RDS.l11, pa_fmc.RDS.l12);
		strcpy(pl_fmc2.RDS.l12, pa_fmc.RDS.l11);
		
		ada2_xml_EVT_write(pl_fmc2,pl_fmc2.Type,""); /*genere le fichier xml*/

							/* ajout des fils virtuels*/
							vl_num= pa_fmc.Evt.cle+50;
							sprintf(tl_str,"%s/format.ksh %d %d %d %d %s",
								C_PATH_XML,
								pa_fmc.Evt.numero,
								vl_num,
								pa_fmc.Evt_Consequence.numero,
								pa_fmc.Evt_Consequence.cle,
								chemin_export);							
							system(tl_str);

							vl_num= pa_fmc.Evt.cle+200;
							sprintf(tl_str,"%s/format.ksh %d %d %d %d %s",
								C_PATH_XML,
								pa_fmc.Evt.numero,
								vl_num,
								pa_fmc.Evt_Consequence.numero,
								pa_fmc.Evt_Consequence.cle,
								chemin_export);							
							system(tl_str);

		}
	}
	XZST_03EcritureTrace(XZSTC_WARNING,"SORTIE TRV 3");
	/*}*/
}

void ada2_traite_ech(XZLGT_Liste_FMC pa_fmc, int va_mode) {
/*va_mode = 0 => on traite un pere*/
/*va_mode = 1 => on traite un fils*/
XZLGT_Liste_FMC	pl_fmc1,pl_fmc2;
char tl_str[250]="";

  XZST_03EcritureTrace(XZSTC_WARNING,"ECH mode=%d,dir %s",va_mode,pa_fmc.RDS.drp);
  XZST_03EcritureTrace(XZSTC_WARNING,"ECH type %d",pa_fmc.Type);
  XZST_03EcritureTrace(XZSTC_WARNING,"ECH num %d %d",pa_fmc.Evt.numero, pa_fmc.Evt.cle);
  XZST_03EcritureTrace(XZSTC_WARNING,"ECH num conseq %d %d",pa_fmc.Evt_Consequence.numero, pa_fmc.Evt_Consequence.cle);

  /*si FMC en sens inconnu*/
  if (!strcmp(pa_fmc.RDS.drp,"unknown")) {
	/*je cree 2 FMC virtuelles par sens*/
	pl_fmc1 = pa_fmc;
	pl_fmc2 = pa_fmc;
	pl_fmc1.Evt.cle= 100+(pa_fmc.Evt.cle);
	pl_fmc2.Evt.cle= 150+(pa_fmc.Evt.cle);
	XZST_03EcritureTrace(XZSTC_WARNING,"ECH fmc 1 = %d",pl_fmc1.Evt.cle);
	XZST_03EcritureTrace(XZSTC_WARNING,"ECH fmc 2 = %d",pl_fmc2.Evt.cle);
	pl_fmc1.Bloc_Optio.Bloc.Echangeur.Entree2=0;
	pl_fmc1.Bloc_Optio.Bloc.Echangeur.Sortie2=0;
	pl_fmc2.Bloc_Optio.Bloc.Echangeur.Entree1=0;
	pl_fmc2.Bloc_Optio.Bloc.Echangeur.Sortie1=0;
	strcpy(pl_fmc1.RDS.drp,"positive");
	strcpy(pl_fmc1.RDS.drs,"positive");
	strcpy(pl_fmc1.RDS.l05,"positive");
	strcpy(pl_fmc1.RDS.l07,tm_l07_p[pa_fmc.Autoroute]);
	strcpy(pl_fmc2.RDS.l07,tm_l07_n[pa_fmc.Autoroute]);
	strcpy(pl_fmc2.RDS.drp,"negative");
	strcpy(pl_fmc2.RDS.drs,"negative");
	strcpy(pl_fmc2.RDS.l05,"negative");
	/*strcpy(pl_fmc1.RDS.l07,tm_l07_p[*/
    ada2_xml_EVT_write(pl_fmc1,pa_fmc.Type,"");
    ada2_xml_EVT_write(pl_fmc2,pa_fmc.Type,"");

  }
  
  else {
    if (va_mode==0){
      ada2_xml_EVT_write(pa_fmc,pa_fmc.Type,"");

        sprintf(tl_str,"%s/format_ech.ksh %d %d %d %d %s",
                        C_PATH_XML,
                        pa_fmc.Evt.numero,
                        pa_fmc.Evt.cle,
                        pa_fmc.Evt_Consequence.numero,
                        pa_fmc.Evt_Consequence.cle,
                        chemin_export
						);
	system(tl_str);
    }
  }   
 
}

void ada2_evt_cb_situation_coupparcoup(T_IPC_CONN va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA pa_data,
		T_CB_ARG pa_Arg)
{
	XDY_Evt va_evt_in;
	XZLGT_Liste_FMC	*pl_liste_fmc;
	XZLGT_Liste_FMC	pl_fmc; 
	XZLGT_Liste_FMC	vl_fmcfils; 
	XDY_Entier	vl_val;
	XDY_Evt vl_evt;
	int vl_num;
	char tl_str[300];
	int 		vl_JourSemaine;
	double 	vl_HorodateSec; 
	int vl_jour,vl_mois,vl_annee,vl_heure,vl_minute,vl_seconde;  
	XZSMT_Horodate  vl_HorodateZero = "";
	char pl_fin_fichier[50];
	int fmc_trouvee=0;
	XDY_Octet vl_cle_cause_out=0;
	XDY_Octet vl_type_out=0;
	XDY_Entier vl_numero_cause_out=0;
	XDY_Entier vl_indice=0;
	XDY_Booleen vl_Test_FMC_Terminee=0;
	XDY_Booleen va_retypage_out=0;
	XDY_Entier va_test_retypage_in=0;
	XDY_Octet va_type_anterieur_out;
	XDY_Booleen va_coupure_route_version_precedente;
	XDY_Booleen va_coupure_route_inverse_version_precedente;
	XDY_Booleen vl_test,vl_retest;


	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_evt_cb_situation_coupparcoup: IN");

	if (!TipcMsgRead(pa_data->msg,
	  	T_IPC_FT_INT4, &va_evt_in.numero,
	  	T_IPC_FT_INT2, &va_evt_in.cle,
		T_IPC_FT_INT4, &va_test_retypage_in,
		NULL))
	{				
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup : Erreur decodage msg.numero=%d.cle=%d.retypage=%d",va_evt_in.numero,va_evt_in.cle,va_test_retypage_in);
		return;
	}		
	else
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup : msg decode.numero=%d.cle=%d.TestRetypage=%d",va_evt_in.numero,va_evt_in.cle,va_test_retypage_in);

	/* test retypage */
	vl_retest=1;
	vl_test=1;
	while (va_test_retypage_in && vl_test)
	{
		/* lecture de la base pour savoir si cette fmc vient d'être retypée */	
		if (XZAE195_Lire_FMC_Retypee (va_evt_in.numero,
					va_evt_in.cle,
					&va_retypage_out,
					&va_type_anterieur_out,
					&va_coupure_route_version_precedente,
					&va_coupure_route_inverse_version_precedente) != XDC_OK) 
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup: pb appel a XZAE195_Lire_FMC_Retypee");
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup : FMC retypee =%d, type=%d, coupure route version précedente = %d, coupure route inverse version précedente = %d",
					va_retypage_out,
					va_type_anterieur_out,
					va_coupure_route_version_precedente,
					va_coupure_route_inverse_version_precedente); 
		}
		if(va_retypage_out==0 && va_type_anterieur_out==0) //fmc non trouvé en base -> réveil réplication plus rapide que l'écriture
		{
			if (vl_retest)
				vl_retest=0; //on arme la fin du test
			else
			{
				vl_test=0; // on arrête le test et la fonction car la fmc n'est pas trouvé en base
				XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup : FMC non trouvee en base FMC_HIS, arret de la fonction");
				return;
			}

			sleep (1); //Tempo pour attendre que l'acriture en base soit faite
		}
		else
			vl_test=0; //Retypage ok on arrête le test
	}
	

	/* renseignement du type de publication */
	vg_publication_type=C_PUBLICATION_COUPPARCOUP;
	
	/*verrouille la memoire*/
	/* ama : on verouille la memoire pendant tout le traitement */
	/* car on ecrit dans la proc */ 
	XZLG500_SemCall(-1,XZLGC_SEM_FMC);

	pl_liste_fmc=NULL;
	pl_liste_fmc=XZLG985_Get_FMC();

	/* ama : on verouille la memoire pendant tout le traitement */
	/* donc on appel le deveirouillage avant chaque return */
	/*deverrouille la memoire*/		
/*	XZLG500_SemCall(1,XZLGC_SEM_FMC); */


	for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) 
	{

		if (pl_liste_fmc[vl_val].Evt.numero==0)
		break;
		
		/* AMA : je sais pas pourquoi c'est faire */
		XZLG10_Memorise_Diffusion_FMC(pl_liste_fmc[vl_val].Evt.numero,
				pl_liste_fmc[vl_val].Evt.cle,
				XZLGC_DIFF_CLIENT_2);
			
	}

	/*Trouver l'evt transmit dans la liste complète*/
	for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) 
	{
		if (pl_liste_fmc[vl_val].Evt.numero==0)
			break;
		if( (pl_liste_fmc[vl_val].Evt.numero==va_evt_in.numero) &&
		(pl_liste_fmc[vl_val].Evt.cle==va_evt_in.cle))
		{
			pl_fmc = pl_liste_fmc[vl_val];
			fmc_trouvee=1;
			break;
		}
	}

	if(fmc_trouvee)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup FMC TRANSMISE trouvee dans la liste num cle = %d %d. Index=%d",pl_fmc.Evt.numero,pl_fmc.Evt.cle,vl_val);
	}	
	else 
	{		
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup FMC TRANSMISE non trouvee dans la liste num cle = %d %d",va_evt_in.numero,va_evt_in.cle);
		XZST_03EcritureTrace(XZSTC_WARNING,"vl_val = %d",vl_val);

		/* AMA : ici on ajoute une FMC dans une case libre mais la memoire est déveroilléé ???  */
		/* FMC non trouvé dans la liste de la mémoire partagée, c'est un nouvel EVT on va le rajouter */
		pl_liste_fmc[vl_val].Evt.numero=va_evt_in.numero;
		pl_liste_fmc[vl_val].Evt.cle=va_evt_in.cle;
	}
	fmc_trouvee=0;	
	
	/* Get FMC transmise Type */
	if (XZAE202_Get_FMC_Type(pl_liste_fmc[vl_val].Evt.numero,
                                 pl_liste_fmc[vl_val].Evt.cle,
				&vl_numero_cause_out,
				&vl_cle_cause_out,
                                &vl_type_out) != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup : pb appel a XZAE202_Get_FMC_Type");
		/* ama : deverrouille la memoire */
		XZLG500_SemCall(1,XZLGC_SEM_FMC); 
		return;
	}
	else 
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup : retour xzae202 cause out=%d, cle_cause_out=%d, type_out=%d",vl_numero_cause_out,vl_cle_cause_out,vl_type_out);

	/* test du retour de XZAE202_Get_FMC_Type */
	if (vl_type_out == 0)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup : retour XZAE202_Get_FMC_Type Type = 0, arret de la publication coupparcoup");
		/* ama : deverrouille la memoire */
		XZLG500_SemCall(1,XZLGC_SEM_FMC); 
		return;
	}

	/* Ajout de la nouvelle FMC à la suite de la liste */	
	pl_liste_fmc[vl_val].Evt_Cause.numero=vl_numero_cause_out;
	pl_liste_fmc[vl_val].Evt_Cause.cle=vl_cle_cause_out;
	if(!va_retypage_out)
		pl_liste_fmc[vl_val].Type=vl_type_out;
	else
		pl_liste_fmc[vl_val].Type=va_type_anterieur_out;
	/* si la fonction n'est pas un retypage */
	if (!va_retypage_out)
	{
	/* Appel fonction de mise à jour de la FMC transmise */
		if (ada_evt_cpc_mise_a_jour(pl_liste_fmc,vl_val)  != XDC_OK) {
			XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup : FMC %s %s non trouvée en base ",va_evt_in.numero,va_evt_in.cle);
			sleep (5);
			if (ada_evt_cpc_mise_a_jour(pl_liste_fmc,vl_val)  != XDC_OK) {
				XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup : FMC %s %s toujours non trouvée en base ",va_evt_in.numero,va_evt_in.cle);
				/* ama : deverrouille la memoire */
				XZLG500_SemCall(1,XZLGC_SEM_FMC); 
				return;
			}
		}
	}
	else 
	{
		/* FMC retyp�e on annule la version en cours et on rappel la publication coup par coup pour publier la FMC avec le nouveau type*/
		pl_liste_fmc[vl_val].Bloc_Optio.Fausse_Alerte='Y';
		
		/* Envoie message pour publication de la FMC avec nouveau type */
		if ( !TipcSrvMsgWrite (	XDG_TIMER_TAGEN,
					XDM_IdentMsg(XDM_AS_TIMER_SITU_CPC),
					XDM_FLG_SRVMSGWRITE,
					T_IPC_FT_INT4, va_evt_in.numero,
					T_IPC_FT_INT2, va_evt_in.cle,
					T_IPC_FT_INT4, 0,
					NULL
					))
		{
			 /*A Trace envoi message XDM_AS_TIMER_SITU_CPC non effectue */    			
			 XZST_03EcritureTrace(XZSTC_WARNING, "Echec envoi message XDM_AS_TIMER_SITU_CPC.");
		}
		else
		{
			/*A Trace envoi message XDM_AS_TIMER_SITU_CPC */ 	 	
			XZST_03EcritureTrace(XZSTC_MESSAGE, "Envoi du message XDM_AS_TIMER_SITU_CPC sur %s. FMC:%d, cle:%d", XDG_TIMER_TAGEN,va_evt_in.numero,va_evt_in.cle);
		}
		
	}

	/*recherche du pere originel de chaque FMC*/
	for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) 
	{
		if (pl_liste_fmc[vl_val].Evt.numero==0)
			break;
		pl_liste_fmc[vl_val].Evt_Consequence.numero = -1;		
		vl_evt = gen_recherche_pere(pl_liste_fmc, vl_val);
		pl_liste_fmc[vl_val].Evt_Consequence = vl_evt;
		vl_fmcfils = gen_recherche_fils(pl_liste_fmc, vl_val);								
		pl_liste_fmc[vl_val].Bloc_Optio.Evt_datex2Fils = vl_fmcfils.Evt;
		pl_liste_fmc[vl_val].Bloc_Optio.Evt_datex2Fils_version =vl_fmcfils.VNM;
	}

	/*Desherite les fmc pere-fils pour des type particulier*/
	for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) 
	{
		if (pl_liste_fmc[vl_val].Evt.numero==0)
			break;
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup FMC desherite : %d %d. conséquence =%d. Index=%d",pl_liste_fmc[vl_val].Evt.numero,pl_liste_fmc[vl_val].Evt.cle,pl_liste_fmc[vl_val].Evt_Consequence.numero,vl_val);
		if (desherite_FMC(pl_liste_fmc,vl_val) )
		{
			pl_liste_fmc[vl_val].Evt_Consequence = pl_liste_fmc[vl_val].Evt;
		}
		/* Quand on trouve la FMC transmise */
		if( (pl_liste_fmc[vl_val].Evt.numero==va_evt_in.numero) &&
		(pl_liste_fmc[vl_val].Evt.cle==va_evt_in.cle))
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup FMC %d/%d consequence =%d",pl_liste_fmc[vl_val].Evt.numero,pl_liste_fmc[vl_val].Evt.cle,pl_liste_fmc[vl_val].Evt_Consequence.numero);
			pl_fmc = pl_liste_fmc[vl_val];
		}
	}

	XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup FMC pl_fmc : %d %d. conséquence =%d. Index=%d",pl_fmc.Evt.numero,pl_fmc.Evt.cle,pl_fmc.Evt_Consequence.numero,vl_val);

	/*Trouver l'evt qui est la conséquence de lévenement transmit (ce qui nous donne la fmc originel, si il est père ou son pere, si il est fils) dans la liste complète*/
	for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) 
	{
		if (pl_liste_fmc[vl_val].Evt.numero==0)
			break;
		if( (pl_liste_fmc[vl_val].Evt.numero==pl_fmc.Evt_Consequence.numero))
		{
			pl_fmc = pl_liste_fmc[vl_val];
			fmc_trouvee=1;
			break;
		}
	}

	if(fmc_trouvee)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup FMC PERE trouvee dans la liste num cle = %d %d. Index=%d, type=%d",pl_fmc.Evt.numero,pl_fmc.Evt.cle,vl_val,pl_liste_fmc[vl_val].Type);
	}	
	else 
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt_cb_situation_coupparcoup FMC PERE non trouvee dans la liste num cle = %d %d",va_evt_in.numero,va_evt_in.cle);
		/* ama : deverrouille la memoire */
		XZLG500_SemCall(1,XZLGC_SEM_FMC); 
		return;
	}

    	/*on genere maintenant les fichiers pour la FMC pere*/
	if (!(pl_fmc.Prevision) && (pl_fmc.Type!=XZAEC_FMC_Autre) && (pl_fmc.Type!=XZAEC_FMC_EvtHorsReseau)) 
	{			
		if ( (pl_fmc.Evt_Consequence.numero == pl_fmc.Evt.numero) &&
		     (pl_fmc.Evt_Consequence.cle == pl_fmc.Evt.cle))
		{	

			vm_type = pl_fmc.Type;			
			/* Allocation de la memoire numero element - dernier numero dans le regroupement */
			if (vg_numero_element_regroupement==NULL)
				vg_numero_element_regroupement = (T_NUMERO_ELEMENT_REGROUPEMENT *) malloc((vg_size_struct_numero_regroupement+1)*sizeof(T_NUMERO_ELEMENT_REGROUPEMENT));
			else
				vg_numero_element_regroupement = (T_NUMERO_ELEMENT_REGROUPEMENT *) realloc( vg_numero_element_regroupement, (vg_size_struct_numero_regroupement+1)*sizeof(T_NUMERO_ELEMENT_REGROUPEMENT));

			vg_size_struct_numero_regroupement++;

			/* renseigne le numero de fmc à l'indice */
			vg_numero_element_regroupement[vg_size_struct_numero_regroupement-1].numero_regroupement=pl_fmc.Evt_Consequence.numero;

			/* initialisation du numero de l'element dans le regroupement */
			vg_numero_element_regroupement[vg_size_struct_numero_regroupement-1].der_numero_element_attache_regroupement=1;

			/* ecriture header fmc pere */
			ada2_xml_EVT_write(pl_fmc,C_GEN_HEADER_EVT_PERE,"sit");

			/*ne pas generer de fils pour ferm ech et ech deconseille en direction unknown*/
			if ( (   (pl_fmc.Type==XZAEC_FMC_FermetureEch) || 
				 (pl_fmc.Type==XZAEC_FMC_EchDeconseille) ) &&
				 ((!strcmp(pl_fmc.RDS.drp,"unknown")) )
				) 
			{
				XZST_03EcritureTrace(XZSTC_FONCTION,"PAS DE FICHIER FILS GENERE"); 
			}
			else 
			{
				ada2_xml_EVT_write(pl_fmc, pl_fmc.Type,"");
			}
		}
		else  
		{		
			XZST_03EcritureTrace(XZSTC_FONCTION,"PAS UN FILS %d %d %d %d",
			pl_fmc.Evt.numero,
			pl_fmc.Evt.cle,
			pl_fmc.Evt_Consequence.numero,
			pl_fmc.Evt_Consequence.cle);
			/* ama : deverrouille la memoire */
			XZLG500_SemCall(1,XZLGC_SEM_FMC); 
			return;
		}
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_FONCTION,"PAS A TRAITER %d %d",
		pl_fmc.Evt.numero,
		pl_fmc.Evt.cle);
		/* ama : deverrouille la memoire */
		XZLG500_SemCall(1,XZLGC_SEM_FMC); 
		return;
	}

	/*on genere maintenant les fichiers DatexII pour les fils*/
	for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) 
	{
		if (pl_liste_fmc[vl_val].Evt.numero==0)
			break;
    
		vm_type = pl_liste_fmc[vl_val].Type;

		/* If FMC[indice].BlocOption.Fin=='Y' il faut que la FMC[indice].numero/cle soit celle en param, pour ne transmettre que cette fmc avec la balise <end>true */
		if (pl_liste_fmc[vl_val].Bloc_Optio.Fin == 'Y')
		{
			if ( pl_liste_fmc[vl_val].Evt.numero ==va_evt_in.numero && 
				 pl_liste_fmc[vl_val].Evt.cle == va_evt_in.cle )
			{
				vl_Test_FMC_Terminee=1;
			}
			else
			{
				vl_Test_FMC_Terminee=0;
			}
		}
		else
		{
			vl_Test_FMC_Terminee=1;
		}

		if 	(!(pl_liste_fmc[vl_val].Prevision) && vl_Test_FMC_Terminee &&
			 (pl_liste_fmc[vl_val].Evt_Consequence.numero==pl_fmc.Evt.numero) && 
			 (pl_liste_fmc[vl_val].Evt_Consequence.cle==pl_fmc.Evt.cle) &&
			 (pl_liste_fmc[vl_val].Type!=XZAEC_FMC_Autre) && 
			 (pl_liste_fmc[vl_val].Type!=XZAEC_FMC_EvtHorsReseau) ) 
		{
			XZST_03EcritureTrace(XZSTC_FONCTION,"FILS %d %d de PERE %d %d -- %d",
					pl_liste_fmc[vl_val].Evt.numero,				
					pl_liste_fmc[vl_val].Evt.cle,
					pl_liste_fmc[vl_val].Evt_Consequence.numero,
					pl_liste_fmc[vl_val].Evt_Consequence.cle, 
					((pl_liste_fmc[vl_val].Evt_Consequence.numero==pl_liste_fmc[vl_val].Evt.numero) && (pl_liste_fmc[vl_val].Evt_Consequence.cle ==pl_liste_fmc[vl_val].Evt.cle)));

			if  ( ( (pl_liste_fmc[vl_val].Evt_Consequence.numero != pl_liste_fmc[vl_val].Evt.numero) ||
					(pl_liste_fmc[vl_val].Evt_Consequence.cle != pl_liste_fmc[vl_val].Evt.cle) ) ||
				  ( (pl_liste_fmc[vl_val].Type==XZAEC_FMC_FermetureEch) ||
					(pl_liste_fmc[vl_val].Type==XZAEC_FMC_EchDeconseille) ) ||
					(pl_liste_fmc[vl_val].Type==XZAEC_FMC_Travaux) ||
					(pl_liste_fmc[vl_val].Type==XZAEC_FMC_Basculement) ||
					(pl_liste_fmc[vl_val].Type==XZAEC_FMC_BasculTravaux) )		 
			{
				/*traitement particulier ferm ech et ech deconseille*/
				if ( (pl_liste_fmc[vl_val].Type==XZAEC_FMC_FermetureEch) ||
					 (pl_liste_fmc[vl_val].Type==XZAEC_FMC_EchDeconseille) ) 
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"ECH genere les 2 fils de %d %d",
						pl_liste_fmc[vl_val].Evt.numero,
						pl_liste_fmc[vl_val].Evt.cle);

					ada2_traite_ech(pl_liste_fmc[vl_val],
									( (pl_liste_fmc[vl_val].Evt_Consequence.numero==pl_liste_fmc[vl_val].Evt.numero) && (pl_liste_fmc[vl_val].Evt_Consequence.cle ==pl_liste_fmc[vl_val].Evt.cle) ) );

					if (!strcmp(pl_liste_fmc[vl_val].RDS.drp,"unknown")) {
						/* ajout des 2 fils virtuels*/
						vl_num= pl_liste_fmc[vl_val].Evt.cle+100;
						sprintf(tl_str,"%s/format_ech.ksh %d %d %d %d %s",
							C_PATH_XML,
							pl_liste_fmc[vl_val].Evt.numero,
							vl_num,
							pl_liste_fmc[vl_val].Evt_Consequence.numero,
							pl_liste_fmc[vl_val].Evt_Consequence.cle,
							chemin_export);
							
						XZST_03EcritureTrace(XZSTC_WARNING,"ECH appel %s",tl_str);
						system(tl_str);
						vl_num= pl_liste_fmc[vl_val].Evt.cle+150;
						sprintf(tl_str,"%s/format_ech.ksh %d %d %d %d %s",
							C_PATH_XML,
							pl_liste_fmc[vl_val].Evt.numero,
							vl_num,
							pl_liste_fmc[vl_val].Evt_Consequence.numero,
							pl_liste_fmc[vl_val].Evt_Consequence.cle,
							chemin_export);
							
						XZST_03EcritureTrace(XZSTC_WARNING,"ECH appel %s",tl_str);
						system(tl_str);
					}
				}
				else if (pl_liste_fmc[vl_val].Type==XZAEC_FMC_Travaux) 
				{			
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 1");
					vm_type=XZAEC_FMC_Travaux;
					
					/*on est sur des travaux qui sont consequence d une autre FMC*/
					if  ( ( (pl_liste_fmc[vl_val].Evt_Consequence.numero != pl_liste_fmc[vl_val].Evt.numero) ||
							(pl_liste_fmc[vl_val].Evt_Consequence.cle != pl_liste_fmc[vl_val].Evt.cle) ) ) 
					{
						/*genere le fichier du fils*/
						XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 2");
						ada2_xml_EVT_write(pl_liste_fmc[vl_val],pl_liste_fmc[vl_val].Type,"");
						XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 3");

						/*ajout des fichiers fils H chaque pere*/
						sprintf(tl_str,"%s/format.ksh %d %d %d %d %s",
							C_PATH_XML,
							pl_liste_fmc[vl_val].Evt.numero,
							pl_liste_fmc[vl_val].Evt.cle,
							pl_liste_fmc[vl_val].Evt_Consequence.numero,
							pl_liste_fmc[vl_val].Evt_Consequence.cle,
							chemin_export);
						system(tl_str);
				
						XZST_03EcritureTrace(XZSTC_WARNING,"ajout fils %s",tl_str);									
					}
					
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 4");
					ada2_xml_traite_travaux_basc(pl_liste_fmc[vl_val]);
						XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
						XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
						XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
						XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
						XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
						XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
						XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
						XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
						XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
						XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
						XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
						XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
						XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
						XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
					
					/*vl_num= pl_liste_fmc[vl_val].Evt.cle+50;
					sprintf(tl_str,"%s/format.ksh %d %d %d %d %s",
						C_PATH_XML,
						pl_liste_fmc[vl_val].Evt.numero,
						vl_num,
						pl_liste_fmc[vl_val].Evt_Consequence.numero,
						pl_liste_fmc[vl_val].Evt_Consequence.cle,
						chemin_export);							
					system(tl_str);*/	
				}			
				//EN COURS	
				else if ((pl_liste_fmc[vl_val].Type==XZAEC_FMC_Basculement) || (pl_liste_fmc[vl_val].Type==XZAEC_FMC_BasculTravaux))
				{
					vm_type=pl_liste_fmc[vl_val].Type;
					if  ( ( (pl_liste_fmc[vl_val].Evt_Consequence.numero != pl_liste_fmc[vl_val].Evt.numero) ||
							(pl_liste_fmc[vl_val].Evt_Consequence.cle != pl_liste_fmc[vl_val].Evt.cle) ) ) 
					{
						ada2_xml_EVT_write(pl_liste_fmc[vl_val],pl_liste_fmc[vl_val].Type,"");
						sprintf(tl_str,"%s/format.ksh %d %d %d %d %s",
							C_PATH_XML,
							pl_liste_fmc[vl_val].Evt.numero,
							pl_liste_fmc[vl_val].Evt.cle,
							pl_liste_fmc[vl_val].Evt_Consequence.numero,
							pl_liste_fmc[vl_val].Evt_Consequence.cle,
							chemin_export);
						system(tl_str);
				
						XZST_03EcritureTrace(XZSTC_WARNING,"ajout fils %s",tl_str);									
					}
					
					ada2_xml_traite_travaux_basc(pl_liste_fmc[vl_val]);
					
					XZST_03EcritureTrace(XZSTC_WARNING,"BASC 1");
					/* ajout des fils virtuels*/
					vl_num= pl_liste_fmc[vl_val].Evt.cle+50;
					sprintf(tl_str,"%s/format.ksh %d %d %d %d %s",
						C_PATH_XML,
						pl_liste_fmc[vl_val].Evt.numero,
						vl_num,
						pl_liste_fmc[vl_val].Evt_Consequence.numero,
						pl_liste_fmc[vl_val].Evt_Consequence.cle,
						chemin_export);							
					system(tl_str);

					XZST_03EcritureTrace(XZSTC_WARNING,"BASC 2");
					vl_num= pl_liste_fmc[vl_val].Evt.cle+200;

					sprintf(tl_str,"%s/format.ksh %d %d %d %d %s",
						C_PATH_XML,
						pl_liste_fmc[vl_val].Evt.numero,
						vl_num,
						pl_liste_fmc[vl_val].Evt_Consequence.numero,
						pl_liste_fmc[vl_val].Evt_Consequence.cle,
						chemin_export);							
					system(tl_str);

					XZST_03EcritureTrace(XZSTC_WARNING,"ajout fils VITESSE BASC");
				}
				else 
				{
					/*genere le fichier du fils*/
					ada2_xml_EVT_write(pl_liste_fmc[vl_val],pl_liste_fmc[vl_val].Type,"");

					/*ajout des fichiers fils à chaque pere*/
					sprintf(tl_str,"%s/format.ksh %d %d %d %d %s",
					C_PATH_XML,
					pl_liste_fmc[vl_val].Evt.numero,
					pl_liste_fmc[vl_val].Evt.cle,
					pl_liste_fmc[vl_val].Evt_Consequence.numero,
					pl_liste_fmc[vl_val].Evt_Consequence.cle,
					chemin_export);
					system(tl_str);
					XZST_03EcritureTrace(XZSTC_WARNING,"ajout fils %s",tl_str);
				}
			}
			
			//FMC virtuel coupure est exclut pour Travaux et Bouchon
			if (pl_liste_fmc[vl_val].Type != XZAEC_FMC_QueueBouchon && 
			pl_liste_fmc[vl_val].Type != XZAEC_FMC_Basculement && 
			pl_liste_fmc[vl_val].Type != XZAEC_FMC_BasculTravaux &&
			pl_liste_fmc[vl_val].Type != XZAEC_FMC_Escargot)
			{
				// Check si la route est coupee pour rajouter une FMC route coupee		
				ada2_xml_traite_route_coupee(pl_liste_fmc[vl_val],va_coupure_route_version_precedente,va_coupure_route_inverse_version_precedente);
			}
		} 
	}

	/*termine le fichier du peres*/
	XZST_03EcritureTrace(XZSTC_FONCTION,"termine FILS %d %d de PERE %d %d",
			pl_liste_fmc[vl_val].Evt.numero,
			pl_liste_fmc[vl_val].Evt.cle,  
			pl_liste_fmc[vl_val].Evt_Consequence.numero,
			pl_liste_fmc[vl_val].Evt_Consequence.cle);

	sprintf(tl_str,"%s/endfile.ksh %d %d %s",
	  C_PATH_XML,		
	  pl_fmc.Evt.numero,
	  pl_fmc.Evt.cle,
	  chemin_export);
	XZST_03EcritureTrace(XZSTC_FONCTION,"endfile %s",tl_str);	
	system(tl_str);   

	/*A Lecture horodate courante systeme */
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
	XZSM_11ConversionHorodate(vl_HorodateSec, &vl_HorodateZero);

	if (sscanf( vl_HorodateZero, "%2d/%2d/%4d %2d:%2d:%2d",
			&vl_jour,
		    &vl_mois,
			&vl_annee,
			&vl_heure,
		    &vl_minute,
		    &vl_seconde) != 6) 
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
		   "ada2_evt_cb_situation_coupparcoup : le format de la date est incorrecte !" );
		/* ama : deverrouille la memoire */
		XZLG500_SemCall(1,XZLGC_SEM_FMC); 
		return;
	}

	sprintf(pl_fin_fichier,"_%02d%02d%04d%02d%02d",
	  vl_jour,
	  vl_mois,		
	  vl_annee,
	  vl_heure,
	  vl_minute);

	/*genere le fichier header global*/		
	ada2_xml_EVT_write(pl_liste_fmc[1],C_GEN_HEADER_EVT_GLOBAL,"glob");

	/*genere le fichier unique*/
	if (is_version_preprod())
		sprintf(tl_str,"%s/concat.ksh %s %s 1",
			C_PATH_XML,
			chemin_export,
			pl_fin_fichier);
	else
		sprintf(tl_str,"%s/concat.ksh %s %s 0",
			C_PATH_XML,
			chemin_export,
			pl_fin_fichier);

	system(tl_str);

	sprintf( tl_str,"find %s -mtime +%d -exec rm {} \\;", chemin_export, CO_NB_JOUR_PURGE);
	system(tl_str);

	//Copie des fichiers datex 2 dans les répertoires des destinataires
	ada2_copie_fichier_vers_ftp_rep(DATA_TYPE_EVT);

	free(vg_numero_element_regroupement);
	vg_numero_element_regroupement=NULL;
	vg_size_struct_numero_regroupement=0;

	/* Disable keep alive Timer */
	if (XZSM_02AnnulerProgrammation("fct_datex2_envoi_keep_alive",
					XDG_TIMER_DATEX2,
					XDM_AS_TIMER_DATEX2))
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "ada2_evt_cb_situation_coupparcoup : pb XZSM_02AnnulerProgrammation");
	}
	else
		XZST_03EcritureTrace( XZSTC_WARNING, "ada2_evt_cb_situation_coupparcoup : XZSM_02AnnulerProgrammation Timer KeepAlive");


	/* armement timer pour message keep_alive */
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
	vl_HorodateSec+=vg_frequence_keep_alive;
	if (XZSM_01DemanderProgrammation(vl_HorodateSec,
				vg_frequence_keep_alive,
				XDG_TIMER_DATEX2,	
				XDM_AS_TIMER_DATEX2,
				"fct_datex2_envoi_keep_alive",
				XZSMC_SEC_VRAI) != XDC_OK) 
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "ada2_evt_cb_situation_coupparcoup : pb XZSM_01DemanderProgrammation KeepAlive Timer");
	}	
	else
		XZST_03EcritureTrace( XZSTC_WARNING, "ada2_evt_cb_situation_coupparcoup : XZSM_01DemanderProgrammation : armement timer KeepAlive effectue");

	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_evt_cb_situation_coupparcoup: OUT");
	/* ama : deverrouille la memoire */
	XZLG500_SemCall(1,XZLGC_SEM_FMC); 

	return ;
}

/* Fichier : @(#)ada2_evt.c	1.0        Release : 1.0        Date : 01/04/20
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* reveil pour verifier qu'il y a des fmc a exporter
*****************************************************
* SEQUENCE D'APPEL :
* ada2_evt_cb_situation_recap
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
*******************************************************/
void ada2_evt_cb_situation_recap()
{
  XZLGT_Liste_FMC	*pl_liste_fmc;
  XDY_Entier	vl_val;
  XDY_Evt vl_evt;
  XZLGT_Liste_FMC vl_fmcfils;
  int 		vl_JourSemaine;
  double 	vl_HorodateSec; 
  char tl_str[300];
  int vl_jour,vl_mois,vl_annee,vl_heure,vl_minute,vl_seconde;
  int vl_num;
  int indice;
  char vl_horodate[30];
  XZSMT_Horodate  vl_HorodateZero = "";
  char pl_fin_fichier[50];
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_evt_cb_situation_recap: IN");

  /* renseignement du type de publication */
  vg_publication_type=C_PUBLICATION_RECAPITULATIVE;
  
 
  /*verrouille la memoire*/
  XZLG500_SemCall(-1,XZLGC_SEM_FMC);

  pl_liste_fmc=NULL;
  pl_liste_fmc=XZLG985_Get_FMC();

  /*deverrouille la memoire*/
  //XZLG500_SemCall(1,XZLGC_SEM_FMC);

  while(pl_liste_fmc[0].Evt.numero==0) //Mémoire partagée vide on tempo
  {
	XZLG500_SemCall(1,XZLGC_SEM_FMC); //Déverouille sémphore
	XZST_03EcritureTrace( XZSTC_WARNING, "ada2_evt_cb_situation_recap : Mémoire partagée vide, on tempo 1min");
	sleep(60);//tempo

	//errouille la memoire
	XZLG500_SemCall(-1,XZLGC_SEM_FMC);

	pl_liste_fmc=NULL;
	pl_liste_fmc=XZLG985_Get_FMC();

  }
  
  for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) {

    if (pl_liste_fmc[vl_val].Evt.numero==0)
      break;
 
    XZLG10_Memorise_Diffusion_FMC(pl_liste_fmc[vl_val].Evt.numero,
				pl_liste_fmc[vl_val].Evt.cle,
				XZLGC_DIFF_CLIENT_2);
				
  }

  /*recherche du pere originel de chaque FMc*/
  for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) {
    if (pl_liste_fmc[vl_val].Evt.numero==0)
      break;
    pl_liste_fmc[vl_val].Evt_Consequence.numero = -1;
    vl_evt = gen_recherche_pere(pl_liste_fmc, vl_val);
    pl_liste_fmc[vl_val].Evt_Consequence = vl_evt;	
    vl_fmcfils = gen_recherche_fils(pl_liste_fmc, vl_val);							
    pl_liste_fmc[vl_val].Bloc_Optio.Evt_datex2Fils = vl_fmcfils.Evt;
    pl_liste_fmc[vl_val].Bloc_Optio.Evt_datex2Fils_version = vl_fmcfils.VNM;
  }

  /*Désherite les fmc père fils */
  for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) 
  {
    if (pl_liste_fmc[vl_val].Evt.numero==0)
	break;
    if (desherite_FMC(pl_liste_fmc,vl_val) )
	pl_liste_fmc[vl_val].Evt_Consequence = pl_liste_fmc[vl_val].Evt;	
  }


  /*on genere maintenant les fichiers pour chaque pere*/
  for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) {
    if (pl_liste_fmc[vl_val].Evt.numero==0)
      break;

    vm_type = pl_liste_fmc[vl_val].Type;
    if (!(pl_liste_fmc[vl_val].Prevision) && (pl_liste_fmc[vl_val].Bloc_Optio.Fin!='Y') && (pl_liste_fmc[vl_val].Type!=XZAEC_FMC_Autre) && (pl_liste_fmc[vl_val].Type!=XZAEC_FMC_EvtHorsReseau) ) {
      if ( (pl_liste_fmc[vl_val].Evt_Consequence.numero == pl_liste_fmc[vl_val].Evt.numero) &&
         (pl_liste_fmc[vl_val].Evt_Consequence.cle == pl_liste_fmc[vl_val].Evt.cle))
	  {
		/* Allocation de la memoire numero element - dernier numero dans le regroupement */
		if (vg_numero_element_regroupement==NULL)

			vg_numero_element_regroupement = (T_NUMERO_ELEMENT_REGROUPEMENT *) malloc((vg_size_struct_numero_regroupement+1)*sizeof(T_NUMERO_ELEMENT_REGROUPEMENT));

		else
			vg_numero_element_regroupement = (T_NUMERO_ELEMENT_REGROUPEMENT *) realloc( vg_numero_element_regroupement, (vg_size_struct_numero_regroupement+1)*sizeof(T_NUMERO_ELEMENT_REGROUPEMENT));

		vg_size_struct_numero_regroupement++;
XZST_03EcritureTrace(XZSTC_FONCTION,"Realloc");
		/* renseigne le numero de fmc à l'indice */
		vg_numero_element_regroupement[vg_size_struct_numero_regroupement-1].numero_regroupement=pl_liste_fmc[vl_val].Evt.numero;

		/* initialisation du numero de l'element dans le regroupement */
		vg_numero_element_regroupement[vg_size_struct_numero_regroupement-1].der_numero_element_attache_regroupement=1;

		/* ecriture header fmc pere */
		ada2_xml_EVT_write(pl_liste_fmc[vl_val],C_GEN_HEADER_EVT_PERE,"sit");

		/*ne pas generer de fils pour ferm ech et ech deconseille en direction unknown*/
		if ( (   (pl_liste_fmc[vl_val].Type==XZAEC_FMC_FermetureEch) || 
				 (pl_liste_fmc[vl_val].Type==XZAEC_FMC_EchDeconseille) ) &&
				 ((!strcmp(pl_liste_fmc[vl_val].RDS.drp,"unknown")) )
		   ) 
		{
		   XZST_03EcritureTrace(XZSTC_FONCTION,"PAS DE FICHIER FILS GENERE"); 
		}
		else 
		{
			ada2_xml_EVT_write(pl_liste_fmc[vl_val], pl_liste_fmc[vl_val].Type,"");
		}
	  }
      else  {
	XZST_03EcritureTrace(XZSTC_FONCTION,"PAS UN FILS %d %d %d %d",
				pl_liste_fmc[vl_val].Evt.numero,
				pl_liste_fmc[vl_val].Evt.cle,
				pl_liste_fmc[vl_val].Evt_Consequence.numero,
				pl_liste_fmc[vl_val].Evt_Consequence.cle);
      }
    }
    else
     XZST_03EcritureTrace(XZSTC_FONCTION,"PAS A TRAITER %d %d",
			pl_liste_fmc[vl_val].Evt.numero,
			pl_liste_fmc[vl_val].Evt.cle);
  }

  /*on genere maintenant les fichiers DatexII pour les fils*/
  for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) 
  {
    if (pl_liste_fmc[vl_val].Evt.numero==0)
      break;
    vm_type = pl_liste_fmc[vl_val].Type;
    if (!(pl_liste_fmc[vl_val].Prevision) && (pl_liste_fmc[vl_val].Bloc_Optio.Fin!='Y') && (pl_liste_fmc[vl_val].Type!=XZAEC_FMC_Autre) && (pl_liste_fmc[vl_val].Type!=XZAEC_FMC_EvtHorsReseau)) 
	{
		XZST_03EcritureTrace(XZSTC_FONCTION,"FILS %d %d de PERE %d %d -- %d",
				pl_liste_fmc[vl_val].Evt.numero,
				pl_liste_fmc[vl_val].Evt.cle,
				pl_liste_fmc[vl_val].Evt_Consequence.numero,
				pl_liste_fmc[vl_val].Evt_Consequence.cle, 
				((pl_liste_fmc[vl_val].Evt_Consequence.numero==pl_liste_fmc[vl_val].Evt.numero) && (pl_liste_fmc[vl_val].Evt_Consequence.cle ==pl_liste_fmc[vl_val].Evt.cle)));

		if  ( ( (pl_liste_fmc[vl_val].Evt_Consequence.numero != pl_liste_fmc[vl_val].Evt.numero) ||
				(pl_liste_fmc[vl_val].Evt_Consequence.cle != pl_liste_fmc[vl_val].Evt.cle) ) ||
			  ( (pl_liste_fmc[vl_val].Type==XZAEC_FMC_FermetureEch) ||
				(pl_liste_fmc[vl_val].Type==XZAEC_FMC_EchDeconseille) ) ||
				(pl_liste_fmc[vl_val].Type==XZAEC_FMC_Travaux) ||
				(pl_liste_fmc[vl_val].Type==XZAEC_FMC_Basculement) ||
				(pl_liste_fmc[vl_val].Type==XZAEC_FMC_BasculTravaux) )		 
		{
			/*traitement particulier ferm ech et ech deconseille*/
			if ( (pl_liste_fmc[vl_val].Type==XZAEC_FMC_FermetureEch) ||
				 (pl_liste_fmc[vl_val].Type==XZAEC_FMC_EchDeconseille) ) 
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"ECH genere les 2 fils de %d %d",
					pl_liste_fmc[vl_val].Evt.numero,
					pl_liste_fmc[vl_val].Evt.cle);

				ada2_traite_ech(pl_liste_fmc[vl_val],
								( (pl_liste_fmc[vl_val].Evt_Consequence.numero==pl_liste_fmc[vl_val].Evt.numero) && (pl_liste_fmc[vl_val].Evt_Consequence.cle ==pl_liste_fmc[vl_val].Evt.cle) ) );

				if (!strcmp(pl_liste_fmc[vl_val].RDS.drp,"unknown")) {
					/* ajout des 2 fils virtuels*/
					vl_num= pl_liste_fmc[vl_val].Evt.cle+100;
					sprintf(tl_str,"%s/format_ech.ksh %d %d %d %d %s",
						C_PATH_XML,
						pl_liste_fmc[vl_val].Evt.numero,
						vl_num,
						pl_liste_fmc[vl_val].Evt_Consequence.numero,
						pl_liste_fmc[vl_val].Evt_Consequence.cle,
						chemin_export);
						
					XZST_03EcritureTrace(XZSTC_WARNING,"ECH appel %s",tl_str);
					system(tl_str);
					vl_num= pl_liste_fmc[vl_val].Evt.cle+150;
					sprintf(tl_str,"%s/format_ech.ksh %d %d %d %d %s",
						C_PATH_XML,
						pl_liste_fmc[vl_val].Evt.numero,
						vl_num,
						pl_liste_fmc[vl_val].Evt_Consequence.numero,
						pl_liste_fmc[vl_val].Evt_Consequence.cle,
						chemin_export);
						
					XZST_03EcritureTrace(XZSTC_WARNING,"ECH appel %s",tl_str);
					system(tl_str);
				}
			}
			else if (pl_liste_fmc[vl_val].Type==XZAEC_FMC_Travaux) 
			{			
				XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 1");
				vm_type=XZAEC_FMC_Travaux;
				
				/*on est sur des travaux qui sont consequence d une autre FMC*/
				if  ( ( (pl_liste_fmc[vl_val].Evt_Consequence.numero != pl_liste_fmc[vl_val].Evt.numero) ||
						(pl_liste_fmc[vl_val].Evt_Consequence.cle != pl_liste_fmc[vl_val].Evt.cle) ) ) 
				{
					/*genere le fichier du fils*/
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 2");
					ada2_xml_EVT_write(pl_liste_fmc[vl_val],pl_liste_fmc[vl_val].Type,"");
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 3");

					/*ajout des fichiers fils H chaque pere*/
					sprintf(tl_str,"%s/format.ksh %d %d %d %d %s",
						C_PATH_XML,
						pl_liste_fmc[vl_val].Evt.numero,
						pl_liste_fmc[vl_val].Evt.cle,
						pl_liste_fmc[vl_val].Evt_Consequence.numero,
						pl_liste_fmc[vl_val].Evt_Consequence.cle,
						chemin_export);
					system(tl_str);
			
					XZST_03EcritureTrace(XZSTC_WARNING,"ajout fils %s",tl_str);									
				}
				
				XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 4");
				ada2_xml_traite_travaux_basc(pl_liste_fmc[vl_val]);
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
					XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 5");
				/*vl_num= pl_liste_fmc[vl_val].Evt.cle+50;
				sprintf(tl_str,"%s/format.ksh %d %d %d %d %s",
					C_PATH_XML,
					pl_liste_fmc[vl_val].Evt.numero,
					vl_num,
					pl_liste_fmc[vl_val].Evt_Consequence.numero,
					pl_liste_fmc[vl_val].Evt_Consequence.cle,
					chemin_export);							
				system(tl_str);*/	
			}			
			//EN COURS	
			else if ((pl_liste_fmc[vl_val].Type==XZAEC_FMC_Basculement) || (pl_liste_fmc[vl_val].Type==XZAEC_FMC_BasculTravaux))
			{
				vm_type=pl_liste_fmc[vl_val].Type;
				if  ( ( (pl_liste_fmc[vl_val].Evt_Consequence.numero != pl_liste_fmc[vl_val].Evt.numero) ||
						(pl_liste_fmc[vl_val].Evt_Consequence.cle != pl_liste_fmc[vl_val].Evt.cle) ) ) 
				{
					ada2_xml_EVT_write(pl_liste_fmc[vl_val],pl_liste_fmc[vl_val].Type,"");
					sprintf(tl_str,"%s/format.ksh %d %d %d %d %s",
						C_PATH_XML,
						pl_liste_fmc[vl_val].Evt.numero,
						pl_liste_fmc[vl_val].Evt.cle,
						pl_liste_fmc[vl_val].Evt_Consequence.numero,
						pl_liste_fmc[vl_val].Evt_Consequence.cle,
						chemin_export);
					system(tl_str);
			
					XZST_03EcritureTrace(XZSTC_WARNING,"ajout fils %s",tl_str);									
				}
				
				ada2_xml_traite_travaux_basc(pl_liste_fmc[vl_val]);
				
				XZST_03EcritureTrace(XZSTC_WARNING,"BASC 1");
				/* ajout des fils virtuels*/
				vl_num= pl_liste_fmc[vl_val].Evt.cle+50;
				sprintf(tl_str,"%s/format.ksh %d %d %d %d %s",
					C_PATH_XML,
					pl_liste_fmc[vl_val].Evt.numero,
					vl_num,
					pl_liste_fmc[vl_val].Evt_Consequence.numero,
					pl_liste_fmc[vl_val].Evt_Consequence.cle,
					chemin_export);							
				system(tl_str);

				XZST_03EcritureTrace(XZSTC_WARNING,"BASC 2");
				vl_num= pl_liste_fmc[vl_val].Evt.cle+200;

				sprintf(tl_str,"%s/format.ksh %d %d %d %d %s",
					C_PATH_XML,
					pl_liste_fmc[vl_val].Evt.numero,
					vl_num,
					pl_liste_fmc[vl_val].Evt_Consequence.numero,
					pl_liste_fmc[vl_val].Evt_Consequence.cle,
					chemin_export);							
				system(tl_str);

				XZST_03EcritureTrace(XZSTC_WARNING,"ajout fils VITESSE BASC");
			}
			else 
			{
				/*genere le fichier du fils*/
				ada2_xml_EVT_write(pl_liste_fmc[vl_val],pl_liste_fmc[vl_val].Type,"");

				/*ajout des fichiers fils à chaque pere*/
				sprintf(tl_str,"%s/format.ksh %d %d %d %d %s",
				C_PATH_XML,
				pl_liste_fmc[vl_val].Evt.numero,
				pl_liste_fmc[vl_val].Evt.cle,
				pl_liste_fmc[vl_val].Evt_Consequence.numero,
				pl_liste_fmc[vl_val].Evt_Consequence.cle,
				chemin_export);
				system(tl_str);
				XZST_03EcritureTrace(XZSTC_WARNING,"ajout fils %s",tl_str);
			}
		}
		//FMC virtuel coupure est exclut pour Travaux et Bouchon
		if (pl_liste_fmc[vl_val].Type != XZAEC_FMC_QueueBouchon && 
		pl_liste_fmc[vl_val].Type != XZAEC_FMC_Basculement && 
		pl_liste_fmc[vl_val].Type != XZAEC_FMC_BasculTravaux &&
		pl_liste_fmc[vl_val].Type != XZAEC_FMC_Escargot)
		{
			// Check si la route est coupee pour rajouter une FMC route coupee	
			ada2_xml_traite_route_coupee(pl_liste_fmc[vl_val],0,0);
		}
	} 
  }

  /*termine le fichier des peres*/
  for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) 
  {
    XZST_03EcritureTrace(XZSTC_FONCTION,"termine FILS %d %d de PERE %d %d",
		    pl_liste_fmc[vl_val].Evt.numero,
		    pl_liste_fmc[vl_val].Evt.cle,  
		    pl_liste_fmc[vl_val].Evt_Consequence.numero,
		    pl_liste_fmc[vl_val].Evt_Consequence.cle);

	if (pl_liste_fmc[vl_val].Evt.numero==0)
		break;

    if (!(pl_liste_fmc[vl_val].Prevision) && (pl_liste_fmc[vl_val].Bloc_Optio.Fin!='Y') && (pl_liste_fmc[vl_val].Type!=XZAEC_FMC_Autre) && (pl_liste_fmc[vl_val].Type!=XZAEC_FMC_EvtHorsReseau) ) 
	{
		if ( (pl_liste_fmc[vl_val].Evt_Consequence.numero == pl_liste_fmc[vl_val].Evt.numero) &&
			 (pl_liste_fmc[vl_val].Evt_Consequence.cle == pl_liste_fmc[vl_val].Evt.cle) )   
		{
			sprintf(tl_str,"%s/endfile.ksh %d %d %s",
				C_PATH_XML,
				pl_liste_fmc[vl_val].Evt.numero,
				pl_liste_fmc[vl_val].Evt.cle,
				chemin_export);
			XZST_03EcritureTrace(XZSTC_FONCTION,"endfile %s",tl_str);	
			system(tl_str);
       }
    }
  }

  /*A Lecture horodate courante systeme */
  XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
  XZSM_11ConversionHorodate(vl_HorodateSec, &vl_HorodateZero);

  if (sscanf( vl_HorodateZero, "%2d/%2d/%4d %2d:%2d:%2d",
			&vl_jour,
		    &vl_mois,
			&vl_annee,
			&vl_heure,
		    &vl_minute,
		    &vl_seconde) != 6) 
  {
	XZST_03EcritureTrace(XZSTC_WARNING,
	   "ada2_evt_cb_situation_recap : le format de la date est incorrecte !" );
	XZLG500_SemCall(1,XZLGC_SEM_FMC); //Déverouille sémphore
	return;
  }

  sprintf(pl_fin_fichier,"_%02d%02d%04d%02d%02d",
	  vl_jour,
	  vl_mois,		
	  vl_annee,
	  vl_heure,
	  vl_minute);

  /*genere le fichier header global*/
  ada2_xml_EVT_write(pl_liste_fmc[1],C_GEN_HEADER_EVT_GLOBAL,"glob");

  /*genere le fichier unique*/
  if (is_version_preprod())
	sprintf(tl_str,"%s/concat.ksh %s %s 1",
		C_PATH_XML,
		chemin_export,
		pl_fin_fichier);
  else
	sprintf(tl_str,"%s/concat.ksh %s %s 0",
		C_PATH_XML,
		chemin_export,
		pl_fin_fichier);

  system(tl_str);
  XZST_03EcritureTrace(XZSTC_WARNING,
		"ada2_evt_cb_situation_recap : derniere commande %s",tl_str);
 
  sprintf( tl_str,"find %s -mtime +%d -exec rm {} \\;", chemin_export, CO_NB_JOUR_PURGE);
  system(tl_str);

  /* Disable keep alive Timer */
  if (XZSM_02AnnulerProgrammation("fct_datex2_envoi_keep_alive",
					XDG_TIMER_DATEX2,
					XDM_AS_TIMER_DATEX2))
  {
  	XZST_03EcritureTrace( XZSTC_WARNING, "ada2_evt_cb_situation_recap : pb XZSM_02AnnulerProgrammation Timer KeepAlive");
  }
  else
  	XZST_03EcritureTrace( XZSTC_WARNING, "ada2_evt_cb_situation_recap : XZSM_02AnnulerProgrammation Timer KeepAlive");
	  
	   
  /* armement du timer de programmation pour keep alive messages */
  XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
  vl_HorodateSec+=vg_frequence_keep_alive;
  if (XZSM_01DemanderProgrammation(vl_HorodateSec,
				vg_frequence_keep_alive,
				XDG_TIMER_DATEX2,	
				XDM_AS_TIMER_DATEX2,
				"fct_datex2_envoi_keep_alive",
				XZSMC_SEC_VRAI) != XDC_OK) 
  {
	XZST_03EcritureTrace( XZSTC_WARNING, "ada2_evt_cb_situation_recap : pb XZSM_01DemanderProgrammation KeepAlive Timer");
  }	
  else
	XZST_03EcritureTrace( XZSTC_WARNING, "ada2_evt_cb_situation_recap : XZSM_01DemanderProgrammation : armement timer KeepAlive effectue");

  ada2_copie_fichier_vers_ftp_rep(DATA_TYPE_EVT);

  free(vg_numero_element_regroupement);
  vg_numero_element_regroupement=NULL;
  vg_size_struct_numero_regroupement=0;

  XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_evt_cb_situation_recap: OUT");
  XZLG500_SemCall(1,XZLGC_SEM_FMC); //Déverouille sémphore
  return ;
}
		
void ada2_get_radt()
{
	char tl_str[300]="";
	char pl_fin_fichier[50]="";
	int 	vl_JourSemaine;
	double 	vl_HorodateSec;
	XZSMT_Horodate  vl_HorodateZero = "";
	int vl_jour,vl_mois,vl_annee,vl_heure,vl_minute,vl_seconde;
	int vl_val=0;

	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_get_radt: IN");

	/* récupération de chaque mesure QTV et appel de ada2write_radt pour chaque mesure */
	if (XZAS91_Derniere_Donnees_RADT((XDY_FonctionInt)ada2_write_radt,&vl_val) != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_get_radt: pb dans l'appel a XZAS91");    	    
	}		
	
	/* Ecriture du header géneral global */
	ada2_xml_TDP_write(0,C_GEN_HEADER_TDP_GLOBAL,"glob");

	/*A Lecture horodate courante systeme */
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
	XZSM_11ConversionHorodate(vl_HorodateSec, &vl_HorodateZero);

	sscanf( vl_HorodateZero, "%2d/%2d/%4d %2d:%2d:%2d",
				&vl_jour,
				&vl_mois,
				&vl_annee,
				&vl_heure,
				&vl_minute,
				&vl_seconde);
	
	sprintf(pl_fin_fichier,"_%02d%02d%04d%02d%02d",
			  vl_jour,
			  vl_mois,		
			  vl_annee,
			  vl_heure,
			  vl_minute);

	sprintf(tl_str,"%s/concat.ksh %s %s 0",
		C_PATH_XML,
		chemin_export,				
		pl_fin_fichier);

	system(tl_str);

	ada2_copie_fichier_vers_ftp_rep(DATA_TYPE_RADT);

	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_get_radt: OUT");
}

void ada2_write_radt(XDY_Horodate va_horodate,
			XDY_Eqt	va_station,
			XDY_Octet va_sens,
			XDY_Mot va_Q,
			XDY_Octet va_V,
			XDY_Octet va_TO,
			XDY_Octet va_PL,		
			XDY_Octet va_validite)
{
	char vl_code_siredo_out[15]="";
	char vl_version_station_out[15]="";

	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_write_radt: IN");

	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_write_radt: station = %d, sens =%d, Q=%d, T=%d, V=%d",va_station,va_sens,va_Q,va_TO,va_V);

	vg_TDP_sens_station=va_sens;
	vg_TDP_debit=va_Q;
	vg_TDP_taux_occupation=va_TO;
	vg_TDP_vitesse=va_V;
	vg_TDP_horodate=va_horodate;
	/* récuperation du code SIREDO à partir du sens et du numero de la station */
	if (XZAE196_Lire_Code_SIREDO (va_station,
					va_sens,
					vl_code_siredo_out,
					vl_version_station_out) != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_write_radt: pb appel a XZAE196_Lire_Code_SIREDO");
		return;
	}

	//Si on trouve un code SIREDO
	if (strcmp(vl_code_siredo_out,""))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_write_radt: code SIREDO=%s, station=%d, sens=%d, version=%s",vl_code_siredo_out,va_station,va_sens,vl_version_station_out);
		strcpy(vg_TDP_code_siredo,vl_code_siredo_out);
		strcpy(vg_TDP_version_station,vl_version_station_out);

		ada2_xml_TDP_write(va_station,C_TDP_DEBIT,"Q");
		ada2_xml_TDP_write(va_station,C_TDP_TAUX_OCCUPATION,"T");
		ada2_xml_TDP_write(va_station,C_TDP_VITESSE,"V");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_write_radt: code SIREDO vide. station=%d, sens=%d",va_station,va_sens);
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_write_radt: OUT");
}

XDY_Booleen gen_a_traiter(XZLGT_Liste_FMC *pa_liste_fmc, int va_indice) 
{
	int vl_val;
    /*recherche si la FMC a une consequence en cours*/
    for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) 
	{
		if (pa_liste_fmc[vl_val].Evt.numero==0)
			break;

		if ( (pa_liste_fmc[vl_val].Evt_Consequence.numero == pa_liste_fmc[va_indice].Evt.numero)  &&
			 (pa_liste_fmc[vl_val].Evt_Consequence.cle == pa_liste_fmc[va_indice].Evt.cle) &&
			 ( !(pa_liste_fmc[vl_val].Prevision) && (pa_liste_fmc[vl_val].Bloc_Optio.Fin!='Y')) ) 
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "consequence %d/%d de %d/%d en COURS",
				pa_liste_fmc[vl_val].Evt.numero,
				pa_liste_fmc[vl_val].Evt.cle,
				pa_liste_fmc[va_indice].Evt.numero,
				pa_liste_fmc[va_indice].Evt.cle);
			return (TRUE);
		}
    }

    /*si je suis ici, pas de consequence en cours*/
    /*donc je regarde juste si je suis en cours moi meme*/		
    if (!(pa_liste_fmc[va_indice].Prevision) && (pa_liste_fmc[va_indice].Bloc_Optio.Fin!='Y')) 
	{
		return (TRUE);
    }
return(FALSE);

}

/* Fichier : @(#)ada2_evt.c     1.0        Release : 1.0       Date : 01/04/20
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE ada2_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* recherche du pere de la FMC
*
******************************************************
* SEQUENCE D'APPEL :
* gen_recherche_pere();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
XDY_Evt gen_recherche_pere(XZLGT_Liste_FMC *pa_liste_fmc, int va_indice)
{
	int vl_index=-1;
	int vl_val;
	int vl_trouve=-1;
	XDY_Evt vl_evt;

	XZST_03EcritureTrace( XZSTC_WARNING, "recherche pere de la FMC %d %d (cause %d %d )",
						pa_liste_fmc[va_indice].Evt.numero,
						pa_liste_fmc[va_indice].Evt.cle,
						pa_liste_fmc[va_indice].Evt_Cause.numero,
						pa_liste_fmc[va_indice].Evt_Cause.cle);

	vl_evt.numero = pa_liste_fmc[va_indice].Evt.numero;
	vl_evt.cle = pa_liste_fmc[va_indice].Evt.cle;

	if ( (pa_liste_fmc[va_indice].Evt_Cause.numero > 0) &&
		( (pa_liste_fmc[va_indice].Evt.numero != pa_liste_fmc[va_indice].Evt_Cause.numero)  ||
		(pa_liste_fmc[va_indice].Evt.cle != pa_liste_fmc[va_indice].Evt_Cause.cle) ) ) 
	{
		/*recherche de l index de la cause dans la liste des FMC*/
		for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) 
		{		
			if (pa_liste_fmc[vl_val].Evt.numero==0)
				break;
			if ( (pa_liste_fmc[vl_val].Evt.numero == pa_liste_fmc[va_indice].Evt_Cause.numero)  &&
			     (pa_liste_fmc[vl_val].Evt.cle == pa_liste_fmc[va_indice].Evt_Cause.cle) && 
			     (! (pa_liste_fmc[vl_val].Prevision) && (pa_liste_fmc[vl_val].Bloc_Optio.Fin!='Y')) )
			{
			XZST_03EcritureTrace( XZSTC_WARNING, "le pere avant verif est %d %d",
					pa_liste_fmc[vl_val].Evt.numero,
					pa_liste_fmc[vl_val].Evt.cle);
			vl_trouve = vl_val;
			}
			
			/*on recherche maintenant le pere de la cause*/
			//XZST_03EcritureTrace( XZSTC_DEBUG1, "trouve %d",vl_trouve);
			if (vl_trouve!=-1) 
			{
				vl_evt = gen_recherche_pere(pa_liste_fmc, vl_trouve);
				break;
			}
		}
	}
	else /*pas de cause, je suis donc le pere originel*/
	{ 
		vl_evt.numero = pa_liste_fmc[va_indice].Evt.numero;
		vl_evt.cle = pa_liste_fmc[va_indice].Evt.cle;
	}																

	XZST_03EcritureTrace( XZSTC_DEBUG1, "le pere apres verif est %d %d",vl_evt.numero, vl_evt.cle);
	return (vl_evt);
}
				
int desherite_FMC(XZLGT_Liste_FMC *pa_liste_fmc, int va_indice)
{
	int vl_val;
	XDY_Evt vl_evt;
	
	vl_evt.numero = pa_liste_fmc[va_indice].Evt.numero;
	vl_evt.cle = pa_liste_fmc[va_indice].Evt.cle;

	/*recherche de l index de la cause dans la liste des FMC*/
	for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) 
	{
		if ( (pa_liste_fmc[vl_val].Evt.numero == pa_liste_fmc[va_indice].Evt_Consequence.numero)  &&
			(pa_liste_fmc[vl_val].Evt.cle == pa_liste_fmc[va_indice].Evt_Consequence.cle) &&
			( (pa_liste_fmc[vl_val].Type==XZAEC_FMC_Autre) ||				
			  (pa_liste_fmc[vl_val].Type==XZAEC_FMC_EvtHorsReseau) ) )
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "desherite_FMC : FMC Pere: %d/%d type=%d desherite fils : %d/%d",
								pa_liste_fmc[vl_val].Evt.numero,
								pa_liste_fmc[vl_val].Evt.cle,
								pa_liste_fmc[vl_val].Type,
								vl_evt.numero,
								vl_evt.cle);
			return 1;
		}
	}
	return 0;				
}

XZLGT_Liste_FMC gen_recherche_fils(XZLGT_Liste_FMC *pa_liste_fmc, int va_indice)
{
	int vl_index=-1;
	int vl_val;
	int vl_trouve=-1;
	XZLGT_Liste_FMC vl_evt;

	vl_evt= pa_liste_fmc[va_indice];

		/*recherche de l index de la cause dans la liste des FMC*/
		for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) 
		{		
			if (pa_liste_fmc[vl_val].Evt.numero==0)
				break;
			if ( (pa_liste_fmc[vl_val].Evt_Cause.numero == vl_evt.Evt.numero)  &&
			     (pa_liste_fmc[vl_val].Evt_Cause.cle == vl_evt.Evt.cle) && 
			     (! (pa_liste_fmc[vl_val].Prevision) && (pa_liste_fmc[vl_val].Bloc_Optio.Fin!='Y')) )
			{
				vl_evt = pa_liste_fmc[vl_val];
				XZST_03EcritureTrace( XZSTC_WARNING, "le fils de %d %d est %d %d",
					vl_evt.Evt.numero,
					vl_evt.Evt.cle,
					pa_liste_fmc[va_indice].Evt_Cause.numero,
					pa_liste_fmc[va_indice].Evt_Cause.cle);
					break;
			}
		}
	return (vl_evt);
}

void ada2_xml_init2(char *va_fichier, int va_indice)
{
 FILE *pl_fp;
 int vl_retcode=0;
 int vl_i;
 char        vl_Lig[255];
 XDY_Entier vl_Resultat_out = 0;
 char  *vl_str;
 char *vl_ch;
 T_NOEUD *vl_noeud;
 T_NOEUD *vl_noeud2;
 T_NOEUD *vl_noeud3;
 T_NOEUD *vl_noeud4;
 T_NOEUD *vl_noeud5;
 char	vl_pere[100];
 T_LISTE	*vl_liste;
 T_LISTE	*vl_liste2;
 T_LISTE	*vl_liste3;

 vl_noeud = (T_NOEUD *)malloc(sizeof(T_NOEUD));
  
 if ( (pl_fp = fopen(va_fichier, "r")) == NULL ) {
	 return;
 }
 while( (vl_retcode = XZAO000_LireLigneFich(pl_fp, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out) ) != XDC_NOK)
 {
    if (vl_Resultat_out != XZAO_EOF )
    {
		vl_str=vl_Lig;
		vl_str[strlen(vl_str)-1]='\0';
		vl_ch=strtok(vl_str,C_PIPE);

		/*elt 1 = libelle*/
		strcpy(vl_noeud->Libelle, vl_ch);

		/*elt 1 prime = baliseXML*/
		vl_ch=strtok(NULL,C_PIPE);
		strcpy(vl_noeud->BaliseXML, vl_ch);

		vl_ch=strtok(NULL,C_PIPE);
		/* elt2 = noeud pere*/
		strcpy(vl_pere,vl_ch);

		vl_ch=strtok(NULL,C_PIPE);
		/*elt 3 = type de noeud*/
		if (!strcmp(vl_ch,"M"))
		  vl_noeud->Type=C_MODULE;
		else if (!strcmp(vl_ch,"AG"))
		  vl_noeud->Type=C_ATTRIBUT_GEN;
		else if (!strcmp(vl_ch,"AS"))
		  vl_noeud->Type=C_ATTRIBUT_SPEC;
		else
		  vl_noeud->Type=C_ATTRIBUT_GEN;

		vl_ch=strtok(NULL,C_PIPE);
		/*elt 4 = valeur*/
		strcpy(vl_noeud->Valeur, vl_ch);
		vl_noeud->TypeValeur=C_VALEUR_DIRECTE;

		/*si la valeur commence par fmc." */
		/*alors c est une fonction Predicat */

		if (!strncmp(vl_noeud->Valeur,C_BALISE_PREFIXE,strlen(C_BALISE_PREFIXE)))
		{
		  vl_noeud->TypeValeur=C_VALEUR_PREDICAT;
		  if (!strcmp(vl_noeud->Valeur, C_BALISE_DATE_COURANTE)) vl_noeud->Predicat.Fct=balise_date_courante;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_ID)) vl_noeud->Predicat.Fct=balise_id;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_ID_EVT)) vl_noeud->Predicat.Fct=balise_id_evt;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_TYPE)) vl_noeud->Predicat.Fct=balise_type;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_ZZ)) vl_noeud->Predicat.Fct=balise_zz;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_QUALIF)) vl_noeud->Predicat.Fct=balise_qualif;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_QIN)) vl_noeud->Predicat.Fct=balise_qin;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_DATE_DEBUT)) vl_noeud->Predicat.Fct=balise_date_debut;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_DATE_FIN)) vl_noeud->Predicat.Fct=balise_date_fin;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_DATE_DEBUT_PREVU)) vl_noeud->Predicat.Fct=balise_date_debut_prevu;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_DATE_FIN_PREVU)) vl_noeud->Predicat.Fct=balise_date_fin_prevu;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_NB_BLOQ)) vl_noeud->Predicat.Fct=balise_nb_bloq;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_NB_OUV)) vl_noeud->Predicat.Fct=balise_nb_ouv;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_NB_TOT)) vl_noeud->Predicat.Fct=balise_nb_tot;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_COMMENT)) vl_noeud->Predicat.Fct=balise_comment;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_L01)) vl_noeud->Predicat.Fct=balise_l01;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_L02)) vl_noeud->Predicat.Fct=balise_l02;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_LTV)) vl_noeud->Predicat.Fct=balise_ltv;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_L05)) vl_noeud->Predicat.Fct=balise_l05;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_L07)) vl_noeud->Predicat.Fct=balise_l07;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_L04)) vl_noeud->Predicat.Fct=balise_l04;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_L06)) vl_noeud->Predicat.Fct=balise_l06;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_L09)) vl_noeud->Predicat.Fct=balise_l09;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_L10)) vl_noeud->Predicat.Fct=balise_l10;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_L11)) vl_noeud->Predicat.Fct=balise_l11;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_L12)) vl_noeud->Predicat.Fct=balise_l12;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_CANCEL)) vl_noeud->Predicat.Fct=balise_cancel;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_END)) vl_noeud->Predicat.Fct=balise_end;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_CARRIAGE)) vl_noeud->Predicat.Fct=balise_carriage;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_LOCATION)) vl_noeud->Predicat.Fct=balise_location;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_CAUSE)) vl_noeud->Predicat.Fct=balise_cause;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_LANES)) vl_noeud->Predicat.Fct=balise_lanes;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_RPIP)) vl_noeud->Predicat.Fct=balise_rpip;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_RNP)) vl_noeud->Predicat.Fct=balise_rnp;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_DRP)) vl_noeud->Predicat.Fct=balise_drp;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_DFPP)) vl_noeud->Predicat.Fct=balise_dfpp;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_RPIS)) vl_noeud->Predicat.Fct=balise_rpis;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_RNS)) vl_noeud->Predicat.Fct=balise_rns;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_DRS)) vl_noeud->Predicat.Fct=balise_drs;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_DFPS)) vl_noeud->Predicat.Fct=balise_dfps;

		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_NB_VL)) vl_noeud->Predicat.Fct=balise_nb_vl;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_NB_BUS)) vl_noeud->Predicat.Fct=balise_nb_bus;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_NB_PL)) vl_noeud->Predicat.Fct=balise_nb_pl;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_NB_REM)) vl_noeud->Predicat.Fct=balise_nb_rem;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_NB_MOTOS)) vl_noeud->Predicat.Fct=balise_nb_motos;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_BLESSES)) vl_noeud->Predicat.Fct=balise_blesses;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_MORTS)) vl_noeud->Predicat.Fct=balise_morts;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_CHARGEMENT)) vl_noeud->Predicat.Fct=balise_chargement;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_MATIERE)) vl_noeud->Predicat.Fct=balise_matiere;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_VIP)) vl_noeud->Predicat.Fct=balise_vip;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_ESCOTA)) vl_noeud->Predicat.Fct=balise_escota;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_TOTAL_VEH)) vl_noeud->Predicat.Fct=balise_total_veh;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_TOTAL_PEOPLE)) vl_noeud->Predicat.Fct=balise_total_people;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_FERM_ENTREES)) vl_noeud->Predicat.Fct=balise_entrees_fermeture;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_FERM_SORTIES)) vl_noeud->Predicat.Fct=balise_sorties_fermeture;  
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_DECON_ENTREES)) vl_noeud->Predicat.Fct=balise_entrees_deconseille;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_DECON_SORTIES)) vl_noeud->Predicat.Fct=balise_sorties_deconseille;    

		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_BASCUL_LONGUEUR)) vl_noeud->Predicat.Fct=balise_bascul_longueur;

		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_BOUCHON_LONGUEUR)) vl_noeud->Predicat.Fct=balise_bouchon_longueur;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_BOUCHON_TYPE)) vl_noeud->Predicat.Fct=balise_bouchon_type;

		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_VEHL_LONGUEUR)) vl_noeud->Predicat.Fct=balise_vehl_longueur;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_VEHL_LARGEUR)) vl_noeud->Predicat.Fct=balise_vehl_largeur;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_VEHL_POIDS)) vl_noeud->Predicat.Fct=balise_vehl_poids;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_VEHL_HAUTEUR)) vl_noeud->Predicat.Fct=balise_vehl_hauteur;

		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_ITINERAIRE)) vl_noeud->Predicat.Fct=balise_itineraire;

		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_FERMETURE)) vl_noeud->Predicat.Fct=balise_fermeture;

		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_PENURIE)) vl_noeud->Predicat.Fct=balise_penurie;

		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_VISIBILITE)) vl_noeud->Predicat.Fct=balise_visibilite;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_METEO_TYPE)) vl_noeud->Predicat.Fct=balise_meteo_type;

		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_TRAVAUX_TYPE)) vl_noeud->Predicat.Fct=balise_travaux_type;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_MOBILE)) vl_noeud->Predicat.Fct=balise_mobile;

		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_STOCKAGE_DEMARRE)) vl_noeud->Predicat.Fct=balise_stockage_demarre;

		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_GLISSANTE)) vl_noeud->Predicat.Fct=balise_glissante;

		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_VEH_MATIERE)) vl_noeud->Predicat.Fct=balise_veh_matiere;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_VEH_CHARGEMENT)) vl_noeud->Predicat.Fct=balise_veh_chargement;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_VEH_TYPE)) vl_noeud->Predicat.Fct=balise_veh_type;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_VITESSE)) vl_noeud->Predicat.Fct=balise_vitesse;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_HORODATE_DEBUT_TACHE)) vl_noeud->Predicat.Fct=balise_horodate_debut_tache;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_OPERATING_MODE)) vl_noeud->Predicat.Fct=balise_operating_mode;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_PUBLICATION_TYPE)) vl_noeud->Predicat.Fct=balise_publication_type;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_ID_REGROUPEMENT)) vl_noeud->Predicat.Fct=balise_id_regroupement;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_VERSION_REGROUPEMENT)) vl_noeud->Predicat.Fct=balise_version_regroupement;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_GRAVITE)) vl_noeud->Predicat.Fct=balise_gravite;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_ID_REGROUPEMENT_CAUSE_CHANTIER)) vl_noeud->Predicat.Fct=balise_id_regroupement_cause_chantier;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_VERSION_ID_REGROUPEMENT_CAUSE_CHANTIER)) vl_noeud->Predicat.Fct=balise_version_id_regroupement_cause_chantier;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_RELATED_SITUATION_TARGETCLASS)) vl_noeud->Predicat.Fct=balise_related_situation_targetClass;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_ID_ELEMENT)) vl_noeud->Predicat.Fct=balise_id_element;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_IMPACT_VOIE)) vl_noeud->Predicat.Fct=balise_impact_voie;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_LONGUEUR_IMPACT)) vl_noeud->Predicat.Fct=balise_longueur_impact;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_SENS)) vl_noeud->Predicat.Fct=balise_sens;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_GROUPE_OF_LOCATION_TYPE)) vl_noeud->Predicat.Fct=balise_groupe_of_location_type;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_DATE_VALIDATION)) vl_noeud->Predicat.Fct=balise_date_validation;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_TDP_SIREDO)) vl_noeud->Predicat.Fct=balise_TDP_siredo;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_TDP_VERSION_PMS)) vl_noeud->Predicat.Fct=balise_TDP_version_PMS;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_TDP_HORODATE)) vl_noeud->Predicat.Fct=balise_TDP_horodate;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_TDP_DEBIT)) vl_noeud->Predicat.Fct=balise_TDP_debit;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_TDP_TAUX_OCCUPATION)) vl_noeud->Predicat.Fct=balise_TDP_taux_occupation;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_TDP_VITESSE)) vl_noeud->Predicat.Fct=balise_TDP_vitesse;
		  else if (!strcmp(vl_noeud->Valeur, C_BALISE_TDP_SENS_STATION)) vl_noeud->Predicat.Fct=balise_TDP_sens_station;
		  else {
			  XZST_03EcritureTrace(XZSTC_WARNING,"ada2_xml_init2: BALISE NON TROUVEE !!! Noeud.Valeur = %s",vl_noeud->Valeur);
		  }
		}


		vl_ch=strtok(NULL,C_PIPE);
		/*elt 5 = sous attribut ou pas*/
		if (!strcmp(vl_ch,"="))
		  vl_noeud->SousAttribut=1;
		else if (!strcmp(vl_ch,"opt"))
		  vl_noeud->SousAttribut=-1;
		else
		  vl_noeud->SousAttribut=0;

		vl_ch=strtok(NULL,C_PIPE);
		/*elt 6 : valeur a tester ou pas*/
		if ((vl_ch!=NULL) && (!strncmp(vl_ch,C_BALISE_PREFIXE,strlen(C_BALISE_PREFIXE)))) 
		{
		  if (!strcmp(vl_ch, C_BALISE_TEST_CAUSE)) vl_noeud->Predicat_Test.Fct=balise_test_cause;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_BUS)) vl_noeud->Predicat_Test.Fct=balise_test_bus;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_CHARGEMENT)) vl_noeud->Predicat_Test.Fct=balise_test_chargement;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_MOTOS)) vl_noeud->Predicat_Test.Fct=balise_test_motos;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_BLESSES)) vl_noeud->Predicat_Test.Fct=balise_test_blesses;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_PL)) vl_noeud->Predicat_Test.Fct=balise_test_pl;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_REM)) vl_noeud->Predicat_Test.Fct=balise_test_rem;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_MORTS)) vl_noeud->Predicat_Test.Fct=balise_test_morts;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_VL)) vl_noeud->Predicat_Test.Fct=balise_test_vl;
		  else if (!strcmp(vl_ch, C_BALISE_VEHL_TEST_POIDS)) vl_noeud->Predicat_Test.Fct=balise_vehl_test_poids;
		  else if (!strcmp(vl_ch, C_BALISE_VEHL_TEST_LONGUEUR)) vl_noeud->Predicat_Test.Fct=balise_vehl_test_longueur;
		  else if (!strcmp(vl_ch, C_BALISE_VEHL_TEST_LARGEUR)) vl_noeud->Predicat_Test.Fct=balise_vehl_test_largeur;
		  else if (!strcmp(vl_ch, C_BALISE_VEHL_TEST_HAUTEUR)) vl_noeud->Predicat_Test.Fct=balise_vehl_test_hauteur;
		  else if (!strcmp(vl_ch, C_BALISE_ESCOTA)) vl_noeud->Predicat_Test.Fct=balise_escota;
		  else if (!strcmp(vl_ch, C_BALISE_TOTAL_VEH)) vl_noeud->Predicat_Test.Fct=balise_total_veh;
		  else if (!strcmp(vl_ch, C_BALISE_TOTAL_PEOPLE)) vl_noeud->Predicat_Test.Fct=balise_total_people;
		  else if (!strcmp(vl_ch, C_BALISE_VEH_CHARGEMENT)) vl_noeud->Predicat_Test.Fct=balise_veh_chargement;
		  else if (!strcmp(vl_ch, C_BALISE_VIP)) vl_noeud->Predicat_Test.Fct=balise_vip;
		  else if (!strcmp(vl_ch, C_BALISE_COMMENT)) vl_noeud->Predicat_Test.Fct=balise_comment;
		  else if (!strcmp(vl_ch, C_BALISE_FERM_ENTREES)) vl_noeud->Predicat_Test.Fct=balise_entrees_fermeture;
		  else if (!strcmp(vl_ch, C_BALISE_FERM_SORTIES)) vl_noeud->Predicat_Test.Fct=balise_sorties_fermeture;
		  else if (!strcmp(vl_ch, C_BALISE_DECON_ENTREES)) vl_noeud->Predicat_Test.Fct=balise_entrees_deconseille;
		  else if (!strcmp(vl_ch, C_BALISE_DECON_SORTIES)) vl_noeud->Predicat_Test.Fct=balise_sorties_deconseille;
		  else if (!strcmp(vl_ch, C_BALISE_VEH_MATIERE)) vl_noeud->Predicat_Test.Fct=balise_veh_matiere;
		  else if (!strcmp(vl_ch, C_BALISE_BASC_VITESSE)) vl_noeud->Predicat_Test.Fct=balise_basc_vitesse;
		  else if (!strcmp(vl_ch, C_BALISE_BASC_VITESSE_OPPOSEE)) vl_noeud->Predicat_Test.Fct=balise_basc_vitesse_opposee;
		  else if (!strcmp(vl_ch, C_BALISE_TRV_VITESSE)) vl_noeud->Predicat_Test.Fct=balise_trv_vitesse;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_BRETELLE)) vl_noeud->Predicat_Test.Fct=balise_test_bretelle;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_CHANTIER)) vl_noeud->Predicat_Test.Fct=balise_test_chantier;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_LANE_RESTRICTED)) vl_noeud->Predicat_Test.Fct=balise_test_lane_restricted;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_PUBLICATION_TYPE_RECAP)) vl_noeud->Predicat_Test.Fct=balise_test_feedType_recap;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_PUBLICATION_TYPE_COUPPARCOUP)) vl_noeud->Predicat_Test.Fct=balise_test_feedType_coupparcoup;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_GROUPE_OF_LOCATION_LINEAR)) vl_noeud->Predicat_Test.Fct=balise_test_groupe_of_location_linear;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_GROUPE_OF_LOCATION_POINT)) vl_noeud->Predicat_Test.Fct=balise_test_groupe_of_location_point;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_ACCIDENT)) vl_noeud->Predicat_Test.Fct=balise_test_accident;
		  else if (!strcmp(vl_ch, C_BALISE_GLISSANTE_METEO)) vl_noeud->Predicat_Test.Fct=balise_test_glissante_meteo;
		  else if (!strcmp(vl_ch, C_BALISE_GLISSANTE_NON_METEO)) vl_noeud->Predicat_Test.Fct=balise_test_glissante_non_meteo;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_BOUCHON_ACCORDEON)) vl_noeud->Predicat_Test.Fct=balise_test_bouchon_accordeon;
		  else if (!strcmp(vl_ch, C_BALISE_OBSTACLE_ANIMALE)) vl_noeud->Predicat_Test.Fct=balise_test_obstacle_animale;
		  else if (!strcmp(vl_ch, C_BALISE_TEST_DATE_FIN_PREVU)) vl_noeud->Predicat_Test.Fct=balise_test_date_fin_prevu;


		  else {
			  XZST_03EcritureTrace(XZSTC_WARNING,"ada2_xml_init2: BALISE NON TROUVEE !!! %s",vl_ch);
		   }
		}
		else  
		{
		  vl_noeud->Predicat_Test.Fct = balise_test_true;
		}

		vl_noeud->Attribut=NULL;
		vl_noeud->FilsAine = NULL;
		vl_noeud->FrereCadet = NULL;
		vl_noeud->Pere=NULL;

		if (vg_arbre[va_indice]==NULL) 
		{	/*1° insertion*/
			vg_arbre[va_indice]=(T_NOEUD *)malloc(sizeof(T_NOEUD));
			vg_arbre[va_indice]->Attribut=NULL;
			vg_arbre[va_indice]->FilsAine = NULL;
			vg_arbre[va_indice]->FrereCadet = NULL;
			vg_arbre[va_indice]->Pere=NULL;
			vg_arbre[va_indice]->Type=vl_noeud->Type;
			vg_arbre[va_indice]->TypeValeur=vl_noeud->TypeValeur;
			vg_arbre[va_indice]->Generique=vl_noeud->Generique;
			vg_arbre[va_indice]->Predicat=vl_noeud->Predicat;
			vg_arbre[va_indice]->Predicat_Test=vl_noeud->Predicat_Test;
			vg_arbre[va_indice]->SousAttribut=vl_noeud->SousAttribut;
			strcpy(vg_arbre[va_indice]->Valeur,vl_noeud->Valeur);
			strcpy(vg_arbre[va_indice]->Libelle,vl_noeud->Libelle);
			strcpy(vg_arbre[va_indice]->BaliseXML,vl_noeud->BaliseXML);
			vl_noeud2 = vg_arbre[va_indice];

			/*c est forcement un noeud principal = un PERE*/
			vg_peres = (T_LISTE *)malloc(sizeof(T_LISTE));
			strcpy(vg_peres->LibellePere, vg_arbre[va_indice]->Libelle);
			vg_peres->PositionArbre = vg_arbre[va_indice];
			vg_peres->Suivant = NULL;
		}
		else 
		{
			if ( (vl_noeud->Type==C_MODULE) || (vl_noeud->Type==C_ATTRIBUT_SPEC)) 
			{ /*c'est un noeud principal*/
			  /*je l insere dans la liste des peres*/
				vl_liste=vg_peres;
				while (vl_liste->Suivant!=NULL)  {
					vl_liste=vl_liste->Suivant;
				}
				vl_liste->Suivant = (T_LISTE *)malloc(sizeof(T_LISTE));
				vl_liste=vl_liste->Suivant;
				strcpy(vl_liste->LibellePere,vl_noeud->Libelle);
				vl_liste->PositionArbre=NULL;
				vl_liste->Suivant=NULL;

				/*je cherche son pere dans l arbre*/
				vl_liste2=vg_peres;
				while (vl_liste2!=NULL) 
				{
				  if (!strcmp(vl_liste2->LibellePere,vl_pere)) 
				  {
					vl_noeud4 = vl_liste2->PositionArbre;
					break;
				  }
					  vl_liste2=vl_liste2->Suivant;	
				}
				if (vl_liste2==NULL) 
				{
				  XZST_03EcritureTrace(XZSTC_WARNING,"pere %s de %s non trouve\n",vl_pere,vl_noeud->Libelle);
				  return;
				}

				/*je le mets au bon endroit dans l arbre*/
				/*je me place sur le pere*/
				if (vl_noeud4->FilsAine == NULL) 
				{ /*premier fils*/
				  vl_noeud4->FilsAine = (T_NOEUD *)malloc(sizeof(T_NOEUD));
				  strcpy(vl_noeud4->FilsAine->Libelle,vl_noeud->Libelle);
				  strcpy(vl_noeud4->FilsAine->BaliseXML,vl_noeud->BaliseXML);
				  vl_noeud4->FilsAine->Type=vl_noeud->Type;
				  vl_noeud4->FilsAine->Predicat=vl_noeud->Predicat;
				  vl_noeud4->FilsAine->Predicat_Test=vl_noeud->Predicat_Test;
				  vl_noeud4->FilsAine->TypeValeur=vl_noeud->TypeValeur;
				  vl_noeud4->FilsAine->Generique=vl_noeud->Generique;
				  vl_noeud4->FilsAine->SousAttribut=vl_noeud->SousAttribut;
				  vl_noeud4->FilsAine->Attribut=NULL;
				  strcpy(vl_noeud4->FilsAine->Valeur,vl_noeud->Valeur);
				  vl_noeud4->FilsAine->Pere = vl_noeud4;
				  vl_noeud4->FilsAine->FrereCadet = NULL;
				  vl_noeud4->FilsAine->FilsAine=NULL;
				  vl_liste->PositionArbre = vl_noeud4->FilsAine;
				}
				else 
				{ /*N° fils : c est donc un frere du fils aine*/
				  vl_noeud5 = vl_noeud4->FilsAine;
				  while (vl_noeud5->FrereCadet != NULL) {
					  vl_noeud5=vl_noeud5->FrereCadet;
				  }
				  vl_noeud5->FrereCadet = (T_NOEUD *)malloc(sizeof(T_NOEUD));
				  strcpy(vl_noeud5->FrereCadet->Libelle,vl_noeud->Libelle);
				  strcpy(vl_noeud5->FrereCadet->BaliseXML,vl_noeud->BaliseXML);
				  vl_noeud5->FrereCadet->Type=vl_noeud->Type;
				  vl_noeud5->FrereCadet->Predicat_Test=vl_noeud->Predicat_Test;
				  vl_noeud5->FrereCadet->Predicat=vl_noeud->Predicat;
				  vl_noeud5->FrereCadet->TypeValeur=vl_noeud->TypeValeur;
				  vl_noeud5->FrereCadet->Generique=vl_noeud->Generique;
				  vl_noeud5->FrereCadet->SousAttribut=vl_noeud->SousAttribut;
				  vl_noeud5->FrereCadet->Attribut=NULL;
				  vl_noeud5->FrereCadet->FilsAine=NULL;
				  vl_noeud5->FrereCadet->FrereCadet=NULL;
				  strcpy(vl_noeud5->FrereCadet->Valeur,vl_noeud->Valeur);
				  vl_noeud5->FrereCadet->Pere = vl_noeud4;
				  vl_liste->PositionArbre = vl_noeud5->FrereCadet;
				}
			}
			else 
			{ 	/*c'est un attribut*/
				/*je cherche son pere dans l arbre*/
				vl_liste2=vg_peres;
				while (vl_liste2!=NULL) 
				{
					if (!strcmp(vl_liste2->LibellePere,vl_pere)) 
					{
						vl_noeud4 = vl_liste2->PositionArbre;
						break;
					}
					vl_liste2=vl_liste2->Suivant;
				}
				if (vl_liste2==NULL) 
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"pere %s de %s non trouve\n",vl_pere,vl_noeud->Libelle);
					return;
				}
					
				/*printf("pere %s de %s trouve\n",vl_pere,vl_noeud->Libelle);*/

				/*si 1° insertion*/
				if (vl_noeud4->Attribut==NULL) {
				  vl_noeud->Attribut=NULL;
				  vl_noeud4->Attribut = (T_NOEUD *)malloc(sizeof(T_NOEUD));
				  vl_noeud4 = vl_noeud4->Attribut;
				  strcpy(vl_noeud4->Libelle,vl_noeud->Libelle);
				  strcpy(vl_noeud4->BaliseXML,vl_noeud->BaliseXML);
				  vl_noeud4->Type=vl_noeud->Type;
				  vl_noeud4->Predicat=vl_noeud->Predicat;
				  vl_noeud4->Predicat_Test=vl_noeud->Predicat_Test;
				  vl_noeud4->TypeValeur=vl_noeud->TypeValeur;
				  vl_noeud4->Generique=vl_noeud->Generique;
				  vl_noeud4->SousAttribut=vl_noeud->SousAttribut;
				  strcpy(vl_noeud4->Valeur,vl_noeud->Valeur);
				  vl_noeud4->Pere = vl_liste2->PositionArbre;
				  vl_noeud4->Attribut = NULL;
				}
				else 
				{
				  /*je l insere en fin de liste des attributs sur mon noeud principal courant*/
				  vl_noeud3 = vl_noeud4;
				  while (vl_noeud3->Attribut != NULL)
					vl_noeud3 = vl_noeud3->Attribut;
				  vl_noeud->Attribut=NULL;
				  vl_noeud3->Attribut = (T_NOEUD *)malloc(sizeof(T_NOEUD));
				  strcpy(vl_noeud3->Attribut->Libelle,vl_noeud->Libelle);
				  strcpy(vl_noeud3->Attribut->BaliseXML,vl_noeud->BaliseXML);
				  vl_noeud3->Attribut->Type=vl_noeud->Type;
				  vl_noeud3->Attribut->Predicat=vl_noeud->Predicat;
				  vl_noeud3->Attribut->Predicat_Test=vl_noeud->Predicat_Test;
				  vl_noeud3->Attribut->TypeValeur=vl_noeud->TypeValeur;
				  vl_noeud3->Attribut->Generique=vl_noeud->Generique;
				  vl_noeud3->Attribut->SousAttribut=vl_noeud->SousAttribut;
				  strcpy(vl_noeud3->Attribut->Valeur,vl_noeud->Valeur);
				  vl_noeud3->Attribut->Pere = vl_noeud4;
				  vl_noeud3->Attribut->Attribut = NULL;
				
				}
			}
		}
	}
 else
	break;
 }
 fclose(pl_fp);

}

int ada2_xml_init()
{
int vl_i;
char vl_cmd[300];
XZSMT_CleProg           vl_cle;
XZSCT_Datagroup         vl_Datagroup,vl_dg ;
int                     vl_JourSemaine;
double                  vl_HorodateSec;
XZSCT_NomMachine        pl_NomMachine   = "\0";
FILE					*pl_fp;
char                    pl_ligne[200];
char                    pl_param[200];
XDY_Entier              vl_val;
int i =0;
int EVT=0;
int RADT =0;
int PMV=0;
int PATR=0;
int                   vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
XZSMT_Horodate        vl_horo_chaine;
XDY_Horodate		vl_date;
int			vl_Jour_De_La_Semaine;

/* lecture horodate pour renseigner horodate début tache */
XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_Jour_De_La_Semaine,&vl_date);
XZSM_11ConversionHorodate(vl_date,&vl_horo_chaine);
sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d:%2d",
			&vl_annee,
			&vl_mois,
			&vl_jour,
			&vl_heure,
			&vl_minute,
			&vl_seconde);
  sprintf(vg_horodate_debut_tache,"%04d-%02d-%02dT%02d:%02d:%02d+%02d:00",
		vl_jour,
		vl_mois,
		vl_annee,
		vl_heure,
		vl_minute,
		vl_seconde,
		get_local_UTC_hour_diff(vl_date));

	XZST_03EcritureTrace(  XZSTC_FONCTION,"ada2_xml_init : IN");	

	/* Recuperation du nom de la machine locale */
	XZSC_07NomMachine(pl_NomMachine) ;

	/* Lecture du fichier des destinataires */
	if ( (pl_fp = fopen(CO_FIC_DESTINATAIRE, "r")) == NULL ) {
		XZST_03EcritureTrace(   XZSTC_WARNING, "erreur de lecture du fichier %s !"
								,CO_FIC_DESTINATAIRE );
		return(XDC_NOK);
	}

	/* On lit les lignes du fichier une par une */
	/* ------------------------------------------------------ */
	while ( fgets(pl_ligne, 200, pl_fp) != NULL) 
	{
		/* On ignore les lignes commencant par un # */
		if ( pl_ligne[0] == '#' )
			continue;

		/* Pour les autres lignes on recherche la config de chaque destinataire : Nom typedetransmission chemin/Url EVT RADT PMV PATR*/
		if (sscanf(pl_ligne,"%s\t%d\t%s\t%d\t%d\t%d\t", vg_destinataire[i].Nom, &vg_destinataire[i].TypeTransmission, vg_destinataire[i].URLWebService_PathFTP, &EVT, &RADT, &PMV)==6)		
		{
			XZST_03EcritureTrace(  XZSTC_WARNING,"EVT=%d, RADT=%d, PMV=%d",EVT, RADT, PMV);
			vg_destinataire[i].DataTypeVoulu[DATA_TYPE_EVT] = EVT;
			vg_destinataire[i].DataTypeVoulu[DATA_TYPE_RADT] = RADT;
			vg_destinataire[i].DataTypeVoulu[DATA_TYPE_PMV] = PMV;
			XZST_03EcritureTrace(XZSTC_WARNING, "Ajout destinataire %s:\n TypeTransmission=%d\n URLWebService_PathFTP=%s\n DataTypeVoulu -> EVT=%d, RADT=%d, PMV=%d, PATR=%d", 
						vg_destinataire[i].Nom,
						vg_destinataire[i].TypeTransmission,
						vg_destinataire[i].URLWebService_PathFTP,
						vg_destinataire[i].DataTypeVoulu[DATA_TYPE_EVT],
						vg_destinataire[i].DataTypeVoulu[DATA_TYPE_RADT],
						vg_destinataire[i].DataTypeVoulu[DATA_TYPE_PMV]);
			vg_nbrDestinataire++;
			i++;
		}
		else 
		{		
			XZST_03EcritureTrace(   XZSTC_WARNING, "le fichier %s n'a pas le format demande !"
									,CO_FIC_DESTINATAIRE );
		}
	}
	fclose(pl_fp);

/* verifie si nous sommes en preprod ou en prod*/
	if (is_version_preprod())
		strcpy( chemin_export, C_PATH_PREPROD);
	else
		strcpy( chemin_export, C_PATH);
			
 /* créer les structure xml pour chaque type de FMC*/
  vg_arbre[0]=NULL;
  for (vl_i=1;vl_i<=C_DATEX2_FMC_NB_TYPE;vl_i++) {
    vg_arbre[vl_i]=NULL;
    /* diferentiation TDP et EVT */
    if (vl_i<=C_DATEX2_FMC_NB_TYPE-C_NB_TDP)
    {
	    sprintf(vl_cmd,"cat %s %s > %s/tempo.xml",
			C_XML_GEN_PERE, /* C_XML_GEN_EVT*/
			tg_types_fmc[vl_i-1].XMLFile,
			C_PATH_XML);
    }
    else
    {
	    sprintf(vl_cmd,"cat %s %s > %s/tempo.xml",
			C_XML_GEN_HEADER_TDP_QTV, /* C_XML_GEN_TDP*/
			tg_types_fmc[vl_i-1].XMLFile,
			C_PATH_XML);
    }
    system(vl_cmd);
    sprintf(vl_cmd,"%s/tempo.xml",C_PATH_XML);
    ada2_xml_init2(vl_cmd, vl_i);
  }

  /*init du header des evts pere*/
  vg_arbre[C_GEN_HEADER_EVT_PERE] = NULL;
  sprintf(vl_cmd,"cat %s %s > %s/tempo.xml",
		C_XML_GEN_HEADER,
		"",
		C_PATH_XML);
  system(vl_cmd);
  sprintf(vl_cmd,"%s/tempo.xml",C_PATH_XML);
  ada2_xml_init2(vl_cmd, C_GEN_HEADER_EVT_PERE);

  /*init du header tdp global*/
  vg_arbre[C_GEN_HEADER_TDP_GLOBAL]=NULL;
  sprintf(vl_cmd,"cat %s %s > %s/tempo.xml",
		C_XML_GEN_TDP_GLOBAL,
		"",
		C_PATH_XML);
  system(vl_cmd);
  sprintf(vl_cmd,"%s/tempo.xml",C_PATH_XML);
  ada2_xml_init2(vl_cmd, C_GEN_HEADER_TDP_GLOBAL);
		
  /*init du header evt global*/
  vg_arbre[C_GEN_HEADER_EVT_GLOBAL]=NULL;
  sprintf(vl_cmd,"cat %s %s > %s/tempo.xml",
		C_XML_GEN_EVT_GLOBAL,
		"",
		C_PATH_XML);
  system(vl_cmd);
  sprintf(vl_cmd,"%s/tempo.xml",C_PATH_XML);
  ada2_xml_init2(vl_cmd, C_GEN_HEADER_EVT_GLOBAL);

  /*recupere le semaphore de tagen*/
  if (XZLG503_SemGet("/produits/migrazur/appliSD/exec/tagen.x",XZLGC_SEM_FMC) != XDC_OK) {
	    XZST_03EcritureTrace( XZSTC_WARNING, "ada2_xml_init: pb appel a XZLG503_SemGet");
	return (XDC_NOK);
  }

  /*recup des evts en cours sur le reseau*/
  if (XZLG986_Accede_Memoire_Partagee_FMC() != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_xml_init : pb appel  XZLG986_Accede_Memoire_Partagee_FMC") ;
	return(XDC_NOK);
  }

  if ( (pl_fp = fopen(CO_FIC_FREQUENCE, "r")) == NULL ) {
			XZST_03EcritureTrace(   XZSTC_WARNING, "erreur de lecture du fichier %s !"
									,CO_FIC_FREQUENCE );
			return(XDC_NOK);
	}

	/* On lit les lignes du fichier frequence.txt une par une */
	/* ------------------------------------------------------ */
	vg_frequence_keep_alive =0;					
	vg_frequence_recap =0;
	while ( fgets(pl_ligne, 200, pl_fp) != NULL) {
			/* On ignore les lignes commencant par un # */
			if ( pl_ligne[0] == '#' )
					continue;

			/* Pour les autres lignes on recherche la frequence de notre tache */
			if (sscanf(pl_ligne,"%s\t%d", pl_param, &vl_val)==2)
			{
					if ( !strcmp( pl_param, "keep_alive") ) 
					{
							vg_frequence_keep_alive = vl_val;
							XZST_03EcritureTrace(   XZSTC_WARNING, "ada2_xml_init frequence keep alive:%d ",vg_frequence_keep_alive );
					}
					else if ( !strcmp( pl_param, "recap") ) 
					{
							vg_frequence_recap = vl_val;
							XZST_03EcritureTrace(   XZSTC_WARNING, "ada2_xml_init frequence recap alive:%d ",vg_frequence_recap );
					}
			}
			else 
			{
					XZST_03EcritureTrace(   XZSTC_WARNING, "ada2_xml_init : le fichier %s n'a pas le format demande !"
													,CO_FIC_FREQUENCE );
			}
	}
	if (vg_frequence_keep_alive==0) 
	{
	  XZST_03EcritureTrace(   XZSTC_WARNING, "ada2_xml_init :pas de frequence keep alive dans le fichier %s : utilisation freq par default %d"
										   , CO_FIC_FREQUENCE
										   , C_FREQ_KEEP_ALIVE);
	  vg_frequence_keep_alive= C_FREQ_KEEP_ALIVE;
	}
	else if (vg_frequence_recap==0)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING, "ada2_xml_init : pas de frequence recap dans le fichier %s : utilisation freq par default %d"
										   , CO_FIC_FREQUENCE
										   , C_FREQ_RECAP_EVT);
	  vg_frequence_recap= C_FREQ_RECAP_EVT;
	}
	fclose(pl_fp);
	
	/* armement du timer de programmation pour keep alive messages */

	if (XZSM_01DemanderProgrammation(0,
					vg_frequence_keep_alive,
					XDG_TIMER_DATEX2,
					XDM_AS_TIMER_DATEX2,
					"fct_datex2_envoi_keep_alive",
					XZSMC_SEC_VRAI) != XDC_OK) 
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "ada2_xml_init : pb XZSM_01DemanderProgrammation");
	}
	else
		XZST_03EcritureTrace( XZSTC_WARNING, "ada2_xml_init : XZSM_01DemanderProgrammation : armement timer ada2 datex2 effectue");
								
	
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_TIMER_DATEX2),		
				ada2_keep_alive,(T_CB_ARG)NULL) == NULL)  {
	 XZST_03EcritureTrace( XZSTC_FATAL,
	 		"ada2_xml_init : Callback sur XDM_AS_TIMER_DATEX2 non cree.");
	}
	else
		XZST_03EcritureTrace( XZSTC_WARNING, "ada2_xml_init : TipcSrvProcessCbCreate : Callback timer ada2 keep_alive effectue");
	  	
	/* Ecriture du Msg FMC validee pour passerelle DATEX2 */


	XZST_03EcritureTrace(  XZSTC_WARNING,"ada2_xml_init : OUT");	

	return (XDC_OK);
}

char *ada2_xml_noeud_valeur(T_NOEUD *pa_noeud)
{
  static char vl_str[255]="";
  char *(*vl_fct)();
  if (pa_noeud->TypeValeur==C_VALEUR_DIRECTE)
    return (pa_noeud->Valeur);
  else {
    vl_fct=pa_noeud->Predicat.Fct;
    strcpy(vl_str,(*vl_fct)());
    return (vl_str);
  }
}

char *ada2_xml_noeud_test(T_NOEUD *pa_noeud)
{
  static char vl_str[255]="";
  char *(*vl_fct)();

    vl_fct=pa_noeud->Predicat_Test.Fct;
    strcpy(vl_str,(*vl_fct)());
    return (vl_str);
}

void ada2_xml_noeud(T_NOEUD *pa_noeud, xmlTextWriterPtr pa_flux)
{
int rc;
T_NOEUD *pl_noeud;
char vl_str[255];
char vl_strtest[255];
char vl_ch[255];

	if (strlen(pa_noeud->Libelle)==0) return;
	strcpy(vl_strtest,ada2_xml_noeud_test(pa_noeud));
     
      /* noeud principal*/
  if(strlen(vl_strtest) > 0) {
      if (pa_noeud->Type==C_MODULE) {
		rc = xmlTextWriterStartElement(pa_flux, BAD_CAST pa_noeud->BaliseXML);
      	if (rc < 0) {
	    XZST_03EcritureTrace (XZSTC_DEBUG1, "testXmlwriterFilename: Error at xmlTextWriterStartElement : %s\n",
				pa_noeud->Libelle);
    	    return;
      	}
      }
	else if  (pa_noeud->Type==C_ATTRIBUT_SPEC) { /*attribut specifique*/
	strcpy(vl_str,ada2_xml_noeud_valeur(pa_noeud));
#ifdef _HPUX_SOURCE
	sprintf(vl_ch,"\<%s\>%s\</%s\>",pa_noeud->BaliseXML,vl_str,pa_noeud->BaliseXML);
#else
	sprintf(vl_ch,"<%s>%s</%s>",pa_noeud->BaliseXML,vl_str,pa_noeud->BaliseXML);
#endif
	    XZST_03EcritureTrace (XZSTC_DEBUG1,  "testXmlwriterFilename: attribut specifique %s = %s",pa_noeud->BaliseXML,vl_ch);
		xmlTextWriterWriteRaw(pa_flux, (xmlChar*)vl_ch);
	rc = xmlTextWriterWriteAttribute(pa_flux, BAD_CAST (char *)pa_noeud->BaliseXML,
							BAD_CAST vl_str);
	}
    /*ecrire les attributs*/
	pl_noeud=pa_noeud->Attribut;
    while(pl_noeud != NULL) {
		strcpy(vl_strtest,ada2_xml_noeud_test(pl_noeud));
		if (strlen(vl_strtest) > 0) 
		{
			if (pl_noeud->SousAttribut<=0)  
			{
				strncpy(vl_str,ada2_xml_noeud_valeur(pl_noeud),128);
				vl_str[127]='\0';
				XZST_03EcritureTrace(   XZSTC_DEBUG1, "ada2_xml_noeud:  texte = %s",vl_str);
				if (pl_noeud->SousAttribut==0) 
				{
					xmlTextWriterWriteString(pa_flux, (xmlChar*)"\n  ");
					rc = xmlTextWriterWriteFormatElement(pa_flux, BAD_CAST pl_noeud->BaliseXML,
														"%s", vl_str);
					XZST_03EcritureTrace(   XZSTC_DEBUG1,"ICI %s",vl_str);
				}
				else {
				XZST_03EcritureTrace(   XZSTC_DEBUG1, "ada2_xml_noeud:  filtre sur la longueur du texte(%s)",vl_str);
				if (strlen(vl_str)>0) 
				{
					xmlTextWriterWriteString(pa_flux, (xmlChar*)"\n  ");
					rc = xmlTextWriterWriteFormatElement(pa_flux, BAD_CAST pl_noeud->BaliseXML,
														"%s", vl_str);
				}
				XZST_03EcritureTrace(   XZSTC_DEBUG1,"ICI");
				}
			}
			else {
			XZST_03EcritureTrace(XZSTC_DEBUG1,"att = %s\n",pl_noeud->BaliseXML);

			strcpy(vl_str,ada2_xml_noeud_valeur(pl_noeud));
			rc = xmlTextWriterWriteAttribute(pa_flux, BAD_CAST (char *)pl_noeud->BaliseXML, 
			BAD_CAST vl_str);
			}
		}
		pl_noeud = pl_noeud->Attribut ;
	}
  }

  /*si fils aine et les freres*/
  if (pa_noeud->FilsAine != NULL) {
    XZST_03EcritureTrace(XZSTC_DEBUG1,"noeud fils de %s = %s\n",pa_noeud->Libelle, pa_noeud->FilsAine->Libelle);
	pl_noeud = pa_noeud->FilsAine;
	xmlTextWriterWriteString(pa_flux, (xmlChar*)"\n  ");
	ada2_xml_noeud(pl_noeud,pa_flux);
  }
  XZST_03EcritureTrace(XZSTC_DEBUG1,"test pour noeud=%s, vl_strtest=%s",pa_noeud->Libelle, vl_strtest);
  if(strlen(vl_strtest) > 0) 
  {
	if (pa_noeud->Type==C_MODULE) 
	{
		//Balises avec sous attributs sans enfant, Il faut les fermer sans sauter de ligne
		if(!strcmp(pa_noeud->BaliseXML,"relatedSituation") ||
			!strcmp(pa_noeud->BaliseXML,"measurementSiteTableReference") ||
			!strcmp(pa_noeud->BaliseXML,"measurementSiteReference")  )
		{
			xmlTextWriterWriteString(pa_flux,(xmlChar*)"");
			xmlTextWriterEndElement(pa_flux);	
		} 
		else
		{
			xmlTextWriterWriteString(pa_flux, (xmlChar*)"\n  ");
			xmlTextWriterEndElement(pa_flux);
		}
	} 
  }

    /*si freres*/
  if (pa_noeud->FrereCadet != NULL) 
  {
	pl_noeud = pa_noeud->FrereCadet;
	XZST_03EcritureTrace(XZSTC_DEBUG1,"noeud frere de %s = %s\n",pa_noeud->Libelle,pl_noeud->Libelle);
	xmlTextWriterWriteString(pa_flux, (xmlChar*)"\n  ");
	ada2_xml_noeud(pl_noeud,pa_flux);
  }
    XZST_03EcritureTrace(XZSTC_DEBUG1,"fin noeud = %s\n",pa_noeud->Libelle);
}

void ada2_xml_EVT_write(XZLGT_Liste_FMC va_noeud, int va_type,char *va_prefixe)
{
    T_NOEUD *vl_noeud;
    T_NOEUD *vl_noeud2;
    int rc;
    xmlTextWriterPtr writer;
    char vl_nom[255];

    /*memorise la FMC courante*/
    vm_noeud = va_noeud;		
		
    /* Create a new XmlWriter for uri, with no compression. */
    if (!strcmp(va_prefixe,"glob")) 
      sprintf(vl_nom,"%s/%s.xml",
		chemin_export,
		va_prefixe);
    else
      sprintf(vl_nom,"%s/%s%d_%d.xml",
		chemin_export,
		va_prefixe,
		va_noeud.Evt.numero,
		va_noeud.Evt.cle);
	
    writer = xmlNewTextWriterFilename(vl_nom, 0);
    if (writer == NULL) {
        printf("testXmlwriterFilename: Error creating the xml writer\n");
        return;
    }
		
    /* Start the document with the xml default for the version,
     * encoding ISO 8859-1 and the default for the standalone
     * declaration. */
    rc = xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartDocument\n");
        return;								
    }

    /*on se positionne sur le bon modele XML*/		
    vl_noeud=vg_arbre[va_type];		

	XZST_03EcritureTrace(XZSTC_WARNING,"ada2_xml_EVT_write : prefixe=%s, FMC num=%d, type=%d",va_prefixe,va_noeud.Evt.numero,va_type);

    /*TRAITEMENT PARTICULIER POUR CHAUSSEE GLISSANTE*/
    if (va_type==XZAEC_FMC_ChausseeGliss) {
	XZST_03EcritureTrace(XZSTC_WARNING,"GLISS %d",va_noeud.Bloc_Optio.Bloc.Meteo.Type);
      if ( (va_noeud.Bloc_Optio.Bloc.Meteo.Type==XZAEC_CH_GLIS_Autre) ||
	         (va_noeud.Bloc_Optio.Bloc.Meteo.Type==XZAEC_CH_GLIS_Huile) ||
	         (va_noeud.Bloc_Optio.Bloc.Meteo.Type==XZAEC_CH_GLIS_Carburant) ||
	         (va_noeud.Bloc_Optio.Bloc.Meteo.Type==XZAEC_CH_GLIS_Gasoil) ) 
          vl_noeud=vg_arbre[XZAEC_FMC_FlaqueHuile];
    }

    ada2_xml_noeud(vl_noeud,writer);

    rc = xmlTextWriterEndDocument(writer);						
    if (rc < 0) {
    printf ("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
	return;
    }
	xmlFreeTextWriter(writer);

}

void ada2_xml_TDP_write(XDY_Eqt va_station, int va_type,char *va_prefixe){
	T_NOEUD *vl_noeud;
	int rc;
	xmlTextWriterPtr writer;
	char vl_nom[255]="";

	/* Create a new XmlWriter for uri, with no compression. */
		
	/* Create a new XmlWriter for uri, with no compression. */
	if (!strcmp(va_prefixe,"glob")) 
	sprintf(vl_nom,"%s/%s.xml",
			chemin_export,
			va_prefixe);
	else
		sprintf(vl_nom,"%s/%s_%d%s.xml",
			chemin_export,
			"SIREDO",
			va_station,
			va_prefixe);
	
	writer = xmlNewTextWriterFilename(vl_nom, 0);
	if (writer == NULL) {
		printf("testXmlwriterFilename: Error creating the xml writer\n");
		return;
	}		

	/* Start the document with the xml default for the version,
	* encoding ISO 8859-1 and the default for the standalone
	* declaration. */
	rc = xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);
	if (rc < 0) {
		printf("testXmlwriterFilename: Error at xmlTextWriterStartDocument\n");
		return;
	}

	/*on se positionne sur le bon modele XML*/		
	vl_noeud=vg_arbre[va_type];

	ada2_xml_noeud(vl_noeud,writer);

	rc = xmlTextWriterEndDocument(writer);								
	if (rc < 0) {
		printf ("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
		return;
	}
	xmlFreeTextWriter(writer);	
}	

void ada2_copie_fichier_vers_ftp_rep (int typeData) 
{
	DIR *dir;
	struct dirent *entry;
	int i,cpt;
	char PathFichier[PATH_MAX_STRING_SIZE]="";
	char NomFichier[64]="";
	char NomDossier[64]="";
	char vl_str[128]="";
	char vl_cmd[64]="";
	int                   vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
	XZSMT_Horodate        vl_horo_chaine;
	int			vl_Jour_De_La_Semaine;
	XDY_Horodate		vl_date;

	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_copie_fichier_vers_ftp_rep: IN");
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_copie_fichier_vers_ftp_rep: Pth:%s",C_PATH);

    	if (!(dir = opendir(C_PATH)))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_copie_fichier_vers_ftp_rep: Erreur impososible d'ouvire le dossier DatexII:%s",C_PATH);
        	return;
	}
	
	/* Pour chaque fichier du dossier*/
	while ((entry = readdir(dir)) != NULL) 
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_copie_fichier_vers_ftp_rep: fichier:%s",entry->d_name);
		char path[1024];
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;
		/* skip les fichier tempon */
		if (strcmp(entry->d_name,"ligne1.xml")==0)
			continue;
		if (strcmp(entry->d_name,"glob.old")==0)
			continue;
		
		if (strrchr(entry->d_name, '.')==NULL) 
		{			
			XZST_03EcritureTrace(XZSTC_WARNING,"ada2_copie_fichier_vers_ftp_rep, fichier:%d sans extension, ignore\n",entry->d_name);
		} 
		else 
		{
			/* pour chaque destinataire */
			for(i=0;i<vg_nbrDestinataire;i++)
			{
				/* si pour ce destinataire la data est voulu et du type evt */
				if (vg_destinataire[i].DataTypeVoulu[typeData] >=1 && typeData==DATA_TYPE_EVT)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"ada2_copie_fichier_vers_ftp_rep : EVT");
					/* copie dans un fichier temporaire */
					ada2_deplacer_fichier("tmp.txt",
							C_PATH,
							C_PATH,
							entry->d_name);

					/* modifie le fichier temporaire */
					/* set feedtype a la valeur du compteur du destinataire */
					ada2_set_feedtype_value("tmp.txt",C_PATH,vg_destinataire[i].Nom);

					/* supprime les FMC holdup/agression/malaise pour les utilisateur non autorisé */
					if (vg_destinataire[i].DataTypeVoulu[DATA_TYPE_EVT] == 2 )
					{
						ada2_remove_FMC_privee(C_PATH,"tmp.txt");
					}
					
					/* determine le nom du fichier de déstination en fonction du type de publication */
					if (!strcmp(vg_publication_type,C_PUBLICATION_RECAPITULATIVE))
					{
						/* publication global = content.xml */
						strcpy(NomFichier,C_PUBLICATION_RECAPITULATIVE_FILE_NAME);
					}
					else if (!strcmp(vg_publication_type,C_PUBLICATION_COUPPARCOUP))
					{
						/* publication cour par coup = compteur pour chaque destinataire */
						cpt=ada2_get_compteur_nom_fichier_et_incr(vg_destinataire[i].Nom,XDC_VRAI);
						sprintf(vl_str,"%06d.temp",cpt);
						strcpy(NomFichier,vl_str);	
					}
					else if (!strcmp(vg_publication_type,C_PUBLICATION_KEEP_ALIVE))
					{
						/* publication keep_alive = content.xml */
						strcpy(NomFichier,C_KEEP_ALIVE_FILE_NAME);
					}
					
					XZST_03EcritureTrace( XZSTC_WARNING, "ada2_copie_fichier_vers_ftp_rep Destinataire:%s fichier:%s deplacer dans dossier:%s",vg_destinataire[i].Nom,entry->d_name,vg_destinataire[i].URLWebService_PathFTP);

					/* direction dossier evt */
					strcpy(NomDossier,vg_destinataire[i].URLWebService_PathFTP);
					strcat(NomDossier,"/");
					strcat(NomDossier,CO_REP_EVT);

					/* Copie le fichier temporaire modifié dans les répertoire des destinataires */
					ada2_deplacer_fichier(NomFichier,
							NomDossier,
							C_PATH,
							"tmp.txt");

					/* création commande export ftp */
					sprintf(vl_cmd,"%s/ftp/%s%s",		
						XDC_PATH_ADA,
						vg_destinataire[i].Nom,
						CO_FTP_SCRIPT_EVTS_AFFIXE);
						XZST_03EcritureTrace( XZSTC_WARNING,"Commande script ftp_evts=%s",vl_cmd);
					system(vl_cmd);
				}
				/* si pour ce destinataire la data est voulu et du type radt */
				else if (vg_destinataire[i].DataTypeVoulu[typeData] >=1 && typeData==DATA_TYPE_RADT)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"ada2_copie_fichier_vers_ftp_rep : RADT");
					XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_Jour_De_La_Semaine,&vl_date);
					XZSM_11ConversionHorodate(vl_date,&vl_horo_chaine);
					sscanf(vl_horo_chaine, "%2d/%2d/%4d %2d:%2d:%2d",		
								&vl_jour,
								&vl_mois,
								&vl_annee,
								&vl_heure,
								&vl_minute,
								&vl_seconde);
					 sprintf(NomFichier,"%s_DataTR_%04d%02d%02d_%02d%02d%02d",
							C_DATEX_TRAFIC_ESCOTA,
							vl_annee,
							vl_mois,
							vl_jour,
							vl_heure,
							vl_minute,
							vl_seconde);						
					XZST_03EcritureTrace( XZSTC_WARNING, "ada2_copie_fichier_vers_ftp_rep Destinataire:%s fichier:%s deplacer dans dossier:%s",vg_destinataire[i].Nom,NomFichier,vg_destinataire[i].URLWebService_PathFTP);

					/* direction dossier radt */
					strcpy(NomDossier,vg_destinataire[i].URLWebService_PathFTP);
					strcat(NomDossier,"/");
					strcat(NomDossier,CO_REP_RADT);

					/* Copie le fichier temporaire modifié dans les répertoire des destinataires */
					ada2_deplacer_fichier(NomFichier,
							NomDossier,
							C_PATH,
							entry->d_name);

					/* création commande export ftp */
					sprintf(vl_cmd,"%s/ftp/%s%s",	
						XDC_PATH_ADA,
						vg_destinataire[i].Nom,
						CO_FTP_SCRIPT_RADT_AFFIXE);
					XZST_03EcritureTrace( XZSTC_WARNING,"Commande script ftp_radt=%s",vl_cmd);
					system(vl_cmd);
				}
					
			}
			/* Supprime le fichier du dossier source */
			PathFichier[0]='\0';	
			strcat(PathFichier,C_PATH);
			strcat(PathFichier,"/");
			strcat(PathFichier,entry->d_name);
			remove(PathFichier);
			/* Supprime le fichier temporaire du dossier source */
			PathFichier[0]='\0';	
			strcat(PathFichier,C_PATH);
			strcat(PathFichier,"/");
			strcat(PathFichier,"tmp.txt");
			remove(PathFichier);						
		}
	}	
	closedir(dir);	
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_copie_fichier_vers_ftp_rep: OUT");
}

int ada2_deplacer_fichier(const char *va_DestNom, const char *va_DirDest, const char *va_DirSource, const char* va_Fichier)
{
	char tmp[PATH_MAX_STRING_SIZE];
	int fd_to, fd_from;
	char buf[4096]="";
	ssize_t nread;
	int saved_errno;
	struct stat sb;
	char *p=NULL;
	char PathFichierSource[PATH_MAX_STRING_SIZE]="";
	char PathFichierDest[PATH_MAX_STRING_SIZE]="";	
	size_t len;
	char *out_ptr;

	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_deplacer_fichier: IN");
	
	PathFichierSource[0]='\0';
	PathFichierDest[0]='\0';
	
	/* concat dossier + fichier */
	strcpy(PathFichierSource,va_DirSource);
	strcat(PathFichierSource,"/");
	strcat(PathFichierSource,va_Fichier);	
	
XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_deplacer_fichier: destIN=%s",va_DirDest);
XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_deplacer_fichier: nomIN=%s",va_DestNom);

	strcpy(PathFichierDest,va_DirDest);
	strcat(PathFichierDest,"/");
	strcat(PathFichierDest,va_DestNom);
	
	/* copie path */
    len = strnlen (PathFichierDest, PATH_MAX_STRING_SIZE);
    if (len == 0 || len == PATH_MAX_STRING_SIZE) {
		XZST_03EcritureTrace(XZSTC_WARNING, "ada2_deplacer_fichier Path Destinataire trop long. Path =%s",PathFichierDest);
        return -1;
    }
    memcpy (tmp, PathFichierDest, len);
    tmp[len] = '\0';

    /* supp dernier slash */
    if(tmp[len - 1] == '/') {
        tmp[len - 1] = '\0';
    }			
			
    /* test si le path vers le dossier destinataire existe */
    if (stat (va_DirDest, &sb) == 0) 
	{
        if (S_ISDIR (sb.st_mode)) 
		{
			/*Le dossier ftp destinataire existe */
			XZST_03EcritureTrace(XZSTC_WARNING, "ada2_deplacer_fichier Dossier ftp destinataire %s existe",va_DirDest);
        }
		else 
		{
			XZST_03EcritureTrace(XZSTC_FATAL, "ada2_deplacer_fichier Un fichier porte le même nom que le dossier ftp destinataire:%s",va_DirDest);
			return -1;
		}
    }
	else /*creer le dossier destinataire */
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "ada2_deplacer_fichier Dossier ftp destinataire %s n'existe pas, on le cree",va_DirDest);
		/* recursive mkdir */	
		for(p = tmp + 1; *p; p++) 
		{
			if(*p == '/') 
			{
				*p = 0;				
				/* test path */
				if (stat(tmp, &sb) != 0) 
				{
					/* le dossier n'existe pas - on le creer */
					if (mkdir(tmp, 0777) < 0) 
					{
						XZST_03EcritureTrace(XZSTC_FATAL, "ada2_deplacer_fichier Impossible de creer le dossier ftp destinataire:%s",tmp);
						return -1;
					}
				} 
				else if (!S_ISDIR(sb.st_mode)) 
				{
					/* pas un dossier */
					XZST_03EcritureTrace(XZSTC_FATAL, "ada2_deplacer_fichier Un fichier porte le même nom que le dossier ftp destinataire:%s",tmp);
					return -1;
				}
				*p = '/';
			}
		}
	}

	/* ouvre le fichier source */
    fd_from = open(PathFichierSource, O_RDONLY);
    if (fd_from < 0)
	{
		XZST_03EcritureTrace(XZSTC_FATAL, "ada2_deplacer_fichier Impossible d'ouvrir le fichier source:%s",PathFichierSource);
        return -1;
	}

	/* supprime le fichier si il existe déjà */	
	remove(PathFichierDest);		

	/* ouvre le fichier destinataire */
    fd_to = open(PathFichierDest, O_WRONLY | O_CREAT, 0777);
    if (fd_to < 0)
	{
        close(fd_from);
		if (fd_to >= 0)
			close(fd_to);	
		XZST_03EcritureTrace(XZSTC_FATAL, "ada2_deplacer_fichier Impossible d'ouvrir le fichier dest:%s",PathFichierDest);
		return -1;
	}

	/* copie du fichier de source vers dest */
    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        out_ptr = buf;
        ssize_t nwritten;
        do {
			nwritten = write(fd_to, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                close(fd_from);
				if (fd_to >= 0)
					close(fd_to);	
				XZST_03EcritureTrace(XZSTC_FATAL, "ada2_deplacer_fichier Erreur d'ecriture errno=%d",errno);
				return -1;
            }
        } while (nread > 0);
    }

    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
		fd_to = -1;
		close(fd_from);
		close(fd_to);	
		XZST_03EcritureTrace(XZSTC_FATAL, "ada2_deplacer_fichier Erreur fermeture fichier dest");
		return -1;
        }
        close(fd_from);
	XZST_03EcritureTrace(XZSTC_WARNING, "ada2_deplacer_fichier: fichier copier de:%s vers %s",PathFichierSource,PathFichierDest);
	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_deplacer_fichier: OUT");
        return XDC_OK;
    }

    close(fd_from);
    if (fd_to >= 0)
        close(fd_to);
	XZST_03EcritureTrace(XZSTC_FATAL, "ada2_deplacer_fichier Erreur inconnue");
	return -1;
}

int ada2_set_feedtype_value(const char * va_NomFichier,const char *va_PathFichier,const char *va_destinataire)
{
	FILE *pFile;
	fpos_t position;
	char vl_line_output[200];
	char vl_ligne[200];		
	int cpt;	
	vl_line_output[0]='\0';
	char PathFichier[PATH_MAX_STRING_SIZE]="";	
		
	/* concat dossier + fichier */
	strcat(PathFichier,va_PathFichier);
	strcat(PathFichier,"/");
	strcat(PathFichier,va_NomFichier);	

	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_set_feedtype_value: IN");

	/* ouvre le fichier xml */
   	if ( (pFile = fopen(PathFichier, "r+"))== NULL )
	{
		XZST_03EcritureTrace(XZSTC_FATAL, "ada2_set_feedtype_value erreur de lecture %s !",PathFichier);
		return -1;
	}	
	/* get position du curseur de lecture */
	fgetpos	(pFile, &position);
	while ( fgets(vl_ligne, 200, pFile) != NULL) 
	{
		/* si le nom correspond au destinataire en paramètre */
		if (!strncmp(vl_ligne,"  <feedType>",12))
		{				
			/* Gestion de la balise nom de fichier unitaire par destinataire */
			cpt=ada2_get_compteur_nom_fichier_et_incr(va_destinataire,XDC_FAUX);
			/* on prépare la ligne pour remplacer */								
			sprintf(vl_line_output,"  <feedType>%010d</feedType>\n",cpt);			
			/* on retourne au début de la ligne */
			fsetpos (pFile,&position);			
			/* on écrit le nom et la nouvelle valeur du compteur */
			fputs (vl_line_output,pFile);
				
			fclose (pFile);
			XZST_03EcritureTrace(XZSTC_WARNING,"ada2_set_feedtype_value fichier=%s.cpt=%d",va_destinataire,cpt);
			XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_set_feedtype_value: OUT");
			return 1;
		}		

		/* get posiiton du curseur de lecture */
		fgetpos	(pFile, &position);
	}

	/* si on ne trouve pas le destinataire dans le fichier */
	XZST_03EcritureTrace(XZSTC_WARNING,"ada2_set_feedtype_value balisefeedtype non trouvee");		

	fclose(pFile);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_set_feedtype_value: OUT");
    
	return -2;		
}

int ada2_remove_FMC_privee (const char *va_DossierFichier,const char *va_NomFichier)
{
	FILE *pInFile;
	FILE *pOutFile;
	fpos_t positionDebut;
	fpos_t positionCurrent;
	int copy=1;
	int FMCaSupprimer=0;
	int FMCvalide=0;
	char vl_line_output[200];
	char vl_ligne[200];	
	vl_line_output[0]='\0';
	char pathFichierTemp[PATH_MAX_STRING_SIZE]="";
	char pathFichier[PATH_MAX_STRING_SIZE]="";
	char fichierNewName[32]="";
	
	strcpy(pathFichier,va_DossierFichier);
	strcat(pathFichier,"/");
	strcat(pathFichier,va_NomFichier);

	strcpy(fichierNewName,va_NomFichier);
	strcat(fichierNewName,"_tmp");

	strcpy(pathFichierTemp,va_DossierFichier);
	strcat(pathFichierTemp,"/");
	strcat(pathFichierTemp,fichierNewName);



	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_remove_FMC_privee: IN");

	/* ouvre le fichier in xml */
   	if ( (pInFile = fopen(pathFichier, "r+"))== NULL )
	{
		XZST_03EcritureTrace(XZSTC_FATAL, "ada2_remove_FMC_privee erreur de lecture %s !",pathFichier);
		return -1;
	}
	/* ouvre le fichier out xml */
   	if ( (pOutFile = fopen(pathFichierTemp, "w+"))== NULL )
	{
		XZST_03EcritureTrace(XZSTC_FATAL, "ada2_remove_FMC_privee erreur de lecture %s !",pathFichierTemp);
		return -1;
	}		
	/* get position du curseur de lecture */
	fgetpos	(pInFile, &positionCurrent);
	while ( fgets(vl_ligne, 200, pInFile) != NULL) 
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"ada2_remove_FMC_privee read:%s",vl_ligne);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"ada2_remove_FMC_privee state:copy=%d FMCaSuppr=%d FMCvalide%d",copy,FMCaSupprimer,FMCvalide);
		if (!strncmp(vl_ligne,"<situation id=",14) && !FMCvalide)
		{	
			XZST_03EcritureTrace(XZSTC_DEBUG1,"ada2_remove_FMC_privee debut FMC:%s",vl_ligne);
			positionDebut=positionCurrent;
			copy=0;
		}
		if (copy)
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,"ada2_remove_FMC_privee copy ligne:%s",vl_ligne);
			fputs (vl_ligne,pOutFile);
		}
		/* get de la position du début de la fmc */
		if ( !strncmp(vl_ligne,"  <disturbanceActivityType>illVehicleOccupants",46) ||
			 !strncmp(vl_ligne,"  <disturbanceActivityType>assault",34) ||
			 !strncmp(vl_ligne,"  <disturbanceActivityType>theft",32) )
		{	
			XZST_03EcritureTrace(XZSTC_WARNING,"ada2_remove_FMC_privee FMC a supprimer:%s",vl_ligne);
			FMCaSupprimer=1;
		}
		if (!strncmp(vl_ligne,"</situation>",12) && !FMCvalide)
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,"ada2_remove_FMC_privee Fin FMC:%s",vl_ligne);
			if (FMCaSupprimer)
			{
				FMCaSupprimer=0;
			}
			else
			{
				FMCvalide=1;
				copy =1;
				fsetpos(pInFile,&positionDebut);
			}
		}
		else if (!strncmp(vl_ligne,"</situation>",12) && FMCvalide)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"ada2_remove_FMC_privee FMC copier en entier");
			FMCvalide=0;
		}
		fgetpos	(pInFile, &positionCurrent);		
	}	

	fclose(pInFile);	
	fclose(pOutFile);

	ada2_deplacer_fichier(va_NomFichier,
				va_DossierFichier,
				va_DossierFichier,
				fichierNewName);
	remove(pathFichierTemp);
	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_remove_FMC_privee: OUT");
    
	return XDC_OK;
}							

int ada2_get_compteur_nom_fichier_et_incr(const char *va_destinataire, int incr)
{
	FILE *pFile;
	fpos_t position;		
	int vl_CptIncr;
	char vl_line_output[200];
	char vl_ligne[200];
	char vl_destinataire[32];		
	int vl_compteur;
		

	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_get_compteur_nom_fichier_et_incr: IN");

	/* ouvre le fichier compteur */
   	if ( (pFile = fopen(CO_CPT_NOM_FICHIER, "r+"))== NULL )
	{
		XZST_03EcritureTrace(XZSTC_FATAL, "ada2_get_compteur_nom_fichier_et_incr erreur de lecture %s !",CO_CPT_NOM_FICHIER);
		return -1;
	}	

	/* get position du curseur de lecture */
	fgetpos	(pFile, &position);
	while ( fgets(vl_ligne, 200, pFile) != NULL) 
	{
				
		/* lecture des champs */
		if (sscanf(vl_ligne,"%s\t%d",vl_destinataire, &vl_compteur)==2)
		{
			/* si le nom correspond au destinataire en paramètre */
			if (!strcmp(vl_destinataire,va_destinataire))
			{
				
				if (incr) //Si on doit incrémenter
				{
					/* on incrémente sont compteur */
					vl_CptIncr=vl_compteur+1;				
					
					/* on prépare la ligne pour remplacer */
					sprintf(vl_line_output,"%s\t%d",vl_destinataire,vl_CptIncr);
					
					/* on retourne au début de la ligne */
					fsetpos (pFile,&position);
					
					/* on écrit le nom et la nouvelle valeur du compteur */
					fputs (vl_line_output,pFile);
				}
				else
					vl_CptIncr=vl_compteur;

				fclose (pFile);
				XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_get_compteur_nom_fichier_et_incr: OUT");
				return vl_CptIncr;
			}
		}
		else
			XZST_03EcritureTrace(XZSTC_WARNING,"ada2_get_compteur_nom_fichier_et_incr format ligne inccorect");

		/* get posiiton du curseur de lecture */
		fgetpos	(pFile, &position);
	}

	/* si on ne trouve pas le destinataire dans le fichier */
	sprintf(vl_line_output,"%s\t0\n",va_destinataire);
	XZST_03EcritureTrace(XZSTC_WARNING,"ada2_get_compteur_nom_fichier_et_incr ajout du destinataire et compteur :%s",vl_line_output);		
	/* on le rajoute à la fin du fichier */
	fputs(vl_line_output,pFile);

	fclose(pFile);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_get_compteur_nom_fichier_et_incr: OUT");
    
	return 1;
		
}

int ada2_get_departement(float va_PR, const char * va_axe)
{
	FILE *pFile;
	char vl_ligne[200];
	
	char vl_axe[8];		
	float vl_PR_debut;
	float vl_PR_fin;
	int vl_departement;		

	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_get_departement: IN");

	/* ouvre le fichier departement */
   	if ( (pFile = fopen(CO_DEPARTEMENT, "r"))== NULL )
	{
		XZST_03EcritureTrace(XZSTC_FATAL, "ada2_get_departement erreur de lecture %s !",CO_DEPARTEMENT);
		return 0;
	}	

	/* Lecture ligne du fichier */
	while ( fgets(vl_ligne, 200, pFile) != NULL) 
	{
		/* On ignore les lignes commencant par un # */
		if ( vl_ligne[0] == '#' )
			continue;
				
		/* lecture des champs */
		if (sscanf(vl_ligne,"%s\t%f\t%f\t%d",vl_axe, &vl_PR_debut,&vl_PR_fin,&vl_departement)==4)
		{
			/* si le nom correspond au destinataire en paramètre */
			if (!strcmp(va_axe,vl_axe)) //l'axe correspond
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"departement va_PR=%f vl_PR_debut=%f, vl_PR_fin=%f",va_PR,vl_PR_debut,vl_PR_fin);
				if (va_PR>=vl_PR_debut && va_PR <= vl_PR_fin) //le PR en param est inclus dans les PR de la ligne
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"ada2_get_departement department pour axe=%s et PR=%f trouvee!",va_axe,va_PR);
					XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_get_departement: OUT");
					fclose(pFile);
					return vl_departement; //Retourne n° du département
				}
			}
		}
		else
			XZST_03EcritureTrace(XZSTC_WARNING,"ada2_get_departement format ligne inccorect");
	}

	XZST_03EcritureTrace(XZSTC_WARNING,"ada2_get_departement department pour axe=%s et PR=%f non trouvee!",va_axe,va_PR);
	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_get_departement: OUT");
	fclose(pFile);
	return 0;		
		
}

void ada2_keep_alive() 
{
	int fd_to, fd_from;
	int i=0;;
	char NomDossier[64]="";		

	/* renseignement du type de publication */
	vg_publication_type=C_PUBLICATION_KEEP_ALIVE;
		
	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_keep_alive: IN");
	
	/* Pour chaque destinataire */
/*	for(i=0;i<vg_nbrDestinataire;i++)
	{
		strcpy(NomDossier,vg_destinataire[i].URLWebService_PathFTP);
		strcat(NomDossier,"/");
		strcat(NomDossier,CO_REP_EVT);
		ada2_deplacer_fichier(C_KEEP_ALIVE_FILE_NAME,NomDossier,C_PATH_XML,C_KEEP_ALIVE_FILE_NAME);

		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_keep_alive: Keep_Alive message transmit au destinataire:%s - %s",NomDossier);
	}	*/

	ada2_deplacer_fichier(C_KEEP_ALIVE_FILE_NAME,C_PATH,C_PATH_XML,C_KEEP_ALIVE_FILE_NAME);
		
	//Copie du fichiers keepalive dans les repertoires des destinataires
	ada2_copie_fichier_vers_ftp_rep(DATA_TYPE_EVT);

	XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_keep_alive: OUT");
}

int get_local_UTC_hour_diff(XDY_Horodate pa_HorodateSec)
{
	//cast en time_t
	time_t rawtime = pa_HorodateSec;
	//Get temps UTC de rawtime
	struct tm * ptm = gmtime(&rawtime);
	//convert time UTC en time_t
	time_t gmt = mktime(ptm);
	//convert time_t rawtime en struct tm
	ptm = localtime(&rawtime);
	//ptm->tm_isdst flag heure éte/hiver (+1/+0 pour UTC)
	time_t offset = rawtime - gmt + (ptm->tm_isdst ? 3600 : 0);
	if (offset>1800 && offset<5400)//entre 30min et 1h30 retourne 1h de diff
		return 1;
	else if (offset>=5400)//> à 1h30 retourne 2h de diff
		return 2;
}

// Renvoie l'indice de la structure en param ou struct.numero = va_numero
int find_indice_numero_element_attache_regroupement(T_NUMERO_ELEMENT_REGROUPEMENT *va_struct,XDY_Entier va_numero_regroupement,int va_size)
{
	int i=0;
	for(i=0;i<va_size;i++)
	{
		if(va_struct[i].numero_regroupement==va_numero_regroupement)
			return i;
	}
return -1;
}	

/**
 * ConvertInput:
 * @in: string in a given encoding
 * @encoding: the encoding used
 *
 * Converts @in into UTF-8 for processing with libxml2 APIs
 *
 * Returns the converted UTF-8 string, or NULL in case of error.
 */
xmlChar *
ConvertInput(const char *in, const char *encoding)
{
    xmlChar *out;
    int ret;
    int size;
    int out_size;
    int temp;
    xmlCharEncodingHandlerPtr handler;

    if (in == 0)
        return 0;

    handler = xmlFindCharEncodingHandler(encoding);

    if (!handler) 
	{
        printf("ConvertInput: no encoding handler found for '%s'\n",
               encoding ? encoding : "");
        return 0;
    }

    size = (int) strlen(in) + 1;
    out_size = size * 2 - 1;
    out = (unsigned char *) xmlMalloc((size_t) out_size);

    if (out != 0) 
	{
        temp = size - 1;
        ret = handler->input(out, &out_size, (const xmlChar *) in, &temp);
        if ((ret < 0) || (temp - size + 1)) 
		{
            if (ret < 0) 
			{
                printf("ConvertInput: conversion wasn't successful.\n");
            } else {
                printf("ConvertInput: conversion wasn't successful. converted: %i octets.\n",
                     temp);
            }

            xmlFree(out);
            out = 0;
        } else 
		{
            out = (unsigned char *) xmlRealloc(out, out_size + 1);
            out[out_size] = 0;  /*null terminating out */
        }
    } else 
	{
        printf("ConvertInput: no mem\n");
    }
    return out;
}

static char *balise_groupe_of_location_type(){
	if (vm_noeud.PR_Fin== vm_noeud.PR_Debut || vm_noeud.PR_Fin==0)
	{
		return C_GROUPE_OF_LOCATION_POINT;
	}
	else
	{
		return C_GROUPE_OF_LOCATION_LINEAR;
	}
}

static char *balise_test_groupe_of_location_linear(){
	if (!strcmp(balise_groupe_of_location_type(),C_GROUPE_OF_LOCATION_LINEAR))
		return "1";
	return "";
}
static char *balise_test_groupe_of_location_point(){
	if (!strcmp(balise_groupe_of_location_type(),C_GROUPE_OF_LOCATION_POINT))
		return "1";
	return "";
}

static char *balise_l01() {
	return (vm_noeud.RDS.l01);
}
static char *balise_l02() {
	return (vm_noeud.RDS.l02);
}
static char *balise_l03() {
	return (vm_noeud.RDS.l03);
}
static char *balise_l04() {
	return (vm_noeud.RDS.l04);
}
static char *balise_l05() {
	return (vm_noeud.RDS.l05);
}
static char *balise_l07() {
	return (vm_noeud.RDS.l07);
}
static char *balise_l06() {
	return (vm_noeud.RDS.l06);
}
static char *balise_l12() {
	return (vm_noeud.RDS.l12);
}
static char *balise_l09() {
	return (vm_noeud.RDS.l09);
}
static char *balise_l10() {
	return (vm_noeud.RDS.l10);
}
static char *balise_l11() {
	return (vm_noeud.RDS.l11);
}
static char *balise_rpip() {
	static char vl_str[16]="";
	static char vl_cote[4]="";
	int vl_departement=0;
	static char *vl_autoroute;
	float test;
	
	switch (vm_noeud.Autoroute) {
		case 1:vl_autoroute="A8";break;
		case 2:vl_autoroute="A500";break;
		case 3:vl_autoroute="A50";break;
		case 4:vl_autoroute="A501";break;
		case 5:vl_autoroute="A51";break;
		case 6:vl_autoroute="A52";break;
		case 7:vl_autoroute="A520";break;
		case 8:vl_autoroute="A57";break;
		case 9:vl_autoroute="A7";break;
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"departement PRdebut=%d PRfin=%d",vm_noeud.PR_Debut,vm_noeud.PR_Fin);

	/* get departement */	
	vl_departement=ada2_get_departement(vm_noeud.PR_Debut/(float)1000,vl_autoroute);
	
	if(vm_noeud.Sens==XDC_SENS_SUD || vm_noeud.Sens==XDC_SENS_SORTIE_SUD || vm_noeud.Sens==XDC_SENS_ENTREE_SUD)
		sprintf(vl_cote,"D");
	else if(vm_noeud.Sens==XDC_SENS_NORD || vm_noeud.Sens==XDC_SENS_SORTIE_NORD || vm_noeud.Sens==XDC_SENS_ENTREE_NORD)
		sprintf(vl_cote,"G");
	else 
		sprintf(vl_cote,"D");
	
	sprintf(vl_str,"%02dPR%d%sC",vl_departement,vm_noeud.PR_Debut/1000,vl_cote);

	return (vl_str);
}
static char *balise_rnp() {
	switch (vm_noeud.Autoroute) {
		case 1: return ("A8");break;
		case 2: return ("A500");break;
		case 3: return ("A50");break;
		case 4: return ("A501");break;
		case 5: return ("A51");break;
		case 6: return ("A52");break;
		case 7: return ("A520");break;
		case 8: return ("A57");break;
		case 9: return ("A7");break;
	}
	
	return (vm_noeud.RDS.rnp);
}
static char *balise_drp() {
	return (vm_noeud.RDS.drp);
}
static char *balise_dfpp() {
	return (vm_noeud.RDS.dfpp);
}
static char *balise_rpis() {
		static char vl_str[16]="";
	static char vl_cote[4]="";
	int vl_departement=0;
	static char *vl_autoroute;
	
	switch (vm_noeud.Autoroute) {
		case 1:vl_autoroute="A8";break;
		case 2:vl_autoroute="A500";break;
		case 3:vl_autoroute="A50";break;
		case 4:vl_autoroute="A501";break;
		case 5:vl_autoroute="A51";break;
		case 6:vl_autoroute="A52";break;
		case 7:vl_autoroute="A520";break;
		case 8:vl_autoroute="A57";break;
		case 9:vl_autoroute="A7";break;
	}
	
	/* get departement */
	vl_departement=ada2_get_departement(vm_noeud.PR_Fin/(float)1000,vl_autoroute);		
	
	if(vm_noeud.Sens==XDC_SENS_SUD || vm_noeud.Sens==XDC_SENS_SORTIE_NORD || vm_noeud.Sens==XDC_SENS_ENTREE_NORD)
		sprintf(vl_cote,"D");
	else if(vm_noeud.Sens==XDC_SENS_NORD || vm_noeud.Sens==XDC_SENS_SORTIE_NORD || vm_noeud.Sens==XDC_SENS_ENTREE_NORD)
		sprintf(vl_cote,"G");
	else 
		sprintf(vl_cote,"U");
	
	sprintf(vl_str,"%02dPR%d%sC",vl_departement,vm_noeud.PR_Fin/1000,vl_cote);

	return (vl_str);
}
static char *balise_rns() {
	switch (vm_noeud.Autoroute) {
		case 1: return ("A8");break;
		case 2: return ("A500");break;
		case 3: return ("A50");break;
		case 4: return ("A501");break;
		case 5: return ("A51");break;
		case 6: return ("A52");break;
		case 7: return ("A520");break;
		case 8: return ("A57");break;
		case 9: return ("A7");break;
	}
	return (vm_noeud.RDS.rns);
}
static char *balise_drs() {
	return (vm_noeud.RDS.drs);
}
static char *balise_dfps() {
	return (vm_noeud.RDS.dfps);
}
static char *balise_ltv() {
	return ("6");
}
static char *balise_cause() {
	return (vm_noeud.Bloc_Optio.Cause);
}
static char *balise_id_evt() {
static char vl_str[20]="";
  if (vm_noeud.Evt.cle > 200)
    sprintf(vl_str,"ID_%d-%d2",vm_noeud.Evt.numero,vm_noeud.Evt.cle-200);
  else if (vm_noeud.Evt.cle > 100)
    sprintf(vl_str,"ID_%d-%d1",vm_noeud.Evt.numero,vm_noeud.Evt.cle-100);
  else
  sprintf(vl_str,"ID_%d-%d",vm_noeud.Evt.numero,vm_noeud.Evt.cle);
  return (vl_str);
}
static char *balise_qin() {
  if (vm_noeud.Bloc_Optio.Veracite!=1)
    return ("false");
  else
    return ("true");
}
static char *balise_cancel() {
  /*si FMC ANNULEE*/
  if (vm_noeud.Bloc_Optio.Fausse_Alerte=='Y')
    return ("true");
  else
    return ("false");
}
static char *balise_end() {
XZST_03EcritureTrace(XZSTC_WARNING,"end=%c",vm_noeud.Bloc_Optio.Fin);
  if (vm_noeud.Bloc_Optio.Fin =='Y')
    return ("true");
  else
    return ("false");
}
static char *balise_id() {
static char vl_str[20]="";
  sprintf(vl_str,"ID_%d_%d",vm_noeud.Evt.numero,vm_noeud.Evt.cle);
  return (vl_str);
}
static char *balise_date_courante() {
static char vl_str[50]="";
int                   vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
XZSMT_Horodate        vl_horo_chaine;
XDY_Horodate		vl_date;
int			vl_Jour_De_La_Semaine;

XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_Jour_De_La_Semaine,&vl_date);
XZSM_11ConversionHorodate(vl_date,&vl_horo_chaine);
sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d:%2d",
			&vl_annee,
			&vl_mois,		
			&vl_jour,
			&vl_heure,
			&vl_minute,
			&vl_seconde);
  sprintf(vl_str,"%04d-%02d-%02dT%02d:%02d:%02d+%02d:00",
		vl_jour,
		vl_mois,
		vl_annee,
		vl_heure,
		vl_minute,
		vl_seconde,
		get_local_UTC_hour_diff(vl_date));
  return (vl_str);	
}
static char *balise_date_debut() {
  XDY_Horodate vl_date;
  static char vl_str[50]="";
  int vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;

 XZST_03EcritureTrace(XZSTC_WARNING, "date debut =%s",vm_noeud.Bloc_Optio.Date_Debut);

 sscanf(vm_noeud.Bloc_Optio.Date_Debut, "%4d-%2d-%2dT%2d:%2d:%2d",
			&vl_annee,
			&vl_mois,
			&vl_jour,
			&vl_heure,
			&vl_minute,
			&vl_seconde);
  sprintf(vl_str,"%02d/%02d/%04d %02d:%02d:%02d",
		vl_jour,	
		vl_mois,	
		vl_annee,
		vl_heure,
		vl_minute,
		vl_seconde);

  //conversion horodate jj/mm/aaaa hh:mn:ss en secondes
  XZSM_13ConversionHorodateSec(vl_str,&vl_date);

  //Ajout heure diff avec l'utc à la date de début
  sprintf(vl_str,"%s+%02d:00",vm_noeud.Bloc_Optio.Date_Debut,get_local_UTC_hour_diff(vl_date));
  return (vl_str);
}
static char *balise_date_fin() {
  return (vm_noeud.Bloc_Optio.Date_Fin);
}
static char *balise_horodate_debut_tache(){
	return vg_horodate_debut_tache;
}

static char *balise_date_debut_prevu() {
  XDY_Horodate vl_date;
  static char vl_str[50]="";
  int vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;

 XZST_03EcritureTrace(XZSTC_WARNING, "date debut prevu =%s",vm_noeud.Bloc_Optio.Date_Debut_Prevu);

/* si date d�but pr�vue non renseign�e, return date d�but*/
 if (!strcmp(vm_noeud.Bloc_Optio.Date_Debut_Prevu,"1970-01-01T00:00:00"))
   return balise_date_debut();

 sscanf(vm_noeud.Bloc_Optio.Date_Debut_Prevu, "%4d-%2d-%2dT%2d:%2d:%2d",
			&vl_annee,
			&vl_mois,
			&vl_jour,
			&vl_heure,
			&vl_minute,
			&vl_seconde);
  sprintf(vl_str,"%02d/%02d/%04d %02d:%02d:%02d",
		vl_jour,	
		vl_mois,	
		vl_annee,
		vl_heure,
		vl_minute,
		vl_seconde);

  //conversion horodate jj/mm/aaaa hh:mn:ss en secondes
  XZSM_13ConversionHorodateSec(vl_str,&vl_date);

  //Ajout heure diff avec l'utc à la date de début
  sprintf(vl_str,"%s+%02d:00",vm_noeud.Bloc_Optio.Date_Debut_Prevu,get_local_UTC_hour_diff(vl_date));
  return (vl_str);
}

static char *balise_date_fin_prevu() {
  XDY_Horodate vl_date;
  static char vl_str[50]="";
  int vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;

 XZST_03EcritureTrace(XZSTC_WARNING, "date Fin prevu =%s",vm_noeud.Bloc_Optio.Date_Fin_Prevu);

 sscanf(vm_noeud.Bloc_Optio.Date_Fin_Prevu, "%4d-%2d-%2dT%2d:%2d:%2d",
			&vl_annee,
			&vl_mois,
			&vl_jour,
			&vl_heure,
			&vl_minute,
			&vl_seconde);
  sprintf(vl_str,"%02d/%02d/%04d %02d:%02d:%02d",
		vl_jour,	
		vl_mois,	
		vl_annee,
		vl_heure,
		vl_minute,
		vl_seconde);

  //conversion horodate jj/mm/aaaa hh:mn:ss en secondes
  XZSM_13ConversionHorodateSec(vl_str,&vl_date);

  //Ajout heure diff avec l'utc à la date de début
  sprintf(vl_str,"%s+%02d:00",vm_noeud.Bloc_Optio.Date_Fin_Prevu,get_local_UTC_hour_diff(vl_date));

 XZST_03EcritureTrace(XZSTC_WARNING, "date Fin prevu2 =%s",vl_str);
  return (vl_str);
}

static char *balise_operating_mode(){
	if (!strcmp(vg_publication_type,C_PUBLICATION_RECAPITULATIVE))
		return "operatingMode2";

	else if (!strcmp(vg_publication_type,C_PUBLICATION_COUPPARCOUP))
		return "operatingMode1";
	else
		return "";
}

static char *balise_type() {								
	if (vm_noeud.Type==XZAEC_FMC_ChausseeGliss) {
	      if ( (vm_noeud.Bloc_Optio.Bloc.Meteo.Type==XZAEC_CH_GLIS_Autre) ||
			 (vm_noeud.Bloc_Optio.Bloc.Meteo.Type==XZAEC_CH_GLIS_Huile) ||
			 (vm_noeud.Bloc_Optio.Bloc.Meteo.Type==XZAEC_CH_GLIS_Carburant) ||
			 (vm_noeud.Bloc_Optio.Bloc.Meteo.Type==XZAEC_CH_GLIS_Gasoil) ) 
		{ 
		 return (tg_types_fmc[XZAEC_FMC_FlaqueHuile-1].Type_Datex);
		}
        }
	return (tg_types_fmc[vm_noeud.Type-1].Type_Datex);
}
static char *balise_comment() {
	static char vl_libelle[255]="";
	ada2_trim_comment(vl_libelle,vm_noeud.Bloc_Optio.Comment);
	return (vl_libelle);
}
static char *balise_zz() {
static char vl_str[5]="";

sprintf(vl_str,"%d",vm_noeud.VNM);
  return (vl_str);
}
static char *balise_qualif() {
	if (vm_noeud.Bloc_Optio.Veracite!=1)
		return ("riskOf");
	else
		return ("certain");
}
static char *balise_nb_ouv() {
  int vl_nb_voies=0;
  int vl_nb_voies_cir=0;
  static char vl_str[5]="";

  if (vm_noeud.Type==XZAEC_FMC_FermetureEch) 
    return ("0");
  if ( vm_noeud.Type==XZAEC_FMC_EchDeconseille)
    return ("1");
  gen_cumul_voies( vm_noeud.VL , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VM1 , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VM2 , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VR, &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.BAU, &vl_nb_voies , &vl_nb_voies_cir);
  if ((vl_nb_voies==0) && (vl_nb_voies_cir==0))
    vl_nb_voies_cir++;
  sprintf(vl_str,"%d",vl_nb_voies_cir);
  return (vl_str);
}
static char *balise_nb_bloq() {
  int vl_nb_voies=0;
  int vl_nb_voies_cir=0;
  static char vl_str[5]="";

  if  (vm_noeud.Type==XZAEC_FMC_FermetureEch)  
    return ("1");
  if (vm_noeud.Type==XZAEC_FMC_EchDeconseille)
    return ("0");
  gen_cumul_voies( vm_noeud.VL , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VM1 , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VM2 , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VR, &vl_nb_voies , &vl_nb_voies_cir);
  sprintf(vl_str,"%d",vl_nb_voies-vl_nb_voies_cir);
  return (vl_str);
}
static char *balise_nb_tot() {
  int vl_nb_voies=0;
  int vl_nb_voies_cir=0;
  static char vl_str[5]="";

  if ( (vm_noeud.Type==XZAEC_FMC_FermetureEch) || 
       (vm_noeud.Type==XZAEC_FMC_EchDeconseille))
    return ("1");
  gen_cumul_voies( vm_noeud.VL , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VM1 , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VM2 , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VR, &vl_nb_voies , &vl_nb_voies_cir);
  if (vl_nb_voies==0) vl_nb_voies++;
  sprintf(vl_str,"%d",vl_nb_voies);
  return (vl_str);
}

static char *balise_carriage(){
  if ( (vm_noeud.Sens==XDC_SENS_ENTREE_SUD) ||
       (vm_noeud.Sens==XDC_SENS_ENTREE_NORD))
    return ("entrySlipRoad");

  if ( (vm_noeud.Sens==XDC_SENS_SORTIE_SUD) ||
       (vm_noeud.Sens==XDC_SENS_SORTIE_NORD))
    return ("exitSlipRoad");

  if ( (vm_noeud.Type==XZAEC_FMC_FermetureEch) ||
       (vm_noeud.Type==XZAEC_FMC_EchDeconseille))
    return ("slipRoads");

  return ("mainCarriageway");
}
static char *balise_location() {
  if (vm_noeud.Type_Point_Car==XDC_POINT_CARACT_AIRE)
    return ("atServiceArea");

  if (vm_noeud.Type_Point_Car==XDC_POINT_CARACT_PEAGE)
    return ("atTollPlaza");

  if (vm_noeud.Type_Point_Car==XDC_POINT_CARACT_ECHANGEUR)
    return ("onConnector");

  return ("onTheRoadway");
}

static char *balise_publication_type() 
{
	return vg_publication_type;
}

static char *balise_test_feedType_recap()
{
	if (!strcmp(vg_publication_type,C_PUBLICATION_RECAPITULATIVE))
		return "1";
	return "";
}

static char *balise_test_feedType_coupparcoup()
{
	if (!strcmp(vg_publication_type,C_PUBLICATION_COUPPARCOUP))
		return "1";
	return "";
}

static char *balise_id_regroupement() 
{
	static char vl_str[50]="";
	int                   vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
	XZSMT_Horodate        vl_horo_chaine;
	XDY_Horodate		vl_date;
	int			vl_Jour_De_La_Semaine;

	sscanf(vm_noeud.Bloc_Optio.Date_Debut, "%04d-%02d-%02dT%02d:%02d:00",
			&vl_annee,
			&vl_mois,
			&vl_jour,
			&vl_heure,
			&vl_minute);



	sprintf(vl_str,"%s-%02d%02d%02d-%d",
			C_DATEX_ESCOTA,				
			vl_annee%100,
			vl_mois,
			vl_jour,
			vm_noeud.Evt.numero
			);
			return (vl_str);
}

static char *balise_version_regroupement()
{
	static char vl_str[5]="";
	sprintf(vl_str,"%d",vm_noeud.VNM);
	return(vl_str);
}

static char *balise_gravite()
{
	return("medium");
}

static char * balise_id_regroupement_cause_chantier()
{
	static char vl_str[50]="";
	int                   vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
	XZSMT_Horodate        vl_horo_chaine;
	XDY_Horodate		vl_date;
	int			vl_Jour_De_La_Semaine;

	sscanf(vm_noeud.Bloc_Optio.Date_Debut, "%04d-%02d-%02dT%02d:%02d:00",
			&vl_annee,
			&vl_mois,
			&vl_jour,
			&vl_heure,
			&vl_minute);



	sprintf(vl_str,"%s-%02d%02d%02d-%d",
			C_DATEX_ESCOTA,				
			vl_annee%100,
			vl_mois,
			vl_jour,
			vm_noeud.Bloc_Optio.Evt_datex2Fils.numero
			);
	return (vl_str);

}

static char * balise_version_id_regroupement_cause_chantier()
{
	static char vl_str[5]="";
	sprintf(vl_str,"%d",vm_noeud.Bloc_Optio.Evt_datex2Fils_version);
	return(vl_str);
}

static char *balise_related_situation_targetClass()
{
	return("Situation");
}

static char *balise_date_validation()
{
	static char vl_str[50]="";
	int                   vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
	XZSMT_Horodate        vl_horo_chaine;
	int			vl_Jour_De_La_Semaine;

	XZSM_11ConversionHorodate(vm_noeud.HValidation,&vl_horo_chaine);
	sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d:%2d",
			&vl_annee,
			&vl_mois,		
			&vl_jour,
			&vl_heure,
			&vl_minute,
			&vl_seconde);
	sprintf(vl_str,"%04d-%02d-%02dT%02d:%02d:%2d+%02d:00",
		vl_jour,
		vl_mois,
		vl_annee,
		vl_heure,
		vl_minute,
		vl_seconde,
		get_local_UTC_hour_diff(vm_noeud.HValidation));
  return (vl_str);
}

static char *balise_id_element()
{
	static char vl_str[50]="";
	int                   vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
	XZSMT_Horodate        vl_horo_chaine;
	XDY_Horodate		vl_date;
	int			vl_Jour_De_La_Semaine;
	int indice=0;
	int vl_numero_element_attache_regroupement;

	sscanf(vm_noeud.Bloc_Optio.Date_Debut, "%04d-%02d-%02dT%02d:%02d:00",
			&vl_annee,
			&vl_mois,
			&vl_jour,
			&vl_heure,
			&vl_minute);

	/* récuperation du numéro de l'élement en cours du regroupement et incrémenter pour le prochain élement */
	// recupération de l'indice où struct[indice].numero = numero_consequence
	indice=find_indice_numero_element_attache_regroupement(vg_numero_element_regroupement,vm_noeud.Evt_Consequence.numero,vg_size_struct_numero_regroupement);
				
	// si on trouve la correspondance
	if(indice>=0)
	{
		// Incrémentation du dernier numero des elements rattachés à ce regroupement
		vl_numero_element_attache_regroupement=vg_numero_element_regroupement[indice].der_numero_element_attache_regroupement;
		vg_numero_element_regroupement[indice].der_numero_element_attache_regroupement++;
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ada2_evt : balise_id_regroupement erreur pour récupérer le numero de l'élement du regroupement=%d, taille de la struct=%d",vm_noeud.Evt.numero,vg_size_struct_numero_regroupement);
		vl_numero_element_attache_regroupement=1;
	}
	
	// Numero FMC accolé au type pour avoir un ID différent en cas de retypage de la FMC

	sprintf(vl_str,"EVE_ESCOTA-%02d%02d%02d-%d%02d%02d-1",
		vl_annee%100,	
		vl_mois,
		vl_jour,
		vm_noeud.Evt.numero,
		vm_noeud.Type,
		vm_noeud.Evt.cle
		);
	XZST_03EcritureTrace(XZSTC_WARNING,"ID_element =%s",vl_str);
	return (vl_str);
}

static char *balise_impact_voie()
{	
  int vl_nb_voies=0;
  int vl_nb_voies_cir=0;
  static char vl_str[32]="";
  int route_coupee=0;
  int route_autre_sens_coupee=0;
  int route_inexistante=0;

  /* set static char array to null */
  vl_str[0]='\0';


  /* sens 1 */
  gen_cumul_voies( vm_noeud.VL , &vl_nb_voies , &vl_nb_voies_cir);		
  gen_cumul_voies( vm_noeud.VM1 , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VM2 , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VR, &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.BAU, &vl_nb_voies , &vl_nb_voies_cir);
XZST_03EcritureTrace (XZSTC_WARNING, "impact_voie1 Evt.numero %d, nblane =%d,nblanecir=%d",vm_noeud.Evt.numero,vl_nb_voies,vl_nb_voies_cir);
  if (vl_nb_voies_cir==0 && vl_nb_voies==0) 
		route_inexistante=1;	//route inexistante
  else if (vl_nb_voies_cir==0) //Pas de voie circulable dans ce sens
		route_coupee =1;
  
  /* on test si une voie est bloqué */
  if ((vl_nb_voies-vl_nb_voies_cir)>0) 
	sprintf(vl_str,"lanesBlocked");	 
  /* on test si toutes les voies de ce sens sont bloquées */
  if (route_coupee==1)  
  {
	sprintf(vl_str,"carriagewayBlocked");
	route_autre_sens_coupee=1;
  }
  
XZST_03EcritureTrace (XZSTC_WARNING, "route_inexistante=%d,route_coupee=%d,vl_str=%s",route_inexistante,route_coupee,vl_str);

  /* sens 2 */
  vl_nb_voies=0;
  vl_nb_voies_cir=0;
  route_coupee=0;

  gen_cumul_voies( vm_noeud.VL_I , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VM1_I , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VM2_I , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VR_I, &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.BAU_I, &vl_nb_voies , &vl_nb_voies_cir);
XZST_03EcritureTrace (XZSTC_WARNING, "impact_voie2 Evt.numero %d, nblane =%d,nblanecir=%d",vm_noeud.Evt.numero,vl_nb_voies,vl_nb_voies_cir);
	if (vl_nb_voies_cir==0 && vl_nb_voies==0 && route_inexistante ==1)
		return "";	//route inexistante
	else if(vl_nb_voies_cir==0 && vl_nb_voies==0)
		route_inexistante =1;
	else if (vl_nb_voies_cir==0) //Pas de voie circulable dans ce sens
		route_coupee =1;
  		
  /* on test si une voie est bloqué et que la route de l'autre sens n'est pas coupé */
  if ((vl_nb_voies-vl_nb_voies_cir)>0 && route_autre_sens_coupee == 0)   
	sprintf(vl_str,"lanesBlocked");	  

  /* on test si toutes les voies de ce sens sont bloquées */
  if (route_coupee==1)
	sprintf(vl_str,"carriagewayBlocked");

  /* on tests si les deux sens sont coupés */
  if (route_autre_sens_coupee==1 && route_coupee==1)
	sprintf(vl_str,"roadBlocked"); 

XZST_03EcritureTrace(XZSTC_WARNING,"balise impact voie return = %s",vl_str);
  return (vl_str);
}

static char *balise_lanes()
{
  int vl_nb_voies=0;
  int vl_nb_voies_cir=0;

  gen_cumul_voies( vm_noeud.VL , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VM1 , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VM2 , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VR, &vl_nb_voies , &vl_nb_voies_cir);
  if (vl_nb_voies!=0 && vl_nb_voies_cir==0)
    return ("allLanesCompleteCarriageway");


  vl_nb_voies=0;
  vl_nb_voies_cir=0;
  gen_cumul_voies( vm_noeud.VR, &vl_nb_voies , &vl_nb_voies_cir);
  if (vl_nb_voies-vl_nb_voies_cir>0)
     return ("rightLane");

  vl_nb_voies=0;
  vl_nb_voies_cir=0;
  gen_cumul_voies( vm_noeud.VM2 , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VM1 , &vl_nb_voies , &vl_nb_voies_cir);
  if (vl_nb_voies-vl_nb_voies_cir>0)
    return ("middleLane");

  vl_nb_voies=0;
  vl_nb_voies_cir=0;																
  gen_cumul_voies( vm_noeud.VL , &vl_nb_voies , &vl_nb_voies_cir);
  if (vl_nb_voies-vl_nb_voies_cir>0)
    return ("leftLane");

  vl_nb_voies=0;
  vl_nb_voies_cir=0;
  gen_cumul_voies( vm_noeud.BAU, &vl_nb_voies , &vl_nb_voies_cir);
  if (vl_nb_voies-vl_nb_voies_cir>0)
    return ("emergencyLane");

  vl_nb_voies=0;
  vl_nb_voies_cir=0;
  gen_cumul_voies( vm_noeud.VL , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VM1 , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VM2 , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VR, &vl_nb_voies , &vl_nb_voies_cir);

XZST_03EcritureTrace (XZSTC_WARNING, "lanes Evt.numero %d, nblane =%d,nblanecir=%d",vm_noeud.Evt.numero,vl_nb_voies,vl_nb_voies_cir);

  return ("");
}
					

static char * balise_test_date_fin_prevu()
{
  if (!strcmp(vm_noeud.Bloc_Optio.Date_Fin_Prevu,"1970-01-01T00:00:00"))
	return "";
  return "1";
}

static char *balise_test_bretelle() 
{
/* longueur d'impact sur la bretelle souvent == 0, provoque un rejet de la validation xsd */
/*	if (!strcmp(balise_test_lane_restricted(),""))
	{
		if (vm_noeud.Type==XZAEC_FMC_Travaux)
		{*/
			/* test pour détermine si l'evt se passe sur une bretelle */
/*			if ((vm_noeud.Sens==XDC_SENS_ENTREE_SUD)    	||
				(vm_noeud.Sens==XDC_SENS_ENTREE_NORD)   ||
				(vm_noeud.Sens==XDC_SENS_SORTIE_SUD)    ||
				(vm_noeud.Sens==XDC_SENS_SORTIE_NORD)   ||
				(vm_noeud.Type==XZAEC_FMC_FermetureEch) ||
				(vm_noeud.Type==XZAEC_FMC_EchDeconseille)
			)
				return "1";
		}
	}*/		
	return "";
}

static char *balise_test_chantier() 
{
	XZST_03EcritureTrace (XZSTC_WARNING, "test chantier %d %d",vm_noeud.Evt.numero, vm_noeud.Bloc_Optio.Evt_datex2Fils.numero);
	/* test pour détermine si la FMC est un chantier, ne doit pas apparaitre si la fmc n'a pas de cause */
	if (vm_noeud.Type==XZAEC_FMC_Travaux && vm_noeud.Evt.numero != vm_noeud.Evt_Cause.numero)  
		return "1";
	
	return "";
}

static char *balise_test_lane_restricted() 
{
	/* test pour détermine si la FMC est un chantier */
	if (!strcmp(balise_lanes(),""))
		return "";	
	return "1";
}

static char *balise_longueur_impact()
{
	static char vl_str[10]="";

	sprintf(vl_str,"%d",vm_noeud.Longueur);
	return (vl_str);
}

static char *balise_sens()
{
	if(vm_noeud.Sens==XDC_SENS_SUD || vm_noeud.Sens==XDC_SENS_SORTIE_SUD || vm_noeud.Sens==XDC_SENS_ENTREE_SUD)
		return ("aligned");
	else if(vm_noeud.Sens==XDC_SENS_NORD || vm_noeud.Sens==XDC_SENS_SORTIE_NORD || vm_noeud.Sens==XDC_SENS_ENTREE_NORD)
		return ("opposite");
	else 
		return ("both");
}

/*****************************************/
/*      ACCIDENT                         */
/*****************************************/
static char *balise_test_accident(){

	if(!strcmp(balise_test_bus(),"1"))
		return "";
	if(!strcmp(balise_test_pl(),"1"))
		return "";
	if(!strcmp(balise_test_chargement(),"1"))
		return "";

	return "1";
}
static char *balise_total_people() {
  int nb=0;
  static char vl_str[10]="";

  nb = vm_noeud.Bloc_Optio.Bloc.Accident.Morts+
	vm_noeud.Bloc_Optio.Bloc.Accident.Blesses_Legers+
	vm_noeud.Bloc_Optio.Bloc.Accident.Blesses_Graves;
  if (nb>0) {
    sprintf(vl_str,"%d",nb);
    return (vl_str);
  }
  else
    return ("");
}

static char *balise_morts() {
static char vl_str[10]="";

  sprintf(vl_str,"%d",vm_noeud.Bloc_Optio.Bloc.Accident.Morts);
  return (vl_str);
}
static char *balise_test_morts() {
 return (vm_noeud.Bloc_Optio.Bloc.Accident.Morts>0 ? "1":"");
}
static char *balise_nb_bus() {
static char vl_str[10]="";

  sprintf(vl_str,"%d",vm_noeud.Bloc_Optio.Bloc.Accident.Cars);
  return (vl_str);
}
static char *balise_test_bus() {
 return (vm_noeud.Bloc_Optio.Bloc.Accident.Cars>0 ? "1":"");
}
static char *balise_nb_pl() {
static char vl_str[10]="";

  sprintf(vl_str,"%d",vm_noeud.Bloc_Optio.Bloc.Accident.PL);
  return (vl_str);
}
static char *balise_test_pl() {
 return (vm_noeud.Bloc_Optio.Bloc.Accident.PL>0 ? "1":"");
}
static char *balise_nb_rem() {
static char vl_str[10]="";

  sprintf(vl_str,"%d",vm_noeud.Bloc_Optio.Bloc.Accident.Rem);
  return (vl_str);
}
static char *balise_test_rem() {
 return (vm_noeud.Bloc_Optio.Bloc.Accident.Rem>0 ? "1":"");
}
static char *balise_total_veh() {
  int nb=0;
  static char vl_str[10]="";

  nb = vm_noeud.Bloc_Optio.Bloc.Accident.Vehicules+
	vm_noeud.Bloc_Optio.Bloc.Accident.Cars+
	vm_noeud.Bloc_Optio.Bloc.Accident.PL+
	vm_noeud.Bloc_Optio.Bloc.Accident.Rem+
	vm_noeud.Bloc_Optio.Bloc.Accident.Motos;
  if (nb>0) {
    sprintf(vl_str,"%d",nb);
    return (vl_str);
  }
  else
    return ("");
}
static char *balise_nb_vl() {
static char vl_str[10]="";

  sprintf(vl_str,"%d",vm_noeud.Bloc_Optio.Bloc.Accident.Vehicules);
  return (vl_str);
}
static char *balise_test_vl() {
 return (vm_noeud.Bloc_Optio.Bloc.Accident.Vehicules>0 ? "1":"");
}
static char *balise_blesses() {
	int blesses;
	static char vl_str[10]="";
	blesses=vm_noeud.Bloc_Optio.Bloc.Accident.Blesses_Graves+vm_noeud.Bloc_Optio.Bloc.Accident.Blesses_Legers;
	sprintf(vl_str,"%d",blesses);
	return (vl_str);
}
static char *balise_test_blesses() {
	int blesses;
	blesses=vm_noeud.Bloc_Optio.Bloc.Accident.Blesses_Graves + vm_noeud.Bloc_Optio.Bloc.Accident.Blesses_Legers;
	return (blesses>0 ? "1":"");
}

static char *balise_nb_motos() {
static char vl_str[10]="";

  sprintf(vl_str,"%d",vm_noeud.Bloc_Optio.Bloc.Accident.Motos);
  return (vl_str);
}
static char *balise_test_motos() {
 return (vm_noeud.Bloc_Optio.Bloc.Accident.Motos>0 ? "1":"");
}
static char *balise_chargement() {
  if (vm_noeud.Bloc_Optio.Bloc.Accident.Matiere)
    return ("hazardousMaterials");
  else
    return ("");
}
static char *balise_test_chargement() {
 return (vm_noeud.Bloc_Optio.Bloc.Accident.Matiere ? "1":"");
}
static char *balise_matiere() {
  if (vm_noeud.Bloc_Optio.Bloc.Accident.Matiere) 
    return ("hazardousMaterials");
  else
    return ("");
}
static char *balise_vip() {
  if (vm_noeud.Bloc_Optio.Bloc.Accident.VIP)
    return ("veryImportantPerson");
  else
    return ("");
}
static char *balise_escota() {
  if (vm_noeud.Bloc_Optio.Bloc.Accident.ESCOTA)
    return ("emergencyServicesPerson");
  else
    return ("");
}

/*****************************************/
/*      BASCULEMENT                      */
/*****************************************/
static char *balise_bascul_longueur() {
static char vl_str[10]="";

  sprintf(vl_str,"%d",vm_noeud.Bloc_Optio.Bloc.Basculement.Longueur);
  return (vl_str);
}

//EN COURS
static char *balise_vitesse() {
static char vl_str[10]="";
/*if (is_version_preprod()) {*/	
	XZST_03EcritureTrace (XZSTC_WARNING, "VITESSE TYPE= '%d'",vm_type);
	if (vm_type == XZAEC_FMC_Travaux){
		strcpy(vl_str,balise_trv_vitesse());
		XZST_03EcritureTrace (XZSTC_WARNING, "vitesse trv %s",vl_str);
		return (vl_str);
		}
	else if (vm_type == XZAEC_FMC_Regulation) {
		strcpy(vl_str,balise_regul_vitesse());
		XZST_03EcritureTrace (XZSTC_WARNING, "vitesse REGUL %s",vl_str);
		return (vl_str);
	}
	else
		{
		strcpy(vl_str,balise_basc_vitesse());
		XZST_03EcritureTrace (XZSTC_WARNING, "vitesse basc %s",vl_str);
			return (vl_str);
		}
	/*}*/
return (vl_str);
}
static char *balise_basc_vitesse(){
 /*if (is_version_preprod()) {*/
	 int nb = 0;
	 static char vl_str[10]="";
	//EN COURS 	

	 XZST_03EcritureTrace (XZSTC_WARNING, "vm_noeud.Evt.numero %d",vm_noeud.Evt.numero);
	 XZST_03EcritureTrace (XZSTC_WARNING, "vm_noeud.Evt.cle %d",vm_noeud.Evt.cle);
	 XZST_03EcritureTrace (XZSTC_WARNING, "vm_noeud.Type %d",vm_noeud.Type);
	 if (vm_noeud.Evt.cle > 200)
	 {
		nb = vm_noeud.Bloc_Optio.Bloc.Basculement.VitesseOpposee;
	 }
	 else
	 {
		nb = vm_noeud.Bloc_Optio.Bloc.Basculement.Vitesse;
	 }
	 
	 XZST_03EcritureTrace (XZSTC_WARNING, "nb %d",nb);
	 if (nb>0) {
		sprintf(vl_str,"%d",nb);
		return (vl_str);
	  }
	  else
		return ("");
	/*}*/
}


static char *balise_basc_vitesse_opposee(){
	 static char vl_str[10]="";
	/*if (is_version_preprod()) {*/
	 int nb = 0;
	 nb = vm_noeud.Bloc_Optio.Bloc.Basculement.VitesseOpposee;
	 if (nb>0) {
		sprintf(vl_str,"%d",nb);
		return (vl_str);
	  }
	  else
		return ("");
		/*}*/
	}

/******************************************/
/*            BOUCHON                     */
/******************************************/
static char *balise_bouchon_longueur() {
static char vl_str[10]="";
  sprintf(vl_str,"%d",abs(vm_noeud.PR_Fin-vm_noeud.PR_Debut));
  return (vl_str);
}
static char *balise_bouchon_type() {
  if (vm_noeud.Bloc_Optio.Bloc.Bouchon.Type==XZAEC_BOU_Dense)
    return ("slowTraffic");
  else if   (vm_noeud.Bloc_Optio.Bloc.Bouchon.Type==XZAEC_BOU_Ralenti)
    return ("slowTraffic");
  else if (vm_noeud.Bloc_Optio.Bloc.Bouchon.Type==XZAEC_BOU_Bouche)
    return ("stationaryTraffic");
  else if (vm_noeud.Bloc_Optio.Bloc.Bouchon.Type==XZAEC_BOU_Bloque)
    return ("stationaryTraffic");
  else if (vm_noeud.Bloc_Optio.Bloc.Bouchon.Type==XZAEC_BOU_Escargot)
    return ("slowTraffic");
  else if (vm_noeud.Bloc_Optio.Bloc.Bouchon.Type==XZAEC_BOU_Accordeon)
    return ("queuingTraffic");
  else return ("slowTraffic");
}
static char *balise_test_bouchon_accordeon()
{
	if (vm_noeud.Bloc_Optio.Bloc.Bouchon.Type==XZAEC_BOU_Accordeon)
		return "1";
	return "";
}

/**********************************************/
/*              VEH LENT                      */
/**********************************************/
static char *balise_vehl_longueur() {
static char vl_str[10]="";
  sprintf(vl_str,"%d",vm_noeud.Bloc_Optio.Bloc.VehicLent.Longueur);
  return (vl_str);
}
static char *balise_vehl_largeur() {
static char vl_str[10]="";
  sprintf(vl_str,"%d",vm_noeud.Bloc_Optio.Bloc.VehicLent.Largeur);
  return (vl_str);
}
static char *balise_vehl_poids() {
static char vl_str[10]="";
  sprintf(vl_str,"%d",vm_noeud.Bloc_Optio.Bloc.VehicLent.Poids);
  return (vl_str);
}
static char *balise_vehl_hauteur() {
static char vl_str[10]="";
  sprintf(vl_str,"%d",vm_noeud.Bloc_Optio.Bloc.VehicLent.Hauteur);
  return (vl_str);
}
static char *balise_vehl_test_largeur() {
  return (vm_noeud.Bloc_Optio.Bloc.VehicLent.Largeur > 0 ? "1" : ""); 
}
static char *balise_vehl_test_longueur() {
  return (vm_noeud.Bloc_Optio.Bloc.VehicLent.Longueur > 0 ? "1" : ""); 
}
static char *balise_vehl_test_hauteur() {
  return (vm_noeud.Bloc_Optio.Bloc.VehicLent.Hauteur > 0 ? "1" : ""); 
}
static char *balise_vehl_test_poids() {
  return (vm_noeud.Bloc_Optio.Bloc.VehicLent.Poids > 0 ? "1" : ""); 
}

/******************************************************/
/*               DELESTAGE                            */
/******************************************************/
static char *balise_itineraire() {
  return (vm_noeud.Bloc_Optio.Bloc.Delestage.Itineraire);
}





/************************************************/
/*        FERMETURE ECHANGEUR		        */
/************************************************/
static char *balise_fermeture() {
  if ( (vm_noeud.Bloc_Optio.Bloc.Echangeur.Nb_Entrees==2) &&
       (vm_noeud.Bloc_Optio.Bloc.Echangeur.Entree1) &&
       (vm_noeud.Bloc_Optio.Bloc.Echangeur.Entree2)
     )
    return ("laneOrCarriagewayClosed");

  if ( (vm_noeud.Bloc_Optio.Bloc.Echangeur.Nb_Entrees==1)  &&
       ( (vm_noeud.Bloc_Optio.Bloc.Echangeur.Entree1)||
	 (vm_noeud.Bloc_Optio.Bloc.Echangeur.Entree2)
       )
     )
    return ("laneOrCarriagewayClosed");

  if ( (vm_noeud.Bloc_Optio.Bloc.Echangeur.Nb_Sorties==2) && 
       (vm_noeud.Bloc_Optio.Bloc.Echangeur.Sortie1) &&
       (vm_noeud.Bloc_Optio.Bloc.Echangeur.Sortie2)
     )
    return ("laneOrCarriagewayClosed");

  if ( (vm_noeud.Bloc_Optio.Bloc.Echangeur.Nb_Sorties==1)  &&
       ( (vm_noeud.Bloc_Optio.Bloc.Echangeur.Sortie1)||
	 (vm_noeud.Bloc_Optio.Bloc.Echangeur.Sortie2)
       )
     )
    return ("laneOrCarriagewayClosed");
  return ("other");
}
static char *balise_entrees_fermeture() {
  XZST_03EcritureTrace (XZSTC_WARNING, "balise_entrees: (%d//%d)",
	vm_noeud.Bloc_Optio.Bloc.Echangeur.Entree1,
	vm_noeud.Bloc_Optio.Bloc.Echangeur.Entree2);
  if ( (vm_noeud.Bloc_Optio.Bloc.Echangeur.Entree1==1) ||
       (vm_noeud.Bloc_Optio.Bloc.Echangeur.Entree2==1)
     )
    return ("doNotUseEntry");
 return("");
}

static char *balise_sorties_fermeture() {
  XZST_03EcritureTrace (XZSTC_WARNING, "balise_sorties: (%d//%d)",
	vm_noeud.Bloc_Optio.Bloc.Echangeur.Sortie1,
	vm_noeud.Bloc_Optio.Bloc.Echangeur.Sortie2);
  if ( (vm_noeud.Bloc_Optio.Bloc.Echangeur.Sortie1==1) ||
      (vm_noeud.Bloc_Optio.Bloc.Echangeur.Sortie2==1)
    )
  return ("doNotUseExit");


  /*cas ou ni entree ni sortie*/
  if (!strcmp(balise_entrees_fermeture(),""))
    return ("followLocalDiversion");

return ("");
}

/************************************************/
/*	        ECHANGEUR DECONSEILLE		*/
/************************************************/

static char *balise_entrees_deconseille() {
  XZST_03EcritureTrace (XZSTC_WARNING, "balise_entrees: (%d//%d)",
	vm_noeud.Bloc_Optio.Bloc.Echangeur.Entree1,
	vm_noeud.Bloc_Optio.Bloc.Echangeur.Entree2);
  if ( (vm_noeud.Bloc_Optio.Bloc.Echangeur.Entree1==1) ||
       (vm_noeud.Bloc_Optio.Bloc.Echangeur.Entree2==1)
     )
    return ("doNotUseIntersectionOrJunction");
 return("");
}

static char *balise_sorties_deconseille() {
  XZST_03EcritureTrace (XZSTC_WARNING, "balise_sorties: (%d//%d)",
	vm_noeud.Bloc_Optio.Bloc.Echangeur.Sortie1,
	vm_noeud.Bloc_Optio.Bloc.Echangeur.Sortie2);
  if ( (vm_noeud.Bloc_Optio.Bloc.Echangeur.Sortie1==1) ||
      (vm_noeud.Bloc_Optio.Bloc.Echangeur.Sortie2==1)
    )
  return ("doNotUseIntersectionOrJunction");

  /*cas ou ni entree ni sortie*/
  if (!strcmp(balise_entrees_deconseille(),""))		
    return ("followLocalDiversion");

  return ("");
}

/****************************************/
/*INCIDENT SOUS CONCESSIONNAIRE         */
/****************************************/
static char *balise_penurie() {
  if (vm_noeud.Bloc_Optio.Bloc.Concession.Fermeture)
    return ("serviceAreaFuelStationClosed");
  else {
   switch (vm_noeud.Bloc_Optio.Bloc.Concession.Carburant) {
    case (XDC_CRB_SUPER) : return ("petrolShortage");
				break;
    case (XDC_CRB_GAZOLE) : return ("dieselShortage");
				break;								
    case (XDC_CRB_SSPB95):
    case (XDC_CRB_SSPB98) :
			return ("petrolShortage");
			break;
    case (XDC_CRB_GPL):		
			return ("lpgShortage");
			break;
    default :
      return ("fuelShortage");
      break;
   }
  }
}

/*************************************/
/*              METEO                */
/*************************************/
static char *balise_meteo_type() {
  switch (vm_noeud.Bloc_Optio.Bloc.Meteo.Type) {
    case XZAEC_MET_ChuteNeige : return ("snowfall"); break;
    case XZAEC_MET_NeigeChaus : return ("snowfall"); break;
    case XZAEC_MET_Pluie : return ("heavyRain"); break;
    case XZAEC_MET_VentFort : return ("strongWinds"); break;
    case XZAEC_MET_Brouillard : return ("fog") ; break;
    case XZAEC_MET_BrouillardGiv : return ("freezingFog"); break;
    case XZAEC_MET_Grele : return ("hail") ; break;
    case XZAEC_MET_Verglas : return ("heavyFrost") ; break;
    case XZAEC_MET_Autre : return ("badWeather"); break;
    case XZAEC_MET_GreleChaus : return ("hail"); break;
    case XZAEC_MET_ChausMouill : return ("precipitationInTheArea"); break;
    // XZAE TYPE salage n'est pas utilisé dans tagen il est transmit en traitement chaussé avec le type dans le bloc méteo
    case XZAEC_TRT_CH_Salage : return ("saltingInProgress"); break;
    case XZAEC_TRT_CH_Deneigement : return ("snowploughsInUse"); break;
	if (vm_noeud.Type==XZAEC_FMC_TraitChaussees) return ("other");
    return ("badWeather");
  }					
}
static char *balise_visibilite() {
static char vl_str[10]="";
  if (vm_noeud.Bloc_Optio.Bloc.Meteo.Visibilite==-1)
    return ("99999");
  sprintf(vl_str,"%d",vm_noeud.Bloc_Optio.Bloc.Meteo.Visibilite);
  return (vl_str);
}
/*************************************/
/*              OBSTACLE             */
/*************************************/
static char * balise_test_obstacle_animale(){

	static char vl_libelle[255]="";
	ada2_trim_comment(vl_libelle,vm_noeud.Bloc_Optio.Comment);
	XZST_03EcritureTrace(XZSTC_WARNING, "commentaire=%s",vl_libelle);
	if ( (!strcmp(vl_libelle,C_COMMENTAIRE_ANIMALE)) || (!strcmp(vl_libelle,C_COMMENTAIRE_ANIMALE2)) )
	{
		return "1";
	}
	return "";
}

/*****************************************/
/*	CHAUSSEE GLISSANTE		 */
/*****************************************/
static char *balise_test_glissante_meteo() {
	/* Test si c'est un evt meteo et pas un évnement non-meteo pour balise default meteo =slipperyRoad */
	if ( (	(vm_noeud.Bloc_Optio.Bloc.Meteo.Type == XZAEC_MET_NeigeChaus) ||
		(vm_noeud.Bloc_Optio.Bloc.Meteo.Type == XZAEC_MET_GreleChaus) ||
		(vm_noeud.Bloc_Optio.Bloc.Meteo.Type == XZAEC_MET_Verglas) 	) ||
	     (	(vm_noeud.Bloc_Optio.Bloc.Meteo.Type != XZAEC_CH_GLIS_Huile) &&
		(vm_noeud.Bloc_Optio.Bloc.Meteo.Type != XZAEC_CH_GLIS_Carburant) &&
		(vm_noeud.Bloc_Optio.Bloc.Meteo.Type != XZAEC_CH_GLIS_Autre) &&
		(vm_noeud.Bloc_Optio.Bloc.Meteo.Type != XZAEC_CH_GLIS_Gasoil))	)
		return "1";
	return "";
}

static char *balise_test_glissante_non_meteo() {
	if ((vm_noeud.Bloc_Optio.Bloc.Meteo.Type == XZAEC_CH_GLIS_Huile) ||
		(vm_noeud.Bloc_Optio.Bloc.Meteo.Type == XZAEC_CH_GLIS_Carburant) ||
		(vm_noeud.Bloc_Optio.Bloc.Meteo.Type == XZAEC_CH_GLIS_Autre) ||
		(vm_noeud.Bloc_Optio.Bloc.Meteo.Type == XZAEC_CH_GLIS_Gasoil))	
		return "1";
	return "";
}

static char *balise_glissante() {
switch (vm_noeud.Bloc_Optio.Bloc.Meteo.Type) {
	case XZAEC_CH_GLIS_Huile : return ("oilOnRoad");break;
	case XZAEC_CH_GLIS_Carburant : return ("petrolOnRoad");break;
	case XZAEC_CH_GLIS_Gasoil : return ("dieselOnRoad");break;
	case XZAEC_MET_NeigeChaus : return ("snowOnTheRoad"); break;
	case XZAEC_MET_GreleChaus : return ("wetAndIcyRoad");break;
	case XZAEC_MET_Verglas : return ("blackIce") ; break;
}
return ("slipperyRoad");
}


/*****************************************/
/*             TRAVAUX                   */
/*****************************************/
static char *balise_travaux_type() {
  if (vm_noeud.Bloc_Optio.Bloc.Travaux.Type==XZAEC_TRV_Neufs)
    return ("installationWork");
  else if (vm_noeud.Bloc_Optio.Bloc.Travaux.Type==XZAEC_TRV_Entretien)
    return ("maintenanceWork");
  else
    return ("other");
}
static char *balise_mobile() {
  if (vm_noeud.Bloc_Optio.Bloc.Travaux.Mobiles==(int)'M')
    return ("mobile");
  else
    return ("stationary");
}

static char *balise_regul_vitesse() {
  static char vl_str[10]="";
  int nb= 0;
  nb = vm_noeud.Bloc_Optio.Bloc.Regulation.Vitesse;
  if (nb>0) {
	  sprintf(vl_str,"%d",nb);
	  return (vl_str);
  }
  else
      return ("");

}

static char *balise_trv_vitesse(){
	 static char vl_str[10]="";
 /*if (is_version_preprod()) {*/
	 int nb = 0;
	 nb = vm_noeud.Bloc_Optio.Bloc.Travaux.Vitesse;

	 XZST_03EcritureTrace (XZSTC_WARNING,"NB TRV %d",nb);
	 if (nb>0) {
		sprintf(vl_str,"%d",nb);
		return (vl_str);
	  }
	  else
		return ("");
	/*}*/
  return (vl_str);
}

/*****************************************/
/*           ZONE DE STOCKAGE            */
/*****************************************/
static char *balise_stockage_demarre() {
  if (vm_noeud.Bloc_Optio.Bloc.Travaux.Mobiles==1)
    return ("true");
  else
    return ("false");
}




/***********************************************/
/*            VEHICULE                         */
/***********************************************/
static char *balise_veh_matiere() {
  XZST_03EcritureTrace (XZSTC_INFO, "balise_veh_matiere: (%s)",vm_noeud.Bloc_Optio.Bloc.Vehicule.Matiere);
  return (vm_noeud.Bloc_Optio.Bloc.Vehicule.Matiere);
}
static char *balise_veh_chargement() {
  XZST_03EcritureTrace (XZSTC_INFO, "balise_veh_chargement: (%s)",vm_noeud.Bloc_Optio.Bloc.Vehicule.Matiere);
  if (strlen(vm_noeud.Bloc_Optio.Bloc.Vehicule.Matiere)>0)
    return ("hazardousMaterials");
  else
    return ("");
}

static char *balise_veh_type() {
  XZST_03EcritureTrace (XZSTC_INFO, "balise_veh_type: (%d)",vm_noeud.Bloc_Optio.Bloc.Vehicule.TypeVehicule);
  switch (vm_noeud.Bloc_Optio.Bloc.Vehicule.TypeVehicule) {
    case XZAEC_VEH_PL : return ("lorry"); break;
    case XZAEC_VEH_VL : return ("carOrLightVehicle"); break;
    case XZAEC_VEH_Car : return ("bus"); break;
    case XZAEC_VEH_Moto : return ("twoWheeledVehicle"); break;
    case XZAEC_VEH_Remorque : return ("articulatedVehicle"); break;
  }
  return ("other");
}

static char *balise_test_true() {
	return ("ok");
}
static char *balise_test_cause() {
	return (balise_cause());
}

/***********************************************/
/*            TDP	                       */
/***********************************************/
static char *balise_TDP_siredo(){
	return (vg_TDP_code_siredo);
}

static char *balise_TDP_version_PMS(){
	return (vg_TDP_version_station);
}

static char *balise_TDP_horodate(){
	static char vl_str[50]="";
	int                   vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
	XZSMT_Horodate        vl_horo_chaine;

	/* lecture horodate pour renseigner horodate début tache */
	XZSM_11ConversionHorodate(vg_TDP_horodate,&vl_horo_chaine);
	sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
			&vl_annee,
			&vl_mois,
			&vl_jour,
			&vl_heure,
			&vl_minute);
	sprintf(vl_str,"%04d-%02d-%02dT%02d:%02d:00+%02d:00",
		vl_jour,
		vl_mois,
		vl_annee,
		vl_heure,
		vl_minute,
		get_local_UTC_hour_diff(vg_TDP_horodate));
	return(vl_str);
}

static char *balise_TDP_debit(){
	static char vl_str[50]="";
	sprintf(vl_str, "%d",vg_TDP_debit);
	return(vl_str);
}
static char *balise_TDP_taux_occupation(){
	static char vl_str[50]="";
	sprintf(vl_str, "%f",vg_TDP_taux_occupation);
	return(vl_str);
}
static char *balise_TDP_vitesse(){
	static char vl_str[50]="";
	sprintf(vl_str, "%f",vg_TDP_vitesse);
	return(vl_str);
}
static char *balise_TDP_sens_station(){
	static char vl_str[2]="";
	sprintf(vl_str, "%d",vg_TDP_sens_station);
	return(vl_str);

}

void ada2_trim_comment(char * vl_comment_out, char * vl_comment_in)
{

	int   i=0,j=0;
	strncpy(vl_comment_out,vm_noeud.Bloc_Optio.Comment,128);
	vl_comment_out[127]='\0';

	/*A on supprime les espaces de fin de ligne */
	i = strlen(vl_comment_out);
	for (j=i-1;j>=0;j--) 
	{
		if ( vl_comment_out[j] == '#' )
			vl_comment_out[j] = ' ';
	}

	for (j=i-1;j>=0;j--) 
	{
		if ( vl_comment_out[j] == 10 )
			vl_comment_out[j] = '\0';
		else if( vl_comment_out[j] == ' ' )
			vl_comment_out[j] = '\0';		
		else break;
	}
	return;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* cumul le nb de voies totales et en circulation
* a pertie de l'etat de la voie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void gen_cumul_voies(XDY_Voie x,int *pa_tot,int *pa_cir)

/*
* ARGUMENTS EN ENTREE :
*   XDY_Voie etat d'une voie de circulation numerique)
*
* ARGUMENTS EN SORTIE :
*   aucun.
*
* CODE RETOUR :
*
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   un simple test
*
------------------------------------------------------*/
{
 switch ((int)x)
        {
        case    XDC_VOIE_INCONNUE       :
        case    XDC_VOIE_INEXISTANTE    :
             break;
        case    XDC_VOIE_SENS_NORMAL    :
             (*pa_cir)++;
        case    XDC_VOIE_SENS_INVERSE   :
        case    XDC_VOIE_BLOQUEE        :
        case    XDC_VOIE_NEUTRALISEE    :
        case    XDC_VOIE_BLOQUEE_NEUTRALISEE :
             (*pa_tot)++;
        default:
                break;
        }
}

/* Fichier : @(#)agen_evt.c	1.26        Release : 1.26        Date : 11/05/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* generation du fichier strada & px-metacor d'une fmc
*
*****************************************************
* SEQUENCE D'APPEL :
* ada_evt_cpc_mise_a_jour
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

int ada_evt_cpc_mise_a_jour(XZLGT_Liste_FMC * pa_fmc, int va_indice)
{
  XDY_Horodate		vl_debut,vl_fin,vl_validation,vl_debut_prevu,vl_fin_prevu;
  XDY_Entier		vl_duree;
  XDY_Nom		vl_fausse_alerte,vl_fin_flag;
  XDY_Entier		vl_num_cause;
  XDY_Entier		vl_num_cause2;
  XDY_Octet		vl_cle_cause;
  XDY_Octet		vl_cle_cause2;
  char			vl_type_cause[26];
  XDY_Octet		vl_veracite,vl_trafic;
  XDY_Nom		vl_prevision;
  XZSMT_Horodate	vl_horo_chaine;
  int                   vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
  int			vl_annee1,vl_annee2;
  XDY_Booleen		vl_veh_esc,vl_hom_esc,vl_perso;
  XDY_Mot		vl_plan;
  XDY_Mot		vl_taille;
  char			tl_string_comment[400];
  char			tl_string[400],tl_buffer[15],tl_loc_float[15];
  int			vl_i;
  int			vl_visibilite, vl_type;
  XDY_Mot		vl_nombre;
  XDY_Booleen		vl_perception,vl_violence;
  int			vl_bebes,vl_agees,vl_animaux,vl_escota;
  char			tl_matiere[100],tl_observations[100],tl_nom[100];
  XDY_Octet		vl_octet1, vl_octet2, vl_octet3, vl_octet4, vl_octet5, vl_octet6;
  XDY_Octet		vl_octet7,vl_octet8, vl_octet9, vl_octet10, vl_octet11;
  XDY_Octet		vl_nb_entrees,vl_nb_sorties;
  XDY_Booleen		vl_bool1,vl_bool2,vl_bool3,vl_bool4;
  XDY_Booleen		vl_degats;
  XDY_Entier		vl_entier1,vl_entier2,vl_entier3,vl_entier4,vl_entier5, vl_entier6;
  XDY_Mot		vl_mot1,vl_mot2,vl_mot3,vl_mot4;
  char			vl_localisation[10];
  char                  vl_nom_autoroute[10];
  char			vl_str1[100],vl_str2[100],vl_str3[100];
  loc_rds		vl_loc;
  XDY_Entier		vl_pr,vl_pr_inf,vl_pr_sup;
  XDY_Octet		vl_sens,vl_sens_sc;
  XDY_Octet		vl_autoroute;
  XDY_Horodate	vl_Horodate;
  int vl_Jour_De_La_Semaine;
  XDY_Octet		vl_nbvoiesop;
  XDY_Octet		vl_gravite;
  char                  vl_reference[36]; /* Ajout SAGA */
  XDY_PR		vl_PR;
  XDY_PR		vl_pr1,vl_pr2,vl_pr3,vl_pr4,vl_pr5;
  XDY_PR		vl_pr6,vl_pr7,vl_pr8,vl_pr9,vl_pr10;
  XDY_PR		vl_pr11,vl_pr12,vl_pr13,vl_pr14,vl_pr15;
  XDY_PR		vl_pr16,vl_pr17,vl_pr18,vl_pr19,vl_pr20;
  XDY_Octet		vl_nb1,vl_nb2;
  XDY_Entier		vl_longueur;

  int vm_num_prec_regul=0;
  int vm_cle_prec_regul=0;
  int vm_indice_regul=0;
  
  int va_Numero;
  int va_Cle;
  int va_Type;
  int vl_pr_entier;
  int vl_pr_m;
  XDY_Entier vl_vitesse;
  XDY_Entier vl_vitesse_opposee;
  
  XDY_Entier vl_contexte;
  XDY_Entier vl_scenario;
  XDY_Entier vl_mesure;
  XDY_Entier vl_indicateur_horaire;
  char			vl_transitoire[200];
  
  va_Numero=pa_fmc[va_indice].Evt.numero;
  va_Cle=pa_fmc[va_indice].Evt.cle;
  va_Type=pa_fmc[va_indice].Type;

  XZST_03EcritureTrace(XZSTC_INTERFACE,"ada_evt_cpc_mise_a_jour: IN %d %d (%d)",
		va_Numero,
		va_Cle,
		va_Type);

  /*recherche en base les infos communes a l'evt qui doivent etre exportees*/
  /*
  horodate debut
  horodate fin
  fausse alerte
  duree
  fin evenement
  horodate validation
  numero cause
  cle cause
  type cause
  veracite
  localisation
  prevision
  */
  strcpy(vl_fin_flag,"");
  if (XZAE127_Infos_obligatoires(va_Numero,
				va_Cle,
				&vl_debut,
				&vl_fin,
				vl_fausse_alerte,
				&vl_duree,
				vl_fin_flag,
				&vl_validation,
				&vl_num_cause,
				&vl_cle_cause,
				vl_type_cause,
				&vl_octet1,
				vl_prevision,
				vl_localisation,
				&vl_entier1,
				&vl_sens,
				&vl_pr,
				&vl_autoroute,
				&vl_trafic,
				&vl_gravite,
				&vl_nbvoiesop,
				vl_reference /*Ajout SAGA*/
				) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ada_evt_cpc_mise_a_jour: pb appel a XZAE127_Infos_obligatoires %d %d",
				va_Numero,
				va_Cle);
    return (XDC_NOK);
  }
    XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour: %d a pour cause %d",
				va_Numero,vl_num_cause);
  pa_fmc[va_indice].VNM = vl_entier1;
  pa_fmc[va_indice].Bloc_Optio.Veracite=vl_octet1;
  pa_fmc[va_indice].Sens = vl_sens;
  pa_fmc[va_indice].PR_Debut = vl_pr;
  pa_fmc[va_indice].PR_Fin = vl_pr;
  pa_fmc[va_indice].Evt_Cause.numero=vl_num_cause;
  pa_fmc[va_indice].Evt_Cause.cle=vl_cle_cause;
  pa_fmc[va_indice].Autoroute=vl_autoroute;

  if (va_Type == XZAEC_FMC_Regulation) {
    vl_pr = pa_fmc[va_indice].PR_Debut;
	pa_fmc[va_indice].Bloc_Optio.Bloc.Regulation.Vitesse =  pa_fmc[va_indice].Longueur;
    if ((va_Numero == vm_num_prec_regul) && (va_Cle== vm_cle_prec_regul)) {
	/*on traite une fmc regulation "virtuelle" */
	/*on va la rattacher a la fmc regulation "vraie" (par la cause) */
	/*et lui coller une cle unique*/
	pa_fmc[va_indice].Evt_Cause.numero=vm_num_prec_regul;
	pa_fmc[va_indice].Evt_Cause.cle=vm_cle_prec_regul;
	vl_pr = pa_fmc[va_indice].PR_Debut;
        pa_fmc[va_indice].Evt.cle = 10*va_Cle+vm_indice_regul;
	vm_indice_regul++;
    }
    else  {
 	vm_num_prec_regul = va_Numero;
	vm_cle_prec_regul = va_Cle;
	vm_indice_regul=0;
    }
  }

  /*lecture de la config des voies*/
  if (XZLG60_Lire_Config_Voies(va_Numero,
			va_Cle,
			&vl_octet1,
			&vl_octet2,
			&vl_octet3,
			&vl_octet4,
			&vl_octet5,
			&vl_octet6,
			&vl_octet7,
			&vl_octet8,
			&vl_octet9,
			&vl_octet10,
			&vl_octet11,
			&vl_entier1,
			&vl_nb_entrees,
			&vl_nb_sorties)!= XDC_OK)
    XZST_03EcritureTrace(XZSTC_WARNING,"ada_evt_cpc_mise_a_jour: pb appel a XZLG60_Lire_Config_Voies %d %d",
			va_Numero,
			va_Cle);

  pa_fmc[va_indice].VR=vl_octet1;
  pa_fmc[va_indice].VM2=vl_octet2;
  pa_fmc[va_indice].VM1=vl_octet3;
  pa_fmc[va_indice].VL=vl_octet4;
  pa_fmc[va_indice].BAU=vl_octet5;
  pa_fmc[va_indice].VR_I=vl_octet6;
  pa_fmc[va_indice].VM2_I=vl_octet7;
  pa_fmc[va_indice].VM1_I=vl_octet8;
  pa_fmc[va_indice].VL_I=vl_octet9;
  pa_fmc[va_indice].BAU_I=vl_octet10;

  pa_fmc[va_indice].Type_Point_Car = vl_octet11;
  pa_fmc[va_indice].Numero_Point_Car = vl_entier1;

  switch (vl_sens)
  {
  case XDC_SENS_ENTREE_SUD :
  case XDC_SENS_SORTIE_SUD :
  case XDC_SENS_SUD : 	vl_sens_sc=XDC_SENS_SUD;
  	   		break;
  case XDC_SENS_ENTREE_NORD :
  case XDC_SENS_SORTIE_NORD :
  case XDC_SENS_NORD : 	vl_sens_sc=XDC_SENS_NORD;
  	   		break;
  default :  	vl_sens_sc=vl_sens;
  	   	break;
  }

  if (XZAE146_Recherche_Loc_Rds(
  				vl_pr,
  				vl_autoroute,
  				vl_loc.l01,
  				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,
				vl_nom_autoroute) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ada_evt_cpc_mise_a_jour: pb appel a XZAE146_Recherche_Loc_Rds %d %d %s %s %s",
				vl_pr,
				va_Cle,
				vl_str1,
				vl_str2,
				vl_nom_autoroute);
    return (XDC_NOK);
  }
  vl_i= strlen(vl_nom_autoroute)-1;
  while (vl_i > 0 && vl_nom_autoroute[vl_i]==' ' ) {
                vl_i--;
  }
  vl_nom_autoroute[vl_i+1]='\0';
  strcpy(vl_loc.rnp,vl_nom_autoroute);
XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour: autoroute %s",vl_loc.rnp);
  if ((vl_sens == XDC_SENS_SUD) || 
  	(vl_sens == XDC_SENS_ENTREE_SUD) || 
  	(vl_sens == XDC_SENS_SORTIE_SUD))
  	{
  	strcpy(vl_loc.l02,C_l02);
  	vl_i= strlen(vl_str1)-1;
 	 while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
 	 	vl_i--;
  	}
  	vl_str1[vl_i+1]='\0';
	vl_str1[49]='\0';
  	strncpy(vl_loc.l10,vl_str1,50);
  	/*vl_loc.l10[vl_i+1]='\0';*/
XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour:1 l10 %s",vl_loc.l10);
  
  	vl_i= strlen(vl_str2)-1;
  	while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  		vl_i--;
  	}
  	vl_str2[vl_i+1]='\0';
 	vl_str2[49]='\0';
	strncpy(vl_loc.l06,vl_str2,50);
  	/*vl_loc.l06[vl_i+1]='\0';*/
XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour:1 l06 %s",vl_loc.l06);
  
 
  	vl_loc.l09=vl_entier3;
  	vl_loc.l04=vl_entier4;
  	vl_loc.l03 = C_l03;
  
  	strcpy(vl_loc.l05,"positive");
	strcpy(vl_loc.drp,"positive");
  	strcpy(vl_loc.l07,tm_l07_p[vl_autoroute]);
XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour:1 l07",vl_loc.l07);
  	
  	vl_loc.l12 = vl_pr - vl_entier1;
  	vl_loc.l11 = vl_entier2 - vl_pr;
	if (vl_loc.l11<0) vl_loc.l11=-vl_loc.l11;
  }
  else if ((vl_sens == XDC_SENS_NORD) || 
  	(vl_sens == XDC_SENS_ENTREE_NORD) || 
  	(vl_sens == XDC_SENS_SORTIE_NORD))
   {
  	strcpy(vl_loc.l02,C_l02);
  	vl_i= strlen(vl_str1)-1;
 	 while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
 	 	vl_i--;
  	}
  	vl_str1[vl_i+1]='\0';
	vl_str1[49]='\0';
  	strncpy(vl_loc.l06,vl_str1,50);
  	/*vl_loc.l06[vl_i+1]='\0';*/
XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour:2 l06 %s",vl_loc.l06);
  
  	vl_i= strlen(vl_str2)-1;
  	while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  		vl_i--;
  	}
  	vl_str2[vl_i+1]='\0';
	vl_str2[49]='\0';
 	strncpy(vl_loc.l10,vl_str2,50);
  	/*vl_loc.l10[vl_i+1]='\0';*/
XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour:2 l10 %s",vl_loc.l10);
  
 
  	vl_loc.l04=vl_entier3;
  	vl_loc.l09=vl_entier4;
  	vl_loc.l03 = C_l03;
  
  	strcpy(vl_loc.l05,"negative");
	strcpy(vl_loc.drp,"negative");
  	strcpy(vl_loc.l07,tm_l07_n[vl_autoroute]);
  	
XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour:2 l07 %s",vl_loc.l07);

  	vl_loc.l11 = vl_pr - vl_entier1;
  	vl_loc.l12 = vl_entier2 - vl_pr;
	if (vl_loc.l12<0) vl_loc.l12 = -vl_loc.l12;
  }
  
  else {
  	strcpy(vl_loc.l02,C_l02);
  	vl_i= strlen(vl_str1)-1;
 	 while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
 	 	vl_i--;
  	}
  	vl_str1[vl_i+1]='\0';
  	strcpy(vl_loc.l10,vl_str1);
  	/*vl_loc.l10[vl_i+1]='\0';*/
  
  	vl_i= strlen(vl_str2)-1;
  	while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  		vl_i--;
  	}
  	vl_str2[vl_i+1]='\0';
 	vl_str2[49]='\0';
	strncpy(vl_loc.l06,vl_str2,50);
  	/*vl_loc.l06[vl_i+1]='\0';*/
  XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour:0 l06 %s",vl_loc.l06);

 
  	vl_loc.l09=vl_entier3;
  	vl_loc.l04=vl_entier4;
  	vl_loc.l03 = C_l03;
  
  	strcpy(vl_loc.l05,"positive");
	strcpy(vl_loc.drp,"unknown");
  	strcpy(vl_loc.l07,tm_l07_p[vl_autoroute]);
  	XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour:0 l07 %s",vl_loc.l07);

  	vl_loc.l12 = vl_pr - vl_entier1;
  	vl_loc.l11 = vl_entier2 - vl_pr;
	if (vl_loc.l12<0) vl_loc.l12=-vl_loc.l12;
	if (vl_loc.l11<0) vl_loc.l11=-vl_loc.l11;

  	strcpy(vl_loc.l05,"unknown");
  	strcpy(vl_loc.l07,C_l07_U);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour:0 l07 %s",vl_loc.l07);

	}
  
  vl_pr_entier=floor(vl_pr/1000)*1000;
  sprintf(vl_loc.rpip,"%d",vl_pr_entier);
  vl_pr_m= vl_pr - vl_pr_entier;
  sprintf(vl_loc.dfpp,"%d",vl_pr_m);

  strcpy(vl_loc.dfps,vl_loc.dfpp);
  strcpy(vl_loc.rpis,vl_loc.rpip);
  strcpy(vl_loc.drs,vl_loc.drp);
  strcpy(vl_loc.rns,vl_loc.rnp);
 

  vl_loc.l08=0;
  
  sprintf(pa_fmc[va_indice].RDS.l01,"%s",vl_loc.l01);
  sprintf(pa_fmc[va_indice].RDS.l02,"%s",vl_loc.l02);
  sprintf(pa_fmc[va_indice].RDS.l03,"%d",vl_loc.l03);
  sprintf(pa_fmc[va_indice].RDS.l04,"%d",vl_loc.l04);
  sprintf(pa_fmc[va_indice].RDS.l05,"%s",vl_loc.l05);
  sprintf(pa_fmc[va_indice].RDS.l06,"%s",vl_loc.l06);
  sprintf(pa_fmc[va_indice].RDS.l07,"%s",vl_loc.l07);
  sprintf(pa_fmc[va_indice].RDS.l08,"%d",vl_loc.l08);
  sprintf(pa_fmc[va_indice].RDS.l09,"%d",vl_loc.l09);
  sprintf(pa_fmc[va_indice].RDS.l10,"%s",vl_loc.l10);
  
  sprintf(pa_fmc[va_indice].RDS.l11,"%d",vl_loc.l11);
  
  sprintf(pa_fmc[va_indice].RDS.l12,"%d",vl_loc.l12);

  strcpy(pa_fmc[va_indice].RDS.rnp,vl_loc.rnp);
  strcpy(pa_fmc[va_indice].RDS.rpip,vl_loc.rpip);
  strcpy(pa_fmc[va_indice].RDS.dfpp,vl_loc.dfpp);
  strcpy(pa_fmc[va_indice].RDS.drp,vl_loc.drp);
  strcpy(pa_fmc[va_indice].RDS.rns,vl_loc.rns);
  strcpy(pa_fmc[va_indice].RDS.rpis,vl_loc.rpis);
  strcpy(pa_fmc[va_indice].RDS.dfps,vl_loc.dfps);
  strcpy(pa_fmc[va_indice].RDS.drs,vl_loc.drs);


XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour: RDS");

   
  pa_fmc[va_indice].Bloc_Optio.Fin=vl_fin_flag[0];
  pa_fmc[va_indice].Bloc_Optio.Fausse_Alerte=vl_fausse_alerte[0];
  
  sprintf(pa_fmc[va_indice].Nb_Voies_Ouvertes ,"%d", vl_nbvoiesop);

  /* Localisation sur Bretelle */

  if (((vl_sens == XDC_SENS_ENTREE_SUD) || (vl_sens == XDC_SENS_ENTREE_NORD) || (vl_sens == XDC_SENS_SORTIE_SUD) || (vl_sens == XDC_SENS_SORTIE_NORD)) && ((vl_loc.l11<100) || (vl_loc.l12<100)))
  {	/* si sur echang les 2 pt sont un seul */
	if (vl_loc.l11<vl_loc.l12)
		{
  		sprintf(pa_fmc[va_indice].RDS.l12,"%d",0);
  		sprintf(pa_fmc[va_indice].RDS.l11,"%d",0);
		sprintf(pa_fmc[va_indice].RDS.l10,"%s",vl_loc.l06);				
  		sprintf(pa_fmc[va_indice].RDS.l09,"%d",vl_loc.l04);
		}
	else
		{
  		sprintf(pa_fmc[va_indice].RDS.l12,"%d",0);
  		sprintf(pa_fmc[va_indice].RDS.l11,"%d",0);
		sprintf(pa_fmc[va_indice].RDS.l06,"%s",vl_loc.l10);				
  		sprintf(pa_fmc[va_indice].RDS.l04,"%d",vl_loc.l09);
		}
		
  }
XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour: RDS2");

  XZSM_11ConversionHorodate(vl_debut,&vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
			&vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ada_evt_cpc_mise_a_jour: format date incorrect");
    return (XDC_NOK);
  }
  /*sprintf(vl_bloc.Bloc_Optio.Date_Debut,"%04d%02d%02d%02d%02dP01",*/
  sprintf(pa_fmc[va_indice].Bloc_Optio.Date_Debut,"%04d-%02d-%02dT%02d:%02d:00",
			vl_annee,
			vl_mois,
			vl_jour,
			vl_heure,
			vl_minute);

  XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour: horodate");

  XZSM_11ConversionHorodate(vl_fin,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date1 : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ada_evt_cpc_mise_a_jour: format date incorrect");
    return (XDC_NOK);
  }
  if (vl_annee!=1970)
    /*sprintf(vl_bloc.Bloc_Optio.Date_Fin,"%04d%02d%02d%02d%02dP01",*/
  sprintf(pa_fmc[va_indice].Bloc_Optio.Date_Fin,"%04d-%02d-%02dT%02d:%02d:00",
                        vl_annee,
                        vl_mois,
                        vl_jour,
                        vl_heure,
                        vl_minute);
  else
    strcpy(pa_fmc[va_indice].Bloc_Optio.Date_Fin,"");

XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour: Date_Fin");
  XZSM_11ConversionHorodate(vl_validation,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_DEBUG1,"date : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute,
                        &vl_seconde ) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ada_evt_cpc_mise_a_jour: format date incorrect");
    return (XDC_NOK);
  }

  XZAE158_Date_prevue(va_Numero,va_Cle,&vl_debut_prevu,&vl_fin_prevu);

  XZSM_11ConversionHorodate(vl_debut_prevu,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_DEBUG1,"date debut prevu : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"agen_evt_genere_strada: format date debut prevu incorrect");
    return (XDC_NOK);
  }
  sprintf(pa_fmc[va_indice].Bloc_Optio.Date_Debut_Prevu,"%04d-%02d-%02dT%02d:%02d:00",
                        vl_annee,
                        vl_mois,
                        vl_jour,
                        vl_heure,
                        vl_minute);
  XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour: debut prevu");

  XZSM_11ConversionHorodate(vl_fin_prevu,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_DEBUG1,"date fin prevu : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"agen_evt_genere_strada: format date fin prevu incorrect");
    return (XDC_NOK);
  }
  sprintf(pa_fmc[va_indice].Bloc_Optio.Date_Fin_Prevu,"%04d-%02d-%02dT%02d:%02d:00",
                        vl_annee,
                        vl_mois,
                        vl_jour,
                        vl_heure,
                        vl_minute);
  XZST_03EcritureTrace(XZSTC_DEBUG1,"ada_evt_cpc_mise_a_jour: fin prevue");

  if (vl_prevision[0]=='Y')
    pa_fmc[va_indice].Prevision=TRUE;
  else
    pa_fmc[va_indice].Prevision=FALSE;

  sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"");		    	
  /*suivant le type d'evt, recherche les infos specifiques*/
  switch(va_Type) {
    case XZAEC_FMC_FrontiereFermee:
	strcpy(pa_fmc[va_indice].Bloc_Optio.Comment,"Frontiere fermee#\n");
	break;

    case XZAEC_FMC_Accident: 
    	vl_veh_esc = -1;
    	vl_hom_esc = -1;
    	vl_perso   = -1;
    	
	if (XZLG41_GetFmcAccident(va_Numero
						    ,va_Cle
						    ,&vl_octet1
						    ,&vl_octet5
						    ,&vl_octet2
						    ,&vl_octet6
						    ,&vl_octet7
						    ,&vl_octet3
						    ,&vl_octet4
						    ,&vl_octet8
						    ,&vl_bool1
						    ,&vl_veh_esc
						    ,&vl_hom_esc
						    ,&vl_perso
						    ,&vl_degats) ==XDC_OK) {

		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.VIP=vl_perso;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.ESCOTA=vl_hom_esc;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Vehicules=vl_octet1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.PL=vl_octet5;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Cars=vl_octet6;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Motos=vl_octet7;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Rem=vl_octet2;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Blesses=vl_octet3+vl_octet4;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Blesses_Legers=vl_octet3;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Blesses_Graves=vl_octet4;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Morts=vl_octet8;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Matiere=vl_bool1;
		XZST_03EcritureTrace(XZSTC_INFO,"XZLG41_GetFmcAccident :%d %d %d %d %d %d",
					pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Vehicules,
					pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.PL,
					pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Cars,
					pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Rem,
					pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Blesses,
					pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Morts);

		tl_string_comment[0]='\0';
		
		if (vl_veh_esc == 1) 
			strcat (tl_string_comment,"vehicule(s) ESCOTA implique(s), ");
		
		if (vl_hom_esc == 1) 
			strcat (tl_string_comment,"personne(s) ESCOTA impliquee(s), ");
		
		if (vl_perso == 1) 
			strcat (tl_string_comment,"personalites impliquee(s)");
		
		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s#\n",tl_string_comment);
		
		break;
	}
	break;

    case XZAEC_FMC_Delestage :
    case XZAEC_FMC_Deviation :
	if (XZLG42_GetFmcDelestageDeviation(va_Numero
						    ,va_Cle
						    ,tl_string_comment
						    ,&vl_entier1
						    ,vl_str1
						    ,&vl_entier2
						    ,&vl_entier3) ==XDC_OK) {
		XZST_03EcritureTrace(XZSTC_INFO,"ada_evt_cpc_mise_a_jour: DEL %s,%d,%s,%d",
						tl_string_comment,vl_entier1,vl_str1,vl_entier2);
		if (vl_entier3==pa_fmc[va_indice].PR_Debut)
		  pa_fmc[va_indice].PR_Fin = vl_entier2;
		else
		  pa_fmc[va_indice].PR_Fin = vl_entier3;
		XZLG300_trim(tl_string_comment);
		strcpy(pa_fmc[va_indice].Bloc_Optio.Bloc.Delestage.Itineraire,tl_string_comment);
		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"plan numero %d , %s#\n",
					vl_entier1,tl_string_comment);  
		}
	break;

    case XZAEC_FMC_FlaqueHuile :
	     pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Type = XZAEC_CH_GLIS_Huile;
             break;
    case XZAEC_FMC_Regulation:
	//strcpy(vm_commandes,"");
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_Jour_De_La_Semaine, &vl_Horodate);
	if (XZAE565_Lire_FMC_Regulation(va_Numero,va_Cle,(XDY_Horodate)vl_Horodate,
					&vl_entier1,
					&vl_entier2,
					&vl_entier3,
					&vl_entier4,
					&vl_entier5,
					&vl_entier6,
					vl_str1) == XDC_OK ) {
		pa_fmc[va_indice].Bloc_Optio.Bloc.Regulation.Scenario = vl_entier2;
		/*JMG 	
		pa_fmc[va_indice].Bloc_Optio.Bloc.Regulation.Contexte=vl_entier1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Regulation.Mesure=vl_entier3;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Regulation.Indicateur_horaire=vl_entier4;
		strcpy(pa_fmc[va_indice].Bloc_Optio.Bloc.Regulation.Transitoire,vl_str1);
		*/

		vl_pr = pa_fmc[va_indice].PR_Fin;
  		if (XZAE146_Recherche_Loc_Rds(pa_fmc[va_indice].PR_Fin,
  				vl_autoroute,
  				vl_loc.l01,
  				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,
				vl_nom_autoroute ) != XDC_OK) {
    			XZST_03EcritureTrace(XZSTC_WARNING,"ada_evt_cpc_mise_a_jour: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_Cle);
    			return (XDC_NOK);
  			}

                strcpy(vl_loc.rnp,vl_nom_autoroute);
                vl_pr_entier=floor(vl_pr/1000)*1000;
                sprintf(vl_loc.rpip,"%d",vl_pr_entier);
                vl_pr_m= vl_pr - vl_pr_entier;
                sprintf(vl_loc.dfpp,"%d",vl_pr_m);
                strcpy(pa_fmc[va_indice].RDS.rnp,vl_loc.rnp);
                strcpy(pa_fmc[va_indice].RDS.rpip,vl_loc.rpip);
                strcpy(pa_fmc[va_indice].RDS.dfpp,vl_loc.dfpp);
                strcpy(pa_fmc[va_indice].RDS.drp,vl_loc.drp);

  		if ((vl_sens == XDC_SENS_SUD) || 
  			(vl_sens == XDC_SENS_ENTREE_SUD) || 
  			(vl_sens == XDC_SENS_SORTIE_SUD))
  		{
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  				vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  		else if ((vl_sens == XDC_SENS_NORD) || 
  			(vl_sens == XDC_SENS_ENTREE_NORD) || 
  			(vl_sens == XDC_SENS_SORTIE_NORD))
   		{
  			vl_i= strlen(vl_str1)-1;
 	 		while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
 	 			vl_i--;
  			}
  			vl_str1[vl_i+1]='\0';
  			strcpy(vl_loc.l06,vl_str1);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier3;
  			vl_loc.l11 = vl_pr - vl_entier1;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  
  		else {
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  					vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;

  			strcpy(vl_loc.l05,"unknown");
  			strcpy(vl_loc.l07,C_l07_U);

                        strcpy(vl_loc.l05,"both");
                        strcpy(pa_fmc[va_indice].RDS.l05,vl_loc.l05);
                        strcpy(vl_loc.l07,"Deux sens");
                        strcpy(vl_loc.drp,"both");
                        strcpy(pa_fmc[va_indice].RDS.drs,vl_loc.drp);
                        strcpy(pa_fmc[va_indice].RDS.drp,vl_loc.drp);
  		}
  
     
  		sprintf(pa_fmc[va_indice].RDS.l04,"%d",vl_loc.l04);
  		sprintf(pa_fmc[va_indice].RDS.l06,"%s",vl_loc.l06);
	}
	break;

    case XZAEC_FMC_Meteo:
    case XZAEC_FMC_TraitChaussees:
    case XZAEC_FMC_ChausseeGliss:
    	pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Visibilite = -1;
    	tl_string[0]='\0';
    	tl_string_comment[0]='\0';

	if (XZLG44_GetFmcMeteo(va_Numero
						    ,va_Cle
						    ,&vl_pr
						    ,&vl_octet2
						    ,&vl_mot1
						    ,&vl_entier1
						    ,&vl_octet1
						    ,vl_str1) == XDC_OK) {
		pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Longueur=vl_entier1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Visibilite=vl_mot1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Type=vl_octet2;
		strcpy(pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Nature,vl_str1);
    		XZST_03EcritureTrace(XZSTC_INFO,"ada_evt_cpc_mise_a_jour: VISI = %d, TYPE = %d",
				pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Visibilite,
				pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Type);
		pa_fmc[va_indice].PR_Fin=vl_pr;
		switch (pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Type) {
			case XZAEC_MET_ChuteNeige :
				strcat (tl_string_comment,"Chute de Neige");
				break;
		
			case XZAEC_MET_NeigeChaus :
				strcat (tl_string_comment,"Neige sur Chaussee");
				break;
				
			case XZAEC_MET_GreleChaus :
				strcat (tl_string_comment,"Grele sur Chaussee");
				break;
			
			case XZAEC_MET_Pluie :
				strcat (tl_string_comment,"Pluie");
				break;
				
			case XZAEC_MET_VentFort :
				strcat (tl_string_comment,"Vent fort");
				break;
				
			case XZAEC_MET_Brouillard :
				strcat (tl_string_comment,"Brouillard");
				break;
				
			case XZAEC_MET_BrouillardGiv :
				strcat (tl_string_comment,"Brouillard Givrant");
				break;
				
			case XZAEC_MET_Grele :
				strcat (tl_string_comment,"Grele");
				break;
				
			case XZAEC_MET_Verglas :
				strcat (tl_string_comment,"Verglas");
				break;
				
			case XZAEC_MET_Autre :
				strcat (tl_string_comment,"Perturbation Meteo");
				break;
				
			case XZAEC_TRT_CH_Salage:
				strcat (tl_string_comment,"Salage");
				break;

			case XZAEC_TRT_CH_Deneigement:
				strcat (tl_string_comment,"Deneigement");
				break;

			case XZAEC_CH_GLIS_Huile:
				strcat (tl_string_comment,"Huile");
				break;

			case XZAEC_CH_GLIS_Carburant:
				strcat (tl_string_comment,"Carburant");
				break;

			case XZAEC_CH_GLIS_Gasoil:
				strcat (tl_string_comment,"Gasoil");
				break;
			default :
				strcat (tl_string_comment,"Perturbation meteo");
				break;
		}
		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s#\n",tl_string_comment);
		strcpy(pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Nature,tl_string_comment);
		/*vl_pr=vl_entier1;*/
  		if (XZAE146_Recherche_Loc_Rds(vl_pr,
  				vl_autoroute,
  				vl_loc.l01,
  				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,
				vl_nom_autoroute ) != XDC_OK) {
    			XZST_03EcritureTrace(XZSTC_WARNING,"ada_evt_cpc_mise_a_jour: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_Cle);
    			return (XDC_NOK);
  			}

                strcpy(vl_loc.rnp,vl_nom_autoroute);
                vl_pr_entier=floor(vl_pr/1000)*1000;
                sprintf(vl_loc.rpip,"%d",vl_pr_entier);
                vl_pr_m= vl_pr - vl_pr_entier;
                sprintf(vl_loc.dfpp,"%d",vl_pr_m);
                strcpy(pa_fmc[va_indice].RDS.rnp,vl_loc.rnp);
                strcpy(pa_fmc[va_indice].RDS.rpip,vl_loc.rpip);
                strcpy(pa_fmc[va_indice].RDS.dfpp,vl_loc.dfpp);
                strcpy(pa_fmc[va_indice].RDS.drp,vl_loc.drp);

  		if ((vl_sens == XDC_SENS_SUD) || 
  			(vl_sens == XDC_SENS_ENTREE_SUD) || 
  			(vl_sens == XDC_SENS_SORTIE_SUD))
  		{
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  				vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  		else if ((vl_sens == XDC_SENS_NORD) || 
  			(vl_sens == XDC_SENS_ENTREE_NORD) || 
  			(vl_sens == XDC_SENS_SORTIE_NORD))
   		{
  			vl_i= strlen(vl_str1)-1;
 	 		while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
 	 			vl_i--;
  			}
  			vl_str1[vl_i+1]='\0';
  			strcpy(vl_loc.l06,vl_str1);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier3;
  			vl_loc.l11 = vl_pr - vl_entier1;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  
  		else {
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  					vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;

  			strcpy(vl_loc.l05,"unknown");
  			strcpy(vl_loc.l07,C_l07_U);

                        strcpy(vl_loc.l05,"both");
                        strcpy(pa_fmc[va_indice].RDS.l05,vl_loc.l05);
                        strcpy(vl_loc.l07,"Deux sens");
                        strcpy(vl_loc.drp,"both");
                        strcpy(pa_fmc[va_indice].RDS.drs,vl_loc.drp);
                        strcpy(pa_fmc[va_indice].RDS.drp,vl_loc.drp);
  		}
  
     
  		sprintf(pa_fmc[va_indice].RDS.l04,"%d",vl_loc.l04);
  		sprintf(pa_fmc[va_indice].RDS.l06,"%s",vl_loc.l06);
  		sprintf(pa_fmc[va_indice].RDS.l11,"%d",vl_loc.l11);
		  
		break;
	}
	break;

    case XZAEC_FMC_VehMarcheArr:
    case XZAEC_FMC_Contresens:
    case XZAEC_FMC_VehArrete:
    case XZAEC_FMC_VehPanne:
    case XZAEC_FMC_VehFeu:
    
    		vl_bebes	= -1 ;
    		vl_agees	= -1 ;
    		vl_animaux	= -1 ;
    		vl_escota	= -1 ;
    		
	if ( XZLG45_GetFmcVehicule(va_Numero
						    	,va_Cle
						    	,&vl_octet1	/*type*/
							,&vl_octet2	/*nb passagers*/
							,&vl_bool3	/*bebes*/
							,&vl_bool1 	/*pers agees*/
							,&vl_bool2	/*animaux*/
							,&vl_bool4	/*veh escota*/
							,&vl_octet3	/*volume chargement*/
			   				,tl_matiere     /*code matiere*/
							,vl_str1	/*code danger*/
							,&vl_mot1	/*cde nature panne*/
							,vl_str2	/*nature panne*/
							,tl_buffer	/*positionnement*/	
							,&vl_octet4	/*roulant*/
							,&vl_octet5	/* pres mat dang*/
			   				,&vl_mot2
			   				,&vl_mot3) ==XDC_OK) {
		tl_string_comment[0]='\0';
		pa_fmc[va_indice].Bloc_Optio.Bloc.Vehicule.TypeVehicule=vl_octet1;
                tl_string[0]='\0';
                if (va_Type==XZAEC_FMC_VehMarcheArr) {
                        switch (pa_fmc[va_indice].Bloc_Optio.Bloc.Vehicule.TypeVehicule) {
                                case XZAEC_VEH_PL : sprintf(tl_string,"%s", "lorry"); break;
                                case XZAEC_VEH_VL : sprintf(tl_string,"%s",  "carOrLightVehicle"); break;
                                case XZAEC_VEH_Car : sprintf(tl_string,"%s","bus"); break;
                                case XZAEC_VEH_Moto : sprintf(tl_string,"%s",  "twoWheeledVehicle"); break;
                                case XZAEC_VEH_Remorque : sprintf(tl_string,"%s",  "articulatedVehicle"); break;
                        }
                        strcat (tl_string_comment,tl_string);
                }


		if ( tl_matiere[0] != '\0' && tl_matiere[0] != ' ' )
			sprintf(pa_fmc[va_indice].Bloc_Optio.Bloc.Vehicule.Matiere,"%s",tl_matiere);
		else
			 sprintf(pa_fmc[va_indice].Bloc_Optio.Bloc.Vehicule.Matiere,"");
		pa_fmc[va_indice].Bloc_Optio.Bloc.Vehicule.Volume=vl_octet3;
		tl_string[0]='\0';
		
		XZST_03EcritureTrace(XZSTC_INFO,"ada_evt_cpc_mise_a_jour: mot2=%d, mot3=%d, bool2=%d, bool1=%d",
					vl_mot2,vl_mot3,vl_bool2,vl_bool4);
		if (vl_mot2 > 0) {
			sprintf ( tl_string , "%d bebes implique(s)", vl_mot2);
			strcat (tl_string_comment,tl_string);
		}
		
		if (vl_mot3 > 0) {
			sprintf (tl_string, ", %d personne(s) agees impliquee(s)", vl_mot3);
			strcat (tl_string_comment,tl_string);
		}
		
		if ( vl_bool2== 1) 
			strcat (tl_string_comment,", presence d'animaux");
		
		if ( vl_bool4== 1) 
			strcat (tl_string_comment,", personne(s) d'Escota impliquee(s),");
		
		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s     ",tl_string_comment);

		break;	
	}					 
	break;


    case XZAEC_FMC_Cirque:
    case XZAEC_FMC_ConvoiExc:
    case XZAEC_FMC_ConvoiMil:
    case XZAEC_FMC_VehLent:
	/*if ( XZAE132_Lire_Fiche_VehLent_Pour_Strada(va_Numero*/
	if ( XZLG46_GetFmcConvoi(va_Numero
						    	,va_Cle
						    	,&vl_mot1
						    	,&vl_octet1
							,&vl_mot4
							,&vl_mot2
							,&vl_mot3
							,vl_str1) == XDC_OK) {
		if (va_Type==XZAEC_FMC_Cirque)
		  sprintf(pa_fmc[va_indice].Bloc_Optio.Comment,"Cirque en convoi - vitesse %dkm/h  ",vl_octet1);
		else
		  sprintf(pa_fmc[va_indice].Bloc_Optio.Comment,"Vitesse %d  ",vl_octet1);
		pa_fmc[va_indice].Bloc_Optio.Bloc.VehicLent.Longueur=vl_mot1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.VehicLent.Vitesse=vl_octet1;
                pa_fmc[va_indice].Bloc_Optio.Bloc.VehicLent.Hauteur=vl_mot4;
                pa_fmc[va_indice].Bloc_Optio.Bloc.VehicLent.Largeur=vl_mot2;
                pa_fmc[va_indice].Bloc_Optio.Bloc.VehicLent.Poids=vl_mot3;
		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s     ",vl_str1);
		break;
	}
	break;

    case XZAEC_FMC_Manifestation:
    case XZAEC_FMC_GreveESCOTA:
    case XZAEC_FMC_Escargot:
    
    		vl_nombre 	= -1;
    		vl_perception	= -1;
    		vl_violence	= -1;
    
	/*if ( XZAE134_Lire_Fiche_Manif_Pour_Strada(va_Numero*/
	if ( XZLG47_GetFmcManifestation(va_Numero
						    	,va_Cle
						    	,&vl_mot1
						    	,&vl_perception
						    	,&vl_violence) ==XDC_OK) {
						    	
		tl_string_comment[0]='\0';
		pa_fmc[va_indice].Bloc_Optio.Bloc.Manif.Nombre=vl_mot1;
		sprintf (tl_string, "Personne(s): %d ", vl_mot1);
		strcat (tl_string_comment,tl_string);
		
		if (vl_perception == XZAEC_FMC_PAS_PER) 
			strcat (tl_string_comment,", pas de perception");
		else 
			strcat (tl_string_comment,", perception maintenue");
		
		if (vl_violence == XDC_VRAI) 
			strcat (tl_string_comment,", violence");
		else
			strcat (tl_string_comment,", sans violence    ");
		
		sprintf(pa_fmc[va_indice].Bloc_Optio.Comment,"%s",tl_string_comment);
						    	
		break;
	}
	break;

    case XZAEC_FMC_Basculement:
	pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Type=-1;
	if ( XZLG48_GetFmcBasculement (va_Numero
					,va_Cle
					,&vl_entier2
					,&vl_entier1
					,&vl_octet1
					,&vl_octet2
					,&vl_octet3
					,&vl_octet4
					,&vl_octet5
					,&vl_octet6
					,&vl_octet7
					,&vl_octet8
					,&vl_octet9
					,&vl_octet10
					,&vl_vitesse
					,&vl_vitesse_opposee) == XDC_OK) {

		XZST_03EcritureTrace(XZSTC_DEBUG1,"vitesse %d, opp %d",
					vl_vitesse,vl_vitesse_opposee);
		pa_fmc[va_indice].Bloc_Optio.Bloc.Basculement.Longueur=vl_entier1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Basculement.Vitesse=vl_vitesse;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Basculement.VitesseOpposee=vl_vitesse_opposee;
		vl_pr=vl_entier2;

  		if (XZAE146_Recherche_Loc_Rds(vl_pr,
  				vl_autoroute,
  				vl_loc.l01,
  				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,
				vl_nom_autoroute ) != XDC_OK) {
    			XZST_03EcritureTrace(XZSTC_WARNING,"ada_evt_cpc_mise_a_jour: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_Cle);
    			return (XDC_NOK);
  			}

                strcpy(vl_loc.rnp,vl_nom_autoroute);
                vl_pr_entier=floor(vl_pr/1000)*1000;
                sprintf(vl_loc.rpip,"%d",vl_pr_entier);
                vl_pr_m= vl_pr - vl_pr_entier;
                sprintf(vl_loc.dfpp,"%d",vl_pr_m);
                strcpy(pa_fmc[va_indice].RDS.rnp,vl_loc.rnp);
                strcpy(pa_fmc[va_indice].RDS.rpip,vl_loc.rpip);
                strcpy(pa_fmc[va_indice].RDS.dfpp,vl_loc.dfpp);
                strcpy(pa_fmc[va_indice].RDS.drp,vl_loc.drp);

  		if ((vl_sens == XDC_SENS_SUD) || 
  			(vl_sens == XDC_SENS_ENTREE_SUD) || 
  			(vl_sens == XDC_SENS_SORTIE_SUD))
  		{
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  				vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  		else if ((vl_sens == XDC_SENS_NORD) || 
  			(vl_sens == XDC_SENS_ENTREE_NORD) || 
  			(vl_sens == XDC_SENS_SORTIE_NORD))
   		{
  			vl_i= strlen(vl_str1)-1;
 	 		while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
 	 			vl_i--;
  			}
  			vl_str1[vl_i+1]='\0';
  			strcpy(vl_loc.l06,vl_str1);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier3;
  			vl_loc.l11 = vl_pr - vl_entier1;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  
  		else {
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  					vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;

  			strcpy(vl_loc.l05,"unknown");
  			strcpy(vl_loc.l07,C_l07_U);
  		}
  
      
  		sprintf(pa_fmc[va_indice].RDS.l04,"%d",vl_loc.l04);
  		sprintf(pa_fmc[va_indice].RDS.l06,"%s",vl_loc.l06);
  		sprintf(pa_fmc[va_indice].RDS.l11,"%d",vl_loc.l11);
		
		break;
	}

	break;
    case XZAEC_FMC_BasculTravaux:
    case XZAEC_FMC_Travaux:
    case XZAEC_FMC_ZoneStockage:
	tl_string[0]='\0';
	if ( XZLG49_GetFmcTravaux(va_Numero
			,va_Cle
			,&vl_bool1
			,&vl_bool2
			,&vl_octet11
			,&vl_bool3
					,&vl_octet1
					,&vl_octet2
					,&vl_octet3
					,&vl_octet4
					,&vl_octet5
					,&vl_octet6
					,&vl_octet7
					,&vl_octet8
					,&vl_octet9
					,&vl_octet10
			,&vl_pr1
			,&vl_pr2
			,&vl_pr3
			,&vl_pr4
			,&vl_pr5
			,&vl_pr6
			,&vl_pr7
			,&vl_pr8
			,&vl_pr8
			,&vl_pr10
			,&vl_pr11
			,&vl_pr12
			,&vl_pr13
			,&vl_pr14
			,&vl_pr15
			,&vl_pr16
			,&vl_pr17
			,&vl_pr18
			,&vl_pr19
			,&vl_pr20
			,&vl_PR
			,&vl_longueur
			,&vl_vitesse) == XDC_OK) {
		pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Longueur=vl_longueur;	
		pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Vitesse=vl_vitesse;
			XZST_03EcritureTrace(XZSTC_DEBUG1,"Travaux.Vitesse %d",
					pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Vitesse);
		if (va_Type == XZAEC_FMC_ZoneStockage) {
			if (vl_bool4==1) /*(si "voies retrecies"=1)*/  {
				pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Mobiles=1;
				sprintf(pa_fmc[va_indice].Bloc_Optio.Comment,"%s","stockage demarre, ");
			}
			else {
				pa_fmc[va_indice].Type = XZAEC_FMC_Travaux;
			}
		}
		else {
			if (vl_bool1==1)
				pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Mobiles=(int)'M';
			else
				pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Mobiles=(int)'S';
			sprintf(pa_fmc[va_indice].Bloc_Optio.Comment,"");
			if (vl_bool2 == 1) {
				pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Type=XZAEC_TRV_Neufs;
				sprintf(pa_fmc[va_indice].Bloc_Optio.Comment,"%s","travaux neufs, ");
			}
			else {
				pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Type=XZAEC_TRV_Entretien;
				sprintf(pa_fmc[va_indice].Bloc_Optio.Comment,"%s","travaux d'entretien, ");
			}
		}
  		vl_i= strlen(tl_string)-1;
  		while (vl_i > 0 && tl_string[vl_i]==' ' ) {
  			vl_i--;
  		}
  		tl_string[vl_i+1]='\0';
		pa_fmc[va_indice].PR_Fin=vl_pr;
		strcat (pa_fmc[va_indice].Bloc_Optio.Comment,tl_string);
		strcat (pa_fmc[va_indice].Bloc_Optio.Comment,"#\n");
		
		/*vl_pr=vl_entier1; */ /*correction JMG 110311*/
		vl_pr = vl_PR;
  		if (XZAE146_Recherche_Loc_Rds(vl_pr,
  				vl_autoroute,
  				vl_loc.l01,
  				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,
				vl_nom_autoroute ) != XDC_OK) {
    			XZST_03EcritureTrace(XZSTC_WARNING,"ada_evt_cpc_mise_a_jour: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_Cle);
    			return (XDC_NOK);
  			}

                strcpy(vl_loc.rnp,vl_nom_autoroute);
                vl_pr_entier=floor(vl_pr/1000)*1000;
                sprintf(vl_loc.rpip,"%d",vl_pr_entier);
                vl_pr_m= vl_pr - vl_pr_entier;
                sprintf(vl_loc.dfpp,"%d",vl_pr_m);
                strcpy(pa_fmc[va_indice].RDS.rnp,vl_loc.rnp);
                strcpy(pa_fmc[va_indice].RDS.rpip,vl_loc.rpip);
                strcpy(pa_fmc[va_indice].RDS.dfpp,vl_loc.dfpp);
                strcpy(pa_fmc[va_indice].RDS.drp,vl_loc.drp);



  		if ((vl_sens == XDC_SENS_SUD) || 
  			(vl_sens == XDC_SENS_ENTREE_SUD) || 
  			(vl_sens == XDC_SENS_SORTIE_SUD))
  		{
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  				vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  		else if ((vl_sens == XDC_SENS_NORD) || 
  			(vl_sens == XDC_SENS_ENTREE_NORD) || 
  			(vl_sens == XDC_SENS_SORTIE_NORD))
   		{
  			vl_i= strlen(vl_str1)-1;
 	 		while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
 	 			vl_i--;
  			}
  			vl_str1[vl_i+1]='\0';
  			strcpy(vl_loc.l06,vl_str1);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier3;
  			vl_loc.l11 = vl_pr - vl_entier1;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  
  		else {
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  					vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;

  			strcpy(vl_loc.l05,"unknown");
  			strcpy(vl_loc.l07,C_l07_U);
  		}
  
      
  		sprintf(pa_fmc[va_indice].RDS.l04,"%d",vl_loc.l04);
  		sprintf(pa_fmc[va_indice].RDS.l06,"%s",vl_loc.l06);
  		sprintf(pa_fmc[va_indice].RDS.l11,"%d",vl_loc.l11);
		
	}


	if (va_Type==XZAEC_FMC_BasculTravaux) {
	if ( XZLG48_GetFmcBasculement (va_Numero
					,va_Cle
					,&vl_entier2
					,&vl_entier1
					,&vl_octet1
					,&vl_octet2
					,&vl_octet3
					,&vl_octet4
					,&vl_octet5
					,&vl_octet6
					,&vl_octet7
					,&vl_octet8
					,&vl_octet9
					,&vl_octet10
					,&vl_vitesse
					,&vl_vitesse_opposee) == XDC_OK) {

		XZST_03EcritureTrace(XZSTC_DEBUG1,"vitesse %d, opp %d",
					vl_vitesse,vl_vitesse_opposee);
		pa_fmc[va_indice].Bloc_Optio.Bloc.Basculement.Longueur=vl_entier1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Basculement.Vitesse=vl_vitesse;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Basculement.VitesseOpposee=vl_vitesse_opposee;
		vl_pr=vl_entier2;
	}
	}

	break;

    case XZAEC_FMC_IncSsConces:
    
	vl_bool1=0;
	vl_bool2=0;
	/*if ( XZAE133_Lire_Fiche_SousCnc_Pour_Strada(va_Numero*/
	if ( XZLG50_GetFmcSousCnc(va_Numero
						    	,va_Cle
						    	,tl_nom
						    	,&vl_bool1
						    	,&vl_bool2
							,&vl_octet1) == XDC_OK) {
		
		sprintf (tl_string, "Concessionnaire %s ",tl_nom);
						    	
		strcpy(pa_fmc[va_indice].Bloc_Optio.Bloc.Concession.Concessionnaire,tl_nom);
		pa_fmc[va_indice].Bloc_Optio.Bloc.Concession.Penurie=vl_bool2;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Concession.Fermeture=vl_bool1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Concession.Carburant=vl_octet1;
		if (vl_bool1 == 1) {
		
			strcat (tl_string, ", ferme");
		}
		if (vl_bool2 == 1) {
		
			strcat (tl_string, ", penurie de carburant");
		}
		
		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s#\n",tl_string);	
		break;
	}
	break;

    case XZAEC_FMC_QueueBouchon:
    
    	vl_type = -1;
    
	if (XZLG51_GetFmcBouchon (va_Numero
					,va_Cle
					,&vl_pr
					,&vl_mot1
					,&vl_entier1
					,&vl_num_cause2
					,&vl_cle_cause2) == XDC_OK) {
		tl_string[0]='\0';
		pa_fmc[va_indice].PR_Fin = vl_pr;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Bouchon.Longueur=vl_entier1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Bouchon.Type=vl_mot1;
		pa_fmc[va_indice].Evt_Cause.numero=vl_num_cause2;
		pa_fmc[va_indice].Evt_Cause.cle=vl_cle_cause2;
		XZST_03EcritureTrace(XZSTC_INFO,"ada_evt_cpc_mise_a_jour: PR FIN = %d, longueur = %d, cause=%d, type=%d",
			pa_fmc[va_indice].PR_Fin,
			pa_fmc[va_indice].Bloc_Optio.Bloc.Bouchon.Longueur,
			vl_num_cause2,
			pa_fmc[va_indice].Bloc_Optio.Bloc.Bouchon.Type);
		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s#\n",tl_string);

  	if (XZAE146_Recherche_Loc_Rds(vl_pr,
  				vl_autoroute,
   				vl_loc.l01,
 				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,vl_nom_autoroute ) != XDC_OK) {
    			XZST_03EcritureTrace(XZSTC_WARNING,"ada_evt_cpc_mise_a_jour: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_Cle);
    			return (XDC_NOK);
  		}
    XZST_03EcritureTrace(XZSTC_INFO,"ada_evt_cpc_mise_a_jour: XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_Cle);

                strcpy(vl_loc.rnp,vl_nom_autoroute);
                vl_pr_entier=floor(vl_pr/1000)*1000;
                sprintf(vl_loc.rpip,"%d",vl_pr_entier);
                vl_pr_m= vl_pr - vl_pr_entier;
                sprintf(vl_loc.dfpp,"%d",vl_pr_m);
                strcpy(pa_fmc[va_indice].RDS.rnp,vl_loc.rnp);
                strcpy(pa_fmc[va_indice].RDS.rpip,vl_loc.rpip);
                strcpy(pa_fmc[va_indice].RDS.dfpp,vl_loc.dfpp);
                strcpy(pa_fmc[va_indice].RDS.drp,vl_loc.drp);

  		if ((vl_sens == XDC_SENS_SUD) || 
  			(vl_sens == XDC_SENS_ENTREE_SUD) || 
  			(vl_sens == XDC_SENS_SORTIE_SUD))
  		{
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  				vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  		else if ((vl_sens == XDC_SENS_NORD) || 
  			(vl_sens == XDC_SENS_ENTREE_NORD) || 
  			(vl_sens == XDC_SENS_SORTIE_NORD))
   		{
  			vl_i= strlen(vl_str1)-1;
 	 		while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
 	 			vl_i--;
  			}
  			vl_str1[vl_i+1]='\0';
  			strcpy(vl_loc.l06,vl_str1);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier3;
  			vl_loc.l11 = vl_pr - vl_entier1;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  
  		else {
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  					vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;

  			strcpy(vl_loc.l05,"unknown");
  			strcpy(vl_loc.l07,C_l07_U);
  		}
  
      
  sprintf(pa_fmc[va_indice].RDS.l04,"%d",vl_loc.l04);
  sprintf(pa_fmc[va_indice].RDS.l06,"%s",vl_loc.l06);
  sprintf(pa_fmc[va_indice].RDS.l11,"%d",vl_loc.l11);
		break;
	}
	break;

    case XZAEC_FMC_PietonChaussee:
    case XZAEC_FMC_Obstacle :
	if (XZAE207_Lit_Nature(va_Numero,va_Cle,&vl_octet1,vl_str1) == XDC_OK) {
		XZLG300_trim(vl_str1);
		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s\n",vl_str1);	
		
	}
	break;

    case XZAEC_FMC_Salage :
    case XZAEC_FMC_AnimalErrant:
	break;
    case XZAEC_FMC_AireFermee:
	break;

    case XZAEC_FMC_FermetureEch:
    
	vl_bool1=0;
	vl_bool2=0;
	vl_bool3=0;
	vl_bool4=0;
	/*if ( XZAE139_Lire_Fiche_Echangeur_Pour_Strada(va_Numero*/
	if ( XZLG52_GetFmcFermEchangeur(va_Numero
						    	,va_Cle
						    	,&vl_bool1
						    	,&vl_bool2
						    	,&vl_bool3
						    	,&vl_bool4 ) == XDC_OK) {

		tl_string[0] = '\0';
		
		if (vl_bool4 == 1) {
			strcat (tl_string, "sens 2 sortie fermee, "); 
		}
		if (vl_bool2 == 1) {
			strcat (tl_string, "sens 2 entree fermee, "); 
		}	
		if (vl_bool3 == 1) {
			strcat (tl_string, "sens 1 sortie fermee, "); 
		}	
		if (vl_bool1 == 1) {
			strcat (tl_string, "sens 1 entree fermee "); 
		}
		pa_fmc[va_indice].Bloc_Optio.Bloc.Echangeur.Entree1=vl_bool1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Echangeur.Entree2=vl_bool2;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Echangeur.Sortie1=vl_bool3;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Echangeur.Sortie2=vl_bool4;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Echangeur.Nb_Entrees=vl_nb_entrees;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Echangeur.Nb_Sorties=vl_nb_sorties;
		}

		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s\n",tl_string);		    	
		
		break;
		

  }

   if (pa_fmc[va_indice].Evt_Cause.numero!=0) {
    sprintf(pa_fmc[va_indice].Bloc_Optio.Cause,"ID_%d_%d",
		pa_fmc[va_indice].Evt_Cause.numero,
		pa_fmc[va_indice].Evt_Cause.cle);
  }
  else {
    strcpy(pa_fmc[va_indice].Bloc_Optio.Cause,"");
  }

 strcpy(pm_comment,""); /*LCL*/
 if ( XZLG02_GetComment(va_Numero, va_Cle,ada2_evt_genere_commentaires) != XDC_OK)
 {
       XZST_03EcritureTrace(XZSTC_WARNING, "Erreur a la recuperation des commentaires FMC") ;
 }
 else if (strlen(pm_comment) != 0)
 {
 	vl_taille=strlen(pa_fmc[va_indice].Bloc_Optio.Comment)-6;
	XZST_03EcritureTrace(XZSTC_DEBUG1, "TAILLE %d",vl_taille);
	if ( vl_taille < TAILLE_SUR)
 	{
		if (vl_taille+4>=0)
		  pa_fmc[va_indice].Bloc_Optio.Comment[vl_taille+4]='\0';
		if (TAILLE_SUR-vl_taille-6>=0)
		  pm_comment[TAILLE_SUR-vl_taille-6]='\0';
 		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s%s",pa_fmc[va_indice].Bloc_Optio.Comment,pm_comment);
 	}
 }

  /*ecrit le fichier strada et envoie le sur le PC strada*/
  /*agen_xml_write2(pa_fmc[va_indice],va_Type);*/

  XZST_03EcritureTrace(XZSTC_DEBUG1, "SORTIE JMG");
  return (XDC_OK);
								
}

/* Fichier : @(#)agen_evt.c	1.26        Release : 1.26        Date : 11/05/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE agen_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* construit les commentaires
*
******************************************************
* SEQUENCE D'APPEL :
* agen_evt_genere_commentaires();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
int ada2_evt_genere_commentaires(char *va_texte)
{
int vl_i=0,vl_j=0;
int vl_taille=0;
int l=0, i=0;

  /*XZST_03EcritureTrace(XZSTC_INFO,"ses_evt_genere_commentaires: IN texte:%s",va_texte);*/
 
	 l = strlen(va_texte); 
	 for (i=0; i < l; i++)
		switch(va_texte[i]) {
                case -64 :      va_texte[i] = 'A'; break;
		case -56 :      va_texte[i] = 'A'; break;
		case -55 :      va_texte[i] = 'C'; break;
		case -23 :      va_texte[i] = 'E'; break;
		case -59 :      va_texte[i] = 'E'; break;
		case -63 :      va_texte[i] = 'E'; break;
		case -54 :      va_texte[i] = 'E'; break;
		case -52 :      va_texte[i] = 'E'; break;
		case -51 :      va_texte[i] = 'E'; break;
		case -47 :      va_texte[i] = 'I'; break;
		case -42 :      va_texte[i] = 'O'; break;
		case -44 :      va_texte[i] = 'O'; break;
		case -62 :      va_texte[i] = 'O'; break;
		case -53 :      va_texte[i] = 'U'; break;
		case -61 :      va_texte[i] = 'U'; break;
		case -80 :      va_texte[i] = ' '; break;
		case -78 :      va_texte[i] = ' '; break;
		case 34 :      va_texte[i] = ' '; break; /* " */
		case 39 :      va_texte[i] = ' '; break; /* ' */
#ifdef _HPUX_SOURCE
		case 178 :      va_texte[i] = ' '; break;
		case 180 :      va_texte[i] = ' '; break;
#endif
		default :       va_texte[i] = va_texte[i]; /*toupper(va_texte[i]);*/
		}


  /*A On enleve les blancs en fin */
  vl_i= strlen(va_texte)-1;
  while (vl_i > 0 && va_texte[vl_i]==' ' ) {
	 vl_i--;
  }
  va_texte[vl_i+1]='\0';
  for (vl_j=0; vl_j <=vl_i;vl_j++)
 	 if ((va_texte[vl_j]=='\n') || (va_texte[vl_j]=='|'))
 	 	va_texte[vl_j]=' ';
  
  /*XZST_03EcritureTrace(XZSTC_INFO,"texte:%s",va_texte);*/
  vl_i= strlen(va_texte);
  vl_taille=strlen(pm_comment);
  if (vl_i+vl_taille > TAILLE_SUR)
  	strncat(pm_comment,va_texte,TAILLE_SUR-vl_taille);
  else
  	strcat(pm_comment,va_texte);
  	
  /*XZST_03EcritureTrace(XZSTC_INFO,"comment:%s",pm_comment);*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"ada2_evt_genere_commentaires: OUT");
  return (XDC_OK);
}
	
