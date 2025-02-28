/*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : acop_evt.c				*
*************************************************
* DESCRIPTION :
* module externe de la tache passerelle generique
* 
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
*************************************************
* HISTORIQUE :
*
* Guilhou	01/01/11 : Creation 	     V1.0
* JMG		14/06/11 : prise en compte BAU dans nb voies ouverts 1.2
* VR		07/10/11 : ajout vitesse et vitesse_opposee 1.3 (DEM/1015)
* JMG		13/12/11 : cause de SpeedManagement DEM/1015 1.4
* JMG		13/12/11 : plus de speedManagement si vitesse = 0 1.5
* JMG		11/11/12 : regulation 1.6
* PNI		09/10/13 : correction de l'absence d'evt fermeture ech en cons�quence d'un autre evt 1.7
* SFO           06/10/14 : Adaptation pour compilation sous linux
* PNI		25/05/21 : Retour arri�re sur la modif des deblocage de ama 1.11
************************************************/


#include "acop_evt.h"
#include "xzao000.h"
#include "xzlg.h"

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#define CO_NB_JOUR_PURGE        3
#define CO_DELAI_FREQUENCE      360
#define CO_DELAI_PURGE          86400 * CO_NB_JOUR_PURGE
#define CO_NB_CHIFFRE           13

/* definition des constantes */
#define MY_ENCODING		"ISO-8859-1"

#define	CO_MAX_CARACTERE	80
#define CO_FIC_FREQUENCE        XDC_PATH_STRADA "/frequence.txt"

#define TAILLE 200
#define C_FREQ_PURGE		86400 * 3
#define C_TAB_SEPAR		"\t"
#define	C_FREQ_VERIF_EVT	60
#define C_CODE_ESCOTA		"ESCOTA"
#define C_DATEX_ESCOTA		"frESCOTA"

#define C_PATH 			XDC_PATH_STRADA	"/datexII"
#define C_PATH_XML		XDC_PATH_STRADA	"/xml"
#define C_PATH_PREPROD		XDC_PATH_STRADA	"/datexIIp"


#define C_MODULE	0
#define C_ATTRIBUT_GEN	1
#define C_ATTRIBUT_SPEC	2

#define C_XML_GEN_PERE		XDC_PATH_STRADA	"/xml/balisesGEN_PERE.txt"
#define C_XML_GEN_HEADER	XDC_PATH_STRADA	"/xml/balisesGEN_HEADER.txt"
#define C_XML_GEN_GLOBAL	XDC_PATH_STRADA	"/xml/balisesGEN_GLOBAL.txt"
#define C_XML_GEN		XDC_PATH_STRADA	"/xml/balisesGEN.txt"
#define C_XML_ACCIDENT		XDC_PATH_STRADA	"/xml/balisesACCIDENT.txt"
#define C_XML_AGRESSION		XDC_PATH_STRADA	"/xml/balisesAGRESSION.txt"
#define C_XML_AIRE_FERMEE	XDC_PATH_STRADA	"/xml/balisesAIRE_FERMEE.txt"
#define C_XML_ALTERC_USAGERS	XDC_PATH_STRADA	"/xml/balisesALTERC_USAGERS.txt"
#define C_XML_ANIMAL		XDC_PATH_STRADA	"/xml/balisesANIMAL.txt"
#define C_XML_AUTRE		XDC_PATH_STRADA	"/xml/balisesAUTRE.txt"
#define C_XML_BASCUL		XDC_PATH_STRADA	"/xml/balisesBASCUL.txt"
#define C_XML_BASCUL_TRV	XDC_PATH_STRADA	"/xml/balisesBASCUL_TRV.txt"
#define C_XML_BOUCHON		XDC_PATH_STRADA	"/xml/balisesBOUCHON.txt"
#define C_XML_CHAUSSEE_DET	XDC_PATH_STRADA	"/xml/balisesCHAUSSEE_DET.txt"
#define C_XML_CHAUSSEE_GLISS	XDC_PATH_STRADA	"/xml/balisesCHAUSSEE_GLISS.txt"
#define C_XML_CIRQUE		XDC_PATH_STRADA	"/xml/balisesCIRQUE.txt"
#define C_XML_CONTRESENS	XDC_PATH_STRADA	"/xml/balisesCONTRESENS.txt"
#define C_XML_CONVOI_EX		XDC_PATH_STRADA	"/xml/balisesCONVOI_EX.txt"
#define C_XML_CONVOI_MIL	XDC_PATH_STRADA	"/xml/balisesCONVOI_MIL.txt"
#define C_XML_DELESTAGE		XDC_PATH_STRADA	"/xml/balisesDELESTAGE.txt"
#define C_XML_DEVIATION		XDC_PATH_STRADA	"/xml/balisesDEVIATION.txt"
#define C_XML_EBOUL		XDC_PATH_STRADA	"/xml/balisesEBOUL.txt"
#define C_XML_ECH_DEC		XDC_PATH_STRADA	"/xml/balisesECH_DEC.txt"
#define C_XML_FERM_ECH		XDC_PATH_STRADA	"/xml/balisesFERM_ECH.txt"
#define C_XML_FLAQUE_HUILE	XDC_PATH_STRADA	"/xml/balisesFLAQUE_HUILE.txt"
#define C_XML_FRONTIERE		XDC_PATH_STRADA	"/xml/balisesFRONTIERE.txt"
#define C_XML_GREVE		XDC_PATH_STRADA	"/xml/balisesGREVE.txt"
#define C_XML_HOLDUP		XDC_PATH_STRADA	"/xml/balisesHOLDUP.txt"
#define C_XML_INCENDIE		XDC_PATH_STRADA	"/xml/balisesINCENDIE.txt"
#define C_XML_SOUS_CONC		XDC_PATH_STRADA	"/xml/balisesSOUS_CONC.txt"
#define C_XML_INONDATION	XDC_PATH_STRADA	"/xml/balisesINONDATION.txt"
#define C_XML_MALAISE		XDC_PATH_STRADA	"/xml/balisesMALAISE.txt"
#define C_XML_MANIF		XDC_PATH_STRADA	"/xml/balisesMANIF.txt"
#define C_XML_METEO		XDC_PATH_STRADA	"/xml/balisesMETEO.txt"
#define C_XML_OBJET		XDC_PATH_STRADA	"/xml/balisesOBJET.txt"
#define C_XML_ESCARGOT		XDC_PATH_STRADA	"/xml/balisesESCARGOT.txt"
#define C_XML_PIETONS		XDC_PATH_STRADA	"/xml/balisesPIETONS.txt"
#define C_XML_SALAGE		XDC_PATH_STRADA	"/xml/balisesSALAGE.txt"
#define C_XML_TRAIT_CHAUSS	XDC_PATH_STRADA	"/xml/balisesTRAIT_CHAUSS.txt"
#define C_XML_TRAVAUX		XDC_PATH_STRADA	"/xml/balisesTRAVAUX.txt"
#define C_XML_VEH_ARRETE	XDC_PATH_STRADA	"/xml/balisesVEH_ARRETE.txt"
#define C_XML_VEH_FEU		XDC_PATH_STRADA	"/xml/balisesVEH_FEU.txt"
#define C_XML_VEH_PANNE		XDC_PATH_STRADA	"/xml/balisesVEH_PANNE.txt"
#define C_XML_VEH_LENT		XDC_PATH_STRADA	"/xml/balisesVEH_LENT.txt"
#define C_XML_MARCHE_ARRIERE	XDC_PATH_STRADA	"/xml/balisesMARCHE_ARRIERE.txt"
#define C_XML_ZONE_STOCKAGE	XDC_PATH_STRADA	"/xml/balisesSTOCKAGE.txt"
#define C_XML_VITESSE	XDC_PATH_STRADA	"/xml/balisesVITESSE.txt"
#define C_XML_REGUL	XDC_PATH_STRADA	"/xml/balisesREGUL.txt"

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

#define C_BALISE_TEST_CAUSE	"fmc.presence_cause"

