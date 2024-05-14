/*E*/
/* Fichier : $Id: drol_nav.c,v 1.19 2017/01/10 11:59:43 devgfi Exp $        Release : $Revision: 1.19 $        Date : $Date: 2017/01/10 11:59:43 $
**************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : drol_nav.c				*
*************************************************
* DESCRIPTION :
* new tdpac
*************************************************
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.0
* JMG 	20/10/04	integration BRA NG 1.1
* PCL 	04/01/05	on active le bra meme si que la bau est bloquee et traces supplementaires 1.2
* JMG	16/02/05	ajout explication claire dans SAD_NAV 1.3
* JMG	14/11/05	mode auto 1.5 1.6
* PNI	29/08/06	Si BAU bloquee pas de presignal BRA DEM572 1.7
* PNI	04/01/06	Si BRA utilise par un evt alors pas de pilotage auto V1.8
* LCL   09/07/07	Suppression DY V1.9
* PNI	28/08/07	Action proposee (pas auto) si seule la BAU est bloquee (BRA ancienne generation) DEM693 1.10
* PNI   19/05/09	Prise en compte des bra sans biseaux DEM884 v1.11
* JPL	17/01/11	Migration architecture HP ia64 (DEM 971) : adequation types parametres et profil fonctions  1.12
* JPL	14/10/11	Pas d'action automatique SAPIENS pour Fmc simulees (Essai ou Exercice) (DEM 995)  1.13
* PNI	30/07/2012	Type de nav en XDY_Octet DEM1048 1.14
* PNI 	29/08/2013	SAPIENS au CI  DEM 1074/FI 105 1.15
* JPL	05/12/16	Traces et niveaux ; identique precedente  1.17
* JPL	07/12/16	Support configuration des conditions et scenario ; comportement identique  1.18
* JPL	09/12/16	Support des scenarios PAL pour les travaux (DEM 1193)  1.19
* LCL	05/10/23	Adaptations CNA DEM-483 : lecture site principal en base
* GGY	22/01/24 	Ajout C_COND_CIRCULATION_BASC_TUNNEL et C_COND_CIRCULATION_NON_BASC_TUNNEL (DEM-483) (DEM-483)
************************************************/
#include "drol_nav.h"

#include "xzac03.h"
#include "xzac103.h"
#include "xzao19.h"
#include "xzap03.h"

#define C_REP_MIG	"/produits/migrazur/appliSD/fichiers/bdc/"
#define C_PERT		C_REP_MIG "bra.txt"


#define C_COMMENTAIRE	'#'
#define C_PIPE		'|'
#define C_SEPAR_LISTE	", \t"

#define C_DISTANCE	750


/* Types de BRA */
#define C_BRA_RAPIDE			1
#define C_BRA_LENT			2
#define C_BRA_NG_RAPIDE			7
#define C_BRA_NG_LENT			8
#define C_BRA_SB_RAPIDE			9


/* Conditions supportees : BRA AG voie rapide */
#define C_CND_BVR_SIG_NON_BASC				"BRA VR ET SIGNALE ET circulation_non_basculee"
#define C_CND_BVR_SIG_BASC					"BRA VR ET SIGNALE ET circulation_basculee"
#define C_CND_BVR_CNF_VLB_NON_BASC			"BRA VR ET CONFIRME ET VL BLOQUEE ET circulation_non_basculee"
#define C_CND_BVR_CNF_VLB_BASC				"BRA VR ET CONFIRME ET VL BLOQUEE ET circulation_basculee"
#define C_CND_BVR_CNF_VRB_NON_BASC			"BRA VR ET CONFIRME ET VR BLOQUEE ET circulation_non_basculee"
#define C_CND_BVR_CNF_VRB_BASC				"BRA VR ET CONFIRME ET VR BLOQUEE ET circulation_basculee"
#define C_CND_BVR_CNF_AUTRES_NON_BASC		"BRA VR ET CONFIRME ET AUTRES CAS ET circulation_non_basculee"
#define C_CND_BVR_CNF_AUTRES_BASC			"BRA VR ET CONFIRME ET AUTRES CAS ET circulation_basculee"

/* Conditions supportees : BRA AG voie lente */
#define C_CND_BVL_SIG_NON_BASC				"BRA VL ET SIGNALE ET circulation_non_basculee"
#define C_CND_BVL_SIG_BASC					"BRA VL ET SIGNALE ET circulation_basculee"
#define C_CND_BVL_CNF_VLB_NON_BASC			"BRA VL ET CONFIRME ET VL BLOQUEE ET circulation_non_basculee"
#define C_CND_BVL_CNF_VLB_BASC				"BRA VL ET CONFIRME ET VL BLOQUEE ET circulation_basculee"
#define C_CND_BVL_CNF_VRB_NON_BASC			"BRA VL ET CONFIRME ET VR BLOQUEE ET circulation_non_basculee"
#define C_CND_BVL_CNF_VRB_BASC				"BRA VL ET CONFIRME ET VR BLOQUEE ET circulation_basculee"
#define C_CND_BVL_CNF_AUTRES_NON_BASC		"BRA VL ET CONFIRME ET AUTRES CAS ET circulation_non_basculee"
#define C_CND_BVL_CNF_AUTRES_BASC			"BRA VL ET CONFIRME ET AUTRES CAS ET circulation_basculee"

/* Conditions supportees : BRA NG voie rapide */
#define C_CND_NGR_BOUCHON_NON_BASC			"BOUCHON ET circulation_non_basculee"
#define C_CND_NGR_BOUCHON_BASC				"BOUCHON ET circulation_basculee"
#define C_CND_NGR_VRB_M750_NON_BASC			"VR BLOQUEE -750M ET circulation_non_basculee"
#define C_CND_NGR_VRB_M750_BASC				"VR BLOQUEE -750M ET circulation_basculee"
#define C_CND_NGR_VRB_P750_ACC_NON_BASC		"VR BLOQUEE +750M ET ACCIDENT ET circulation_non_basculee"
#define C_CND_NGR_VRB_P750_ACC_BASC			"VR BLOQUEE +750M ET ACCIDENT ET circulation_basculee"
#define C_CND_NGR_VRB_P750_OBS_NON_BASC		"VR BLOQUEE +750M ET OBSTACLE ET circulation_non_basculee"
#define C_CND_NGR_VRB_P750_OBS_BASC			"VR BLOQUEE +750M ET OBSTACLE ET circulation_basculee"
#define C_CND_NGR_VRB_P750_AUTRES_NON_BASC	"VR BLOQUEE +750M ET AUTRES CAS ET circulation_non_basculee"
#define C_CND_NGR_VRB_P750_AUTRES_BASC		"VR BLOQUEE +750M ET AUTRES CAS ET circulation_basculee"
#define C_CND_NGR_VLB_NON_BASC				"VL BLOQUEE ET circulation_non_basculee"
#define C_CND_NGR_VLB_BASC					"VL BLOQUEE ET circulation_basculee"
#define C_CND_NGR_VXX_ACC_NON_BASC			"AUTRES VOIES ET ACCIDENT ET circulation_non_basculee"
#define C_CND_NGR_VXX_ACC_BASC				"AUTRES VOIES ET ACCIDENT ET circulation_basculee"
#define C_CND_NGR_VXX_OBS_NON_BASC			"AUTRES VOIES ET OBSTACLE ET circulation_non_basculee"
#define C_CND_NGR_VXX_OBS_BASC				"AUTRES VOIES ET OBSTACLE ET circulation_basculee"
#define C_CND_NGR_VXX_AUTRES_NON_BASC		"AUTRES VOIES ET AUTRES CAS ET circulation_non_basculee"
#define C_CND_NGR_VXX_AUTRES_BASC			"AUTRES VOIES ET AUTRES CAS ET circulation_basculee"

