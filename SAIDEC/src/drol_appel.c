/*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : SAIDEC				*
*
* FICHIER : drol_appel.c			*
*************************************************
* DESCRIPTION :
* new tdpac
*************************************************
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.0
* CHI   20/04/05	:rajout du sous-type grele sur chaussee DEM456 v1.3
* JMG   17/02/04        codage de drol_appel_datex (SAGA)
* PNI	21/06/07 : Ajout des nouveaux type de fmc DEM658
* JMG	12/12/07	: Ajout mail DEM718 1.6 1.7
* JPL	06/09/10	Prise en compte du type Fmc Zone de stockage et du predicat "demarre" (DEM 942) 1.8
* JPL	13/01/11	Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.9
* JMG	24/02/12 : linux 1.10
* JMG			regulation dem 1016
* PNI	22/11/13	: au_moins_1_voie_bloquee prend en compte les voies bloquees neutralisees  1.12 DEM1087
* JPL	09/10/14	Prise en compte du sens des astreintes (DEM 1100) 1.13
* LCL	15/11/16	Ajout de "sur_viaduc" et "dans_tunnel" (LOT23) 1.14
* JMG	15/05/17	contresens majeur : rechercher peages dnas les 2 sens 1.15
* PNI   16/11/17 	correction strncpy pour mise a jour Remarque avec liste des peage 1.16
* JPL	29/06/18	Prise en compte de condition sur le nombre de passagers (DEM 1295)  1.17
* JMG	05/09/18	SMS LOT25 DEM1301 1.18
* JPL	08/04/19	Support de conditions d'ecoulement / non ecoulement (DEM 1326)  1.19
* JPL	23/09/19	Type d'evenement "INCENDIE" attendu en configuration et non "INCENDIE DE VEGETATION" (DEM 1352)  1.20
* JPL	27/05/21	Terminaison du texte de Remarque du cas d'appel Astreinte Peage  1.21
* LCL	04/10/23	Adaptations CNA DEM-483 : lecture site principal en base
* JPL	15/01/24	Support de la condition "nature ANIMAL" pour appel astreinte PATRIMOINE (SAE-567)  1.22
* JPL	07/02/24	Augmentation des tailles de tampons pour lecture de la configuration 
************************************************/

#include "drol_appel.h"

#include "xzac23.h"

#include "xzao445.h"

#define C_REP_MIG							"/produits/migrazur/appliSD/fichiers/bdc/"
#define C_MOD_PC	C_REP_MIG 				"appels_pc.txt"
#define C_MOD_CI	C_REP_MIG 				"appels_ci.txt"
#define C_CAS_CI	C_REP_MIG 				"cas_appels_ci.txt"
#define C_CAS_PC	C_REP_MIG 				"cas_appels_pc.txt"

#define C_SEPARATEUR						"#"
#define C_PIPE								"|"
#define C_ET								"&"

#define C_INFERIEUR							"<"
#define C_SUPERIEUR							">"
#define C_EGAL								"="

#define C_COND_ECHANGEUR					"sur_echangeur"
#define C_COND_PEAGE						"au_peage"
#define C_COND_LIT_ARRET					"sur_lit_arret"
#define C_COND_AIRE							"sur_aire"
#define C_COND_VIADUC						"sur_viaduc"
#define C_COND_TUNNEL						"dans_tunnel"
#define C_COND_CONFIRME						"confirme"
#define C_COND_SIGNALE						"signale"
#define C_COND_TERMINE						"termine"
#define C_COND_TRAFIC						"niveau_trafic"
#define C_COND_PR_MIN						"pr_min"
#define C_COND_PR_MAX						"pr_max"
#define C_COND_AUTOROUTE					"autoroute"
#define C_COND_SANS_VOIE_BLOQUEE			"sans_voie_bloquee"
#define C_COND_AU_MOINS_1_VOIE				"au_moins_1_voie_bloquee"
#define	C_COND_COUPURE_CHAUSSEE				"coupure_chaussee"
#define C_COND_NON_PREVU					"non_prevu"
#define C_COND_SENS_2						"sens_2"
#define C_COND_SENS_1						"sens_1"
#define C_COND_DUREE						"duree"
#define C_COND_LONGUEUR						"longueur"
#define C_COND_MAJEUR						"majeur"

#define C_COND_VISIBILITE					"visibilite"
#define C_COND_TYPE_VEHICULE				"type_vehicule"
#define C_COND_NB_PASSAGERS					"nb_passagers"
#define C_COND_NB_PL						"nb_pl"
#define C_COND_NB_VL						"nb_vl"
#define C_COND_NB_MOTOS						"nb_motos"
#define C_COND_MATIERES						"matieres_dangereuses"
#define C_COND_PERSONNALITES				"personnalites"
#define C_COND_NB_VEHICULES					"nb_vehicules"
#define C_COND_NB_VICTIMES					"nb_victimes"
#define C_COND_NB_MORTS						"nb_morts"
#define C_COND_NB_BG						"nb_blesses_graves"
#define C_COND_NB_CARS						"nb_cars"
#define C_COND_AGENT_ESCOTA					"agent_escota"
#define C_COND_DATEX						"datex"
#define C_COND_SUR_RAU						"sur_rau"
#define C_COND_ANIMAUX						"animaux"
#define C_COND_NATURE_ANIMAL				"nature_animal"
#define C_COND_STOCKAGE_DEMARRE				"stockage_demarre"
#define C_COND_STOCKAGE_NON_DEMARRE			"stockage_non_demarre"
#define C_COND_TRANSITOIRE					"transitoire"
#define C_COND_ECOULEMENT					"ecoulement"
#define C_COND_NON_ECOULEMENT				"non_ecoulement"

#define C_MAX_PEAGES						10

typedef struct {
  XDY_Mot	Numero;
  char		Nom[100];
  XDY_Autoroute	Autoroute;
  XDY_PR	PR;
  XDY_Sens	Sens;
 XDY_Booleen	Utilise;
 char		Perturbation[100];
 int		Priorite;
 XDY_District  District;
} T_Peage;

T_Peage		tm_peages[C_MAX_PEAGES];

T_INTERLOCUTEUR *vg_interlocuteurs_ci;
T_INTERLOCUTEUR *vg_interlocuteurs_pc;
T_CONDITION 	*vg_conditions_ci;
T_CONDITION	*vg_conditions_pc;
T_CAS		*vg_cas_ci;
T_CAS		*vg_cas_pc;
T_NOEUD		*vm_noeud;
static T_NOEUD		*vm_noeud2;
T_NOEUD 	*vg_racine;

static XDY_Octet        *vm_TabAjout;
static int              vm_IndexTabAjout;
static XDY_Octet        *vm_TabSuppr;
static int              vm_IndexTabSuppr;
extern XDY_District		  vg_NumeroSite;
extern XDY_Mot			    vg_SitePrincipal;


