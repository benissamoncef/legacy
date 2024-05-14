/*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : agen_evt.c				*
*************************************************
* DESCRIPTION :
* module externe de la tache passerelle generique
* 
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
*************************************************
* HISTORIQUE :
*
* Guilhou	01/11/08 : Creation 	     V1.1 DEM 835
* JMG		17/11/08 : correction libelle Meteo Autre 1.2 DEM 835
* JMG            12/10/08        ajout XDG_AGEN_TIMER DEM 837
* JPL		31/08/10 : Ajout Fmc Zone de stockage (DEM 942) 1.4
* JPL		18/11/10 : Suppression warnings mineurs pour compilation sur architecture HP ia64 (DEM 961)  1.5
* JMG		15/02/11 : passage tacop 1.6
* JMG		08/08/11 : suppression accents 1.7
* JMG		17/10/11 : correction accents 1.8
* VR		07/10/11 : Ajout vitesse et vitesse_opposee 1.9 (DEM/1015)
* JMG		16/01/12 : correction accents 1.9
* JMG		16/02/12 : correction accents 1.10
* VR		09/03/12 : Ajout XZLG565_GetFmcRegulation (DEM/1016)
* JMG		17/09/12 : zone de stockage comme travaux si pas demarre 
* PNI		18/09/13 : filtrage de " et ' 1.14
* ABE		24/09/20 : Appel agen_evt_genere_strada pour une FMC synchroniser sur le réveil de XZIT08. SAE-DEM130 V1.17
* PNI		14/10/20 : correctif copie l10 et l06 dans agen_evt_genere_strada V1.18
* ABE		23/10/20 : Suppression tagen gestion des CPC SAE-DEM130 V1.18
* ABE		15/04/21 : Ajout debu_prevu et fin_previ SAE-DEM130 1.19
************************************************/

#include "agen_evt.h"
#include "xzao000.h"
#include "xzao551.h"
#include "xzlg.h"
#include "xzae565.h"

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#define CO_NB_JOUR_PURGE        3
#define CO_DELAI_FREQUENCE      360
#define CO_DELAI_PURGE          86400 * CO_NB_JOUR_PURGE
#define CO_NB_CHIFFRE           13

/* definition des constantes */
#define MY_ENCODING		"ISO-8859-1"

#define CO_FIC_COMPTEUR         XDC_PATH_STRADA "/compteur_evt.txt"
#define CO_FIC_EVTSTR		XDC_PATH_STRADA "/evt_out.txt"
#define	CO_MAX_CARACTERE	80
#define TAILLE 200
#define C_FREQ_PURGE		86400 * 3
#define C_TAB_SEPAR		"\t"
#define	C_FREQ_VERIF_EVT	60
#define C_CODE_ESCOTA		"ESCOTA"
#define C_DATEX_ESCOTA		"fr204"

void gen_cumul_voies(XDY_Voie, int *, int *);


#define NBR_PREMIER 	53
#define NBR_ELT_TABLE	59

#define C_l01		"F32"
#define C_l02		"LOC"
#define C_l03		4
#define C_l07_P		"Aix en Provence - Frontiere Italienne"
#define C_l07_N		"Frontiere italienne - Aix en Provence"
#define C_l07_U		"Sens non defini"


#define TAILLE_SUR	350

/* dÅfinition des variables globales È l'application */


/* definition des variables locales au module */
XZLGT_Liste_FMC		*tg_fmc;
XZLGT_Liste_FMC		*tg_fmc_courant;
static XZLGT_Nombre_Mem vm_nombre_mem;
int vm_num_prec_regul;
int vm_cle_prec_regul;
int vm_indice_regul;

static char			*tm_l07_p[]={
	"",
	"Aix en Provence - Frontiere Italienne",
	"A8 - Monaco",
	"Marseille - Toulon",
	"Marseille - A52",
	"Aix - Les Alpes",
	"Aix - Aubagne",
	"A52 - Auriol",
	"Le Luc - Toulon"};

static char			*tm_l07_n[]={
	"",
	"Frontiere Italienne - Aix en Provence",
	"MonacoA8 - Monaco",
	"Toulon - Marseille",
	"A52 - Marseille",
	"Les Alpes - Aix",
	"Aubagne - Aix",
	"Auriol - A52",
	"Toulon - Le Luc"};

static XDY_Entier vm_nb_fmc=0;

static char vm_commandes[500]="";

static char pm_comment[TAILLE_SUR+1]="";
 
static char *	version = "@(#)agen_evt.c	1.26 11/05/07 : commentaire" ;


/* declaration de fonctions internes */
void	agen_evt_purge();
void	agen_evt_cb_validation_fmc();
int	agen_evt_genere_strada(int);
int	agen_evt_genere_commentaire();
/* definition de fonctions externes */



/*A
*******************************************
* Fonction d'initialisation pour la tache
* serveur de donnees trafic
 Elle lit les fichiers de parametrage
* et arme les deux timers :
* - le timer de purge
* - charger le tableau de listes chainees
* contenant l'intitule relatif au code 
* evenement Strada
*******************************************
* Arguments : Aucun
*******************************************/

int agen_evt_init()
{
XZSMT_CleProg		vl_cle;
XZSCT_Datagroup 	vl_Datagroup ;
int 			vl_JourSemaine;
double 			vl_HorodateSec; 
XZSCT_NomMachine 	pl_NomMachine	= "\0";
T_BOOL 			vl_etat;
int			vl_val;
char			vl_str1[80];
char			*vl_chaine;
int			vl_ValRet;
XDY_NomSite		pl_NomSite;

  	/* Recuperation du nom de la machine locale */
	XZSC_07NomMachine(pl_NomMachine) ;
  	
	
	/*A initialisation des timers */
	XZSM_00InitServices();
	
	/*A Lecture horodate courante systeme */
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
   
	/*abonnement au dg pour les validations de fmc */
	XDG_JoindreDG(XDG_AGEN_TIMER);
	
	vl_HorodateSec+=0;
	/* armement du timer de programmation */
	if (XZSM_01DemanderProgrammation(       vl_HorodateSec,
						0,
						XDG_AGEN_TIMER,
						XDM_AS_SRVEVT_STRADA_FMC,
						"cle_timer_datexII",
						XZSMC_SEC_VRAI) != XDC_OK)
	{
	  XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de verif des evts non declenchee." );
	  return (XDC_NOK);
  	}

	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_SRVEVT_STRADA_FMC), agen_evt_cb_validation_fmc,(T_CB_ARG)NULL) == NULL)
	{
		XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AS_SRVEVT_STRADA non cree.",version );
		return XDC_NOK;
	}
	else
		XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AS_SRVEVT_STRADA cree.",version );

	vm_nombre_mem.Nombre_ZDP=0;
	vm_nombre_mem.Nombre_FMC=0;
	vm_nombre_mem.Nombre_RDT=0;
	vm_nombre_mem.Checksum=1;
	
	/*initialise la memoire partagee Nombre*/
	if (XZLG999_Cree_Memoire_Partagee_Nombre() != XDC_OK) {
		XZST_03EcritureTrace( XZSTC_FATAL,"agen_evt_init: pb appel a XZLG999_Cree_Memoire_Partagee_Nombre");
		return (XDC_NOK);
	}

	if (XZLG988_Ecrit_Nombre(vm_nombre_mem) != XDC_OK) {
		XZST_03EcritureTrace( XZSTC_FATAL,"agen_evt_init: pb appel a XZLG988_Ecrit_Nombre");
		return (XDC_NOK);
	}

	/*cree le semaphore pour la memoire partagee*/
	if (XZLG501_SemCree("/produits/migrazur/appliSD/exec/tagen.x",XZLGC_SEM_FMC) != XDC_OK) {
		XZST_03EcritureTrace( XZSTC_FATAL,"agen_tdp_init: pb appel a XZLG501_SemCree");
		return (XDC_NOK);
	}

  /*initialise la memoire partagee FMC*/
  if (XZLG997_Cree_Memoire_Partagee_FMC() != XDC_OK) {
		XZST_03EcritureTrace( XZSTC_FATAL,"agen_evt_cb_validation_fmc: pb appel a XZLG997_Cree_Memoire_Partagee_TDP");
		return ;
  }

	/* Ecriture Trace */
	XZST_03EcritureTrace( XZSTC_INFO, "OUT : agen_evt_init  ");
	return XDC_OK;

}

int creer_liste_commandes(XZAOT_Commandes_Scenarios va_commande) {
char vl_str[20];

  sprintf(vl_str,"%d/%d%/d-", va_commande.autoroute,
			va_commande.PR,
			va_commande.sens);
  strcat(vm_commandes,vl_str);
  return (XDC_OK);
}

/* Fichier : @(#)agen_evt.c	1.26        Release : 1.26        Date : 11/05/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE agen_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* libere les ressources
*
******************************************************
* SEQUENCE D'APPEL :
* agen_free_ressources();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
extern int agen_free_ressources()
{
  XZLGT_Liste_FMC	*pl_ex_fmc;

  XZST_03EcritureTrace(XZSTC_FONCTION,"agen_free_ressources: IN");

  while (tg_fmc!=NULL) {
    pl_ex_fmc=tg_fmc;
    /*tg_fmc=tg_fmc->Suivant;*/
    free(pl_ex_fmc);
  }
 
  XZST_03EcritureTrace(XZSTC_FONCTION,"agen_free_ressources: OUT");
  return (XDC_OK);
}


