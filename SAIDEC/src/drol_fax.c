/* Fichier : $Id: drol_fax.c,v 1.36 2020/09/24 11:45:23 gesconf Exp $        $Revision: 1.36 $        $Date: 2020/09/24 11:45:23 $
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : DROL_FAX				*
* FICHIER : drol_fax.c				*
*************************************************
* DESCRIPTION :
* new tdpac
*************************************************
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.0
* JMG	28/02/03	numero et nom dans objet fmc deviation
* HILMARCHER  17/11/04  numero et nom dans objet fmc delestage DEM 345 v1.4
* JMG	16/02/05	ajout explication claire dans SAD_FAX (competence interlocuteurS) 1.5
* PNI	29/08/05	ajout cas sur brelle (pour la localisation) 1.6 DEM505
* JPL	14/05/07	Prise en compte fmc Vehicule en marche arriere (DEM 643) 1.8
* JPL	14/05/07	Prise en compte fmc Echangeur deconseille (DEM 645) 1.9
* LCL   09/07/07        Suppression DY
* JMG 12/12/07        : Ajout mail DEM718 1.11
* PNI	28/02/08	: Modifier le nom district par nom secteur DEM743 v1.12
* PNI	18/11/08	: Ajout de Personnel ESCOTA dans le paragraphe remarques pour les accidents DEM822 V1.13
* PCL	16/03/09	: ne plus mentionner le dr dans les fax police DEM822 V1.14
* JPL	31/03/09	Filtrage des evenements termines depuis un certain temps (DEM 411) 1.15
* JPL	04/05/09	Ne plus ajouter systematiquement une proposition de fax 1.16
* JMG	02/10/09	secto DEM887 1.17
* JPL	13/01/11	Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.18
* JPL	13/01/11	Migration architecture HP ia64 (DEM 971) : Zones des portions de texte 'static'  1.19
* PNI	07/06/11	vm_local passe � [1500] 1.20
* PNI	14/06/11	Init vl_message � vide dans drol_fax_genere  1.20 DEM985
* JMG	24/05/12 	linux 1.21
* VR	25/01/12    : Ajout Regulation (DEM/1016)
* JPL	12/07/12	Utilisation des noms de sortie "usager" et indicateurs de bifurcations (DEM 1033)  1.23
* PCL	02/05/13	CI -> CIT  1.25
* PNI	28/08/14	DEM 1094 maj numero de sortir avec la SortieUsager 1.26
* PNI	3/12/14		DEM 1115 maj lib fax -> Mail 1.28
* JMG	14/08/15	DEM1130 ordonnancement 1.29
* JPL	16/12/16	Donnees statiques et de taille suffisante (table districts, zone, scenario)  1.30
* JPL	18/11/16	Ajout niveau de bouchon et message temps de parcours (DEM 1198)  1.31
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.32
* JPL	20/03/17	Changement texte 'Niveau de bouchon' en 'Niveau de Perturbation'  1.33
* JPL	20/03/17	Prise en compte numero de rappel client pour depanneur (DEM 1192)  1.34
* JMG	30/08/18	SMS LOT25 DEM1301 1.35
* JPL	22/07/21	Gestion des libelles de site  1.37
* LCL	28/09/23 	: Adaptations CNA DEM-483	
* LCL	04/10/23  : Adaptations CNA DEM-483 : lecture site principal en base
************************************************/

#include "drol_fax.h"
#include "drol_caltdp.h"

#include "xzan37.h"


#define C_REP_MIG	"/produits/migrazur/appliSD/fichiers/bdc/"
#define C_MOD_FAX	C_REP_MIG "fax.txt"
#define C_MOD_SMS	C_REP_MIG "sms.txt"
#define C_MOD_FAX_POLICE	C_REP_MIG "fax_police.txt"

#define C_SEPARATEUR	"#"
#define C_DISTANCE_INFINIE	999999
#define ETDPC_TEXTE_TDP                 "%2dmn"
#define ETDPC_TEXTE_TDP_HOR             "%1dh%02d"

#define C_VAR_OBJET	"$objet"
#define C_VAR_AUTOROUTE "$autoroute"
#define C_VAR_AUTOROUTE_POLICE "$autoroute_police"
#define C_VAR_DEBUT	"$debut"
#define C_VAR_FIN	"$fin"
#define C_VAR_PR	"$pr"
#define C_VAR_SECTION    "$section"
#define C_VAR_SENS	"$sens"
#define C_VAR_VEHICULES	"$nombre_vehicules"
#define C_VAR_GRAVITE	"$gravite"
#define C_VAR_MATIERES	"$matieres"
#define C_VAR_RESTRICTIONS	"$restrictions"
#define C_VAR_VOIES_BLOQUEES	"$voies_bloquees"
#define C_VAR_DIFFICULTES	"$difficultes"
#define C_VAR_NIVEAU_BOUCHON	"$niveau_bouchon"
#define C_VAR_TEL_CLIENT	"$tel_client"
#define C_VAR_TDP	"$tdp"
#define C_VAR_REMARQUES	"$remarques"
#define C_VAR_CONSEQ1	"$conseq1"
#define C_VAR_CONSEQ2	"$conseq2"
#define C_VAR_TRAVERSEE	"$traversee"
#define C_VAR_TYPE	"$type"

#define C_VAR_MANIF     "$nb_manifestants"
#define C_VAR_SENS_POLICE      "$sens_police"
#define C_VAR_DIST_SORTIE_AMONT "$distance_sortie_amont"
#define C_VAR_NUM_SORTIE_AMONT  "$num_sortie_amont"
#define C_VAR_NOM_SORTIE_AMONT  "$nom_sortie_amont"
#define C_VAR_VIOLENCE          "$violence"
#define C_VAR_PEAGE             "$perception"
#define C_VAR_DR        "$directeur_reseau"
#define C_VAR_DEX       "$directeur_exploitation"
#define C_VAR_DG        "$direction_generale"
#define C_VAR_ZONE		"$zone"
#define C_VAR_SCENARIO	"$scenario"
#define C_VAR_TRANSITOIRE "$transitoire"


/* Delai maximal (secondes) de mention d'un evenement sur RTFM ou dans les fax apres sa terminaison */
#define C_DELAI_DIFFUSION	(15 * 60)



/* definition des variables globales */

T_NOEUD *vg_racine;
extern T_NOEUD *vg_B1[50];
extern T_NOEUD *vg_B2[50];
extern T_NOEUD *vg_B3[50];
extern T_NOEUD *vg_B4[50];
extern XZADT_Liste_Zdp                 *pg_zdp;

extern XDY_Booleen      vg_calcul_auto;
extern XDY_NomMachine	  NomMachine;
extern XDY_Mot	        vg_SitePrincipal;

/* definition des variables locales au module */

static T_Modele *pm_modele=NULL;
int	 vm_nb_lexemes = 0;
static T_Modele *pm_modele_police=NULL;
int	 vm_nb_lexemes_police = 0;
static T_Modele *pm_modeleSMS=NULL;
int      vm_nb_lexemesSMS=0;

static T_NOEUD *vm_noeud;
static char vm_pr[70];
static char vm_objet[80];
static char vm_local[1500];

static char *version = "$Id: drol_fax.c,v 1.36 2020/09/24 11:45:23 gesconf Exp $ : module" ;


/* definition des fonctions internes */
int drol_fax_charge(char *,T_Modele *, int *);
int drol_fax_remplit_modele(T_Modele *,int);

/*fonctions elementaires qui renvoient une chaine en fonction de la valeur de la variable*/
static char *drol_fax_objet();
static char *drol_fax_autoroute();
static char *drol_fax_autoroute_police();
static char *drol_fax_debut();
static char *drol_fax_fin();
static char *drol_fax_pr();
static char *drol_fax_section(); 
static char *drol_fax_sens();
static char *drol_fax_niveau_bouchon();
static char *drol_fax_vehicules();
static char *drol_fax_gravite();
static char *drol_fax_matieres();
static char *drol_fax_restrictions();
static char *drol_fax_voies_bloquees();
static char *drol_fax_difficultes();
static char *drol_fax_tel_client();
static char *drol_fax_tdp();
static char *drol_fax_remarques();
static char *drol_fax_nb_manifestants();
static char *drol_fax_dist_sortie_amont();
static char *drol_fax_num_sortie_amont();
static char *drol_fax_nom_sortie_amont();
static char *drol_fax_violence();
static char *drol_fax_perception();
static char *drol_fax_dr();
static char *drol_fax_dex();
static char *drol_fax_dg();
static char *drol_fax_sens_police();
static char *drol_fax_zone();
static char *drol_fax_scenario();
static char *drol_fax_transitoire();
static char *drol_fax_conseq1();
static char *drol_fax_conseq2();
static char *drol_fax_traversee();
static char *drol_fax_type();