#define C_CND_NGR_TRV_VIT_HAUTE_NON_BASC	"TRAVAUX LIMITE VITESSE HAUTE ET circulation_non_basculee"
#define C_CND_NGR_TRV_VIT_HAUTE_BASC		"TRAVAUX LIMITE VITESSE HAUTE ET circulation_basculee"
#define C_CND_NGR_TRV_VIT_BASSE_NON_BASC	"TRAVAUX LIMITE VITESSE BASSE ET circulation_non_basculee"
#define C_CND_NGR_TRV_VIT_BASSE_BASC		"TRAVAUX LIMITE VITESSE BASSE ET circulation_basculee"

/* Scenarios des panneaux d'alerte (BRA NG / SB rapide) */
#define	C_BRA_SCN_EXTINCTION		"AL11"
#define	C_BRA_NOM_SCN_EXTINCTION	"EXTINCTION PANNEAU ALERTE"

/* Liste exhaustive des conditions supportees, pour controle de la configuration */
static	char		*lm_conditions[] = {

	C_CND_BVR_SIG_NON_BASC,
	C_CND_BVR_SIG_BASC,
	C_CND_BVR_CNF_VLB_NON_BASC,
	C_CND_BVR_CNF_VLB_BASC,
	C_CND_BVR_CNF_VRB_NON_BASC,
	C_CND_BVR_CNF_VRB_BASC,
	C_CND_BVR_CNF_AUTRES_NON_BASC,
	C_CND_BVR_CNF_AUTRES_BASC,

	C_CND_BVL_SIG_NON_BASC,
	C_CND_BVL_SIG_BASC,
	C_CND_BVL_CNF_VLB_NON_BASC,
	C_CND_BVL_CNF_VLB_BASC,
	C_CND_BVL_CNF_VRB_NON_BASC,
	C_CND_BVL_CNF_VRB_BASC,
	C_CND_BVL_CNF_AUTRES_NON_BASC,
	C_CND_BVL_CNF_AUTRES_BASC,

	C_CND_NGR_BOUCHON_NON_BASC,
	C_CND_NGR_BOUCHON_BASC,
	C_CND_NGR_VRB_M750_NON_BASC,
	C_CND_NGR_VRB_M750_BASC,
	C_CND_NGR_VRB_P750_ACC_NON_BASC,
	C_CND_NGR_VRB_P750_ACC_BASC,
	C_CND_NGR_VRB_P750_OBS_NON_BASC,
	C_CND_NGR_VRB_P750_OBS_BASC,
	C_CND_NGR_VRB_P750_AUTRES_NON_BASC,
	C_CND_NGR_VRB_P750_AUTRES_BASC,
	C_CND_NGR_VLB_NON_BASC,
	C_CND_NGR_VLB_BASC,
	C_CND_NGR_VXX_ACC_NON_BASC,
	C_CND_NGR_VXX_ACC_BASC,
	C_CND_NGR_VXX_OBS_NON_BASC,
	C_CND_NGR_VXX_OBS_BASC,
	C_CND_NGR_VXX_AUTRES_NON_BASC,
	C_CND_NGR_VXX_AUTRES_NON_BASC,

	C_CND_NGR_TRV_VIT_HAUTE_NON_BASC,
	C_CND_NGR_TRV_VIT_HAUTE_BASC,
	C_CND_NGR_TRV_VIT_BASSE_NON_BASC,
	C_CND_NGR_TRV_VIT_BASSE_BASC,

	NULL
};

static	T_PERT_BRA	vm_pert[30];
static	int		vm_nb_pert=0;

static	int		tm_fmc[50];
static	int		vm_nb_types=0;

T_NOEUD		*vm_noeud;
static T_NOEUD	*vm_noeud2;
T_NOEUD 	*vg_racine;
T_BRA		vm_bra;

extern XDY_Booleen  vg_presence_site[];
extern XDY_Mot	    vg_SitePrincipal;

static int drol_nav_charge();
static int drol_nav_recherche(XDY_Autoroute ,
		        XDY_PR ,
			XDY_Sens ,
			XDY_Horodate ,
			XDY_District,
			XDY_Booleen,
			int,
			int,
			T_NOEUD *); /*BRANG*/

static XDY_Booleen drol_nav_fmc_simulee();

static XDY_Booleen drol_nav_mode_rapide(XDY_Booleen,int,int,T_NOEUD *,int); /*BRA AG VR*/
static XDY_Booleen drol_nav_mode_lent(XDY_Booleen,int,int,T_NOEUD *,int); /*BRA AG VL*/
static XDY_Booleen drol_nav_mode_NG_rapide(XDY_Booleen,int,int,T_NOEUD *,int); /*BRA NG VR*/
static XDY_Booleen drol_nav_mode_NG_lent(XDY_Booleen,int,int,T_NOEUD *,int); /*BRA NG VL*/
static XDY_Booleen drol_nav_conditions_pal (T_NOEUD *, XDY_Octet);
static XDY_Booleen drol_nav_mode_type(T_NOEUD *);
static int drol_nav_index_conditions(char *);
static int ajouter_bra_pal (XDY_Eqt, XDY_Octet, XDY_District, XDY_Octet, XDY_Octet, XDY_District, XDY_Horodate);

