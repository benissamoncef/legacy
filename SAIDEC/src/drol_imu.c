/* Fichier : $Id: drol_imu.c,v 1.1 2021/07/20 12:27:25 pc2dpdy Exp $        $Revision: 1.1 $        $Date: 2021/07/20 12:27:25 $
**************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : SAIDEC				*
* FICHIER : drol_imu.c				*
*************************************************
* DESCRIPTION :
* new 
*************************************************
* HISTORIQUE :
*
* ABE	21/05/2021	Creation IMU DEM-SAE155 1.1
* GGY	18/01/24 	Ajout C_COND_CIRCULATION_BASC_TUNNEL et C_COND_CIRCULATION_NON_BASC_TUNNEL (DEM-483)
************************************************/

#include "drol_imu.h"

/* Scenarios specifiques proposes */


/* Valeurs des limites de vitesse */


/* Valeurs des distances maximales a l'evenement pour activation */



/* Description de la configuration */

#define C_REP_MIG							"/produits/migrazur/appliSD/fichiers/bdc/"
#define C_PERT		C_REP_MIG 				"imu.txt"

#define C_SEPARATEUR						"#"
#define C_PIPE								"|"
#define C_ET								"$ET$"
#define C_CAR_DOLLAR						'$'

#define C_COND_SUR_BAU						"sur bau"
#define C_COND_SUR_SECTION_COURANTE			"sur section courante"
#define C_COND_SUR_BRETELLE_ENTREE			"sur bretelle entree"
#define C_COND_BOUCHON_MOBILE				"bouchon mobile"
#define C_COND_CIRCULATION_BASC_TUNNEL		"circulation_basculee"
#define C_COND_CIRCULATION_NON_BASC_TUNNEL	"circulation_non_basculee"

/*fonctions utilisees dans les conditions*/
static XDY_Booleen drol_imu_egal(int, int);
static XDY_Booleen drol_imu_sur_bretelle_entree();
static XDY_Booleen drol_imu_fmc_simulee();
static XDY_Booleen drol_imu_bouchon_mobile();
static XDY_Booleen drol_imu_sur_bau();
static XDY_Booleen drol_imu_sur_section_courante();
static XDY_Booleen drol_imu_circulation_basculee_tunnel ();
static XDY_Booleen drol_imu_circulation_non_basculee_tunnel ();

T_CONDITION *drol_imu_decode_condition(char *);
int drol_imu_charge();
static int drol_imu_recherche(T_PERT_IMU *va_pert,int *va_NumEqtIMU);
static int drol_imu_voies(int, int, int, int, int);

/* definition des variables locales au module */
T_PERT_IMU      *vg_pert_imu;
T_NOEUD		*vm_noeud;
static T_NOEUD	*vm_noeud2;

static char *version = "$Id: drol_imu.c,v 1.1 2021/07/20 12:27:25 pc2dpdy Exp $ : drol_imu" ;

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_imu_init()

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
*   fonction passee en argument a 
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

  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : drol_imu_init");
  vm_noeud2=(T_NOEUD *)malloc(sizeof(T_NOEUD));

  drol_imu_charge();

  XZST_03EcritureTrace(   XZSTC_FONCTION,"OUT drol_imu_init");

  return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des perturbations
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_imu_charge()

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
  T_PERT_IMU *pl_liste;
  int	vl_num_prec,vl_num;
  char vl_condition[300];
  XDY_Sens tests;
  XDY_PR testi;

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : drol_imu_charge");

  if ( (pl_fp = fopen(C_PERT, "r")) == NULL ) {
    XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !" ,C_PERT );
    return(XDC_NOK);
  }
  
  while (fgets(vl_Lig,sizeof(vl_Lig),pl_fp) != NULL)
  {
      pl_liste = (T_PERT_IMU *)malloc(sizeof(T_PERT_IMU));
      vl_str=vl_Lig; 
      XZST_03EcritureTrace(XZSTC_WARNING, "ligne = %s",vl_str);
      if(sscanf(vl_str,"%[^|]|%[^|]|%[^|]|%hhu|%d|%d|%[^|]|%[^|]|%d|%d|%d|%[^|]|",
							pl_liste->Libelle,								
							vl_condition,
							pl_liste->NomAutoroute,
							&pl_liste->Sens,
							&pl_liste->PR_Debut,
							&pl_liste->PR_Fin,
							pl_liste->NomBICE,
							pl_liste->Message,
							&pl_liste->Cycles,
							&pl_liste->Intervalle,
							&pl_liste->Duree,
							pl_liste->ListeZone)!=12)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "Erreur décodage perturbation IMU. fichier :%s",C_PERT);
	}

	XZST_03EcritureTrace(XZSTC_WARNING, "drol_imu_charge : libelle:%s, condition:%s, Autoroute=%s, Sens:%d, PR_Debut=%d, PR_fin=%d, NomBICE=%s, Message=%s, Cycle=%d, Intervalle=%d, Duree=%d, ListeZone=%s",
      				pl_liste->Libelle,
				vl_condition,
				pl_liste->NomAutoroute,
				pl_liste->Sens,
				pl_liste->PR_Debut,
				pl_liste->PR_Fin,
				pl_liste->NomBICE,
				pl_liste->Message,
				pl_liste->Cycles,
				pl_liste->Intervalle,
				pl_liste->Duree,
				pl_liste->ListeZone);
	pl_liste->Condition = drol_imu_decode_condition(vl_condition);
	pl_liste->Suivant = vg_pert_imu;
	vg_pert_imu = pl_liste;

  }
  fclose(pl_fp);

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT  : drol_imu_charge");
  return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* decodage de la condition
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

