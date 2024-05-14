/* Fichier : $Id: drol_pal.c,v 1.24 2020/06/02 21:44:04 devgfi Exp $        $Revision: 1.24 $        $Date: 2020/06/02 21:44:04 $
**************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : SAIDEC				*
* FICHIER : drol_pal.c				*
*************************************************
* DESCRIPTION :
* new tdpac
*************************************************
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.0
* PCL	17/09/04	permettre la proposition de plusieurs pal (contresens) 
			et eteindre les pal dans le bouchon V1.2
* JMG	12/11/04	pilotage des PAL en automatique 1.3
* PCL	17/11/04	traces sur appel XZAC67 + on passe en parametre le no d'evt 
			gestion automatique et manuel dans certains cas 1.4
* JMG	15/12/04	ajout explication claire dans XZAP 1.5
* CHI	20/04/05	:rajout du sous-type grele sur la chaussee DEM457 v1.6
* PNI	21/06/07 : Ajout des nouveaux type de fmc DEM658 1.7
* LCL   09/07/07 : Suppression DY 1.8
* JMG	02/10/9	: secto DEM887 1.9
* JPL	06/09/10 : Prise en compte des Fmc Zone de stockage (DEM 942) 1.10
* JPL	21/02/11 : Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.11
* JPL	21/02/11 : Migration HP ia64 (DEM 971) : Gestion conversions types de tailles differentes (=)  1.12
* JPL	13/10/11 : Pas d'action automatique SAPIENS pour Fmc simulees (Essai ou Exercice) (DEM 995)  1.13
* PNI	26/02/13 : Pas d'activation PAL pour bouchon contenu sur bretelle 1.14
* PNI	23/08/13	 : SAPIENS au CI  DEM 1074/FR 105 1.15
* PNI	26/06/15 : Prise en compte de XDC_VOIE_BLOQUEE_NEUTRALISEE dans drol_pal_sur_bau DEM 1132 1.17
* JPL	30/11/16 : Niveaux de traces; suppression de traces superflues  1.18
* JPL	02/12/16 : Support du type Travaux et consigne de vitesse associee (DEM 1198)  1.19
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.20
* PNI	16/06/17	: Pas de pilotage automatique pour les PAL sur bretelle DEM1226 1.21
* JPL	23/09/19 : Type d'evenement "INCENDIE" attendu en configuration et non "INCENDIE DE VEGETATION" (DEM 1352)  1.23
* JPL	01/10/19 : Support d'un predicat "bouchon mobile" (DEM 1346)  1.24
* LCL   27/09/23 : Adaptations CNA DEM-483
* LCL	04/10/23 : Adaptations CNA DEM-483 : lecture site principal en base
* GGY	18/01/24 : Ajout C_COND_CIRCULATION_BASC_TUNNEL et C_COND_CIRCULATION_NON_BASC_TUNNEL (DEM-483)
* PNI 04/03/24 : xzat280, ajout traces SAE-594
************************************************/

#include "drol_pal.h"

#include "xzac66.h"
#include "xzac67.h"
#include "xzap27.h"


/* Scenarios specifiques proposes */
#define	C_SCN_EXTINCTION		"M_SC11"
#define	C_NOM_SCN_EXTINCTION		"EXTINCTION"

#define	C_SCN_LIM_VIT_HAUTE		"M_SC12"
#define	C_NOM_SCN_LIM_VIT_HAUTE		"LIMITATION VITESSE HAUTE"

#define	C_SCN_LIM_VIT_BASSE		"M_SC13"
#define	C_NOM_SCN_LIM_VIT_BASSE		"LIMITATION VITESSE BASSE"


/* Valeurs des limites de vitesse */
#define	C_LIM_VIT_HAUTE		90
#define	C_LIM_VIT_BASSE		70


/* Valeurs des distances maximales a l'evenement pour activation */
#define C_DISTANCE		1200
#define C_DISTANCE_CONTRESENS	10000


/* Description de la configuration */
#define C_REP_MIG							"/produits/migrazur/appliSD/fichiers/bdc/"
#define C_PERT		C_REP_MIG 				"pal.txt"

#define C_SEPARATEUR						"#"
#define C_PIPE								"|"
#define C_ET								"$ET$"
#define C_CAR_DOLLAR						'$'

#define C_COND_SUR_BAU						"sur bau"
#define C_COND_SUR_SECTION_COURANTE			"sur section courante"
#define C_COND_SUR_BRETELLE_ENTREE      	"sur bretelle entree"
#define C_COND_BOUCHON_MOBILE				"bouchon mobile"
#define C_COND_CIRCULATION_BASC_TUNNEL		"circulation_basculee"
#define C_COND_CIRCULATION_NON_BASC_TUNNEL	"circulation_non_basculee"

#define C_MODE_AUTO	1
#define C_MODE_MANUEL	0


T_PERT_PAL      *vg_pert_pal;
T_NOEUD		*vm_noeud;
static T_NOEUD	*vm_noeud2;
T_NOEUD 	*vg_racine;
T_PAL		vm_pal;