/* definition des variables locales au module */
static char *version = "$Id: drol_nav.c,v 1.19 2017/01/10 11:59:43 devgfi Exp $ : drol_nav" ;


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_nav_init()

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
  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : drol_nav_init");
  vm_noeud2=(T_NOEUD *)malloc(sizeof(T_NOEUD));

  drol_nav_charge();

  XZST_03EcritureTrace(   XZSTC_FONCTION,"OUT drol_nav_init");

  return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des perturbations
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_nav_charge()

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
  int		vl_i, vl_j;
  char        vl_Lig[500];
  char	*vl_str;
  char	*pl_deb_ligne;
  char vl_evt[80];
  char vl_mode[80];
  char vl_conditions[80];
  int vl_type;
  int vl_auto;

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : drol_nav_charge");

  if ( (pl_fp = fopen(C_PERT, "r")) == NULL ) {
    XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !" ,C_PERT );
    return(XDC_NOK);
  }

  while (fgets(vl_Lig,sizeof(vl_Lig),pl_fp) != NULL)
  {
    for (pl_deb_ligne=vl_Lig;  isspace(*pl_deb_ligne); pl_deb_ligne++)
      { }

    if ( (*pl_deb_ligne != XDC_FIN_CHN)  &&  (*pl_deb_ligne != C_COMMENTAIRE) ) {
      if (sscanf (pl_deb_ligne, "FMC concern%[^|]|%[^|]", vl_evt, vl_conditions) >= 2) {
        /* extraire les types de FMC concernes*/
        vl_str = strtok(vl_conditions, C_SEPAR_LISTE);
        while (vl_str != NULL) {
          if (sscanf(vl_str, "%d", &vl_type) > 0) {
            tm_fmc[vm_nb_types] = vl_type;
            vm_nb_types++;
          }
          vl_str = strtok(NULL, C_SEPAR_LISTE);
        }
      }

      else {
        if (sscanf(pl_deb_ligne, " %[^|]| %[^|]| %[^|]|%d", vl_evt, vl_conditions, vl_mode, &vl_auto) >= 4) {
          drol_trim(vl_conditions);
          drol_trim(vl_mode);

          /* Verifier la validite des conditions */
          vl_i = 0;
          while ( (NULL != lm_conditions[vl_i])  &&  (0 != strcasecmp(vl_conditions, lm_conditions[vl_i])) ) {
            vl_i = vl_i + 1;
          }

          if (NULL != lm_conditions[vl_i]) {
            strcpy(vm_pert[vm_nb_pert].Libelle, vl_evt);
            strcpy(vm_pert[vm_nb_pert].Conditions, vl_conditions);
            strcpy(vm_pert[vm_nb_pert].Scenario, vl_mode);
            vm_pert[vm_nb_pert].Auto = vl_auto;
            vm_nb_pert++;
          } else {
            XZST_03EcritureTrace(XZSTC_WARNING, "Conditions activation BRA inconnues: '%s'", vl_conditions);
          }
        }
        else {
          XZST_03EcritureTrace(XZSTC_WARNING, "Erreur configuration BRA: %s", vl_Lig);
        }
      }
    }
  }

  fclose(pl_fp);


  /* Verifier les contraintes relatives a la configuration */
  if (vm_nb_types < 1) {
    XZST_03EcritureTrace(XZSTC_WARNING, "drol_nav_charge: Aucun type FMC declare !");
  } else {
    XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_nav_charge: %d types FMC (%d ... %d)", vm_nb_types, tm_fmc[0], tm_fmc[vm_nb_types-1]);
  }

  /* Trace de la configuration obtenue */
  for (vl_i = 0; vl_i < vm_nb_pert; vl_i++) {
    XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_nav_charge: %s | %s | %s | %d",
                            vm_pert[vl_i].Libelle, vm_pert[vl_i].Conditions, vm_pert[vl_i].Scenario, vm_pert[vl_i].Auto);
  }

  /* Mettre les conditions d'application des scenarios en majuscules */
  for (vl_i = 0; vl_i < vm_nb_pert; vl_i++) {
    vl_str = vm_pert[vl_i].Conditions;
    for (vl_j = strlen(vl_str) - 1; vl_j >= 0; vl_j--) {
      vl_str[vl_j] = toupper((int) vl_str[vl_j]);
    }
  }

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT  : drol_nav_charge");
  return (XDC_OK);
}



/**************************************/
/**************************************/
static XDY_Booleen drol_nav_fmc_simulee() {
  return (strstr (vm_noeud->Infos_TFM.Origine, "ESSAI") != NULL  ||
          strstr (vm_noeud->Infos_TFM.Origine, "EXERCICE") != NULL);
}