/* Fichier : @(#)agen_evt.c	1.26        Release : 1.26        Date : 11/05/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* purge
******************************************************
* SEQUENCE D'APPEL :
* agen_evt_purge
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

void agen_evt_purge()
{
  return ;
}

/* Fichier : @(#)agen_evt.c	1.26        Release : 1.26        Date : 11/05/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* reveil pour verifier qu'il y a des fmc a exporter
*****************************************************
* SEQUENCE D'APPEL :
* agen_evt_cb_validation_fmc
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
void agen_evt_cb_validation_fmc()
{
  XZLGT_Liste_FMC	*pl_liste_fmc;
  XZLGT_Liste_FMC	*pl_ex_fmc;
  XDY_Entier	vl_val;
  int 		vl_JourSemaine;
  double 	vl_HorodateSec; 
  char tl_str[100];


  XZST_03EcritureTrace(XZSTC_FONCTION,"agen_evt_cb_validation_fmc: debut de fonction !");

  /*recup de la liste des fmc modifiees*/
  pl_liste_fmc=NULL;
  if (XZLG01_GetFMC(TRUE,TRUE,TRUE,TRUE,TRUE,FALSE) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"agen_evt_cb_validation_fmc: pb dans l'appel a XZLG10_GetDernieresFmc");
    return;
  }

  /* pour chaque fmc de la liste */
  pl_liste_fmc = XZLG99_GetListe(&vm_nb_fmc);

  vm_nombre_mem.Nombre_FMC=vm_nb_fmc;

  /*mise en place du semaphore*/
  XZLG500_SemCall(-1,XZLGC_SEM_FMC);

  /*on fait +1 sur le checksum pour montrer que tagen vit toujours*/
  if (XZLG983_Update_Checksum() != XDC_OK) 
    XZST_03EcritureTrace( XZSTC_WARNING,"agen_evt_cb_validation_fmc: pb appel a XZLG983_Update_Checksum");
  vm_num_prec_regul=0;
  vm_cle_prec_regul=0;
  vm_indice_regul=0;

  XZST_03EcritureTrace( XZSTC_DEBUG1,"JMG FMC %d",vm_nb_fmc);
  /*for (vl_val=0;vl_val<XZLGC_NB_MAX_FMC;vl_val++) {*/
  for (vl_val=0;vl_val<vm_nb_fmc;vl_val++) {
    XZST_03EcritureTrace( XZSTC_DEBUG1,"JMG %d(%d)",pl_liste_fmc[vl_val].Evt.numero,vl_val);
    /*XZST_03EcritureTrace( XZSTC_DEBUG1,"JMG2 %d(%d)",pl_liste_fmc[vl_val+1].Evt.numero,vl_val+1);*/
    if (pl_liste_fmc[vl_val].Evt.numero==0)
      break;
 
    	/* genere le fichier strada correspondant*/
    	agen_evt_genere_strada(vl_val);

    /*
    XZLG10_Memorise_Diffusion_FMC(pl_liste_fmc[vl_val].Evt.numero,
				pl_liste_fmc[vl_val].Evt.cle,
				XZLGC_DIFF_CLIENT_2);*/
  }

  XZST_03EcritureTrace( XZSTC_DEBUG1,"fin de agen_evt_genere_strada JMG");

  if (XZLG984_Ecrit_Memoire_Partagee_FMC() != XDC_OK) {
	XZST_03EcritureTrace( XZSTC_WARNING,"agen_evt_cb_validation_fmc: pb appel a XZLG984_Ecrit_Memoire_Partagee_FMC");
        XZLG500_SemCall(1,XZLGC_SEM_FMC);
	return;
  }

  /*liberation du semaphore*/
  XZLG500_SemCall(1,XZLGC_SEM_FMC);

	/*A Lecture horodate courante systeme */
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
  	vl_HorodateSec+=C_FREQ_VERIF_EVT;
	/* armement du timer de programmation */
	if (XZSM_01DemanderProgrammation(       vl_HorodateSec,
						0,
						XDG_AGEN_TIMER,
						XDM_AS_SRVEVT_STRADA_FMC,
						"cle_timer_datexII",
						XZSMC_SEC_VRAI) != XDC_OK)
	{
	  XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de verif des evts non declenchee." );
	  return ;
  	}

  /*sprintf( tl_str,"find %s -mtime +%d -exec rm {} \\;", C_PATH, CO_NB_JOUR_PURGE);
  system(tl_str);*/

  return ;
}



/* Fichier : @(#)agen_evt.c	1.26        Release : 1.26        Date : 11/05/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE agen_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* construit les commentaires
*
******************************************************
* SEQUENCE D'APPEL :
* agen_evt_genere_commentaires();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
int agen_evt_genere_commentaires(char *va_texte)
{
int vl_i=0,vl_j=0;
int vl_taille=0;
int l=0, i=0;

  /*XZST_03EcritureTrace(XZSTC_INFO,"ses_evt_genere_commentaires: IN texte:%s",va_texte);*/
 
	 l = strlen(va_texte); 
	 for (i=0; i < l; i++)
		switch(va_texte[i]) {
                case -64 :      va_texte[i] = 'A'; break;
		case -56 :      va_texte[i] = 'A'; break;
		case -55 :      va_texte[i] = 'C'; break;
		case -23 :      va_texte[i] = 'E'; break;
		case -59 :      va_texte[i] = 'E'; break;
		case -63 :      va_texte[i] = 'E'; break;
		case -54 :      va_texte[i] = 'E'; break;
		case -52 :      va_texte[i] = 'E'; break;
		case -51 :      va_texte[i] = 'E'; break;
		case -47 :      va_texte[i] = 'I'; break;
		case -42 :      va_texte[i] = 'O'; break;
		case -44 :      va_texte[i] = 'O'; break;
		case -62 :      va_texte[i] = 'O'; break;
		case -53 :      va_texte[i] = 'U'; break;
		case -61 :      va_texte[i] = 'U'; break;
		case -80 :      va_texte[i] = ' '; break;
		case -78 :      va_texte[i] = ' '; break;
		case 34 :      va_texte[i] = ' '; break; /* " */
		case 39 :      va_texte[i] = ' '; break; /* ' */
#ifdef _HPUX_SOURCE
		case 178 :      va_texte[i] = ' '; break;
		case 180 :      va_texte[i] = ' '; break;
#endif
		default :       va_texte[i] = va_texte[i]; /*toupper(va_texte[i]);*/
		}


  /*A On enleve les blancs en fin */
  vl_i= strlen(va_texte)-1;
  while (vl_i > 0 && va_texte[vl_i]==' ' ) {
	 vl_i--;
  }
  va_texte[vl_i+1]='\0';
  for (vl_j=0; vl_j <=vl_i;vl_j++)
 	 if ((va_texte[vl_j]=='\n') || (va_texte[vl_j]=='|'))
 	 	va_texte[vl_j]=' ';
  
  /*XZST_03EcritureTrace(XZSTC_INFO,"texte:%s",va_texte);*/
  vl_i= strlen(va_texte);
  vl_taille=strlen(pm_comment);
  if (vl_i+vl_taille > TAILLE_SUR)
  	strncat(pm_comment,va_texte,TAILLE_SUR-vl_taille);
  else
  	strcat(pm_comment,va_texte);
  	
  /*XZST_03EcritureTrace(XZSTC_INFO,"comment:%s",pm_comment);*/
  XZST_03EcritureTrace(XZSTC_FONCTION,"ses_evt_genere_commentaires: OUT");
  return (XDC_OK);
}



/* Fichier : @(#)agen_evt.c	1.26        Release : 1.26        Date : 11/05/07
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* generation du fichier strada & px-metacor d'une fmc
*
*****************************************************
* SEQUENCE D'APPEL :
* agen_evt_genere_strada
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
int agen_evt_genere_strada(int va_indice)
{
  XDY_Horodate		vl_debut,vl_fin,vl_validation,vl_debut_prevu,vl_fin_prevu;
  XDY_Entier		vl_duree;
  XDY_Nom		vl_fausse_alerte,vl_fin_flag;
  XDY_Entier		vl_num_cause;
  XDY_Entier		vl_num_cause2;
  XDY_Octet		vl_cle_cause;
  XDY_Octet		vl_cle_cause2;
  char			vl_type_cause[26];
  XDY_Octet		vl_veracite,vl_trafic;
  XDY_Nom		vl_prevision;
  XZSMT_Horodate	vl_horo_chaine;
  int                   vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
  int			vl_annee1,vl_annee2;
  XDY_Booleen		vl_veh_esc,vl_hom_esc,vl_perso;
  XDY_Mot		vl_plan;
  XDY_Mot		vl_taille;
  char			tl_string_comment[400];
  char			tl_string[400],tl_buffer[15],tl_loc_float[15];
  int			vl_i;
  int			vl_visibilite, vl_type;
  XDY_Mot		vl_nombre;
  XDY_Booleen		vl_perception,vl_violence;
  int			vl_bebes,vl_agees,vl_animaux,vl_escota;
  char			tl_matiere[100],tl_observations[100],tl_nom[100];
  XDY_Octet		vl_octet1, vl_octet2, vl_octet3, vl_octet4, vl_octet5, vl_octet6;
  XDY_Octet		vl_octet7,vl_octet8, vl_octet9, vl_octet10, vl_octet11;
  XDY_Octet		vl_nb_entrees,vl_nb_sorties;
  XDY_Booleen		vl_bool1,vl_bool2,vl_bool3,vl_bool4;
  XDY_Booleen		vl_degats;
  XDY_Entier		vl_entier1,vl_entier2,vl_entier3,vl_entier4,vl_entier5, vl_entier6;
  XDY_Mot		vl_mot1,vl_mot2,vl_mot3,vl_mot4;
  char			vl_localisation[10];
  char                  vl_nom_autoroute[10];
  char			vl_str1[100],vl_str2[100],vl_str3[100];
  loc_rds		vl_loc;
  XDY_Entier		vl_pr,vl_pr_inf,vl_pr_sup;
  XDY_Octet		vl_sens,vl_sens_sc;
  XDY_Octet		vl_autoroute;
  XDY_Horodate	vl_Horodate;
  int vl_Jour_De_La_Semaine;
  XDY_Octet		vl_nbvoiesop;
  XDY_Octet		vl_gravite;
  char                  vl_reference[36]; /* Ajout SAGA */
  XDY_PR		vl_PR;
  XDY_PR		vl_pr1,vl_pr2,vl_pr3,vl_pr4,vl_pr5;
  XDY_PR		vl_pr6,vl_pr7,vl_pr8,vl_pr9,vl_pr10;
  XDY_PR		vl_pr11,vl_pr12,vl_pr13,vl_pr14,vl_pr15;
  XDY_PR		vl_pr16,vl_pr17,vl_pr18,vl_pr19,vl_pr20;
  XDY_Octet		vl_nb1,vl_nb2;
  XDY_Entier		vl_longueur;
  XZLGT_Liste_FMC *pa_fmc;
  int va_Numero;
  int va_Cle;
  int va_Type;
  int vl_pr_entier;
  int vl_pr_m;
  XDY_Entier vl_vitesse;
  XDY_Entier vl_vitesse_opposee;
  
