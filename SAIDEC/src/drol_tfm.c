/* Fichier : $Id: drol_tfm.c,v 1.56 2021/08/19 15:39:22 devgfi Exp $        $Revision: 1.56 $        $Date: 2021/08/19 15:39:22 $
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : DROL_TFM				*
* FICHIER : drol_tfm.c				*
*************************************************
* DESCRIPTION :
* new tdpac
*************************************************
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.0
* PCL	31/10/03	"APRES LE KM" au lieu de "ENTRE LE KM ET LE KM"  1.2
* JMG   17/02/04        ajout de datex (SAGA)
* HILMARCHER 15/11/2004 DEM 375 PB ELAB. FAX EN LIMITE DE CONCESSION AMONT 1.3
* JMG 14/06/05		SAGA
* PNI	29/08/05	Correction drol_msg_kilometre pour cas A57 v1.5 DEM506
* JMG	25/10/05	ajout PMVA 1.6
* PCL	07/12/06	erreur de recherche limite quans sens non precise 1.7 (601)
* JPL	14/05/07	Ajout FMCs Veh. marche arriere, Trait. chaussees, Echang. deconseille,
* 			   Chaussee glissante, Basculement pour travaux (DEM 643-647) 1.8
* JPL	27/02/09	Gestion champ 'Feu maitrise' pour Vehicule en feu 1.9
* JPL	24/03/09	Localisation en tunnel prioritaire (DEM 271) 1.10
* PNI	30/03/09 	Ajout  d'un flag cause_operation pour affichage cause sur PMVA DEM876 1.11
* JPL	31/03/09	Filtrage des evenements termines depuis un certain temps (DEM 411) 1.12
* JPL	07/04/09	Ne jamais indiquer de cause pour le premier evenement mentionne (DEM 411) 1.13
* JPL	06/09/10	Prise en compte des Fmc Zone de stockage (DEM 942) 1.14
* JPL	13/01/11	Migration HP ia64 (DEM 971) : Gestion conversions types de tailles differentes (=)  1.15
* JPL	13/01/11	Migration architecture HP ia64 (DEM 971) : Gestion tampons chaines caracteres  1.16
* JPL	13/10/11	Prise en compte du parametre "origine" retourne par la procedure (DEM 995)  1.17
* JPL	20/01/12	Localisation des Fmc "Meteo" et "Vehicule lent" entre deux sorties (DEM 1010)  1.18
* JPL	03/02/12	Prise en compte de tous les numeros de bifurcations (pour DEM 1011)  1.19
* JMG	24/05/12	Linyx 1.20
* VR	25/01/12    Ajout Regulation (DEM/1016)
* JPL	09/07/12	Gestion des noms de sortie "usager" et indicateurs de bifurcations (DEM 1033)  1.22
* JPL	27/08/12	Localisation des Fmc "Traitements chausses" entre deux sorties (DEM 1038)  1.23
* JPL	04/09/12	Mettre la distance de bifurcation a INFINIE s'il n'y en a pas  1.24
* JMG	11/11/12	regulation 1.25
* PNI	06/02/13	Msg TFM pour Cas d'evt  en bout de reseau A51 (entre sisteron et la saulce) 1.26
* PNI	26/02/13	Msg TFM pour Cas d'evt  en bout de reseau FI 1866 1.26
* PCL	02/05/13	CI -> CIT 1.28
* PNI	19/9/13		Correction pour evt signal� en d�but A500 s1 dEM1084 1.29
* PNI	13/2/14		Prise en cpte des evt "double sens" dans drol_msg_limites_amont DEM1088 1.30
* PNI	25/09/14	Ajout d'items dans le msg TFM DEM1097 1.31 1.32 1.33
* JMG   18/06/15 : ajout h debut et h debut prevu datetime DEM1130 1.35
* NDE	22/07/15	Logique de la localisation modifi�e pour les tunnels DEM1139 1.36
* JMG   12/10/15 : Correctif sur presence des evt termin�s dans le msg tfm DEM1130 1.37
* PNI   28/10/15 : Correctif pour l'indication du lit d'arret occup� FI3530 DEM 1139 1.38
* PNI	09/11/15 : Correctif limite aval/amont FI3587 1.39
* PNI	04/01/16 : Correctif limite aval/amont FI3657 1.40
* PNI 	15/06/16 : Correctif absence localisa PR pour evt confirm� si pas de sortie amont FI3757 3748 3758 1.41
* PNI 	27/06/16 : Ajout de l'autoroute 11 (A570) pour les bifurcation DEM1176 1.42
* LCL	15/11/16 : Ajout localisation tunnel/viaduc (LOT23) 1.43
* JPL	13/12/16 : Taille suffisante pour la zone des differentes portions de texte  1.44
* JMG	20/03/17 : ajout type_cause et complement rappel dans xzae200 1.45
* JMG	20/03/17 : ajout C_VAR_APPELS_MAJEUR 1.46
* JMG	26/06/17 : convergence zone urbaine 1.47
* PNI	25/01/18 : correction message RVA pour evt dans bifurcation drol_tfm_localisation  DEM1268 1.48
* JMG   08/03/18 ; personnel travaux 1.49 DEM1282
* JPL	29/06/18 : Ajout nombre de passagers (DEM 1295)  1.50
* JMG	02/09/18 : ajout cause et cle_cause LOT25 DEM1301 1.51
* JPL	08/04/19 : Ajout indicateur d'�coulement (DEM 1326)  1.53
* JPL	23/09/19 : Renommage du fichier "Niveau_Incendie_Vegetation.txt" en "Niveau_Incendie.txt" (DEM 1352)  1.54
* JPL	25/09/19 : Ajout indicateur de pr�sence de bouchon (DEM 1346)  1.55
* JPL	02/07/21 : Extension champ "remarque" des appels (pour ech. et barrieres fermes si appel majeur) (SAE-287)  1.56
* PNI 19/07/22 : initialisation vl_niveau=0, test si le pa_niveau<1 pour forcer le calcul d'un niveau TFM SAE_396
* LCL	05/10/23 	: Adaptations CNA DEM-483 : lecture site principal en base
************************************************/

#include "drol_tfm.h"

#include "xzae21.h"
#include "xzae150.h"
#include "xzap180.h"
#include <stdlib.h>


#define C_REP_MIG	"/produits/migrazur/appliSD/fichiers/bdc/"
#define C_TFM_FIN	C_REP_MIG "tfm_fin.txt"
#define C_TFM_ALERTE	C_REP_MIG "tfm_alerte.txt"
#define C_TFM_PREVISION	C_REP_MIG "tfm_prevision.txt"
#define C_TFM_PREALERTE	C_REP_MIG "tfm_prealerte.txt"
#define C_TFM_LIMITES	C_REP_MIG "tfm_limites.txt"


#define C_SEPARATEUR	"#"


#define C_VAR_AUTOROUTE "$autoroute"
#define C_VAR_DUREE	"$duree"
#define C_VAR_SECOURS	"$secours"
#define C_VAR_SECOURS_B	"$secours_bref"
#define C_VAR_DEBUT	"$debut"
#define C_VAR_LOCALISATION "$localisation"
#define C_VAR_NEUTRALISATION "$neutralisations"
#define C_VAR_VICTIMES	"$victimes"
#define C_VAR_GRAVITE	"$gravite"
#define C_VAR_VEHICULES	"$vehicules"
#define C_VAR_GLISSE	"$glisse"
#define C_VAR_CONSEIL	"$conseil"
#define C_VAR_VOIES_BLOQUEES "$voies_bloquees"
#define C_VAR_EVT_GN	"$evt_GN"
#define C_VAR_EVT_GV	"$evt_GV"
#define C_VAR_FIN	"$fin"
#define C_VAR_LGR_BOUCHON	"$bouchon"
#define C_VAR_PERIODE	"$periode"
#define C_VAR_RETOUR	"$retour_normal"
#define C_VAR_VISIBILITE	"$visibilite"
#define C_VAR_CAUSE	"$cause"
#define C_VAR_TDP	"$tdp"
#define C_VAR_ZONE	"$zone"
#define C_VAR_SCENARIO	"$scenario"
#define C_VAR_TRANSITOIRE	"$transitoire"
#define C_VAR_BILAN_MAT	"$bilan_materiel"
#define C_VAR_VOIES_IMPACT	"$voies_impactees"
#define C_VAR_VOIES_REST	"$voies_restantes"
#define C_VAR_CAUSE_NAT		"$cause_ou_nature"
#define C_VAR_BILAN_CORP	"$bilan_corporel"
#define C_VAR_APPELS_MAJEUR	"$appels_majeur"

#define C_PR_ST_LAURENT         185200
#define C_PR_NICE_OUEST         186500


/* Delai maximal (secondes) de mention d'un evenement sur RTFM ou dans les fax apres sa terminaison */
#define C_DELAI_DIFFUSION	(15 * 60)


#define C_DISTANCE_INFINIE	999999



/* definition des variables globales */

T_NOEUD *vg_racine;
extern T_NOEUD *vg_B1[50];
extern T_NOEUD *vg_B2[50];
extern T_NOEUD *vg_B3[50];
extern T_NOEUD *vg_B4[50];
extern XDY_Mot vg_numero_plan;
extern XDY_Mot vg_SitePrincipal;


/* definition des variables locales au module */

T_NOEUD *vm_noeud;
static XDY_Booleen vm_flag_cause;

static char vm_messageTFM[10000];
static char vm_local[150];
static char vm_km[70];
static char vm_str1[70];
static char vm_str2[70];

#ifndef _TIBCO_EMS
static T_STRING vm_str;
#else
static char vm_str[400];
#endif

static XDY_Booleen vm_indiquer_cause = FALSE;

static T_Modele *pm_fin=NULL;
static int	 vm_fin_nb_lexemes = 0;
static T_Modele *pm_alerte=NULL;
static int	 vm_alerte_nb_lexemes = 0;
static T_Modele *pm_prealerte=NULL;
static int	 vm_prealerte_nb_lexemes = 0;
static T_Modele *pm_prevision=NULL;
static int	 vm_prevision_nb_lexemes = 0;
static T_Modele *pm_limites=NULL;
static int	 vm_limites_nb_lexemes = 0;

static char *version = "$Id: drol_tfm.c,v 1.56 2021/08/19 15:39:22 devgfi Exp $ : drol_tfm" ;



/* declaration de fonctions internes */

int drol_tfm_charge(char *,T_Modele *, int *);
int drol_tfm_remplit_modele(T_Modele *,int );

void drol_cumul_voies(XDY_Voie, int *, int *);


int drol_tfm_xzae200(char *,char *,char *,char *,
		char *, char *, char *,
		char *, char *, char *,
		char *,char *,char *,char *,
		char *,char *, char *, char *,
		char *,char *,
		XDY_Booleen , XDY_Booleen ,XDY_Booleen ,
		XDY_Autoroute, XDY_Sens, XDY_PR, XDY_PR,
		XDY_Octet, XDY_Octet, char *,
		int, XDY_Octet, XDY_Octet, XDY_Octet, XDY_Octet, XDY_Octet,
		char *, char *,
		XDY_Octet, XDY_Octet,
		XDY_Booleen, XDY_Booleen,
		int, int,
		XDY_Octet, XDY_Octet, XDY_Octet, XDY_Octet, XDY_Octet, XDY_Octet, 
		XDY_Octet, XDY_Octet,
		XDY_Booleen, XDY_Booleen,
		XDY_Octet, XDY_Octet, XDY_Octet, XDY_Octet,
		XDY_Booleen,
		XDY_Booleen, XDY_Booleen, XDY_Booleen, XDY_Booleen,
		XDY_Booleen, XDY_Octet,
		char *, XDY_Octet,
		char *, XDY_Octet, XDY_Octet ,
		char *,
		XDY_Octet, XDY_Booleen, XDY_Booleen,
		char *,XDY_Octet, /*SAGA*/
		char *, char *, char *, XDY_Octet, XDY_Octet, XDY_Octet, XDY_Octet,
		char*, int, int,
		char *, char *, char *, XDY_Eqt,
		char *, XDY_Booleen,char *,char *, XDY_Horodate, XDY_Horodate, XDY_Octet, char *, XDY_Mot,
		XDY_Octet, XDY_Octet,
		XDY_Octet, XDY_Octet,
		int, XDY_Octet, XDY_Octet,
		XDY_Octet);

void drol_tfm_localiser(T_Localisation *,
        XDY_Octet       ,
        XDY_Entier      ,
        XDY_Octet       ,
        XDY_Octet       ,
        XDY_Entier      ,
        XDY_Octet       ,
        XDY_Octet       ,
        XDY_Entier      ,
        XDY_Octet       ,
        XDY_Entier      ,
        XDY_Entier      ,
        XDY_Autoroute   ,
        XDY_Entier      ,
        XDY_Autoroute   ,
        XDY_Entier      ,
        char        *,
        char        *,
        char        *,
        char        *,
        char        *,
        char        *,
        char        *,
        char        *,
        char        *,
        char        *,
        char        *,
        char        *,
        XDY_Mot         ,
        XDY_Octet       ,
        XDY_PR          ,
        XDY_PR          ,
        XDY_PR          ,
        XDY_PR          ,
	XDY_Nom ,
	XDY_Mot);

int drol_calcul_niveau_TFM(XDY_Octet *, int,
			XDY_Voie, XDY_Voie, XDY_Voie, XDY_Voie, XDY_Voie,
			int, int, int, int);

char *drol_msg_kilometre(int, int, int, int);
char *drol_msg_distance(int);
char *drol_msg_limites_amont(int, int);
char *drol_msg_limites_aval(int, int);
void drol_trim(char *);
char *drol_majuscule(char *);

/*fonctions elementaires qui renvoient une chaine en fonction de la valeur de la variable*/
static char *drol_tfm_autoroute();
static char *drol_tfm_duree();
static char *drol_tfm_secours();
static char *drol_tfm_secours_bref();
static char *drol_tfm_debut();
char *drol_tfm_localisation();
static char *drol_tfm_neutralisation();
static char *drol_tfm_victimes(); 
static char *drol_tfm_gravite();
static char *drol_tfm_vehicules();
static char *drol_tfm_glisse();
static char *drol_tfm_conseil();
static char *drol_tfm_voies_bloquees();
static char *drol_tfm_evt_GN();
static char *drol_tfm_evt_GV();
static char *drol_tfm_fin();
static char *drol_tfm_longueur_bouchon();
static char *drol_tfm_periode();
static char *drol_tfm_retour_normal();
static char *drol_tfm_visibilite();
static char *drol_tfm_cause();
static char *drol_tfm_tdp();
static char *drol_tfm_zone();
static char *drol_tfm_scenario();
static char *drol_tfm_transitoire();
static char *drol_tfm_bilan_materiel();
static char *drol_tfm_voies_impactees();
static char *drol_tfm_voies_restantes();
static char *drol_tfm_cause_ou_nature();
static char *drol_tfm_bilan_corporel();
static char *drol_tfm_appels_majeur();


char *drol_tfm_get_heure();

