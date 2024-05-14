/*E*/
/*Fichier : $Id: drol_pic.c
/*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : drol_pic.c				*
*************************************************
* DESCRIPTION :
* new tdpac
*************************************************
* HISTORIQUE :
*
* GGY	01/12/23 : Creation (DEM483)
* GGY	18/01/24 : Ajout C_COND_CIRCULATION_BASC_TUNNEL et C_COND_CIRCULATION_NON_BASC_TUNNEL (DEM-483)
************************************************/

#include "drol_pic.h"

#include "xzac01.h"
#include "xzac803.h"
#include "xzae21.h"
#include "xzap821.h"
#include "xzac8021.h"
#include "xzao574.h"

#define C_REP_MIG		"/produits/migrazur/appliSD/fichiers/bdc/"
#define C_PERT			C_REP_MIG "pert_pic.txt"

#define C_SEPARATEUR	"#"
#define C_PIPE			"|"
#define C_ET			"$ET$"
#define C_CAR_DOLLAR	'$'
#define C_PIC_EPSILON	1000
#define C_PIC_DMAX_FIN	1000
#define C_NB_CAR_LIGNE	15
#define C_INFINI		1000000

#define C_INFERIEUR		"<"
#define C_SUPERIEUR		">"
#define C_EGAL			"="

#define C_VITESSE_NULLE	"0"

#define C_COND_UNE_VOIE_BLOQ				"au moins une voie bloquee"
#define C_COND_UNE_VOIE_OUVERTE				"au moins une voie ouverte"
#define C_COND_TOUTES_VOIES_BLOQ			"toutes voies bloquees"
#define C_COND_UNE_VOIE_NEUT				"au moins une voie neutralisee"
#define C_COND_TOUTES_VOIES_NEUT			"toutes voies neutralisees"
#define C_COND_CIRCULATION_UNE_VOIE			"circulation sur une voie"
#define C_COND_CIRCULATION_BASC				"circulation basculee"
#define C_COND_SUR_BAU						"sur bau"
#define C_COND_SUR_SECTION_COURANTE			"sur section courante"
#define C_COND_SUR_LIT_ARRET				"sur lit d'arret"
#define C_COND_SUR_ECHANGEUR				"sur echangeur"
#define C_COND_SUR_BRETELLE_ENTREE			"sur bretelle d'entree"
#define C_COND_SUR_BRETELLE_SORTIE			"sur bretelle de sortie"
#define C_COND_SIGNALE						"signale"
#define C_COND_CONFIRME						"confirme"
#define C_COND_TRAFIC_DENSE					"trafic dense"
#define C_COND_TRAFIC_RALENTI				"trafic ralenti"
#define C_COND_TRAFIC_BOUCHE				"trafic bouche"
#define C_COND_TRAFIC_BLOQUE				"trafic bloque"
#define C_COND_VOIES_RETRECIES				"voies retrecies"
#define C_COND_ENTREE_FERMEE				"entree fermee"
#define C_COND_SORTIE_FERMEE				"sortie fermee"
#define C_COND_DATEX						"datex"
#define C_COND_FEU_MAITRISE					"feu_maitrise"
#define C_COND_FEU_NON_MAITRISE				"feu_non_maitrise"
#define C_COND_STOCKAGE_DEMARRE				"stockage_demarre"
#define C_COND_STOCKAGE_NON_DEMARRE			"stockage_non_demarre"
#define C_COND_CIRCULATION_BASC_TUNNEL		"circulation_basculee"
#define C_COND_CIRCULATION_NON_BASC_TUNNEL	"circulation_non_basculee"

/*variables utilisees dans les messages PIC*/
//#define C_VAR_VITESSE		"VITESSE$"
T_PERT_PIC 		*vg_pert_pic;
T_NOEUD			*vm_noeud;
static T_NOEUD	*vm_noeud2;
T_NOEUD 		*vg_racine;
T_PIC			vm_pic;
int				vm_ligne_courante;
T_PERT_PIC		*pm_pert;

int drol_pic_charge ();
T_CONDITION *drol_pic_decode_condition (char *);
T_LIGNE_PIC	drol_pic_remplit_ligne (char *);
void drol_pic_message (T_PIC, T_PERT_PIC *, XDY_Horodate, XDY_District, XDY_Sens);

static XDY_Booleen drol_pic_egal (int, int);
/*
static XDY_Booleen drol_pic_true(int, int);
static XDY_Booleen drol_pic_inferieur();
static XDY_Booleen drol_pic_superieur();
*/

/*fonctions utilisees dans les conditions*/
static XDY_Booleen drol_pic_une_voie_bloquee ();
static XDY_Booleen drol_pic_une_voie_ouverte ();
static XDY_Booleen drol_pic_toutes_voies_bloquees ();
static XDY_Booleen drol_pic_une_voie_neutralisee ();
static XDY_Booleen drol_pic_toutes_voies_neutralisees ();
static XDY_Booleen drol_pic_circulation_une_voie ();
static XDY_Booleen drol_pic_circulation_basculee ();
static XDY_Booleen drol_pic_sur_bau ();
static XDY_Booleen drol_pic_sur_section_courante ();
static XDY_Booleen drol_pic_sur_lit_arret ();
static XDY_Booleen drol_pic_sur_echangeur ();
static XDY_Booleen drol_pic_sur_bretelle_entree ();
static XDY_Booleen drol_pic_sur_bretelle_sortie ();
static XDY_Booleen drol_pic_signale ();
static XDY_Booleen drol_pic_confirme ();
static XDY_Booleen drol_pic_trafic_dense ();
static XDY_Booleen drol_pic_trafic_ralenti ();
static XDY_Booleen drol_pic_trafic_bouche ();
static XDY_Booleen drol_pic_trafic_bloque ();
static XDY_Booleen drol_pic_voies_retrecies ();
static XDY_Booleen drol_pic_entree_fermee ();
static XDY_Booleen drol_pic_sortie_fermee ();
static XDY_Booleen drol_pic_datex ();
static XDY_Booleen drol_pic_feu_maitrise ();
static XDY_Booleen drol_pic_feu_non_maitrise ();
static XDY_Booleen drol_pic_stockage_demarre ();
static XDY_Booleen drol_pic_stockage_non_demarre ();
static XDY_Booleen drol_pic_circulation_basculee_tunnel ();
static XDY_Booleen drol_pic_circulation_non_basculee_tunnel ();