/**************************************/
/**************************************/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des perturbations engendrees par l'evt
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen drol_nav_filtre(T_NOEUD *va_noeud, XDY_District va_site, XDY_Horodate va_horodate)

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
  XZST_03EcritureTrace(   XZSTC_FONCTION,"IN : drol_nav_filtre %d/%d/%d/%d",
			va_noeud->NumEvt.numero,
			va_noeud->NumEvt.cle,
			va_noeud->Infos_TFM.Termine,
			va_noeud->Localisation.VL);

  /*vm_noeud=va_noeud;*/
  /*vm_noeud2->Localisation=vm_noeud->Localisation;
  vm_noeud2->Infos_FAX=vm_noeud->Infos_FAX;
  vm_noeud2->Type=vm_noeud->Type;*/

  /*s'il y a au moins une voie bloquee, on recherche les NAV candidates*/
  if (va_noeud->Infos_TFM.Termine) return(TRUE);
  if (va_noeud->Infos_TFM.Prevu) return (TRUE);
  if ( (va_noeud->Localisation.VL!=XDC_VOIE_BLOQUEE) &&
       (va_noeud->Localisation.VM1!=XDC_VOIE_BLOQUEE) &&
       (va_noeud->Localisation.VM2!=XDC_VOIE_BLOQUEE) &&
       (va_noeud->Localisation.BAU!=XDC_VOIE_BLOQUEE) &&
       (va_noeud->Localisation.VR!=XDC_VOIE_BLOQUEE)
     ) return(TRUE);

  /*sur bouchon, ne pas activer les BRA*/
  /*if (va_noeud->Type==XZAEC_FMC_QueueBouchon) return (TRUE);*/

  /*recherche les BRA candidats*/
  drol_nav_recherche(va_noeud->Localisation.Autoroute, 
		va_noeud->Localisation.PR,
		va_noeud->Localisation.Sens,
		va_horodate,
		va_site,
		va_noeud->Infos_TFM.Confirme,
		va_noeud->Localisation.VL,
		va_noeud->Localisation.VR,
		va_noeud);

  return (TRUE);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Fonction de recherche des BRA en amont de l'evenement
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_nav_recherche(XDY_Autoroute va_autoroute,
		        XDY_PR va_PR,
			XDY_Sens va_sens,
			XDY_Horodate va_horodate,
			XDY_District va_site,
			XDY_Booleen va_confirme,
			int va_VL,
			int va_VR,
			T_NOEUD *va_noeud)	/*BRANG*/

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
  XDY_Octet       Type_BRA;
  XDY_Entier vl_inversee=1;
  XDY_Octet       Dispo;
  T_STRING        Mode;
  XDY_Entier      NumEvt;
  XDY_Octet       CleEvt;
  XDY_Octet       TypeEvt;
  XDY_Octet       Priorite;
  XDY_Entier      DistanceEvt;
  XDY_Booleen	  vl_cr=FALSE;

  XZST_03EcritureTrace(   XZSTC_FONCTION,"IN : drol_nav_recherche aut=%d,PR=%d,sens=%d,site=%d",
						va_autoroute,va_PR,va_sens,va_site);

  if ((va_sens == XDC_SENS_INCONNU) || 
      (va_sens == XDC_SENS_ENTREE_NORD) || 
      (va_sens == XDC_SENS_ENTREE_SUD)  ) {
    XZST_03EcritureTrace(XZSTC_FONCTION, "drol_nav_recherche : OUT evt sur une entree");
    return (XDC_NOK);
  }

  /*PATCH A57*/
  if (va_autoroute==8)
    vl_inversee=-1;

  vl_sens=va_sens;
  if (va_sens == XDC_SENS_SORTIE_NORD)
    vl_sens = XDC_SENS_NORD;
  if (va_sens == XDC_SENS_SORTIE_SUD)
    vl_sens = XDC_SENS_SUD;

  /*appelle XZAO19*/
  if ( (XZAO19_NAV_amont(va_autoroute, va_PR, vl_sens, (XDY_Mot) 1,
	  &Eqt, &DistrictEqt, &AutorouteEqt, &PREqt, &SensEqt, &Distance, &PRInfluence,
	  &Type_BRA,
	  va_horodate) == XDC_OK)
       &&
       (Eqt != 0)
       &&
       (
         ( (vl_sens == XDC_SENS_NORD) && 
           (va_PR*vl_inversee >= PRInfluence*vl_inversee)
         )
         ||
         ( (vl_sens == XDC_SENS_SUD) && 
           (va_PR*vl_inversee <= PRInfluence*vl_inversee)
         )
       )
     ) {
    /*on a trouve une NAV*/
    /*on verifie qu'il appartient bien au district*/
    if ((va_site!=DistrictEqt) && (va_site!=vg_SitePrincipal))
      return (XDC_NOK);

    /*appel XZAT06*/
    if (XZAT06_Utilisation_NAV(     va_horodate, Eqt,
		    &Dispo, Mode, &NumEvt, &CleEvt, &Priorite, &DistanceEvt, &TypeEvt) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING,
	    "Etat BRA %d inconnu a la date %lf. On le considere indispo.", (int) Eqt, va_horodate);
      Dispo = XDC_EQT_MAJEUR;
    }														    

    if (Dispo > XDC_EQT_MINEUR) {
      XZST_03EcritureTrace(XZSTC_INFO,"BRA %d indispo, ignore", (int) Eqt);
      return (XDC_NOK);
    }


    vm_bra.Identifiant = Eqt;
    vm_bra.District = DistrictEqt;
    vm_bra.Distance = Distance;
    vm_bra.Type = Type_BRA;
    strcpy(vm_bra.Mode_Courant,Mode);
    vm_bra.Dispo=TRUE;
    if (NumEvt!=0) {
      vm_bra.NumEvt = NumEvt;
      vm_bra.CleEvt = CleEvt;
      vm_bra.Priorite = Priorite;
      vm_bra.DistanceEvt = DistanceEvt;
    }

    /*au CI, si le BRA est utilise par un evt plus proche, on l'oublie*/
/* ANNULE v1.19
    if ( (va_site==XDC_CI) &&
         (Distance>DistanceEvt) &&
         (NumEvt!=0) && (TypeEvt != XZAEC_FMC_Travaux) )
    {
      XZST_03EcritureTrace(XZSTC_INFO,"BRA %d utilise par un evt plus proche, ignore Distance=%d,DistanceEvt=%d,NumEvt=%d",
                             (int) Eqt,Distance,DistanceEvt,NumEvt);
      return (XDC_NOK);
    }
*/


    /*recherche du mode de fonctionnement*/
    switch(vm_bra.Type) {
      case C_BRA_RAPIDE : vl_cr = drol_nav_mode_rapide(va_confirme,va_VL,va_VR,va_noeud,Distance);break; 
      case C_BRA_LENT : vl_cr = drol_nav_mode_lent(va_confirme,va_VL,va_VR,va_noeud,Distance);break;
      case C_BRA_NG_RAPIDE : vl_cr = drol_nav_mode_NG_rapide(va_confirme,va_VL,va_VR,va_noeud,Distance);break; /* DEM884 */
      case C_BRA_SB_RAPIDE : vl_cr = drol_nav_mode_NG_rapide(va_confirme,va_VL,va_VR,va_noeud,Distance);break;
      case C_BRA_NG_LENT : vl_cr = drol_nav_mode_NG_lent(va_confirme,va_VL,va_VR,va_noeud,Distance);break;
	default :       XZST_03EcritureTrace(XZSTC_WARNING,"drol_nav_message :type=%d inconnu",vm_bra.Type);
    }

    XZST_03EcritureTrace(XZSTC_DEBUG1, "AU RETOUR DE DROL_NAV...type=%d->mode= %d",vm_bra.Type,vl_cr);

    if (!vl_cr) return (XDC_OK);

    /*suppression des utilisations identiques a l'utilisation courante*/
    if (!strcmp(vm_bra.Mode_Courant,vm_bra.Mode)) {
      if ((vm_bra.Auto==DROL_MODE_MANUEL)   || (NumEvt!=0))	{
   	/*mise en base de la proposition avec priorite=255 si prop identique a l'utilisation courante*/
	if (XZAP03_Ajout_Prop_NAV(vg_numero_plan,
			  vg_ordre_nav++,
			  vm_bra.District,
			  "",
			  255,
			  vm_bra.Identifiant,
			  vm_bra.Mode, 
			  "",
			  vm_bra.Mode,
			  /*vm_bra.Realisable) != XDC_OK)*/ /*JMG ??????????*/
			  TRUE) != XDC_OK)
      	XZST_03EcritureTrace(XZSTC_WARNING,"drol_nav_message : pb appel a XZAP03_Ajout_Prop_NAV");
	}
	XZST_03EcritureTrace(XZSTC_INFO,"proposition identique a l'utilisation courante. On la supprime");
      return (XDC_OK);
    }


  XZST_03EcritureTrace(XZSTC_DEBUG3,"vg_site=%d;vm_bra.District=%d",vg_site,vm_bra.District);