static char *drol_fax_null();
extern char *drol_fax_transitoire_extern();



static char *drol_libelle_site(int va_site) {
  char *pl_lib_site = "<Inconnu>";

  switch(va_site) {
    case XDC_CI :
      pl_lib_site = "CENTRE INFORMATION TRAFIC";   break;
    case XDC_VC :
      pl_lib_site = "VAR COTE D'AZUR";   break;
    case XDC_DP :
      pl_lib_site = "DURANCE PROVENCE";   break;
    case XDC_CA :
      pl_lib_site = "ANGERS";   break;
    default :
      break;
  }

    return (pl_lib_site);
}

char *drol_fax_transitoire_extern() {
return (drol_fax_transitoire());
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_fax_init()

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
  int vl_nb_lexemes=0;
  int vl_nb_lexemesSMS=0;
  int vl_nb_lexemes_police=0;
  T_Modele *tl_lexemes;
  T_Modele *tl_lexemes_police;
  T_Modele *tl_lexemesSMS;
  int vl_i=0;

  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : drol_fax_init");

  /*chargement des fichiers de config fax*/
  tl_lexemes=calloc(255,sizeof(T_Modele));
  tl_lexemes_police=calloc(255,sizeof(T_Modele));
  tl_lexemesSMS=calloc(255,sizeof(T_Modele));


  drol_fax_charge(C_MOD_FAX,tl_lexemes,&vl_nb_lexemes);
  pm_modele = calloc (vl_nb_lexemes, sizeof(T_Modele));
  for (vl_i=0;vl_i<vl_nb_lexemes;vl_i++)
    strcpy(pm_modele[vl_i].Valeur,tl_lexemes[vl_i].Valeur);
  vm_nb_lexemes=vl_nb_lexemes;
  drol_fax_remplit_modele(pm_modele,vm_nb_lexemes);

  drol_fax_charge(C_MOD_FAX_POLICE,tl_lexemes_police,&vl_nb_lexemes_police);
  pm_modele_police = calloc (vl_nb_lexemes_police, sizeof(T_Modele));
  for (vl_i=0;vl_i<vl_nb_lexemes_police;vl_i++)
    strcpy(pm_modele_police[vl_i].Valeur,tl_lexemes_police[vl_i].Valeur);
  vm_nb_lexemes_police=vl_nb_lexemes_police;
  drol_fax_remplit_modele(pm_modele_police,vm_nb_lexemes_police);

  drol_fax_charge(C_MOD_SMS,tl_lexemesSMS,&vl_nb_lexemesSMS);
  pm_modeleSMS = calloc (vl_nb_lexemesSMS, sizeof(T_Modele));
  for (vl_i=0;vl_i<vl_nb_lexemesSMS;vl_i++)
    strcpy(pm_modeleSMS[vl_i].Valeur,tl_lexemesSMS[vl_i].Valeur);
  vm_nb_lexemesSMS=vl_nb_lexemesSMS;
  drol_fax_remplit_modele(pm_modeleSMS,vm_nb_lexemesSMS);

  free(tl_lexemes);
  free(tl_lexemes_police);
  free(tl_lexemesSMS);

  XZST_03EcritureTrace( XZSTC_WARNING,"drol_fax_init : vm_nb_lexemes %d, vm_nb_lexemes_police %d, vm_nb_lexemesSMS %d", vm_nb_lexemes, vm_nb_lexemes_police, vm_nb_lexemesSMS);
  return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config  fax
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_fax_charge(char *va_nom,T_Modele *ta_lexemes, int *va_nombre)

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
  char        vl_Lig[255];
  XDY_Entier vl_Resultat_out = 0;
  char	*vl_str;
  char *vl_ch;
  int	vl_lgr = 0;

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : drol_fax_charge (%s)",va_nom);

  if ( (pl_fp = fopen(va_nom, "r")) == NULL ) {
    XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !" ,va_nom );
    return(XDC_NOK);
  }
  
  /*memorisation de la structure du message*/
  *va_nombre=0;
  while ( ( vl_retcode = XZAO000_LireLigneFich ( pl_fp, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
  {
    if (vl_Resultat_out != XZAO_EOF )
    {
/*
      vl_lgr = strlen (vl_Lig);
      if ( (vl_lgr > 0)  &&  (vl_Lig[vl_lgr-1] == '\n') ) {
	vl_Lig[vl_lgr-1] = XDC_FIN_CHN;
      }
*/

      vl_str=vl_Lig; 
      while ((vl_ch=strtok(vl_str,C_SEPARATEUR))!=NULL)
      {
	strcpy(ta_lexemes[*va_nombre].Valeur,vl_ch);
	vl_str=NULL;
	XZST_03EcritureTrace(XZSTC_INFO,"drol_fax_charge : lexeme %s, indice %d",ta_lexemes[*va_nombre].Valeur,
					*va_nombre);
	(*va_nombre)++;
      }
    }
    else break;
  }
  fclose(pl_fp);
  XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  : drol_fax_charge");
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* remplissage de la liste chainee Modele (T_Lexemes)
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_fax_remplit_modele(T_Modele *va_modele,int va_taille)

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
  int vl_i = 0;

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : drol_fax_remplit_modele");

  for (vl_i=0;vl_i<va_taille;vl_i++)
  {
    if (!strcmp(va_modele[vl_i].Valeur,C_VAR_OBJET)){
      va_modele[vl_i].Fct = drol_fax_objet;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_AUTOROUTE)) {
      va_modele[vl_i].Fct = drol_fax_autoroute;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_AUTOROUTE_POLICE)) {
      va_modele[vl_i].Fct = drol_fax_autoroute_police;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_DEBUT)) {
      va_modele[vl_i].Fct = drol_fax_debut;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_FIN)) {
      va_modele[vl_i].Fct = drol_fax_fin;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_PR)) {
      va_modele[vl_i].Fct = drol_fax_pr;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_SECTION)) {
      va_modele[vl_i].Fct = drol_fax_section;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_SENS)) {
      va_modele[vl_i].Fct = drol_fax_sens;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_NIVEAU_BOUCHON)) {
      va_modele[vl_i].Fct = drol_fax_niveau_bouchon;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_VEHICULES)) {
      va_modele[vl_i].Fct = drol_fax_vehicules;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_GRAVITE)) {
      va_modele[vl_i].Fct = drol_fax_gravite;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_MATIERES)) {
      va_modele[vl_i].Fct = drol_fax_matieres;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_VOIES_BLOQUEES)) {
      va_modele[vl_i].Fct = drol_fax_voies_bloquees;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_RESTRICTIONS)) {
      va_modele[vl_i].Fct = drol_fax_restrictions;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_DIFFICULTES)) {
      va_modele[vl_i].Fct = drol_fax_difficultes;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_TEL_CLIENT)) {
      va_modele[vl_i].Fct = drol_fax_tel_client;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_TDP)) {
      va_modele[vl_i].Fct = drol_fax_tdp;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_REMARQUES)) {
      va_modele[vl_i].Fct = drol_fax_remarques;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_CONSEQ1)) {
      va_modele[vl_i].Fct = drol_fax_conseq1;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_CONSEQ2)) {
      va_modele[vl_i].Fct = drol_fax_conseq2;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_TRAVERSEE)) {
      va_modele[vl_i].Fct = drol_fax_traversee;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_TYPE)) {
      va_modele[vl_i].Fct = drol_fax_type;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_SENS_POLICE)) {
      va_modele[vl_i].Fct = drol_fax_sens_police;
      strcpy(va_modele[vl_i].Valeur, "");
    }
	else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_ZONE)) {
      va_modele[vl_i].Fct = drol_fax_zone;
      strcpy(va_modele[vl_i].Valeur, "");
    }
	else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_SCENARIO)) {
      va_modele[vl_i].Fct = drol_fax_scenario;
      strcpy(va_modele[vl_i].Valeur, "");
    }
	else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_TRANSITOIRE)) {
      va_modele[vl_i].Fct = drol_fax_transitoire;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_MANIF)) {
      va_modele[vl_i].Fct = drol_fax_nb_manifestants;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_DIST_SORTIE_AMONT)) {
      va_modele[vl_i].Fct = drol_fax_dist_sortie_amont;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_NUM_SORTIE_AMONT)) {
      va_modele[vl_i].Fct = drol_fax_num_sortie_amont;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_NOM_SORTIE_AMONT)) {
      va_modele[vl_i].Fct = drol_fax_nom_sortie_amont;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_VIOLENCE)) {
      va_modele[vl_i].Fct = drol_fax_violence;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_PEAGE)) {
      va_modele[vl_i].Fct = drol_fax_perception;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_DR)) {
      va_modele[vl_i].Fct = drol_fax_dr;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_DEX)) {
      va_modele[vl_i].Fct = drol_fax_dex;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_DG)) {
      va_modele[vl_i].Fct = drol_fax_dg;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    /*si ce n'est pas une variable*/
    else
      va_modele[vl_i].Fct = drol_fax_null;
  }
  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : drol_fax_remplit_modele");
  return (XDC_OK);
}