static int drol_pic_voies (int, int, int, int, int);
static int drol_pic_recherche (T_PERT_PIC *, XDY_Horodate, XDY_District );

/*fonctions utilisees dans les variables des messages*/
static char *drol_pic_var_picto ();
static char *drol_pic_majuscules (char *);

/* definition des variables locales au module */
static char *	version = "drol_pic.c : commentaire" ;

/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int drol_pic_init ()
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
	XZST_03EcritureTrace (XZSTC_WARNING, "IN  : drol_pic_init");
	vm_noeud2 = (T_NOEUD *) malloc (sizeof(T_NOEUD));
	vg_pert_pic = NULL;
	drol_pic_charge ();
	XZST_03EcritureTrace (XZSTC_FONCTION, "OUT drol_pic_init");
	return (XDC_OK);
}

/*------------------------------------------------------
* SERVICE RENDU :
* init des perturbations
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int drol_pic_charge ()
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
	FILE  		*pl_fp;
	int			vl_retcode = 0;
	int			vl_i;
	char		vl_Lig [500];
	XDY_Entier	vl_Resultat_out = 0;
	char		*vl_str;
	char 		*vl_ch;
	T_PERT_PIC	*pl_liste;
	int			vl_num_prec, vl_num;
	char		vl_l1 [100], vl_l2[100], vl_l3[100], vl_l1f[50], vl_l2f[50], vl_l3f[50];
	char		vl_b1 [50], vl_b2[50], vl_b3[50], vl_b1f[50], vl_b2f[50], vl_b3f[50];
	char		vl_s1 [50], vl_s2[50], vl_s3[50], vl_s4[50], vl_s5[50], vl_s6[50];
	char		vl_bandeau [100];
	char		vl_pic1 [150];
	char		vl_pic2 [150];
	char		vl_condition [300];
	char		vl_bandeau_bifurcation [100];
	int 		vl_auto;
	XDY_Booleen vl_vide;

	XZST_03EcritureTrace (XZSTC_DEBUG1, "IN  : drol_pic_charge_cas");

	if ((pl_fp = fopen(C_PERT, "r")) == NULL )
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "erreur de lecture du fichier %s !", C_PERT);
		return (XDC_NOK);
	}
	
	/* memorisation de la structure du message */
	while (fgets (vl_Lig, sizeof (vl_Lig), pl_fp) != NULL)
	{
	    pl_liste = (T_PERT_PIC *) malloc (sizeof (T_PERT_PIC));
	    vl_str=vl_Lig; 

	    sscanf (vl_str,"%[^|]|%d|", pl_liste->Libelle, &(pl_liste->Priorite));

	    XZST_03EcritureTrace (XZSTC_WARNING, "drol_pic_charge : libelle=%s, priorite=%d", pl_liste->Libelle, pl_liste->Priorite);

	    sprintf (vl_l1,"%d", pl_liste->Priorite);
	    vl_str = vl_str + strlen (pl_liste->Libelle) + 2 + strlen (vl_l1);
	    
	    sscanf (vl_str,"%[^|]|", vl_condition);
	    vl_str = vl_str + strlen (vl_condition) + 1;
	    pl_liste->Condition = drol_pic_decode_condition (vl_condition);

	    XZST_03EcritureTrace (XZSTC_WARNING, "COND %s vl_str=%s ", vl_condition, vl_str);
	    /*mode automatique*/

	    sscanf (vl_str, "%d|", &(pl_liste->Automatique));
	    vl_str = vl_str + 2;

	    /* Bandeau */
	    XZST_03EcritureTrace (XZSTC_WARNING, "Bandeau str %s", vl_str);
	    if (vl_str[0] == '|')
		{
			strcpy (pl_liste->Bandeau, "");
			vl_str = vl_str + 1;
		}
		else
		{	
			sscanf (vl_str, "%[^|]|", pl_liste->Bandeau);
	    	vl_str = vl_str + 5;
		}
		XZST_03EcritureTrace (XZSTC_WARNING, "Bandeau %s", pl_liste->Bandeau);

	    /* Flash */
	    XZST_03EcritureTrace (XZSTC_WARNING, "Flash str %s", vl_str);
	    if (vl_str[0] == '|')
		{
			strcpy (pl_liste->Flash, "");
			vl_str = vl_str + 1;
		}
		else
	    {
			sscanf(vl_str, "%[^|]|", pl_liste->Flash);
			vl_str = vl_str + 2;
		}
	    XZST_03EcritureTrace (XZSTC_WARNING, "Flash %s", pl_liste->Flash);

	    /* Picto */
	    XZST_03EcritureTrace (XZSTC_WARNING, "Picto str %s", vl_str);
	    if (vl_str[0]=='|')
			strcpy (pl_liste->Picto, "");
	    else
	      sscanf (vl_str, "%[^|]|", pl_liste->Picto);
	    XZST_03EcritureTrace (XZSTC_WARNING, "Picto %s", pl_liste->Picto);

	    pl_liste->Suivant = vg_pert_pic;
	    vg_pert_pic = pl_liste;
	}
	fclose(pl_fp);

	XZST_03EcritureTrace (XZSTC_DEBUG1, "OUT  : drol_pic_charge_cas");
	return (XDC_OK);
}