XZST_03EcritureTrace(XZSTC_DEBUG3,"vg_presence_site[vm_bra.District]=%d",vg_presence_site[vm_bra.District]);

    /*si on calcule un plan d'actions pour une simulation*/
    if (((vg_site==XDC_CI) && (vg_presence_site[vm_bra.District]==XDC_VRAI)) || drol_nav_fmc_simulee()) {
	    /*on ne fait que proposer, pas de pilotage automatique*/
      /* Ne s'applique pas sur COFIROUTE, uniquement sur ESCOTA */
      /* Sur COFIROUTE, le site principal peut piloter les BAD */
	    vm_bra.Auto=DROL_MODE_MANUEL;
    }

    /*mise en base de la proposition NAV si pas automatique*/
    if ((vm_bra.Auto==DROL_MODE_MANUEL) || (NumEvt!=0)) {
      if (XZAP03_Ajout_Prop_NAV(vg_numero_plan,
			  vg_ordre_nav++,
			  vm_bra.District,
			  "",
			  vm_bra.Priorite,
			  vm_bra.Identifiant,
			  vm_bra.Mode, 
			  "",
			  vm_bra.Mode,
			  /*vm_bra.Realisable) != XDC_OK)*/ /*JMG ??????????*/
			  TRUE) != XDC_OK)
        XZST_03EcritureTrace(XZSTC_WARNING,"drol_nav_message : pb appel a XZAP03_Ajout_Prop_NAV");
      return (XDC_OK);
    }
    
    /*on est ici, donc on traite du pilotage automatique*/
    /*premier cas, on est sur le district de l equipement*/
    if (vm_bra.District==va_site) {
      if (XZAC03_Commande_NAV(vm_noeud->NumEvt.numero, /*num evt*/
			    vm_noeud->NumEvt.cle,    /*cle evt*/
			    vg_num_poste,            /*poste operateur*/
			    vm_bra.Identifiant,      /*numero equipement*/
			    va_horodate ,            /*horodate lancement*/
			    XDC_NON,                   /*manuelle*/
			    vm_bra.Priorite,         /*priorite*/
			    vm_bra.Distance,      /*distance evt*/
		            vm_bra.Mode,         /*nom court du scenario*/
		            vm_bra.District,         /*site du bra*/
			    va_site)             /*site d'origine de l'action*/
          != XDC_OK)
       XZST_03EcritureTrace(XZSTC_WARNING, "drol_nav_recherche : pb appel a XZAC03_Commande_NAV");
    }
    else { /*sur districts differents, envoyer un msg a TACLI*/
      if (XZAC103_Commande_NAV_TACLI( vm_noeud->NumEvt.numero,
                                vm_noeud->NumEvt.cle,
                                vg_num_poste,
                                vm_bra.Identifiant,
                                va_horodate,
                                XDC_NON,
                                vm_bra.Priorite,
                                vm_bra.Distance,
                                vm_bra.Mode,
                                vm_bra.District,
                                va_site)!=XDC_OK)
        XZST_03EcritureTrace(XZSTC_WARNING, "drol_nav_recherche : appel XZAC103_Commande_NAV_TACLI a echoue");
    }
  }
  return (XDC_OK);
}




/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
* Fonction de recherche des BRA pour activation des panneaux d'alerte
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen drol_nav_filtre_pal(T_NOEUD *va_noeud, XDY_District va_site, XDY_Horodate va_horodate)

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
------------------------------------------------------------------------------*/
{
  XDY_Sens	vl_sens;
  XDY_Booleen	vl_cr=FALSE;

  XZST_03EcritureTrace(   XZSTC_FONCTION,"IN : drol_nav_filtre_pal %d/%d/%d/%d",
			va_noeud->NumEvt.numero,
			va_noeud->NumEvt.cle,
			va_noeud->Infos_TFM.Termine,
			va_noeud->Localisation.VL);

  /*vm_noeud=va_noeud;*/

  /* Les panneaux d'alerte ne sont actives que pour les Travaux */
  if (va_noeud->Type != XZAEC_FMC_Travaux) return (XDC_OK);


  vl_sens = va_noeud->Localisation.Sens;
  if ((vl_sens == XDC_SENS_INCONNU) || 
      (vl_sens == XDC_SENS_ENTREE_NORD) || 
      (vl_sens == XDC_SENS_ENTREE_SUD) )
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, "drol_nav_filtre_pal : OUT evt sur une entree");
    return (XDC_NOK);
  }

  if (vl_sens == XDC_SENS_SORTIE_NORD)
    vl_sens = XDC_SENS_NORD;
  if (vl_sens == XDC_SENS_SORTIE_SUD)
    vl_sens = XDC_SENS_SUD;


  /* Rechercher les BRA dont le panneau d'alerte est dans la perturbation */
  XZAO444_BRA_En_Perturbation (va_noeud->Localisation.Autoroute, vl_sens, va_noeud->Localisation.PR,
                                  va_site, va_horodate,
                                  va_noeud->Localisation_Tete.PR,
                                  va_noeud->NumEvt.numero, va_noeud->NumEvt.cle,
                                  (XDY_FonctionInt) ajouter_bra_pal);

  return (XDC_OK);
}



/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
* Fonction utilisateur passee dans XZAO444,
* ajoutant un BRA pour activation de son panneau d'alerte.
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int ajouter_bra_pal (
		XDY_Eqt		va_numero_bra,
		XDY_Octet	va_type_bra,
		XDY_District	va_site_eqt,
		XDY_Octet	va_limite_vitesse,
		XDY_Octet	va_mise_au_neutre,
		XDY_District	va_site,
		XDY_Horodate	va_horodate)