/*ACCIDENT*/
#define C_BALISE_TOTAL_PEOPLE	"fmc.total_people"
#define C_BALISE_TOTAL_VEH	"fmc.total_veh"
#define C_BALISE_NB_VL		"fmc.nb_vl"
#define C_BALISE_NB_BUS		"fmc.nb_bus"
#define C_BALISE_NB_PL		"fmc.nb_pl"
#define C_BALISE_NB_REM		"fmc.nb_rem"
#define C_BALISE_NB_MOTOS	"fmc.nb_motos"
#define C_BALISE_BG		"fmc.bg"
#define C_BALISE_BL		"fmc.bl"
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
#define C_BALISE_TEST_BG	"fmc.test_bg"
#define C_BALISE_TEST_BL	"fmc.test_bl"
#define C_BALISE_TEST_VL	"fmc.test_vl"
#define C_BALISE_TEST_REM	"fmc.test_rem"
#define C_BALISE_TEST_MORTS	"fmc.test_morts"


/*BASCULEMENT*/
#define C_BALISE_BASCUL_LONGUEUR	"fmc.bascul_longueur"
#define C_BALISE_BASC_VITESSE		"fmc.bascul_vitesse"
#define C_BALISE_BASC_VITESSE_OPPOSEE "fmc.bascul_vitesse_opposee"

/*BOUCHON*/
#define C_BALISE_BOUCHON_LONGUEUR	"fmc.longueurBouchon"
#define C_BALISE_BOUCHON_TYPE		"fmc.typeBouchon"


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
#define C_BALISE_FERM_ENTREES		"fmc.entrees"
#define C_BALISE_FERM_SORTIES		"fmc.sorties"

/*INCIDENT SOUS CONCESSIONNAIRE*/
#define C_BALISE_PENURIE		"fmc.penurie"


/*METEO*/
#define C_BALISE_VISIBILITE		"fmc.visi"
#define C_BALISE_METEO_TYPE		"fmc.meteo_type"

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


static char *	balise_date_courante();
static char *	balise_id();
static char *	balise_type();
static char *	balise_id_evt();
static char *	balise_zz();
static char *	balise_qualif();
static char *	balise_qin();
static char *	balise_date_debut();
static char *	balise_date_fin();
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

/*ACCIDENT*/
static char *	balise_nb_vl();
static char * 	balise_nb_bus();
static char * 	balise_nb_rem();
static char *	balise_nb_pl();
static char *	balise_nb_motos();
static char *	balise_bg();
static char *	balise_bl();
static char *	balise_morts();
static char *	balise_chargement();
static char *	balise_matiere();
static char *	balise_vip();
static char *	balise_escota();
static char * balise_test_bus();
static char * balise_test_chargement();
static char * balise_test_pl();
static char * balise_test_motos();
static char * balise_test_bg();
static char * balise_test_bl();
static char * balise_test_vl();
static char * balise_test_rem();
static char * balise_test_morts();
static char * balise_total_people();
static char * balise_total_veh();

/*BASCULEMENT*/
static char *	balise_bascul_longueur();
static char *	balise_basc_vitesse();
static char *	balise_basc_vitesse_opposee();
static char *	balise_vitesse();

/*BOUCHON*/
static char *	balise_bouchon_type();
static char *	balise_bouchon_longueur();

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
static char *	balise_entrees();
static char *	balise_sorties();

/*INCIDENT SOUS CONCESSIONNAIRE*/
static char *	balise_penurie();

/*METEO*/
static char *	balise_visibilite();
static char *	balise_meteo_type();

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
/*REGULATION*/
static char *balise_regul_vitesse();

void gen_cumul_voies(XDY_Voie, int *, int *);
XDY_Evt gen_recherche_pere(XZLGT_Liste_FMC *, int);
XDY_Booleen gen_a_traiter(XZLGT_Liste_FMC *, int);

#define NBR_PREMIER 	53
#define NBR_ELT_TABLE	59

#define C_l01		"F32"
#define C_l02		"LOC"
#define C_l03		4
#define C_l07_P		"Aix en Provence - Frontiere Italienne"
#define C_l07_N		"Frontiere italienne - Aix en Provence"
#define C_l07_U		"Sens non defini"

#define C_FMC_VITESSE	XZAEC_FMC_NB_TYP+1

#define TAILLE_SUR	350

/* d�finition des variables globales � l'application */

/* definition des variables locales au module */
XZLGT_Liste_FMC		*tg_fmc;
XZLGT_Liste_FMC		*tg_fmc_courant;

xmlChar *ConvertInput(const char *in, const char *encoding);


static char			*tm_l07_p[]={
	"",
	"Aix en Provence - Frontiere Italienne",
	"A8 - Monaco",
	"Marseille - Toulon",
	"Marseille - A52",
	"Aix - Les Alpes",
	"Aix - Aubagne",
	"A52 - Auriol",
	"Le Luc - Toulon"};