extern XDY_Booleen vg_presence_site[];
extern XDY_Booleen      vg_calcul_auto;

int drol_pal_charge();
T_CONDITION *drol_pal_decode_condition(char *);

static XDY_Booleen drol_pal_egal(int, int);
static XDY_Booleen drol_pal_sur_bretelle_entree();


/*fonctions utilisees dans les conditions*/
static XDY_Booleen drol_pal_fmc_simulee();
static XDY_Booleen drol_pal_bouchon_mobile();
static XDY_Booleen drol_pal_sur_bau();
static XDY_Booleen drol_pal_sur_section_courante();
static XDY_Booleen drol_pal_circulation_basculee_tunnel ();
static XDY_Booleen drol_pal_circulation_non_basculee_tunnel ();

static int drol_pal_recherche(T_PERT_PAL *,
			XDY_Autoroute ,
		        XDY_PR ,
			XDY_Sens ,
			XDY_Horodate ,
			XDY_District,
			XDY_Booleen,
			int,
			int,
			int,
			XDY_PR,
			XDY_Booleen,
			XDY_Autoroute,
			XDY_Sens,
			XDY_Booleen);
XDY_Booleen drol_pal_mode(XDY_Booleen,int,int);
static int drol_pal_voies(int, int, int, int, int);

static int ajouter_pal(XDY_Eqt ,
			XDY_District ,
			XDY_Autoroute ,
			XDY_PR ,
			XDY_Sens ,
			int,
			XDY_District ,
			XDY_Horodate ,
			char *,
			int     *);

/* definition des variables locales au module */
static char *version = "$Id: drol_pal.c,v 1.24 2020/06/02 21:44:04 devgfi Exp $ : drol_pal" ;

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_pal_init()

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
  char *vl_mode;
  FILE  *pl_fp;
  char        vl_Lig[500];
  char	*vl_str;

  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : drol_pal_init");
  vm_noeud2=(T_NOEUD *)malloc(sizeof(T_NOEUD));

  drol_pal_charge();

  XZST_03EcritureTrace(   XZSTC_FONCTION,"OUT drol_pal_init");

  return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des perturbations
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_pal_charge()

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
  char *vl_ch;
  T_PERT_PAL *pl_liste;
  int	vl_num_prec,vl_num;
  char vl_condition[300];

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : drol_pal_charge");

  if ( (pl_fp = fopen(C_PERT, "r")) == NULL ) {
    XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !" ,C_PERT );
    return(XDC_NOK);
  }
  
  while (fgets(vl_Lig,sizeof(vl_Lig),pl_fp) != NULL)
  {
      pl_liste = (T_PERT_PAL *)malloc(sizeof(T_PERT_PAL));
      vl_str=vl_Lig; 
      sscanf(vl_str,"%[^|]|%[^|]|%[^|]|%[^|]|%d",pl_liste->Libelle,vl_condition,pl_liste->Scenario,pl_liste->Nom_Scenario, &pl_liste->Mode);
      
      XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pal_charge : libelle %s, condition = %s, scenario = %s, nom scenario = %s, mode = %d",
      				pl_liste->Libelle,vl_condition,pl_liste->Scenario,
				pl_liste->Nom_Scenario,
				pl_liste->Mode);
      pl_liste->Condition = drol_pal_decode_condition(vl_condition);
      pl_liste->Suivant = vg_pert_pal;
      vg_pert_pal = pl_liste;
  }
  fclose(pl_fp);

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT  : drol_pal_charge");
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* decodage de la condition
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