/*
* ARGUMENTS EN ENTREE :
*   va_numero_bra     : Numero de l'equipement
*   va_type_bra       : (Sous-)type de BRA
*   va_site_eqt       : Site de gestion du BRA
*   va_limite_vitesse : Limite de vitesse en vigueur au PR du BRA
*   va_mise_au_neutre : Indicateur d'extinction (BRA hors zone ou evt termine)
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR :
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
------------------------------------------------------------------------------*/
{
	XDY_Octet	Dispo;
	T_STRING	Scenario_Courant;
	XDY_Entier	NumEvt;
	XDY_Octet	CleEvt;
	XDY_Octet	TypeEvt;
	XDY_Octet	Priorite;
	XDY_Entier	DistanceEvt;
	XDY_Booleen	vl_cr = XDC_FAUX;

    /* Verifier que l'equipement est d'un type supportant la fonction */
    if ((va_type_bra != C_BRA_NG_RAPIDE) && (va_type_bra != C_BRA_SB_RAPIDE))
      return (XDC_OK);


    /* Verifier que l'equipement est pilotable sur le site */
    /* Uniquement pour ESCOTA, les autres sites principaux sont des PC */
    if ((va_site!=va_site_eqt) && (va_site!=XDC_CI))
      return (XDC_NOK);


    /* Determiner l'utilisation actuelle du BRA */
    if (XZAT06_Utilisation_NAV(     va_horodate, va_numero_bra,
		    &Dispo, Scenario_Courant, &NumEvt, &CleEvt, &Priorite, &DistanceEvt, &TypeEvt) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING,
	    "Etat BRA %d inconnu a la date %lf. On le considere indispo.", (int) va_numero_bra, va_horodate);
      Dispo = XDC_EQT_MAJEUR;
    }														    

    if (Dispo > XDC_EQT_MINEUR) {
      XZST_03EcritureTrace(XZSTC_INFO,"BRA %d indispo, ignore", (int) va_numero_bra);
      return (XDC_NOK);
    }


    /* Determiner le scenario a commander a l'equipement */
    /* Si l'evenement est termine ou l'equipement est hors zone d'influence, il doit etre eteint */
    if (va_mise_au_neutre == XDC_VRAI) {
      strcpy (vm_bra.Mode, C_BRA_SCN_EXTINCTION);
      vm_bra.Auto = DROL_MODE_MANUEL;
    } else {
      vl_cr = drol_nav_conditions_pal (vm_noeud, va_limite_vitesse);

      /* si aucune condition d'activation n'est satisfaite alors */
      if (vl_cr != XDC_VRAI) {
        if ( (NumEvt == vm_noeud->NumEvt.numero) && (CleEvt == vm_noeud->NumEvt.cle) ) {
          /* proposer l'extinction si le (PAL du) BRA est utilise pour l'evenement */
          strcpy (vm_bra.Mode, C_BRA_SCN_EXTINCTION);
          vm_bra.Auto = DROL_MODE_MANUEL;
        } else {
          return (XDC_OK);
        }
      }
    }


    /* Ignorer le BRA s'il est utilise par un autre evenement */
    if ( (NumEvt != 0)  &&  ((NumEvt != vm_noeud->NumEvt.numero) || (CleEvt != vm_noeud->NumEvt.cle)) ) {
      XZST_03EcritureTrace (XZSTC_INFO, "BRA %d utilise par un evt prioritaire, PAL ignore pour Travaux", (int) va_numero_bra);
      return (XDC_NOK);
    }


    vm_bra.Identifiant = va_numero_bra;
    vm_bra.District = va_site_eqt;
    vm_bra.Distance = 0;
    vm_bra.Type = va_type_bra;
    strcpy(vm_bra.Mode_Courant, Scenario_Courant);
    vm_bra.Dispo=TRUE;
    vm_bra.NumEvt = vm_noeud->NumEvt.numero;
    vm_bra.CleEvt = vm_noeud->NumEvt.cle;
    vm_bra.Priorite = Priorite;
    vm_bra.DistanceEvt = DistanceEvt;


    /*suppression des utilisations identiques a l'utilisation courante*/
    if (!strcmp(vm_bra.Mode_Courant,vm_bra.Mode)) {
      XZST_03EcritureTrace(XZSTC_INFO,"proposition PAL du BRA identique a l'utilisation courante, ignoree");
      return (XDC_OK);
    }


  XZST_03EcritureTrace(XZSTC_DEBUG3,"vg_site=%d;vm_bra.District=%d",vg_site,vm_bra.District);