/*****************************************************************/
/*****************************************************************/
/*****************************************************************/
static char *drol_fax_null() { return (""); }

/*JMG 28/02/03 */
/*fax deviation : indiquer numero et nom de la sortie*/
/*DEM 65*/
static char *drol_fax_objet() 
{ 
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_objet");
  if (vm_noeud->Type == XZAEC_FMC_FermetureEch)
    sprintf(vm_objet,"OBJET : ECHANGEUR %s %s FERME (%d)",
		vm_noeud->Localisation.SortieUsager,
		vm_noeud->Localisation.NomPointCar,
		vm_noeud->NumEvt.numero);
  else if (vm_noeud->Type == XZAEC_FMC_EchDeconseille)
    sprintf(vm_objet,"OBJET : ECHANGEUR %s %s DECONSEILLE (%d)",
		vm_noeud->Localisation.SortieUsager,
		vm_noeud->Localisation.NomPointCar,
		vm_noeud->NumEvt.numero);
  else if (vm_noeud->Type == XZAEC_FMC_AireFermee)
    sprintf(vm_objet,"OBJET : AIRE %s FERMEE (%d)",
		vm_noeud->Localisation.NomPointCar,
		vm_noeud->NumEvt.numero);
  else if (vm_noeud->Type == XZAEC_FMC_Deviation)
    sprintf(vm_objet,"OBJET : SORTIE %s %s OBLIGATOIRE (%d)",
		vm_noeud->Localisation.SortieUsager,
		vm_noeud->Localisation.NomPointCar,
		vm_noeud->NumEvt.numero);
/*HILMARCHER 17/11/04 */
/*fax delestage: indiquer numero et nom echangeur*/
/*DEM 345*/
/*DEM 1094 : maj numero de sortire avec la SortieUsager */
  else if (vm_noeud->Type == XZAEC_FMC_Delestage)
    sprintf(vm_objet,"OBJET : SORTIE %s %s CONSEILLEE (%d)",
		vm_noeud->Localisation.SortieUsager,
		vm_noeud->Localisation.NomPointCar,
		vm_noeud->NumEvt.numero);
  else if (vm_noeud->Type == XZAEC_FMC_Regulation)
    sprintf(vm_objet,"OBJET : Regulation de vitesse (%d)",
    		vm_noeud->NumEvt.numero);
  else
    sprintf(vm_objet,"OBJET : %s (%d)",
		vm_noeud->Infos_FAX.Objet,
		vm_noeud->NumEvt.numero);
  return (vm_objet);
}

static char *drol_fax_autoroute() { 
  static char vl_str[100];
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_autoroute");
  sprintf(vl_str,"AUTOROUTE : %s",vm_noeud->Localisation.NomAutoroute); 
  return (vl_str);
}

static char *drol_fax_autoroute_police() { 
  static char vl_str[100];
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_autoroute_police");
  strcpy(vl_str,vm_noeud->Localisation.NomAutoroute); 
  return (vl_str);
}

static char *drol_fax_debut() { 
  static char vl_str[100];
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_debut");
  sprintf(vl_str,"HEURE DE DEBUT D'EVENEMENT : %s",vm_noeud->Infos_TFM.Debut);
  return (vl_str);
}

static char *drol_fax_type() {
  static char vl_str[100];
  strcpy(vl_str,vm_noeud->Infos_TFM.Groupe_Nominal);
  return (vl_str);
}

static char *drol_fax_traversee() {
  static char vl_str[100];
  XDY_TDP vl_tdp;
  XDY_PR vl_pr_min, vl_pr_max;
  XZADT_Liste_Zdp       *pl_liste_zdp;

  if (vm_noeud->Type != XZAEC_FMC_QueueBouchon)
    return ("");

  vl_pr_max = vm_noeud->Localisation.PR > vm_noeud->Localisation_Tete.PR ? vm_noeud->Localisation.PR : vm_noeud->Localisation_Tete.PR;
  vl_pr_min = vm_noeud->Localisation.PR <= vm_noeud->Localisation_Tete.PR ? vm_noeud->Localisation.PR : vm_noeud->Localisation_Tete.PR;
  
  pl_liste_zdp = pg_zdp;
  while (pl_liste_zdp != NULL) {
	XZST_03EcritureTrace(   XZSTC_FONCTION, "zdp %d",pl_liste_zdp->zdp.Numero);
  	pl_liste_zdp = pl_liste_zdp->suivant;
  }
  if (XZAD30_Calculer_Traversee_Bouchon(vm_noeud->Localisation.Autoroute,
  				vm_noeud->Localisation.Sens,
				vl_pr_min,
				vl_pr_max,
				pg_zdp,
				&vl_tdp) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_fax_traversee : pb appel a XZAD30_Calculer_Traversee_Bouchon");
    return ("");
  }
  if (vl_tdp <= 0)
    return ("");

strcpy(vl_str,"TRAVERSEE :");
   if ( vl_tdp > 600 ) {
     return ("");
   }
   else if ( vl_tdp > 59 )
   {
        sprintf( &vl_str[11], ETDPC_TEXTE_TDP_HOR, vl_tdp/60, vl_tdp%60 );
       vl_str[15] = '\0';
   }
   else  if ( vl_tdp > 0 ) {
              sprintf ( &vl_str[11], ETDPC_TEXTE_TDP, vl_tdp );
           vl_str[15] = '\0';
  }
   else {
        strcpy(vl_str,"");
   }
  return (vl_str);

}
static char *drol_fax_fin() { 
  static char vl_str[100];
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_fin");
  if (!(vm_noeud->Infos_TFM.Termine)) return("");

  sprintf(vl_str,"**** TERMINE **** HEURE DE FIN : %s\n",
		vm_noeud->Infos_TFM.Date_Fin);
  return (vl_str); 
}
static char *drol_fax_conseq1() {
  static char vl_str[100];
  T_NOEUD         *vl_noeud;

  vl_noeud = vg_racine;
  strcpy(vl_str,"");
  while (vl_noeud !=NULL) {
    XZST_03EcritureTrace(   XZSTC_FONCTION, " drol_fax_conseq1 : %d %d pour %d %d",
    				vl_noeud->Infos_FAX.Numero_Cause, 
				vl_noeud->Infos_FAX.Cle_Cause,
				vm_noeud->NumEvt.numero,
				vm_noeud->NumEvt.cle);
    if ((vl_noeud->Infos_FAX.Numero_Cause == vm_noeud->NumEvt.numero) &&
       (vl_noeud->Infos_FAX.Cle_Cause == vm_noeud->NumEvt.cle)) {
	/*consequence 1 trouvee*/
        sprintf (vl_str,"Consequence trafic : %s (%d/%d), %s au PR %3.3f\n",
				vl_noeud->Infos_TFM.Groupe_Nominal,
				vl_noeud->NumEvt.numero,
				vl_noeud->NumEvt.cle,
				vl_noeud->Infos_TFM.Autoroute,
				(float)(vl_noeud->Localisation.PR)/1000);
         return (vl_str);
    }
    vl_noeud=vl_noeud->Suivant;
  }
  return (vl_str);
}
static char *drol_fax_conseq2() {
  static char vl_str[100];
  T_NOEUD         *vl_noeud;
  int vl_indice=1;

  vl_noeud = vg_racine;
  strcpy(vl_str,"");
  while (vl_noeud !=NULL) {
    XZST_03EcritureTrace(   XZSTC_FONCTION, " drol_fax_conseq2 : %d %d pour %d %d",
    				vl_noeud->Infos_FAX.Numero_Cause, 
				vl_noeud->Infos_FAX.Cle_Cause,
				vm_noeud->NumEvt.numero,
				vm_noeud->NumEvt.cle);
    if ((vl_noeud->Infos_FAX.Numero_Cause == vm_noeud->NumEvt.numero) &&
       (vl_noeud->Infos_FAX.Cle_Cause == vm_noeud->NumEvt.cle)) {
	/*consequence 1 trouvee*/
	if (vl_indice ==2) {
          sprintf (vl_str,"Consequence trafic : %s (%d/%d), %s au PR %3.3f\n",
				vl_noeud->Infos_TFM.Groupe_Nominal,
				vl_noeud->NumEvt.numero,
				vl_noeud->NumEvt.cle,
				vl_noeud->Infos_TFM.Autoroute,
				(float)(vl_noeud->Localisation.PR)/1000);
           return (vl_str);
        }
	vl_indice=2;
    }
    vl_noeud=vl_noeud->Suivant;
  }

  return (vl_str);
}

