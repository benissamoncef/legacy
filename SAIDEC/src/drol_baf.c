/*E*/
/* Fichier : $Id: drol_baf.c,v 1.15 2021/06/10 14:13:04 gesconf Exp $        Release : $Revision: 1.15 $        Date : $Date: 2021/06/10 14:13:04 $
**************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : drol_baf.c				*
*************************************************
* DESCRIPTION :
* new tdpac
*************************************************
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.0
* PNI	17/01/07	Pas de pilotage auto si BAF deja utilisee 1.2
* PNI	21/06/07 : Ajout des nouveaux type de fmc DEM658
* LCL   09/07/07 : Suppression DY
* PNI	15/09/08 : DEM810 recherche les BAFA par bretelle en cas de Fermeture Echangeur v1.5
* JPL	06/09/10 : Prise en compte du type Fmc Zone de stockage (DEM 942) 1.6
* JPL	18/02/11 : Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.7
* JPL	21/02/11 : Migration architecture HP ia64 (DEM 971) : adequation types parametres et profil fonctions  1.8
* JPL	14/10/11 : Pas d'action automatique SAPIENS pour Fmc simulees (Essai ou Exercice) (DEM 995)  1.9
* JPL	12/07/12 : Utilisation de l'indicateur de bifurcation pour localisation sur bretelle (DEM 1033)  1.10
* PNI	23/08/13	 : SAPIENS au CI  DEM 1074/FR 105 1.11
* JMG	15/05/17 : contresens recherche de BAF dans leS 2 sens 1.12
* JPL	23/09/19 : Type d'evenement "INCENDIE" attendu en configuration et non "INCENDIE DE VEGETATION" (DEM 1352)  1.14
* PNI	10/06/21 : Ne pas proposer les Baf pour les fermeture ech prevues SAE-289 1.15
* PNI	10/06/21 : Suppression de la precedente modif, correctif dans baf.txt SAE-289 1.16
* LCL	04/10/23 : Adaptations CNA DEM-483 : lecture site principal en base
* GGY	18/01/24 : Ajout C_COND_CIRCULATION_BASC_TUNNEL et C_COND_CIRCULATION_NON_BASC_TUNNEL (DEM-483)
************************************************/

#include "drol_baf.h"

#include "xzac92.h"
#include "xzac192.h"
#include "xzao441.h"
#include "xzao841.h"
#include "xzap29.h"
#include "xzap70.h"
#include "xzat35.h"
#include "xzat150.h"

#define C_REP_MIG	"/produits/migrazur/appliSD/fichiers/bdc/"
#define C_PERT_BAF		C_REP_MIG "baf.txt"

#define C_SEPARATEUR	"#"
#define C_PIPE		"|"
#define C_ET		"$ET$"
#define C_CAR_DOLLAR	'$'
#define C_INFINI	1000000

#define C_INFERIEUR	"<"
#define C_SUPERIEUR	">"
#define C_EGAL		"="

#define C_COND_UNE_VOIE_BLOQ	"au moins une voie bloquee"
#define C_COND_UNE_VOIE_OUVERTE	"au moins une voie ouverte"
#define C_COND_TOUTES_VOIES_BLOQ	"toutes voies bloquees"
#define C_COND_UNE_VOIE_NEUT	"au moins une voie neutralisee"
#define C_COND_TOUTES_VOIES_NEUT	"toutes voies neutralisees"
#define C_COND_TOUTES_VOIES_FERMEES	"toutes voies fermees"
#define C_COND_CIRCULATION_UNE_VOIE	"circulation sur une voie"
#define C_COND_SUR_BAU		"sur bau"
#define C_COND_SUR_SECTION_COURANTE	"sur section courante"
#define C_COND_SUR_LIT_ARRET	"sur lit d'arret"
#define C_COND_SUR_ECHANGEUR	"sur echangeur"
#define C_COND_SUR_BRETELLE_ENTREE	"sur bretelle d'entree"
#define C_COND_SUR_BRETELLE_SORTIE	"sur bretelle de sortie"
#define C_COND_SIGNALE	"signale"
#define C_COND_CONFIRME	"confirme"
#define C_COND_TRAFIC_DENSE	"trafic dense"
#define C_COND_TRAFIC_RALENTI	"trafic ralenti"
#define C_COND_TRAFIC_BOUCHE	"trafic bouche"
#define C_COND_TRAFIC_BLOQUE	"trafic bloque"
#define C_COND_VOIES_RETRECIES	"voies retrecies"
#define C_COND_ENTREE_FERMEE	"entree fermee"
#define C_COND_SORTIE_FERMEE	"sortie fermee"
#define C_COND_DATEX	"datex"
#define C_COND_BOUCHON_MOINS_3_KM	"longueur<3"
#define C_COND_BOUCHON_ENTRE_3_ET_5_KM	"longueur<5"
#define C_COND_BOUCHON_PLUS_5_KM	"longueur>5"
#define C_COND_PREVU		"prevu"
#define C_COND_NON_PREVU	"non prevu"
#define C_COND_TOUS	"TOUS"
#define C_COND_TRAVAUX_ENTRETIEN	"entretien"
#define C_COND_STOCKAGE		"stockage"
#define C_COND_AIRE_SERVICE	"service"
#define C_COND_AIRE_REPOS	"repos"
#define C_COND_MAJEUR	"majeur"
#define C_COND_MATIERES	"matieres_dangereuses"
#define C_COND_PEAGE_AMONT "en aval peage amont"
#define C_COND_CIRCULATION_BASC_TUNNEL		"circulation_basculee"
#define C_COND_CIRCULATION_NON_BASC_TUNNEL	"circulation_non_basculee"

#define C_SCE_NEUTRALISATION	"N_BARVR1" 
#define C_SCE_LIBERATION	"L_BARVR1"		

#define C_EVT_SIGNALE_RAPIDE		0
#define C_EVT_CONFIRME_VL_RAPIDE	1
#define C_EVT_CONFIRME_VR_RAPIDE	2
#define C_EVT_CONFIRME_RAPIDE		3
#define C_EVT_SIGNALE_LENT		4
#define C_EVT_CONFIRME_VL_LENT		5
#define C_EVT_CONFIRME_VR_LENT		6
#define C_EVT_CONFIRME_LENT		7

T_PERT_BAF	*vg_pert_baf;
T_NOEUD		*vm_noeud;
static T_NOEUD	*vm_noeud2;
T_NOEUD 	*vg_racine;
T_BAF		vm_baf;

extern XDY_Booleen vg_presence_site[];

static int drol_baf_recherche(XDY_Autoroute ,
		        XDY_PR ,
		        XDY_PR ,
			XDY_Sens ,
			XDY_Horodate ,
			XDY_District,
			int,
			char *,
			int,
			int);
int drol_baf_charge();
T_CONDITION *drol_baf_decode_condition(char *);
extern XDY_Mot			    vg_SitePrincipal;

