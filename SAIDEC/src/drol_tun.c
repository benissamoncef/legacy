/* Fichier : $Id: drol_tun.c
**************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : drol_tun.c				*
*************************************************
* DESCRIPTION :
* new tdpac
*************************************************
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.0
* JMG	16/02/05	ajout explication claire dans SAD_TUB	1.2
* PNI	19/12/06	suppression prop FERMETURE_VLVR (inconnue de la GTC)
			pas de prop fermeutue VL ou VR pour FMC bouchon
* PNI   25/06/07	Ajout nouveau type FMC Basculement travaux DEM658 1.5
* LCL   09/07/07        Suppression DY 1.6
* PNI	17/07/07	Cas basculement uniquement qd le basculement est actif DEM658 1.7
* PNI	21/04/08	Sectorisation : XDC_DN->XDC_DM 1.8 DEM743
* JMG 02/10/09 secto DEM887 + pastun DEM895 1.10
* JMG 	31/05/10	Sapiens DEM933
* PNI	15/06/10	Correction pour Sapiens DEM933 v1.12
* JPL	06/09/10	Prise en compte du type Fmc Zone de stockage (DEM 942) 1.13
* JPL		17/01/11 : Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.14
* JPL	13/10/11	Pas d'action automatique SAPIENS pour Fmc simulees (Essai ou Exercice) (DEM 995)  1.15
* JPL	11/07/12	Utilisation de l'indicateur de bifurcation pour localisation sur bretelle (DEM 1033)  1.16
* PNI	30/09/13	correction test sur l'egalite de groupe ventilation 1.17
* JMG 	08/04/19	correctio filtre PR SAGA 1.18
* JPL	23/09/19	Type d'evenement "INCENDIE" attendu en configuration et non "INCENDIE DE VEGETATION" (DEM 1352)  1.22
* JMG	10/06/20	evol fichiers config perturbations tunnel 1.23
* JMG	10/06/20	meilleure gestion etat des voies terrain 1.24
* JMG	11/06/20	correction pour proposition PA sur VC 1.25
* ABE	10/05/21	Ajout RAU SONO DEM-SAE244 1.32
* JPL	11/04/22	Prise en compte des cas de voies inexistantes (sens 0) pour determination des voies bloquees (SAE-373)  1.33
* GGA	16/02/23	Proposition de pilotage sur voie d'entree possible pour la nouvelle GTC (DEM-SAE461)
* GGY	22/01/24 	Ajout C_COND_CIRCULATION_BASC_TUNNEL et C_COND_CIRCULATION_NON_BASC_TUNNEL (DEM-483) (DEM-483)
************************************************/

static char *version = "drol_tun.c 1.33" ;

#include "drol_tun.h"

#include "xzac06.h"
#include "xzac106.h"
#include "xzao20.h"
#include "xzlg.h"
#include "xzac102.h"
#include "xzac100.h"
#include "xzac108.h"
#include "xzao350.h"
#include "xzao801.h"
#include "xzap04.h"
#include "xzap70.h"
#include "xzae860.h"

#define C_MAX_TUN	10
#define C_NB_TUN	40
#define C_FERMETURE_VL	3
#define C_FERMETURE_VR	5
#define C_FERMETURE_VLVC	24
#define C_FERMETURE_VRVC	25
#define C_FERMETURE_VLVR	26
#define C_FERMETURE_TUNNEL	1
#define C_BASCULEMENT		9
#define C_DOUBLE_SENS		8
#define C_DEBUT_LIMITATION	22

#define C_LIB_FERMETURE_VL	"FERMETURE_VL"
#define C_LIB_FERMETURE_VR	"FERMETURE_VR"
#define C_LIB_FERMETURE_VLVC	"FERMETURE_VLVC"
#define C_LIB_FERMETURE_VRVC	"FERMETURE_VRVC"
#define C_LIB_FERMETURE_VLVR	"FERMETURE_VLVR"
#define C_LIB_FERMETURE_TUNNEL	"FERMETURE_TUNNEL"
#define C_LIB_BASCULEMENT		"BASCULEMENT"
#define C_LIB_DOUBLE_SENS		"DOUBLE_SENS"
#define C_LIB_DEBUT_LIMITATION	"DEBUT_LIMITATION"
#define C_LIB_OUVERTURE		"OUVERTURE"

#define C_REP_MIG       "/produits/migrazur/appliSD/fichiers/bdc/"
#define C_PERT_SIG      C_REP_MIG "tun_sig_VC.txt"
#define C_PERT_ECL      C_REP_MIG "tun_ecl_VC.txt"
#define C_PERT_VEN      C_REP_MIG "tun_ven_VC.txt"
#define C_PERT_SIG_NEW     C_REP_MIG "tun_sig_DP.txt"
#define C_PERT_ECL_NEW      C_REP_MIG "tun_ecl_DP.txt"
#define C_PERT_VEN_NEW      C_REP_MIG "tun_ven_DP.txt"
#define C_ZONE      C_REP_MIG "zones.txt"

static T_NOEUD	*vm_noeud2;
T_NOEUD 	*vg_racine;
T_PERT_TUN      *vg_pert_sig,*vg_pert_ecl,*vg_pert_ven;
T_PERT_GTC      *vg_pert_sig_new,*vg_pert_ecl_new,*vg_pert_ven_new;
T_NOEUD 	*vm_noeud;
T_TUNNEL	tm_tunnels[C_MAX_TUN];
static 		char vm_libelle[50];
static		T_CONDITION *vm_condition;
T_ZONE_GTC		*vg_zones;

static char vm_nom_scenario[50];
static char vm_nom_scenario_opp[50];
static char vm_nom_scenario_nat[50];
static char vm_nom_scenario_nat_opp[50];

#define C_SEPARATEUR    "#"
#define C_PIPE          "|"
#define C_ET            "$ET$"
#define C_CAR_DOLLAR    '$'
#define C_INFINI        1000000

#define C_INFERIEUR     "<"
#define C_SUPERIEUR     ">"
#define C_EGAL          "="

#define C_SAGA_PRMIN	69050
#define C_SAGA_PRMAX	72030

#define C_COND_UNE_VOIE_BLOQ    "au moins une voie bloquee"
#define C_COND_UNE_VOIE_OUVERTE "au moins une voie ouverte"
#define C_COND_TOUTES_VOIES_BLOQ        "toutes voies bloquees"
#define C_COND_TOUTES_VOIES_BLOQ_NEUT        "toutes voies bloquees neutralisees"
#define C_COND_UNE_VOIE_NEUT    "au moins une voie neutralisee"
#define C_COND_TOUTES_VOIES_NEUT        "toutes voies neutralisees"
#define C_COND_TOUTES_VOIES_FERMEES     "toutes voies fermees"
#define C_COND_CIRCULATION_UNE_VOIE     "circulation sur une voie"
#define C_COND_SUR_BAU          "sur bau"
#define C_COND_SUR_SECTION_COURANTE     "sur section courante"
#define C_COND_SUR_LIT_ARRET    "sur lit d'arret"
#define C_COND_SUR_ECHANGEUR    "sur echangeur"
#define C_COND_SUR_BRETELLE_ENTREE      "sur bretelle d'entree"
#define C_COND_SUR_BRETELLE_SORTIE      "sur bretelle de sortie"
#define C_COND_SIGNALE  "signale"
#define C_COND_CONFIRME "confirme"
#define C_COND_TRAFIC_DENSE     "trafic dense"
#define C_COND_TRAFIC_RALENTI   "trafic ralenti"
#define C_COND_TRAFIC_BOUCHE    "trafic bouche"
#define C_COND_TRAFIC_BLOQUE    "trafic bloque"
#define C_COND_VOIES_RETRECIES  "voies retrecies"
#define C_COND_ENTREE_FERMEE    "entree fermee"
#define C_COND_SORTIE_FERMEE    "sortie fermee"
#define C_COND_DATEX    "datex"
#define C_COND_BOUCHON_MOINS_3_KM       "longueur<3"
#define C_COND_BOUCHON_ENTRE_3_ET_5_KM  "longueur<5"
#define C_COND_BOUCHON_PLUS_5_KM        "longueur>5"
#define C_COND_PREVU            "prevu"
#define C_COND_NON_PREVU        "non prevu"
#define C_COND_TOUS     "TOUS"
#define C_COND_TRAVAUX_ENTRETIEN        "entretien"
#define C_COND_STOCKAGE         "stockage"
#define C_COND_AIRE_SERVICE     "service"
#define C_COND_AIRE_REPOS       "repos"
#define C_COND_MATIERES "matieres_dangereuses"
#define C_COND_MAJEUR "majeur"
#define C_COND_AUTOROUTE	"autoroute"
#define C_COND_PR	"pr"
#define C_COND_SUD	"sud"
#define C_COND_NORD	"nord"
#define C_COND_SUR_VOIE_LENTE	"sur voie lente"
#define C_COND_SUR_VOIE_MEDIANE	"sur voie mediane"
#define C_COND_SUR_VOIE_RAPIDE	"sur voie rapide"
#define C_COND_CIRCULATION_BASC_TUNNEL		"circulation_basculee"
#define C_COND_CIRCULATION_NON_BASC_TUNNEL	"circulation_non_basculee"

#define C_COND_TRAFIC_LIBRE_AVAL	"trafic libre en aval"
#define C_COND_TRAFIC_BLOQUE_AVAL	"trafic bloque en aval"

#define C_COND_VL_BLOQUEE		"VL bloquee"
#define C_COND_VL_BLOQUEE_NEUTRALISEE	"VL bloquee neutralisee"
#define C_COND_VL_BLOQUEE_OU_NEUTRALISEE	"VL bloquee ou neutralisee"
#define C_COND_VL_INVERSEE		"VL inversee"
#define C_COND_VL_NEUTRALISEE		"VL neutralisee"

#define C_COND_VM_BLOQUEE		"VM bloquee"
#define C_COND_VM_BLOQUEE_NEUTRALISEE	"VM bloquee neutralisee"
#define C_COND_VM_BLOQUEE_OU_NEUTRALISEE	"VM bloquee ou neutralisee"
#define C_COND_VM_INVERSEE		"VM inversee"
#define C_COND_VM_NEUTRALISEE		"VM neutralisee"

#define C_COND_VR_BLOQUEE		"VR bloquee"
#define C_COND_VR_BLOQUEE_NEUTRALISEE	"VR bloquee neutralisee"
#define C_COND_VR_BLOQUEE_OU_NEUTRALISEE	"VR bloquee ou neutralisee"
#define C_COND_VR_INVERSEE		"VR inversee"
#define C_COND_VR_NEUTRALISEE		"VR neutralisee"

#define C_COND_BAU_BLOQUEE		"BAU bloquee"
#define C_COND_BAU_BLOQUEE_NEUTRALISEE	"BAU bloquee neutralisee"
#define C_COND_BAU_BLOQUEE_OU_NEUTRALISEE	"BAU bloquee ou neutralisee"
#define C_COND_BAU_INVERSEE		"BAU inversee"
#define C_COND_BAU_NEUTRALISEE		"BAU neutralisee"

#define C_COND_3_VOIES		"3 voies"
#define C_COND_2_VOIES		"2 voies"
#define C_COND_1_VOIES		"1 voies"

static int vm_auto_C_FERMETURE_VL=0;
static int vm_auto_C_FERMETURE_VR=0;
static int vm_auto_C_FERMETURE_VLVC=0;
static int vm_auto_C_FERMETURE_VRVC=0;
static int vm_auto_C_FERMETURE_VLVR=0;
static int vm_auto_C_FERMETURE_TUNNEL=0;
static int vm_auto_C_BASCULEMENT=0;
static int vm_auto_C_DOUBLE_SENS=0;
static int vm_auto_C_DEBUT_LIMITATION=0;
static int vm_auto_C_OUVERTURE=0;

static int FctTunnel(int,XDY_Octet,char *,int, XDY_Octet,XDY_Octet,XDY_Octet,
			XDY_Octet,XDY_Octet,XDY_Octet, XDY_Octet, char *,char *, int,
			XDY_Voie, XDY_Voie, XDY_Voie, XDY_Voie, XDY_Voie);

static XDY_Booleen drol_tun_renove(char *, char *);
static void drol_tun_cumul_voies(XDY_Voie, int *, int *);
static int drol_tun_autorise(int, int, int);

static int drol_tun_recherche(XDY_Autoroute ,
		        XDY_PR ,
			XDY_Sens ,
			XDY_District,
			XDY_Horodate ,
			XDY_PR,
			int,
			int,
			int,
			int,
			int,
			XDY_Octet,
			T_PERT_TUN *,
			XDY_Octet,T_PERT_GTC *);

static int drol_tun_recherche_basculement(XDY_Autoroute ,
		        XDY_PR ,
			XDY_Sens ,
			XDY_District,
			XDY_Horodate ,
			XDY_PR,
			int,
			int,
			int,
			int,
			int,
			XDY_Octet);

T_CONDITION *drol_tun_decode_condition(char *);

static XDY_Booleen drol_tun_egal(int, int);
static XDY_Booleen drol_tun_different(int, int);

static XDY_Booleen drol_tun_true(int, int);

static XDY_Booleen drol_tun_inferieur();
static XDY_Booleen drol_tun_superieur();


/*fonctions utilisees dans les conditions*/
static XDY_Booleen drol_tun_fmc_simulee();
static XDY_Booleen drol_tun_une_voie_bloquee();
static XDY_Booleen drol_tun_une_voie_ouverte();
static XDY_Booleen drol_tun_toutes_voies_bloquees();
static XDY_Booleen drol_tun_une_voie_neutralisee();
static XDY_Booleen drol_tun_toutes_voies_neutralisees();
static XDY_Booleen drol_tun_toutes_voies_bloquees_neutralisees();
static XDY_Booleen drol_tun_toutes_voies_fermees();
static XDY_Booleen drol_tun_circulation_une_voie();
static XDY_Booleen drol_tun_sur_bau();
static XDY_Booleen drol_tun_sur_section_courante();
static XDY_Booleen drol_tun_sur_lit_arret();
static XDY_Booleen drol_tun_sur_echangeur();
static XDY_Booleen drol_tun_sur_bretelle_entree();
static XDY_Booleen drol_tun_sur_bretelle_sortie();
static XDY_Booleen drol_tun_signale();
static XDY_Booleen drol_tun_confirme();
static XDY_Booleen drol_tun_trafic_dense();
static XDY_Booleen drol_tun_trafic_ralenti();
static XDY_Booleen drol_tun_trafic_bouche();
static XDY_Booleen drol_tun_trafic_bloque();
static XDY_Booleen drol_tun_voies_retrecies();
static XDY_Booleen drol_tun_entree_fermee();
static XDY_Booleen drol_tun_sortie_fermee();
static XDY_Booleen drol_tun_datex();
static XDY_Booleen drol_tun_prevu();
static XDY_Booleen drol_tun_non_prevu();
static XDY_Booleen drol_tun_travaux_entretien();
static XDY_Booleen drol_tun_stockage();
static XDY_Booleen drol_tun_matieres();
static XDY_Booleen drol_tun_aire_service();
static XDY_Booleen drol_tun_aire_repos();
static int drol_tun_voies(int, int, int, int, int);


static XDY_Booleen drol_tun_majeur();
static XDY_Booleen drol_tun_sur_voie_lente();
static XDY_Booleen drol_tun_sur_voie_mediane();
static XDY_Booleen drol_tun_sur_voie_rapide();
static XDY_Booleen drol_tun_trafic_libre_aval();
static XDY_Booleen drol_tun_trafic_bloque_aval();

static XDY_Booleen drol_tun_vl_bloquee();
static XDY_Booleen drol_tun_vl_neutralisee();
static XDY_Booleen drol_tun_vl_bloquee_neutralisee();
static XDY_Booleen drol_tun_vl_bloquee_ou_neutralisee();
static XDY_Booleen drol_tun_vl_inversee();

static XDY_Booleen drol_tun_vm_bloquee();
static XDY_Booleen drol_tun_vm_neutralisee();
static XDY_Booleen drol_tun_vm_bloquee_neutralisee();
static XDY_Booleen drol_tun_vm_bloquee_ou_neutralisee();
static XDY_Booleen drol_tun_vm_inversee();

static XDY_Booleen drol_tun_vr_bloquee();
static XDY_Booleen drol_tun_vr_neutralisee();
static XDY_Booleen drol_tun_vr_bloquee_neutralisee();
static XDY_Booleen drol_tun_vr_bloquee_ou_neutralisee();
static XDY_Booleen drol_tun_vr_inversee();

static XDY_Booleen drol_tun_bau_bloquee();
static XDY_Booleen drol_tun_bau_neutralisee();
static XDY_Booleen drol_tun_bau_bloquee_neutralisee();
static XDY_Booleen drol_tun_bau_bloquee_ou_neutralisee();
static XDY_Booleen drol_tun_bau_inversee();
static XDY_Booleen drol_tun_1_voies();
static XDY_Booleen drol_tun_2_voies();
static XDY_Booleen drol_tun_3_voies();

static XDY_Booleen drol_tun_sud();
static XDY_Booleen drol_tun_nord();
static XDY_Booleen drol_tun_etat_voie(XDY_Octet *,XDY_Octet ) ;

static XDY_Booleen drol_tun_circulation_basculee_tunnel ();
static XDY_Booleen drol_tun_circulation_non_basculee_tunnel ();

/* definition des variables locales au module */
static XDY_District     DistrictDemandeur;
static XDY_Horodate     HorodateDemande;
static int		vm_last_tunnel;
static XDY_Voie	vm_vr,vm_vm2,vm_vm1,vm_vl,vm_bau;
static XDY_Voie	vm_vr_i,vm_vm2_i,vm_vm1_i,vm_vl_i,vm_bau_i;
static T_CFG_TUNNEL	tm_liste_tunnels[40];
static int vm_mode_new;