static char *drol_fax_pr() { 
  int vl_km;
  int vl_m;

  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_pr");
  XZST_03EcritureTrace(   XZSTC_WARNING,"%d %d",
  			vm_noeud->Localisation.PR,
			vm_noeud->Localisation_Tete.PR);
  vl_km = (int)(vm_noeud->Localisation.PR/1000);
  vl_m = (int)(((vm_noeud->Localisation.PR)-((int)(vm_noeud->Localisation.PR/1000)*1000))/100);

  if (vl_m==0)
    sprintf(vm_pr,"PR : %d (%s)",
	vl_km,	
	drol_libelle_site(vm_noeud->Localisation.Site)); 
  else
    sprintf(vm_pr,"PR : %d.%d (%s)",
		vl_km,
		vl_m,
		drol_libelle_site(vm_noeud->Localisation.Site));
  return (vm_pr); 
}

static char *drol_fax_sens() { 
  static char vl_str[100];
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_sens");
  sprintf(vl_str,"SENS : %s",vm_noeud->Infos_FAX.Sens); 
  return (vl_str);
}

static char *drol_fax_sens_police() { 
  static char vl_str[100];
  strcpy(vl_str,vm_noeud->Infos_FAX.Sens); 
  return (vl_str);
}

static char *drol_fax_vehicules() { 
  static char vl_str[200];
  char vl_type[50];

  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_vehicules");
  if ( (vm_noeud->Type==XZAEC_FMC_VehArrete) ||
       (vm_noeud->Type==XZAEC_FMC_VehPanne) ||
       (vm_noeud->Type==XZAEC_FMC_VehFeu) ||
       (vm_noeud->Type==XZAEC_FMC_VehMarcheArr)) {
    if (vm_noeud->Infos_FAX.Type_Vehicule==XZAEC_VEH_PL)
      strcpy(vl_type,"PL");
    else if (vm_noeud->Infos_FAX.Type_Vehicule==XZAEC_VEH_VL)
      strcpy(vl_type,"VL");
    else if (vm_noeud->Infos_FAX.Type_Vehicule==XZAEC_VEH_Car)
      strcpy(vl_type,"CAR");
    else if (vm_noeud->Infos_FAX.Type_Vehicule==XZAEC_VEH_Moto)
      strcpy(vl_type,"MOTO");
    else if (vm_noeud->Infos_FAX.Type_Vehicule==XZAEC_VEH_Remorque)
      strcpy(vl_type,"REMORQUE");
    else
      strcpy(vl_type,"VL");
    sprintf(vl_str,"NOMBRE DE VEHICULES : 1 %s\n", vl_type);
    return (vl_str);
  }

  if (vm_noeud->Type==XZAEC_FMC_Accident) {
    strcpy(vl_str,"NOMBRE DE VEHICULES :");
    if (vm_noeud->Infos_FAX.Nb_VL>0) {
      sprintf(vl_type,"%d VL,",vm_noeud->Infos_FAX.Nb_VL);
      strcat(vl_str,vl_type);
    }
    if (vm_noeud->Infos_FAX.Nb_PL>0) {
      sprintf(vl_type,"%d PL,",vm_noeud->Infos_FAX.Nb_PL);
      strcat(vl_str,vl_type);
    }
    if (vm_noeud->Infos_FAX.Nb_Motos>0) {
      sprintf(vl_type,"%d MOTOS,",vm_noeud->Infos_FAX.Nb_Motos);
      strcat(vl_str,vl_type);
    }
    if (vm_noeud->Infos_FAX.Nb_Cars>0) {
      sprintf(vl_type,"%d CARS,",vm_noeud->Infos_FAX.Nb_Cars);
      strcat(vl_str,vl_type);
    }
    if (vm_noeud->Infos_FAX.Nb_Remorques>0) {
      sprintf(vl_type,"%d REMORQUES,",vm_noeud->Infos_FAX.Nb_Remorques);
      strcat(vl_str,vl_type);
    }
    strcat(vl_str,"\n");
    return(vl_str);
  }

  return (""); 
}

static char *drol_fax_gravite() { 
  static char vl_str[90];
  char vl_ch[50];
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_gravite");
  if (vm_noeud->Type!=XZAEC_FMC_Accident) return (""); 

  if (vm_noeud->Infos_FAX.Nb_Victimes==0) return ("GRAVITE : ACCIDENT MATERIEL\n");

  strcpy(vl_str,"GRAVITE : ");
  if (vm_noeud->Infos_FAX.Nb_Victimes-vm_noeud->Infos_FAX.Nb_BG-vm_noeud->Infos_FAX.Nb_Morts>0) {
    sprintf(vl_ch,"%d BLESSE(S) LEGER(S),",
	vm_noeud->Infos_FAX.Nb_Victimes-vm_noeud->Infos_FAX.Nb_BG-vm_noeud->Infos_FAX.Nb_Morts);
    strcat(vl_str,vl_ch);
  }
  if (vm_noeud->Infos_FAX.Nb_BG>0) {
    sprintf(vl_ch,"%d BLESSE(S) GRAVE(S)",
		vm_noeud->Infos_FAX.Nb_BG);
    strcat(vl_str,vl_ch);
  }
  if (vm_noeud->Infos_FAX.Nb_Morts>0) {
    sprintf(vl_ch,"%d MORT(S)",vm_noeud->Infos_FAX.Nb_Morts);
    strcat(vl_str,vl_ch);
  }
  strcat(vl_str,"\n");
  return(vl_str);
}

static char *drol_fax_matieres() { 
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_matieres");
  if (vm_noeud->Type!=XZAEC_FMC_Accident) return (""); 

  if (vm_noeud->Infos_FAX.Matieres_Dangereuses)
    return("PRESENCE DE MATIERES DANGEREUSES\n");

  return("");
}

static char *drol_fax_niveau_bouchon() {
  static char vl_str[40];
  char *pl_lib_type;
  /*XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_niveau_bouchon");*/
  if (vm_noeud->Type!=XZAEC_FMC_QueueBouchon) return (""); 

  switch(vm_noeud->Infos_FAX.Type_Bouchon) {
    case XZAEC_BOU_Dense :
      pl_lib_type = XZAEC_BOU_LIB_Dense;   break;
    case XZAEC_BOU_Ralenti :
      pl_lib_type = XZAEC_BOU_LIB_Ralenti;   break;
    case XZAEC_BOU_Bouche :
      pl_lib_type = XZAEC_BOU_LIB_Bouche;   break;
    case XZAEC_BOU_Bloque :
      pl_lib_type = XZAEC_BOU_LIB_Bloque;   break;
    case XZAEC_BOU_Escargot :
      pl_lib_type = XZAEC_BOU_LIB_Escargot;   break;
    case XZAEC_BOU_Accordeon :
      pl_lib_type = XZAEC_BOU_LIB_Accordeon;   break;
    default :
      return ("");
  }

  sprintf(vl_str, "NIVEAU DE PERTURBATION : %s\n", pl_lib_type); 
  return(vl_str);
}

static char *drol_fax_tdp() {
  static char vl_str[200] = "";
  if (NULL != strstr(vm_noeud->Infos_TFM.TDP, "INVALIDE"))
    return (""); 

  strcpy (vl_str, vm_noeud->Infos_TFM.TDP);
  return (vl_str); 
}