T_CONDITION *drol_pal_decode_condition(char *va_cond)

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
  int vl_vitesse;
  int vl_len;

  /*decode les elements separes par des $ET$*/
  XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pal_decode_condition:  IN %s",va_cond);

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
  pl_cond->Fonction.Comp.Fct=drol_pal_egal;
  pl_cond->Suivant=NULL;
  pl_tmp=pl_cond;

  /*numero en fonction du type*/
  XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_pal_decode_condition: type = %s",pl_cond->Libelle);

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
    pl_tmp->Fonction.Comp.Fct=drol_pal_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Verglas;
  }
  else if (!strcmp(vl_ch,"CHUTE NEIGE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pal_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_ChuteNeige;
  }
  else if (!strcmp(vl_ch,"PLUIE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pal_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Pluie;
  }
  else if (!strcmp(vl_ch,"BROUILLARD GIVRANT")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pal_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_BrouillardGiv;
  }
  else if (!strcmp(vl_ch,"NEIGE SUR CHAUSSEE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pal_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_NeigeChaus;
  }
  else if (!strcmp(vl_ch,"GRELE SUR CHAUSSEE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pal_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_GreleChaus;
  }
  else if (!strcmp(vl_ch,"BROUILLARD")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pal_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Brouillard;
  }
  else if (!strcmp(vl_ch,"VENT")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pal_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_VentFort;
  }
  else if (!strcmp(vl_ch,"GRELE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pal_egal;
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
    pl_tmp->Fonction.Comp.Fct=drol_pal_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Incident);
    pl_tmp->Fonction.Comp.Valeur=1;
  }
  else if (!strcmp(vl_ch,"PENURIE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_IncSsConces;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pal_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Incident);
    pl_tmp->Fonction.Comp.Valeur=0;
  }
  /*cas particulier du lit d'arret*/
  else if (!strcmp(vl_ch,"sur lit d'arret")) {
    pl_cond->Fonction.Comp.Slot=&(vm_noeud2->Localisation.TypePointCar);
    pl_cond->Fonction.Comp.Valeur=XDC_POINT_CARACT_LIT_ARRET;
  }
  else {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_pal_decode_condition : pb decodage type !!!");
    pl_cond->Fonction.Comp.Valeur=0;
  }
 
  /*si pas de condition supplementaire*/
  if (vl_indice==strlen(va_cond)) {
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : drol_pal_decode_condition : que le type");
    return (pl_cond);
  }

  /*supprimer $ET$*/
  va_cond=va_cond+strlen(pl_cond->Libelle) + strlen(C_ET);


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

    XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_pal_decode_condition:  cond %s",vl_str);
    strcpy(pl_tmp->Libelle,vl_str);

    /* Le type de condition est suppose etre un PREDICAT par defaut */
    pl_tmp->Type=C_TYPE_PREDICAT;
 /*   if (!strcmp(vl_str,C_COND_UNE_VOIE_BLOQ)) pl_tmp->Fonction.Predicat.Fct=drol_pal_une_voie_bloquee;
    else if (!strcmp(vl_str,C_COND_UNE_VOIE_OUVERTE)) pl_tmp->Fonction.Predicat.Fct=drol_pal_une_voie_ouverte;
    else if (!strcmp(vl_str,C_COND_TOUTES_VOIES_BLOQ)) pl_tmp->Fonction.Predicat.Fct=drol_pal_toutes_voies_bloquees;
    else if (!strcmp(vl_str,C_COND_TOUTES_VOIES_NEUT)) pl_tmp->Fonction.Predicat.Fct=drol_pal_toutes_voies_neutralisees;
    else if (!strcmp(vl_str,C_COND_UNE_VOIE_NEUT)) pl_tmp->Fonction.Predicat.Fct=drol_pal_une_voie_neutralisee;
    else if (!strcmp(vl_str,C_COND_CIRCULATION_UNE_VOIE)) pl_tmp->Fonction.Predicat.Fct=drol_pal_circulation_une_voie;
    else if (!strcmp(vl_str,C_COND_CONFIRME)) pl_tmp->Fonction.Predicat.Fct=drol_pal_confirme;
    else if (!strcmp(vl_str,C_COND_SIGNALE)) pl_tmp->Fonction.Predicat.Fct=drol_pal_signale;*/
    if (!strcmp(vl_str,C_COND_SUR_BAU)) pl_tmp->Fonction.Predicat.Fct=drol_pal_sur_bau;
    else if (!strcmp(vl_str,C_COND_SUR_SECTION_COURANTE)) pl_tmp->Fonction.Predicat.Fct=drol_pal_sur_section_courante;
    else if (!strcmp(vl_str,C_COND_SUR_BRETELLE_ENTREE)) pl_tmp->Fonction.Predicat.Fct=drol_pal_sur_bretelle_entree;
    else if (!strcmp(vl_str,C_COND_BOUCHON_MOBILE)) pl_tmp->Fonction.Predicat.Fct=drol_pal_bouchon_mobile;
 /*   else if (!strcmp(vl_str,C_COND_SUR_LIT_ARRET)) pl_tmp->Fonction.Predicat.Fct=drol_pal_sur_lit_arret;
    else if (!strcmp(vl_str,C_COND_SUR_ECHANGEUR)) pl_tmp->Fonction.Predicat.Fct=drol_pal_sur_echangeur;
    else if (!strcmp(vl_str,C_COND_SUR_BRETELLE_ENTREE)) pl_tmp->Fonction.Predicat.Fct=drol_pal_sur_bretelle_entree;
    else if (!strcmp(vl_str,C_COND_SUR_BRETELLE_SORTIE)) pl_tmp->Fonction.Predicat.Fct=drol_pal_sur_bretelle_sortie;
    else if (!strcmp(vl_str,C_COND_TRAFIC_DENSE)) pl_tmp->Fonction.Predicat.Fct=drol_pal_trafic_dense;
    else if (!strcmp(vl_str,C_COND_TRAFIC_RALENTI)) pl_tmp->Fonction.Predicat.Fct=drol_pal_trafic_ralenti;
    else if (!strcmp(vl_str,C_COND_TRAFIC_BOUCHE)) pl_tmp->Fonction.Predicat.Fct=drol_pal_trafic_bouche;
    else if (!strcmp(vl_str,C_COND_TRAFIC_BLOQUE)) pl_tmp->Fonction.Predicat.Fct=drol_pal_trafic_bloque;
    else if (!strcmp(vl_str,C_COND_DATEX)) pl_tmp->Fonction.Predicat.Fct=drol_pal_datex;
    else if (!strcmp(vl_str,C_COND_VOIES_RETRECIES)) pl_tmp->Fonction.Predicat.Fct=drol_pal_voies_retrecies;
    else if (!strcmp(vl_str,C_COND_STOCKAGE_DEMARRE)) pl_tmp->Fonction.Predicat.Fct=drol_pal_stockage_demarre;
    else if (!strcmp(vl_str,C_COND_STOCKAGE_NON_DEMARRE)) pl_tmp->Fonction.Predicat.Fct=drol_pal_stockage_non_demarre;
    else if (!strcmp(vl_str,C_COND_ENTREE_FERMEE)) pl_tmp->Fonction.Predicat.Fct=drol_pal_entree_fermee;
    else if (!strcmp(vl_str,C_COND_SORTIE_FERMEE)) pl_tmp->Fonction.Predicat.Fct=drol_pal_sortie_fermee;*/
	else if (!strcmp(vl_str,C_COND_CIRCULATION_BASC_TUNNEL)) pl_tmp->Fonction.Predicat.Fct = drol_pal_circulation_basculee_tunnel;
	else if (!strcmp(vl_str,C_COND_CIRCULATION_NON_BASC_TUNNEL)) pl_tmp->Fonction.Predicat.Fct = drol_pal_circulation_non_basculee_tunnel;
    else {
      pl_tmp->Type=C_TYPE_COMPARATEUR;
      pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Type);
      pl_tmp->Fonction.Comp.Fct=drol_pal_egal;
      pl_tmp->Fonction.Comp.Valeur=0;

      if (sscanf(vl_str, " vitesse = %d", &vl_vitesse) > 0) {
        pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Vitesse);
        pl_tmp->Fonction.Comp.Valeur=vl_vitesse;
      } else {
        XZST_03EcritureTrace(   XZSTC_WARNING,"drol_pal_decode_condition : pb decodage condition !!!C_COND_SUR_BRETELLE_ENTREE:%s.",vl_str);
      }
    }
 
  /*si fin de chaine*/
  if (vl_indice==strlen(va_cond)) {
    XZST_03EcritureTrace(   XZSTC_DEBUG1,"drol_pal_decode_condition : fin de chaine");
    return (pl_cond);
  }
  else {
    va_cond=va_cond+strlen(vl_str) + strlen(C_ET);
    goto pos_decode;
  }
}


/**************************************/
/**************************************/
static XDY_Booleen drol_pal_fmc_simulee() {
  return (strstr (vm_noeud->Infos_TFM.Origine, "ESSAI") != NULL  ||
          strstr (vm_noeud->Infos_TFM.Origine, "EXERCICE") != NULL);
}

static XDY_Booleen drol_pal_bouchon_mobile() {
  if (vm_noeud->Infos_FAX.Presence_Bouchon == XDC_VRAI)
	return (XDC_VRAI);
  return (XDC_FAUX);
}

static XDY_Booleen drol_pal_sur_bau() { 
  return ( (drol_pal_voies(vm_noeud->Localisation.VL,vm_noeud->Localisation.VM1,
			   vm_noeud->Localisation.VM2,
			   vm_noeud->Localisation.VR,
			   XDC_VOIE_SENS_NORMAL)==15) &&
           ((vm_noeud->Localisation.BAU==XDC_VOIE_BLOQUEE) ||
	    (vm_noeud->Localisation.BAU==XDC_VOIE_NEUTRALISEE)||
	    (vm_noeud->Localisation.BAU==XDC_VOIE_BLOQUEE_NEUTRALISEE)
           )
         );
}

/*renvoie TRUE si l evt sur la chaussee circulee*/
static XDY_Booleen drol_pal_sur_section_courante() { 
  return ( (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_AIRE) &&
	   (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_ECHANGEUR) &&
	   (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_PEAGE) &&
	   (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_LIT_ARRET) &&
	   (drol_pal_voies(vm_noeud->Localisation.VL,vm_noeud->Localisation.VM1,
		    vm_noeud->Localisation.VM2,
		    vm_noeud->Localisation.VR,
		    XDC_VOIE_SENS_NORMAL)!=15)
         );
}

/*renvoie TRUE si l evt sur la chaussee circulee*/
static XDY_Booleen drol_pal_sur_bretelle_entree() { 
 return ( (vm_noeud->Localisation.TypePointCar==XDC_POINT_CARACT_ECHANGEUR) && ((vm_noeud->Localisation.Sens==XDC_SENS_ENTREE_NORD) || (vm_noeud->Localisation.Sens==XDC_SENS_ENTREE_SUD))
         );
}

static XDY_Booleen drol_pal_egal(int va_valeur1, int va_valeur2) 
{ return (va_valeur1==va_valeur2); }

static XDY_Booleen drol_pal_circulation_basculee_tunnel ()
{
	XDY_Booleen vl_basculement = XDC_FAUX;
	
	XZST_03EcritureTrace(XZSTC_WARNING,"drol_pal_circulation_basculee_tunnel : avant XZAE861_Basculement_Tunnel");

	if (XZAE861_Basculement_Tunnel (vm_noeud->Localisation.Autoroute,
									vm_noeud->Localisation.PR,
									vm_noeud->Localisation_Tete.PR,
									vl_basculement)	!= XDC_OK) 
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_pal_circulation_basculee_tunnel : pb appel a XZAE861_Basculement_Tunnel");
		return (FALSE);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_pal_circulation_basculee_tunnel : XZAE861_Basculement_Tunnel retourne %s", vl_basculement);
	}
	
	return (vl_basculement);
}

static XDY_Booleen drol_pal_circulation_non_basculee_tunnel ()
{
	XZST_03EcritureTrace (XZSTC_WARNING, "drol_pal_circulation_non_basculee_tunnel : not drol_pal_circulation_basculee_tunnel");
	return (!drol_pal_circulation_basculee_tunnel());
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  compare l'etat de chaque voie a l'etat de reference
*  tient compte du nombre de voies de circulation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_pal_voies(int VL, int VM1, int VM2, int VR, int Reference)

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
* fct de recherche des perturbations engendrees par l'evt
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen drol_pal_filtre(T_NOEUD *va_noeud, XDY_District va_site, XDY_Horodate va_horodate)


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
  T_PERT_PAL *pl_cas;
  T_CONDITION *pl_cond;
  XDY_Booleen (*vl_fct)();
  XDY_Booleen vl_res=FALSE;
  T_PERT_PAL *vl_pert;
  int vl_indice;
  XDY_Sens vl_sens;
  XDY_Booleen vl_bouchon;

  XZST_03EcritureTrace(   XZSTC_FONCTION,"IN : drol_pal_filtre %d/%d",
			va_noeud->NumEvt.numero,
			va_noeud->NumEvt.cle);


  vm_noeud=va_noeud;
  vm_noeud2->Localisation=vm_noeud->Localisation;
  vm_noeud2->Infos_FAX=vm_noeud->Infos_FAX;
  vm_noeud2->Type=vm_noeud->Type;

  vl_pert=NULL;
  pl_cas= vg_pert_pal;


  /*sur bouchon*/
  if (va_noeud->Type==XZAEC_FMC_QueueBouchon)
  {
    vl_bouchon=TRUE;
  }
  else
  {
    vl_bouchon=FALSE;
  }

  while (pl_cas!=NULL)
  {
    XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pal_filtre %s",pl_cas->Libelle);
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
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pal_filtre OK pour %s ",
					pl_cas->Libelle);
      drol_pal_recherche(pl_cas,va_noeud->Localisation.Autoroute, 
		va_noeud->Localisation.PR,
		va_noeud->Localisation.Sens,
		va_horodate,
		va_site,
		va_noeud->Infos_TFM.Confirme,
		va_noeud->Localisation.VL,
		va_noeud->Localisation.VR,
		va_noeud->Type,
		va_noeud->Localisation_Tete.PR,
		vl_bouchon,
		va_noeud->Localisation_Tete.Autoroute,
		va_noeud->Localisation_Tete.Sens,
		va_noeud->Infos_TFM.Termine);
      return (TRUE);
    }
    pl_cas=pl_cas->Suivant;
  }
  return (TRUE);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des PAL
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_pal_recherche(T_PERT_PAL *va_pert, 
			XDY_Autoroute va_autoroute,
		        XDY_PR va_PR,
			XDY_Sens va_sens,
			XDY_Horodate va_horodate,
			XDY_District va_site,
			XDY_Booleen va_confirme,
			int va_VL,
			int va_VR,
			int va_type_evt,
			XDY_PR va_PR_tete,
			XDY_Booleen va_flag_bouchon,
			XDY_Autoroute va_autoroute_tete,
			XDY_Sens va_sens_tete,
			XDY_Booleen va_termine)

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
  XDY_Entier  vl_inversee=1;
  char		  vl_scenario[60];
  int vl_distance, vl_numero;
  XDY_Entier vl_cr;

  XZST_03EcritureTrace(   XZSTC_FONCTION,"IN : drol_pal_recherche aut=%d,PR=%d,sens=%d,site=%d",
						va_autoroute,va_PR,va_sens,va_site);

  if ((va_sens == XDC_SENS_INCONNU) /* || 
      (va_sens == XDC_SENS_ENTREE_NORD) || 
      (va_sens == XDC_SENS_ENTREE_SUD)  */ ) {
    XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pal_recherche : OUT evt sur une entree");
    return (XDC_NOK);
  }

  /*PATCH A57
  if (va_autoroute==8)
    vl_inversee=-1;*/

  vl_sens=va_sens;
