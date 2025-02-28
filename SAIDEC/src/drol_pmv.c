/*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : drol_pmv.c				*
*************************************************
* DESCRIPTION :
* new tdpac
*************************************************
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.0
* cluchague 26/04/2002 REPERE1 si le pr du pmv est inferieur ou egal au PR du PMV
           on considere que le pmv est dans la perturbation
* cluchague 31/10/2003 REPERE2 on arrondit la longueur du bouchon
           a l'entier le plus proche en km (1.2)
* cluchague 31/12/2003 REPERE3 pour un bandeau sous picto indiquant une distance
          < 100m on met 50m ligne 918. D'autre part, les voies bloquees/neutralisees
          doivent etre vues comme des voies neutralisees pour les pmv (1.3)
* JMG	30/03/04	ajout bandeau et picto si bifurcation  1.4
* cluchague 11/05/2004 REPERE4 si ttes voies fermees SI -> CA V1.5
* JMG	17/12/04	ajout explication claire dans XZAP 1.6
* CHI	20/04/05	:rajout du sous-type grele sur la chaussee DEM457 v1.7
* JMG   17/02/04 codage de drol_pmv_datex (SAGA) v1.8
* PNI	14/06/05	:traitement des bouchons accordeon comme ralenti pour PMV DEM428 v1.9
* PNI	27/10/05	: Pour les PMV dont la proposition est egale a l'etat courant, on ajoute la prop avec une priorite 255 (cf xzap11sp.prc) DEM524 v1.10
* JMG	22/11/06	: SAPIENS V1.11 DEM 584
* PNI	18/12/06	: SAPIENS : Pas de pilotage auto si le PMV est utilise pour une perturbation a priorite sup ou egale a la proposition V1.12 DEM584
* PNI	20/02/06	: Pas de pilotage si FMC prevue (pmv_filtre)  V1.13 DEM628
* JMG	27/03/07	: TDP sur PMV dans bouchon 1.14 1.15 DEM640
* PNI	21/06/07 : Ajout des nouveaux type de fmc DEM658
* LCL   09/07/07 : Suppression DY
* PNI 17/07/07	:  Basculement pour travaux DEM658 v1.18
* PNI 23/07/07  : Prise en compte des distances pour ne pas piloter en SAPIENS DEM655 v1.19
* PNI 22/08/07  :  drol_pmv_var_localisation: retrait de l'offset 2 sens de la localisation DEM688 v1.20
DEM655 v1.19
* PNI 01/10/07  :drol_pmv_var_distance renvoie 0 M pour distance < 100m => mettre 50M dans ce cas DEM707 v1.21
		 Ajout de recuperation des localisants (XZAE21)dans le sens du pmv pour l pmv en sens opppose a l'evt DEM724 v1.21
* PCL 25/01/08  :FI214 pour basculement, PMV sens oppose affiche "apres sortie" qui est derriere lui. (mauvaise localisation p/r a la sortie amont) v1.22
* PNI 30/05/08	: Correction du "A 0 M" DEM777
* JPL	02/03/09	: Gestion conditions "feu maitrise / non maitrise" pour FMC "Vehicule en feu"" (DEM 845) 1.24
* PNI 10/03/09  : Correction du texte bandeau : K au lieu de KM pour distance > = 100 v1.25 DEM868
* PNI 09/10/09	: Traitement du picto en sens inverse DEM915 1.26
* PNI 24/08/10 : Suppression de l'exclusion de XDC_POINT_CARACT_PEAGE dans drol_pmv_sur_section_courante DEM956 1.27
* JPL	06/09/10 : Prise en compte du type Fmc Zone de stockage et du predicat "demarre" (DEM 942) 1.28
* JPL	18/02/11 : Migration architecture HP ia64 (DEM 971) : constantes et types SPECIFIQUES PMV  1.29
* JPL	21/02/11 : Migration architecture HP ia64 (DEM 971) : Zones des portions de texte 'static'  1.30
* JPL	21/02/11 : Migration HP ia64 (DEM 971) : Gestion conversions types de tailles differentes (=,-)  1.31
* JMG	31/08/11 : suppression des PMV intermediaires sur la zone TDP anticipe 1.32
* JPL	13/10/11 : Pas d'action automatique SAPIENS pour Fmc simulees (Essai ou Exercice) (DEM 995)  1.33
* JMG	31/01/12 : tdp etendu dem/1014 1.34
* JMG	29/05/12 : correction linux 1.35
* JPL	12/07/12 : Utilisation des noms de sortie "usager" et indicateurs de bifurcations (DEM 1033)  1.36
* PNI	27/07/12 : Correction drol_pmv_var_num_pt_car pour lit d'arret DEM1046 1.37
* JMG	11/11/12 : regulation vitesse 1.38
* PNI	06/02/13 : Correction ligne vide sur proposition PMV : strncpy remplace strcpy pour TextxPMV 1.39
* PNI	4/04/13	 : Correction ligne vide sur proposition PMV (Texte1PMV)
     : Prise en compte des sens des sorties fermees pour les FMC FermetureEch DEM 1066 1.40
* PNI	23/08/13	 : SAPIENS au CI  DEM 1074/FR 105 1.41
* PNI	20/01/15	: modification dela pol d'affichage PMV PMVA 1.43 DEM1106
* PNI	03/04/15	: correction TDP en sens 2 1.45 DEM1106
* JMG	16/06/15 	: evt dans les 2 sens lotPA 1.46 DEM1143
* PNI	16/09/15	: Ignorer les bifurcations pour la localisation apres sortie ou entre sorties 1.47 DEM1141
* PNI 	07/06/16	: verification 'avoir touve la destination prevue pour les 3 destinations attendues 1.48 DEM1169
* PNI	02/09/16	: Contresens : affichage dans les 2 sens v1.49 DEM1182
* LCL	17/11/16	: Ajout viaduc sur localisation sur appel XZAE21 (LOT23) v1.50
* JMG   23/02/17        : lot 23 traversee DEM1223 1.51
* JMG	20/06/17	: lot convergence 1.52
cd ..*			  correction pb abrevsortieamont sur autoroute inversee
* JMG	15/10/17	: fermeture tunnel toulon 1.53
* MEH   09/01/18        : retrait verification action intervenant DEM1662 1.54
* MEH   16/01/2018	: ajout cas bifurcation dans drol_pmv_var_num_pt_car DEM1268 1.54
* MEH   30/01/2018      : ajout cas Salage et deneigement DEM1259  1.55
* JPL	09/02/18 : Migration Linux 64 bits (DEM 1274): Zone texte 'static'; drol_pmv_var_dest_ou_loc: parentheses conditions  1.56
* JMG	18/05/18	: personnel travaux 1.57
* JMG	18/08/18	: type pmv. suppression SAGA DEM1306 1.58
* JPL	23/09/19	: Type d'evenement "INCENDIE" attendu en configuration et non "INCENDIE DE VEGETATION" (DEM 1352)  1.59
* JPL	25/09/19	: Support d'un predicat "bouchon mobile" et d'une variable "DISTANCE-1000" (DEM 1346)  1.60
* JMG	25/06/20 : on force la tabulation dans SAD_PMV pour l usage de EQT_FIL 1.61
* JPL	08/10/20	: Mise en majuscules du texte avant ecriture en base et envoi (DEM SAE168)  1.62
* ABE	24/11/20	: Ajout appel XZAO962 pour calcul tdp_fcd sur traversee de bouchon DEM-SAE152 1.63
* ABE	03/12/20 : Changement des seuils de validation TDP_FCD pour un const declaree dans xdc.h DEM-SAE152 1.64
* JPL	28/12/20	: Mise en majuscules du bandeau avant ecriture en base et envoi (DEM SAE168)  1.65
* PNI	23/03/20	: Ajout de la VR_I dans la fonction "au moins une voie neutralisee" SAE-249 1.66
* ABE	05/01/22  : Sur mode Auto, autoriser le pilotage des PMV/PMVA voisin DEM-SAE446
* GGY	02/05/23 : Suppression du force du Flash à XDC_PMV_PASFLASH (DEM-473)
* PNI	23/05/23  : si priorite superieure mais plus distant, alors on ne pilote pas en auto sauf priorité=255 SAE-485
* LCL	04/10/23 : Adaptations CNA DEM-483 : lecture site principal en base
* GGY	17/11/23	: Enregistrement Flash + Clignotement depuis la meme variable du fichier pert.txt (cf. mail PNI 17/11/23 9h52) (DEM483)
* GGY	28/11/23	: Remplacement traitement PMV Saga par PMV 1 et 2 lignes pour CNA (DEM483)
* LCL	15/12/23  : Fix Correction Flash pour pilotage auto
* GGY	15/12/23  : Fix Correction Cligno pour pilotage auto (DEM-562)
* PNI 19/03/23 : correctif var_num_pt_car : utiliser le numero de sortie pour les bifurcation aussi SAE-598
* GGY	26/02/24  : Fix Correction cligno lignes si Flash sur PMV sans Flash (DEM-562)
* GGY	23/01/24	: Ajout C_COND_CIRCULATION_BASC_TUNNEL et C_COND_CIRCULATION_NON_BASC_TUNNEL (DEM-483)
************************************************/

#include "drol_pmv.h"
#include "drol_caltdp.h"
#include <xdc.h>

#include "xzao574.h"
#include "xzac01.h"
#include "xzac101.h"
#include "xzae21.h"
#include "xzap02.h"
#include "xzap70.h"

#define ETDPC_TEXTE_TDP "%2dmn"
#define ETDPC_TEXTE_TDP_HOR "%1dh%02d"

#define C_REP_MIG "/produits/migrazur/appliSD/fichiers/bdc/"
#define C_PERT C_REP_MIG "pert.txt"

#define C_SEPARATEUR "#"
#define C_PIPE "|"
#define C_ET "$ET$"
#define C_CAR_DOLLAR '$'
#define C_PMV_EPSILON 1000
#define C_PMV_DMAX_FIN 1000
#define C_NB_CAR_LIGNE 15
#define C_INFINI 1000000

#define C_INFERIEUR "<"
#define C_SUPERIEUR ">"
#define C_EGAL "="

#define C_COND_UNE_VOIE_BLOQ "au moins une voie bloquee"
#define C_COND_UNE_VOIE_OUVERTE "au moins une voie ouverte"
#define C_COND_TOUTES_VOIES_BLOQ "toutes voies bloquees"
#define C_COND_UNE_VOIE_NEUT "au moins une voie neutralisee"
#define C_COND_TOUTES_VOIES_NEUT "toutes voies neutralisees"
#define C_COND_CIRCULATION_UNE_VOIE "circulation sur une voie"
#define C_COND_CIRCULATION_BASC "circulation basculee"
#define C_COND_SUR_BAU "sur bau"
#define C_COND_SUR_SECTION_COURANTE "sur section courante"
#define C_COND_SUR_LIT_ARRET "sur lit d'arret"
#define C_COND_SUR_ECHANGEUR "sur echangeur"
#define C_COND_SUR_BRETELLE_ENTREE "sur bretelle d'entree"
#define C_COND_SUR_BRETELLE_SORTIE "sur bretelle de sortie"
#define C_COND_SIGNALE "signale"
#define C_COND_CONFIRME "confirme"
#define C_COND_TRAFIC_DENSE "trafic dense"
#define C_COND_TRAFIC_RALENTI "trafic ralenti"
#define C_COND_TRAFIC_BOUCHE "trafic bouche"
#define C_COND_TRAFIC_BLOQUE "trafic bloque"
#define C_COND_VOIES_RETRECIES "voies retrecies"
#define C_COND_ENTREE_FERMEE "entree fermee"
#define C_COND_SORTIE_FERMEE "sortie fermee"
#define C_COND_DATEX "datex"
#define C_COND_FEU_MAITRISE "feu_maitrise"
#define C_COND_FEU_NON_MAITRISE "feu_non_maitrise"
#define C_COND_STOCKAGE_DEMARRE "stockage_demarre"
#define C_COND_STOCKAGE_NON_DEMARRE 		"stockage_non_demarre"
#define C_COND_HORS_GPL 					"hors_gpl"
#define C_COND_TRAFIC_RALENTISSEMENT 		"trafic_ralentissement"
#define C_COND_TRAFIC_BOUCHON 				"trafic_bouchon"
#define C_COND_MAJEUR						"majeur"
#define C_COND_INTERVENTION					"intervention"
#define C_COND_FERMETURE_TOULON				"fermeture toulon"
#define C_COND_PERSONNEL					"personnel"
#define C_COND_BOUCHON_MOBILE				"bouchon mobile"
#define C_COND_CIRCULATION_BASC_TUNNEL		"circulation_basculee"
#define C_COND_CIRCULATION_NON_BASC_TUNNEL	"circulation_non_basculee"

/*variables utilisees dans les messages PMV*/
#define C_VAR_LONGUEUR "LONGUEUR$"
#define C_VAR_LOCALISATION "LOCALISATION$"
#define C_VAR_DISTANCE_BANDEAU "DISTANCE BANDEAU$"
#define C_VAR_NUM_PT_CAR "NUM PT CAR$"
#define C_VAR_NOM_PT_CAR "NOM PT CAR$"
#define C_VAR_DISTANCE "DISTANCE$"
#define C_VAR_DISTANCE_1000 "DISTANCE-1000$"
#define C_VAR_CARBURANT "CARBURANT$"
#define C_VAR_CAR "CAR$"
#define C_VAR_AUTOROUTE "AUTOROUTE FERMEE$"
#define C_VAR_DESTINATION1 "DESTINATION1$"
#define C_VAR_DESTINATION2 "DESTINATION2$"
#define C_VAR_DESTINATION3 "DESTINATION3$"
#define C_VAR_DEST_OU_LOC "DEST_OU_LOC$"
#define C_VAR_TRAVERSEE "TRAVERSEE$"
#define C_VAR_AUTOROUTE_DISTANCE "AUTOROUTE DISTANCE$"
#define C_VAR_CAUSE "CAUSE$"

T_PERT *vg_pert;
T_NOEUD *vm_noeud;
static T_NOEUD *vm_noeud2;
T_NOEUD *vg_racine;
T_PMV vm_pmv;
XDY_Booleen vm_bouchon_apres_sortie;
XDY_Booleen vm_indice_bouchon;
XDY_Booleen vm_presence_nature_bouchon;
int vm_ligne_courante;
XDY_Booleen vm_destination1_HS;
XDY_Booleen vm_destination2_HS;
XDY_Booleen vm_destination3_HS;
int vm_destination1_numero;
int vm_destination2_numero;
int vm_destination3_numero;

/*extern XZADT_CONFIG_PMV                *pg_ListePMV=NULL;*/
extern XDY_Booleen vg_calcul_auto;
extern XZADT_Liste_Destinations *pg_dest;
extern XZAOT_TypePMV vg_type_pmv[25];
extern XDY_Booleen vg_presence_site[];

int drol_pmv_charge();
T_CONDITION *drol_pmv_decode_condition(char *);
T_LIGNE_PMV drol_pmv_remplit_ligne(char *);
void drol_pmv_message(T_PMV, T_PERT *, XDY_Horodate, XDY_District, XDY_Sens);
char *drol_pmv_calcul_ligne(T_LIGNE_PMV);
char *drol_pmv_calcul_ligne_minuscules(T_LIGNE_PMV);
char *drol_pmv_calcul_picto(char *, XDY_Booleen);

static XDY_Booleen drol_pmv_egal(int, int);
static XDY_Booleen drol_pmv_true(int, int);
/*
static XDY_Booleen drol_pmv_inferieur();
static XDY_Booleen drol_pmv_superieur();
*/

/*fonctions utilisees dans les conditions*/
static XDY_Booleen drol_pmv_fmc_simulee();
static XDY_Booleen drol_pmv_une_voie_bloquee();
static XDY_Booleen drol_pmv_une_voie_ouverte();
static XDY_Booleen drol_pmv_toutes_voies_bloquees();
static XDY_Booleen drol_pmv_une_voie_neutralisee();
static XDY_Booleen drol_pmv_toutes_voies_neutralisees();
static XDY_Booleen drol_pmv_circulation_une_voie();
static XDY_Booleen drol_pmv_circulation_basculee();
static XDY_Booleen drol_pmv_sur_bau();
static XDY_Booleen drol_pmv_sur_section_courante();
static XDY_Booleen drol_pmv_sur_lit_arret();
static XDY_Booleen drol_pmv_sur_echangeur();
static XDY_Booleen drol_pmv_sur_bretelle_entree();
static XDY_Booleen drol_pmv_sur_bretelle_sortie();
static XDY_Booleen drol_pmv_signale();
static XDY_Booleen drol_pmv_confirme();
static XDY_Booleen drol_pmv_trafic_dense();
static XDY_Booleen drol_pmv_trafic_ralenti();
static XDY_Booleen drol_pmv_trafic_bouche();
static XDY_Booleen drol_pmv_trafic_bloque();
static XDY_Booleen drol_pmv_voies_retrecies();
static XDY_Booleen drol_pmv_entree_fermee();
static XDY_Booleen drol_pmv_sortie_fermee();
static XDY_Booleen drol_pmv_datex();
static XDY_Booleen drol_pmv_feu_maitrise();
static XDY_Booleen drol_pmv_feu_non_maitrise();
static XDY_Booleen drol_pmv_stockage_demarre();
static XDY_Booleen drol_pmv_stockage_non_demarre();
static XDY_Booleen drol_pmv_hors_gpl();
static XDY_Booleen drol_pmv_trafic_ralentissement();
static XDY_Booleen drol_pmv_trafic_bouchon();
static XDY_Booleen drol_pmv_majeur();
static XDY_Booleen drol_pmv_intervention();
static XDY_Booleen drol_pmv_personnel();
static XDY_Booleen drol_pmv_bouchon_mobile();
static XDY_Booleen drol_pmv_fermeture_toulon();
static int drol_pmv_voies(int, int, int, int, int);
static int drol_pmv_recherche(T_PERT *, XDY_Horodate, XDY_District);
static XDY_Booleen drol_pmv_circulation_basculee_tunnel ();
static XDY_Booleen drol_pmv_circulation_non_basculee_tunnel ();

/*fonctions utilisees dans les variables des messages*/
static char *drol_pmv_var_longueur();
static char *drol_pmv_var_localisation();
static char *drol_pmv_var_distance_bandeau();
static char *drol_pmv_var_num_pt_car();
static char *drol_pmv_var_nom_pt_car();
static char *drol_pmv_var_distance();
static char *drol_pmv_var_distance_1000();
static char *drol_pmv_var_carburant();
static char *drol_pmv_var_car();
static char *drol_pmv_var_null();
static char *drol_pmv_var_autoroute();
static char *drol_pmv_var_destination1();
static char *drol_pmv_var_destination2();
static char *drol_pmv_var_destination3();
static char *drol_pmv_var_dest_ou_loc();
static char *drol_pmv_var_vitesse();
static char *drol_pmv_var_localisation_longueur();
static char *drol_pmv_var_traversee();
static char *drol_pmv_var_autoroute_distance();
static char *drol_pmv_var_cause();

static char *drol_pmv_majuscules(char *);

/* definition des variables locales au module */
static char *version = "$Id: drol_pmv.c,v 1.66 2021/03/23 16:28:26 gesconf Exp $ $Revision: 1.66 $ $Date: 2021/03/23 16:28:26 $ : commentaire";