int drol_appel_charge(char *);
int drol_appel_charge_cas(char *);
T_CONDITION *drol_appel_decode_condition(char *, XDY_Booleen *);


static XDY_Booleen drol_appel_egal(int, int);
static XDY_Booleen drol_appel_true(int, int);
static XDY_Booleen drol_appel_inferieur();
static XDY_Booleen drol_appel_superieur();


static XDY_Booleen drol_appel_echangeur();
static XDY_Booleen drol_appel_peage();
static XDY_Booleen drol_appel_lit_arret();
static XDY_Booleen drol_appel_aire();
static XDY_Booleen drol_appel_sur_viaduc();
static XDY_Booleen drol_appel_dans_tunnel();
static XDY_Booleen drol_appel_confirme();
static XDY_Booleen drol_appel_signale();
static XDY_Booleen drol_appel_termine();
static XDY_Booleen drol_appel_sans_voie_bloquee();
static XDY_Booleen drol_appel_une_voie();
static XDY_Booleen drol_appel_coupure_chaussee();
static XDY_Booleen drol_appel_non_prevu();
static XDY_Booleen drol_appel_sens2();
static XDY_Booleen drol_appel_sens1();
static XDY_Booleen drol_appel_matieres();
static XDY_Booleen drol_appel_personnalites();
static XDY_Booleen drol_appel_agent_escota();
static XDY_Booleen drol_appel_datex();
static XDY_Booleen drol_appel_sur_rau();
static XDY_Booleen drol_appel_animaux();
static XDY_Booleen drol_appel_stockage_demarre();
static XDY_Booleen drol_appel_stockage_non_demarre();
static XDY_Booleen drol_appel_transitoire();
static XDY_Booleen drol_appel_ecoulement();
static XDY_Booleen drol_appel_non_ecoulement();
static XDY_Booleen drol_appel_majeur();
static XDY_Booleen drol_appel_nature_animal();

XDY_Booleen drol_appel_interlocuteurs(T_CAS *, T_NOEUD *, XDY_District, XDY_Booleen * );
XDY_Booleen drol_appel_a_recontacter(T_NOEUD *, XDY_District );
static int ajouter_peage(XDY_Mot, char *, XDY_Autoroute, XDY_PR, XDY_Sens, XDY_District, int, XDY_Horodate, char *, int *);


/* definition des variables locales au module */
static char *version = "drol_appel.c 1.22" ;