static char *drol_tfm_null();


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_tfm_init()

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
*
------------------------------------------------------*/
{
  int vl_nb_lexemes=0;
  T_Modele *tl_lexemes;
  int vl_i=0;

  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : drol_tfm_init");

  /*chargement des fichiers de config trafic FM*/
  tl_lexemes=calloc(255,sizeof(T_Modele));

  drol_tfm_charge(C_TFM_FIN,tl_lexemes,&vl_nb_lexemes);
  pm_fin = calloc (vl_nb_lexemes, sizeof(T_Modele));
  for (vl_i=0;vl_i<vl_nb_lexemes;vl_i++)
    strcpy(pm_fin[vl_i].Valeur,tl_lexemes[vl_i].Valeur);
  vm_fin_nb_lexemes=vl_nb_lexemes;
  drol_tfm_remplit_modele(pm_fin,vm_fin_nb_lexemes);

  drol_tfm_charge(C_TFM_ALERTE,tl_lexemes,&vl_nb_lexemes);
  pm_alerte = calloc (vl_nb_lexemes, sizeof(T_Modele));
  for (vl_i=0;vl_i<vl_nb_lexemes;vl_i++)
    strcpy(pm_alerte[vl_i].Valeur,tl_lexemes[vl_i].Valeur);
  vm_alerte_nb_lexemes=vl_nb_lexemes;
  drol_tfm_remplit_modele(pm_alerte,vm_alerte_nb_lexemes);

  drol_tfm_charge(C_TFM_PREALERTE,tl_lexemes,&vl_nb_lexemes);
  pm_prealerte = calloc (vl_nb_lexemes, sizeof(T_Modele));
  for (vl_i=0;vl_i<vl_nb_lexemes;vl_i++)
    strcpy(pm_prealerte[vl_i].Valeur,tl_lexemes[vl_i].Valeur);
  vm_prealerte_nb_lexemes=vl_nb_lexemes;
  drol_tfm_remplit_modele(pm_prealerte,vm_prealerte_nb_lexemes);

  drol_tfm_charge(C_TFM_PREVISION,tl_lexemes,&vl_nb_lexemes);
  pm_prevision = calloc (vl_nb_lexemes, sizeof(T_Modele));
  for (vl_i=0;vl_i<vl_nb_lexemes;vl_i++)
    strcpy(pm_prevision[vl_i].Valeur,tl_lexemes[vl_i].Valeur);
  vm_prevision_nb_lexemes=vl_nb_lexemes;
  drol_tfm_remplit_modele(pm_prevision,vm_prevision_nb_lexemes);

  drol_tfm_charge(C_TFM_LIMITES,tl_lexemes,&vl_nb_lexemes);
  pm_limites = calloc (vl_nb_lexemes, sizeof(T_Modele));
  for (vl_i=0;vl_i<vl_nb_lexemes;vl_i++) {
    strcpy(pm_limites[vl_i].Valeur,tl_lexemes[vl_i].Valeur);
    pm_limites[vl_i].Valeur[strlen(pm_limites[vl_i].Valeur)-1]='\0';
    pm_limites[vl_i].Fct = drol_tfm_null; 
  }
  vm_limites_nb_lexemes=vl_nb_lexemes;

  XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_tfm_init : nb fin %d, nb alerte %d, nb prealerte %d, nb prev %d, nb limites %d",
			vm_fin_nb_lexemes,
			vm_alerte_nb_lexemes,
			vm_prealerte_nb_lexemes,
			vm_prevision_nb_lexemes,
			vm_limites_nb_lexemes);
  return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config Trafic FM
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_tfm_charge(char *va_nom,T_Modele *ta_lexemes, int *va_nombre)

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

  XZST_03EcritureTrace(XZSTC_DEBUG1, "IN  : drol_tfm_charge (%s)",va_nom);

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
      vl_str=vl_Lig; 
      while ((vl_ch=strtok(vl_str,C_SEPARATEUR))!=NULL)
      {
	strcpy(ta_lexemes[*va_nombre].Valeur,vl_ch);
	vl_str=NULL;
	XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_tfm_charge : lexeme %s, indice %d",ta_lexemes[*va_nombre].Valeur,
					*va_nombre);
	(*va_nombre)++;
      }
    }
    else break;
  }
  fclose(pl_fp);
  XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT  : drol_tfm_charge");
  return (XDC_OK);
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* remplissage de la liste chainee Modele (T_Lexemes)
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_tfm_remplit_modele(T_Modele *va_modele,int va_taille)

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

  XZST_03EcritureTrace(XZSTC_DEBUG1, "IN : drol_tfm_remplit_modele");

  for (vl_i=0;vl_i<va_taille;vl_i++)
  {
    if (!strcmp(va_modele[vl_i].Valeur,C_VAR_CAUSE)){
      va_modele[vl_i].Fct = drol_tfm_cause;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_AUTOROUTE)) {
      va_modele[vl_i].Fct = drol_tfm_autoroute;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_APPELS_MAJEUR)) {
      va_modele[vl_i].Fct = drol_tfm_appels_majeur;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_DUREE)) {
      va_modele[vl_i].Fct = drol_tfm_duree;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_SECOURS)) {
      va_modele[vl_i].Fct = drol_tfm_secours;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_SECOURS_B)) {
      va_modele[vl_i].Fct = drol_tfm_secours_bref;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_DEBUT)) {
      va_modele[vl_i].Fct = drol_tfm_debut;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_LOCALISATION)) {
      va_modele[vl_i].Fct = drol_tfm_localisation;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_NEUTRALISATION)) {
      va_modele[vl_i].Fct = drol_tfm_neutralisation;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_VICTIMES)) {
      va_modele[vl_i].Fct = drol_tfm_victimes;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_GRAVITE)) {
      va_modele[vl_i].Fct = drol_tfm_gravite;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_VEHICULES)) {
      va_modele[vl_i].Fct = drol_tfm_vehicules;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_GLISSE)) {
      va_modele[vl_i].Fct = drol_tfm_glisse;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_CONSEIL)) {
      va_modele[vl_i].Fct = drol_tfm_conseil;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_VOIES_BLOQUEES)) {
      va_modele[vl_i].Fct = drol_tfm_voies_bloquees;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_EVT_GN)) {
      va_modele[vl_i].Fct = drol_tfm_evt_GN;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_EVT_GV)) {
      va_modele[vl_i].Fct = drol_tfm_evt_GV;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_FIN)) {
      va_modele[vl_i].Fct = drol_tfm_fin;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_LGR_BOUCHON)) {
      va_modele[vl_i].Fct = drol_tfm_longueur_bouchon;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_PERIODE)) {
      va_modele[vl_i].Fct = drol_tfm_periode;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_RETOUR)) {
      va_modele[vl_i].Fct = drol_tfm_retour_normal;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_VISIBILITE)) {
      va_modele[vl_i].Fct = drol_tfm_visibilite;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_TDP)) {
      va_modele[vl_i].Fct = drol_tfm_tdp;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_ZONE)) {
      va_modele[vl_i].Fct = drol_tfm_zone;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_SCENARIO)) {
      va_modele[vl_i].Fct = drol_tfm_scenario;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_TRANSITOIRE)) {
      va_modele[vl_i].Fct = drol_tfm_transitoire;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_BILAN_MAT)) {
      va_modele[vl_i].Fct = drol_tfm_bilan_materiel;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_VOIES_IMPACT)) {
      va_modele[vl_i].Fct = drol_tfm_voies_impactees;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_VOIES_REST)) {
      va_modele[vl_i].Fct = drol_tfm_voies_restantes;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_CAUSE_NAT)) {
      va_modele[vl_i].Fct = drol_tfm_cause_ou_nature;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    else if (!strcmp(va_modele[vl_i].Valeur,C_VAR_BILAN_CORP)) {
      va_modele[vl_i].Fct = drol_tfm_bilan_corporel;
      strcpy(va_modele[vl_i].Valeur, "");
    }
    /*si ce n'est pas une variable*/
    else
      va_modele[vl_i].Fct = drol_tfm_null;
  }
  XZST_03EcritureTrace(XZSTC_DEBUG1, "OUT : drol_tfm_remplit_modele");
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* lit en base les infos de l'evt pour le calcul du 
* msg TFM : recupere les variables elementaires formattees
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_tfm_lit_infos(XDY_Horodate va_horodate,T_NOEUD *va_noeud, XDY_District va_site)

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
        XDY_Octet       NumSortieAmont=0;
        XDY_Entier      DistanceSortieAmont;
        XDY_Octet       SortieAmontEstBifurc=XDC_FAUX;
        XDY_Octet       NumSortieAval=0;
        XDY_Entier      DistanceSortieAval;
        XDY_Octet       SortieAvalEstBifurc=XDC_FAUX;
        XDY_Octet       NumPeageAmont=0;
        XDY_Entier      DistancePeageAmont;
        XDY_Octet       NumPeageAval=0;
        XDY_Entier      DistancePeageAval;
        XDY_Entier      LongueurPlateformePeageAval;
        XDY_Autoroute   NumAutorouteBifAmont=0;
        XDY_Entier      DistanceBifAmont;
        XDY_Autoroute   NumAutorouteBifAval=0;
        XDY_Entier      DistanceBifAval;
        char        Descente[50];
        char        Tunnel[50];
        char        NomSortieAmont[50];
        char        AbrevSortieAmont[50];
        XDY_Sortie  SortieAmontUsager;
        char        NomSortieAval[50];
        char        AbrevSortieAval[50];
        XDY_Sortie  SortieAvalUsager;
        char        NomPeageAmont[50];
        char        AbrevPeageAmont[50];
        char        NomPeageAval[50];
        char        AbrevPeageAval[50];
        XDY_Mot         Pente=0;
	XDY_Mot		ZoneUrbaine=0;
        XDY_Octet       Site;
        XDY_PR          PrBifurcAmont;
        XDY_PR          PrBifurcAval;
        XDY_PR          PrDebutBifurcAmont;
        XDY_PR          PrDebutBifurcAval;
	char		NomViaduc[50];


  vm_noeud = va_noeud;
  XZST_03EcritureTrace(   XZSTC_INFO, "In : drol_tfm_lit_infos: %d %d",
				vm_noeud->NumEvt.numero,vm_noeud->NumEvt.cle);

  /*si en mode historique*/
  if (vg_mode_historique) {
    /*if (XZAE201_Lire_Infos_TFM_Hist((XDY_FonctionInt)drol_tfm_xzae200,*/
    if (XZAE200_Lire_Infos_TFM((XDY_FonctionInt)drol_tfm_xzae200,
				vm_noeud->NumEvt.numero,
				vm_noeud->NumEvt.cle) != XDC_OK)
    {
      XZST_03EcritureTrace(   XZSTC_WARNING,"drol_tfm_lit_infos:  appel a XZAE200 a echoue");
      return (XDC_NOK);
    }
  }
  else { /*si pas en mode historique*/
    /*appelle la proc XZAE200*/
    if (XZAE200_Lire_Infos_TFM((XDY_FonctionInt)drol_tfm_xzae200,
				vm_noeud->NumEvt.numero,
				vm_noeud->NumEvt.cle) != XDC_OK)
    { 
      XZST_03EcritureTrace(   XZSTC_WARNING,"drol_tfm_lit_infos: 	appel a XZAE200 a echoue");
      return (XDC_NOK);
    }
  }

  /*appelle la proc XZAE;21 pour recuperer la localisation*/
  if (XZAE21_Localiser_Evt_PA(    vm_noeud->Localisation.Autoroute, vm_noeud->Localisation.PR, 
					vm_noeud->Localisation.Sens,
                                        &NumSortieAmont, &DistanceSortieAmont, &SortieAmontEstBifurc,
                                        &NumSortieAval, &DistanceSortieAval, &SortieAvalEstBifurc,
                                        &NumPeageAmont, &DistancePeageAmont,
                                        &NumPeageAval, &DistancePeageAval,
                                        &LongueurPlateformePeageAval,
                                        &NumAutorouteBifAmont, &DistanceBifAmont,
                                        &NumAutorouteBifAval, &DistanceBifAval,
                                        Descente,
                                        Tunnel,
                                        NomSortieAmont, AbrevSortieAmont, SortieAmontUsager,
                                        NomSortieAval, AbrevSortieAval, SortieAvalUsager,
                                        NomPeageAmont, AbrevPeageAmont,
                                        NomPeageAval, AbrevPeageAval,
                                        &Pente, &Site, &PrBifurcAmont, &PrBifurcAval,
                                        &PrDebutBifurcAmont,&PrDebutBifurcAval, NomViaduc,
					&ZoneUrbaine) != XDC_OK )
  {
    XZST_03EcritureTrace(   XZSTC_WARNING,"drol_tfm_lit_infos:  appel a XZAE21 a echoue");
    return (XDC_NOK);
  }

  XZST_03EcritureTrace(   XZSTC_INFO,"NumSortieAmont %d%s %d",NumSortieAmont,(SortieAmontEstBifurc) ? "(BIF)" : "",DistanceSortieAmont);
  XZST_03EcritureTrace(   XZSTC_INFO,"NumSortieAval %d%s %d",NumSortieAval,(SortieAvalEstBifurc) ? "(BIF)" : "",DistanceSortieAval);
  XZST_03EcritureTrace(   XZSTC_INFO,"NumAutorouteBifAmont %d %d",NumAutorouteBifAmont,DistanceBifAmont);
  XZST_03EcritureTrace(   XZSTC_INFO,"NumAutorouteBifAval %d %d",NumAutorouteBifAval,DistanceBifAval);
  XZST_03EcritureTrace(   XZSTC_INFO,"tunnel %s",Tunnel);
  XZST_03EcritureTrace(   XZSTC_INFO,"s amont %s(%s) // s aval %s(%s)",NomSortieAmont,SortieAmontUsager,NomSortieAval,SortieAvalUsager);
  XZST_03EcritureTrace(   XZSTC_INFO,"peage amont  %s // aval %s",NomPeageAmont, NomPeageAval);
  XZST_03EcritureTrace(   XZSTC_WARNING,"abrev sortie amont  %s // aval %s",AbrevSortieAmont, AbrevSortieAval);
  XZST_03EcritureTrace(   XZSTC_WARNING,"nom viaduc %s",NomViaduc);
  XZST_03EcritureTrace(   XZSTC_WARNING,"urbaine %d",ZoneUrbaine);

  if (NumAutorouteBifAmont==0) DistanceBifAmont = C_DISTANCE_INFINIE;
  if (NumAutorouteBifAval==0) DistanceBifAval = C_DISTANCE_INFINIE;
  if (SortieAmontEstBifurc == XDC_VRAI) DistanceSortieAmont = C_DISTANCE_INFINIE;
  if (SortieAvalEstBifurc == XDC_VRAI) DistanceSortieAval = C_DISTANCE_INFINIE;

  /*memorisation des infos recuperees*/
  drol_tfm_localiser(&(vm_noeud->Localisation),NumSortieAmont,DistanceSortieAmont,SortieAmontEstBifurc,
					NumSortieAval,DistanceSortieAval,SortieAvalEstBifurc,
					NumPeageAmont,DistancePeageAmont,
					NumPeageAval,DistancePeageAval,
					LongueurPlateformePeageAval,
					NumAutorouteBifAmont,DistanceBifAmont,
					NumAutorouteBifAval,DistanceBifAval,
					Descente,Tunnel,
					NomSortieAmont,AbrevSortieAmont,SortieAmontUsager,
					NomSortieAval,AbrevSortieAval,SortieAvalUsager,
					NomPeageAmont, AbrevPeageAmont,
					NomPeageAval, AbrevPeageAval,
					Pente,Site,PrBifurcAmont,PrBifurcAval,
					PrDebutBifurcAmont,PrDebutBifurcAval,NomViaduc,ZoneUrbaine);

  /*si evt ponctuel*/
  if (vm_noeud->Localisation_Tete.PR == vm_noeud->Localisation.PR)
  {
    XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_tfm_lit_infos : evt ponctuel");
    vm_noeud->Localisation_Tete.NumSortieAmont=vm_noeud->Localisation.NumSortieAmont;
    vm_noeud->Localisation_Tete.SortieAmontEstBifurc=vm_noeud->Localisation.SortieAmontEstBifurc;
    vm_noeud->Localisation_Tete.DistanceSortieAmont=vm_noeud->Localisation.DistanceSortieAmont;
    vm_noeud->Localisation_Tete.NumSortieAval=vm_noeud->Localisation.NumSortieAval;
    vm_noeud->Localisation_Tete.SortieAvalEstBifurc=vm_noeud->Localisation.SortieAvalEstBifurc;
    vm_noeud->Localisation_Tete.DistanceSortieAval=vm_noeud->Localisation.DistanceSortieAval;
    vm_noeud->Localisation_Tete.NumPeageAmont=vm_noeud->Localisation.NumPeageAmont;
    vm_noeud->Localisation_Tete.DistancePeageAmont=vm_noeud->Localisation.DistancePeageAmont;
    vm_noeud->Localisation_Tete.NumPeageAval=vm_noeud->Localisation.NumPeageAval;
    vm_noeud->Localisation_Tete.DistancePeageAval=vm_noeud->Localisation.DistancePeageAval;
    vm_noeud->Localisation_Tete.AutorouteBifAmont=vm_noeud->Localisation.AutorouteBifAmont;
    strcpy(vm_noeud->Localisation_Tete.NumAutorouteBifAmont,vm_noeud->Localisation.NumAutorouteBifAmont);
    vm_noeud->Localisation_Tete.DistanceBifAmont=vm_noeud->Localisation.DistanceBifAmont;
    vm_noeud->Localisation_Tete.AutorouteBifAval=vm_noeud->Localisation.AutorouteBifAval;
    strcpy(vm_noeud->Localisation_Tete.NumAutorouteBifAval,vm_noeud->Localisation.NumAutorouteBifAval);
    vm_noeud->Localisation_Tete.DistanceBifAval=vm_noeud->Localisation.DistanceBifAval;

    strcpy(vm_noeud->Localisation_Tete.NomSortieAmont,vm_noeud->Localisation.NomSortieAmont);
    strcpy(vm_noeud->Localisation_Tete.SortieAmontUsager,vm_noeud->Localisation.SortieAmontUsager);
    strcpy(vm_noeud->Localisation_Tete.NomSortieAval,vm_noeud->Localisation.NomSortieAval);
    strcpy(vm_noeud->Localisation_Tete.SortieAvalUsager,vm_noeud->Localisation.SortieAvalUsager);
    strcpy(vm_noeud->Localisation_Tete.NomPeageAmont,vm_noeud->Localisation.NomPeageAmont);
    strcpy(vm_noeud->Localisation_Tete.NomPeageAval,vm_noeud->Localisation.NomPeageAval);
    vm_noeud->Localisation_Tete.PRBifAmont=vm_noeud->Localisation.PRBifAmont;
    vm_noeud->Localisation_Tete.PRBifAval=vm_noeud->Localisation.PRBifAval;
    vm_noeud->Localisation_Tete.PRDebutBifAmont=vm_noeud->Localisation.PRDebutBifAmont;
    vm_noeud->Localisation_Tete.PRDebutBifAval=vm_noeud->Localisation.PRDebutBifAval;
    vm_noeud->Localisation_Tete.ZoneUrbaine=vm_noeud->Localisation.ZoneUrbaine;

    vm_noeud->Infos_FAX.PR_Min=vm_noeud->Localisation.PR;
    vm_noeud->Infos_FAX.PR_Max=vm_noeud->Localisation.PR;
  }
  /*rechercher la localisation de la tete*/
  else
  {
    /*appelle la proc XZAE;21 pour recuperer la localisation*/
    if (XZAE21_Localiser_Evt_PA(    vm_noeud->Localisation.Autoroute, vm_noeud->Localisation_Tete.PR, 
					vm_noeud->Localisation.Sens,
                                        &NumSortieAmont, &DistanceSortieAmont, &SortieAmontEstBifurc,
                                        &NumSortieAval, &DistanceSortieAval, &SortieAvalEstBifurc,
                                        &NumPeageAmont, &DistancePeageAmont,
                                        &NumPeageAval, &DistancePeageAval,
                                        &LongueurPlateformePeageAval,
                                        &NumAutorouteBifAmont, &DistanceBifAmont,
                                        &NumAutorouteBifAval, &DistanceBifAval,
                                        Descente,
                                        Tunnel,
                                        NomSortieAmont, AbrevSortieAmont, SortieAmontUsager,
                                        NomSortieAval, AbrevSortieAval, SortieAvalUsager,
                                        NomPeageAmont, AbrevPeageAmont,
                                        NomPeageAval, AbrevPeageAval,
                                        &Pente, &Site, &PrBifurcAmont, &PrBifurcAval,
                                        &PrDebutBifurcAmont,&PrDebutBifurcAval,NomViaduc,&ZoneUrbaine) != XDC_OK )
    {
      XZST_03EcritureTrace(   XZSTC_WARNING,"drol_tfm_lit_infos:  appel a XZAE21 a echoue pour la tete");
      return (XDC_NOK);
    }

    /*memorisation des infos recuperees*/
    drol_tfm_localiser(&(vm_noeud->Localisation_Tete),NumSortieAmont,DistanceSortieAmont,SortieAmontEstBifurc,
					NumSortieAval,DistanceSortieAval,SortieAvalEstBifurc,
					NumPeageAmont,DistancePeageAmont,
					NumPeageAval,DistancePeageAval,
					LongueurPlateformePeageAval,
					NumAutorouteBifAmont,DistanceBifAmont,
					NumAutorouteBifAval,DistanceBifAval,
					Descente,Tunnel,
					NomSortieAmont,AbrevSortieAmont,SortieAmontUsager,
					NomSortieAval,AbrevSortieAval,SortieAvalUsager,
					NomPeageAmont, AbrevPeageAmont,
					NomPeageAval, AbrevPeageAval,
					Pente,Site,PrBifurcAmont,PrBifurcAval,
					PrDebutBifurcAmont,PrDebutBifurcAval,NomViaduc,ZoneUrbaine);

    /*mise a jour des slots PR min et PR max*/
    if (vm_noeud->Localisation.PR > vm_noeud->Localisation_Tete.PR) {
      vm_noeud->Infos_FAX.PR_Min = vm_noeud->Localisation_Tete.PR;
      vm_noeud->Infos_FAX.PR_Max = vm_noeud->Localisation.PR;
    }
    else {
      vm_noeud->Infos_FAX.PR_Max = vm_noeud->Localisation_Tete.PR;
      vm_noeud->Infos_FAX.PR_Min = vm_noeud->Localisation.PR;
    }
  }

  /*recuperation du trafic au niveau de l'evt*/
  /*utilise pour le calcul du niveau*/
  if (va_site==vg_SitePrincipal) {
    if (XZAP25_Trafic(vm_noeud->Localisation.Autoroute,vm_noeud->Localisation.PR,
			vm_noeud->Localisation.Sens,
			va_horodate,
			&(vm_noeud->Trafic)) != XDC_OK)
    XZST_03EcritureTrace(   XZSTC_WARNING,"drol_tfm_lit_infos:  appel a XZAP25 a echoue"); 
  }

    XZST_03EcritureTrace(   XZSTC_INFO,"drol_tfm_lit_infos:  out"); 
  return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* met a jour le slot localisation
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void drol_tfm_localiser(T_Localisation *va_loc,
        XDY_Octet       NumSortieAmont,
        XDY_Entier      DistanceSortieAmont,
        XDY_Octet       SortieAmontEstBifurc,
        XDY_Octet       NumSortieAval,
        XDY_Entier      DistanceSortieAval,
        XDY_Octet       SortieAvalEstBifurc,
        XDY_Octet       NumPeageAmont,
        XDY_Entier      DistancePeageAmont,
        XDY_Octet       NumPeageAval,
        XDY_Entier      DistancePeageAval,
        XDY_Entier      LongueurPlateformePeageAval,
        XDY_Autoroute   NumAutorouteBifAmont,
        XDY_Entier      DistanceBifAmont,
        XDY_Autoroute   NumAutorouteBifAval,
        XDY_Entier      DistanceBifAval,
        char        *Descente,
        char        *Tunnel,
        char        *NomSortieAmont,
        char        *AbrevSortieAmont,
        char        *SortieAmontUsager,
        char        *NomSortieAval,
        char        *AbrevSortieAval,
        char        *SortieAvalUsager,
        char        *NomPeageAmont,
        char        *AbrevPeageAmont,
        char        *NomPeageAval,
        char        *AbrevPeageAval,
        XDY_Mot         Pente,
        XDY_Octet       Site,
        XDY_PR          PrBifurcAmont,
        XDY_PR          PrBifurcAval,
        XDY_PR          PrDebutBifurcAmont,
        XDY_PR          PrDebutBifurcAval,
	char		*NomViaduc,
	XDY_Mot		ZoneUrbaine)

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
  va_loc->NumSortieAmont=NumSortieAmont;
  va_loc->SortieAmontEstBifurc=SortieAmontEstBifurc;
  if ( (NumSortieAmont==0) || (SortieAmontEstBifurc==XDC_VRAI) )
    va_loc->DistanceSortieAmont=C_DISTANCE_INFINIE;
  else
    va_loc->DistanceSortieAmont=DistanceSortieAmont;

  va_loc->NumSortieAval=NumSortieAval;
  va_loc->SortieAvalEstBifurc=SortieAvalEstBifurc;
  if ( (NumSortieAval==0) || (SortieAvalEstBifurc==XDC_VRAI) )
    va_loc->DistanceSortieAval=C_DISTANCE_INFINIE;
  else
    va_loc->DistanceSortieAval=DistanceSortieAval;

  va_loc->NumPeageAmont=NumPeageAmont;
  XZST_03EcritureTrace(   XZSTC_WARNING,"PEAGE %s",NomPeageAmont);

  if (NumPeageAmont==0)
    va_loc->DistancePeageAmont=C_DISTANCE_INFINIE;
  else
    va_loc->DistancePeageAmont=DistancePeageAmont;

  va_loc->NumPeageAval=NumPeageAval;
  if (NumPeageAval==0)
    va_loc->DistancePeageAval=C_DISTANCE_INFINIE;
  else
    va_loc->DistancePeageAval=DistancePeageAval;

  va_loc->LongueurPlateforme=LongueurPlateformePeageAval;

  /*transformer les numeros d'autoroutes de bifurcation en nom Axx*/
  va_loc->AutorouteBifAmont=NumAutorouteBifAmont;
  va_loc->DistanceBifAmont=DistanceBifAmont;
  switch(NumAutorouteBifAmont){
  case (1): strcpy(va_loc->NumAutorouteBifAmont,"A8"); break;
  case (2): strcpy(va_loc->NumAutorouteBifAmont,"A500"); break;
  case (3): strcpy(va_loc->NumAutorouteBifAmont,"A50"); break;
  case (4): strcpy(va_loc->NumAutorouteBifAmont,"A501"); break;
  case (5): strcpy(va_loc->NumAutorouteBifAmont,"A51"); break;
  case (6): strcpy(va_loc->NumAutorouteBifAmont,"A52"); break;
  case (7): strcpy(va_loc->NumAutorouteBifAmont,"A520"); break;
  case (8): strcpy(va_loc->NumAutorouteBifAmont,"A57"); break;
  case (11): strcpy(va_loc->NumAutorouteBifAmont,"A570"); break;
  default:
    va_loc->DistanceBifAmont=C_DISTANCE_INFINIE;
    strcpy(va_loc->NumAutorouteBifAmont,"");
    break;
  }

  va_loc->AutorouteBifAval=NumAutorouteBifAval;
  va_loc->DistanceBifAval=DistanceBifAval;
  switch(NumAutorouteBifAval){
  case (1): strcpy(va_loc->NumAutorouteBifAval,"A8"); break;
  case (2): strcpy(va_loc->NumAutorouteBifAval,"A500"); break;
  case (3): strcpy(va_loc->NumAutorouteBifAval,"A50"); break;
  case (4): strcpy(va_loc->NumAutorouteBifAval,"A501"); break;
  case (5): strcpy(va_loc->NumAutorouteBifAval,"A51"); break;
  case (6): strcpy(va_loc->NumAutorouteBifAval,"A52"); break;
  case (7): strcpy(va_loc->NumAutorouteBifAval,"A520"); break;
  case (8): strcpy(va_loc->NumAutorouteBifAval,"A57"); break;
  case (11): strcpy(va_loc->NumAutorouteBifAval,"A570"); break;
  default:
    va_loc->DistanceBifAval=C_DISTANCE_INFINIE;
    strcpy(va_loc->NumAutorouteBifAval,"");
    break;
  }

  strcpy(va_loc->Descente,drol_majuscule(Descente));
  strcpy(va_loc->Tunnel,drol_majuscule(Tunnel));
  strcpy(va_loc->NomSortieAmont,drol_majuscule(NomSortieAmont));
  drol_trim(AbrevSortieAmont);
  strcpy(va_loc->AbrevSortieAmont,AbrevSortieAmont);
  strcpy(va_loc->AbrevSortieAval,AbrevSortieAval);
  strcpy(va_loc->SortieAmontUsager,drol_majuscule(SortieAmontUsager));
  strcpy(va_loc->NomSortieAval,drol_majuscule(NomSortieAval));
  strcpy(va_loc->SortieAvalUsager,drol_majuscule(SortieAvalUsager));
  strcpy(va_loc->NomPeageAmont,drol_majuscule(NomPeageAmont));
  drol_trim(AbrevPeageAmont);
  strcpy(va_loc->AbrevPeageAmont,AbrevPeageAmont);
  strcpy(va_loc->NomPeageAval,drol_majuscule(NomPeageAval));
  strcpy(va_loc->AbrevPeageAval,AbrevPeageAval);
  va_loc->Pente=Pente;
  va_loc->ZoneUrbaine  = ZoneUrbaine;
  va_loc->Site=Site;
  va_loc->PRBifAmont=PrBifurcAmont;
  va_loc->PRBifAval=PrBifurcAval;
  va_loc->PRDebutBifAmont=PrDebutBifurcAmont;
  va_loc->PRDebutBifAval=PrDebutBifurcAval;
  strcpy(va_loc->Viaduc,drol_majuscule(NomViaduc));
  return;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* fct utilisateur passee dans xzae200