//TODO
  XDY_Entier vl_contexte;
  XDY_Entier vl_scenario;
  XDY_Entier vl_mesure;
  XDY_Entier vl_indicateur_horaire;
  char			vl_transitoire[200];
  
  pa_fmc = XZLG99_GetListe(&vl_longueur);

  va_Numero=pa_fmc[va_indice].Evt.numero;
  va_Cle=pa_fmc[va_indice].Evt.cle;
  va_Type=pa_fmc[va_indice].Type;

  XZST_03EcritureTrace(XZSTC_INTERFACE,"agen_evt_genere_strada: IN %d %d (%d)",
		va_Numero,
		va_Cle,
		va_Type);

  /*recherche en base les infos communes a l'evt qui doivent etre exportees*/
  /*
  horodate debut
  horodate fin
  fausse alerte
  duree
  fin evenement
  horodate validation
  numero cause
  cle cause
  type cause
  veracite
  localisation
  prevision
  */
  strcpy(vl_fin_flag,"");
  if (XZAE127_Infos_obligatoires(va_Numero,
				va_Cle,								
				&vl_debut,
				&vl_fin,
				vl_fausse_alerte,
				&vl_duree,
				vl_fin_flag,
				&vl_validation,
				&vl_num_cause,
				&vl_cle_cause,
				vl_type_cause,
				&vl_octet1,
				vl_prevision,
				vl_localisation,
				&vl_entier1,
				&vl_sens,
				&vl_pr,
				&vl_autoroute,
				&vl_trafic,
				&vl_gravite,
				&vl_nbvoiesop,
				vl_reference /*Ajout SAGA*/				
				) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"agen_evt_genere_strada: pb appel a XZAE127_Infos_obligatoires %d %d",
				va_Numero,
				va_Cle);
    return (XDC_NOK);
  }
    XZST_03EcritureTrace(XZSTC_DEBUG1,"agen_evt_genere_strada: %d a pour cause %d",
				va_Numero,vl_num_cause);
  pa_fmc[va_indice].VNM = vl_entier1;
  pa_fmc[va_indice].Bloc_Optio.Veracite=vl_octet1;
  pa_fmc[va_indice].Sens = vl_sens;
  pa_fmc[va_indice].Evt_Cause.numero=vl_num_cause;
  pa_fmc[va_indice].Evt_Cause.cle=vl_cle_cause;
  pa_fmc[va_indice].Autoroute=vl_autoroute;

  if (va_Type == XZAEC_FMC_Regulation) {
    vl_pr = pa_fmc[va_indice].PR_Debut;
	pa_fmc[va_indice].Bloc_Optio.Bloc.Regulation.Vitesse =  pa_fmc[va_indice].Longueur;
    if ((va_Numero == vm_num_prec_regul) && (va_Cle== vm_cle_prec_regul)) {
	/*on traite une fmc regulation "virtuelle" */
	/*on va la rattacher a la fmc regulation "vraie" (par la cause) */
	/*et lui coller une cle unique*/
	pa_fmc[va_indice].Evt_Cause.numero=vm_num_prec_regul;
	pa_fmc[va_indice].Evt_Cause.cle=vm_cle_prec_regul;
	vl_pr = pa_fmc[va_indice].PR_Debut;
        pa_fmc[va_indice].Evt.cle = 10*va_Cle+vm_indice_regul;
	vm_indice_regul++;
    }
    else  {
 	vm_num_prec_regul = va_Numero;
	vm_cle_prec_regul = va_Cle;
	vm_indice_regul=0;
    }
  }

  /*lecture de la config des voies*/
  if (XZLG60_Lire_Config_Voies(va_Numero,
			va_Cle,
			&vl_octet1,
			&vl_octet2,
			&vl_octet3,
			&vl_octet4,
			&vl_octet5,
			&vl_octet6,
			&vl_octet7,
			&vl_octet8,
			&vl_octet9,
			&vl_octet10,
			&vl_octet11,
			&vl_entier1,
			&vl_nb_entrees,
			&vl_nb_sorties)!= XDC_OK)
    XZST_03EcritureTrace(XZSTC_WARNING,"agen_evt_genere_strada: pb appel a XZLG60_Lire_Config_Voies %d %d",
			va_Numero,
			va_Cle);

  pa_fmc[va_indice].VR=vl_octet1;
  pa_fmc[va_indice].VM2=vl_octet2;
  pa_fmc[va_indice].VM1=vl_octet3;
  pa_fmc[va_indice].VL=vl_octet4;
  pa_fmc[va_indice].BAU=vl_octet5;
  pa_fmc[va_indice].VR_I=vl_octet6;
  pa_fmc[va_indice].VM2_I=vl_octet7;
  pa_fmc[va_indice].VM1_I=vl_octet8;
  pa_fmc[va_indice].VL_I=vl_octet9;
  pa_fmc[va_indice].BAU_I=vl_octet10;

  pa_fmc[va_indice].Type_Point_Car = vl_octet11;
  pa_fmc[va_indice].Numero_Point_Car = vl_entier1;

  switch (vl_sens)
  {
  case XDC_SENS_ENTREE_SUD :
  case XDC_SENS_SORTIE_SUD :
  case XDC_SENS_SUD : 	vl_sens_sc=XDC_SENS_SUD;
  	   		break;
  case XDC_SENS_ENTREE_NORD :
  case XDC_SENS_SORTIE_NORD :
  case XDC_SENS_NORD : 	vl_sens_sc=XDC_SENS_NORD;
  	   		break;
  default :  	vl_sens_sc=vl_sens;
  	   	break;
  }

  if (XZAE146_Recherche_Loc_Rds(
  				vl_pr,
  				vl_autoroute,
  				vl_loc.l01,
  				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,
				vl_nom_autoroute) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"agen_evt_genere_strada: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_Cle);
    return (XDC_NOK);
  }
  vl_i= strlen(vl_nom_autoroute)-1;
  while (vl_i > 0 && vl_nom_autoroute[vl_i]==' ' ) {
                vl_i--;
  }
  vl_nom_autoroute[vl_i+1]='\0';
  strcpy(vl_loc.rnp,vl_nom_autoroute);

  if ((vl_sens == XDC_SENS_SUD) || 
  	(vl_sens == XDC_SENS_ENTREE_SUD) || 
  	(vl_sens == XDC_SENS_SORTIE_SUD))
  	{
  	strcpy(vl_loc.l02,C_l02);
  	vl_i= strlen(vl_str1)-1;
 	 while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
 	 	vl_i--;
  	}
  	vl_str1[vl_i+1]='\0';
	vl_str1[49]='\0';
  	strncpy(vl_loc.l10,vl_str1,50);
  	/*vl_loc.l10[vl_i+1]='\0';*/
  
  	vl_i= strlen(vl_str2)-1;
  	while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  		vl_i--;
  	}
  	vl_str2[vl_i+1]='\0';
 	vl_str2[49]='\0';
	strncpy(vl_loc.l06,vl_str2,50);
  	/*vl_loc.l06[vl_i+1]='\0';*/
  
 
  	vl_loc.l09=vl_entier3;
  	vl_loc.l04=vl_entier4;
  	vl_loc.l03 = C_l03;
  
  	strcpy(vl_loc.l05,"positive");
	strcpy(vl_loc.drp,"positive");
  	strcpy(vl_loc.l07,tm_l07_p[vl_autoroute]);
  	
  	vl_loc.l12 = vl_pr - vl_entier1;
  	vl_loc.l11 = vl_entier2 - vl_pr;
	if (vl_loc.l11<0) vl_loc.l11=-vl_loc.l11;
  }
  else if ((vl_sens == XDC_SENS_NORD) || 
  	(vl_sens == XDC_SENS_ENTREE_NORD) || 
  	(vl_sens == XDC_SENS_SORTIE_NORD))
   {
  	strcpy(vl_loc.l02,C_l02);
  	vl_i= strlen(vl_str1)-1;
 	 while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
 	 	vl_i--;
  	}
  	vl_str1[vl_i+1]='\0';
	vl_str1[49]='\0';
  	strncpy(vl_loc.l06,vl_str1,50);
  	/*vl_loc.l06[vl_i+1]='\0';*/
  
  	vl_i= strlen(vl_str2)-1;
  	while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  		vl_i--;
  	}
  	vl_str2[vl_i+1]='\0';
	vl_str2[49]='\0';
 	strncpy(vl_loc.l10,vl_str2,50);
  	/*vl_loc.l10[vl_i+1]='\0';*/
  
 
  	vl_loc.l04=vl_entier3;
  	vl_loc.l09=vl_entier4;
  	vl_loc.l03 = C_l03;
  
  	strcpy(vl_loc.l05,"negative");
	strcpy(vl_loc.drp,"negative");
  	strcpy(vl_loc.l07,tm_l07_n[vl_autoroute]);
  	
  	vl_loc.l11 = vl_pr - vl_entier1;
  	vl_loc.l12 = vl_entier2 - vl_pr;
	if (vl_loc.l12<0) vl_loc.l12 = -vl_loc.l12;
  }
  
  else {
  	strcpy(vl_loc.l02,C_l02);
  	vl_i= strlen(vl_str1)-1;
 	 while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
 	 	vl_i--;
  	}
  	vl_str1[vl_i+1]='\0';
  	strcpy(vl_loc.l10,vl_str1);
  	vl_loc.l10[vl_i+1]='\0';
  
  	vl_i= strlen(vl_str2)-1;
  	while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  		vl_i--;
  	}
  	vl_str2[vl_i+1]='\0';
 	vl_str2[49]='\0';
	strncpy(vl_loc.l06,vl_str2,50);
  	/*vl_loc.l06[vl_i+1]='\0';*/
  
 
  	vl_loc.l09=vl_entier3;
  	vl_loc.l04=vl_entier4;
  	vl_loc.l03 = C_l03;
  
  	strcpy(vl_loc.l05,"positive");
	strcpy(vl_loc.drp,"unknown");
  	strcpy(vl_loc.l07,tm_l07_p[vl_autoroute]);
  	
  	vl_loc.l12 = vl_pr - vl_entier1;
  	vl_loc.l11 = vl_entier2 - vl_pr;
	if (vl_loc.l12<0) vl_loc.l12=-vl_loc.l12;
	if (vl_loc.l11<0) vl_loc.l11=-vl_loc.l11;

  	strcpy(vl_loc.l05,"unknown");
  	strcpy(vl_loc.l07,C_l07_U);
	}
  
  vl_pr_entier=floor(vl_pr/1000)*1000;
  sprintf(vl_loc.rpip,"%d",vl_pr_entier);
  vl_pr_m= vl_pr - vl_pr_entier;
  sprintf(vl_loc.dfpp,"%d",vl_pr_m);

  strcpy(vl_loc.dfps,vl_loc.dfpp);
  strcpy(vl_loc.rpis,vl_loc.rpip);
  strcpy(vl_loc.drs,vl_loc.drp);
  strcpy(vl_loc.rns,vl_loc.rnp);
 

  vl_loc.l08=0;
  
  sprintf(pa_fmc[va_indice].RDS.l01,"%s",vl_loc.l01);
  sprintf(pa_fmc[va_indice].RDS.l02,"%s",vl_loc.l02);
  sprintf(pa_fmc[va_indice].RDS.l03,"%d",vl_loc.l03);
  sprintf(pa_fmc[va_indice].RDS.l04,"%d",vl_loc.l04);
  sprintf(pa_fmc[va_indice].RDS.l05,"%s",vl_loc.l05);
  sprintf(pa_fmc[va_indice].RDS.l06,"%s",vl_loc.l06);
  sprintf(pa_fmc[va_indice].RDS.l07,"%s",vl_loc.l07);
  sprintf(pa_fmc[va_indice].RDS.l08,"%d",vl_loc.l08);
  sprintf(pa_fmc[va_indice].RDS.l09,"%d",vl_loc.l09);
  sprintf(pa_fmc[va_indice].RDS.l10,"%s",vl_loc.l10);
  
  sprintf(pa_fmc[va_indice].RDS.l11,"%d",vl_loc.l11);
  
  sprintf(pa_fmc[va_indice].RDS.l12,"%d",vl_loc.l12);

  strcpy(pa_fmc[va_indice].RDS.rnp,vl_loc.rnp);
  strcpy(pa_fmc[va_indice].RDS.rpip,vl_loc.rpip);
  strcpy(pa_fmc[va_indice].RDS.dfpp,vl_loc.dfpp);
  strcpy(pa_fmc[va_indice].RDS.drp,vl_loc.drp);
  strcpy(pa_fmc[va_indice].RDS.rns,vl_loc.rns);
  strcpy(pa_fmc[va_indice].RDS.rpis,vl_loc.rpis);
  strcpy(pa_fmc[va_indice].RDS.dfps,vl_loc.dfps);
  strcpy(pa_fmc[va_indice].RDS.drs,vl_loc.drs);



   
  pa_fmc[va_indice].Bloc_Optio.Fin=vl_fin_flag[0];
  pa_fmc[va_indice].Bloc_Optio.Fausse_Alerte=vl_fausse_alerte[0];
  
  sprintf(pa_fmc[va_indice].Nb_Voies_Ouvertes ,"%d", vl_nbvoiesop);

  /* Localisation sur Bretelle */

  if (((vl_sens == XDC_SENS_ENTREE_SUD) || (vl_sens == XDC_SENS_ENTREE_NORD) || (vl_sens == XDC_SENS_SORTIE_SUD) || (vl_sens == XDC_SENS_SORTIE_NORD)) && ((vl_loc.l11<100) || (vl_loc.l12<100)))
  {	/* si sur echang les 2 pt sont un seul */
	if (vl_loc.l11<vl_loc.l12)
		{
  		sprintf(pa_fmc[va_indice].RDS.l12,"%d",0);
  		sprintf(pa_fmc[va_indice].RDS.l11,"%d",0);
		sprintf(pa_fmc[va_indice].RDS.l10,"%s",vl_loc.l06);				
  		sprintf(pa_fmc[va_indice].RDS.l09,"%d",vl_loc.l04);
		}
	else
		{
  		sprintf(pa_fmc[va_indice].RDS.l12,"%d",0);
  		sprintf(pa_fmc[va_indice].RDS.l11,"%d",0);
		sprintf(pa_fmc[va_indice].RDS.l06,"%s",vl_loc.l10);				
  		sprintf(pa_fmc[va_indice].RDS.l04,"%d",vl_loc.l09);
		}
		
  }

  XZSM_11ConversionHorodate(vl_debut,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
			&vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"agen_evt_genere_strada: format date incorrect");
    return (XDC_NOK);
  }
  /*sprintf(vl_bloc.Bloc_Optio.Date_Debut,"%04d%02d%02d%02d%02dP01",*/
  sprintf(pa_fmc[va_indice].Bloc_Optio.Date_Debut,"%04d-%02d-%02dT%02d:%02d:00",
			vl_annee,
			vl_mois,
			vl_jour,
			vl_heure,
			vl_minute);

  XZSM_11ConversionHorodate(vl_fin,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"agen_evt_genere_strada: format date incorrect");
    return (XDC_NOK);
  }
  if (vl_annee!=1970)
    /*sprintf(vl_bloc.Bloc_Optio.Date_Fin,"%04d%02d%02d%02d%02dP01",*/
  sprintf(pa_fmc[va_indice].Bloc_Optio.Date_Fin,"%04d-%02d-%02dT%02d:%02d:00",
                        vl_annee,
                        vl_mois,
                        vl_jour,
                        vl_heure,
                        vl_minute);
  else
    strcpy(pa_fmc[va_indice].Bloc_Optio.Date_Fin,"");

  XZSM_11ConversionHorodate(vl_validation,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute,
                        &vl_seconde ) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"agen_evt_genere_strada: format date incorrect");
    return (XDC_NOK);
  }

  XZAE158_Date_prevue(va_Numero,va_Cle,&vl_debut_prevu,&vl_fin_prevu);

  XZSM_11ConversionHorodate(vl_debut_prevu,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date debut prevu : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"agen_evt_genere_strada: format date debut prevu incorrect");
    return (XDC_NOK);
  }
  sprintf(pa_fmc[va_indice].Bloc_Optio.Date_Debut_Prevu,"%04d-%02d-%02dT%02d:%02d:00",
                        vl_annee,
                        vl_mois,
                        vl_jour,
                        vl_heure,
                        vl_minute);

  XZSM_11ConversionHorodate(vl_fin_prevu,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date fin prevu : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"agen_evt_genere_strada: format date fin prevu incorrect");
    return (XDC_NOK);
  }
  sprintf(pa_fmc[va_indice].Bloc_Optio.Date_Fin_Prevu,"%04d-%02d-%02dT%02d:%02d:00",
                        vl_annee,
                        vl_mois,
                        vl_jour,
                        vl_heure,
                        vl_minute);

  if (vl_prevision[0]=='Y')
    pa_fmc[va_indice].Prevision=TRUE;
  else
    pa_fmc[va_indice].Prevision=FALSE;

  sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"");		    	
  /*suivant le type d'evt, recherche les infos specifiques*/
  switch(va_Type) {
    case XZAEC_FMC_FrontiereFermee:
	strcpy(pa_fmc[va_indice].Bloc_Optio.Comment,"Frontiere fermee#\n");
	break;

    case XZAEC_FMC_Accident: 
    	vl_veh_esc = -1;
    	vl_hom_esc = -1;
    	vl_perso   = -1;
    	
	if (XZLG41_GetFmcAccident(va_Numero
						    ,va_Cle
						    ,&vl_octet1
						    ,&vl_octet5
						    ,&vl_octet2
						    ,&vl_octet6
						    ,&vl_octet7
						    ,&vl_octet3
						    ,&vl_octet4
						    ,&vl_octet8
						    ,&vl_bool1
						    ,&vl_veh_esc
						    ,&vl_hom_esc
						    ,&vl_perso
						    ,&vl_degats) ==XDC_OK) {

		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.VIP=vl_perso;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.ESCOTA=vl_hom_esc;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Vehicules=vl_octet1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.PL=vl_octet5;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Cars=vl_octet6;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Motos=vl_octet7;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Rem=vl_octet2;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Blesses=vl_octet3+vl_octet4;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Blesses_Legers=vl_octet3;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Blesses_Graves=vl_octet4;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Morts=vl_octet8;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Matiere=vl_bool1;
		XZST_03EcritureTrace(XZSTC_INFO,"XZLG41_GetFmcAccident :%d %d %d %d %d %d",
					pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Vehicules,
					pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.PL,
					pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Cars,
					pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Rem,
					pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Blesses,
					pa_fmc[va_indice].Bloc_Optio.Bloc.Accident.Morts);

		tl_string_comment[0]='\0';
		
		if (vl_veh_esc == 1) 
			strcat (tl_string_comment,"vehicule(s) ESCOTA implique(s), ");
		
		if (vl_hom_esc == 1) 
			strcat (tl_string_comment,"personne(s) ESCOTA impliquee(s), ");
		
		if (vl_perso == 1) 
			strcat (tl_string_comment,"personalites impliquee(s)");
		
		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s#\n",tl_string_comment);
		
		break;
	}
	break;

    case XZAEC_FMC_Delestage :
    case XZAEC_FMC_Deviation :
	if (XZLG42_GetFmcDelestageDeviation(va_Numero
						    ,va_Cle
						    ,tl_string_comment
						    ,&vl_entier1
						    ,vl_str1
						    ,&vl_entier2
						    ,&vl_entier3) ==XDC_OK) {
		XZST_03EcritureTrace(XZSTC_INFO,"agen_evt_genere_strada: DEL %s,%d,%s,%d",
						tl_string_comment,vl_entier1,vl_str1,vl_entier2);
		if (vl_entier3==pa_fmc[va_indice].PR_Debut)
		  pa_fmc[va_indice].PR_Fin = vl_entier2;
		else
		  pa_fmc[va_indice].PR_Fin = vl_entier3;
		XZLG300_trim(tl_string_comment);
		strcpy(pa_fmc[va_indice].Bloc_Optio.Bloc.Delestage.Itineraire,tl_string_comment);
		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"plan numero %d , %s#\n",
					vl_entier1,tl_string_comment);  
		}
	break;

    case XZAEC_FMC_FlaqueHuile :
	     pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Type = XZAEC_CH_GLIS_Huile;
             break;
    case XZAEC_FMC_Regulation:
	strcpy(vm_commandes,"");
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_Jour_De_La_Semaine, &vl_Horodate);
	if (XZAE565_Lire_FMC_Regulation(va_Numero,va_Cle,(XDY_Horodate)vl_Horodate,
					&vl_entier1,
					&vl_entier2,
					&vl_entier3,
					&vl_entier4,
					&vl_entier5,
					&vl_entier6,
					vl_str1) == XDC_OK ) {
		pa_fmc[va_indice].Bloc_Optio.Bloc.Regulation.Scenario = vl_entier2;
		/*JMG 	
		pa_fmc[va_indice].Bloc_Optio.Bloc.Regulation.Contexte=vl_entier1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Regulation.Mesure=vl_entier3;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Regulation.Indicateur_horaire=vl_entier4;
		strcpy(pa_fmc[va_indice].Bloc_Optio.Bloc.Regulation.Transitoire,vl_str1);
		*/

		vl_pr = pa_fmc[va_indice].PR_Fin;
  		if (XZAE146_Recherche_Loc_Rds(pa_fmc[va_indice].PR_Fin,
  				vl_autoroute,
  				vl_loc.l01,
  				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,
				vl_nom_autoroute ) != XDC_OK) {
    			XZST_03EcritureTrace(XZSTC_WARNING,"agen_evt_genere_strada: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_Cle);
    			return (XDC_NOK);
  			}

                strcpy(vl_loc.rnp,vl_nom_autoroute);
                vl_pr_entier=floor(vl_pr/1000)*1000;
                sprintf(vl_loc.rpip,"%d",vl_pr_entier);
                vl_pr_m= vl_pr - vl_pr_entier;
                sprintf(vl_loc.dfpp,"%d",vl_pr_m);
                strcpy(pa_fmc[va_indice].RDS.rnp,vl_loc.rnp);
                strcpy(pa_fmc[va_indice].RDS.rpip,vl_loc.rpip);
                strcpy(pa_fmc[va_indice].RDS.dfpp,vl_loc.dfpp);
                strcpy(pa_fmc[va_indice].RDS.drp,vl_loc.drp);

  		if ((vl_sens == XDC_SENS_SUD) || 
  			(vl_sens == XDC_SENS_ENTREE_SUD) || 
  			(vl_sens == XDC_SENS_SORTIE_SUD))
  		{
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  				vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  		else if ((vl_sens == XDC_SENS_NORD) || 
  			(vl_sens == XDC_SENS_ENTREE_NORD) || 
  			(vl_sens == XDC_SENS_SORTIE_NORD))
   		{
  			vl_i= strlen(vl_str1)-1;
 	 		while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
 	 			vl_i--;
  			}
  			vl_str1[vl_i+1]='\0';
  			strcpy(vl_loc.l06,vl_str1);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier3;
  			vl_loc.l11 = vl_pr - vl_entier1;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  
  		else {
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  					vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;

  			strcpy(vl_loc.l05,"unknown");
  			strcpy(vl_loc.l07,C_l07_U);

                        strcpy(vl_loc.l05,"both");
                        strcpy(pa_fmc[va_indice].RDS.l05,vl_loc.l05);
                        strcpy(vl_loc.l07,"Deux sens");
                        strcpy(vl_loc.drp,"both");
                        strcpy(pa_fmc[va_indice].RDS.drs,vl_loc.drp);
                        strcpy(pa_fmc[va_indice].RDS.drp,vl_loc.drp);
  		}
  
     
  		sprintf(pa_fmc[va_indice].RDS.l04,"%d",vl_loc.l04);
  		sprintf(pa_fmc[va_indice].RDS.l06,"%s",vl_loc.l06);
	}
	break;

    case XZAEC_FMC_Meteo:
    case XZAEC_FMC_TraitChaussees:
    case XZAEC_FMC_ChausseeGliss:
    	pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Visibilite = -1;
    	tl_string[0]='\0';
    	tl_string_comment[0]='\0';

	if (XZLG44_GetFmcMeteo(va_Numero
						    ,va_Cle
						    ,&vl_pr
						    ,&vl_octet2
						    ,&vl_mot1
						    ,&vl_entier1
						    ,&vl_octet1
						    ,vl_str1) == XDC_OK) {
		pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Longueur=vl_entier1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Visibilite=vl_mot1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Type=vl_octet2;
		strcpy(pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Nature,vl_str1);
    		XZST_03EcritureTrace(XZSTC_INFO,"agen_evt_genere_strada: VISI = %d, TYPE = %d",
				pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Visibilite,
				pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Type);
		pa_fmc[va_indice].PR_Fin=vl_pr;
		switch (pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Type) {
			case XZAEC_MET_ChuteNeige :
				strcat (tl_string_comment,"Chute de Neige");
				break;
		
			case XZAEC_MET_NeigeChaus :
				strcat (tl_string_comment,"Neige sur Chaussee");
				break;
				
			case XZAEC_MET_GreleChaus :
				strcat (tl_string_comment,"Grele sur Chaussee");
				break;
			
			case XZAEC_MET_Pluie :
				strcat (tl_string_comment,"Pluie");
				break;
				
			case XZAEC_MET_VentFort :
				strcat (tl_string_comment,"Vent fort");
				break;
				
			case XZAEC_MET_Brouillard :
				strcat (tl_string_comment,"Brouillard");
				break;
				
			case XZAEC_MET_BrouillardGiv :
				strcat (tl_string_comment,"Brouillard Givrant");
				break;
				
			case XZAEC_MET_Grele :
				strcat (tl_string_comment,"Grele");
				break;
				
			case XZAEC_MET_Verglas :
				strcat (tl_string_comment,"Verglas");
				break;
				
			case XZAEC_MET_Autre :
				strcat (tl_string_comment,"Perturbation Meteo");
				break;
				
			case XZAEC_TRT_CH_Salage:
				strcat (tl_string_comment,"Salage");
				break;

			case XZAEC_TRT_CH_Deneigement:
				strcat (tl_string_comment,"Deneigement");
				break;

			case XZAEC_CH_GLIS_Huile:
				strcat (tl_string_comment,"Huile");
				break;

			case XZAEC_CH_GLIS_Carburant:
				strcat (tl_string_comment,"Carburant");
				break;

			case XZAEC_CH_GLIS_Gasoil:
				strcat (tl_string_comment,"Gasoil");
				break;
			default :
				strcat (tl_string_comment,"Perturbation meteo");
				break;
		}
		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s#\n",tl_string_comment);
		strcpy(pa_fmc[va_indice].Bloc_Optio.Bloc.Meteo.Nature,tl_string_comment);
		/*vl_pr=vl_entier1;*/
  		if (XZAE146_Recherche_Loc_Rds(vl_pr,
  				vl_autoroute,
  				vl_loc.l01,
  				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,
				vl_nom_autoroute ) != XDC_OK) {
    			XZST_03EcritureTrace(XZSTC_WARNING,"agen_evt_genere_strada: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_Cle);
    			return (XDC_NOK);
  			}

                strcpy(vl_loc.rnp,vl_nom_autoroute);
                vl_pr_entier=floor(vl_pr/1000)*1000;
                sprintf(vl_loc.rpip,"%d",vl_pr_entier);
                vl_pr_m= vl_pr - vl_pr_entier;
                sprintf(vl_loc.dfpp,"%d",vl_pr_m);
                strcpy(pa_fmc[va_indice].RDS.rnp,vl_loc.rnp);
                strcpy(pa_fmc[va_indice].RDS.rpip,vl_loc.rpip);
                strcpy(pa_fmc[va_indice].RDS.dfpp,vl_loc.dfpp);
                strcpy(pa_fmc[va_indice].RDS.drp,vl_loc.drp);

  		if ((vl_sens == XDC_SENS_SUD) || 
  			(vl_sens == XDC_SENS_ENTREE_SUD) || 
  			(vl_sens == XDC_SENS_SORTIE_SUD))
  		{
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  				vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  		else if ((vl_sens == XDC_SENS_NORD) || 
  			(vl_sens == XDC_SENS_ENTREE_NORD) || 
  			(vl_sens == XDC_SENS_SORTIE_NORD))
   		{
  			vl_i= strlen(vl_str1)-1;
 	 		while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
 	 			vl_i--;
  			}
  			vl_str1[vl_i+1]='\0';
  			strcpy(vl_loc.l06,vl_str1);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier3;
  			vl_loc.l11 = vl_pr - vl_entier1;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  
  		else {
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  					vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;

  			strcpy(vl_loc.l05,"unknown");
  			strcpy(vl_loc.l07,C_l07_U);

                        strcpy(vl_loc.l05,"both");
                        strcpy(pa_fmc[va_indice].RDS.l05,vl_loc.l05);
                        strcpy(vl_loc.l07,"Deux sens");
                        strcpy(vl_loc.drp,"both");
                        strcpy(pa_fmc[va_indice].RDS.drs,vl_loc.drp);
                        strcpy(pa_fmc[va_indice].RDS.drp,vl_loc.drp);
  		}
  
     
  		sprintf(pa_fmc[va_indice].RDS.l04,"%d",vl_loc.l04);
  		sprintf(pa_fmc[va_indice].RDS.l06,"%s",vl_loc.l06);
  		sprintf(pa_fmc[va_indice].RDS.l11,"%d",vl_loc.l11);
		  
		break;
	}
	break;

    case XZAEC_FMC_VehMarcheArr:
    case XZAEC_FMC_Contresens:
    case XZAEC_FMC_VehArrete:
    case XZAEC_FMC_VehPanne:
    case XZAEC_FMC_VehFeu:
    
    		vl_bebes	= -1 ;
    		vl_agees	= -1 ;
    		vl_animaux	= -1 ;
    		vl_escota	= -1 ;
    		
	if ( XZLG45_GetFmcVehicule(va_Numero
						    	,va_Cle
						    	,&vl_octet1	/*type*/
							,&vl_octet2	/*nb passagers*/
							,&vl_bool3	/*bebes*/
							,&vl_bool1 	/*pers agees*/
							,&vl_bool2	/*animaux*/
							,&vl_bool4	/*veh escota*/
							,&vl_octet3	/*volume chargement*/
			   				,tl_matiere     /*code matiere*/
							,vl_str1	/*code danger*/
							,&vl_mot1	/*cde nature panne*/
							,vl_str2	/*nature panne*/
							,tl_buffer	/*positionnement*/	
							,&vl_octet4	/*roulant*/
							,&vl_octet5	/* pres mat dang*/
			   				,&vl_mot2
			   				,&vl_mot3) ==XDC_OK) {
		tl_string_comment[0]='\0';
		pa_fmc[va_indice].Bloc_Optio.Bloc.Vehicule.TypeVehicule=vl_octet1;
                tl_string[0]='\0';
                if (va_Type==XZAEC_FMC_VehMarcheArr) {
                        switch (pa_fmc[va_indice].Bloc_Optio.Bloc.Vehicule.TypeVehicule) {
                                case XZAEC_VEH_PL : sprintf(tl_string,"%s", "lorry"); break;
                                case XZAEC_VEH_VL : sprintf(tl_string,"%s",  "carOrLightVehicle"); break;
                                case XZAEC_VEH_Car : sprintf(tl_string,"%s","bus"); break;
                                case XZAEC_VEH_Moto : sprintf(tl_string,"%s",  "twoWheeledVehicle"); break;
                                case XZAEC_VEH_Remorque : sprintf(tl_string,"%s",  "articulatedVehicle"); break;
                        }
                        strcat (tl_string_comment,tl_string);
                }


		if ( tl_matiere[0] != '\0' && tl_matiere[0] != ' ' )
			sprintf(pa_fmc[va_indice].Bloc_Optio.Bloc.Vehicule.Matiere,"%s",tl_matiere);
		else
			 sprintf(pa_fmc[va_indice].Bloc_Optio.Bloc.Vehicule.Matiere,"");
		pa_fmc[va_indice].Bloc_Optio.Bloc.Vehicule.Volume=vl_octet3;
		tl_string[0]='\0';
		
		XZST_03EcritureTrace(XZSTC_INFO,"agen_evt_genere_strada: mot2=%d, mot3=%d, bool2=%d, bool1=%d",
					vl_mot2,vl_mot3,vl_bool2,vl_bool4);
		if (vl_mot2 > 0) {
			sprintf ( tl_string , "%d bebes implique(s)", vl_mot2);
			strcat (tl_string_comment,tl_string);
		}
		
		if (vl_mot3 > 0) {
			sprintf (tl_string, ", %d personne(s) agees impliquee(s)", vl_mot3);
			strcat (tl_string_comment,tl_string);
		}
		
		if ( vl_bool2== 1) 
			strcat (tl_string_comment,", presence d'animaux");
		
		if ( vl_bool4== 1) 
			strcat (tl_string_comment,", personne(s) d'Escota impliquee(s),");
		
		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s     ",tl_string_comment);

		break;	
	}					 
	break;


    case XZAEC_FMC_Cirque:
    case XZAEC_FMC_ConvoiExc:
    case XZAEC_FMC_ConvoiMil:
    case XZAEC_FMC_VehLent:
	/*if ( XZAE132_Lire_Fiche_VehLent_Pour_Strada(va_Numero*/
	if ( XZLG46_GetFmcConvoi(va_Numero
						    	,va_Cle
						    	,&vl_mot1
						    	,&vl_octet1
							,&vl_mot4
							,&vl_mot2
							,&vl_mot3
							,vl_str1) == XDC_OK) {
		if (va_Type==XZAEC_FMC_Cirque)
		  sprintf(pa_fmc[va_indice].Bloc_Optio.Comment,"Cirque en convoi - vitesse %dkm/h  ",vl_octet1);
		else
		  sprintf(pa_fmc[va_indice].Bloc_Optio.Comment,"Vitesse %d  ",vl_octet1);
		pa_fmc[va_indice].Bloc_Optio.Bloc.VehicLent.Longueur=vl_mot1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.VehicLent.Vitesse=vl_octet1;
                pa_fmc[va_indice].Bloc_Optio.Bloc.VehicLent.Hauteur=vl_mot4;
                pa_fmc[va_indice].Bloc_Optio.Bloc.VehicLent.Largeur=vl_mot2;
                pa_fmc[va_indice].Bloc_Optio.Bloc.VehicLent.Poids=vl_mot3;
		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s     ",vl_str1);
		break;
	}
	break;

    case XZAEC_FMC_Manifestation:
    case XZAEC_FMC_GreveESCOTA:
    case XZAEC_FMC_Escargot:
    
    		vl_nombre 	= -1;
    		vl_perception	= -1;
    		vl_violence	= -1;
    
	/*if ( XZAE134_Lire_Fiche_Manif_Pour_Strada(va_Numero*/
	if ( XZLG47_GetFmcManifestation(va_Numero
						    	,va_Cle
						    	,&vl_mot1
						    	,&vl_perception
						    	,&vl_violence) ==XDC_OK) {
						    	
		tl_string_comment[0]='\0';
		pa_fmc[va_indice].Bloc_Optio.Bloc.Manif.Nombre=vl_mot1;
		sprintf (tl_string, "Personne(s): %d ", vl_mot1);
		strcat (tl_string_comment,tl_string);
		
		if (vl_perception == XZAEC_FMC_PAS_PER) 
			strcat (tl_string_comment,", pas de perception");
		else 
			strcat (tl_string_comment,", perception maintenue");
		
		if (vl_violence == XDC_VRAI) 
			strcat (tl_string_comment,", violence");
		else
			strcat (tl_string_comment,", sans violence    ");
		
		sprintf(pa_fmc[va_indice].Bloc_Optio.Comment,"%s",tl_string_comment);
						    	
		break;
	}
	break;

    case XZAEC_FMC_Basculement:
	pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Type=-1;
	if ( XZLG48_GetFmcBasculement (va_Numero
					,va_Cle
					,&vl_entier2
					,&vl_entier1
					,&vl_octet1
					,&vl_octet2
					,&vl_octet3
					,&vl_octet4
					,&vl_octet5
					,&vl_octet6
					,&vl_octet7
					,&vl_octet8
					,&vl_octet9
					,&vl_octet10
					,&vl_vitesse
					,&vl_vitesse_opposee) == XDC_OK) {

		XZST_03EcritureTrace(XZSTC_DEBUG1,"vitesse %d, opp %d",
					vl_vitesse,vl_vitesse_opposee);
		pa_fmc[va_indice].Bloc_Optio.Bloc.Basculement.Longueur=vl_entier1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Basculement.Vitesse=vl_vitesse;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Basculement.VitesseOpposee=vl_vitesse_opposee;
		vl_pr=vl_entier2;

  		if (XZAE146_Recherche_Loc_Rds(vl_pr,
  				vl_autoroute,
  				vl_loc.l01,
  				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,
				vl_nom_autoroute ) != XDC_OK) {
    			XZST_03EcritureTrace(XZSTC_WARNING,"agen_evt_genere_strada: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_Cle);
    			return (XDC_NOK);
  			}

                strcpy(vl_loc.rnp,vl_nom_autoroute);
                vl_pr_entier=floor(vl_pr/1000)*1000;
                sprintf(vl_loc.rpip,"%d",vl_pr_entier);
                vl_pr_m= vl_pr - vl_pr_entier;
                sprintf(vl_loc.dfpp,"%d",vl_pr_m);
                strcpy(pa_fmc[va_indice].RDS.rnp,vl_loc.rnp);
                strcpy(pa_fmc[va_indice].RDS.rpip,vl_loc.rpip);
                strcpy(pa_fmc[va_indice].RDS.dfpp,vl_loc.dfpp);
                strcpy(pa_fmc[va_indice].RDS.drp,vl_loc.drp);

  		if ((vl_sens == XDC_SENS_SUD) || 
  			(vl_sens == XDC_SENS_ENTREE_SUD) || 
  			(vl_sens == XDC_SENS_SORTIE_SUD))
  		{
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  				vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  		else if ((vl_sens == XDC_SENS_NORD) || 
  			(vl_sens == XDC_SENS_ENTREE_NORD) || 
  			(vl_sens == XDC_SENS_SORTIE_NORD))
   		{
  			vl_i= strlen(vl_str1)-1;
 	 		while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
 	 			vl_i--;
  			}
  			vl_str1[vl_i+1]='\0';
  			strcpy(vl_loc.l06,vl_str1);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier3;
  			vl_loc.l11 = vl_pr - vl_entier1;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  
  		else {
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  					vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;

  			strcpy(vl_loc.l05,"unknown");
  			strcpy(vl_loc.l07,C_l07_U);
  		}
  
      
  		sprintf(pa_fmc[va_indice].RDS.l04,"%d",vl_loc.l04);
  		sprintf(pa_fmc[va_indice].RDS.l06,"%s",vl_loc.l06);
  		sprintf(pa_fmc[va_indice].RDS.l11,"%d",vl_loc.l11);
		
		break;
	}

	break;
    case XZAEC_FMC_BasculTravaux:
    case XZAEC_FMC_Travaux:
    case XZAEC_FMC_ZoneStockage:
	tl_string[0]='\0';
	if ( XZLG49_GetFmcTravaux(va_Numero
			,va_Cle
			,&vl_bool1
			,&vl_bool2
			,&vl_octet11
			,&vl_bool3
					,&vl_octet1
					,&vl_octet2
					,&vl_octet3
					,&vl_octet4
					,&vl_octet5
					,&vl_octet6
					,&vl_octet7
					,&vl_octet8
					,&vl_octet9
					,&vl_octet10
			,&vl_pr1
			,&vl_pr2
			,&vl_pr3
			,&vl_pr4
			,&vl_pr5
			,&vl_pr6
			,&vl_pr7
			,&vl_pr8
			,&vl_pr8
			,&vl_pr10
			,&vl_pr11
			,&vl_pr12
			,&vl_pr13
			,&vl_pr14
			,&vl_pr15
			,&vl_pr16
			,&vl_pr17
			,&vl_pr18
			,&vl_pr19
			,&vl_pr20
			,&vl_PR
			,&vl_longueur
			,&vl_vitesse) == XDC_OK) {
		pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Longueur=vl_longueur;	
		pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Vitesse=vl_vitesse;
			XZST_03EcritureTrace(XZSTC_DEBUG1,"Travaux.Vitesse %d",
					pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Vitesse);
		if (va_Type == XZAEC_FMC_ZoneStockage) {
			if (vl_bool4==1) /*(si "voies retrecies"=1)*/  {
				pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Mobiles=1;
				sprintf(pa_fmc[va_indice].Bloc_Optio.Comment,"%s","stockage demarre, ");
			}
			else {
				pa_fmc[va_indice].Type = XZAEC_FMC_Travaux;
			}
		}
		else {
			if (vl_bool1==1)
				pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Mobiles=(int)'M';
			else
				pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Mobiles=(int)'S';
			sprintf(pa_fmc[va_indice].Bloc_Optio.Comment,"");
			if (vl_bool2 == 1) {
				pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Type=XZAEC_TRV_Neufs;
				sprintf(pa_fmc[va_indice].Bloc_Optio.Comment,"%s","travaux neufs, ");
			}
			else {
				pa_fmc[va_indice].Bloc_Optio.Bloc.Travaux.Type=XZAEC_TRV_Entretien;
				sprintf(pa_fmc[va_indice].Bloc_Optio.Comment,"%s","travaux d'entretien, ");
			}
		}
  		vl_i= strlen(tl_string)-1;
  		while (vl_i > 0 && tl_string[vl_i]==' ' ) {
  			vl_i--;
  		}
  		tl_string[vl_i+1]='\0';
		pa_fmc[va_indice].PR_Fin=vl_pr;
		strcat (pa_fmc[va_indice].Bloc_Optio.Comment,tl_string);
		strcat (pa_fmc[va_indice].Bloc_Optio.Comment,"#\n");
		
		/*vl_pr=vl_entier1; */ /*correction JMG 110311*/
		vl_pr = vl_PR;
  		if (XZAE146_Recherche_Loc_Rds(vl_pr,
  				vl_autoroute,
  				vl_loc.l01,
  				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,
				vl_nom_autoroute ) != XDC_OK) {
    			XZST_03EcritureTrace(XZSTC_WARNING,"agen_evt_genere_strada: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_Cle);
    			return (XDC_NOK);
  			}

                strcpy(vl_loc.rnp,vl_nom_autoroute);
                vl_pr_entier=floor(vl_pr/1000)*1000;
                sprintf(vl_loc.rpip,"%d",vl_pr_entier);
                vl_pr_m= vl_pr - vl_pr_entier;
                sprintf(vl_loc.dfpp,"%d",vl_pr_m);
                strcpy(pa_fmc[va_indice].RDS.rnp,vl_loc.rnp);
                strcpy(pa_fmc[va_indice].RDS.rpip,vl_loc.rpip);
                strcpy(pa_fmc[va_indice].RDS.dfpp,vl_loc.dfpp);
                strcpy(pa_fmc[va_indice].RDS.drp,vl_loc.drp);



  		if ((vl_sens == XDC_SENS_SUD) || 
  			(vl_sens == XDC_SENS_ENTREE_SUD) || 
  			(vl_sens == XDC_SENS_SORTIE_SUD))
  		{
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  				vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  		else if ((vl_sens == XDC_SENS_NORD) || 
  			(vl_sens == XDC_SENS_ENTREE_NORD) || 
  			(vl_sens == XDC_SENS_SORTIE_NORD))
   		{
  			vl_i= strlen(vl_str1)-1;
 	 		while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
 	 			vl_i--;
  			}
  			vl_str1[vl_i+1]='\0';
  			strcpy(vl_loc.l06,vl_str1);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier3;
  			vl_loc.l11 = vl_pr - vl_entier1;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  
  		else {
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  					vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;

  			strcpy(vl_loc.l05,"unknown");
  			strcpy(vl_loc.l07,C_l07_U);
  		}
  
      
  		sprintf(pa_fmc[va_indice].RDS.l04,"%d",vl_loc.l04);
  		sprintf(pa_fmc[va_indice].RDS.l06,"%s",vl_loc.l06);
  		sprintf(pa_fmc[va_indice].RDS.l11,"%d",vl_loc.l11);
		
	}


	if (va_Type==XZAEC_FMC_BasculTravaux) {
	if ( XZLG48_GetFmcBasculement (va_Numero
					,va_Cle
					,&vl_entier2
					,&vl_entier1
					,&vl_octet1
					,&vl_octet2
					,&vl_octet3
					,&vl_octet4
					,&vl_octet5
					,&vl_octet6
					,&vl_octet7
					,&vl_octet8
					,&vl_octet9
					,&vl_octet10
					,&vl_vitesse
					,&vl_vitesse_opposee) == XDC_OK) {

		XZST_03EcritureTrace(XZSTC_DEBUG1,"vitesse %d, opp %d",
					vl_vitesse,vl_vitesse_opposee);
		pa_fmc[va_indice].Bloc_Optio.Bloc.Basculement.Longueur=vl_entier1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Basculement.Vitesse=vl_vitesse;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Basculement.VitesseOpposee=vl_vitesse_opposee;
		vl_pr=vl_entier2;
	}
	}

	break;

    case XZAEC_FMC_IncSsConces:
    
	vl_bool1=0;
	vl_bool2=0;
	/*if ( XZAE133_Lire_Fiche_SousCnc_Pour_Strada(va_Numero*/
	if ( XZLG50_GetFmcSousCnc(va_Numero
						    	,va_Cle
						    	,tl_nom
						    	,&vl_bool1
						    	,&vl_bool2
							,&vl_octet1) == XDC_OK) {
		
		sprintf (tl_string, "Concessionnaire %s ",tl_nom);
						    	
		strcpy(pa_fmc[va_indice].Bloc_Optio.Bloc.Concession.Concessionnaire,tl_nom);
		pa_fmc[va_indice].Bloc_Optio.Bloc.Concession.Penurie=vl_bool2;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Concession.Fermeture=vl_bool1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Concession.Carburant=vl_octet1;
		if (vl_bool1 == 1) {
		
			strcat (tl_string, ", ferme");
		}
		if (vl_bool2 == 1) {
		
			strcat (tl_string, ", penurie de carburant");
		}
		
		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s#\n",tl_string);	
		break;
	}
	break;

    case XZAEC_FMC_QueueBouchon:
    
    	vl_type = -1;
    
	if (XZLG51_GetFmcBouchon (va_Numero
					,va_Cle
					,&vl_pr
					,&vl_mot1
					,&vl_entier1
					,&vl_num_cause2
					,&vl_cle_cause2) == XDC_OK) {
		tl_string[0]='\0';
		pa_fmc[va_indice].PR_Fin = vl_pr;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Bouchon.Longueur=vl_entier1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Bouchon.Type=vl_mot1;
		pa_fmc[va_indice].Evt_Cause.numero=vl_num_cause2;
		pa_fmc[va_indice].Evt_Cause.cle=vl_cle_cause2;
		XZST_03EcritureTrace(XZSTC_INFO,"agen_evt_genere_strada: PR FIN = %d, longueur = %d, cause=%d, type=%d",
			pa_fmc[va_indice].PR_Fin,
			pa_fmc[va_indice].Bloc_Optio.Bloc.Bouchon.Longueur,
			vl_num_cause2,
			pa_fmc[va_indice].Bloc_Optio.Bloc.Bouchon.Type);
		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s#\n",tl_string);

  	if (XZAE146_Recherche_Loc_Rds(vl_pr,
  				vl_autoroute,
   				vl_loc.l01,
 				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,vl_nom_autoroute ) != XDC_OK) {
    			XZST_03EcritureTrace(XZSTC_WARNING,"agen_evt_genere_strada: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_Cle);
    			return (XDC_NOK);
  		}
    XZST_03EcritureTrace(XZSTC_INFO,"agen_evt_genere_strada: XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_Cle);

                strcpy(vl_loc.rnp,vl_nom_autoroute);
                vl_pr_entier=floor(vl_pr/1000)*1000;
                sprintf(vl_loc.rpip,"%d",vl_pr_entier);
                vl_pr_m= vl_pr - vl_pr_entier;
                sprintf(vl_loc.dfpp,"%d",vl_pr_m);
                strcpy(pa_fmc[va_indice].RDS.rnp,vl_loc.rnp);
                strcpy(pa_fmc[va_indice].RDS.rpip,vl_loc.rpip);
                strcpy(pa_fmc[va_indice].RDS.dfpp,vl_loc.dfpp);
                strcpy(pa_fmc[va_indice].RDS.drp,vl_loc.drp);

  		if ((vl_sens == XDC_SENS_SUD) || 
  			(vl_sens == XDC_SENS_ENTREE_SUD) || 
  			(vl_sens == XDC_SENS_SORTIE_SUD))
  		{
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  				vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  		else if ((vl_sens == XDC_SENS_NORD) || 
  			(vl_sens == XDC_SENS_ENTREE_NORD) || 
  			(vl_sens == XDC_SENS_SORTIE_NORD))
   		{
  			vl_i= strlen(vl_str1)-1;
 	 		while (vl_i > 0 && vl_str1[vl_i]==' ' ) {
 	 			vl_i--;
  			}
  			vl_str1[vl_i+1]='\0';
  			strcpy(vl_loc.l06,vl_str1);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier3;
  			vl_loc.l11 = vl_pr - vl_entier1;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;
  		}
  
  		else {
  			vl_i= strlen(vl_str2)-1;
  			while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  					vl_i--;
  			}
  			vl_str2[vl_i+1]='\0';
 			strcpy(vl_loc.l06,vl_str2);
  			vl_loc.l06[vl_i+1]='\0';
  
  			vl_loc.l04=vl_entier4;
  			vl_loc.l11 = vl_entier2 - vl_pr;
			if (vl_loc.l11<0) vl_loc.l11 = -vl_loc.l11;

  			strcpy(vl_loc.l05,"unknown");
  			strcpy(vl_loc.l07,C_l07_U);
  		}
  
      
  sprintf(pa_fmc[va_indice].RDS.l04,"%d",vl_loc.l04);
  sprintf(pa_fmc[va_indice].RDS.l06,"%s",vl_loc.l06);
  sprintf(pa_fmc[va_indice].RDS.l11,"%d",vl_loc.l11);
		break;
	}
	break;

    case XZAEC_FMC_PietonChaussee:
    case XZAEC_FMC_Obstacle :
	if (XZAE207_Lit_Nature(va_Numero,va_Cle,&vl_octet1,vl_str1) == XDC_OK) {
		XZLG300_trim(vl_str1);
		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s\n",vl_str1);	
		
	}
	break;

    case XZAEC_FMC_Salage :
    case XZAEC_FMC_AnimalErrant:
	break;
    case XZAEC_FMC_AireFermee:
	break;

    case XZAEC_FMC_FermetureEch:
    
	vl_bool1=0;
	vl_bool2=0;
	vl_bool3=0;
	vl_bool4=0;
	/*if ( XZAE139_Lire_Fiche_Echangeur_Pour_Strada(va_Numero*/
	if ( XZLG52_GetFmcFermEchangeur(va_Numero
						    	,va_Cle
						    	,&vl_bool1
						    	,&vl_bool2
						    	,&vl_bool3
						    	,&vl_bool4 ) == XDC_OK) {

		tl_string[0] = '\0';
		
		if (vl_bool4 == 1) {
			strcat (tl_string, "sens 2 sortie fermee, "); 
		}
		if (vl_bool2 == 1) {
			strcat (tl_string, "sens 2 entree fermee, "); 
		}	
		if (vl_bool3 == 1) {
			strcat (tl_string, "sens 1 sortie fermee, "); 
		}	
		if (vl_bool1 == 1) {
			strcat (tl_string, "sens 1 entree fermee "); 
		}
		pa_fmc[va_indice].Bloc_Optio.Bloc.Echangeur.Entree1=vl_bool1;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Echangeur.Entree2=vl_bool2;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Echangeur.Sortie1=vl_bool3;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Echangeur.Sortie2=vl_bool4;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Echangeur.Nb_Entrees=vl_nb_entrees;
		pa_fmc[va_indice].Bloc_Optio.Bloc.Echangeur.Nb_Sorties=vl_nb_sorties;
		}

		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s\n",tl_string);		    	
		
		break;
		

  }

   if (pa_fmc[va_indice].Evt_Cause.numero!=0) {
    sprintf(pa_fmc[va_indice].Bloc_Optio.Cause,"ID_%d_%d",
		pa_fmc[va_indice].Evt_Cause.numero,
		pa_fmc[va_indice].Evt_Cause.cle);
  }
  else {
    strcpy(pa_fmc[va_indice].Bloc_Optio.Cause,"");
  }

 strcpy(pm_comment,""); /*LCL*/
 if ( XZLG02_GetComment(va_Numero, va_Cle,agen_evt_genere_commentaires) != XDC_OK)
 {
       XZST_03EcritureTrace(XZSTC_WARNING, "Erreur a la recuperation des commentaires FMC") ;
 }
 else if (strlen(pm_comment) != 0)
 {
 	vl_taille=strlen(pa_fmc[va_indice].Bloc_Optio.Comment)-6;
	XZST_03EcritureTrace(XZSTC_DEBUG1, "TAILLE %d",vl_taille);
	if ( vl_taille < TAILLE_SUR)
 	{
		if (vl_taille+4>=0)
		  pa_fmc[va_indice].Bloc_Optio.Comment[vl_taille+4]='\0';
		if (TAILLE_SUR-vl_taille-6>=0)
		  pm_comment[TAILLE_SUR-vl_taille-6]='\0';
 		sprintf (pa_fmc[va_indice].Bloc_Optio.Comment,"%s%s",pa_fmc[va_indice].Bloc_Optio.Comment,pm_comment);
 	}
 }

  /*ecrit le fichier strada et envoie le sur le PC strada*/
  /*agen_xml_write2(pa_fmc[va_indice],va_Type);*/

  XZST_03EcritureTrace(XZSTC_DEBUG1, "SORTIE JMG");
  return (XDC_OK);

}







/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* cumul le nb de voies totales et en circulation
* a pertie de l'etat de la voie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void gen_cumul_voies(XDY_Voie x,int *pa_tot,int *pa_cir)

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