static char			*tm_l07_n[]={
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
	{	XZAEC_FMC_Accident,		"ACC",	C_XML_ACCIDENT	,	"Accident"},
	{	XZAEC_FMC_Delestage,		"ROU"	, C_XML_DELESTAGE,	"ReroutingManagement"},
	{	XZAEC_FMC_Deviation,		"ROU"	, C_XML_DEVIATION,	"ReroutingManagement"},
	{	XZAEC_FMC_PosteOperateur,	"OPA"	, "",			""},
	{	XZAEC_FMC_Meteo,		"FOG"	, C_XML_METEO,		"PoorEnvironmentConditions"},
	{	XZAEC_FMC_Contresens,		"INC"	, C_XML_CONTRESENS,	"VehicleObstruction"},
	{	XZAEC_FMC_VehArrete,		"INC"	, C_XML_VEH_ARRETE,	"VehicleObstruction"},
	{	XZAEC_FMC_VehPanne,		"INC"	, C_XML_VEH_PANNE,	"VehicleObstruction"},
	{	XZAEC_FMC_VehFeu,		"INC"	, C_XML_VEH_FEU,	"VehicleObstruction"},
	{	XZAEC_FMC_Cirque,		"INC"	, C_XML_CIRQUE,		"VehicleObstruction"},
	{	XZAEC_FMC_ConvoiExc,		"INC"	, C_XML_CONVOI_EX,	"VehicleObstruction"},
	{	XZAEC_FMC_ConvoiMil,		"INC"	, C_XML_CONVOI_MIL,	"VehicleObstruction"},
	{	XZAEC_FMC_VehLent,		"INC"	, C_XML_VEH_LENT,	"VehicleObstruction"},
	{	XZAEC_FMC_Manifestation,	"ACT"	, C_XML_MANIF,		"DisturbanceActivity"},
	{	XZAEC_FMC_GreveESCOTA,		"INC"	, C_XML_GREVE,		"DisturbanceActivity"},
	{	XZAEC_FMC_Escargot,		"INC"	, C_XML_ESCARGOT,	"DisturbanceActivity"},
	{	XZAEC_FMC_Basculement,		"RMT"	, C_XML_BASCUL,		"RoadOrCarriagewayOrLaneManagement"},
	{	XZAEC_FMC_Travaux,		"RMT"	, C_XML_TRAVAUX,	"MaintenanceWorks"},
	{	XZAEC_FMC_IncSsConces,		"INC"	, C_XML_SOUS_CONC,	"RoadsideServiceDisruption"},
	{	XZAEC_FMC_QueueBouchon,		"LOS"	, C_XML_BOUCHON,	"AbnormalTraffic"},
	{	XZAEC_FMC_FermetureEch,		"RES"	, C_XML_FERM_ECH,	"ReroutingManagement"},
	{	XZAEC_FMC_Eboulement,		"INC"	, C_XML_EBOUL,		"EnvironmentalObstruction"},
	{	XZAEC_FMC_Incendie,		"INC"	, C_XML_INCENDIE,	"EnvironmentalObstruction"},
	{	XZAEC_FMC_Inondation,		"INC"	, C_XML_INONDATION,	"EnvironmentalObstruction"},
	{	XZAEC_FMC_Obstacle,		"OHZ"	, C_XML_OBJET,		"GeneralObstruction"},
	{	XZAEC_FMC_ChausseDeter,		"INC"	, C_XML_CHAUSSEE_DET,	"InfrastructureDamageObstruction"},
	{	XZAEC_FMC_FlaqueHuile,		"SHZ"	, C_XML_FLAQUE_HUILE,	"NonWeatherRelatedRoadConditions"},
	{	XZAEC_FMC_AnimalErrant,		"INC"	, C_XML_ANIMAL,		"AnimalPresenceObstruction"},
	{	XZAEC_FMC_FrontiereFermee,	"INC"	, C_XML_FRONTIERE,	"RoadOrCarriagewayOrLaneManagement"},
	{	XZAEC_FMC_AltercationUsager,	"INC"	, C_XML_ALTERC_USAGERS,	"DisturbanceActivity"},
	{	XZAEC_FMC_Agression,		"INC"	, C_XML_AGRESSION,	"DisturbanceActivity"},
	{	XZAEC_FMC_MalaiseUsager,	"INC"	, C_XML_MALAISE,	"DisturbanceActivity"},
	{	XZAEC_FMC_EvtHorsReseau,	"INC"	, C_XML_AUTRE,		"GeneralObstruction"},
	{	XZAEC_FMC_AireFermee,		"INC"	, C_XML_AIRE_FERMEE,	"RoadsideServiceDisruption"},
	{	XZAEC_FMC_Salage,		"INC"	, C_XML_SALAGE,		"MaintenanceWorks"},
	{	XZAEC_FMC_HoldUpVol,		"INC"	, C_XML_HOLDUP,		"DisturbanceActivity"},
	{	XZAEC_FMC_TeteBouchon,		"INC"	, "",			""},
	{	XZAEC_FMC_PietonChaussee,	"INC"	, C_XML_PIETONS,	"GeneralObstruction"},
	{	XZAEC_FMC_Autre,		"INC"	, C_XML_AUTRE,		"GeneralObstruction"},
	{	XZAEC_FMC_VehMarcheArr,		"INC"	, C_XML_MARCHE_ARRIERE,	"GeneralObstruction"},
	{	XZAEC_FMC_TraitChaussees,	"INC"	, C_XML_TRAIT_CHAUSS,	"MaintenanceWorks"},
	{	XZAEC_FMC_EchDeconseille,	"RES"	, C_XML_ECH_DEC,	"ReroutingManagement"},
	{	XZAEC_FMC_ChausseeGliss,	"SHZ"	, C_XML_CHAUSSEE_GLISS,	"WeatherRelatedRoadConditions"},
	{	XZAEC_FMC_BasculTravaux,	"RMT"	, C_XML_BASCUL_TRV,	"RoadOrCarriagewayOrLaneManagement"},
	{	XZAEC_FMC_ZoneStockage,		"RES"	, C_XML_ZONE_STOCKAGE,	"RoadOrCarriagewayOrLaneManagement"},
	{       XZAEC_FMC_Regulation,         "RES"   , C_XML_REGUL,	"SpeedManagement"},
	{	C_FMC_VITESSE,		"RES"	, C_XML_VITESSE,	"SpeedManagement"}
};

static char pm_comment[TAILLE_SUR+1]="";
 
static char *	version = "@(#)acop_evt.c	1.26 11/05/07 : commentaire" ;


static int vm_frequence;
static char chemin_export[80];
static int vm_type;

#define C_PIPE	"\t"
#define C_VALEUR_PREDICAT 	1
#define C_VALEUR_DIRECTE	0

typedef struct {
	char *     (*Fct)();
	} T_PREDICAT;

typedef struct S_NOEUD {
	char 	Libelle[100];
	char	BaliseXML[100];
	int	Type;
	char	Valeur[255];
	int	SousAttribut;
	T_PREDICAT Predicat;
	T_PREDICAT Predicat_Test;
	int	TypeValeur;
	XDY_Booleen	Generique;
	struct S_NOEUD	*Attribut;
	struct S_NOEUD	*FilsAine;
	struct S_NOEUD	*FrereCadet;
	struct S_NOEUD	*Pere;
} T_NOEUD;

typedef struct S_LISTE {
	char	LibellePere[100];
	T_NOEUD	*PositionArbre;
	struct S_LISTE *Suivant;
} T_LISTE;

static T_NOEUD *vg_arbre[XZAEC_FMC_NB_TYP+4];
static T_LISTE	*vg_peres=NULL;

XZLGT_Liste_FMC	vm_noeud;

int vm_indice;

/* declaration de fonctions internes */
void	acop_evt_purge();
void	acop_evt_cb_validation_fmc();
void acop_xml_write2(XZLGT_Liste_FMC, int, char * );
/* definition de fonctions externes */




/* Fichier : @(#)acop_evt.c	1.26        Release : 1.26        Date : 11/05/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* purge
******************************************************
* SEQUENCE D'APPEL :
* acop_evt_purge
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

void acop_evt_purge()
{
  return ;
}

//Fonction qui genere les fichiers Datex vitesse et vitesse_opposee
void acop_xml_traite_travaux_basc(XZLGT_Liste_FMC pa_fmc) {
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

	acop_xml_write2(pl_fmc1,pl_fmc1.Type,""); /*genere le fichier xml de speedmanagement*/
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
		acop_xml_write2(pl_fmc1,pl_fmc1.Type,""); /*genere le fichier xml*/
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
		
		acop_xml_write2(pl_fmc2,pl_fmc2.Type,""); /*genere le fichier xml*/

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