static int ajouter_peage(XDY_Mot va_numero,
			char *va_nom,
			XDY_Autoroute	va_autoroute,
			XDY_PR va_PR,
			XDY_Sens va_sens,
			XDY_District va_district,
			int va_priorite,
			XDY_Horodate va_horodate,
			char *va_perturbation,
			int *pa_resultat)
{

  int vl_indice;

  XZST_03EcritureTrace(XZSTC_WARNING,
	"ajouter_peage: numero %d, nom %s, autoroute %d, PR %d, sens %d, district %d, prio %d, pert %s",
			va_numero, va_nom, va_autoroute, va_PR, va_sens, va_district, va_priorite, va_perturbation);


  for (vl_indice=0; vl_indice < C_MAX_PEAGES; vl_indice++){
    if (!(tm_peages[vl_indice].Utilise)) {
      XZST_03EcritureTrace(XZSTC_FONCTION,"ajout peage");
      tm_peages[vl_indice].Utilise = TRUE;
      tm_peages[vl_indice].Numero = va_numero;
      tm_peages[vl_indice].Autoroute = va_autoroute;
      tm_peages[vl_indice].PR = va_PR;
      tm_peages[vl_indice].Sens = va_sens;
      tm_peages[vl_indice].District = va_district;
      tm_peages[vl_indice].Priorite = va_priorite;
      strcpy(tm_peages[vl_indice].Nom, va_nom);
      strcpy(tm_peages[vl_indice].Perturbation,va_perturbation);
    
    *pa_resultat=XDC_OK;
    return (XDC_OK);
    }
  }
  XZST_03EcritureTrace(XZSTC_WARNING, "plus de peage dispo");
  *pa_resultat=XDC_NOK;
  return (XDC_NOK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   enregistre les types d'astreinte deja contactes
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int FctTabAjout(TypeAstreinte)
XDY_Octet       TypeAstreinte;

/*
* ARGUMENTS EN ENTREE :
*   XDY_Octet   TypeAstreinte;
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*   fonction passee en argument a XZAE23
*
* FONCTION
*   ajoute le type d'astreinte dans le tableau TabAjout
*
------------------------------------------------------*/
{
        XZST_03EcritureTrace(XZSTC_FONCTION, "IN : FctTabAjout TypeAstreinte = %d",TypeAstreinte);
        /*A
        **  on stocke TypeAstreinte dans TabAjout
        */
        vm_TabAjout = realloc(vm_TabAjout, ((size_t) vm_IndexTabAjout+1)*sizeof(XDY_Octet));
        vm_TabAjout[vm_IndexTabAjout++] = TypeAstreinte;
        return(XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   enregistre les types d'astreinte deja contactes
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int FctTabSuppr(TypeAstreinte)
XDY_Octet       TypeAstreinte;

/*
* ARGUMENTS EN ENTREE :
*   XDY_Octet   TypeAstreinte;
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*   fonction passee en argument a XZAE23
*
* FONCTION
*   ajoute le type d'astreinte dans le tableau TabSuppr
*
------------------------------------------------------*/
{
        XZST_03EcritureTrace(XZSTC_FONCTION, "IN : FctTabSuppr TypeAstreinte = %d",TypeAstreinte);
        /*A
        **  on stocke TypeAstreinte dans TabSuppr
        */
        vm_TabSuppr = realloc(vm_TabSuppr, ((size_t) vm_IndexTabSuppr+1)*sizeof(XDY_Octet));
        vm_TabSuppr[vm_IndexTabSuppr++] = TypeAstreinte;
        return(XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_appel_init()

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
  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : drol_appel_init");
  vm_noeud2=(T_NOEUD *)malloc(sizeof(T_NOEUD));

  vg_interlocuteurs_ci=NULL;
  drol_appel_charge(C_MOD_CI);

  vg_interlocuteurs_pc=NULL;
  drol_appel_charge(C_MOD_PC);

  vg_cas_ci=NULL;
  drol_appel_charge_cas(C_CAS_CI);

  vg_cas_pc=NULL;
  drol_appel_charge_cas(C_CAS_PC);

  XZST_03EcritureTrace(   XZSTC_FONCTION,"OUT drol_appel_init");

  return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des cas d'appels
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_appel_charge_cas(char *va_nom)

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
  char        vl_Lig[360];
  XDY_Entier vl_Resultat_out = 0;
  char	*vl_str;
  char *vl_ch;
  XDY_Booleen vl_majeur;
  T_CAS *pl_liste;
  int	vl_num_prec,vl_num;
  int vl_len,vl_pos;
  char vl_l1[5];

  XZST_03EcritureTrace(XZSTC_DEBUG1, "IN  : drol_appel_charge_cas (%s)",va_nom);

  if ( (pl_fp = fopen(va_nom, "r")) == NULL ) {
    XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !" ,va_nom );
    return(XDC_NOK);
  }
  
  /*memorisation de la structure du message*/
  while ( ( vl_retcode = XZAO000_LireLigneFich ( pl_fp, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
  {
    vl_i=0;
    if (vl_Resultat_out != XZAO_EOF )
    {
      pl_liste = (T_CAS *)malloc(sizeof(T_CAS));
      vl_str=vl_Lig; 
      vl_str[strlen(vl_str)-1]='\0';
      vl_ch=strtok(vl_str,C_PIPE);

      /*element 1 = libelle*/
      strcpy(pl_liste->Libelle,vl_ch);

      vl_ch=strtok(NULL,C_PIPE);
      /*element 2 = libelle de la condition*/
      strcpy(pl_liste->Libelle_Condition,vl_ch);

      vl_num_prec=0;
      vl_ch=vl_Lig+strlen(pl_liste->Libelle)+2+strlen(pl_liste->Libelle_Condition);

      vl_len=(int) strlen(vl_ch);
      vl_pos=0;
      while (vl_pos < vl_len) {
        sscanf(vl_ch,"%[^|]|",vl_l1);
	sscanf(vl_l1,"%d",&vl_num_prec);
	vl_ch=vl_ch+strlen(vl_l1)+1;
        vl_pos = vl_pos + (int) strlen(vl_l1)+1;

	/*si le caractere suivant le pipe n'est pas un pipe*/
	if ( (vl_ch[0]!='|') && (vl_ch[0]!='\0')) {
	  pl_liste->Interlocuteurs[vl_i]=vl_num_prec;
	  vl_i++;
	  sscanf(vl_ch,"%[^|]|",vl_l1);
	  vl_ch=vl_ch+strlen(vl_l1)+1;
	  vl_pos = vl_pos + (int) strlen(vl_l1)+1;
	}
	else {
	  vl_ch=vl_ch+1;
	  vl_pos ++;
	}
      }
      pl_liste->Nombre_Interlocuteurs=vl_i;
      
      /*decodage de la condition*/
      pl_liste->Condition=drol_appel_decode_condition(pl_liste->Libelle_Condition, &vl_majeur);

      pl_liste->Majeur = vl_majeur;

      if (!strcmp(va_nom,C_CAS_PC)) {
        pl_liste->Suivant = vg_cas_pc;
        vg_cas_pc =pl_liste;
      }
      else {
	pl_liste->Suivant = vg_cas_ci;
	vg_cas_ci = pl_liste;
      }
    }
    else break;
  }
  fclose(pl_fp);

  XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT  : drol_appel_charge_cas");
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* chargement des interlocuteurs
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_appel_charge(char *va_nom)

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
  char        vl_Lig[256];
  XDY_Entier vl_Resultat_out = 0;
  char	*vl_str;
  char *vl_ch;
  T_INTERLOCUTEUR *pl_tmp;

  XZST_03EcritureTrace(XZSTC_DEBUG1, "IN  : drol_appel_charge (%s)",va_nom);

  if ( (pl_fp = fopen(va_nom, "r")) == NULL ) {
    XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !" ,va_nom );
    return(XDC_NOK);
  }
  
  /*memorisation de la structure du message*/
  while ( ( vl_retcode = XZAO000_LireLigneFich ( pl_fp, vl_Lig, sizeof(vl_Lig), &vl_Resultat_out ) ) != XDC_NOK )
  {
    if (vl_Resultat_out != XZAO_EOF )
    {
      pl_tmp = (T_INTERLOCUTEUR *)malloc(sizeof(T_INTERLOCUTEUR));
      pl_tmp->A_Appeler = FALSE;
      vl_str=vl_Lig; 
      vl_ch=strtok(vl_str,C_SEPARATEUR);

      /*element 1 = competence*/
      strcpy(pl_tmp->Competence,vl_ch);

      vl_ch=strtok(NULL,C_SEPARATEUR);
      /*element 2 = type astreinte*/
      sscanf(vl_ch,"%d",&(pl_tmp->Type_Astreinte));

      vl_ch=strtok(NULL,C_SEPARATEUR);
      /*element 3 = appel fax*/
      sscanf(vl_ch,"%d",&(pl_tmp->Appel_Fax));

      vl_ch=strtok(NULL,C_SEPARATEUR);
      /*element 4 = appel tel*/
      sscanf(vl_ch,"%d",&(pl_tmp->Appel_Tel));

      /*element 5 = appel mail*/
      vl_ch=strtok(NULL,C_SEPARATEUR);
      sscanf(vl_ch,"%d",&(pl_tmp->Appel_Mail));

      /*element 6 = envoi SMS*/
      vl_ch=strtok(NULL,C_SEPARATEUR);
      sscanf(vl_ch,"%d",&(pl_tmp->Appel_SMS));

      pl_tmp->A_Appeler = FALSE;

      XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_appel_charge(%s):  comp %s/%d/%d/%d/%d/%d",
						va_nom,
						pl_tmp->Competence,
						pl_tmp->Type_Astreinte,
						pl_tmp->Appel_Fax,
						pl_tmp->Appel_Tel,
						pl_tmp->Appel_Mail,
						pl_tmp->Appel_SMS);
      if (vg_SitePrincipal==XDC_CI)
      {
        /* On est sur ESCOTA */
        if (!strcmp(va_nom,C_MOD_PC)) {
          pl_tmp->Suivant = vg_interlocuteurs_pc;
          vg_interlocuteurs_pc =pl_tmp;
        }
        else {
          pl_tmp->Suivant = vg_interlocuteurs_ci;
          vg_interlocuteurs_ci = pl_tmp;
        }
      }
      else
      {
        /* On est sur COFIROUTE*/
        /* on merge les deux listes et on les duplique */
        pl_tmp->Suivant = vg_interlocuteurs_pc;
        vg_interlocuteurs_pc = pl_tmp;
        vg_interlocuteurs_ci = pl_tmp;
      }
    }
    else break;
  }
  fclose(pl_fp);

  XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT  : drol_appel_charge");
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* decodage de la condition
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

T_CONDITION *drol_appel_decode_condition(char *va_cond, XDY_Booleen *va_majeur)

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
  char	*vl_str;
  int vl_indice;
  char *vl_ch;
  T_CONDITION *pl_cond;
  T_CONDITION *pl_tmp;
  int vl_type;
  char vl_ch_init[500];
  char vl_ch2[100];
  char *vl_ch3;
  int vl_len;

  /*decode les elements separes par des &*/
  XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_appel_decode_condition:  IN %s",va_cond);

  *va_majeur = XDC_FAUX;

  /*allocation memoire*/
  pl_cond = (T_CONDITION *)malloc(sizeof(T_CONDITION));

  /*1 condition  = le type*/
  strcpy(vl_ch_init,va_cond);
  vl_str=strtok(va_cond,C_ET);

  /*recupere le nom du type*/
  vm_noeud2->Type=0;
  strcpy(pl_cond->Libelle,vl_str);
  vl_ch=strtok(vl_str,"=");
  vl_ch=strtok(NULL,"=");
  if (vl_ch==NULL) 
  {
    XZST_03EcritureTrace(   XZSTC_DEBUG1,"drol_appel_decode_condition : pas de type");
    pl_cond->Type=C_TYPE_COMPARATEUR;
    pl_cond->Fonction.Comp.Slot=&(vm_noeud2->Type);
    pl_cond->Fonction.Comp.Fct=drol_appel_true;
    pl_cond->Suivant=NULL;
  }
  else
  {
  /*supprimer le premier espace*/
  drol_trim(vl_ch);
  XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_appel_decode_condition:  TYPE %s",vl_ch);

  pl_cond->Type=C_TYPE_COMPARATEUR;
  pl_cond->Fonction.Comp.Slot=&(vm_noeud2->Type);
  pl_cond->Fonction.Comp.Fct=drol_appel_egal;
  pl_cond->Suivant=NULL;
  pl_tmp=pl_cond;

  /*numero en fonction du type*/
  if (!strcmp(vl_ch,"ACCIDENT")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Accident;
  else if (!strcmp(vl_ch,"TOUS")) {
    pl_cond->Fonction.Comp.Valeur=0;
    pl_cond->Fonction.Comp.Fct=drol_appel_true;
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
  else if (!strcmp(vl_ch,"VEHICULE EN MARCHE ARRIERE")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_VehMarcheArr;
  else if (!strcmp(vl_ch,"Traitement chaussees")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_TraitChaussees    ;
  else if (!strcmp(vl_ch,"Echangeur d�conseille")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_EchDeconseille;
  else if (!strcmp(vl_ch,"CHAUSSEE GLISSANTE")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_ChausseeGliss;
  else if (!strcmp(vl_ch,"Basculement pour travaux")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_BasculTravaux;
  else if (!strcmp(vl_ch,"Zone de stockage")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_ZoneStockage;
  else if (!strcmp(vl_ch,"Regulation")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Regulation;
  /*traiter le cas particulier de la meteo*/
  else if (!strcmp(vl_ch,"VERGLAS")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_appel_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Verglas;
  }
  else if (!strcmp(vl_ch,"CHUTE NEIGE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_appel_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_ChuteNeige;
  }
  else if (!strcmp(vl_ch,"PLUIE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_appel_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Pluie;
  }
  else if (!strcmp(vl_ch,"BROUILLARD GIVRANT")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_appel_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_BrouillardGiv;
  }
  else if (!strcmp(vl_ch,"NEIGE SUR CHAUSSEE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_appel_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_NeigeChaus;
  }
  else if (!strcmp(vl_ch,"GRELE SUR CHAUSSEE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_appel_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_GreleChaus;
  }
  else if (!strcmp(vl_ch,"BROUILLARD")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_appel_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Brouillard;
  }
  else if (!strcmp(vl_ch,"VENT")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_appel_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_VentFort;
  }
  else if (!strcmp(vl_ch,"GRELE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_appel_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Grele;
  }
  /*traitement du cas particulier de l'incident sous concessionnaire*/
  else if (!strcmp(vl_ch,"FERMETURE STATION")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_IncSsConces;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_appel_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Incident);
    pl_tmp->Fonction.Comp.Valeur=1;
  }
  else if (!strcmp(vl_ch,"PENURIE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_IncSsConces;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_appel_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Incident);
    pl_tmp->Fonction.Comp.Valeur=0;
  }
  else {
    XZST_03EcritureTrace(   XZSTC_WARNING,"drol_appel_decode_condition : pb decodage type !!!");
    pl_cond->Fonction.Comp.Valeur=0;
  }
  }

  /*decode les autres conditions*/
  vl_str=strtok(vl_ch_init,C_ET);
  while ( (vl_str=strtok(NULL,C_ET))!=NULL)
  {
    /*allocation de la memoire pour la condition suivante*/
    pl_tmp->Suivant= (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_tmp->Suivant->Suivant=NULL;
    pl_tmp=pl_tmp->Suivant;

    drol_trim(vl_str);

    XZST_03EcritureTrace(   XZSTC_WARNING, "drol_appel_decode_condition:  cond %s",vl_str);
    strcpy(pl_tmp->Libelle,vl_str);
    /*il s'agit d'une fonction PREDICAT*/
    pl_tmp->Type=C_TYPE_PREDICAT;
    if (!strcmp(vl_str,C_COND_PEAGE)) pl_tmp->Fonction.Predicat.Fct=drol_appel_peage;
    else if (!strcmp(vl_str,C_COND_ECHANGEUR)) pl_tmp->Fonction.Predicat.Fct=drol_appel_echangeur;
    else if (!strcmp(vl_str,C_COND_LIT_ARRET)) pl_tmp->Fonction.Predicat.Fct=drol_appel_lit_arret;
    else if (!strcmp(vl_str,C_COND_AIRE)) pl_tmp->Fonction.Predicat.Fct=drol_appel_aire;
    else if (!strcmp(vl_str,C_COND_VIADUC)) pl_tmp->Fonction.Predicat.Fct=drol_appel_sur_viaduc;
    else if (!strcmp(vl_str,C_COND_TUNNEL)) pl_tmp->Fonction.Predicat.Fct=drol_appel_dans_tunnel;
    else if (!strcmp(vl_str,C_COND_CONFIRME)) pl_tmp->Fonction.Predicat.Fct=drol_appel_confirme;
    else if (!strcmp(vl_str,C_COND_SIGNALE)) pl_tmp->Fonction.Predicat.Fct=drol_appel_signale;
    else if (!strcmp(vl_str,C_COND_TERMINE)) pl_tmp->Fonction.Predicat.Fct=drol_appel_termine;
    else if (!strcmp(vl_str,C_COND_SANS_VOIE_BLOQUEE)) pl_tmp->Fonction.Predicat.Fct=drol_appel_sans_voie_bloquee;
    else if (!strcmp(vl_str,C_COND_AU_MOINS_1_VOIE)) pl_tmp->Fonction.Predicat.Fct=drol_appel_une_voie;
    else if (!strcmp(vl_str,C_COND_COUPURE_CHAUSSEE)) pl_tmp->Fonction.Predicat.Fct=drol_appel_coupure_chaussee;
    else if (!strcmp(vl_str,C_COND_NON_PREVU)) pl_tmp->Fonction.Predicat.Fct=drol_appel_non_prevu;
    else if (!strcmp(vl_str,C_COND_SENS_2)) pl_tmp->Fonction.Predicat.Fct=drol_appel_sens2;
    else if (!strcmp(vl_str,C_COND_SENS_1)) pl_tmp->Fonction.Predicat.Fct=drol_appel_sens1;
    else if (!strcmp(vl_str,C_COND_MATIERES)) pl_tmp->Fonction.Predicat.Fct=drol_appel_matieres;
    else if (!strcmp(vl_str,C_COND_PERSONNALITES)) pl_tmp->Fonction.Predicat.Fct=drol_appel_personnalites;
    else if (!strcmp(vl_str,C_COND_AGENT_ESCOTA)) pl_tmp->Fonction.Predicat.Fct=drol_appel_agent_escota;
    else if (!strcmp(vl_str,C_COND_DATEX)) pl_tmp->Fonction.Predicat.Fct=drol_appel_datex;
    else if (!strcmp(vl_str,C_COND_SUR_RAU)) pl_tmp->Fonction.Predicat.Fct=drol_appel_sur_rau;
    else if (!strcmp(vl_str,C_COND_ANIMAUX)) pl_tmp->Fonction.Predicat.Fct=drol_appel_animaux;
    else if (!strcmp(vl_str,C_COND_STOCKAGE_DEMARRE)) pl_tmp->Fonction.Predicat.Fct=drol_appel_stockage_demarre;
    else if (!strcmp(vl_str,C_COND_STOCKAGE_NON_DEMARRE)) pl_tmp->Fonction.Predicat.Fct=drol_appel_stockage_non_demarre;
    else if (!strcmp(vl_str,C_COND_TRANSITOIRE)) pl_tmp->Fonction.Predicat.Fct=drol_appel_transitoire;
    else if (!strcmp(vl_str,C_COND_ECOULEMENT)) pl_tmp->Fonction.Predicat.Fct=drol_appel_ecoulement;
    else if (!strcmp(vl_str,C_COND_NON_ECOULEMENT)) pl_tmp->Fonction.Predicat.Fct=drol_appel_non_ecoulement;
    else if (!strcmp(vl_str,C_COND_NATURE_ANIMAL)) pl_tmp->Fonction.Predicat.Fct=drol_appel_nature_animal;
    else if (!strcmp(vl_str,C_COND_MAJEUR)) {
      pl_tmp->Fonction.Predicat.Fct=drol_appel_majeur;
      *va_majeur=XDC_VRAI;
    }
    /*fonction de type variable ><= valeur*/
    else {
      pl_tmp->Type=C_TYPE_COMPARATEUR;
      /*recherche le signe =*/
      if ((vl_ch3=strchr(vl_str,'='))!=NULL) 
	pl_tmp->Fonction.Comp.Fct=drol_appel_egal;
      else if ((vl_ch3=strchr(vl_str,'>'))!=NULL)
	pl_tmp->Fonction.Comp.Fct=drol_appel_superieur;
      else if ((vl_ch3=strchr(vl_str,'<'))!=NULL)
	pl_tmp->Fonction.Comp.Fct=drol_appel_inferieur;

      vl_len= (int) (strlen(vl_str) - strlen(vl_ch3));
      strncpy(vl_ch2,vl_str,vl_len);
      vl_ch2[vl_len]='\0';
      vl_ch3++;
      vl_ch3++;
      drol_trim(vl_ch3);
      drol_trim(vl_ch2);
     
      /*memorisation de la valeur*/
      if (sscanf(vl_ch3,"%d",&vl_len)==1)
        pl_tmp->Fonction.Comp.Valeur=vl_len;
      else { /*la valeur est litterale*/
        if (!strcmp(vl_ch3,"VL")) pl_tmp->Fonction.Comp.Valeur=XZAEC_VEH_VL;
	else if (!strcmp(vl_ch3,"PL")) pl_tmp->Fonction.Comp.Valeur=XZAEC_VEH_PL;
	else if (!strcmp(vl_ch3,"Car")) pl_tmp->Fonction.Comp.Valeur=XZAEC_VEH_Car;
	else if (!strcmp(vl_ch3,"Moto")) pl_tmp->Fonction.Comp.Valeur=XZAEC_VEH_Moto;
	else if (!strcmp(vl_ch3,"Remorque")) pl_tmp->Fonction.Comp.Valeur=XZAEC_VEH_Remorque;
      }

      /*memorisation du slot a comparer*/
      if (!strcmp(vl_ch2,C_COND_DUREE)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Duree);
      else if (!strcmp(vl_ch2,C_COND_LONGUEUR)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Localisation.Longueur);
      else if (!strcmp(vl_ch2,C_COND_VISIBILITE)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Visibilite);
      else if (!strcmp(vl_ch2,C_COND_TYPE_VEHICULE)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Vehicule);
      else if (!strcmp(vl_ch2,C_COND_NB_PASSAGERS)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Nb_Passagers);
      else if (!strcmp(vl_ch2,C_COND_NB_PL)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Nb_PL);
      else if (!strcmp(vl_ch2,C_COND_NB_VL)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Nb_VL);
      else if (!strcmp(vl_ch2,C_COND_NB_MOTOS)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Nb_Motos);
      else if (!strcmp(vl_ch2,C_COND_NB_VEHICULES)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Nb_Vehicules);
      else if (!strcmp(vl_ch2,C_COND_NB_VICTIMES)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Nb_Victimes);
      else if (!strcmp(vl_ch2,C_COND_NB_MORTS)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Nb_Morts);
      else if (!strcmp(vl_ch2,C_COND_NB_BG)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Nb_BG);
      else if (!strcmp(vl_ch2,C_COND_NB_CARS)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Nb_Cars);
      else if (!strcmp(vl_ch2,C_COND_PR_MIN)) pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.PR_Min);
      else if (!strcmp(vl_ch2,C_COND_PR_MAX)) pl_cond->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.PR_Max);
    }
  }

  XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : drol_appel_decode_condition");
  return (pl_cond);
}


/**************************************/
/**************************************/
static XDY_Booleen drol_appel_echangeur() {
  return (vm_noeud->Localisation.TypePointCar==XDC_POINT_CARACT_ECHANGEUR);
}
static XDY_Booleen drol_appel_aire() {
  return (vm_noeud->Localisation.TypePointCar==XDC_POINT_CARACT_AIRE);
}
static XDY_Booleen drol_appel_dans_tunnel() {
  return (strcmp(vm_noeud->Localisation.Tunnel,""));
}
static XDY_Booleen drol_appel_sur_viaduc() {
  return (strcmp(vm_noeud->Localisation.Viaduc,""));
}
static XDY_Booleen drol_appel_peage() {
  return (vm_noeud->Localisation.TypePointCar==XDC_POINT_CARACT_PEAGE);
}

static XDY_Booleen drol_appel_egal(int va_valeur1, int va_valeur2) { return (va_valeur1==va_valeur2); }
static XDY_Booleen drol_appel_true(int va_valeur1, int va_valeur2) { return (TRUE); }
static XDY_Booleen drol_appel_superieur(int va_valeur1, int va_valeur2) { return (va_valeur1>va_valeur2); }
static XDY_Booleen drol_appel_inferieur(int va_valeur1, int va_valeur2) { return (va_valeur1<va_valeur2); }

static XDY_Booleen drol_appel_lit_arret() {
  return (vm_noeud->Localisation.TypePointCar==XDC_POINT_CARACT_LIT_ARRET);
}

static XDY_Booleen drol_appel_confirme() { return (vm_noeud->Infos_TFM.Confirme); }
static XDY_Booleen drol_appel_signale() { return (vm_noeud->Infos_TFM.Signale); }
static XDY_Booleen drol_appel_termine() { return (vm_noeud->Infos_TFM.Termine); }
static XDY_Booleen drol_appel_non_prevu() { return (!vm_noeud->Infos_TFM.Prevu); }

static XDY_Booleen drol_appel_sans_voie_bloquee() {
  return (!(drol_appel_une_voie()));
}
static XDY_Booleen drol_appel_une_voie() {
  return ((vm_noeud->Localisation.VL==XDC_VOIE_BLOQUEE) ||
          (vm_noeud->Localisation.VM1==XDC_VOIE_BLOQUEE) ||
	  (vm_noeud->Localisation.VM2==XDC_VOIE_BLOQUEE) ||
	  (vm_noeud->Localisation.VR==XDC_VOIE_BLOQUEE) ||
	  (vm_noeud->Localisation.VL==XDC_VOIE_BLOQUEE_NEUTRALISEE) ||
	  (vm_noeud->Localisation.VM1==XDC_VOIE_BLOQUEE_NEUTRALISEE) ||
	  (vm_noeud->Localisation.VM2==XDC_VOIE_BLOQUEE_NEUTRALISEE) ||
	  (vm_noeud->Localisation.VR==XDC_VOIE_BLOQUEE_NEUTRALISEE)
	 );
}
static XDY_Booleen drol_appel_coupure_chaussee() {
  return ((vm_noeud->Localisation.VL!=XDC_VOIE_SENS_NORMAL) &&
	  (vm_noeud->Localisation.VL!=XDC_VOIE_INCONNUE) &&
	  (vm_noeud->Localisation.VL!=XDC_VOIE_INEXISTANTE) &&
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
static XDY_Booleen drol_appel_sens2() {
  if ( (vm_noeud->Localisation.Sens==XDC_SENS_NORD) ||
       (vm_noeud->Localisation.Sens==XDC_SENS_ENTREE_NORD) ||
       (vm_noeud->Localisation.Sens==XDC_SENS_SORTIE_NORD) )
    return (TRUE);
  else
    return (FALSE);
}
static XDY_Booleen drol_appel_sens1() {
  return (!(drol_appel_sens2()));
}
static XDY_Booleen drol_appel_matieres() {
  return (vm_noeud->Infos_FAX.Matieres_Dangereuses);
}
static XDY_Booleen drol_appel_animaux() {
  return (vm_noeud->Infos_FAX.Animaux_Vivants);
}
static XDY_Booleen drol_appel_personnalites() { return (vm_noeud->Infos_FAX.Personnalites); }
static XDY_Booleen drol_appel_agent_escota() {return (vm_noeud->Infos_FAX.Agent_Escota); }
static XDY_Booleen drol_appel_datex() { return (vm_noeud->Infos_TFM.Datex==XDC_VRAI); } /*SAGA*/
static XDY_Booleen drol_appel_sur_rau() { 
  if (vm_noeud->Infos_TFM.Type_Travaux==XZAEC_TRV_CableRAU) return (TRUE);
  else return (FALSE);
}

static XDY_Booleen drol_appel_stockage_demarre() { 
  return (vm_noeud->Localisation.Voies_Retrecies);
}

static XDY_Booleen drol_appel_stockage_non_demarre() { 
  if (vm_noeud->Localisation.Voies_Retrecies == XDC_VRAI) {
    return (XDC_FAUX);
  }
  return (XDC_VRAI);
}

static XDY_Booleen drol_appel_transitoire() {
  if (strlen(vm_noeud->Infos_FAX.Transitoire)>0)
    return (XDC_VRAI);
  return (XDC_FAUX); 
}

static XDY_Booleen drol_appel_ecoulement() {
  if (vm_noeud->Infos_FAX.Ecoulement == XDC_VRAI)
    return (XDC_VRAI);
  return (XDC_FAUX); 
}

static XDY_Booleen drol_appel_non_ecoulement() {
  if (vm_noeud->Infos_FAX.Ecoulement == XDC_VRAI)
    return (XDC_FAUX);
  return (XDC_VRAI); 
}

static XDY_Booleen drol_appel_majeur() {
  if (vm_noeud->Infos_TFM.Majeur==XDC_VRAI)
    return (XDC_VRAI);
  return (XDC_FAUX);
}

static XDY_Booleen drol_appel_nature_animal() {
  /* Contrainte sur le type FMC par securite */
  if (vm_noeud->Type == XZAEC_FMC_Obstacle)
  {
    if (strcasestr (vm_noeud->Infos_FAX.Objet, "ANIMAL") != NULL)
      return (XDC_VRAI);
  }
  return (XDC_FAUX);
}

/**************************************/
/**************************************/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des cas d'appels
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen drol_appel_filtre(T_NOEUD *va_noeud, XDY_District va_site, XDY_Horodate va_horodate, XDY_Booleen *va_sms)

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
  T_CAS *pl_cas;
  T_CONDITION *pl_cond;
  XDY_Booleen (*vl_fct)();
  XDY_Booleen vl_res;
  XDY_Booleen vl_fax_a_emettre=FALSE;
  XDY_Booleen vl_SMS_a_emettre=FALSE;
  XDY_Booleen vl_ret;
 int vl_indice;

    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_appel_filtre EVT=%d/%d, %d / %d",
					va_noeud->NumEvt.numero,
					va_noeud->NumEvt.cle,
					va_noeud->Type,
					va_site);
  *va_sms = vl_SMS_a_emettre;
  vm_noeud=va_noeud;
  vm_noeud2->Localisation = vm_noeud->Localisation;
  vm_noeud2->Infos_FAX = vm_noeud->Infos_FAX;
  vm_noeud2->Type=vm_noeud->Type;

  if (va_site==vg_SitePrincipal)
    pl_cas= vg_cas_ci;
  else
    pl_cas= vg_cas_pc;

  vm_TabAjout = NULL;
  vm_IndexTabAjout = 0;
  vm_TabSuppr = NULL;
  vm_IndexTabSuppr = 0;

  for (vl_indice=0; vl_indice < C_MAX_PEAGES; vl_indice++) {
    tm_peages[vl_indice].Utilise = FALSE;
  }

  /*au CI, ajoute les interlocuteurs CI deja contactes*/
  if (va_site==vg_SitePrincipal)
    XZAC23_Rech_Partenaires_Informes(va_horodate, va_noeud->NumEvt, vg_site, FctTabAjout);

  /*si evt pas termine, supprimer les interlocuteurs PC deja contactes*/
  if ( ((vg_site==XDC_CI) || (vg_site==va_site)) &&
       (!va_noeud->Infos_TFM.Termine) &&
       (va_noeud->Type != XZAEC_FMC_Regulation) &&
       (va_site!=XDC_CI) &&
       (va_site!=XDC_CA) )
    XZAC23_Rech_Partenaires_Informes(va_horodate, va_noeud->NumEvt, va_site, FctTabSuppr);

  while (pl_cas!=NULL)
  {
    pl_cond=pl_cas->Condition;
    vl_res=TRUE;
    XZST_03EcritureTrace(XZSTC_INFO,"drol_appel_filtre cas = %s", pl_cas->Libelle);

    while (pl_cond!=NULL)
    {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_appel_filtre condition = %s",
		pl_cond->Libelle);

      if (pl_cond->Type==C_TYPE_COMPARATEUR)
      {
        XZST_03EcritureTrace(   XZSTC_DEBUG1,"drol_appel_filtre slot=%d valeur=%d",
					*(pl_cond->Fonction.Comp.Slot),
					pl_cond->Fonction.Comp.Valeur);

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
    if (vl_res) {
      XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_appel_filtre OK pour %s (%d)",
					pl_cas->Libelle,
					pl_cas->Nombre_Interlocuteurs);
      va_noeud->Cas=pl_cas;

      /*met a jour la liste des interlocuteurs a appeler*/
      vl_ret=drol_appel_interlocuteurs(pl_cas,va_noeud, va_site, &vl_SMS_a_emettre);
      *va_sms = *va_sms || vl_SMS_a_emettre;
      vl_fax_a_emettre = vl_fax_a_emettre || vl_ret;
    }
    pl_cas=pl_cas->Suivant;
  }

  vl_fax_a_emettre = (drol_appel_a_recontacter(va_noeud,va_site))||vl_fax_a_emettre;

  if (vm_TabAjout!=NULL)
  free(vm_TabAjout);
  if (vm_TabSuppr!=NULL)
  free(vm_TabSuppr);

  XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_appel_filtre out SMS=%d",*va_sms);
  va_noeud->Cas=NULL;
  return (vl_fax_a_emettre);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de mise a jour des interlocuteurs a appeler
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen drol_appel_interlocuteurs(T_CAS *pa_cas, T_NOEUD *va_noeud, XDY_District va_site, XDY_Booleen *va_sms)

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
  T_INTERLOCUTEUR	*pl_liste;
  T_INTERLOCUTEUR	*pl_tmp;
  int			vl_indice;
  XDY_Booleen		vl_fax_a_emettre;
  int			vl_i;
  XDY_Entier vl_cr;
  char vl_str[500];

  XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_appel_interlocuteurs %s,%d",
				pa_cas->Libelle,va_site);

  vl_fax_a_emettre=FALSE;
  *va_sms = FALSE;

    if (va_site==XDC_CI)
      /* On est au CI ESCOTA */
      pl_liste=vg_interlocuteurs_ci;
    else
      /* On est soit sur un PC ESCOTA, soit on est sur COFIROUTE et la liste PC est mergee avec la liste CI */
      pl_liste=vg_interlocuteurs_pc;

  /*pour chaque interlocuteur a appeler*/
  for (vl_indice=0;vl_indice<pa_cas->Nombre_Interlocuteurs;vl_indice++)
  {
    if (va_site==XDC_CI)
      /* On est au CI ESCOTA */
      pl_liste=vg_interlocuteurs_ci;
    else
      /* On est soit sur un PC ESCOTA, soit on est sur COFIROUTE et la liste PC est mergee avec la liste CI */
      pl_liste=vg_interlocuteurs_pc;

    /*je commence par regarder si je n'ai pas deja contacte cet interlocuteur*/
    for (vl_i=0; vl_i < vm_IndexTabSuppr; vl_i++) {
      if (vm_TabSuppr[vl_i] == pa_cas->Interlocuteurs[vl_indice]) {
	XZST_03EcritureTrace(   XZSTC_FONCTION,"SUPPRIMER %d",pa_cas->Interlocuteurs[vl_indice]);
	goto pos_next;
      }
    } 

    /*recherche l'interlocuteur a contacter dans la liste des interlocuteurs*/
    while (pl_liste!=NULL)
    {
	XZST_03EcritureTrace(   XZSTC_DEBUG1,"APPEL %d %d",
					pl_liste->Type_Astreinte,
					pl_liste->A_Appeler);

      if (!(pl_liste->A_Appeler)) {
      /*si je trouve mon interlocuteur*/
      if (pl_liste->Type_Astreinte==pa_cas->Interlocuteurs[vl_indice]) {
	XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_appel_interlocuteur A APPELER (%d) SMS (%d)",
				pl_liste->Type_Astreinte, pl_liste->Appel_SMS);
	/*je memorise que je dois l'appeler*/
	pl_liste->A_Appeler=TRUE;
	strcpy(pl_liste->Remarque,"");
	strcpy(pl_liste->Justification,pa_cas->Libelle);
	pl_liste->Autoroute=va_noeud->Localisation.Autoroute;
	pl_liste->PR=va_noeud->Localisation.PR;
	pl_liste->Sens=va_noeud->Localisation.Sens;
	if (pl_liste->Appel_Fax==1) vl_fax_a_emettre=TRUE;
	if (pl_liste->Appel_Mail==1) vl_fax_a_emettre=TRUE;
	if (pl_liste->Appel_SMS==1) *va_sms =TRUE;


	/*cas particulier de l'evt majeur en astreinte peage*/
  if (vm_noeud->Infos_TFM.Majeur==XDC_VRAI)  {
/*	if (pa_cas->Majeur == XDC_VRAI) {*/
	  if (pl_liste->Type_Astreinte == XDC_AST_PEAGE)  {
	    if (va_noeud->Type == XZAEC_FMC_Contresens) {
	      /*rechercher les peages amont*/
	      XZAO445_Rechercher_Peages_Amont(va_noeud->Localisation.Autoroute,
	    			XDC_SENS_SUD,
				va_noeud->Localisation.PR,
				va_noeud->Localisation_Tete.PR,
				0,
				va_site,
				(XDY_Horodate)0,
				pa_cas->Libelle,
				(XDY_FonctionInt)ajouter_peage,
				&vl_cr);
	      XZAO445_Rechercher_Peages_Amont(va_noeud->Localisation.Autoroute,
	    			XDC_SENS_NORD,
				va_noeud->Localisation.PR,
				va_noeud->Localisation_Tete.PR,
				0,
				va_site,
				(XDY_Horodate)0,
				pa_cas->Libelle,
				(XDY_FonctionInt)ajouter_peage,
				&vl_cr);
	    }
	    else {
	      XZAO445_Rechercher_Peages_Amont(va_noeud->Localisation.Autoroute,
	    			va_noeud->Localisation.Sens,
				va_noeud->Localisation.PR,
				va_noeud->Localisation_Tete.PR,
				0,
				va_site,
				(XDY_Horodate)0,
				pa_cas->Libelle,
				(XDY_FonctionInt)ajouter_peage,
				&vl_cr);
	    }
	    /*les rajouter dans la remarque du cas d appel*/
	    strcpy(vl_str,"");
	    for (vl_indice=0; vl_indice<C_MAX_PEAGES; vl_indice++) {
	      if ((tm_peages[vl_indice].Utilise)) {
		if (vl_indice==0)
		  strcpy(vl_str," ");
		else 
		  strcat(vl_str,", ");
	        strcat(vl_str,tm_peages[vl_indice].Nom);
	      }
	    }
	    strncpy(pl_liste->Remarque, vl_str,250);
	    pl_liste->Remarque[250] = XDC_FIN_CHN;
	  }
	}
      }
      }

      /*interlocuteur suivant dans la liste des interlocuteurs*/
      pl_liste=pl_liste->Suivant;
    }
pos_next:
    XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_appel_interlocuteur filtre");
  }

  return (vl_fax_a_emettre);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de mise a jour des interlocuteurs a recontacter
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen drol_appel_a_recontacter(T_NOEUD *va_noeud, XDY_District va_site)

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
  T_INTERLOCUTEUR	*pl_liste;
  int			vl_indice;
  XDY_Booleen		vl_fax_a_emettre;
  int			vl_i;

  XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_appel_a_recontacter %d = %d",va_site,vm_IndexTabAjout);

  if (vm_IndexTabAjout==0) return (FALSE);

  vl_fax_a_emettre=FALSE;

    if (va_site==XDC_CI)
      /* On est au CI ESCOTA */
      pl_liste=vg_interlocuteurs_ci;
    else
      /* On est soit sur un PC ESCOTA, soit on est sur COFIROUTE et la liste PC est mergee avec la liste CI */
      pl_liste=vg_interlocuteurs_pc;

  /*je traite maintenant les interlocuteurs a recontacter */
  for (vl_i=0; vl_i < vm_IndexTabAjout; vl_i++)
  {
    XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_appel_interlocuteur deja interl %d",
			vm_TabAjout[vl_i]);
    if (va_site==XDC_CI) {
      /* On est au CI ESCOTA */
      XZST_03EcritureTrace(   XZSTC_FONCTION,"AJOUT CI");
      pl_liste=vg_interlocuteurs_ci;
    }
    else {
      /* On est soit sur un PC ESCOTA, soit on est sur COFIROUTE et la liste PC est mergee avec la liste CI */
      XZST_03EcritureTrace(   XZSTC_FONCTION,"AJOUT PC ESCOTA ou site COFIROUTE");
      pl_liste=vg_interlocuteurs_pc;
    }
    while (pl_liste!=NULL)
    {  
      if (pl_liste->Type_Astreinte==vm_TabAjout[vl_i])
      {
	/*je memorise que je dois l'appeler*/
    XZST_03EcritureTrace(   XZSTC_FONCTION,"AJOUT %s, tel %d, fax %d",pl_liste->Competence,
		pl_liste->Appel_Tel,pl_liste->Appel_Fax);
	pl_liste->A_Appeler=TRUE;
	strcpy(pl_liste->Remarque,"");
	strcpy(pl_liste->Justification,"interlocuteur a recontacter");
	pl_liste->Autoroute=va_noeud->Localisation.Autoroute;
	pl_liste->PR=va_noeud->Localisation.PR;
	pl_liste->Sens=va_noeud->Localisation.Sens;
	vl_fax_a_emettre=TRUE;
      }
      pl_liste=pl_liste->Suivant;
    }
  }
      
  return (vl_fax_a_emettre);
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de generation de la proposition d'appel tel
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void drol_appel_elabore(XDY_District va_site)

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
  T_INTERLOCUTEUR	*pl_liste;
  int			vl_cr;

  if (va_site==XDC_CI)
    /* On est au CI ESCOTA */
    pl_liste=vg_interlocuteurs_ci;
  else
    /* On est soit sur un PC ESCOTA, soit on est sur COFIROUTE et la liste PC est mergee avec la liste CI */
    pl_liste=vg_interlocuteurs_pc;

  while (pl_liste!=NULL)
  {
    XZST_03EcritureTrace(   XZSTC_FONCTION,"ELAB %s,%d, tel %d",pl_liste->Competence,
					pl_liste->A_Appeler,pl_liste->Appel_Tel);
				
    /*si je dois contacter l'interlocuteur par tel*/
    if ((pl_liste->A_Appeler) && (pl_liste->Appel_Tel==1))
      vl_cr = XZAP08_Ajout_Prop_Tel(vg_numero_plan, 
				vg_ordre_appel++, 
				va_site, 
				pl_liste->Remarque, 
				pl_liste->Justification, 
				"Appel ", 
				pl_liste->PR, 
				pl_liste->Autoroute, 
				pl_liste->Sens, 
				pl_liste->Type_Astreinte);

    pl_liste=pl_liste->Suivant;
  }
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de remise a jour des listes des interlocuteurs (statiques)
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void drol_appel_reinit()

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
  T_INTERLOCUTEUR       *pl_liste;

  pl_liste=vg_interlocuteurs_ci;
  while (pl_liste!=NULL) 
  {
    pl_liste->A_Appeler=FALSE;
    pl_liste=pl_liste->Suivant;
  }

  pl_liste=vg_interlocuteurs_pc;
  while (pl_liste!=NULL) 
  {
    pl_liste->A_Appeler=FALSE;
    pl_liste=pl_liste->Suivant;
  }
}