T_CONDITION *drol_imu_decode_condition(char *va_cond)

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
  XZST_03EcritureTrace(XZSTC_FONCTION, "drol_imu_decode_condition:  IN %s",va_cond);

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
  pl_cond->Fonction.Comp.Fct=drol_imu_egal;
  pl_cond->Suivant=NULL;
  pl_tmp=pl_cond;

  /*numero en fonction du type*/
  XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_imu_decode_condition: type = %s",pl_cond->Libelle);

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
  else if (!strcmp(vl_ch,"Echangeur déconseille")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_EchDeconseille;
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
    pl_tmp->Fonction.Comp.Fct=drol_imu_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Verglas;
  }
  else if (!strcmp(vl_ch,"CHUTE NEIGE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_imu_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_ChuteNeige;
  }
  else if (!strcmp(vl_ch,"PLUIE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_imu_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Pluie;
  }
  else if (!strcmp(vl_ch,"BROUILLARD GIVRANT")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_imu_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_BrouillardGiv;
  }
  else if (!strcmp(vl_ch,"NEIGE SUR CHAUSSEE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_imu_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_NeigeChaus;
  }
  else if (!strcmp(vl_ch,"GRELE SUR CHAUSSEE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_imu_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_GreleChaus;
  }
  else if (!strcmp(vl_ch,"BROUILLARD")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_imu_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Brouillard;
  }
  else if (!strcmp(vl_ch,"VENT")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_imu_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_VentFort;
  }
  else if (!strcmp(vl_ch,"GRELE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_imu_egal;
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
    pl_tmp->Fonction.Comp.Fct=drol_imu_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Incident);
    pl_tmp->Fonction.Comp.Valeur=1;
  }
  else if (!strcmp(vl_ch,"PENURIE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_IncSsConces;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_imu_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Incident);
    pl_tmp->Fonction.Comp.Valeur=0;
  }
  /*cas particulier du lit d'arret*/
  else if (!strcmp(vl_ch,"sur lit d'arret")) {
    pl_cond->Fonction.Comp.Slot=&(vm_noeud2->Localisation.TypePointCar);
    pl_cond->Fonction.Comp.Valeur=XDC_POINT_CARACT_LIT_ARRET;
  }
  else {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_imu_decode_condition : pb decodage type !!!");
    pl_cond->Fonction.Comp.Valeur=0;
  }
 
  /*si pas de condition supplementaire*/
  if (vl_indice==strlen(va_cond)) {
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : drol_imu_decode_condition : que le type");
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

    XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_imu_decode_condition:  cond %s",vl_str);
    strcpy(pl_tmp->Libelle,vl_str);

    /* Le type de condition est suppose etre un PREDICAT par defaut */
    pl_tmp->Type=C_TYPE_PREDICAT;
 /*   if (!strcmp(vl_str,C_COND_UNE_VOIE_BLOQ)) pl_tmp->Fonction.Predicat.Fct=drol_imu_une_voie_bloquee;
    else if (!strcmp(vl_str,C_COND_UNE_VOIE_OUVERTE)) pl_tmp->Fonction.Predicat.Fct=drol_imu_une_voie_ouverte;
    else if (!strcmp(vl_str,C_COND_TOUTES_VOIES_BLOQ)) pl_tmp->Fonction.Predicat.Fct=drol_imu_toutes_voies_bloquees;
    else if (!strcmp(vl_str,C_COND_TOUTES_VOIES_NEUT)) pl_tmp->Fonction.Predicat.Fct=drol_imu_toutes_voies_neutralisees;
    else if (!strcmp(vl_str,C_COND_UNE_VOIE_NEUT)) pl_tmp->Fonction.Predicat.Fct=drol_imu_une_voie_neutralisee;
    else if (!strcmp(vl_str,C_COND_CIRCULATION_UNE_VOIE)) pl_tmp->Fonction.Predicat.Fct=drol_imu_circulation_une_voie;
    else if (!strcmp(vl_str,C_COND_CONFIRME)) pl_tmp->Fonction.Predicat.Fct=drol_imu_confirme;
    else if (!strcmp(vl_str,C_COND_SIGNALE)) pl_tmp->Fonction.Predicat.Fct=drol_imu_signale;*/
    if (!strcmp(vl_str,C_COND_SUR_BAU)) pl_tmp->Fonction.Predicat.Fct=drol_imu_sur_bau;
    else if (!strcmp(vl_str,C_COND_SUR_SECTION_COURANTE)) pl_tmp->Fonction.Predicat.Fct=drol_imu_sur_section_courante;
    else if (!strcmp(vl_str,C_COND_SUR_BRETELLE_ENTREE)) pl_tmp->Fonction.Predicat.Fct=drol_imu_sur_bretelle_entree;
    else if (!strcmp(vl_str,C_COND_BOUCHON_MOBILE)) pl_tmp->Fonction.Predicat.Fct=drol_imu_bouchon_mobile;
 /*   else if (!strcmp(vl_str,C_COND_SUR_LIT_ARRET)) pl_tmp->Fonction.Predicat.Fct=drol_imu_sur_lit_arret;
    else if (!strcmp(vl_str,C_COND_SUR_ECHANGEUR)) pl_tmp->Fonction.Predicat.Fct=drol_imu_sur_echangeur;
    else if (!strcmp(vl_str,C_COND_SUR_BRETELLE_ENTREE)) pl_tmp->Fonction.Predicat.Fct=drol_imu_sur_bretelle_entree;
    else if (!strcmp(vl_str,C_COND_SUR_BRETELLE_SORTIE)) pl_tmp->Fonction.Predicat.Fct=drol_imu_sur_bretelle_sortie;
    else if (!strcmp(vl_str,C_COND_TRAFIC_DENSE)) pl_tmp->Fonction.Predicat.Fct=drol_imu_trafic_dense;
    else if (!strcmp(vl_str,C_COND_TRAFIC_RALENTI)) pl_tmp->Fonction.Predicat.Fct=drol_imu_trafic_ralenti;
    else if (!strcmp(vl_str,C_COND_TRAFIC_BOUCHE)) pl_tmp->Fonction.Predicat.Fct=drol_imu_trafic_bouche;
    else if (!strcmp(vl_str,C_COND_TRAFIC_BLOQUE)) pl_tmp->Fonction.Predicat.Fct=drol_imu_trafic_bloque;
    else if (!strcmp(vl_str,C_COND_DATEX)) pl_tmp->Fonction.Predicat.Fct=drol_imu_datex;
    else if (!strcmp(vl_str,C_COND_VOIES_RETRECIES)) pl_tmp->Fonction.Predicat.Fct=drol_imu_voies_retrecies;
    else if (!strcmp(vl_str,C_COND_STOCKAGE_DEMARRE)) pl_tmp->Fonction.Predicat.Fct=drol_imu_stockage_demarre;
    else if (!strcmp(vl_str,C_COND_STOCKAGE_NON_DEMARRE)) pl_tmp->Fonction.Predicat.Fct=drol_imu_stockage_non_demarre;
    else if (!strcmp(vl_str,C_COND_ENTREE_FERMEE)) pl_tmp->Fonction.Predicat.Fct=drol_imu_entree_fermee;
    else if (!strcmp(vl_str,C_COND_SORTIE_FERMEE)) pl_tmp->Fonction.Predicat.Fct=drol_imu_sortie_fermee;*/
	else if (!strcmp(vl_str,C_COND_CIRCULATION_BASC_TUNNEL)) pl_tmp->Fonction.Predicat.Fct = drol_imu_circulation_basculee_tunnel;
	else if (!strcmp(vl_str,C_COND_CIRCULATION_NON_BASC_TUNNEL)) pl_tmp->Fonction.Predicat.Fct = drol_imu_circulation_non_basculee_tunnel;
    else {
      pl_tmp->Type=C_TYPE_COMPARATEUR;
      pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Type);
      pl_tmp->Fonction.Comp.Fct=drol_imu_egal;
      pl_tmp->Fonction.Comp.Valeur=0;

      if (sscanf(vl_str, " vitesse = %d", &vl_vitesse) > 0) {
        pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Vitesse);
        pl_tmp->Fonction.Comp.Valeur=vl_vitesse;
      } else {
        XZST_03EcritureTrace(   XZSTC_WARNING,"drol_imu_decode_condition : pb decodage condition !!!C_COND_SUR_BRETELLE_ENTREE:%s.",vl_str);
      }
    }
 
  /*si fin de chaine*/
  if (vl_indice==strlen(va_cond)) {
    XZST_03EcritureTrace(   XZSTC_DEBUG1,"drol_imu_decode_condition : fin de chaine");
    return (pl_cond);
  }
  else {
    va_cond=va_cond+strlen(vl_str) + strlen(C_ET);
    goto pos_decode;
  }
}

/**************************************/
/**************************************/
static XDY_Booleen drol_imu_fmc_simulee() {
  return (strstr (vm_noeud->Infos_TFM.Origine, "ESSAI") != NULL  ||
          strstr (vm_noeud->Infos_TFM.Origine, "EXERCICE") != NULL);
}

static XDY_Booleen drol_imu_bouchon_mobile() {
  if (vm_noeud->Infos_FAX.Presence_Bouchon == XDC_VRAI)
	return (XDC_VRAI);
  return (XDC_FAUX);
}

static XDY_Booleen drol_imu_sur_bau() { 
  return ( (drol_imu_voies(vm_noeud->Localisation.VL,vm_noeud->Localisation.VM1,
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
static XDY_Booleen drol_imu_sur_section_courante() { 
  return ( (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_AIRE) &&
	   (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_ECHANGEUR) &&
	   (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_PEAGE) &&
	   (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_LIT_ARRET) &&
	   (drol_imu_voies(vm_noeud->Localisation.VL,vm_noeud->Localisation.VM1,
		    vm_noeud->Localisation.VM2,
		    vm_noeud->Localisation.VR,
		    XDC_VOIE_SENS_NORMAL)!=15)
         );
}

/*renvoie TRUE si l evt sur la chaussee circulee*/
static XDY_Booleen drol_imu_sur_bretelle_entree() { 
 return ( (vm_noeud->Localisation.TypePointCar==XDC_POINT_CARACT_ECHANGEUR) && ((vm_noeud->Localisation.Sens==XDC_SENS_ENTREE_NORD) || (vm_noeud->Localisation.Sens==XDC_SENS_ENTREE_SUD))
         );
}

static XDY_Booleen drol_imu_egal(int va_valeur1, int va_valeur2) 
{ return (va_valeur1==va_valeur2); }

static XDY_Booleen drol_imu_circulation_basculee_tunnel ()
{
	XDY_Booleen vl_basculement = XDC_FAUX;
	
	XZST_03EcritureTrace(XZSTC_WARNING,"drol_imu_circulation_basculee_tunnel : avant XZAE861_Basculement_Tunnel");

	if (XZAE861_Basculement_Tunnel (vm_noeud->Localisation.Autoroute,
									vm_noeud->Localisation.PR,
									vm_noeud->Localisation_Tete.PR,
									vl_basculement)	!= XDC_OK) 
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_imu_circulation_basculee_tunnel : pb appel a XZAE861_Basculement_Tunnel");
		return (FALSE);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_imu_circulation_basculee_tunnel : XZAE861_Basculement_Tunnel retourne %s", vl_basculement);
	}
	
	return (vl_basculement);
}

static XDY_Booleen drol_imu_circulation_non_basculee_tunnel ()
{
	XZST_03EcritureTrace (XZSTC_WARNING, "drol_imu_circulation_non_basculee_tunnel : not drol_imu_circulation_basculee_tunnel");
	return (!drol_imu_circulation_basculee_tunnel());
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  compare l'etat de chaque voie a l'etat de reference
*  tient compte du nombre de voies de circulation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_imu_voies(int VL, int VM1, int VM2, int VR, int Reference)

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

XDY_Booleen drol_imu_filtre(T_NOEUD *va_noeud, XDY_District va_site, XDY_Horodate va_horodate)


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
	T_PERT_IMU		*pl_cas;
	T_CONDITION 	*pl_cond;
	XDY_Booleen 	(*vl_fct)();
	XDY_Booleen 	vl_res=FALSE;
	T_PERT_IMU		*vl_pert;
	int 			vl_indice;
	XDY_Sens		vl_sens;
	XDY_Booleen		vl_bouchon;
	int				vl_NumEqtIMU;
	XDY_ChaineIMU	vl_chaineIMU;
	int				i=0;

	XZST_03EcritureTrace (XZSTC_FONCTION,"IN : drol_imu_filtre %d/%d", va_noeud->NumEvt.numero, va_noeud->NumEvt.cle);

	vm_noeud=va_noeud;
	vm_noeud2->Localisation=vm_noeud->Localisation;
	vm_noeud2->Infos_FAX=vm_noeud->Infos_FAX;
	vm_noeud2->Type=vm_noeud->Type;

	vl_pert=NULL;
	pl_cas= vg_pert_imu;

	while (pl_cas!=NULL)
	{
		vl_NumEqtIMU=0;
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_imu_filtre %s",pl_cas->Libelle);
		pl_cond=pl_cas->Condition;
		vl_res=TRUE;

		while (pl_cond!=NULL)
		{
			if (pl_cond->Type==C_TYPE_COMPARATEUR)
			{
				vl_fct = pl_cond->Fonction.Comp.Fct;
				vl_res = vl_res && (*vl_fct) (*(pl_cond->Fonction.Comp.Slot), pl_cond->Fonction.Comp.Valeur);
				if (!vl_res)
					break;
			}
			else
			{
				vl_fct=pl_cond->Fonction.Predicat.Fct;
				vl_res = vl_res && (*vl_fct)();
				if (!vl_res) 
					break;
			}
			pl_cond = pl_cond->Suivant;
		}

		/* on a trouve une perturbation*/
		if (vl_res) 
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"drol_imu_filtre vm_noeud : Autoroute:%s Sens:%d PR :%d",va_noeud->Localisation.NomAutoroute,va_noeud->Localisation.Sens, va_noeud->Localisation.PR);
			XZST_03EcritureTrace(XZSTC_WARNING,"drol_imu_filtre vm_noeud : Autoroute:%s Sens:%d PRd :%d PRf:%d",pl_cas->NomAutoroute,pl_cas->Sens,pl_cas->PR_Debut,pl_cas->PR_Fin);
			/* Recherche numero equipement */
			if (!(strcmp(pl_cas->NomAutoroute,va_noeud->Localisation.NomAutoroute)) && 
				(pl_cas->Sens == va_noeud->Localisation.Sens) && //Si l'autoroute le sens correspondent
				(pl_cas->PR_Debut <= va_noeud->Localisation.PR && pl_cas->PR_Fin >= va_noeud->Localisation.PR) ) // Si le PR est compris dedans
					drol_imu_recherche(pl_cas, &vl_NumEqtIMU); //On recherche le num IMU correspondant
			
			XZST_03EcritureTrace(XZSTC_WARNING,"eqt =%d.",vl_NumEqtIMU);

			if(vl_NumEqtIMU>0) //Ajouter IMU dans SAD IMU
			{
				sprintf (vl_chaineIMU,"%s|%s", pl_cas->Message, pl_cas->ListeZone);
				XZST_03EcritureTrace (XZSTC_WARNING, "ChaineIMU:%s", vl_chaineIMU);
				i=0;
				for(i=0 ; vl_chaineIMU[i] != '\0' ; i++)
					if(vl_chaineIMU[i]==';')
						vl_chaineIMU[i]='|'; //Remplace les ; de la liste zone par des |
				XZST_03EcritureTrace(XZSTC_WARNING,"ChaineIMU:%s",vl_chaineIMU);

				XZAP36_ajout_Prop_IMU(vg_numero_plan,
							1,
							va_site,
							vl_NumEqtIMU,
							0,
							"",
							"EVENEMENT DANS TUNNEL",
							TRUE,
							vl_chaineIMU,
			      				pl_cas->Intervalle,
							pl_cas->Cycles,
			      				pl_cas->Duree,
							va_site,
							pl_cas->Libelle);
				
				XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : drol_imu_filtre proposition ajoutée");
			  	return (TRUE);
			}
		}
		pl_cas=pl_cas->Suivant;
	}
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : drol_imu_filtre");
	return (TRUE);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des IMU
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_imu_recherche(T_PERT_IMU *va_pert,
			int *va_NumEqtIMU)

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

  XZST_03EcritureTrace(   XZSTC_FONCTION,"IN : drol_imu_recherche NomBICE=%s",va_pert->NomBICE);
				
  XZAO505_Recherche_IMU(va_pert->NomBICE,
			&vl_numero);
  *va_NumEqtIMU=vl_numero;
  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : drol_imu_recherche retourne %d",*va_NumEqtIMU);
  return (XDC_OK);
}