* met a jour le NOEUD pour les donnees tfm
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_tfm_xzae200(char *va_autoroute,char *va_duree,char *va_secours,char *va_debut,
		char *va_localisation, char *va_neutralisation, char *va_victimes,
		char *va_gravite, char *va_vehicules, char *va_chaussee,
		char *va_conseil,char *va_voies_bloquees,char *va_gn,char *va_gv,
		char *va_fin,char *va_longueur_bouchon, char *va_periode, char *va_retour_normal,
		char *va_visibilite,char *va_cause,
		XDY_Booleen va_prevu,
		XDY_Booleen va_confirme,XDY_Booleen va_termine,
		XDY_Autoroute va_numero_autoroute,XDY_Sens va_sens,XDY_PR va_pr, XDY_PR va_pr_tete,
		XDY_Octet va_pt_car, XDY_Octet va_num_pt_car, char *va_nom_pt_car,
		int va_longueur, XDY_Octet va_BAU, XDY_Octet va_VL, XDY_Octet va_VM1,
		XDY_Octet va_VM2, XDY_Octet va_VR,
		char *va_objet_fax, char *va_lib_sens,
		XDY_Octet va_nb_vehicules, XDY_Octet va_type_meteo,
		XDY_Booleen va_personnalites, XDY_Booleen va_agent_escota,
		int va_valeur_duree, int va_distance_visibilite,
		XDY_Octet va_type_vehicule, XDY_Octet va_nb_vl, XDY_Octet va_nb_pl,
		XDY_Octet va_nb_motos, XDY_Octet va_nb_victimes,
		XDY_Octet va_nb_morts, XDY_Octet va_nb_bg, XDY_Octet va_nb_cars,
		XDY_Booleen va_matieres_dangereuses, XDY_Booleen va_animaux_vivants,
		XDY_Octet va_VL_I,XDY_Octet va_VM1_I,XDY_Octet va_VM2_I,XDY_Octet va_VR_I,
		XDY_Booleen va_voies_retrecies,
		XDY_Booleen va_entree1, XDY_Booleen va_entree2, XDY_Booleen va_sortie1, XDY_Booleen va_sortie2,
		XDY_Booleen va_type_incident, XDY_Octet va_numero_carburant,
		char *va_abrev_pt_car,XDY_Octet va_type_bouchon, char *va_tdp,
		XDY_Octet va_type_travaux, XDY_Octet va_nb_remorques,
		char *va_date_fin,
		XDY_Octet va_nb_manifestants, XDY_Booleen va_violence, XDY_Booleen va_perception_peage,
		char *va_config_voies, XDY_Octet va_datex,
		char *va_horaire_debut_prevu, char *va_horaire_fin_prevue, char *va_nuit,
		XDY_Octet va_nature_travaux, XDY_Octet va_sous_conc, XDY_Octet va_feu_maitrise,
		XDY_Octet va_cause_operation, char *va_origine,
		int va_vitesse, int va_vitesse_opposee,
		char *va_zone, char *va_scenario, char *va_sequence_vitesse, XDY_Eqt va_numero_zone,
		char *va_sortie_usager, XDY_Booleen va_indic_bifurcation, char *va_lib_cause, char *va_lib_secours,
		XDY_Horodate va_HDebut, XDY_Horodate va_HDebut_prevu, XDY_Octet va_majeur, char * va_complement_rappel,
		XDY_Mot va_type_cause, XDY_Octet va_zone_urbaine, XDY_Octet va_intervention,
		XDY_Octet va_personnel, XDY_Octet va_nb_passagers,
		int va_num_cause, XDY_Octet va_cle_cause, XDY_Octet va_ecoulement, XDY_Octet va_presence_bouchon)

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
  int vl_nb_voies=0,vl_nb_voies_cir=0;

  /*memorise les caracteristiques de l'evt*/
  XZST_03EcritureTrace(   XZSTC_FONCTION,"%s/%s/%s/%s",
				va_autoroute,va_duree,va_secours,va_debut);
  XZST_03EcritureTrace(   XZSTC_FONCTION,"%s/%s/%s/%s/%d",
			va_localisation,va_neutralisation,va_victimes,va_gravite,va_nb_victimes);
  XZST_03EcritureTrace(   XZSTC_FONCTION,"%s/%s/%s/%s",
			va_vehicules,va_chaussee,va_conseil,va_voies_bloquees);
  XZST_03EcritureTrace(   XZSTC_FONCTION,"%s/%s/%s/%s",
			va_gn,va_gv,va_fin,va_longueur_bouchon);
  XZST_03EcritureTrace(   XZSTC_FONCTION,"%s/%s/%s/%s",
			va_periode,va_retour_normal,va_visibilite,va_cause);
  XZST_03EcritureTrace(   XZSTC_FONCTION,"%d/%d/%d/%d",
			va_numero_autoroute,va_sens,va_pr,va_pr_tete);
  XZST_03EcritureTrace(   XZSTC_FONCTION,"%s/%s/%s/%d/%s",
			va_objet_fax,va_lib_sens,va_abrev_pt_car,va_indic_bifurcation,va_sortie_usager);
  XZST_03EcritureTrace(   XZSTC_FONCTION,"%d/%d/%d/%d",
			va_VL,va_VM1,va_VM2,va_VR);
  XZST_03EcritureTrace(   XZSTC_FONCTION,"%d/%d/%d/%d/%s",
			va_VL_I,va_VM1_I,va_VM2_I,va_VR_I,va_tdp);
  XZST_03EcritureTrace(   XZSTC_FONCTION,"%d/%d", va_vitesse, va_vitesse_opposee);
  XZST_03EcritureTrace(   XZSTC_WARNING,"%lf/%lf",va_HDebut,va_HDebut_prevu);
  XZST_03EcritureTrace(   XZSTC_FONCTION,"majeur %d", va_majeur);
  XZST_03EcritureTrace(   XZSTC_FONCTION,"zone_urbaine %d", va_zone_urbaine);
  XZST_03EcritureTrace(   XZSTC_FONCTION,"personnel %d",va_personnel);
  XZST_03EcritureTrace(   XZSTC_FONCTION,"cause %d/%d",va_num_cause, va_cle_cause);
  XZST_03EcritureTrace(   XZSTC_FONCTION,"ecoulement %d",va_ecoulement);
  
  strcpy(vm_noeud->Infos_TFM.Origine,drol_majuscule(va_origine));
  sprintf(vm_noeud->Infos_TFM.Autoroute,"%s, %s",va_autoroute,va_lib_sens);
  strcpy(vm_noeud->Infos_TFM.Duree,va_duree);
  strcpy(vm_noeud->Infos_TFM.Secours,va_secours);
  strcpy(vm_noeud->Infos_TFM.Debut,va_debut);
  strcpy(vm_noeud->Infos_TFM.Date_Fin,va_date_fin);
  strcpy(vm_noeud->Infos_TFM.Localisation,va_localisation);
  strcpy(vm_noeud->Infos_TFM.Neutralisation,va_neutralisation);
  strcpy(vm_noeud->Infos_TFM.Victimes,va_victimes);
  strcpy(vm_noeud->Infos_TFM.Gravite,va_gravite);
  strcpy(vm_noeud->Infos_TFM.Vehicules,va_vehicules);
  strcpy(vm_noeud->Infos_TFM.Chaussee_Glissante,va_chaussee);
  strcpy(vm_noeud->Infos_TFM.Conseil,va_conseil);
  strcpy(vm_noeud->Infos_TFM.Voies_Bloquees,va_voies_bloquees);
  strcpy(vm_noeud->Infos_TFM.Groupe_Nominal,drol_majuscule(va_gn));
  strcpy(vm_noeud->Infos_TFM.Groupe_Verbal,drol_majuscule(va_gv));
  strcpy(vm_noeud->Infos_TFM.Fin,va_fin);
  strcpy(vm_noeud->Infos_TFM.Longueur_Bouchon,va_longueur_bouchon);
  strcpy(vm_noeud->Infos_TFM.Periode,va_periode);
  strcpy(vm_noeud->Infos_TFM.Retour_Normal,va_retour_normal);
  strcpy(vm_noeud->Infos_TFM.Visibilite,va_visibilite);
  strcpy(vm_noeud->Infos_TFM.Cause,va_cause);
  strcpy(vm_noeud->Infos_TFM.TDP,va_tdp); 
  strcpy(vm_noeud->Infos_TFM.Lib_Cause,va_lib_cause);
  strcpy(vm_noeud->Infos_TFM.Lib_Secours,va_lib_secours);
  vm_noeud->Infos_TFM.Majeur = va_majeur;
  vm_noeud->Infos_TFM.Intervention = va_intervention;

  vm_noeud->Infos_TFM.Prevu=va_prevu;
  vm_noeud->Infos_TFM.Type_Cause=va_type_cause;
  XZST_03EcritureTrace(   XZSTC_WARNING,"Prevu %d",vm_noeud->Infos_TFM.Prevu);
  vm_noeud->Infos_TFM.Signale=!(va_confirme);
  vm_noeud->Infos_TFM.Confirme=va_confirme;
  vm_noeud->Infos_TFM.Termine=va_termine;
  XZST_03EcritureTrace(   XZSTC_WARNING,"Termine %d",vm_noeud->Infos_TFM.Termine);
  strcpy(vm_noeud->Infos_TFM.Config_Voies,va_config_voies); 
  vm_noeud->Infos_TFM.Datex=va_datex; /*SAGA*/
  strcpy(vm_noeud->Infos_TFM.Horaire_Debut_Prevu, va_horaire_debut_prevu);
   XZST_03EcritureTrace(   XZSTC_FONCTION,"Horaire_Debut_Prevu %s // Horaire_Fin_Prevue %s",
			va_horaire_debut_prevu,va_horaire_fin_prevue);
  strcpy(vm_noeud->Infos_TFM.Horaire_Fin_Prevue, va_horaire_fin_prevue);
  strcpy(vm_noeud->Infos_TFM.Nuit, va_nuit);
  vm_noeud->Infos_TFM.HDebut=va_HDebut;
  vm_noeud->Infos_TFM.HDebut_Prevu=va_HDebut_prevu;
   XZST_03EcritureTrace(   XZSTC_FONCTION,"nuit %s",vm_noeud->Infos_TFM.Nuit);
  vm_noeud->Infos_TFM.Nature_Travaux = va_nature_travaux;
  if (va_sous_conc!=0 )
    vm_noeud->Infos_TFM.Aire_Service=TRUE;
  else
    vm_noeud->Infos_TFM.Aire_Service=FALSE;

  vm_noeud->Localisation.Autoroute=va_numero_autoroute;
  switch(va_numero_autoroute){
  case (1): strcpy(vm_noeud->Localisation.NomAutoroute,"A8"); break;
  case (2): strcpy(vm_noeud->Localisation.NomAutoroute,"A500"); break;
  case (3): strcpy(vm_noeud->Localisation.NomAutoroute,"A50"); break;
  case (4): strcpy(vm_noeud->Localisation.NomAutoroute,"A501"); break;
  case (5): strcpy(vm_noeud->Localisation.NomAutoroute,"A51"); break;
  case (6): strcpy(vm_noeud->Localisation.NomAutoroute,"A52"); break;
  case (7): strcpy(vm_noeud->Localisation.NomAutoroute,"A520"); break;
  case (8): strcpy(vm_noeud->Localisation.NomAutoroute,"A57"); break;
  default: strcpy(vm_noeud->Localisation.NomAutoroute,"");break;
  }
  vm_noeud->Localisation.Sens=va_sens;
  vm_noeud->Localisation.PR=va_pr;
  vm_noeud->Localisation.Longueur = va_longueur;
  vm_noeud->Localisation.BAU = va_BAU;
  vm_noeud->Localisation.VL = va_VL;
  vm_noeud->Localisation.VM1 = va_VM1;
  vm_noeud->Localisation.VM2 = va_VM2;
  vm_noeud->Localisation.VR = va_VR;
  vm_noeud->Localisation.VL_I = va_VL_I;
  vm_noeud->Localisation.VM1_I = va_VM1_I;
  vm_noeud->Localisation.VM2_I = va_VM2_I;
  vm_noeud->Localisation.VR_I = va_VR_I;

  drol_cumul_voies( va_VL , &vl_nb_voies , &vl_nb_voies_cir);
  drol_cumul_voies( va_VM1 , &vl_nb_voies , &vl_nb_voies_cir);
  drol_cumul_voies( va_VM2 , &vl_nb_voies , &vl_nb_voies_cir);
  drol_cumul_voies( va_VR, &vl_nb_voies , &vl_nb_voies_cir);
  vm_noeud->Localisation.NbVoies = vl_nb_voies;
  vm_noeud->Localisation.NbVoiesCirculees = vl_nb_voies_cir;
  vm_noeud->Localisation.Voies_Retrecies=va_voies_retrecies;

  if (va_pr_tete==0) 
    vm_noeud->Localisation_Tete.PR = 0;
  else
    vm_noeud->Localisation_Tete.PR=va_pr_tete;
  vm_noeud->Localisation_Tete.NumSortieAval=0;
  vm_noeud->Localisation_Tete.NumSortieAmont=0;
  strcpy(vm_noeud->Localisation_Tete.NumAutorouteBifAval,"");
  vm_noeud->Localisation.NumPointCar=va_num_pt_car;
  strcpy(vm_noeud->Localisation.NomPointCar,drol_majuscule(va_nom_pt_car));
  strcpy(vm_noeud->Localisation.AbrevPointCar,drol_majuscule(va_abrev_pt_car));
  strcpy(vm_noeud->Localisation.SortieUsager,drol_majuscule(va_sortie_usager));
  vm_noeud->Localisation.Bifurcation=va_indic_bifurcation;
  vm_noeud->Localisation.TypePointCar=va_pt_car;

  vm_noeud->Modele_TFM=(T_Modele *)malloc(sizeof(T_Modele));

  /*assigner les infos pour le fax*/
  strcpy(vm_noeud->Infos_FAX.Objet, va_objet_fax);
  strcpy(vm_noeud->Infos_FAX.Sens, va_lib_sens);
  vm_noeud->Infos_FAX.Nb_Vehicules = va_nb_vehicules;
  vm_noeud->Infos_FAX.Type_Meteo = va_type_meteo;
  vm_noeud->Infos_FAX.Personnalites = va_personnalites;
  vm_noeud->Infos_FAX.Agent_Escota = va_agent_escota;
  vm_noeud->Infos_FAX.Duree = va_valeur_duree;
  vm_noeud->Infos_FAX.Visibilite = va_distance_visibilite;
  vm_noeud->Infos_FAX.Type_Vehicule = va_type_vehicule;
  vm_noeud->Infos_FAX.Nb_Passagers = va_nb_passagers;
  vm_noeud->Infos_FAX.Nb_VL = va_nb_vl;
  vm_noeud->Infos_FAX.Nb_PL = va_nb_pl;
  vm_noeud->Infos_FAX.Nb_Motos = va_nb_motos;
  vm_noeud->Infos_FAX.Nb_Remorques = va_nb_remorques;
  vm_noeud->Infos_FAX.Nb_Victimes = va_nb_victimes;
  vm_noeud->Infos_FAX.Nb_Morts = va_nb_morts;
  vm_noeud->Infos_FAX.Nb_BG = va_nb_bg;
  vm_noeud->Infos_FAX.Nb_Cars = va_nb_cars;
  vm_noeud->Infos_FAX.Type_Incident = va_type_incident;
  vm_noeud->Infos_FAX.Matieres_Dangereuses = va_matieres_dangereuses;
  vm_noeud->Infos_FAX.Animaux_Vivants = va_animaux_vivants;
  vm_noeud->Infos_FAX.Entree1 = va_entree1;
  vm_noeud->Infos_FAX.Entree2 = va_entree2;
  vm_noeud->Infos_FAX.Sortie1 = va_sortie1;
  vm_noeud->Infos_FAX.Sortie2 = va_sortie2;
  vm_noeud->Infos_FAX.Numero_Carburant = va_numero_carburant;
  vm_noeud->Infos_FAX.Type_Bouchon = va_type_bouchon;
  vm_noeud->Infos_TFM.Type_Travaux = va_type_travaux;
  vm_noeud->Infos_FAX.Nb_Manifestants = va_nb_manifestants;
  vm_noeud->Infos_FAX.Violence = va_violence;
  vm_noeud->Infos_FAX.Perception_Peage = va_perception_peage;
  vm_noeud->Infos_FAX.Feu_Maitrise = va_feu_maitrise;
  vm_noeud->Infos_FAX.Cause_Operation = va_cause_operation;
  vm_noeud->Infos_FAX.Vitesse = va_vitesse;
  vm_noeud->Infos_FAX.Vitesse_Opposee = va_vitesse_opposee;
  vm_noeud->Infos_FAX.Personnel = va_personnel;
  vm_noeud->Infos_FAX.Ecoulement = va_ecoulement;
  vm_noeud->Infos_FAX.Presence_Bouchon = va_presence_bouchon;
  strcpy(vm_noeud->Infos_FAX.Zone , va_zone);
  strcpy(vm_noeud->Infos_FAX.Scenario , va_scenario);
  strcpy(vm_noeud->Infos_FAX.Transitoire , va_sequence_vitesse);
  strcpy(vm_noeud->Infos_FAX.Complement_Rappel, va_complement_rappel);
  vm_noeud->Infos_FAX.Numero_Zone = va_numero_zone;
  vm_noeud->Infos_FAX.Numero_Cause = va_num_cause;
  vm_noeud->Infos_FAX.Cle_Cause = va_cle_cause;
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_xzae200 : INCIDENT %d",va_type_incident);
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_xzae200 : transitoire %s",va_sequence_vitesse);
  /*assigner le bon modele de message*/
  /*si evt prevu*/
  if (vm_noeud->Infos_TFM.Termine==1) {
    vm_noeud->Modele_TFM = pm_fin;
    vm_noeud->Nombre_Lexemes = vm_fin_nb_lexemes;
  }
  else if (vm_noeud->Infos_TFM.Prevu==1){
    vm_noeud->Modele_TFM = pm_prevision;
    vm_noeud->Nombre_Lexemes = vm_prevision_nb_lexemes;
  }
  /*si evt signale*/
  else if (vm_noeud->Infos_TFM.Signale==1) {
    vm_noeud->Modele_TFM = pm_prealerte;
    vm_noeud->Nombre_Lexemes = vm_prealerte_nb_lexemes;
  }
  /*si evt confirme et non termine*/
  else if (vm_noeud->Infos_TFM.Confirme==1) {
    vm_noeud->Modele_TFM = pm_alerte;
    vm_noeud->Nombre_Lexemes = vm_alerte_nb_lexemes;
  }
  else
    return (XDC_NOK);

  XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_tfm_xzae200 : nb lexemes = %d",vm_noeud->Nombre_Lexemes);
  return (XDC_OK);
}

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/
static char *drol_tfm_null() { return (""); }