static XDY_Booleen drol_baf_egal(int, int);
static XDY_Booleen drol_baf_different(int, int);
/*
static XDY_Booleen drol_baf_true(int, int);
static XDY_Booleen drol_baf_inferieur();
static XDY_Booleen drol_baf_superieur();
*/

/*fonctions utilisees dans les conditions*/
static XDY_Booleen drol_baf_fmc_simulee();
static XDY_Booleen drol_baf_une_voie_bloquee();
static XDY_Booleen drol_baf_une_voie_ouverte();
static XDY_Booleen drol_baf_toutes_voies_bloquees();
static XDY_Booleen drol_baf_une_voie_neutralisee();
static XDY_Booleen drol_baf_toutes_voies_neutralisees();
static XDY_Booleen drol_baf_toutes_voies_bloquees_neutralisees();
static XDY_Booleen drol_baf_toutes_voies_fermees();
static XDY_Booleen drol_baf_circulation_une_voie();
static XDY_Booleen drol_baf_sur_bau();
static XDY_Booleen drol_baf_sur_section_courante();
static XDY_Booleen drol_baf_sur_lit_arret();
static XDY_Booleen drol_baf_sur_echangeur();
static XDY_Booleen drol_baf_sur_bretelle_entree();
static XDY_Booleen drol_baf_sur_bretelle_sortie();
static XDY_Booleen drol_baf_signale();
static XDY_Booleen drol_baf_confirme();
static XDY_Booleen drol_baf_trafic_dense();
static XDY_Booleen drol_baf_trafic_ralenti();
static XDY_Booleen drol_baf_trafic_bouche();
static XDY_Booleen drol_baf_trafic_bloque();
static XDY_Booleen drol_baf_voies_retrecies();
static XDY_Booleen drol_baf_entree_fermee();
static XDY_Booleen drol_baf_sortie_fermee();
static XDY_Booleen drol_baf_datex();
static XDY_Booleen drol_baf_prevu();
static XDY_Booleen drol_baf_non_prevu();
static XDY_Booleen drol_baf_travaux_entretien();
static XDY_Booleen drol_baf_stockage();
static XDY_Booleen drol_baf_matieres();
static XDY_Booleen drol_baf_aire_service();
static XDY_Booleen drol_baf_aire_repos();
static XDY_Booleen drol_baf_majeur();
static XDY_Booleen drol_baf_circulation_basculee_tunnel ();
static XDY_Booleen drol_baf_circulation_non_basculee_tunnel ();

static int drol_baf_voies(int, int, int, int, int);

static int ajouter_baf(XDY_Eqt ,
                        XDY_District ,
                        XDY_Autoroute ,
                        XDY_PR ,
                        int ,
                        XDY_Sens ,
                        XDY_District ,
                        int ,
                        XDY_Horodate ,
                        char *,
			int,
                        int *);
static int ajouter_baf_saga(XDY_Eqt ,
                        XDY_District ,
                        XDY_Autoroute ,
                        XDY_PR ,
                        int ,
                        XDY_Sens ,
                        XDY_District ,
                        int ,
                        XDY_Horodate ,
                        char *,
			int,
                        int *);

/* definition des variables locales au module */
static char *version = "$Id: drol_baf.c,v 1.15 2021/06/10 14:13:04 gesconf Exp $ : drol_baf" ;


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_baf_init()

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
  
  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : drol_baf_init");
  vm_noeud2=(T_NOEUD *)malloc(sizeof(T_NOEUD));
  vg_pert_baf=NULL;
  drol_baf_charge();

  XZST_03EcritureTrace(   XZSTC_FONCTION,"OUT drol_baf_init");

  return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des perturbations
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_baf_charge()

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
  FILE  *pl_fp;
  int          vl_retcode = 0;
  int		vl_i;
  char        vl_Lig[500];
  XDY_Entier vl_Resultat_out = 0;
  char	*vl_str;
  char vl_evt[50];
  char vl_mode[50];
  char 	vl_condition[500];
  T_PERT_BAF *pl_liste;
  int vl_auto;

  XZST_03EcritureTrace(XZSTC_DEBUG1, "IN  : drol_baf_charge");

  if ( (pl_fp = fopen(C_PERT_BAF, "r")) == NULL ) {
    XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !" ,C_PERT_BAF );
    return(XDC_NOK);
  }
  
  /*premiere ligne = commentaires*/
  fgets(vl_Lig,sizeof(vl_Lig),pl_fp) ;
  
    /*memorisation de la structure du message*/
  while (fgets(vl_Lig,sizeof(vl_Lig),pl_fp) != NULL)
  {
      pl_liste = (T_PERT_BAF *)malloc(sizeof(T_PERT_BAF));
      vl_str=vl_Lig; 

      sscanf(vl_str,"%[^|]|%[^|]|%d|%d",pl_liste->Libelle,vl_condition,&(pl_liste->Automatique), &(pl_liste->Filtre_peage));
      pl_liste->Condition = drol_baf_decode_condition(vl_condition);
      
      pl_liste->Suivant = vg_pert_baf;
      vg_pert_baf = pl_liste;
  }
  fclose(pl_fp);
  XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT  : drol_baf_charge");
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* decodage de la condition
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

