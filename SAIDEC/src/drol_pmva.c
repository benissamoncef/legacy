/* Fichier : $Id: drol_pmva.c,v 1.49 2021/02/23 07:09:37 gesconf Exp $        Release : $Revision: 1.49 $        Date : $Date: 2021/02/23 07:09:37 $
**************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : drol_pmva.c				*
*************************************************
* DESCRIPTION :   
* new tdpac
*************************************************
* HISTORIQUE :
*
* JMG	16/06/05	creation 1.0
* JMG	06/04/07	correction BOUCHON SUR pour PMVA sur entree
* JMG	0/05/07	traitement BOUCHON DE XX km SUR YY km + remplacement de "SUR YY km" par  "DE XX km" (PNI)
* PNI	21/06/07 : Ajout des nouveaux type de fmc DEM658
* LCL   09/07/07 : Suppression DY
* PNI	21/08/07 : correction de drol_pmva_var_num_pt_car & "O M"DEM686 1.8
* PNI	03/09/07 : Ajout d'un predicat $LOCALISATION_KM$ pour forcer la localisation a " A XX KM " pour les coupures autoroute DEM698 v1.9
* PNI	14/09/07 : correction de drol_pmva_var_num_pt_car DEM686 1.10
* PNI	24/09/07 : correction SAPIENS 1.11
* JPL	02/03/09 : Gestion conditions "feu maitrise / non maitrise" pour FMC "Vehicule en feu"" (DEM 845) 1.12
* PNI	15/05/09 : Correction calcul de longueur :Suppression de la l'effacement de DE X KM pour les bouchon DEM885 1.14
* JPL	06/09/10 : Prise en compte du type Fmc Zone de stockage et du predicat "demarre" (DEM 942) 1.15
* PNI	23/09/10 : Modifier le creneau horaire de nuit : doit etre < a 6h DEM 957 1.16
* JMG	02/02/11 : correction calcul longeur 1.17
* JPL	18/02/11 : Migration architecture HP ia64 (DEM 971) : constantes et types SPECIFIQUES PMVA  1.18
* JPL	21/02/11 : Migration architecture HP ia64 (DEM 971) : Zones des portions de texte 'static'  1.19
* JPL	21/02/11 : Migration HP ia64 (DEM 971) : Gestion conversions types de tailles differentes (=)  1.20
* JPL	12/10/11 : Pas d'action automatique SAPIENS pour Fmc simulees (Essai ou Exercice) (DEM 995)  1.21
* JMG	31/01/12 : inhibition XZAD23_Calculer_TdP_PMV_Dest DEM/1014 1.22
* JPL	25/04/12 : Ajout var. $LOCALISATION_SORTIE$ situant un evt. par sa sortie amont seulement et non en km (DEM 1026)  1.23
* JPL	11/07/12 : Utilisation des noms de sortie "usager" et indicateurs de bifurcations (DEM 1033)  1.24
* VR	11/06/12 : Ajout TDP PMA (DEM/1014 PMA) 1.25
* PNI	08/11/12 :prise en compte des bretelles dans le SAD pour Fermeture Echangeur DEM1050 v1.26
* JMG	11/11/12 : regulation 1.27
* PNI	06/02/13 : Correction ligne vide sur proposition PMV : strncpy remplace strcpy pour TextxPMV 1.28
* PNI	23/08/13	 : SAPIENS au CI  DEM 1074/FR 105 1.29
* JMG	12/09/13 : TDP > 1h bien affiche 1.30
* PNI	20/01/15 : modification dela pol d'affichage PMV PMVA 1.32 DEM1106
* PNI	13/03/15 : ajout predicat circulation_basculee 1.33 DEM1106
* JMG	16/06/15 : evt dans les 2 sens lotPA 1.34 DEM1143
* PNI	29/01/16 : Correctif pour la localisation PMVA 1.35 DEM1148
* PNI	06/06/16 : Correctif pour ne pas chercher de destination pour les PMVA A57 sur evt autre autoroute (pb sens) 1.36 DEM1168 
* PNI	02/09/16	: Contresens : affichage dans les 2 sens v1.37 DEM1182
* JMG	06/12/16	: lot 23 1.38
* JMG   23/02/17        : lot 23 traversee DEM1223 1.40
* JMG	15/01/17	: info fermeture toulon 1.41
* MEH   18/01/18	: retrait verification action intervenant DEM1262  1.42
* MEH   06/02/2018      : ajout cas Salage et deneigement DEM1259  1.43
* JMG	08/03/18	: personnel travaux 1.44
* JPL	23/09/19 : Type d'evenement "INCENDIE" attendu en configuration et non "INCENDIE DE VEGETATION" (DEM 1352)  1.45
* JMG	25/09/20 : on force la tabulation dans SAD_PMV pour l usage de EQT_FIL 1.46
* JPL	08/10/20 : Mise en majuscules du texte avant ecriture en base et envoi (DEM SAE168)  1.48
* PNI	23/02/21 : on force la tabulation dans SAD_PMV pour l usage de EQT_FIL 1.49
* ABE	05/01/22 : Sur mode Auto, autoriser le pilotage des PMV/PMVA voisin DEM-SAE446
* LCL	04/10/23 : Adaptations CNA DEM-483 : lecture site principal en base
* GGY	23/01/24 : Ajout C_COND_CIRCULATION_BASC_TUNNEL et C_COND_CIRCULATION_NON_BASC_TUNNEL (DEM-483)
************************************************/

#include "drol_pmva.h"
#include "drol_pmva_util.h"
#include "drol_caltdp.h"
#include "drol_tdp.h"
#include "xzao16.h"
#include "xzao22.h"
#include "xzac82.h"
#include "xzac182.h"


#define C_REP_MIG	"/produits/migrazur/appliSD/fichiers/bdc/"
#define C_PERT_PMVA	C_REP_MIG "pert_pmva.txt"

#define C_PMV_DMAX_FIN       1000
#define C_SEPARATEUR	"#"
#define C_PIPE		"|"
#define C_ET		"$ET$"
#define C_CAR_DOLLAR	'$'
#define C_PMV_EPSILON	1000
#define C_NB_CAR_LIGNE	15
#define C_INFINI	1000000

#define C_INFERIEUR	"<"
#define C_SUPERIEUR	">"
#define C_EGAL		"="


#define C_COND_MAJEUR	"majeur"
#define C_COND_VOIES_RETRECIES	"voies retrecies"
#define C_COND_UNE_VOIE_BLOQ	"au moins une voie bloquee"
#define C_COND_UNE_VOIE_OUVERTE	"au moins une voie ouverte"
#define C_COND_TOUTES_VOIES_BLOQ	"toutes voies bloquees"
#define C_COND_UNE_VOIE_NEUT	"au moins une voie neutralisee"
#define C_COND_TOUTES_VOIES_NEUT	"toutes voies neutralisees"
#define C_COND_TOUTES_VOIES_FERMEES	"toutes voies fermees"
#define C_COND_CIRCULATION_UNE_VOIE	"circulation sur une voie"
#define C_COND_CIRCULATION_BASC	"circulation basculee"
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
#define C_COND_TRAFIC_ACCORDEON	"trafic accordeon"
#define C_COND_VOIES_RETRECIES	"voies retrecies"
#define C_COND_ENTREE_FERMEE	"entree fermee"
#define C_COND_SORTIE_FERMEE	"sortie fermee"
#define C_COND_DATEX	"datex"
#define C_COND_BOUCHON_MOINS_3_KM	"longueur<3"
#define C_COND_BOUCHON_ENTRE_3_ET_5_KM	"longueur>3<5"
#define C_COND_BOUCHON_PLUS_5_KM	"longueur>5"
#define C_COND_PREVU		"prevu"
#define C_COND_NON_PREVU	"non prevu"
#define C_COND_PAS_BOUCHON	"HORS BOUCHONS"
#define C_COND_CAUSE_OPERATION	"cause_op"
#define C_COND_CAUSE_EVENEMENT	"cause_evt"
#define C_COND_TRAVAUX_ENTRETIEN	"entretien"
#define C_COND_STOCKAGE		"stockage"
#define C_COND_AIRE_SERVICE	"service"
#define C_COND_AIRE_REPOS	"repos"
#define C_COND_FEU_MAITRISE		"feu_maitrise"
#define C_COND_FEU_NON_MAITRISE		"feu_non_maitrise"
#define C_COND_STOCKAGE_DEMARRE		"stockage_demarre"
#define C_COND_STOCKAGE_NON_DEMARRE	"stockage_non_demarre"
#define C_COND_FERMETURE_TOULON		"fermeture toulon"
#define C_COND_INTERVENTION	"intervention"
#define C_COND_PERSONNEL	"personnel"
#define C_COND_CIRCULATION_BASC_TUNNEL		"circulation_basculee"
#define C_COND_CIRCULATION_NON_BASC_TUNNEL	"circulation_non_basculee"

/*variables utilisees dans les messages PMVA*/
#define C_VAR_LONGUEUR		"LONGUEUR$"
#define C_VAR_LOCALISATION	"LOCALISATION$"
#define C_VAR_LOCALISATION_KM	"LOCALISATION_KM$"
#define C_VAR_LOCALIS_SORTIE	"LOCALISATION_SORTIE$"
#define C_VAR_LOCALISATION_TDP	"LOCALISATION_TDP$"
#define C_VAR_DISTANCE_BANDEAU	"DISTANCE BANDEAU$"
#define C_VAR_NUM_PT_CAR	"NUM PT CAR$"
#define C_VAR_NOM_PT_CAR	"NOM PT CAR$"
#define C_VAR_DISTANCE		"DISTANCE$"
#define C_VAR_CARBURANT		"CARBURANT$"
#define C_VAR_CAR		"CAR$" 
#define C_VAR_AUTOROUTE		"AUTOROUTE FERMEE$" 
#define C_VAR_SITUATION		"SITUATION$"
#define C_VAR_TDP		"TDP$"
#define C_VAR_NUIT		"NUIT$"
#define C_VAR_HORAIRES		"HORAIRES$"
#define C_VAR_NOM_AUTOROUTE	"AUTOROUTE$"
#define C_VAR_AIRE_AMONT	"STATION AMONT$"
#define C_VAR_DIRECTION	"DIRECTION$"
#define C_VAR_VITESSE "VITESSE$"
#define C_VAR_DEST1PROCHE_LOC	"DEST1PROCHE_LOC$"
#define C_VAR_DEST1PROCHE_LONG	"DEST1PROCHE_LONG$"
#define C_VAR_DEST1PROCHE	"DEST1PROCHE$"
#define C_VAR_TRAVERSEE         "TRAVERSEE$"
#define C_VAR_CAUSE		"CAUSE$"

T_PERT_PMVA	*vg_pert_pmva;
T_NOEUD		*vm_noeud;
static T_NOEUD	*vm_noeud2;
T_NOEUD 	*vg_racine;
T_PMVA		vm_pmv;
T_PERT_PMVA	*vm_pert;
XDY_Booleen	vm_dest1proche_HS;
XDY_Booleen	vm_bouchon_apres_sortie;
XDY_Booleen	vm_indice_bouchon;
XDY_Booleen	vm_presence_nature_bouchon;
int		vm_ligne_courante;
int		vm_dest1proche_numero;

XDY_Booleen     vm_mode_verif_bouchon;
XDY_Booleen     vm_mode_dans_pert;
XDY_Booleen     vm_type_bouchon;
XDY_Booleen     in_or_out;

extern XDY_Booleen vg_presence_site[];

int drol_pmva_charge();
T_CONDITION *drol_pmva_decode_condition(char *);
T_LIGNE_PMVA drol_pmva_remplit_ligne(char *);
void drol_pmva_message (T_PMVA, T_PERT_PMVA , XDY_Horodate, XDY_District);
char *drol_pmva_calcul_ligne(T_LIGNE_PMVA);


static XDY_Booleen drol_pmva_egal(int, int);
static XDY_Booleen drol_pmva_different(int, int);
static XDY_Booleen drol_pmva_true(int, int);
/*
static XDY_Booleen drol_pmva_true(int, int);
static XDY_Booleen drol_pmva_inferieur(int, int);
static XDY_Booleen drol_pmva_superieur(int, int);
*/

/*fonctions utilisees dans les conditions*/
static XDY_Booleen drol_pmva_majeur();
static XDY_Booleen drol_pmva_fmc_simulee();
static XDY_Booleen drol_pmva_une_voie_bloquee();
static XDY_Booleen drol_pmva_une_voie_ouverte();
static XDY_Booleen drol_pmva_toutes_voies_bloquees();
static XDY_Booleen drol_pmva_une_voie_neutralisee();
static XDY_Booleen drol_pmva_toutes_voies_neutralisees();
static XDY_Booleen drol_pmva_toutes_voies_bloquees_neutralisees();
static XDY_Booleen drol_pmva_toutes_voies_fermees();
static XDY_Booleen drol_pmva_circulation_une_voie();
static XDY_Booleen drol_pmva_circulation_basculee();
static XDY_Booleen drol_pmva_sur_bau();
static XDY_Booleen drol_pmva_sur_section_courante();
static XDY_Booleen drol_pmva_sur_lit_arret();
static XDY_Booleen drol_pmva_sur_echangeur();
static XDY_Booleen drol_pmva_sur_bretelle_entree();
static XDY_Booleen drol_pmva_sur_bretelle_sortie();
static XDY_Booleen drol_pmva_signale();
static XDY_Booleen drol_pmva_confirme();
static XDY_Booleen drol_pmva_trafic_dense();
static XDY_Booleen drol_pmva_trafic_ralenti();
static XDY_Booleen drol_pmva_trafic_bouche();
static XDY_Booleen drol_pmva_trafic_bloque();
static XDY_Booleen drol_pmva_trafic_accordeon();
static XDY_Booleen drol_pmva_voies_retrecies();
static XDY_Booleen drol_pmva_entree_fermee();
static XDY_Booleen drol_pmva_sortie_fermee();
static XDY_Booleen drol_pmva_datex();
static XDY_Booleen drol_pmva_bouchon_moins_3_km();
static XDY_Booleen drol_pmva_bouchon_entre_3_et_5_km();
static XDY_Booleen drol_pmva_bouchon_plus_5_km();
static XDY_Booleen drol_pmva_prevu();
static XDY_Booleen drol_pmva_non_prevu();
static XDY_Booleen drol_pmva_travaux_entretien();
static XDY_Booleen drol_pmva_stockage();
static XDY_Booleen drol_pmva_aire_service();
static XDY_Booleen drol_pmva_aire_repos();
static XDY_Booleen drol_pmva_feu_maitrise();
static XDY_Booleen drol_pmva_feu_non_maitrise();
static XDY_Booleen drol_pmva_cause_operation();
static XDY_Booleen drol_pmva_cause_evenement();
static XDY_Booleen drol_pmva_stockage_demarre();
static XDY_Booleen drol_pmva_stockage_non_demarre();
static XDY_Booleen drol_pmva_fermeture_toulon();
static XDY_Booleen drol_pmva_intervention();
static XDY_Booleen drol_pmva_personnel();
static XDY_Booleen drol_pmva_circulation_basculee_tunnel ();
static XDY_Booleen drol_pmva_circulation_non_basculee_tunnel ();

static int drol_pmva_voies(int, int, int, int, int);
static int drol_pmva_recherche(T_PERT_PMVA *, XDY_Horodate, XDY_District );

/*fonctions utilisees dans les variables des messages*/
static char *drol_pmva_var_longueur();
static char *drol_pmva_var_localisation();
static char *drol_pmva_var_localisation_km();
static char *drol_pmva_var_localisation_km_long();
static char *drol_pmva_var_localisation_tdp();
static char *drol_pmva_var_localisation_sortie();
static char *drol_pmva_var_num_pt_car();
static char *drol_pmva_var_nom_pt_car();
static char *drol_pmva_var_distance();
static char *drol_pmva_var_carburant();
static char *drol_pmva_var_car();
static char *drol_pmva_var_null();
static char *drol_pmva_var_autoroute();
static char *drol_pmva_var_dest_un_proche();
static char *drol_pmva_var_dest_un_proche_loc();
static char *drol_pmva_var_dest_un_proche_long();
static char *drol_pmva_var_situation();
static char *drol_pmva_var_tdp();
static char *drol_pmva_var_direction();
static char *drol_pmva_var_traversee();
static char *drol_pmva_var_cause();

static char *drol_pmva_majuscules(char *);