/* PAs d'activation PAL pour evt sur bretelle 
if (va_sens == XDC_SENS_SORTIE_NORD)
    vl_sens = XDC_SENS_NORD;
  if (va_sens == XDC_SENS_SORTIE_SUD)
    vl_sens = XDC_SENS_SUD;
*/


  /* Pour des travaux, les PALs eligibles sont ceux situes dans les voies en travaux */
  if (va_type_evt==XZAEC_FMC_Travaux) {
    vl_distance=abs(va_PR - va_PR_tete);
    va_PR = va_PR_tete;
  } else if (va_type_evt==XZAEC_FMC_Contresens) {
    vl_distance=C_DISTANCE_CONTRESENS;
  } else {
    vl_distance=C_DISTANCE;
  }

  sprintf(vl_scenario,"%s|%s|%d",va_pert->Scenario,va_pert->Nom_Scenario,va_pert->Mode);

  /*si evt termine*/
  if (va_termine)
    vl_numero = 2;
  else if (va_flag_bouchon)
    vl_numero = 1;
  else
    vl_numero = 0;

    XZAO500_PAL_amont(va_autoroute, va_PR, vl_sens%10, vl_numero,
	  vl_distance,va_site,
	  vl_scenario,(XDY_FonctionInt)ajouter_pal,
	  va_horodate,&vl_cr,va_PR_tete,va_flag_bouchon,vm_noeud->NumEvt.numero,vm_noeud->NumEvt.cle);

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : drol_pal_recherche retourne %d", vl_cr);
  return (vl_cr);
}