T_CONDITION *drol_baf_decode_condition(char *va_cond)

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
  char	vl_str[100];
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
  XZST_03EcritureTrace(   XZSTC_WARNING, "drol_baf_decode_condition:  IN %s",va_cond);

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
  pl_cond->Fonction.Comp.Fct=drol_baf_egal;
  pl_cond->Suivant=NULL;
  pl_tmp=pl_cond;

  /*numero en fonction du type*/
  XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_baf_decode_condition: type = %s",pl_cond->Libelle);

  strcpy(vl_ch,pl_cond->Libelle);
  if (!strcmp(vl_ch,"ACCIDENT")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Accident;
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
  else if (!strcmp(vl_ch,"Echangeur deconseille")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_EchDeconseille;
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
    pl_tmp->Fonction.Comp.Fct=drol_baf_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Verglas;
  }
  else if (!strcmp(vl_ch,"CHUTE NEIGE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_baf_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_ChuteNeige;
  }
  else if (!strcmp(vl_ch,"PLUIE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_baf_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Pluie;
  }
  else if (!strcmp(vl_ch,"BROUILLARD GIVRANT")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_baf_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_BrouillardGiv;
  }
  else if (!strcmp(vl_ch,"NEIGE SUR CHAUSSEE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_baf_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_NeigeChaus;
  }
  else if (!strcmp(vl_ch,"BROUILLARD")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_baf_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Brouillard;
  }
  else if (!strcmp(vl_ch,"VENT")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_baf_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_VentFort;
  }
  else if (!strcmp(vl_ch,"GRELE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_baf_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Grele;
  }
  /*traitement du cas particulier de l'incident sous concessionnaire*/
  else if ( (!strcmp(vl_ch,"FERMETURE STATION")) ||
	    (!strcmp(vl_ch,"FERMETURE STATION\n"))
          ) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_IncSsConces;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_baf_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Incident);
    pl_tmp->Fonction.Comp.Valeur=1;
  }
  else if (!strcmp(vl_ch,"PENURIE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_IncSsConces;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_baf_egal;
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
    pl_tmp->Fonction.Comp.Fct=drol_baf_different;
  }
  else {
    XZST_03EcritureTrace(   XZSTC_DEBUG1,"drol_pmv_decode_condition : pb decodage type !!!");
    pl_cond->Fonction.Comp.Valeur=0;
  }
 
  /*si pas de condition supplementaire*/
  if (vl_indice==strlen(va_cond)) {
    XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : drol_pmv_decode_condition : que le type");
    return (pl_cond);
  }

  /*supprimer $ET$*/
  va_cond=va_cond+strlen(pl_cond->Libelle)+4;

  strcpy(vl_ch_init,va_cond);
  XZST_03EcritureTrace(   XZSTC_DEBUG1,"drol_pmv_decode_condition : apres le type = %s",vl_ch_init);

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

    XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_pmv_decode_condition:  cond %s",vl_str);
    strcpy(pl_tmp->Libelle,vl_str);

    /*il s'agit d'une fonction PREDICAT*/
    pl_tmp->Type=C_TYPE_PREDICAT;
    if (!strcmp(vl_str,C_COND_UNE_VOIE_BLOQ)) pl_tmp->Fonction.Predicat.Fct=drol_baf_une_voie_bloquee;
    else if (!strcmp(vl_str,C_COND_UNE_VOIE_OUVERTE)) pl_tmp->Fonction.Predicat.Fct=drol_baf_une_voie_ouverte;
    else if (!strcmp(vl_str,C_COND_TOUTES_VOIES_BLOQ)) pl_tmp->Fonction.Predicat.Fct=drol_baf_toutes_voies_bloquees;
    else if (!strcmp(vl_str,C_COND_TOUTES_VOIES_NEUT)) pl_tmp->Fonction.Predicat.Fct=drol_baf_toutes_voies_neutralisees;
    else if (!strcmp(vl_str,C_COND_UNE_VOIE_NEUT)) pl_tmp->Fonction.Predicat.Fct=drol_baf_une_voie_neutralisee;
    else if (!strcmp(vl_str,C_COND_CIRCULATION_UNE_VOIE)) pl_tmp->Fonction.Predicat.Fct=drol_baf_circulation_une_voie;
    else if (!strcmp(vl_str,C_COND_CONFIRME)) pl_tmp->Fonction.Predicat.Fct=drol_baf_confirme;
    else if (!strcmp(vl_str,C_COND_SIGNALE)) pl_tmp->Fonction.Predicat.Fct=drol_baf_signale;
    else if (!strcmp(vl_str,C_COND_SUR_BAU)) pl_tmp->Fonction.Predicat.Fct=drol_baf_sur_bau;
    else if (!strcmp(vl_str,C_COND_SUR_SECTION_COURANTE)) pl_tmp->Fonction.Predicat.Fct=drol_baf_sur_section_courante;
    else if (!strcmp(vl_str,C_COND_SUR_LIT_ARRET)) pl_tmp->Fonction.Predicat.Fct=drol_baf_sur_lit_arret;
    else if (!strcmp(vl_str,C_COND_SUR_ECHANGEUR)) pl_tmp->Fonction.Predicat.Fct=drol_baf_sur_echangeur;
    else if (!strcmp(vl_str,C_COND_SUR_BRETELLE_ENTREE)) pl_tmp->Fonction.Predicat.Fct=drol_baf_sur_bretelle_entree;
    else if (!strcmp(vl_str,C_COND_SUR_BRETELLE_SORTIE)) pl_tmp->Fonction.Predicat.Fct=drol_baf_sur_bretelle_sortie;
    else if (!strcmp(vl_str,C_COND_TRAFIC_DENSE)) pl_tmp->Fonction.Predicat.Fct=drol_baf_trafic_dense;
    else if (!strcmp(vl_str,C_COND_TRAFIC_RALENTI)) pl_tmp->Fonction.Predicat.Fct=drol_baf_trafic_ralenti;
    else if (!strcmp(vl_str,C_COND_TRAFIC_BOUCHE)) pl_tmp->Fonction.Predicat.Fct=drol_baf_trafic_bouche;
    else if (!strcmp(vl_str,C_COND_TRAFIC_BLOQUE)) pl_tmp->Fonction.Predicat.Fct=drol_baf_trafic_bloque;
    else if (!strcmp(vl_str,C_COND_DATEX)) pl_tmp->Fonction.Predicat.Fct=drol_baf_datex;
    else if (!strcmp(vl_str,C_COND_VOIES_RETRECIES)) pl_tmp->Fonction.Predicat.Fct=drol_baf_voies_retrecies;
    else if (!strcmp(vl_str,C_COND_ENTREE_FERMEE)) pl_tmp->Fonction.Predicat.Fct=drol_baf_entree_fermee;
    else if (!strcmp(vl_str,C_COND_SORTIE_FERMEE)) pl_tmp->Fonction.Predicat.Fct=drol_baf_sortie_fermee;
    else if (!strcmp(vl_str,C_COND_PREVU)) pl_tmp->Fonction.Predicat.Fct=drol_baf_prevu;    
    else if (!strcmp(vl_str,C_COND_NON_PREVU)) pl_tmp->Fonction.Predicat.Fct=drol_baf_non_prevu;    
    else if (!strcmp(vl_str,C_COND_TOUTES_VOIES_FERMEES)) pl_tmp->Fonction.Predicat.Fct=drol_baf_toutes_voies_fermees;    
    else if (!strcmp(vl_str,C_COND_TRAVAUX_ENTRETIEN)) pl_tmp->Fonction.Predicat.Fct=drol_baf_travaux_entretien;    
    else if (!strcmp(vl_str,C_COND_STOCKAGE)) pl_tmp->Fonction.Predicat.Fct=drol_baf_stockage;    
    else if (!strcmp(vl_str,C_COND_AIRE_SERVICE)) pl_tmp->Fonction.Predicat.Fct=drol_baf_aire_service;    
    else if (!strcmp(vl_str,C_COND_AIRE_REPOS)) pl_tmp->Fonction.Predicat.Fct=drol_baf_aire_repos;    
    else if (!strcmp(vl_str,C_COND_MATIERES)) pl_tmp->Fonction.Predicat.Fct=drol_baf_matieres;    
    else if (!strcmp(vl_str,C_COND_MAJEUR)) pl_tmp->Fonction.Predicat.Fct=drol_baf_majeur;    
	else if (!strcmp(vl_str,C_COND_CIRCULATION_BASC_TUNNEL)) pl_tmp->Fonction.Predicat.Fct = drol_baf_circulation_basculee_tunnel;
	else if (!strcmp(vl_str,C_COND_CIRCULATION_NON_BASC_TUNNEL)) pl_tmp->Fonction.Predicat.Fct = drol_baf_circulation_non_basculee_tunnel;

 
  /*si fin de chaine*/
  if (vl_indice==strlen(va_cond)) {
    XZST_03EcritureTrace(   XZSTC_DEBUG1,"drol_pmv_decode_condition : fin de chaine");
    return (pl_cond);
  }
  else {
    va_cond=va_cond+strlen(vl_str)+4;
    goto pos_decode;
  }
}


/**************************************/
/**************************************/
static XDY_Booleen drol_baf_fmc_simulee() {
  return (strstr (vm_noeud->Infos_TFM.Origine, "ESSAI") != NULL  ||
          strstr (vm_noeud->Infos_TFM.Origine, "EXERCICE") != NULL);
}

static XDY_Booleen drol_baf_sur_echangeur() {
  return (vm_noeud->Localisation.TypePointCar==XDC_POINT_CARACT_ECHANGEUR);
}
static XDY_Booleen drol_baf_sur_lit_arret() {
  return (vm_noeud->Localisation.TypePointCar==XDC_POINT_CARACT_LIT_ARRET);
}

static XDY_Booleen drol_baf_matieres() {
  return (vm_noeud->Infos_FAX.Matieres_Dangereuses);
  }

static XDY_Booleen drol_baf_egal(int va_valeur1, int va_valeur2) { return (va_valeur1==va_valeur2); }
static XDY_Booleen drol_baf_different(int va_valeur1, int va_valeur2) { return (va_valeur1!=va_valeur2); }
/*
static XDY_Booleen drol_baf_true(int va_valeur1, int va_valeur2) { return (TRUE); }
static XDY_Booleen drol_baf_superieur(int va_valeur1, int va_valeur2) { return (va_valeur1>va_valeur2); }
static XDY_Booleen drol_baf_inferieur(int va_valeur1, int va_valeur2) { return (va_valeur1<va_valeur2); }
*/

static XDY_Booleen drol_baf_confirme() { return (vm_noeud->Infos_TFM.Confirme); }
static XDY_Booleen drol_baf_signale() { return (vm_noeud->Infos_TFM.Signale); }

static XDY_Booleen drol_baf_une_voie_bloquee() {
  return ((vm_noeud->Localisation.VL==XDC_VOIE_BLOQUEE) ||
          (vm_noeud->Localisation.VM1==XDC_VOIE_BLOQUEE) ||
	  (vm_noeud->Localisation.VM2==XDC_VOIE_BLOQUEE) ||
	  (vm_noeud->Localisation.VR==XDC_VOIE_BLOQUEE)
	 );
}
static XDY_Booleen drol_baf_une_voie_ouverte() {
  return ((vm_noeud->Localisation.VL==XDC_VOIE_SENS_NORMAL) ||
	  (vm_noeud->Localisation.VM1==XDC_VOIE_SENS_NORMAL) ||
	  (vm_noeud->Localisation.VM2==XDC_VOIE_SENS_NORMAL) ||
	  (vm_noeud->Localisation.VR==XDC_VOIE_SENS_NORMAL)
	 );
}
static XDY_Booleen drol_baf_toutes_voies_bloquees() {
  return ( (vm_noeud->Localisation.VL!=XDC_VOIE_INCONNUE) &&
	   (vm_noeud->Localisation.VL!=XDC_VOIE_INEXISTANTE) &&
	   (vm_noeud->Localisation.VL!=XDC_VOIE_SENS_NORMAL) &&
	   (vm_noeud->Localisation.VM1!=XDC_VOIE_SENS_NORMAL) &&
	   (vm_noeud->Localisation.VM2!=XDC_VOIE_SENS_NORMAL) &&
	   (vm_noeud->Localisation.VR!=XDC_VOIE_SENS_NORMAL) &&
	   (vm_noeud->Localisation.BAU!=XDC_VOIE_SENS_NORMAL) &&
	   (vm_noeud->Localisation.VR_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VM2_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VM1_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VL_I!=XDC_VOIE_SENS_INVERSE)
	 );
}

static XDY_Booleen drol_baf_une_voie_neutralisee() { 
  return ((vm_noeud->Localisation.VL==XDC_VOIE_NEUTRALISEE) ||
	  (vm_noeud->Localisation.VM1==XDC_VOIE_NEUTRALISEE) ||
	  (vm_noeud->Localisation.VM2==XDC_VOIE_NEUTRALISEE) ||
	  (vm_noeud->Localisation.VR==XDC_VOIE_NEUTRALISEE)
         );
}
       
static XDY_Booleen drol_baf_toutes_voies_neutralisees() { 
  return ( (vm_noeud->Localisation.BAU!=XDC_VOIE_SENS_NORMAL) &&
	   (vm_noeud->Localisation.VR_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VM2_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VM1_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VL_I!=XDC_VOIE_SENS_INVERSE) &&
	   (drol_baf_voies(vm_noeud->Localisation.VL,
			  vm_noeud->Localisation.VM1,
			  vm_noeud->Localisation.VM2,
			  vm_noeud->Localisation.VR,
			  XDC_VOIE_NEUTRALISEE)==15)
         );
}

static XDY_Booleen drol_baf_toutes_voies_bloquees_neutralisees() { 
  return ( (vm_noeud->Localisation.BAU!=XDC_VOIE_SENS_NORMAL) &&
	   (vm_noeud->Localisation.VR_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VM2_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VM1_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VL_I!=XDC_VOIE_SENS_INVERSE) &&
	   (drol_baf_voies(vm_noeud->Localisation.VL,
			  vm_noeud->Localisation.VM1,
			  vm_noeud->Localisation.VM2,
			  vm_noeud->Localisation.VR,
			  XDC_VOIE_BLOQUEE_NEUTRALISEE)==15)
         );
}


static XDY_Booleen drol_baf_toutes_voies_fermees() {
	return ( drol_baf_toutes_voies_neutralisees() || drol_baf_toutes_voies_bloquees_neutralisees());
}

static XDY_Booleen drol_baf_circulation_une_voie() {
  return ( ( (vm_noeud->Localisation.VL!=XDC_VOIE_SENS_NORMAL) &&
	     (vm_noeud->Localisation.VM1!=XDC_VOIE_SENS_NORMAL) &&
	     (vm_noeud->Localisation.VM2!=XDC_VOIE_SENS_NORMAL) &&
	     (vm_noeud->Localisation.VR==XDC_VOIE_SENS_NORMAL)
           )
	   ||
	   ( (vm_noeud->Localisation.VL!=XDC_VOIE_SENS_NORMAL) &&
	     (vm_noeud->Localisation.VM1!=XDC_VOIE_SENS_NORMAL) &&
	     (vm_noeud->Localisation.VM2==XDC_VOIE_SENS_NORMAL) &&
	     (vm_noeud->Localisation.VR!=XDC_VOIE_SENS_NORMAL)
	   )
	   ||
	   ( (vm_noeud->Localisation.VL!=XDC_VOIE_SENS_NORMAL) &&
	     (vm_noeud->Localisation.VM1==XDC_VOIE_SENS_NORMAL) &&
	     (vm_noeud->Localisation.VM2!=XDC_VOIE_SENS_NORMAL) &&
	     (vm_noeud->Localisation.VR!=XDC_VOIE_SENS_NORMAL)
           )
           ||
	   ( (vm_noeud->Localisation.VL==XDC_VOIE_SENS_NORMAL) &&
	     (vm_noeud->Localisation.VM1!=XDC_VOIE_SENS_NORMAL) &&
	     (vm_noeud->Localisation.VM2!=XDC_VOIE_SENS_NORMAL) &&
	     (vm_noeud->Localisation.VR!=XDC_VOIE_SENS_NORMAL)
           )
         );
}


static XDY_Booleen drol_baf_sur_bau() { 
  return ( (drol_baf_voies(vm_noeud->Localisation.VL,vm_noeud->Localisation.VM1,
			   vm_noeud->Localisation.VM2,
			   vm_noeud->Localisation.VR,
			   XDC_VOIE_SENS_NORMAL)==15) &&
           ((vm_noeud->Localisation.BAU==XDC_VOIE_BLOQUEE) ||
	    (vm_noeud->Localisation.BAU==XDC_VOIE_NEUTRALISEE)
           )
         );
}

static XDY_Booleen drol_baf_sur_section_courante() { 
  return ( (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_AIRE) &&
	   (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_ECHANGEUR) &&
	   (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_PEAGE) &&
	   (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_LIT_ARRET)
         );
}
static XDY_Booleen drol_baf_sur_bretelle_entree() { 
  return (  (  (vm_noeud->Localisation.Sens==XDC_SENS_ENTREE_SUD) ||
	       (vm_noeud->Localisation.Sens==XDC_SENS_ENTREE_NORD)
	    )
	    &&
	    (vm_noeud->Localisation.Bifurcation != XDC_VRAI)
         );
}

static XDY_Booleen drol_baf_sur_bretelle_sortie() { 
  return (  (      (vm_noeud->Localisation.Sens==XDC_SENS_SORTIE_SUD) ||
	           (vm_noeud->Localisation.Sens==XDC_SENS_SORTIE_NORD)
	    )
	    &&
            (vm_noeud->Localisation.Bifurcation != XDC_VRAI)
         );
}

static XDY_Booleen drol_baf_trafic_dense() { 
  return (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Dense); }

static XDY_Booleen drol_baf_trafic_ralenti() { 
  return (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Ralenti); }

static XDY_Booleen drol_baf_trafic_bouche() { 
  return (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Bouche); }

static XDY_Booleen drol_baf_trafic_bloque() { 
  return (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Bloque); }

static XDY_Booleen drol_baf_datex() { return (FALSE); }

static XDY_Booleen drol_baf_voies_retrecies() { return (vm_noeud->Localisation.Voies_Retrecies); }

static XDY_Booleen drol_baf_entree_fermee() { 
  return ( (vm_noeud->Infos_FAX.Entree1) || (vm_noeud->Infos_FAX.Entree2) );
}

static XDY_Booleen drol_baf_sortie_fermee() { 
  return ( (vm_noeud->Infos_FAX.Sortie1) || (vm_noeud->Infos_FAX.Sortie2) );
}
static XDY_Booleen drol_baf_non_prevu() { 
  return (!(vm_noeud->Infos_TFM.Prevu) );
}
static XDY_Booleen drol_baf_travaux_entretien() {
  return (vm_noeud->Infos_TFM.Nature_Travaux==0);
}
static XDY_Booleen drol_baf_stockage() {
  return (vm_noeud->Infos_TFM.Type_Travaux==24);
}
static XDY_Booleen drol_baf_aire_service() {
  return (vm_noeud->Infos_TFM.Aire_Service);
}
static XDY_Booleen drol_baf_aire_repos() {
  return !(vm_noeud->Infos_TFM.Aire_Service);
}
static XDY_Booleen drol_baf_prevu() { 
  return (!drol_baf_non_prevu());
}

static XDY_Booleen drol_baf_majeur() {
  if (vm_noeud->Infos_TFM.Majeur==XDC_VRAI)
      return (XDC_VRAI);
	return (XDC_FAUX);
}

static XDY_Booleen drol_baf_circulation_basculee_tunnel ()
{
	XDY_Booleen vl_basculement = XDC_FAUX;
	
	XZST_03EcritureTrace(XZSTC_WARNING,"drol_baf_circulation_basculee_tunnel : avant XZAE861_Basculement_Tunnel");

	if (XZAE861_Basculement_Tunnel (vm_noeud->Localisation.Autoroute,
									vm_noeud->Localisation.PR,
									vm_noeud->Localisation_Tete.PR,
									vl_basculement)	!= XDC_OK) 
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_baf_circulation_basculee_tunnel : pb appel a XZAE861_Basculement_Tunnel");
		return (FALSE);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_baf_circulation_basculee_tunnel : XZAE861_Basculement_Tunnel retourne %s", vl_basculement);
	}
	
	return (vl_basculement);
}

static XDY_Booleen drol_baf_circulation_non_basculee_tunnel ()
{
	XZST_03EcritureTrace (XZSTC_WARNING, "drol_baf_circulation_non_basculee_tunnel : not drol_baf_circulation_basculee_tunnel");
	return (!drol_baf_circulation_basculee_tunnel());
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des perturbations engendrees par l'evt
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen drol_baf_filtre(T_NOEUD *va_noeud, XDY_District va_site, XDY_Horodate va_horodate)

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
   T_PERT_BAF *pl_cas;
  T_CONDITION *pl_cond;
  XDY_Booleen (*vl_fct)();
  XDY_Booleen vl_res=FALSE; 
  XDY_Entier vl_distance_max = 0;
  XDY_Sens vl_sens;
  
  XZST_03EcritureTrace(   XZSTC_FONCTION,"IN : drol_baf_filtre %d/%d",
			va_noeud->NumEvt.numero,
			va_noeud->NumEvt.cle);

  vm_noeud=va_noeud;
  vm_noeud2->Localisation=vm_noeud->Localisation;
  vm_noeud2->Infos_FAX=vm_noeud->Infos_FAX;
  vm_noeud2->Type=vm_noeud->Type;

   pl_cas= vg_pert_baf;
  
  /*on n'active les BAF que sur evenement majeur*/
  while (pl_cas!=NULL)
  {
    XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_baf_filtre %s",pl_cas->Libelle);
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
      XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_baf_filtre OK pour %s ",
					pl_cas->Libelle);
      /*recherche les BAF candidats*/
  /*distance max ??????????*/
      if (va_noeud->Type==XZAEC_FMC_FermetureEch) /*DEM810*/
	{
	if ( va_noeud->Infos_FAX.Entree1)
		drol_baf_recherche(va_noeud->Localisation.Autoroute, 
		va_noeud->Localisation.PR,
		va_noeud->Localisation_Tete.PR,
		XDC_SENS_ENTREE_SUD,
		va_horodate,
		va_site,
		vl_distance_max,
		(char *)pl_cas->Libelle,
		pl_cas->Automatique,
		pl_cas->Filtre_peage);
		
	if ( va_noeud->Infos_FAX.Entree2)
		drol_baf_recherche(va_noeud->Localisation.Autoroute, 
		va_noeud->Localisation.PR,
		va_noeud->Localisation_Tete.PR,
		XDC_SENS_ENTREE_NORD,
		va_horodate,
		va_site,
		vl_distance_max,
		(char *)pl_cas->Libelle,
		pl_cas->Automatique,
		pl_cas->Filtre_peage);
	      }
      else if (va_noeud->Type == XZAEC_FMC_Contresens) {
        drol_baf_recherche(va_noeud->Localisation.Autoroute, 
		va_noeud->Localisation.PR,
		va_noeud->Localisation_Tete.PR,
		XDC_SENS_SUD,
		va_horodate,
		va_site,
		vl_distance_max,
		(char *)pl_cas->Libelle,
		pl_cas->Automatique,
		pl_cas->Filtre_peage);
        drol_baf_recherche(va_noeud->Localisation.Autoroute, 
		va_noeud->Localisation.PR,
		va_noeud->Localisation_Tete.PR,
		XDC_SENS_NORD,
		va_horodate,
		va_site,
		vl_distance_max,
		(char *)pl_cas->Libelle,
		pl_cas->Automatique,
		pl_cas->Filtre_peage);
      }
      else 
        drol_baf_recherche(va_noeud->Localisation.Autoroute, 
		va_noeud->Localisation.PR,
		va_noeud->Localisation_Tete.PR,
		va_noeud->Localisation.Sens,
		va_horodate,
		va_site,
		vl_distance_max,
		(char *)pl_cas->Libelle,
		pl_cas->Automatique,
		pl_cas->Filtre_peage);

	XZST_03EcritureTrace(   XZSTC_INFO,"drol_baf_filtre retour TRUEa pres recherche baf");
      return (TRUE);
    }
    else
	XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_baf_filtre NOK");
    pl_cas=pl_cas->Suivant;
  }


  

  return (TRUE);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des BAF
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_baf_recherche(XDY_Autoroute va_autoroute,
		        XDY_PR va_PR,
		        XDY_PR va_PR_tete,
			XDY_Sens va_sens,
			XDY_Horodate va_horodate,
			XDY_District va_site,
			int va_distance,
			char *va_perturbation,
			int va_automatique,
			int va_filtre_peage)

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
  XDY_Sens	vl_sens;
  XDY_Mot         Eqt=0;
  XDY_District    DistrictEqt;
  XDY_Autoroute   AutorouteEqt;
  XDY_PR          PREqt;
  XDY_Sens        SensEqt;
  XDY_Entier      Distance;
  XDY_Entier      PRInfluence;
  XDY_Entier    vl_inversee=1;
  XDY_Octet       Dispo;
  T_STRING        Mode;
  XDY_Entier      NumEvt;
  XDY_Octet       CleEvt;
  XDY_Octet       Priorite;
  XDY_Entier      DistanceEvt;
  XDY_Entier             flag=FALSE;

  XZST_03EcritureTrace(   XZSTC_FONCTION,"IN : drol_baf_recherche aut=%d,PR=%d,sens=%d,site=%d, filtre_peage=%d",
						va_autoroute,va_PR,va_sens,va_site,va_filtre_peage);

  /*PATCH A57
  if (va_autoroute==8)
    vl_inversee=-1;*/

  vl_sens=va_sens;
  if (va_sens == XDC_SENS_SORTIE_NORD)
    vl_sens = XDC_SENS_NORD;
  if (va_sens == XDC_SENS_SORTIE_SUD)
    vl_sens = XDC_SENS_SUD;

    XZST_03EcritureTrace(XZSTC_FONCTION, "drol_nav_recherche : avant XZAO19");

  if ( ((XDY_Sens) va_sens) != XDC_SENS_INCONNU)
        {

                if ((va_sens==XDC_SENS_SORTIE_NORD) ||
                    (va_sens==XDC_SENS_ENTREE_NORD) ||
                    (va_sens==XDC_SENS_NORD))
                     vl_sens=XDC_SENS_NORD;
                else
                     vl_sens=XDC_SENS_SUD;
                /*recherche les BAF en amont*/
                XZAO441_Rechercher_tous_les_BAF_amont((XDY_Autoroute) va_autoroute,
                                                (XDY_Sens) va_sens,
                                                (XDY_PR) va_PR,
                                                (XDY_PR) va_PR_tete,
                                                (XDY_District) va_site,
                                                (XDY_Entier) 0,
                                                (XDY_Horodate) va_horodate,
                                                (char *) va_perturbation,
						va_automatique,
						va_filtre_peage,
                                                (XDY_FonctionInt)ajouter_baf,
                                                &flag
                                               );
                XZAO841_Rechercher_tous_les_BAF_SAGA_amont((XDY_Autoroute) va_autoroute,
                                                (XDY_Sens) va_sens,
                                                (XDY_PR) va_PR,
                                                (XDY_PR) va_PR_tete,
                                                (XDY_District) va_site,
                                                (XDY_Entier) 0,
                                                (XDY_Horodate) va_horodate,
                                                (char *) va_perturbation,
						va_automatique,
						va_filtre_peage,
                                                (XDY_FonctionInt)ajouter_baf_saga,
                                                &flag
                                               );

        }
   return (XDC_OK);
}