/* definition des variables locales au module */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des donnees de config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_pmva_init()

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
  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : drol_pmva_init");
  vm_noeud2=(T_NOEUD *)malloc(sizeof(T_NOEUD));

  vg_pert_pmva=NULL;
  drol_pmva_charge();

  XZST_03EcritureTrace(   XZSTC_FONCTION,"OUT drol_pmva_init");

  return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* init des perturbations
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int drol_pmva_charge()

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
  T_PERT_PMVA *pl_liste;
  int	vl_num_prec,vl_num;
  char vl_l1[100],vl_l2[100],vl_l3[100],vl_l4[50],vl_l5[50];
  int	vl_flash;
  int vl_flag;
  char vl_condition[300];
  XDY_Booleen vl_vide;

  XZST_03EcritureTrace(XZSTC_DEBUG1, "IN  : drol_pmva_charge");

  if ( (pl_fp = fopen(C_PERT_PMVA, "r")) == NULL ) {
    XZST_03EcritureTrace(XZSTC_WARNING, "erreur de lecture du fichier %s !" ,C_PERT_PMVA );
    return(XDC_NOK);
  }
  
  /*premiere ligne = commentaires*/
  fgets(vl_Lig,sizeof(vl_Lig),pl_fp) ;
  
  /*memorisation de la structure du message*/
  while (fgets(vl_Lig,sizeof(vl_Lig),pl_fp) != NULL)
  {
      pl_liste = (T_PERT_PMVA *)malloc(sizeof(T_PERT_PMVA));
      vl_str=vl_Lig; 

      sscanf(vl_str,"%[^|]|%d|%d|",pl_liste->Libelle,&(pl_liste->Priorite),&(pl_liste->Type_Zone));

      XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmva_charge : libelle=%s, priorite=%d, type_zone=%d",
					pl_liste->Libelle,pl_liste->Priorite,pl_liste->Type_Zone);

      sprintf(vl_l1,"%d",pl_liste->Priorite);
      sprintf(vl_l2,"%d",pl_liste->Type_Zone);
      vl_str=vl_str+strlen(pl_liste->Libelle)+2+strlen(vl_l1)+strlen(vl_l2)+1;
      
      sscanf(vl_str,"%[^|]|",vl_l1);
      pl_liste->Ligne1=drol_pmva_remplit_ligne(vl_l1);
      vl_str=vl_str+strlen(vl_l1)+1;

      if (vl_str[0]=='|') { strcpy(vl_l2,""); pl_liste->Ligne2=drol_pmva_remplit_ligne(""); vl_str=vl_str+1; }
      else {
        sscanf(vl_str,"%[^|]|",vl_l2);
	pl_liste->Ligne2=drol_pmva_remplit_ligne(vl_l2);vl_str=vl_str+strlen(vl_l2)+1; }
      if (vl_str[0]=='|') { strcpy(vl_l3,""); pl_liste->Ligne3=drol_pmva_remplit_ligne(""); vl_str=vl_str+1; }
      else {
	sscanf(vl_str,"%[^|]|",vl_l3);
	pl_liste->Ligne3=drol_pmva_remplit_ligne(vl_l3); vl_str=vl_str+strlen(vl_l3)+1; }
      if (vl_str[0]=='|') { strcpy(vl_l4,""); pl_liste->Ligne4=drol_pmva_remplit_ligne(""); vl_str=vl_str+1; }
      else {
	sscanf(vl_str,"%[^|]|",vl_l4);
	pl_liste->Ligne4=drol_pmva_remplit_ligne(vl_l4); vl_str=vl_str+strlen(vl_l4)+1; }
      if (vl_str[0]=='|') { strcpy(vl_l5,""); pl_liste->Ligne5=drol_pmva_remplit_ligne(""); vl_str=vl_str+1; }
      else {
	sscanf(vl_str,"%[^|]|",vl_l5);
	pl_liste->Ligne5=drol_pmva_remplit_ligne(vl_l5); vl_str=vl_str+strlen(vl_l5)+1; }
	
      XZST_03EcritureTrace(XZSTC_DEBUG1, "drol_pmva_charge : ligne1=%s, ligne2=%s, ligne3=%s, ligne4=%s, ligne5=%s",
					vl_l1,vl_l2,vl_l3,vl_l4,vl_l5);

      /*flash*/
      if (sscanf(vl_str,"%d|",&vl_flash)!=1) {
      	XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_charge : pb decodage flash %s",vl_str);
      	return (XDC_NOK);	
      }
      pl_liste->Flash = vl_flash;
      sprintf(vl_l1,"%d",vl_flash);
      vl_str = vl_str+strlen(vl_l1)+1;

      sscanf(vl_str,"%[^|]|",vl_condition);
      vl_str=vl_str+strlen(vl_condition)+1;
      pl_liste->Condition = drol_pmva_decode_condition(vl_condition);

      /*flag de recherche du PMVA dans la perturbation*/
      if (sscanf(vl_str,"%d|",&vl_flag)!=1) {
      	XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_charge : pb decodage flag de recherche du PMVA %s",vl_str);
      	return (XDC_NOK);	
      }
      pl_liste->PMVA_Dans_Perturbation = vl_flag;
      sprintf(vl_l1,"%d",vl_flag);
      vl_str = vl_str+strlen(vl_l1)+1;

      /*flag de recherche du mode automatique*/
      if (sscanf(vl_str,"%d|",&vl_flag)!=1) {
      	XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_charge : pb decodage mode automatique %s",vl_str);
      	return (XDC_NOK);	
      }
      pl_liste->Automatique = vl_flag;
      sprintf(vl_l1,"%d",vl_flag);
      vl_str = vl_str+strlen(vl_l1)+1;
     
     /* if (vl_str[0]=='|') { strcpy(vl_l1,""); pl_liste->Alternat1=drol_pmva_remplit_ligne(""); vl_str=vl_str+1; }
      else {
        sscanf(vl_str,"%[^|]|",vl_l1);
	pl_liste->Alternat1=drol_pmva_remplit_ligne(vl_l1);vl_str=vl_str+strlen(vl_l1)+1; }
      if (vl_str[0]=='|') { strcpy(vl_l2,""); pl_liste->Alternat2=drol_pmva_remplit_ligne(""); vl_str=vl_str+1; }
      else {
        sscanf(vl_str,"%[^|]|",vl_l2);
	pl_liste->Alternat2=drol_pmva_remplit_ligne(vl_l2);vl_str=vl_str+strlen(vl_l2)+1; }
      if (vl_str[0]=='|') { strcpy(vl_l3,""); pl_liste->Alternat3=drol_pmva_remplit_ligne(""); vl_str=vl_str+1; }
      else {
	sscanf(vl_str,"%[^|]|",vl_l3);
	pl_liste->Alternat3=drol_pmva_remplit_ligne(vl_l3); vl_str=vl_str+strlen(vl_l3)+1; }
      if (vl_str[0]=='|') { strcpy(vl_l4,""); pl_liste->Alternat4=drol_pmva_remplit_ligne(""); vl_str=vl_str+1; }
      else {
	sscanf(vl_str,"%[^|]|",vl_l4);
	pl_liste->Alternat4=drol_pmva_remplit_ligne(vl_l4); vl_str=vl_str+strlen(vl_l4)+1; }
      if (vl_str[0]=='|') { strcpy(vl_l5,""); pl_liste->Alternat5=drol_pmva_remplit_ligne(""); vl_str=vl_str+1; }
      else {
	sscanf(vl_str,"%[^|]|",vl_l5);
	pl_liste->Alternat5=drol_pmva_remplit_ligne(vl_l5); vl_str=vl_str+strlen(vl_l5)+1; */
	
      pl_liste->Suivant = vg_pert_pmva;
      vg_pert_pmva = pl_liste;
  }
  fclose(pl_fp);


  XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT  : drol_pmva_charge");
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* decodage de la condition
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