/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* fct utilisateur passee dans XZAO500
* utilisee pour creer un PAL
*
******************************************************
* SEQUENCE D'APPEL :
* ajouter_pal
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
static int ajouter_pal (XDY_Eqt va_numero_pal,
		XDY_District va_district_pal,
		XDY_Autoroute va_autoroute,
		XDY_PR va_pr_pal,
		XDY_Sens va_sens,
		int va_distance,
		XDY_District va_district,
		XDY_Horodate va_horodate,
		char *va_scenario,
		int *pa_resultat)
{
  XDY_Entier      Distance;
  XDY_Entier      PRInfluence;
  XDY_Octet       Type;
  XDY_Octet       Dispo;
  T_STRING        Scenario;
  XDY_Entier      NumEvt;
  XDY_Octet       CleEvt;
  XDY_Octet       TypeEvt;
  XDY_Octet       Priorite = 0;
  XDY_Entier      DistanceEvt;
  char		  vl_cmd[20] = "";
  T_STRING        vl_scenario;
  T_STRING        vl_nom_scenario;
  T_STRING        vl_explication;
  XDY_Datagroup	  vl_dg;
  int	  	  vl_mode_auto;

    *pa_resultat = XDC_OK;

    /*on a trouve un PAL*/
    /*on verifie qu'il appartient bien au district*/
    if ((va_district_pal!=va_district) && (va_district!=XDC_CI))
    /* Uniquement pour ESCOTA, les autres sites principaux sont des PC */
      return (XDC_OK);

    /* determiner l'utilisation actuelle du PAL */
    if (XZAT280_Utilisation_PAL(     va_horodate, va_numero_pal,
		    &Dispo, Scenario, &NumEvt, &CleEvt, &Priorite, &DistanceEvt, &TypeEvt) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING,
	    "Etat PAL %d inconnu a la date %lf. On le considere indispo.", (int) va_numero_pal, va_horodate);
      Dispo = XDC_EQT_MAJEUR;
    }

    XZST_03EcritureTrace(XZSTC_INFO,"retour XZAT280 scenario=%s.dispo=%d",Scenario,Dispo);

    if (Dispo > XDC_EQT_MINEUR) {
      XZST_03EcritureTrace(XZSTC_INFO,"OUT: PAL %d indispo, ignore", (int) va_numero_pal);
      return (XDC_OK);
    }


    /* Decoder le nom du scenario configure */
    sscanf(va_scenario,"%[^|]|%[^|]|%d",vl_scenario,vl_nom_scenario,&vl_mode_auto);

    vm_pal.Identifiant = va_numero_pal;
    vm_pal.District = va_district_pal;
    vm_pal.Distance = va_distance;
    strcpy(vm_pal.Scenario_Courant,Scenario);
    vm_pal.Dispo=TRUE;
    vm_pal.NumEvt = vm_noeud->NumEvt.numero;
    vm_pal.CleEvt = vm_noeud->NumEvt.cle;
    vm_pal.Priorite = Priorite;
    vm_pal.DistanceEvt = va_distance;