static int ajouter_baf(XDY_Eqt va_numero_baf,
                        XDY_District va_district_baf,
                        XDY_Autoroute va_autoroute_baf,
                        XDY_PR va_pr_baf,
                        int va_distance_baf,
                        XDY_Sens va_sens_baf,
                        XDY_District va_district,
                        int va_priorite,
                        XDY_Horodate va_horodate,
                        char *va_perturbation,
			int va_automatique,
                        int *pa_resultat)
{
  XDY_Octet Module11;
  XDY_Octet Module12;
  XDY_Octet Module15;
  XDY_Octet Module21;
  XDY_Octet Module22;
  XDY_Octet Module25;
  XDY_Octet Module11_P;
  XDY_Octet Module12_P;
  XDY_Octet Module15_P;
  XDY_Octet Module21_P;
  XDY_Octet Module22_P;
  XDY_Octet Module25_P;
  XDY_Octet TypeBAF;
  XDY_Octet       Dispo;
  T_STRING        Mode;
  XDY_Entier      NumEvt;
  XDY_Octet       CleEvt;
  XDY_Entier      DistanceEvt;
  XDY_Octet       Priorite;
  int             vl_mode_auto;
  char            vl_dg[50];

    /*on a trouve une BAF*/
    if ((va_district_baf!=va_district) && (va_district!=vg_SitePrincipal))
      return (XDC_NOK);

    /*appel XZAT06*/
    if (XZAT35_Utilisation_BAF(     va_horodate, va_numero_baf,
		    &Dispo, &Module11, &Module12, &Module15,
			&Module21, &Module22, &Module25, &NumEvt, &CleEvt, &Priorite, &DistanceEvt,
			&TypeBAF) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING,
	    "Etat de %d inconnu a la date %lf. On le considere indispo.",va_numero_baf, va_horodate);
      Dispo = XDC_EQT_MAJEUR;
    }														    
    if (Dispo > XDC_EQT_MINEUR)
    {
      XZST_03EcritureTrace(XZSTC_FONCTION,"baf indispo, on l'oublie");
      return (XDC_NOK);
    }

    /*au CI, si la nav est utilise par un evt plus proche, on l'oublie*/
    /* Uniquement pour ESCOTA, les autres sites principaux sont des PC */
    if ( (va_district==XDC_CI) &&
         (va_distance_baf>DistanceEvt) && (DistanceEvt!=0)) {
      XZST_03EcritureTrace(XZSTC_FONCTION,"baf est utilise par un evt plus proche, on l'oublie");
      return (XDC_NOK);
    }
  
    /*si on calcule un plan d'actions pour une simulation*/
    vl_mode_auto= va_automatique;


    XZST_03EcritureTrace(XZSTC_WARNING,"vg_site=%d;va_baf.District=%d",vg_site,va_district_baf);
    XZST_03EcritureTrace(XZSTC_WARNING,"vg_presence_site[va_baf.District]=%d",vg_presence_site[va_district_baf]);
    if (((vg_site==XDC_CI) && (vg_presence_site[va_district_baf]==XDC_VRAI)) || drol_baf_fmc_simulee()) {
  	  /*on ne fait que proposer, pas de pilotage automatique*/
      /* Ne s'applique pas sur COFIROUTE, uniquement sur ESCOTA */
      /* Sur COFIROUTE, le site principal peut piloter les BAD */
  	  vl_mode_auto=0;
    }

    if (TypeBAF==1) {
	Module11_P=1;
	Module12_P=1;
	Module15_P=1;
	Module21_P=0;
	Module22_P=0;
	Module25_P=0;
	if ((Module11==Module11_P) && (Module12==Module12_P) && (Module15==Module15_P)) {
	   XZST_03EcritureTrace(XZSTC_FONCTION,"proposition identique a l'utilisation courante. On la supprime");
          if (XZAP29_Ajout_Prop_BAF(vg_numero_plan,
			  vg_ordre_baf++,
			  va_district_baf,
			  "",
			  255,
			  va_numero_baf,
			  "fermeture", 
			  Module11_P,
			  Module12_P,
			  Module15_P,
			  Module21_P,
			  Module22_P,
			  Module25_P,
			  "",
			  /*vm_bra.Realisable) != XDC_OK)*/ /*JMG ??????????*/
			  TRUE) != XDC_OK)
                XZST_03EcritureTrace(XZSTC_FONCTION,"ajouter_baf : pb appel a XZAP29_Ajout_Prop_BAF");
             return (XDC_OK);
	}
    }
    else {
	Module11_P=1;
	Module12_P=1;
	Module15_P=1;
	Module21_P=1;
	Module22_P=1;
	Module25_P=1;
	if ((Module11==Module11_P) && (Module12==Module12_P) && (Module15==Module15_P)  &&
	    (Module21==Module21_P) && (Module22==Module22_P) && (Module25==Module25_P)) {
	   XZST_03EcritureTrace(XZSTC_FONCTION,"proposition identique a l'utilisation courante. On la supprime");
           if (XZAP29_Ajout_Prop_BAF(vg_numero_plan,
			  vg_ordre_baf++,
			  va_district_baf,
			  "",
			  255,
			  va_numero_baf,
			  "fermeture", 
			  Module11_P,
			  Module12_P,
			  Module15_P,
			  Module21_P,
			  Module22_P,
			  Module25_P,
			  "",
			  /*vm_bra.Realisable) != XDC_OK)*/ /*JMG ??????????*/
			  TRUE) != XDC_OK)
              XZST_03EcritureTrace(XZSTC_FONCTION,"ajouter_baf : pb appel a XZAP29_Ajout_Prop_BAF");
           return (XDC_OK);
	}
    }

    /*la presignalisation peut etre pilotee en automatique*/
    if ((vl_mode_auto==0) || ( NumEvt!=0) ){
      /*mise en base de la proposition BAF*/
      if (XZAP29_Ajout_Prop_BAF(vg_numero_plan,
			  vg_ordre_baf++,
			  va_district_baf,
			  "",
			  va_priorite,
			  va_numero_baf,
			  "signalisation", 
			  Module11_P,
			  Module12_P,
			  0,
			  Module21_P,
			  Module22_P,
			  0,
			  "",
			  /*vm_bra.Realisable) != XDC_OK)*/ /*JMG ??????????*/
			  TRUE) != XDC_OK)
        XZST_03EcritureTrace(XZSTC_FONCTION,"ajouter_baf : pb appel a XZAP29_Ajout_Prop_BAF");
    }
    else {
      /*on est ici, donc on traite du pilotage automatique*/
      /*premier cas : on est sur le district de l equipement*/
      if (va_district_baf == vg_site) {
        if (XZAC92_Commande_BAF ( vm_noeud->NumEvt.numero ,
                                 vm_noeud->NumEvt.cle,
                                 vg_num_poste,
                                 va_numero_baf,
                                 va_horodate,
                                 0,
                                 va_priorite,
                                 0,
                                 Module11_P,
                                 Module12_P,
                                 0,
                                 Module21_P,
                                 Module22_P,
                                 0,
                                 va_district_baf,
                                 vg_site) != XDC_OK)
          XZST_03EcritureTrace(XZSTC_WARNING,
                        "ajouter_baf : pb dans l'appel a XZAC92_Commande_BAF");
        else
          XZST_03EcritureTrace(XZSTC_FONCTION,"ajouter_baf : appel a XZAC92 OK");
      }
      else { /*sur districts differents, envoyer un msg a TACLI*/ 
        if (XZAC192_Commande_BAF_TACLI ( vm_noeud->NumEvt.numero ,
                                 vm_noeud->NumEvt.cle,
                                 vg_num_poste,
                                 va_numero_baf,
                                 va_horodate,
                                 0,
                                 va_priorite,
                                 0,
                                 Module11_P,
                                 Module12_P,
                                 0,
                                 Module21_P,
                                 Module22_P,
                                 0,
                                 va_district_baf,
                                 vg_site) != XDC_OK)
          XZST_03EcritureTrace(XZSTC_WARNING,
                        "ajouter_baf : pb dans l'appel a XZAC192_Commande_BAF_TACLI");
        else
          XZST_03EcritureTrace(XZSTC_FONCTION,"ajouter_baf : appel a XZAC192_Commande_BAF_TACLI OK");

      }
    }

    /*la fermeture n'est JAMAIS en automatique*/
    /*mise en base de la proposition BAF*/
    if (XZAP29_Ajout_Prop_BAF(vg_numero_plan,
			  vg_ordre_baf++,
			  va_district_baf,
			  "",
			  va_priorite,
			  va_numero_baf,
			  "fermeture", 
			  Module11_P,
			  Module12_P,
			  Module15_P,
			  Module21_P,
			  Module22_P,
			  Module25_P,
			  "",
			  /*vm_bra.Realisable) != XDC_OK)*/ /*JMG ??????????*/
			  TRUE) != XDC_OK)
      XZST_03EcritureTrace(XZSTC_FONCTION,"ajouter_baf : pb appel a XZAP29_Ajout_Prop_BAF");
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