/*------------------------------------------------------
* SERVICE RENDU :
* decodage de la condition
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
T_CONDITION *drol_pic_decode_condition (char *va_cond)
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
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_pic_decode_condition:  IN %s",va_cond);

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
	pl_cond->Fonction.Comp.Fct=drol_pic_egal;
	pl_cond->Suivant=NULL;
	pl_tmp=pl_cond;

	/*numero en fonction du type*/
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_pic_decode_condition: type = %s",pl_cond->Libelle);
	XZST_03EcritureTrace(   XZSTC_FONCTION, "drol_pic_decode_condition: type = %s %d",pl_cond->Libelle,pl_cond->Fonction.Comp.Valeur);

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
	else if (!strcmp(vl_ch,"REGULATION")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Regulation;
	/* Cas PIC */
	else if (!strcmp(vl_ch,"DANGER")) {
	  pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Accident;
	  pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
	  pl_cond->Suivant->Suivant=NULL;
	  pl_tmp=pl_cond->Suivant;
	  pl_tmp->Type=C_TYPE_COMPARATEUR;
	  pl_tmp->Fonction.Comp.Fct=drol_pic_egal;
	  pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Incident);
	  pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Autre;
	}
	else if (!strcmp(vl_ch,"RETRECISSEMENT DROITE")) {
	  pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Accident;
	  pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
	  pl_cond->Suivant->Suivant=NULL;
	  pl_tmp=pl_cond->Suivant;
	  pl_tmp->Type=C_TYPE_COMPARATEUR;
	  pl_tmp->Fonction.Comp.Fct=drol_pic_egal;
	  pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Incident);
	  pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Autre;
	}
	else if (!strcmp(vl_ch,"RETRECISSEMENT GAUCHE")) {
	  pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Accident;
	  pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
	  pl_cond->Suivant->Suivant=NULL;
	  pl_tmp=pl_cond->Suivant;
	  pl_tmp->Type=C_TYPE_COMPARATEUR;
	  pl_tmp->Fonction.Comp.Fct=drol_pic_egal;
	  pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Incident);
	  pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Autre;
	}
	/*traiter le cas particulier de la meteo*/
	else if (!strcmp(vl_ch,"VERGLAS")) {
	  pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
	  pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
	  pl_cond->Suivant->Suivant=NULL;
	  pl_tmp=pl_cond->Suivant;
	  pl_tmp->Type=C_TYPE_COMPARATEUR;
	  pl_tmp->Fonction.Comp.Fct=drol_pic_egal;
	  pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
	  pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Verglas;
	}
	else if (!strcmp(vl_ch,"CHUTE NEIGE")) {
	  pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
	  pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
	  pl_cond->Suivant->Suivant=NULL;
	  pl_tmp=pl_cond->Suivant;
	  pl_tmp->Type=C_TYPE_COMPARATEUR;
	  pl_tmp->Fonction.Comp.Fct=drol_pic_egal;
	  pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
	  pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_ChuteNeige;
	}
	else if (!strcmp(vl_ch,"PLUIE")) {
	  pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
	  pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
	  pl_cond->Suivant->Suivant=NULL;
	  pl_tmp=pl_cond->Suivant;
	  pl_tmp->Type=C_TYPE_COMPARATEUR;
	  pl_tmp->Fonction.Comp.Fct=drol_pic_egal;
	  pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
	  pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Pluie;
	}
	else if (!strcmp(vl_ch,"BROUILLARD GIVRANT")) {
	  pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
	  pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
	  pl_cond->Suivant->Suivant=NULL;
	  pl_tmp=pl_cond->Suivant;
	  pl_tmp->Type=C_TYPE_COMPARATEUR;
	  pl_tmp->Fonction.Comp.Fct=drol_pic_egal;
	  pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
	  pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_BrouillardGiv;
	}
	else if (!strcmp(vl_ch,"NEIGE SUR CHAUSSEE")) {
	  pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
	  pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
	  pl_cond->Suivant->Suivant=NULL;
	  pl_tmp=pl_cond->Suivant;
	  pl_tmp->Type=C_TYPE_COMPARATEUR;
	  pl_tmp->Fonction.Comp.Fct=drol_pic_egal;
	  pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
	  pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_NeigeChaus;
	}
	else if (!strcmp(vl_ch,"GRELE SUR CHAUSSEE")) {
	  pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
	  pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
	  pl_cond->Suivant->Suivant=NULL;
	  pl_tmp=pl_cond->Suivant;
	  pl_tmp->Type=C_TYPE_COMPARATEUR;
	  pl_tmp->Fonction.Comp.Fct=drol_pic_egal;
	  pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
	  pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_GreleChaus;
	}
	else if (!strcmp(vl_ch,"BROUILLARD")) {
	  pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
	  pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
	  pl_cond->Suivant->Suivant=NULL;
	  pl_tmp=pl_cond->Suivant;
	  pl_tmp->Type=C_TYPE_COMPARATEUR;
	  pl_tmp->Fonction.Comp.Fct=drol_pic_egal;
	  pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
	  pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Brouillard;
	}
	else if (!strcmp(vl_ch,"VENT")) {
	  pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
	  pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
	  pl_cond->Suivant->Suivant=NULL;
	  pl_tmp=pl_cond->Suivant;
	  pl_tmp->Type=C_TYPE_COMPARATEUR;
	  pl_tmp->Fonction.Comp.Fct=drol_pic_egal;
	  pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
	  pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_VentFort;
	}
	else if (!strcmp(vl_ch,"GRELE")) {
	  pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
	  pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
	  pl_cond->Suivant->Suivant=NULL;
	  pl_tmp=pl_cond->Suivant;
	  pl_tmp->Type=C_TYPE_COMPARATEUR;
	  pl_tmp->Fonction.Comp.Fct=drol_pic_egal;
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
	  pl_tmp->Fonction.Comp.Fct=drol_pic_egal;
	  pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Incident);
	  pl_tmp->Fonction.Comp.Valeur=1;
	}
	else if (!strcmp(vl_ch,"PENURIE")) {
	  pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_IncSsConces;
	  pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
	  pl_cond->Suivant->Suivant=NULL;
	  pl_tmp=pl_cond->Suivant;
	  pl_tmp->Type=C_TYPE_COMPARATEUR;
	  pl_tmp->Fonction.Comp.Fct=drol_pic_egal;
	  pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Incident);
	  pl_tmp->Fonction.Comp.Valeur=0;
	}
	/*cas particulier du lit d'arret*/
	else if (!strcmp(vl_ch,"sur lit d'arret")) {
	  pl_cond->Fonction.Comp.Slot=&(vm_noeud2->Localisation.TypePointCar);
	  pl_cond->Fonction.Comp.Valeur=XDC_POINT_CARACT_LIT_ARRET;
	}
	else {
	  XZST_03EcritureTrace(   XZSTC_WARNING,"drol_pic_decode_condition : pb decodage type !!!");
	  pl_cond->Fonction.Comp.Valeur=0;
	}
	
	/*si pas de condition supplementaire*/
	if (vl_indice==strlen(va_cond)) {
	  XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : drol_pic_decode_condition : que le type");
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
	
	  XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_pic_decode_condition:  cond %s",vl_str);
	  strcpy(pl_tmp->Libelle,vl_str);
	
	  /*il s'agit d'une fonction PREDICAT*/
	  pl_tmp->Type=C_TYPE_PREDICAT;
	  if (!strcmp(vl_str,C_COND_UNE_VOIE_BLOQ)) pl_tmp->Fonction.Predicat.Fct=drol_pic_une_voie_bloquee;
	  else if (!strcmp(vl_str,C_COND_UNE_VOIE_OUVERTE)) pl_tmp->Fonction.Predicat.Fct=drol_pic_une_voie_ouverte;
	  else if (!strcmp(vl_str,C_COND_TOUTES_VOIES_BLOQ)) pl_tmp->Fonction.Predicat.Fct=drol_pic_toutes_voies_bloquees;
	  else if (!strcmp(vl_str,C_COND_TOUTES_VOIES_NEUT)) pl_tmp->Fonction.Predicat.Fct=drol_pic_toutes_voies_neutralisees;
	  else if (!strcmp(vl_str,C_COND_UNE_VOIE_NEUT)) pl_tmp->Fonction.Predicat.Fct=drol_pic_une_voie_neutralisee;
	  else if (!strcmp(vl_str,C_COND_CIRCULATION_UNE_VOIE)) pl_tmp->Fonction.Predicat.Fct=drol_pic_circulation_une_voie;
	  else if (!strcmp(vl_str,C_COND_CIRCULATION_BASC)) pl_tmp->Fonction.Predicat.Fct=drol_pic_circulation_basculee;
	  else if (!strcmp(vl_str,C_COND_CONFIRME)) pl_tmp->Fonction.Predicat.Fct=drol_pic_confirme;
	  else if (!strcmp(vl_str,C_COND_SIGNALE)) pl_tmp->Fonction.Predicat.Fct=drol_pic_signale;
	  else if (!strcmp(vl_str,C_COND_SUR_BAU)) pl_tmp->Fonction.Predicat.Fct=drol_pic_sur_bau;
	  else if (!strcmp(vl_str,C_COND_SUR_SECTION_COURANTE)) pl_tmp->Fonction.Predicat.Fct=drol_pic_sur_section_courante;
	  else if (!strcmp(vl_str,C_COND_SUR_LIT_ARRET)) pl_tmp->Fonction.Predicat.Fct=drol_pic_sur_lit_arret;
	  else if (!strcmp(vl_str,C_COND_SUR_ECHANGEUR)) pl_tmp->Fonction.Predicat.Fct=drol_pic_sur_echangeur;
	  else if (!strcmp(vl_str,C_COND_SUR_BRETELLE_ENTREE)) pl_tmp->Fonction.Predicat.Fct=drol_pic_sur_bretelle_entree;
	  else if (!strcmp(vl_str,C_COND_SUR_BRETELLE_SORTIE)) pl_tmp->Fonction.Predicat.Fct=drol_pic_sur_bretelle_sortie;
	  else if (!strcmp(vl_str,C_COND_TRAFIC_DENSE)) pl_tmp->Fonction.Predicat.Fct=drol_pic_trafic_dense;
	  else if (!strcmp(vl_str,C_COND_TRAFIC_RALENTI)) pl_tmp->Fonction.Predicat.Fct=drol_pic_trafic_ralenti;
	  else if (!strcmp(vl_str,C_COND_TRAFIC_BOUCHE)) pl_tmp->Fonction.Predicat.Fct=drol_pic_trafic_bouche;
	  else if (!strcmp(vl_str,C_COND_TRAFIC_BLOQUE)) pl_tmp->Fonction.Predicat.Fct=drol_pic_trafic_bloque;
	  else if (!strcmp(vl_str,C_COND_DATEX)) pl_tmp->Fonction.Predicat.Fct=drol_pic_datex;
	  else if (!strcmp(vl_str,C_COND_VOIES_RETRECIES)) pl_tmp->Fonction.Predicat.Fct=drol_pic_voies_retrecies;
	  else if (!strcmp(vl_str,C_COND_ENTREE_FERMEE)) pl_tmp->Fonction.Predicat.Fct=drol_pic_entree_fermee;
	  else if (!strcmp(vl_str,C_COND_SORTIE_FERMEE)) pl_tmp->Fonction.Predicat.Fct=drol_pic_sortie_fermee;
	  else if (!strcmp(vl_str,C_COND_FEU_MAITRISE)) pl_tmp->Fonction.Predicat.Fct=drol_pic_feu_maitrise;
	  else if (!strcmp(vl_str,C_COND_FEU_NON_MAITRISE)) pl_tmp->Fonction.Predicat.Fct=drol_pic_feu_non_maitrise;
	  else if (!strcmp(vl_str,C_COND_STOCKAGE_DEMARRE)) pl_tmp->Fonction.Predicat.Fct=drol_pic_stockage_demarre;
	  else if (!strcmp(vl_str,C_COND_STOCKAGE_NON_DEMARRE)) pl_tmp->Fonction.Predicat.Fct=drol_pic_stockage_non_demarre;
	  else if (!strcmp(vl_str,C_COND_CIRCULATION_BASC_TUNNEL)) pl_tmp->Fonction.Predicat.Fct = drol_pic_circulation_basculee_tunnel;
	  else if (!strcmp(vl_str,C_COND_CIRCULATION_NON_BASC_TUNNEL)) pl_tmp->Fonction.Predicat.Fct = drol_pic_circulation_non_basculee_tunnel;
	
	/*si fin de chaine*/
	if (vl_indice==strlen(va_cond)) {
	  XZST_03EcritureTrace(   XZSTC_DEBUG1,"drol_pic_decode_condition : fin de chaine");
	  return (pl_cond);
	}
	else {
	  va_cond=va_cond+strlen(vl_str)+4;
	  goto pos_decode;
	}
}