XZST_03EcritureTrace(XZSTC_INFO,"vm_pal.Scenario_Courant=%s.",Scenario);
    /* Pas de pilotage automatique pour les PAL sur bretelle DEM1226*/
    if ((va_sens!=XDC_SENS_NORD) && (va_sens!=XDC_SENS_SUD))
	vl_mode_auto=C_MODE_MANUEL;

    /* Si l'evenement est termine ou le PAL est hors zone d'influence, il doit etre eteint */
    if ( (vm_noeud->Infos_TFM.Termine)  ||  (va_distance==-99) ) {
      strcpy(vm_pal.Scenario,C_SCN_EXTINCTION);
      strcpy(vm_pal.Nom_Scenario,C_NOM_SCN_EXTINCTION);
    }
    else if (vm_noeud->Type == XZAEC_FMC_Travaux)
    {
      /* Pour des travaux ignorer le PAL s'il est utilise par un autre evenement (donc dans le kilometre aval ou prioritaire) */
      if ( (NumEvt != 0)  &&  ((NumEvt != vm_noeud->NumEvt.numero) || (CleEvt != vm_noeud->NumEvt.cle)) )
      {
        XZST_03EcritureTrace(XZSTC_INFO,"PAL %d utilise par un evt prioritaire, ignore", (int) va_numero_pal);
        return (XDC_OK);
      }
      else {
        /* sinon choisir le scenario adapte a la vitesse limite selectionnee pour les travaux, s'il y en a un */
        if (C_LIM_VIT_HAUTE == vm_noeud->Infos_FAX.Vitesse) {
          strcpy(vm_pal.Scenario, C_SCN_LIM_VIT_HAUTE);
          strcpy(vm_pal.Nom_Scenario, C_NOM_SCN_LIM_VIT_HAUTE);
        } else if (C_LIM_VIT_BASSE == vm_noeud->Infos_FAX.Vitesse) {
          strcpy(vm_pal.Scenario, C_SCN_LIM_VIT_BASSE);
          strcpy(vm_pal.Nom_Scenario, C_NOM_SCN_LIM_VIT_BASSE);
        } else if ( (NumEvt == vm_noeud->NumEvt.numero) && (CleEvt == vm_noeud->NumEvt.cle) ) {
          /* et si aucune condition d'activation n'est satisfaite, proposer l'extinction si le PAL est utilise pour l'evenement */
          strcpy(vm_pal.Scenario,C_SCN_EXTINCTION);
          strcpy(vm_pal.Nom_Scenario,C_NOM_SCN_EXTINCTION);
        } else {
          return (XDC_OK);
        }
      }
    }
    else
    {
      /* Pour un autre type d'evenement ignorer le PAL s'il est utilise par un evenement plus proche, sauf des travaux */
      if ( (NumEvt != 0)  &&  ((NumEvt != vm_noeud->NumEvt.numero) || (CleEvt != vm_noeud->NumEvt.cle))  &&
           (DistanceEvt <= va_distance)  &&  (TypeEvt != XZAEC_FMC_Travaux)
         )
      {
        XZST_03EcritureTrace(XZSTC_INFO,"PAL %d utilise par un evt plus proche, ignore", (int) va_numero_pal);
        return (XDC_OK);
      }
      else {
        /* sinon proposer le scenario configure */
        strcpy(vm_pal.Scenario, vl_scenario);
        strcpy(vm_pal.Nom_Scenario, vl_nom_scenario);
        XZST_03EcritureTrace(XZSTC_INFO,"pal scenario propose = %s, distance %d, mode %d, scenario %s.",va_scenario,va_distance,vl_mode_auto,vm_pal.Scenario );
      }
    }


    /*suppression des utilisations identiques a l'utilisation courante*/
    if (!strcmp(vm_pal.Scenario_Courant,vm_pal.Scenario)) {
      XZST_03EcritureTrace(XZSTC_INFO,"proposition PAL identique a l'utilisation courante, ignoree");
      return (XDC_OK);
    }


    if (!strcmp(vl_scenario,C_SCN_EXTINCTION)) {
      strcpy(vl_cmd, "LIBERATION");
    }

    sprintf(vl_explication,"%s (%s)",vm_pal.Nom_Scenario, vm_pal.Scenario);


    switch (vm_pal.District) {
     case (XDC_CI) : XDG_EncodeDG2(vl_dg,XDG_EPAL_AUTO,XDC_NOM_SITE_CI);
		     break;
     case (XDC_VC) : XDG_EncodeDG2(vl_dg,XDG_EPAL_AUTO,XDC_NOM_SITE_VC);
		     break;
     case (XDC_DP) : XDG_EncodeDG2(vl_dg,XDG_EPAL_AUTO,XDC_NOM_SITE_DP);
		     break;
     case (XDC_CA) : XDG_EncodeDG2(vl_dg,XDG_EPAL_AUTO,XDC_NOM_SITE_CA);
		     break;
    }


  XZST_03EcritureTrace(XZSTC_DEBUG3,"vg_site=%d;va_pal.District=%d",vg_site,vm_pal.District);