static char *drol_fax_voies_bloquees() { 
  T_Localisation vl_loc;
  static char  vl_str[150];

  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_voies_bloquees");
  vl_loc=vm_noeud->Localisation;

  if (vm_noeud->Infos_TFM.Termine) return("");
  if ( (vl_loc.VL!=XDC_VOIE_BLOQUEE) &&
       (vl_loc.VM1!=XDC_VOIE_BLOQUEE) &&
       (vl_loc.VM2!=XDC_VOIE_BLOQUEE) &&
       (vl_loc.VR!=XDC_VOIE_BLOQUEE) &&
       (vl_loc.VL!=XDC_VOIE_BLOQUEE_NEUTRALISEE) &&
       (vl_loc.VM1!=XDC_VOIE_BLOQUEE_NEUTRALISEE) &&
       (vl_loc.VM2!=XDC_VOIE_BLOQUEE_NEUTRALISEE) &&
       (vl_loc.VR!=XDC_VOIE_BLOQUEE_NEUTRALISEE)
     ) return ("");

  if ( (vl_loc.VL!=XDC_VOIE_SENS_NORMAL) &&
       (vl_loc.VM1!=XDC_VOIE_SENS_NORMAL) &&
       (vl_loc.VM2!=XDC_VOIE_SENS_NORMAL) &&
       (vl_loc.VR!=XDC_VOIE_SENS_NORMAL)
     )
     return ("POSITION DES VEHICULES : SUR TOUTES LES VOIES\n");
 
  strcpy(vl_str,"POSITION DES VEHICULES : ");
  if ((vl_loc.VL==XDC_VOIE_BLOQUEE) || (vl_loc.VL==XDC_VOIE_BLOQUEE_NEUTRALISEE))
	strcat(vl_str,"_SUR LA VOIE LENTE ");
  if ((vl_loc.VM1==XDC_VOIE_BLOQUEE) || (vl_loc.VM1==XDC_VOIE_BLOQUEE_NEUTRALISEE))
	strcat(vl_str,"_SUR LA VOIE MEDIANE ");
  if ((vl_loc.VM2==XDC_VOIE_BLOQUEE) || (vl_loc.VM2==XDC_VOIE_BLOQUEE_NEUTRALISEE))
  	strcat(vl_str,"_SUR LA VOIE MEDIANE GAUCHE");
  if ((vl_loc.VR==XDC_VOIE_BLOQUEE) || (vl_loc.VR==XDC_VOIE_BLOQUEE_NEUTRALISEE))
	strcat(vl_str,"_SUR LA VOIE RAPIDE");
  strcat(vl_str,".\n");
  return (vl_str);
}

static char *drol_fax_restrictions() { 
  T_Localisation vl_loc;
  static char  vl_str[150];
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_restrictions");

  if (vm_noeud->Infos_TFM.Termine) return("");
  vl_loc=vm_noeud->Localisation;

  if ( (vl_loc.VL!=XDC_VOIE_NEUTRALISEE) &&
       (vl_loc.VM1!=XDC_VOIE_NEUTRALISEE) &&
       (vl_loc.VM2!=XDC_VOIE_NEUTRALISEE) &&
       (vl_loc.VR!=XDC_VOIE_NEUTRALISEE)

     ) return (""); 

  if ( (vl_loc.VL==XDC_VOIE_SENS_NORMAL) &&
       ( (vl_loc.VM1==XDC_VOIE_SENS_NORMAL) ||
	 (vl_loc.VM1==XDC_VOIE_INEXISTANTE)
       ) &&
       ( (vl_loc.VM2==XDC_VOIE_SENS_NORMAL) ||
	 (vl_loc.VM2==XDC_VOIE_INEXISTANTE)
       ) &&
       ( (vl_loc.VR==XDC_VOIE_SENS_NORMAL) ||
	 (vl_loc.VR==XDC_VOIE_INEXISTANTE)
       )
     ) return ("PAS DE RESTRICTIONS DE CIRCULATIONS\n");

  strcpy(vl_str,"RESTRICTIONS DE CIRCULATIONS : NEUTRALISATION ");
  if (vl_loc.VL==XDC_VOIE_NEUTRALISEE) strcat(vl_str,"VOIE LENTE");
  if (vl_loc.VM1==XDC_VOIE_NEUTRALISEE) strcat(vl_str," VOIE MEDIANE");
  if (vl_loc.VM2==XDC_VOIE_NEUTRALISEE) strcat(vl_str," VOIE MEDIANE GAUCHE");
  if (vl_loc.VR==XDC_VOIE_NEUTRALISEE) strcat(vl_str," VOIE RAPIDE");
  strcat(vl_str,"\n");
  return (vl_str);
}

static char *drol_fax_difficultes() { 
  static char vl_str[100];
  int vl_voies=0;
  char vl_longueur[15];

  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_difficultes");
  if (vm_noeud->Infos_TFM.Termine) return("");

  if (vm_noeud->Localisation.Longueur>=1000)
   sprintf(vl_longueur,"%d KM",
		(int)((vm_noeud->Localisation.Longueur+500)/1000));
  else
    sprintf(vl_longueur,"%d M",
		vm_noeud->Localisation.Longueur);

  if ( (vm_noeud->Type==XZAEC_FMC_QueueBouchon) &&
       (vm_noeud->Localisation_Tete.TypePointCar!=XDC_POINT_CARACT_ECHANGEUR)
     ) {
     if (vm_noeud->Localisation.VL==XDC_VOIE_BLOQUEE) vl_voies++;
     if (vm_noeud->Localisation.VM1==XDC_VOIE_BLOQUEE) vl_voies++;
     if (vm_noeud->Localisation.VM2==XDC_VOIE_BLOQUEE) vl_voies++;
     if (vm_noeud->Localisation.VR==XDC_VOIE_BLOQUEE) vl_voies++;
     sprintf(vl_str,"DIFFICULTES DE CIRCULATION : BOUCHON DE %s SUR %d VOIES\n",
		vl_longueur,
		vl_voies);
     return (vl_str);
  }

  if ( (vm_noeud->Type==XZAEC_FMC_QueueBouchon) &&
       (vm_noeud->Localisation_Tete.TypePointCar==XDC_POINT_CARACT_ECHANGEUR)
     ) {
    sprintf(vl_str,"DIFFICULTES DE CIRCULATION : BOUCHON DE %s\n",vl_longueur);
    return (vl_str);
  }
  return ("");
}

static char *drol_fax_tel_client() {
  static char vl_str[40];
  char *pl_tel_client = vm_noeud->Infos_FAX.Complement_Rappel;
  int vl_nb_chiffres = 0;

  if ( (vm_noeud->Type!=XZAEC_FMC_VehArrete) &&
       (vm_noeud->Type!=XZAEC_FMC_VehPanne) &&
       (vm_noeud->Type!=XZAEC_FMC_VehFeu) )
  {
    return ("");
  }
  if (vm_noeud->Infos_TFM.Termine) return("");

  while (*pl_tel_client != XDC_FIN_CHN)
  {
    if ( (*pl_tel_client >= '0')  &&  (*pl_tel_client <= '9') )
      vl_nb_chiffres = vl_nb_chiffres + 1;
  }

  if (vl_nb_chiffres < 6) return("");

  sprintf(vl_str, "TEL CLIENT : %s\n", pl_tel_client); 
  return(vl_str);
}

static char *drol_fax_remarques() { 
  static char vl_str[100];

  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_remarques");
  if (vm_noeud->Type==XZAEC_FMC_Accident) {
    if (vm_noeud->Infos_FAX.Agent_Escota) {
      sprintf(vl_str,"Presence de personnel ESCOTA");
      return (vl_str);
    }
   }
   return (""); 
}

static char *drol_fax_zone() { 
  static char vl_str[120];

  if (vm_noeud->Type == XZAEC_FMC_Regulation) {
 	sprintf(vl_str,"ZONE : %s",vm_noeud->Infos_FAX.Zone);
  	return (vl_str);
  }
  else
      return ("");
}

static char *drol_fax_scenario() { 
  static char vl_str[120];

  if (vm_noeud->Type == XZAEC_FMC_Regulation) {
  	sprintf(vl_str,"SCENARIO : %s",vm_noeud->Infos_FAX.Scenario);
  	return (vl_str);
  }
  else
    return ("");
}