static char *drol_tfm_autoroute() { return (vm_noeud->Infos_TFM.Autoroute); } 
static char *drol_tfm_duree() { return (vm_noeud->Infos_TFM.Duree); }
static char *drol_tfm_secours() { return (vm_noeud->Infos_TFM.Secours); }
static char *drol_tfm_debut() { return (vm_noeud->Infos_TFM.Debut); }
static char *drol_tfm_neutralisation() { return (vm_noeud->Infos_TFM.Neutralisation); }
static char *drol_tfm_victimes() { return (vm_noeud->Infos_TFM.Victimes);}
static char *drol_tfm_gravite() { return (vm_noeud->Infos_TFM.Gravite);}
static char *drol_tfm_evt_GV() { return (vm_noeud->Infos_TFM.Groupe_Verbal);}
static char *drol_tfm_evt_GN() { return (vm_noeud->Infos_TFM.Groupe_Nominal);}
static char *drol_tfm_longueur_bouchon() { return (vm_noeud->Infos_TFM.Longueur_Bouchon);}
static char *drol_tfm_visibilite() { return (vm_noeud->Infos_TFM.Visibilite);}
static char *drol_tfm_glisse() { return (vm_noeud->Infos_TFM.Chaussee_Glissante);}
static char *drol_tfm_conseil() { return (vm_noeud->Infos_TFM.Conseil);}
static char *drol_tfm_retour_normal() {return (vm_noeud->Infos_TFM.Retour_Normal);}
static char *drol_tfm_fin() {return (vm_noeud->Infos_TFM.Fin);}
static char *drol_tfm_periode() { return (vm_noeud->Infos_TFM.Periode);}
static char *drol_tfm_vehicules() {return (vm_noeud->Infos_TFM.Vehicules);}
static char *drol_tfm_voies_bloquees() {return (vm_noeud->Infos_TFM.Voies_Bloquees);}
static char *drol_tfm_zone() { return (vm_noeud->Infos_FAX.Zone);}
static char *drol_tfm_scenario() { return (vm_noeud->Infos_FAX.Scenario);}
static char *drol_tfm_transitoire() { 
  static char vl_str[400];
  char *vl_str2;
  char vl_str3[500];
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

static char *drol_tfm_tdp() 
{
 vm_str[0]='\0';
 strcpy(vm_str,vm_noeud->Infos_TFM.TDP);
return (vm_str);
}

static char *drol_tfm_cause() 
{ 
  if (vm_flag_cause)
    return (vm_noeud->Infos_TFM.Cause); 
else
    return ("");
}

 

static char *drol_tfm_bilan_materiel()
{ 
  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :bilan_materile");
  /* si accident prendre nb sinon type (panne arret)*/
  vm_str[0]='\0';
  vm_str1[0]='\0';
   if (vm_noeud->Type == XZAEC_FMC_Contresens || vm_noeud->Type == XZAEC_FMC_VehArrete || vm_noeud->Type == XZAEC_FMC_VehFeu || vm_noeud->Type == XZAEC_FMC_VehPanne)
   {
    if (vm_noeud->Infos_FAX.Type_Vehicule==XZAEC_VEH_PL)
      strcpy(vm_str,"PL,");
    else if (vm_noeud->Infos_FAX.Type_Vehicule==XZAEC_VEH_VL)
      strcpy(vm_str,"VL,");
    else if (vm_noeud->Infos_FAX.Type_Vehicule==XZAEC_VEH_Car)
      strcpy(vm_str,"CAR,");
    else if (vm_noeud->Infos_FAX.Type_Vehicule==XZAEC_VEH_Moto)
      strcpy(vm_str,"MOTO,");
    else if (vm_noeud->Infos_FAX.Type_Vehicule==XZAEC_VEH_Remorque)
      strcpy(vm_str,"REMORQUE,");
    else
      strcpy(vm_str,"VL,");
  }
  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :bilan_materile %s",vm_str);
  if  (vm_noeud->Type == XZAEC_FMC_Accident)
	{ vm_str[0]='\0';
  if ( vm_noeud->Infos_FAX.Nb_Cars >0)
	sprintf(vm_str,"%d BUS,",vm_noeud->Infos_FAX.Nb_Cars);
  
  if ( vm_noeud->Infos_FAX.Nb_PL >0)
	sprintf(vm_str,"%s %d PL,",vm_str,vm_noeud->Infos_FAX.Nb_PL);
  
  if ( vm_noeud->Infos_FAX.Nb_VL >0)
	sprintf(vm_str,"%s %d VL,",vm_str,vm_noeud->Infos_FAX.Nb_VL);
  
  if ( vm_noeud->Infos_FAX.Nb_Motos >0)
	sprintf(vm_str,"%s %d MOTO,",vm_str,vm_noeud->Infos_FAX.Nb_Motos);
  
  if ( vm_noeud->Infos_FAX.Nb_Remorques >0)
	sprintf(vm_str,"%s %d REMORQUE,",vm_str,vm_noeud->Infos_FAX.Nb_Remorques);
    }
  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :bilan_materile %s",vm_str);
    if (vm_str[0]!='\0') {
	vm_str[strlen(vm_str)-1]='\0';
	sprintf(vm_str1,"- BILAN MATERIEL : %s\n",vm_str);
	}
  XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  :bilan_materile %s",vm_str1);
    return (vm_str1);
}

static char *drol_tfm_voies_impactees()
{ 
  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :voies impact");
    vm_str[0]='\0';
    vm_str1[0]='\0';
    if (vm_noeud->Type == XZAEC_FMC_EvtHorsReseau || vm_noeud->Type == XZAEC_FMC_Autre || vm_noeud->Type == XZAEC_FMC_Regulation || vm_noeud->Type == XZAEC_FMC_AireFermee || vm_noeud->Type == XZAEC_FMC_IncSsConces || vm_noeud->Type == XZAEC_FMC_FermetureEch)
    	return ("");

    if ( (vm_noeud->Localisation.VR_I==XDC_VOIE_SENS_INVERSE) || (vm_noeud->Localisation.VM2_I ==XDC_VOIE_SENS_INVERSE) || (vm_noeud->Localisation.VM1_I ==XDC_VOIE_SENS_INVERSE ))
	sprintf(vm_str,"CIRCULATION A DOUBLE SENS SUR LA CHAUSSEE OPPOSEE");
    else
	{
	if (( vm_noeud->Localisation.VR == XDC_VOIE_BLOQUEE) || (vm_noeud->Localisation.VR == XDC_VOIE_NEUTRALISEE) || (vm_noeud->Localisation.VR == XDC_VOIE_BLOQUEE_NEUTRALISEE ))
		sprintf(vm_str," VG,");
	if ( (vm_noeud->Localisation.VM2 == XDC_VOIE_BLOQUEE) || (vm_noeud->Localisation.VM2 == XDC_VOIE_NEUTRALISEE) || (vm_noeud->Localisation.VM2 == XDC_VOIE_BLOQUEE_NEUTRALISEE ))
		sprintf(vm_str,"%s VM2,",vm_str);
	if ( (vm_noeud->Localisation.VM1 == XDC_VOIE_BLOQUEE) || (vm_noeud->Localisation.VM1 == XDC_VOIE_NEUTRALISEE) || (vm_noeud->Localisation.VM1 == XDC_VOIE_BLOQUEE_NEUTRALISEE ))
		sprintf(vm_str,"%s VM1,",vm_str);
	if ( (vm_noeud->Localisation.VL == XDC_VOIE_BLOQUEE) || (vm_noeud->Localisation.VL == XDC_VOIE_NEUTRALISEE) || (vm_noeud->Localisation.VL == XDC_VOIE_BLOQUEE_NEUTRALISEE ))
		sprintf(vm_str,"%s VD,",vm_str);
	if ( (vm_noeud->Localisation.BAU == XDC_VOIE_BLOQUEE) || (vm_noeud->Localisation.BAU == XDC_VOIE_NEUTRALISEE) || (vm_noeud->Localisation.BAU == XDC_VOIE_BLOQUEE_NEUTRALISEE ))
		sprintf(vm_str,"%s BAU,",vm_str);
	}
    if (vm_str[0]!='\0') {
	vm_str[strlen(vm_str)-1]='\0';
	sprintf(vm_str1,"- VOIES IMPACTEES :%s\n",vm_str);
	}
	else	
	sprintf(vm_str1,"- VOIES IMPACTEES : NEANT\n");
  XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  : voies impact");
    return (vm_str1);
}

static char *drol_tfm_voies_restantes()
{ 
    if (vm_noeud->Type == XZAEC_FMC_EvtHorsReseau || vm_noeud->Type == XZAEC_FMC_Autre || vm_noeud->Type == XZAEC_FMC_Regulation || vm_noeud->Type == XZAEC_FMC_AireFermee || vm_noeud->Type == XZAEC_FMC_IncSsConces || vm_noeud->Type == XZAEC_FMC_FermetureEch)
    	return ("");
 sprintf(vm_str,"- VOIES RESTANTES : %d VOIES RESTANTES\n",vm_noeud->Localisation.NbVoiesCirculees);
    return (vm_str);
}
static char *drol_tfm_cause_ou_nature()
{   
XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :cause nature %s#",vm_noeud->Infos_TFM.Lib_Cause);
vm_str[0]='\0';
  if (strlen(vm_noeud->Infos_TFM.Lib_Cause)>2)
    sprintf(vm_str,"- CAUSE OU NATURE : %s\n",vm_noeud->Infos_TFM.Lib_Cause); 

XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  :cause nature");

    return (vm_str);   
}
static char *drol_tfm_bilan_corporel()
{ 
  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :bilan_corp");
  int vl_bl=0;

  vm_str1[0]='\0';
  vm_str2[0]='\0';
  vm_str[0]='\0';
  if ( vm_noeud->Infos_FAX.Nb_Morts >0)
	sprintf(vm_str1,"%d MORTS,",vm_noeud->Infos_FAX.Nb_Morts);
  vl_bl=vm_noeud->Infos_FAX.Nb_Victimes-vm_noeud->Infos_FAX.Nb_Morts-vm_noeud->Infos_FAX.Nb_BG;
  
  if ( vl_bl >0)
	sprintf(vm_str1,"%s %d BLESSES LEGERS,",vm_str1,vl_bl);
  
  if (vm_noeud->Infos_FAX.Nb_BG > 0) 
	sprintf(vm_str1,"%s %d BLESSES GRAVES,",vm_str1,vm_noeud->Infos_FAX.Nb_BG);  	
   
   if (vm_str1[0]!='\0') {
	vm_str1[strlen(vm_str1)-1]='\0';
	sprintf(vm_str,"- BILAN CORPOREL : %s\n",vm_str1);
	}
      XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  :bilan corp");

    return (vm_str);
    
}
static char *drol_tfm_appels_majeur()
{ 
 char vl_remarque[251];
  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :drol_tfm_appels_majeur %d", vg_numero_plan);
  vm_str[0]='\0';

  if (XZAP180_Lire_Prop_Appel_Majeur(vg_numero_plan,vl_remarque) != XDC_OK) {
    XZST_03EcritureTrace(   XZSTC_WARNING, "pb appel a XZAP180_Lire_Prop_Appel_Majeur");
    return (vm_str);
  }
  XDY_Supprimer_Espaces_De_Fin(vl_remarque);

  sprintf(vm_str, "\nECHANGEURS ET BARRIERES FERMES : %s",vl_remarque);
  XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  : drol_tfm_appels_majeur (%s)",
  		vm_str);
  return (vm_str);
}
static char *drol_tfm_secours_bref () 
{ 	vm_str[0]='\0';
	if (vm_noeud->Infos_TFM.Lib_Secours[0]!='\0')
		sprintf(vm_str,"- SERVICES SUR PLACE : %s\n",vm_noeud->Infos_TFM.Lib_Secours);

	return (vm_str);
}