void acop_traite_ech(XZLGT_Liste_FMC pa_fmc, int va_mode) {
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
    acop_xml_write2(pl_fmc1,pa_fmc.Type,"");
    acop_xml_write2(pl_fmc2,pa_fmc.Type,"");

  }
  
  else {
    if (va_mode==0){
      acop_xml_write2(pa_fmc,pa_fmc.Type,"");

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



/* Fichier : @(#)acop_evt.c	1.26        Release : 1.26        Date : 11/05/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* reveil pour verifier qu'il y a des fmc a exporter
*****************************************************
* SEQUENCE D'APPEL :
* acop_evt_cb_validation_fmc
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
void acop_evt_cb_validation_fmc()
{
  XZLGT_Liste_FMC	*pl_liste_fmc;
  XZLGT_Liste_FMC	*pl_ex_fmc;
  XDY_Entier	vl_val;
  XDY_Evt vl_evt;
  int 		vl_JourSemaine;
  double 	vl_HorodateSec; 
  char tl_str[300];
  int vl_jour,vl_mois,vl_annee,vl_heure,vl_minute,vl_seconde;
  int vl_num;
  char vl_horodate[30];
  XZSMT_Horodate  vl_HorodateZero = "";
  char pl_fin_fichier[50];
XZSMT_CleProg           vl_cle;
XZSCT_Datagroup         vl_dg ;


  XZST_03EcritureTrace(XZSTC_FONCTION,"acop_evt_cb_validation_fmc: debut de fonction !");

  /*verrouille la memoire*/
  XZLG500_SemCall(-1,XZLGC_SEM_FMC);

  pl_liste_fmc=NULL;
  pl_liste_fmc=XZLG985_Get_FMC();

  /*deverrouille la memoire*/
  XZLG500_SemCall(1,XZLGC_SEM_FMC);


  for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) {
    XZST_03EcritureTrace(XZSTC_FONCTION,"acop_evt_cb_validation_fmc : %d %d",
				pl_liste_fmc[vl_val].Evt.numero,
				pl_liste_fmc[vl_val].Evt.cle);
    if (pl_liste_fmc[vl_val].Evt.numero==0)
      break;
 
    XZLG10_Memorise_Diffusion_FMC(pl_liste_fmc[vl_val].Evt.numero,
				pl_liste_fmc[vl_val].Evt.cle,
				XZLGC_DIFF_CLIENT_2);

    
  /*  if (!is_version_preprod()) {
	if (pl_liste_fmc[vl_val].Type==XZAEC_FMC_Regulation)
	  pl_liste_fmc[vl_val].Bloc_Optio.Fin='Y';
    }*/
  }

  /*recherche du pere originel de chaque FMc*/
  for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) {
    if (pl_liste_fmc[vl_val].Evt.numero==0)
      break;
    pl_liste_fmc[vl_val].Evt_Consequence.numero = -1;
    vl_evt = gen_recherche_pere(pl_liste_fmc, vl_val);
    pl_liste_fmc[vl_val].Evt_Consequence = vl_evt;
  }

  /*on genere maintenant les fichiers pour chaque pere*/
  for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) {
    if (pl_liste_fmc[vl_val].Evt.numero==0)
      break;

    vm_type = pl_liste_fmc[vl_val].Type;
    /*
    if (gen_a_traiter(pl_liste_fmc, vl_val) {*/
    if (!(pl_liste_fmc[vl_val].Prevision) && (pl_liste_fmc[vl_val].Bloc_Optio.Fin!='Y')) {
      if ( (pl_liste_fmc[vl_val].Evt_Consequence.numero == pl_liste_fmc[vl_val].Evt.numero) &&
         (pl_liste_fmc[vl_val].Evt_Consequence.cle == pl_liste_fmc[vl_val].Evt.cle))   {
        acop_xml_write2(pl_liste_fmc[vl_val],XZAEC_FMC_NB_TYP+2,"sit");

	/*ne pas generer de fils pour ferm ech et ech deconseille en direction unknown*/
	if ( (   (pl_liste_fmc[vl_val].Type==XZAEC_FMC_FermetureEch) || 
	         (pl_liste_fmc[vl_val].Type==XZAEC_FMC_EchDeconseille)
             ) 
	     &&
	     (
		(!strcmp(pl_liste_fmc[vl_val].RDS.drp,"unknown"))
	     )
	   ) {
	   XZST_03EcritureTrace(XZSTC_FONCTION,"PAS DE FICHIER FILS GENERE"); 
	}
	else {
		acop_xml_write2(pl_liste_fmc[vl_val], pl_liste_fmc[vl_val].Type,
						"");
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

  /*on genere maiNTenant les fichiers DatexII pour les fils*/
  for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) {
    if (pl_liste_fmc[vl_val].Evt.numero==0)
      break;
    vm_type = pl_liste_fmc[vl_val].Type;
    if (!(pl_liste_fmc[vl_val].Prevision) && (pl_liste_fmc[vl_val].Bloc_Optio.Fin!='Y')) {
    XZST_03EcritureTrace(XZSTC_FONCTION,"FILS %d %d de PERE %d %d -- %d",
			pl_liste_fmc[vl_val].Evt.numero,
			pl_liste_fmc[vl_val].Evt.cle,
			pl_liste_fmc[vl_val].Evt_Consequence.numero,
			pl_liste_fmc[vl_val].Evt_Consequence.cle, 
			((pl_liste_fmc[vl_val].Evt_Consequence.numero==pl_liste_fmc[vl_val].Evt.numero) && (pl_liste_fmc[vl_val].Evt_Consequence.cle ==pl_liste_fmc[vl_val].Evt.cle)));

    if  ( ( (pl_liste_fmc[vl_val].Evt_Consequence.numero != pl_liste_fmc[vl_val].Evt.numero) ||
	  (pl_liste_fmc[vl_val].Evt_Consequence.cle != pl_liste_fmc[vl_val].Evt.cle)) 
	  ||
	 ( (pl_liste_fmc[vl_val].Type==XZAEC_FMC_FermetureEch) ||
		    (pl_liste_fmc[vl_val].Type==XZAEC_FMC_EchDeconseille)
	 )
	||
	(pl_liste_fmc[vl_val].Type==XZAEC_FMC_Travaux)
	||
		(pl_liste_fmc[vl_val].Type==XZAEC_FMC_Basculement)
	||
		(pl_liste_fmc[vl_val].Type==XZAEC_FMC_BasculTravaux)
	)
	 {
      /*traitement particulier ferm ech et ech deconseille*/
      if ( (pl_liste_fmc[vl_val].Type==XZAEC_FMC_FermetureEch) ||
	   (pl_liste_fmc[vl_val].Type==XZAEC_FMC_EchDeconseille)) {
	  XZST_03EcritureTrace(XZSTC_WARNING,"ECH genere les 2 fils de %d %d",
				pl_liste_fmc[vl_val].Evt.numero,
				pl_liste_fmc[vl_val].Evt.cle);

/*	   acop_traite_ech(pl_liste_fmc[vl_val],0);*/
	   acop_traite_ech(pl_liste_fmc[vl_val],((pl_liste_fmc[vl_val].Evt_Consequence.numero==pl_liste_fmc[vl_val].Evt.numero) && (pl_liste_fmc[vl_val].Evt_Consequence.cle ==pl_liste_fmc[vl_val].Evt.cle)) );

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
	else if (pl_liste_fmc[vl_val].Type==XZAEC_FMC_Travaux) {
									XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 1");
									vm_type=XZAEC_FMC_Travaux;
								/*on est sur des travaux qui sont consequence d une autre FMC*/
							if  ( ( (pl_liste_fmc[vl_val].Evt_Consequence.numero != pl_liste_fmc[vl_val].Evt.numero) ||
								(pl_liste_fmc[vl_val].Evt_Consequence.cle != pl_liste_fmc[vl_val].Evt.cle))) {
		/*genere le fichier du fils*/
									XZST_03EcritureTrace(XZSTC_WARNING,"TRAVAUX 2");
									acop_xml_write2(pl_liste_fmc[vl_val],pl_liste_fmc[vl_val].Type,"");
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
								acop_xml_traite_travaux_basc(pl_liste_fmc[vl_val]);
								/*
								vl_num= pl_liste_fmc[vl_val].Evt.cle+50;
								sprintf(tl_str,"%s/format.ksh %d %d %d %d %s",
									C_PATH_XML,
									pl_liste_fmc[vl_val].Evt.numero,
									vl_num,
									pl_liste_fmc[vl_val].Evt_Consequence.numero,
									pl_liste_fmc[vl_val].Evt_Consequence.cle,
									chemin_export);							
								system(tl_str);

								*/	
							}
	
//EN COURS	
      else if ((pl_liste_fmc[vl_val].Type==XZAEC_FMC_Basculement) || (pl_liste_fmc[vl_val].Type==XZAEC_FMC_BasculTravaux)){
		
									vm_type=pl_liste_fmc[vl_val].Type;
							if  ( ( (pl_liste_fmc[vl_val].Evt_Consequence.numero != pl_liste_fmc[vl_val].Evt.numero) ||
								(pl_liste_fmc[vl_val].Evt_Consequence.cle != pl_liste_fmc[vl_val].Evt.cle))) {
									acop_xml_write2(pl_liste_fmc[vl_val],pl_liste_fmc[vl_val].Type,"");
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
							
							acop_xml_traite_travaux_basc(pl_liste_fmc[vl_val]);
							
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
							
      
	  
      else {
        /*genere le fichier du fils*/
        acop_xml_write2(pl_liste_fmc[vl_val],pl_liste_fmc[vl_val].Type,"");

        /*ajout des fichiers fils � chaque pere*/
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
   } 
  }

  XZST_03EcritureTrace(XZSTC_WARNING,"avant termine");
  /*termine le fichier des peres*/
  for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) {
    XZST_03EcritureTrace(XZSTC_FONCTION,"termine FILS %d %d de PERE %d %d",
		    pl_liste_fmc[vl_val].Evt.numero,
		    pl_liste_fmc[vl_val].Evt.cle,  
		    pl_liste_fmc[vl_val].Evt_Consequence.numero,
		    pl_liste_fmc[vl_val].Evt_Consequence.cle);

      if (pl_liste_fmc[vl_val].Evt.numero==0)
      break;

    if (!(pl_liste_fmc[vl_val].Prevision) && (pl_liste_fmc[vl_val].Bloc_Optio.Fin!='Y')) {
      if ( (pl_liste_fmc[vl_val].Evt_Consequence.numero == pl_liste_fmc[vl_val].Evt.numero) &&
	(pl_liste_fmc[vl_val].Evt_Consequence.cle == pl_liste_fmc[vl_val].Evt.cle))   {
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
		      &vl_seconde) != 6) {
	   XZST_03EcritureTrace(XZSTC_WARNING,
	       "aweb_ftp_genere : le format de la date est incorrecte !" );
	    return;
  }

  sprintf(pl_fin_fichier,"_%02d%02d%04d%02d%02d",
		  vl_jour,
	  vl_mois,
	  vl_annee,
	  vl_heure,
	  vl_minute);

  /*genere le fichier header global*/
  acop_xml_write2(pl_liste_fmc[1],XZAEC_FMC_NB_TYP+3,"glob");

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
		"aweb_ftp_genere : derniere commande %s",tl_str);

  /*acop_tfm_cb();
  acop_pmv_cb();*/
 
	/*A Lecture horodate courante systeme */
  	vl_HorodateSec+=vm_frequence;
	/* armement du timer de programmation */
        /*abonnement au dg pour les validations de fmc */
	if (is_version_preprod()) {
		XDG_EncodeDG2(vl_dg , XDG_ACOP_TIMER, "p");
		strcpy(vl_cle,"cle_timer_acopp");
	}
	else {
		strcpy(vl_dg,XDG_ACOP_TIMER);
		strcpy(vl_cle,"cle_timer_acop");
	}
	if (XZSM_01DemanderProgrammation(       vl_HorodateSec,
						0,
						vl_dg,
						XDM_AS_SRVEVT_STRADA_FMC,
						vl_cle,
						XZSMC_SEC_VRAI) != XDC_OK)
	{
	  XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de verif des evts non declenchee." );
	  return ;
  	}

  sprintf( tl_str,"find %s -mtime +%d -exec rm {} \\;", chemin_export, CO_NB_JOUR_PURGE);
  system(tl_str);
  return ;
}

XDY_Booleen gen_a_traiter(XZLGT_Liste_FMC *pa_liste_fmc, int va_indice) {
int vl_val;

    /*recherche si la FMC a une consequence en cours*/
    for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) {
      if (pa_liste_fmc[vl_val].Evt.numero==0)
      break;

      if (  (pa_liste_fmc[vl_val].Evt_Consequence.numero == pa_liste_fmc[va_indice].Evt.numero)  &&
            (pa_liste_fmc[vl_val].Evt_Consequence.cle == pa_liste_fmc[va_indice].Evt.cle) &&
	    (!(pa_liste_fmc[vl_val].Prevision) && (pa_liste_fmc[vl_val].Bloc_Optio.Fin!='Y'))) {
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
    if (!(pa_liste_fmc[va_indice].Prevision) && (pa_liste_fmc[va_indice].Bloc_Optio.Fin!='Y')) {
      return (TRUE);
    }
return(FALSE);

}


/* Fichier : @(#)acop_evt.c     1.26        Release : 1.26        Date : 11/05/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE acop_evt
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
XDY_Evt gen_recherche_pere(XZLGT_Liste_FMC *pa_liste_fmc, int va_indice){
int vl_index=-1;
int vl_val;
int vl_trouve=-1;
XDY_Evt vl_evt;

  XZST_03EcritureTrace( XZSTC_DEBUG1, "recherche pere de la FMC %d %d (cause %d %d )",
					pa_liste_fmc[va_indice].Evt.numero,
					pa_liste_fmc[va_indice].Evt.cle,
					pa_liste_fmc[va_indice].Evt_Cause.numero,
					pa_liste_fmc[va_indice].Evt_Cause.cle);


  vl_evt.numero = pa_liste_fmc[va_indice].Evt.numero;
  vl_evt.cle = pa_liste_fmc[va_indice].Evt.cle;

  if (pa_liste_fmc[va_indice].Evt_Cause.numero > 0) {
    /*recherche de l index de la cause dans la liste des FMC*/
    for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) {
      if (pa_liste_fmc[vl_val].Evt.numero==0)
      break;
      if ( (pa_liste_fmc[vl_val].Evt.numero == pa_liste_fmc[va_indice].Evt_Cause.numero)  &&
	   (pa_liste_fmc[vl_val].Evt.cle == pa_liste_fmc[va_indice].Evt_Cause.cle) && 
    	   (!(pa_liste_fmc[vl_val].Prevision) && (pa_liste_fmc[vl_val].Bloc_Optio.Fin!='Y'))) {
	 XZST_03EcritureTrace( XZSTC_DEBUG1, "le pere avant verif est %d %d",
			pa_liste_fmc[vl_val].Evt.numero,
			pa_liste_fmc[vl_val].Evt.cle);
	 vl_trouve = vl_val;
       }

     /*on recherche maintenant le pere de la cause*/
      XZST_03EcritureTrace( XZSTC_DEBUG1, "trouve %d",vl_trouve);
      if (vl_trouve!=-1) {
       vl_evt = gen_recherche_pere(pa_liste_fmc, vl_trouve);
       break;
      }
    }
  }
  else { /*pas de cause, je suis donc le pere originel*/
    vl_evt.numero = pa_liste_fmc[va_indice].Evt.numero;
    vl_evt.cle = pa_liste_fmc[va_indice].Evt.cle;
  }

  XZST_03EcritureTrace( XZSTC_DEBUG1, "le pere apres verif est %d %d",vl_evt.numero, vl_evt.cle);
  return (vl_evt);
}





void acop_xml_init2(char *va_fichier, int va_indice)
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

 while ( ( vl_retcode = XZAO000_LireLigneFich ( pl_fp, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
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
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_BG)) vl_noeud->Predicat.Fct=balise_bg;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_BL)) vl_noeud->Predicat.Fct=balise_bl;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_MORTS)) vl_noeud->Predicat.Fct=balise_morts;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_CHARGEMENT)) vl_noeud->Predicat.Fct=balise_chargement;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_MATIERE)) vl_noeud->Predicat.Fct=balise_matiere;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_VIP)) vl_noeud->Predicat.Fct=balise_vip;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_ESCOTA)) vl_noeud->Predicat.Fct=balise_escota;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_TOTAL_VEH)) vl_noeud->Predicat.Fct=balise_total_veh;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_TOTAL_PEOPLE)) vl_noeud->Predicat.Fct=balise_total_people;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_FERM_ENTREES)) vl_noeud->Predicat.Fct=balise_entrees;
      else if (!strcmp(vl_noeud->Valeur, C_BALISE_FERM_SORTIES)) vl_noeud->Predicat.Fct=balise_sorties;     

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
	else {
          XZST_03EcritureTrace(XZSTC_WARNING,"acop_xml_init2: BALISE NON TROUVEE !!! Noeud.Valeur = %s",vl_noeud->Valeur);
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
    if ((vl_ch!=NULL) && (!strncmp(vl_ch,C_BALISE_PREFIXE,strlen(C_BALISE_PREFIXE)))) {
      if (!strcmp(vl_ch, C_BALISE_TEST_CAUSE)) vl_noeud->Predicat_Test.Fct=balise_test_cause;
      else if (!strcmp(vl_ch, C_BALISE_TEST_BUS)) vl_noeud->Predicat_Test.Fct=balise_test_bus;
      else if (!strcmp(vl_ch, C_BALISE_TEST_CHARGEMENT)) vl_noeud->Predicat_Test.Fct=balise_test_chargement;
      else if (!strcmp(vl_ch, C_BALISE_TEST_MOTOS)) vl_noeud->Predicat_Test.Fct=balise_test_motos;
      else if (!strcmp(vl_ch, C_BALISE_TEST_BG)) vl_noeud->Predicat_Test.Fct=balise_test_bg;
      else if (!strcmp(vl_ch, C_BALISE_TEST_PL)) vl_noeud->Predicat_Test.Fct=balise_test_pl;
      else if (!strcmp(vl_ch, C_BALISE_TEST_BL)) vl_noeud->Predicat_Test.Fct=balise_test_bl;
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
      else if (!strcmp(vl_ch, C_BALISE_FERM_ENTREES)) vl_noeud->Predicat_Test.Fct=balise_entrees;
      else if (!strcmp(vl_ch, C_BALISE_FERM_SORTIES)) vl_noeud->Predicat_Test.Fct=balise_sorties;
      else if (!strcmp(vl_ch, C_BALISE_VEH_MATIERE)) vl_noeud->Predicat_Test.Fct=balise_veh_matiere;
      else if (!strcmp(vl_ch, C_BALISE_BASC_VITESSE)) vl_noeud->Predicat_Test.Fct=balise_basc_vitesse;
      else if (!strcmp(vl_ch, C_BALISE_BASC_VITESSE_OPPOSEE)) vl_noeud->Predicat_Test.Fct=balise_basc_vitesse_opposee;
      else if (!strcmp(vl_ch, C_BALISE_TRV_VITESSE)) vl_noeud->Predicat_Test.Fct=balise_trv_vitesse;
      else {
	XZST_03EcritureTrace(XZSTC_WARNING,"acop_xml_init2: BALISE NON TROUVEE !!! %s",vl_ch);
       }
    }
    else  {
      vl_noeud->Predicat_Test.Fct = balise_test_true;
    }

    vl_noeud->Attribut=NULL;
    vl_noeud->FilsAine = NULL;
    vl_noeud->FrereCadet = NULL;
    vl_noeud->Pere=NULL;

    if (vg_arbre[va_indice]==NULL) {/*1� insertion*/
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
    else {
      if ( (vl_noeud->Type==C_MODULE) || (vl_noeud->Type==C_ATTRIBUT_SPEC)) { /*c'est un noeud principal*/
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
	while (vl_liste2!=NULL) {
	  if (!strcmp(vl_liste2->LibellePere,vl_pere)) {
	    vl_noeud4 = vl_liste2->PositionArbre;
	    break;
	  }
          vl_liste2=vl_liste2->Suivant;	
	}
	if (vl_liste2==NULL) {
          /*printf("pere %s de %s non trouve\n",vl_pere,vl_noeud->Libelle);*/
	  return;
        }
        /*printf("pere %s trouve !!! \n",vl_liste2->PositionArbre->Libelle);*/

	/*je le mets au bon endroit dans l arbre*/
	/*je me place sur le pere*/
	if (vl_noeud4->FilsAine == NULL) { /*premier fils*/
	  /* printf("premier fils\n");*/
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
	else { /*N� fils : c est donc un frere du fils aine*/
	  vl_noeud5 = vl_noeud4->FilsAine;
	  while (vl_noeud5->FrereCadet != NULL) {
	/*	  printf("frere= %s\n",vl_noeud5->FrereCadet->Libelle);*/
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
      else { /*c'est un attribut*/
	/*je cherche son pere dans l arbre*/
	vl_liste2=vg_peres;
	while (vl_liste2!=NULL) {
	  if (!strcmp(vl_liste2->LibellePere,vl_pere)) {
	   vl_noeud4 = vl_liste2->PositionArbre;
	   break;
	  }
	  vl_liste2=vl_liste2->Suivant;
        }
	if (vl_liste2==NULL) {
	  /*printf("pere %s de %s non trouve\n",vl_pere,vl_noeud->Libelle);*/
	  return;
	}
        
	/*printf("pere %s de %s trouve\n",vl_pere,vl_noeud->Libelle);*/

	/*si 1� insertion*/
	if (vl_noeud4->Attribut==NULL) {
	  /*printf("1� insertion\n");*/
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
	else {
	  /*je l insere en fin de liste des attributs sur mon noeud principal courant*/
	  /*printf("N� insertion\n");*/
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


int acop_xml_init()
{
  int vl_i;
  char vl_cmd[300];
XZSMT_CleProg           vl_cle;
XZSCT_Datagroup         vl_Datagroup,vl_dg ;
int                     vl_JourSemaine;
double                  vl_HorodateSec;
XZSCT_NomMachine        pl_NomMachine   = "\0";
FILE		*pl_fp;
char                    pl_ligne[200];
char                    pl_param[200];
XDY_Entier              vl_val;


        /* Recuperation du nom de la machine locale */
        XZSC_07NomMachine(pl_NomMachine) ;

	vm_indice=0;

        /*A initialisation des timers */
        XZSM_00InitServices();

        /* Lecture de la frequence d'exportation des donnees vers les ASF */
        if ( (pl_fp = fopen(CO_FIC_FREQUENCE, "r")) == NULL ) {
                XZST_03EcritureTrace(   XZSTC_WARNING, "erreur de lecture du fichier %s !"
                                                        ,CO_FIC_FREQUENCE );
                return(XDC_NOK);
        }

        /* On lit les lignes du fichier frequence.txt une par une */
        /* ------------------------------------------------------ */
        while ( fgets(pl_ligne, 200, pl_fp) != NULL) {
                /* On ignore les lignes commencant par un # */
                if ( pl_ligne[0] == '#' )
                        continue;

                /* Pour les autres lignes on recherche la frequence de notre tache */
                if (sscanf(pl_ligne,"%[^;];%d", pl_param, &vl_val)==2){
                        if ( !strcmp( pl_param, "tacop.x") ) {
                                vm_frequence = vl_val;
                                break;
                        }
                }
                else {
                        XZST_03EcritureTrace(   XZSTC_WARNING, "le fichier %s n'a pas le format demande !"
                                                        ,CO_FIC_FREQUENCE );
                        vm_frequence =0;
                }
        }
        if (vm_frequence==0) {
          XZST_03EcritureTrace(   XZSTC_WARNING, "pas de frequence dans le fichier %s : utilisation freq par default %d"
                                               , CO_FIC_FREQUENCE
                                               , CO_DELAI_FREQUENCE);
          vm_frequence= C_FREQ_VERIF_EVT;
        }

        fclose(pl_fp);


        /*A Lecture horodate courante systeme */
        XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);

        /*abonnement au dg pour les validations de fmc */
	if (is_version_preprod()) {
		XDG_EncodeDG2(vl_dg , XDG_ACOP_TIMER, "p");
		strcpy(vl_cle,"cle_timer_acopp");
	}
	else {
		strcpy(vl_dg,XDG_ACOP_TIMER);
		strcpy(vl_cle,"cle_timer_acop");
	}
        XDG_JoindreDG(vl_dg);

        vl_HorodateSec+=vm_frequence;
        /* armement du timer de programmation */
        if (XZSM_01DemanderProgrammation(       vl_HorodateSec,
                                                0,
                                                vl_dg,
                                                XDM_AS_SRVEVT_STRADA_FMC,
                                                vl_cle,
                                                XZSMC_SEC_VRAI) != XDC_OK)
        {
          XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de verif des evts non declenchee." );
          return (XDC_NOK);
        }

        if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_SRVEVT_STRADA_FMC), acop_evt_cb_validation_fmc,(T_CB_ARG)NULL) == NULL)
        {
                XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AS_SRVEVT_STRADA non cree.",version );
                return XDC_NOK;
        }

/* v�rifie si nous sommes en preprod ou en prod*/
		if (is_version_preprod())
			strcpy( chemin_export, C_PATH_PREPROD);
		else
			strcpy( chemin_export, C_PATH);
			
			
  vg_arbre[0]=NULL;
  for (vl_i=1;vl_i<=XZAEC_FMC_NB_TYP+1;vl_i++) {
    vg_arbre[vl_i]=NULL;
    sprintf(vl_cmd,"cat %s %s > %s/tempo.xml",
		C_XML_GEN_PERE, /* C_XML_GEN*/
		tg_types_fmc[vl_i-1].XMLFile,
		C_PATH_XML);

    system(vl_cmd);
    sprintf(vl_cmd,"%s/tempo.xml",C_PATH_XML);
    acop_xml_init2(vl_cmd, vl_i);
  }

  /*init du header*/
  vg_arbre[XZAEC_FMC_NB_TYP+2] = NULL;
  sprintf(vl_cmd,"cat %s %s > %s/tempo.xml",
		C_XML_GEN_HEADER,
		"",
		C_PATH_XML);
  system(vl_cmd);
  sprintf(vl_cmd,"%s/tempo.xml",C_PATH_XML);
  acop_xml_init2(vl_cmd, XZAEC_FMC_NB_TYP+2);


  /*init du header global*/
  vg_arbre[XZAEC_FMC_NB_TYP+3]=NULL;
  sprintf(vl_cmd,"cat %s %s > %s/tempo.xml",
		C_XML_GEN_GLOBAL,
		"",
		C_PATH_XML);
  system(vl_cmd);
  sprintf(vl_cmd,"%s/tempo.xml",C_PATH_XML);
  acop_xml_init2(vl_cmd, XZAEC_FMC_NB_TYP+3);

  /*recupere le semaphore de tagen*/
  if (XZLG503_SemGet("/produits/migrazur/appliSD/exec/tagen.x",XZLGC_SEM_FMC) != XDC_OK) {
	    XZST_03EcritureTrace( XZSTC_WARNING, "atdp_export_init: pb appel a XZLG503_SemGet");
	return (XDC_NOK);
  }

  /*recup des evts en cours sur le reseau*/
  if (XZLG986_Accede_Memoire_Partagee_FMC() != XDC_OK) {
	XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel  XZLG986_Accede_Memoire_Partagee_FMC") ;
	return(XDC_NOK);
  }

  /*une generation au demarrage*/
  acop_evt_cb_validation_fmc();

  return (XDC_OK);
}

char *acop_xml_noeud_valeur(T_NOEUD *pa_noeud)
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

char *acop_xml_noeud_test(T_NOEUD *pa_noeud)
{
  static char vl_str[255]="";
  char *(*vl_fct)();

    vl_fct=pa_noeud->Predicat_Test.Fct;
    strcpy(vl_str,(*vl_fct)());
    return (vl_str);
}

void acop_xml_noeud(T_NOEUD *pa_noeud, xmlTextWriterPtr pa_flux)
{
int rc;
T_NOEUD *pl_noeud;
char vl_str[255];
char vl_strtest[255];
char vl_ch[255];

	if (strlen(pa_noeud->Libelle)==0) return;
	strcpy(vl_strtest,acop_xml_noeud_test(pa_noeud));
     
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
	strcpy(vl_str,acop_xml_noeud_valeur(pa_noeud));
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
		strcpy(vl_strtest,acop_xml_noeud_test(pl_noeud));
	      if (strlen(vl_strtest) > 0) {
	      if (pl_noeud->SousAttribut<=0)  {
	        strncpy(vl_str,acop_xml_noeud_valeur(pl_noeud),128);
		vl_str[127]='\0';
	        XZST_03EcritureTrace(   XZSTC_DEBUG1, "acop_xml_noeud:  texte = %s",vl_str);
	        if (pl_noeud->SousAttribut==0) {
		   xmlTextWriterWriteString(pa_flux, (xmlChar*)"\n  ");
	           rc = xmlTextWriterWriteFormatElement(pa_flux, BAD_CAST pl_noeud->BaliseXML,
				"%s", vl_str);
  		  XZST_03EcritureTrace(   XZSTC_DEBUG1,"ICI %s",vl_str);
		}
	        else {
	          XZST_03EcritureTrace(   XZSTC_DEBUG1, "acop_xml_noeud:  filtre sur la longueur du texte(%s)",vl_str);
		  if (strlen(vl_str)>0) {
		    xmlTextWriterWriteString(pa_flux, (xmlChar*)"\n  ");
		    rc = xmlTextWriterWriteFormatElement(pa_flux, BAD_CAST pl_noeud->BaliseXML,
						  "%s", vl_str);
	          }
  		  XZST_03EcritureTrace(   XZSTC_DEBUG1,"ICI");
		}
	      }
	      else {
		XZST_03EcritureTrace(XZSTC_DEBUG1,"att = %s\n",pl_noeud->BaliseXML);

	      strcpy(vl_str,acop_xml_noeud_valeur(pl_noeud));
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
	  acop_xml_noeud(pl_noeud,pa_flux);
  }
      if(strlen(vl_strtest) > 0) {
    if (pa_noeud->Type==C_MODULE) {
    xmlTextWriterWriteString(pa_flux, (xmlChar*)"\n  ");
      xmlTextWriterEndElement(pa_flux);
   }
   }

    /*si freres*/
    if (pa_noeud->FrereCadet != NULL) {
      pl_noeud = pa_noeud->FrereCadet;
		XZST_03EcritureTrace(XZSTC_DEBUG1,"noeud frere de %s = %s\n",pa_noeud->Libelle,pl_noeud->Libelle);
		xmlTextWriterWriteString(pa_flux, (xmlChar*)"\n  ");
	acop_xml_noeud(pl_noeud,pa_flux);
    }

      XZST_03EcritureTrace(XZSTC_DEBUG1,"fin noeud = %s\n",pa_noeud->Libelle);
}






void acop_xml_write2(XZLGT_Liste_FMC va_noeud, int va_type,char *va_prefixe)
{
    T_NOEUD *vl_noeud;
    T_NOEUD *vl_noeud2;
    int rc;
    xmlTextWriterPtr writer;
    xmlChar *tmp;
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
    XZST_03EcritureTrace(XZSTC_WARNING,"acop_xml_write2 LCL %s",vl_nom);

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

    /*partie generique*/
    /*vl_noeud=vg_arbre[0];
    acop_xml_noeud(vl_noeud,writer);*/

    /*on se positionne sur le bon modele XML*/
    vl_noeud=vg_arbre[va_type];
    /*TRAITEMENT PARTICULIER POUR CHAUSSEE GLISSANTE*/
    if (va_type==XZAEC_FMC_ChausseeGliss) {
	XZST_03EcritureTrace(XZSTC_WARNING,"GLISS %d",va_noeud.Bloc_Optio.Bloc.Meteo.Type);
      if ( (va_noeud.Bloc_Optio.Bloc.Meteo.Type==XZAEC_MET_Autre) ||
	   (va_noeud.Bloc_Optio.Bloc.Meteo.Type==XZAEC_CH_GLIS_Huile) ||
	   (va_noeud.Bloc_Optio.Bloc.Meteo.Type==XZAEC_CH_GLIS_Carburant) ||
	   (va_noeud.Bloc_Optio.Bloc.Meteo.Type==XZAEC_CH_GLIS_Gasoil) ) 
        vl_noeud=vg_arbre[XZAEC_FMC_FlaqueHuile];
    }

    acop_xml_noeud(vl_noeud,writer);

    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
    printf ("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
	return;
    }
	xmlFreeTextWriter(writer);

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

    if (!handler) {
        printf("ConvertInput: no encoding handler found for '%s'\n",
               encoding ? encoding : "");
        return 0;
    }

    size = (int) strlen(in) + 1;
    out_size = size * 2 - 1;
    out = (unsigned char *) xmlMalloc((size_t) out_size);

    if (out != 0) {
        temp = size - 1;
        ret = handler->input(out, &out_size, (const xmlChar *) in, &temp);
        if ((ret < 0) || (temp - size + 1)) {
            if (ret < 0) {
                printf("ConvertInput: conversion wasn't successful.\n");
            } else {
                printf
                    ("ConvertInput: conversion wasn't successful. converted: %i octets.\n",
                     temp);
            }

            xmlFree(out);
            out = 0;
        } else {
            out = (unsigned char *) xmlRealloc(out, out_size + 1);
            out[out_size] = 0;  /*null terminating out */
        }
    } else {
        printf("ConvertInput: no mem\n");
    }

    return out;
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
	return (vm_noeud.RDS.rpip);
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
	return (vm_noeud.RDS.rpis);
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
sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
			&vl_annee,
			&vl_mois,
			&vl_jour,
			&vl_heure,
			&vl_minute);
  sprintf(vl_str,"%04d-%02d-%02dT%02d:%02d:00",
		vl_jour,
		vl_mois,
		vl_annee,
		vl_heure,
		vl_minute);
  return (vl_str);
}
static char *balise_date_debut() {
  return (vm_noeud.Bloc_Optio.Date_Debut);
}
static char *balise_date_fin() {
  return (vm_noeud.Bloc_Optio.Date_Fin);
}
static char *balise_type() {
	if (vm_noeud.Type==XZAEC_FMC_ChausseeGliss) {
      if ( (vm_noeud.Bloc_Optio.Bloc.Meteo.Type==XZAEC_MET_Autre) ||
	   (vm_noeud.Bloc_Optio.Bloc.Meteo.Type==XZAEC_CH_GLIS_Huile) ||
	   (vm_noeud.Bloc_Optio.Bloc.Meteo.Type==XZAEC_CH_GLIS_Carburant) ||
	   (vm_noeud.Bloc_Optio.Bloc.Meteo.Type==XZAEC_CH_GLIS_Gasoil) ) 
	 return (tg_types_fmc[XZAEC_FMC_FlaqueHuile-1].Type_Datex);
        }
	return (tg_types_fmc[vm_noeud.Type-1].Type_Datex);
}
static char *balise_comment() {
static char vl_libelle[255]="";
  int                   i=0,j=0;
  strncpy(vl_libelle,vm_noeud.Bloc_Optio.Comment,128);
   vl_libelle[127]='\0';
  /*A on supprime les espaces de fin de ligne */
  i = strlen(vl_libelle);
  for (j=i-1;j>=0;j--) {
    if ( vl_libelle[j] == ' ' )
	  vl_libelle[j] = '\0';
    else break;
  }
  return (vl_libelle);
}
static char *balise_zz() {
static char vl_str[5]="";

  sprintf(vl_str,"%d",vm_noeud.VNM);
  return (vl_str);
}
static char *balise_qualif() {
   if (vm_noeud.Bloc_Optio.Veracite!=1)
     return ("probable");
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
static char *balise_nb_tot(){
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
static char *balise_lanes(){
  int vl_nb_voies=0;
  int vl_nb_voies_cir=0;

  gen_cumul_voies( vm_noeud.VL , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VM1 , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VM2 , &vl_nb_voies , &vl_nb_voies_cir);
  gen_cumul_voies( vm_noeud.VR, &vl_nb_voies , &vl_nb_voies_cir);

  if (vl_nb_voies_cir==0)
    return ("allLanesCompleteCarriageway");


  vl_nb_voies=0;
  vl_nb_voies_cir=0;
  gen_cumul_voies( vm_noeud.VR, &vl_nb_voies , &vl_nb_voies_cir);
  if (vl_nb_voies-vl_nb_voies_cir>0)
     return ("expressLane");

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
    return ("slowVehicleLane");

  vl_nb_voies=0;
  vl_nb_voies_cir=0;
  gen_cumul_voies( vm_noeud.BAU, &vl_nb_voies , &vl_nb_voies_cir);
  if (vl_nb_voies-vl_nb_voies_cir>0)
    return ("emergencyLane");

  return ("");
}


/*****************************************/
/*      ACCIDENT                         */
/*****************************************/
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
static char *balise_bg() {
static char vl_str[10]="";

  sprintf(vl_str,"%d",vm_noeud.Bloc_Optio.Bloc.Accident.Blesses_Graves);
  return (vl_str);
}
static char *balise_test_bg() {
 return (vm_noeud.Bloc_Optio.Bloc.Accident.Blesses_Graves>0 ? "1":"");
}
static char *balise_bl() {
static char vl_str[10]="";

  sprintf(vl_str,"%d",vm_noeud.Bloc_Optio.Bloc.Accident.Blesses_Legers);
  return (vl_str);
}
static char *balise_test_bl() {
 return (vm_noeud.Bloc_Optio.Bloc.Accident.Blesses_Legers>0 ? "1":"");
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
	XZST_03EcritureTrace (XZSTC_WARNING, "VITESSE '%d'",vm_type);
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

  sprintf(vl_str,"%d",vm_noeud.Bloc_Optio.Bloc.Bouchon.Longueur);
  return (vl_str);
}
static char *balise_bouchon_type() {
  if (vm_noeud.Bloc_Optio.Bloc.Bouchon.Type==XZAEC_BOU_Dense)
    return ("slowTraffic");
  else if   (vm_noeud.Bloc_Optio.Bloc.Bouchon.Type==XZAEC_BOU_Ralenti)
    return ("queuingTraffic");
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
static char *balise_entrees() {
  XZST_03EcritureTrace (XZSTC_WARNING, "balise_entrees: (%d//%d)",
	vm_noeud.Bloc_Optio.Bloc.Echangeur.Entree1,
	vm_noeud.Bloc_Optio.Bloc.Echangeur.Entree2);
  if ( (vm_noeud.Bloc_Optio.Bloc.Echangeur.Entree1==1) ||
       (vm_noeud.Bloc_Optio.Bloc.Echangeur.Entree2==1)
     )
    return ("doNotUseEntry");
 return("");
} 
static char *balise_sorties() {
  XZST_03EcritureTrace (XZSTC_WARNING, "balise_sorties: (%d//%d)",
	vm_noeud.Bloc_Optio.Bloc.Echangeur.Sortie1,
	vm_noeud.Bloc_Optio.Bloc.Echangeur.Sortie2);
  if ( (vm_noeud.Bloc_Optio.Bloc.Echangeur.Sortie1==1) ||
      (vm_noeud.Bloc_Optio.Bloc.Echangeur.Sortie2==1)
    )
  return ("doNotUseExit");


  /*cas ou ni entree ni sortie*/
  if (!strcmp(balise_entrees(),""))
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
    case XZAEC_MET_Pluie : return ("rain"); break;
    case XZAEC_MET_VentFort : return ("strongWinds"); break;
    case XZAEC_MET_Brouillard : return ("fog") ; break;
    case XZAEC_MET_BrouillardGiv : return ("freezingFog"); break;
    case XZAEC_MET_Grele : return ("hail") ; break;
    case XZAEC_MET_Verglas : return ("heavyFrost") ; break;
    case XZAEC_MET_Autre : return ("badWeather"); break;
    case XZAEC_MET_GreleChaus : return ("hail"); break;
    case XZAEC_MET_ChausMouill : return ("precipitationInTheArea"); break;
    case XZAEC_TRT_CH_Salage : return ("saltingInProgress"); break;
    case XZAEC_TRT_CH_Deneigement : return ("snowploughsInUse"); break;
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

/*****************************************/
/*	CHAUSSEE GLISSANTE		 */
/*****************************************/
static char *balise_glissante() {
switch (vm_noeud.Bloc_Optio.Bloc.Meteo.Type) {
	case XZAEC_CH_GLIS_Huile : return ("oilOnRoad");break;
	case XZAEC_CH_GLIS_Carburant : return ("petrolOnRoad");break;
	case XZAEC_CH_GLIS_Gasoil : return ("dieselOnRoad");break;
	case XZAEC_MET_NeigeChaus : return ("snowOnTheRoad"); break;
	case XZAEC_MET_GreleChaus : return ("wetAndIcyRoad");break;
	case XZAEC_MET_Verglas : return ("blackIce") ; break;
}
return ("other");
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
  XZST_03EcritureTrace (XZSTC_INFO, "balise_veh_type: (%s)",vm_noeud.Bloc_Optio.Bloc.Vehicule.TypeVehicule);
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