static char *drol_fax_transitoire() { 
  static char vl_str[100];
  char *vl_str2;
  char vl_str3[50];
  char vl_str4[500];
  float vl_pr;
  int vl_vitesse;

  float tl_pr[10];
  int tl_vitesse[10];
  int index=0;

  if (vm_noeud->Type != XZAEC_FMC_Regulation) 
    return ("");

  /* 56.0(130km/h) 51.0(130km/h) 40.0(130km/h) 36.0(130km/h) 29.0(130km/h)*/
  XZST_03EcritureTrace(   XZSTC_WARNING,"transitoire %s",vm_noeud->Infos_FAX.Transitoire);

  if (strlen(vm_noeud->Infos_FAX.Transitoire)==0)
    return ("");

  sscanf(vm_noeud->Infos_FAX.Transitoire,"%f(%dkm/h)", &vl_pr,&vl_vitesse);
  XZST_03EcritureTrace(   XZSTC_WARNING,"vitesse %d PR %.1f",vl_vitesse,vl_pr);

  for (index=1; index <10; index++) {
	tl_pr[index]=0;
	tl_vitesse[index]=0;
  }
  index=0;
  tl_pr[index]=vl_pr;
  tl_vitesse[index]=vl_vitesse;
  strcpy(vl_str4,vm_noeud->Infos_FAX.Transitoire);
  vl_str2=vl_str4;
  sprintf(vl_str3,"%.1f(%dkm/h)",vl_pr,vl_vitesse);
  vl_str2 = vl_str2+strlen(vl_str3)+1;

  while (strlen(vl_str2) >0) {
  sscanf(vl_str2,"%f(%dkm/h)", &vl_pr,&vl_vitesse);
  sprintf(vl_str3,"%.1f(%dkm/h)",vl_pr,vl_vitesse);

  index++;
  tl_pr[index]=vl_pr;
  tl_vitesse[index]=vl_vitesse;
  XZST_03EcritureTrace(   XZSTC_WARNING,"vitesse %d PR %.1f",tl_vitesse[index],tl_pr[index]);

  vl_str2 = vl_str2+strlen(vl_str3)+1;
  XZST_03EcritureTrace(   XZSTC_WARNING,"%s",vl_str2);
  }

  strcpy(vl_str4,"");
  for (index=0; index <10, tl_vitesse[index]>0; index++) {
  if (tl_pr[index+1]==0)
    tl_pr[index+1]=(float)(vm_noeud->Localisation_Tete.PR)/1000.0;

  XZST_03EcritureTrace(   XZSTC_WARNING,"(%d) vitesse %d PR %.1f",index, tl_vitesse[index],tl_pr[index]);
	sprintf(vl_str3,"PR %.1f a %.1f : %d km/h\n",tl_pr[index], tl_pr[index+1],tl_vitesse[index]);
	strcat(vl_str4,vl_str3);
	}

  sprintf(vl_str,"SEQUENCE : %s",vl_str4);
  return (vl_str);
}

static char *drol_fax_nb_manifestants() {
  static char vl_str[100];

  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_manifestants");
  sprintf(vl_str,"%d",vm_noeud->Infos_FAX.Nb_Manifestants);
  return (vl_str);
}

static char *drol_fax_violence() {
  static char vl_str[100];
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_violence");
  if (vm_noeud->Infos_FAX.Violence) 
    strcpy(vl_str, "AVEC");
  else
    strcpy(vl_str, "SANS");
  return (vl_str);
}

static char *drol_fax_perception() {
  static char vl_str[100];
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_perception");
  if (vm_noeud->Infos_FAX.Perception_Peage) 
    strcpy(vl_str, "SANS");
  else
    strcpy(vl_str, "AVEC");
  return (vl_str);
}

static char *drol_fax_dist_sortie_amont() {
  static char vl_str[100];
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_dist_sortie_amont");
  if (vm_noeud->Localisation.DistanceSortieAmont != C_DISTANCE_INFINIE)
    sprintf(vl_str,"%d KM", (int)(vm_noeud->Localisation.DistanceSortieAmont/1000));
  else
    strcpy(vl_str,"");
  return (vl_str);
}

static char *drol_fax_nom_sortie_amont() {
  static char vl_str[100];
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_nom_sortie_amont");
  if (vm_noeud->Localisation.DistanceSortieAmont != C_DISTANCE_INFINIE)
    strcpy(vl_str,vm_noeud->Localisation.NomSortieAmont);
  else
    strcpy(vl_str,"");
  return (vl_str);
}

static char *drol_fax_num_sortie_amont() {
  static char vl_str[100];
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_num_sortie_amont");
  if (vm_noeud->Localisation.DistanceSortieAmont != C_DISTANCE_INFINIE)
    sprintf(vl_str,"%s",vm_noeud->Localisation.SortieAmontUsager);
  else
    strcpy(vl_str,"");
  return (vl_str);
}

static char *drol_fax_dr() {
  static char vl_str[100];
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_dr");
  strcpy(vl_str,"");
  XZAN37_Nom_Fonction(2,vl_str);
  return (vl_str);
}

static char *drol_fax_dex() {
  static char vl_str[100];
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_dex");
  strcpy(vl_str,"");
  XZAN37_Nom_Fonction(1,vl_str);
  return (vl_str);
}

static char *drol_fax_dg() {
  static char vl_str[100];
  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_dg");
  strcpy(vl_str,"");
  XZAN37_Nom_Fonction(33,vl_str);
  return (vl_str);
}

/********************************************************************/
/********************************************************************/
/********************************************************************/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de generation du fax elementaire pour un evt
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *drol_fax_genere(T_NOEUD *va_noeud)

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
  static char vl_message[1500]="";
  char vl_i=0;
  char * (*vl_fct)();
  char vl_date[30];
  int vl_jour,vl_mois,vl_annee,vl_heure,vl_minute,vl_seconde;
  char vl_str[30];

  vm_noeud = va_noeud;

  /*Init vl_message � vide */
  vl_message[0]='\0';
  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :drol_fax_genere %d %d, type %d",
				va_noeud->NumEvt.numero,
				va_noeud->NumEvt.cle,
				vm_noeud->Type);

  /*concatene tous les elements de messages*/
  for (vl_i=0;vl_i<vm_nb_lexemes;vl_i++) {
    strcat(vl_message,pm_modele[vl_i].Valeur);  
    vl_fct = pm_modele[vl_i].Fct;
    if (NULL != vl_fct) {
      strcat(vl_message,(*vl_fct)());
    }
  }
  return (vl_message);
}



int drol_fax_lexemes()
{
return vm_nb_lexemes;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de generation du fax police pour une manif
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *drol_fax_genere_police(T_NOEUD *va_noeud)

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
  static char vl_message[1000]="";
  char vl_i=0;
  char * (*vl_fct)();
  char vl_date[30];
  int vl_jour,vl_mois,vl_annee,vl_heure,vl_minute,vl_seconde;
  char vl_str[30];

  vm_noeud = va_noeud;

  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :drol_fax_genere_police %d %d, type %d",
				va_noeud->NumEvt.numero,
				va_noeud->NumEvt.cle,
				vm_noeud->Type);

  /*concatene tous les elements de messages*/
  for (vl_i=0;vl_i<vm_nb_lexemes_police;vl_i++) {
    vl_fct = pm_modele_police[vl_i].Fct;
    strcat(vl_message,pm_modele_police[vl_i].Valeur);
    strcat(vl_message,(*vl_fct)());
  }
  XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT drol_fax_genere_police, MSG = %s",vl_message);
  return (vl_message);
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de generation du SMS
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *drol_fax_genere_SMS(T_NOEUD *va_noeud)

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
  static char vl_message[1000]="";
  char vl_i=0;
  char * (*vl_fct)();
  char vl_date[30];
  int vl_jour,vl_mois,vl_annee,vl_heure,vl_minute,vl_seconde;
  char vl_str[30];

  vm_noeud = va_noeud;

  strcpy(vl_message,"");
  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :drol_fax_genere_SMS %d %d, type %d",
				va_noeud->NumEvt.numero,
				va_noeud->NumEvt.cle,
				vm_noeud->Type);

  /*concatene tous les elements de messages*/
  for (vl_i=0;vl_i<vm_nb_lexemesSMS;vl_i++) {
    vl_fct = pm_modeleSMS[vl_i].Fct;
    strcat(vl_message,pm_modeleSMS[vl_i].Valeur);
    strcat(vl_message,(*vl_fct)());
  }
  XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT drol_fax_genere_SMS, MSG = %s",vl_message);
  return (vl_message);
}