/********************************************************************/
/********************************************************************/
/********************************************************************/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de generation du message TFM
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *drol_tfm_genere(T_NOEUD *va_noeud,XDY_Booleen va_flag_cause)

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
  static char vl_message[10000]="";
  char vl_i=0;
  char * (*vl_fct)();
  char vl_date[30];
  int vl_jour,vl_mois,vl_annee,vl_heure,vl_minute,vl_seconde;
  char vl_str[30];
  char vl_message_datex[300];
  char vl_emetteur[101];
  int vl_datex_trouve = FALSE;

  vm_noeud = va_noeud;
  vm_flag_cause = va_flag_cause;

  XZST_03EcritureTrace(   XZSTC_WARNING, "IN  :drol_tfm_genere %d %d, nb lexemes %d, type %d",
				va_noeud->NumEvt.numero,
				va_noeud->NumEvt.cle,
				vm_noeud->Nombre_Lexemes,
				vm_noeud->Type);

  strcpy(vl_message,"");
    /* SAGA*/
    /*rechercher si on a recu un message DATEX du partenaire*/
    /*

    if (XZAE150_Message_Datex(vm_noeud->NumEvt.numero,
	    		      vm_noeud->NumEvt.cle,
			      vl_emetteur,
		              vl_message_datex) != XDC_OK) {
    	XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :drol_tfm_genere pb appel a XZAE150");
    }
    else {
      XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :drol_tfm_genere XZAE150 = %s/%s",
			  vl_emetteur, vl_message_datex);
      if (vl_message_datex[0] != 0)
      {
	if (!strcmp(vl_emetteur,"DDE83"))
        	sprintf(vl_message,"La DDE du Var nous informe :\n\n%s",
				   vl_message_datex);
        else
		sprintf(vl_message,"Le partenaire %s nous informe : \n\n%s",
				vl_emetteur,
				vl_message_datex);
	vl_datex_trouve = TRUE;
      }
   }
   */

   /*si pas de message DATEX*/
   if (!vl_datex_trouve) {
  /*si FMC travaux, mettre une etoile devant*/
  if (vm_noeud->Type == XZAEC_FMC_Travaux)
    strcpy(vl_message,"* ");

  /*concatene tous les elements de messages*/
  for (vl_i=0;vl_i<vm_noeud->Nombre_Lexemes;vl_i++) {
    vl_fct = vm_noeud->Modele_TFM[vl_i].Fct;
    strcat(vl_message,vm_noeud->Modele_TFM[vl_i].Valeur);
    strcat(vl_message,(*vl_fct)());
  }
  }
  XZST_03EcritureTrace(   XZSTC_WARNING, "OUT drol_tfm_genere, MSG = %s",vl_message);
  return (vl_message);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Retour de la date courante
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char * drol_tfm_get_heure(char *pa_texte_date)
/*
* ARGUMENTS EN ENTREE :
*   Aucun
*
* ARGUMENTS EN SORTIE :
*   Date courante sous forme textuelle.
*
* CODE RETOUR :
*   Date courante sous forme textuelle.
*
* CONDITION D'UTILISATION
*
* FONCTION
------------------------------------------------------*/
{
XZSMT_Horodate  vl_HorodateZero = "";
XZSMT_Horodate  vl_Horodate     = "";
double          vl_HorodateSec  = 0.0;
int             vl_JourSemaine  = 0;

XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_HorodateSec);
XZSM_11ConversionHorodate(vl_HorodateSec, &vl_HorodateZero);