int ajouter_type_pmv(XZAOT_TypePMV va_type_pmv, int va_indice)
{
	XZST_03EcritureTrace(XZSTC_WARNING, "ajouter_type_pmv va_type_pmv.NumType=%d, va_indice=%d", va_type_pmv.NumType, va_indice);

  vg_type_pmv[va_type_pmv.NumType] = va_type_pmv;
  return (XDC_OK);
}
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_pmv_init()

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
  XDY_ResConf vl_cr;
  XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : drol_pmv_init");
  vm_noeud2 = (T_NOEUD *)malloc(sizeof(T_NOEUD));

  vg_pert = NULL;
  drol_pmv_charge();

  if (XZAO74_Lire_Types_PMV(XDC_BASE_CFG, ajouter_type_pmv, &vl_cr) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "pb appel XZAO74_Lire_Types_PMV");
    return (XDC_NOK);
  }

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT drol_pmv_init");

  return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des perturbations
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_pmv_charge()

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
	FILE *pl_fp;
	int vl_retcode = 0;
	int vl_i;
	char vl_Lig[500];
	XDY_Entier vl_Resultat_out = 0;
	char *vl_str;
	char *vl_ch;
	T_PERT *pl_liste;
	int vl_num_prec, vl_num;
	char vl_l1[100], vl_l2[100], vl_l3[100], vl_l1f[50], vl_l2f[50], vl_l3f[50];
	char vl_b1[50], vl_b2[50], vl_b3[50], vl_b1f[50], vl_b2f[50], vl_b3f[50];
	char vl_s1[50], vl_s2[50], vl_s3[50], vl_s4[50], vl_s5[50], vl_s6[50];
	char vl_bandeau[100];
	char vl_pmv1[150];
	char vl_pmv2[150];
	char vl_condition[300];
	char vl_bandeau_bifurcation[100];
	int vl_auto;
	int vl_clignotement;
	char Ligne1_1_18[50], Ligne1_2_15[50], Ligne2_2_15[50], Ligne1_2_18[50], Ligne2_2_18[50];

	XDY_Booleen vl_vide;

	XZST_03EcritureTrace(XZSTC_DEBUG1, "IN  : drol_pmv_charge_cas");

	if ((pl_fp = fopen(C_PERT, "r")) == NULL)
	{
	  XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !", C_PERT);
	  return (XDC_NOK);
	}

	/*memorisation de la structure du message*/
	while (fgets(vl_Lig, sizeof(vl_Lig), pl_fp) != NULL)
	{
		pl_liste = (T_PERT *)malloc(sizeof(T_PERT));
		vl_str = vl_Lig;

		sscanf(vl_str, "%[^|]|%d|", pl_liste->Libelle, &(pl_liste->Priorite));

		XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_charge : libelle=%s, priorite=%d",
		                     pl_liste->Libelle, pl_liste->Priorite);

		sprintf(vl_l1, "%d", pl_liste->Priorite);
		vl_str = vl_str + strlen(pl_liste->Libelle) + 2 + strlen(vl_l1);

		sscanf(vl_str, "%[^|]|", vl_l1);
		pl_liste->Ligne1 = drol_pmv_remplit_ligne(vl_l1);
		vl_str = vl_str + strlen(vl_l1) + 1;

		if (vl_str[0] == '|')
		{
		  strcpy(vl_l2, "");
		  pl_liste->Ligne2 = drol_pmv_remplit_ligne("");
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l2);
		  pl_liste->Ligne2 = drol_pmv_remplit_ligne(vl_l2);
		  vl_str = vl_str + strlen(vl_l2) + 1;
		}
		if (vl_str[0] == '|')
		{
		  strcpy(vl_l3, "");
		  pl_liste->Ligne3 = drol_pmv_remplit_ligne("");
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l3);
		  pl_liste->Ligne3 = drol_pmv_remplit_ligne(vl_l3);
		  vl_str = vl_str + strlen(vl_l3) + 1;
		}
		XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_charge : ligne1=%s, ligne2=%s, ligne3=%s",
		                     vl_l1, vl_l2, vl_l3);

		vl_vide = TRUE;
		if (vl_str[0] == '|')
		{
		  strcpy(vl_l1, "");
		  pl_liste->Ligne1_Fin = drol_pmv_remplit_ligne("");
		  vl_str = vl_str + 1;
		}
		else
		{
		  vl_vide = FALSE;
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  pl_liste->Ligne1_Fin = drol_pmv_remplit_ligne(vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		if (vl_str[0] == '|')
		{
		  strcpy(vl_l2, "");
		  pl_liste->Ligne2_Fin = drol_pmv_remplit_ligne("");
		  vl_str = vl_str + 1;
		}
		else
		{
		  vl_vide = FALSE;
		  sscanf(vl_str, "%[^|]|", vl_l2);
		  pl_liste->Ligne2_Fin = drol_pmv_remplit_ligne(vl_l2);
		  vl_str = vl_str + strlen(vl_l2) + 1;
		}
		if (vl_str[0] == '|')
		{
		  strcpy(vl_l3, "");
		  pl_liste->Ligne3_Fin = drol_pmv_remplit_ligne("");
		  vl_str = vl_str + 1;
		}
		else
		{
		  vl_vide = FALSE;
		  sscanf(vl_str, "%[^|]|", vl_l3);
		  pl_liste->Ligne3_Fin = drol_pmv_remplit_ligne(vl_l3);
		  vl_str = vl_str + strlen(vl_l3) + 1;
		}
		XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_charge : ligne1_fin=%s,ligne2_fin=%s,ligne3_fin=%s",
		                     vl_l1, vl_l2, vl_l3);
		pl_liste->L_Fin_Vide = vl_vide;

		vl_vide = TRUE;
		if (vl_str[0] == '|')
		{
		  strcpy(vl_l1, "");
		  pl_liste->Bifurc1 = drol_pmv_remplit_ligne("");
		  vl_str = vl_str + 1;
		}
		else
		{
		  vl_vide = FALSE;
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  pl_liste->Bifurc1 = drol_pmv_remplit_ligne(vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		if (vl_str[0] == '|')
		{
		  strcpy(vl_l2, "");
		  pl_liste->Bifurc2 = drol_pmv_remplit_ligne("");
		  vl_str = vl_str + 1;
		}
		else
		{
		  vl_vide = FALSE;
		  sscanf(vl_str, "%[^|]|", vl_l2);
		  pl_liste->Bifurc2 = drol_pmv_remplit_ligne(vl_l2);
		  vl_str = vl_str + strlen(vl_l2) + 1;
		}
		if (vl_str[0] == '|')
		{
		  strcpy(vl_l3, "");
		  pl_liste->Bifurc3 = drol_pmv_remplit_ligne("");
		  vl_str = vl_str + 1;
		}
		else
		{
		  vl_vide = FALSE;
		  sscanf(vl_str, "%[^|]|", vl_l3);
		  pl_liste->Bifurc3 = drol_pmv_remplit_ligne(vl_l3);
		  vl_str = vl_str + strlen(vl_l3) + 1;
		}
		XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_charge : bifurc1=%s,bifurc2=%s,bifurc3=%s",
		                     vl_l1, vl_l2, vl_l3);
		pl_liste->B_Vide = vl_vide;

		if (vl_str[0] == '|')
		{
		  strcpy(vl_l1, "");
		  pl_liste->Bandeau = drol_pmv_remplit_ligne("");
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  pl_liste->Bandeau = drol_pmv_remplit_ligne(vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_charge : bandeau=%s", vl_l1);

		if (vl_str[0] == '|')
		{
		  strcpy(vl_pmv1, "");
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_pmv1);
		  vl_str = vl_str + strlen(vl_pmv1) + 1;
		}
		if (vl_str[0] == '|')
		{
		  strcpy(vl_pmv2, "");
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_pmv2);
		  vl_str = vl_str + strlen(vl_pmv2) + 1;
		}
		XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_charge : pmv1=%s, pmv2=%s",
		                     vl_pmv1, vl_pmv2);
		strcpy(pl_liste->Pmv1, vl_pmv1);
		strcpy(pl_liste->Pmv2, vl_pmv2);

		sscanf(vl_str, "%[^|]|", vl_condition);
		vl_str = vl_str + strlen(vl_condition) + 1;
		pl_liste->Condition = drol_pmv_decode_condition(vl_condition);

		if (vl_str[0] == '|')
		{
		  strcpy(vl_l1, " ");
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_charge : picto=%s", vl_l1);
		strcpy(pl_liste->Picto, vl_l1);

		/*autoroute, pr, sens pmv1 et pmv 2 : inutiles*/
		if (vl_str[0] == '|')
		{
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		if (vl_str[0] == '|')
		{
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		if (vl_str[0] == '|')
		{
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		if (vl_str[0] == '|')
		{
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		if (vl_str[0] == '|')
		{
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		if (vl_str[0] == '|')
		{
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}

		/*distances*/
		if (vl_str[0] == '|')
		{
		  pl_liste->DMax1 = C_INFINI;
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  sscanf(vl_l1, "%d", &(pl_liste->DMax1));
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		if (vl_str[0] == '|')
		{
		  pl_liste->DMax1_Bifurc = C_INFINI;
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  sscanf(vl_l1, "%d", &(pl_liste->DMax1_Bifurc));
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		if (vl_str[0] == '|')
		{
		  pl_liste->DMax2 = C_INFINI;
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  sscanf(vl_l1, "%d", &(pl_liste->DMax2));
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		if (vl_str[0] == '|')
		{
		  pl_liste->DMax2_Bifurc = C_INFINI;
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  sscanf(vl_l1, "%d", &(pl_liste->DMax2_Bifurc));
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_charge : D1=%d,D1b=%d,D2=%d,D2b=%d",
		                     pl_liste->DMax1, pl_liste->DMax1_Bifurc,
		                     pl_liste->DMax2, pl_liste->DMax2_Bifurc);

		vl_vide = TRUE;
		if (vl_str[0] == '|')
		{
		  strcpy(vl_l1, "");
		  pl_liste->Bifurc1_Fin = drol_pmv_remplit_ligne("");
		  vl_str = vl_str + 1;
		}
		else
		{
		  vl_vide = FALSE;
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  pl_liste->Bifurc1_Fin = drol_pmv_remplit_ligne(vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		if (vl_str[0] == '|')
		{
		  strcpy(vl_l2, "");
		  pl_liste->Bifurc2_Fin = drol_pmv_remplit_ligne("");
		  vl_str = vl_str + 1;
		}
		else
		{
		  vl_vide = FALSE;
		  sscanf(vl_str, "%[^|]|", vl_l2);
		  pl_liste->Bifurc2_Fin = drol_pmv_remplit_ligne(vl_l2);
		  vl_str = vl_str + strlen(vl_l2) + 1;
		}
		if (vl_str[0] == '|')
		{
		  strcpy(vl_l3, "");
		  pl_liste->Bifurc3_Fin = drol_pmv_remplit_ligne("");
		  vl_str = vl_str + 1;
		}
		else
		{
		  vl_vide = FALSE;
		  sscanf(vl_str, "%[^|]|", vl_l3);
		  pl_liste->Bifurc3_Fin = drol_pmv_remplit_ligne(vl_l3);
		  vl_str = vl_str + strlen(vl_l3) + 1;
		}
		XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_charge : bifurc1_fin=%s,bifurc2_fin=%s,bifurc3_fin=%s",
		                     vl_l1, vl_l2, vl_l3);
		pl_liste->B_Fin_Vide = vl_vide;

		/*1.4*/
		/*chargement picto si bifurcation*/
		if (vl_str[0] == '|')
		{
		  strcpy(vl_l1, " ");
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_charge : picto_bifurcation=%s", vl_l1);
		strcpy(pl_liste->Picto_Bifurcation, vl_l1);

		/*bandeau si bifurcation*/
		if (vl_str[0] == '|')
		{
		  strcpy(vl_l1, "");
		  pl_liste->Bandeau_Bifurcation = drol_pmv_remplit_ligne("");
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  pl_liste->Bandeau_Bifurcation = drol_pmv_remplit_ligne(vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_charge : bandeau_Bifurcation=%s", vl_l1);

		/*mode automatique*/
		if (vl_str[0] == '|')
		{
		  pl_liste->Auto = 0;
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  sscanf(vl_l1, "%d", &(pl_liste->Auto));
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_charge : automatique=%s", vl_l1);

		/* Partie specifique pour les PMV CNA 1 et 2 lignes */
		/* PMV 1 Ligne */
		if (vl_str[0] == '|')
		{
		  strcpy(vl_l1, "");
		  pl_liste->Ligne1_1_15 = drol_pmv_remplit_ligne("");
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  pl_liste->Ligne1_1_15 = drol_pmv_remplit_ligne(vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}

		/* PMV 2 Lignes */
		if (vl_str[0] == '|')
		{
		  strcpy(vl_l1, "");
		  pl_liste->Ligne1_2_15 = drol_pmv_remplit_ligne("");
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  pl_liste->Ligne1_2_15 = drol_pmv_remplit_ligne(vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}

		if (vl_str[0] == '|')
		{
			strcpy(vl_l2, "");
			pl_liste->Ligne2_2_15 = drol_pmv_remplit_ligne("");
			vl_str = vl_str + 1;
		}
		else
		{
			sscanf(vl_str, "%[^|]|", vl_l2);
			pl_liste->Ligne2_2_15 = drol_pmv_remplit_ligne(vl_l2);
			vl_str = vl_str + strlen(vl_l2) + 1;
		}

		/* Partie specifique pour les PMV SAGA -- plus utilise */
		// Ligne1_2_18
		if (vl_str[0] == '|')
		{
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}
		// Ligne2_2_18
		if (vl_str[0] == '|')
		{
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  vl_str = vl_str + strlen(vl_l1) + 1;
		}

		/* Clignotement */
		if (vl_str[0] == '|')
		{
			pl_liste->Clignotement = XDC_PMV_PASCLIGNO;
			vl_str = vl_str + 1;
		}
		else
		{
			sscanf(vl_str, "%[^|]|", vl_l1);
			sscanf(vl_l1,"%d",&(pl_liste->Clignotement));
			vl_str = vl_str + strlen(vl_l1) + 1;
		  	if (pl_liste->Clignotement != 0)
			{
				pl_liste->Clignotement = XDC_PMV_CLIGNOLENT;
			}
		}
		/* Flash */
		if (vl_str[0] == '|')
		{
		  pl_liste->Flash = XDC_PMV_PASFLASH;
		  vl_str = vl_str + 1;
		}
		else
		{
		  sscanf(vl_str, "%[^|]|", vl_l1);
		  sscanf(vl_l1, "%d", &(pl_liste->Flash));
		  vl_str = vl_str + strlen(vl_l1) + 1;
		  if (pl_liste->Flash != 0)
		  {
		    pl_liste->Flash = XDC_PMV_FLASH;
		  }
		}

		pl_liste->Suivant = vg_pert;
		vg_pert = pl_liste;
	}

	fclose(pl_fp);
	XZST_03EcritureTrace(XZSTC_DEBUG1, "OUT  : drol_pmv_charge_cas");
	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* decodage de la condition
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

T_CONDITION *drol_pmv_decode_condition(char *va_cond)

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
  char vl_str[100];
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
  XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_decode_condition:  IN %s", va_cond);

  /*allocation memoire*/
  pl_cond = (T_CONDITION *)malloc(sizeof(T_CONDITION));

  /*1 condition  = le type*/
  vl_indice = 0;
  while ((va_cond[vl_indice] != C_CAR_DOLLAR) && (vl_indice < strlen(va_cond)))
  {
    pl_cond->Libelle[vl_indice] = va_cond[vl_indice];
    vl_indice++;
  }
  pl_cond->Libelle[vl_indice] = '\0';

  pl_cond->Type = C_TYPE_COMPARATEUR;
  pl_cond->Fonction.Comp.Slot = &(vm_noeud2->Type);
  pl_cond->Fonction.Comp.Fct = drol_pmv_egal;
  pl_cond->Suivant = NULL;
  pl_tmp = pl_cond;

  /*numero en fonction du type*/
  XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_decode_condition: type = %s", pl_cond->Libelle);
  XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_decode_condition: type = %s %d", pl_cond->Libelle, pl_cond->Fonction.Comp.Valeur);

  strcpy(vl_ch, pl_cond->Libelle);
  if (!strcmp(vl_ch, "ACCIDENT"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Accident;
  else if (!strcmp(vl_ch, "TOUS"))
  {
    pl_cond->Fonction.Comp.Fct = drol_pmv_true;
    pl_cond->Fonction.Comp.Valeur = 0;
  }
  else if (!strcmp(vl_ch, "VEHICULE ARRETE"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_VehArrete;
  else if (!strcmp(vl_ch, "VEHICULE EN PANNE"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_VehPanne;
  else if (!strcmp(vl_ch, "VEHICULE EN FEU"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_VehFeu;
  else if (!strcmp(vl_ch, "VEHICULE A CONTRESENS"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Contresens;
  else if (!strcmp(vl_ch, "BOUCHON"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_QueueBouchon;
  else if (!strcmp(vl_ch, "MANIFESTATION"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Manifestation;
  else if (!strcmp(vl_ch, "Basculement"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Basculement;
  else if (!strcmp(vl_ch, "Aire fermee"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_AireFermee;
  else if (!strcmp(vl_ch, "Delestage"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Delestage;
  else if (!strcmp(vl_ch, "Deviation"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Deviation;
  else if (!strcmp(vl_ch, "Travaux"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Travaux;
  else if (!strcmp(vl_ch, "Salage"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Salage;
  else if (!strcmp(vl_ch, "Fermeture echangeur"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_FermetureEch;
  else if (!strcmp(vl_ch, "INCENDIE"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Incendie;
  else if (!strcmp(vl_ch, "GREVE PERSONNEL ESCOTA"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_GreveESCOTA;
  else if (!strcmp(vl_ch, "HOLD-UP / VOL"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_HoldUpVol;
  else if (!strcmp(vl_ch, "OPERATION ESCARGOT"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Escargot;
  else if (!strcmp(vl_ch, "AUTRE EVENEMENT"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Autre;
  else if (!strcmp(vl_ch, "CIRQUE EN CONVOI"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Cirque;
  else if (!strcmp(vl_ch, "CONVOI EXCEPTIONNEL"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_ConvoiExc;
  else if (!strcmp(vl_ch, "CONVOI MILITAIRE"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_ConvoiMil;
  else if (!strcmp(vl_ch, "VEHICULE LENT"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_VehLent;
  else if (!strcmp(vl_ch, "EBOULEMENT"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Eboulement;
  else if (!strcmp(vl_ch, "OBSTACLE SUR CHAUSSEE"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Obstacle;
  else if (!strcmp(vl_ch, "INONDATION"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Inondation;
  else if (!strcmp(vl_ch, "ALTERCATION USAGERS"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_AltercationUsager;
  else if (!strcmp(vl_ch, "AGRESSION"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Agression;
  else if (!strcmp(vl_ch, "PIETONS SUR LA CHAUSSEE"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_PietonChaussee;
  else if (!strcmp(vl_ch, "EVENEMENT HORS RESEAU"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_EvtHorsReseau;
  else if (!strcmp(vl_ch, "MALAISE USAGER"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_MalaiseUsager;
  else if (!strcmp(vl_ch, "FRONTIERE FERMEE"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_FrontiereFermee;
  else if (!strcmp(vl_ch, "CHAUSSEE DETERIORIEE"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_ChausseDeter;
  else if (!strcmp(vl_ch, "FLAQUE D'HUILE"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_FlaqueHuile;
  else if (!strcmp(vl_ch, "ANIMAL ERRANT"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_AnimalErrant;
  else if (!strcmp(vl_ch, "CHAUSSEE DETERIOREE"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_ChausseDeter;
  else if (!strcmp(vl_ch, "VEHICULE EN MARCHE ARRIERE"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_VehMarcheArr;
  else if (!strcmp(vl_ch, "Traitement chaussees"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_TraitChaussees;
  else if (!strcmp(vl_ch, "Echangeur deconseille"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_EchDeconseille;
  else if (!strcmp(vl_ch, "CHAUSSEE GLISSANTE"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_ChausseeGliss;
  else if (!strcmp(vl_ch, "Basculement pour travaux"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_BasculTravaux;
  else if (!strcmp(vl_ch, "Regulation"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Regulation;
  else if (!strcmp(vl_ch, "Zone de stockage"))
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_ZoneStockage;

  /*traiter le cas particulier du traitement de chaussees*/
  else if (!strcmp(vl_ch, "SALAGE"))
  {
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_TraitChaussees;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant = NULL;
    pl_tmp = pl_cond->Suivant;
    pl_tmp->Type = C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct = drol_pmv_egal;
    pl_tmp->Fonction.Comp.Slot = &(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur = XZAEC_TRT_CH_Salage;
  }

  else if (!strcmp(vl_ch, "DENEIGEMENT"))
  {
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_TraitChaussees;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant = NULL;
    pl_tmp = pl_cond->Suivant;
    pl_tmp->Type = C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct = drol_pmv_egal;
    pl_tmp->Fonction.Comp.Slot = &(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur = XZAEC_TRT_CH_Deneigement;
  }

  /*traiter le cas particulier de la meteo*/
  else if (!strcmp(vl_ch, "VERGLAS"))
  {
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant = NULL;
    pl_tmp = pl_cond->Suivant;
    pl_tmp->Type = C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct = drol_pmv_egal;
    pl_tmp->Fonction.Comp.Slot = &(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur = XZAEC_MET_Verglas;
  }
  else if (!strcmp(vl_ch, "CHUTE NEIGE"))
  {
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant = NULL;
    pl_tmp = pl_cond->Suivant;
    pl_tmp->Type = C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct = drol_pmv_egal;
    pl_tmp->Fonction.Comp.Slot = &(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur = XZAEC_MET_ChuteNeige;
  }
  else if (!strcmp(vl_ch, "PLUIE"))
  {
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant = NULL;
    pl_tmp = pl_cond->Suivant;
    pl_tmp->Type = C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct = drol_pmv_egal;
    pl_tmp->Fonction.Comp.Slot = &(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur = XZAEC_MET_Pluie;
  }
  else if (!strcmp(vl_ch, "BROUILLARD GIVRANT"))
  {
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant = NULL;
    pl_tmp = pl_cond->Suivant;
    pl_tmp->Type = C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct = drol_pmv_egal;
    pl_tmp->Fonction.Comp.Slot = &(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur = XZAEC_MET_BrouillardGiv;
  }
  else if (!strcmp(vl_ch, "NEIGE SUR CHAUSSEE"))
  {
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant = NULL;
    pl_tmp = pl_cond->Suivant;
    pl_tmp->Type = C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct = drol_pmv_egal;
    pl_tmp->Fonction.Comp.Slot = &(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur = XZAEC_MET_NeigeChaus;
  }
  else if (!strcmp(vl_ch, "GRELE SUR CHAUSSEE"))
  {
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant = NULL;
    pl_tmp = pl_cond->Suivant;
    pl_tmp->Type = C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct = drol_pmv_egal;
    pl_tmp->Fonction.Comp.Slot = &(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur = XZAEC_MET_GreleChaus;
  }
  else if (!strcmp(vl_ch, "BROUILLARD"))
  {
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant = NULL;
    pl_tmp = pl_cond->Suivant;
    pl_tmp->Type = C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct = drol_pmv_egal;
    pl_tmp->Fonction.Comp.Slot = &(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur = XZAEC_MET_Brouillard;
  }
  else if (!strcmp(vl_ch, "VENT"))
  {
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant = NULL;
    pl_tmp = pl_cond->Suivant;
    pl_tmp->Type = C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct = drol_pmv_egal;
    pl_tmp->Fonction.Comp.Slot = &(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur = XZAEC_MET_VentFort;
  }
  else if (!strcmp(vl_ch, "GRELE"))
  {
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant = NULL;
    pl_tmp = pl_cond->Suivant;
    pl_tmp->Type = C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct = drol_pmv_egal;
    pl_tmp->Fonction.Comp.Slot = &(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur = XZAEC_MET_Grele;
  }
  /*traitement du cas particulier de l'incident sous concessionnaire*/
  else if ((!strcmp(vl_ch, "FERMETURE STATION")) ||
           (!strcmp(vl_ch, "FERMETURE STATION\n")))
  {
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_IncSsConces;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant = NULL;
    pl_tmp = pl_cond->Suivant;
    pl_tmp->Type = C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct = drol_pmv_egal;
    pl_tmp->Fonction.Comp.Slot = &(vm_noeud2->Infos_FAX.Type_Incident);
    pl_tmp->Fonction.Comp.Valeur = 1;
  }
  else if (!strcmp(vl_ch, "PENURIE"))
  {
    pl_cond->Fonction.Comp.Valeur = XZAEC_FMC_IncSsConces;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant = NULL;
    pl_tmp = pl_cond->Suivant;
    pl_tmp->Type = C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct = drol_pmv_egal;
    pl_tmp->Fonction.Comp.Slot = &(vm_noeud2->Infos_FAX.Type_Incident);
    pl_tmp->Fonction.Comp.Valeur = 0;
  }
  /*cas particulier du lit d'arret*/
  else if (!strcmp(vl_ch, "sur lit d'arret"))
  {
    pl_cond->Fonction.Comp.Slot = &(vm_noeud2->Localisation.TypePointCar);
    pl_cond->Fonction.Comp.Valeur = XDC_POINT_CARACT_LIT_ARRET;
  }
  else
  {
    XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_decode_condition : pb decodage type !!!");
    pl_cond->Fonction.Comp.Valeur = 0;
  }

  /*si pas de condition supplementaire*/
  if (vl_indice == strlen(va_cond))
  {
    XZST_03EcritureTrace(XZSTC_DEBUG1, "OUT : drol_pmv_decode_condition : que le type");
    return (pl_cond);
  }

  /*supprimer $ET$*/
  va_cond = va_cond + strlen(pl_cond->Libelle) + 4;

  strcpy(vl_ch_init, va_cond);
  XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_decode_condition : apres le type = %s", vl_ch_init);

pos_decode:
  vl_indice = 0;
  while ((va_cond[vl_indice] != C_CAR_DOLLAR) && (vl_indice < strlen(va_cond)))
  {
    vl_str[vl_indice] = va_cond[vl_indice];
    vl_indice++;
  }
  vl_str[vl_indice] = '\0';

  /*allocation de la memoire pour la condition suivante*/
  pl_tmp->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
  pl_tmp->Suivant->Suivant = NULL;
  pl_tmp = pl_tmp->Suivant;

  //XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_decode_condition:  cond %s", vl_str);
  XZST_03EcritureTrace(XZSTC_WARNING, " drol_pmv_decode_condition:  cond %s", vl_str);
  strcpy(pl_tmp->Libelle, vl_str);

  /*il s'agit d'une fonction PREDICAT*/
  pl_tmp->Type = C_TYPE_PREDICAT;
  if (!strcmp(vl_str, C_COND_UNE_VOIE_BLOQ))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_une_voie_bloquee;
  else if (!strcmp(vl_str, C_COND_UNE_VOIE_OUVERTE))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_une_voie_ouverte;
  else if (!strcmp(vl_str, C_COND_TOUTES_VOIES_BLOQ))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_toutes_voies_bloquees;
  else if (!strcmp(vl_str, C_COND_TOUTES_VOIES_NEUT))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_toutes_voies_neutralisees;
  else if (!strcmp(vl_str, C_COND_UNE_VOIE_NEUT))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_une_voie_neutralisee;
  else if (!strcmp(vl_str, C_COND_CIRCULATION_UNE_VOIE))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_circulation_une_voie;
  else if (!strcmp(vl_str, C_COND_CIRCULATION_BASC))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_circulation_basculee;
  else if (!strcmp(vl_str, C_COND_CONFIRME))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_confirme;
  else if (!strcmp(vl_str, C_COND_SIGNALE))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_signale;
  else if (!strcmp(vl_str, C_COND_SUR_BAU))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_sur_bau;
  else if (!strcmp(vl_str, C_COND_SUR_SECTION_COURANTE))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_sur_section_courante;
  else if (!strcmp(vl_str, C_COND_SUR_LIT_ARRET))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_sur_lit_arret;
  else if (!strcmp(vl_str, C_COND_SUR_ECHANGEUR))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_sur_echangeur;
  else if (!strcmp(vl_str, C_COND_SUR_BRETELLE_ENTREE))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_sur_bretelle_entree;
  else if (!strcmp(vl_str, C_COND_SUR_BRETELLE_SORTIE))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_sur_bretelle_sortie;
  else if (!strcmp(vl_str, C_COND_TRAFIC_DENSE))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_trafic_dense;
  else if (!strcmp(vl_str, C_COND_TRAFIC_RALENTI))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_trafic_ralenti;
  else if (!strcmp(vl_str, C_COND_TRAFIC_BOUCHE))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_trafic_bouche;
  else if (!strcmp(vl_str, C_COND_TRAFIC_BLOQUE))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_trafic_bloque;
  else if (!strcmp(vl_str, C_COND_DATEX))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_datex;
  else if (!strcmp(vl_str, C_COND_VOIES_RETRECIES))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_voies_retrecies;
  else if (!strcmp(vl_str, C_COND_ENTREE_FERMEE))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_entree_fermee;
  else if (!strcmp(vl_str, C_COND_SORTIE_FERMEE))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_sortie_fermee;
  else if (!strcmp(vl_str, C_COND_FEU_MAITRISE))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_feu_maitrise;
  else if (!strcmp(vl_str, C_COND_FEU_NON_MAITRISE))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_feu_non_maitrise;
  else if (!strcmp(vl_str, C_COND_STOCKAGE_DEMARRE))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_stockage_demarre;
  else if (!strcmp(vl_str, C_COND_STOCKAGE_NON_DEMARRE))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_stockage_non_demarre;
  else if (!strcmp(vl_str, C_COND_HORS_GPL))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_hors_gpl;
  else if (!strcmp(vl_str, C_COND_TRAFIC_RALENTISSEMENT))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_trafic_ralentissement;
  else if (!strcmp(vl_str, C_COND_TRAFIC_BOUCHON))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_trafic_bouchon;
  else if (!strcmp(vl_str, C_COND_MAJEUR))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_majeur;
  else if (!strcmp(vl_str, C_COND_INTERVENTION))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_intervention;
  else if (!strcmp(vl_str, C_COND_FERMETURE_TOULON))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_fermeture_toulon;
  else if (!strcmp(vl_str, C_COND_PERSONNEL))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_personnel;
  else if (!strcmp(vl_str, C_COND_BOUCHON_MOBILE))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_bouchon_mobile;
  else if (!strcmp(vl_str, C_COND_CIRCULATION_BASC_TUNNEL))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_circulation_basculee_tunnel;
  else if (!strcmp(vl_str, C_COND_CIRCULATION_NON_BASC_TUNNEL))
    pl_tmp->Fonction.Predicat.Fct = drol_pmv_circulation_non_basculee_tunnel;

  /*si fin de chaine*/
  if (vl_indice == strlen(va_cond))
  {
    XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_decode_condition : fin de chaine");
    return (pl_cond);
  }
  else
  {
    va_cond = va_cond + strlen(vl_str) + 4;
    goto pos_decode;
  }
}

/**************************************/
/**************************************/
static XDY_Booleen drol_pmv_fermeture_toulon()
{
  if (vm_noeud->Type != XZAEC_FMC_Deviation)
    return (FALSE);
  if (vm_noeud->Localisation.TypePointCar != XDC_POINT_CARACT_ECHANGEUR)
    return (FALSE);
  XZST_03EcritureTrace(XZSTC_WARNING, "FERMETURE echangeur %d", vm_noeud->Localisation.NumPointCar);
  /*localisation sur l'echangeur immediatement en amont du tunnel*/
  /* vm_noeud->Localisation.NumPointCar*/
  if (vm_noeud->Localisation.NumPointCar == 217)
    return (TRUE);
  if (vm_noeud->Localisation.NumPointCar == 218)
    return (TRUE);
  return (FALSE);
}
static XDY_Booleen drol_pmv_intervention()
{
  return (vm_noeud->Infos_FAX.Agent_Escota);
}
static XDY_Booleen drol_pmv_personnel()
{
  XZST_03EcritureTrace(XZSTC_WARNING, "PERSONNEL");
  return (vm_noeud->Infos_FAX.Personnel != 0);
}

static XDY_Booleen drol_pmv_bouchon_mobile()
{
  if (vm_noeud->Infos_FAX.Presence_Bouchon == XDC_VRAI)
    return (XDC_VRAI);
  return (XDC_FAUX);
}

static XDY_Booleen drol_pmv_majeur()
{
  if (vm_noeud->Infos_TFM.Majeur == XDC_VRAI)
    return (XDC_VRAI);
  return (XDC_FAUX);
}
static XDY_Booleen drol_pmv_fmc_simulee()
{
  return (strstr(vm_noeud->Infos_TFM.Origine, "ESSAI") != NULL ||
          strstr(vm_noeud->Infos_TFM.Origine, "EXERCICE") != NULL);
}

static XDY_Booleen drol_pmv_sur_echangeur()
{
  return (vm_noeud->Localisation.TypePointCar == XDC_POINT_CARACT_ECHANGEUR);
}
static XDY_Booleen drol_pmv_sur_lit_arret()
{
  return (vm_noeud->Localisation.TypePointCar == XDC_POINT_CARACT_LIT_ARRET);
}

static XDY_Booleen drol_pmv_egal(int va_valeur1, int va_valeur2) { return (va_valeur1 == va_valeur2); }
static XDY_Booleen drol_pmv_true(int va_valeur1, int va_valeur2) { return (TRUE); }
/*
static XDY_Booleen drol_pmv_superieur(int va_valeur1, int va_valeur2) { return (va_valeur1>va_valeur2); }
static XDY_Booleen drol_pmv_inferieur(int va_valeur1, int va_valeur2) { return (va_valeur1<va_valeur2); }
*/

static XDY_Booleen drol_pmv_confirme() { return (vm_noeud->Infos_TFM.Confirme); }
static XDY_Booleen drol_pmv_signale() { return (vm_noeud->Infos_TFM.Signale); }

static XDY_Booleen drol_pmv_une_voie_bloquee()
{
  return ((vm_noeud->Localisation.VL == XDC_VOIE_BLOQUEE) ||
          (vm_noeud->Localisation.VM1 == XDC_VOIE_BLOQUEE) ||
          (vm_noeud->Localisation.VM2 == XDC_VOIE_BLOQUEE) ||
          (vm_noeud->Localisation.VR == XDC_VOIE_BLOQUEE));
}
static XDY_Booleen drol_pmv_une_voie_ouverte()
{
  return ((vm_noeud->Localisation.VL == XDC_VOIE_SENS_NORMAL) ||
          (vm_noeud->Localisation.VM1 == XDC_VOIE_SENS_NORMAL) ||
          (vm_noeud->Localisation.VM2 == XDC_VOIE_SENS_NORMAL) ||
          (vm_noeud->Localisation.VR == XDC_VOIE_SENS_NORMAL));
}
static XDY_Booleen drol_pmv_toutes_voies_bloquees()
{
  return ((vm_noeud->Localisation.VL != XDC_VOIE_INCONNUE) &&
          (vm_noeud->Localisation.VL != XDC_VOIE_INEXISTANTE) &&
          (vm_noeud->Localisation.VL != XDC_VOIE_SENS_NORMAL) &&
          (vm_noeud->Localisation.VM1 != XDC_VOIE_SENS_NORMAL) &&
          (vm_noeud->Localisation.VM2 != XDC_VOIE_SENS_NORMAL) &&
          (vm_noeud->Localisation.VR != XDC_VOIE_SENS_NORMAL) &&
          (vm_noeud->Localisation.BAU != XDC_VOIE_SENS_NORMAL) &&
          (vm_noeud->Localisation.VR_I != XDC_VOIE_SENS_INVERSE) &&
          (vm_noeud->Localisation.VM2_I != XDC_VOIE_SENS_INVERSE) &&
          (vm_noeud->Localisation.VM1_I != XDC_VOIE_SENS_INVERSE) &&
          (vm_noeud->Localisation.VL_I != XDC_VOIE_SENS_INVERSE));
}
/* REPERE3 */
static XDY_Booleen drol_pmv_une_voie_neutralisee()
{
  return ((vm_noeud->Localisation.VL == XDC_VOIE_NEUTRALISEE) ||
          (vm_noeud->Localisation.VM1 == XDC_VOIE_NEUTRALISEE) ||
          (vm_noeud->Localisation.VM2 == XDC_VOIE_NEUTRALISEE) ||
          (vm_noeud->Localisation.VR == XDC_VOIE_NEUTRALISEE) ||
          (vm_noeud->Localisation.VL == XDC_VOIE_BLOQUEE_NEUTRALISEE) ||
          (vm_noeud->Localisation.VM1 == XDC_VOIE_BLOQUEE_NEUTRALISEE) ||
          (vm_noeud->Localisation.VM2 == XDC_VOIE_BLOQUEE_NEUTRALISEE) ||
          (vm_noeud->Localisation.VR == XDC_VOIE_BLOQUEE_NEUTRALISEE) ||
          (vm_noeud->Localisation.VR_I == XDC_VOIE_NEUTRALISEE) ||
          (vm_noeud->Localisation.VR_I == XDC_VOIE_BLOQUEE_NEUTRALISEE));
}

static XDY_Booleen drol_pmv_toutes_voies_neutralisees()
{
  return ((vm_noeud->Localisation.BAU != XDC_VOIE_SENS_NORMAL) &&
          (vm_noeud->Localisation.VR_I != XDC_VOIE_SENS_INVERSE) &&
          (vm_noeud->Localisation.VM2_I != XDC_VOIE_SENS_INVERSE) &&
          (vm_noeud->Localisation.VM1_I != XDC_VOIE_SENS_INVERSE) &&
          (vm_noeud->Localisation.VL_I != XDC_VOIE_SENS_INVERSE) &&
          (drol_pmv_voies(vm_noeud->Localisation.VL,
                          vm_noeud->Localisation.VM1,
                          vm_noeud->Localisation.VM2,
                          vm_noeud->Localisation.VR,
                          XDC_VOIE_NEUTRALISEE) == 15));
}

static XDY_Booleen drol_pmv_circulation_une_voie()
{
  return (((vm_noeud->Localisation.VL != XDC_VOIE_SENS_NORMAL) &&
           (vm_noeud->Localisation.VM1 != XDC_VOIE_SENS_NORMAL) &&
           (vm_noeud->Localisation.VM2 != XDC_VOIE_SENS_NORMAL) &&
           (vm_noeud->Localisation.VR == XDC_VOIE_SENS_NORMAL)) ||
          ((vm_noeud->Localisation.VL != XDC_VOIE_SENS_NORMAL) &&
           (vm_noeud->Localisation.VM1 != XDC_VOIE_SENS_NORMAL) &&
           (vm_noeud->Localisation.VM2 == XDC_VOIE_SENS_NORMAL) &&
           (vm_noeud->Localisation.VR != XDC_VOIE_SENS_NORMAL)) ||
          ((vm_noeud->Localisation.VL != XDC_VOIE_SENS_NORMAL) &&
           (vm_noeud->Localisation.VM1 == XDC_VOIE_SENS_NORMAL) &&
           (vm_noeud->Localisation.VM2 != XDC_VOIE_SENS_NORMAL) &&
           (vm_noeud->Localisation.VR != XDC_VOIE_SENS_NORMAL)) ||
          ((vm_noeud->Localisation.VL == XDC_VOIE_SENS_NORMAL) &&
           (vm_noeud->Localisation.VM1 != XDC_VOIE_SENS_NORMAL) &&
           (vm_noeud->Localisation.VM2 != XDC_VOIE_SENS_NORMAL) &&
           (vm_noeud->Localisation.VR != XDC_VOIE_SENS_NORMAL)));
}

static XDY_Booleen drol_pmv_circulation_basculee()
{
  return (((vm_noeud->Localisation.VL == XDC_VOIE_SENS_INVERSE) ||
           (vm_noeud->Localisation.VM1 == XDC_VOIE_SENS_INVERSE) ||
           (vm_noeud->Localisation.VM2 == XDC_VOIE_SENS_INVERSE) ||
           (vm_noeud->Localisation.VR == XDC_VOIE_SENS_INVERSE) ||
           (vm_noeud->Localisation.VL_I == XDC_VOIE_SENS_INVERSE) ||
           (vm_noeud->Localisation.VM1_I == XDC_VOIE_SENS_INVERSE) ||
           (vm_noeud->Localisation.VM2_I == XDC_VOIE_SENS_INVERSE) ||
           (vm_noeud->Localisation.VR_I == XDC_VOIE_SENS_INVERSE))

  );
}

static XDY_Booleen drol_pmv_sur_bau()
{
  return ((drol_pmv_voies(vm_noeud->Localisation.VL, vm_noeud->Localisation.VM1,
                          vm_noeud->Localisation.VM2,
                          vm_noeud->Localisation.VR,
                          XDC_VOIE_SENS_NORMAL) == 15) &&
          ((vm_noeud->Localisation.BAU == XDC_VOIE_BLOQUEE) ||
           (vm_noeud->Localisation.BAU == XDC_VOIE_NEUTRALISEE)));
}

static XDY_Booleen drol_pmv_sur_section_courante()
{
  return ((vm_noeud->Localisation.TypePointCar != XDC_POINT_CARACT_AIRE) &&
          (vm_noeud->Localisation.TypePointCar != XDC_POINT_CARACT_ECHANGEUR) &&
          (vm_noeud->Localisation.TypePointCar != XDC_POINT_CARACT_LIT_ARRET));
}
static XDY_Booleen drol_pmv_sur_bretelle_entree()
{
  return (((vm_noeud->Localisation.Sens == XDC_SENS_ENTREE_SUD) ||
           (vm_noeud->Localisation.Sens == XDC_SENS_ENTREE_NORD)) &&
          (vm_noeud->Localisation.Bifurcation != XDC_VRAI));
}

static XDY_Booleen drol_pmv_sur_bretelle_sortie()
{
  return (((vm_noeud->Localisation.Sens == XDC_SENS_SORTIE_SUD) ||
           (vm_noeud->Localisation.Sens == XDC_SENS_SORTIE_NORD)) &&
          (vm_noeud->Localisation.Bifurcation != XDC_VRAI));
}

static XDY_Booleen drol_pmv_trafic_ralentissement()
{
  return ((vm_noeud->Infos_FAX.Type_Bouchon == XZAEC_BOU_Ralenti) || (vm_noeud->Infos_FAX.Type_Bouchon == XZAEC_BOU_Accordeon) || (vm_noeud->Infos_FAX.Type_Bouchon == XZAEC_BOU_Dense));
}

static XDY_Booleen drol_pmv_trafic_bouchon()
{
  return ((vm_noeud->Infos_FAX.Type_Bouchon == XZAEC_BOU_Bouche) || (vm_noeud->Infos_FAX.Type_Bouchon == XZAEC_BOU_Bloque));
}

static XDY_Booleen drol_pmv_trafic_dense()
{
  return (vm_noeud->Infos_FAX.Type_Bouchon == XZAEC_BOU_Dense);
}

static XDY_Booleen drol_pmv_trafic_ralenti()
{ /*DEM428*/
  return ((vm_noeud->Infos_FAX.Type_Bouchon == XZAEC_BOU_Ralenti) || (vm_noeud->Infos_FAX.Type_Bouchon == XZAEC_BOU_Accordeon));
}

static XDY_Booleen drol_pmv_trafic_bouche()
{
  return (vm_noeud->Infos_FAX.Type_Bouchon == XZAEC_BOU_Bouche);
}

static XDY_Booleen drol_pmv_trafic_bloque()
{
  return (vm_noeud->Infos_FAX.Type_Bouchon == XZAEC_BOU_Bloque);
}

static XDY_Booleen drol_pmv_datex() { return (vm_noeud->Infos_TFM.Datex == XDC_VRAI); } /*SAGA*/

static XDY_Booleen drol_pmv_voies_retrecies() { return (vm_noeud->Localisation.Voies_Retrecies); }

static XDY_Booleen drol_pmv_entree_fermee()
{
  return ((vm_noeud->Infos_FAX.Entree1) || (vm_noeud->Infos_FAX.Entree2));
}

static XDY_Booleen drol_pmv_sortie_fermee()
{
  return ((vm_noeud->Infos_FAX.Sortie1) || (vm_noeud->Infos_FAX.Sortie2));
}

static XDY_Booleen drol_pmv_feu_maitrise()
{
  return (vm_noeud->Infos_FAX.Feu_Maitrise != XDC_FAUX);
}

static XDY_Booleen drol_pmv_feu_non_maitrise()
{
  return (vm_noeud->Infos_FAX.Feu_Maitrise == XDC_FAUX);
}

static XDY_Booleen drol_pmv_stockage_demarre()
{
  return (vm_noeud->Localisation.Voies_Retrecies);
}

static XDY_Booleen drol_pmv_stockage_non_demarre()
{
  if (vm_noeud->Localisation.Voies_Retrecies == XDC_VRAI)
  {
    return (XDC_FAUX);
  }
  return (XDC_VRAI);
}

static XDY_Booleen drol_pmv_hors_gpl()
{
  return (vm_noeud->Infos_FAX.Numero_Carburant != XDC_CRB_GPL);
}

static XDY_Booleen drol_pmv_circulation_basculee_tunnel ()
{
	XDY_Booleen vl_basculement = XDC_FAUX;
	XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmv_circulation_basculee_tunnel : avant XZAE861_Basculement_Tunnel");

	if (XZAE861_Basculement_Tunnel (vm_noeud->Localisation.Autoroute,
									vm_noeud->Localisation.PR,
									vm_noeud->Localisation_Tete.PR,
									&vl_basculement)	!= XDC_OK) 
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmv_circulation_basculee_tunnel : pb appel a XZAE861_Basculement_Tunnel");
		return (FALSE);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmv_circulation_basculee_tunnel : XZAE861_Basculement_Tunnel retourne %d", vl_basculement);
	}
	return (vl_basculement);
}

static XDY_Booleen drol_pmv_circulation_non_basculee_tunnel ()
{
	XZST_03EcritureTrace (XZSTC_WARNING, "drol_pmv_circulation_non_basculee_tunnel : not drol_pmv_circulation_basculee_tunnel");
	return (!drol_pmv_circulation_basculee_tunnel());
}

/**************************************/
/**************************************/
/*fonctions des variables dans les messages*/
static char *drol_pmv_var_longueur()
{
  T_Localisation vl_loc;
  T_Localisation vl_loc_tete;
  static char vl_str[150];
  int vl_inversee;
  int vl_longueur_bouchon_km_arrondi;
  int vl_longueur_fin_bouchon_km_arrondi;

  vl_loc = vm_noeud->Localisation;
  vl_loc_tete = vm_noeud->Localisation_Tete;
  strcpy(vl_str, "");

  vl_inversee = 1;
  if (vl_loc.Autoroute == 8)
    vl_inversee = -1;
  vm_indice_bouchon = vm_ligne_courante;
  /*cas d'un PMV en amont de la perturbation*/
  /* REPERE1 remplacer et < et > par <= et >= */
  if ((!((vm_pmv.Sens == XDC_SENS_SUD) &&
         (vm_pmv.PR * vl_inversee >= vl_loc.PR * vl_inversee)) &&
       !((vm_pmv.Sens == XDC_SENS_NORD) &&
         (vm_pmv.PR * vl_inversee <= vl_loc.PR * vl_inversee))) ||
      (vm_pmv.Autoroute != vl_loc.Autoroute))
  {
    if (vl_loc.Longueur < 2000)
      sprintf(vl_str, "BOUCHON");
    else
    {
      /* REPERE 2 : on arrondit la longueur du bouchon a l'entier le plus proche */
      if ((vl_loc.Longueur % 1000) >= 500)
        vl_longueur_bouchon_km_arrondi = (int)(vl_loc.Longueur / 1000) + 1;
      else
        vl_longueur_bouchon_km_arrondi = (int)(vl_loc.Longueur / 1000);
      if (vl_longueur_bouchon_km_arrondi > 9)
        sprintf(vl_str, "BOUCHON de %dkm",
                vl_longueur_bouchon_km_arrondi);
      else
        sprintf(vl_str, "BOUCHON de %d km",
                vl_longueur_bouchon_km_arrondi);
      vm_presence_nature_bouchon = TRUE;
    }
    return (vl_str);
  }

  /*cas d'un PMV dans la perturbation*/
  /* REPERE1 remplacer et < et > par <= et >= */
  if ((((vm_pmv.Sens == XDC_SENS_SUD) &&
        (vm_pmv.PR * vl_inversee >= vl_loc.PR * vl_inversee)) ||
       ((vm_pmv.Sens == XDC_SENS_NORD) &&
        (vm_pmv.PR * vl_inversee <= vl_loc.PR * vl_inversee))) &&
      (vm_pmv.Autoroute == vl_loc.Autoroute))
  {
    if (vm_pmv.Distance_C <= 500)
    {
      if ((vm_pmv.Distance_C >= 100))
      {
        sprintf(vl_str, "BOUCHON\nsur %d m", (int)(vm_pmv.Distance_C / 100) * 100);
      }
      else
        sprintf(vl_str, "BOUCHON"); /*DEM777*/
    }
    else
    {
      /* REPERE 2 : on arrondit la longueur du bouchon a l'entier le plus proche */
      if ((vm_pmv.Distance_C % 1000) >= 500)
        vl_longueur_fin_bouchon_km_arrondi = (int)(vm_pmv.Distance_C / 1000) + 1;
      else
        vl_longueur_fin_bouchon_km_arrondi = (int)(vm_pmv.Distance_C / 1000);
      sprintf(vl_str, "BOUCHON\nsur %d km",
              vl_longueur_fin_bouchon_km_arrondi);
    }
    return (vl_str);
  }
  return (vl_str);
}

static char *drol_pmv_var_localisation_longueur()
{

  static char vl_str_l1[150] = "";
  static char vl_str_l2[150] = "";
  static char vl_str_l[150] = "";

  sprintf(vl_str_l1, "%s", drol_pmv_var_longueur());
  sprintf(vl_str_l2, "%s", drol_pmv_var_localisation());
  sprintf(vl_str_l, "%s\\%s", &vl_str_l1[8], vl_str_l2);
  vm_indice_bouchon = vm_ligne_courante - 1;
  XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_localisation_longueur=%s/%s/ %s-%d", vl_str_l1, vl_str_l2, vl_str_l, vm_indice_bouchon);
  return (vl_str_l);
}

static char *drol_pmv_var_traversee() {
	static char vl_str[50];
	XDY_PR vl_pr_min;
	XDY_PR vl_pr_max;
	XDY_TDP vl_tdp;
	XZADT_CONFIG_PMV pl_pmv;
	XDY_TDP vl_tdp_fcd=0;
	XDY_Octet vl_validite_fcd=0;
	XDY_Booleen vl_validite_faible=1;
  XDY_Octet						vl_inhb;

  vm_pmv.Destination1 = 0;
  if (vm_noeud->Localisation.PR < vm_noeud->Localisation_Tete.PR)
  {
    vl_pr_min = vm_noeud->Localisation.PR;
    vl_pr_max = vm_noeud->Localisation_Tete.PR;
  }
  else
  {
    vl_pr_max = vm_noeud->Localisation.PR;
    vl_pr_min = vm_noeud->Localisation_Tete.PR;
  }

  if (vm_pmv.PMV_SAGA)
    return ("");

  if (XZAD_recherche_dest_pmv(vm_pmv.Identifiant, &pl_pmv) != XDC_OK)
    return ("");

  /* Test pour savoir si les tdp sur cette eqt sont inhibe */
  xzae59_Rechercher_EQT_TDP_INHIB(vm_pmv.Identifiant,XDC_EQT_PMV,&vl_inhb);
  if (vl_inhb)
  {
	  XZST_03EcritureTrace(XZSTC_INFO,"drol_pmv_var_traversee TDP PMV %d inhibe, on retourne -1",vm_pmv.Identifiant);
  	return ("");
  }

	/*Recuperation du TDP grace a l'appel a la procedure XZAO962 (entre PMV et fin bouchon si bouchon est sur la meme autoroute*/
	XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmv_var_traversee num/cle=%d/%d",vm_noeud->NumEvt.numero,vm_noeud->NumEvt.cle);
	if (XZAO962_Calcul_TDP_FCD(  	vm_pmv.Autoroute,
		vm_pmv.PR,
		vm_pmv.Sens,				
		0, // on ne peut pas donner vm_noeud->Localisation_Tete.Autoroute car jamais renseigne
		vm_noeud->Localisation_Tete.PR,
		0,// on ne peut pas donner vm_noeud->Localisation_Tete.Sens car jamais renseigne 
		vm_noeud->NumEvt.numero,
		vm_noeud->NumEvt.cle,
		&vl_tdp_fcd,
		&vl_validite_fcd,
		&vl_validite_faible)!= XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmv_var_traversee : pb appel a XZAO962_Calcul_TDP_FCD");
		return ("");
	}

  /*Si vl_validite_fcd >= 60% et que toutes les donnees ont une validite > 20% (vl_validite_faible = 0) alors on utilise le temps de parcous de la procedure XZAO962*/
  if ((vl_validite_fcd >= XDC_SEUIL_AFFICHAGE_TDP_FCD) && (vl_validite_faible == 0))
  {
    vl_tdp = (vl_tdp_fcd + 59) / 60; /*TDP converti en minutes et arrondi a la minute superieure*/
    XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_traversee: pour Autoroute : %d PR PMV : %d, validite tdp_fcd : %d, validite tdp_fcd_faible :%d validite tdp_fcd>%d. Tdp=%d", vm_pmv.Autoroute, vm_pmv.PR, vl_validite_fcd, vl_validite_faible, XDC_SEUIL_AFFICHAGE_TDP_FCD, vl_tdp_fcd);
  }
  else
  {

    if (XZAD30_Calculer_Traversee(vm_pmv.Autoroute,
                                  vm_pmv.PR,
                                  vm_pmv.Sens,
                                  vl_pr_min,
                                  vl_pr_max,
                                  pl_pmv.Graphe_Destination,
                                  &vl_tdp) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_traversee : pb appel a XZAD30_Calculer_Traversee");
      return ("");
    }
    if (vl_tdp <= 0 && vl_validite_fcd >= XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA) // Si le tdp est negatif ou nul et si la validite issue de XZAO962 >=40% alors on recupere le tdp issu de XZAO962*/
    {
      vl_tdp = (vl_tdp_fcd + 59) / 60; /*TDP converti en minutes et arrondi a la minute superieure*/
      XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_traversee: pour Autoroute : %d PR PMV : %d, validite tdp_fcd : %d, validite tdp_fcd_faible :%d, validite tdp_fcd>%d & ESCOTA tdp<=0. Tdp=%d", vm_pmv.Autoroute, vm_pmv.PR, vl_validite_fcd, vl_validite_faible, XDC_SEUIL_AFFICHAGE_TDP_FCD_ABSENCE_TDP_ESCOTA, vl_tdp);
    }
    else // Sinon on utilise le tdp issu de XZAD30_Calculer_Traversee
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_traversee: pour Autoroute : %d PR PMV : %d, validite tdp_fcd : %d, validite tdp_fcd_faible :%d. Tdp ESCCOTA=%d", vm_pmv.Autoroute, vm_pmv.PR, vl_validite_fcd, vl_validite_faible, vl_tdp);
    }
  }

  XZST_03EcritureTrace(XZSTC_WARNING, "toto1");

  if (vl_tdp <= 0)
    return ("");

  /*SEUIL 10 MINUTES TRAVERSEE*/
  if (vl_tdp <= 10)
    return ("");

  strcpy(vl_str, "TRAVERSEE :");
  if (vl_tdp > 600)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "vl_tdp>600");
    /*sprintf( &vl_str[11], ETDPC_TEXTE_TDP_HOR, 9, 59 );
    vl_str[15] = '\0';*/
    return ("");
  }
  else if (vl_tdp > 59)
  {
    sprintf(&vl_str[11], ETDPC_TEXTE_TDP_HOR, vl_tdp / 60, vl_tdp % 60);
    vl_str[15] = '\0';
  }
  else if (vl_tdp > 0)
  {
    sprintf(&vl_str[11], ETDPC_TEXTE_TDP, vl_tdp);
    vl_str[15] = '\0';
  }
  else
  {
    vm_destination1_HS = TRUE;
    XZST_03EcritureTrace(XZSTC_WARNING, "toto");
    strcpy(vl_str, "");
  }
  XZST_03EcritureTrace(XZSTC_FONCTION, "traversee : %s", vl_str);
  vm_pmv.Destination1 = XDC_DESTINATION_TRAVERSEE;
  vm_pmv.Destination2 = XDC_DESTINATION_TRAVERSEE;
  vm_pmv.Destination3 = XDC_DESTINATION_TRAVERSEE;
  vm_destination1_numero = XDC_DESTINATION_TRAVERSEE;
  vm_destination2_numero = XDC_DESTINATION_TRAVERSEE;
  vm_destination3_numero = XDC_DESTINATION_TRAVERSEE;

  return (vl_str);
}

static char *drol_pmv_var_destination1()
{

  XDY_Destination_SAD vl_dest;
  XDY_PR vl_pr_min;
  XDY_PR vl_pr_max;
  XDY_TDP vl_tdp;
  static char vl_str[50];
  int vl_longueur;
  XDY_Nom vl_libelle;
  XZADT_CONFIG_PMV pl_pmv;
  XZADT_CONFIG_PMV *pl_pmv2;
  char vl_nom[15] = "";

  vm_destination1_HS = TRUE;
  if (vm_noeud->Localisation.PR < vm_noeud->Localisation_Tete.PR)
  {
    vl_pr_min = vm_noeud->Localisation.PR;
    vl_pr_max = vm_noeud->Localisation_Tete.PR;
  }
  else
  {
    vl_pr_max = vm_noeud->Localisation.PR;
    vl_pr_min = vm_noeud->Localisation_Tete.PR;
  }

  /*on commence par chercher la destination associe au PMV*/
  /* = premiere destination en aval*/
  vm_pmv.Destination1 = 0;
  vm_pmv.Destination2 = 0;
  vm_pmv.Destination3 = 0;
  vm_destination1_numero = 0;
  vm_destination2_numero = 0;
  vm_destination3_numero = 0;

  XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_destination1:  avant xZAD_recherche_dest_pmv");
  if (vm_pmv.PMV_SAGA)
    return ("");

  if (XZAD_recherche_dest_pmv(vm_pmv.Identifiant, &pl_pmv) != XDC_OK)
    return ("");

  /*for ( pl_PMV2 = pg_ListePMV; pl_PMV != NULL; pl_PMV = pl_PMV->suivant  ) {
if ( pl_PMV2->Numero ==vm_pmv.Identifiant  )
break;
  }*/

  XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_destination1:  apres XZAD_recherche_dest_pmv num %d %d", pl_pmv.Numero, pl_pmv.Destination);
  /*on calcule le tdp entre le PMV et cette destination*/
  if (XZAD23_Calculer_TdP_PMV_Dest(vm_pmv.PR,
                                   vm_pmv.Autoroute,
                                   vm_pmv.Sens,
			                             pl_pmv.Destination,
			                             pl_pmv.Graphe_Destination,
                                   &vl_tdp,vl_nom,vm_pmv.Identifiant,XDC_EQT_PMV) != XDC_OK)
    return ("");

  XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_destination1:  TDP %d", vl_tdp);
  if (vl_tdp <= 0)
  {
    return ("");
  }

  vm_pmv.Destination1 = pl_pmv.Destination;
  vm_pmv.Destination2 = 0;
  vm_pmv.Destination3 = 0;
  vm_destination1_numero = pl_pmv.Destination;

  /*TDP DOIT ETRE CADRE A DROITE*/
  if (XZAD27_Libelle_Destination((int)pl_pmv.Destination, vl_libelle) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_destination1: pb dans l'appel a XZAD27_Libelle_Destination");
    return ("");
  }
  strcpy(vl_dest.Libelle_PMV, vl_nom);
  vl_longueur = (int)strlen(vl_dest.Libelle_PMV);
  strncat(vl_dest.Libelle_PMV, "                          ", (size_t)15 - vl_longueur);
  vl_dest.Libelle_PMV[11] = '\0';
  vm_destination1_HS = FALSE;
  strcpy(vl_str, vl_dest.Libelle_PMV);
  if (vl_tdp > 600)
  {
    sprintf(vl_str, ETDPC_TEXTE_TDP_HOR, 9, 59);
    vl_str[15] = '\0';
  }
  else if (vl_tdp > 59)
  {
    sprintf(&vl_str[11], ETDPC_TEXTE_TDP_HOR, vl_tdp / 60, vl_tdp % 60);
    vl_str[15] = '\0';
  }
  else if (vl_tdp > 0)
  {
    sprintf(&vl_str[11], ETDPC_TEXTE_TDP, vl_tdp);
    vl_str[15] = '\0';
  }
  else
  {
    vm_destination1_HS = TRUE;
    strcpy(vl_str, "");
  }
  /*sprintf(vl_str,"%s%2dmn", vl_dest.Libelle_PMV, vl_tdp);*/
  XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_destination1: sortie = %s", vl_str);

  return (vl_str);
}

static char *drol_pmv_var_destination2()
{
  XDY_Destination_SAD vl_dest;
  XDY_PR vl_pr_min;
  XDY_PR vl_pr_max;
  XDY_TDP vl_tdp;
  static char vl_str[50];
  XZADT_CONFIG_PMV pl_PMV;
  XDY_Nom vl_libelle;
  char vl_nom[15];

  int vl_longueur;

  vm_destination2_HS = TRUE;

  /*on commence par chercher la destination associe au PMV*/
  /* = DEUXIEME destination en aval*/
  vm_destination2_numero = 0;
  if (vm_pmv.PMV_SAGA)
    return ("");

      XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmv_var_destination2: avant XZAD_recherche_dest_pmv");
 if (XZAD_recherche_dest_pmv(vm_pmv.Identifiant, &pl_PMV)==XDC_OK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmv_var_destination2: dest lointaine %d",pl_PMV.DestinationLointaine);
    if (pl_PMV.DestinationLointaine != 0 ){
    /*on calcule le tdp entre le PMV et cette destination*/
    if (XZAD23_Calculer_TdP_PMV_Dest(     vm_pmv.PR,
                          vm_pmv.Autoroute,
                          vm_pmv.Sens,
			  pl_PMV.DestinationLointaine,
			  pl_PMV.Graphe_DestinationLointaine,
                          &vl_tdp,vl_nom,vm_pmv.Identifiant,XDC_EQT_PMV) == XDC_OK) {
      XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmv_var_destination2: tdp %d",vl_tdp);
      if (vl_tdp>0)
        vm_destination2_HS=FALSE; 
    }
  }
   }

  if (!vm_destination2_HS)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_destination2: avant XZAD27_Libelle_Destination");
    vm_pmv.Destination2 = pl_PMV.DestinationLointaine;
    vm_destination2_numero = pl_PMV.DestinationLointaine;
    /*TDP DOIT ETRE CADRE A DROITE*/
    if (XZAD27_Libelle_Destination((int)pl_PMV.DestinationLointaine, vl_libelle) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_destination2: pb dans l'appel a XZAD27_Libelle_Destination");
      return ("");
    }

    strcpy(vl_dest.Libelle_PMV, vl_nom);
    vl_longueur = (int)strlen(vl_dest.Libelle_PMV);
    strncat(vl_dest.Libelle_PMV, "                          ", (size_t)15 - vl_longueur);
    vl_dest.Libelle_PMV[11] = '\0';
    strcpy(vl_str, vl_dest.Libelle_PMV);

    if (vl_tdp > 600)
    {
      sprintf(vl_str, ETDPC_TEXTE_TDP_HOR, 9, 59);
      vl_str[15] = '\0';
    }
    else if (vl_tdp > 59)
    {
      sprintf(&vl_str[11], ETDPC_TEXTE_TDP_HOR, vl_tdp / 60, vl_tdp % 60);
      vl_str[15] = '\0';
    }
    else if (vl_tdp > 0)
    {
      sprintf(&vl_str[11], ETDPC_TEXTE_TDP, vl_tdp);
      vl_str[15] = '\0';
    }
    else
    {
      vm_destination1_HS = TRUE;
      strcpy(vl_str, "");
    }

    /*   sprintf(vl_str,"%s%2dmn", vl_dest.Libelle_PMV, vl_tdp);*/
    XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_destination2: sortie %s", vl_str);
    return (vl_str);
  }
  XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_destination2: sortie nulle");
  return ("");
}

static char *drol_pmv_var_destination3()
{
  XDY_Destination_SAD vl_dest;
  XDY_Destination_SAD vl_destL;
  XDY_Destination_SAD vl_destN;
  XDY_PR vl_pr_min;
  XDY_PR vl_pr_max;
  XDY_TDP vl_tdp;
  static char vl_str[50];
  int vl_longueur;
  XZADT_CONFIG_PMV pl_PMV;
  XDY_Nom vl_libelle;
  char vl_nom[15];

  vm_destination3_HS = TRUE;

  /*on commence par chercher la destination associe au PMV*/
  /* = TROISIEME destination en aval*/
  vm_destination3_numero = 0;

  if (vm_pmv.PMV_SAGA)
    return ("");

  XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_destination3:  avant XZAD_recherche_dest_pmv");
  if (XZAD_recherche_dest_pmv(vm_pmv.Identifiant, &pl_PMV) == XDC_OK)
  {
    /*on calcule le tdp entre le PMV et cette destination*/
	XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmv_var_destination3: DestinationNoeud %d",pl_PMV.DestinationNoeud);
        if (pl_PMV.DestinationNoeud != 0)
        {
                if (XZAD23_Calculer_TdP_PMV_Dest( vm_pmv.PR,
                                                          vm_pmv.Autoroute,
                                                          vm_pmv.Sens,
                                                          pl_PMV.DestinationNoeud,
                                                          pl_PMV.Graphe_DestinationNoeud,
                                                          &vl_tdp,vl_nom,vm_pmv.Identifiant,XDC_EQT_PMV) == XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmv_var_destination3: tdp %d",vl_tdp);
    if (vl_tdp>0)
        vm_destination3_HS=FALSE;
	}
        }
   }

  if (!vm_destination3_HS)
  {
    vm_pmv.Destination3 = pl_PMV.DestinationNoeud;
    vm_destination3_numero = pl_PMV.DestinationNoeud;
    /*TDP DOIT ETRE CADRE A DROITE*/
    XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_destination3:avant XZAD27_Libelle_Destination");
    if (XZAD27_Libelle_Destination((int)pl_PMV.DestinationNoeud, vl_libelle) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING,
                           "drol_pmv_var_destination1: pb dans l'appel a XZAD27_Libelle_Destination");
      return ("");
    }

    strcpy(vl_libelle, vl_nom);
    vl_longueur = (int)strlen(vl_libelle);
    strncat(vl_libelle, "                          ", (size_t)15 - vl_longueur);
    vl_libelle[11] = '\0';
    strcpy(vl_str, vl_libelle);
    if (vl_tdp > 600)
    {
      sprintf(vl_str, ETDPC_TEXTE_TDP_HOR, 9, 59);
      vl_str[15] = '\0';
    }
    else if (vl_tdp > 59)
    {
      sprintf(&vl_str[11], ETDPC_TEXTE_TDP_HOR, vl_tdp / 60, vl_tdp % 60);
      vl_str[15] = '\0';
    }
    else if (vl_tdp > 0)
    {
      sprintf(&vl_str[11], ETDPC_TEXTE_TDP, vl_tdp);
      vl_str[15] = '\0';
    }
    else
    {
      vm_destination1_HS = TRUE;
      strcpy(vl_str, "");
    }

    /*sprintf(vl_str,"%s%2dmn", vl_libelle, vl_tdp);*/
    XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_destination3:sortie %s", vl_str);
    return (vl_str);
  }
  XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_destination3: sortie nulle");
  return ("");
}

static char *drol_pmv_var_dest_ou_loc()
{

  XZADT_Liste_Destinations *pl_dest;
  XZADT_Liste_Destinations *pl_destination;
  XZADT_Liste_Destinations *pl_destinationNoeud;
  XZADT_Liste_Destinations *pl_destinationLointaine;
  XZADT_CONFIG_PMV pl_pmv;
  XDY_PR vl_pr_min;
  XDY_PR vl_pr_max;
  XDY_TDP vl_tdp;
  static char vl_str[50];
  static char vl_str1[50];
  static char vl_str2[50];
  char *pl_ligne = NULL;
  int vl_longueur;
  XDY_Nom vl_libelle;
  char vl_nom[15];
  XZEZT_Liste_Zdp *pl_graphe_Destination = NULL;
  int vl_destination = 0;
  char vl_trouve = XDC_FAUX;
  XZST_03EcritureTrace(XZSTC_FONCTION, "IN:drol_pmv_var_dest_ou_loc");
  vm_destination3_HS = TRUE;
  if (vm_noeud->Localisation.PR < vm_noeud->Localisation_Tete.PR)
  {
    vl_pr_min = vm_noeud->Localisation.PR;
    vl_pr_max = vm_noeud->Localisation_Tete.PR;
  }
  else
  {
    vl_pr_max = vm_noeud->Localisation.PR;
    vl_pr_min = vm_noeud->Localisation_Tete.PR;
  }
  XZST_03EcritureTrace(XZSTC_WARNING, "avant XZAD_recherche_dest_pmv %d %d %d",
                       vm_noeud->Localisation.Sens,
                       vm_noeud->Localisation.Autoroute,
                       vm_noeud->Localisation_Tete.PR);
  /*on commence par chercher la destination associe au PMVA*/
  /* = premiere destination en aval*/
  vm_destination3_HS = TRUE;
  vm_destination3_numero = 0;

  pl_destination = (XZADT_Liste_Destinations *)0;
  pl_destinationNoeud = (XZADT_Liste_Destinations *)0;
  pl_destinationLointaine = (XZADT_Liste_Destinations *)0;

  if (vm_pmv.PMV_SAGA)
  {
    vm_destination3_HS = TRUE;
    if (vm_noeud->Localisation.Longueur > 3000)
      return (drol_pmv_var_localisation_longueur());
    else
      return (drol_pmv_var_localisation());
  }
  if (XZAD_recherche_dest_pmv(vm_pmv.Identifiant, &pl_pmv) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, " XZAD_recherche_dest_pmv NOK");
    vm_destination3_HS = TRUE;
    if (vm_noeud->Localisation.Longueur > 3000)
      return (drol_pmv_var_localisation_longueur());
    else
      return (drol_pmv_var_localisation());
  }

  /*TRAVERSEE*/
  strcpy(vl_str, "");
  if ((vm_noeud->Type == XZAEC_FMC_QueueBouchon) &&
      (vm_noeud->Infos_TFM.Type_Cause != 0) &&
      (vm_noeud->Infos_TFM.Type_Cause != XZAEC_FMC_TeteBouchon) &&
      (vm_noeud->Infos_TFM.Type_Cause != XZAEC_FMC_QueueBouchon) &&
      (vm_noeud->Infos_TFM.Type_Cause != XZAEC_FMC_Autre))
  {
    strcpy(vl_str, drol_pmv_var_traversee());
  }

  if (strlen(vl_str) == 0)
  {
    /* recherche des infos des 3 destinations */
    pl_dest = pg_dest;
    vl_trouve = 0;

    while (pl_dest != NULL)
    {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "dest_ou_loc : dest :%d", pl_dest->destination.Numero);
      if (pl_dest->destination.Numero == pl_pmv.Destination)
      {
        pl_destination = pl_dest;
        vl_trouve = 1;
      }
      else if (pl_dest->destination.Numero == pl_pmv.DestinationLointaine)
      {
        pl_destinationLointaine = pl_dest;
        vl_trouve = 1;
      }
      else if (pl_dest->destination.Numero == pl_pmv.DestinationNoeud)
      {
        pl_destinationNoeud = pl_dest;
        vl_trouve = 1;
      }
      pl_dest = pl_dest->suivant;
    }
    XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_dest_ou_loc trouve=%d", vl_trouve);
    if (vl_trouve == 0)
    {
      vm_destination3_HS = TRUE;
      if (vm_noeud->Localisation.Longueur > 3000)
        return (drol_pmv_var_localisation_longueur());
      else
        return (drol_pmv_var_localisation());
    }
    /* DEM1169 : verification d'avoir trouve chaque destination */
    if ((pl_pmv.Destination != 0) && (pl_destination == 0))
      pl_pmv.Destination = 0;
    if ((pl_pmv.DestinationLointaine != 0) && (pl_destinationLointaine == 0))
      pl_pmv.DestinationLointaine = 0;
    if ((pl_pmv.DestinationNoeud != 0) && (pl_destinationNoeud == 0))
      pl_pmv.DestinationNoeud = 0;
    XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_dest_ou_loc %d,%d,%d", pl_pmv.Destination, pl_pmv.DestinationLointaine, pl_pmv.DestinationNoeud);
    vl_trouve = 0;
    /* recherche de la destination en aval du bouchon */
    if (((vm_noeud->Localisation.Sens == XDC_SENS_SUD) && (vm_noeud->Localisation.Autoroute != 8)) ||
        ((vm_noeud->Localisation.Sens == XDC_SENS_NORD) && (vm_noeud->Localisation.Autoroute == 8)))
    {
      if (pl_pmv.Destination != 0)
      {
        if ((pl_destination->destination.Autoroute == vm_noeud->Localisation.Autoroute) && (pl_destination->destination.PR >= vm_noeud->Localisation_Tete.PR))
        {
          vl_destination = pl_pmv.Destination;
          pl_graphe_Destination = pl_pmv.Graphe_Destination;
          vl_trouve = XDC_VRAI;
        }
      }
      if ((pl_pmv.DestinationLointaine != 0) && (vl_trouve != XDC_VRAI))
      {
        if ((pl_destinationLointaine->destination.Autoroute == vm_noeud->Localisation.Autoroute) && (pl_destinationLointaine->destination.PR >= vm_noeud->Localisation_Tete.PR))
        {
          vl_destination = pl_pmv.DestinationLointaine;
          pl_graphe_Destination = pl_pmv.Graphe_DestinationLointaine;
          vl_trouve = XDC_VRAI;
        }
      }

      if ((pl_pmv.DestinationNoeud != 0) && (vl_trouve != XDC_VRAI))
      {
        if ((pl_destinationNoeud->destination.Autoroute == vm_noeud->Localisation.Autoroute) && (pl_destinationNoeud->destination.PR >= vm_noeud->Localisation_Tete.PR))
        {
          vl_destination = pl_pmv.DestinationNoeud;
          pl_graphe_Destination = pl_pmv.Graphe_DestinationNoeud;
          vl_trouve = XDC_VRAI;
        }
      }
    }
    XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_dest_ou_loc 1");

    /*	XZST_03EcritureTrace(XZSTC_WARNING,"avant avant XZAD23_Calculer_TdP_PMV_Dest sens%d %d %d %d %d %d",vm_noeud->Localisation.Sens,vl_destination,pl_pmv.DestinationLointaine,pl_destinationLointaine->destination.Numero,pl_destinationLointaine->destination.Autoroute,pl_destinationLointaine->destination.PR );

   XZST_03EcritureTrace(XZSTC_WARNING,"pl_pmv.DestinationLointaine=%d;pl_destinationLointaine->destination.Autoroute=%d; vm_noeud->Localisation.Autoroute=%d;pl_destinationLointaine->destination.PR=%d, vm_noeud->Localisation_Tete.PR=%d %d",pl_pmv.DestinationLointaine,pl_destinationLointaine->destination.Autoroute , vm_noeud->Localisation.Autoroute, pl_destinationLointaine->destination.PR , vm_noeud->Localisation_Tete.PR,vl_trouve);*/

    if (((vm_noeud->Localisation.Sens == XDC_SENS_NORD) && (vm_noeud->Localisation.Autoroute != 8)) ||
        ((vm_noeud->Localisation.Sens == XDC_SENS_SUD) && (vm_noeud->Localisation.Autoroute == 8)))
    {
      if (pl_pmv.Destination != 0)
      {
        if ((pl_destination->destination.Autoroute == vm_noeud->Localisation.Autoroute) && (pl_destination->destination.PR <= vm_noeud->Localisation_Tete.PR))
        {
          vl_destination = pl_pmv.Destination;
          pl_graphe_Destination = pl_pmv.Graphe_Destination;
          vl_trouve = XDC_VRAI;
        }
      }
      if ((pl_pmv.DestinationLointaine != 0) && (vl_trouve != XDC_VRAI))
      {
        if ((pl_destinationLointaine->destination.Autoroute == vm_noeud->Localisation.Autoroute) && (pl_destinationLointaine->destination.PR <= vm_noeud->Localisation_Tete.PR))
        {
          vl_destination = pl_pmv.DestinationLointaine;
          pl_graphe_Destination = pl_pmv.Graphe_DestinationLointaine;
          vl_trouve = XDC_VRAI;
        }
      }

      if ((pl_pmv.DestinationNoeud != 0) && (vl_trouve != XDC_VRAI))
      {
        if ((pl_destinationNoeud->destination.Autoroute == vm_noeud->Localisation.Autoroute) && (pl_destinationNoeud->destination.PR <= vm_noeud->Localisation_Tete.PR))
        {
          vl_destination = pl_pmv.DestinationNoeud;
          pl_graphe_Destination = pl_pmv.Graphe_DestinationNoeud;
          vl_trouve = XDC_VRAI;
        }
      }
    }

    /* pas de destination trouve , donc pas de TDP, donc affichage longueur + localisation
  XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmv_var_dest_ou_loc 2 trouve=%d",vl_trouve);
    if ( vl_trouve!=XDC_VRAI ) {
      vm_destination3_HS=TRUE;
      if ( vm_noeud->Localisation.Longueur > 3000)
      return (drol_pmv_var_localisation_longueur());
      else
      return (drol_pmv_var_localisation());
    }*/



  /*on calcule le tdp entre le PMV et cette destination*/
	XZST_03EcritureTrace(XZSTC_WARNING,"avant XZAD23_Calculer_TdP_PMV_Dest %d",vl_destination); 
  if (XZAD23_Calculer_TdP_PMV_Dest(     vm_pmv.PR,
                          vm_pmv.Autoroute,
                          vm_pmv.Sens,
                          vl_destination,
                          pl_graphe_Destination,
                          &vl_tdp, vl_nom,vm_pmv.Identifiant,XDC_EQT_PMV) != XDC_OK)	{
		vm_destination3_HS=TRUE;
		if ( vm_noeud->Localisation.Longueur > 3000) 
			return (drol_pmv_var_localisation_longueur());	
		else
			return (drol_pmv_var_localisation());	
		
  }
/*vl_tdp=0;*/
  	XZST_03EcritureTrace(XZSTC_WARNING,"TDP %d",vl_tdp); 
   if (vl_tdp<=0) {
		vm_destination3_HS=TRUE;
	/* Si pas de TDP alors localisation ou longueur si bouchon > 3km) */
		if ( vm_noeud->Localisation.Longueur > 3000) 
			return (drol_pmv_var_localisation_longueur());	
		else
			return (drol_pmv_var_localisation());	
		
   }
   
vm_destination3_HS=FALSE;

    /*TDP DOIT ETRE CADRE A DROITE*/
    if (XZAD27_Libelle_Destination(vl_destination, vl_libelle) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_Dest_ou_loc: pb dans l'appel a XZAD27_Libelle_Destination");
      vm_destination3_HS = TRUE;
      /* Si pas de TDP alors localisation ou longueur si bouchon > 3km) */
      if (vm_noeud->Localisation.Longueur > 3000)
        return (drol_pmv_var_localisation_longueur());
      else
        return (drol_pmv_var_localisation());
    }

    vm_destination3_numero = vl_destination;

    XZST_03EcritureTrace(XZSTC_WARNING, "%s MAX2 %d", vl_libelle, vm_destination3_numero);
    if (vl_tdp > 59)
    {
      sprintf(vl_str, "%s           ", vl_libelle);
      sprintf(&vl_str[11], "%1dh%02d", vl_tdp / 60, vl_tdp % 60);
    }
    else
    {
      sprintf(vl_str, "%s           ", vl_libelle);
      sprintf(&vl_str[11], "%2dmn", vl_tdp);
    }
    vm_destination3_HS = FALSE;
  }

  sprintf(vl_str2, "%s", drol_pmv_var_localisation());
  pl_ligne = strchr(vl_str2, '\n');
  XZST_03EcritureTrace(XZSTC_WARNING, "str2=%s %s", vl_str2, pl_ligne);
  if (pl_ligne != NULL)
  {
    vl_longueur = strlen(vl_str2);
    vl_str2[vl_longueur - strlen(pl_ligne)] = '\0';
    XZST_03EcritureTrace(XZSTC_WARNING, "lg=%d,str2=%s", vl_longueur, vl_str2);
  }
  sprintf(vl_str1, "%s\\%s", vl_str2, vl_str);
  XZST_03EcritureTrace(XZSTC_WARNING, "OUT:dest_loc vl_str1 = %s", vl_str1);
  return (vl_str1);
}

static char *drol_pmv_var_distance_bandeau()
{
  static char vl_str[20];
  /* REPERE3 */
  XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_distance_bandeau IN");
  /*if ((vm_noeud->Infos_TFM.Zone_Urbaine==0) && (vm_pmv.Distance_C < 15000))  {*/
  if (!vm_pmv.Presence_Bifurcation && (drol_pmv_sur_section_courante()))
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_distance_bandeau : sur section courante");
    if ((vm_noeud->Localisation.ZoneUrbaine == 0) && (vm_pmv.Distance_C < 15000))
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "pas zone urbaine & distance < 15 km => pas de distance bandeau");
      strcpy(vl_str, "");
      return (vl_str);
    }
  }

  if (vm_pmv.Distance_C > 500)
  {
    if (vm_pmv.Distance_C >= 100000)
      sprintf(vl_str, "a %dK", (int)(vm_pmv.Distance_C / 1000));
    else if ((vm_pmv.Distance_C % 1000) > 500)
      sprintf(vl_str, "a %dkm", (int)(vm_pmv.Distance_C / 1000) + 1);
    else
      sprintf(vl_str, "a %dkm", (int)(vm_pmv.Distance_C / 1000));
  }
  else
  {
    if ((vm_pmv.Distance_C >= 100))
    {
      if ((vm_pmv.Distance_C % 100) > 50)
        sprintf(vl_str, "a %dM", ((int)(vm_pmv.Distance_C / 100) + 1) * 100);
      else
        sprintf(vl_str, "a %dM", (int)(vm_pmv.Distance_C / 100) * 100);
    }
    else
      sprintf(vl_str, "a 50 m");
  }
  XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_distance_bandeau out %s", vl_str);
  return (vl_str);
}

static char *drol_pmv_var_cause()
{
  static char vl_str[20];

  XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_cause : type %d", vm_noeud->Infos_TFM.Type_Cause);
  strcpy(vl_str, "");
  switch (vm_noeud->Infos_TFM.Type_Cause)
  {
  case (XZAEC_FMC_MalaiseUsager):
  case (XZAEC_FMC_Accident):
    strcpy(vl_str, "ACCIDENT");
    break;
  case (XZAEC_FMC_VehMarcheArr):
  case (XZAEC_FMC_Contresens):
    strcpy(vl_str, "CONTRESENS");
    break;
  case (XZAEC_FMC_VehPanne):
  case (XZAEC_FMC_VehArrete):
    strcpy(vl_str, "VEH ARRETE");
    break;
  case (XZAEC_FMC_VehFeu):
    strcpy(vl_str, "VEH EN FEU");
    break;
  case (XZAEC_FMC_ConvoiExc):
  case (XZAEC_FMC_ConvoiMil):
  case (XZAEC_FMC_Cirque):
  case (XZAEC_FMC_VehLent):
    strcpy(vl_str, "CONVOI LENT");
    break;
  case (XZAEC_FMC_Manifestation):
  case (XZAEC_FMC_GreveESCOTA):
  case (XZAEC_FMC_Escargot):
    strcpy(vl_str, "MANIFESTATION");
    break;
  case (XZAEC_FMC_Travaux):
    strcpy(vl_str, "TRAVAUX");
    break;
  case (XZAEC_FMC_QueueBouchon):
    strcpy(vl_str, "BOUCHON");
    break;
  case (XZAEC_FMC_Eboulement):
    strcpy(vl_str, "EBOULEMENT");
    break;
  case (XZAEC_FMC_Incendie):
    strcpy(vl_str, "INCENDIE");
    break;
  case (XZAEC_FMC_Inondation):
    strcpy(vl_str, "INONDATION");
    break;
  case (XZAEC_FMC_Obstacle):
    strcpy(vl_str, "OBSTACLE");
    break;
  case (XZAEC_FMC_ChausseDeter):
    strcpy(vl_str, "CHAUSSEE ABIMEE");
    break;
  case (XZAEC_FMC_AnimalErrant):
    strcpy(vl_str, "ANIMAL ERRANT");
    break;
  case (XZAEC_FMC_AltercationUsager):
  case (XZAEC_FMC_HoldUpVol):
  case (XZAEC_FMC_Agression):
    strcpy(vl_str, "INCIDENT");
    break;
  case (XZAEC_FMC_PietonChaussee):
    strcpy(vl_str, "PIETONS");
    break;
  case (XZAEC_FMC_ChausseeGliss):
    strcpy(vl_str, "NETTOYAGE VOIES");
    break;
  default:
    strcpy(vl_str, "");
    break;
  }
  XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_cause : cause %s", vl_str);
  return (vl_str);
}
static char *drol_pmv_var_num_pt_car()
{
  T_Localisation vl_loc;
  T_Localisation vl_loc_tete;
  static char vl_str[20];

  vl_loc = vm_noeud->Localisation;
  vl_loc_tete = vm_noeud->Localisation_Tete;
  /* if ( (vl_loc.Sens==XDC_SENS_INCONNU) && (vm_pmv.Sens==XDC_SENS_SUD))
     sprintf(vl_str,"%s", vl_loc_tete.SortieAvalUsager);
   else
     sprintf(vl_str,"%s", vl_loc_tete.SortieAmontUsager);
   */

  if (vl_loc.TypePointCar == XDC_POINT_CARACT_LIT_ARRET)
    sprintf(vl_str, "%d", vl_loc.NumPointCar);
  else 
  /********************** SAE 598 if (vl_loc.Bifurcation == XDC_VRAI)
    sprintf(vl_str, "%s", vl_loc.AbrevPointCar);
  else
  ******************************************************************/
    sprintf(vl_str, "%s", vl_loc.SortieUsager);
  return (vl_str);
}

static char *drol_pmv_var_nom_pt_car()
{
  T_Localisation vl_loc;
  T_Localisation vl_loc_tete;
  static char vl_str[40];

  vl_loc = vm_noeud->Localisation;
  vl_loc_tete = vm_noeud->Localisation_Tete;
  /*if ( (vl_loc.Sens==XDC_SENS_INCONNU) && (vm_pmv.Sens==XDC_SENS_SUD))
    sprintf(vl_str,"%s", vl_loc_tete.AbrevSortieAval);
  else
    sprintf(vl_str,"%s", vl_loc_tete.AbrevSortieAmont);*/
  sprintf(vl_str, "%s", vl_loc.AbrevPointCar);
  XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_nom_pt_car = %s", vl_str);
  return (vl_str);
}

static char *drol_pmv_var_distance()
{
  static char vl_str[20];
  if (vm_pmv.Distance_C > 500)
    sprintf(vl_str, "%d km", (vm_pmv.Distance_C + 500) / 1000);
  else
  {
    if ((vm_pmv.Distance_C >= 100))
    {
      sprintf(vl_str, "%dM", (int)(vm_pmv.Distance_C / 100) * 100);
    }
    else
      sprintf(vl_str, "50 m");
  }

  return (vl_str);
}

static char *drol_pmv_var_distance_1000()
{
  static char vl_str[20];
  int vl_distance = vm_pmv.Distance_C - 1000;

  if (vl_distance > 500)
  {
    sprintf(vl_str, "%d km", (vl_distance + 500) / 1000);
  }
  else if (vl_distance >= 100)
  {
    sprintf(vl_str, "%dM", (int)(vl_distance / 100) * 100);
  }
  else
  {
    sprintf(vl_str, "50 m");
  }

  return (vl_str);
}

static char *drol_pmv_var_vitesse(int va_amont)
{
  static XDY_VitesseRegulTxt vl_vitesse;
  char vl_str[15] = "";

  /* amont= 1 => vitesse sur le PMV amont*/
  /* => = vitesse nominale de la zone - 20 km*/
  /* sinon vitesse sur le PRV en amont du PMV*/

  if (XZAO574_Vitesse_PRV_amont(vm_noeud->Infos_FAX.Numero_Zone,
                                vm_pmv.Autoroute,
                                vm_pmv.PR,
                                vm_pmv.Sens,
                                va_amont,
                                vl_vitesse) == XDC_OK)
    return (vl_vitesse);
  else
    return ("XXXX");
}

static char *drol_pmv_var_carburant()
{
  if (vm_noeud->Infos_FAX.Numero_Carburant == XDC_CRB_SUPER)
    return ("SUPER");
  else if (vm_noeud->Infos_FAX.Numero_Carburant == XDC_CRB_GAZOLE)
    return ("GAZOLE");
  else if (vm_noeud->Infos_FAX.Numero_Carburant == XDC_CRB_SSPB95)
    return ("SP95");
  else if (vm_noeud->Infos_FAX.Numero_Carburant == XDC_CRB_SSPB98)
    return ("SP98");
  else if (vm_noeud->Infos_FAX.Numero_Carburant == XDC_CRB_GPL)
    return ("GPL");
  else
    return ("\nCARBURANT");
}

static char *drol_pmv_var_autoroute_distance()
{
  static char vl_str[30];

  sprintf(vl_str, "%s A %s", drol_pmv_var_autoroute(), drol_pmv_var_distance());
  XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_autoroute_distance = %s", vl_str);
  return (vl_str);
}
static char *drol_pmv_var_autoroute()
{
  static char vl_str[30];

  sprintf(vl_str, "%s FERMEE", vm_noeud->Localisation.NomAutoroute);
  XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_var_autoroute = %s", vl_str);
  return (vl_str);
}

static char *drol_pmv_var_car()
{
  return drol_pmv_var_carburant();
}
static char *drol_pmv_var_null() { return (""); }

/**************************************/
/**************************************/
T_LIGNE_PMV drol_pmv_remplit_ligne(char *va_str)
{
  T_LIGNE_PMV vl_ligne;
  int vl_indice;

  vm_presence_nature_bouchon = FALSE;
  XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_remplit_ligne IN = %s", va_str);
  vl_indice = 0;
  strcpy(vl_ligne.Texte, "");
  while ((va_str[vl_indice] != C_CAR_DOLLAR) && vl_indice < strlen(va_str))
  {
    vl_ligne.Texte[vl_indice] = va_str[vl_indice];
    vl_indice++;
  }
  vl_ligne.Texte[vl_indice] = '\0';

  vl_ligne.Fct = drol_pmv_var_null;
  /*si on a trouve un DOLLAR*/
  if (va_str[vl_indice] == C_CAR_DOLLAR)
  {
    va_str = va_str + vl_indice + 1;
    XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_remplit_ligne va_str = %s", va_str);
    if (!strcmp(va_str, C_VAR_LONGUEUR))
    {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_remplit_ligne fct C_VAR_LONGUEUR");
      vm_presence_nature_bouchon = TRUE;
      vl_ligne.Fct = drol_pmv_var_longueur;
    }
    else if (!strcmp(va_str, C_VAR_TRAVERSEE))
    {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_remplit_ligne fct C_VAR_TRAVERSEE");
      vl_ligne.Fct = drol_pmv_var_traversee;
    }
    else if (!strcmp(va_str, C_VAR_AUTOROUTE_DISTANCE))
    {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_remplit_ligne fct C_VAR_AUTOROUTE_DISTANCE");
      vl_ligne.Fct = drol_pmv_var_autoroute_distance;
    }
    else if (!strcmp(va_str, C_VAR_LOCALISATION))
    {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_remplit_ligne fct C_VAR_LOCALISATION");
      vl_ligne.Fct = drol_pmv_var_localisation;
    }
    else if (!strcmp(va_str, C_VAR_DISTANCE_BANDEAU))
    {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_remplit_ligne fct C_VAR_DISTANCE_BANDEAU");
      vl_ligne.Fct = drol_pmv_var_distance_bandeau;
    }
    else if (!strcmp(va_str, C_VAR_NUM_PT_CAR))
    {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_remplit_ligne fct C_VAR_NUM_PT_CAR");
      vl_ligne.Fct = drol_pmv_var_num_pt_car;
    }
    else if (!strcmp(va_str, C_VAR_NOM_PT_CAR))
    {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_remplit_ligne fct C_VAR_NOM_PT_CAR");
      vl_ligne.Fct = drol_pmv_var_nom_pt_car;
    }
    else if (!strcmp(va_str, C_VAR_DISTANCE))
    {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_remplit_ligne fct C_VAR_DISTANCE");
      vl_ligne.Fct = drol_pmv_var_distance;
    }
    else if (!strcmp(va_str, C_VAR_DISTANCE_1000))
    {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_remplit_ligne fct C_VAR_DISTANCE_1000");
      vl_ligne.Fct = drol_pmv_var_distance_1000;
    }
    else if (!strcmp(va_str, C_VAR_CARBURANT))
    {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_remplit_ligne fct C_VAR_CARBURANT");
      vl_ligne.Fct = drol_pmv_var_carburant;
    }
    else if (!strcmp(va_str, C_VAR_CAR))
    {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_remplit_ligne fct C_VAR_CAR");
      vl_ligne.Fct = drol_pmv_var_car;
    }
    else if (!strcmp(va_str, C_VAR_AUTOROUTE))
    {
      XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_remplit_ligne fct C_VAR_AUTOROUTE");
      vl_ligne.Fct = drol_pmv_var_autoroute;
    }
    else if (!strcmp(va_str, C_VAR_DESTINATION1))
    {
      XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_remplit_ligne fct C_VAR_DESTINATION1");
      vl_ligne.Fct = drol_pmv_var_destination1;
    }
    else if (!strcmp(va_str, C_VAR_DESTINATION2))
    {
      XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_remplit_ligne fct C_VAR_DESTINATION2");
      vl_ligne.Fct = drol_pmv_var_destination2;
    }
    else if (!strcmp(va_str, C_VAR_DESTINATION3))
    {
      XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_remplit_ligne fct C_VAR_DESTINATION3");
      vl_ligne.Fct = drol_pmv_var_destination3;
    }
    else if (!strcmp(va_str, C_VAR_DEST_OU_LOC))
    {
      XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_remplit_ligne fct C_VAR_DEST_OU_LOC");
      vl_ligne.Fct = drol_pmv_var_dest_ou_loc;
    }
    else if (!strcmp(va_str, C_VAR_CAUSE))
    {
      XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_remplit_ligne fct C_VAR_CAUSE");
      vl_ligne.Fct = drol_pmv_var_cause;
    }
    else
    {
      XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_remplit_ligne fct NULL");
      vl_ligne.Fct = drol_pmv_var_null;
    }
  }
  return (vl_ligne);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des perturbations engendrees par l'evt
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen drol_pmv_filtre(T_NOEUD *va_noeud, XDY_District va_site, XDY_Horodate va_horodate)

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
  T_PERT *pl_cas;
  T_CONDITION *pl_cond;
  XDY_Booleen (*vl_fct)();
  XDY_Booleen vl_res = FALSE;
  XDY_Booleen vl_fax_a_emettre = FALSE;
  T_PERT *vl_pert;
  int vl_indice;


  /* Si FMC prevue alors pas d'action PMV !!!!!!!!!!!!!!!!!!!!!!*/
  /* DEM 628*/
  if (vm_noeud->Infos_TFM.Prevu)
    return (TRUE);

  /*remet a jour la liste des PMV*/
  pmv_init_vg_requetes();

  vm_noeud = va_noeud;
  vm_noeud2->Localisation = vm_noeud->Localisation;
  vm_noeud2->Infos_FAX = vm_noeud->Infos_FAX;
  vm_noeud2->Type = vm_noeud->Type;

  vl_pert = NULL;
  pl_cas = vg_pert;

  while (pl_cas != NULL)
  {
    XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmv_filtre %s", pl_cas->Libelle);
    pl_cond = pl_cas->Condition;
    vl_res = TRUE;

    while (pl_cond != NULL)
    {
      if (pl_cond->Type == C_TYPE_COMPARATEUR)
      {
        vl_fct = pl_cond->Fonction.Comp.Fct;
        vl_res = vl_res && (*vl_fct)(*(pl_cond->Fonction.Comp.Slot),
                                     pl_cond->Fonction.Comp.Valeur);

        if (!vl_res)
        {
          break;
        }
      }
      else
      {
        vl_fct = pl_cond->Fonction.Predicat.Fct;
        vl_res = vl_res && (*vl_fct)();
        if (!vl_res)
          break;
      }
      pl_cond = pl_cond->Suivant;
    }
    /*on a trouve une perturbation*/
    if (vl_res)
    {
      XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_filtre OK pour %s ",
                           pl_cas->Libelle);
      drol_pmv_recherche(pl_cas, va_horodate, va_site);
    }
    pl_cas = pl_cas->Suivant;
  }

  /*pour chacun des PMV recuperes*/
  for (vl_indice = 0; vl_indice < C_NB_MAX_PMV; vl_indice++)
  {
    /* si le PMV doit etre utilise*/
    if ((vg_pmv[vl_indice].Utilise) /* && (vg_pmv[vl_indice].Perturbation->Priorite<vg_pmv[vl_indice].Priorite_U)*/)
    {
      XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_filtre: Perturbation->Priorite=%d;Priorite_U=%d", vg_pmv[vl_indice].Perturbation->Priorite, vg_pmv[vl_indice].Priorite_U);
      XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_filtre: pert pmv %d, priorite %d",
                           vg_pmv[vl_indice].Perturbation->Priorite,
                           vg_priorite_retenue);
      vm_pmv = vg_pmv[vl_indice];
      drol_pmv_message(vg_pmv[vl_indice], vg_pmv[vl_indice].Perturbation, va_horodate, va_site, vm_noeud->Localisation.Sens);
    }
  }
  /*pour chacun des PMV SAGA recuperes*/
  XZST_03EcritureTrace(XZSTC_FONCTION, "PMV SAGA");
  for (vl_indice = 0; vl_indice < C_NB_MAX_PMV; vl_indice++)
  {
    /* si le PMV doit etre utilise*/
    if ((vg_pmv_saga[vl_indice].Utilise) /* && (vg_pmv_saga[vl_indice].Perturbation->Priorite<vg_pmv_saga[vl_indice].Priorite_U)*/)
    {
      XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_filtre: SAGA Perturbation->Priorite=%d;Priorite_U=%d", vg_pmv_saga[vl_indice].Perturbation->Priorite, vg_pmv_saga[vl_indice].Priorite_U);
      XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_filtre: SAGA pert pmv %d, priorite %d",
                           vg_pmv_saga[vl_indice].Perturbation->Priorite,
                           vg_priorite_retenue);
      vm_pmv = vg_pmv_saga[vl_indice];
      drol_pmv_message(vg_pmv_saga[vl_indice], vg_pmv_saga[vl_indice].Perturbation, va_horodate, va_site, vm_noeud->Localisation.Sens);
    }
  }
  return (TRUE);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des PMV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_pmv_recherche(T_PERT *va_pert,
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
  int vl_distance, vl_distanceb;
  XDY_Sens vl_Sens;
  XZST_03EcritureTrace(XZSTC_DEBUG1, "IN : drol_pmv_recherche");

  /*appel de la fonction de recherche du ou des PMV*/

  /*Cas Fermeture echangeur DEM 1066*/
  vl_Sens = vm_noeud->Localisation.Sens;
  if ((vl_Sens == XDC_SENS_INCONNU) && (vm_noeud->Type == XZAEC_FMC_FermetureEch))
  {
    if ((vm_noeud->Infos_FAX.Sortie1) && (vm_noeud->Infos_FAX.Sortie2))
      vl_Sens = XDC_SENS_INCONNU;
    else if ((vm_noeud->Infos_FAX.Sortie1) && !(vm_noeud->Infos_FAX.Sortie2))
      vl_Sens = XDC_SENS_SORTIE_SUD;
    else if (!(vm_noeud->Infos_FAX.Sortie1) && (vm_noeud->Infos_FAX.Sortie2))
      vl_Sens = XDC_SENS_SORTIE_NORD;
  }

  if ((vm_noeud->Type == XZAEC_FMC_AnimalErrant) ||
      (vm_noeud->Type == XZAEC_FMC_Meteo) ||
      (vm_noeud->Type == XZAEC_FMC_PietonChaussee) ||
      (vm_noeud->Type == XZAEC_FMC_Contresens))
    vl_Sens = XDC_SENS_INCONNU;

  vl_distance = va_pert->DMax1;
  vl_distanceb = va_pert->DMax1_Bifurc;
  if (vm_noeud->Localisation.ZoneUrbaine == 1)
  {
    if (va_pert->DMax1 == 2000)
      vl_distance = 1000;
    if (va_pert->DMax1_Bifurc == 2000)
      vl_distanceb = 1000;
  }

  if (!strcmp(va_pert->Pmv1, "le deuxieme PMV en amont"))
    le_second_PMV_amont(vm_noeud->Localisation.Autoroute,
                        vm_noeud->Localisation.PR,
                        vm_noeud->Localisation_Tete.PR,
                        vl_Sens,
                        va_horodate,
                        va_site,
                        va_pert->Priorite,
                        va_pert->Libelle,
                        vl_distance,
                        vl_distanceb,
                        vm_noeud->Localisation.PRBifAmont,
                        vm_noeud->Localisation.PRBifAval,
                        vm_noeud->Localisation_Tete.PRBifAmont,
                        vm_noeud->Localisation_Tete.PRBifAval,
                        vm_noeud->Localisation.AutorouteBifAmont,
                        vm_noeud->Localisation.AutorouteBifAval,
                        vm_noeud->Localisation_Tete.AutorouteBifAmont,
                        vm_noeud->Localisation_Tete.AutorouteBifAval,
                        vm_noeud->Localisation.PRDebutBifAmont,
                        vm_noeud->Localisation.PRDebutBifAval,
                        vm_noeud->Localisation_Tete.PRDebutBifAmont,
                        vm_noeud->Localisation_Tete.PRDebutBifAval,
                        va_pert);

  else if (!strcmp(va_pert->Pmv1, "le premier PMV en amont"))
    le_premier_PMV_amont(vm_noeud->Localisation.Autoroute,
                         vm_noeud->Localisation.PR,
                         vm_noeud->Localisation_Tete.PR,
                         vl_Sens,
                         va_horodate,
                         va_site,
                         va_pert->Priorite,
                         va_pert->Libelle,
                         vl_distance,
                         vl_distanceb,
                         vm_noeud->Localisation.PRBifAmont,
                         vm_noeud->Localisation.PRBifAval,
                         vm_noeud->Localisation_Tete.PRBifAmont,
                         vm_noeud->Localisation_Tete.PRBifAval,
                         vm_noeud->Localisation.AutorouteBifAmont,
                         vm_noeud->Localisation.AutorouteBifAval,
                         vm_noeud->Localisation_Tete.AutorouteBifAmont,
                         vm_noeud->Localisation_Tete.AutorouteBifAval,
                         vm_noeud->Localisation.PRDebutBifAmont,
                         vm_noeud->Localisation.PRDebutBifAval,
                         vm_noeud->Localisation_Tete.PRDebutBifAmont,
                         vm_noeud->Localisation_Tete.PRDebutBifAval,
                         va_pert);

  else if (!strcmp(va_pert->Pmv1, "le PMV en amont de l'aire precedente"))
    le_PMV_amont_aire(vm_noeud->Localisation.Autoroute,
                      vm_noeud->Localisation.PR,
                      vm_noeud->Localisation_Tete.PR,
                      vl_Sens,
                      va_horodate,
                      va_site,
                      va_pert->Priorite,
                      va_pert->Libelle,
                      vl_distance,
                      vl_distanceb,
                      vm_noeud->Localisation.PRBifAmont,
                      vm_noeud->Localisation.PRBifAval,
                      vm_noeud->Localisation_Tete.PRBifAmont,
                      vm_noeud->Localisation_Tete.PRBifAval,
                      vm_noeud->Localisation.AutorouteBifAmont,
                      vm_noeud->Localisation.AutorouteBifAval,
                      vm_noeud->Localisation_Tete.AutorouteBifAmont,
                      vm_noeud->Localisation_Tete.AutorouteBifAval,
                      vm_noeud->Localisation.PRDebutBifAmont,
                      vm_noeud->Localisation.PRDebutBifAval,
                      vm_noeud->Localisation_Tete.PRDebutBifAmont,
                      vm_noeud->Localisation_Tete.PRDebutBifAval,
                      va_pert);

  else if (!strcmp(va_pert->Pmv1, "les PMV dans la perturbation"))
    les_PMV_ds_perturbation(vm_noeud->Localisation.Autoroute,
                            vm_noeud->Localisation.PR,
                            vm_noeud->Localisation_Tete.PR,
                            vl_Sens,
                            va_horodate,
                            va_site,
                            va_pert->Priorite,
                            va_pert->Libelle,
                            vl_distance,
                            vl_distanceb,
                            vm_noeud->Localisation.PRBifAmont,
                            vm_noeud->Localisation.PRBifAval,
                            vm_noeud->Localisation_Tete.PRBifAmont,
                            vm_noeud->Localisation_Tete.PRBifAval,
                            vm_noeud->Localisation.AutorouteBifAmont,
                            vm_noeud->Localisation.AutorouteBifAval,
                            vm_noeud->Localisation_Tete.AutorouteBifAmont,
                            vm_noeud->Localisation_Tete.AutorouteBifAval,
                            vm_noeud->Localisation.PRDebutBifAmont,
                            vm_noeud->Localisation.PRDebutBifAval,
                            vm_noeud->Localisation_Tete.PRDebutBifAmont,
                            vm_noeud->Localisation_Tete.PRDebutBifAval,
                            va_pert);

  else if (!strcmp(va_pert->Pmv1, "le PMV en amont de la sortie precedente"))
    le_PMV_amont_sortie(vm_noeud->Localisation.Autoroute,
                        vm_noeud->Localisation.PR,
                        vm_noeud->Localisation_Tete.PR,
                        vl_Sens,
                        va_horodate,
                        va_site,
                        va_pert->Priorite,
                        va_pert->Libelle,
                        vl_distance,
                        vl_distanceb,
                        vm_noeud->Localisation.PRBifAmont,
                        vm_noeud->Localisation.PRBifAval,
                        vm_noeud->Localisation_Tete.PRBifAmont,
                        vm_noeud->Localisation_Tete.PRBifAval,
                        vm_noeud->Localisation.AutorouteBifAmont,
                        vm_noeud->Localisation.AutorouteBifAval,
                        vm_noeud->Localisation_Tete.AutorouteBifAmont,
                        vm_noeud->Localisation_Tete.AutorouteBifAval,
                        vm_noeud->Localisation.PRDebutBifAmont,
                        vm_noeud->Localisation.PRDebutBifAval,
                        vm_noeud->Localisation_Tete.PRDebutBifAmont,
                        vm_noeud->Localisation_Tete.PRDebutBifAval,
                        va_pert);

  else if (!strcmp(va_pert->Pmv1, "les PMV en amont"))
    tous_les_PMV_amont(vm_noeud->Localisation.Autoroute,
                       vm_noeud->Localisation.PR,
                       vm_noeud->Localisation_Tete.PR,
                       vl_Sens,
                       va_horodate,
                       va_site,
                       va_pert->Priorite,
                       va_pert->Libelle,
                       vl_distance,
                       vl_distanceb,
                       vm_noeud->Localisation.PRBifAmont,
                       vm_noeud->Localisation.PRBifAval,
                       vm_noeud->Localisation_Tete.PRBifAmont,
                       vm_noeud->Localisation_Tete.PRBifAval,
                       vm_noeud->Localisation.AutorouteBifAmont,
                       vm_noeud->Localisation.AutorouteBifAval,
                       vm_noeud->Localisation_Tete.AutorouteBifAmont,
                       vm_noeud->Localisation_Tete.AutorouteBifAval,
                       vm_noeud->Localisation.PRDebutBifAmont,
                       vm_noeud->Localisation.PRDebutBifAval,
                       vm_noeud->Localisation_Tete.PRDebutBifAmont,
                       vm_noeud->Localisation_Tete.PRDebutBifAval,
                       va_pert);

  else if (!strcmp(va_pert->Pmv1, "les 2 PMV eligibles en amont"))
    les_2_PMV_eligibles_en_amont(vm_noeud->Localisation.Autoroute,
                                 vm_noeud->Localisation.PR,
                                 vm_noeud->Localisation_Tete.PR,
                                 vl_Sens,
                                 va_horodate,
                                 va_site,
                                 va_pert->Priorite,
                                 va_pert->Libelle,
                                 vl_distance,
                                 vl_distanceb,
                                 vm_noeud->Localisation.PRBifAmont,
                                 vm_noeud->Localisation.PRBifAval,
                                 vm_noeud->Localisation_Tete.PRBifAmont,
                                 vm_noeud->Localisation_Tete.PRBifAval,
                                 vm_noeud->Localisation.AutorouteBifAmont,
                                 vm_noeud->Localisation.AutorouteBifAval,
                                 vm_noeud->Localisation_Tete.AutorouteBifAmont,
                                 vm_noeud->Localisation_Tete.AutorouteBifAval,
                                 vm_noeud->Localisation.PRDebutBifAmont,
                                 vm_noeud->Localisation.PRDebutBifAval,
                                 vm_noeud->Localisation_Tete.PRDebutBifAmont,
                                 vm_noeud->Localisation_Tete.PRDebutBifAval,
                                 va_pert);

  else if (!strcmp(va_pert->Pmv1, "le PMV sur la chaussee opposee"))
    le_PMV_sur_la_chaussee_opposee(vm_noeud->Localisation.Autoroute,
                                   vm_noeud->Localisation.PR,
                                   vm_noeud->Localisation_Tete.PR,
                                   vl_Sens,
                                   va_horodate,
                                   va_site,
                                   va_pert->Priorite,
                                   va_pert->Libelle,
                                   vl_distance,
                                   vl_distanceb,
                                   vm_noeud->Localisation.PRBifAmont,
                                   vm_noeud->Localisation.PRBifAval,
                                   vm_noeud->Localisation_Tete.PRBifAmont,
                                   vm_noeud->Localisation_Tete.PRBifAval,
                                   vm_noeud->Localisation.AutorouteBifAmont,
                                   vm_noeud->Localisation.AutorouteBifAval,
                                   vm_noeud->Localisation_Tete.AutorouteBifAmont,
                                   vm_noeud->Localisation_Tete.AutorouteBifAval,
                                   vm_noeud->Localisation.PRDebutBifAmont,
                                   vm_noeud->Localisation.PRDebutBifAval,
                                   vm_noeud->Localisation_Tete.PRDebutBifAmont,
                                   vm_noeud->Localisation_Tete.PRDebutBifAval,
                                   va_pert);

  else
  {
    XZST_03EcritureTrace(XZSTC_INFO, "drol_pmv_charge : pmv1 = pas de fonction");
  }

  if (!strcmp(va_pert->Pmv2, "le deuxieme PMV en amont"))
    le_second_PMV_amont(vm_noeud->Localisation.Autoroute,
                        vm_noeud->Localisation.PR,
                        vm_noeud->Localisation_Tete.PR,
                        vl_Sens,
                        va_horodate,
                        va_site,
                        va_pert->Priorite,
                        va_pert->Libelle,
                        vl_distance,
                        vl_distanceb,
                        vm_noeud->Localisation.PRBifAmont,
                        vm_noeud->Localisation.PRBifAval,
                        vm_noeud->Localisation_Tete.PRBifAmont,
                        vm_noeud->Localisation_Tete.PRBifAval,
                        vm_noeud->Localisation.AutorouteBifAmont,
                        vm_noeud->Localisation.AutorouteBifAval,
                        vm_noeud->Localisation_Tete.AutorouteBifAmont,
                        vm_noeud->Localisation_Tete.AutorouteBifAval,
                        vm_noeud->Localisation.PRDebutBifAmont,
                        vm_noeud->Localisation.PRDebutBifAval,
                        vm_noeud->Localisation_Tete.PRDebutBifAmont,
                        vm_noeud->Localisation_Tete.PRDebutBifAval,
                        va_pert);

  else if (!strcmp(va_pert->Pmv2, "le premier PMV en amont"))
    le_premier_PMV_amont(vm_noeud->Localisation.Autoroute,
                         vm_noeud->Localisation.PR,
                         vm_noeud->Localisation_Tete.PR,
                         vl_Sens,
                         va_horodate,
                         va_site,
                         va_pert->Priorite,
                         va_pert->Libelle,
                         vl_distance,
                         vl_distanceb,
                         vm_noeud->Localisation.PRBifAmont,
                         vm_noeud->Localisation.PRBifAval,
                         vm_noeud->Localisation_Tete.PRBifAmont,
                         vm_noeud->Localisation_Tete.PRBifAval,
                         vm_noeud->Localisation.AutorouteBifAmont,
                         vm_noeud->Localisation.AutorouteBifAval,
                         vm_noeud->Localisation_Tete.AutorouteBifAmont,
                         vm_noeud->Localisation_Tete.AutorouteBifAval,
                         vm_noeud->Localisation.PRDebutBifAmont,
                         vm_noeud->Localisation.PRDebutBifAval,
                         vm_noeud->Localisation_Tete.PRDebutBifAmont,
                         vm_noeud->Localisation_Tete.PRDebutBifAval,
                         va_pert);

  else if (!strcmp(va_pert->Pmv2, "le PMV en amont de l'aire precedente"))
    le_PMV_amont_aire(vm_noeud->Localisation.Autoroute,
                      vm_noeud->Localisation.PR,
                      vm_noeud->Localisation_Tete.PR,
                      vl_Sens,
                      va_horodate,
                      va_site,
                      va_pert->Priorite,
                      va_pert->Libelle,
                      vl_distance,
                      vl_distanceb,
                      vm_noeud->Localisation.PRBifAmont,
                      vm_noeud->Localisation.PRBifAval,
                      vm_noeud->Localisation_Tete.PRBifAmont,
                      vm_noeud->Localisation_Tete.PRBifAval,
                      vm_noeud->Localisation.AutorouteBifAmont,
                      vm_noeud->Localisation.AutorouteBifAval,
                      vm_noeud->Localisation_Tete.AutorouteBifAmont,
                      vm_noeud->Localisation_Tete.AutorouteBifAval,
                      vm_noeud->Localisation.PRDebutBifAmont,
                      vm_noeud->Localisation.PRDebutBifAval,
                      vm_noeud->Localisation_Tete.PRDebutBifAmont,
                      vm_noeud->Localisation_Tete.PRDebutBifAval,
                      va_pert);

  else if (!strcmp(va_pert->Pmv2, "les PMV dans la perturbation"))
    les_PMV_ds_perturbation(vm_noeud->Localisation.Autoroute,
                            vm_noeud->Localisation.PR,
                            vm_noeud->Localisation_Tete.PR,
                            vl_Sens,
                            va_horodate,
                            va_site,
                            va_pert->Priorite,
                            va_pert->Libelle,
                            vl_distance,
                            vl_distanceb,
                            vm_noeud->Localisation.PRBifAmont,
                            vm_noeud->Localisation.PRBifAval,
                            vm_noeud->Localisation_Tete.PRBifAmont,
                            vm_noeud->Localisation_Tete.PRBifAval,
                            vm_noeud->Localisation.AutorouteBifAmont,
                            vm_noeud->Localisation.AutorouteBifAval,
                            vm_noeud->Localisation_Tete.AutorouteBifAmont,
                            vm_noeud->Localisation_Tete.AutorouteBifAval,
                            vm_noeud->Localisation.PRDebutBifAmont,
                            vm_noeud->Localisation.PRDebutBifAval,
                            vm_noeud->Localisation_Tete.PRDebutBifAmont,
                            vm_noeud->Localisation_Tete.PRDebutBifAval,
                            va_pert);

  else if (!strcmp(va_pert->Pmv2, "le PMV en amont de la sortie precedente"))
    le_PMV_amont_sortie(vm_noeud->Localisation.Autoroute,
                        vm_noeud->Localisation.PR,
                        vm_noeud->Localisation_Tete.PR,
                        vl_Sens,
                        va_horodate,
                        va_site,
                        va_pert->Priorite,
                        va_pert->Libelle,
                        vl_distance,
                        vl_distanceb,
                        vm_noeud->Localisation.PRBifAmont,
                        vm_noeud->Localisation.PRBifAval,
                        vm_noeud->Localisation_Tete.PRBifAmont,
                        vm_noeud->Localisation_Tete.PRBifAval,
                        vm_noeud->Localisation.AutorouteBifAmont,
                        vm_noeud->Localisation.AutorouteBifAval,
                        vm_noeud->Localisation_Tete.AutorouteBifAmont,
                        vm_noeud->Localisation_Tete.AutorouteBifAval,
                        vm_noeud->Localisation.PRDebutBifAmont,
                        vm_noeud->Localisation.PRDebutBifAval,
                        vm_noeud->Localisation_Tete.PRDebutBifAmont,
                        vm_noeud->Localisation_Tete.PRDebutBifAval,
                        va_pert);

  else if (!strcmp(va_pert->Pmv2, "les PMV en amont"))
    tous_les_PMV_amont(vm_noeud->Localisation.Autoroute,
                       vm_noeud->Localisation.PR,
                       vm_noeud->Localisation_Tete.PR,
                       vl_Sens,
                       va_horodate,
                       va_site,
                       va_pert->Priorite,
                       va_pert->Libelle,
                       vl_distance,
                       vl_distanceb,
                       vm_noeud->Localisation.PRBifAmont,
                       vm_noeud->Localisation.PRBifAval,
                       vm_noeud->Localisation_Tete.PRBifAmont,
                       vm_noeud->Localisation_Tete.PRBifAval,
                       vm_noeud->Localisation.AutorouteBifAmont,
                       vm_noeud->Localisation.AutorouteBifAval,
                       vm_noeud->Localisation_Tete.AutorouteBifAmont,
                       vm_noeud->Localisation_Tete.AutorouteBifAval,
                       vm_noeud->Localisation.PRDebutBifAmont,
                       vm_noeud->Localisation.PRDebutBifAval,
                       vm_noeud->Localisation_Tete.PRDebutBifAmont,
                       vm_noeud->Localisation_Tete.PRDebutBifAval,
                       va_pert);

  else if (!strcmp(va_pert->Pmv2, "les 2 PMV eligibles en amont"))
    les_2_PMV_eligibles_en_amont(vm_noeud->Localisation.Autoroute,
                                 vm_noeud->Localisation.PR,
                                 vm_noeud->Localisation_Tete.PR,
                                 vl_Sens,
                                 va_horodate,
                                 va_site,
                                 va_pert->Priorite,
                                 va_pert->Libelle,
                                 vl_distance,
                                 vl_distanceb,
                                 vm_noeud->Localisation.PRBifAmont,
                                 vm_noeud->Localisation.PRBifAval,
                                 vm_noeud->Localisation_Tete.PRBifAmont,
                                 vm_noeud->Localisation_Tete.PRBifAval,
                                 vm_noeud->Localisation.AutorouteBifAmont,
                                 vm_noeud->Localisation.AutorouteBifAval,
                                 vm_noeud->Localisation_Tete.AutorouteBifAmont,
                                 vm_noeud->Localisation_Tete.AutorouteBifAval,
                                 vm_noeud->Localisation.PRDebutBifAmont,
                                 vm_noeud->Localisation.PRDebutBifAval,
                                 vm_noeud->Localisation_Tete.PRDebutBifAmont,
                                 vm_noeud->Localisation_Tete.PRDebutBifAval,
                                 va_pert);

  else if (!strcmp(va_pert->Pmv2, "le PMV sur la chaussee opposee"))
    le_PMV_sur_la_chaussee_opposee(vm_noeud->Localisation.Autoroute,
                                   vm_noeud->Localisation.PR,
                                   vm_noeud->Localisation_Tete.PR,
                                   vl_Sens,
                                   va_horodate,
                                   va_site,
                                   va_pert->Priorite,
                                   va_pert->Libelle,
                                   vl_distance,
                                   vl_distanceb,
                                   vm_noeud->Localisation.PRBifAmont,
                                   vm_noeud->Localisation.PRBifAval,
                                   vm_noeud->Localisation_Tete.PRBifAmont,
                                   vm_noeud->Localisation_Tete.PRBifAval,
                                   vm_noeud->Localisation.AutorouteBifAmont,
                                   vm_noeud->Localisation.AutorouteBifAval,
                                   vm_noeud->Localisation_Tete.AutorouteBifAmont,
                                   vm_noeud->Localisation_Tete.AutorouteBifAval,
                                   vm_noeud->Localisation.PRDebutBifAmont,
                                   vm_noeud->Localisation.PRDebutBifAval,
                                   vm_noeud->Localisation_Tete.PRDebutBifAmont,
                                   vm_noeud->Localisation_Tete.PRDebutBifAval,
                                   va_pert);

  else
  {
    XZST_03EcritureTrace(XZSTC_INFO, "drol_pmv_recherche : pmv2 = pas de fonction");
  }

  return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de calcul du message a afficher sur le PMV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void drol_pmv_message(T_PMV va_pmv, T_PERT *va_pert, XDY_Horodate va_horodate, XDY_District va_site, XDY_Sens va_Sens)

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
  T_LIGNE_PMV vl_l1, vl_l2, vl_l3;
  T_LIGNE_PMV vl_l1_tdp, vl_l2_tdp, vl_l3_tdp;
  char vl_t1[100];
  int vl_clignotement;
  char vl_t2[100];
  char vl_t3[100];
  char vl_texte[1000];
  char Texte1PMV[21];
  char Texte2PMV[21];
  char Texte3PMV[21];
  char vl_bandeau[50];
  char vl_picto[10];
  char vl_explication[100];
  int vl_mode_automatique = 0;
  char vl_dg[50];
  int vl_Sens = 0;
  char *vl_str;
  XDY_Booleen vl_flash;

  XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_message : bifurc %d, distance fin %d",
                       va_pmv.Presence_Bifurcation,
                       va_pmv.Distance_C);

  /*si presence bifurcation*/
  if (va_pmv.Presence_Bifurcation)
  {
    /*si moins de 1000 m*/
    if ((va_pmv.Distance_C <= C_PMV_DMAX_FIN) &&
        !va_pert->B_Fin_Vide)
    {
      vl_l1 = va_pert->Bifurc1_Fin;
      vl_l2 = va_pert->Bifurc2_Fin;
      vl_l3 = va_pert->Bifurc3_Fin;
    }
    else
    {
      vl_l1 = va_pert->Bifurc1;
      vl_l2 = va_pert->Bifurc2;
      vl_l3 = va_pert->Bifurc3;
    }
  }
  else
  {
    /*si moins de 1000 m*/
    if ((va_pmv.Distance_C <= C_PMV_DMAX_FIN /*C_PMV_EPSILON*/) &&
        !va_pert->L_Fin_Vide)
    {
      vl_l1 = va_pert->Ligne1_Fin;
      vl_l2 = va_pert->Ligne2_Fin;
      vl_l3 = va_pert->Ligne3_Fin;
    }
    else
    {
      vl_l1 = va_pert->Ligne1;
      vl_l2 = va_pert->Ligne2;
      vl_l3 = va_pert->Ligne3;
    }
  }

  XZST_03EcritureTrace(XZSTC_FONCTION, "LIGNE %d carac %d pour le type %d",
                       vg_type_pmv[va_pmv.Type].NombreLignes,
                       vg_type_pmv[va_pmv.Type].NombreCaracteres,
                       va_pmv.Type);
  
  
	if ((vg_type_pmv[va_pmv.Type].NombreLignes == 1) &&
      		(vg_type_pmv[va_pmv.Type].NombreCaracteres == 15))
  	{
		vl_l1 = va_pert->Ligne1_1_15;
		vl_l2 = drol_pmv_remplit_ligne("");
		vl_l3 = drol_pmv_remplit_ligne("");
	} 
	else if ((vg_type_pmv[va_pmv.Type].NombreLignes == 2) &&
			(vg_type_pmv[va_pmv.Type].NombreCaracteres == 15))
	{
		vl_l1 = va_pert->Ligne1_2_15;
		vl_l2 = va_pert->Ligne2_2_15;
		vl_l3 = drol_pmv_remplit_ligne("");
	}

  /*si PMV SAGA*/
  /*
  if (va_pmv.PMV_SAGA)
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, "traitement PMV SAGA %d", va_pmv.Identifiant);
    if (va_pmv.Type_SAGA == XDC_SAGA_PMV_1_18)
    {
      XZST_03EcritureTrace(XZSTC_FONCTION, "XDC_SAGA_PMV_1_18");
      vl_l1 = va_pert->Ligne1_1_18;
      vl_l2 = drol_pmv_remplit_ligne("");
      vl_l3 = drol_pmv_remplit_ligne("");
    }
    else if (va_pmv.Type_SAGA == XDC_SAGA_PMV_2_18)
    {
      XZST_03EcritureTrace(XZSTC_FONCTION, "XDC_SAGA_PMV_2_18");
      vl_l1 = va_pert->Ligne1_2_18;
      vl_l2 = va_pert->Ligne2_2_18;
      vl_l3 = drol_pmv_remplit_ligne("");
    }
    else if (va_pmv.Type_SAGA == XDC_SAGA_PMV_2_18_P)
    {
      XZST_03EcritureTrace(XZSTC_FONCTION, "XDC_SAGA_PMV_2_18_P");
      vl_l1 = va_pert->Ligne1_2_18;
      vl_l2 = va_pert->Ligne2_2_18;
      vl_l3 = drol_pmv_remplit_ligne("");
    }
    else if (va_pmv.Type_SAGA == XDC_SAGA_PMV_2_15)
    {
      XZST_03EcritureTrace(XZSTC_FONCTION, "XDC_SAGA_PMV_2_15");
      vl_l1 = va_pert->Ligne1_2_15;
      vl_l2 = va_pert->Ligne2_2_15;
      vl_l3 = drol_pmv_remplit_ligne("");
    }
  }
  */

  /*si presence de bifurcation*/
  vm_ligne_courante = 1;
  if ((va_pmv.Presence_Bifurcation) &&
      (!(va_pmv.PMV_SAGA) || va_pmv.Type_SAGA == XDC_SAGA_PMV_3_15_P || va_pmv.Type_SAGA == XDC_SAGA_PMV_3_15))
  {
    sprintf(vl_texte, "%s\\", va_pmv.Direction);
  }
  else
    strcpy(vl_texte, "");

  /*calcul des elements de message*/
  vm_presence_nature_bouchon = FALSE;
  /*si perturbation avec TDP (?) */
  if (!strcmp(va_pert->Libelle, "PMV dans bouchon"))
  {
    /* calculer le message TDP avec 3 destinations L1 L2 et L3*/
    vl_l1_tdp.Texte[0] = '\0';
    strcpy(vl_l2_tdp.Texte, "\\");
    vl_l2_tdp.Texte[0] = '\0';
    vl_l3_tdp.Texte[0] = '\0';

    XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_message : TDP JMG");
    vl_l1_tdp.Fct = drol_pmv_var_destination1;
    vl_l2_tdp.Fct = drol_pmv_var_destination2;
    vl_l3_tdp.Fct = drol_pmv_var_destination3;
    strcpy(vl_t1, drol_pmv_calcul_ligne_minuscules(vl_l1_tdp));
    vm_ligne_courante++;
    strcpy(vl_t2, drol_pmv_calcul_ligne_minuscules(vl_l2_tdp));
    vm_ligne_courante++;
    strcpy(vl_t3, drol_pmv_calcul_ligne_minuscules(vl_l3_tdp));

    /* si vide, basculer sur le message de la perturbation*/
    if ((vm_destination1_numero == 0) && (vm_destination2_numero == 0) && (vm_destination3_numero == 0))
    {
      vm_ligne_courante = 1;
      strcpy(vl_t1, drol_pmv_calcul_ligne(vl_l1));
      vm_ligne_courante++;
      strcpy(vl_t2, drol_pmv_calcul_ligne(vl_l2));
      vm_ligne_courante++;
      strcpy(vl_t3, drol_pmv_calcul_ligne(vl_l3));
    }
  }
  else
  {
    strcpy(vl_t1, drol_pmv_calcul_ligne(vl_l1));
    vm_ligne_courante++;
    strcpy(vl_t2, drol_pmv_calcul_ligne(vl_l2));
    vm_ligne_courante++;
    strcpy(vl_t3, drol_pmv_calcul_ligne(vl_l3));
  }
  /*XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmv_message=%s/%s/%s nature=%d apres=%d ind=%d",vl_t1,vl_t2,vl_t3,vm_presence_nature_bouchon,vm_bouchon_apres_sortie,vm_indice_bouchon);*/

  /*JMG A FAIRE traiter le cas bouchon apres sortie*/
  /*pour ne pas avoir BOUCHON DE X KM A N KM*/
  if ((vm_presence_nature_bouchon) && !(vm_bouchon_apres_sortie))
  {
    if (vm_indice_bouchon == 1)
      strcpy(vl_t1, "BOUCHON");
    else if (vm_indice_bouchon == 2)
      strcpy(vl_t2, "BOUCHON");
    else if (vm_indice_bouchon == 3)
      strcpy(vl_t3, "BOUCHON");
  }
  /*concatenation des elements de message*/
  if (((vm_destination1_numero == 0) && (vm_destination2_numero == 0)) ||
      (vm_destination1_numero == XDC_DESTINATION_TRAVERSEE))
  {
    if (strcmp(vl_t1, ""))
      sprintf(vl_texte, "%s%s", vl_texte, vl_t1);
    if (strcmp(vl_t2, ""))
      sprintf(vl_texte, "%s\\%s", vl_texte, vl_t2);
    if (strcmp(vl_t3, ""))
      sprintf(vl_texte, "%s\\%s", vl_texte, vl_t3);
    strcpy(vl_texte, pmv_mise_en_page(vl_texte, C_NB_CAR_LIGNE));
  }

  if (va_Sens > 10)
    vl_Sens = va_Sens - 10;
  else
    vl_Sens = va_Sens;

  XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_message :m_destination3_numero=%d;t1/t2/t3=%s/%s/%s; message = %s pmv:%d %d-%d", vm_destination3_numero, vl_t1, vl_t2, vl_t3, vl_texte, va_pmv.Identifiant, va_pmv.Sens, vl_Sens);

  /*choix du pictogramme*/
  /*1.4*/


  /*texte sur le bandeau*/
  /*1.4*/
  if (va_pmv.Presence_Bifurcation)
    strcpy(vl_bandeau, drol_pmv_calcul_ligne(va_pert->Bandeau_Bifurcation));
  else
    strcpy(vl_bandeau, drol_pmv_calcul_ligne(va_pert->Bandeau));

  /*si bandeau contient km et message contient A %d km*/
  /* alors vider le bandeau*/
  if (va_pmv.Presence_Bifurcation) /*ne pas regarder la ligne 3 car elle ne sera pas visible*/
  {
    if ((strstr(vl_bandeau, "km") != NULL) &&
        (((strstr(vl_t1, "km") != NULL) && (strstr(vl_t1, "a ") != NULL)) ||
         ((strstr(vl_t2, "km") != NULL) && (strstr(vl_t2, "a ") != NULL))))
      strcpy(vl_bandeau, "");
  }
  else
  {
    if ((strstr(vl_bandeau, "km") != NULL) &&
        (((strstr(vl_t1, "km") != NULL) && (strstr(vl_t1, "a ") != NULL)) ||
         ((strstr(vl_t2, "km") != NULL) && (strstr(vl_t2, "a ") != NULL)) ||
         ((strstr(vl_t3, "km") != NULL) && (strstr(vl_t3, "a ") != NULL))))
      strcpy(vl_bandeau, "");
  }
  /*DEM915*/
  if (va_pmv.Presence_Bifurcation)
    strcpy(vl_picto, drol_pmv_calcul_picto(va_pmv.Perturbation->Picto_Bifurcation, (va_pmv.Sens == vl_Sens)));
  else
    strcpy(vl_picto, drol_pmv_calcul_picto(va_pmv.Perturbation->Picto, (va_pmv.Sens == vl_Sens)));

  /*PATCH PB VITESSE 130 VITESSE LIMITEE*/
  if (!strcmp(vl_picto, "L130"))
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_message : vitesse 130 ne rien afficher sur le PMV");
    vm_presence_nature_bouchon = FALSE;
    vm_bouchon_apres_sortie = TRUE;
    vm_destination1_numero = 0;
    vm_destination2_numero = 0;
    vm_destination3_numero = 0;
    return;
  }

  XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_message : avant comparaison");
  /*si distance deja affiche a la fois dans message et sur bandeau*/
  if ((((vl_t1[0] == 'A') && (vl_t1[1] == ' ')) ||
       ((vl_t2[0] == 'A') && (vl_t2[1] == ' ')) ||
       ((vl_t3[0] == 'A') && (vl_t3[1] == ' '))) &&
      (((vl_bandeau[0] == 'A') && (vl_bandeau[1] == ' '))))
    strcpy(vl_bandeau, "");

  XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_message : apres comparaison %s", vl_texte);

  /*mise en majuscules des textes du message et du bandeau */
  drol_pmv_majuscules(vl_texte);
  drol_pmv_majuscules(vl_t1);
  drol_pmv_majuscules(vl_t2);
  drol_pmv_majuscules(vl_t3);
  drol_pmv_majuscules(vl_bandeau);

  /*ajout de la proposition de message PMV en base*/
  if (strlen(vl_texte) > 0)
  {
    strncpy(Texte1PMV, strtok(vl_texte, "\n"), 20);
    if (strcmp(Texte1PMV, "\0") != 0)
    {
      vl_str = strtok(NULL, "\n");
      if (vl_str != NULL)
        strncpy(Texte2PMV, vl_str, 20);
      else
        strcpy(Texte2PMV, "\0");
      if (strcmp(Texte2PMV, "\0") != 0)
      {
        vl_str = strtok(NULL, "\n");
        if (vl_str != NULL)
          strncpy(Texte3PMV, vl_str, 20);
        else
          strcpy(Texte3PMV, "\0");
      }
      else
        strcpy(Texte3PMV, "\0");
    }
    else
    {
      strcpy(Texte2PMV, "\0");
      strcpy(Texte3PMV, "\0");
    }
  }
  else
  {
    strcpy(Texte1PMV, "\0");
    strcpy(Texte2PMV, "\0");
    strcpy(Texte3PMV, "\0");
  }

  if (((vm_destination1_numero == 0) && (vm_destination2_numero == 0)) ||
      (vm_destination1_numero == XDC_DESTINATION_TRAVERSEE))
  {
    texte_pmv_de_base(Texte1PMV);
    texte_pmv_de_base(Texte2PMV);
    texte_pmv_de_base(Texte3PMV);
  }
  else
  {
    strcpy(Texte1PMV, vl_t1);
    strcpy(Texte2PMV, vl_t2);
    strcpy(Texte3PMV, vl_t3);
  }

  if (va_pmv.PMV_SAGA)
  {
    if ((va_pmv.Type_SAGA == XDC_SAGA_PMV_2_18) ||
        (va_pmv.Type_SAGA == XDC_SAGA_PMV_1_18) ||
        (va_pmv.Type_SAGA == XDC_SAGA_PMV_2_15) ||
        (va_pmv.Type_SAGA == XDC_SAGA_PMV_3_15))
    {
      strcpy(vl_bandeau, "");
      strcpy(vl_picto, "");
    }
  }

  if (vg_type_pmv[va_pmv.Type].Picto == 0)
  {
    strcpy(vl_bandeau, "");
    strcpy(vl_picto, "");
  }

  /*supprimer la proposition si identique a usage courant*/
  /*gerer la mise en file d'attente*/
  XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_message: courant %s/%s/%s/%s/%s",
                       va_pmv.Ligne1,
                       va_pmv.Ligne2,
                       va_pmv.Ligne3,
                       va_pmv.Bandeau,
                       va_pmv.Picto);
  XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_message : propose %s/%s/%s/%s/%s",
                       Texte1PMV,
                       Texte2PMV,
                       Texte3PMV,
                       vl_bandeau,
                       vl_picto);

	/*si on calcule un plan d'actions pour une simulation*/
	vl_mode_automatique = va_pert->Auto;

	vl_clignotement = va_pert->Clignotement;
	vl_flash = va_pert->Flash;

	XZST_03EcritureTrace(XZSTC_WARNING, "vg_site=%d;va_pmv.District=%d;va_pert->cligno = %d;va_pert->flash = %d", vg_site, va_pmv.District, va_pert->Clignotement, va_pert->Flash);
	XZST_03EcritureTrace(XZSTC_WARNING, "vg_presence_site[va_pmv.District]=%d", vg_presence_site[va_pmv.District]);

  if (((vg_site == XDC_CI) && (vg_presence_site[va_pmv.District] == XDC_VRAI)) || drol_pmv_fmc_simulee())
  {
    /*on ne fait que proposer, pas de pilotage automatique*/
    /* Ne s'applique pas sur COFIROUTE, uniquement sur ESCOTA */
    /* Sur COFIROUTE, le site principal peut piloter les BAD */
    vl_mode_automatique = 0;
  }

  if ((vg_calcul_auto) && (!drol_pmv_fmc_simulee()))
    vl_mode_automatique = 1;

  if (va_pmv.PMV_SAGA)
    vl_mode_automatique = 0;

  if ((strcmp(va_pmv.Ligne1, Texte1PMV)) ||
      (strcmp(va_pmv.Ligne2, Texte2PMV)) ||
      (strcmp(va_pmv.Ligne3, Texte3PMV)) ||
      (strcmp(va_pmv.Bandeau, vl_bandeau)) ||
      (strcmp(va_pmv.Picto, vl_picto)))
  {
    sprintf(vl_explication, "(%s)(%s) %s/%s/%s",
            vl_picto,
            vl_bandeau,
            Texte1PMV,
            Texte2PMV,
            Texte3PMV);

    /* si ce n'est pas une maj du pmva pour le meme evt */
    if ((vm_noeud->NumEvt.numero != va_pmv.Numero_Evt_U) || (vm_noeud->NumEvt.cle != va_pmv.Cle_Evt_U))
    {

      /* DEM655 si meme priorite , l'evt le plus proche est prioritaire */
      if ((va_pmv.Priorite_C == va_pmv.Priorite_U) && (va_pmv.Distance_C < va_pmv.Distance_U))
      {
        va_pmv.Priorite_U++;
        XZST_03EcritureTrace(XZSTC_INFO, "drol_pmv_message :si meme priorite , l'evt le plus proche est prioritaire :Distance_C=%d;Diastance_U=%d =>va_pmv.Priorite_C=%d;va_pmv.Priorite_U=%d  ", va_pmv.Distance_C, va_pmv.Distance_U, va_pmv.Priorite_C, va_pmv.Priorite_U);
      }

      /* DEM655 si  priorite superieure mais plus distant, alors on ne pilote pas en auto */
      if ((va_pmv.Priorite_C < va_pmv.Priorite_U) && (va_pmv.Distance_C > va_pmv.Distance_U) && (va_pmv.Priorite_U!=255))
      {
        vl_mode_automatique = 0;
        XZST_03EcritureTrace(XZSTC_INFO, "drol_pmv_message :si priorite superieure mais plus distant, alors on ne pilote pas en autoDistance_C=%d;Diastance_U=%d =>va_pmv.Priorite_C=%d;va_pmv.Priorite_U=%d  ", va_pmv.Distance_C, va_pmv.Distance_U, va_pmv.Priorite_C, va_pmv.Priorite_U);
      }
    }

    XZST_03EcritureTrace(XZSTC_INFO, "drol_pmv_message :auto=%d; autoDistance_C=%d;Diastance_U=%d =>va_pmv.Priorite_C=%d;va_pmv.Priorite_U=%d  ", vl_mode_automatique, va_pmv.Distance_C, va_pmv.Distance_U, va_pmv.Priorite_C, va_pmv.Priorite_U);
    /* si le mode est non auto ou
 si la priorite du pilotage en cours sur un autre evt est superieure
 alors on passe en proposition PA */
    if ((vl_mode_automatique == 0) || ((va_pmv.Priorite_C >= va_pmv.Priorite_U) && !((vm_noeud->NumEvt.numero == va_pmv.Numero_Evt_U) && (vm_noeud->NumEvt.cle == va_pmv.Cle_Evt_U) && (va_pmv.Priorite_U != 0))))
    {
      if (!va_pmv.PMV_SAGA)
      {
        /*mise en base de la proposition PMV*/
        /*si pas automatique*/
        pmv_tabuler_ligne(Texte1PMV, vg_type_pmv[va_pmv.Type].NombreCaracteres, vl_t1);
        pmv_tabuler_ligne(Texte2PMV, vg_type_pmv[va_pmv.Type].NombreCaracteres, vl_t2);
        pmv_tabuler_ligne(Texte3PMV, vg_type_pmv[va_pmv.Type].NombreCaracteres, vl_t3);
        XZST_03EcritureTrace(XZSTC_INFO, "drol_pmv_message :XZAP02;L1:%s;L2:%s;L3:%s;", vl_t1, vl_t2, vl_t3);
        if (XZAP02_Ajout_Prop_PMV(vg_numero_plan,
                                  vg_ordre_pmv++,
                                  va_pmv.District,
                                  "",
                                  va_pmv.Priorite_C,
                                  va_pmv.Identifiant,
                                  vl_explication,
                                  vl_t1,
                                  vl_t2,
                                  vl_t3,
                                  "", /*texte 4*/
                                  "", /*texte 5*/
                                  "", /*texte 6*/
                                  "Affichage",
                                  vl_bandeau, /*texte picto*/
                                  vl_picto,
                                  vm_destination1_numero,
                                  vm_destination2_numero,
                                  vm_destination3_numero,
                                  /*va_pmv.Destination1,*/ /*destination*/
                                  /*va_pmv.Destination2,*/ /*destination alternat*/
                                  va_pmv.Realisable,
                                  vl_clignotement,
								  vl_flash) != XDC_OK)
          XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_message : pb appel a XZAP02_Ajout_Prop_PMV");
      }
      else
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "SAGA appel a XZAP70_Ajout_Prop_SAGA eqt %d, ligne 1 %s",
                             va_pmv.Identifiant, Texte1PMV);
        if (XZAP70_Ajout_Prop_SAGA(vg_numero_plan,
                                   vg_ordre_pmv++,
                                   va_pmv.District,
                                   "",
                                   va_pmv.Priorite_C,
                                   va_pmv.Identifiant,
                                   vl_explication,
                                   Texte1PMV,
                                   Texte2PMV,
                                   Texte3PMV,
                                   "",
                                   "",
                                   "Affichage",
                                   vl_bandeau, /*texte picto*/
                                   vl_picto,
                                   0,
                                   0,
                                   vm_destination1_numero,
                                   vm_destination2_numero,
                                   vm_destination3_numero,
                                   va_pmv.Realisable,
                                   vl_clignotement) != XDC_OK)
          XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_message : pb appel a XZAP70_Ajout_Prop_SAGA");
      }
	}
	else
	{ /*automatique*/
		/*on est ici, donc on traite du pilotage automatique*/
		/*premier cas : on est sur le district de l equipement*/
		if (va_pmv.District == va_site)
		{
			/* Si PMV sans Flash mais Flash present dans les perturbation 
				Alors les lignes doivent clignoter */
			if (vg_type_pmv[va_pmv.Type].Flash == 0)
			{
				if (vl_flash == 2)
				{
					vl_clignotement = vl_flash;
				}		
			}
			XZST_03EcritureTrace (XZSTC_WARNING, "vg_type_pmv[va_pmv.Type].Flash=%d; vl_clignotement=%d; vl_flash = %d", vg_type_pmv[va_pmv.Type].Flash, vl_clignotement, vl_flash);

			pmv_tabuler_ligne(Texte1PMV, vg_type_pmv[va_pmv.Type].NombreCaracteres, vl_t1);
			pmv_tabuler_ligne(Texte2PMV, vg_type_pmv[va_pmv.Type].NombreCaracteres, vl_t2);
			pmv_tabuler_ligne(Texte3PMV, vg_type_pmv[va_pmv.Type].NombreCaracteres, vl_t3);
			if (XZAC01_Commande_PMV (	vm_noeud->NumEvt.numero,
										vm_noeud->NumEvt.cle,
										vg_num_poste,
										va_pmv.Identifiant,
										va_horodate,
										va_pmv.District,
										vl_t1,
										vl_t2,
										vl_t3,
										"",
										"",
										"",
										vl_clignotement ? XDC_PMV_CLIGNOLENT : XDC_PMV_PASCLIGNO,
										vl_clignotement ? XDC_PMV_CLIGNOLENT : XDC_PMV_PASCLIGNO,
										vl_clignotement ? XDC_PMV_CLIGNOLENT : XDC_PMV_PASCLIGNO,
										vl_flash?XDC_PMV_FLASH:XDC_PMV_PASFLASH,
										vl_picto,
										vl_bandeau,
										XDC_PMV_PASCLIGNO,
										va_pmv.Priorite_C,
										va_pmv.Distance_C,
										"",
										"",
										vm_destination1_numero,
										vm_destination2_numero,
										0,
										va_pmv.District,
										vm_destination3_numero) != XDC_OK)
				XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_recherche : pb dans l'appel a XZAC01_Commande_PMV");
			else
				XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_recherche : appel a XZAC01 OK");
		}
		else
		{ /*sur districts differents, envoyer un msg a TACLI*/
			pmv_tabuler_ligne(Texte1PMV, vg_type_pmv[va_pmv.Type].NombreCaracteres, vl_t1);
			pmv_tabuler_ligne(Texte2PMV, vg_type_pmv[va_pmv.Type].NombreCaracteres, vl_t2);
			pmv_tabuler_ligne(Texte3PMV, vg_type_pmv[va_pmv.Type].NombreCaracteres, vl_t3);
			if (XZAC101_Commande_PMV_TACLI (	vm_noeud->NumEvt.numero,
												vm_noeud->NumEvt.cle,
												vg_num_poste,
												va_pmv.Identifiant,
												va_horodate,
												va_pmv.District,
												vl_t1,
												vl_t2,
												vl_t3,
												"",
												"",
												"",
												vl_clignotement ? XDC_PMV_CLIGNOLENT : XDC_PMV_PASCLIGNO,
												vl_clignotement ? XDC_PMV_CLIGNOLENT : XDC_PMV_PASCLIGNO,
												vl_clignotement ? XDC_PMV_CLIGNOLENT : XDC_PMV_PASCLIGNO,
												vl_flash?XDC_PMV_FLASH:XDC_PMV_PASFLASH,
												vl_picto,
												vl_bandeau,
												XDC_PMV_PASCLIGNO,
												va_pmv.Priorite_C,
												va_pmv.Distance_C,
												"",
												"",
												vm_destination1_numero,
												vm_destination2_numero,
												0,
												vl_mode_automatique ? va_pmv.District : vg_site,
												vm_destination3_numero) != XDC_OK)
				XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_recherche : pb dans l'appel a XZAC101_Commande_PMV_TACLI");
			else
				XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_recherche : appel a XZAC101 OK");
		}

      /*mise en base de la proposition PMV*/
      if (XZAP02_Ajout_Prop_PMV(vg_numero_plan,
                                vg_ordre_pmv++,
                                va_pmv.District,
                                "",
                                255,
                                va_pmv.Identifiant,
                                vl_explication,
                                vl_t1,
                                vl_t2,
                                vl_t3,
                                "", /*texte 4*/
                                "", /*texte 5*/
                                "", /*texte 6*/
                                "SAPIENS",
                                vl_bandeau, /*texte picto*/
                                vl_picto,
                                vm_destination1_numero,
                                vm_destination2_numero,
                                vm_destination3_numero,
                                /*va_pmv.Destination1,*/ /*destination*/
                                /*va_pmv.Destination2,*/ /*destination alternat*/
                                va_pmv.Realisable,
                                vl_clignotement,
								vl_flash) != XDC_OK)
        XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_message : pb appel a XZAP02_Ajout_Prop_PMV");
    }
  }
  else
  { /*DEM524*/
    sprintf(vl_explication, "(%s)(%s) %s/%s/%s",
            vl_picto,
            vl_bandeau,
            Texte1PMV,
            Texte2PMV,
            Texte3PMV);

    /*mise en base de la proposition PMV*/
    if (!va_pmv.PMV_SAGA)
    {
      pmv_tabuler_ligne(Texte1PMV, vg_type_pmv[va_pmv.Type].NombreCaracteres, vl_t1);
      pmv_tabuler_ligne(Texte2PMV, vg_type_pmv[va_pmv.Type].NombreCaracteres, vl_t2);
      pmv_tabuler_ligne(Texte3PMV, vg_type_pmv[va_pmv.Type].NombreCaracteres, vl_t3);
      if (XZAP02_Ajout_Prop_PMV(vg_numero_plan,
                                vg_ordre_pmv++,
                                va_pmv.District,
                                "",
                                255,
                                va_pmv.Identifiant,
                                vl_explication,
                                vl_t1,
                                vl_t2,
                                vl_t3,
                                "", /*texte 4*/
                                "", /*texte 5*/
                                "", /*texte 6*/
                                "Affichage",
                                vl_bandeau, /*texte picto*/
                                vl_picto,
                                vm_destination1_numero,
                                vm_destination2_numero,
                                vm_destination3_numero,
                                /*va_pmv.Destination1,*/ /*destination*/
                                /*va_pmv.Destination2,*/ /*destination alternat*/
                                va_pmv.Realisable,
                                vl_clignotement,
								vl_flash) != XDC_OK )
        XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_message : pb appel a XZAP02_Ajout_Prop_PMV");
    }
    else
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "appel XZAP70 (2) : eqt %d, ligne 1 %s",
                           va_pmv.Identifiant, Texte1PMV);
      if (XZAP70_Ajout_Prop_SAGA(vg_numero_plan,
                                 vg_ordre_pmv++,
                                 va_pmv.District,
                                 "",
                                 255,
                                 va_pmv.Identifiant,
                                 vl_explication,
                                 Texte1PMV,
                                 Texte2PMV,
                                 Texte3PMV,
                                 "",
                                 "",
                                 "Affichage",
                                 vl_bandeau, /*texte picto*/
                                 vl_picto,
                                 0,
                                 0,
                                 vm_destination1_numero,
                                 vm_destination2_numero,
                                 vm_destination3_numero,
                                 va_pmv.Realisable,
                                 vl_clignotement) != XDC_OK)
        XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_message : pb appel a XZAP70_Ajout_Prop_SAGA");
    }
    XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_message : prop identique a l etat courant");
  }
  vm_presence_nature_bouchon = FALSE;
  vm_bouchon_apres_sortie = TRUE;
  vm_destination1_numero = 0;
  vm_destination2_numero = 0;
  vm_destination3_numero = 0;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  compare l'etat de chaque voie a l'etat de reference
*  tient compte du nombre de voies de circulation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_pmv_voies(int VL, int VM1, int VM2, int VR, int Reference)

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
  int Etat = 0;

  if ((VR != Reference) && (VM2 != Reference) && (VM1 != Reference) && (VL != Reference))
    Etat = 0; /* (aucune voie) */

  else if ((VR != Reference) && (VR != XDC_VOIE_INEXISTANTE) && (VM2 != Reference) && (VM1 != Reference) && (VL == Reference))
    Etat = 1; /* (la voie de droite) */

  else if ((VR != Reference) && (VR != XDC_VOIE_INEXISTANTE) && (VM2 != Reference) && (VM1 == Reference) && (VL == Reference))
    Etat = 2; /* (les deux voies de droite) */

  else if ((VR != Reference) && (VR != XDC_VOIE_INEXISTANTE) && (VM2 == Reference) && (VM1 == Reference) && (VL == Reference))
    Etat = 3; /* (les trois voies de droite) */

  else if ((VR == Reference) && (VM2 != Reference) && (VM1 != Reference) && (VL != Reference))
    Etat = 4; /* (la voie de gauche) */

  else if ((VR == Reference) && (((VM2 == Reference) && (VM1 != Reference)) || ((VM2 == XDC_VOIE_INEXISTANTE) && (VM1 == Reference))) && (VL != Reference))
    Etat = 5; /* (les deux voies de gauche) */

  else if ((VR == Reference) && (VM2 == Reference) && (VM1 == Reference) && (VL != Reference))
    Etat = 6; /* (les trois voies de gauche) */

  else if ((VR != Reference) && (((VM2 != Reference) && (VM2 != XDC_VOIE_INEXISTANTE) && (VM1 == Reference)) || ((VM2 == Reference) && (VM1 != Reference))) && (VL != Reference))
    Etat = 7; /* (une voie centrale) */

  else if ((VR != Reference) && (VM2 == XDC_VOIE_INEXISTANTE) && (VM1 == Reference) && (VL != Reference))
    Etat = 8; /* (la voie centrale) */

  else if ((VR != Reference) && (VM2 == Reference) && (VM1 == Reference) && (VL != Reference))
    Etat = 9; /* (les voies centrales) */

  else if ((((VR == Reference) && (VM2 != Reference) && (VM2 != XDC_VOIE_INEXISTANTE)) || ((VR != Reference) && (VM2 == Reference))) && (VM1 == Reference) && (VL == Reference))
    Etat = 10; /* (sauf sur une voie centrale) */

  else if ((VR == Reference) && (VM2 == XDC_VOIE_INEXISTANTE) && (VM1 != Reference) && (VM1 != XDC_VOIE_INEXISTANTE) && (VL == Reference))
    Etat = 11; /* (sauf sur la voie centrale) */

  else if ((VR == Reference) && (VM2 != Reference) && (VM2 != XDC_VOIE_INEXISTANTE) && (VM1 != Reference) && (VM1 != XDC_VOIE_INEXISTANTE) && (VL == Reference))
    Etat = 12; /* (sauf sur les voies centrales) */

  else if ((VR == Reference) && (VM2 != XDC_VOIE_INEXISTANTE) && (VM2 != Reference) && (VM1 == Reference) && (VL != Reference))
    Etat = 13; /* (sauf sur la voie de gauche et une voie centrale) */

  else if ((VR != Reference) && (VM2 == Reference) && (VM1 != XDC_VOIE_INEXISTANTE) && (VM1 != Reference) && (VL == Reference))
    Etat = 14; /* (sauf sur une voie centrale et la voie de droite) */

  else if (((VR == Reference) || (VR == XDC_VOIE_INEXISTANTE)) && ((VM2 == Reference) || (VM2 == XDC_VOIE_INEXISTANTE)) && ((VM1 == Reference) || (VM1 == XDC_VOIE_INEXISTANTE)) && (VL == Reference))
    Etat = 15; /* (toutes les voies) */

  return (Etat);
}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 * calcule le texte d'une ligne d'un PMV
 *------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

char *drol_pmv_calcul_ligne(T_LIGNE_PMV va_ligne)

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
  static char vl_str[100];
  char vl_str2[100];
  char *(*vl_fct)();
  vl_fct = va_ligne.Fct;

  XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_calcul_ligne texte = %s", va_ligne.Texte);

  strcpy(vl_str2, (*vl_fct)());
  XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_calcul_ligne fct = %s", vl_str2);

  /*concatenation texte et variable*/
  if (strcmp(va_ligne.Texte, ""))
    sprintf(vl_str, "%s%s", va_ligne.Texte, vl_str2);
  else
    sprintf(vl_str, "%s", vl_str2);

  XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_calcul_ligne = %s", vl_str);
  return (vl_str);
}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 * calcule le texte d'une ligne d'un PMV
 *------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

char *drol_pmv_calcul_ligne_minuscules(T_LIGNE_PMV va_ligne)

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
  static char vl_str[100];
  char vl_str2[100];
  char *(*vl_fct)();
  vl_fct = va_ligne.Fct;

  XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_calcul_ligne texte_minuscules = %s", va_ligne.Texte);

  strcpy(vl_str2, (*vl_fct)());
  XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_calcul_ligne_minuscules fct = %s", vl_str2);

  /*concatenation texte et variable*/
  sprintf(vl_str, "%s%s", va_ligne.Texte, vl_str2);

  XZST_03EcritureTrace(XZSTC_FONCTION, "drol_pmv_calcul_ligne_minuscules = %s", vl_str);
  return (vl_str);
}

static char *drol_pmv_var_localisation()
{
  T_Localisation vl_loc;
  T_Localisation vl_loc_inv;
  T_Localisation vl_loc_tete;
  static char vl_str[150];
  int vl_inversee = 1;
  int vl_longueur_fin_bouchon_km_arrondi;
  /* pour appel XZAE21 pour les PMV en sens oppose */
  XDY_Octet NumSortieAmont = 0;
  XDY_Entier DistanceSortieAmont;
  XDY_Octet SortieAmontEstBifurc = XDC_FAUX;
  XDY_Octet NumSortieAval = 0;
  XDY_Entier DistanceSortieAval;
  XDY_Octet SortieAvalEstBifurc = XDC_FAUX;
  XDY_Octet NumPeageAmont = 0;
  XDY_Entier DistancePeageAmont;
  XDY_Octet NumPeageAval = 0;
  XDY_Entier DistancePeageAval;
  XDY_Entier LongueurPlateformePeageAval;
  XDY_Autoroute NumAutorouteBifAmont = 0;
  XDY_Entier DistanceBifAmont;
  XDY_Autoroute NumAutorouteBifAval = 0;
  XDY_Entier DistanceBifAval;
  char Descente[50];
  char Tunnel[50];
  char NomSortieAmont[50];
  char AbrevSortieAmont[50];
  XDY_Sortie SortieAmontUsager;
  char NomSortieAval[50];
  char AbrevSortieAval[50];
  XDY_Sortie SortieAvalUsager;
  char NomPeageAmont[50];
  char AbrevPeageAmont[50];
  char NomPeageAval[50];
  char AbrevPeageAval[50];
  XDY_Mot Pente = 0;
  XDY_Octet Site;
  char NomAutoroute[50];
  XDY_PR PrBifurcAmont;
  XDY_PR PrBifurcAval;
  XDY_PR PrDebutBifurcAmont;
  XDY_PR PrDebutBifurcAval;
  char NomViaduc[50];
  char Nom[50];
  XDY_Octet NumSortie;
  XDY_Mot ZoneUrbaine;

  vl_loc = vm_noeud->Localisation;
  vl_loc_tete = vm_noeud->Localisation_Tete;
  vm_bouchon_apres_sortie = TRUE;
  ZoneUrbaine = vl_loc.ZoneUrbaine;
  /* PNI DEM688: s'il s'agit d'un calucl de PA pour les 2 sens, alors on soustrait l'offset (10) pour obtenir le vrai sens de l'evt */
  if (vl_loc.Sens >= 10)
    vl_loc.Sens = vl_loc.Sens - 10;

  /*PATCH A57*/
  if (vl_loc.Autoroute == 8) /*JM rajoute {*/
    vl_inversee = -1;
  DistanceSortieAmont = vl_loc.DistanceSortieAmont;
  DistanceSortieAval = vl_loc_tete.DistanceSortieAval;

  /* Recherche les sortie pour le PMV en sens oppose */
  if ((vm_pmv.Sens != vl_loc.Sens) && (vl_loc.Autoroute == vm_pmv.Autoroute))
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "XZAE21_Localiser_Evt_PA %d", vm_pmv.Sens);
    if (XZAE21_Localiser_Evt_PA(vm_noeud->Localisation.Autoroute, vm_noeud->Localisation_Tete.PR,
                                vm_pmv.Sens,
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
                                &PrDebutBifurcAmont, &PrDebutBifurcAval, NomViaduc,
                                &ZoneUrbaine) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "drol_tfm_lit_infos:  appel a XZAE21 a echoue pour la tete");
      return (vl_str);
    }
    drol_trim(SortieAmontUsager);
    drol_trim(SortieAvalUsager);
    if ((vl_inversee) && (vm_noeud->Type != XZAEC_FMC_Basculement) && (vm_noeud->Type != XZAEC_FMC_BasculTravaux))
    {
      strcpy(Nom, SortieAmontUsager);
      strcpy(SortieAmontUsager, SortieAvalUsager);
      strcpy(SortieAvalUsager, Nom);
      NumSortie = NumSortieAmont;
      NumSortieAmont = NumSortieAval;
      NumSortieAval = NumSortie;
      strcpy(Nom, AbrevSortieAmont);
      strcpy(AbrevSortieAmont, AbrevSortieAval);
      strcpy(AbrevSortieAval, Nom);
    }
    XZST_03EcritureTrace(XZSTC_WARNING, "amont %s(%d) %s(%d)", SortieAmontUsager, NumSortieAmont, SortieAvalUsager, NumSortieAval);
  }

  XZST_03EcritureTrace(XZSTC_DEBUG1, "LOCAL %d, %d, %d, %d",
                       vm_pmv.PR,
                       vl_loc.PR,
                       vl_loc_tete.PR,
                       vl_inversee);

  XZST_03EcritureTrace(XZSTC_WARNING, "zone urbaine %d", ZoneUrbaine);
  /*localisation entre deux sorties (nom et numero)*/
  if ((util_pmv_amont_fmc(vm_pmv.PR,
                          vm_pmv.Sens,
                          vl_loc.PR,
                          vl_loc_tete.PR,
                          vl_loc.Sens,
                          vl_inversee)) &&
      (util_pmv_amont_sortie(vm_pmv.Distance_C,
                             vm_pmv.Sens,
                             DistanceSortieAmont,
                             DistanceSortieAval,
                             vl_loc.Sens,
                             ZoneUrbaine)) &&
      (DistanceSortieAmont < C_PMV_EPSILON) &&
      (DistanceSortieAval < C_PMV_EPSILON) &&
      (vl_loc.PR != vl_loc_tete.PR) &&
      (SortieAmontEstBifurc == XDC_FAUX))
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, "localisation entre deux sorties vm_pmv.Sens=%d;vl_loc.Sens=%d.", vm_pmv.Sens, vl_loc.Sens);
    if (((vl_loc.Sens == XDC_SENS_INCONNU) || ((vm_pmv.Sens != vl_loc.Sens) && (vl_loc.Autoroute == vm_pmv.Autoroute)))) /*&& (vm_pmv.Sens==XDC_SENS_SUD))*/
      sprintf(vl_str, "apres SORTIE %s \n %s",
              SortieAmontUsager,
              AbrevSortieAmont);
    else
      sprintf(vl_str, "apres SORTIE %s \n %s",
              vl_loc.SortieAmontUsager,
              vl_loc.AbrevSortieAmont);
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }

  /*localisation par rapport a la sortie amont*/
  if ((util_pmv_amont_fmc(vm_pmv.PR,
                          vm_pmv.Sens,
                          vl_loc.PR,
                          vl_loc_tete.PR,
                          vl_loc.Sens,
                          vl_inversee)) &&
      (util_pmv_amont_sortie(vm_pmv.Distance_C,
                             vm_pmv.Sens,
                             DistanceSortieAmont,
                             DistanceSortieAval,
                             vl_loc.Sens,
                             ZoneUrbaine)) &&
      (SortieAmontEstBifurc == XDC_FAUX))
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, "localisation par rapport a la sortie amont vm_pmv.Sens=%d;vl_loc.Sens=%d.", vm_pmv.Sens, vl_loc.Sens);
    XZST_03EcritureTrace(XZSTC_FONCTION, "localisation par rapport a la distance de la sortie p/r au pmv vm_pmv.Distance_C=%d, vm_pmv.Sens=%d,vl_loc.DistanceSortieAmont=%d, vl_loc_tete.DistanceSortieAval=%d,vl_loc.Sens=%d",
                         vm_pmv.Distance_C, vm_pmv.Sens, DistanceSortieAmont, DistanceSortieAval, vl_loc.Sens);
    if (((vl_loc.Sens == XDC_SENS_INCONNU) || ((vm_pmv.Sens != vl_loc.Sens) && (vl_loc.Autoroute == vm_pmv.Autoroute)))) /*  && (vm_pmv.Sens==XDC_SENS_SUD))*/

      sprintf(vl_str, "apres SORTIE %s \n %s",
              SortieAmontUsager,
              AbrevSortieAmont);
    else
      sprintf(vl_str, "apres SORTIE %s \n %s",
              vl_loc.SortieAmontUsager,
              vl_loc.AbrevSortieAmont);
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }

  /*localisation de la fin a la sortie aval (nom et numero)*/
  if ((util_pmv_amont_fmc(vm_pmv.PR,
                          vm_pmv.Sens,
                          vl_loc.PR,
                          vl_loc_tete.PR,
                          vl_loc.Sens,
                          vl_inversee)) &&
      (((vl_loc.Sens == XDC_SENS_INCONNU) &&
        (vm_pmv.Sens == XDC_SENS_SUD) &&
        (DistanceSortieAmont < C_PMV_EPSILON)) ||
       ((vl_loc.Sens == XDC_SENS_INCONNU) &&
        (vm_pmv.Sens == XDC_SENS_NORD) &&
        (DistanceSortieAval < C_PMV_EPSILON)) ||
       ((vl_loc.Sens != XDC_SENS_INCONNU) &&
        (DistanceSortieAval < C_PMV_EPSILON))) &&
      (vm_pmv.Distance_C > C_PMV_DMAX_FIN) &&
      (vl_loc_tete.PR != vl_loc.PR))
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, "localisation de la fin a la sortie aval vm_pmv.Sens=%d;vl_loc.Sens=%d.", vm_pmv.Sens, vl_loc.Sens);
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str, "a %d km", (vm_pmv.Distance_C + 500) / 1000);
    else if ((vm_pmv.Distance_C >= 100))
    {
      sprintf(vl_str, "a %d m", (int)(vm_pmv.Distance_C / 100) * 100);
    }
    else
      sprintf(vl_str, " "); /*DEM777*/

    vm_bouchon_apres_sortie = FALSE;
    strcpy(vm_pmv.Bandeau, "");
    return (vl_str);
  }

  /*localisation entre deux sorties (nom)*/
  if ((util_pmv_amont_fmc(vm_pmv.PR,
                          vm_pmv.Sens,
                          vl_loc.PR,
                          vl_loc_tete.PR,
                          vl_loc.Sens,
                          vl_inversee)) &&
      (util_pmv_amont_sortie(vm_pmv.Distance_C,
                             vm_pmv.Sens,
                             DistanceSortieAmont,
                             DistanceSortieAval,
                             vl_loc.Sens,
                             ZoneUrbaine)) &&
      (DistanceSortieAmont < C_PMV_EPSILON) &&
      (DistanceSortieAval < C_PMV_EPSILON))
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, "localisation entre deux sorties (nom) vm_pmv.Sens=%d;vl_loc.Sens=%d.", vm_pmv.Sens, vl_loc.Sens);
    if (((vl_loc.Sens == XDC_SENS_INCONNU) || ((vm_pmv.Sens != vl_loc.Sens) && (vl_loc.Autoroute == vm_pmv.Autoroute)))) /* && (vm_pmv.Sens==XDC_SENS_SUD))*/
      sprintf(vl_str, "de %s\na %s",
              AbrevSortieAmont,
              vl_loc.AbrevSortieAmont);
    else
      sprintf(vl_str, "de %s\na %s",
              vl_loc.AbrevSortieAmont,
              vl_loc_tete.AbrevSortieAval);
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }

  /*localisation par defaut*/
  if ((!(vm_pmv.Presence_Bifurcation) &&
       (((vm_pmv.Sens == XDC_SENS_SUD) &&
         (vm_pmv.PR * vl_inversee < vl_loc.PR * vl_inversee)) ||
        ((vm_pmv.Sens == XDC_SENS_NORD) &&
         (vm_pmv.PR * vl_inversee > vl_loc.PR * vl_inversee)))) ||
      (vm_pmv.Presence_Bifurcation))
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, "localisation par defaut vm_pmv.Sens=%d;vl_loc.Sens=%d.", vm_pmv.Sens, vl_loc.Sens);
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str, "a %d km", (vm_pmv.Distance_C + 500) / 1000);
    else if ((vm_pmv.Distance_C >= 100))
    {
      sprintf(vl_str, "a %d m", (int)(vm_pmv.Distance_C / 100) * 100);
    }
    else
      sprintf(vl_str, " "); /*DEM777*/

    vm_bouchon_apres_sortie = FALSE;
    strcpy(vm_pmv.Bandeau, "");
    return (vl_str);
  }

  /*localisation - indication de la longueur separant de la fin*/
  if ((((vm_pmv.Sens == XDC_SENS_SUD) &&
        (vm_pmv.PR * vl_inversee >= vl_loc.PR * vl_inversee)) ||
       ((vm_pmv.Sens == XDC_SENS_NORD) &&
        (vm_pmv.PR * vl_inversee <= vl_loc.PR * vl_inversee))) &&
      (vm_pmv.Distance_C > C_PMV_DMAX_FIN) &&
      (!(vm_pmv.Presence_Bifurcation)))
  {
    if (vm_pmv.Distance_C <= 500)
    {
      if ((vm_pmv.Distance_C >= 100))
      {
        sprintf(vl_str, "sur %d m", (int)(vm_pmv.Distance_C / 100) * 100);
      }
      else
        sprintf(vl_str, " "); /*DEM777*/
    }
    else
    {
      /* REPERE 2 : on arrondit la longueur du bouchon a l'entier le plus proche */
      if ((vm_pmv.Distance_C % 1000) >= 500)
        vl_longueur_fin_bouchon_km_arrondi = (int)(vm_pmv.Distance_C / 1000) + 1;
      else
        vl_longueur_fin_bouchon_km_arrondi = (int)(vm_pmv.Distance_C / 1000);
      sprintf(vl_str, "sur %d km", vl_longueur_fin_bouchon_km_arrondi);
    }
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }

  /*localisation - indication de la distance separant de la fin*/
  if ((((vm_pmv.Sens == XDC_SENS_SUD) &&
        (vm_pmv.PR * vl_inversee > vl_loc.PR * vl_inversee)) ||
       ((vm_pmv.Sens == XDC_SENS_NORD) &&
        (vm_pmv.PR * vl_inversee < vl_loc.PR * vl_inversee))) &&
      (vm_pmv.Distance_C <= C_PMV_DMAX_FIN) &&
      (!(vm_pmv.Presence_Bifurcation)))
  {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str, "a %d km", (vm_pmv.Distance_C + 500) / 1000);
    else if ((vm_pmv.Distance_C >= 100))
    {
      sprintf(vl_str, "a %d m", (int)(vm_pmv.Distance_C / 100) * 100);
    }
    else
      sprintf(vl_str, " "); /*DEM777*/

    vm_bouchon_apres_sortie = FALSE;
    return (vl_str);
  }

  return ("");
}

static char *drol_pmv_var_localisation1ligne()
{
  T_Localisation vl_loc;
  T_Localisation vl_loc_inv;
  T_Localisation vl_loc_tete;
  static char vl_str[150];
  int vl_inversee = 1;
  int vl_longueur_fin_bouchon_km_arrondi;
  /* pour appel XZAE21 pour les PMV en sens oppose */
  XDY_Octet NumSortieAmont = 0;
  XDY_Entier DistanceSortieAmont;
  XDY_Octet SortieAmontEstBifurc = XDC_FAUX;
  XDY_Octet NumSortieAval = 0;
  XDY_Entier DistanceSortieAval;
  XDY_Octet SortieAvalEstBifurc = XDC_FAUX;
  XDY_Octet NumPeageAmont = 0;
  XDY_Entier DistancePeageAmont;
  XDY_Octet NumPeageAval = 0;
  XDY_Entier DistancePeageAval;
  XDY_Entier LongueurPlateformePeageAval;
  XDY_Autoroute NumAutorouteBifAmont = 0;
  XDY_Entier DistanceBifAmont;
  XDY_Autoroute NumAutorouteBifAval = 0;
  XDY_Entier DistanceBifAval;
  char Descente[50];
  char Tunnel[50];
  char NomSortieAmont[50];
  char AbrevSortieAmont[50];
  XDY_Sortie SortieAmontUsager;
  char NomSortieAval[50];
  char AbrevSortieAval[50];
  XDY_Sortie SortieAvalUsager;
  char NomPeageAmont[50];
  char AbrevPeageAmont[50];
  char NomPeageAval[50];
  char AbrevPeageAval[50];
  XDY_Mot Pente = 0;
  XDY_Octet Site;
  char NomAutoroute[50];
  XDY_PR PrBifurcAmont;
  XDY_PR PrBifurcAval;
  XDY_PR PrDebutBifurcAmont;
  XDY_PR PrDebutBifurcAval;
  char NomViaduc[50];
  XDY_Mot ZoneUrbaine;

  vl_loc = vm_noeud->Localisation;
  vl_loc_tete = vm_noeud->Localisation_Tete;
  vm_bouchon_apres_sortie = TRUE;

  /* PNI DEM688: s'il s'agit d'un calucl de PA pour les 2 sens, alors on soustrait l'offset (10) pour obtenir le vrai sens de l'evt */
  if (vl_loc.Sens >= 10)
    vl_loc.Sens = vl_loc.Sens - 10;

  /*PATCH A57*/
  if (vl_loc.Autoroute == 8)
    vl_inversee = -1;
  DistanceSortieAmont = vl_loc.DistanceSortieAmont;
  DistanceSortieAval = vl_loc_tete.DistanceSortieAval;

  /* Recherche les sortie pour le PMV en sens oppose */
  if (vm_pmv.Sens != vl_loc.Sens)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "appel XZAE21 avec %d %d %d",
                         vm_noeud->Localisation.Autoroute,
                         vm_noeud->Localisation_Tete.PR,
                         vm_pmv.Sens);
    if (XZAE21_Localiser_Evt_PA(vm_noeud->Localisation.Autoroute, vm_noeud->Localisation_Tete.PR,
                                vm_pmv.Sens,
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
                                &PrDebutBifurcAmont, &PrDebutBifurcAval, NomViaduc, &ZoneUrbaine) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "drol_tfm_lit_infos:  appel a XZAE21 a echoue pour la tete");
      return (vl_str);
    }
    XZST_03EcritureTrace(XZSTC_WARNING, " SortieAmontUsager %s, AbrevSortieAmont %s, NumSortieAmont %d, NumSortieAval %d, AbrevSortieAval %s", SortieAmontUsager, AbrevSortieAmont, NumSortieAmont, NumSortieAval, AbrevSortieAval);
  }

  XZST_03EcritureTrace(XZSTC_DEBUG1, "LOCAL %d, %d, %d, %d",
                       vm_pmv.PR,
                       vl_loc.PR,
                       vl_loc_tete.PR,
                       vl_inversee);

  vm_noeud->Infos_TFM.Zone_Urbaine = ZoneUrbaine;
  vm_noeud->Localisation.ZoneUrbaine = ZoneUrbaine;

  /*localisation entre deux sorties (nom et numero)*/
  if ((util_pmv_amont_fmc(vm_pmv.PR,
                          vm_pmv.Sens,
                          vl_loc.PR,
                          vl_loc_tete.PR,
                          vl_loc.Sens,
                          vl_inversee)) &&
      (util_pmv_amont_sortie(vm_pmv.Distance_C,
                             vm_pmv.Sens,
                             DistanceSortieAmont,
                             DistanceSortieAval,
                             vl_loc.Sens,
                             ZoneUrbaine)) &&
      (DistanceSortieAmont < C_PMV_EPSILON) &&
      (DistanceSortieAval < C_PMV_EPSILON) &&
      (vl_loc.PR != vl_loc_tete.PR) &&
      (SortieAmontEstBifurc == XDC_FAUX))
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, "localisation entre deux sorties vm_pmv.Sens=%d;vl_loc.Sens=%d.", vm_pmv.Sens, vl_loc.Sens);
    if (((vl_loc.Sens == XDC_SENS_INCONNU) || (vm_pmv.Sens != vl_loc.Sens))) /*&& (vm_pmv.Sens==XDC_SENS_SUD))*/
      sprintf(vl_str, "apres SORTIE %s \n %s",
              SortieAmontUsager,
              AbrevSortieAmont);
    else
      sprintf(vl_str, "apres SORTIE %s \n %s",
              vl_loc.SortieAmontUsager,
              vl_loc.AbrevSortieAmont);
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }

  /*localisation par rapport a la sortie amont*/
  if ((util_pmv_amont_fmc(vm_pmv.PR,
                          vm_pmv.Sens,
                          vl_loc.PR,
                          vl_loc_tete.PR,
                          vl_loc.Sens,
                          vl_inversee)) &&
      (util_pmv_amont_sortie(vm_pmv.Distance_C,
                             vm_pmv.Sens,
                             DistanceSortieAmont,
                             DistanceSortieAval,
                             vl_loc.Sens,
                             ZoneUrbaine)) &&
      (SortieAmontEstBifurc == XDC_FAUX))
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, "localisation par rapport a la sortie amont vm_pmv.Sens=%d;vl_loc.Sens=%d.", vm_pmv.Sens, vl_loc.Sens);
    XZST_03EcritureTrace(XZSTC_FONCTION, "localisation par rapport a la distance de la sortie p/r au pmv vm_pmv.Distance_C=%d, vm_pmv.Sens=%d,vl_loc.DistanceSortieAmont=%d, vl_loc_tete.DistanceSortieAval=%d,vl_loc.Sens=%d",
                         vm_pmv.Distance_C, vm_pmv.Sens, DistanceSortieAmont, DistanceSortieAval, vl_loc.Sens);
    if (((vl_loc.Sens == XDC_SENS_INCONNU) || (vm_pmv.Sens != vl_loc.Sens))) /*  && (vm_pmv.Sens==XDC_SENS_SUD))*/

      sprintf(vl_str, "apres SORTIE %s \n %s",
              SortieAmontUsager,
              AbrevSortieAmont);
    else
      sprintf(vl_str, "apres SORTIE %s \n %s",
              vl_loc.SortieAmontUsager,
              vl_loc.AbrevSortieAmont);
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }

  /*localisation de la fin a la sortie aval (nom et numero)*/
  if ((util_pmv_amont_fmc(vm_pmv.PR,
                          vm_pmv.Sens,
                          vl_loc.PR,
                          vl_loc_tete.PR,
                          vl_loc.Sens,
                          vl_inversee)) &&
      (((vl_loc.Sens == XDC_SENS_INCONNU) &&
        (vm_pmv.Sens == XDC_SENS_SUD) &&
        (DistanceSortieAmont < C_PMV_EPSILON)) ||
       ((vl_loc.Sens == XDC_SENS_INCONNU) &&
        (vm_pmv.Sens == XDC_SENS_NORD) &&
        (DistanceSortieAval < C_PMV_EPSILON)) ||
       ((vl_loc.Sens != XDC_SENS_INCONNU) &&
        (DistanceSortieAval < C_PMV_EPSILON))) &&
      (vm_pmv.Distance_C > C_PMV_DMAX_FIN) &&
      (vl_loc_tete.PR != vl_loc.PR))
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, "localisation de la fin a la sortie aval vm_pmv.Sens=%d;vl_loc.Sens=%d.", vm_pmv.Sens, vl_loc.Sens);
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str, "a %d km", (vm_pmv.Distance_C + 500) / 1000);
    else if ((vm_pmv.Distance_C >= 100))
    {
      sprintf(vl_str, "a %d m", (int)(vm_pmv.Distance_C / 100) * 100);
    }
    else
      sprintf(vl_str, " "); /*DEM777*/

    vm_bouchon_apres_sortie = FALSE;
    strcpy(vm_pmv.Bandeau, "");
    return (vl_str);
  }

  /*localisation entre deux sorties (nom)*/
  if ((util_pmv_amont_fmc(vm_pmv.PR,
                          vm_pmv.Sens,
                          vl_loc.PR,
                          vl_loc_tete.PR,
                          vl_loc.Sens,
                          vl_inversee)) &&
      (util_pmv_amont_sortie(vm_pmv.Distance_C,
                             vm_pmv.Sens,
                             DistanceSortieAmont,
                             DistanceSortieAval,
                             vl_loc.Sens,
                             ZoneUrbaine)) &&
      (DistanceSortieAmont < C_PMV_EPSILON) &&
      (DistanceSortieAval < C_PMV_EPSILON))
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, "localisation entre deux sorties (nom) vm_pmv.Sens=%d;vl_loc.Sens=%d.", vm_pmv.Sens, vl_loc.Sens);
    if (((vl_loc.Sens == XDC_SENS_INCONNU) || (vm_pmv.Sens != vl_loc.Sens))) /* && (vm_pmv.Sens==XDC_SENS_SUD))*/
      sprintf(vl_str, "de %s\na %s",
              AbrevSortieAmont,
              vl_loc.AbrevSortieAmont);
    else
      sprintf(vl_str, "de %s\na %s",
              vl_loc.AbrevSortieAmont,
              vl_loc_tete.AbrevSortieAval);
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }

  /*localisation par defaut*/
  if ((!(vm_pmv.Presence_Bifurcation) &&
       (((vm_pmv.Sens == XDC_SENS_SUD) &&
         (vm_pmv.PR * vl_inversee < vl_loc.PR * vl_inversee)) ||
        ((vm_pmv.Sens == XDC_SENS_NORD) &&
         (vm_pmv.PR * vl_inversee > vl_loc.PR * vl_inversee)))) ||
      (vm_pmv.Presence_Bifurcation))
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, "localisation par defaut vm_pmv.Sens=%d;vl_loc.Sens=%d.", vm_pmv.Sens, vl_loc.Sens);
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str, "a %d km", (vm_pmv.Distance_C + 500) / 1000);
    else if ((vm_pmv.Distance_C >= 100))
    {
      sprintf(vl_str, "a %d m", (int)(vm_pmv.Distance_C / 100) * 100);
    }
    else
      sprintf(vl_str, " "); /*DEM777*/

    vm_bouchon_apres_sortie = FALSE;
    strcpy(vm_pmv.Bandeau, "");
    return (vl_str);
  }

  /*localisation - indication de la longueur separant de la fin*/
  if ((((vm_pmv.Sens == XDC_SENS_SUD) &&
        (vm_pmv.PR * vl_inversee >= vl_loc.PR * vl_inversee)) ||
       ((vm_pmv.Sens == XDC_SENS_NORD) &&
        (vm_pmv.PR * vl_inversee <= vl_loc.PR * vl_inversee))) &&
      (vm_pmv.Distance_C > C_PMV_DMAX_FIN) &&
      (!(vm_pmv.Presence_Bifurcation)))
  {
    if (vm_pmv.Distance_C <= 500)
    {
      if ((vm_pmv.Distance_C >= 100))
      {
        sprintf(vl_str, "sur %d m", (int)(vm_pmv.Distance_C / 100) * 100);
      }
      else
        sprintf(vl_str, " "); /*DEM777*/
    }
    else
    {
      /* REPERE 2 : on arrondit la longueur du bouchon a l'entier le plus proche */
      if ((vm_pmv.Distance_C % 1000) >= 500)
        vl_longueur_fin_bouchon_km_arrondi = (int)(vm_pmv.Distance_C / 1000) + 1;
      else
        vl_longueur_fin_bouchon_km_arrondi = (int)(vm_pmv.Distance_C / 1000);
      sprintf(vl_str, "sur %d km", vl_longueur_fin_bouchon_km_arrondi);
    }
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }

  /*localisation - indication de la distance separant de la fin*/
  if ((((vm_pmv.Sens == XDC_SENS_SUD) &&
        (vm_pmv.PR * vl_inversee > vl_loc.PR * vl_inversee)) ||
       ((vm_pmv.Sens == XDC_SENS_NORD) &&
        (vm_pmv.PR * vl_inversee < vl_loc.PR * vl_inversee))) &&
      (vm_pmv.Distance_C <= C_PMV_DMAX_FIN) &&
      (!(vm_pmv.Presence_Bifurcation)))
  {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str, "a %d km", (vm_pmv.Distance_C + 500) / 1000);
    else if ((vm_pmv.Distance_C >= 100))
    {
      sprintf(vl_str, "a %d m", (int)(vm_pmv.Distance_C / 100) * 100);
    }
    else
      sprintf(vl_str, " "); /*DEM777*/

    vm_bouchon_apres_sortie = FALSE;
    return (vl_str);
  }

  return ("");
}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 * Renvoie la chaine en caracteres majuscules
 *------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

static char *drol_pmv_majuscules(char *Texte)

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
  int i;
  int l;

  /*A
  ** on passe en majuscules SANS accent
  */
  l = (int)strlen(Texte);
  for (i = 0; i < l; i++)
    switch ((int)Texte[i])
    {
    case -64:
      Texte[i] = 'A';
      break;
    case -56:
      Texte[i] = 'A';
      break;
    case -59:
      Texte[i] = 'E';
      break;
    case -55:
      Texte[i] = 'E';
      break;
    case -63:
      Texte[i] = 'E';
      break;
    case -47:
      Texte[i] = 'I';
      break;
    case -62:
      Texte[i] = 'O';
      break;
    case -53:
      Texte[i] = 'U';
      break;
    case -61:
      Texte[i] = 'U';
      break;
    default:
      Texte[i] = toupper(Texte[i]);
    }

  /*A
  ** on enleve les blancs a la fin de la chaine
  */
  for (i = l - 1; i >= 0; i--)
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
 * Renvoie le picto a afficher sur le PMV
 *------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

char *drol_pmv_calcul_picto(char *va_picto, XDY_Booleen va_sensok)

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
  char *pl_picto = va_picto;
  char vl_picto[11];

  /* Remplacement du separateur des 2 pictos par fin de ligne */
  strcpy(vl_picto, vm_noeud->Infos_TFM.Config_Voies);
  if (strstr(vl_picto, "|") != NULL)
  {
    vl_picto[4] = 0;
    /* Si PMV dans le sens de l'evt alors on prend le second picto, sinon le premier */
    if (va_sensok)
      pl_picto = vl_picto + 5;
    else
      pl_picto = vl_picto;
  }

  if (!strcmp(va_picto, "VITAM"))
    return (drol_pmv_var_vitesse(1));

  if (!strcmp(va_picto, "VIT"))
    return (drol_pmv_var_vitesse(0));

  /*REPERE2 utiliser vm_noeud->Infos_TFM.Config_Voies*/
  XZST_03EcritureTrace(XZSTC_WARNING, "drol_pmv_calcul_picto: Infos_TFM.Config_Voies=%s.va_picto=%s;pl_picto=%s;va_sensok=%d.", vm_noeud->Infos_TFM.Config_Voies, va_picto, pl_picto, va_sensok);
  /*si pas rabattement*/
  if (strcmp(va_picto, "RABATTEMENT"))
    return (va_picto);

  if (strcmp(pl_picto, "AAAR") == 0)
    return ("AAAI");

  if (strcmp(pl_picto, "AIAR") == 0)
    return ("AAI");

  if (strcmp(pl_picto, "AIIR") == 0)
    return ("AI");

  if (strcmp(pl_picto, "AARR") == 0)
    return ("AAAI");

  if (strcmp(pl_picto, "AIRR") == 0)
    return ("AAI");

  if (strcmp(pl_picto, "ARRR") == 0)
    return ("AAAI");

  if (strcmp(pl_picto, "RAAA") == 0)
    return ("IAAA");

  if (strcmp(pl_picto, "RIAA") == 0)
    return ("IAA");

  if (strcmp(pl_picto, "RIIA") == 0)
    return ("IA");

  if (strcmp(pl_picto, "RRAA") == 0)
    return ("IAAA");

  if (strcmp(pl_picto, "RIRA") == 0)
    return ("IAA");

  if (strcmp(pl_picto, "RRRA") == 0)
    return ("IAAA");

  if (strcmp(pl_picto, "ARAA") == 0)
    return ("DANG");

  if (strcmp(pl_picto, "AARA") == 0)
    return ("DANG");

  if (strcmp(pl_picto, "AIRA") == 0)
    return ("DANG");

  if (strcmp(pl_picto, "ARRA") == 0)
    return ("DANG");

  if (strcmp(pl_picto, "RRAR") == 0)
    return ("DANG");

  if (strcmp(pl_picto, "RARR") == 0)
    return ("DANG");

  if (strcmp(pl_picto, "RIAR") == 0)
    return ("DANG");

  if (strcmp(pl_picto, "RAAR") == 0)
    return ("DANG");

  if (strcmp(pl_picto, "RARA") == 0)
    return ("DANG");

  if (strcmp(pl_picto, "ARAR") == 0)
    return ("DANG");

  if (
      strcmp(pl_picto, "RRRR") == 0 ||
      strcmp(pl_picto, "RIRR") == 0 ||
      strcmp(pl_picto, "RIIR") == 0 ||
      strcmp(pl_picto, "IIIR") == 0)
    /* REPERE4 */
    return ("CA");
  /* FIN REPERE4 */

  if (
      strcmp(pl_picto, "AAAA") == 0 ||
      strcmp(pl_picto, "AIIA") == 0 ||
      strcmp(pl_picto, "AIAA") == 0 ||
      strcmp(pl_picto, "IIIA") == 0)
    return ("DANG");

  /*picto de rabattement dans le cas chaussee bloquee*/
  /*  if ( (vm_noeud->Localisation.VL != XDC_VOIE_SENS_NORMAL) &&
         (vm_noeud->Localisation.VM1 != XDC_VOIE_SENS_NORMAL) &&
         (vm_noeud->Localisation.VM2 != XDC_VOIE_SENS_NORMAL) &&
         (vm_noeud->Localisation.VR != XDC_VOIE_SENS_NORMAL)
       )
      return ("I.A");
  */
  /*picto de rabattement dans le cas rabattement par la gauche, passage de 4 voies a 3 voies*/
  /*  if ( (vm_noeud->Localisation.VM2 != XDC_VOIE_INEXISTANTE) &&
         (vm_noeud->Localisation.VR != XDC_VOIE_SENS_NORMAL)
       )
      return ("IAAA");
  */
  /*picto de rabattement dans le cas rabattement par la gauche, passage de 3 voies a 2 voies*/
  /*  if ( (vm_noeud->Localisation.VM1 != XDC_VOIE_INEXISTANTE) &&
         (vm_noeud->Localisation.VM2 == XDC_VOIE_INEXISTANTE) &&
         (vm_noeud->Localisation.VR != XDC_VOIE_SENS_NORMAL)
       )
      return ("IAA");
  */
  /*picto de rabattement dans le cas rabattement par la gauche, passage de 2 voies a 1 voie*/
  /*(VR et VM2 inexistantes)*/
  /*  if ( (vm_noeud->Localisation.VL != XDC_VOIE_INEXISTANTE) &&
         (vm_noeud->Localisation.VM1 == XDC_VOIE_INEXISTANTE) &&
         (vm_noeud->Localisation.VM2 == XDC_VOIE_INEXISTANTE) &&
         (vm_noeud->Localisation.VR != XDC_VOIE_INEXISTANTE) &&
         (vm_noeud->Localisation.VR != XDC_VOIE_SENS_NORMAL)
       )
      return ("IA");
  */
  /*picto de rabattement dans le cas rabattement par la droite, passage de 2 voies a 1 voie */
  /*( VR et VM2 inexistantes).*/
  /*  if ( (vm_noeud->Localisation.VL != XDC_VOIE_SENS_NORMAL) &&
         (vm_noeud->Localisation.VM1 == XDC_VOIE_INEXISTANTE) &&
         (vm_noeud->Localisation.VM2 == XDC_VOIE_INEXISTANTE) &&
         (vm_noeud->Localisation.VR != XDC_VOIE_INEXISTANTE)
       )
      return ("AI");
  */
  /*picto de rabattement dans le cas rabattement par la droite, passage de 3 voies a 2 voies*/
  /* (VR inexistante) */
  /*  if ( (vm_noeud->Localisation.VL != XDC_VOIE_SENS_NORMAL) &&
         (vm_noeud->Localisation.VM1 != XDC_VOIE_INEXISTANTE) &&
         (vm_noeud->Localisation.VM2 == XDC_VOIE_INEXISTANTE)
       )
      return ("AAI");
  */
  /*picto de rabattement dans le cas rabattement par la droite, passage de 4 voies a 3 voies*/
  /*  if ( (vm_noeud->Localisation.VL != XDC_VOIE_SENS_NORMAL) &&
         (vm_noeud->Localisation.VM2 != XDC_VOIE_INEXISTANTE)
       )
      return ("AAAI");
  */

  return ("CHAN");
}