XZST_03EcritureTrace(XZSTC_DEBUG3,"vg_presence_site[va_pal.District]=%d",vg_presence_site[vm_pal.District]);

    /*si on calcule un plan d'actions pour une simulation*/
    if (((vg_site==XDC_CI) && (vg_presence_site[vm_pal.District]==XDC_VRAI)) || drol_pal_fmc_simulee()) {
    	/*on ne fait que proposer, pas de pilotage automatique*/
      /* Ne s'applique pas sur COFIROUTE, uniquement sur ESCOTA */
      /* Sur COFIROUTE, le site principal peut piloter les BAD */
	    vl_mode_auto=C_MODE_MANUEL;
    }

   if ((vg_calcul_auto) && (!drol_pal_fmc_simulee()))
       vl_mode_auto=C_MODE_AUTO;

   if ((vm_noeud->Type==XZAEC_FMC_VehPanne) && (vm_noeud->Infos_TFM.Termine))
        vl_mode_auto=C_MODE_MANUEL;

    /*on pilote directement en mode automatique*/
    if (vl_mode_auto==C_MODE_AUTO)
    {
      /*si commande d extinction*/
      if (!strcmp(vm_pal.Scenario,C_SCN_EXTINCTION)) {
	if (vm_pal.District == va_district) {
          if (XZAC67_Commande_Fin_PAL(vm_pal.Identifiant,      /*numero equipement*/ 
				  vm_pal.District,         /*site du pal*/
				  va_horodate ,            /*horodate lancement fin*/
				  XDC_NON,                 /*manuelle*/
				  vm_pal.Scenario,         /*nom court du scenario*/
				  vm_pal.Nom_Scenario      /*nom long du scenario*/
                                  ) != XDC_OK)
	  {
            *pa_resultat = XDC_NOK;
            XZST_03EcritureTrace(XZSTC_WARNING,"drol_pal_recherche : pb appel a XZAC67_Commande_Fin_PAL %d / %d / %s / %s / %s",
				  vm_pal.Identifiant,vm_pal.District,va_horodate,vm_pal.Scenario,vm_pal.Nom_Scenario);
	  }
	}
        else {
          if (!TipcSrvMsgWrite(vl_dg,XDM_IdentMsg(XDM_EPAL_AUTO_FIN) ,
                                XDM_FLG_SRVMSGWRITE,
                                T_IPC_FT_INT2, vm_pal.Identifiant,
				T_IPC_FT_INT2, vm_pal.District,
				T_IPC_FT_REAL8, va_horodate,
				T_IPC_FT_INT2, XDC_NON,
				T_IPC_FT_STR, vm_pal.Scenario,
				T_IPC_FT_STR, vm_pal.Nom_Scenario,
                                NULL))
	  {
            *pa_resultat = XDC_NOK;
            XZST_03EcritureTrace(XZSTC_WARNING ,"drol_pal_recherche : TipcSrvMsgWrite XDM_EPAL_AUTO_FIN a echoue");
	  }
        }
      }
      else { /*pilotage d un scenario*/
	/*si le pal est sur le meme district*/
	if (vm_pal.District == va_district) {
          if (XZAC66_Commande_PAL(vm_noeud->NumEvt.numero, /*num evt*/
                              vm_noeud->NumEvt.cle,    /*cle evt*/
                              vg_num_poste,            /*poste operateur*/
                              vm_pal.Identifiant,      /*numero equipement*/
                              va_horodate ,            /*horodate lancement*/
                              XDC_NON,                   /*manuelle*/
                              vm_pal.Priorite,         /*priorite*/
                              vm_pal.DistanceEvt,      /*distance evt*/
                              vm_pal.Scenario,         /*nom court du scenario*/
                              vm_pal.Nom_Scenario,     /*nom long du scenario*/
                              vm_pal.District,         /*site du pal*/
                              va_district)             /*site d'origine de l'action*/
                               != XDC_OK)
	  {
            *pa_resultat = XDC_NOK;
            XZST_03EcritureTrace(XZSTC_WARNING,"drol_pal_recherche : pb appel a XZAC66_Commande_PAL");
	  }
	}
        else { /*sur districts differents, envoyer un msg a TACLI*/
          if (!TipcSrvMsgWrite(vl_dg,XDM_IdentMsg(XDM_EPAL_AUTO) ,
                                XDM_FLG_SRVMSGWRITE,
                                T_IPC_FT_INT4, vm_noeud->NumEvt.numero,
                                T_IPC_FT_INT2, vm_noeud->NumEvt.cle,
                                T_IPC_FT_INT2, vg_num_poste,
                                T_IPC_FT_INT2, vm_pal.Identifiant,
				T_IPC_FT_REAL8, va_horodate,
				T_IPC_FT_INT2, XDC_NON,
				T_IPC_FT_INT2, vm_pal.Priorite,
				T_IPC_FT_INT4, vm_pal.DistanceEvt,
				T_IPC_FT_STR, vm_pal.Scenario,
				T_IPC_FT_STR, vm_pal.Nom_Scenario,
				T_IPC_FT_INT2, vm_pal.District,
				T_IPC_FT_INT2, va_district,
                                NULL))
	  {
            *pa_resultat = XDC_NOK;
            XZST_03EcritureTrace(XZSTC_WARNING ,"drol_pal_recherche : TipcSrvMsgWrite XDM_EPAL_AUTO a echoue");
	  }
        }
      }
    }
    else { /*pilotage via le plan d'actions*/ 
      /*mise en base de la proposition PAL*/
      if (XZAP27_Ajout_Prop_PAL(vg_numero_plan,
			  vg_ordre_pal++,
			  vm_pal.District,
			  "",
			  vm_pal.Priorite,
			  vm_pal.Identifiant,
			  vl_explication, 
			  vl_cmd,
			  vm_pal.Scenario,
			  TRUE) != XDC_OK)
      {
        *pa_resultat = XDC_NOK;
        XZST_03EcritureTrace(XZSTC_WARNING,"drol_pal_recherche : pb appel a XZAP03_Ajout_Prop_PAL");
      }
    }

  return (XDC_OK);
}