strcpy(pa_texte_date,vl_HorodateZero);

return(pa_texte_date);
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Constitution de la signature
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *drol_tfm_signature(XDY_Horodate va_horodate)

/*
* ARGUMENTS EN ENTREE :
*   Date courante (texte).
*
* ARGUMENTS EN SORTIE :
*   Aucun.
*
* CODE RETOUR :
*   Texte de signature.
*
* CONDITION D'UTILISATION
*
* FONCTION
------------------------------------------------------*/
{
  char vl_date[30];
  int vl_jour,vl_mois,vl_annee,vl_heure,vl_minute,vl_seconde;
  static char vl_signature[64];

  /*ajout de la signature*/
  drol_tfm_get_heure(vl_date);
  sscanf( vl_date, "%2d/%2d/%4d %2d:%2d:%2d", &vl_jour, &vl_mois,
		&vl_annee, &vl_heure, &vl_minute, &vl_seconde) ;
  sprintf(vl_signature,"\nCENTRE INFORMATION TRAFIC LE %02d/%02d/%04d A %02d:%02d",
			vl_jour,
			vl_mois,
			vl_annee,
			vl_heure,
			vl_minute);
  return (vl_signature);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* retourne le libelle de la phrase consequence
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *drol_tfm_consequence(T_NOEUD *va_noeud)

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
  vm_noeud=va_noeud;
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_consequence = %s",drol_tfm_cause());
  return (drol_tfm_cause());
}


/*X*/
/*----------------------------------------------------------------------------
* SERVICE RENDU :
*  Construit et retourne la localisation d'un evenement.
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *drol_tfm_localisation()
/*
* ARGUMENTS EN ENTREE :
*   Aucun
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* VALEUR DE RETOUR :
*   Texte de localisation d'un evenement.
*
* CONDITION D'UTILISATION
*
* FONCTION
----------------------------------------------------------------------------*/
{
  T_Localisation vl_loc;
  T_Localisation vl_loc_tete;
  char	vl_str[150];

  vl_loc=vm_noeud->Localisation;
  vl_loc_tete=vm_noeud->Localisation_Tete;

  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : SAM %d[%d], BAM %d[%d], SAV %d[%d], BAV %d[%d]",
						vl_loc.NumSortieAmont, vl_loc.DistanceSortieAmont,
						vl_loc.AutorouteBifAmont, vl_loc.DistanceBifAmont,
						vl_loc_tete.NumSortieAval, vl_loc_tete.DistanceSortieAval,
						vl_loc_tete.AutorouteBifAval, vl_loc_tete.DistanceBifAval);
  strcpy(vm_local,""); 
  /*cas particulier des evenements relatifs a un echangeur*/
  if (vm_noeud->Type == XZAEC_FMC_FermetureEch  ||  vm_noeud->Type == XZAEC_FMC_EchDeconseille)
  {
    sprintf(vm_local,"");
    return (vm_local);
  }

  /*localisation sur une bretelle d'entree*/
  if ( (vl_loc.TypePointCar == XDC_POINT_CARACT_ECHANGEUR) &&
	    ( (vl_loc.Sens == XDC_SENS_ENTREE_SUD) || (vl_loc.Sens == XDC_SENS_ENTREE_NORD)) )
  {
    if (vl_loc.Bifurcation==XDC_VRAI)
    {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : bretelle bifurc d'entree");
      sprintf(vm_local," DANS LA BRETELLE DE LA BIFURCATION %s.",vl_loc.AbrevPointCar);
    }
    else
    {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : bretelle d'entree");
    sprintf(vm_local," DANS LA BRETELLE D'ENTREE DE L'ECHANGEUR %s %s.",
		vl_loc.SortieUsager,
		vl_loc.NomPointCar);
    }
    return (vm_local);
  }

  /*localisation sur une bretelle de sortie*/
  if ( (vl_loc.TypePointCar == XDC_POINT_CARACT_ECHANGEUR) &&
	    ( (vl_loc.Sens == XDC_SENS_SORTIE_SUD) || (vl_loc.Sens == XDC_SENS_SORTIE_NORD)) )
  {
    if (vl_loc.Bifurcation==XDC_VRAI)
    {   
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : bretelle bifurc de sortie");
    sprintf(vm_local," DANS LA BRETELLE DE LA BIFURCATION %s.",vl_loc.AbrevPointCar);
    }
    else
    {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : bretelle de sortie");
    sprintf(vm_local," DANS LA BRETELLE DE SORTIE DE L'ECHANGEUR %s %s.",
		vl_loc.SortieUsager,
		vl_loc.NomPointCar);
    }
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

  /*localisation dans un tunnel*/
  if (strcmp(vl_loc.Tunnel,""))
  {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : tunnel");
    sprintf(vm_local," DANS LE TUNNEL DE %s,",
			vl_loc.Tunnel);
  /*localisation sur lit d arret*/
  if (strcmp(vl_str,"")) {
    if ( (vl_loc.TypePointCar == XDC_POINT_CARACT_LIT_ARRET) && 
         !(vm_noeud->Infos_TFM.Termine) ) {
      sprintf(vm_local,"%s LE LIT D ARRET NUMERO %d EST OCCUPE. ",
			vm_local,
			vl_loc.NumPointCar);
    }
    return (vm_local);
  }
  }
  else {
    strcpy(vm_local, "");
  }

  /*les evenements signales et de certains types sont localises entre deux echangurs ou bifurcations*/
  if ( (vm_noeud->Infos_TFM.Signale) ||
       (vm_noeud->Type == XZAEC_FMC_Meteo) ||
       (vm_noeud->Type == XZAEC_FMC_VehLent) ||
       (vm_noeud->Type == XZAEC_FMC_Regulation) ||
       (vm_noeud->Type == XZAEC_FMC_ZoneStockage) ||
       (vm_noeud->Type == XZAEC_FMC_TraitChaussees) )
  {
    /*entre sortie amont et sortie aval*/
    if ( (vl_loc.DistanceSortieAmont <= vl_loc.DistanceBifAmont) &&
       (vl_loc_tete.DistanceSortieAval <= vl_loc_tete.DistanceBifAval) &&
       (vl_loc.NumSortieAmont != 0) &&
       (vl_loc.NumSortieAval != 0) &&
       (vl_loc_tete.NumSortieAval != 0) &&
       (vl_loc_tete.NumSortieAmont !=0))
    {
      XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : entre sortie amont et sortie aval");
      sprintf(vl_str," ENTRE LA SORTIE %s %s ET LA SORTIE %s %s.",
			vl_loc.SortieAmontUsager,
			vl_loc.NomSortieAmont,
			vl_loc_tete.SortieAvalUsager,
			vl_loc_tete.NomSortieAval);
    }
    /*localisation entre sortie amont et bifurc aval*/
    else if ( (vl_loc.DistanceSortieAmont <= vl_loc.DistanceBifAmont) &&
	      (vl_loc_tete.DistanceSortieAval > vl_loc_tete.DistanceBifAval) &&
	      (vl_loc.NumSortieAmont != 0) &&
	      (strcmp(vl_loc_tete.NumAutorouteBifAval,"")) )
    {
      XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : entre sortie amont et bifurc aval");
      sprintf(vl_str," ENTRE LA SORTIE %s %s ET LA BIFURCATION VERS %s.",
			vl_loc.SortieAmontUsager,
			vl_loc.NomSortieAmont,
			vl_loc_tete.NumAutorouteBifAval);
    }
    /*localisation entre bifurc amont et sortie aval*/
    else if ( (vl_loc.DistanceSortieAmont > vl_loc.DistanceBifAmont) &&
	      (vl_loc_tete.DistanceSortieAval <= vl_loc_tete.DistanceBifAval) &&
	      (vl_loc_tete.NumSortieAval != 0) &&
	      (strcmp(vl_loc.NumAutorouteBifAmont,"")) )
    {  
      XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : entre bifurc amont et sortie aval");
      sprintf(vl_str," ENTRE LA BIFURCATION VERS %s ET LA SORTIE %s %s.",
			vl_loc.NumAutorouteBifAmont,
			vl_loc_tete.SortieAvalUsager,
			vl_loc_tete.NomSortieAval);
    }
    /*localisation entre bifurc amont et bifurc aval*/
    else if ( (vl_loc.DistanceSortieAmont > vl_loc.DistanceBifAmont) &&
	      (vl_loc_tete.DistanceSortieAval > vl_loc_tete.DistanceBifAval) )
    {
      XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : entre bifurc amont et bifurc aval");
      sprintf(vl_str," ENTRE LA BIFURCATION %s ET LA BIFURCATION %s.",
			vl_loc.NumAutorouteBifAmont,
			vl_loc_tete.NumAutorouteBifAval);
    } 
    /*localisation entre peage et sortie avavl*/
    else if ( (vl_loc_tete.NumSortieAval != 0) &&
	      (vl_loc.NumSortieAmont==0) )
    {
      XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : entre limite amont et sortie aval");
      sprintf(vl_str," ENTRE %s ET LA SORTIE %s %s.",
				drol_msg_limites_amont(vl_loc.Autoroute,vl_loc.Sens),
				vl_loc_tete.SortieAvalUsager,
				vl_loc_tete.NomSortieAval);
   }/*localisation entre peage et sortie avavl*/
   else if ( (vl_loc_tete.NumSortieAval == 0) &&
	     (vl_loc.NumSortieAmont!=0) )
   {
      XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : entre sortie amont et limite aval");
      sprintf(vl_str," ENTRE LA SORTIE %s %s ET %s %s.",
				vl_loc.SortieAmontUsager,
    	  			vl_loc.NomSortieAmont,
				drol_msg_limites_aval(vl_loc.Autoroute,vl_loc.Sens),
				drol_msg_kilometre(vl_loc_tete.PR,vl_loc.PR,vl_loc.Sens,vl_loc.Autoroute));
    }
    else {
/*      sprintf(vm_local," ENTRE LA SORTIE %s %s ET %s %s.",
				vl_loc.SortieAmontUsager,
				vl_loc.NomSortieAmont,   DEM 1084*/
      sprintf(vl_str," ENTRE %s ET %s %s.",
				drol_msg_limites_amont(vl_loc.Autoroute,vl_loc.Sens),
				drol_msg_limites_aval(vl_loc.Autoroute,vl_loc.Sens),
				drol_msg_kilometre(vl_loc_tete.PR,vl_loc.PR,vl_loc.Sens,vl_loc.Autoroute));
    }
  }
  /*localisation a la hauteur de la sortie aval*/
  else if ( (vl_loc.DistanceSortieAval < 500) &&
	    (vl_loc.NumSortieAval !=0 ))
  {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : hauteur de la sortie aval");
    sprintf(vl_str," A LA HAUTEUR DE LA SORTIE %s %s %s.",
			vl_loc.SortieAvalUsager,
			vl_loc.NomSortieAval,
			drol_msg_kilometre(vl_loc_tete.PR,vl_loc.PR,vl_loc.Sens,vl_loc.Autoroute));
  }
  /*localisation a la hauteur de la bifurc aval*/
  else if ( (vl_loc.DistanceBifAval < 500) &&
	    (strcmp(vl_loc.NumAutorouteBifAval,"")) )
  {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : hauteur de la bifurc aval");
    sprintf(vl_str," A LA HAUTEUR DE LA BIFURCATION VERS %s %s.",
			vl_loc.NumAutorouteBifAval,
			drol_msg_kilometre(vl_loc_tete.PR,vl_loc.PR,vl_loc.Sens,vl_loc.Autoroute));
  }
  /*localisation a la hauteur de la sortie amont*/
  else if (( vl_loc.DistanceSortieAmont < 500)  &&
	    (vl_loc.NumSortieAmont != 0))
  {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : hauteur de la sortie amont");
    sprintf(vl_str," A LA HAUTEUR DE LA SORTIE %s %s %s.",
			vl_loc.SortieAmontUsager,
			vl_loc.NomSortieAmont,
			drol_msg_kilometre(vl_loc_tete.PR,vl_loc.PR,vl_loc.Sens,vl_loc.Autoroute));
  }
  /*localisation a la hauteur de la bifurc amont*/
  else if ( (vl_loc.DistanceBifAmont < 500) &&
	    (strcmp(vl_loc.NumAutorouteBifAmont,"")) )
  {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : hauteur de la bifurc amont");
    sprintf(vl_str," A LA HAUTEUR DE LA BIFURCATION VERS %s %s.",
			vl_loc.NumAutorouteBifAmont,
			drol_msg_kilometre(vl_loc_tete.PR,vl_loc.PR,vl_loc.Sens,vl_loc.Autoroute));
  }
  /*localisation par rapport a la sortie amont*/
  else if ( (vl_loc.DistanceSortieAmont <= vl_loc.DistanceBifAmont) &&
	    (vl_loc.NumSortieAmont != 0) && 
	    (vl_loc.DistanceSortieAmont != C_DISTANCE_INFINIE) &&
	    (vl_loc.SortieAmontEstBifurc != XDC_VRAI))
  {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : par rapport a la sortie amont");
    sprintf(vl_str," %s APRES LA SORTIE %s %s %s.",
		drol_msg_distance(vl_loc.DistanceSortieAmont),
		vl_loc.SortieAmontUsager,
		vl_loc.NomSortieAmont,
		drol_msg_kilometre(vl_loc_tete.PR,vl_loc.PR,vl_loc.Sens,vl_loc.Autoroute));
  }
  /*localisation par rapport a la bifurc amont*/
  else if ( (vl_loc.DistanceSortieAmont >= vl_loc.DistanceBifAmont) && 
	    (vl_loc.DistanceBifAmont != C_DISTANCE_INFINIE)  &&
	    (strcmp(vl_loc.NumAutorouteBifAmont,"")) /*SAGA*/
	  )
  {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : par rapport a la bifurc amont");
    sprintf(vl_str," %s APRES LA BIFURCATION VERS %s %s.",
		drol_msg_distance(vl_loc.DistanceBifAmont),
		vl_loc.NumAutorouteBifAmont,
		drol_msg_kilometre(vl_loc_tete.PR,vl_loc.PR,vl_loc.Sens,vl_loc.Autoroute));
  }

  /*localisation en limite de concession amont*/
  /*********************************************************** FI3657 deja traite plus haut 'entre limite amont et sortie aval'
  if ( (vl_loc.NumSortieAmont == 0) ) {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : limite de concession amont");
      if (vl_loc.Sens==XDC_SENS_SUD)
        sprintf(vm_local," ENTRE LA SORTIE %s %s ET %s %s.",
		vl_loc.SortieAvalUsager,
		vl_loc.NomSortieAval,
    		drol_msg_limites_amont(vl_loc.Autoroute, vl_loc.Sens),
		drol_msg_kilometre(vl_loc_tete.PR,vl_loc.PR,vl_loc.Sens,vl_loc.Autoroute));
      else 
	sprintf(vl_str," ENTRE %s ET LA SORTIE %s %s %s.",
		drol_msg_limites_amont(vl_loc.Autoroute, vl_loc.Sens),
		vl_loc.SortieAvalUsager,
		vl_loc.NomSortieAval,
		drol_msg_kilometre(vl_loc_tete.PR,vl_loc.PR,vl_loc.Sens,vl_loc.Autoroute));
  }
FI3657 deja traite plus haut 'entre sortie amont et limite aval'

  if ( (vl_loc.NumSortieAval == 0) ) {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : limite de concession aval");
      if (vl_loc.Sens==XDC_SENS_NORD)
      sprintf(vm_local," ENTRE %s ET LA SORTIE %s %s %s.",
    		drol_msg_limites_aval(vl_loc.Autoroute,vl_loc.Sens),
		vl_loc.SortieAmontUsager,
		vl_loc.NomSortieAmont,
		drol_msg_kilometre(vl_loc_tete.PR,vl_loc.PR,vl_loc.Sens,vl_loc.Autoroute));
    else
      sprintf(vl_str," ENTRE LA SORTIE %s %s ET %s %s.",
				vl_loc.SortieAmontUsager,
				vl_loc.NomSortieAmont,
				drol_msg_limites_aval(vl_loc.Autoroute,vl_loc.Sens),
				drol_msg_kilometre(vl_loc_tete.PR,vl_loc.PR,vl_loc.Sens,vl_loc.Autoroute));
  }

     FI3657 ************************************************************/
  if ( (vl_loc.NumSortieAmont == 0) && (vl_loc.NumSortieAval == 0)) {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : limite de concession amont et aval");
    sprintf(vl_str," ENTRE %s ET %s %s.",
		drol_msg_limites_amont(vl_loc.Autoroute, vl_loc.Sens),
		drol_msg_limites_aval(vl_loc.Autoroute,vl_loc.Sens),
		drol_msg_kilometre(vl_loc_tete.PR,vl_loc.PR,vl_loc.Sens,vl_loc.Autoroute));
  }
    

  /*localisation sur lit d arret*/
  if (strcmp(vl_str,"")) {
    if ( (vl_loc.TypePointCar == XDC_POINT_CARACT_LIT_ARRET) && 
         !(vm_noeud->Infos_TFM.Termine) ) {
      sprintf(vl_str,"%s LE LIT D ARRET NUMERO %d EST OCCUPE. ",
			vl_str,
			vl_loc.NumPointCar);
    }
  }
  if (!strcmp(vl_str,"")) {
	  sprintf(vl_str," %s.",
		drol_msg_kilometre(vl_loc_tete.PR,vl_loc.PR,vl_loc.Sens,vl_loc.Autoroute));
    }


  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_tfm_localisation : vm_local=%s;vl_str=%s",vm_local, vl_str);
  if(strlen(vm_local) < 149)
     strncat(vm_local, vl_str, 149 - strlen(vm_local));

  return (vm_local);
}