void drol_charge_new(char *va_fichier) 
{
  int vl_type;
  T_Scenario *vl_scenario;
  T_CONDITION  *vl_condition;
  FILE  *pl_fp;
  T_PERT_GTC *vl_pert;
  T_ZONE_GTC *vl_zone;
  int          vl_retcode = 0;
  int           vl_i;
  char        vl_Lig[500];
  XDY_Entier vl_Resultat_out = 0;
  char  *vl_str;

  if ( (pl_fp = fopen(va_fichier, "r")) == NULL ) {
     XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !" ,va_fichier );
    return;
  }
  while (fgets(vl_Lig,sizeof(vl_Lig),pl_fp)!= NULL) {
    vl_str=vl_Lig;

   /* si ligne commence par < */
   /*on est sur une perturbation*/
   if ( (vl_str[0]=='<') && (vl_str[1]!='/')   ) {
     /*nouvelle perturbation*/
     vl_pert = (T_PERT_GTC *)malloc(sizeof(T_PERT_GTC));
     vl_str=vl_str+1;
     vl_str[strlen(vl_str)-2] = '\0';
     strcpy(vl_pert->Libelle, vl_str);
     vl_pert->Condition = drol_tun_decode_condition(vl_str);
     vl_pert->Scenarios = NULL;
XZST_03EcritureTrace(XZSTC_DEBUG1, "scenario trouvee apres %s " ,vl_str );
   }
   /*sinon si ligne commence par </*
   /*on est sur la derniere ligne d une perturbation*/
   else if ( (vl_str[0]=='<') && (vl_str[1]=='/')   ) {
XZST_03EcritureTrace(XZSTC_DEBUG1, "fin de perturbation trouvee "  );
      if (!strcmp(va_fichier,C_PERT_ECL_NEW)){
        vl_pert->suivant = vg_pert_ecl_new;
        vg_pert_ecl_new = vl_pert;
      }
      else if (!strcmp(va_fichier,C_PERT_VEN_NEW)){
        vl_pert->suivant = vg_pert_ven_new;
        vg_pert_ven_new = vl_pert;
      }
      if (!strcmp(va_fichier,C_PERT_SIG_NEW)){
        vl_pert->suivant = vg_pert_sig_new;
        vg_pert_sig_new = vl_pert;
      }
   }
   /*sinon on est sur une association zone scenario */
   else  {
      vl_scenario = (T_Scenario *)malloc(sizeof(T_Scenario));
      vl_scenario->Zone=NULL;
    if (sscanf(vl_str,"%d|%d|%[^|]|%d|%d|%d|%[^|]|%[^|]|%d|%d|%[^|]|%d|%[^|]|",
      	&vl_scenario->IdZone,
	&vl_scenario->Scenario,
	vl_scenario->Scenario_Nom,
	&vl_scenario->Priorite,
	&vl_scenario->Automatique,
	&vl_scenario->Scenario_Oppose,
	vl_scenario->Scenario_Oppose_Nom,
	vl_scenario->Groupe,
	&vl_scenario->Filtre_PSTT,
	&vl_scenario->Scenario_Nat,
	vl_scenario->Scenario_Nat_Nom,
	&vl_scenario->Scenario_Nat_Oppose,
	vl_scenario->Scenario_Nat_Oppose_Nom
	) ==13) {
      vl_zone = vg_zones;
      while (vl_zone !=NULL)  {
        if (vl_zone->ID==vl_scenario->IdZone)  {
	  vl_scenario->Zone = vl_zone;
	  break;
        }
        vl_zone = vl_zone->suivant;
      }
      if (!strcmp(va_fichier,C_PERT_ECL_NEW)){
        vl_scenario->suivant = vl_pert->Scenarios;
        vl_pert->Scenarios = vl_scenario;
	vl_pert->Priorite = vl_scenario->Priorite;
      }
      else if (!strcmp(va_fichier,C_PERT_VEN_NEW)){
        	vl_scenario->suivant = vl_pert->Scenarios;
        vl_pert->Scenarios = vl_scenario;
	vl_pert->Priorite = vl_scenario->Priorite;
      }
      if (!strcmp(va_fichier,C_PERT_SIG_NEW)){
        vl_scenario->suivant = vl_pert->Scenarios;
        vl_pert->Scenarios = vl_scenario;
	vl_pert->Priorite = vl_scenario->Priorite;
      }

    }
    else {
     XZST_03EcritureTrace(XZSTC_WARNING, "pb decodage %s",vl_str);
    }
   }
  }
  fclose(pl_fp);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_tun_init()

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*   fonction passee en argument a XZAE69
*
* FONCTION
*   on stocke le resultat dans des variables globales
*
------------------------------------------------------*/
{
  FILE  *pl_fp;
  int          vl_retcode = 0;
  int           vl_i;
  char        vl_Lig[500];
  XDY_Entier vl_Resultat_out = 0;
  char  *vl_str;
  int vl_priorite;
  XDY_Nom vl_nom1,vl_nom2;
  int vl_type_zone;
  char vl_groupe[5];
  char vl_str1[10];
  char vl_str2[10];
  T_PERT_TUN	*pl_liste;
  T_PERT_GTC	*pl_liste_new;
  T_ZONE_GTC	*pl_zone;
  char vl_condition[300];
  int vl_sig_opp,vl_sig,vl_ecl,vl_ven1,vl_ven2,vl_ven3,vl_ven4;

  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : drol_tun_init");
  vm_noeud2=(T_NOEUD *)malloc(sizeof(T_NOEUD));
  vg_zones=(T_ZONE_GTC *)NULL;
  if ( (pl_fp = fopen(C_ZONE, "r")) == NULL ) {
     XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !" ,C_ZONE );
    return(XDC_NOK);
  }
  while (fgets(vl_Lig,sizeof(vl_Lig),pl_fp)!= NULL) {
    pl_zone=(T_ZONE_GTC *)malloc(sizeof(T_ZONE_GTC));
    vl_str=vl_Lig;
    /* on ne decode pas les commentaires # */
    if (vl_str[0]=='#')
    {
	   XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_tun_init: Ligne commentaire %s %s",C_ZONE,vl_str);
    }
    else
    {
	    if (vl_i=sscanf(vl_str,"%d|%[^|]|%d|%d|%d|%d|%d",
    		&pl_zone->ID,
		pl_zone->Libelle,
	  	&pl_zone->Autoroute,
		&pl_zone->Sens,
		&pl_zone->Debut,
		&pl_zone->Fin,
		&pl_zone->Type_Zone) != 7) {
	        XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_init: erreur decodage %s %s sscanf retourne %d",C_ZONE,vl_str,vl_i);
     		} else {
	        XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_tun_init: decodage OK %s %s",C_ZONE,vl_str);
	        pl_zone->suivant = vg_zones;
	        vg_zones = pl_zone;
            }
    }

  }
  fclose(pl_fp);

  vg_pert_ecl_new=NULL;
  vg_pert_sig_new=NULL;
  vg_pert_ven_new=NULL;
  drol_charge_new(C_PERT_VEN_NEW);
  drol_charge_new(C_PERT_SIG_NEW);
  drol_charge_new(C_PERT_ECL_NEW);
  if ( (pl_fp = fopen(C_PERT_SIG, "r")) == NULL ) {
     XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !" ,C_PERT_SIG );
    return(XDC_NOK);
  }
  while (fgets(vl_Lig,sizeof(vl_Lig),pl_fp)!= NULL) {
    pl_liste = (T_PERT_TUN *)malloc(sizeof(T_PERT_TUN));
    vl_str=vl_Lig;
    XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_tun_init(SIG): %s",vl_str);
    /* on ne decode pas les commentaires # */
    if (vl_str[0]=='#')
    {
	   XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_tun_init: Ligne commentaire %s %s",C_PERT_SIG_NEW,vl_str);
    }
    else
    {
    if (sscanf(vl_str,"%[^|]|%[^|]|%d|%d|%d|%d|%[^|]|%d|%[^|]|%d|%[^|]|%d|%[^|]|%d|%d|%d|",
   					pl_liste->Libelle,
					vl_condition,
					&pl_liste->Automatique,
					&pl_liste->Priorite,
					&pl_liste->Type_Zone,
					&pl_liste->Sequence,
					pl_liste->Sequence_Nom,
					&pl_liste->Sequence_Nat,
					pl_liste->Sequence_Nat_Nom,
					&pl_liste->Sequence_Opp,
					pl_liste->Sequence_Opp_Nom,
					&pl_liste->Sequence_Nat_Opp,
					pl_liste->Sequence_Nat_Opp_Nom,
					&pl_liste->Filtre_PSTT,
					&pl_liste->Saga_Sud,&pl_liste->Saga_Nord)!=16) {
        XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_init: erreur decodage %s",vl_str);
    }
    else {
    if (pl_liste->Automatique==1) {
	XZST_03EcritureTrace(XZSTC_INFO,"drol_tun_init(SIG): %s : auto=%d, condition = %s, prio =%d, zone=%d, PSTT=%d, SUD=%d, NORD=%d"
			,pl_liste->Libelle,pl_liste->Automatique,vl_condition,
			pl_liste->Priorite, pl_liste->Type_Zone,
			pl_liste->Filtre_PSTT, pl_liste->Saga_Sud, pl_liste->Saga_Nord);
        XZST_03EcritureTrace(XZSTC_INFO,"drol_tun_init(SIG): %s : seq = %d, seq_nom = %s, seq_nat = %d, seq_nat_nom = %s",
					pl_liste->Libelle,
					pl_liste->Sequence,
					pl_liste->Sequence_Nom,
					pl_liste->Sequence_Nat,
					pl_liste->Sequence_Nat_Nom);
        XZST_03EcritureTrace(XZSTC_INFO,"drol_tun_init(SIG): %s : seq opp = %d, seq opp nom = %s, seq nat opp = %d, seq nat opp nom = %s",
					pl_liste->Libelle,
						pl_liste->Sequence_Opp,
						pl_liste->Sequence_Opp_Nom,
						pl_liste->Sequence_Nat_Opp,
						pl_liste->Sequence_Nat_Opp_Nom);
	}
     pl_liste->Condition = drol_tun_decode_condition(vl_condition);
     pl_liste->Suivant = vg_pert_sig;
     vg_pert_sig = pl_liste;
    }
    }
  }
  fclose(pl_fp);

  if ( (pl_fp = fopen(C_PERT_ECL, "r")) == NULL ) {
    XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !" ,C_PERT_ECL);
    return(XDC_NOK);
  }
  while (fgets(vl_Lig,sizeof(vl_Lig),pl_fp)!= NULL) {
    pl_liste = (T_PERT_TUN *)malloc(sizeof(T_PERT_TUN));
    vl_str=vl_Lig;
    if (sscanf(vl_str,"%[^|]|%[^|]|%d|%d|%d|%d|%[^|]|",
			pl_liste->Libelle,
			vl_condition,
			&pl_liste->Automatique,
			&pl_liste->Priorite,
			&pl_liste->Type_Zone,
			&pl_liste->Sequence,
			pl_liste->Sequence_Nom)!=7) {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_init: erreur decodage %s",vl_str);
  }
  else {
    XZST_03EcritureTrace(XZSTC_INFO,"drol_tun_init: %s : auto=%d, prio = %d, type zone = %d, seq = %d, seq nom = %s",
			pl_liste->Libelle,pl_liste->Automatique,
			pl_liste->Priorite,
			pl_liste->Type_Zone,
			pl_liste->Sequence,
			pl_liste->Sequence_Nom);
    pl_liste->Condition = drol_tun_decode_condition(vl_condition);
    pl_liste->Suivant = vg_pert_ecl;
    vg_pert_ecl = pl_liste;
  }
  }
  if ( (pl_fp = fopen(C_PERT_VEN, "r")) == NULL ) {
    XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !" ,C_PERT_VEN);
    return(XDC_NOK);
  }
  while (fgets(vl_Lig,sizeof(vl_Lig),pl_fp)!= NULL) {
    pl_liste = (T_PERT_TUN *)malloc(sizeof(T_PERT_TUN));
    vl_str=vl_Lig;
    if (sscanf(vl_str,"%[^|]|%[^|]|%d|%d|%d|%[^|]|%d|%[^|]|",
			pl_liste->Libelle,
			vl_condition,
			&pl_liste->Automatique,
			&pl_liste->Priorite,
			&pl_liste->Type_Zone,
			pl_liste->Groupe,
			&pl_liste->Sequence,
			pl_liste->Sequence_Nom)!=8) {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_init: erreur decodage %s",vl_str);
  }
  else {
    XZST_03EcritureTrace(XZSTC_INFO,"drol_tun_init(ECL): %s : auto=%d, prio=%d, type zone = %d, groupe = %s, seq = %d, seq_nom = %s",
			pl_liste->Libelle,pl_liste->Automatique,
			pl_liste->Priorite,
			pl_liste->Type_Zone,
			pl_liste->Groupe,
			pl_liste->Sequence,
			pl_liste->Sequence_Nom);
    pl_liste->Condition = drol_tun_decode_condition(vl_condition);
    pl_liste->Suivant = vg_pert_ven;
    vg_pert_ven = pl_liste;
    }
  }
  /*chargement du fichier de correspondante nom tunnel - libelle tunnel*/
  /*dans fichiers/pmv/ConfigBDTunnel.cfg*/
  if ( (pl_fp = fopen("/produits/migrazur/appliSD/fichiers/pmv/ConfigBDTunnel.cfg", 
		"r")) == NULL ) {
    XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !" ,
		"/produits/migrazur/appliSD/fichiers/pmv/ConfigBDTunnel.cfg");
    return(XDC_NOK);
  }  
  vl_i=0;
  fgets(vl_Lig,sizeof(vl_Lig),pl_fp);
  while (fgets(vl_Lig,sizeof(vl_Lig),pl_fp)!= NULL) {
    vl_str=vl_Lig;
    if (sscanf(vl_str,"%d,\t%[^,],\t%[^,],",
		&vl_type_zone,
		vl_nom1,vl_nom2) != 3) {
      XZST_03EcritureTrace(XZSTC_WARNING, "pb decodage %s",vl_str);
      return (XDC_NOK);
    }
    XZLG300_trim(vl_nom1);
    XZLG300_trim(vl_nom2);
    strcpy(tm_liste_tunnels[vl_i].NomBase,vl_nom1);
    strcpy(tm_liste_tunnels[vl_i].Libelle,vl_nom2);
    XZST_03EcritureTrace(XZSTC_FONCTION," NomBase=%s // Libelle=%s",
				vl_nom1,vl_nom2);
			
    vl_i++;
  }
  XZST_03EcritureTrace(   XZSTC_FONCTION,"OUT drol_tun_init");
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* decodage de la condition
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

T_CONDITION *drol_tun_decode_condition(char *va_cond)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
*   on stocke le resultat dans des variables globales
*
------------------------------------------------------*/
{
  char  vl_str[100];
  int vl_indice;
  char vl_ch[150];
  T_CONDITION *pl_cond;
  T_CONDITION *pl_tmp;
  int vl_type;
  char vl_ch_init[500];
  char vl_ch2[150];
  char *vl_ch3;
  int vl_len;

  /*decode les elements separes par des $ET$*/
  XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_tun_decode_condition:  IN %s",va_cond);

  /*allocation memoire*/
  pl_cond = (T_CONDITION *)malloc(sizeof(T_CONDITION));

  /*1 condition  = le type*/
  vl_indice=0;
  while ( (va_cond[vl_indice]!=C_CAR_DOLLAR) && (vl_indice<strlen(va_cond))) {
    pl_cond->Libelle[vl_indice]=va_cond[vl_indice];
    vl_indice++;
  }
  pl_cond->Libelle[vl_indice]='\0';

  pl_cond->Type=C_TYPE_COMPARATEUR;
  pl_cond->Fonction.Comp.Slot=&(vm_noeud2->Type);
  pl_cond->Fonction.Comp.Fct=drol_tun_egal;
  pl_cond->Suivant=NULL;
  pl_tmp=pl_cond;

  /*numero en fonction du type*/
  XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_tun_decode_condition: type = %s",pl_cond->Libelle);

  strcpy(vl_ch,pl_cond->Libelle);
  if (!strcmp(vl_ch,"ACCIDENT")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Accident;
  else if (!strcmp(vl_ch,"TOUS")) {
      pl_cond->Fonction.Comp.Valeur=0;
	  pl_cond->Fonction.Comp.Fct=drol_tun_true;
  }
  else if (!strcmp(vl_ch,"VEHICULE ARRETE")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_VehArrete;
  else if (!strcmp(vl_ch,"VEHICULE EN PANNE")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_VehPanne;
  else if (!strcmp(vl_ch,"VEHICULE EN FEU")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_VehFeu;
  else if (!strcmp(vl_ch,"VEHICULE A CONTRESENS")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Contresens;
  else if (!strcmp(vl_ch,"BOUCHON")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_QueueBouchon;
  else if (!strcmp(vl_ch,"MANIFESTATION")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Manifestation;
  else if (!strcmp(vl_ch,"Basculement")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Basculement;
  else if (!strcmp(vl_ch,"Aire fermee")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_AireFermee;
  else if (!strcmp(vl_ch,"Delestage")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Delestage;
  else if (!strcmp(vl_ch,"Deviation")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Deviation;
  else if (!strcmp(vl_ch,"Travaux")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Travaux;
  else if (!strcmp(vl_ch,"Salage")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Salage;
  else if (!strcmp(vl_ch,"Fermeture echangeur")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_FermetureEch;
  else if (!strcmp(vl_ch,"INCENDIE")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Incendie;
  else if (!strcmp(vl_ch,"GREVE PERSONNEL ESCOTA")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_GreveESCOTA;
  else if (!strcmp(vl_ch,"HOLD-UP / VOL")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_HoldUpVol;
  else if (!strcmp(vl_ch,"OPERATION ESCARGOT")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Escargot;
  else if (!strcmp(vl_ch,"AUTRE EVENEMENT")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Autre;
  else if (!strcmp(vl_ch,"CIRQUE EN CONVOI")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Cirque;
  else if (!strcmp(vl_ch,"CONVOI EXCEPTIONNEL")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_ConvoiExc;
  else if (!strcmp(vl_ch,"CONVOI MILITAIRE")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_ConvoiMil;
  else if (!strcmp(vl_ch,"VEHICULE LENT")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_VehLent;
  else if (!strcmp(vl_ch,"EBOULEMENT")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Eboulement;
  else if (!strcmp(vl_ch,"OBSTACLE SUR CHAUSSEE")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Obstacle;
  else if (!strcmp(vl_ch,"INONDATION")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Inondation;
  else if (!strcmp(vl_ch,"ALTERCATION USAGERS")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_AltercationUsager;
  else if (!strcmp(vl_ch,"AGRESSION")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Agression;
  else if (!strcmp(vl_ch,"PIETONS SUR LA CHAUSSEE")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_PietonChaussee;
  else if (!strcmp(vl_ch,"EVENEMENT HORS RESEAU")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_EvtHorsReseau;
  else if (!strcmp(vl_ch,"MALAISE USAGER")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_MalaiseUsager;
  else if (!strcmp(vl_ch,"FRONTIERE FERMEE")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_FrontiereFermee;
  else if (!strcmp(vl_ch,"CHAUSSEE DETERIORIEE")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_ChausseDeter;
  else if (!strcmp(vl_ch,"FLAQUE D'HUILE")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_FlaqueHuile;
  else if (!strcmp(vl_ch,"ANIMAL ERRANT")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_AnimalErrant;
  else if (!strcmp(vl_ch,"CHAUSSEE DETERIOREE")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_ChausseDeter;
  else if (!strcmp(vl_ch,"VEHICULE EN MARCHE ARRIERE")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_VehMarcheArr;
  else if (!strcmp(vl_ch,"Traitement chaussees")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_TraitChaussees    ;
  else if (!strcmp(vl_ch,"Echangeur d�conseille")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_EchDeconseille;
  else if (!strcmp(vl_ch,"CHAUSSEE GLISSANTE")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_ChausseeGliss;
  else if (!strcmp(vl_ch,"Basculement pour travaux")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_BasculTravaux;
  else if (!strcmp(vl_ch,"Zone de stockage")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_ZoneStockage;
  /*traiter le cas particulier de la meteo*/
  else if (!strcmp(vl_ch,"VERGLAS")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_tun_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Verglas;
  }
  else if (!strcmp(vl_ch,"CHUTE NEIGE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_tun_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_ChuteNeige;
  }
  else if (!strcmp(vl_ch,"PLUIE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_tun_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Pluie;
  }
  else if (!strcmp(vl_ch,"BROUILLARD GIVRANT")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_tun_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_BrouillardGiv;
  }
  else if (!strcmp(vl_ch,"NEIGE SUR CHAUSSEE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_tun_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_NeigeChaus;
  }
  else if (!strcmp(vl_ch,"BROUILLARD")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_tun_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Brouillard;
  }
  else if (!strcmp(vl_ch,"VENT")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_tun_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_VentFort;
  }
  else if (!strcmp(vl_ch,"GRELE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_tun_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Grele;
  }
  else if ( (!strcmp(vl_ch,"FERMETURE STATION")) ||
            (!strcmp(vl_ch,"FERMETURE STATION\n"))
          ) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_IncSsConces;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_tun_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Incident);
    pl_tmp->Fonction.Comp.Valeur=1;
  }
  else if (!strcmp(vl_ch,"PENURIE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_IncSsConces;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_tun_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Incident);
    pl_tmp->Fonction.Comp.Valeur=0;
  }
  /*cas particulier du lit d'arret*/
  else if (!strcmp(vl_ch,"sur lit d'arret")) {
    pl_cond->Fonction.Comp.Slot=&(vm_noeud2->Localisation.TypePointCar);
    pl_cond->Fonction.Comp.Valeur=XDC_POINT_CARACT_LIT_ARRET;
  }
  /*cas particulier du tous*/
  else if (!strcmp(vl_ch,"TOUS")) {
    pl_cond->Fonction.Comp.Valeur=-99;
    pl_tmp->Fonction.Comp.Fct=drol_tun_different;
  }
  else {
    XZST_03EcritureTrace(   XZSTC_WARNING,"drol_tun_decode_condition : pb decodage type !!!");
    pl_cond->Fonction.Comp.Valeur=0;
  }

  /*si pas de condition supplementaire*/
  if (vl_indice==strlen(va_cond)) {
    XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : drol_tun_decode_condition : que le type");
    return (pl_cond);
  }

  /*supprimer $ET$*/
  va_cond=va_cond+strlen(pl_cond->Libelle)+4;

  strcpy(vl_ch_init,va_cond);

pos_decode:
  vl_indice=0;
  while ( ( va_cond[vl_indice]!=C_CAR_DOLLAR) && (vl_indice<strlen(va_cond))) {
    vl_str[vl_indice]=va_cond[vl_indice];
    vl_indice++;
  }
  vl_str[vl_indice]='\0';

    /*allocation de la memoire pour la condition suivante*/
    pl_tmp->Suivant= (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_tmp->Suivant->Suivant=NULL;
    pl_tmp=pl_tmp->Suivant;

    XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_tun_decode_condition:  cond %s",vl_str);
    strcpy(pl_tmp->Libelle,vl_str);

    /*il s'agit d'une fonction PREDICAT*/
    pl_tmp->Type=C_TYPE_PREDICAT;
    if (!strcmp(vl_str,C_COND_UNE_VOIE_BLOQ)) pl_tmp->Fonction.Predicat.Fct=drol_tun_une_voie_bloquee;
    else if (!strcmp(vl_str,C_COND_UNE_VOIE_OUVERTE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_une_voie_ouverte;
    else if (!strcmp(vl_str,C_COND_TOUTES_VOIES_BLOQ)) pl_tmp->Fonction.Predicat.Fct=drol_tun_toutes_voies_bloquees;
    else if (!strcmp(vl_str,C_COND_TOUTES_VOIES_BLOQ_NEUT)) pl_tmp->Fonction.Predicat.Fct=drol_tun_toutes_voies_bloquees_neutralisees;
    else if (!strcmp(vl_str,C_COND_TOUTES_VOIES_NEUT)) pl_tmp->Fonction.Predicat.Fct=drol_tun_toutes_voies_neutralisees;
    else if (!strcmp(vl_str,C_COND_UNE_VOIE_NEUT)) pl_tmp->Fonction.Predicat.Fct=drol_tun_une_voie_neutralisee;
    else if (!strcmp(vl_str,C_COND_CIRCULATION_UNE_VOIE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_circulation_une_voie;
    else if (!strcmp(vl_str,C_COND_CONFIRME)) pl_tmp->Fonction.Predicat.Fct=drol_tun_confirme;
    else if (!strcmp(vl_str,C_COND_SIGNALE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_signale;
    else if (!strcmp(vl_str,C_COND_SUR_BAU)) pl_tmp->Fonction.Predicat.Fct=drol_tun_sur_bau;
    else if (!strcmp(vl_str,C_COND_SUR_SECTION_COURANTE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_sur_section_courante;
    else if (!strcmp(vl_str,C_COND_SUR_LIT_ARRET)) pl_tmp->Fonction.Predicat.Fct=drol_tun_sur_lit_arret;
    else if (!strcmp(vl_str,C_COND_SUR_ECHANGEUR)) pl_tmp->Fonction.Predicat.Fct=drol_tun_sur_echangeur;
    else if (!strcmp(vl_str,C_COND_SUR_BRETELLE_ENTREE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_sur_bretelle_entree;
    else if (!strcmp(vl_str,C_COND_SUR_BRETELLE_SORTIE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_sur_bretelle_sortie;
    else if (!strcmp(vl_str,C_COND_TRAFIC_DENSE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_trafic_dense;
    else if (!strcmp(vl_str,C_COND_TRAFIC_RALENTI)) pl_tmp->Fonction.Predicat.Fct=drol_tun_trafic_ralenti;
    else if (!strcmp(vl_str,C_COND_TRAFIC_BOUCHE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_trafic_bouche;
    else if (!strcmp(vl_str,C_COND_TRAFIC_BLOQUE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_trafic_bloque;
    else if (!strcmp(vl_str,C_COND_DATEX)) pl_tmp->Fonction.Predicat.Fct=drol_tun_datex;
    else if (!strcmp(vl_str,C_COND_VOIES_RETRECIES)) pl_tmp->Fonction.Predicat.Fct=drol_tun_voies_retrecies;
    else if (!strcmp(vl_str,C_COND_ENTREE_FERMEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_entree_fermee;
    else if (!strcmp(vl_str,C_COND_SORTIE_FERMEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_sortie_fermee;
    else if (!strcmp(vl_str,C_COND_PREVU)) pl_tmp->Fonction.Predicat.Fct=drol_tun_prevu;
    else if (!strcmp(vl_str,C_COND_NON_PREVU)) pl_tmp->Fonction.Predicat.Fct=drol_tun_non_prevu;
    else if (!strcmp(vl_str,C_COND_TOUTES_VOIES_FERMEES)) pl_tmp->Fonction.Predicat.Fct=drol_tun_toutes_voies_fermees;
    else if (!strcmp(vl_str,C_COND_TRAVAUX_ENTRETIEN)) pl_tmp->Fonction.Predicat.Fct=drol_tun_travaux_entretien;
    else if (!strcmp(vl_str,C_COND_STOCKAGE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_stockage;
    else if (!strcmp(vl_str,C_COND_AIRE_SERVICE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_aire_service;
    else if (!strcmp(vl_str,C_COND_AIRE_REPOS)) pl_tmp->Fonction.Predicat.Fct=drol_tun_aire_repos;
    else if (!strcmp(vl_str,C_COND_MATIERES)) pl_tmp->Fonction.Predicat.Fct=drol_tun_matieres;
    else if (!strcmp(vl_str,C_COND_MAJEUR)) pl_tmp->Fonction.Predicat.Fct=drol_tun_majeur;
    else if (!strcmp(vl_str,C_COND_SUD)) pl_tmp->Fonction.Predicat.Fct=drol_tun_sud;
    else if (!strcmp(vl_str,C_COND_NORD)) pl_tmp->Fonction.Predicat.Fct=drol_tun_nord;

    else if (!strcmp(vl_str,C_COND_SUR_VOIE_LENTE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_sur_voie_lente;
    else if (!strcmp(vl_str,C_COND_SUR_VOIE_MEDIANE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_sur_voie_mediane;
    else if (!strcmp(vl_str,C_COND_SUR_VOIE_RAPIDE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_sur_voie_rapide;
    else if (!strcmp(vl_str,C_COND_TRAFIC_LIBRE_AVAL)) pl_tmp->Fonction.Predicat.Fct=drol_tun_trafic_libre_aval;
    else if (!strcmp(vl_str,C_COND_TRAFIC_BLOQUE_AVAL)) pl_tmp->Fonction.Predicat.Fct=drol_tun_trafic_bloque_aval;
   
    else if (!strcmp(vl_str,C_COND_VL_NEUTRALISEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_vl_neutralisee;
    else if (!strcmp(vl_str,C_COND_VL_BLOQUEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_vl_bloquee;
    else if (!strcmp(vl_str,C_COND_VL_INVERSEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_vl_inversee;
    else if (!strcmp(vl_str,C_COND_VL_BLOQUEE_NEUTRALISEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_vl_bloquee_neutralisee;
    else if (!strcmp(vl_str,C_COND_VL_BLOQUEE_OU_NEUTRALISEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_vl_bloquee_ou_neutralisee;

    else if (!strcmp(vl_str,C_COND_VM_NEUTRALISEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_vm_neutralisee;
    else if (!strcmp(vl_str,C_COND_VM_BLOQUEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_vm_bloquee;
    else if (!strcmp(vl_str,C_COND_VM_INVERSEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_vm_inversee;
    else if (!strcmp(vl_str,C_COND_VM_BLOQUEE_NEUTRALISEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_vm_bloquee_neutralisee;
    else if (!strcmp(vl_str,C_COND_VM_BLOQUEE_OU_NEUTRALISEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_vm_bloquee_ou_neutralisee;

    else if (!strcmp(vl_str,C_COND_VR_NEUTRALISEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_vr_neutralisee;
    else if (!strcmp(vl_str,C_COND_VR_BLOQUEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_vr_bloquee;
    else if (!strcmp(vl_str,C_COND_VR_INVERSEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_vr_inversee;
    else if (!strcmp(vl_str,C_COND_VR_BLOQUEE_NEUTRALISEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_vr_bloquee_neutralisee;
    else if (!strcmp(vl_str,C_COND_VR_BLOQUEE_OU_NEUTRALISEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_vr_bloquee_ou_neutralisee;

    else if (!strcmp(vl_str,C_COND_BAU_NEUTRALISEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_bau_neutralisee;
    else if (!strcmp(vl_str,C_COND_BAU_BLOQUEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_bau_bloquee;
    else if (!strcmp(vl_str,C_COND_BAU_INVERSEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_bau_inversee;
    else if (!strcmp(vl_str,C_COND_BAU_BLOQUEE_NEUTRALISEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_bau_bloquee_neutralisee;
    else if (!strcmp(vl_str,C_COND_BAU_BLOQUEE_OU_NEUTRALISEE)) pl_tmp->Fonction.Predicat.Fct=drol_tun_bau_bloquee_ou_neutralisee;

    else if (!strcmp(vl_str,C_COND_1_VOIES))  pl_tmp->Fonction.Predicat.Fct=drol_tun_1_voies;
    else if (!strcmp(vl_str,C_COND_2_VOIES))  pl_tmp->Fonction.Predicat.Fct=drol_tun_2_voies;
    else if (!strcmp(vl_str,C_COND_3_VOIES))  pl_tmp->Fonction.Predicat.Fct=drol_tun_3_voies;

	else if (!strcmp(vl_str,C_COND_CIRCULATION_BASC_TUNNEL)) pl_tmp->Fonction.Predicat.Fct = drol_tun_circulation_basculee_tunnel;
	else if (!strcmp(vl_str,C_COND_CIRCULATION_NON_BASC_TUNNEL)) pl_tmp->Fonction.Predicat.Fct = drol_tun_circulation_non_basculee_tunnel;

    else {
      pl_tmp->Type=C_TYPE_COMPARATEUR;
      /*recherche le signe =*/
      if ((vl_ch3=strchr(vl_str,'='))!=NULL)
        pl_tmp->Fonction.Comp.Fct=drol_tun_egal;
      else if ((vl_ch3=strchr(vl_str,'>'))!=NULL)
        pl_tmp->Fonction.Comp.Fct=drol_tun_superieur;
      else if ((vl_ch3=strchr(vl_str,'<'))!=NULL)
        pl_tmp->Fonction.Comp.Fct=drol_tun_inferieur;

      vl_len= (int) (strlen(vl_str) - strlen(vl_ch3));
      strncpy(vl_ch2,vl_str,vl_len);
      vl_ch2[vl_len]='\0';
      vl_ch3++;
    /*  vl_ch3++;*/
      drol_trim(vl_ch3);
      drol_trim(vl_ch2);

      /*memorisation de la valeur*/
      if (sscanf(vl_ch3,"%d",&vl_len)==1)
        pl_tmp->Fonction.Comp.Valeur=vl_len;

      /*memorisation du slot a comparer*/
      if (!strcmp(vl_ch2,C_COND_AUTOROUTE)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Localisation.Autoroute);
      else if (!strcmp(vl_ch2,C_COND_PR)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Localisation.PR);
      else if (!strcmp(vl_ch2,C_COND_PR)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Localisation.PR);
    }
  /*si fin de chaine*/
  if (vl_indice==strlen(va_cond)) {
    XZST_03EcritureTrace(   XZSTC_DEBUG1,"drol_tun_decode_condition : fin de chaine");
    return (pl_cond);
  }
  else {
    va_cond=va_cond+strlen(vl_str)+4;
    goto pos_decode;
  }
}


/**************************************/
/**************************************/
static XDY_Booleen drol_tun_fmc_simulee() {
  return (strstr (vm_noeud->Infos_TFM.Origine, "ESSAI") != NULL  ||
          strstr (vm_noeud->Infos_TFM.Origine, "EXERCICE") != NULL);
}

static XDY_Booleen drol_tun_sur_echangeur() {
  return (vm_noeud->Localisation.TypePointCar==XDC_POINT_CARACT_ECHANGEUR);
}

static XDY_Booleen drol_tun_majeur() {
  if (vm_noeud->Infos_TFM.Majeur==XDC_VRAI)
      return (XDC_VRAI);
  return (XDC_FAUX);
}

static XDY_Booleen drol_tun_sur_lit_arret() {
  return (vm_noeud->Localisation.TypePointCar==XDC_POINT_CARACT_LIT_ARRET);
}

static XDY_Booleen drol_tun_matieres() {
  return (vm_noeud->Infos_FAX.Matieres_Dangereuses);
}

static XDY_Booleen drol_tun_egal(int va_valeur1, int va_valeur2) { return (va_valeur1==va_valeur2); }
static XDY_Booleen drol_tun_different(int va_valeur1, int va_valeur2) { return (va_valeur1!=va_valeur2); }
/*
static XDY_Booleen drol_tun_true(int va_valeur1, int va_valeur2) { return (TRUE); }
*/
static XDY_Booleen drol_tun_superieur(int va_valeur1, int va_valeur2) { 
return (va_valeur1>=va_valeur2); }
static XDY_Booleen drol_tun_inferieur(int va_valeur1, int va_valeur2) { return (va_valeur1<=va_valeur2); }


static XDY_Booleen drol_tun_true(int va_valeur1, int va_valeur2) { return (TRUE); }
static XDY_Booleen drol_tun_confirme() { return (vm_noeud->Infos_TFM.Confirme); }
static XDY_Booleen drol_tun_signale() { return (vm_noeud->Infos_TFM.Signale); }

static XDY_Booleen drol_tun_une_voie_bloquee() {
  return ((vm_vl==XDC_VOIE_BLOQUEE) ||
          (vm_vm1==XDC_VOIE_BLOQUEE) ||
          (vm_vm2==XDC_VOIE_BLOQUEE) ||
          (vm_vr==XDC_VOIE_BLOQUEE)
         );
}
static XDY_Booleen drol_tun_une_voie_ouverte() {
  return ((vm_vl==XDC_VOIE_SENS_NORMAL) ||
          (vm_vm1==XDC_VOIE_SENS_NORMAL) ||
          (vm_vm2==XDC_VOIE_SENS_NORMAL) ||
          (vm_vr==XDC_VOIE_SENS_NORMAL)
         );
}
static XDY_Booleen drol_tun_toutes_voies_bloquees() {
  XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_toutes_voies_bloquees : %d / %d / %d / %d",
				vm_vr,
				vm_vm2,
				vm_vm1,
				vm_vl);

  return ( (vm_vl!=XDC_VOIE_INEXISTANTE) &&
           (vm_vl!=XDC_VOIE_INCONNUE) &&
           (vm_vl!=XDC_VOIE_SENS_NORMAL) &&
           (vm_vm1!=XDC_VOIE_SENS_NORMAL) &&
           (vm_vm2!=XDC_VOIE_SENS_NORMAL) &&
           (vm_vr!=XDC_VOIE_SENS_NORMAL) 
         );
}

static XDY_Booleen drol_tun_une_voie_neutralisee() {
  return ((vm_vl==XDC_VOIE_NEUTRALISEE) ||
          (vm_vm1==XDC_VOIE_NEUTRALISEE) ||
          (vm_vm2==XDC_VOIE_NEUTRALISEE) ||
          (vm_vr==XDC_VOIE_NEUTRALISEE)
         );
}

static XDY_Booleen drol_tun_toutes_voies_neutralisees() {
  return ( (vm_bau!=XDC_VOIE_SENS_NORMAL) &&
           (vm_vr_i!=XDC_VOIE_SENS_INVERSE) &&
           (vm_vm2_i!=XDC_VOIE_SENS_INVERSE) &&
           (vm_vm1_i!=XDC_VOIE_SENS_INVERSE) &&
           (vm_vl_i!=XDC_VOIE_SENS_INVERSE) &&
           (drol_tun_voies(vm_vl,
                          vm_vm1,
                          vm_vm2,
                          vm_vr,
                          XDC_VOIE_NEUTRALISEE)==15)
         );
}

static XDY_Booleen drol_tun_toutes_voies_bloquees_neutralisees() {
  return ( (vm_bau!=XDC_VOIE_SENS_NORMAL) &&
           (vm_vr_i!=XDC_VOIE_SENS_INVERSE) &&
           (vm_vm2_i!=XDC_VOIE_SENS_INVERSE) &&
           (vm_vm1_i!=XDC_VOIE_SENS_INVERSE) &&
           (vm_vl_i!=XDC_VOIE_SENS_INVERSE) &&
           (drol_tun_voies(vm_vl,
                          vm_vm1,
                          vm_vm2,
                          vm_vr,
                          XDC_VOIE_BLOQUEE_NEUTRALISEE)==15)
         );
}


static XDY_Booleen drol_tun_toutes_voies_fermees() {
        return ( drol_tun_toutes_voies_neutralisees() || drol_tun_toutes_voies_bloquees_neutralisees());
}

static XDY_Booleen drol_tun_circulation_une_voie() {
  return ( ( (vm_vl!=XDC_VOIE_SENS_NORMAL) &&
             (vm_vm1!=XDC_VOIE_SENS_NORMAL) &&
             (vm_vm2!=XDC_VOIE_SENS_NORMAL) &&
             (vm_vr==XDC_VOIE_SENS_NORMAL)
           )
           ||
           ( (vm_vl!=XDC_VOIE_SENS_NORMAL) &&
             (vm_vm1!=XDC_VOIE_SENS_NORMAL) &&
             (vm_vm2==XDC_VOIE_SENS_NORMAL) &&
             (vm_vr!=XDC_VOIE_SENS_NORMAL)
           )
           ||
           ( (vm_vl!=XDC_VOIE_SENS_NORMAL) &&
             (vm_vm1==XDC_VOIE_SENS_NORMAL) &&
             (vm_vm2!=XDC_VOIE_SENS_NORMAL) &&
             (vm_vr!=XDC_VOIE_SENS_NORMAL)
           )
           ||
           ( (vm_vl==XDC_VOIE_SENS_NORMAL) &&
             (vm_vm1!=XDC_VOIE_SENS_NORMAL) &&
             (vm_vm2!=XDC_VOIE_SENS_NORMAL) &&
             (vm_vr!=XDC_VOIE_SENS_NORMAL)
           )
         );
}


static XDY_Booleen drol_tun_sur_bau() {
  return ( (drol_tun_voies(vm_vl,vm_vm1,
                           vm_vm2,
                           vm_vr,
                           XDC_VOIE_SENS_NORMAL)==15) &&
           ((vm_bau==XDC_VOIE_BLOQUEE) ||
            (vm_bau==XDC_VOIE_NEUTRALISEE)
           )
         );
}

static XDY_Booleen drol_tun_sur_section_courante() {
  return ( (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_AIRE) &&
           (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_ECHANGEUR) &&
           (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_PEAGE) &&
           (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_LIT_ARRET)
         );
}
static XDY_Booleen drol_tun_sud() {
  return ((vm_noeud->Localisation.Autoroute==3) && (vm_noeud->Localisation.Sens==XDC_SENS_SUD));
}
static XDY_Booleen drol_tun_nord() {
  return ((vm_noeud->Localisation.Autoroute==3) && (vm_noeud->Localisation.Sens==XDC_SENS_NORD));
}
static XDY_Booleen drol_tun_sur_bretelle_entree() {
  return (  (  (vm_noeud->Localisation.Sens==XDC_SENS_ENTREE_SUD) ||
               (vm_noeud->Localisation.Sens==XDC_SENS_ENTREE_NORD)
            )
            &&
            (vm_noeud->Localisation.Bifurcation != XDC_VRAI)
         );
}

static XDY_Booleen drol_tun_sur_bretelle_sortie() {
  return (  (      (vm_noeud->Localisation.Sens==XDC_SENS_SORTIE_SUD) ||
                   (vm_noeud->Localisation.Sens==XDC_SENS_SORTIE_NORD)
            )
            &&
            (vm_noeud->Localisation.Bifurcation != XDC_VRAI)
         );
}

static XDY_Booleen drol_tun_trafic_dense() {
  return (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Dense); }

static XDY_Booleen drol_tun_trafic_ralenti() {
  return (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Ralenti); }

static XDY_Booleen drol_tun_trafic_bouche() {
  return (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Bouche); }

static XDY_Booleen drol_tun_trafic_bloque() {
  return (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Bloque); }

static XDY_Booleen drol_tun_datex() { return (FALSE); }

static XDY_Booleen drol_tun_voies_retrecies() { return (vm_noeud->Localisation.Voies_Retrecies); }

static XDY_Booleen drol_tun_entree_fermee() {
  return ( (vm_noeud->Infos_FAX.Entree1) || (vm_noeud->Infos_FAX.Entree2) );
}

static XDY_Booleen drol_tun_sortie_fermee() {
  return ( (vm_noeud->Infos_FAX.Sortie1) || (vm_noeud->Infos_FAX.Sortie2) );
}
static XDY_Booleen drol_tun_non_prevu() {
  return (!(vm_noeud->Infos_TFM.Prevu) );
}
static XDY_Booleen drol_tun_travaux_entretien() {
  return (vm_noeud->Infos_TFM.Nature_Travaux==0);
}
static XDY_Booleen drol_tun_stockage() {
  return (vm_noeud->Infos_TFM.Type_Travaux==24);
}
static XDY_Booleen drol_tun_aire_service() {
  return (vm_noeud->Infos_TFM.Aire_Service);
}
static XDY_Booleen drol_tun_aire_repos() {
  return !(vm_noeud->Infos_TFM.Aire_Service);
}
static XDY_Booleen drol_tun_prevu() {
  return (!drol_tun_non_prevu());
}

static XDY_Booleen drol_tun_sur_voie_lente() {
  return ( (vm_noeud->Localisation.VL==XDC_VOIE_BLOQUEE) || (vm_noeud->Localisation.VL==XDC_VOIE_BLOQUEE_NEUTRALISEE));
}

static XDY_Booleen drol_tun_sur_voie_rapide() {
  return ( (vm_noeud->Localisation.VR==XDC_VOIE_BLOQUEE) || (vm_noeud->Localisation.VR==XDC_VOIE_BLOQUEE_NEUTRALISEE));
}

static XDY_Booleen drol_tun_sur_voie_mediane() {
  return ( (vm_noeud->Localisation.VM1 ==XDC_VOIE_BLOQUEE) || (vm_noeud->Localisation.VM1==XDC_VOIE_BLOQUEE_NEUTRALISEE) ||
	   (vm_noeud->Localisation.VM2 ==XDC_VOIE_BLOQUEE) || (vm_noeud->Localisation.VM2==XDC_VOIE_BLOQUEE_NEUTRALISEE));
}
static int drol_tun_config_terrain() {
  XDY_Voie vl_vr_i,vl_vm2_i,vl_vm1_i,vl_vl_i,vl_bau_i;
  XDY_Octet vl_nb_zi;
  int vl_nb_voies=0,vl_nb_voies_cir=0;

  if (vm_vl==XDC_VOIE_INEXISTANTE) {
    XZAE860_Lire_Config_Terrain(vm_noeud->Localisation.Autoroute,
			vm_noeud->Localisation.PR,
			vm_noeud->Localisation.Sens%10,
			vm_noeud->Localisation_Tete.PR,
			&vm_vr,
			&vm_vm2,
			&vm_vm1,
			&vm_vl,
			&vm_bau,
			&vl_vr_i,&vl_vm2_i,&vl_vm1_i,&vl_vl_i,&vl_bau_i,
			&vl_nb_zi);
  }
  /*si VL inexistante*/
  if (vm_vl==XDC_VOIE_INEXISTANTE) {
    /*alors prendre les infos de la FMC*/
    vm_vr = vm_noeud->Localisation.VR;
    vm_vl = vm_noeud->Localisation.VL;
    vm_vm1 = vm_noeud->Localisation.VM1;
    vm_vm2 = vm_noeud->Localisation.VM2;
    vm_bau = vm_noeud->Localisation.BAU;
    vm_vr_i = vm_noeud->Localisation.VR_I;
    vm_vl_i = vm_noeud->Localisation.VL_I;
    vm_vm1_i = vm_noeud->Localisation.VM1_I;
    vm_vm2_i = vm_noeud->Localisation.VM2_I;
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_config_terrain : VR %d / VM2 %d / VM1 %d / VL %d / BAU %d",
    		vm_vr,vm_vm2, vm_vm1, vm_vl, vm_bau);
  }

  drol_tun_cumul_voies( vm_vl , &vl_nb_voies , &vl_nb_voies_cir);
  drol_tun_cumul_voies( vm_vm1 , &vl_nb_voies , &vl_nb_voies_cir);
  drol_tun_cumul_voies( vm_vm2 , &vl_nb_voies , &vl_nb_voies_cir);
  drol_tun_cumul_voies( vm_vr, &vl_nb_voies , &vl_nb_voies_cir);

  return (vl_nb_voies);
}

static XDY_Booleen drol_tun_1_voies(){
  return (drol_tun_config_terrain()==1);
}
static XDY_Booleen drol_tun_2_voies(){
  return (drol_tun_config_terrain()==2);
}
static XDY_Booleen drol_tun_3_voies(){
  return (drol_tun_config_terrain()==3);
}
static XDY_Booleen drol_tun_etat_voie(XDY_Octet *va_voie,XDY_Octet va_etat) {
  drol_tun_config_terrain();
  return (*va_voie == va_etat);
}
static XDY_Booleen drol_tun_vl_bloquee() {
  return (drol_tun_etat_voie(&vm_vl,XDC_VOIE_BLOQUEE));
}
static XDY_Booleen drol_tun_vl_bloquee_ou_neutralisee() {
  if (drol_tun_vl_bloquee())
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_vl_bloquee_ou_neutralisee : drol_tun_vl_bloquee");
if (drol_tun_vl_bloquee_neutralisee())
  XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_vl_bloquee_ou_neutralisee : drol_tun_vl_bloquee_neutralisee");
  return ( (drol_tun_vl_bloquee()) ||
	   (drol_tun_vl_bloquee_neutralisee()) );
}
static XDY_Booleen drol_tun_vl_bloquee_neutralisee() {
  return ( (drol_tun_etat_voie(&vm_vl,XDC_VOIE_BLOQUEE_NEUTRALISEE)) ||
	 (drol_tun_etat_voie(&vm_vl,XDC_VOIE_NEUTRALISEE)) );
}
static XDY_Booleen drol_tun_vl_inversee() {
  return (drol_tun_etat_voie(&vm_vl,XDC_VOIE_SENS_INVERSE));
}
static XDY_Booleen drol_tun_vl_neutralisee() {
  return (drol_tun_etat_voie(&vm_vl,XDC_VOIE_NEUTRALISEE));
}
static XDY_Booleen drol_tun_vm_bloquee() {
  return ( (drol_tun_etat_voie(&vm_vm1,XDC_VOIE_BLOQUEE))  ||
           (drol_tun_etat_voie(&vm_vm2,XDC_VOIE_BLOQUEE)) );
}
static XDY_Booleen drol_tun_vm_bloquee_neutralisee() {
  return ( (drol_tun_etat_voie(&vm_vm1,XDC_VOIE_BLOQUEE_NEUTRALISEE))  ||
           (drol_tun_etat_voie(&vm_vm2,XDC_VOIE_BLOQUEE_NEUTRALISEE))  ||
	   (drol_tun_etat_voie(&vm_vm1,XDC_VOIE_NEUTRALISEE)) ||
	   (drol_tun_etat_voie(&vm_vm2,XDC_VOIE_NEUTRALISEE)) );
}
static XDY_Booleen drol_tun_vm_inversee() {
  return ( (drol_tun_etat_voie(&vm_vm1,XDC_VOIE_SENS_INVERSE))  ||
           (drol_tun_etat_voie(&vm_vm2,XDC_VOIE_SENS_INVERSE)) );
}
static XDY_Booleen drol_tun_vm_neutralisee() {
  return ( (drol_tun_etat_voie(&vm_vm1,XDC_VOIE_NEUTRALISEE))  ||
           (drol_tun_etat_voie(&vm_vm2,XDC_VOIE_NEUTRALISEE)) );
}
static XDY_Booleen drol_tun_vm_bloquee_ou_neutralisee() {
  return ( (drol_tun_vm_bloquee()) ||
	   (drol_tun_vm_bloquee_neutralisee()) );
}
static XDY_Booleen drol_tun_vr_bloquee() {
  return (drol_tun_etat_voie(&vm_vr,XDC_VOIE_BLOQUEE));
}
static XDY_Booleen drol_tun_vr_bloquee_neutralisee() {
  return ( (drol_tun_etat_voie(&vm_vr,XDC_VOIE_BLOQUEE_NEUTRALISEE)) ||
	   (drol_tun_etat_voie(&vm_vr,XDC_VOIE_NEUTRALISEE)) );
}
static XDY_Booleen drol_tun_vr_inversee() {
  return (drol_tun_etat_voie(&vm_vr,XDC_VOIE_SENS_INVERSE));
}
static XDY_Booleen drol_tun_vr_neutralisee() {
  return (drol_tun_etat_voie(&vm_vr,XDC_VOIE_NEUTRALISEE));
}
static XDY_Booleen drol_tun_bau_bloquee() {
  return ( (drol_tun_etat_voie(&vm_bau,XDC_VOIE_BLOQUEE)) &&
	   ( !(drol_tun_vm_bloquee_ou_neutralisee()) &&
	     !(drol_tun_vl_bloquee_ou_neutralisee()) &&
	     !(drol_tun_vr_bloquee_ou_neutralisee()) 
	     )
	  ) ;
}
static XDY_Booleen drol_tun_vr_bloquee_ou_neutralisee() {
if (drol_tun_vr_bloquee())
XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_vr_bloquee_ou_neutralisee : drol_tun_vr_bloquee ");
if (drol_tun_vr_bloquee_neutralisee())
  XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_vr_bloquee_ou_neutralisee : drol_tun_vr_bloquee_neutralisee");
  XZST_03EcritureTrace(XZSTC_WARNING," VR = %d",vm_vr);
  return ( (drol_tun_vr_bloquee()) ||
	   (drol_tun_vr_bloquee_neutralisee()) );
}
static XDY_Booleen drol_tun_bau_bloquee_neutralisee() {
  return ( (drol_tun_etat_voie(&vm_bau,XDC_VOIE_BLOQUEE_NEUTRALISEE)) ||
	   (drol_tun_etat_voie(&vm_bau,XDC_VOIE_NEUTRALISEE)) );
}
static XDY_Booleen drol_tun_bau_inversee() {
  return (drol_tun_etat_voie(&vm_bau,XDC_VOIE_SENS_INVERSE));
}
static XDY_Booleen drol_tun_bau_neutralisee() {
  return (drol_tun_etat_voie(&vm_bau,XDC_VOIE_NEUTRALISEE));
}
static XDY_Booleen drol_tun_bau_bloquee_ou_neutralisee() {
  return ( (drol_tun_bau_bloquee()) ||
	   (drol_tun_bau_bloquee_neutralisee()) );
}


static XDY_Booleen drol_tun_trafic_libre_aval() {
  XDY_Booleen vl_flag;

  XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_trafic_libre_aval : IN");
  
  if (XZLG90_Recherche_FMC_Aval(XZAEC_FMC_QueueBouchon,vm_noeud->Localisation.Autoroute,
				vm_noeud->Localisation_Tete.PR,
				vm_noeud->Localisation.Sens%10,
				500,
				&vl_flag)!=XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_trafic_libre_aval: pb appel a XZLG90_Recherche_FMC_Aval");
    return (TRUE);
  }
  
  return (!vl_flag);
}

static XDY_Booleen drol_tun_trafic_bloque_aval() {
  XDY_Booleen vl_flag;

  XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_trafic_bloque_aval : IN");
  
  if (XZLG90_Recherche_FMC_Aval(XZAEC_FMC_QueueBouchon,vm_noeud->Localisation.Autoroute,
				vm_noeud->Localisation_Tete.PR,
				vm_noeud->Localisation.Sens%10,
				500,
				&vl_flag)!=XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_trafic_bloque_aval: pb appel a XZLG90_Recherche_FMC_Aval");
    return (FALSE);
  }
  
  return (vl_flag);
}

static XDY_Booleen drol_tun_circulation_basculee_tunnel ()
{
	XDY_Booleen vl_basculement = XDC_FAUX;
	
	XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_circulation_basculee_tunnel : avant XZAE861_Basculement_Tunnel");

	if (XZAE861_Basculement_Tunnel (vm_noeud->Localisation.Autoroute,
									vm_noeud->Localisation.PR,
									vm_noeud->Localisation_Tete.PR,
									vl_basculement)	!= XDC_OK) 
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_circulation_basculee_tunnel : pb appel a XZAE861_Basculement_Tunnel");
		return (FALSE);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_circulation_basculee_tunnel : XZAE861_Basculement_Tunnel retourne %s", vl_basculement);
	}
	
	return (vl_basculement);
}

static XDY_Booleen drol_tun_circulation_non_basculee_tunnel ()
{
	XZST_03EcritureTrace (XZSTC_WARNING, "drol_tun_circulation_non_basculee_tunnel : not drol_tun_circulation_basculee_tunnel");
	return (!drol_tun_circulation_basculee_tunnel());
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des perturbations engendrees par l'evt
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen drol_tun_filtre(T_NOEUD *va_noeud, XDY_District va_site, XDY_Horodate va_horodate,
				XDY_Octet va_domaine)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
------------------------------------------------------*/
{
 int vl_indice;
  T_PERT_TUN *pl_cas;
  T_PERT_GTC *pl_cas_gtc;
  T_CONDITION *pl_cond;
  XDY_Booleen (*vl_fct)();
  XDY_Booleen vl_res=FALSE;
  XDY_Octet	vl_sens;
  XDY_Octet	vl_domaine;
  XDY_Octet	vl_sequence;
  int		vl_mode_automatique;
  XDY_PR vl_min, vl_max;
  char vl_str[100];
T_Scenario *pl_scenario;
  T_ZONE_GTC    *vl_zone;

  for (vl_indice=0;vl_indice<C_MAX_TUN;vl_indice++)
    tm_tunnels[vl_indice].Numero=0;
  vm_last_tunnel=0;
  vm_vl=XDC_VOIE_INEXISTANTE;

  vm_noeud=va_noeud;
  vm_noeud2->Localisation=vm_noeud->Localisation;
  vm_noeud2->Infos_FAX=vm_noeud->Infos_FAX;
  vm_noeud2->Type=vm_noeud->Type;
  XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_tun_filtre : type = %d(%d)",vm_noeud2->Type,&(vm_noeud2->Type));
  switch (va_domaine) {
    case (XDC_ACT_GTC_SIG) :
	pl_cas = vg_pert_sig;
	pl_cas_gtc = vg_pert_sig_new;
	break;
    case (XDC_ACT_GTC_ECL) : 
	pl_cas = vg_pert_ecl;
	pl_cas_gtc = vg_pert_ecl_new;
	break;
    case (XDC_ACT_GTC_VEN):
	pl_cas=vg_pert_ven;
	pl_cas_gtc = vg_pert_ven_new;
	break;
  }


  /*s'il y a au moins une voie bloquee ou neutralisee, ou s'il s'agit d'une deviation, on cherche les tunnels candidats*/
  if (va_noeud->Infos_TFM.Termine) return(XDC_OK);
  if (va_noeud->Infos_TFM.Prevu) return (XDC_OK);

  /* on force la recuperation de la config terrain*/
  drol_tun_config_terrain();

  if (va_site==XDC_VC) {
  vm_mode_new=0;
  while (pl_cas!=NULL)
  {
    XZST_03EcritureTrace(   XZSTC_DEBUG1,"drol_tun_filtre %s domaine=%d",pl_cas->Libelle,va_domaine);
    pl_cond=pl_cas->Condition;
    vl_res=TRUE;

    while (pl_cond!=NULL)
    {
      if (pl_cond->Type==C_TYPE_COMPARATEUR)
      {
        vl_fct=pl_cond->Fonction.Comp.Fct;
        vl_res=vl_res && (*vl_fct)(*(pl_cond->Fonction.Comp.Slot),
                        pl_cond->Fonction.Comp.Valeur);

        if (!vl_res){
          break;
        }
      }
      else
      {
        vl_fct=pl_cond->Fonction.Predicat.Fct;
        vl_res = vl_res && (*vl_fct)();
        if (!vl_res) break;
      }
      pl_cond = pl_cond->Suivant;
    }

    /*on a trouve une perturbation*/
    if (vl_res) {
      XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_tun_filtre %s domaine=%d",pl_cas->Libelle,va_domaine);
      if (va_noeud->Localisation.Sens != XDC_SENS_INCONNU) 
        drol_tun_recherche(va_noeud->Localisation.Autoroute, 
		va_noeud->Localisation.PR,
		va_noeud->Localisation.Sens%10,
		va_site,
		va_horodate,
  		va_noeud->Localisation_Tete.PR,
		va_noeud->Localisation.VL,
		va_noeud->Localisation.VM1,
		va_noeud->Localisation.VM2,
		va_noeud->Localisation.VR,
		va_noeud->Type,
		va_domaine,
		pl_cas,
		XDC_VOIE_SENS_NORMAL,NULL);
      else {
        drol_tun_recherche(va_noeud->Localisation.Autoroute, 
		va_noeud->Localisation.PR,
		XDC_SENS_SUD,
		va_site,
		va_horodate,
  		va_noeud->Localisation_Tete.PR,
		va_noeud->Localisation.VL,
		va_noeud->Localisation.VM1,
		va_noeud->Localisation.VM2,
		va_noeud->Localisation.VR,
		va_noeud->Type,
		va_domaine,
		pl_cas,
		XDC_VOIE_SENS_NORMAL,NULL);
        drol_tun_recherche(va_noeud->Localisation.Autoroute, 
		va_noeud->Localisation.PR,
	    	XDC_SENS_NORD,	
		va_site,
		va_horodate,
  		va_noeud->Localisation_Tete.PR,
		va_noeud->Localisation.VL,
		va_noeud->Localisation.VM1,
		va_noeud->Localisation.VM2,
		va_noeud->Localisation.VR,
		va_noeud->Type,
		va_domaine,
		pl_cas,
		XDC_VOIE_SENS_NORMAL,NULL);
      }

      /*si je suis sur la signalisation*/
      if (va_domaine==XDC_ACT_GTC_SIG) {
	/*si j ai un scenario en signalisation opposee*/
	if ( (pl_cas->Sequence_Opp>0) || (pl_cas->Sequence_Nat_Opp>0)){
	  /*je cherche un tunnel en sens oppose*/
	  if (va_noeud->Localisation.Sens%10==XDC_SENS_SUD)
	    vl_sens=XDC_SENS_NORD;
          else if (va_noeud->Localisation.Sens%10==XDC_SENS_NORD)
	    vl_sens=XDC_SENS_SUD;
	  else
	    vl_sens=XDC_SENS_INCONNU;
	  if (vl_sens!=XDC_SENS_INCONNU)
	    drol_tun_recherche(va_noeud->Localisation.Autoroute,
			va_noeud->Localisation_Tete.PR,
			vl_sens,
			va_site,
			va_horodate,
			va_noeud->Localisation.PR,
			va_noeud->Localisation.VL,
			va_noeud->Localisation.VM1,
			va_noeud->Localisation.VM2,
			va_noeud->Localisation.VR,
			va_noeud->Type,
			va_domaine,
			pl_cas,
			XDC_VOIE_SENS_INVERSE,NULL);
	}

        /*SAGA*/
	vl_min = va_noeud->Localisation.PR <= va_noeud->Localisation_Tete.PR ? va_noeud->Localisation.PR : va_noeud->Localisation_Tete.PR;
	vl_max = va_noeud->Localisation.PR >= va_noeud->Localisation_Tete.PR ? va_noeud->Localisation.PR : va_noeud->Localisation_Tete.PR;
	if (( va_noeud->Localisation.Autoroute==3) &&
	   (! ((vl_min > C_SAGA_PRMAX) || (vl_max < C_SAGA_PRMIN)))) {
	  if ((pl_cas->Saga_Sud !=0) && (va_noeud->Localisation.Sens%10==XDC_SENS_SUD)) {
	    sprintf(vl_str,"%s",pl_cas->Libelle);
	    if (XZAP70_Ajout_Prop_SAGA(vg_numero_plan,		
		vg_ordre_tun++,
		XDC_DP,
		"",
		pl_cas->Priorite,
		170,
		vl_str,
		"",
		"",
		"",
		"",
		"",
		"scenario SAGA",
		"",
		"",
		0,
		pl_cas->Saga_Sud,
		0,
		0,
		0,
		TRUE,0) != XDC_OK)
		XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tun : pb appel a XZAP70_Ajout_Prop_SAGA");
	  }
	  if ((pl_cas->Saga_Nord !=0) && (va_noeud->Localisation.Sens%10==XDC_SENS_NORD)) {
	    sprintf(vl_str,"%s",pl_cas->Libelle);
	    if (XZAP70_Ajout_Prop_SAGA(vg_numero_plan,		
		vg_ordre_tun++,
		XDC_DP,
		"",
		pl_cas->Priorite,
		170,
		vl_str,
		"",
		"",
		"",
		"",
		"",
		"scenario SAGA",
		"",
		"",
		0,
		pl_cas->Saga_Nord,
		0,
		0,
		0,
		TRUE,0) != XDC_OK)
		XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tun : pb appel a XZAP70_Ajout_Prop_SAGA");
	}
	}
      }
    }
    pl_cas=pl_cas->Suivant;
   }
  } /*fin vm_mode_new==0*/
  else {
  vm_mode_new=1;
  /*pour chacun des tunnels recuperes*/
  XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_tun_filtre NEW");
  while (pl_cas_gtc!=NULL)
  {
    XZST_03EcritureTrace(   XZSTC_DEBUG1,"drol_tun_filtre GTC %s domaine=%d",pl_cas_gtc->Libelle,va_domaine);
    pl_cond=pl_cas_gtc->Condition;
    XZST_03EcritureTrace(   XZSTC_DEBUG1,"libelle %s condition %s",
    				pl_cas_gtc->Libelle, pl_cas_gtc->Condition);
    vl_res=TRUE;

    while (pl_cond!=NULL)
    {
      if (pl_cond->Type==C_TYPE_COMPARATEUR)
      {
        vl_fct=pl_cond->Fonction.Comp.Fct;
        vl_res=vl_res && (*vl_fct)(*(pl_cond->Fonction.Comp.Slot),
                        pl_cond->Fonction.Comp.Valeur);

        if (!vl_res){
          break;
        }
      }
      else
      {
        vl_fct=pl_cond->Fonction.Predicat.Fct;
        vl_res = vl_res && (*vl_fct)();
        if (!vl_res) break;
      }
      pl_cond = pl_cond->Suivant;
    }

    /*on a trouve une perturbation*/
    if (vl_res) {
      XZST_03EcritureTrace( XZSTC_FONCTION,"drol_tun_filtre GTC %s domaine=%d",pl_cas_gtc->Libelle,va_domaine );
      XZST_03EcritureTrace( XZSTC_FONCTION,"libelle %s condition %s",pl_cas_gtc->Libelle, pl_cas_gtc->Condition );
      if (va_noeud->Localisation.Sens != XDC_SENS_INCONNU) 
        drol_tun_recherche(va_noeud->Localisation.Autoroute, 
		va_noeud->Localisation.PR,
		va_noeud->Localisation.Sens%10,
		va_site,
		va_horodate,
  		va_noeud->Localisation_Tete.PR,
		va_noeud->Localisation.VL,
		va_noeud->Localisation.VM1,
		va_noeud->Localisation.VM2,
		va_noeud->Localisation.VR,
		va_noeud->Type,
		va_domaine,
		NULL,
		XDC_VOIE_SENS_NORMAL,pl_cas_gtc);
      else {
        drol_tun_recherche(va_noeud->Localisation.Autoroute, 
		va_noeud->Localisation.PR,
		XDC_SENS_SUD,
		va_site,
		va_horodate,
  		va_noeud->Localisation_Tete.PR,
		va_noeud->Localisation.VL,
		va_noeud->Localisation.VM1,
		va_noeud->Localisation.VM2,
		va_noeud->Localisation.VR,
		va_noeud->Type,
		va_domaine,
		NULL,
		XDC_VOIE_SENS_NORMAL, pl_cas_gtc);
        drol_tun_recherche(va_noeud->Localisation.Autoroute, 
		va_noeud->Localisation.PR,
	    	XDC_SENS_NORD,	
		va_site,
		va_horodate,
  		va_noeud->Localisation_Tete.PR,
		va_noeud->Localisation.VL,
		va_noeud->Localisation.VM1,
		va_noeud->Localisation.VM2,
		va_noeud->Localisation.VR,
		va_noeud->Type,
		va_domaine,
		NULL,
		XDC_VOIE_SENS_NORMAL, pl_cas_gtc);
      }

      /*si je suis sur la signalisation*/
      if (va_domaine==XDC_ACT_GTC_SIG) {
	/*si j ai un scenario en signalisation opposee*/
     pl_scenario = pl_cas_gtc->Scenarios;
     while (pl_scenario !=NULL) {
       vl_zone = pl_scenario->Zone;
       if ((vl_zone->Autoroute ==va_noeud->Localisation.Autoroute) &&
	   (vl_zone->Sens == va_noeud->Localisation.Sens) &&
	   (vl_zone->Debut <= va_noeud->Localisation.PR) &&
	   (vl_zone->Fin >= va_noeud->Localisation.PR)
          ) {
		break;
       }
       pl_scenario = pl_scenario->suivant;
     }
    if (pl_scenario == NULL) {
       XZST_03EcritureTrace(XZSTC_FONCTION, 
       		"drol_tun_recherche : pas de zone trouvee");
    }
	else {
	XZST_03EcritureTrace(XZSTC_FONCTION,"SCENARIO %d %d %d %d",
			pl_scenario->Scenario,
			pl_scenario->Scenario_Oppose,
			pl_scenario->Scenario_Nat,
			pl_scenario->Scenario_Nat_Oppose);
	  /*je cherche un tunnel en sens oppose*/
	  if (va_noeud->Localisation.Sens%10==XDC_SENS_SUD)
	    vl_sens=XDC_SENS_NORD;
          else if (va_noeud->Localisation.Sens%10==XDC_SENS_NORD)
	    vl_sens=XDC_SENS_SUD;
	  else
	    vl_sens=XDC_SENS_INCONNU;
	  if (vl_sens!=XDC_SENS_INCONNU)
	    drol_tun_recherche(va_noeud->Localisation.Autoroute,
			va_noeud->Localisation_Tete.PR,
			vl_sens,
			va_site,
			va_horodate,
			va_noeud->Localisation.PR,
			va_noeud->Localisation.VL,
			va_noeud->Localisation.VM1,
			va_noeud->Localisation.VM2,
			va_noeud->Localisation.VR,
			va_noeud->Type,
			va_domaine,
			NULL,
			XDC_VOIE_SENS_INVERSE,pl_cas_gtc);
	}
	}
	}
    pl_cas_gtc=pl_cas_gtc->suivant;
  }
  }
  for (vl_indice=0;vl_indice<C_MAX_TUN;vl_indice++)
  {
    if (  (tm_tunnels[vl_indice].Numero!=0)  ) {
      XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tunnel(%d) : renove = %d, .SequenceC_Nat=%d, domaine = %d",
						vl_indice,
						tm_tunnels[vl_indice].Renove,
						tm_tunnels[vl_indice].SequenceC_Nat,
						va_domaine);
       /* si pas d'evt utilisant le tunnel alors on met la priorite U a 254 */
	 if (tm_tunnels[vl_indice].NumEvtU==0)
		tm_tunnels[vl_indice].PrioriteU=254;

      /*si c'est une simulation*/
      vl_mode_automatique = tm_tunnels[vl_indice].Automatique;
      if (drol_tun_fmc_simulee()) {
        /*on ne fait que proposer, pas de pilotage automatique*/
        vl_mode_automatique=0;
      }

      /* si le mode est non auto ou
	  si la priorite du pilotage en cours sur un autre evt est superieure
	 alors on passe en proposition PA */
	 XZST_03EcritureTrace(XZSTC_INFO,"Auto=%d:::%d",vl_mode_automatique,((tm_tunnels[vl_indice].PrioriteEvtC >= tm_tunnels[vl_indice].PrioriteU) && !((vm_noeud->NumEvt.numero==tm_tunnels[vl_indice].NumEvtU) && (vm_noeud->NumEvt.cle==tm_tunnels[vl_indice].CleEvtU) && (tm_tunnels[vl_indice].PrioriteU!=0))));

	 XZST_03EcritureTrace(XZSTC_INFO,"PrioriteEvtC=%d,PrioriteU=%d,umEvt.numero=%d,NumEvtU=%d,NumEvt.cle=%d,CleEvtU=%d",tm_tunnels[vl_indice].PrioriteEvtC,tm_tunnels[vl_indice].PrioriteU,vm_noeud->NumEvt.numero,tm_tunnels[vl_indice].NumEvtU,vm_noeud->NumEvt.cle,tm_tunnels[vl_indice].CleEvtU);

     /* si le mode est non auto ou
	si c'est un autre evt que celui qui pilote l'eqt 
	alors on passe en proposition PA */
							/* Pour l'instant, un evt plus prio ne fait pas conserver l'automatisme */
      if ((vl_mode_automatique==0)  || ( (tm_tunnels[vl_indice].PrioriteEvtC >= tm_tunnels[vl_indice].PrioriteU) && !((vm_noeud->NumEvt.numero==tm_tunnels[vl_indice].NumEvtU) && (vm_noeud->NumEvt.cle==tm_tunnels[vl_indice].CleEvtU) && (tm_tunnels[vl_indice].PrioriteU!=0))))   {
	if (va_domaine==XDC_ACT_GTC_SIG) {
	  /*traitement particulier pour la sequence nature*/
	  if ( (tm_tunnels[vl_indice].SequenceC_Nat > 0) && (tm_tunnels[vl_indice].Renove) &&
	       (tm_tunnels[vl_indice].SequenceC_Nat != tm_tunnels[vl_indice].SequenceNat)
	     ){
            /*mise en base de la proposition tunnel*/
            if (XZAP04_Ajout_Prop_Tunnel(vg_numero_plan,
			  vg_ordre_tun++,
			  tm_tunnels[vl_indice].District,
			  "",
			  tm_tunnels[vl_indice].PrioriteEvtC,
			  tm_tunnels[vl_indice].Numero,
			  tm_tunnels[vl_indice].LibelleNat, 
			  "Signal Nature" ,
			  tm_tunnels[vl_indice].SequenceC_Nat,
			  TRUE,
			  tm_tunnels[vl_indice].Renove ? va_domaine : XDC_ACT_TUNNEL) != XDC_OK) {
              XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tunnel : pb appel a XZAP04_Ajout_Prop_Tunnel");
	    }
	  }
	}

        /*mise en base de la proposition tunnel*/
	 vl_sequence =  va_domaine == XDC_ACT_TUNNEL ? tm_tunnels[vl_indice].Sequence : va_domaine==XDC_ACT_GTC_SIG ? tm_tunnels[vl_indice].Sequence:  va_domaine==XDC_ACT_GTC_VEN ? tm_tunnels[vl_indice].SequenceVEN : tm_tunnels[vl_indice].SequenceECL;
	 if ( (tm_tunnels[vl_indice].SequenceC > 0) &&
	      (tm_tunnels[vl_indice].SequenceC != vl_sequence)) {
           if (XZAP04_Ajout_Prop_Tunnel(vg_numero_plan,
			  vg_ordre_tun++,
			  tm_tunnels[vl_indice].District,
			  "",
			  tm_tunnels[vl_indice].PrioriteEvtC,
			  tm_tunnels[vl_indice].Numero,
			  tm_tunnels[vl_indice].Libelle, 
			  va_domaine == XDC_ACT_TUNNEL ? "" : va_domaine==XDC_ACT_GTC_SIG ? "Signal" : va_domaine==XDC_ACT_GTC_VEN ? "Ventil" : "Eclair",
			  tm_tunnels[vl_indice].SequenceC,
			  TRUE,
			  tm_tunnels[vl_indice].Renove ? va_domaine : XDC_ACT_TUNNEL) != XDC_OK) {
              XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tunnel : pb appel a XZAP04_Ajout_Prop_Tunnel");
	   }
	 }
       }
       else {
         /*premier cas : on est sur le district de l equipement*/
         if (tm_tunnels[vl_indice].District == vg_site) {
	   XZST_03EcritureTrace(XZSTC_WARNING,"drol_tunnel: %d,%d,%d,%d,%d,%d",
					vm_noeud->NumEvt.numero,
					vm_noeud->NumEvt.cle,
					vg_num_poste,
					tm_tunnels[vl_indice].Numero,
					tm_tunnels[vl_indice].PrioriteEvtC,
					tm_tunnels[vl_indice].SequenceC);
	   /*pilotage suivant le type de domaine*/
	   vl_domaine = tm_tunnels[vl_indice].Renove ? va_domaine : XDC_ACT_TUNNEL;

	   switch (vl_domaine) {
		case (XDC_ACT_TUNNEL) : if (XZAC06_Commande_Tunnel ( vm_noeud->NumEvt.numero    ,
				      	 	 vm_noeud->NumEvt.cle     ,
				      		  vg_num_poste ,
			              		  tm_tunnels[vl_indice].Numero       ,
			               		 va_horodate,
				       		 0   ,
				       		 tm_tunnels[vl_indice].PrioriteEvtC     ,
				       		 0    ,
				       		 tm_tunnels[vl_indice].SequenceC      ) != XDC_OK)
             						XZST_03EcritureTrace(XZSTC_WARNING,"drol_tunnel: pb appel a XZAC06_Commande_Tunnel");
           				else
	     					XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tunnel : appel a XZAC06_Commande_Tunnel OK");
					break;

		case (XDC_ACT_GTC_VEN) : if (XZAC102_Commande_Seq_VEN (vm_noeud->NumEvt.numero    ,
						vm_noeud->NumEvt.cle     ,
						vg_num_poste ,
						tm_tunnels[vl_indice].Numero       ,
						XDC_EQT_TUB,
						va_horodate,
						0,
						tm_tunnels[vl_indice].PrioriteEvtC     ,
						0,
						tm_tunnels[vl_indice].SequenceC      ) != XDC_OK)
							XZST_03EcritureTrace(XZSTC_WARNING,"drol_tunnel: pb appel a XZAC102_Commande_Seq_VEN");
					 else
							XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tunnel : appel a XZAC102_Commande_Seq_VEN OK");
					break;

		case (XDC_ACT_GTC_SIG) : if (XZAC100_Commande_Seq_SIG (vm_noeud->NumEvt.numero    ,
						vm_noeud->NumEvt.cle     ,
						vg_num_poste ,
						XDC_EQT_TUB,
						tm_tunnels[vl_indice].Numero       ,
						va_horodate,
						0,
						tm_tunnels[vl_indice].PrioriteEvtC     ,
						0,
						tm_tunnels[vl_indice].SequenceC      ) != XDC_OK)
							XZST_03EcritureTrace(XZSTC_WARNING,"drol_tunnel: pb appel a XZAC100_Commande_Seq_SIG");
					else
							XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tunnel : appel a XZAC100_Commande_Seq_SIG OK");
					break;

		case (XDC_ACT_GTC_ECL) : if (XZAC108_Commande_Seq_ECL (vm_noeud->NumEvt.numero    ,
						vm_noeud->NumEvt.cle     ,
						vg_num_poste ,
						XDC_EQT_TUB,
						tm_tunnels[vl_indice].Numero       ,
						va_horodate,
						0,
						tm_tunnels[vl_indice].PrioriteEvtC     ,
						0,
						tm_tunnels[vl_indice].SequenceC      ) != XDC_OK)
							XZST_03EcritureTrace(XZSTC_WARNING,"drol_tunnel: pb appel a XZAC108_Commande_Seq_ECL");
					else
						XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tunnel : appel a XZAC108_Commande_Seq_ECL OK");
					break;
	   }
         }
	 else { /*districts differents*/
	   switch (vl_domaine) {
	   	case (XDC_ACT_TUNNEL) : if (XZAC106_Commande_Tunnel_TACLI ( vm_noeud->NumEvt.numero    ,
				        			vm_noeud->NumEvt.cle     ,
				        			vg_num_poste ,
			                			tm_tunnels[vl_indice].Numero       ,
			                			va_horodate,
				       				0   ,
				        			tm_tunnels[vl_indice].PrioriteEvtC     ,
				        			0    ,
				        			tm_tunnels[vl_indice].SequenceC,
								tm_tunnels[vl_indice].District) != XDC_OK)
             					XZST_03EcritureTrace(XZSTC_WARNING,"drol_tunnel: pb appel a XZAC06_Commande_Tunnel");
           				else
	     					XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tunnel : appel a XZAC06_Commande_Tunnel OK");
					break;

		case (XDC_ACT_GTC_SIG) : break;

		case (XDC_ACT_GTC_VEN) : break;

		case (XDC_ACT_GTC_ECL) : break;
	   }
           if (XZAP04_Ajout_Prop_Tunnel(vg_numero_plan,
			  vg_ordre_tun++,
			  tm_tunnels[vl_indice].District,
			  "",
			  255,
			  tm_tunnels[vl_indice].Numero,
			  tm_tunnels[vl_indice].Libelle, 
			  va_domaine == XDC_ACT_TUNNEL ? "SAPIENS" : va_domaine==XDC_ACT_GTC_SIG ? "SAP Signal" : va_domaine==XDC_ACT_GTC_VEN ? "SAP Ventil" : "SAP Eclair",
			  tm_tunnels[vl_indice].SequenceC,
			  TRUE,
			  tm_tunnels[vl_indice].Renove ? va_domaine : XDC_ACT_TUNNEL) != XDC_OK)
            XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tunnel : pb appel a XZAP04_Ajout_Prop_Tunnel");


       }
     }
   } 
  }
  return (TRUE);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des tunnels
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_tun_recherche(XDY_Autoroute va_autoroute,
		        XDY_PR va_PR,
			XDY_Sens va_sens,
			XDY_District va_site,
			XDY_Horodate va_horodate,
			XDY_PR va_PR_tete,
			int va_VL,
			int va_VM1,
			int va_VM2,
			int va_VR,
			int va_type,
			XDY_Octet va_domaine,
			T_PERT_TUN *pa_pert,
			XDY_Octet va_type_sens,
			T_PERT_GTC *pa_pert_gtc)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
------------------------------------------------------*/
{
  XDY_Mot         Eqt=0;
  XDY_District    DistrictEqt;
  XDY_Autoroute   AutorouteEqt;
  XDY_PR          PREqt;
  XDY_Sens        SensEqt;
  XDY_Entier      PRInfluence;
  XDY_Octet       Type;
  XDY_Octet       Dispo;
  T_STRING        Mode;
  XDY_Entier      NumEvt;
  XDY_Octet       CleEvt;
  XDY_Octet       Priorite;
  XDY_Entier      DistanceEvt;
  int		vl_indice;
  T_PERT_TUN	vl_pert;
  T_Scenario *pl_scenario;
  T_ZONE_GTC	*vl_zone;
	XDY_Sens vl_sens;

  XZST_03EcritureTrace(   XZSTC_FONCTION,"IN : x drol_tun_recherche aut=%d,PR=%d,sens=%d,pert=%s, pert_new=%s %d",
						va_autoroute,va_PR,va_sens,pa_pert->Libelle,pa_pert_gtc->Libelle,vm_mode_new);

  if (((va_sens == XDC_SENS_INCONNU) || 
      (va_sens == XDC_SENS_ENTREE_NORD) || 
      (va_sens == XDC_SENS_ENTREE_SUD)) && 
      (vm_mode_new == 0)) 
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, "drol_tun_recherche : OUT evt sur une entree");
    return (XDC_NOK);
  }

  DistrictDemandeur = va_site;
  HorodateDemande = va_horodate;

  /* si mode new gtc*/
  if (vm_mode_new==0)
    vl_pert = *pa_pert;
  else  
  {
    if (va_type_sens==XDC_VOIE_SENS_NORMAL)
    	vl_sens = va_sens; 
    else 
    {
	    if (va_sens==XDC_SENS_NORD)
		    vl_sens=XDC_SENS_SUD;
	    else
		    vl_sens=XDC_SENS_NORD;
    }
    strcpy(vl_pert.Libelle, pa_pert_gtc->Libelle);
    vl_pert.Priorite = pa_pert_gtc->Priorite;
    /*recherche du bon scenario en fonction de la zone de la fmc*/
     pl_scenario = pa_pert_gtc->Scenarios;
     while (pl_scenario !=NULL) 
     {
        vl_zone = pl_scenario->Zone;
        if ((vl_zone->Autoroute ==va_autoroute) &&
          (vl_zone->Sens == vl_sens) &&
          (vl_zone->Debut <= va_PR) &&
          (vl_zone->Fin >= va_PR) ) 
        { 
          break;     
        }
       pl_scenario = pl_scenario->suivant;
     }
     if (pl_scenario == NULL) 
     {
       XZST_03EcritureTrace(XZSTC_FONCTION, "drol_tun_recherche : pas de zone trouvee");
       	return (XDC_OK);
     }
    XZST_03EcritureTrace(XZSTC_FONCTION, 
       		"drol_tun_recherche : trouvee");
    vl_pert.Automatique = pl_scenario->Automatique;
    vl_pert.Sequence = pl_scenario->Scenario;
    vl_pert.Sequence_Nat = pl_scenario->Scenario_Nat;
    vl_pert.Sequence_Nat_Opp = pl_scenario->Scenario_Nat_Oppose;
    vl_pert.Sequence_Opp = pl_scenario->Scenario_Oppose;
    vl_pert.Filtre_PSTT = pl_scenario->Filtre_PSTT;
    strcpy(vl_pert.Groupe , pl_scenario->Groupe);
    vl_pert.Type_Zone = vl_zone->Type_Zone;
    strcpy(vm_nom_scenario,pl_scenario->Scenario_Nom);
    strcpy(vm_nom_scenario_nat,pl_scenario->Scenario_Nat_Nom);
    strcpy(vm_nom_scenario_opp,pl_scenario->Scenario_Oppose_Nom);
    strcpy(vm_nom_scenario_nat_opp,pl_scenario->Scenario_Nat_Oppose_Nom);
  XZST_03EcritureTrace(XZSTC_FONCTION,"JMGLCL %s %s %s %s",
  		vm_nom_scenario,vm_nom_scenario_nat,vm_nom_scenario_opp,vm_nom_scenario_nat_opp);
  }

  XZST_03EcritureTrace(XZSTC_FONCTION, "drol_tun_recherche : avant XZAO20");
  /*appelle XZAO20*/
  if (va_sens>10) {
  XZAO20_Rech_Tunnels(va_autoroute, va_PR, va_sens-10, (XDY_FonctionInt)FctTunnel,va_horodate,va_PR_tete,
				vl_pert.Automatique,
				vl_pert.Libelle,
				vl_pert.Priorite,
				va_domaine,
				vl_pert.Type_Zone,
				vl_pert.Sequence,
				vl_pert.Sequence_Nat,
				vl_pert.Sequence_Opp,
				vl_pert.Sequence_Nat_Opp,
				va_type_sens,
				vl_pert.Groupe,
				vl_pert.Filtre_PSTT) ;
  }
  else
  XZAO20_Rech_Tunnels(va_autoroute, va_PR, va_sens, (XDY_FonctionInt)FctTunnel,va_horodate,va_PR_tete,
				vl_pert.Automatique,
				vl_pert.Libelle,
				vl_pert.Priorite,
				va_domaine,
				vl_pert.Type_Zone,
				vl_pert.Sequence,
				vl_pert.Sequence_Nat,
				vl_pert.Sequence_Opp,
				vl_pert.Sequence_Nat_Opp,
				va_type_sens,
				vl_pert.Groupe,
				vl_pert.Filtre_PSTT) ;
  
XZST_03EcritureTrace(XZSTC_FONCTION,"APRES XZAO20");
  
  for (vl_indice=0;vl_indice<C_MAX_TUN;vl_indice++) {

  if (tm_tunnels[vl_indice].Numero==0)
    continue;

  /*suppression des utilisations identiques a l'utilisation courante*/
  if (tm_tunnels[vl_indice].SequenceC==tm_tunnels[vl_indice].Sequence) {
   /*mise en base de la proposition avec priorite=255 si prop identique a l'utilisation courante*/
/*   if (XZAP04_Ajout_Prop_Tunnel(vg_numero_plan,
			  vg_ordre_tun++,
			  tm_tunnels[vl_indice].District,
			  "",
			  255,
			  tm_tunnels[vl_indice].Numero,
			  tm_tunnels[vl_indice].Libelle, 
			  "",
			  tm_tunnels[vl_indice].SequenceC,
			  TRUE,
			  tm_tunnels[vl_indice].Renove ? va_domaine : XDC_ACT_TUNNEL) != XDC_OK)
          XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tunnel : pb appel a XZAP04_Ajout_Prop_Tunnel");
    XZST_03EcritureTrace(XZSTC_FONCTION,"proposition identique a l'utilisation courante. On la supprime");*/

    /* JMG PATCH*/
    /*tm_tunnels[vl_indice].Numero=0;*/
  }
  }

  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  creation d'un tunnel dans le RTie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int FctTunnel(Tunnel,Automatique, Perturbation, Priorite, Domaine, Type_Zone, Sequence1, Sequence2, Sequence3, Sequence4,Type_Sens,NomTunnel,Groupe,Filtre_PSTT,VR, VM2, VM1, VL, BAU)
XDY_Mot         Tunnel;
XDY_Octet	Automatique;
char		*Perturbation;
int		Priorite;
XDY_Octet	Domaine;
XDY_Octet	Type_Zone;
XDY_Octet	Sequence1;
XDY_Octet	Sequence2;
XDY_Octet	Sequence3;
XDY_Octet	Sequence4;
XDY_Octet	Type_Sens;
char		*NomTunnel;
char	*Groupe;
int	Filtre_PSTT;
XDY_Voie VR;
XDY_Voie VM2;
XDY_Voie VM1;
XDY_Voie VL;
XDY_Voie BAU;

/*
* ARGUMENTS EN ENTREE :
*   XDY_Eqt             Tunnel;
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*   fonction passee en argument a XZAO20
*
* FONCTION
*   creation d'une instance de la classe tunnel dans le RTie
*   appel de XZAT07 et mise a jour des attributs du tunnel
*
------------------------------------------------------*/
{
        XDY_District    District=XDC_VC;
        XDY_Octet       Dispo;
        XDY_Octet       Sequence;
        XDY_Octet       SequenceNat;
        XDY_Entier      NumEvt;
        XDY_Octet       CleEvt;
        XDY_Octet       vl_Priorite;
	XDY_Entier	DistanceEvt;
	int		vl_indice;
	XDY_Octet	vl_numero_zone=0;
	XDY_Booleen	vl_renove;
	char	vl_groupe[5];
	XDY_Booleen (*vl_fct)();
	XDY_Octet	vl_type_zone=0;
	T_CONDITION	*pl_cond;
	XDY_Booleen	vl_res;
	char vl_trigramme[10];
	XDY_PR			vl_prpstt;
	XDY_Voie vl_vr,vl_vm2,vl_vm1,vl_vl,vl_bau;
	T_PERT_TUN *pl_cas;
	T_PERT_GTC *pl_cas_gtc;
	XZST_03EcritureTrace(XZSTC_WARNING,"FctTunnel");
        XZST_03EcritureTrace(XZSTC_FONCTION, "IN : Tunnel = %d(%s), domaine=%d",Tunnel,NomTunnel,Domaine);

	XZST_03EcritureTrace(XZSTC_FONCTION, "%d %d %d %d-%d %d",
			Sequence1, Sequence2, Sequence3,Sequence4,District,DistrictDemandeur);
	/*A
        **  on verifie que l'objet appartient bien au district
        */
        /*if ((District != DistrictDemandeur) && (DistrictDemandeur != XDC_CI))
        {
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT");
                return(XDC_OK);
        }*/

	/*recherche si le tunnel a deja ete traite*/
	for (vl_indice=0;vl_indice<C_MAX_TUN;vl_indice++)
	{
	  if (tm_tunnels[vl_indice].Numero==Tunnel) {
	    XZST_03EcritureTrace(XZSTC_FONCTION, " Le tunnel %d existe deja",Tunnel);
	    break;
          }
	}

        /*A
        **  on appelle XZAT07
        */
	/*le tunnel n a pas encore ete utilise*/
	if (vl_indice==C_MAX_TUN) {
	  /*je commence par regarder si mon tunnel est renove*/
	  XZLG300_trim(NomTunnel);
	  vl_renove=drol_tun_renove(NomTunnel,vl_trigramme);

	  /*s'il n est pas renove et que le domaine n'est pas signal, alors sortir*/
	  if ((!vl_renove) && (Domaine!=XDC_ACT_GTC_SIG)) {
	    XZST_03EcritureTrace(XZSTC_FONCTION, " Le tunnel %d non renove, et domaine %d => bye",
							Tunnel,Domaine);
            return (XDC_OK);
          }

          if (XZAT07_Utilisation_Tunnel(  HorodateDemande, Tunnel,Domaine,
                                        &Dispo, &Sequence, &NumEvt, &CleEvt, &vl_Priorite, &DistanceEvt,&SequenceNat) != XDC_OK)
          {
                XZST_03EcritureTrace(XZSTC_WARNING,
                        "%s etat de %s inconnu a la date %f. On le considere indispo.",version, NomTunnel, HorodateDemande);
                Dispo = XDC_EQT_MAJEUR;
          }

	  /*je cherche un tunnel disponible*/
	  for (vl_indice=0;vl_indice<C_MAX_TUN;vl_indice++)
	  {
		if (tm_tunnels[vl_indice].Numero==0)
			break;
	  }
          /*A
          **  on cree l'objet et on met a jour les slots
          **  identifiant, dispo, sequence, numevt_u, priorite_u, distance_u
          */
          XZST_03EcritureTrace(XZSTC_INFO, "creation du tunnel %d(last=%d) evt utilisateur : %d",Tunnel,vl_indice,NumEvt);
	  tm_tunnels[vl_indice].Numero=Tunnel;
	  tm_tunnels[vl_indice].District=District;
	  if (Domaine==XDC_ACT_GTC_SIG) {
	  	tm_tunnels[vl_indice].Sequence=Sequence;
		tm_tunnels[vl_indice].SequenceNat=SequenceNat;
	  }
	  else if (Domaine==XDC_ACT_GTC_ECL)
		tm_tunnels[vl_indice].SequenceECL = Sequence;
	  else
		tm_tunnels[vl_indice].SequenceVEN = Sequence;
	  tm_tunnels[vl_indice].SequenceC=0;
	  tm_tunnels[vl_indice].SequenceC_Nat=0;
	  tm_tunnels[vl_indice].Renove=vl_renove;
	  XZST_03EcritureTrace(XZSTC_INFO, "renove(%d) = %d",vl_indice,tm_tunnels[vl_indice].Renove);
	  strcpy(tm_tunnels[vl_indice].Libelle,"");
	  strcpy(tm_tunnels[vl_indice].Nom,NomTunnel);
	  XZST_03EcritureTrace(XZSTC_INFO, "renove(%d) = %d",vl_indice,tm_tunnels[vl_indice].Renove);
	  tm_tunnels[vl_indice].PrioriteU=0;
	  tm_tunnels[vl_indice].DistanceEvtC=0;
	  tm_tunnels[vl_indice].PrioriteEvtC=255;
	  tm_tunnels[vl_indice].VR = VR;
	  tm_tunnels[vl_indice].VM2 = VM2;
	  tm_tunnels[vl_indice].VM1 = VM1;
	  tm_tunnels[vl_indice].VL = VL;
	  tm_tunnels[vl_indice].BAU = BAU;
	XZST_03EcritureTrace(XZSTC_INFO,"XZAO20 tm_tunnels[vl_indice].VR = %d, tm_tunnels[vl_indice].VL=%d",
					VR,VL);

          if (Dispo <= XDC_EQT_MINEUR)
		  tm_tunnels[vl_indice].Dispo=TRUE;
          else
		  tm_tunnels[vl_indice].Dispo=FALSE;
          if (NumEvt != 0)
          {
		tm_tunnels[vl_indice].NumEvtU=NumEvt;
		tm_tunnels[vl_indice].CleEvtU=CleEvt;
		tm_tunnels[vl_indice].PrioriteU=vl_Priorite;
		tm_tunnels[vl_indice].DistanceEvtU=DistanceEvt;
          }
	  else
	  {	tm_tunnels[vl_indice].NumEvtU=NumEvt;
		tm_tunnels[vl_indice].CleEvtU=CleEvt;
		/**tm_tunnels[vl_indice].PrioriteU=vl_Priorite=254;*/
		tm_tunnels[vl_indice].DistanceEvtU=DistanceEvt=500000;

	  }

        }

       /*je verifie a nouveau que la config des voies du TUNNEL */
        /*correspond bien a la definition de la perturbation*/
        switch (Domaine) {
          case (XDC_ACT_GTC_SIG) :
            pl_cas = vg_pert_sig;
            pl_cas_gtc = vg_pert_sig_new;
            break;
          case (XDC_ACT_GTC_ECL) :
            pl_cas = vg_pert_ecl;
            pl_cas_gtc = vg_pert_ecl_new;
            break;
          case (XDC_ACT_GTC_VEN):
            pl_cas=vg_pert_ven;
            pl_cas_gtc = vg_pert_ven_new;
            break;
        }

        /*on met de cote la config des voies*/
        vl_vr = vm_vr;
        vl_vm2 = vm_vm2;
        vl_vm1 = vm_vm1;
        vl_vl = vm_vl;
        vl_bau = vm_bau;
	if (vm_mode_new==0) {
         while (pl_cas!=NULL) {
          if (!strcmp(pl_cas->Libelle,Perturbation)) {
                XZST_03EcritureTrace(XZSTC_INFO,"on verifie a nouveau les conditions de %s",pl_cas->Libelle);
                /*on traite la config des voies avec CELLE du tunnel*/
		XZST_03EcritureTrace(XZSTC_WARNING,"VL = %d, VR = %d",
					tm_tunnels[vl_indice].VL,
					tm_tunnels[vl_indice].VR);
                vm_vr = tm_tunnels[vl_indice].VR ;
                vm_vm2 = tm_tunnels[vl_indice].VM2 ;
                vm_vm1 = tm_tunnels[vl_indice].VM1 ;
                vm_vl = tm_tunnels[vl_indice].VL ;
                vm_bau = tm_tunnels[vl_indice].BAU ;
                pl_cond=pl_cas->Condition;
                    vl_res=TRUE;
                while (pl_cond!=NULL)
                {
                        if (pl_cond->Type==C_TYPE_COMPARATEUR)
                        {
                                vl_fct=pl_cond->Fonction.Comp.Fct;
                                vl_res=vl_res && (*vl_fct)(*(pl_cond->Fonction.Comp.Slot),
                                        pl_cond->Fonction.Comp.Valeur);

                                if (!vl_res){
                                        break;
                                }
                        }
                        else
                        {
                                vl_fct=pl_cond->Fonction.Predicat.Fct;
                                vl_res = vl_res && (*vl_fct)();
                                if (!vl_res) break;
                        }
                        pl_cond = pl_cond->Suivant;
                }
	    break;
          }
          pl_cas=pl_cas->Suivant;
         }
        }
	else {
         while (pl_cas_gtc!=NULL) {
          if (!strcmp(pl_cas_gtc->Libelle,Perturbation)) {
                XZST_03EcritureTrace(XZSTC_INFO,"on verifie a nouveau les conditions de %s",pl_cas_gtc->Libelle);
                /*on traite la config des voies avec CELLE du tunnel*/
		XZST_03EcritureTrace(XZSTC_WARNING,"VL = %d, VR = %d",
					tm_tunnels[vl_indice].VL,
					tm_tunnels[vl_indice].VR);
                vm_vr = tm_tunnels[vl_indice].VR ;
                vm_vm2 = tm_tunnels[vl_indice].VM2 ;
                vm_vm1 = tm_tunnels[vl_indice].VM1 ;
                vm_vl = tm_tunnels[vl_indice].VL ;
                vm_bau = tm_tunnels[vl_indice].BAU ;
                pl_cond=pl_cas_gtc->Condition;
                    vl_res=TRUE;
                while (pl_cond!=NULL)
                {
                        if (pl_cond->Type==C_TYPE_COMPARATEUR)
                        {
                                vl_fct=pl_cond->Fonction.Comp.Fct;
                                vl_res=vl_res && (*vl_fct)(*(pl_cond->Fonction.Comp.Slot),
                                        pl_cond->Fonction.Comp.Valeur);

                                if (!vl_res){
                                        break;
                                }
                        }
                        else
                        {
                                vl_fct=pl_cond->Fonction.Predicat.Fct;
                                vl_res = vl_res && (*vl_fct)();
                                if (!vl_res) break;
                        }
                        pl_cond = pl_cond->Suivant;
                }
	    break;
          }
          pl_cas_gtc=pl_cas_gtc->suivant;
         }
        }

        /*on remet correctement la config des voies*/
        vm_vr = vl_vr;
        vm_vm2 = vl_vm2;
        vm_vm1 = vl_vm1;
        vm_vl = vl_vl;
        vm_bau = vl_bau;
        /******************************************/
        if (!vl_res) {
		if (vm_mode_new==0) 
                XZST_03EcritureTrace(XZSTC_FONCTION,"Perturbation %s NON VERIFIEE",pl_cas->Libelle);
		else
                XZST_03EcritureTrace(XZSTC_FONCTION,"Perturbation %s NON VERIFIEE",pl_cas_gtc->Libelle);
                return (XDC_OK);
        }
XZST_03EcritureTrace(XZSTC_FONCTION,"Perturbation les prio : U:%d;Priorite=%d;C=%d",tm_tunnels[vl_indice].PrioriteU,Priorite,tm_tunnels[vl_indice].PrioriteEvtC);
	/*le tunnel est-il utilise par un evt de priorite superieure */
	if (tm_tunnels[vl_indice].PrioriteU<Priorite) { /*non*/
	  /*le tunnel est il deja reserve par une autre perturbation pour cet evt ? */
	    if (tm_tunnels[vl_indice].PrioriteEvtC>Priorite) { /*non*/
	      XZST_03EcritureTrace(XZSTC_WARNING,"type_zone = %d",Type_Zone);
	      /*si on doit traiter par zone*/
	      if ( (Type_Zone!=0) ||(Filtre_PSTT!=0)) {
	      	/*recuperer la zone de l evt dans le tunnel*/
              	if (XZAO350_Zone_Tunnel(vm_noeud->Localisation.Autoroute,
					vm_noeud->Localisation_Tete.PR,
					vm_noeud->Localisation.PR,
					vm_noeud->Localisation.Sens%10,
					Tunnel,
					&vl_numero_zone,
					&vl_type_zone,
					vl_groupe,
					&vl_prpstt) != XDC_OK ) {
			XZST_03EcritureTrace(XZSTC_WARNING,"pb appel a XZAO350_Zone_Tunnel");
			if (vl_indice==C_MAX_TUN) 
				tm_tunnels[vl_indice].Numero=0;
			return (XDC_OK);
	      	}

	      	/*si pas de zone trouvee*/
	      	if (vl_numero_zone==0) {
			XZST_03EcritureTrace(XZSTC_INFO, "pas de zone trouvee");
			if (vl_indice==C_MAX_TUN) 
				tm_tunnels[vl_indice].Numero=0;
			return (XDC_OK);
              	}
	      	else 
			XZST_03EcritureTrace(XZSTC_WARNING,"vl_type_zone = %d, PR PSTT = %d",vl_type_zone,vl_prpstt);

	      	/* si on n est pas sur la bonne zone*/
	      	if ( (vl_type_zone!=Type_Zone) && (Type_Zone!=0)) {
	        	XZST_03EcritureTrace(XZSTC_INFO, "on n est pas sur la bonne zone (%d <> %d)",
					Type_Zone,vl_type_zone);
			if (vl_indice==C_MAX_TUN) 
				tm_tunnels[vl_indice].Numero=0;
			return (XDC_OK);
	      	}

		/*gestion du filtre PSTT*/
		if (Filtre_PSTT!=0) {
			/*si le basculement (PR Queue) est dans la zone tunnel alors  */
			/* picto basculement (la zone de depart tunnel pourra etre avancee jusqu'au PSTT par configuration)  */
			/*si le basculement (PR Queue) est en amont de la zone tunnel alors fermeture tunnel. */
			
			/*verifier PR queue avant le PRPSTT */
			if ( (vm_noeud->Localisation.Sens%10==XDC_SENS_SUD)  && (vm_noeud->Localisation.PR>vl_prpstt)) {
			  XZST_03EcritureTrace(XZSTC_WARNING,"PR queue apres le PRPSTT, on ne traite pas cette perturbation");
                          return (XDC_OK);
			}
			if ( (vm_noeud->Localisation.Sens%10==XDC_SENS_NORD)  && (vm_noeud->Localisation.PR<vl_prpstt)) {
			  XZST_03EcritureTrace(XZSTC_WARNING,"PR queue apres le PRPSTT, on ne traite pas cette perturbation");
			  return (XDC_OK);
                        }
		}
              }

	      /*si on n est pas sur le bon groupe*/
	      if (Type_Zone!=0) {
	      if (Domaine==XDC_ACT_GTC_VEN) {
	        if (strcmp(Groupe,vl_groupe)) {
		  XZST_03EcritureTrace(XZSTC_INFO, "on n est pas sur le bon groupe (%s:%d<>%s:%d)%d",
					Groupe,strlen(Groupe),vl_groupe,strlen(vl_groupe),strcmp(Groupe,vl_groupe));
		  if (vl_indice==C_MAX_TUN) 
			tm_tunnels[vl_indice].Numero=0;
		  return (XDC_OK);
	        }
	      }
	      }
	      /*************************************/

	      /*proposer une nouvelle sequence*/
	      if ((Domaine==XDC_ACT_GTC_ECL) ||(Domaine==XDC_ACT_GTC_VEN))  {
	        tm_tunnels[vl_indice].SequenceC=drol_tun_autorise(Tunnel,Sequence1,Domaine);
		if (vm_mode_new==0) 
	        strcpy(tm_tunnels[vl_indice].Libelle,pl_cas->Sequence_Nom);
	        else
	        strcpy(tm_tunnels[vl_indice].Libelle,vm_nom_scenario);
	      }
	      else { /*signal*/
	        /*sens normal*/
		if (Type_Sens==XDC_VOIE_SENS_NORMAL) {
		  tm_tunnels[vl_indice].SequenceC=drol_tun_autorise(Tunnel,Sequence1,
				tm_tunnels[vl_indice].Renove ? Domaine : XDC_ACT_TUNNEL);
		  tm_tunnels[vl_indice].SequenceC_Nat=drol_tun_autorise(Tunnel,Sequence2,Domaine);
		if (vm_mode_new==0) {
	        strcpy(tm_tunnels[vl_indice].Libelle,pl_cas->Sequence_Nom);
	        strcpy(tm_tunnels[vl_indice].LibelleNat,pl_cas->Sequence_Nat_Nom);
		}
		else {
	        strcpy(tm_tunnels[vl_indice].Libelle,vm_nom_scenario);
	        strcpy(tm_tunnels[vl_indice].LibelleNat,vm_nom_scenario_nat);
		}
		}
		else {
		  /*sens oppose*/
		  tm_tunnels[vl_indice].SequenceC=drol_tun_autorise(Tunnel,Sequence3,
				tm_tunnels[vl_indice].Renove ? Domaine : XDC_ACT_TUNNEL);
		  tm_tunnels[vl_indice].SequenceC_Nat=drol_tun_autorise(Tunnel,Sequence4,Domaine);
		if (vm_mode_new==0) {
	        strcpy(tm_tunnels[vl_indice].Libelle,pl_cas->Sequence_Opp_Nom);
	        strcpy(tm_tunnels[vl_indice].LibelleNat,pl_cas->Sequence_Nat_Opp_Nom);
		}
		else {
	        strcpy(tm_tunnels[vl_indice].Libelle,vm_nom_scenario_opp);
	        strcpy(tm_tunnels[vl_indice].LibelleNat,vm_nom_scenario_nat_opp);
		}
		}
	      }
	        /*strcpy(tm_tunnels[vl_indice].Libelle,Perturbation);*/
	      tm_tunnels[vl_indice].Automatique=Automatique;
	      tm_tunnels[vl_indice].PrioriteEvtC=Priorite;

	  }
	}

        if ((Domaine==XDC_ACT_GTC_ECL) ||(Domaine==XDC_ACT_GTC_VEN))
          XZST_03EcritureTrace(XZSTC_INFO,"drol_tunnel: sequence proposee %d - priorite %d -renove %d",
		tm_tunnels[vl_indice].SequenceC,
		tm_tunnels[vl_indice].PrioriteEvtC,
		tm_tunnels[vl_indice].Renove);	
	else
	  XZST_03EcritureTrace(XZSTC_INFO,"drol_tunnel: sequence proposee %d - sequence nat proposee %d - priorite %d - renove %d",
		tm_tunnels[vl_indice].SequenceC,
		tm_tunnels[vl_indice].SequenceC_Nat,
		tm_tunnels[vl_indice].PrioriteEvtC,
		tm_tunnels[vl_indice].Renove);

        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT");
        return(XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des tunnels en cas de basculement
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_tun_recherche_basculement(XDY_Autoroute va_autoroute,
		        XDY_PR va_PR,
			XDY_Sens va_sens,
			XDY_District va_site,
			XDY_Horodate va_horodate,
			XDY_PR va_PR_tete,
			int va_VL,
			int va_VM1,
			int va_VM2,
			int va_VR,
			int va_type,
			XDY_Octet va_domaine)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
------------------------------------------------------*/
{
  XDY_Mot         Eqt=NULL;
  XDY_District    DistrictEqt;
  XDY_Autoroute   AutorouteEqt;
  XDY_PR          PREqt;
  XDY_Sens        SensEqt;
  XDY_Entier      PRInfluence;
  XDY_Octet       Type;
  XDY_Octet       Dispo;
  T_STRING        Mode;
  XDY_Entier      NumEvt;
  XDY_Octet       CleEvt;
  XDY_Octet       Priorite;
  XDY_Entier      DistanceEvt;
  int		vl_indice;
  int		vl_sens;

  XZST_03EcritureTrace(   XZSTC_FONCTION,"IN : drol_tun_recherche_basculement aut=%d,PR=%d,sens=%d,site=%d",
						va_autoroute,va_PR,va_sens,va_site);

  if ((va_sens == XDC_SENS_INCONNU) || 
      (va_sens == XDC_SENS_ENTREE_NORD) || 
      (va_sens == XDC_SENS_ENTREE_SUD)  ) {
    XZST_03EcritureTrace(XZSTC_FONCTION, "drol_tun_recherche_basculement : OUT evt sur une entree");
    return (XDC_NOK);
  }

  DistrictDemandeur = va_site;
  HorodateDemande = va_horodate;

  /*appelle XZAO20*/
  /*XZAO20_Rech_Tunnels(va_autoroute, va_PR, va_sens, FctTunnel,va_horodate,va_PR_tete) ;*/

  /*sequence a basculement pour chaque tunnel*/
  for (vl_indice=0;vl_indice<C_MAX_TUN;vl_indice++)
  {
    if (tm_tunnels[vl_indice].Numero==0) break;

    /*recherche de la sequence tunnel*/ 
    if (tm_tunnels[vl_indice].SequenceC==0) {
      tm_tunnels[vl_indice].SequenceC=C_BASCULEMENT;
      strcpy(tm_tunnels[vl_indice].Libelle, C_LIB_BASCULEMENT);

      /*suppression des utilisations identiques a l'utilisation courante*/
      if (tm_tunnels[vl_indice].SequenceC==tm_tunnels[vl_indice].Sequence) {
        XZST_03EcritureTrace(XZSTC_FONCTION,"proposition identique a l'utilisation courante. On la supprime");
        tm_tunnels[vl_indice].Numero=0;
      }
    }
  }

  /*recherche des tunnels dans le sens oppose*/
  switch(va_sens) {
        case XDC_SENS_NORD :
        case XDC_SENS_SORTIE_NORD :
                vl_sens = XDC_SENS_SUD;
                break;
        case XDC_SENS_SUD :
        case XDC_SENS_SORTIE_SUD :
                vl_sens = XDC_SENS_NORD;
                break;
        default :
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s",version);
                return (XDC_NOK);
  }

  /*XZAO20_Rech_Tunnels(va_autoroute, va_PR, vl_sens, FctTunnel,va_horodate,va_PR_tete) ;*/

  /*sequence a double sens pour chaque tunnel*/
  for (vl_indice=0;vl_indice<C_MAX_TUN;vl_indice++)
  {
    if (tm_tunnels[vl_indice].Numero==0) break;

    if (tm_tunnels[vl_indice].SequenceC==0) {
      tm_tunnels[vl_indice].SequenceC=C_DOUBLE_SENS;
      strcpy(tm_tunnels[vl_indice].Libelle,C_LIB_DOUBLE_SENS);

      /*suppression des utilisations identiques a l'utilisation courante*/
      if (tm_tunnels[vl_indice].SequenceC==tm_tunnels[vl_indice].Sequence) {
   	/*mise en base de la proposition avec priorite=255 si prop identique a l'utilisation courante*/
   	if (XZAP04_Ajout_Prop_Tunnel(vg_numero_plan,
			  vg_ordre_tun++,
			  tm_tunnels[vl_indice].District,
			  "",
			  255,
			  tm_tunnels[vl_indice].Numero,
			  tm_tunnels[vl_indice].Libelle, 
			  "",
			  tm_tunnels[vl_indice].SequenceC,
			  TRUE,
			  va_domaine) != XDC_OK)
          	XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tunnel : pb appel a XZAP04_Ajout_Prop_Tunnel");
	XZST_03EcritureTrace(XZSTC_FONCTION,"proposition identique a l'utilisation courante. On la supprime");
	tm_tunnels[vl_indice].Numero=0;
      }
    }
  }
  return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  compare l'etat de chaque voie a l'etat de reference
*  tient compte du nombre de voies de circulation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_tun_voies(int VL, int VM1, int VM2, int VR, int Reference)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
------------------------------------------------------*/
{
  int Etat;

        if (       (VR != Reference)
                && (VM2 != Reference)
                && (VM1 != Reference)
                && (VL != Reference) )
                Etat = 0;       /* (aucune voie) */

        else if (  (VR != Reference) && (VR != XDC_VOIE_INEXISTANTE)
                && (VM2 != Reference)
                && (VM1 != Reference)
                && (VL == Reference) )
                Etat = 1;       /* (la voie de droite) */
        else if (  (VR != Reference) && (VR != XDC_VOIE_INEXISTANTE)
                && (VM2 != Reference)
                && (VM1 == Reference)
                && (VL == Reference) )
                Etat = 2;       /* (les deux voies de droite) */

        else if (  (VR != Reference) && (VR != XDC_VOIE_INEXISTANTE)
                && (VM2 == Reference)
                && (VM1 == Reference)
                && (VL == Reference) )
                Etat = 3;       /* (les trois voies de droite) */

        else if (  (VR == Reference)
                && (VM2 != Reference)
                && (VM1 != Reference)
                && (VL != Reference) )
                Etat = 4;       /* (la voie de gauche) */

        else if (  (VR == Reference)
                && (  ( (VM2 == Reference) && (VM1 != Reference) )
                   || ( (VM2 == XDC_VOIE_INEXISTANTE) && (VM1 == Reference) ) )
                && (VL != Reference) )
                Etat = 5;       /* (les deux voies de gauche) */

        else if (  (VR == Reference)
                && (VM2 == Reference)
                && (VM1 == Reference)
                && (VL != Reference) )
                Etat = 6;       /* (les trois voies de gauche) */

        else if (  (VR != Reference)
                && (  ( (VM2 != Reference) && (VM2 != XDC_VOIE_INEXISTANTE) && (VM1 == Reference) )
                   || ( (VM2 == Reference) && (VM1 != Reference) ) )
                && (VL != Reference) )
                Etat = 7;       /* (une voie centrale) */

        else if (  (VR != Reference)
                && (VM2 == XDC_VOIE_INEXISTANTE)
                && (VM1 == Reference)
                && (VL != Reference) )
                Etat = 8;       /* (la voie centrale) */

        else if (  (VR != Reference)
                && (VM2 == Reference)
                && (VM1 == Reference)
                && (VL != Reference) )
                Etat = 9;       /* (les voies centrales) */

        else if (  (  ( (VR == Reference) && (VM2 != Reference) && (VM2 != XDC_VOIE_INEXISTANTE) )
                   || ( (VR != Reference) && (VM2 == Reference) ) )
                && (VM1 == Reference)
                && (VL == Reference) )
                Etat = 10;      /* (sauf sur une voie centrale) */

        else if (  (VR == Reference)
                && (VM2 == XDC_VOIE_INEXISTANTE)
                && (VM1 != Reference) && (VM1 != XDC_VOIE_INEXISTANTE)
                && (VL == Reference) )
                Etat = 11;      /* (sauf sur la voie centrale) */

        else if (  (VR == Reference)
                && (VM2 != Reference) && (VM2 != XDC_VOIE_INEXISTANTE)
                && (VM1 != Reference) && (VM1 != XDC_VOIE_INEXISTANTE)
                && (VL == Reference) )
                Etat = 12;      /* (sauf sur les voies centrales) */

        else if (  (VR == Reference)
                && (VM2 != XDC_VOIE_INEXISTANTE) && (VM2 != Reference)
                && (VM1 == Reference)
                && (VL != Reference) )
                Etat = 13;      /* (sauf sur la voie de gauche et une voie centrale) */

        else if (  (VR != Reference)
                && (VM2 == Reference)
                && (VM1 != XDC_VOIE_INEXISTANTE) && (VM1 != Reference)
                && (VL == Reference) )
                Etat = 14;      /* (sauf sur une voie centrale et la voie de droite) */

        else if (  ( (VR == Reference) || (VR == XDC_VOIE_INEXISTANTE) )
                && ( (VM2 == Reference) || (VM2 == XDC_VOIE_INEXISTANTE) )
                && ( (VM1 == Reference) || (VM1 == XDC_VOIE_INEXISTANTE) )
                && (VL == Reference) )
                Etat = 15;      /* (toutes les voies) */

  return (Etat);

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static XDY_Booleen drol_tun_renove(char *va_nom, char *va_trig)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
------------------------------------------------------*/
{

  FILE *pl_fd=NULL;
  char vl_str[200];
  char vl_nom[50];
  char vl_trigramme[10];
  int vl_indice;

   XZST_03EcritureTrace(XZSTC_INTERFACE,"drol_tun_renove: tunnel = %s",va_nom);

	/* tous les tunnels sont renoves*/
	return (TRUE);

  /*on cherche le libelle du tunnel par rapport a son nom en base*/
  for (vl_indice=0;vl_indice< C_NB_TUN;vl_indice++) {
      XZST_03EcritureTrace(XZSTC_FONCTION,"tunnel %s",tm_liste_tunnels[vl_indice].NomBase);
    if (!strcmp(va_nom,tm_liste_tunnels[vl_indice].NomBase)) {
      XZST_03EcritureTrace(XZSTC_FONCTION,"tunnel %s => %s",
				va_nom,
				tm_liste_tunnels[vl_indice].Libelle);
      break;
    }
  }
  if (vl_indice==C_NB_TUN) {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_renove : pas de libelle pour %s",va_nom);
    return (FALSE);
  }
  /*ouverture du fichier*/
  pl_fd=fopen("/produits/migrazur/appliSD/fichiers/renov/ConfigMenu.cfg","r");

  if (pl_fd==NULL) {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_renove: impossible d ouvrir le fichier /renov/ConfigMenu.cfg");
    return (FALSE);
  }

  /*parcourt le fichier*/
  while (fgets(vl_str,sizeof(vl_str),pl_fd)!= NULL) {
    if (vl_str[0]=='#')
	continue;

    if (sscanf(vl_str,"%[^,],%s",vl_nom,vl_trigramme)!=2) {
      XZST_03EcritureTrace(XZSTC_WARNING,"drol_tun_renove: pb decodage %s",vl_str);
      continue;
    }

    if (!strcmp(vl_nom,tm_liste_tunnels[vl_indice].Libelle)) {
      XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tun_renove : tube %s renove !",va_nom);
      strcpy(va_trig,vl_trigramme);
      fclose(pl_fd);
      return (TRUE);
    }
  }
  fclose(pl_fd);
  return (FALSE);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* cumul le nb de voies totales et en circulation
* a pertie de l'etat de la voie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void drol_tun_cumul_voies(XDY_Voie x,int *pa_tot,int *pa_cir)

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
        case    XDC_VOIE_BAU_NORMALE    :
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




/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
verifie si une sequence est autorise pour un tunnel
1/ si renove
aller dans fichiers/renov/'TRIGRAMME TUNNEL'
regarder si la sequence existe dans 
ConfigSceVentilation_<TRIG>.cfg
2/ si non renove
dans fichiers/deq/seq_aut_sig.txt, rechercher si la sequence
existe pour le nom 'base' de ce tunnel

------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_tun_autorise(int va_tunnel, int va_sequence, int va_domaine)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
------------------------------------------------------*/
{
  XDY_Octet vl_autorise;

  XZST_03EcritureTrace (XZSTC_INTERFACE,"drol_tun_autorise : tun %d - seq %d - dom %d",
						va_tunnel,va_sequence,va_domaine);

  if (XZAO801_Verif_Sequence_Autorisee(va_tunnel, va_sequence, va_domaine, &vl_autorise) != XDC_OK) {
    XZST_03EcritureTrace( XZSTC_WARNING, "drol_tun_autorise : pb appel a XZAO801_Verif_Sequence_Autorisee");
    return (0);
  }

  if (vl_autorise==0) {
    XZST_03EcritureTrace (XZSTC_FONCTION, "drol_tun_autorise : autorise");
    return (va_sequence);
  }

  XZST_03EcritureTrace (XZSTC_FONCTION, "drol_tun_autorise : NON autorise");
  return (0);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des perturbations engendrees par l'evt
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen drol_tun_filtre_sono(T_NOEUD *va_noeud, XDY_District va_site, XDY_Horodate va_horodate)

/*
* ARGUMENTS EN ENTREE :
* T_NOEUD information de la FMC
* XDY_Disctrict Site
* XDY_Horodate Horodate
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
------------------------------------------------------*/
{
	int vl_PR_Debut_tunnel_oppose_out,vl_PR_Fin_tunnel_oppose_out;
	int vl_NumEqt_PAU=0;
	XDY_NomAuto vl_Autoroute;

	XDY_Octet	va_DispoPAU_out;
	XDY_Entier	va_NumEvt_out=0;
	XDY_Octet	va_CleEvt_out=0;
	XDY_Octet	va_Priorite_out;
	XDY_Entier	va_DistanceEvt_out;
	XDY_Entier	va_Action_out;
	XDY_District	va_SiteAction_out;
	XDY_MessageSONO	va_Message_out;
	XDY_Autoroute	va_Autoroute_out;
	XDY_Sens	va_Sens_out;
	XDY_PR		va_PrDebut_out;
	XDY_PR		va_PrFin_out;
	int	vl_Sens;
	

	/* on inverse pour chercher l'eqt du tunnel oppose */
	vl_Sens =va_noeud->Localisation.Sens;
	if (vl_Sens == XDC_SENS_SUD)
	{
		vl_Sens = XDC_SENS_NORD;
	}
	else if (vl_Sens == XDC_SENS_NORD)
	{
		 vl_Sens = XDC_SENS_SUD;
	}

	XZST_03EcritureTrace( XZSTC_FONCTION,"IN : drol_tun_filtre_sono FMC:%d Autoroute:%d Sens:%d PR:%d Sens oppose:%d",
								va_noeud->NumEvt.numero,
								va_noeud->Localisation.Autoroute,
								va_noeud->Localisation.Sens,
								va_noeud->Localisation.PR,
								vl_Sens);
	// On test si le PR oppose est dans un tunnel
	XZAO802_Tunnel_Oppose(va_noeud->Localisation.Autoroute,
				va_noeud->Localisation.Sens,
				va_noeud->Localisation.PR,
				&vl_PR_Debut_tunnel_oppose_out,
				&vl_PR_Fin_tunnel_oppose_out);

XZST_03EcritureTrace( XZSTC_FONCTION,"IN : drol_tun_filtre_sono vl_PR_Debut_tunnel_oppose_out:%d vl_PR_Fin_tunnel_oppose_out:%d", vl_PR_Debut_tunnel_oppose_out ,vl_PR_Fin_tunnel_oppose_out);

	if (vl_PR_Debut_tunnel_oppose_out >0 && vl_PR_Fin_tunnel_oppose_out >0)
	{
	sprintf(vl_Autoroute, "%d",va_noeud->Localisation.Autoroute,vl_Autoroute);	
	XZST_03EcritureTrace( XZSTC_FONCTION,"Autoroute=%s",vl_Autoroute);

		/* On cherche le PAU le plus proche de la FMC*/
		XZAO907_Trouve_PAU(va_site,
			vl_Autoroute,
			vl_Sens,
			vl_PR_Debut_tunnel_oppose_out,
			vl_PR_Fin_tunnel_oppose_out,
			va_noeud->Localisation.Site,			
			0,
			&vl_NumEqt_PAU);
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_WARNING,"drol_tun_filtre_sono : Retour XZAO802, pas de tunnel dans sens oppose.");
		return XDC_OK;
	}
XZST_03EcritureTrace( XZSTC_WARNING,"NumEqt:%d, horodate:%lf",vl_NumEqt_PAU,va_horodate);
								
	if (vl_NumEqt_PAU!=0)
	{								

		XZAT067_Utilisation_PAU(va_horodate,
				va_site,
				&va_DispoPAU_out,
				&va_NumEvt_out,
				&va_CleEvt_out,
				&va_Priorite_out,
				&va_DistanceEvt_out,
				&va_Action_out,
				&va_SiteAction_out,
				&va_Message_out,
				&va_Autoroute_out,
				&va_Sens_out,								
				&va_PrDebut_out,
				&va_PrFin_out);

		if(va_NumEvt_out==0 && va_CleEvt_out ==0)
		{
			XZAP35_ajout_Prop_SONO(	vg_numero_plan,		
						vg_ordre_tun++,
						va_site,
						vl_NumEqt_PAU,
						0,
						"",
						"INCENDIE DANS TUNNEL : ATTENTE",
						TRUE,
						XZEUC_SONO_ATTENTE,
						va_noeud->Localisation.Autoroute,
						vl_Sens,
						vl_PR_Debut_tunnel_oppose_out,
						vl_PR_Fin_tunnel_oppose_out,
						"Sonorisation PAUs");
		}
		else
		{
			XZAP35_ajout_Prop_SONO(	vg_numero_plan,		
						vg_ordre_tun++,
						va_site,
						vl_NumEqt_PAU,
						0,
						"",
						"INCENDIE DANS TUNNEL : EVACUATION",
						TRUE,
						XZEUC_SONO_EVACUATION,
						va_noeud->Localisation.Autoroute,
						vl_Sens,
						vl_PR_Debut_tunnel_oppose_out,
						vl_PR_Fin_tunnel_oppose_out,
						"Sonorisation PAUs");			
		}
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_WARNING,"drol_tun_filtre_sono : Retourn XZAO907, pas de NumEqt retourne. Site:%d\nAutoroute:%d\nSens:%d\nPR debut:%d\nPR fin:%d\nSite origine:%d",
								va_site,
								va_noeud->Localisation.Autoroute,
								vl_Sens,
								vl_PR_Debut_tunnel_oppose_out,
								vl_PR_Fin_tunnel_oppose_out,
								va_noeud->Localisation.Site);
	}
	
	XZST_03EcritureTrace( XZSTC_FONCTION,"OUT : drol_tun_filtre_sono");
	return XDC_OK;

}