XZST_03EcritureTrace(XZSTC_DEBUG3,"vg_presence_site[vm_bra.District]=%d",vg_presence_site[vm_bra.District]);

    /*si on calcule un plan d'actions pour une simulation*/
    if (((vg_site==XDC_CI) && (vg_presence_site[vm_bra.District]==XDC_VRAI)) || drol_nav_fmc_simulee()) {
	    /*on ne fait que proposer, pas de pilotage automatique*/
      /* Ne s'applique pas sur COFIROUTE, uniquement sur ESCOTA */
      /* Sur COFIROUTE, le site principal peut piloter les BAD */
	    vm_bra.Auto=DROL_MODE_MANUEL;
    }


    /*mise en base de la proposition NAV si pas automatique*/
    if ((vm_bra.Auto==DROL_MODE_MANUEL) || (NumEvt!=0)) {
      if (XZAP03_Ajout_Prop_NAV(vg_numero_plan,
			  vg_ordre_nav++,
			  vm_bra.District,
			  "",
			  vm_bra.Priorite,
			  vm_bra.Identifiant,
			  vm_bra.Mode, 
			  "",
			  vm_bra.Mode,
			  /*vm_bra.Realisable) != XDC_OK)*/ /*JMG ??????????*/
			  TRUE) != XDC_OK)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"ajouter_bra_pal : pb appel a XZAP03_Ajout_Prop_NAV");
      }
      return (XDC_OK);
    }

    /* Pilotage automatique de l'equipement */
    /* sur le district de l equipement, executer la commande en base */
    if (vm_bra.District==va_site) {
      if (XZAC03_Commande_NAV(vm_noeud->NumEvt.numero, /*num evt*/
			    vm_noeud->NumEvt.cle,    /*cle evt*/
			    vg_num_poste,            /*poste operateur*/
			    vm_bra.Identifiant,      /*numero equipement*/
			    va_horodate ,            /*horodate lancement*/
			    XDC_NON,                   /*manuelle*/
			    vm_bra.Priorite,         /*priorite*/
			    vm_bra.Distance,      /*distance evt*/
		            vm_bra.Mode,         /*nom court du scenario*/
		            vm_bra.District,         /*site du bra*/
			    va_site)             /*site d'origine de l'action*/
          != XDC_OK)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "ajouter_bra_pal : pb appel a XZAC03_Commande_NAV");
        return (XDC_NOK);
      }
    }
    else { /*sur districts differents, envoyer un msg a TACLI*/
      if (XZAC103_Commande_NAV_TACLI( vm_noeud->NumEvt.numero,
                                vm_noeud->NumEvt.cle,
                                vg_num_poste,
                                vm_bra.Identifiant,
                                va_horodate,
                                XDC_NON,
                                vm_bra.Priorite,
                                vm_bra.Distance,
                                vm_bra.Mode,
                                vm_bra.District,
                                va_site)!=XDC_OK)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "ajouter_bra_pal : appel XZAC103_Commande_NAV_TACLI a echoue");
        return (XDC_NOK);
      }
    }

  return (XDC_OK);
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static XDY_Booleen drol_nav_mode_rapide(XDY_Booleen va_confirme,int va_VL, int va_VR,T_NOEUD *va_noeud, int va_distance) /*BRANG*/

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
  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : drol_nav_mode %d/%d/%d",
				va_confirme,
				va_VL,
				va_VR);

  char		*vl_conditions = "";
  XDY_Booleen	vl_autre_cas_aucune_voie = XDC_FAUX;
  int		vl_i_pert = 0;

  /* valeurs par defaut */
  strcpy(vm_bra.Mode,"");
  vm_bra.Auto=DROL_MODE_MANUEL;

  /*sur bouchon, ne pas activer les BRA*/
  if (va_noeud->Type==XZAEC_FMC_QueueBouchon) return (FALSE);

  /*BRA VOIE RAPIDE*/
  /*evt signale*/
  if (!va_confirme)
  {
    vl_conditions = C_CND_BVR_SIG_NON_BASC;
  }

  /*obstacle confirme sur VL*/
  else if ( (va_confirme) &&
       (va_VL==XDC_VOIE_BLOQUEE) &&
       (va_VR==XDC_VOIE_SENS_NORMAL)
     )
  {
    vl_conditions = C_CND_BVR_CNF_VLB_NON_BASC;
  } 

  /*obstacle confirme sur VR*/
  else if ( (va_confirme) &&
       (va_VL==XDC_VOIE_SENS_NORMAL) &&
       (va_VR==XDC_VOIE_BLOQUEE)
     )
  {
    vl_conditions = C_CND_BVR_CNF_VRB_NON_BASC;
  }

  /*autres cas*/
  else {
    vl_conditions = C_CND_BVR_CNF_AUTRES_NON_BASC;

    /*DEM 693 :  si pas d' autre voie  bloquee (hors BAU) */
    if ((va_noeud->Localisation.VL!=XDC_VOIE_BLOQUEE) &&
        (va_noeud->Localisation.VM1!=XDC_VOIE_BLOQUEE) &&
        (va_noeud->Localisation.VM2!=XDC_VOIE_BLOQUEE) &&
        (va_noeud->Localisation.VR!=XDC_VOIE_BLOQUEE) && (va_noeud->Localisation.BAU==XDC_VOIE_BLOQUEE)
       )
    {
      vl_autre_cas_aucune_voie = XDC_VRAI;
    }
  }
 

  /* Rechercher une configuration correspondant a ces conditions */
  vl_i_pert = drol_nav_index_conditions (vl_conditions);
  if (vl_i_pert >= 0) {
    XZST_03EcritureTrace(XZSTC_DEBUG3,
        "drol_nav_mode : BRA voie rapide mode propose = %s",vm_pert[vl_i_pert].Scenario);
    strcpy(vm_bra.Mode,vm_pert[vl_i_pert].Scenario);
    if (XDC_VRAI == vl_autre_cas_aucune_voie) {
      vm_bra.Auto = DROL_MODE_MANUEL;
    } else {
      vm_bra.Auto = vm_pert[vl_i_pert].Auto;
    }
    return (TRUE);
  } else {
    XZST_03EcritureTrace(XZSTC_WARNING,
        "drol_nav_mode : BRA voie rapide : aucun scenario configure pour la situation '%s'", vl_conditions);
    return (FALSE);
  }
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static XDY_Booleen drol_nav_mode_lent(XDY_Booleen va_confirme,int va_VL, int va_VR,T_NOEUD *va_noeud, int va_distance) /*BRANG*/

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
  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : drol_nav_mode %d/%d/%d",
				va_confirme,
				va_VL,
				va_VR);

  char	*vl_conditions = "";
  int	vl_i_pert = 0;

  /* valeurs par defaut */
  strcpy(vm_bra.Mode,"");
  vm_bra.Auto=DROL_MODE_MANUEL;

  /*sur bouchon, ne pas activer les BRA*/
  if (va_noeud->Type==XZAEC_FMC_QueueBouchon) return (FALSE);

  /*BRA VOIE LENTE*/
  /*evt signale*/
  if (!va_confirme)
  {
    vl_conditions = C_CND_BVL_SIG_NON_BASC;
  }

  /*obstacle confirme sur VL*/
  if ( (va_confirme) &&
       (va_VL==XDC_VOIE_BLOQUEE) &&
       (va_VR==XDC_VOIE_SENS_NORMAL)
     )
  {
    vl_conditions = C_CND_BVL_CNF_VLB_NON_BASC;
  } 

  /*obstacle confirme sur VR*/
  if ( (va_confirme) &&
       (va_VL==XDC_VOIE_SENS_NORMAL) &&
       (va_VR==XDC_VOIE_BLOQUEE)
     )
  {
    vl_conditions = C_CND_BVL_CNF_VRB_NON_BASC;
  }

  /*autres cas*/
  else {
    vl_conditions = C_CND_BVL_CNF_AUTRES_NON_BASC;
  }


  /* Rechercher une configuration correspondant a ces conditions */
  vl_i_pert = drol_nav_index_conditions (vl_conditions);
  if (vl_i_pert >= 0) {
    XZST_03EcritureTrace(XZSTC_DEBUG3,
        "drol_nav_mode : BRA voie lente mode propose = %s",vm_pert[vl_i_pert].Scenario);
    strcpy(vm_bra.Mode,vm_pert[vl_i_pert].Scenario);
    vm_bra.Auto = vm_pert[vl_i_pert].Auto;
    return (TRUE);
  } else {
    XZST_03EcritureTrace(XZSTC_WARNING,
        "drol_nav_mode : BRA voie lente : aucun scenario configure pour la situation '%s'", vl_conditions);
    return (FALSE);
  }
}