/*X*/
/*----------------------------------------------------------------------------
* SERVICE RENDU :
*  Retourne la localisation d'un evenement par rapport a un kilometre.
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *drol_msg_kilometre(int va_pr_tete, int va_pr, int va_sens, int va_auto)
/*
* ARGUMENTS EN ENTREE :
*   va_pr_tete        : PR de tete de l'evenement
*   va_pr             : PR de l'evenement
*   va_sens           : sens de l'evenement
*   va_autok          : autoroute de l'evenement
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* VALEUR DE RETOUR :
*   Texte de localisation de l'evenement par rapport a un kilometre.
*
* CONDITION D'UTILISATION
*
* FONCTION
----------------------------------------------------------------------------*/
{
float vl_pr_queue;
float vl_pr_tete;
int vl_km_tete;
int vl_km_queue;
int vl_pr_inter;
int vl_autoinversee=XDC_FAUX;

	XZST_03EcritureTrace(XZSTC_FONCTION,"drol_msg_kilometre : %d %d %d",va_pr_tete,va_pr,va_auto);

	vl_pr_tete=va_pr_tete;
	vl_pr_queue=va_pr;
	
	/* sens autoroute */ /*DEM506*/
	if (va_auto == 8 ) /* A57 */
		vl_autoinversee=XDC_VRAI;
	else
		vl_autoinversee=XDC_FAUX;

	/*si l'evenement touche la section St Laurent - Nice Ouest*/
        if ( ( (vl_pr_tete>=C_PR_ST_LAURENT) && (vl_pr_tete<=C_PR_NICE_OUEST)) ||
           ( (vl_pr_queue>=C_PR_ST_LAURENT) && (vl_pr_queue<=C_PR_NICE_OUEST))) {
           XZST_03EcritureTrace(XZSTC_FONCTION,"drol_msg_kilometre: l'evt touche la section St Laurent - Nice Ouest");
           return (""); 
        }

        /*sinon recupere les parties entieres des PR*/
        vl_pr_tete = vl_pr_tete / 1000;
        vl_pr_queue = vl_pr_queue / 1000;
        vl_km_tete = (int)vl_pr_tete;
        vl_km_queue = (int)vl_pr_queue;

	/*si l'evenement est ponctuel*/
        if (vl_pr_tete==vl_pr_queue)
        {
          /*s'il est dans la zone A (plus ou moins 100 m d'un pr entier)*/
          if ( abs((vl_pr_tete-vl_km_tete)*1000)<=100)
            sprintf(vm_km,"AU NIVEAU DU KM %d",vl_km_tete);
          else if ( abs((vl_pr_tete-vl_km_tete-1)*1000)<=100)
            sprintf(vm_km,"AU NIVEAU DU KM %d",vl_km_tete+1);
          else { /*on est dans la zone B (a plus de 100 m d'un pr entier)*/
            if (((va_sens==XDC_SENS_NORD)  && (vl_autoinversee==XDC_FAUX)) || ( (va_sens==XDC_SENS_SUD) && (vl_autoinversee==XDC_VRAI)) ) /*sens decroissant*/
              sprintf(vm_km,"APRES LE KM %d",vl_km_tete+1);
            else /*sens croissant*/
              sprintf(vm_km,"APRES LE KM %d",vl_km_tete);
          }
        }
	else if ( /*( (vl_pr_queue<vl_pr_tete) && (va_sens==XDC_SENS_NORD)) ||*/
             ( (vl_pr_queue>=vl_pr_tete) /*&& (va_sens==XDC_SENS_SUD)*/)        ) 
        {
            if (vl_km_queue != vl_pr_queue)
              vl_km_queue = vl_km_queue +1 ;
            if (vl_km_tete != vl_pr_tete)
              vl_km_tete = vl_km_tete;
            sprintf(vm_km,"APRES LE KM %d",vl_km_queue);
        }
        else
        {
            if (vl_km_tete != vl_pr_tete)
              vl_km_tete = vl_km_tete + 1;
            if (vl_km_queue != vl_pr_queue)
              vl_km_queue = vl_km_queue;
            sprintf(vm_km,"APRES LE KM %d",vl_km_queue);
        }
	XZST_03EcritureTrace(XZSTC_FONCTION,"drol_msg_kilometre : OUT = %s",vm_km);
	return (vm_km);
}


char *drol_msg_distance(int va_pr)
{
  static char vl_str[30];
  int vl_pr;
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_msg_distance : DISTANCE = %d",va_pr);

  vl_pr = ((va_pr+50)/100)*100;

  if (vl_pr < 1000)
    sprintf(vl_str,"%d M", vl_pr);
  else
    sprintf(vl_str,"%d KM", (int)((va_pr+500)/1000));
  return (vl_str);
}

char *drol_msg_limites_amont(int va_Autoroute, int va_sens)
{
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_msg_limites_amont : IN = %d/%d",va_Autoroute,va_sens);
  vm_str1[0]='\0';
  if ((va_Autoroute > 0) && (va_Autoroute <= vm_limites_nb_lexemes)) {
    /*hilmarcher dem 375: rajout condition sur sens 7 et sens 8 */
    /*cluchague dem 601: rajout egalement sens non precise */
    if ((va_sens==XDC_SENS_NORD) || (va_sens==XDC_SENS_ENTREE_NORD) || (va_sens==XDC_SENS_SORTIE_NORD) || (va_sens==XDC_SENS_INCONNU)|| (va_sens==XDC_SENS_NORD+10) ) /* DEM1088/PNI : va_sens==XDC_SENS_NORD+10 : cas evt a gerer dans les 2 sens */
      strcpy(vm_str1, pm_limites[va_Autoroute*2-1].Valeur);
    else
      strcpy(vm_str1, pm_limites[va_Autoroute*2-2].Valeur);
    }
  else
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_msg_limites_amont : Autoroute inconnue : %d",va_Autoroute);
    
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_msg_limites_amont : OUT = %s",vm_str1);
  return (vm_str1);
}
char *drol_msg_limites_aval(int va_Autoroute, int va_sens)
{
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_msg_limites_aval : IN = %d/%d",va_Autoroute,va_sens);
  vm_str2[0]='\0';
  if ((va_Autoroute > 0) && (va_Autoroute <= vm_limites_nb_lexemes)) {
    if ((va_sens==XDC_SENS_NORD) || (va_sens==XDC_SENS_ENTREE_NORD) || (va_sens==XDC_SENS_SORTIE_NORD) || (va_sens==XDC_SENS_INCONNU) || (va_sens==XDC_SENS_NORD+10) ) /* DEM1088/PNI : va_sens==XDC_SENS_NORD+10 : cas evt a gerer dans les 2 sens */

      strcpy(vm_str2, pm_limites[va_Autoroute*2-2].Valeur);
    else
      strcpy(vm_str2, pm_limites[va_Autoroute*2-1].Valeur);
    }
  else
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_msg_limites_aval : Autoroute inconnue : %d",va_Autoroute);
    
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_msg_limites_aval : OUT = %s",vm_str2);
  return (vm_str2);
}


void drol_trim(char * Ligne)
{
  int     i=0, j=0;

 /*A on supprime les espaces de debut de ligne */
  while ( Ligne[j] == ' ' ) j++;
  for ( i=j; i<strlen(Ligne); i++)
	  Ligne[i-j] = Ligne[i];
  Ligne[i-j] = '\0';

  /*A on supprime les espaces de fin de ligne */
  i = (int) strlen(Ligne);
  for (j=i-1;j>=0;j--) {
	  if ( Ligne[j] == ' ' )
		  Ligne[j] = '\0';
	  else break;
  }
}