static char *drol_fax_section()
{
  T_Localisation vl_loc;
  T_Localisation vl_loc_tete;
  char  vl_str[150];

  vl_loc=vm_noeud->Localisation;
  vl_loc_tete=vm_noeud->Localisation_Tete;

  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_section: SAM %d,BAM %d,PAM %d, BAV %d",
				vl_loc.DistanceSortieAmont,
				vl_loc.DistanceBifAmont,
				vl_loc.DistancePeageAmont,
				vl_loc.DistanceBifAval);

  strcpy(vm_local,"");

  /*localisation sur une bretelle d'entree*/ /*DEM505*/
  if ( (vl_loc.TypePointCar == XDC_POINT_CARACT_ECHANGEUR) &&
	    ( (vl_loc.Sens == XDC_SENS_ENTREE_SUD) || (vl_loc.Sens == XDC_SENS_ENTREE_NORD)) )
  {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : bretelle d'entree");
    sprintf(vm_local,"DANS LA BRETELLE D'ENTREE DE L'ECHANGEUR %s %s.",
		vl_loc.SortieUsager,
		vl_loc.NomPointCar);
    return (vm_local);
  }
  /*localisation sur une bretelle de sortie*/
  if ( (vl_loc.TypePointCar == XDC_POINT_CARACT_ECHANGEUR) &&
	    ( (vl_loc.Sens == XDC_SENS_SORTIE_SUD) || (vl_loc.Sens == XDC_SENS_SORTIE_NORD)) )
  {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : bretelle de sortie");
    sprintf(vm_local,"DANS LA BRETELLE DE SORTIE DE L'ECHANGEUR %s %s.",
		vl_loc.SortieUsager,
		vl_loc.NomPointCar);
    return (vm_local);
  }


  /*localisation a une barriere de peage*/
  if (vl_loc.TypePointCar == XDC_POINT_CARACT_PEAGE) {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : peage");
    sprintf(vm_local," A LA BARRIERE DE %s.", vl_loc.NomPointCar);
    return (vm_local);
  }

  /*localisation sur une aire*/
  if ( (vl_loc.TypePointCar == XDC_POINT_CARACT_AIRE) &&
       (vm_noeud->Type != XZAEC_FMC_AireFermee) ) {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : aire");
    sprintf(vm_local," SUR L'AIRE DE %s.", vl_loc.NomPointCar);
    return (vm_local);
  }



  /*cas bifurc<sortie<fmc et peage<sortie<fmc*/
  if ( (vl_loc.DistanceSortieAmont <= vl_loc.DistanceBifAmont) &&
       (vl_loc.DistanceSortieAmont <= vl_loc.DistancePeageAmont) &&
       (vl_loc.NumSortieAmont != 0) )
  {
    sprintf(vm_local,"ENTRE LA SORTIE %s %s",
			vl_loc.SortieAmontUsager,
			vl_loc.NomSortieAmont);
  }
  else if ( (vl_loc.DistanceBifAmont <= vl_loc.DistanceSortieAmont) &&
	    (vl_loc.DistanceBifAmont < vl_loc.DistancePeageAmont) &&
	    (strcmp(vl_loc.NumAutorouteBifAmont,"")))
  {
    sprintf(vm_local,"ENTRE LA BIFURCATION VERS L'%s",
			vl_loc.NumAutorouteBifAmont);
  }
  /*cas sortie<peage<fmc et bifurc<peage<fmc*/
  else if ( (vl_loc.DistancePeageAmont < vl_loc.DistanceSortieAmont) &&
	    (vl_loc.DistancePeageAmont <= vl_loc.DistanceBifAmont) )
  {
    sprintf(vm_local,"ENTRE LA BARRIERE DE PEAGE DE %s",
			vl_loc.NomPeageAmont);
  }
  else  {
    /* sprintf(vm_local,"ENTRE LA LIMITE AMONT DE CONCESSION"); */
    sprintf(vm_local,"ENTRE %s",
	drol_msg_limites_amont(vl_loc.Autoroute, vl_loc.Sens));
  }

  XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_fax_section: SAV %d,BAV %d,PAV %d",
				vl_loc_tete.DistanceSortieAval,
				vl_loc_tete.DistanceBifAval,
				vl_loc_tete.DistancePeageAval);
  if (vl_loc_tete.DistanceBifAval==0) vl_loc_tete.DistanceBifAval=C_DISTANCE_INFINIE;

  if ( (vl_loc_tete.DistanceSortieAval < vl_loc_tete.DistanceBifAval) &&
       (vl_loc_tete.DistanceSortieAval <= vl_loc_tete.DistancePeageAval) &&
       (vl_loc_tete.NumSortieAval!=0) )
  {
    sprintf(vm_local,"%s ET LA SORTIE %s %s",
			vm_local,
			vl_loc_tete.SortieAvalUsager,
			vl_loc_tete.NomSortieAval);
  }
  else if ( (vl_loc_tete.DistanceBifAval <= vl_loc_tete.DistanceSortieAval) &&
	    (vl_loc_tete.DistanceBifAval <= vl_loc_tete.DistancePeageAval)  &&
	    (vl_loc_tete.DistanceBifAval != C_DISTANCE_INFINIE) &&
	    (vl_loc_tete.DistanceBifAval !=0))
  {
    sprintf(vm_local,"%s ET LA BIFURCATION VERS L'%s",
			vm_local,
			vl_loc_tete.NumAutorouteBifAval);
  }
  else if ( (vl_loc_tete.DistancePeageAval <= vl_loc_tete.DistanceSortieAval) &&
	    (vl_loc_tete.DistancePeageAval <= vl_loc_tete.DistanceBifAval) &&
	    (vl_loc_tete.DistancePeageAval != C_DISTANCE_INFINIE))
  {
    sprintf(vm_local,"%s ET LA BARRIERE DE PEAGE DE %s",
			vm_local,
			vl_loc_tete.NomPeageAval);
  }
  else  {
    /*strcat(vm_local," ET LA LIMITE AVAL DE CONCESSION");*/

    sprintf(vm_local,"%s ET %s",
		vm_local,
		drol_msg_limites_aval(vl_loc.Autoroute,vl_loc.Sens));
  }
 XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT drol_fax_section: vm_local=%s",vm_local);

  return (vm_local);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de generation du fax global pour l'arbre
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *drol_fax_elabore(XDY_District va_site, XDY_Evt va_evt, int va_bloc, char *va_msg)