static int drol_baf_voies(int VL, int VM1, int VM2, int VR, int Reference)

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


static int ajouter_baf_saga(XDY_Eqt va_numero_baf,
                        XDY_District va_district_baf,
                        XDY_Autoroute va_autoroute_baf,
                        XDY_PR va_pr_baf,
                        int va_distance_baf,
                        XDY_Sens va_sens_baf,
                        XDY_District va_district,
                        int va_priorite,
                        XDY_Horodate va_horodate,
                        char *va_perturbation,
			int va_automatique,
                        int *pa_resultat)
{
  XDY_Octet Module11;
  XDY_Octet Module12;
  XDY_Octet Module15;
  XDY_Octet Module21;
  XDY_Octet Module22;
  XDY_Octet Module25;
  XDY_Octet Module11_P;
  XDY_Octet Module12_P;
  XDY_Octet Module15_P;
  XDY_Octet Module21_P;
  XDY_Octet Module22_P;
  XDY_Octet Module25_P;
  XDY_Octet TypeBAF;
  XDY_Octet       Dispo;
  T_STRING        Mode;
  XDY_Entier      NumEvt;
  XDY_Octet       CleEvt;
  XDY_Entier      DistanceEvt;
  XDY_Octet       Priorite;
  int             vl_mode_auto;
  char            vl_dg[50];
       char    l1[30];
        char    l2[30];
        char    l3[30];
        char    l4[30];
        char    l5[30];
        char    a1[30];
        char    a2[30];
        char    a3[30];
        char    a4[30];
        char    a5[30];
        char    picto[30];
        char    bandeau[30];
        XDY_Booleen     cligno;
        XDY_Mot lumino, barriere, scenario;
        XDY_Octet       numpicto;

    /*on a trouve une BAF*/
    if ((va_district_baf!=va_district) && (va_district!=vg_SitePrincipal))
      return (XDC_NOK);

    /*appel XZAT06*/
    if (XZAT150_Utilisation_Eqt_SAGA(     va_horodate, va_numero_baf,
		    &Dispo, l1, l2, l3, l4, l5,
		     a1, a2, a3, a4, a5,
		      &cligno, picto, &lumino, bandeau,
		       &barriere, &scenario,
			&NumEvt, &CleEvt, &Priorite, &DistanceEvt,
			&TypeBAF) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING,
	    "Etat de %d inconnu a la date %lf. On le considere indispo.",va_numero_baf, va_horodate);
      Dispo = XDC_EQT_MAJEUR;
    }														    
    if (Dispo > XDC_EQT_MINEUR)
    {
      XZST_03EcritureTrace(XZSTC_FONCTION,"baf indispo, on l'oublie");
      return (XDC_NOK);
    }

    /*au CI, si la nav est utilise par un evt plus proche, on l'oublie*/
    /* Uniquement pour ESCOTA, les autres sites principaux sont des PC */
    if ( (va_district==XDC_CI) &&
         (va_distance_baf>DistanceEvt) && (DistanceEvt!=0)) {
      XZST_03EcritureTrace(XZSTC_FONCTION,"baf est utilise par un evt plus proche, on l'oublie");
      return (XDC_NOK);
    }
  
    /*si on calcule un plan d'actions pour une simulation*/
    vl_mode_auto= 0;


    XZST_03EcritureTrace(XZSTC_WARNING,"vg_site=%d;va_baf.District=%d",vg_site,va_district_baf);
    XZST_03EcritureTrace(XZSTC_WARNING,"vg_presence_site[va_baf.District]=%d",vg_presence_site[va_district_baf]);
    if (((vg_site==XDC_CI) && (vg_presence_site[va_district_baf]==XDC_VRAI)) || drol_baf_fmc_simulee()) {
      /*on ne fait que proposer, pas de pilotage automatique*/
      /* Ne s'applique pas sur COFIROUTE, uniquement sur ESCOTA */
      /* Sur COFIROUTE, le site principal peut piloter les BAD */
      vl_mode_auto=0;
    }

	Module11_P=1;
	if (barriere==Module11_P)  {
	   XZST_03EcritureTrace(XZSTC_FONCTION,"proposition identique a l'utilisation courante. On la supprime");
          if (XZAP70_Ajout_Prop_SAGA(vg_numero_plan,
			  vg_ordre_baf++,
			  va_district_baf,
			  "",
			  255,
			  va_numero_baf,
			  "fermeture", 
			  "", /*T1*/
			  "",
			  "",
			  "",
			  "", /*T5*/
			  "", /*libelle*/
			  "", /*bandeau*/
			  "", /*picto*/
			  1, /*barriere*/
			  0, /*scenario*/
			  0, 
			  0,
			  0,
			  TRUE,0) != XDC_OK)
                XZST_03EcritureTrace(XZSTC_FONCTION,"ajouter_baf : pb appel a XZAP70_Ajout_Prop_SAGA");
             return (XDC_OK);
	}

    /*la presignalisation peut etre pilotee en automatique*/
    if ((vl_mode_auto==0) || ( NumEvt!=0) ){
      /*mise en base de la proposition BAF*/
          if (XZAP70_Ajout_Prop_SAGA(vg_numero_plan,
			  vg_ordre_baf++,
			  va_district_baf,
			  "",
			  va_priorite,
			  va_numero_baf,
			  "fermeture", 
			  "", /*T1*/
			  "",
			  "",
			  "",
			  "", /*T5*/
			  "", /*libelle*/
			  "", /*bandeau*/
			  "", /*picto*/
			  1, /*barriere*/
			  0, /*scenario*/
			  0, 
			  0,
			  0,
			  TRUE,0) != XDC_OK)
                XZST_03EcritureTrace(XZSTC_FONCTION,"ajouter_baf : pb appel a XZAP70_Ajout_Prop_SAGA");
    }

  return (XDC_OK);
}