/**************************************/
/**************************************/
static XDY_Booleen drol_pic_sur_echangeur() {
  return (vm_noeud->Localisation.TypePointCar==XDC_POINT_CARACT_ECHANGEUR);
}
static XDY_Booleen drol_pic_sur_lit_arret() {
  return (vm_noeud->Localisation.TypePointCar==XDC_POINT_CARACT_LIT_ARRET);
}

static XDY_Booleen drol_pic_egal(int va_valeur1, int va_valeur2) { return (va_valeur1==va_valeur2); }
/*
static XDY_Booleen drol_pic_true(int va_valeur1, int va_valeur2) { return (TRUE); }
static XDY_Booleen drol_pic_superieur(int va_valeur1, int va_valeur2) { return (va_valeur1>va_valeur2); }
static XDY_Booleen drol_pic_inferieur(int va_valeur1, int va_valeur2) { return (va_valeur1<va_valeur2); }
*/

static XDY_Booleen drol_pic_confirme() { return (vm_noeud->Infos_TFM.Confirme); }
static XDY_Booleen drol_pic_signale() { return (vm_noeud->Infos_TFM.Signale); }

static XDY_Booleen drol_pic_une_voie_bloquee() {
  return ((vm_noeud->Localisation.VL==XDC_VOIE_BLOQUEE) ||
          (vm_noeud->Localisation.VM1==XDC_VOIE_BLOQUEE) ||
	  (vm_noeud->Localisation.VM2==XDC_VOIE_BLOQUEE) ||
	  (vm_noeud->Localisation.VR==XDC_VOIE_BLOQUEE)
	 );
}
static XDY_Booleen drol_pic_une_voie_ouverte() {
  return ((vm_noeud->Localisation.VL==XDC_VOIE_SENS_NORMAL) ||
	  (vm_noeud->Localisation.VM1==XDC_VOIE_SENS_NORMAL) ||
	  (vm_noeud->Localisation.VM2==XDC_VOIE_SENS_NORMAL) ||
	  (vm_noeud->Localisation.VR==XDC_VOIE_SENS_NORMAL)
	 );
}
static XDY_Booleen drol_pic_toutes_voies_bloquees() {
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
/* REPERE3 */
static XDY_Booleen drol_pic_une_voie_neutralisee() { 
  return ((vm_noeud->Localisation.VL==XDC_VOIE_NEUTRALISEE) ||
	  (vm_noeud->Localisation.VM1==XDC_VOIE_NEUTRALISEE) ||
	  (vm_noeud->Localisation.VM2==XDC_VOIE_NEUTRALISEE) ||
	  (vm_noeud->Localisation.VR==XDC_VOIE_NEUTRALISEE) ||
          (vm_noeud->Localisation.VL==XDC_VOIE_BLOQUEE_NEUTRALISEE) ||
	  (vm_noeud->Localisation.VM1==XDC_VOIE_BLOQUEE_NEUTRALISEE) ||
	  (vm_noeud->Localisation.VM2==XDC_VOIE_BLOQUEE_NEUTRALISEE) ||
	  (vm_noeud->Localisation.VR==XDC_VOIE_BLOQUEE_NEUTRALISEE)
         );
}
       
static XDY_Booleen drol_pic_toutes_voies_neutralisees() { 
  return ( (vm_noeud->Localisation.BAU!=XDC_VOIE_SENS_NORMAL) &&
	   (vm_noeud->Localisation.VR_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VM2_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VM1_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VL_I!=XDC_VOIE_SENS_INVERSE) &&
	   (drol_pic_voies(vm_noeud->Localisation.VL,
			  vm_noeud->Localisation.VM1,
			  vm_noeud->Localisation.VM2,
			  vm_noeud->Localisation.VR,
			  XDC_VOIE_NEUTRALISEE)==15)
         );
}

static XDY_Booleen drol_pic_circulation_une_voie() {
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


static XDY_Booleen drol_pic_circulation_basculee() {
  return ( ( (vm_noeud->Localisation.VL==XDC_VOIE_SENS_INVERSE) ||
	     (vm_noeud->Localisation.VM1==XDC_VOIE_SENS_INVERSE) ||
	     (vm_noeud->Localisation.VM2==XDC_VOIE_SENS_INVERSE) ||
	     (vm_noeud->Localisation.VR==XDC_VOIE_SENS_INVERSE) ||
	     (vm_noeud->Localisation.VL_I==XDC_VOIE_SENS_INVERSE) ||
	     (vm_noeud->Localisation.VM1_I==XDC_VOIE_SENS_INVERSE) ||
	     (vm_noeud->Localisation.VM2_I==XDC_VOIE_SENS_INVERSE) ||
	     (vm_noeud->Localisation.VR_I==XDC_VOIE_SENS_INVERSE)  )
	   
         );
}

static XDY_Booleen drol_pic_sur_bau() { 
  return ( (drol_pic_voies(vm_noeud->Localisation.VL,vm_noeud->Localisation.VM1,
			   vm_noeud->Localisation.VM2,
			   vm_noeud->Localisation.VR,
			   XDC_VOIE_SENS_NORMAL)==15) &&
           ((vm_noeud->Localisation.BAU==XDC_VOIE_BLOQUEE) ||
	    (vm_noeud->Localisation.BAU==XDC_VOIE_NEUTRALISEE)
           )
         );
}

static XDY_Booleen drol_pic_sur_section_courante() { 
  return ( (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_AIRE) &&
	   (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_PEAGE) &&
	   (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_ECHANGEUR) &&
	   (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_LIT_ARRET) &&
	   (drol_pic_voies(vm_noeud->Localisation.VL,vm_noeud->Localisation.VM1,
	                       vm_noeud->Localisation.VM2,
			       vm_noeud->Localisation.VR,
			       XDC_VOIE_SENS_NORMAL)!=15)
         );
}
static XDY_Booleen drol_pic_sur_bretelle_entree() { 
  return (  (  (vm_noeud->Localisation.Sens==XDC_SENS_ENTREE_SUD) ||
	       (vm_noeud->Localisation.Sens==XDC_SENS_ENTREE_NORD)
	    )
	    &&
	    (vm_noeud->Localisation.Bifurcation != XDC_VRAI)
         );
}

static XDY_Booleen drol_pic_sur_bretelle_sortie() { 
  return (  (      (vm_noeud->Localisation.Sens==XDC_SENS_SORTIE_SUD) ||
	           (vm_noeud->Localisation.Sens==XDC_SENS_SORTIE_NORD)
	    )
	    &&
            (vm_noeud->Localisation.Bifurcation != XDC_VRAI)
         );
}

static XDY_Booleen drol_pic_trafic_dense() { 
  return (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Dense); }

static XDY_Booleen drol_pic_trafic_ralenti() { /*DEM428*/
  return ((vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Ralenti) || (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Accordeon)); }

static XDY_Booleen drol_pic_trafic_bouche() { 
  return (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Bouche); }

static XDY_Booleen drol_pic_trafic_bloque() { 
  return (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Bloque); }

static XDY_Booleen drol_pic_datex() { return (vm_noeud->Infos_TFM.Datex==XDC_VRAI); } /*SAGA*/

static XDY_Booleen drol_pic_voies_retrecies() { return (vm_noeud->Localisation.Voies_Retrecies); }

static XDY_Booleen drol_pic_entree_fermee() { 
  return ( (vm_noeud->Infos_FAX.Entree1) || (vm_noeud->Infos_FAX.Entree2) );
}

static XDY_Booleen drol_pic_sortie_fermee() { 
  return ( (vm_noeud->Infos_FAX.Sortie1) || (vm_noeud->Infos_FAX.Sortie2) );
}

static XDY_Booleen drol_pic_feu_maitrise() { 
  return (vm_noeud->Infos_FAX.Feu_Maitrise != XDC_FAUX);
}

static XDY_Booleen drol_pic_feu_non_maitrise() { 
  return (vm_noeud->Infos_FAX.Feu_Maitrise == XDC_FAUX);
}

static XDY_Booleen drol_pic_stockage_demarre() { 
  return (vm_noeud->Localisation.Voies_Retrecies);
}

static XDY_Booleen drol_pic_stockage_non_demarre() { 
  if (vm_noeud->Localisation.Voies_Retrecies == XDC_VRAI) {
    return (XDC_FAUX);
  }
  return (XDC_VRAI);
}

static XDY_Booleen drol_pic_circulation_basculee_tunnel ()
{
	XDY_Booleen vl_basculement = XDC_FAUX;
	
	XZST_03EcritureTrace(XZSTC_WARNING,"drol_pic_circulation_basculee_tunnel : avant XZAE861_Basculement_Tunnel");

	if (XZAE861_Basculement_Tunnel (vm_noeud->Localisation.Autoroute,
									vm_noeud->Localisation.PR,
									vm_noeud->Localisation_Tete.PR,
									vl_basculement)	!= XDC_OK) 
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_pic_circulation_basculee_tunnel : pb appel a XZAE861_Basculement_Tunnel");
		return (FALSE);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_pic_circulation_basculee_tunnel : XZAE861_Basculement_Tunnel retourne %s", vl_basculement);
	}
	
	return (vl_basculement);
}

static XDY_Booleen drol_pic_circulation_non_basculee_tunnel ()
{
	XZST_03EcritureTrace (XZSTC_WARNING, "drol_pic_circulation_non_basculee_tunnel : not drol_pic_circulation_basculee_tunnel");
	return (!drol_pic_circulation_basculee_tunnel());
}

/**************************************/
/**************************************/
/*fonctions des variables dans les messages*/

static char *drol_pic_var_picto () 
{
	XDY_Picto_PIC vl_picto;
	static char vl_str[XDC_Lon_Picto_PIC] = "";

	XZST_03EcritureTrace(XZSTC_WARNING,"drol_pic_var_picto pm_pert->Picto %s",pm_pert->Picto);
	strcpy(vl_str,pm_pert->Picto);
	XZST_03EcritureTrace(XZSTC_WARNING,"drol_pic_var_picto %s",vl_str);

	return (vl_str);
}

static char *drol_pic_var_null() 
{
	return ("");
}
/**************************************/
/**************************************/
T_LIGNE_PIC drol_pic_remplit_ligne (char *va_str)
{
	T_LIGNE_PIC vl_ligne;
	int vl_indice;

	XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pic_remplit_ligne IN = %s", va_str);
	vl_indice=0;
	strcpy(vl_ligne.Texte,"");
	while ((va_str[vl_indice] != C_CAR_DOLLAR) && vl_indice<strlen(va_str))
	{
		vl_ligne.Texte[vl_indice]=va_str[vl_indice];
		vl_indice++;
	}
	vl_ligne.Texte[vl_indice]='\0';

	va_str=va_str+vl_indice+1;
	XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pic_remplit_ligne va_str = %s", va_str);
	vl_ligne.Fct = drol_pic_var_picto;

	return (vl_ligne);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des perturbations engendrees par l'evt
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen drol_pic_filtre (T_NOEUD *va_noeud, XDY_District va_site, XDY_Horodate va_horodate)
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
	T_PERT_PIC *pl_cas;
	T_CONDITION *pl_cond;
	XDY_Booleen (*vl_fct)();
	XDY_Booleen vl_res=FALSE;
	XDY_Booleen vl_fax_a_emettre=FALSE;
	T_PERT_PIC *vl_pert;
	int vl_indice;

	XZST_03EcritureTrace (XZSTC_FONCTION,"drol_pic_filtre IN");

	/* Si FMC prevue alors pas d'action PIC !!!!!!!!!!!!!!!!!!!!!!*/
	/* DEM 628*/
	if (vm_noeud->Infos_TFM.Prevu)
		return (TRUE);

	/*remet a jour la liste des PIC*/
	pic_init_vg_requetes();

	vm_noeud=va_noeud;
	vm_noeud2->Localisation=vm_noeud->Localisation;
	vm_noeud2->Infos_FAX=vm_noeud->Infos_FAX;
	vm_noeud2->Type=vm_noeud->Type;

	vl_pert=NULL;
	pl_cas= vg_pert_pic;

	while (pl_cas!=NULL)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "drol_pic_filtre %s",pl_cas->Libelle);
		pl_cond=pl_cas->Condition;
		vl_res=TRUE;

		while (pl_cond != NULL)
		{
			if (pl_cond->Type==C_TYPE_COMPARATEUR)
			{
				vl_fct=pl_cond->Fonction.Comp.Fct;
				vl_res=vl_res && (*vl_fct) (*(pl_cond->Fonction.Comp.Slot), pl_cond->Fonction.Comp.Valeur);
				if (!vl_res)
				{
					break;
				}
			}
			else
			{
				vl_fct=pl_cond->Fonction.Predicat.Fct;
				vl_res = vl_res && (*vl_fct)();
				if (!vl_res)
				{
					break;
				}
			}
			pl_cond = pl_cond->Suivant;
		}
		
		/*on a trouve une perturbation*/
		if (vl_res) 
		{
			XZST_03EcritureTrace (XZSTC_FONCTION,"drol_pic_filtre OK pour %s ", pl_cas->Libelle);
			drol_pic_recherche (pl_cas,va_horodate,va_site);
		}
		
		pl_cas=pl_cas->Suivant;
	}

	/*pour chacun des PIC recuperes*/
	for (vl_indice=0;vl_indice<C_NB_MAX_PIC;vl_indice++)
	{
		/* si le PIC doit etre utilise*/
		if (vg_pic[vl_indice].Utilise) 
		{
			XZST_03EcritureTrace (XZSTC_FONCTION, "drol_pic_filtre: pert pic %d, priorite %d",
													vg_pic[vl_indice].Perturbation->Priorite,
													vg_priorite_retenue);
			vm_pic = vg_pic[vl_indice];
			drol_pic_message (vg_pic[vl_indice], vg_pic[vl_indice].Perturbation, va_horodate, va_site,vm_noeud->Localisation.Sens); 
		}
	}

	XZST_03EcritureTrace (XZSTC_FONCTION,"drol_pic_filtre OUT");
	return (TRUE);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des PIC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_pic_recherche(T_PERT_PIC *va_pert,
			XDY_Horodate va_horodate,
			XDY_District va_site)

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
  XDY_PR vl_pr_min, vl_pr_max;

  XZST_03EcritureTrace(   XZSTC_DEBUG1,"IN : drol_pic_recherche");

  pm_pert = va_pert;

  vl_pr_min = vm_noeud->Localisation.PR <= vm_noeud->Localisation_Tete.PR ? vm_noeud->Localisation.PR : vm_noeud->Localisation_Tete.PR;
  vl_pr_max = vm_noeud->Localisation.PR >= vm_noeud->Localisation_Tete.PR ? vm_noeud->Localisation.PR : vm_noeud->Localisation_Tete.PR;

  /*appel de la fonction de recherche du ou des PIC*/
  if ( (vm_noeud->Type == XZAEC_FMC_Travaux) ||
		  (vm_noeud->Type == XZAEC_FMC_ZoneStockage) ||
		  (vm_noeud->Type == XZAEC_FMC_TraitChaussees)) {
	if ((vm_noeud->Localisation.Sens == XDC_SENS_SUD) && (vm_noeud->Localisation.Autoroute!=8)) /****** traitement A57 (8) SAE - 442 ****/
	  vl_pr_min = vl_pr_min - 400;
	else
	  vl_pr_max = vl_pr_max + 400;
	les_PIC_ds_perturbation (vm_noeud->Localisation.Autoroute,
	     vl_pr_min,vl_pr_max,
	     vm_noeud->Localisation.Sens,
	     va_horodate,
	     va_site,
	     va_pert->Priorite,
	     va_pert->Libelle,
	      va_pert);
  }
  else if ((vm_noeud->Type == XZAEC_FMC_BasculTravaux) ||
		  (vm_noeud->Type == XZAEC_FMC_Basculement) ) {
		les_PIC_du_basculement (vm_noeud->Localisation.Autoroute,
		     vm_noeud->Localisation.PR,
		     vm_noeud->Localisation_Tete.PR,
		     vm_noeud->Localisation.Sens,
		     va_horodate,
		     va_site,
		     va_pert->Priorite,
		     va_pert->Libelle,
		      va_pert);
  }
  else {
    /* on recherche le PIC qui va etre en amont de l'evt sur un kilometre*/
    if ((vm_noeud->Localisation.Sens == XDC_SENS_SUD) && (vm_noeud->Localisation.Autoroute!=8)) /****** traitement A57 (8) SAE - 442 ****/
       vl_pr_min = vl_pr_min - 1000;
    else
       vl_pr_max = vl_pr_max + 1000;
les_PIC_ds_perturbation (vm_noeud->Localisation.Autoroute,
		 vl_pr_min,
		 vl_pr_max,
		   vm_noeud->Localisation.Sens,
		va_horodate,
	     va_site,
	  va_pert->Priorite,
	       va_pert->Libelle,
	     va_pert);

  }
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de calcul du message a afficher sur le PIC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void drol_pic_message (T_PIC va_pic, T_PERT_PIC *va_pert, XDY_Horodate va_horodate, XDY_District va_site, XDY_Sens va_Sens)

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
	XDY_Bandeau_PIC	vl_bandeau;
	XDY_Picto_PIC	vl_picto;
	XDY_Booleen		vl_Flash;
	char			vl_explication[100];
	int				vl_mode_automatique = 0;
	char			vl_dg[50];
	int 			vl_Sens=0;
	XDY_Entier		vl_action=0;

	XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pic_message : IN ");

	/*calcul des elements de message*/
	strcpy(vl_picto,drol_pic_var_picto());

	/* texte sur le bandeau*/
	strcpy(vl_bandeau,va_pert->Bandeau);

	/* Flash */
	vl_Flash = va_pert->Flash;

 	/*supprimer la proposition si identique a usage courant*/
 	/*gerer la mise en file d'attente*/
 	XZST_03EcritureTrace ( XZSTC_FONCTION, "drol_pic_message: courant %s/%d/%s.",
							va_pic.Bandeau,
							va_pic.Flash,
							va_pic.Picto);

	/*si on calcule un plan d'actions au CI*/
	vl_mode_automatique = va_pert->Automatique;
	XZST_03EcritureTrace (XZSTC_INFO,"drol_pic_message : va_pert->Automatique=%d", va_pert->Automatique);
	if (vg_site == XDC_CI) 
	{
		/*on ne fait que proposer, pas de pilotage automatique*/
		/* Ne s'applique pas sur COFIROUTE, uniquement sur ESCOTA */
		/* Sur COFIROUTE, le site principal peut piloter les PMV */
		vl_mode_automatique=0;
	}
	/* si on n'est pas au CI, on ne pilote pas en auto les PIC du voisin */
	else if (vg_site != va_pic.District)
		vl_mode_automatique=0;

	if ((strcmp (va_pic.Bandeau, vl_bandeau)) || (va_pic.Flash == vl_Flash) || (strcmp (va_pic.Picto,vl_picto)))
	{
		sprintf (vl_explication,"(%s)(%s)(%d)", vl_picto, vl_bandeau,vl_Flash);

		XZST_03EcritureTrace (XZSTC_INFO, "drol_pic_message : vl_mode_automatique=%d", vl_mode_automatique);

		/* si ce n'est pas une maj du pic pour le meme evt */
		if ((vm_noeud->NumEvt.numero != va_pic.Numero_Evt_U) || (vm_noeud->NumEvt.cle != va_pic.Cle_Evt_U))
		{
			/* DEM655 si meme priorite , l'evt le plus proche est prioritaire */
			if ((va_pic.Priorite_C == va_pic.Priorite_U) && (va_pic.Distance_C < va_pic.Distance_U))
			{
				va_pic.Priorite_U++;
				XZST_03EcritureTrace(XZSTC_INFO,"drol_pic_message :si meme priorite , l'evt le plus proche est prioritaire :Distance_C=%d;Diastance_U=%d =>va_pic.Priorite_C=%d;va_pic.Priorite_U=%d  ",va_pic.Distance_C,va_pic.Distance_U,va_pic.Priorite_C,va_pic.Priorite_U);
			}

			/* DEM655 si  priorite superieure mais plus distant, alors on ne pilote pas en auto */
			if ((va_pic.Priorite_C < va_pic.Priorite_U) && (va_pic.Distance_C > va_pic.Distance_U))
			{
				vl_mode_automatique = 0;
				XZST_03EcritureTrace (XZSTC_INFO, "drol_pic_message :si priorite superieure mais plus distant, alors on ne pilote pas en autoDistance_C=%d;Diastance_U=%d =>va_pic.Priorite_C=%d;va_pic.Priorite_U=%d  ",va_pic.Distance_C,va_pic.Distance_U,va_pic.Priorite_C,va_pic.Priorite_U);
			}
		}

		/* si le mode est non auto ou
		si la priorite du pilotage en cours sur un autre evt est superieure
		alors on passe en proposition PA */
		if ((vl_mode_automatique == 0)  || ((va_pic.Priorite_C >= va_pic.Priorite_U) && !((vm_noeud->NumEvt.numero==va_pic.Numero_Evt_U) && (vm_noeud->NumEvt.cle==va_pic.Cle_Evt_U) && (va_pic.Priorite_U!=0))))
		{
			/*mise en base de la proposition PIC*/
			/*si pas automatique*/
			if (XZAP821_Ajout_Prop_PIC( vg_numero_plan, 
										vg_ordre_pic++,
										va_pic.District,
										"",
										va_pic.Priorite_C,
										va_pic.Identifiant,
										vl_explication,
										"Affichage",	
										vl_bandeau,	/*texte picto*/
										vl_Flash,
										vl_picto, 
										va_pic.Realisable) != XDC_OK)
				XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pic_message : pb appel a XZAP821_Ajout_Prop_PIC");
    	}
    	else /*automatique*/
		{
			/*on est ici, donc on traite du pilotage automatique*/
			/*premier cas : on est sur le district de l equipement*/
			if (va_pic.District == va_site) 
			{
				if (XZAC8021_Commande_PIC ( vm_noeud->NumEvt.numero,
											vm_noeud->NumEvt.cle,
											vg_num_poste,
											va_pic.Identifiant,
											va_horodate,
											va_pic.District,
											vl_bandeau,
											vl_Flash,
											vl_picto,
											va_pic.Priorite_C,
											va_pic.Distance_C,
											va_pic.District,
											&vl_action) != XDC_OK)
			    	XZST_03EcritureTrace (XZSTC_FONCTION, "drol_pic_recherche : pb dans l'appel a XZAC8021_Commande_PIC");
				else
					XZST_03EcritureTrace (XZSTC_FONCTION, "drol_pic_recherche : appel a XZAC8021_Commande_PIC OK");
			}
			else /*sur districts differents, envoyer un msg a TACLI*/ 
			{
#if 0
				if (XZAC803_Commande_PIC_TACLI (	vm_noeud->NumEvt.numero,
													vm_noeud->NumEvt.cle,
													vg_num_poste,
													va_pic.Identifiant,
													va_horodate,
													va_pic.District,
													vl_bandeau,
													vl_Flash,
													vl_picto,
													va_pic.Priorite_C,
													va_pic.Distance_C,
													vg_site) != XDC_OK)
				  	XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pic_recherche : pb dans l'appel a XZAC803_Commande_PIC_TACLI");
				else
					XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pic_recherche : appel a XZAC803_Commande_PIC_TACLI OK");
#endif
			}

			/*mise en base de la proposition PIC*/
			if (XZAP821_Ajout_Prop_PIC (vg_numero_plan,
										vg_ordre_pic++,
										va_pic.District,
										"",
										255,
										va_pic.Identifiant,
										vl_explication,
										"SAPIENS",	
										vl_bandeau,	/*texte picto*/
										vl_Flash,
										vl_picto, 
										/*va_pic.Destination1,*/		/*destination*/
										/*va_pic.Destination2,*/		/*destination alternat*/
										va_pic.Realisable) != XDC_OK)
				XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pic_message : pb appel a XZAP821_Ajout_Prop_PIC");
		}
	}
 	else /*DEM524*/
	{
		sprintf (vl_explication,"(%s)(%s)(%d)", vl_picto, vl_bandeau,vl_Flash);

		/*mise en base de la proposition PIC*/
		if (XZAP821_Ajout_Prop_PIC (vg_numero_plan,
									vg_ordre_pic++,
									va_pic.District,
									"",
									255,
									va_pic.Identifiant,
									vl_explication,
									"Affichage",	
									vl_bandeau,	/*texte picto*/
									vl_Flash,
									vl_picto, 
									/*va_pic.Destination1,*/		/*destination*/
									/*va_pic.Destination2,*/		/*destination alternat*/
									va_pic.Realisable) != XDC_OK)
			XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pic_message : pb appel a XZAP821_Ajout_Prop_PIC");

    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pic_message : prop identique a l etat courant");
	}
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  compare l'etat de chaque voie a l'etat de reference
*  tient compte du nombre de voies de circulation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_pic_voies(int VL, int VM1, int VM2, int VR, int Reference)

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