char *drol_majuscule(char *Texte)
{
  int     i;
  int     l;

  l = (int) strlen(Texte);
  for (i=0; i < l; i++)
	  switch((int) Texte[i]) {
		  case -64 :      Texte[i] = 'A'; break;
		  case -56 :      Texte[i] = 'A'; break;
		  case -59 :      Texte[i] = 'E'; break;
		  case -55 :      Texte[i] = 'E'; break;
		  case -63 :      Texte[i] = 'E'; break;
		  case -47 :      Texte[i] = 'I'; break;
		  case -62 :      Texte[i] = 'O'; break;
		  case -53 :      Texte[i] = 'U'; break;
		  case -61 :      Texte[i] = 'U'; break;
		  default :       Texte[i] = toupper(Texte[i]);
	  }

  for (i=l-1; i>=0;i--)
  {
	  if (Texte[i] == ' ')
	  {
		  Texte[i] = '\0';
	  }
	  else
		  break;
  }
  return (Texte);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* cumul le nb de voies totales et en circulation
* a pertie de l'etat de la voie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void drol_cumul_voies(XDY_Voie x,int *pa_tot,int *pa_cir)

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
* generation du message TFM
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *drol_tfm_elabore(int va_numero, int va_cle, int *pa_niveau,XDY_Horodate Horodate, int va_bloc)

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
*
------------------------------------------------------*/
{
  T_NOEUD *vl_noeud;
  int             vl_longueur,vl_trafic;
  XDY_Voie        vl_BAU,vl_VL,vl_VM1,vl_VM2,vl_VR;
  XDY_Booleen     vl_evt_du_PA;
  int             vl_nb_voies=0, vl_nb_circulees=0;
  XDY_Octet	vl_niveau=0;
  int		vl_type;
  int             vl_JourSemaine = 0;
  double          vl_Maintenant = 0.0;
  double          vl_DateFin_Max = 0.0;
  double          vl_DateFin = 0.0;
  char *pl_tmp;
  T_NOEUD *tl_noeud[50];
  int vl_indice;

  /* calculer l'horodate de fin butee pour filtrage des evenements */
  XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_Maintenant);
  vl_DateFin_Max = vl_Maintenant - C_DELAI_DIFFUSION;
  XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_tfm_elabore: Date Fin Max=%lf", vl_DateFin_Max);

  strcpy(vm_messageTFM,"");

  XZST_03EcritureTrace(XZSTC_DEBUG3,"va_bloc %d",va_bloc);
  if (va_bloc==0) {
  
  vl_evt_du_PA = XDC_FAUX;
  vl_noeud = vg_racine;
  XZST_03EcritureTrace(XZSTC_DEBUG3,"va_bloc %d %d %d - %d %d -- %d ",va_bloc,vl_noeud->NumEvt.numero,vl_noeud->NumEvt.cle,va_numero,va_cle,vl_niveau );
   if (( (vl_noeud->NumEvt.numero == va_numero) && (vl_noeud->NumEvt.cle == va_cle) )  || (*pa_niveau < 1))
    {
      vl_evt_du_PA = XDC_VRAI;
      vl_type=vl_noeud->Type;
      vl_longueur = vl_noeud->Localisation.Longueur;
      vl_BAU = vl_noeud->Localisation.BAU;
      vl_VL = vl_noeud->Localisation.VL;
      vl_VM1 = vl_noeud->Localisation.VM1;
      vl_VM2 = vl_noeud->Localisation.VM2;
      vl_VR = vl_noeud->Localisation.VR;
      vl_trafic = vl_noeud->Trafic;
      vl_nb_voies = vl_noeud->Localisation.NbVoies;
      vl_nb_circulees = vl_noeud->Localisation.NbVoiesCirculees;
      drol_calcul_niveau_TFM(&vl_niveau,vl_type,
			  vl_BAU,vl_VL,vl_VM1,vl_VM2,vl_VR,vl_trafic,vl_longueur,
			  vl_nb_voies, vl_nb_circulees);
      *pa_niveau = vl_niveau;
    }
  XZST_03EcritureTrace(XZSTC_DEBUG3,"va_bloc %d %d %d - %d %d -- %d ",va_bloc,vl_noeud->NumEvt.numero,vl_noeud->NumEvt.cle,va_numero,va_cle,vl_niveau );
    if (vl_noeud->Infos_TFM.Termine) {
      XZST_03EcritureTrace(XZSTC_WARNING,"JMG termine");
      /*generer le texte pour cet evenement s'il n'est pas termine depuis plus du delai maximal*/
      if ( vl_evt_du_PA == XDC_FAUX )
      {
        if (XZSM_13ConversionHorodateSec(vl_noeud->Infos_TFM.Date_Fin, &vl_DateFin) != XZSMC_OK)
        {
          vl_DateFin = vl_Maintenant;
        }
        XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_tfm_elabore: Evenement %d/%d, Date Fin=%lf", vl_noeud->NumEvt.numero,vl_noeud->NumEvt.cle, vl_DateFin);
      }

      if ( (vl_evt_du_PA == XDC_VRAI)  ||  (vl_DateFin >= vl_DateFin_Max) )
      {
        strcat( vm_messageTFM,drol_tfm_genere(vl_noeud,TRUE));
        strcat( vm_messageTFM,"\n");
        vm_indiquer_cause = TRUE;
      }
    }
    else {
      strcat( vm_messageTFM,drol_tfm_genere(vl_noeud,FALSE));
      strcat( vm_messageTFM,"\n");
      vm_indiquer_cause = TRUE;
    }
     XZST_03EcritureTrace(XZSTC_DEBUG3,"FIN0 va_bloc %d %d %d - %d %d -- %d ",va_bloc,vl_noeud->NumEvt.numero,vl_noeud->NumEvt.cle,va_numero,va_cle,vl_niveau );
      return (vm_messageTFM);
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

    if ((tl_noeud[vl_indice]->NumEvt.numero==vg_racine->NumEvt.numero) &&
       (tl_noeud[vl_indice]->NumEvt.cle==vg_racine->NumEvt.cle))
      continue;

    vl_noeud = tl_noeud[vl_indice];

    XZST_03EcritureTrace(XZSTC_WARNING,"EVT tl_noeud %d/%d",vl_noeud->NumEvt.numero,vl_noeud->NumEvt.cle);
    if (vl_noeud->NumEvt.numero ==0)
      continue;

    vl_evt_du_PA = XDC_FAUX;
    if (( (vl_noeud->NumEvt.numero == va_numero) && (vl_noeud->NumEvt.cle == va_cle)) || (*pa_niveau < 1))
    {
      vl_evt_du_PA = XDC_VRAI;
      vl_type=vl_noeud->Type;
      vl_longueur = vl_noeud->Localisation.Longueur;
      vl_BAU = vl_noeud->Localisation.BAU;
      vl_VL = vl_noeud->Localisation.VL;
      vl_VM1 = vl_noeud->Localisation.VM1;
      vl_VM2 = vl_noeud->Localisation.VM2;
      vl_VR = vl_noeud->Localisation.VR;
      vl_trafic = vl_noeud->Trafic;
      vl_nb_voies = vl_noeud->Localisation.NbVoies;
      vl_nb_circulees = vl_noeud->Localisation.NbVoiesCirculees;
      drol_calcul_niveau_TFM(&vl_niveau,vl_type,
			  vl_BAU,vl_VL,vl_VM1,vl_VM2,vl_VR,vl_trafic,vl_longueur,
			  vl_nb_voies, vl_nb_circulees);
      *pa_niveau = vl_niveau;
    }

    /*si evt fini*/
    if (vl_noeud->Infos_TFM.Termine) {
      XZST_03EcritureTrace(XZSTC_WARNING,"JMG termine");
      /*generer le texte pour cet evenement s'il n'est pas termine depuis plus du delai maximal*/
      if ( vl_evt_du_PA == XDC_FAUX )
      {
        if (XZSM_13ConversionHorodateSec(vl_noeud->Infos_TFM.Date_Fin, &vl_DateFin) != XZSMC_OK)
        {
          vl_DateFin = vl_Maintenant;
        }
        XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_tfm_elabore: Evenement %d/%d, Date Fin=%lf", vl_noeud->NumEvt.numero,vl_noeud->NumEvt.cle, vl_DateFin);
      }

      if ( (vl_evt_du_PA == XDC_VRAI)  ||  (vl_DateFin >= vl_DateFin_Max) )
      {
        strcat( vm_messageTFM,drol_tfm_genere(vl_noeud,FALSE));
        strcat( vm_messageTFM,"\n");
        vm_indiquer_cause = TRUE;
      }
      
    }
    else {
         strcat( vm_messageTFM,drol_tfm_genere(vl_noeud,vm_indiquer_cause));
         strcat(vm_messageTFM,"\n");
         vm_indiquer_cause = TRUE;
    }

  }

  if (va_bloc==4)
    strcat(vm_messageTFM, drol_tfm_signature(Horodate));

  XZST_03EcritureTrace(XZSTC_WARNING,"msg en sortie %s",vm_messageTFM);

  XZST_03EcritureTrace(XZSTC_DEBUG3,"FIN va_bloc %d -- %d ",va_bloc,vl_niveau );
    return(vm_messageTFM);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Lance le calcul du niveau TFM avec l'equation adEquate
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_calcul_niveau_TFM(XDY_Octet *pa_nivTFM,int va_type,
                                XDY_Voie va_BAU,
                                XDY_Voie va_VL,
                                XDY_Voie va_VM1,
                                XDY_Voie va_VM2,
                                XDY_Voie va_VR,
                                int va_trafic,
                                int va_longueur,
                                int va_nb_voies,
                                int va_nb_circulees)

/*
* ARGUMENTS EN ENTREE :
*   XDY_Octet   pa_nivTFM : pointeur sur le niveau TFM a calculer;
*
* ARGUMENTS EN SORTIE :
*   aucun.
*
* CODE RETOUR :
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*   fonction appelee dans pac_ecrire_msgTFM
*
* FONCTION
*   appel de XZAP21
*
------------------------------------------------------*/
{
        int             code_retour=0;

        int             vl_retreci=0;

        /* Tableau des variables de l'equation */

        XDEAT_VAR       pl_variables[3]=
                                {
                                        {"LONGUEUR",0.0},       /* 1 */
                                        {"RETRECISSEMENT",0.0},  /* 2 */
                                        {"TRAFIC",0.0}          /* 3 */
                                };

        char    vl_nom_fichier[200];
        FILE*   pl_fichier;

        XZST_03EcritureTrace(XZSTC_FONCTION,"IN : %s", version);

        /*A Recuperation du type de FMC et choix de l'equation adequate */
        switch ( va_type )
        {
                case XZAEC_FMC_Accident :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Accident.txt" );
                        break;
                case XZAEC_FMC_Delestage :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Delestage.txt" );
                        break;
                case XZAEC_FMC_Deviation :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Deviation.txt" );
                        break;
                case XZAEC_FMC_Meteo     :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Meteo.txt" );
                        break;
                case XZAEC_FMC_Contresens :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Contresens.txt" );
                        break;
                case XZAEC_FMC_VehArrete :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Veh_Arrete.txt" );
                        break;
                case XZAEC_FMC_VehPanne :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Veh_Panne.txt" );
                        break;
                case XZAEC_FMC_VehFeu :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Veh_Feu.txt" );
                        break;
                case XZAEC_FMC_Cirque :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Cirque.txt" );
                        break;
                case XZAEC_FMC_ConvoiExc :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Convoi.txt" );
                        break;
                case XZAEC_FMC_ConvoiMil :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Militaire.txt" );
                        break;
                case XZAEC_FMC_VehLent :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Veh_Lent.txt" );
                        break;
                case XZAEC_FMC_Manifestation :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Manifestation.txt" );
                        break;
                case XZAEC_FMC_GreveESCOTA :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Greve.txt" );
                        break;
                case XZAEC_FMC_Escargot :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Escargot.txt" );
                        break;
                case XZAEC_FMC_Basculement :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Basculement.txt" );
                        break;
                case XZAEC_FMC_Travaux :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Travaux.txt" );
                        break;
                case XZAEC_FMC_IncSsConces :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Ss_Concess.txt" );
                        break;
                case XZAEC_FMC_QueueBouchon :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Queue_Bouchon.txt" );
                        break;
                case XZAEC_FMC_FermetureEch :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Echangeur.txt" );
                        break;
                case XZAEC_FMC_Eboulement :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Eboulement.txt" );
                        break;
                case XZAEC_FMC_Incendie :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Incendie.txt" );
                        break;
                case XZAEC_FMC_Inondation :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Inondation.txt" );
                        break;
                case XZAEC_FMC_Obstacle :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Obstacle.txt" );
                        break;
                case XZAEC_FMC_ChausseDeter :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Chaussee_Deterioree.txt" );
                        break;
                case XZAEC_FMC_FlaqueHuile :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Huile.txt" );
                        break;
                case XZAEC_FMC_AnimalErrant :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Animal.txt" );
                        break;
                case XZAEC_FMC_FrontiereFermee :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Frontiere.txt" );
                        break;
                case XZAEC_FMC_AltercationUsager :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Altercation.txt" );
                        break;
                case XZAEC_FMC_Agression :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Agression.txt" );
                        break;
                case XZAEC_FMC_MalaiseUsager :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Malaise.txt" );
                        break;
                case XZAEC_FMC_EvtHorsReseau :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Hors_Reseau.txt" );
                        break;
                case XZAEC_FMC_AireFermee :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Aire.txt" );
                        break;
                case XZAEC_FMC_Salage :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Salage.txt" );
                        break;
                case XZAEC_FMC_HoldUpVol :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Hold_Up.txt" );
                        break;
                case XZAEC_FMC_PietonChaussee :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Pieton.txt" );
                        break;
                case XZAEC_FMC_Autre :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Autre.txt" );
                        break;
                case XZAEC_FMC_PosteOperateur :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Operateur.txt");
                        break;
                case XZAEC_FMC_VehMarcheArr :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Veh_Marche_Arr.txt" );
                        break;
                case XZAEC_FMC_TraitChaussees :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Trait_Chaussees.txt" );
                        break;
                case XZAEC_FMC_EchDeconseille :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Ech_Deconseille.txt" );
                        break;
                case XZAEC_FMC_ChausseeGliss :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Chaussee_Glissante.txt" );
                        break;
                case XZAEC_FMC_BasculTravaux :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Bascul_Travaux.txt" );
                        break;
                case XZAEC_FMC_ZoneStockage :
                        sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Zone_Stockage.txt" );
                        break;
		case XZAEC_FMC_Regulation : 
			sprintf( vl_nom_fichier , "%s/%s", XDC_PATH_NTFM , "Niveau_Regulation_Vitesse.txt");
			break;
                default:
                        XZST_03EcritureTrace(XZSTC_WARNING,"%s: Type de FMC non prevu : %d", version, (int) va_type);
                }

        vl_retreci = (float)(va_nb_voies-va_nb_circulees)*100.0/(float)va_nb_voies;

        /*A Initialisation des  VARIABLES LONGUEUR et RETRECISSEMENT */
        pl_variables[0].valeur=(double)va_longueur;
        pl_variables[1].valeur=(double)vl_retreci;
        pl_variables[2].valeur=(double)va_trafic;
        XZST_03EcritureTrace(XZSTC_INFO,"%s: LONGUEUR=%lf RETRECISSEMENT=%lf TRAFIC=%lf",version,pl_variables[0].valeur,pl_variables[1].valeur,pl_variables[2].valeur);
        /*A Calcul du niveau par l'equation */
        pl_fichier=fopen(vl_nom_fichier,"r");

        if (pl_fichier==NULL)
                {
                        XZST_03EcritureTrace(XZSTC_WARNING,"%s echec ouverture %s",version,vl_nom_fichier);
                }
                else
                {
                        XZST_03EcritureTrace(XZSTC_INFO,"%s succes ouverture %s",version,vl_nom_fichier);
                        if (xdea01_evaluer(     pl_fichier,
                                                pl_variables,
                                                3,
                                                (XDY_Horodate)0,
                                                (XDY_TypeEqt)0,
                                                (XDY_Eqt)0,
                                                XDC_NON,
                                                pa_nivTFM)!=XDC_OK)
                                                                {
                                XZST_03EcritureTrace(XZSTC_WARNING,"%s echec de xdea01_evaluer",version);
                        }
                        fclose(pl_fichier);
                }

        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne %d",version, code_retour);

        return (code_retour);
}