/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
* Recherche des conditions d'activation d'un BRA de type NG rapide
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static XDY_Booleen drol_nav_mode_NG_rapide(XDY_Booleen va_confirme,int va_VL, int va_VR,T_NOEUD *va_noeud, int va_distance) /*BRANG*/

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
*   fonction appelee si un BRA de ce type est selectionne.
*
* FONCTION
*   le scenario et le mode d'application sont retournes dans les donnees BRA globales
*
-----------------------------------------------------------------------------*/
{
  char	*vl_conditions = "";
  int	vl_i_pert = 0;

  /* valeurs par defaut */
  strcpy(vm_bra.Mode,"");
  vm_bra.Auto=DROL_MODE_MANUEL;

  if (!drol_nav_mode_type(va_noeud))
  {
    return (FALSE);
  }

  if (va_noeud->Type==XZAEC_FMC_QueueBouchon) 
  {
    vl_conditions = C_CND_NGR_BOUCHON_NON_BASC;
  }

  /*VR seule bloquee*/
  else if ( (va_noeud->Localisation.VR==XDC_VOIE_BLOQUEE) &&
       (va_noeud->Localisation.VM1!=XDC_VOIE_BLOQUEE) &&
       (va_noeud->Localisation.VM2!=XDC_VOIE_BLOQUEE) &&
       (va_noeud->Localisation.VL!=XDC_VOIE_BLOQUEE)
     ) 
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, "VR SEULE BLOQUEE");
    if (va_distance < C_DISTANCE) 
    {
      vl_conditions = C_CND_NGR_VRB_M750_NON_BASC;
    }
    else 
    {
      if (va_noeud->Type==XZAEC_FMC_Accident) {
        vl_conditions = C_CND_NGR_VRB_P750_ACC_NON_BASC;
      }
      else if (va_noeud->Type==XZAEC_FMC_Obstacle) {
        vl_conditions = C_CND_NGR_VRB_P750_OBS_NON_BASC;
      }
      else {
        vl_conditions = C_CND_NGR_VRB_P750_AUTRES_NON_BASC;
      }
    }
  }

  /*VL seule bloquee*/
  else if ((va_noeud->Localisation.VL==XDC_VOIE_BLOQUEE) &&
      (va_noeud->Localisation.VM1!=XDC_VOIE_BLOQUEE) &&
      (va_noeud->Localisation.VM2!=XDC_VOIE_BLOQUEE) &&
      (va_noeud->Localisation.VR!=XDC_VOIE_BLOQUEE)
     ) 
  {
    vl_conditions = C_CND_NGR_VLB_NON_BASC;
  }

  /*autres cas*/
  else
  {
    /*DEM 572 :  si une autre voie est bloquee (hors BAU) */
    if ((va_noeud->Localisation.VL==XDC_VOIE_BLOQUEE) ||
        (va_noeud->Localisation.VM1==XDC_VOIE_BLOQUEE) ||
        (va_noeud->Localisation.VM2==XDC_VOIE_BLOQUEE) ||
        (va_noeud->Localisation.VR==XDC_VOIE_BLOQUEE)
       )
    {
      if (va_noeud->Type==XZAEC_FMC_Accident)
      {
        vl_conditions = C_CND_NGR_VXX_ACC_NON_BASC;
      }
      else if (va_noeud->Type==XZAEC_FMC_Obstacle)
      {
        vl_conditions = C_CND_NGR_VXX_OBS_NON_BASC;
      }
      else
      {
        vl_conditions = C_CND_NGR_VXX_AUTRES_NON_BASC;
      }
    }
  }


  /* Si les conditions ne correspondent a aucun cas alors aucune proposition */
  if (0 == strcmp (vl_conditions, "")) {
    return (FALSE);
  }

  /* Rechercher une configuration correspondant a la condition prioritaire */
  vl_i_pert = drol_nav_index_conditions (vl_conditions);
  if (vl_i_pert >= 0) {
    XZST_03EcritureTrace(XZSTC_DEBUG3,
        "drol_nav_mode : BRA NG VR mode propose = %s",vm_pert[vl_i_pert].Scenario);
    strcpy(vm_bra.Mode,vm_pert[vl_i_pert].Scenario);
    vm_bra.Auto = vm_pert[vl_i_pert].Auto;
    return (TRUE);
  } else {
    XZST_03EcritureTrace(XZSTC_WARNING,
        "drol_nav_mode : BRA NG VR : aucun scenario configure pour la situation '%s'", vl_conditions);
    return (FALSE);
  }
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static XDY_Booleen drol_nav_mode_NG_lent(XDY_Booleen va_confirme,int va_VL, int va_VR,T_NOEUD *va_noeud, int va_distance) /*BRANG*/

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
  if (!drol_nav_mode_type(va_noeud))
    return (FALSE);
  return (FALSE);
}




/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU :
* Recherche des conditions d'activation du panneau d'alerte d'un BRA
-------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static XDY_Booleen drol_nav_conditions_pal (T_NOEUD *va_noeud, XDY_Octet va_limite_vitesse)

/*
* ARGUMENTS EN ENTREE :
*   va_noeud          : noeud de l'evenement courant
*   va_limite_vitesse : Limite de vitesse en vigueur au PR du BRA
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR :
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*   Fonction appelee pour chaque BRA selectionne en zone d'activation,
*   dans le cas ou l'evenement n'est pas termine.
*
* FONCTION
*   Le scenario et le mode d'application sont retournes dans les donnees BRA globales.
*
-----------------------------------------------------------------------------*/
{
  char	*vl_conditions = "";
  int	vl_i_pert = 0;

  /* valeurs par defaut */
  strcpy (vm_bra.Mode, "");
  vm_bra.Auto = DROL_MODE_MANUEL;


  /* Filtrer les conditions sur le type d'eveneent */
  if (va_noeud->Type != XZAEC_FMC_Travaux)
  {
    return (XDC_FAUX);
  }


  /* Rechercher les conditions sur la consigne de vitesse de l'evenement */
  /* Consigne de vitesse evenement = limite vitesse haute au lieu de l'equipement */
  if (va_noeud->Infos_FAX.Vitesse == (va_limite_vitesse - 20))
  {
    vl_conditions = C_CND_NGR_TRV_VIT_HAUTE_NON_BASC;
  }

  /* Consigne de vitesse evenement = limite vitesse basse au lieu de l'equipement */
  else if (va_noeud->Infos_FAX.Vitesse == (va_limite_vitesse - 40))
  {
    vl_conditions = C_CND_NGR_TRV_VIT_BASSE_NON_BASC;
  }


  /* Si les conditions ne correspondent a aucun cas alors aucune proposition */
  if (0 == strcmp (vl_conditions, "")) {
    return (XDC_FAUX);
  }

  /* Rechercher une configuration correspondant a la condition prioritaire */
  vl_i_pert = drol_nav_index_conditions (vl_conditions);
  if (vl_i_pert >= 0) {
    XZST_03EcritureTrace (XZSTC_DEBUG3, "drol_nav_conditions_pal: scenario propose = %s",vm_pert[vl_i_pert].Scenario);
    strcpy (vm_bra.Mode ,vm_pert[vl_i_pert].Scenario);
    vm_bra.Auto = vm_pert[vl_i_pert].Auto;
    return (XDC_VRAI);
  } else {
    XZST_03EcritureTrace (XZSTC_WARNING, "drol_nav_conditions_pal : aucun scenario configure pour la situation '%s'", vl_conditions);
    return (XDC_FAUX);
  }
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Determine si le type de l'evenement est parmi les types concernes
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static XDY_Booleen drol_nav_mode_type(T_NOEUD *va_noeud)
{
  int vl_indice=0;

  for (vl_indice=0;vl_indice<vm_nb_types;vl_indice++) {
    if (tm_fmc[vl_indice]==va_noeud->Type) {
      return (TRUE);
    }
  }
  XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_nav_mode_type : type fmc %d non trouve", (int) va_noeud->Type);
  return (FALSE);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* Recherche dans la configuration une condition d'application d'un scenario
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_nav_index_conditions(char *pa_conditions)
{
  int vl_index = 0;

  for (vl_index = 0; vl_index < vm_nb_pert; vl_index++) {
    if (0 == strcmp(vm_pert[vl_index].Conditions, pa_conditions)) {
      return (vl_index);
    }
  }

  XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_nav_index_conditions : conditions non trouvees (%s)", pa_conditions);
  return (-1);
}