/*
* ARGUMENTS EN ENTREE :
*   va_site           : le site local ;
*   va_evt            : l'evenement objet du plan d'action.
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
  T_NOEUD *vl_noeud;
  T_INTERLOCUTEUR *pl_liste;
  static char	vl_msg[10000];
  char	vl_libelle[250];
  char	*vl_texte_suite = "";
  XDY_Booleen	vl_evt_mentionne=XDC_FAUX;
  XDY_Booleen	vl_fax=XDC_FAUX;
  int           vl_JourSemaine = 0;
  double        vl_Maintenant = 0.0;
  double        vl_DateFin_Max = 0.0;
  double        vl_DateFin = 0.0;
  T_NOEUD *tl_noeud[50];
  int vl_indice;

  /* calculer l'horodate de fin butee pour filtrage des evenements */
  XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_Maintenant);
  vl_DateFin_Max = vl_Maintenant - C_DELAI_DIFFUSION;

  strcpy(vl_msg,"");
  strcpy(vl_libelle,"");

  if (va_bloc==0) { 
  vl_noeud=vg_racine;
      strcat( vl_msg,drol_fax_genere(vl_noeud));
      vl_texte_suite = "\nCONSEQUENCES :\n";
     XZST_03EcritureTrace(XZSTC_FONCTION,"drol_fax_elabore bloc 0 : MSG = %s",vl_msg);
    return (vl_msg);
  }

  for (vl_indice = 0; vl_indice < 50 ; vl_indice ++) {
      tl_noeud[vl_indice] = NULL;
  }

  /*on est ici donc on traite un des 4 blocs*/
  switch(va_bloc) {
    case 1 : for (vl_indice = 0; vl_indice < 50 ; vl_indice ++) { tl_noeud[vl_indice] = vg_B1[vl_indice]; } break;
    case 2 : for (vl_indice = 0; vl_indice < 50 ; vl_indice ++) { tl_noeud[vl_indice] = vg_B2[vl_indice]; } break;
    case 3 : for (vl_indice = 0; vl_indice < 50 ; vl_indice ++) { tl_noeud[vl_indice] = vg_B3[vl_indice]; } break;
    case 4 : for (vl_indice = 0; vl_indice < 50 ; vl_indice ++) { tl_noeud[vl_indice] = vg_B4[vl_indice]; } break;
    default : return ("");
  }

  for (vl_indice = 0; vl_indice < 50 ; vl_indice ++) {
    if (tl_noeud[vl_indice]==NULL)
      continue;

    vl_noeud = tl_noeud[vl_indice];
    if (vl_noeud->NumEvt.numero ==0)
      return (vl_msg);

    vl_evt_mentionne = XDC_VRAI;
    if ( (vl_noeud->NumEvt.numero == vg_racine->NumEvt.numero)  &&  (vl_noeud->NumEvt.cle ==  vg_racine->NumEvt.cle) )
    {
      vl_evt_mentionne = XDC_FAUX;
    }
    if (vl_evt_mentionne == XDC_VRAI) {
      if (vl_noeud->Infos_TFM.Termine)
      {
        if (XZSM_13ConversionHorodateSec(vl_noeud->Infos_TFM.Date_Fin, &vl_DateFin) == XZSMC_OK) {
          if (vl_DateFin < vl_DateFin_Max) {
	    vl_evt_mentionne = XDC_FAUX;
          }
	}
      }
      if (vl_evt_mentionne == XDC_VRAI) {
	strcat(vl_msg,"\nCONSEQUENCES :\n");
	strcat( vl_msg,drol_fax_genere(vl_noeud));
      }
    }
  }
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_fax_elabore MSG = %s",vl_msg);
  if (va_bloc <4) return(vl_msg);

  if (va_bloc==4) {
  /*generation du message complet*/
  strcat(va_msg,vl_msg);

  if (va_site==XDC_CI)
    /* On est au CI ESCOTA */
    pl_liste=vg_interlocuteurs_ci;
  else
    /* On est soit sur un PC ESCOTA, soit on est sur COFIROUTE et la liste PC est mergee avec la liste CI */
    pl_liste=vg_interlocuteurs_pc;

  /*met en base les interlocuteurs a appeler*/
  while (pl_liste!=NULL)
  {
    if ( (pl_liste->A_Appeler) && (pl_liste->Appel_Fax==1) )
    {
      XZAP09_Ajout_Dest_Fax(    vg_numero_plan, vg_ordre_fax,
			      pl_liste->PR, 
			      pl_liste->Autoroute,
			      pl_liste->Sens,
			      pl_liste->Type_Astreinte, 
			      pl_liste->Justification,0);
      if ((strlen(pl_liste->Competence)+strlen(vl_libelle)) < 250)
        strcat(vl_libelle,pl_liste->Competence);
	strcat(vl_libelle,"; ");
      vl_fax=XDC_VRAI;
    }
    if ( (pl_liste->A_Appeler) && (pl_liste->Appel_Mail==1))
    {
      XZAP09_Ajout_Dest_Fax(    vg_numero_plan, vg_ordre_fax,
				pl_liste->PR,
				pl_liste->Autoroute,
				pl_liste->Sens,
				pl_liste->Type_Astreinte,
				"mail",0);
      vl_fax=XDC_VRAI;
    }
    pl_liste=pl_liste->Suivant;
  }

  if (vl_fax==XDC_VRAI)
  {
    XZST_03EcritureTrace(XZSTC_WARNING ,"valeur de va_site ds le fonction drol_fax_elabore: %d",va_site);
    	if (XZAP07_Ajout_Prop_Fax(vg_numero_plan, 
				vg_ordre_fax, 
				va_site, 
				"", 
				vl_libelle, 
				"Envoi d'un Mail", 
				va_msg,XDC_FAUX	) != XDC_OK)
      		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAP07_Ajout_Prop_Fax retourne XDC_NOK");
      	
    XZST_03EcritureTrace(XZSTC_WARNING,"vg_calcul_auto %d",vg_calcul_auto);
    if (vg_calcul_auto) {
		/*envoi fax en auto*/
	  XZIT22_Fax_Auto(vg_numero_plan,vg_ordre_fax,va_evt.numero,va_evt.cle,vl_libelle,1,NomMachine);
	}
  }

  vg_ordre_fax++;
  }
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de generation du fax police
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void drol_fax_elabore_police(T_NOEUD *va_noeud)

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
  char	vl_msg[10000];

  strcpy(vl_msg,"");
  strcat( vl_msg,drol_fax_genere_police(va_noeud));

  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_fax_elabore_police MSG = %s",vl_msg);

  /*on doit contacter prefecture2*/
  XZAP09_Ajout_Dest_Fax(    vg_numero_plan, vg_ordre_fax,
			va_noeud->Localisation.PR,
			va_noeud->Localisation.Autoroute,
			va_noeud->Localisation.Sens,
			17,
			"Fax special police",0);

  /*on doit contacter dg*/
  XZAP09_Ajout_Dest_Fax(    vg_numero_plan, vg_ordre_fax,
			va_noeud->Localisation.PR,
			va_noeud->Localisation.Autoroute,
			va_noeud->Localisation.Sens,
			33,
			"Fax special police",0);

  if (XZAP07_Ajout_Prop_Fax(vg_numero_plan, 
				vg_ordre_fax, 
				vg_SitePrincipal, 
				"", 
				"PREFECTURE|DG",
				"Envoi d'un Mail", 
				vl_msg,XDC_FAUX) != XDC_OK)
  {
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAP07_Ajout_Prop_Fax retourne XDC_NOK");
  }

  vg_ordre_fax++;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de generation du SMS global 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *drol_sms_elabore(XDY_District va_site, XDY_Evt va_evt, T_NOEUD *va_noeud)

/*
* ARGUMENTS EN ENTREE :
*   va_site           : le site local ;
*   va_evt            : l'evenement objet du plan d'action.
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
  T_NOEUD *vl_noeud;
  T_INTERLOCUTEUR *pl_liste;
  static char	vl_msg[10000];
  char	vl_libelle[250];
  char	*vl_texte_suite = "";
  XDY_Booleen	vl_evt_mentionne=XDC_FAUX;
  XDY_Booleen	vl_sms=XDC_FAUX;
  int           vl_JourSemaine = 0;
  double        vl_Maintenant = 0.0;
  double        vl_DateFin_Max = 0.0;
  double        vl_DateFin = 0.0;
  T_NOEUD *tl_noeud[50];
  int vl_indice;

  /* calculer l'horodate de fin butee pour filtrage des evenements */
  XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_Maintenant);
  vl_DateFin_Max = vl_Maintenant - C_DELAI_DIFFUSION;

  strcpy(vl_msg,"");
  strcpy(vl_libelle,"");

  vl_noeud = va_noeud;
      strcat( vl_msg,drol_fax_genere_SMS(vl_noeud));
     XZST_03EcritureTrace(XZSTC_FONCTION,"drol_sms_elabore : MSG = %s",vl_msg);

  if (va_site==XDC_CI)
    /* On est au CI ESCOTA */
    pl_liste=vg_interlocuteurs_ci;
  else
    /* On est soit sur un PC ESCOTA, soit on est sur COFIROUTE et la liste PC est mergee avec la liste CI */
    pl_liste=vg_interlocuteurs_pc;

  /*met en base les interlocuteurs a appeler*/
  while (pl_liste!=NULL)
  {
    if ( (pl_liste->A_Appeler) && (pl_liste->Appel_SMS==1) )
    {
      XZAP09_Ajout_Dest_Fax(    vg_numero_plan, vg_ordre_fax,
			      pl_liste->PR, 
			      pl_liste->Autoroute,
			      pl_liste->Sens,
			      pl_liste->Type_Astreinte, 
			      "SMS",
			      1);
      if ((strlen(pl_liste->Competence)+strlen(vl_libelle)) < 250)
        strcat(vl_libelle,pl_liste->Competence);
	strcat(vl_libelle,"; ");
      vl_sms=XDC_VRAI;
    }
    pl_liste=pl_liste->Suivant;
  }

  if (vl_sms==XDC_VRAI)
  {
    XZST_03EcritureTrace(XZSTC_WARNING ,"valeur de va_site ds le fonction drol_fax_elabore: %d",va_site);
    	if (XZAP07_Ajout_Prop_Fax(vg_numero_plan, 
				vg_ordre_fax, 
				va_site, 
				"", 
				vl_libelle, 
				"Envoi d'un SMS", 
				vl_msg, XDC_VRAI) != XDC_OK)
      		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : XZAP07_Ajout_Prop_Fax retourne XDC_NOK");
      	
    /*XZST_03EcritureTrace(XZSTC_WARNING,"vg_calcul_auto %d",vg_calcul_auto);
    if (vg_calcul_auto) {
	  XZIT22_Fax_Auto(vg_numero_plan,vg_ordre_fax,va_evt.numero,va_evt.cle,vl_libelle,1,NomMachine);
	}
  */
  }

  vg_ordre_fax++;
}