T_CONDITION *drol_pmva_decode_condition(char *va_cond)

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
  XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_pmva_decode_condition:  IN %s",va_cond);

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
  pl_cond->Fonction.Comp.Fct=drol_pmva_egal;
  pl_cond->Suivant=NULL;
  pl_tmp=pl_cond;

  /*numero en fonction du type*/
  XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_pmva_decode_condition: type = %s",pl_cond->Libelle);

  strcpy(vl_ch,pl_cond->Libelle);
  if (!strcmp(vl_ch,"ACCIDENT")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Accident;
  else if (!strcmp(vl_ch,"TOUS")) {
	pl_cond->Fonction.Comp.Fct=drol_pmva_true;
	    pl_cond->Fonction.Comp.Valeur=0;
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
  else if (!strcmp(vl_ch,"Regulation")) pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Regulation;

/*traiter le cas particulier du traitement de chaussees*/
  else if (!strcmp(vl_ch,"SALAGE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_TraitChaussees;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pmva_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_TRT_CH_Salage;
  }	

  else if (!strcmp(vl_ch,"DENEIGEMENT")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_TraitChaussees;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pmva_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_TRT_CH_Deneigement;
  }

  /*traiter le cas particulier de la meteo*/
  else if (!strcmp(vl_ch,"VERGLAS")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pmva_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Verglas;
  }
  else if (!strcmp(vl_ch,"CHUTE NEIGE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pmva_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_ChuteNeige;
  }
  else if (!strcmp(vl_ch,"PLUIE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pmva_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Pluie;
  }
  else if (!strcmp(vl_ch,"BROUILLARD GIVRANT")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pmva_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_BrouillardGiv;
  }
  else if (!strcmp(vl_ch,"NEIGE SUR CHAUSSEE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pmva_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_NeigeChaus;
  }
  else if (!strcmp(vl_ch,"BROUILLARD")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pmva_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_Brouillard;
  }
  else if (!strcmp(vl_ch,"VENT")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pmva_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Meteo);
    pl_tmp->Fonction.Comp.Valeur=XZAEC_MET_VentFort;
  }
  else if (!strcmp(vl_ch,"GRELE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_Meteo;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pmva_egal;
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
    pl_tmp->Fonction.Comp.Fct=drol_pmva_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Incident);
    pl_tmp->Fonction.Comp.Valeur=1;
  }
  else if (!strcmp(vl_ch,"PENURIE")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_IncSsConces;
    pl_cond->Suivant = (T_CONDITION *)malloc(sizeof(T_CONDITION));
    pl_cond->Suivant->Suivant=NULL;
    pl_tmp=pl_cond->Suivant;
    pl_tmp->Type=C_TYPE_COMPARATEUR;
    pl_tmp->Fonction.Comp.Fct=drol_pmva_egal;
    pl_tmp->Fonction.Comp.Slot=&(vm_noeud2->Infos_FAX.Type_Incident);
    pl_tmp->Fonction.Comp.Valeur=0;
  }
  /*cas particulier du lit d'arret*/
  else if (!strcmp(vl_ch,"sur lit d'arret")) {
    pl_cond->Fonction.Comp.Slot=&(vm_noeud2->Localisation.TypePointCar);
    pl_cond->Fonction.Comp.Valeur=XDC_POINT_CARACT_LIT_ARRET;
  }
  /*cas particulier du hors bouchons*/
  else if (!strcmp(vl_ch,"HORS BOUCHONS")) {
    pl_cond->Fonction.Comp.Valeur=XZAEC_FMC_QueueBouchon;
    pl_tmp->Fonction.Comp.Fct=drol_pmva_different;
  }
  else {
    XZST_03EcritureTrace(   XZSTC_DEBUG1,"drol_pmva_decode_condition : pb decodage type !!!");
    pl_cond->Fonction.Comp.Valeur=0;
  }
 
  /*si pas de condition supplementaire*/
  if (vl_indice==strlen(va_cond)) {
    XZST_03EcritureTrace(   XZSTC_DEBUG1, "OUT : drol_pmva_decode_condition : que le type");
    return (pl_cond);
  }

  /*supprimer $ET$*/
  va_cond=va_cond+strlen(pl_cond->Libelle)+4;

  strcpy(vl_ch_init,va_cond);
  XZST_03EcritureTrace(   XZSTC_DEBUG1,"drol_pmva_decode_condition : apres le type = %s",vl_ch_init);

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

    XZST_03EcritureTrace(   XZSTC_DEBUG1, "drol_pmva_decode_condition:  cond %s",vl_str);
    strcpy(pl_tmp->Libelle,vl_str);

    /*il s'agit d'une fonction PREDICAT*/
    pl_tmp->Type=C_TYPE_PREDICAT;
    if (!strcmp(vl_str,C_COND_UNE_VOIE_BLOQ)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_une_voie_bloquee;
    else if (!strcmp(vl_str,C_COND_UNE_VOIE_OUVERTE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_une_voie_ouverte;
    else if (!strcmp(vl_str,C_COND_TOUTES_VOIES_BLOQ)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_toutes_voies_bloquees;
    else if (!strcmp(vl_str,C_COND_TOUTES_VOIES_NEUT)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_toutes_voies_neutralisees;
    else if (!strcmp(vl_str,C_COND_UNE_VOIE_NEUT)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_une_voie_neutralisee;
    else if (!strcmp(vl_str,C_COND_CIRCULATION_UNE_VOIE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_circulation_une_voie;
    else if (!strcmp(vl_str,C_COND_CIRCULATION_BASC)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_circulation_basculee;
    else if (!strcmp(vl_str,C_COND_CONFIRME)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_confirme;
    else if (!strcmp(vl_str,C_COND_SIGNALE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_signale;
    else if (!strcmp(vl_str,C_COND_SUR_BAU)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_sur_bau;
    else if (!strcmp(vl_str,C_COND_SUR_SECTION_COURANTE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_sur_section_courante;
    else if (!strcmp(vl_str,C_COND_SUR_LIT_ARRET)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_sur_lit_arret;
    else if (!strcmp(vl_str,C_COND_SUR_ECHANGEUR)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_sur_echangeur;
    else if (!strcmp(vl_str,C_COND_SUR_BRETELLE_ENTREE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_sur_bretelle_entree;
    else if (!strcmp(vl_str,C_COND_SUR_BRETELLE_SORTIE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_sur_bretelle_sortie;
    else if (!strcmp(vl_str,C_COND_TRAFIC_DENSE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_trafic_dense;
    else if (!strcmp(vl_str,C_COND_TRAFIC_RALENTI)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_trafic_ralenti;
    else if (!strcmp(vl_str,C_COND_TRAFIC_BOUCHE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_trafic_bouche;
    else if (!strcmp(vl_str,C_COND_TRAFIC_BLOQUE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_trafic_bloque;
    else if (!strcmp(vl_str,C_COND_TRAFIC_ACCORDEON)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_trafic_accordeon;
    else if (!strcmp(vl_str,C_COND_DATEX)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_datex;
    else if (!strcmp(vl_str,C_COND_VOIES_RETRECIES)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_voies_retrecies;
    else if (!strcmp(vl_str,C_COND_ENTREE_FERMEE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_entree_fermee;
    else if (!strcmp(vl_str,C_COND_SORTIE_FERMEE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_sortie_fermee;
    else if (!strcmp(vl_str,C_COND_BOUCHON_MOINS_3_KM)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_bouchon_moins_3_km;
    else if (!strcmp(vl_str,C_COND_BOUCHON_ENTRE_3_ET_5_KM)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_bouchon_entre_3_et_5_km;
    else if (!strcmp(vl_str,C_COND_BOUCHON_PLUS_5_KM)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_bouchon_plus_5_km;    
    else if (!strcmp(vl_str,C_COND_PREVU)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_prevu;    
    else if (!strcmp(vl_str,C_COND_NON_PREVU)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_non_prevu;    
    else if (!strcmp(vl_str,C_COND_TOUTES_VOIES_FERMEES)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_toutes_voies_fermees;    
    else if (!strcmp(vl_str,C_COND_TRAVAUX_ENTRETIEN)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_travaux_entretien;    
    else if (!strcmp(vl_str,C_COND_STOCKAGE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_stockage;    
    else if (!strcmp(vl_str,C_COND_AIRE_SERVICE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_aire_service;    
    else if (!strcmp(vl_str,C_COND_AIRE_REPOS)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_aire_repos;    
    else if (!strcmp(vl_str,C_COND_FEU_MAITRISE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_feu_maitrise;
    else if (!strcmp(vl_str,C_COND_FEU_NON_MAITRISE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_feu_non_maitrise;
    else if (!strcmp(vl_str,C_COND_CAUSE_EVENEMENT)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_cause_evenement;
    else if (!strcmp(vl_str,C_COND_CAUSE_OPERATION)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_cause_operation;
    else if (!strcmp(vl_str,C_COND_STOCKAGE_DEMARRE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_stockage_demarre;
    else if (!strcmp(vl_str,C_COND_STOCKAGE_NON_DEMARRE)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_stockage_non_demarre;
    else if (!strcmp(vl_str,C_COND_MAJEUR)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_majeur;
    else if (!strcmp(vl_str,C_COND_FERMETURE_TOULON)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_fermeture_toulon;
    else if (!strcmp(vl_str,C_COND_INTERVENTION)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_intervention;
    else if (!strcmp(vl_str,C_COND_PERSONNEL)) pl_tmp->Fonction.Predicat.Fct=drol_pmva_personnel;
	else if (!strcmp(vl_str,C_COND_CIRCULATION_BASC_TUNNEL)) pl_tmp->Fonction.Predicat.Fct = drol_pmva_circulation_basculee_tunnel;
	else if (!strcmp(vl_str,C_COND_CIRCULATION_NON_BASC_TUNNEL)) pl_tmp->Fonction.Predicat.Fct = drol_pmva_circulation_non_basculee_tunnel;

  /*si fin de chaine*/
  if (vl_indice==strlen(va_cond)) {
    XZST_03EcritureTrace(   XZSTC_DEBUG1,"drol_pmva_decode_condition : fin de chaine");
    XZST_03EcritureTrace(XZSTC_WARNING,"Fin cond1");
    return (pl_cond);
  }
  else {
    va_cond=va_cond+strlen(vl_str)+4;
    goto pos_decode;
  }

}
static XDY_Booleen drol_pmva_majeur() {
  if (vm_noeud->Infos_TFM.Majeur==XDC_VRAI)
      return (XDC_VRAI);
  return (XDC_FAUX);
}


/**************************************/
/**************************************/
static XDY_Booleen drol_pmva_intervention() {
  return (vm_noeud->Infos_FAX.Agent_Escota);
}
static XDY_Booleen drol_pmva_personnel() {
  return (vm_noeud->Infos_FAX.Personnel!=0);
}


static XDY_Booleen drol_pmva_fermeture_toulon() {
  if (vm_noeud->Type != XZAEC_FMC_Deviation) return (FALSE);
  if (vm_noeud->Localisation.TypePointCar != XDC_POINT_CARACT_ECHANGEUR) return (FALSE);
  /*localisation sur l'échangeur immediatement en amont du tunnel*/
  /* vm_noeud->Localisation.NumPointCar*/
  if (vm_noeud->Localisation.NumPointCar == 217) return (TRUE);
  if (vm_noeud->Localisation.NumPointCar==218) return (TRUE);
  return (FALSE);

}
static XDY_Booleen drol_pmva_sur_echangeur() {
  return (vm_noeud->Localisation.TypePointCar==XDC_POINT_CARACT_ECHANGEUR);
}
static XDY_Booleen drol_pmva_sur_lit_arret() {
  return (vm_noeud->Localisation.TypePointCar==XDC_POINT_CARACT_LIT_ARRET);
}

static XDY_Booleen drol_pmva_egal(int va_valeur1, int va_valeur2) { return (va_valeur1==va_valeur2); }
static XDY_Booleen drol_pmva_different(int va_valeur1, int va_valeur2) { return (va_valeur1!=va_valeur2); }

static XDY_Booleen drol_pmva_true(int va_valeur1, int va_valeur2) { return (TRUE); }
/*
static XDY_Booleen drol_pmva_superieur(int va_valeur1, int va_valeur2) { return (va_valeur1>va_valeur2); }
static XDY_Booleen drol_pmva_inferieur(int va_valeur1, int va_valeur2) { return (va_valeur1<va_valeur2); }
*/

static XDY_Booleen drol_pmva_confirme() { return (vm_noeud->Infos_TFM.Confirme); }
static XDY_Booleen drol_pmva_signale() { return (vm_noeud->Infos_TFM.Signale); }

static XDY_Booleen drol_pmva_fmc_simulee() {
  return (strstr (vm_noeud->Infos_TFM.Origine, "ESSAI") != NULL  ||
          strstr (vm_noeud->Infos_TFM.Origine, "EXERCICE") != NULL);
}


static XDY_Booleen drol_pmva_une_voie_bloquee() {
  return ((vm_noeud->Localisation.VL==XDC_VOIE_BLOQUEE) ||
          (vm_noeud->Localisation.VM1==XDC_VOIE_BLOQUEE) ||
	  (vm_noeud->Localisation.VM2==XDC_VOIE_BLOQUEE) ||
	  (vm_noeud->Localisation.VR==XDC_VOIE_BLOQUEE)
	 );
}
static XDY_Booleen drol_pmva_une_voie_ouverte() {
  return ((vm_noeud->Localisation.VL==XDC_VOIE_SENS_NORMAL) ||
	  (vm_noeud->Localisation.VM1==XDC_VOIE_SENS_NORMAL) ||
	  (vm_noeud->Localisation.VM2==XDC_VOIE_SENS_NORMAL) ||
	  (vm_noeud->Localisation.VR==XDC_VOIE_SENS_NORMAL)
	 );
}
static XDY_Booleen drol_pmva_toutes_voies_bloquees() {

  return (( (vm_noeud->Localisation.VL!=XDC_VOIE_INCONNUE) &&
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
	 ) && (vm_noeud->Type!=XZAEC_FMC_BasculTravaux) && (vm_noeud->Type!=XZAEC_FMC_Basculement));
}

static XDY_Booleen drol_pmva_une_voie_neutralisee() { 
  return ((vm_noeud->Localisation.VL==XDC_VOIE_NEUTRALISEE) ||
	  (vm_noeud->Localisation.VM1==XDC_VOIE_NEUTRALISEE) ||
	  (vm_noeud->Localisation.VM2==XDC_VOIE_NEUTRALISEE) ||
	  (vm_noeud->Localisation.VR==XDC_VOIE_NEUTRALISEE)
         );
}
       
static XDY_Booleen drol_pmva_toutes_voies_neutralisees() { 
  return ( (vm_noeud->Localisation.BAU!=XDC_VOIE_SENS_NORMAL) &&
	   (vm_noeud->Localisation.VR_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VM2_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VM1_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VL_I!=XDC_VOIE_SENS_INVERSE) &&
	   (drol_pmva_voies(vm_noeud->Localisation.VL,
			  vm_noeud->Localisation.VM1,
			  vm_noeud->Localisation.VM2,
			  vm_noeud->Localisation.VR,
			  XDC_VOIE_NEUTRALISEE)==15)
         );
}

static XDY_Booleen drol_pmva_toutes_voies_bloquees_neutralisees() { 
  return ( (vm_noeud->Localisation.BAU!=XDC_VOIE_SENS_NORMAL) &&
	   (vm_noeud->Localisation.VR_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VM2_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VM1_I!=XDC_VOIE_SENS_INVERSE) &&
	   (vm_noeud->Localisation.VL_I!=XDC_VOIE_SENS_INVERSE) &&
	   (drol_pmva_voies(vm_noeud->Localisation.VL,
			  vm_noeud->Localisation.VM1,
			  vm_noeud->Localisation.VM2,
			  vm_noeud->Localisation.VR,
			  XDC_VOIE_BLOQUEE_NEUTRALISEE)==15)
         );
}


static XDY_Booleen drol_pmva_toutes_voies_fermees() {
XZST_03EcritureTrace(XZSTC_WARNING,"vm_noeud->Type=%d",vm_noeud->Type);
	return ( (drol_pmva_toutes_voies_neutralisees() || drol_pmva_toutes_voies_bloquees_neutralisees()) && (vm_noeud->Type!=XZAEC_FMC_BasculTravaux) && (vm_noeud->Type!=XZAEC_FMC_Basculement) );
}

static XDY_Booleen drol_pmva_circulation_une_voie() {
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


static XDY_Booleen drol_pmva_circulation_basculee() {
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

static XDY_Booleen drol_pmva_sur_bau() { 
  return ( (drol_pmva_voies(vm_noeud->Localisation.VL,vm_noeud->Localisation.VM1,
			   vm_noeud->Localisation.VM2,
			   vm_noeud->Localisation.VR,
			   XDC_VOIE_SENS_NORMAL)==15) &&
           ((vm_noeud->Localisation.BAU==XDC_VOIE_BLOQUEE) ||
	    (vm_noeud->Localisation.BAU==XDC_VOIE_NEUTRALISEE)||
	    (vm_noeud->Localisation.BAU==XDC_VOIE_BLOQUEE_NEUTRALISEE)
           )
         );
}

static XDY_Booleen drol_pmva_sur_section_courante() { 
  return ( (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_AIRE) &&
	   (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_ECHANGEUR) &&
	   (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_PEAGE) &&
	   (vm_noeud->Localisation.TypePointCar!=XDC_POINT_CARACT_LIT_ARRET)
         );
}
static XDY_Booleen drol_pmva_sur_bretelle_entree() { 
  return (  (  (vm_noeud->Localisation.Sens==XDC_SENS_ENTREE_SUD) ||
	       (vm_noeud->Localisation.Sens==XDC_SENS_ENTREE_NORD)
	    )
	    &&
	    (vm_noeud->Localisation.Bifurcation != XDC_VRAI)
         );
}

static XDY_Booleen drol_pmva_sur_bretelle_sortie() { 
  return (  (      (vm_noeud->Localisation.Sens==XDC_SENS_SORTIE_SUD) ||
	           (vm_noeud->Localisation.Sens==XDC_SENS_SORTIE_NORD)
	    )
	    &&
            (vm_noeud->Localisation.Bifurcation != XDC_VRAI)
         );
}

static XDY_Booleen drol_pmva_bouchon_moins_3_km() {
  if (vm_mode_dans_pert)
    return (TRUE);
  if (vm_mode_verif_bouchon) {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_bouchon_moins_3_km %d",vm_pmv.Distance_C);
    return (vm_pmv.Distance_C <= 3000);
  }
  else
  return (vm_noeud->Localisation.Longueur<=3000);}
  
static XDY_Booleen drol_pmva_bouchon_plus_5_km() {
  if (vm_mode_dans_pert)
    return (TRUE);

  if (vm_mode_verif_bouchon) {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_bouchon_plus_5_km %d",vm_pmv.Distance_C);
    return (vm_pmv.Distance_C >= 5000);
  }
  else
  return (vm_noeud->Localisation.Longueur >=5000); }

static XDY_Booleen drol_pmva_bouchon_entre_3_et_5_km() {
  if (vm_mode_dans_pert)
    return (TRUE);

  return (!drol_pmva_bouchon_moins_3_km() && !drol_pmva_bouchon_plus_5_km());}
	
static XDY_Booleen drol_pmva_trafic_dense() { 
  return (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Dense); }

static XDY_Booleen drol_pmva_trafic_ralenti() { 
  return (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Ralenti); }

static XDY_Booleen drol_pmva_trafic_bouche() { 
  return (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Bouche); }

static XDY_Booleen drol_pmva_trafic_accordeon() { 
  return (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Accordeon); }

static XDY_Booleen drol_pmva_trafic_bloque() { 
  return (vm_noeud->Infos_FAX.Type_Bouchon==XZAEC_BOU_Bloque); }

static XDY_Booleen drol_pmva_datex() { return (FALSE); }

static XDY_Booleen drol_pmva_voies_retrecies() { return (vm_noeud->Localisation.Voies_Retrecies); }

static XDY_Booleen drol_pmva_entree_fermee() { 
  return ( (vm_noeud->Infos_FAX.Entree1) || (vm_noeud->Infos_FAX.Entree2) );
}

static XDY_Booleen drol_pmva_sortie_fermee() { 
  return ( (vm_noeud->Infos_FAX.Sortie1) || (vm_noeud->Infos_FAX.Sortie2) );
}
static XDY_Booleen drol_pmva_prevu() { 
XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_prevu retourne : %d",((vm_noeud->Infos_TFM.Prevu)));
  return (vm_noeud->Infos_TFM.Prevu) ;
}
static XDY_Booleen drol_pmva_non_prevu() { 
XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_non_prevu retourne : %d",(!(vm_noeud->Infos_TFM.Prevu)));
  return (!(vm_noeud->Infos_TFM.Prevu) );
}
static XDY_Booleen drol_pmva_travaux_entretien() {
  return (vm_noeud->Infos_TFM.Nature_Travaux==0);
}
static XDY_Booleen drol_pmva_stockage() {
  return (vm_noeud->Infos_TFM.Type_Travaux==24);
}
static XDY_Booleen drol_pmva_aire_service() {
  return (vm_noeud->Infos_TFM.Aire_Service);
}
static XDY_Booleen drol_pmva_aire_repos() {
  return !(vm_noeud->Infos_TFM.Aire_Service);
}

static XDY_Booleen drol_pmva_feu_maitrise() { 
  return (vm_noeud->Infos_FAX.Feu_Maitrise != XDC_FAUX);
}

static XDY_Booleen drol_pmva_feu_non_maitrise() { 
  return (vm_noeud->Infos_FAX.Feu_Maitrise == XDC_FAUX);
}

static XDY_Booleen drol_pmva_cause_operation() { 
	return (vm_noeud->Infos_FAX.Cause_Operation != XDC_FAUX);
}


static XDY_Booleen drol_pmva_cause_evenement() { 
  return (vm_noeud->Infos_FAX.Cause_Operation == XDC_FAUX);
}


static XDY_Booleen drol_pmva_stockage_demarre() { 
  return (vm_noeud->Localisation.Voies_Retrecies);
}

static XDY_Booleen drol_pmva_stockage_non_demarre() { 
  if (vm_noeud->Localisation.Voies_Retrecies == XDC_VRAI) {
    return (XDC_FAUX);
  }
  return (XDC_VRAI);
}

static XDY_Booleen drol_pmva_circulation_basculee_tunnel ()
{
	XDY_Booleen vl_basculement = XDC_FAUX;
	
	XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_circulation_basculee_tunnel : avant XZAE861_Basculement_Tunnel");

	if (XZAE861_Basculement_Tunnel (vm_noeud->Localisation.Autoroute,
									vm_noeud->Localisation.PR,
									vm_noeud->Localisation_Tete.PR,
									vl_basculement)	!= XDC_OK) 
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_circulation_basculee_tunnel : pb appel a XZAE861_Basculement_Tunnel");
		return (FALSE);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_circulation_basculee_tunnel : XZAE861_Basculement_Tunnel retourne %s", vl_basculement);
	}
	
	return (vl_basculement);
}

static XDY_Booleen drol_pmva_circulation_non_basculee_tunnel ()
{
	XZST_03EcritureTrace (XZSTC_WARNING, "drol_pmva_circulation_non_basculee_tunnel : not drol_pmva_circulation_basculee_tunnel");
	return (!drol_pmva_circulation_basculee_tunnel());
}

/**************************************/
/**************************************/
/*fonctions des variables dans les messages*/
static char *drol_pmva_var_longueur() {
T_Localisation vl_loc;
T_Localisation vl_loc_tete;
static char vl_str[150];
int vl_inversee;
int vl_longueur_bouchon_km_arrondi;
int vl_longueur_fin_bouchon_km_arrondi;

  vl_loc=vm_noeud->Localisation;
  vl_loc_tete=vm_noeud->Localisation_Tete;
  strcpy(vl_str,"");

  vl_inversee=1;
  if (vl_loc.Autoroute==8) vl_inversee=-1;
  vm_indice_bouchon=vm_ligne_courante;
  /*cas d'un PMV en amont de la perturbation*/
  /* REPERE1 remplacer et < et > par <= et >= */
XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_var_longueur vm_pmv.Sens:%d;vm_pmv.PR=%d;vl_loc.PR=%d",vm_pmv.Sens,vm_pmv.PR,vl_loc.PR);
  if ( ( ( ((vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD) || (vm_pmv.Sens==XDC_SENS_SORTIE_SUD)) &&
	  (vm_pmv.PR*vl_inversee <= vl_loc.PR*vl_inversee)
         )
         ||
         (( (vm_pmv.Sens==XDC_SENS_NORD) || (vm_pmv.Sens==XDC_SENS_ENTREE_NORD) || (vm_pmv.Sens==XDC_SENS_SORTIE_NORD)) &&
	    (vm_pmv.PR*vl_inversee >= vl_loc.PR*vl_inversee)
         )
       )
       ||
       (vm_pmv.Autoroute != vl_loc.Autoroute)
     )
  {
    if (vl_loc.Longueur < 2000) 
      sprintf(vl_str,"");
    else  {
      /* on arrondit la longueur du bouchon a l'entier le plus proche */
      if( ( vl_loc.Longueur % 1000 ) >=  500)
	    vl_longueur_bouchon_km_arrondi = (int)(vl_loc.Longueur /1000) + 1 ;
      else
	  vl_longueur_bouchon_km_arrondi = (int)(vl_loc.Longueur /1000);

      if (vl_longueur_bouchon_km_arrondi > 9)
        sprintf(vl_str,"de %dkm", vl_longueur_bouchon_km_arrondi);
      else
	sprintf(vl_str,"de %d km", vl_longueur_bouchon_km_arrondi);
      vm_presence_nature_bouchon=TRUE;
    }

XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_var_longueur vl_str:%s",vl_str);
    return (vl_str);
  }

  /*cas d'un PMV dans la perturbation*/
  /* REPERE1 remplacer et < et > par <= et >= */
  if ( ( ( (( vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD) || (vm_pmv.Sens==XDC_SENS_SORTIE_SUD)) &&
           (vm_pmv.PR*vl_inversee >= vl_loc.PR*vl_inversee)
         )
         ||
         ((  (vm_pmv.Sens==XDC_SENS_NORD)  || (vm_pmv.Sens==XDC_SENS_ENTREE_NORD) || (vm_pmv.Sens==XDC_SENS_SORTIE_NORD)) &&
         (vm_pmv.PR*vl_inversee <= vl_loc.PR*vl_inversee)
         )
       )
       &&
       (vm_pmv.Autoroute==vl_loc.Autoroute)
     )
  {
    if (vm_pmv.Distance_C <= 500) 
      {
      if (vm_pmv.Distance_C >0) 
	sprintf(vl_str,"de %dM",
                ((int)(vm_pmv.Distance_C/100))*100);
	}
    else
    {
      /* REPERE 2 : on arrondit la longueur du bouchon a l'entier le plus proche */
      if( ( vm_pmv.Distance_C % 1000 ) >=  500)
        vl_longueur_fin_bouchon_km_arrondi = (int)(vm_pmv.Distance_C /1000) + 1 ;
      else
        vl_longueur_fin_bouchon_km_arrondi = (int)(vm_pmv.Distance_C /1000);
/* Replacement de SUR par DE PNI 10/05/07 */
      if (vl_longueur_fin_bouchon_km_arrondi > 9)
        sprintf(vl_str,"de %dkm", vl_longueur_fin_bouchon_km_arrondi);
      else
	sprintf(vl_str,"de %d km", vl_longueur_fin_bouchon_km_arrondi);
      vm_presence_nature_bouchon=TRUE;
    }
XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_var_longueur vl_str:%s",vl_str);
    return (vl_str);
  }

XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_var_longueur vl_str:%s",vl_str);


  return (vl_str);
}

static char *drol_pmva_var_cause() {
static char vl_str[20];

  strcpy(vl_str,"");
  switch (vm_noeud->Infos_TFM.Type_Cause) {
    case (XZAEC_FMC_MalaiseUsager):
    case (XZAEC_FMC_Accident): strcpy(vl_str,"ACCIDENT");
                        break;
    case (XZAEC_FMC_VehMarcheArr):
    case (XZAEC_FMC_Contresens): strcpy(vl_str,"CONTRESENS");
                        break;
    case (XZAEC_FMC_VehPanne):
    case (XZAEC_FMC_VehArrete): strcpy(vl_str,"VEH ARRETE");
                        break;
    case (XZAEC_FMC_VehFeu): strcpy(vl_str,"VEH EN FEU");
                        break;
    case (XZAEC_FMC_ConvoiExc):
    case (XZAEC_FMC_ConvoiMil):
    case (XZAEC_FMC_Cirque):
    case (XZAEC_FMC_VehLent): strcpy(vl_str,"CONVOI LENT");
                        break;
    case (XZAEC_FMC_Manifestation):
    case (XZAEC_FMC_GreveESCOTA):
    case (XZAEC_FMC_Escargot): strcpy(vl_str,"MANIFESTATION");
                        break;
    case (XZAEC_FMC_Travaux) : strcpy(vl_str,"TRAVAUX");
                        break;
    case (XZAEC_FMC_QueueBouchon): strcpy(vl_str,"BOUCHON");
                        break;
    case (XZAEC_FMC_Eboulement): strcpy(vl_str,"EBOULEMENT");
                        break;
    case (XZAEC_FMC_Incendie): strcpy(vl_str,"INCENDIE");
                        break;
    case (XZAEC_FMC_Inondation): strcpy(vl_str,"INONDATION");
                        break;
    case (XZAEC_FMC_Obstacle): strcpy(vl_str,"OBSTACLE");
                        break;
    case (XZAEC_FMC_ChausseDeter): strcpy(vl_str,"CHAUSSEE ABIMEE");
                        break;
    case (XZAEC_FMC_AnimalErrant): strcpy(vl_str,"ANIMAL ERRANT");
                        break;
    case (XZAEC_FMC_AltercationUsager):
    case (XZAEC_FMC_HoldUpVol):
    case (XZAEC_FMC_Agression): strcpy(vl_str,"INCIDENT");
                        break;
   case (XZAEC_FMC_PietonChaussee): strcpy(vl_str,"PIETONS");
                                break;
    case (XZAEC_FMC_ChausseeGliss): strcpy(vl_str,"NETTOYAGE VOIES");
                                break;
    case (XZAEC_FMC_Meteo):
        if (vm_noeud2->Infos_FAX.Type_Meteo==XZAEC_MET_Verglas) strcpy(vl_str,"VERGLAS");
        else if (vm_noeud2->Infos_FAX.Type_Meteo==XZAEC_MET_ChuteNeige) strcpy(vl_str,"NEIGE");
        else if (vm_noeud2->Infos_FAX.Type_Meteo=XZAEC_MET_Pluie) strcpy(vl_str,"PLUIE");
        else if (vm_noeud2->Infos_FAX.Type_Meteo=XZAEC_MET_BrouillardGiv) strcpy(vl_str,"BROUILLARD");
        else if (vm_noeud2->Infos_FAX.Type_Meteo=XZAEC_MET_NeigeChaus) strcpy(vl_str,"NEIGE");
        else if (vm_noeud2->Infos_FAX.Type_Meteo=XZAEC_MET_Brouillard) strcpy(vl_str,"BROUILLARD");
        else if (vm_noeud2->Infos_FAX.Type_Meteo=XZAEC_MET_VentFort) strcpy(vl_str,"VENT FORT");
        else if (vm_noeud2->Infos_FAX.Type_Meteo=XZAEC_MET_Grele) strcpy(vl_str,"GRELE");

    default : strcpy(vl_str,"");
                break;
  }
  return (vl_str);

}

static char *drol_pmva_var_dest_un_proche() {

  XZADT_CONFIG_PMA pl_pma;
  XDY_PR vl_pr_min;
  XDY_PR vl_pr_max;
  XDY_TDP vl_tdp;
  static char vl_str[50];
  int vl_longueur;
  XDY_Nom               vl_libelle;
  char vl_nom[15];

  vm_dest1proche_HS=TRUE;
  if (vm_noeud->Localisation.PR < vm_noeud->Localisation_Tete.PR) {
        vl_pr_min = vm_noeud->Localisation.PR;
        vl_pr_max = vm_noeud->Localisation_Tete.PR;
  }
  else {
        vl_pr_max = vm_noeud->Localisation.PR;
        vl_pr_min = vm_noeud->Localisation_Tete.PR;
  }
XZST_03EcritureTrace(XZSTC_WARNING,"avant XZAD_recherche_dest_pmva %d %d %d",
			vm_noeud->Localisation.Sens,
                                vm_noeud->Localisation.Autoroute,
                                vm_noeud->Localisation_Tete.PR);
  /*on commence par chercher la destination associe au PMVA*/
  /* = premiere destination en aval*/
  vm_pmv.Dest_un_proche=0;
  vm_dest1proche_numero=0;

  if (vm_pmv.PMVA_SAGA) 
    return ("");

  if ( ((vm_pmv.Autoroute==vm_noeud->Localisation.Autoroute) || (vm_pmv.Autoroute!=8)) && (XZAD_recherche_dest_pmva(vm_pmv.Identifiant, &pl_pma, vm_noeud->Localisation.Sens,
				vm_noeud->Localisation.Autoroute,
				vm_noeud->Localisation_Tete.PR)!=XDC_OK))
    return ("");
  /*on calcule le tdp entre le PMV et cette destination*/
	XZST_03EcritureTrace(XZSTC_WARNING,"avant XZAD23_Calculer_TdP_PMV_Dest %d",pl_pma.Dest_un_proche); 
  if (XZAD23_Calculer_TdP_PMV_Dest(     vm_pmv.PR,
                          vm_pmv.Autoroute,
                          vm_pmv.Sens,
                          pl_pma.Dest_un_proche,
                          pl_pma.Graphe_Dest_un_proche,
                          &vl_tdp, vl_nom,vm_pmv.Identifiant,XDC_EQT_PMVA) != XDC_OK)	{
      if (vm_noeud->Localisation.Longueur > 3000)
      	return (drol_pmva_var_longueur());
      else	
      	return ("");
  }

  	XZST_03EcritureTrace(XZSTC_WARNING,"TDP %d",vl_tdp); 
   if (vl_tdp<=0) {
	/* Si pas de TDP alors localisation ou longueur si bouchon > 3km)*/
      if (vm_noeud->Localisation.Longueur > 3000)
      	return (drol_pmva_var_longueur());
      else	
      	return ("");
   }

   vm_pmv.Dest_un_proche=pl_pma.Dest_un_proche;
   vm_dest1proche_numero=pl_pma.Dest_un_proche;

   /*TDP DOIT ETRE CADRE A DROITE*/
   if (XZAD27_Libelle_Destination(pl_pma.Dest_un_proche,vl_libelle) != XDC_OK) {
      XZST_03EcritureTrace(XZSTC_WARNING,"drol_pma_var_Dest_un_proche: pb dans l'appel a XZAD27_Libelle_Destination");
	/* Si pas de TDP alors localisation ou longueur si bouchon > 3km) */
      if (vm_noeud->Localisation.Longueur > 3000)
      	return (drol_pmva_var_longueur());
      else	
      	return ("");
   }

   XZST_03EcritureTrace(XZSTC_WARNING,"MAX2 %d",vm_pmv.Dest_un_proche);
   if (vl_tdp > 59) {
/*	sprintf(vl_str,"%1dh%02d JUSQU'A\n%s",vl_tdp/60, vl_tdp%60,vl_libelle);*/
 	sprintf(vl_str,"%s           ",vl_libelle);
	sprintf(&vl_str[11],"%1dh%02d",vl_tdp/60, vl_tdp%60);
  }
  else {
/*     sprintf(vl_str,"%2dMN JUSQU'A\n%s", vl_tdp, vl_libelle);*/
     sprintf(vl_str,"%s           ",vl_libelle);
	sprintf(&vl_str[11],"%2dmn", vl_tdp);
   }
   vm_dest1proche_HS=FALSE;
  XZST_03EcritureTrace(XZSTC_WARNING,"vl_str = %s",vl_str);
  return (vl_str);
}

static char *drol_pmva_var_dest_un_proche_long() {

  XZADT_CONFIG_PMA pl_pma;
  XDY_PR vl_pr_min;
  XDY_PR vl_pr_max;
  XDY_TDP vl_tdp;
  static char vl_str[50];
  int vl_longueur;
  XDY_Nom               vl_libelle;
  char vl_nom[15];
  	

  vm_dest1proche_HS=TRUE;
  if (vm_noeud->Localisation.PR < vm_noeud->Localisation_Tete.PR) {
        vl_pr_min = vm_noeud->Localisation.PR;
        vl_pr_max = vm_noeud->Localisation_Tete.PR;
  }
  else {
        vl_pr_max = vm_noeud->Localisation.PR;
        vl_pr_min = vm_noeud->Localisation_Tete.PR;
  }
XZST_03EcritureTrace(XZSTC_WARNING,"proche long avant XZAD_recherche_dest_pmva %d %d %d",
			vm_noeud->Localisation.Sens,
                                vm_noeud->Localisation.Autoroute,
                                vm_noeud->Localisation_Tete.PR);
  /*on commence par chercher la destination associe au PMVA*/
  /* = premiere destination en aval*/
  vm_pmv.Dest_un_proche=0;
  vm_dest1proche_numero=0;

  if (vm_pmv.PMVA_SAGA) 
    return (drol_pmva_var_localisation_km_long());

  if (XZAD_recherche_dest_pmva(vm_pmv.Identifiant, &pl_pma, vm_noeud->Localisation.Sens,
				vm_noeud->Localisation.Autoroute,
				vm_noeud->Localisation_Tete.PR)!=XDC_OK)
   return (drol_pmva_var_localisation_km_long());

  /*on calcule le tdp entre le PMV et cette destination*/
	XZST_03EcritureTrace(XZSTC_WARNING,"avant XZAD23_Calculer_TdP_PMV_Dest %d",pl_pma.Dest_un_proche); 
  if (XZAD23_Calculer_TdP_PMV_Dest(     vm_pmv.PR,
                          vm_pmv.Autoroute,
                          vm_pmv.Sens,
                          pl_pma.Dest_un_proche,
                          pl_pma.Graphe_Dest_un_proche,
                          &vl_tdp, vl_nom,vm_pmv.Identifiant,XDC_EQT_PMVA) != XDC_OK)	{
	
      	return (drol_pmva_var_localisation_km_long());
  }

  	XZST_03EcritureTrace(XZSTC_WARNING,"TDP %d",vl_tdp); 
   if (vl_tdp<=0) {
	/* Si pas de TDP alors localisation ou longueur si bouchon > 3km)*/
      	return (drol_pmva_var_localisation_km_long());
   }

   vm_pmv.Dest_un_proche=pl_pma.Dest_un_proche;
   vm_dest1proche_numero=pl_pma.Dest_un_proche;

   /*TDP DOIT ETRE CADRE A DROITE*/
   if (XZAD27_Libelle_Destination(pl_pma.Dest_un_proche,vl_libelle) != XDC_OK) {
      XZST_03EcritureTrace(XZSTC_WARNING,"drol_pma_var_Dest_un_proche: pb dans l'appel a XZAD27_Libelle_Destination");
	/* Si pas de TDP alors localisation ou longueur si bouchon > 3km) */
      	return (drol_pmva_var_localisation_km_long());
   }

   XZST_03EcritureTrace(XZSTC_WARNING,"MAX2 %d",vm_pmv.Dest_un_proche);
   if (vl_tdp > 59) {
/*	sprintf(vl_str,"%1dh%02d JUSQU'A\n%s",vl_tdp/60, vl_tdp%60,vl_libelle);*/
 	sprintf(vl_str,"%s           ",vl_libelle);
	sprintf(&vl_str[11],"%1dh%02d",vl_tdp/60, vl_tdp%60);
  }
  else {
/*     sprintf(vl_str,"%2dMN JUSQU'A\n%s", vl_tdp, vl_libelle);*/
     sprintf(vl_str,"%s           ",vl_libelle);
	sprintf(&vl_str[11],"%2dmn", vl_tdp);
   }
   vm_dest1proche_HS=FALSE;

  /*
  if (  (vm_noeud->Type == XZAEC_FMC_QueueBouchon) && 
       (vm_noeud->Infos_TFM.Type_Cause !=0) &&
       (vm_noeud->Infos_TFM.Type_Cause != XZAEC_FMC_TeteBouchon) &&
	(vm_noeud->Infos_TFM.Type_Cause  != XZAEC_FMC_QueueBouchon) &&
	(vm_noeud->Infos_TFM.Type_Cause != XZAEC_FMC_Autre))
   strcpy(vl_str,drol_pmva_var_traversee());*/

  XZST_03EcritureTrace(XZSTC_WARNING,"vl_str = %s",vl_str);
  return (vl_str);
}

static char *drol_pmva_var_dest_un_proche_loc() {

  XZADT_CONFIG_PMA pl_pma;
  XDY_PR vl_pr_min;
  XDY_PR vl_pr_max;
  XDY_TDP vl_tdp;
  static char vl_str[50];
  int vl_longueur;
  XDY_Nom               vl_libelle;
  char vl_nom[15];

  vm_dest1proche_HS=TRUE;
  if (vm_noeud->Localisation.PR < vm_noeud->Localisation_Tete.PR) {
        vl_pr_min = vm_noeud->Localisation.PR;
        vl_pr_max = vm_noeud->Localisation_Tete.PR;
  }
  else {
        vl_pr_max = vm_noeud->Localisation.PR;
        vl_pr_min = vm_noeud->Localisation_Tete.PR;
  }
XZST_03EcritureTrace(XZSTC_WARNING,"avant XZAD_recherche_dest_pmva %d %d %d",
			vm_noeud->Localisation.Sens,
                                vm_noeud->Localisation.Autoroute,
                                vm_noeud->Localisation_Tete.PR);
  /*on commence par chercher la destination associe au PMVA*/
  /* = premiere destination en aval*/
  vm_pmv.Dest_un_proche=0;
  vm_dest1proche_numero=0;

  if (vm_pmv.PMVA_SAGA) 
    return (drol_pmva_var_localisation_km());

  if (XZAD_recherche_dest_pmva(vm_pmv.Identifiant, &pl_pma, vm_noeud->Localisation.Sens,
				vm_noeud->Localisation.Autoroute,
				vm_noeud->Localisation_Tete.PR)!=XDC_OK)
    return (drol_pmva_var_localisation_km());
  /*on calcule le tdp entre le PMV et cette destination*/
	XZST_03EcritureTrace(XZSTC_WARNING,"avant XZAD23_Calculer_TdP_PMV_Dest %d",pl_pma.Dest_un_proche); 
  if (XZAD23_Calculer_TdP_PMV_Dest(     vm_pmv.PR,
                          vm_pmv.Autoroute,
                          vm_pmv.Sens,
                          pl_pma.Dest_un_proche,
                          pl_pma.Graphe_Dest_un_proche,
                          &vl_tdp, vl_nom,vm_pmv.Identifiant,XDC_EQT_PMVA) != XDC_OK)	{
      if (vm_noeud->Localisation.Longueur > 3000)
      	return (drol_pmva_var_longueur());
      else	
      	return (drol_pmva_var_localisation_km());
  }

  	XZST_03EcritureTrace(XZSTC_WARNING,"TDP %d",vl_tdp); 
   if (vl_tdp<=0) {
	/* Si pas de TDP alors localisation ou longueur si bouchon > 3km)*/
      if (vm_noeud->Localisation.Longueur > 3000)
      	return (drol_pmva_var_longueur());
      else	
      	return (drol_pmva_var_localisation_km());
   }

   vm_pmv.Dest_un_proche=pl_pma.Dest_un_proche;
   vm_dest1proche_numero=pl_pma.Dest_un_proche;

   /*TDP DOIT ETRE CADRE A DROITE*/
   if (XZAD27_Libelle_Destination(pl_pma.Dest_un_proche,vl_libelle) != XDC_OK) {
      XZST_03EcritureTrace(XZSTC_WARNING,"drol_pma_var_Dest_un_proche: pb dans l'appel a XZAD27_Libelle_Destination");
	/* Si pas de TDP alors localisation ou longueur si bouchon > 3km) */
      if (vm_noeud->Localisation.Longueur > 3000)
      	return (drol_pmva_var_longueur());
      else	
      	return (drol_pmva_var_localisation_km());
   }

   XZST_03EcritureTrace(XZSTC_WARNING,"MAX2 %d",vm_pmv.Dest_un_proche);
   if (vl_tdp > 59) {
/*	sprintf(vl_str,"%1dh%02d JUSQU'A\n%s",vl_tdp/60, vl_tdp%60,vl_libelle);*/
 	sprintf(vl_str,"%s           ",vl_libelle);
	sprintf(&vl_str[11],"%1dh%02d",vl_tdp/60, vl_tdp%60);
  }
  else {
/*     sprintf(vl_str,"%2dMN JUSQU'A\n%s", vl_tdp, vl_libelle);*/
     sprintf(vl_str,"%s           ",vl_libelle);
	sprintf(&vl_str[11],"%2dmn", vl_tdp);
   }
   vm_dest1proche_HS=FALSE;

  /*
  if (  (vm_noeud->Type == XZAEC_FMC_QueueBouchon) && 
       (vm_noeud->Infos_TFM.Type_Cause !=0) &&
       (vm_noeud->Infos_TFM.Type_Cause != XZAEC_FMC_TeteBouchon) &&
	(vm_noeud->Infos_TFM.Type_Cause  != XZAEC_FMC_QueueBouchon) &&
	(vm_noeud->Infos_TFM.Type_Cause != XZAEC_FMC_Autre))
   strcpy(vl_str,drol_pmva_var_traversee());*/

  XZST_03EcritureTrace(XZSTC_WARNING,"vl_str = %s",vl_str);
  return (vl_str);
}


static char *drol_pmva_var_direction() {
  static char vl_str[50];
  strcpy(vl_str,vm_pmv.Direction);
  return(vl_str);
}

static char *drol_pmva_var_num_pt_car() {
T_Localisation vl_loc;
T_Localisation vl_loc_tete;
static char vl_str[20];

  vl_loc=vm_noeud->Localisation;
  vl_loc_tete=vm_noeud->Localisation_Tete;
  if ( (vl_loc.Sens==XDC_SENS_INCONNU) && ((vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD)) )
    sprintf(vl_str,"%s", vl_loc_tete.SortieAvalUsager);
  else
    sprintf(vl_str,"%s", vl_loc_tete.SortieAmontUsager);
/* PNI DEM686*/
  XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_var_num_pt_car = %s;TypePointCar=%d;vm_pmv.Sens=%d", vl_str,vl_loc.TypePointCar,vm_pmv.Sens);
  if ( vl_loc.TypePointCar==XDC_POINT_CARACT_ECHANGEUR) 
/*  if (  (vm_pmv.Sens==XDC_SENS_ENTREE_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_NORD) || (vm_pmv.Sens==XDC_SENS_SORTIE_SUD) || (vm_pmv.Sens==XDC_SENS_SORTIE_NORD))*/
  	sprintf(vl_str,"%s",vl_loc.SortieUsager);
  XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_var_num_pt_car = %s", vl_str);
  return (vl_str);
}

static char *drol_pmva_var_nom_pt_car() {
T_Localisation vl_loc;
T_Localisation vl_loc_tete;
static char vl_str[40];

  vl_loc=vm_noeud->Localisation;
  vl_loc_tete=vm_noeud->Localisation_Tete;
/*  if ( (vl_loc.Sens==XDC_SENS_INCONNU) && ((vm_pmv.Sens==XDC_SENS_SUD)  || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD)))
    sprintf(vl_str,"%s", vl_loc_tete.AbrevSortieAval);
  else
    sprintf(vl_str,"%s", vl_loc_tete.AbrevSortieAmont);*/
  sprintf(vl_str,"%s",vl_loc.AbrevPointCar);
  XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_var_nom_pt_car = %s", vl_str);
  return (vl_str);
}

static char *drol_pmva_var_distance() {
static char vl_str[20];
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"%d km", (vm_pmv.Distance_C+500)/1000);
    else if (vm_pmv.Distance_C > 0)
      sprintf(vl_str,"%d m", ((int)(vm_pmv.Distance_C/100))*100);
    else
      sprintf(vl_str,"");
  return (vl_str);
} 

static char *drol_pmva_var_situation() {


    /*cas LA voie de droite*/
    if (  (drol_pmva_voies(vm_noeud->Localisation.VL,vm_noeud->Localisation.VM1,
			   vm_noeud->Localisation.VM2,
			   vm_noeud->Localisation.VR,
			   XDC_VOIE_NEUTRALISEE)==1) ||
          (drol_pmva_voies(vm_noeud->Localisation.VL,vm_noeud->Localisation.VM1,
			   vm_noeud->Localisation.VM2,
			   vm_noeud->Localisation.VR,
			   XDC_VOIE_BLOQUEE)==1) 
	)
	return ("VOIE DE DROITE");	  
	
    /*cas LA voie de gauche*/
    if (  (drol_pmva_voies(vm_noeud->Localisation.VL,vm_noeud->Localisation.VM1,
			   vm_noeud->Localisation.VM2,
			   vm_noeud->Localisation.VR,
			   XDC_VOIE_NEUTRALISEE)==4) ||
          (drol_pmva_voies(vm_noeud->Localisation.VL,vm_noeud->Localisation.VM1,
			   vm_noeud->Localisation.VM2,
			   vm_noeud->Localisation.VR,
			   XDC_VOIE_BLOQUEE)==4) 
	)
	return ("VOIE DE GAUCHE");	  	

    /*cas LA voie centrale*/
    if (  (drol_pmva_voies(vm_noeud->Localisation.VL,vm_noeud->Localisation.VM1,
			   vm_noeud->Localisation.VM2,
			   vm_noeud->Localisation.VR,
			   XDC_VOIE_NEUTRALISEE)==7) ||
          (drol_pmva_voies(vm_noeud->Localisation.VL,vm_noeud->Localisation.VM1,
			   vm_noeud->Localisation.VM2,
			   vm_noeud->Localisation.VR,
			   XDC_VOIE_BLOQUEE)==7) ||
	 (drol_pmva_voies(vm_noeud->Localisation.VL,vm_noeud->Localisation.VM1,
			   vm_noeud->Localisation.VM2,
			   vm_noeud->Localisation.VR,
			   XDC_VOIE_NEUTRALISEE)==8) ||
          (drol_pmva_voies(vm_noeud->Localisation.VL,vm_noeud->Localisation.VM1,
			   vm_noeud->Localisation.VM2,
			   vm_noeud->Localisation.VR,
			   XDC_VOIE_BLOQUEE)==8 )		   
	)
	return ("VOIE CENTRALE");
			  		
   return ("");	      
} 

static char *drol_pmva_var_carburant() {
  if (vm_noeud->Infos_FAX.Numero_Carburant==XDC_CRB_SUPER)
    return ("SUPER");
  else if (vm_noeud->Infos_FAX.Numero_Carburant==XDC_CRB_GAZOLE)
    return ("GAZOLE");
  else if (vm_noeud->Infos_FAX.Numero_Carburant==XDC_CRB_SSPB95)
    return ("SP95");
  else if (vm_noeud->Infos_FAX.Numero_Carburant==XDC_CRB_SSPB98)
    return ("SP98");
  else if (vm_noeud->Infos_FAX.Numero_Carburant==XDC_CRB_GPL)
    return ("GPL");
  else
    return ("\nCARBURANT");
}

static char *drol_pmva_var_autoroute() {
static char vl_str[30];

  sprintf(vl_str,"%s FERMEE", vm_noeud->Localisation.NomAutoroute);
  XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_var_autoroute = %s", vl_str);
  return (vl_str);
}

static char *drol_pmva_var_nom_autoroute() {
static char vl_str[30];

  sprintf(vl_str,"%s", vm_noeud->Localisation.NomAutoroute);
  XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmva_var_nom_autoroute = %s", vl_str);
  return (vl_str);
}

static char *drol_pmva_var_car() {
  return drol_pmva_var_carburant();
}
static char *drol_pmva_var_null() { return (""); }


static char *drol_pmva_var_tdp() {
  XDY_Destination_SAD vl_dest;
  XDY_PR vl_pr_min;
  XDY_PR vl_pr_max;
  XDY_TDP vl_tdp;
  static char vl_str[50];
char vl_nom[15];
  
  if (vm_noeud->Localisation.PR < vm_noeud->Localisation_Tete.PR) {
  	vl_pr_min = vm_noeud->Localisation.PR;
  	vl_pr_max = vm_noeud->Localisation_Tete.PR;
  }
  else {
  	vl_pr_max = vm_noeud->Localisation.PR;
  	vl_pr_min = vm_noeud->Localisation_Tete.PR;
  }
    	
  
  /*on commence par chercher la destination associe a la FMC*/
  /* = premiere destination en aval*/
  if (XZAD_recherche_dest_fmc(vm_noeud->Localisation.Autoroute,vl_pr_min,vl_pr_max,vm_noeud->Localisation.Sens,&vl_dest)!=XDC_OK)
    return ("");
    
  /*on calcule le tdp entre le PMVA et cette destination*/
  /*
  if (XZAD23_Calculer_TdP_PMV_Dest( 	vm_pmv.PR,
                          vm_pmv.Autoroute,
			  vm_pmv.Sens,
                          vl_dest.Numero,
			  &vl_tdp, vl_nom) != XDC_OK)*/
    return ("");
  
  	
   vm_pmv.Dest_un_proche=vl_dest.Numero;
   
   sprintf(vl_str,"%s:%2dmn",vl_dest.Libelle_PMV,
   			vl_tdp);
   return(vl_str);
}
static char *drol_pmva_var_nuit() {
static char vl_str[20]="";
int vl_debut;
char vl_str2[20];

  if (sscanf(vm_noeud->Infos_TFM.Horaire_Debut_Prevu,"%d",&vl_debut)==1) {
    if ((vl_debut>=19) || (vl_debut<6)) {
       /*NUIT DU JJ/MM*/
       strcpy(vl_str2,vm_noeud->Infos_TFM.Nuit);
       texte_pmv_de_base(vl_str2);
       vl_str2[5]='\0';
       sprintf(vl_str,"NUIT DU %s",vl_str2);
    }
    else {
       sprintf(vl_str,"LE %s",vm_noeud->Infos_TFM.Nuit);
    }
  }
  return (vl_str);
}
static char *drol_pmva_var_horaires() {
static char vl_str[20];
char vl_debut[10];
char vl_fin[10];
  /*DE nnH A mmH*/
  strcpy(vl_debut,vm_noeud->Infos_TFM.Horaire_Debut_Prevu);
  strcpy(vl_fin,vm_noeud->Infos_TFM.Horaire_Fin_Prevue);
  vl_debut[2]='\0';
  vl_fin[2]='\0';
  texte_pmva_de_base(vl_debut);
  texte_pmva_de_base(vl_fin);
  sprintf(vl_str,"de %sh a %sh",vl_debut,vl_fin);
  return (vl_str);
}

static char *drol_pmva_var_aire_amont() {
static char vl_str[50]="";
XDY_Octet 	vl_NumAire_Out;		
XDY_Nom		vl_NomAire_Out;
XDY_Abrev	vl_AbrevAire_Out; 
XDY_Autoroute 	vl_Autoroute_Out; 
XDY_PR 		vl_PR_Out;
XDY_Sens 	vl_Sens_Out; 
XDY_Entier 	vl_distance_out;

	/*PROCHAINE AIRE A XX KM*/	

  /*appel XZAO16 pour trouver l'aire amont*/
  if (XZAO16_Aire_Amont(vm_noeud->Localisation.Autoroute,
  			vm_noeud->Localisation.PR,
  			vm_noeud->Localisation.Sens,
  			1,
  			&vl_NumAire_Out,
  			vl_NomAire_Out,
  			vl_AbrevAire_Out,
  			&vl_Autoroute_Out,
  			&vl_PR_Out,
  			&vl_Sens_Out,
  			&vl_distance_out) !=  XDC_OK) {
     return ("");
  }

  /*calcule la distance entre le PMVA et cette aire*/
  if (XZAO22_Distance (	vm_pmv.Autoroute,
				vm_pmv.PR,
				vl_Autoroute_Out,
				vl_PR_Out,
				&vl_distance_out) != XDC_OK) {
    return ("");
  }

  if (vl_distance_out > 500)
      sprintf(vl_str,"PROCHAINE STATION\\a %d km", (vl_distance_out+500)/1000);
  else
      sprintf(vl_str,"PROCHAINE STATION\\a %d m", ((int)(vl_distance_out/100))*100);
  return (vl_str);
}

/**************************************/
/**************************************/
T_LIGNE_PMVA drol_pmva_remplit_ligne(char *va_str)
{
  T_LIGNE_PMVA vl_ligne;
  int vl_indice;

  vm_presence_nature_bouchon=FALSE;
  XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne IN = %s", va_str);
  vl_indice=0;
  strcpy(vl_ligne.Texte,"");
  while ( (va_str[vl_indice]!=C_CAR_DOLLAR) && vl_indice<strlen(va_str))
  {
    vl_ligne.Texte[vl_indice]=va_str[vl_indice];
    vl_indice++;
  }
  vl_ligne.Texte[vl_indice]='\0';

  vl_ligne.Fct = drol_pmva_var_null;
  /*si on a trouve un DOLLAR*/
  if (va_str[vl_indice]==C_CAR_DOLLAR) {
    va_str=va_str+vl_indice+1;
  XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne va_str = %s", va_str);
    if (!strcmp(va_str,C_VAR_LONGUEUR)) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct C_VAR_LONGUEUR");
      vm_presence_nature_bouchon=TRUE;
      vl_ligne.Fct = drol_pmva_var_longueur;
    }
    else if (!strcmp(va_str,C_VAR_LOCALISATION)) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct C_VAR_LOCALISATION");
      vl_ligne.Fct = drol_pmva_var_localisation;
    }
    else if (!strcmp(va_str,C_VAR_LOCALISATION_TDP)) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct C_VAR_LOCALISATION_TDP");
      vl_ligne.Fct = drol_pmva_var_localisation_tdp;
    }
    else if (!strcmp(va_str,C_VAR_LOCALISATION_KM)) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct C_VAR_LOCALISATION_KM");
      vl_ligne.Fct = drol_pmva_var_localisation_km;
    }
    else if (!strcmp(va_str,C_VAR_LOCALIS_SORTIE)) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct C_VAR_LOCALIS_SORTIE");
      vl_ligne.Fct = drol_pmva_var_localisation_sortie;
    }
    else if (!strcmp(va_str,C_VAR_NUM_PT_CAR)) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct C_VAR_NUM_PT_CAR");
      vl_ligne.Fct = drol_pmva_var_num_pt_car;
    }
    else if (!strcmp(va_str,C_VAR_NOM_PT_CAR)) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct C_VAR_NOM_PT_CAR");
      vl_ligne.Fct = drol_pmva_var_nom_pt_car;
    }
    else if (!strcmp(va_str,C_VAR_DISTANCE)) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct C_VAR_DISTANCE");
      vl_ligne.Fct = drol_pmva_var_distance;
    }
    else if (!strcmp(va_str,C_VAR_CARBURANT)) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct C_VAR_CARBURANT");
      vl_ligne.Fct = drol_pmva_var_carburant;
    }
    else if (!strcmp(va_str,C_VAR_CAR)) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct C_VAR_CAR");
      vl_ligne.Fct = drol_pmva_var_car;
    }
    else if (!strcmp(va_str,C_VAR_AUTOROUTE)) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct C_VAR_AUTOROUTE");
      vl_ligne.Fct = drol_pmva_var_autoroute;
    }
     else if (!strcmp(va_str,C_VAR_NOM_AUTOROUTE)) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct C_VAR_NOM_AUTOROUTE");
      vl_ligne.Fct = drol_pmva_var_nom_autoroute;
    }
   else if (!strcmp(va_str,C_VAR_SITUATION)) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct C_VAR_SITUATION");
      vl_ligne.Fct = drol_pmva_var_situation;
    }
    else if (!strcmp(va_str,C_VAR_TDP)) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct C_VAR_TDP");
      vl_ligne.Fct = drol_pmva_var_tdp;
    }
    else if (!strcmp(va_str,C_VAR_NUIT)) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct C_VAR_NUIT");
      vl_ligne.Fct = drol_pmva_var_nuit;
    }
    else if (!strcmp(va_str,C_VAR_HORAIRES)) {
      XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_remplit_ligne fct C_VAR_HORAIRES");
      vl_ligne.Fct = drol_pmva_var_horaires;
    }
    else if (!strcmp(va_str,C_VAR_AIRE_AMONT)) {
      XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_remplit_ligne fct C_VAR_AIRE_AMONT");
      vl_ligne.Fct = drol_pmva_var_aire_amont;
    }    
    else if (!strcmp(va_str,C_VAR_DIRECTION)) {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct C_VAR_DIRECTION");
      vl_ligne.Fct = drol_pmva_var_direction;
    }   
    else if (!strcmp(va_str,C_VAR_DEST1PROCHE)) {
      XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_remplit_ligne fct C_VAR_DEST1PROCHE");
      vl_ligne.Fct = drol_pmva_var_dest_un_proche;
    }
    else if (!strcmp(va_str,C_VAR_DEST1PROCHE_LOC)) {
      XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_remplit_ligne fct C_VAR_DEST1PROCHE_LOC");
      vl_ligne.Fct = drol_pmva_var_dest_un_proche_loc;
    } 
    else if (!strcmp(va_str,C_VAR_DEST1PROCHE_LONG)) {
      XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_remplit_ligne fct C_VAR_DEST1PROCHE_LONG");
      vl_ligne.Fct = drol_pmva_var_dest_un_proche_long;
    }
    else if (!strcmp(va_str,C_VAR_CAUSE)) {
      XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_remplit_ligne fct C_VAR_CAUSE");
      vl_ligne.Fct = drol_pmva_var_cause;
    }
    else {
      XZST_03EcritureTrace(XZSTC_DEBUG1,"drol_pmva_remplit_ligne fct NULL");
      vl_ligne.Fct = drol_pmva_var_null;
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

XDY_Booleen drol_pmva_filtre(T_NOEUD *va_noeud, XDY_District va_site, XDY_Horodate va_horodate)

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
  T_PERT_PMVA *pl_cas;
  T_CONDITION *pl_cond;
  XDY_Booleen (*vl_fct)();
  XDY_Booleen vl_res=FALSE;
  XDY_Booleen vl_fax_a_emettre=FALSE;
  T_PERT_PMVA *vl_pert;
  int vl_indice;

  /*remet a jour la liste des PMV*/
  pmva_init_vg_requetes();

  vm_noeud=va_noeud;
  vm_noeud2->Localisation=vm_noeud->Localisation;
  vm_noeud2->Infos_FAX=vm_noeud->Infos_FAX;
  vm_noeud2->Type=vm_noeud->Type;

  vl_pert=NULL;
  pl_cas= vg_pert_pmva;
  vm_mode_verif_bouchon=FALSE;
  vm_mode_dans_pert=FALSE;

    XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_pmva_filtre IN");
  while (pl_cas!=NULL)
  {
    XZST_03EcritureTrace(   XZSTC_DEBUG1,"drol_pmva_filtre %s",pl_cas->Libelle);
    pl_cond=pl_cas->Condition;
    vl_res=TRUE;
    if (pl_cas->PMVA_Dans_Perturbation)
     vm_mode_dans_pert=TRUE;
        else
          vm_mode_dans_pert=FALSE;
    vm_mode_verif_bouchon=FALSE;
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
      XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_pmva_filtre OK pour %s ",
					pl_cas->Libelle);
      vm_pert=pl_cas;
      drol_pmva_recherche(pl_cas,va_horodate,va_site);

  /*pour chacun des PMVA recuperes*/
  for (vl_indice=0;vl_indice<C_NB_MAX_PMVA;vl_indice++)
  {
    /* si le PMVA doit etre utilise*/
    if ((vg_pmva[vl_indice].Utilise) && (!strcmp(vm_pert->Libelle,vg_pmva[vl_indice].Perturbation.Libelle))) {
      /*on verifie a nouveau les conditions si bouchon et */
        /*si on cherche les PMVA dans la perturbation*/
        if ( (vm_noeud->Type==XZAEC_FMC_QueueBouchon) &&
             (vm_pert->PMVA_Dans_Perturbation==1)
           ) {
                XZST_03EcritureTrace(   XZSTC_WARNING,"on verifie a nouveau PMVA dans pert pour %s",
                                        vm_pert->Libelle);
                vm_mode_dans_pert=FALSE;
                vm_mode_verif_bouchon=TRUE;
    pl_cond=vm_pert->Condition;
    vl_res=TRUE;
        vm_pmv = vg_pmva[vl_indice];

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
    if (!vl_res) {
      XZST_03EcritureTrace(   XZSTC_WARNING," NOK pour %s=> CORRESPOND PLUS ",
                                        vm_pert->Libelle);
                vg_pmva[vl_indice].Utilise=FALSE;
    }
    }
    }
    }

  /*pour chacun des PMVA recuperes*/
  for (vl_indice=0;vl_indice<C_NB_MAX_PMVA;vl_indice++)
  {
    /* si le PMVA doit etre utilise*/
    if ((vg_pmva_saga[vl_indice].Utilise) && (!strcmp(vm_pert->Libelle,vg_pmva_saga[vl_indice].Perturbation.Libelle))) {
      /*on verifie a nouveau les conditions si bouchon et */
        /*si on cherche les PMVA dans la perturbation*/
        if ( (vm_noeud->Type==XZAEC_FMC_QueueBouchon) &&
             (vm_pert->PMVA_Dans_Perturbation==1)
           ) {
                XZST_03EcritureTrace(   XZSTC_WARNING,"on verifie a nouveau PMVA dans pert pour %s",
                                        vm_pert->Libelle);
                vm_mode_dans_pert=FALSE;
                vm_mode_verif_bouchon=TRUE;
    pl_cond=vm_pert->Condition;
    vl_res=TRUE;
        vm_pmv = vg_pmva_saga[vl_indice];

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
    if (!vl_res) {
      XZST_03EcritureTrace(   XZSTC_WARNING," NOK pour %s=> CORRESPOND PLUS ",
                                        vm_pert->Libelle);
                vg_pmva_saga[vl_indice].Utilise=FALSE;
    }
    }
    }
    }


    }

    pl_cas=pl_cas->Suivant;
  }

  /*pour chacun des PMVA recuperes*/
  for (vl_indice=0;vl_indice<C_NB_MAX_PMVA;vl_indice++)
  {
    /* si le PMVA doit etre utilise*/
    if (vg_pmva[vl_indice].Utilise) {
      XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_pmva_filtre: pert pmv %d, priorite %d, auto = %d",
					vg_pmva[vl_indice].Perturbation.Priorite,
					vg_priorite_retenue,
					vg_pmva[vl_indice].Perturbation.Automatique);
      vm_pmv = vg_pmva[vl_indice];
      drol_pmva_message(vg_pmva[vl_indice], vg_pmva[vl_indice].Perturbation, va_horodate, va_site); 
    }
  }
  /*pour chacun des PMVA recuperes*/
  for (vl_indice=0;vl_indice<C_NB_MAX_PMVA;vl_indice++)
  {
    /* si le PMVA doit etre utilise*/
    if (vg_pmva_saga[vl_indice].Utilise) {
      XZST_03EcritureTrace(   XZSTC_FONCTION,"drol_pmva_filtre: pert pmv %d, priorite %d, auto = %d",
					vg_pmva_saga[vl_indice].Perturbation.Priorite,
					vg_priorite_retenue,
					vg_pmva_saga[vl_indice].Perturbation.Automatique);
      vm_pmv = vg_pmva_saga[vl_indice];
      drol_pmva_message(vg_pmva_saga[vl_indice], vg_pmva_saga[vl_indice].Perturbation, va_horodate, va_site); 
    }
  }
  return (TRUE);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de recherche des PMVA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_pmva_recherche(T_PERT_PMVA *va_pert,
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
  int vl_sens;

  XZST_03EcritureTrace(   XZSTC_FONCTION,"IN : drol_pmva_recherche");

		vl_sens=vm_noeud->Localisation.Sens;

  /*appel de la fonction de recherche du ou des PMV*/
  if (va_pert->PMVA_Dans_Perturbation==0) {
/*DEM 1050 */
/*	prise en compte des bretelles dans le SAD pour Fermeture Echangeur*/
      if (vm_noeud->Type==XZAEC_FMC_FermetureEch) /*DEM810*/ 
      {
	if ( vm_noeud->Infos_FAX.Entree1)
		les_PMVA_amont(vm_noeud->Localisation.Autoroute,
	     	vm_noeud->Localisation.PR,
	     	vm_noeud->Localisation_Tete.PR,
		XDC_SENS_ENTREE_SUD,
		va_horodate,
		va_site,
	     	va_pert->Priorite,
	     	va_pert->Libelle,
	     	*va_pert);
		
	if ( vm_noeud->Infos_FAX.Entree2)
		les_PMVA_amont (vm_noeud->Localisation.Autoroute,
	     	vm_noeud->Localisation.PR,
	     	vm_noeud->Localisation_Tete.PR,
	     	XDC_SENS_ENTREE_NORD,
	     	va_horodate,
	     	va_site,
	     	va_pert->Priorite,
	     	va_pert->Libelle,
	     	*va_pert);
	      
	if ( vm_noeud->Infos_FAX.Sortie1)
		les_PMVA_amont (vm_noeud->Localisation.Autoroute,
	     	vm_noeud->Localisation.PR,
	     	vm_noeud->Localisation_Tete.PR,
	     	XDC_SENS_SORTIE_SUD,
	     	va_horodate,
	     	va_site,
	     	va_pert->Priorite,
	     	va_pert->Libelle,
	     	*va_pert);

	if ( vm_noeud->Infos_FAX.Sortie2)
		les_PMVA_amont (vm_noeud->Localisation.Autoroute,
	     	vm_noeud->Localisation.PR,
	     	vm_noeud->Localisation_Tete.PR,
	     	XDC_SENS_SORTIE_NORD,
	     	va_horodate,
	     	va_site,
	     	va_pert->Priorite,
	     	va_pert->Libelle,
	     	*va_pert);
	}
	else {
	if (vm_noeud->Type == XZAEC_FMC_AireFermee) {

		if ((vm_noeud->Localisation.Sens==XDC_SENS_SORTIE_NORD) ||
		    (vm_noeud->Localisation.Sens==XDC_SENS_ENTREE_NORD) ||
		    (vm_noeud->Localisation.Sens==XDC_SENS_NORD))
		     vl_sens=XDC_SENS_NORD;
		else
		     vl_sens=XDC_SENS_SUD;
	
		}
	else
		vl_sens=vm_noeud->Localisation.Sens;

        if ((vm_noeud->Type == XZAEC_FMC_AnimalErrant) ||
            (vm_noeud->Type == XZAEC_FMC_Meteo) ||
            (vm_noeud->Type == XZAEC_FMC_PietonChaussee)  ||
            (vm_noeud->Type == XZAEC_FMC_Contresens)) 
          vl_sens = XDC_SENS_INCONNU;

	les_PMVA_amont (vm_noeud->Localisation.Autoroute,
	     vm_noeud->Localisation.PR,
	     vm_noeud->Localisation_Tete.PR,
	     vl_sens,
	     va_horodate,
	     va_site,
	     va_pert->Priorite,
	     va_pert->Libelle,
	     *va_pert);
   	}
   }
   else {
  XZST_03EcritureTrace(   XZSTC_FONCTION,"IN : drol_pmva_recherche les_PMVA_ds_perturbation");
        if ((vm_noeud->Type == XZAEC_FMC_AnimalErrant) ||
            (vm_noeud->Type == XZAEC_FMC_Meteo) ||
            (vm_noeud->Type == XZAEC_FMC_PietonChaussee) ||
            (vm_noeud->Type == XZAEC_FMC_Contresens))
          vl_sens = XDC_SENS_INCONNU;

	les_PMVA_ds_perturbation (vm_noeud->Localisation.Autoroute,
	     vm_noeud->Localisation.PR,
	     vm_noeud->Localisation_Tete.PR,
	     vl_sens /*vm_noeud->Localisation.Sens*/,
	     va_horodate,
	     va_site,
	     va_pert->Priorite,
	     va_pert->Libelle,
	     C_INFINI,
	     C_INFINI,
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
	     *va_pert);
  }

 return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* fct de calcul du message a afficher sur le PMVA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void drol_pmva_message (T_PMVA va_pmv, T_PERT_PMVA va_pert, XDY_Horodate va_horodate, XDY_District va_site)

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
  T_LIGNE_PMVA vl_l1,vl_l2,vl_l3,vl_l4,vl_l5;
  T_LIGNE_PMVA vl_al1,vl_al2,vl_al3,vl_al4,vl_al5;
  char vl_t1[50];
  char vl_t2[50];
  char vl_t3[50];
  char vl_t4[50];
  char vl_t5[50];
  char vl_a1[50];
  char vl_a2[50];
  char vl_a3[50];
  char vl_a4[50]; 
  char *vl_str;
  char vl_a5[50];  
  char vl_texte[250];
  char vl_alternat[250];
  char   Texte1PMV[21];
  char   Texte2PMV[21];
  char   Texte3PMV[21];
  char   Texte4PMV[21];
  char   Texte5PMV[21];
  char   Alternat1PMV[21];
  char   Alternat2PMV[21];
  char   Alternat3PMV[21];
  char   Alternat4PMV[21];
  char   Alternat5PMV[21];
  char	vl_explication[100];
  int   vl_mode_automatique;
  int  vl_num_action;

 XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message");


      vl_l1 = va_pert.Ligne1;
      vl_l2 = va_pert.Ligne2;
      vl_l3 = va_pert.Ligne3;
      vl_l4 = va_pert.Ligne4;
      vl_l5 = va_pert.Ligne5;

     vl_al1 = va_pert.Alternat1;
      vl_al2 = va_pert.Alternat2;
      vl_al3 = va_pert.Alternat3;
      vl_al4 = va_pert.Alternat4;
      vl_al5 = va_pert.Alternat5;

  /*calcul des elements de message*/
  vm_presence_nature_bouchon=FALSE;
  vm_ligne_courante=1;
  strcpy(vl_t1,drol_pmva_calcul_ligne(vl_l1));
  vm_ligne_courante++;
  strcpy(vl_t2,drol_pmva_calcul_ligne(vl_l2));
  vm_ligne_courante++;
  strcpy(vl_t3,drol_pmva_calcul_ligne(vl_l3));
  vm_ligne_courante++;
  strcpy(vl_t4,drol_pmva_calcul_ligne(vl_l4));
  vm_ligne_courante++;
  strcpy(vl_t5,drol_pmva_calcul_ligne(vl_l5));

 /* strcpy(vl_a1,drol_pmva_calcul_ligne(vl_al1));
  strcpy(vl_a2,drol_pmva_calcul_ligne(vl_al2));
  strcpy(vl_a3,drol_pmva_calcul_ligne(vl_al3));
  strcpy(vl_a4,drol_pmva_calcul_ligne(vl_al4));
  strcpy(vl_a5,drol_pmva_calcul_ligne(vl_al5));*/
 

  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : message = %s",vl_t1);
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : message = %s",vl_t2);
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : message = %s",vl_t3);
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : message = %s",vl_t4);
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : message = %s",vl_t5);

  /*concatenation des elements de message*/
  if (strcmp(vl_t1,"")) sprintf(vl_texte,"%s",vl_t1);
  if (strcmp(vl_t2,"")) sprintf(vl_texte,"%s\\%s",vl_texte,vl_t2);
  if (strcmp(vl_t3,"")) sprintf(vl_texte,"%s\\%s",vl_texte,vl_t3);
  if (strcmp(vl_t4,"")) sprintf(vl_texte,"%s\\%s",vl_texte,vl_t4);
  if (strcmp(vl_t5,"")) sprintf(vl_texte,"%s\\%s",vl_texte,vl_t5);
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : message = %s",vl_texte);
  strcpy(vl_texte,pmva_mise_en_page(vl_texte,C_NB_CAR_LIGNE));

/*  if (strcmp(vl_a1,"")) sprintf(vl_alternat,"%s",vl_a1);
  if (strcmp(vl_a2,"")) sprintf(vl_alternat,"%s\\%s",vl_alternat,vl_a2);
  if (strcmp(vl_a3,"")) sprintf(vl_alternat,"%s\\%s",vl_alternat,vl_a3);
  if (strcmp(vl_a4,"")) sprintf(vl_alternat,"%s\\%s",vl_alternat,vl_a4);
  if (strcmp(vl_a5,"")) sprintf(vl_alternat,"%s\\%s",vl_alternat,vl_a5);
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : message = %s",vl_texte);
  strcpy(vl_alternat,pmv_mise_en_page(vl_alternat,C_NB_CAR_LIGNE));*/
  strcpy(vl_alternat,"");
  
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : message = %s",vl_texte);
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : alternat = %s",vl_alternat);

  /*mise en majuscules du texte du message */
  drol_pmva_majuscules(vl_t1);
  drol_pmva_majuscules(vl_t2);
  drol_pmva_majuscules(vl_t3);
  drol_pmva_majuscules(vl_t4);
  drol_pmva_majuscules(vl_t5);
  drol_pmva_majuscules(vl_texte);

  /*ajout de la proposition de message PMV en base*/
  strcpy(Texte1PMV,strtok(vl_texte,"\n"));
  if (strcmp(Texte1PMV,"\0") != 0) {
    vl_str = (Texte2PMV,strtok(NULL,"\n"));
    if (vl_str != NULL) 
      strncpy(Texte2PMV,vl_str,20);
    else
      strcpy(Texte2PMV,"\0");
    if (strcmp(Texte2PMV,"\0") != 0) {
      vl_str= strtok(NULL,"\n");
      if (vl_str!=NULL)
        strncpy(Texte3PMV,vl_str,20);
      else
        strcpy(Texte3PMV,"\0");
      if (strcmp(Texte3PMV,"\0") != 0) {
        vl_str = strtok(NULL,"\n");
        if (vl_str!=NULL)
          strncpy(Texte4PMV,vl_str,20);
	else
          strcpy(Texte4PMV,"\0");
        if (strcmp(Texte4PMV,"\0") != 0)  {
	  vl_str= strtok(NULL,"\n");
	  if (vl_str!=NULL)
            strcpy(Texte5PMV,vl_str);
	  else
           strncpy(Texte5PMV,"\0",20);
 	}
        else {
          strcpy(Texte5PMV,"\0");
	}
      }
      else {
      	strcpy(Texte4PMV,"\0");
        strcpy(Texte5PMV,"\0");
      }
    }
    else {
      strcpy(Texte3PMV,"\0");
      strcpy(Texte4PMV,"\0");
      strcpy(Texte5PMV,"\0");
    }
  }
  else {
    strcpy(Texte2PMV,"\0");
    strcpy(Texte3PMV,"\0");
    strcpy(Texte4PMV,"\0");
    strcpy(Texte5PMV,"\0");
  }

  texte_pmv_de_base(Texte1PMV);
  texte_pmv_de_base(Texte2PMV);
  texte_pmv_de_base(Texte3PMV);
  texte_pmv_de_base(Texte4PMV);
  texte_pmv_de_base(Texte5PMV);

  /* texte_pmv_de_base(Alternat1PMV);
  texte_pmv_de_base(Alternat2PMV);
  texte_pmv_de_base(Alternat3PMV);
  texte_pmv_de_base(Alternat4PMV);
  texte_pmv_de_base(Alternat5PMV);
  */


  /*supprimer la proposition si identique a usage courant*/
  /*gerer la mise en file d'attente*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message: courant %s/%s/%s/%s/%s",
			va_pmv.Ligne1,
			va_pmv.Ligne2,
			va_pmv.Ligne3,
			va_pmv.Ligne4,
			va_pmv.Ligne5);
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : propose %s/%s/%s/%s/%s",
			Texte1PMV,
			Texte2PMV,
			Texte3PMV,
			Texte4PMV,
			Texte5PMV);

  /*si on calcule un plan d'actions pour une simulation*/
  vl_mode_automatique = va_pert.Automatique;

  if (va_pmv.PMVA_SAGA)
	vl_mode_automatique=0;

  XZST_03EcritureTrace(XZSTC_WARNING,"vg_site=%d;va_pmv.District=%d",vg_site,va_pmv.District);
  XZST_03EcritureTrace(XZSTC_WARNING,"vg_presence_site[va_pmv.District]=%d",vg_presence_site[va_pmv.District]);
  if (((vg_site==XDC_CI) && (vg_presence_site[va_pmv.District]==XDC_VRAI)) || drol_pmva_fmc_simulee()) {
    /*on ne fait que proposer, pas de pilotage automatique*/
    /* Ne s'applique pas sur COFIROUTE, uniquement sur ESCOTA */
    /* Sur COFIROUTE, le site principal peut piloter les BAD */
    vl_mode_automatique=0;
  }

  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message: vg_site=%d, district=%d, auto=%d, prioU = %d",
			vg_site,
			va_pmv.District,
			va_pert.Automatique,
			va_pmv.Priorite_U);

  if ( (strcmp(va_pmv.Ligne1,Texte1PMV)) ||
       (strcmp(va_pmv.Ligne2,Texte2PMV)) ||
       (strcmp(va_pmv.Ligne3,Texte3PMV)) ||
       (strcmp(va_pmv.Ligne4,Texte4PMV)) ||
       (strcmp(va_pmv.Ligne5,Texte5PMV))
     )
  {
    sprintf(vl_explication,"%s/%s/%s/%s/%s",
			Texte1PMV,
			Texte2PMV,
			Texte3PMV,
			  Texte4PMV,
			  Texte5PMV);


	/* si ce n'est pas une maj du pmva pour le meme evt */
   if ((vm_noeud->NumEvt.numero!=va_pmv.Numero_Evt_U) || (vm_noeud->NumEvt.cle!=va_pmv.Cle_Evt_U))
   {
     /* DEM655 si meme priorite , l'evt le plus proche est prioritaire */
    if ((va_pmv.Priorite_C == va_pmv.Priorite_U) && (va_pmv.Distance_C < va_pmv.Distance_U))
	{
        	va_pmv.Priorite_U++;
		XZST_03EcritureTrace(XZSTC_INFO,"drol_pmva_message :si meme priorite , l'evt le plus proche est prioritaire :Distance_C=%d;Diastance_U=%d =>va_pmv.Priorite_C=%d;va_pmv.Priorite_U=%d  ",va_pmv.Distance_C,va_pmv.Distance_U,va_pmv.Priorite_C,va_pmv.Priorite_U);
	}

     /* DEM655 si  priorite superieure mais plus distant, alors on ne pilote pas en auto */
    if ((va_pmv.Priorite_C < va_pmv.Priorite_U) && (va_pmv.Distance_C > va_pmv.Distance_U) && (va_pmv.Priorite_U!=255))
	{
        	vl_mode_automatique=0;
		XZST_03EcritureTrace(XZSTC_INFO,"drol_pmva_message :si priorite superieure mais plus distant, alors on ne pilote pas en autoDistance_C=%d;Diastance_U=%d =>va_pmv.Priorite_C=%d;va_pmv.Priorite_U=%d  ",va_pmv.Distance_C,va_pmv.Distance_U,va_pmv.Priorite_C,va_pmv.Priorite_U);
	}
    }

/* si le mode est non auto ou
	si la priorite du pilotage en cours sur un autre evt est superieure
   alors on passe en proposition PA */
    XZST_03EcritureTrace(XZSTC_INFO,"drol_pmva_message SAPIENS : vl_mode_automatique=%d;Priorite_C=%d;Priorite_U=%d;NumEvt=%d:%d;NumEvt.cle=%d:%d;",vl_mode_automatique,va_pmv.Priorite_C,va_pmv.Priorite_U,vm_noeud->NumEvt.numero,va_pmv.Numero_Evt_U,vm_noeud->NumEvt.cle,va_pmv.Cle_Evt_U);
    if ((vl_mode_automatique==0)  || ((va_pmv.Priorite_C >= va_pmv.Priorite_U) && !((vm_noeud->NumEvt.numero==va_pmv.Numero_Evt_U) && (vm_noeud->NumEvt.cle==va_pmv.Cle_Evt_U)&& (va_pmv.Priorite_U!=0))))
    
     {
      if (!va_pmv.PMVA_SAGA) {
      /*mise en base de la proposition PMV*/
  pmv_tabuler_ligne(Texte1PMV,C_NB_CAR_LIGNE,vl_t1);
	pmv_tabuler_ligne(Texte2PMV,C_NB_CAR_LIGNE,vl_t2);
	pmv_tabuler_ligne(Texte3PMV,C_NB_CAR_LIGNE,vl_t3);
	pmv_tabuler_ligne(Texte4PMV,C_NB_CAR_LIGNE,vl_t4);
	pmv_tabuler_ligne(Texte5PMV,C_NB_CAR_LIGNE,vl_t5);

      if (XZAP28_Ajout_Prop_PMVA(vg_numero_plan,
			  vg_ordre_pmva++,
			  va_pmv.District,
			  "",
			  va_pmv.Priorite_C,
			  va_pmv.Identifiant,
			  vl_explication,
                                 vl_t1           ,
                                 vl_t2           ,
                                 vl_t3           ,
                                 vl_t4           ,
                                 vl_t5           ,
			  "",	/*alternat1 */
			  "",	/*alternat2*/
			  "",	/*alternat3*/
			  "",	/*alternat4*/
			  "",	/*alternat5*/
			  va_pert.Flash,
			  vm_pmv.Dest_un_proche,
			  0,
			  0,
			  0,
			  "Affichage",
			  va_pmv.Realisable) != XDC_OK)
      XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : pb appel a XZAP28_Ajout_Prop_PMVA");
      }
	else {
	if (XZAP70_Ajout_Prop_SAGA(vg_numero_plan,
                          vg_ordre_pmva++,
                          va_pmv.District,
                          "",
                          va_pmv.Priorite_C,
                          va_pmv.Identifiant,
                          vl_explication,
                                 vl_t1           ,
                                 vl_t2           ,
                                 vl_t3           ,
                                 vl_t4           ,
                                 vl_t5           ,
                          "Affichage",
                          "",   /*texte picto*/
                          "",
                          0,
                          0,
                          0,
                          0,
                          0,
                          va_pmv.Realisable) != XDC_OK)
        XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : pb appel a XZAP70_Ajout_Prop_SAGA");
      }
    }
    else { /*automatique*/
      /*on est ici, donc on traite du pilotage automatique*/
      /*premier cas : on est sur le district de l equipement*/
      if (va_pmv.District == va_site) {
	pmv_tabuler_ligne(Texte1PMV,C_NB_CAR_LIGNE,vl_t1);
	pmv_tabuler_ligne(Texte2PMV,C_NB_CAR_LIGNE,vl_t2);
	pmv_tabuler_ligne(Texte3PMV,C_NB_CAR_LIGNE,vl_t3);
	pmv_tabuler_ligne(Texte4PMV,C_NB_CAR_LIGNE,vl_t4);
	pmv_tabuler_ligne(Texte5PMV,C_NB_CAR_LIGNE,vl_t5);
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : flash=%d, prio=%d, distance=%d, site=%d",
					va_pmv.Flash,va_pmv.Priorite_C,va_pmv.Distance_C,va_pmv.District);
        if (XZAC82_Commande_PMVA ( va_horodate  ,
								 vm_noeud->NumEvt.numero    ,
                                 vm_noeud->NumEvt.cle     ,
                                 vg_num_poste ,
                                 va_pmv.Identifiant       ,
                                 va_pmv.District      ,
                                 vl_t1           ,
                                 vl_t2           ,
                                 vl_t3           ,
                                 vl_t4           ,
                                 vl_t5           ,
                                 ""           ,
                                 ""          , 
                                 ""           ,
                                 ""           ,
                                 ""           ,
                                 XDC_PMVA_PASCLIGNO      ,
                                 XDC_PMVA_PASCLIGNO      ,
                                 XDC_PMVA_PASCLIGNO      ,
                                 XDC_PMVA_PASCLIGNO      ,
                                 XDC_PMVA_PASCLIGNO      ,
                                 va_pert.Flash==0?XDC_PMVA_PASFLASH:va_pert.Flash      ,
                                 va_pmv.Priorite_C     ,
                                 va_pmv.Distance_C    ,
				 0,
                                 va_pmv.District,
				 va_pmv.Dest_un_proche,
				 0,
				 0,
				 0,
				 vl_num_action) != XDC_OK)
          XZST_03EcritureTrace(XZSTC_FONCTION,
                        "drol_pmva_message : pb dans l'appel a XZAC82_Commande_PMVA");
        else
          XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : appel a XZAC82 OK");
      }
      else {  /*sur districts differents, envoyer un msg a TACLI*/
	pmv_tabuler_ligne(Texte1PMV,C_NB_CAR_LIGNE,vl_t1);
	pmv_tabuler_ligne(Texte2PMV,C_NB_CAR_LIGNE,vl_t2);
	pmv_tabuler_ligne(Texte3PMV,C_NB_CAR_LIGNE,vl_t3);
	pmv_tabuler_ligne(Texte4PMV,C_NB_CAR_LIGNE,vl_t4);
	pmv_tabuler_ligne(Texte5PMV,C_NB_CAR_LIGNE,vl_t5);
        if (XZAC182_Commande_PMVA_TACLI ( 
									vm_noeud->NumEvt.numero    ,
                                 vm_noeud->NumEvt.cle     ,
                                 vg_num_poste ,
                                 va_pmv.Identifiant       ,
                                 va_horodate  ,
				va_pmv.District      ,
                                 vl_t1           ,
                                 vl_t2           ,
                                 vl_t3           ,
                                 vl_t4           ,
                                 vl_t5           ,
                                 ""           ,
                                 ""          , 
                                 ""           ,
                                 ""           ,
                                 ""           ,
                                 XDC_PMVA_PASCLIGNO      ,
                                 XDC_PMVA_PASCLIGNO      ,
                                 XDC_PMVA_PASCLIGNO      ,
                                 XDC_PMVA_PASCLIGNO      ,
                                 XDC_PMVA_PASCLIGNO      ,
                                 va_pert.Flash==0?XDC_PMVA_PASFLASH:va_pert.Flash,
                                 va_pmv.Priorite_C     ,
                                 va_pmv.Distance_C    ,
                                 vl_mode_automatique?va_pmv.District:vg_site,
                                 va_pmv.Dest_un_proche,
                                 0,
                                 0,
                                 0) != XDC_OK)
          XZST_03EcritureTrace(XZSTC_FONCTION,
                        "drol_pmva_message : pb dans l'appel a XZAC182_Commande_PMVA_TACLI");
        else
          XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : appel a XZAC182 OK");
      }
       /*mise en base de la proposition PMV avec priorite a 255 pour filtrage par xzap11*/
  	if (XZAP28_Ajout_Prop_PMVA(vg_numero_plan,
			  vg_ordre_pmva++,
			  va_pmv.District,
			  "",
			  255,
			  va_pmv.Identifiant,
			  va_pmv.Nom_Pert,
                                 vl_t1           ,
                                 vl_t2           ,
                                 vl_t3           ,
                                 vl_t4           ,
                                 vl_t5           ,
			  "",	/*alternat1 */
			  "",	/*alternat2*/
			  "",	/*alternat3*/
			  "",	/*alternat4*/
			  "",	/*alternat5*/
			  va_pert.Flash,
			  va_pmv.Dest_un_proche,
			  0,
			  0,
			  0,
			  "SAPIENS",
			  va_pmv.Realisable) != XDC_OK)
         XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : pb appel a XZAP28_Ajout_Prop_PMVA");

    }
  }
  else {
    XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : prop identique a l etat courant");
  /*mise en base de la proposition PMV avec priorite a 255 pour filtrage par xzap11*/
	pmv_tabuler_ligne(Texte1PMV,C_NB_CAR_LIGNE,vl_t1);
	pmv_tabuler_ligne(Texte2PMV,C_NB_CAR_LIGNE,vl_t2);
	pmv_tabuler_ligne(Texte3PMV,C_NB_CAR_LIGNE,vl_t3);
	pmv_tabuler_ligne(Texte4PMV,C_NB_CAR_LIGNE,vl_t4);
	pmv_tabuler_ligne(Texte5PMV,C_NB_CAR_LIGNE,vl_t5);
   if (!va_pmv.PMVA_SAGA) {
    if (XZAP28_Ajout_Prop_PMVA(vg_numero_plan,
			  vg_ordre_pmva++,
			  va_pmv.District,
			  "",
			  255,
			  va_pmv.Identifiant,
			  va_pmv.Nom_Pert,
                          vl_t1           ,
                          vl_t2           ,
                          vl_t3           ,
                          vl_t4           ,
                          vl_t5           ,
			  "",	/*alternat1 */
			  "",	/*alternat2*/
			  "",	/*alternat3*/
			  "",	/*alternat4*/
			  "",	/*alternat5*/
			  va_pert.Flash,
			  va_pmv.Dest_un_proche,
			  0,
			  0,
			  0,
			  "Affichage",
			  va_pmv.Realisable) != XDC_OK)
         XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : pb appel a XZAP28_Ajout_Prop_PMVA");
  }
  else {
	if (XZAP70_Ajout_Prop_SAGA(vg_numero_plan,
                          vg_ordre_pmva++,
                          va_pmv.District,
                          "",
                          255,
                          va_pmv.Identifiant,
                          vl_explication,
                          vl_t1           ,
                          vl_t2           ,
                          vl_t3           ,
                          vl_t4           ,
                          vl_t5           ,
                          "Affichage",
                          "",   /*texte picto*/
                          "",
                          0,
                          0,
                          0,
                          0,
                          0,
                          va_pmv.Realisable) != XDC_OK)
        XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_message : pb appel a XZAP70_Ajout_Prop_SAGA");
       }
       }
  vm_presence_nature_bouchon=FALSE;
  vm_bouchon_apres_sortie=TRUE;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  compare l'etat de chaque voie a l'etat de reference
*  tient compte du nombre de voies de circulation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int drol_pmva_voies(int VL, int VM1, int VM2, int VR, int Reference)

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
* calcule le texte d'une ligne d'un PMV
*------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *drol_pmva_calcul_ligne(T_LIGNE_PMVA va_ligne)

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

  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_calcul_ligne texte = %s",va_ligne.Texte);

  strcpy(vl_str2,(*vl_fct)());
  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_calcul_ligne fct = %s",vl_str2);

  /*concatenation texte et variable*/
  if (strcmp(va_ligne.Texte,""))
    sprintf(vl_str,"%s%s",va_ligne.Texte,vl_str2);
  else
    sprintf(vl_str,"%s",vl_str2);

  XZST_03EcritureTrace(XZSTC_FONCTION,"drol_pmva_calcul_ligne = %s",vl_str);
  /*return (drol_pmva_majuscules(vl_str));*/
  return (vl_str);
}




static char *drol_pmva_var_localisation() {
  T_Localisation vl_loc;
  T_Localisation vl_loc_tete;
  static char vl_str[150];
  int vl_inversee=1;

  vl_loc=vm_noeud->Localisation;
  vl_loc_tete=vm_noeud->Localisation_Tete;
  vm_bouchon_apres_sortie = TRUE;

  /*PATCH A57*/
  if (vl_loc.Autoroute==8)
    vl_inversee=-1;

  XZST_03EcritureTrace(XZSTC_WARNING,"LOCAL %d, %d, %d, %d, BIFURC %d",
			vm_pmv.PR,
			vl_loc.PR,
			vl_loc_tete.PR,
			vl_inversee,
			vm_pmv.Presence_Bifurcation);

  /*localisation entre deux sorties (nom et numero)*/
  if (  (util_pmv_amont_fmc(vm_pmv.PR,
			vm_pmv.Sens,
			vl_loc.PR,
			vl_loc_tete.PR,
			vl_loc.Sens,
			vl_inversee)
        )
	&&
	(util_pmv_amont_sortie(vm_pmv.Distance_C,
				vm_pmv.Sens,
				vl_loc.DistanceSortieAmont,
				vl_loc_tete.DistanceSortieAval,
				vl_loc.Sens,
				vm_noeud->Infos_TFM.Zone_Urbaine)
        )
	&&
	(vl_loc.DistanceSortieAmont < C_PMV_EPSILON)
	&&
	(vl_loc_tete.DistanceSortieAval < C_PMV_EPSILON)
	&&
	(vl_loc.PR != vl_loc_tete.PR)
      ) {
    if ( ((vl_loc.Sens==XDC_SENS_INCONNU) || (vl_loc.Sens>10)) && 
	  ( (vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD)))
      sprintf(vl_str,"apres SORTIE %s \n %s",
			vl_loc_tete.SortieAvalUsager,
			vl_loc_tete.AbrevSortieAval);
    else
      sprintf(vl_str,"apres SORTIE %s \n %s",
			vl_loc.SortieAmontUsager,
			vl_loc.AbrevSortieAmont);
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }


  /*localisation par rapport a la sortie amont*/
if ( (util_pmv_amont_fmc(vm_pmv.PR,
			   vm_pmv.Sens,
			   vl_loc.PR,
			   vl_loc_tete.PR,
			   vl_loc.Sens,
			   vl_inversee)
       )
       &&
       (util_pmv_amont_sortie(vm_pmv.Distance_C,
			      vm_pmv.Sens,
			      vl_loc.DistanceSortieAmont,
			      vl_loc_tete.DistanceSortieAval,
			      vl_loc.Sens,
			      vm_noeud->Infos_TFM.Zone_Urbaine)
       )
	&&
	(vl_loc.DistanceSortieAmont < C_PMV_EPSILON)
	&&
	(vl_loc_tete.DistanceSortieAval < C_PMV_EPSILON)
	/*********DEM1148&&
	(vl_loc.PR != vl_loc_tete.PR)*/
     ) {
    if ( ((vl_loc.Sens==XDC_SENS_INCONNU) || (vl_loc.Sens>10))&& 
	 ( (vm_pmv.Sens==XDC_SENS_SUD)|| (vm_pmv.Sens==XDC_SENS_ENTREE_SUD)))
      sprintf(vl_str,"apres SORTIE %s \n %s",
			vl_loc_tete.SortieAvalUsager,
			vl_loc_tete.AbrevSortieAval);
    else
      sprintf(vl_str,"apres SORTIE %s \n %s",
			vl_loc.SortieAmontUsager,
			vl_loc.AbrevSortieAmont);
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }

  /*localisation de la fin a la sortie aval (nom et numero)*/
  if ( (util_pmv_amont_fmc(vm_pmv.PR,
			   vm_pmv.Sens,
			   vl_loc.PR,
			   vl_loc_tete.PR,
			   vl_loc.Sens,
			   vl_inversee)
       )
       &&
       ( (((vl_loc.Sens==XDC_SENS_INCONNU)|| (vl_loc.Sens>10)))
	  &&
	  (vm_pmv.Sens==XDC_SENS_SUD)
	  &&
	  (vl_loc.DistanceSortieAmont < C_PMV_EPSILON)
	 )
	 ||
	 (((vl_loc.Sens==XDC_SENS_INCONNU)|| (vl_loc.Sens>10))
	  &&
	  (vm_pmv.Sens==XDC_SENS_NORD)
	  &&
	  (vl_loc.DistanceSortieAval < C_PMV_EPSILON)
         )
	 ||
	 ((((vl_loc.Sens!=XDC_SENS_INCONNU)|| (vl_loc.Sens>10))
	  &&
	  (vl_loc_tete.DistanceSortieAval < C_PMV_EPSILON)
         )
       )
       &&
       (vm_pmv.Distance_C > C_PMV_DMAX_FIN)
       &&
       (vl_loc_tete.PR != vl_loc.PR)
     ) {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"a %d km", (vm_pmv.Distance_C+500)/1000);
    else if ((((int)(vm_pmv.Distance_C/100))*100) > 0)
      sprintf(vl_str,"a %d m", ((int)(vm_pmv.Distance_C/100))*100);
    else
      sprintf(vl_str,"");
    vm_bouchon_apres_sortie = FALSE;
    return (vl_str);
  }

  /*localisation entre deux sorties (nom)*/
/******************************************
/** DEM1148
  if ( (util_pmv_amont_fmc(vm_pmv.PR,
		      vm_pmv.Sens,
		      vl_loc.PR,
		      vl_loc_tete.PR,
		      vl_loc.Sens,
		      vl_inversee)
	)
	&&
	(util_pmv_amont_sortie(vm_pmv.Distance_C,
			       vm_pmv.Sens,
			       vl_loc.DistanceSortieAmont,
		               vl_loc_tete.DistanceSortieAval,
			       vl_loc.Sens,
			       vm_noeud->Infos_TFM.Zone_Urbaine)
        )
	&&
	(vl_loc.DistanceSortieAmont < C_PMV_EPSILON)
	&&
	(vl_loc_tete.DistanceSortieAval < C_PMV_EPSILON)
       &&
       (vl_loc_tete.PR != vl_loc.PR)
     )
  {
XZST_03EcritureTrace(XZSTC_WARNING,"Ici %d!=%d %d %d",vl_loc_tete.PR,vl_loc.PR,vl_loc.DistanceSortieAmont ,vl_loc_tete.DistanceSortieAval);
    if ( ((vl_loc.Sens==XDC_SENS_INCONNU)|| (vl_loc.Sens>10)) && (vm_pmv.Sens==XDC_SENS_SUD))
      sprintf(vl_str,"de %s\na %s",
			  vl_loc_tete.AbrevSortieAval,
			  vl_loc.AbrevSortieAmont);
    else
      sprintf(vl_str,"de %s\na %s",
			  vl_loc.AbrevSortieAmont,
			  vl_loc_tete.AbrevSortieAval);
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }
**************************************************/


  /*localisation par defaut*/
  if ( (!(vm_pmv.Presence_Bifurcation)
	&&
	( ( ((vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD) )
	    &&
	    (vm_pmv.PR*vl_inversee < vl_loc.PR * vl_inversee)
	  )
	  ||
	  ( ( (vm_pmv.Sens==XDC_SENS_NORD) || (vm_pmv.Sens==XDC_SENS_ENTREE_NORD) )
	    &&
	    (vm_pmv.PR*vl_inversee > vl_loc.PR * vl_inversee)
          )
	)
       )
       ||
       (vm_pmv.Presence_Bifurcation)
     )
  {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"a %d km", (vm_pmv.Distance_C+500)/1000);
    else if ((((int)(vm_pmv.Distance_C/100))*100) > 0)
      sprintf(vl_str,"a %d m", ((int)(vm_pmv.Distance_C/100))*100);
    else
      sprintf(vl_str,"");
    vm_bouchon_apres_sortie = FALSE;
    return (vl_str);
  } 

  /*localisation - indication de la longueur separant de la fin*/
  if ( ( ( ( (vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD) )
	   &&
	   (vm_pmv.PR*vl_inversee >= vl_loc.PR * vl_inversee) 
	   &&
	   (vm_pmv.PR*vl_inversee < vl_loc_tete.PR * vl_inversee)
         )
         ||
         (  ( (vm_pmv.Sens==XDC_SENS_NORD) || (vm_pmv.Sens==XDC_SENS_ENTREE_NORD) )
	   &&
	   (vm_pmv.PR*vl_inversee <= vl_loc.PR * vl_inversee)
	   &&
	   (vm_pmv.PR*vl_inversee > vl_loc_tete.PR * vl_inversee)
         )
       )
       &&
       (vm_pmv.Distance_C > C_PMV_DMAX_FIN)
       &&
       (vl_loc.PR != vl_loc_tete.PR)
       &&
       (!(vm_pmv.Presence_Bifurcation))
     )
  {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"sur %d km", (vm_pmv.Distance_C+500)/1000);
    else if (vm_pmv.Distance_C > 0)
      sprintf(vl_str,"sur %d m", ((int)(vm_pmv.Distance_C/100))*100);
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }

  /*localisation - indication de la distance separant de la fin*/
  if ( ( ( ( (vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD)   )
	   &&
	   (vm_pmv.PR*vl_inversee > vl_loc.PR * vl_inversee)
	   &&
	   (vm_pmv.PR*vl_inversee < vl_loc_tete.PR * vl_inversee)
         )
         ||
         ( ( (vm_pmv.Sens==XDC_SENS_NORD) || (vm_pmv.Sens==XDC_SENS_ENTREE_NORD) )
	   &&
	   (vm_pmv.PR*vl_inversee < vl_loc.PR * vl_inversee)
	   &&
	   (vm_pmv.PR*vl_inversee > vl_loc_tete.PR * vl_inversee)
         )
       )
       &&
       (vm_pmv.Distance_C <= C_PMV_DMAX_FIN)
       &&
       (!(vm_pmv.Presence_Bifurcation))
    )
  {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"a %d km", (vm_pmv.Distance_C+500)/1000);
    else if ((((int)(vm_pmv.Distance_C/100))*100) > 0)
      sprintf(vl_str,"a %d m", ((int)(vm_pmv.Distance_C/100))*100);
    else
      sprintf(vl_str,"");
    vm_bouchon_apres_sortie = FALSE;
    return (vl_str);
  }

  /*localisation - autre cas*/
  if ( ( ( ( (vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD) )
	   &&
	   (vm_pmv.PR*vl_inversee > vl_loc.PR * vl_inversee)
         )
         ||
         ( ( (vm_pmv.Sens==XDC_SENS_NORD) || (vm_pmv.Sens==XDC_SENS_ENTREE_NORD))
	   &&
	   (vm_pmv.PR*vl_inversee < vl_loc.PR * vl_inversee)
         )
       )
    )
  {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"a %d km", (vm_pmv.Distance_C+500)/1000);
    else if ((((int)(vm_pmv.Distance_C/100))*100) > 0)
      sprintf(vl_str,"a %d m", ((int)(vm_pmv.Distance_C/100))*100);
    else
      sprintf(vl_str,"");
    vm_bouchon_apres_sortie = FALSE;
    return (vl_str);
  }
  return ("");
}




static char *drol_pmva_var_localisation_sortie() {
  T_Localisation vl_loc;
  T_Localisation vl_loc_tete;
  static char vl_str[150];
  int vl_inversee=1;

  vl_loc=vm_noeud->Localisation;
  vl_loc_tete=vm_noeud->Localisation_Tete;
  vm_bouchon_apres_sortie = TRUE;

  /*PATCH A57*/
  if (vl_loc.Autoroute==8)
    vl_inversee=-1;

  XZST_03EcritureTrace(XZSTC_WARNING,"LOCAL SORTIE %d, %d, %d, %d, BIFURC %d",
			vm_pmv.PR,
			vl_loc.PR,
			vl_loc_tete.PR,
			vl_inversee,
			vm_pmv.Presence_Bifurcation);

  /*localisation entre deux sorties (nom et numero)*/
  if (  (util_pmv_amont_fmc(vm_pmv.PR,
			vm_pmv.Sens,
			vl_loc.PR,
			vl_loc_tete.PR,
			vl_loc.Sens,
			vl_inversee)
        )
	&&
	(util_pmv_amont_sortie(vm_pmv.Distance_C,
				vm_pmv.Sens,
				vl_loc.DistanceSortieAmont,
				vl_loc_tete.DistanceSortieAval,
				vl_loc.Sens,
				vm_noeud->Infos_TFM.Zone_Urbaine)
        )
	&&
	(vl_loc.DistanceSortieAmont < C_PMV_EPSILON)
	&&
	(vl_loc_tete.DistanceSortieAval < C_PMV_EPSILON)
	&&
	(vl_loc.PR != vl_loc_tete.PR)
      ) {
    if ( (vl_loc.Sens==XDC_SENS_INCONNU) && 
	  ( (vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD)))
      sprintf(vl_str,"apres SORTIE %s \n %s",
			vl_loc_tete.SortieAvalUsager,
			vl_loc_tete.AbrevSortieAval);
    else
      sprintf(vl_str,"apres SORTIE %s \n %s",
			vl_loc.SortieAmontUsager,
			vl_loc.AbrevSortieAmont);
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }


  /*localisation par rapport a la sortie amont*/
  if ( (util_pmv_amont_fmc(vm_pmv.PR,
			   vm_pmv.Sens,
			   vl_loc.PR,
			   vl_loc_tete.PR,
			   vl_loc.Sens,
			   vl_inversee)
       )
       &&
       (util_pmv_amont_sortie(vm_pmv.Distance_C,
			      vm_pmv.Sens,
			      vl_loc.DistanceSortieAmont,
			      vl_loc_tete.DistanceSortieAval,
			      vl_loc.Sens,
			      vm_noeud->Infos_TFM.Zone_Urbaine)
       )
     ) {
    if ( (vl_loc.Sens==XDC_SENS_INCONNU) && 
	 ( (vm_pmv.Sens==XDC_SENS_SUD)|| (vm_pmv.Sens==XDC_SENS_ENTREE_SUD)))
      sprintf(vl_str,"apres SORTIE %s \n %s",
			vl_loc_tete.SortieAmontUsager,
			vl_loc_tete.AbrevSortieAval);
    else
      sprintf(vl_str,"apres SORTIE %s \n %s",
			vl_loc.SortieAmontUsager,
			vl_loc.AbrevSortieAmont);
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }

  /*localisation de la fin a la sortie aval (nom et numero)*/
  if ( (util_pmv_amont_fmc(vm_pmv.PR,
			   vm_pmv.Sens,
			   vl_loc.PR,
			   vl_loc_tete.PR,
			   vl_loc.Sens,
			   vl_inversee)
       )
       &&
       ( ((vl_loc.Sens==XDC_SENS_INCONNU)
	  &&
	  (vm_pmv.Sens==XDC_SENS_SUD)
	  &&
	  (vl_loc.DistanceSortieAmont < C_PMV_EPSILON)
	 )
	 ||
	 ((vl_loc.Sens==XDC_SENS_INCONNU)
	  &&
	  (vm_pmv.Sens==XDC_SENS_NORD)
	  &&
	  (vl_loc.DistanceSortieAval < C_PMV_EPSILON)
         )
	 ||
	 ((vl_loc.Sens!=XDC_SENS_INCONNU)
	  &&
	  (vl_loc_tete.DistanceSortieAval < C_PMV_EPSILON)
         )
       )
       &&
       (vm_pmv.Distance_C > C_PMV_DMAX_FIN)
       &&
       (vl_loc_tete.PR != vl_loc.PR)
     ) {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"a %d km", (vm_pmv.Distance_C+500)/1000);
    else if ((((int)(vm_pmv.Distance_C/100))*100) > 0)
      sprintf(vl_str,"a %d m", ((int)(vm_pmv.Distance_C/100))*100);
    else
      sprintf(vl_str,"");
    vm_bouchon_apres_sortie = FALSE;
    return (vl_str);
  }

  /*localisation entre deux sorties (nom)*/
  if ( (util_pmv_amont_fmc(vm_pmv.PR,
		      vm_pmv.Sens,
		      vl_loc.PR,
		      vl_loc_tete.PR,
		      vl_loc.Sens,
		      vl_inversee)
	)
	&&
	(util_pmv_amont_sortie(vm_pmv.Distance_C,
			       vm_pmv.Sens,
			       vl_loc.DistanceSortieAmont,
		               vl_loc_tete.DistanceSortieAval,
			       vl_loc.Sens,
			       vm_noeud->Infos_TFM.Zone_Urbaine)
        )
	&&
	(vl_loc.DistanceSortieAmont < C_PMV_EPSILON)
	&&
	(vl_loc_tete.DistanceSortieAval < C_PMV_EPSILON)
     )
  {
    if ( (vl_loc.Sens==XDC_SENS_INCONNU) && (vm_pmv.Sens==XDC_SENS_SUD))
      sprintf(vl_str,"de %s\na %s",
			  vl_loc_tete.AbrevSortieAval,
			  vl_loc.AbrevSortieAmont);
    else
      sprintf(vl_str,"de %s\na %s",
			  vl_loc.AbrevSortieAmont,
			  vl_loc_tete.AbrevSortieAval);
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }

  /*localisation par defaut*/
  if ( (!(vm_pmv.Presence_Bifurcation)
	&&
	( ( ((vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD) )
	    &&
	    (vm_pmv.PR*vl_inversee < vl_loc.PR * vl_inversee)
	  )
	  ||
	  ( ( (vm_pmv.Sens==XDC_SENS_NORD) || (vm_pmv.Sens==XDC_SENS_ENTREE_NORD) )
	    &&
	    (vm_pmv.PR*vl_inversee > vl_loc.PR * vl_inversee)
          )
	)
       )
       ||
       (vm_pmv.Presence_Bifurcation)
     )
  {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"a %d km", (vm_pmv.Distance_C+500)/1000);
    else if ((((int)(vm_pmv.Distance_C/100))*100) > 0)
      sprintf(vl_str,"a %d m", ((int)(vm_pmv.Distance_C/100))*100);
    else
      sprintf(vl_str,"");
    vm_bouchon_apres_sortie = FALSE;
    return (vl_str);
  } 

  /*localisation - indication de la longueur separant de la fin*/
  if ( ( ( ( (vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD) )
	   &&
	   (vm_pmv.PR*vl_inversee >= vl_loc.PR * vl_inversee) 
	   &&
	   (vm_pmv.PR*vl_inversee < vl_loc_tete.PR * vl_inversee)
         )
         ||
         (  ( (vm_pmv.Sens==XDC_SENS_NORD) || (vm_pmv.Sens==XDC_SENS_ENTREE_NORD) )
	   &&
	   (vm_pmv.PR*vl_inversee <= vl_loc.PR * vl_inversee)
	   &&
	   (vm_pmv.PR*vl_inversee > vl_loc_tete.PR * vl_inversee)
         )
       )
       &&
       (vm_pmv.Distance_C > C_PMV_DMAX_FIN)
       &&
       (vl_loc.PR != vl_loc_tete.PR)
       &&
       (!(vm_pmv.Presence_Bifurcation))
     )
  {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"sur %d km", (vm_pmv.Distance_C+500)/1000);
    else if (vm_pmv.Distance_C > 0)
      sprintf(vl_str,"sur %d m", ((int)(vm_pmv.Distance_C/100))*100);
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }

  /*localisation - indication de la distance separant de la fin*/
  if ( ( ( ( (vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD)   )
	   &&
	   (vm_pmv.PR*vl_inversee > vl_loc.PR * vl_inversee)
	   &&
	   (vm_pmv.PR*vl_inversee < vl_loc_tete.PR * vl_inversee)
         )
         ||
         ( ( (vm_pmv.Sens==XDC_SENS_NORD) || (vm_pmv.Sens==XDC_SENS_ENTREE_NORD) )
	   &&
	   (vm_pmv.PR*vl_inversee < vl_loc.PR * vl_inversee)
	   &&
	   (vm_pmv.PR*vl_inversee > vl_loc_tete.PR * vl_inversee)
         )
       )
       &&
       (vm_pmv.Distance_C <= C_PMV_DMAX_FIN)
       &&
       (!(vm_pmv.Presence_Bifurcation))
    )
  {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"a %d km", (vm_pmv.Distance_C+500)/1000);
    else if ((((int)(vm_pmv.Distance_C/100))*100) > 0)
      sprintf(vl_str,"a %d m", ((int)(vm_pmv.Distance_C/100))*100);
    else
      sprintf(vl_str,"");
    vm_bouchon_apres_sortie = FALSE;
    return (vl_str);
  }

  /*localisation - autre cas*/
  if ( ( ( ( (vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD) )
	   &&
	   (vm_pmv.PR*vl_inversee > vl_loc.PR * vl_inversee)
         )
         ||
         ( ( (vm_pmv.Sens==XDC_SENS_NORD) || (vm_pmv.Sens==XDC_SENS_ENTREE_NORD))
	   &&
	   (vm_pmv.PR*vl_inversee < vl_loc.PR * vl_inversee)
         )
       )
    )
  {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"a %d km", (vm_pmv.Distance_C+500)/1000);
    else if ((((int)(vm_pmv.Distance_C/100))*100) > 0)
      sprintf(vl_str,"a %d m", ((int)(vm_pmv.Distance_C/100))*100);
    else
      sprintf(vl_str,"");
    vm_bouchon_apres_sortie = FALSE;
    return (vl_str);
  }
  return ("");
}


static char *drol_pmva_var_localisation_km() {
  T_Localisation vl_loc;
  T_Localisation vl_loc_tete;
  static char vl_str[20];
  int vl_inversee=1;

  vl_loc=vm_noeud->Localisation;
  vl_loc_tete=vm_noeud->Localisation_Tete;
  vm_bouchon_apres_sortie = TRUE;

  /*PATCH A57*/
  if (vl_loc.Autoroute==8)
    vl_inversee=-1;

  XZST_03EcritureTrace(XZSTC_WARNING,"LOCAL %d, %d, %d, %d, BIFURC %d",
			vm_pmv.PR,
			vl_loc.PR,
			vl_loc_tete.PR,
			vl_inversee,
			vm_pmv.Presence_Bifurcation);

  	
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"a %d km", (vm_pmv.Distance_C+500)/1000);
    else if ((((int)(vm_pmv.Distance_C/100))*100) > 0)
      sprintf(vl_str,"a %d m", ((int)(vm_pmv.Distance_C/100))*100);
    else
      sprintf(vl_str,"");
    vm_bouchon_apres_sortie = FALSE;
    return (vl_str);

}


static char *drol_pmva_var_localisation_km_long() {
  T_Localisation vl_loc;
  T_Localisation vl_loc_tete;
  static char vl_str[20];
  int vl_inversee=1;

  vl_loc=vm_noeud->Localisation;
  vl_loc_tete=vm_noeud->Localisation_Tete;
  vm_bouchon_apres_sortie = TRUE;

  /*PATCH A57*/
  if (vl_loc.Autoroute==8)
    vl_inversee=-1;

  XZST_03EcritureTrace(XZSTC_WARNING,"LOCAL %d, %d, %d, %d, BIFURC %d",
			vm_pmv.PR,
			vl_loc.PR,
			vl_loc_tete.PR,
			vl_inversee,
			vm_pmv.Presence_Bifurcation);
 		
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"sur %d km", (vm_pmv.Distance_C+500)/1000);
    else if ((((int)(vm_pmv.Distance_C/100))*100) > 0)
      sprintf(vl_str,"sur %d m", ((int)(vm_pmv.Distance_C/100))*100);
    else
      sprintf(vl_str,"");
    vm_bouchon_apres_sortie = FALSE;
    return (vl_str);		

}


static char *drol_pmva_var_localisation_tdp() {
  T_Localisation vl_loc;
  T_Localisation vl_loc_tete;
  static char vl_str[150];
  int vl_inversee=1;

  vl_loc=vm_noeud->Localisation;
  vl_loc_tete=vm_noeud->Localisation_Tete;
  vm_bouchon_apres_sortie = TRUE;

  /*PATCH A57*/
  if (vl_loc.Autoroute==8)
    vl_inversee=-1;

  XZST_03EcritureTrace(XZSTC_WARNING,"TDP LOCAL %d, %d, %d, %d, BIFURC %d",
			vm_pmv.PR,
			vl_loc.PR,
			vl_loc_tete.PR,
			vl_inversee,
			vm_pmv.Presence_Bifurcation);

  /*localisation entre deux sorties (nom et numero)*/
  if (  (util_pmv_amont_fmc(vm_pmv.PR,
			vm_pmv.Sens,
			vl_loc.PR,
			vl_loc_tete.PR,
			vl_loc.Sens,
			vl_inversee)
        )
	&&
	(util_pmv_amont_sortie(vm_pmv.Distance_C,
				vm_pmv.Sens,
				vl_loc.DistanceSortieAmont,
				vl_loc_tete.DistanceSortieAval,
				vl_loc.Sens,
				vm_noeud->Infos_TFM.Zone_Urbaine)
        )
	&&
	(vl_loc.DistanceSortieAmont < C_PMV_EPSILON)
	&&
	(vl_loc_tete.DistanceSortieAval < C_PMV_EPSILON)
	&&
	(vl_loc.PR != vl_loc_tete.PR)
      ) {
    if ( (vl_loc.Sens==XDC_SENS_INCONNU) && 
	  ( (vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD)))
      sprintf(vl_str,"apres SORTIE %s",
			vl_loc_tete.SortieAvalUsager);
		/*	vl_loc_tete.NumSortieAval);*/
    else
      sprintf(vl_str,"apres SORTIE %s", vl_loc.SortieAmontUsager);
     /* sprintf(vl_str,"apres SORTIE %d", vl_loc.NumSortieAmont);*/
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }


  /*localisation par rapport a la sortie amont*/
  if ( (util_pmv_amont_fmc(vm_pmv.PR,
			   vm_pmv.Sens,
			   vl_loc.PR,
			   vl_loc_tete.PR,
			   vl_loc.Sens,
			   vl_inversee)
       )
       &&
       (util_pmv_amont_sortie(vm_pmv.Distance_C,
			      vm_pmv.Sens,
			      vl_loc.DistanceSortieAmont,
			      vl_loc_tete.DistanceSortieAval,
			      vl_loc.Sens,
			      vm_noeud->Infos_TFM.Zone_Urbaine)
       )
     ) {
    if ( (vl_loc.Sens==XDC_SENS_INCONNU) && 
	 ( (vm_pmv.Sens==XDC_SENS_SUD)|| (vm_pmv.Sens==XDC_SENS_ENTREE_SUD)))
      sprintf(vl_str,"apres SORTIE %s",
			vl_loc_tete.SortieAvalUsager );
			/*vl_loc_tete.NumSortieAval );*/
    else
      sprintf(vl_str,"apres SORTIE %s",
			vl_loc.SortieAmontUsager);
			/*vl_loc.NumSortieAmont);*/
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }

  /*localisation de la fin a la sortie aval (nom et numero)*/
  if ( (util_pmv_amont_fmc(vm_pmv.PR,
			   vm_pmv.Sens,
			   vl_loc.PR,
			   vl_loc_tete.PR,
			   vl_loc.Sens,
			   vl_inversee)
       )
       &&
       ( ((vl_loc.Sens==XDC_SENS_INCONNU)
	  &&
	  (vm_pmv.Sens==XDC_SENS_SUD)
	  &&
	  (vl_loc.DistanceSortieAmont < C_PMV_EPSILON)
	 )
	 ||
	 ((vl_loc.Sens==XDC_SENS_INCONNU)
	  &&
	  (vm_pmv.Sens==XDC_SENS_NORD)
	  &&
	  (vl_loc.DistanceSortieAval < C_PMV_EPSILON)
         )
	 ||
	 ((vl_loc.Sens!=XDC_SENS_INCONNU)
	  &&
	  (vl_loc_tete.DistanceSortieAval < C_PMV_EPSILON)
         )
       )
       &&
       (vm_pmv.Distance_C > C_PMV_DMAX_FIN)
       &&
       (vl_loc_tete.PR != vl_loc.PR)
     ) {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"a %d km", (vm_pmv.Distance_C+500)/1000);
    else if ((((int)(vm_pmv.Distance_C/100))*100) > 0)
      sprintf(vl_str,"a %d m", ((int)(vm_pmv.Distance_C/100))*100);
    else
      sprintf(vl_str,"");
    vm_bouchon_apres_sortie = FALSE;
    return (vl_str);
  }

  /*localisation entre deux sorties (nom)*/
  if ( (util_pmv_amont_fmc(vm_pmv.PR,
		      vm_pmv.Sens,
		      vl_loc.PR,
		      vl_loc_tete.PR,
		      vl_loc.Sens,
		      vl_inversee)
	)
	&&
	(util_pmv_amont_sortie(vm_pmv.Distance_C,
			       vm_pmv.Sens,
			       vl_loc.DistanceSortieAmont,
		               vl_loc_tete.DistanceSortieAval,
			       vl_loc.Sens,
			       vm_noeud->Infos_TFM.Zone_Urbaine)
        )
	&&
	(vl_loc.DistanceSortieAmont < C_PMV_EPSILON)
	&&
	(vl_loc_tete.DistanceSortieAval < C_PMV_EPSILON)
     )
  {
    if ( (vl_loc.Sens==XDC_SENS_INCONNU) && (vm_pmv.Sens==XDC_SENS_SUD))
      sprintf(vl_str,"de %s\na %s",
			  vl_loc_tete.AbrevSortieAval,
			  vl_loc.AbrevSortieAmont);
    else
      sprintf(vl_str,"de %s\na %s",
			  vl_loc.AbrevSortieAmont,
			  vl_loc_tete.AbrevSortieAval);
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }

  /*localisation par defaut*/
  if ( (!(vm_pmv.Presence_Bifurcation)
	&&
	( ( ((vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD) )
	    &&
	    (vm_pmv.PR*vl_inversee < vl_loc.PR * vl_inversee)
	  )
	  ||
	  ( ( (vm_pmv.Sens==XDC_SENS_NORD) || (vm_pmv.Sens==XDC_SENS_ENTREE_NORD) )
	    &&
	    (vm_pmv.PR*vl_inversee > vl_loc.PR * vl_inversee)
          )
	)
       )
       ||
       (vm_pmv.Presence_Bifurcation)
     )
  {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"a %d km", (vm_pmv.Distance_C+500)/1000);
    else if ((((int)(vm_pmv.Distance_C/100))*100) > 0)
      sprintf(vl_str,"a %d m", ((int)(vm_pmv.Distance_C/100))*100);
    else
      sprintf(vl_str,"");
    vm_bouchon_apres_sortie = FALSE;
    return (vl_str);
  } 

  /*localisation - indication de la longueur separant de la fin*/
  if ( ( ( ( (vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD) )
	   &&
	   (vm_pmv.PR*vl_inversee >= vl_loc.PR * vl_inversee) 
	   &&
	   (vm_pmv.PR*vl_inversee < vl_loc_tete.PR * vl_inversee)
         )
         ||
         (  ( (vm_pmv.Sens==XDC_SENS_NORD) || (vm_pmv.Sens==XDC_SENS_ENTREE_NORD) )
	   &&
	   (vm_pmv.PR*vl_inversee <= vl_loc.PR * vl_inversee)
	   &&
	   (vm_pmv.PR*vl_inversee > vl_loc_tete.PR * vl_inversee)
         )
       )
       &&
       (vm_pmv.Distance_C > C_PMV_DMAX_FIN)
       &&
       (vl_loc.PR != vl_loc_tete.PR)
       &&
       (!(vm_pmv.Presence_Bifurcation))
     )
  {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"sur %d km", (vm_pmv.Distance_C+500)/1000);
    else if (vm_pmv.Distance_C > 0)
      sprintf(vl_str,"sur %d m", ((int)(vm_pmv.Distance_C/100))*100);
    vm_bouchon_apres_sortie = TRUE;
    return (vl_str);
  }

  /*localisation - indication de la distance separant de la fin*/
  if ( ( ( ( (vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD)   )
	   &&
	   (vm_pmv.PR*vl_inversee > vl_loc.PR * vl_inversee)
	   &&
	   (vm_pmv.PR*vl_inversee < vl_loc_tete.PR * vl_inversee)
         )
         ||
         ( ( (vm_pmv.Sens==XDC_SENS_NORD) || (vm_pmv.Sens==XDC_SENS_ENTREE_NORD) )
	   &&
	   (vm_pmv.PR*vl_inversee < vl_loc.PR * vl_inversee)
	   &&
	   (vm_pmv.PR*vl_inversee > vl_loc_tete.PR * vl_inversee)
         )
       )
       &&
       (vm_pmv.Distance_C <= C_PMV_DMAX_FIN)
       &&
       (!(vm_pmv.Presence_Bifurcation))
    )
  {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"a %d km", (vm_pmv.Distance_C+500)/1000);
    else if ((((int)(vm_pmv.Distance_C/100))*100) > 0)
      sprintf(vl_str,"a %d m", ((int)(vm_pmv.Distance_C/100))*100);
    else
      sprintf(vl_str,"");
    vm_bouchon_apres_sortie = FALSE;
    return (vl_str);
  }

  /*localisation - autre cas*/
  if ( ( ( ( (vm_pmv.Sens==XDC_SENS_SUD) || (vm_pmv.Sens==XDC_SENS_ENTREE_SUD) )
	   &&
	   (vm_pmv.PR*vl_inversee > vl_loc.PR * vl_inversee)
         )
         ||
         ( ( (vm_pmv.Sens==XDC_SENS_NORD) || (vm_pmv.Sens==XDC_SENS_ENTREE_NORD))
	   &&
	   (vm_pmv.PR*vl_inversee < vl_loc.PR * vl_inversee)
         )
       )
    )
  {
    if (vm_pmv.Distance_C > 500)
      sprintf(vl_str,"a %d km", (vm_pmv.Distance_C+500)/1000);
    else if ((((int)(vm_pmv.Distance_C/100))*100) > 0)
      sprintf(vl_str,"a %d m", ((int)(vm_pmv.Distance_C/100))*100);
    else
      sprintf(vl_str,"");
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

static char *drol_pmva_majuscules(char *Texte)

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
        int     i;
        int     l;

        /*A
        ** on passe en majuscules SANS accent
        */
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

	if (Texte[0]==' ')
	  return (Texte);

        /*A
        ** on enleve les blancs a la fin de la chaine
        */
        for (i=l-1; i>=0;i--)
        {
                if (Texte[i] == ' ')
                {
                        Texte[i] = '\0';
                }
                else
                        break;
        }
        return(Texte);
}

static char *drol_pmva_var_traversee() {
  static char vl_str[50];
  XDY_PR vl_pr_min;
  XDY_PR vl_pr_max;
  XDY_TDP vl_tdp;
  XZADT_CONFIG_PMV pl_pmv;

  vm_pmv.Dest_un_proche=0;
  if (vm_noeud->Localisation.PR < vm_noeud->Localisation_Tete.PR) {
        vl_pr_min = vm_noeud->Localisation.PR;
        vl_pr_max = vm_noeud->Localisation_Tete.PR;
  }
  else {
        vl_pr_max = vm_noeud->Localisation.PR;
        vl_pr_min = vm_noeud->Localisation_Tete.PR;
  }

  if (XZAD_recherche_dest_pmv(vm_pmv.Identifiant, &pl_pmv)!=XDC_OK)
      return ("");

  if (XZAD30_Calculer_Traversee(vm_pmv.Autoroute,
                                vm_pmv.PR,
                                vm_pmv.Sens,
                                vl_pr_min,
                                vl_pr_max,
                                pl_pmv.Graphe_Destination,
                                &vl_tdp) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"drol_pmv_var_traversee : pb appel a XZAD30_Calculer_Traversee");
    return ("");
  }

  if (vl_tdp <= 0)
    return ("");

   if (vl_tdp<=10)
     return ("");
     
   strcpy(vl_str,"TRAVERSEE :");
   if ( vl_tdp > 600 ) {
     return ("");
     sprintf( &vl_str[11], "%1dh%02d", 9, 59 );
     vl_str[15] = '\0';
   }
   else if ( vl_tdp > 59 )
   {
        sprintf( &vl_str[11], "%1dh%02d", vl_tdp/60, vl_tdp%60 );
       vl_str[15] = '\0';
   }
   else  if ( vl_tdp > 0 ) {
              sprintf ( &vl_str[11], "%2dmn", vl_tdp );
           vl_str[15] = '\0';
  }
   else {
        vm_dest1proche_HS=TRUE;
        strcpy(vl_str,"");
   }
  XZST_03EcritureTrace(XZSTC_FONCTION,"traversee : %s",vl_str);
  vm_pmv.Dest_un_proche=XDC_DESTINATION_TRAVERSEE;
  vm_dest1proche_numero=XDC_DESTINATION_TRAVERSEE;

  return (vl_str);
}

