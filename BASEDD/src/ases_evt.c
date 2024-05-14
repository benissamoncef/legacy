/*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : ases_evt.c				*
*************************************************
* DESCRIPTION :
* module externe de la tache serveur evt
* pour le PC STRADA
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
*************************************************
* HISTORIQUE :
*
* Guilhou	08 Mai 1997 : Creation 	     V1.1
* Penot		26 Oct 1997 : Codage module ases_evt_genere_strada
* Niepceron	30 Oct 1998 : PHA devient PHR cf "STRADA-NET V1 Interface pour situation v1.3" v1.5
* Niepceron	21 Dec 1998 : Integration intergestionnaire dem/1724 v1.6
* Niepceron	02 Mar 1999 : correction integration intergest v1.7
* Niepceron	01 Avr 1999 : substitue "|" par " " ds les commentaires des FtmC v1.9
* Cluchague	29 Jun 2004 : modification du nom de fichier Snnnnnnn.sit -> SESCnnnn.sit v1.10
* Niepeceron 	03 Mar 2005 : Modif bouchon accordeon 1.11 DEM428
* CHI   13/04/05        : Ajout sous type meteo "Grele sur chaussee" et "Chaussee mouillee" 1.2 DEM456
* JMG   02/03/04        ; appel a ases_action_cb dans ases_evt_cb_validation_fmc (SAGA)  1.13
* Niepceron	26/09/05    : Correction du d�bordement du commentaire dans les attributs du vl_bloc 1.14
* JMG	30/11/05	: correction nom de fichier emis 1.15
* niepceron	12/04/06 1.16 : LTV =5 DEM568
* niepceron	12/04/06 1.17 : correction du VCL (type de veh) DEM569 
* niepceron	04/01/07 1.18 : Ajout pour le remplissage et l'�criture des fichiers passerelle pour PX-METACOR DEM605
* niepceron	02/04/07 1.19 : blesses graves= bless�s graves+morts
* lecrivain+PNI	22/05/07 1.20 : Ajout de nvx types FMC DEM647
* PNI		22/05/07 1.21 : Ajout du second destinataire pour les fichiers passerelle METACOR
* PNI		18/07/07 1.22 : Les evt sur p�age ne sont pas pris en compte pour PX-METACOR DEM675
* PNI		23/07/07 1.23 :Bouchon sur bretelle : forcer la Queue sur bretelle DEM678
* PNI		06/09/07 1.24 : DATEX : Ajout code pour fermeture echang & evt sur echang
					Ajout PHR suivant type de bouchon DEM701
* PNI		05/11/07 1.25 :test sccs 
* PNI		05/11/07 1.26 :test sccs 
* niepceron	11/12/07 1.27 : LTV =6 DEM732
* niepceron	19/12/07 1.26 : Pas de prise en compte du r�seultat du ping vers ASF DEM738
* PNI		06/06/08 1.27 : Ajoute SNA pour Italie, Pas d'envoide LNP si vide, nom loca rds sur char[51] DEM701
* niepceron	07/01/09 1.30 : LTV =7 DEM859
* PNI		02/11/09      : PHR SAL pour salage/traitChauss DEM902
* PNI		18/11/09 1.32 : Prise en compte des nouvelles PHR DEM902
* PNI		05/03/10 1.33 : On ne supprime plus les fichiers si strada hs pour les conserver et les transf�rerer � AT
* PNI		16/08/10 1.34 : Recup�ration de la date de fin prevue pour datex STO
* JPL		31/08/10 1.35 : Ajout du type Fmc "Zone de stockage"
* JPL		21/09/10 1.36 : Changement du commentaire pour les Fmc "Zone de stockage"
* JMG   15/02/11 : ajout nom autoroute dans XZAE146 1.38
* PNI 19/08/11 : Suppression des copies de fichiers METACOR sur les serveurs 1.39 DEM988
* PNI         09/03/12 1.40 : correction code datex pour veh en marche arriere 
* PNI         11/04/12 1.40 : correction code datex pour basculement
* JMG	20/04/12 : regulation de vitesse DEM 1016 1.41
* PNI	31/07/12 : basuclement ==> RES/CTR + correction du merge rgv 1.42
* PNI	28/03/13 : En cas de perte du PC STRADA, copie des fichiers pour taweb.x puis suppression v1.44
* SFO   06/10/14 : Adaptation pour compilation sous linux
* PNI	05/03/15 : Suppression de la transmission des commentaires v1.46 DEM1118
* PNI 9/9/2015 : Remplacement Frontiere Italienne par Italie pour L07 lim. 35car DEM1145 1.47
* PNI	9/9/2015 : Remplacement LOC par P1.1 + integration heure hivers/ete DEM1145 1.47
* PNI	6/1/2016 : Correction LNP pour Aire et peage DEM1210 1.48
* PNI	26/03/18 : Suppression du noeud STRADA=>suppression du test de presence STRADA DEM1285 1.49
* LCL	22/06/20 : Correction plantage sous Linux 64 : trace sur variable pas initialis�e MOVIS DEMXXXX 1.50
* LCL	04/10/23 : Adaptation CNA DEM-483 : lecture site principal en base
************************************************/


#include "ases_evt.h"

#define CO_NB_JOUR_PURGE        3
#define CO_DELAI_FREQUENCE      360
#define CO_DELAI_PURGE          86400 * CO_NB_JOUR_PURGE
#define CO_NB_CHIFFRE           13

#define C_PC_STRADA             "STRADA"
#define C_PC_METACOR             "METACOR"
#define C_PC_METACOR_ASF             "METACOR_ASF"

/* definition des constantes */

#define CO_FIC_COMPTEUR         XDC_PATH_STRADA "/compteur_evt.txt"
#define CO_FIC_EVTSTR		XDC_PATH_STRADA "/evt_out.txt"
#define	CO_MAX_CARACTERE	80
#define TAILLE 200
#define C_FREQ_PURGE		86400 * 3
#define C_TAB_SEPAR		"\t"
#define	C_FREQ_VERIF_EVT	10
#define C_CODE_ESCOTA		"ESCOTA"
#define C_DATEX_ESCOTA		"fr204"

#define C_PATH 			XDC_PATH_STRADA	"/evtout"
#define C_PATH_METACOR 		XDC_PATH_STRADA	"/evtout_M"
#define C_SCRIPT_FTP 		XDC_PATH_STRADA "/ftp/sit_script_put"
#define C_SCRIPT_FTP_METACOR 	XDC_PATH_STRADA "/ftp/sit_script_put_M"

#define NBR_PREMIER 	53
#define NBR_ELT_TABLE	59

#define C_l01		"F32"
#define C_l02		"P1.1"
#define C_l03		4
#define C_l07_P		"Aix en Provence -> Frontiere Italienne"
#define C_l07_N		"Frontiere italienne -> Aix en Provence"
#define C_l07_U		"Sens non d�fini"
#define C_l07_B		"2 Sens"


#define SG_PING         "/etc/ping"
#define SG_TO_PING      8
#define SG_BYTES_PING   64
#define SG_COUNT_PING   1
#define SG_TOKEN_TST    "64"

#define TAILLE_SUR	350

/* METACOR */
#define C_SECTION_COURANTE	"SC"
#define C_ECHANGEUR	"Brt Ech"
#define C_AIRE	"Brt Aire"
#define C_BIFURC	"Brt Bif"

#define C_ACTIF	"ACTIF"
#define C_PREVU	"PREVU"
#define C_FIN	"FIN"



/* d�finition des variables globales � l'application */


/* definition des variables locales au module */
T_Liste_FMC		*tg_fmc;
T_Liste_FMC		*tg_fmc_courant;


static char			*tm_l07_p[]={
	"",
	"Aix en Provence -> Italie",
	"A8 -> Monaco",
	"Marseille -> Toulon",
	"Marseille -> A52",
	"Aix -> Les Alpes",
	"Aix -> Aubagne",
	"A52 -> Auriol",
	"Toulon -> Le Luc"};

static char			*tm_l07_n[]={
	"",
	"Italie -> Aix en Provence",
	"MonacoA8 -> Monaco",
	"Toulon -> Marseille",
	"A52 -> Marseille",
	"Les Alpes -> Aix",
	"Aubagne -> Aix",
	"Auriol -> A52",
	"Le Luc -> Toulon"};

T_Type_FMC		tg_types_fmc[]=
{
	{	XZAEC_FMC_Accident,		"ACC"	,	"ACI"	},
	{	XZAEC_FMC_Delestage,		"ROU"	,	"RAD"	},
	{	XZAEC_FMC_Deviation,		"ROU"	,	"DO"	},
	{	XZAEC_FMC_PosteOperateur,		"OPA"	,	"CLE"	},
	{	XZAEC_FMC_Meteo,		"WDA"	,	"WDA"	},
	{	XZAEC_FMC_Contresens,		"MHZ"	,	"VWC"	},
	{	XZAEC_FMC_VehArrete,		"INC"	,	"INS"	},
	{	XZAEC_FMC_VehPanne,		"INC"	,	"BKD"	},
	{	XZAEC_FMC_VehFeu,		"INC"	,	"VFR"	},
	{	XZAEC_FMC_Cirque,		"MHZ"	,	"CVY"	},
	{	XZAEC_FMC_ConvoiExc,		"MHZ"	,	"ABL"	},
	{	XZAEC_FMC_ConvoiMil,		"MHZ"	,	"MIL"	},
	{	XZAEC_FMC_VehLent,		"MHZ"	,	"SVH"	},
	{	XZAEC_FMC_Manifestation,		"ACT"	,	"EVD"	},
	{	XZAEC_FMC_GreveESCOTA,		"ACT"	,	"EST"	},
	{	XZAEC_FMC_Escargot,		"ACT"	,	"EPR"	},
	{	XZAEC_FMC_Basculement,		"RES"	,	"CTR"	},
	{	XZAEC_FMC_Travaux,		"RMT"	,	"RWK"	},
	{	XZAEC_FMC_IncSsConces,		"RES"	,	"SFC"	},
	{	XZAEC_FMC_QueueBouchon,		"LOS"	,	"LS1"	},
	{	XZAEC_FMC_FermetureEch,		"RES"	,	"REX"	},
	{	XZAEC_FMC_Eboulement,		"OHZ"	,	"ROC"	},
	{	XZAEC_FMC_Incendie,		"OHZ"	,	"FIG"	},
	{	XZAEC_FMC_Inondation,		"OHZ"	,	"FLD"	},
	{	XZAEC_FMC_Obstacle,		"OHZ"	,	"OHX"	},
	{	XZAEC_FMC_ChausseDeter,		"SHZ"	,	"RPC"	},
	{	XZAEC_FMC_FlaqueHuile,		"SHZ"	,	"FUE"	},
	{	XZAEC_FMC_AnimalErrant,		"OHZ"	,	"ANM"	},
	{	XZAEC_FMC_FrontiereFermee,		"INC"	,	"INS"	},
	{	XZAEC_FMC_AltercationUsager,		"ACT"	,	"ESI"	},
	{	XZAEC_FMC_Agression,		"ACT"	,	"ESI"	},
	{	XZAEC_FMC_MalaiseUsager,		"INC"	,	"INS"	},
	{	XZAEC_FMC_EvtHorsReseau,		"ACT"	,	"ESM"	},
	{	XZAEC_FMC_AireFermee,		"RES"	,	"SRC"	},
	{	XZAEC_FMC_Salage,		"OPA"	,	"SAL"	},
	{	XZAEC_FMC_HoldUpVol,		"ACT"	,	"GUN"	},
	{	XZAEC_FMC_TeteBouchon,		"LOS"	,	"LS1"	},
	{	XZAEC_FMC_PietonChaussee,		"ACT"	,	"PEO"	},
	{	XZAEC_FMC_Autre,		"INC"	,	"INS"	},
	{	XZAEC_FMC_VehMarcheArr,		"MHZ"	,	"VWC"	},
	{	XZAEC_FMC_TraitChaussees,		"RMT"	,	"RWK"	},
	{	XZAEC_FMC_EchDeconseille,		"ROU"	,	"RAD"	},
	{	XZAEC_FMC_ChausseeGliss,		"SHZ"	,	"RSL"	},
	{	XZAEC_FMC_BasculTravaux,		"RES"	,	"CTR"	},
	{	XZAEC_FMC_ZoneStockage,		"RES"	,	"RET"	},
	{	XZAEC_FMC_Regulation,		"OPA"	,	"SPM"	}
};

static char pm_comment[TAILLE_SUR+1]="";

/* definition des vriables locales au module */
static char *	version = "@(#)ases_evt.c	1.38 04/04/11 : commentaire" ;
extern XDY_Mot		vg_SitePrincipal;
extern XDY_NomSite vg_NomSitePrincipal;



/* declaration de fonctions internes */
int	ases_recuperer_fmc(XDY_Entier, XDY_Octet, XDY_Mot);
void	ases_evt_purge();
void	ases_evt_cb_validation_fmc();
int	ases_evt_genere_strada();
int	ases_evt_genere_commentaire();
int ases_evt_ecrit_strada();

char *heurehiversete(int, int, int);

/* definition de fonctions externes */



/*A
*******************************************
* Fonction d'initialisation pour la tache
* serveur de donnees trafic
* Elle lit les fichiers de parametrage
* et arme les deux timers :
* - le timer de purge
* - charger le tableau de listes chainees
* contenant l'intitule relatif au code 
* evenement Strada
*******************************************
* Arguments : Aucun
*******************************************/

int ases_evt_init()
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
	XDG_JoindreDG(XDG_AS_SRVEVT_STRADA);
	
	vl_HorodateSec+=C_FREQ_VERIF_EVT;
	/* armement du timer de programmation */
	if (XZSM_01DemanderProgrammation(       vl_HorodateSec,
						0,
						XDG_AS_SRVEVT_STRADA,
						XDM_AS_SRVEVT_STRADA,
						"cle_timer_srvevt",
						XZSMC_SEC_VRAI) != XDC_OK)
	{
	  XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de verif des evts non declenchee." );
	  return (XDC_NOK);
  	}

	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_SRVEVT_STRADA), ases_evt_cb_validation_fmc,(T_CB_ARG)NULL) == NULL)
	{
		XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AS_SRVEVT_STRADA non cree.",version );
		return XDC_NOK;
	}

	tg_fmc=NULL;
	tg_fmc_courant=NULL;

	/* Lecture site principal en base */
	if (XZAO00_SitePrincipal(&vg_SitePrincipal,vg_NomSitePrincipal) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL,"%s : Site principal non defini en base RES_DIS.",version );
		return XDC_NOK;
	}

	/* Ecriture Trace */
	XZST_03EcritureTrace( XZSTC_INFO, "OUT : ases_evt_init" );
	return XDC_OK;

}



/* Fichier : @(#)ases_evt.c	1.38        Release : 1.38        Date : 04/04/11
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE ases_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* libere les ressources
*
******************************************************
* SEQUENCE D'APPEL :
* ases_free_ressources();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************
/*extern*/ 
int ases_free_ressources()
{
  T_Liste_FMC	*pl_ex_fmc;

  XZST_03EcritureTrace(XZSTC_FONCTION,"ases_free_ressources: IN");

  while (tg_fmc!=NULL) {
    pl_ex_fmc=tg_fmc;
    tg_fmc=tg_fmc->Suivant;
    free(pl_ex_fmc);
  }
 
  XZST_03EcritureTrace(XZSTC_FONCTION,"ases_free_ressources: OUT");
  return (XDC_OK);
}


/* Fichier : @(#)ases_evt.c	1.38        Release : 1.38        Date : 04/04/11
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* purge
******************************************************
* SEQUENCE D'APPEL :
* ases_evt_purge
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

void ases_evt_purge()
{
  return ;
}


/* Fichier : @(#)ases_evt.c	1.38        Release : 1.38        Date : 04/04/11
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* reveil pour verifier qu'il y a des fmc a exporter
*****************************************************
* SEQUENCE D'APPEL :
* ases_evt_cb_validation_fmc
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
void ases_evt_cb_validation_fmc()
{
  T_Liste_FMC	*pl_liste_fmc;
  T_Liste_FMC	*pl_ex_fmc;
  int		vl_val;
  int 		vl_JourSemaine;
  double 	vl_HorodateSec; 


  XZST_03EcritureTrace(XZSTC_INFO,"ases_evt_cb_validation_fmc: debut de fonction !");

  /*recup de la liste des fmc modifiees*/
  tg_fmc=NULL;
  pl_liste_fmc=NULL;
  if (XZAE126_Recherche_Dernieres_FMC((XDY_FonctionInt)ases_recuperer_fmc,&vl_val) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_FATAL,"ases_evt_cb_validation_fmc: pb dans l'appel a XZAE126_Recherche_Dernieres_FMC");
    return;
  }

  /* pour chaque fmc de la liste */
  XZST_03EcritureTrace(XZSTC_INFO,"ases_evt_cb_validation_fmc: avant while");
 
  pl_liste_fmc=tg_fmc;
  while (pl_liste_fmc!=NULL) {
    /* genere le fichier strada correspondant*/
    ases_evt_genere_strada(pl_liste_fmc->FMC);

    /* libere la memoire */
    pl_ex_fmc=pl_liste_fmc;

    pl_liste_fmc=pl_liste_fmc->Suivant;
    free(pl_ex_fmc);
  }

  tg_fmc=NULL;
  
  /* Envoi TFM*/
  ases_tfm_cb();
  /* Envoi PMV*/
  ases_pmv_cb();
 
   /*SAGA*/
    /*envoi des actions*/
     ases_action_cb();

  
	/*A Lecture horodate courante systeme */
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
  	vl_HorodateSec+=C_FREQ_VERIF_EVT;
	/* armement du timer de programmation */
	if (XZSM_01DemanderProgrammation(       vl_HorodateSec,
						0,
						XDG_AS_SRVEVT_STRADA,
						XDM_AS_SRVEVT_STRADA,
						"cle_timer_srvevt",
						XZSMC_SEC_VRAI) != XDC_OK)
	{
	  XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de verif des evts non declenchee." );
	  return ;
  	}

  return ;
}



/* Fichier : @(#)ases_evt.c	1.38        Release : 1.38        Date : 04/04/11
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* fct utilisateur passee dans XZAE126
* ajoute une fmc a la liste des fmc a exporter
*
******************************************************
* SEQUENCE D'APPEL :
* ases_recuperer_fmc(va_numero,va_cle,va_type)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/

int ases_recuperer_fmc(	XDY_Entier	va_numero,
			XDY_Octet	va_cle,
			XDY_Mot		va_type)
{
  T_FMC	vl_fmc;

  XZST_03EcritureTrace(XZSTC_INFO,"ases_recuperer_fmc: numero %d, cle %d, type %d",
					va_numero,
					va_cle,
					va_type);

  /*premiere insertion*/
  if (tg_fmc==NULL) {
    XZST_03EcritureTrace(XZSTC_INFO,"ases_recuperer_fmc: avant malloc");
    if ((tg_fmc=(T_Liste_FMC *)malloc(sizeof(T_Liste_FMC)))==NULL) {
	XZST_03EcritureTrace(XZSTC_WARNING,"ases_ajouter_fmc: pb de memoire pour le malloc debut de liste");
        return (XDC_NOK);
    }

    XZST_03EcritureTrace(XZSTC_INFO,"ases_recuperer_fmc: apres malloc");
    tg_fmc->FMC.Type=va_type;
    tg_fmc->FMC.Numero=va_numero;
    tg_fmc->FMC.Cle=va_cle;
    tg_fmc->Suivant=NULL;
    tg_fmc_courant=tg_fmc;
    XZST_03EcritureTrace(XZSTC_INFO,"ases_recuperer_fmc: sortie");
    return (XDC_OK);
  }

  /*fin de liste*/
  if ((tg_fmc_courant->Suivant=(T_Liste_FMC *)malloc(sizeof(T_Liste_FMC)))==NULL) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_ajouter_fmc: pb de memoire pour le malloc fin de liste");
    return (XDC_NOK);
  }

  tg_fmc_courant=tg_fmc_courant->Suivant;
  tg_fmc_courant->FMC.Type=va_type;
  tg_fmc_courant->FMC.Numero=va_numero;
  tg_fmc_courant->FMC.Cle=va_cle;
  tg_fmc_courant->Suivant=NULL;
  return (XDC_OK);
}

/* Fichier : @(#)ases_evt.c	1.38        Release : 1.38        Date : 04/04/11
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE ases_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* construit les commentaires
*
******************************************************
* SEQUENCE D'APPEL :
* ases_evt_genere_commentaires();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
int ases_evt_genere_commentaires(char *va_texte)
{
int vl_i=0,vl_j=0;
int vl_taille=0;

  XZST_03EcritureTrace(XZSTC_INFO,"ses_evt_genere_commentaires: IN texte:%s",va_texte);
  
  /*A On enleve les blancs en fin */
  vl_i= strlen(va_texte)-1;
  while (vl_i > 0 && va_texte[vl_i]==' ' ) {
	 vl_i--;
  }
  va_texte[vl_i+1]='\0';
  for (vl_j=0; vl_j <=vl_i;vl_j++)
 	 if ((va_texte[vl_j]=='\n') || (va_texte[vl_j]=='|'))
 	 	va_texte[vl_j]=' ';
  
  XZST_03EcritureTrace(XZSTC_INFO,"texte:%s",va_texte);
  vl_i= strlen(va_texte);
  vl_taille=strlen(pm_comment);
  if (vl_i+vl_taille > TAILLE_SUR)
  	strncat(pm_comment,va_texte,TAILLE_SUR-vl_taille);
  else
  	strcat(pm_comment,va_texte);
  	
  XZST_03EcritureTrace(XZSTC_INFO,"comment:%s",pm_comment);
  XZST_03EcritureTrace(XZSTC_FONCTION,"ses_evt_genere_commentaires: OUT");
  return (XDC_OK);
}



/* Fichier : @(#)ases_evt.c	1.38        Release : 1.38        Date : 04/04/11
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
* ases_evt_genere_strada
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
int ases_evt_genere_strada(T_FMC va_fmc)
{
  T_Fichier		vl_bloc;
  T_Fichier_METACOR	vl_bloc_metacor;
  XDY_Horodate		vl_debut,vl_fin,vl_validation,vl_fin_prevue;
  XDY_Entier		vl_duree;
  XDY_Nom		vl_fausse_alerte,vl_fin_flag;
  XDY_Entier		vl_num_cause;
  XDY_Octet		vl_cle_cause;
  char			vl_type_cause[26];
  XDY_Octet		vl_veracite,vl_trafic;
  XDY_Octet		vl_type_loc;
  XDY_Nom		vl_prevision;
  XZSMT_Horodate	vl_horo_chaine;
  int                   vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
  int			vl_annee1,vl_annee2;
  XDY_Booleen		vl_veh_esc,vl_hom_esc,vl_perso;
  XDY_Mot		vl_plan;
  XDY_Mot		vl_taille;
  char			vl_Config_Voies[256];
  char			vl_Config_Voies_suite[256];
  char			vl_nom_loca[26];
  XDY_Entier		vl_id_loca;
  char			tl_string_comment[400];
  char			tl_string[400],tl_buffer[15],tl_loc_float[15];
  int			vl_i;
  int			vl_visibilite, vl_type;
  XDY_Mot		vl_nombre;
  XDY_Booleen		vl_perception,vl_violence;
  int			vl_bebes,vl_agees,vl_animaux,vl_escota;
  char			tl_matiere[100],tl_observations[100],tl_nom[100];
  XDY_Octet		vl_octet1, vl_octet2, vl_octet3, vl_octet4, vl_octet5, vl_octet6;
  XDY_Booleen		vl_bool1,vl_bool2,vl_bool3,vl_bool4;
  XDY_Entier		vl_entier1,vl_entier2,vl_entier3,vl_entier4;
  XDY_Mot		vl_mot1,vl_mot2,vl_mot3;
  char			vl_localisation[10];
  char			vl_str1[51],vl_str2[51],vl_str3[51];
  loc_rds		vl_loc;
  XDY_Entier		vl_pr,vl_pr_inf,vl_pr_sup;
  XDY_Octet		vl_sens,vl_sens_sc;
  XDY_Octet		vl_autoroute;
  XDY_Horodate	vl_Horodate;
  int vl_Jour_De_La_Semaine;
  XDY_Octet		vl_type_horodate=XDC_ACT_ETAT_SUCCES;
  XDY_Octet		vl_nbvoiesop;
  XDY_Octet		vl_gravite,vl_blesses_graves=0;
  char                  vl_reference[36]; /* Ajout SAGA */
  XDY_Booleen		vl_a_traiter_metacor;
  char			vl_typeheure[5]="";

  XZST_03EcritureTrace(XZSTC_INTERFACE,"ases_evt_genere_strada: IN %d %d",
		va_fmc.Numero,
		va_fmc.Cle);

  vl_fin_prevue=0.0;
  vl_id_loca=0;
  vl_type_loc=0;
  vl_blesses_graves=0;
  sprintf(vl_Config_Voies,"non implement�");
  sprintf(vl_Config_Voies_suite," ");
  sprintf(vl_nom_loca,"non renseign�");

  /*remplissage des champs dont on a deja les infos*/
  sprintf(vl_bloc.Bloc_Oblig.SNM,"SNM#%s/%d/%d#\n"
				  ,C_CODE_ESCOTA
				  ,va_fmc.Numero
				  ,va_fmc.Cle);
  strcpy(vl_bloc.Bloc_Oblig.ERF,"ERF#1#\n");
  vl_bloc.Bloc_Oblig.CLV=1;
  vl_bloc.Bloc_Oblig.LTV=CM_V_ALERT_C;
  vl_bloc.Bloc_Oblig.VNM=1; 
  sprintf(vl_bloc.Bloc_Oblig.DOB,"DOB#%s#\n",tg_types_fmc[va_fmc.Type-1].Code_Strada);
  sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#%s#\n",tg_types_fmc[va_fmc.Type-1].PHR_Strada);

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
  if (XZAE127_Infos_obligatoires(va_fmc.Numero,
				va_fmc.Cle,
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
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: pb appel a XZAE127_Infos_obligatoires %d %d",
				va_fmc.Numero,
				va_fmc.Cle);
    return (XDC_NOK);
  }

  if (XZAE1270_Infos_Metacor(va_fmc.Numero,
				va_fmc.Cle,
				&vl_fin_prevue,
				&vl_type_loc,
				vl_nom_loca,
				&vl_id_loca,
				vl_Config_Voies) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: pb appel a XZAE1270_Infos_Metacor %d %d",
				va_fmc.Numero,
				va_fmc.Cle);
    return (XDC_NOK);
  }

   /* Renseignement pour PX-METACOR */
  if ( vl_type_loc==XDC_POINT_CARACT_PEAGE)
	vl_a_traiter_metacor=XDC_FAUX;
  else
	vl_a_traiter_metacor=XDC_VRAI;
  
  snprintf(vl_bloc_metacor.Bloc_Oblig.Ref,49,"%d-%d",va_fmc.Numero,va_fmc.Cle);
  vl_bloc_metacor.Bloc_Oblig.Evo=vl_entier1;
  

  vl_bloc.Bloc_Oblig.VNM = vl_entier1;
  vl_bloc.Bloc_Optio.Veracite=vl_octet1;

  XZSM_11ConversionHorodate(vl_debut,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: format date incorrect");
    return (XDC_NOK);
  }
  if (vl_prevision[0]=='Y')
  {
  if (vl_annee!=1970)
	sprintf(vl_bloc_metacor.Bloc_Oblig.Debut_prevu,"%02d/%02d/%04d %02d:%02d",
			vl_jour,
			vl_mois,
			vl_annee,
			vl_heure,
			vl_minute);
  else
	sprintf(vl_bloc_metacor.Bloc_Oblig.Debut_prevu,"");
  sprintf(vl_bloc_metacor.Bloc_Oblig.Debut,"");
  }
  else
  {
  if (vl_annee!=1970)
	sprintf(vl_bloc_metacor.Bloc_Oblig.Debut,"%02d/%02d/%04d %02d:%02d",
			vl_jour,
			vl_mois,
			vl_annee,
			vl_heure,
			vl_minute);
  else
	sprintf(vl_bloc_metacor.Bloc_Oblig.Debut,"");
  sprintf(vl_bloc_metacor.Bloc_Oblig.Debut_prevu,"");
  }

  XZSM_11ConversionHorodate(vl_fin,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: format date incorrect");
    return (XDC_NOK);
  }
  if (vl_annee!=1970)
  sprintf(vl_bloc_metacor.Bloc_Oblig.Fin,"%02d/%02d/%04d %02d:%02d",
			vl_jour,
			vl_mois,
			vl_annee,
			vl_heure,
			vl_minute);
  else
	sprintf(vl_bloc_metacor.Bloc_Oblig.Fin,"");


  XZSM_11ConversionHorodate(vl_fin_prevue,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: format date incorrect");
    return (XDC_NOK);
  }
  if (vl_annee!=1970)
  sprintf(vl_bloc_metacor.Bloc_Oblig.Fin_prevue,"%02d/%02d/%04d %02d:%02d",
			vl_jour,
			vl_mois,
			vl_annee,
			vl_heure,
			vl_minute);
  else
	sprintf(vl_bloc_metacor.Bloc_Oblig.Fin_prevue,"");




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

  sprintf(vl_bloc_metacor.Bloc_Oblig.sns_deb_id,"%d",vl_sens_sc);
  sprintf(vl_bloc_metacor.Bloc_Oblig.sns_fin_id,"%d",vl_sens_sc);

  sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_nom,"%s",vl_nom_loca);
  sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s",vl_nom_loca);
  sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_id,"%d",vl_id_loca);
  sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d",vl_id_loca);

  XZST_03EcritureTrace(XZSTC_INFO, "vl_nom_loca : %s %d",vl_nom_loca,vl_id_loca);



  switch (vl_type_loc)
  {
  case 0 : sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_type,C_SECTION_COURANTE);
  	   sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_SECTION_COURANTE);
	   sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_id,"%d",vl_autoroute);
  	   sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d",vl_autoroute);
  	   break;
  case XDC_POINT_CARACT_AIRE  : sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_type,C_AIRE);
  	   sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_AIRE);
	   sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_id,"%d",vl_id_loca);
	   sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d",vl_id_loca);


  	   break;
  case  XDC_POINT_CARACT_ECHANGEUR : sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_type,C_ECHANGEUR);
  	   sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_ECHANGEUR);
	   if (( vl_sens== XDC_SENS_ENTREE_SUD) || ( vl_sens== XDC_SENS_ENTREE_NORD))  {
		sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_nom,"%s entree %d",vl_nom_loca,vl_sens_sc);
		sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s entree %d",vl_nom_loca,vl_sens_sc);
		sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_id,"%d%d",vl_id_loca,vl_sens);
		sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d%d",vl_id_loca,vl_sens);
	   }
	   if (( vl_sens== XDC_SENS_SORTIE_SUD) || ( vl_sens== XDC_SENS_SORTIE_NORD))  {
		sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_nom,"%s sortie %d",vl_nom_loca,vl_sens_sc);
		sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s sortie %d",vl_nom_loca,vl_sens_sc);
		sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_id,"%d%d",vl_id_loca,vl_sens);
		sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d%d",vl_id_loca,vl_sens);
	   }
  	   break;
  case  XDC_POINT_CARACT_BIFURCATION : sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_type,C_BIFURC);
  	   sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_BIFURC);
	   if (( vl_sens== XDC_SENS_ENTREE_SUD) || ( vl_sens== XDC_SENS_ENTREE_NORD))  {
		sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_nom,"%s entree %d",vl_nom_loca,vl_sens_sc);
		sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s entree %d",vl_nom_loca,vl_sens_sc);
		sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_id,"%d%d",vl_id_loca,vl_sens);
		sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d%d",vl_id_loca,vl_sens);
	   }
	   if (( vl_sens== XDC_SENS_SORTIE_SUD) || ( vl_sens== XDC_SENS_SORTIE_NORD))  {
		sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_nom,"%s sortie %d",vl_nom_loca,vl_sens_sc);
		sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s sortie %d",vl_nom_loca,vl_sens_sc);
		sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_id,"%d%d",vl_id_loca,vl_sens);
		sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d%d",vl_id_loca,vl_sens);
	   }
  	   break;
  default : sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_type,C_SECTION_COURANTE);
  	   sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_SECTION_COURANTE);
  }

  


  sprintf(vl_bloc_metacor.Bloc_Oblig.pk_deb,"%.3f",(float) vl_pr/1000);

  sprintf(vl_bloc_metacor.Bloc_Oblig.pr_deb,"%.3f",(float) vl_pr/1000);

  sprintf(vl_bloc_metacor.Bloc_Oblig.pk_fin,"%.3f",(float) vl_pr/1000);

  sprintf(vl_bloc_metacor.Bloc_Oblig.pr_fin,"%.3f",(float) vl_pr/1000);

  
  snprintf(vl_bloc_metacor.Bloc_Oblig.rest,255,"%s",vl_Config_Voies);


  if ( vl_fin != 0.0 )
  {	XZST_03EcritureTrace(XZSTC_INFO,"Evt fini!");
  	sprintf(vl_bloc_metacor.Bloc_Oblig.etat,C_FIN);
  }
  else if ( vl_prevision[0] == 'Y' )
  {	XZST_03EcritureTrace(XZSTC_INFO,"Evt prevu!");
  	sprintf(vl_bloc_metacor.Bloc_Oblig.etat,C_PREVU);
  }
  else
  {	XZST_03EcritureTrace(XZSTC_INFO,"Evt actif!");
  	sprintf(vl_bloc_metacor.Bloc_Oblig.etat,C_ACTIF);
  }

  vl_bloc_metacor.Bloc_Optio.Bloc.Incident.VL=0;
  vl_bloc_metacor.Bloc_Optio.Bloc.Incident.PL=0;
  vl_bloc_metacor.Bloc_Optio.Bloc.Incident.Blesses_graves=0;

  sprintf(vl_bloc_metacor.Bloc_Oblig.Type,"RES");

  /* Rensignement pour STRADA */

  switch (vl_gravite)
  {
  case 0 : vl_bloc.Bloc_Optio.Gravite=5;
  	   break;
  case 1 : vl_bloc.Bloc_Optio.Gravite=3;
  	   break;
  case 2 : vl_bloc.Bloc_Optio.Gravite=2;
  	   break;
  case 3 : vl_bloc.Bloc_Optio.Gravite=1;
  	   break;
  default : vl_bloc.Bloc_Optio.Gravite=5;
  }
     XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: avant xzae146");
  if (XZAE146_Recherche_Loc_Rds(
  				vl_pr,
  				vl_autoroute,
  				vl_loc.l01,
  				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,vl_str3 ) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_fmc.Cle);
    return (XDC_NOK);
  }
     XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: apres xzae146");
  
 /* Traitement de la reference Ajout SAGA */
  vl_i= strlen(vl_reference)-1;
  while (vl_i > 0 && vl_reference[vl_i]==' ' ) {
        vl_i--;
  }
  vl_reference[vl_i+1]='\0';
  if (vl_reference[0])
     sprintf(vl_bloc.Bloc_Oblig.SNM,"SNM#%s#\n",vl_reference);


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
  	strcpy(vl_loc.l10,vl_str1);
  	vl_loc.l10[vl_i+1]='\0';
  
  	vl_i= strlen(vl_str2)-1;
  	while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  		vl_i--;
  	}
  	vl_str2[vl_i+1]='\0';
 	strcpy(vl_loc.l06,vl_str2);
  	vl_loc.l06[vl_i+1]='\0';
  
 
  	vl_loc.l09=vl_entier3;
  	vl_loc.l04=vl_entier4;
  	vl_loc.l03 = C_l03;
  
  	strcpy(vl_loc.l05,"P");
  	strcpy(vl_loc.l07,tm_l07_p[vl_autoroute]);
  	
  	vl_loc.l12 = vl_pr - vl_entier1;
  	vl_loc.l11 = vl_entier2 - vl_pr;
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
  	strcpy(vl_loc.l06,vl_str1);
  	vl_loc.l06[vl_i+1]='\0';
  
  	vl_i= strlen(vl_str2)-1;
  	while (vl_i > 0 && vl_str2[vl_i]==' ' ) {
  		vl_i--;
  	}
  	vl_str2[vl_i+1]='\0';
 	strcpy(vl_loc.l10,vl_str2);
  	vl_loc.l10[vl_i+1]='\0';
  
 
  	vl_loc.l04=vl_entier3;
  	vl_loc.l09=vl_entier4;
  	vl_loc.l03 = C_l03;
  
  	strcpy(vl_loc.l05,"N");
  	strcpy(vl_loc.l07,tm_l07_n[vl_autoroute]);
  	
  	vl_loc.l11 = vl_pr - vl_entier1;
  	vl_loc.l12 = vl_entier2 - vl_pr;
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
 	strcpy(vl_loc.l06,vl_str2);
  	vl_loc.l06[vl_i+1]='\0';
  
 
  	vl_loc.l09=vl_entier3;
  	vl_loc.l04=vl_entier4;
  	vl_loc.l03 = C_l03;
  
  	strcpy(vl_loc.l05,"P");
  	strcpy(vl_loc.l07,tm_l07_p[vl_autoroute]);
  	
  	vl_loc.l12 = vl_pr - vl_entier1;
  	vl_loc.l11 = vl_entier2 - vl_pr;

  	strcpy(vl_loc.l05,"U");
  	strcpy(vl_loc.l07,C_l07_U);
        sprintf (vl_bloc.Localisation,"");
	sprintf(vl_bloc.nbvoiesop,"");
	}
  
  

  vl_loc.l08=0;
  
  XZST_03EcritureTrace(XZSTC_INFO,"ases_evt_genere_strada: pr 1: %d   pr 2: %d",vl_loc.l11,vl_loc.l12);
XZST_03EcritureTrace(XZSTC_INFO,"ases_evt_genere_strada: %s , %s , %d , %d , %s - %s,\n %s - %s , %d ",
vl_loc.l01,vl_loc.l02,vl_loc.l04,vl_loc.l09,vl_loc.l06,vl_str1,vl_loc.l10,vl_str2,vl_loc.l03);
  vl_loc.l06[35]=(char) 0;
  vl_loc.l07[35]=(char) 0;
  vl_loc.l10[35]=(char) 0;
	
  sprintf(vl_bloc.loc.l01,"L01#%s#\n",vl_loc.l01);
  sprintf(vl_bloc.loc.l02,"L02#%s#\n",vl_loc.l02);
  sprintf(vl_bloc.loc.l03,"L03#%d#\n",vl_loc.l03);
  sprintf(vl_bloc.loc.l04,"L04#%d#\n",vl_loc.l04);
  sprintf(vl_bloc.loc.l05,"L05#%s#\n",vl_loc.l05);
  sprintf(vl_bloc.loc.l06,"L06#%s#\n",vl_loc.l06);
  sprintf(vl_bloc.loc.l07,"L07#%s#\n",vl_loc.l07);
  sprintf(vl_bloc.loc.l08,"L08#%d#\n",vl_loc.l08);
  sprintf(vl_bloc.loc.l09,"L09#%d#\n",vl_loc.l09);
  sprintf(vl_bloc.loc.l10,"L10#%s#\n",vl_loc.l10);
  if (vl_loc.l11<0) vl_loc.l11=0;
  if (vl_loc.l12<0) vl_loc.l12=0;
  sprintf(vl_bloc.loc.l11,"L11#%d#\n",vl_loc.l11);
  
  sprintf(vl_bloc.loc.l12,"L12#%d#\n",vl_loc.l12);
   
XZST_03EcritureTrace(XZSTC_INFO,"ases_evt_genere_strada: l11: %s - l12 : %s",vl_bloc.loc.l11,vl_bloc.loc.l12);

  vl_bloc.Bloc_Optio.Voies[0]='\0';

  vl_bloc.Bloc_Optio.Fin=vl_fin_flag[0];
  vl_bloc.Bloc_Optio.Prevision=vl_prevision[0];
  vl_bloc.Bloc_Optio.Fausse_Alerte=vl_fausse_alerte[0];
  
  sprintf(vl_bloc.Localisation,"%s",vl_localisation);
  sprintf(vl_bloc.nbvoiesop,"NLN#%d#\n",vl_nbvoiesop);
XZST_03EcritureTrace(XZSTC_INFO,"ases_evt_genere_strada: vl_sens=%d LNP=%s",vl_sens,vl_bloc.Localisation);
  if (vl_type_loc==XDC_POINT_CARACT_AIRE)
  {
  /* Localisation sur Bretelle */
  if (vl_sens == XDC_SENS_ENTREE_SUD) 
	sprintf(vl_bloc.Localisation,"R#\nRDI#P");
  if (vl_sens == XDC_SENS_ENTREE_NORD) 
	sprintf(vl_bloc.Localisation,"R#\nRDI#N");
  if (vl_sens == XDC_SENS_SORTIE_SUD) 
	sprintf(vl_bloc.Localisation,"R#\nRDI#P");
  if (vl_sens == XDC_SENS_SORTIE_NORD) 
	sprintf(vl_bloc.Localisation,"R#\nRDI#N");
  }
  else
   if (vl_type_loc==XDC_POINT_CARACT_PEAGE)
  {
  /* Localisation sur Bretelle */
  if (vl_sens == XDC_SENS_ENTREE_SUD) 
	sprintf(vl_bloc.Localisation,"T#\nRDI#P");
  if (vl_sens == XDC_SENS_ENTREE_NORD) 
	sprintf(vl_bloc.Localisation,"T#\nRDI#N");
  if (vl_sens == XDC_SENS_SORTIE_SUD) 
	sprintf(vl_bloc.Localisation,"T#\nRDI#P");
  if (vl_sens == XDC_SENS_SORTIE_NORD) 
	sprintf(vl_bloc.Localisation,"T#\nRDI#N");
  }
  else
  {
  /* Localisation sur Bretelle */
  if (vl_sens == XDC_SENS_ENTREE_SUD) 
	sprintf(vl_bloc.Localisation,"I#\nRDI#P");
  if (vl_sens == XDC_SENS_ENTREE_NORD) 
	sprintf(vl_bloc.Localisation,"I#\nRDI#N");
  if (vl_sens == XDC_SENS_SORTIE_SUD) 
	sprintf(vl_bloc.Localisation,"O#\nRDI#P");
  if (vl_sens == XDC_SENS_SORTIE_NORD) 
	sprintf(vl_bloc.Localisation,"O#\nRDI#N");
  }

  if (((vl_sens == XDC_SENS_ENTREE_SUD) || (vl_sens == XDC_SENS_ENTREE_NORD) || (vl_sens == XDC_SENS_SORTIE_SUD) || (vl_sens == XDC_SENS_SORTIE_NORD))  &&  ((vl_loc.l11<100) || (vl_loc.l12<100)))
  {	/* si sur echang les 2 pt sont un seul */
	if (vl_loc.l11<vl_loc.l12)
		{
  		sprintf(vl_bloc.loc.l12,"L12#%d#\n",0);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",0);
		sprintf(vl_bloc.loc.l10,"L10#%s#\n",vl_loc.l06);				
  		sprintf(vl_bloc.loc.l09,"L09#%d#\n",vl_loc.l04);
		}
	else
		{
  		sprintf(vl_bloc.loc.l12,"L12#%d#\n",0);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",0);
		sprintf(vl_bloc.loc.l06,"L06#%s#\n",vl_loc.l10);				
  		sprintf(vl_bloc.loc.l04,"L04#%d#\n",vl_loc.l09);
		}
		
	XZST_03EcritureTrace(XZSTC_INFO,"ases_evt_genere_strada: l11: %s - l12 : %s",vl_bloc.loc.l11,vl_bloc.loc.l12);
  }

  XZSM_11ConversionHorodate(vl_debut,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
			&vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: format date incorrect");
    return (XDC_NOK);
  }
  sprintf(vl_bloc.Bloc_Optio.Date_Debut,"STA#%04d%02d%02d%02d%02d%s#\n",
			vl_annee,
			vl_mois,
			vl_jour,
			vl_heure,
			vl_minute,
			heurehiversete(vl_jour,vl_mois,vl_annee) );

  /* S'il y a une date de fin alors on termine l'action exporter */
  if ( vl_fin != 0.0 )
  {	XZST_03EcritureTrace(XZSTC_INFO,"Evt fini!");
  	vl_type_horodate=XDC_ACT_ETAT_FIN;
  }
  	
  /* si la date de fin pr�vue est renseign�e alors elle est stock�e en STO */
  XZSM_11ConversionHorodate(vl_fin_prevue,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: format date incorrect");
    return (XDC_NOK);
  }
  if (vl_annee!=1970)
    sprintf(vl_bloc.Bloc_Optio.Date_Fin,"STO#%04d%02d%02d%02d%02d%s#\n",
                        vl_annee,
                        vl_mois,
                        vl_jour,
                        vl_heure,
                        vl_minute,
			heurehiversete(vl_jour,vl_mois,vl_annee));
  else
    strcpy(vl_bloc.Bloc_Optio.Date_Fin,"");

  XZSM_11ConversionHorodate(vl_fin,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: format date incorrect");
    return (XDC_NOK);
  }
  if (vl_annee!=1970)
    sprintf(vl_bloc.Bloc_Optio.Date_Fin,"STO#%04d%02d%02d%02d%02d%s#\n",
                        vl_annee,
                        vl_mois,
                        vl_jour,
                        vl_heure,
                        vl_minute,
			heurehiversete(vl_jour,vl_mois,vl_annee));


  XZSM_11ConversionHorodate(vl_validation,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute,
                        &vl_seconde ) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: format date incorrect");
    return (XDC_NOK);
  }
  sprintf(vl_bloc.Bloc_Oblig.INP,"INP#%04d%02d%02d%02d%02d%s#\n",
                        vl_annee,
                        vl_mois,
                        vl_jour,
                        vl_heure,
                        vl_minute,
			heurehiversete(vl_jour,vl_mois,vl_annee));
  XZST_03EcritureTrace(XZSTC_INFO,"%s %s %c %c - %d %c %s",
			vl_bloc.Bloc_Optio.Date_Debut,
			vl_bloc.Bloc_Optio.Date_Fin,
			vl_bloc.Bloc_Optio.Fausse_Alerte,
			vl_bloc.Bloc_Optio.Fin,
			vl_bloc.Bloc_Optio.Veracite,
			vl_bloc.Bloc_Optio.Prevision,
			vl_bloc.Localisation);

  vl_bloc.Bloc_Optio.Duree=vl_duree/60;
  XZST_03EcritureTrace(XZSTC_INFO,"duree=%d minutes",vl_bloc.Bloc_Optio.Duree);
 
  vl_bloc.Bloc_Optio.Trafic=vl_trafic;
  XZST_03EcritureTrace(XZSTC_INFO,"trafic=%d .",vl_bloc.Bloc_Optio.Trafic);
  
  sprintf (vl_bloc.Bloc_Optio.Comment,"SUR##\n");


		    	
  /*suivant le type d'evt, recherche les infos specifiques*/
  switch(va_fmc.Type) {
    case XZAEC_FMC_Accident: 
    	vl_veh_esc = -1;
    	vl_hom_esc = -1;
    	vl_perso   = -1;
    	
	if (XZAE129_Lire_Fiche_Accident_Pour_Strada(va_fmc.Numero
						    ,va_fmc.Cle
						    ,&vl_octet1
						    ,&vl_octet5
						    ,&vl_octet2
						    ,&vl_octet6
						    ,&vl_octet3
						    ,&vl_octet4
						    ,&vl_bool1
						    ,&vl_veh_esc
						    ,&vl_hom_esc
						    ,&vl_perso
						    ,&vl_blesses_graves) ==XDC_OK) {

  		/* Renseignement pour PX-METACOR */
  		
		sprintf(vl_bloc_metacor.Bloc_Oblig.Type,"INC");
		vl_bloc_metacor.Bloc_Optio.Bloc.Incident.VL=vl_octet1-vl_octet5;
		vl_bloc_metacor.Bloc_Optio.Bloc.Incident.PL=vl_octet5;
		vl_bloc_metacor.Bloc_Optio.Bloc.Incident.Blesses_graves=vl_blesses_graves+vl_octet4; /*Blesses_graves=Blesses_graves+morts*/


		vl_bloc.Bloc_Optio.Bloc.Accident.Vehicules=vl_octet1;
		vl_bloc.Bloc_Optio.Bloc.Accident.PL=vl_octet5;
		vl_bloc.Bloc_Optio.Bloc.Accident.Cars=vl_octet2;
		vl_bloc.Bloc_Optio.Bloc.Accident.Rem=vl_octet6;
		vl_bloc.Bloc_Optio.Bloc.Accident.Blesses=vl_octet3;
		vl_bloc.Bloc_Optio.Bloc.Accident.Morts=vl_octet4;
		vl_bloc.Bloc_Optio.Bloc.Accident.Matiere=vl_bool1;
		XZST_03EcritureTrace(XZSTC_INFO,"XZAE129_Lire_Fiche_Accident_Pour_Strada :%d %d %d %d %d %d",
					vl_bloc.Bloc_Optio.Bloc.Accident.Vehicules,
					vl_bloc.Bloc_Optio.Bloc.Accident.PL,
					vl_bloc.Bloc_Optio.Bloc.Accident.Cars,
					vl_bloc.Bloc_Optio.Bloc.Accident.Rem,
					vl_bloc.Bloc_Optio.Bloc.Accident.Blesses,
					vl_bloc.Bloc_Optio.Bloc.Accident.Morts);

/* suppression de la transmission des commentaires 
		tl_string_comment[0]='\0';
		
		if (vl_veh_esc == 1) 
			strcat (tl_string_comment,"vehicule(s) ESCOTA implique(s), ");
		
		if (vl_hom_esc == 1) 
			strcat (tl_string_comment,"personne(s) ESCOTA impliquee(s), ");
		
		if (vl_perso == 1) 
			strcat (tl_string_comment,"personalites impliquee(s)");
		
		sprintf (vl_bloc.Bloc_Optio.Comment,"SUR#%s#\n",tl_string_comment);
		*********************************************************************/
		break;
	}
	break;

    case XZAEC_FMC_Delestage :
    	if (XZAE131_Lire_Fiche_Delestage_Pour_Strada(va_fmc.Numero
						    ,va_fmc.Cle
						    ,&vl_mot1
						    ,tl_string_comment) ==XDC_OK) {
/* suppression de la transmission des commentaires 
		sprintf (vl_bloc.Bloc_Optio.Comment,"SUR#plan numero %d , %s#\n",
					vl_mot1,tl_string_comment);  
********************************************/
		sprintf(vl_bloc_metacor.Bloc_Oblig.Type,"SC");
		
		}
        sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#RRI#\n");
		if (vl_loc.l11<vl_loc.l12)
		{
  		sprintf(vl_bloc.loc.l12,"L12#%d#\n",0);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",0);
		sprintf(vl_bloc.loc.l10,"L10#%s#\n",vl_loc.l06);				
  		sprintf(vl_bloc.loc.l09,"L09#%d#\n",vl_loc.l04);
		}
		else
		{
  		sprintf(vl_bloc.loc.l12,"L12#%d#\n",0);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",0);
		sprintf(vl_bloc.loc.l06,"L06#%s#\n",vl_loc.l10);				
  		sprintf(vl_bloc.loc.l04,"L04#%d#\n",vl_loc.l09);
		}

	break;

    case XZAEC_FMC_Deviation :
    	if (XZAE131_Lire_Fiche_Delestage_Pour_Strada(va_fmc.Numero
						    ,va_fmc.Cle
						    ,&vl_mot1
						    ,tl_string_comment) ==XDC_OK) {
/* suppression de la transmission des commentaires 
		sprintf (vl_bloc.Bloc_Optio.Comment,"SUR#plan numero %d , %s#\n",
					vl_mot1,tl_string_comment);  
***************************************************/
		
		sprintf(vl_bloc_metacor.Bloc_Oblig.Type,"SO");
		
	}
        sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#DO#\n");
		if (vl_loc.l11<vl_loc.l12)
		{
  		sprintf(vl_bloc.loc.l12,"L12#%d#\n",0);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",0);
		sprintf(vl_bloc.loc.l10,"L10#%s#\n",vl_loc.l06);				
  		sprintf(vl_bloc.loc.l09,"L09#%d#\n",vl_loc.l04);
		}
		else
		{
  		sprintf(vl_bloc.loc.l12,"L12#%d#\n",0);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",0);
		sprintf(vl_bloc.loc.l06,"L06#%s#\n",vl_loc.l10);				
  		sprintf(vl_bloc.loc.l04,"L04#%d#\n",vl_loc.l09);
		}

	break;

    case XZAEC_FMC_TraitChaussees :/* PX-METACOR */
		vl_a_traiter_metacor=XDC_FAUX;
    	vl_bloc.Bloc_Optio.Bloc.Meteo.Visibilite = -1;
    	tl_string[0]='\0';
    	tl_string_comment[0]='\0';

	if (  XZAE135_Lire_Fiche_Meteo_Pour_Strada(va_fmc.Numero
						    ,va_fmc.Cle
						    ,&vl_entier1
						    ,&vl_mot1
						    ,&vl_octet1) == XDC_OK) {
		vl_bloc.Bloc_Optio.Bloc.Meteo.Longueur=abs(vl_entier1-vl_pr);	
		vl_bloc.Bloc_Optio.Bloc.Meteo.Visibilite=vl_mot1;

		switch (vl_octet1) {
			case XZAEC_TRT_CH_Salage :
				strcat (tl_string_comment,"Salage.");
  				sprintf(vl_bloc.Bloc_Oblig.DOB,"DOB#OPA#\n");
				sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#SAL#\n");
				break;
		
			case XZAEC_TRT_CH_Deneigement :
				strcat (tl_string_comment,"Deneigement.");
				sprintf(vl_bloc.Bloc_Oblig.DOB,"DOB#OPA#\n");
				sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#SN#\n");
				break;
			default :
				strcat (tl_string_comment,"Autre.");
				break;
		}
		sprintf (vl_bloc.Bloc_Optio.Comment,"SUR#%s#\n",tl_string_comment);
		vl_pr=vl_entier1;
  		if (XZAE146_Recherche_Loc_Rds(vl_pr,
  				vl_autoroute,
  				vl_loc.l01,
  				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,vl_str3 ) != XDC_OK) {
    			XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_fmc.Cle);
    			return (XDC_NOK);
  			}

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

  			strcpy(vl_loc.l05,"U");
  			strcpy(vl_loc.l07,C_l07_U);
  		}
  
     		if (vl_loc.l11<0) vl_loc.l11=0;
  		sprintf(vl_bloc.loc.l04,"L04#%d#\n",vl_loc.l04);
  		sprintf(vl_bloc.loc.l06,"L06#%s#\n",vl_loc.l06);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",vl_loc.l11);
		  
		/* FMC Non traitee dans PX METACOR */
		vl_a_traiter_metacor=XDC_FAUX;

		break;
	}
	break;

    case XZAEC_FMC_Meteo:
   		/* Renseignement pour PX-METACOR */
  	vl_a_traiter_metacor=XDC_VRAI;
    
    	vl_bloc.Bloc_Optio.Bloc.Meteo.Visibilite = -1;
    	tl_string[0]='\0';
    	tl_string_comment[0]='\0';

	if (  XZAE135_Lire_Fiche_Meteo_Pour_Strada(va_fmc.Numero
						    ,va_fmc.Cle
						    ,&vl_entier1
						    ,&vl_mot1
						    ,&vl_octet1) == XDC_OK) {
		vl_bloc.Bloc_Optio.Bloc.Meteo.Longueur=abs(vl_entier1-vl_pr);	
		vl_bloc.Bloc_Optio.Bloc.Meteo.Visibilite=vl_mot1;
  		strcpy(vl_loc.l05,"B");
  		strcpy(vl_loc.l07,C_l07_B);

		switch (vl_octet1) {
			case XZAEC_MET_ChuteNeige :
				strcat (tl_string_comment,"Chute de Neige");
  				sprintf(vl_bloc.Bloc_Oblig.DOB,"DOB#SNO#\n");
				sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#SRO#\n");
				break;
		
			case XZAEC_MET_NeigeChaus :
				strcat (tl_string_comment,"Neige sur Chaussee");
				sprintf(vl_bloc.Bloc_Oblig.DOB,"DOB#SNO#\n");
				sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#SRO#\n");
				break;
				
			case XZAEC_MET_GreleChaus :
				strcat (tl_string_comment,"Grele sur Chaussee");
				sprintf(vl_bloc.Bloc_Oblig.DOB,"DOB#SNO#\n");
				sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#SRO#\n");
				break;
			
			case XZAEC_MET_Pluie :
				strcat (tl_string_comment,"Pluie");
				sprintf(vl_bloc.Bloc_Oblig.DOB,"DOB#PRE#\n");
				sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#RAI#\n");
				break;
				
			case XZAEC_MET_VentFort :
				strcat (tl_string_comment,"Vent fort");
				sprintf(vl_bloc.Bloc_Oblig.DOB,"DOB#WIN#\n");
				sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#GAL#\n");
				break;
				
			case XZAEC_MET_Brouillard :
				strcat (tl_string_comment,"Brouillard");
				sprintf(vl_bloc.Bloc_Oblig.DOB,"DOB#FOS#\n");
				if (vl_bloc.Bloc_Optio.Bloc.Meteo.Visibilite<=50)
					sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#FOD#\n");
				else
					sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#FOG#\n");
				break;
				
			case XZAEC_MET_BrouillardGiv :
				strcat (tl_string_comment,"Brouillard Givrant");
				sprintf(vl_bloc.Bloc_Oblig.DOB,"DOB#FOS#\n");
				sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#FOF#\n");
				break;
				
			case XZAEC_MET_Grele :
				strcat (tl_string_comment,"Grele");
				sprintf(vl_bloc.Bloc_Oblig.DOB,"DOB#PRE#\n");
				sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#HAD#\n");
				break;
				
			case XZAEC_MET_Verglas :
				strcat (tl_string_comment,"Verglas");
				sprintf(vl_bloc.Bloc_Oblig.DOB,"DOB#SHZ#\n");
				sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#IBU#\n");
				break;
				
			case XZAEC_MET_Autre :
				sprintf(vl_bloc.Bloc_Oblig.DOB,"DOB#WDA#\n");
				sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#WDA#\n");
				sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#WDA#\n");
				break;
				
			default :
				sprintf(vl_bloc.Bloc_Oblig.DOB,"DOB#WDA#\n");
				sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#WDA#\n");
				sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#WDA#\n");
				break;
		}
		sprintf (vl_bloc.Bloc_Optio.Comment,"SUR#%s#\n",tl_string_comment);
		vl_pr=vl_entier1;
  		if (XZAE146_Recherche_Loc_Rds(vl_pr,
  				vl_autoroute,
  				vl_loc.l01,
  				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,vl_str3 ) != XDC_OK) {
    			XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_fmc.Cle);
    			return (XDC_NOK);
  			}

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

  			strcpy(vl_loc.l05,"B");
  			strcpy(vl_loc.l07,C_l07_B);
  		}
  
     		if (vl_loc.l11<0) vl_loc.l11=0;
  		sprintf(vl_bloc.loc.l04,"L04#%d#\n",vl_loc.l04);
  		sprintf(vl_bloc.loc.l06,"L06#%s#\n",vl_loc.l06);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",vl_loc.l11);
		  
		/* FMC Non traitee dans PX METACOR */
		vl_a_traiter_metacor=XDC_FAUX;

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
    		
	if (  XZAE140_Lire_Fiche_Vehicule_Pour_Strada(va_fmc.Numero
						    	,va_fmc.Cle
						    	,&vl_octet1
						    	,&vl_mot3
			   				,tl_matiere
			   				,&vl_mot1
			   				,&vl_mot2
			   				,&vl_bool1
			   				,&vl_bool2
			   				,&tl_observations) ==XDC_OK) {
  		/* Renseignement pour PX-METACOR */
  		
		sprintf(vl_bloc_metacor.Bloc_Oblig.Type,"INC");
		vl_bloc_metacor.Bloc_Optio.Bloc.Incident.VL=0;
		vl_bloc_metacor.Bloc_Optio.Bloc.Incident.PL=0;
		vl_bloc_metacor.Bloc_Optio.Bloc.Incident.Blesses_graves=0;

		switch (va_fmc.Type) {
    			case XZAEC_FMC_Contresens:
				vl_a_traiter_metacor=XDC_FAUX;
				break;
    			case XZAEC_FMC_VehArrete:
    			case XZAEC_FMC_VehPanne:
    			case XZAEC_FMC_VehFeu:
				break;
		}

		tl_string_comment[0]='\0';
		switch (vl_octet1) {
		  case 1 : strcpy(tl_string_comment," PL ");
			   strcpy(vl_bloc.Bloc_Optio.Bloc.Vehicule.Type,"VCL#KX2#\n");
			   vl_bloc_metacor.Bloc_Optio.Bloc.Incident.PL=1;
			   break;

		  case 2 : strcpy(tl_string_comment," VL ");
			   strcpy(vl_bloc.Bloc_Optio.Bloc.Vehicule.Type,"VCL#KX1#\n");
			   vl_bloc_metacor.Bloc_Optio.Bloc.Incident.VL=1;
			   break;

		  case 3 : strcpy(tl_string_comment," Car ");
			   strcpy(vl_bloc.Bloc_Optio.Bloc.Vehicule.Type,"VCL#KX2#\n");
			   vl_bloc_metacor.Bloc_Optio.Bloc.Incident.PL=1;
			   break;

		  case 4 : strcpy(tl_string_comment," Moto ");
			   vl_bloc_metacor.Bloc_Optio.Bloc.Incident.VL=1;
			   strcpy(vl_bloc.Bloc_Optio.Bloc.Vehicule.Type,"VCL#KX1#\n");
			   break;

                  case 5 : strcpy(tl_string_comment," Remorque/caravane ");
			   vl_bloc_metacor.Bloc_Optio.Bloc.Incident.VL=1;
			   strcpy(vl_bloc.Bloc_Optio.Bloc.Vehicule.Type,"VCL#KX1#\n");
			   break;

		  default: strcpy(vl_bloc.Bloc_Optio.Bloc.Vehicule.Type,"");
			   vl_bloc_metacor.Bloc_Optio.Bloc.Incident.VL=1;
			   break;
	        }
		if ( tl_matiere[0] != '\0' )
			sprintf(vl_bloc.Bloc_Optio.Bloc.Vehicule.Matiere,"AAD#%s#\n",tl_matiere);
		else
			 sprintf(vl_bloc.Bloc_Optio.Bloc.Vehicule.Matiere,"");
		vl_bloc.Bloc_Optio.Bloc.Vehicule.Volume=vl_mot3;
		tl_string[0]='\0';
		
/* suppression de la transmission des commentaires 
		if (vl_mot1 > 0) {
			sprintf ( tl_string , "%d bebes implique(s)", vl_mot1);
			strcat (tl_string_comment,tl_string);
		}
		
		if (vl_mot2 > 0) {
			sprintf (tl_string, ", %d personne(s) agees impliquee(s)", vl_mot2);
			strcat (tl_string_comment,tl_string);
		}
		
		if ( vl_bool1== 1) 
			strcat (tl_string_comment,", presence d'animaux");
		
		if ( vl_bool2== 1) 
			strcat (tl_string_comment,", personne(s) d'Escota impliquee(s),");
*******************************************************/		
		strcat (tl_string_comment,tl_observations);
		
		sprintf (vl_bloc.Bloc_Optio.Comment,"SUR#%s#\n",tl_string_comment);
					    
		break;	
	}					 
	break;


    case XZAEC_FMC_Cirque:
    case XZAEC_FMC_ConvoiExc:
    case XZAEC_FMC_ConvoiMil:
    case XZAEC_FMC_VehLent:

		/* FMC Non traitee dans PX METACOR */
		vl_a_traiter_metacor=XDC_FAUX;

	if ( XZAE132_Lire_Fiche_VehLent_Pour_Strada(va_fmc.Numero
						    	,va_fmc.Cle
						    	,&vl_mot1
						    	,&vl_octet1) == XDC_OK) {
		vl_bloc.Bloc_Optio.Bloc.VehicLent.Longueur=vl_mot1;
		vl_bloc.Bloc_Optio.Bloc.VehicLent.Vitesse=vl_octet1;
		break;
	}
	break;

    case XZAEC_FMC_Manifestation:
    case XZAEC_FMC_GreveESCOTA:
    case XZAEC_FMC_Escargot:
    
		/* FMC Non traitee dans PX METACOR */
		vl_a_traiter_metacor=XDC_FAUX;

    		vl_nombre 	= -1;
    		vl_perception	= -1;
    		vl_violence	= -1;
    
	if ( XZAE134_Lire_Fiche_Manif_Pour_Strada(va_fmc.Numero
						    	,va_fmc.Cle
						    	,&vl_mot1
						    	,&vl_perception
						    	,&vl_violence) ==XDC_OK) {
						    	
		tl_string_comment[0]='\0';
/* suppression de la transmission des commentaires 
		sprintf (tl_string, "nombre de personne(s): %d ", vl_mot1);
		strcat (tl_string_comment,tl_string);
		
		if (vl_perception == XZAEC_FMC_PAS_PER) 
			strcat (tl_string_comment,", pas de perception ");
		else 
			strcat (tl_string_comment,", perception maintenue ");
		
		if (vl_violence == XDC_VRAI) 
			strcat (tl_string_comment,", violence ");
		else
			strcat (tl_string_comment,", pas de violence ");
		
		sprintf(vl_bloc.Bloc_Optio.Comment,"SUR#%s#\n",tl_string_comment);
			***************************************************************/			    	
		break;
	}
	break;

    case XZAEC_FMC_Basculement:
	if ( XZAE138_Lire_Fiche_Basculement_Pour_Strada(va_fmc.Numero
						    	,va_fmc.Cle
						    	,&vl_entier1
						    	,&vl_entier2,
							&vl_type_loc,
							vl_nom_loca,
							&vl_id_loca,
							vl_Config_Voies,
							vl_Config_Voies_suite) == XDC_OK) {

		XZST_03EcritureTrace(XZSTC_INFO,"restriction= %s.%s;nom=%s;type=%d;long=%d,pr=%d",vl_Config_Voies,vl_Config_Voies_suite,  vl_nom_loca,vl_type_loc,vl_entier1,vl_entier2);
		vl_bloc.Bloc_Optio.Bloc.Basculement.Longueur=vl_entier1;
		vl_pr=vl_entier2;
		/* PX-METACOR */
  		
		sprintf(vl_bloc_metacor.Bloc_Oblig.Type,"RES");
  		sprintf(vl_bloc_metacor.Bloc_Oblig.pk_fin,"%.3f",(float) vl_pr/1000);
  		sprintf(vl_bloc_metacor.Bloc_Oblig.pr_fin,"%.3f",(float) vl_pr/1000);
  		snprintf(vl_bloc_metacor.Bloc_Oblig.rest,512,"%s%s",vl_Config_Voies,vl_Config_Voies_suite);

/*		if (strcmp(vl_bloc_metacor.Bloc_Oblig.Loca_deb_type,"Brt Ech") || (vl_type_loc != 0))*/
		{
	  		switch (vl_type_loc)
	  		{
	  		  case 0 : 
	  	   		 sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_SECTION_COURANTE);
	  	   		 break;
			  case XDC_POINT_CARACT_AIRE  : 
  	   			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_AIRE);
				sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d",vl_id_loca);

  	   			break;
			  case  XDC_POINT_CARACT_ECHANGEUR : 
  	   			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_ECHANGEUR);
	   			if (( vl_sens== XDC_SENS_ENTREE_SUD) || ( vl_sens== XDC_SENS_ENTREE_NORD))  {
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s entree %d",vl_nom_loca,vl_sens_sc);
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d%d",vl_id_loca,vl_sens);
	   			}
	   			if (( vl_sens== XDC_SENS_SORTIE_SUD) || ( vl_sens== XDC_SENS_SORTIE_NORD))  {
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s sortie %d",vl_nom_loca,vl_sens_sc);
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d%d",vl_id_loca,vl_sens);
	   			}
			  case  XDC_POINT_CARACT_BIFURCATION : 
  	   			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_ECHANGEUR);
	   			if (( vl_sens== XDC_SENS_ENTREE_SUD) || ( vl_sens== XDC_SENS_ENTREE_NORD))  {
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s entree %d",vl_nom_loca,vl_sens_sc);
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d%d",vl_id_loca,vl_sens);
	   			}
	   			if (( vl_sens== XDC_SENS_SORTIE_SUD) || ( vl_sens== XDC_SENS_SORTIE_NORD))  {
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s sortie %d",vl_nom_loca,vl_sens_sc);
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d%d",vl_id_loca,vl_sens);
	   			}
				break;

			  default : 
			  	   sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_SECTION_COURANTE);
			  }
  			sprintf(vl_bloc_metacor.Bloc_Oblig.pk_fin,"%.3f",(float) vl_entier1/1000);
  			sprintf(vl_bloc_metacor.Bloc_Oblig.pr_fin,"%.3f",(float) vl_entier1/1000);
		}
/*		else
		{  Cas evt debut sur bretelle et fin sur section courante 
	  		sprintf(vl_bloc_metacor.Bloc_Oblig.pk_fin,"");
	  		sprintf(vl_bloc_metacor.Bloc_Oblig.pr_fin,"");
			sprintf(vl_bloc_metacor.Bloc_Oblig.sns_fin_id,"");
			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"");
			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,"");
		}*/		    	
				    	

  		if (XZAE146_Recherche_Loc_Rds(vl_pr,
  				vl_autoroute,
  				vl_loc.l01,
  				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,vl_str3 ) != XDC_OK) {
    			XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_fmc.Cle);
    			return (XDC_NOK);
  			}
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

  			strcpy(vl_loc.l05,"U");
  			strcpy(vl_loc.l07,C_l07_U);
  		}
  
      		if (vl_loc.l11<0) vl_loc.l11=0;
  		sprintf(vl_bloc.loc.l04,"L04#%d#\n",vl_loc.l04);
  		sprintf(vl_bloc.loc.l06,"L06#%s#\n",vl_loc.l06);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",vl_loc.l11);
		
		break;
	}

	break;
    case XZAEC_FMC_BasculTravaux:
    case XZAEC_FMC_Travaux:
    case XZAEC_FMC_ZoneStockage:
	tl_string[0]='\0';
	if ( XZAE137_Lire_Fiche_Travaux_Pour_Strada(va_fmc.Numero
						    	,va_fmc.Cle
						    	,&vl_bool2
						    	,tl_string
						    	,&vl_bool1
							,&vl_entier1,
							&vl_type_loc,
							vl_nom_loca,
							&vl_id_loca,
							vl_Config_Voies,
							vl_Config_Voies_suite) == XDC_OK) {
		/* PX-METACOR */
  		
		sprintf(vl_bloc_metacor.Bloc_Oblig.Type,"RES");
  		sprintf(vl_bloc_metacor.Bloc_Oblig.pk_fin,"%.3f",(float) vl_entier1/1000);
  		sprintf(vl_bloc_metacor.Bloc_Oblig.pr_fin,"%.3f",(float) vl_entier1/1000);
XZST_03EcritureTrace(XZSTC_INFO,"vl_Config_Voies=%s,vl_Config_Voies_suite=%s.",vl_Config_Voies,vl_Config_Voies_suite);
  		snprintf(vl_bloc_metacor.Bloc_Oblig.rest,512,"%s%s",vl_Config_Voies,vl_Config_Voies_suite);

/*		if (strcmp(vl_bloc_metacor.Bloc_Oblig.Loca_deb_type,"Brt Ech") || (vl_type_loc != 0))*/
		{
	  		switch (vl_type_loc)
	  		{
	  		  case 0 : 
	  	   		 sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_SECTION_COURANTE);
	  	   		 break;
			  case XDC_POINT_CARACT_AIRE  : 
  	   			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_AIRE);
				sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d",vl_id_loca);

  	   			break;
			  case  XDC_POINT_CARACT_ECHANGEUR : 
  	   			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_ECHANGEUR);
	   			if (( vl_sens== XDC_SENS_ENTREE_SUD) || ( vl_sens== XDC_SENS_ENTREE_NORD))  {
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s entree %d",vl_nom_loca,vl_sens_sc);
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d%d",vl_id_loca,vl_sens);
	   			}
	   			if (( vl_sens== XDC_SENS_SORTIE_SUD) || ( vl_sens== XDC_SENS_SORTIE_NORD))  {
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s sortie %d",vl_nom_loca,vl_sens_sc);
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d%d",vl_id_loca,vl_sens);
	   			}
			  case  XDC_POINT_CARACT_BIFURCATION : 
  	   			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_ECHANGEUR);
	   			if (( vl_sens== XDC_SENS_ENTREE_SUD) || ( vl_sens== XDC_SENS_ENTREE_NORD))  {
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s entree %d",vl_nom_loca,vl_sens_sc);
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d%d",vl_id_loca,vl_sens);
	   			}
	   			if (( vl_sens== XDC_SENS_SORTIE_SUD) || ( vl_sens== XDC_SENS_SORTIE_NORD))  {
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s sortie %d",vl_nom_loca,vl_sens_sc);
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d%d",vl_id_loca,vl_sens);
	   			}
				break;

			  default : 
			  	   sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_SECTION_COURANTE);
			  }
  			sprintf(vl_bloc_metacor.Bloc_Oblig.pk_fin,"%.3f",(float) vl_entier1/1000);
  			sprintf(vl_bloc_metacor.Bloc_Oblig.pr_fin,"%.3f",(float) vl_entier1/1000);
		}
/*		else
		{  Cas evt debut sur bretelle et fin sur section courante 
	  		sprintf(vl_bloc_metacor.Bloc_Oblig.pk_fin,"");
	  		sprintf(vl_bloc_metacor.Bloc_Oblig.pr_fin,"");
			sprintf(vl_bloc_metacor.Bloc_Oblig.sns_fin_id,"");
			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"");
			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,"");
		}*/		    	
				    	
		vl_bloc.Bloc_Optio.Bloc.Travaux.Longueur=abs(vl_entier1-vl_pr);
		
XZST_03EcritureTrace(XZSTC_WARNING,"entier1:%d;vl_pr:%d",vl_entier1,vl_pr);
		if (vl_bool1==1)
			vl_bloc.Bloc_Optio.Bloc.Travaux.Mobiles=(int)'M';
		else
			vl_bloc.Bloc_Optio.Bloc.Travaux.Mobiles=(int)'S';

		if (va_fmc.Type == XZAEC_FMC_ZoneStockage)
		{
			sprintf(vl_bloc.Bloc_Optio.Comment,"SUR#%s#","Zone de stockage ");
		}
		else
		{
			vl_i= strlen(tl_string)-1;
			while (vl_i > 0 && tl_string[vl_i]==' ' ) {
				vl_i--;
			}
			tl_string[vl_i+1]='\0';

			if (vl_bool2 == 1)
			  sprintf(vl_bloc.Bloc_Optio.Comment,"SUR#%s%s#\n","travaux neufs, ",tl_string);
			else
			  sprintf(vl_bloc.Bloc_Optio.Comment,"SUR#%s%s#\n","travaux d'entretien, ",tl_string);
		}
		
		vl_pr=vl_entier1;
  		if (XZAE146_Recherche_Loc_Rds(vl_pr,
  				vl_autoroute,
  				vl_loc.l01,
  				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,vl_str3 ) != XDC_OK) {
    			XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_fmc.Cle);
    			return (XDC_NOK);
  			}



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

  			strcpy(vl_loc.l05,"U");
  			strcpy(vl_loc.l07,C_l07_U);
  		}
  
      		if (vl_loc.l11<0) vl_loc.l11=0;
  		sprintf(vl_bloc.loc.l04,"L04#%d#\n",vl_loc.l04);
  		sprintf(vl_bloc.loc.l06,"L06#%s#\n",vl_loc.l06);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",vl_loc.l11);
		
		break;
	}
	break;

    case XZAEC_FMC_IncSsConces:

		if (vl_loc.l11<vl_loc.l12)
		{
  		sprintf(vl_bloc.loc.l12,"L12#%d#\n",0);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",0);
		sprintf(vl_bloc.loc.l10,"L10#%s#\n",vl_loc.l06);				
  		sprintf(vl_bloc.loc.l09,"L09#%d#\n",vl_loc.l04);
		}
		else
		{
  		sprintf(vl_bloc.loc.l12,"L12#%d#\n",0);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",0);
		sprintf(vl_bloc.loc.l06,"L06#%s#\n",vl_loc.l10);				
  		sprintf(vl_bloc.loc.l04,"L04#%d#\n",vl_loc.l09);
		}
    
		/* FMC Non traitee dans PX METACOR */
		vl_a_traiter_metacor=XDC_FAUX;

	vl_bool1=0;
	vl_bool2=0;
	if ( XZAE133_Lire_Fiche_SousCnc_Pour_Strada(va_fmc.Numero
						    	,va_fmc.Cle
						    	,&tl_nom
						    	,&vl_bool1
						    	,&vl_bool2) == XDC_OK) {
		
		sprintf (tl_string, "Concessionnaire %s ",tl_nom);
						    	
		
		if (vl_bool1 == 1) {
		
			strcat (tl_string, ", ferme");
		}
		if (vl_bool2 == 1) {
		
			strcat (tl_string, ", penurie de carburant");
		}
		
		sprintf (vl_bloc.Bloc_Optio.Comment,"SUR#%s#\n",tl_string);	
	    	sprintf (vl_bloc.Localisation,"");
		sprintf(vl_bloc.nbvoiesop,"");
		break;
	}
	break;

    case XZAEC_FMC_QueueBouchon:
    
    	vl_type = -1;
    
	if ( XZAE136_Lire_Fiche_Bouchon_Pour_Strada(va_fmc.Numero
						    	,va_fmc.Cle
						    	,&vl_mot1
						    	,&vl_entier1
						    	,&vl_autoroute
						    	,&vl_pr
						    	,&vl_sens
						    	,&vl_num_cause
							,&vl_cle_cause,
							&vl_type_loc,
							vl_nom_loca,
							&vl_id_loca) == XDC_OK) {

		/* PX-METACOR */
		sprintf(vl_bloc_metacor.Bloc_Oblig.Type,"BOU");
  		
		if (strcmp(vl_bloc_metacor.Bloc_Oblig.Loca_deb_type,"Brt Ech") || (vl_type_loc != 0))
		{
	  		switch (vl_type_loc)
	  		{
	  		  case 0 : 
	  	   		 sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_SECTION_COURANTE);
	  	   		 break;
			  case XDC_POINT_CARACT_AIRE  : 
  	   			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_AIRE);
  	   			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_type,C_AIRE);
					/* On ecrase la queue avec la tete dans le ca ou la tete est sur bretelle */
				sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d",vl_id_loca);
				sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_id,"%d",vl_id_loca);
  	   			break;
			  case  XDC_POINT_CARACT_ECHANGEUR : 
  	   			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_ECHANGEUR);
  	   			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_type,C_ECHANGEUR);
	   			if (( vl_sens== XDC_SENS_ENTREE_SUD) || ( vl_sens== XDC_SENS_ENTREE_NORD))  {
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s entree %d",vl_nom_loca,vl_sens_sc);
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d%d",vl_id_loca,vl_sens);
					/* On ecrase la queue avec la tete dans le ca ou la tete est sur bretelle */
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_nom,"%s sortie %d",vl_nom_loca,vl_sens_sc);
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_id,"%d%d",vl_id_loca,vl_sens);
	   			}
	   			if (( vl_sens== XDC_SENS_SORTIE_SUD) || ( vl_sens== XDC_SENS_SORTIE_NORD))  {
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s sortie %d",vl_nom_loca,vl_sens_sc);
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d%d",vl_id_loca,vl_sens);
					/* On ecrase la queue avec la tete dans le ca ou la tete est sur bretelle */
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_nom,"%s sortie %d",vl_nom_loca,vl_sens_sc);
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_id,"%d%d",vl_id_loca,vl_sens);
	   			}
			  case  XDC_POINT_CARACT_BIFURCATION : 
  	   			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_ECHANGEUR);
  	   			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_type,C_ECHANGEUR);
	   			if (( vl_sens== XDC_SENS_ENTREE_SUD) || ( vl_sens== XDC_SENS_ENTREE_NORD))  {
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s entree %d",vl_nom_loca,vl_sens_sc);
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d%d",vl_id_loca,vl_sens);
					/* On ecrase la queue avec la tete dans le ca ou la tete est sur bretelle */
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_nom,"%s sortie %d",vl_nom_loca,vl_sens_sc);
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_id,"%d%d",vl_id_loca,vl_sens);
	   			}
	   			if (( vl_sens== XDC_SENS_SORTIE_SUD) || ( vl_sens== XDC_SENS_SORTIE_NORD))  {
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"%s sortie %d",vl_nom_loca,vl_sens_sc);
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_id,"%d%d",vl_id_loca,vl_sens);
					/* On ecrase la queue avec la tete dans le ca ou la tete est sur bretelle */
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_nom,"%s sortie %d",vl_nom_loca,vl_sens_sc);
					sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_deb_id,"%d%d",vl_id_loca,vl_sens);
	   			}
				break;
			  default : 
			  	   sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,C_SECTION_COURANTE);
			  }
		

			switch (vl_sens)
			  {
			  case XDC_SENS_ENTREE_SUD :
			  case XDC_SENS_SORTIE_SUD :
			  case XDC_SENS_SUD : 	
			  	   		sprintf(vl_bloc_metacor.Bloc_Oblig.sns_fin_id,"1");
			  	   		break;
			  case XDC_SENS_ENTREE_NORD :
			  case XDC_SENS_SORTIE_NORD :
			  case XDC_SENS_NORD : 	
			  	   		sprintf(vl_bloc_metacor.Bloc_Oblig.sns_fin_id,"2");
			  	   		break;
			  default :  	
		  	   	sprintf(vl_bloc_metacor.Bloc_Oblig.sns_fin_id,"1");
		  	   	break;
			  }
	  		sprintf(vl_bloc_metacor.Bloc_Oblig.pk_fin,"%.3f",(float) vl_pr/1000);
	  		sprintf(vl_bloc_metacor.Bloc_Oblig.pr_fin,"%.3f",(float) vl_pr/1000);
		}
		else
		{ /* Cas evt debut sur bretelle et fin sur section courante */
	  		sprintf(vl_bloc_metacor.Bloc_Oblig.pk_fin,"");
	  		sprintf(vl_bloc_metacor.Bloc_Oblig.pr_fin,"");
			sprintf(vl_bloc_metacor.Bloc_Oblig.sns_fin_id,"");
			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_nom,"");
			sprintf(vl_bloc_metacor.Bloc_Oblig.Loca_fin_type,"");
		}		    	
		tl_string[0]='\0';
		vl_bloc.Bloc_Optio.Bloc.Bouchon.Longueur=vl_entier1;
		if (vl_mot1 > 0) {
		
			vl_bloc.Bloc_Optio.Bloc.Bouchon.Type=6;
			switch (vl_mot1) {
			/* PHR par type de bouchon */
				case XZAEC_BOU_Dense : 
					strcpy (tl_string, "Trafic dense");
					vl_bloc.Bloc_Optio.Bloc.Bouchon.Type=5;
					sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#LS3#\n");
					break;	
				case XZAEC_BOU_Accordeon : 
					strcpy (tl_string, "Trafic en accordeon");
					vl_bloc.Bloc_Optio.Bloc.Bouchon.Type=4;
					sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#LS2#\n");
					break;	
				case XZAEC_BOU_Ralenti : 
					strcpy (tl_string, "Trafic ralenti");
					vl_bloc.Bloc_Optio.Bloc.Bouchon.Type=3;
					sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#LS3#\n");
					break;	
				case XZAEC_BOU_Bouche : 
					strcpy (tl_string, "Trafic bouche");
					vl_bloc.Bloc_Optio.Bloc.Bouchon.Type=2;
					sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#LS1#\n");
					break;	
				case XZAEC_BOU_Bloque : 
					strcpy (tl_string, "Trafic bloque");
					vl_bloc.Bloc_Optio.Bloc.Bouchon.Type=1;
					sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#LS1#\n");
					break;	
				case XZAEC_BOU_Escargot : 
					strcpy (tl_string, "Operation escargot");
					sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#LS3#\n");
					vl_bloc.Bloc_Optio.Bloc.Bouchon.Type=4;
					break;	
			}
		}		   
		sprintf (vl_bloc.Bloc_Optio.Comment,"SUR#%s#\n",tl_string);	
  	if (XZAE146_Recherche_Loc_Rds(vl_pr,
  				vl_autoroute,
   				vl_loc.l01,
 				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,vl_str3 ) != XDC_OK) {
    			XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_genere_strada: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_fmc.Cle);
    			return (XDC_NOK);
  		}
    XZST_03EcritureTrace(XZSTC_INFO,"ases_evt_genere_strada: XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_fmc.Cle);


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

  			strcpy(vl_loc.l05,"U");
  			strcpy(vl_loc.l07,C_l07_U);
  		}
  
  if (vl_loc.l11<0) vl_loc.l11=0;
  sprintf(vl_bloc.loc.l04,"L04#%d#\n",vl_loc.l04);
  sprintf(vl_bloc.loc.l06,"L06#%s#\n",vl_loc.l06);
  sprintf(vl_bloc.loc.l11,"L11#%d#\n",vl_loc.l11);
	sprintf (vl_bloc.Localisation,"");
		break;
	}
	break;

    case XZAEC_FMC_Salage :
    case XZAEC_FMC_AnimalErrant:
    case XZAEC_FMC_PietonChaussee: /* PX-METACOR */
		vl_a_traiter_metacor=XDC_FAUX;

    case XZAEC_FMC_AireFermee:
				/* PX-METACOR */
		sprintf(vl_bloc_metacor.Bloc_Oblig.Type,"CA");
		vl_bloc_metacor.Bloc_Optio.Bloc.Acces.Entree1=0;
		vl_bloc_metacor.Bloc_Optio.Bloc.Acces.Entree2=0;
		vl_bloc_metacor.Bloc_Optio.Bloc.Acces.Sortie1=0;
		vl_bloc_metacor.Bloc_Optio.Bloc.Acces.Sortie2=0;
		if (vl_sens_sc==XDC_SENS_SUD) {
			vl_bloc_metacor.Bloc_Optio.Bloc.Acces.Entree1=1;
			vl_bloc_metacor.Bloc_Optio.Bloc.Acces.Sortie1=1;
		}
		if (vl_sens_sc==XDC_SENS_NORD) {
			vl_bloc_metacor.Bloc_Optio.Bloc.Acces.Entree2=1;
			vl_bloc_metacor.Bloc_Optio.Bloc.Acces.Sortie2=1;
		}

		if (vl_loc.l11<vl_loc.l12)
		{
  		sprintf(vl_bloc.loc.l12,"L12#%d#\n",0);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",0);
		sprintf(vl_bloc.loc.l10,"L10#%s#\n",vl_loc.l06);				
  		sprintf(vl_bloc.loc.l09,"L09#%d#\n",vl_loc.l04);
		}
		else
		{
  		sprintf(vl_bloc.loc.l12,"L12#%d#\n",0);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",0);
		sprintf(vl_bloc.loc.l06,"L06#%s#\n",vl_loc.l10);				
  		sprintf(vl_bloc.loc.l04,"L04#%d#\n",vl_loc.l09);
		}

	break;


    case XZAEC_FMC_FermetureEch:
    
	vl_bool1=0;
	vl_bool2=0;
	vl_bool3=0;
	vl_bool4=0;

		if (vl_loc.l11<vl_loc.l12)
		{
  		sprintf(vl_bloc.loc.l12,"L12#%d#\n",0);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",0);
		sprintf(vl_bloc.loc.l10,"L10#%s#\n",vl_loc.l06);				
  		sprintf(vl_bloc.loc.l09,"L09#%d#\n",vl_loc.l04);
		}
		else
		{
  		sprintf(vl_bloc.loc.l12,"L12#%d#\n",0);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",0);
		sprintf(vl_bloc.loc.l06,"L06#%s#\n",vl_loc.l10);				
  		sprintf(vl_bloc.loc.l04,"L04#%d#\n",vl_loc.l09);
		}

	if ( XZAE139_Lire_Fiche_Echangeur_Pour_Strada(va_fmc.Numero
						    	,va_fmc.Cle
						    	,&vl_bool1
						    	,&vl_bool2
						    	,&vl_bool3
						    	,&vl_bool4 ) == XDC_OK) {
				/* PX-METACOR */
		sprintf(vl_bloc_metacor.Bloc_Oblig.Type,"CA");

		vl_bloc_metacor.Bloc_Optio.Bloc.Acces.Entree1=vl_bool1;
		vl_bloc_metacor.Bloc_Optio.Bloc.Acces.Entree2=vl_bool2;

		vl_bloc_metacor.Bloc_Optio.Bloc.Acces.Sortie1=vl_bool3;
		vl_bloc_metacor.Bloc_Optio.Bloc.Acces.Sortie2=vl_bool4;


		sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#REX#\n");

		tl_string[0] = '\0';
		
		if (vl_bool4 == 1) {
			strcat (tl_string, "sens 2 sortie fermee, "); 
		sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#RCE#\n");
		}
		if (vl_bool2 == 1) {
			strcat (tl_string, "sens 2 entree fermee, "); 
		sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#REC#\n");
		}	
		if (vl_bool3 == 1) {
			strcat (tl_string, "sens 1 sortie fermee, "); 
		sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#RCE#\n");
		}	
		if (vl_bool1 == 1) {
			strcat (tl_string, "sens 1 entree fermee "); 
		sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#REC#\n");
		}
		
		if ((vl_bool3 == 1) && (vl_bool4 == 1))
			sprintf(vl_bloc.Localisation,"O#\nRDI#B");
		if ((vl_bool1 == 1) && (vl_bool2 == 1))
			sprintf(vl_bloc.Localisation,"I#\nRDI#B");
		if ((vl_bool1 == 1) && (vl_bool2 == 1) && (vl_bool3 == 1) && (vl_bool4 == 1))  {
			sprintf(vl_bloc.Localisation,"S#\nRDI#B");
			sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#REX#\n");
		}
		if (((vl_bool4 == 1) && (vl_bool2 == 1)) || ((vl_bool3 == 1) && (vl_bool1 == 1)))  {
			sprintf(vl_bloc.Localisation,"S#\nRDI#B");
			sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#REX#\n");
		}
		if (((vl_bool2 == 1) && (vl_bool3 == 1)) || ((vl_bool1 == 1) && (vl_bool4 == 1)))  {
			sprintf(vl_bloc.Localisation,"S#\nRDI#B");
			sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#REX#\n");
		}

		sprintf (vl_bloc.Bloc_Optio.Comment,"SUR#%s#\n",tl_string);		    	
		XZST_03EcritureTrace(XZSTC_INFO, "Fermeture ech : PHR=%s;LNP:%s,vl_bool1 =%d,vl_bool2=%d,vl_bool3=%d,vl_bool4=%d", vl_bloc.Bloc_Oblig.PHR,vl_bloc.Localisation,vl_bool1,vl_bool2,vl_bool3,vl_bool4);
		break;
		

	}
	break;

    case XZAEC_FMC_EchDeconseille:
    

		if (vl_loc.l11<vl_loc.l12)
		{
  		sprintf(vl_bloc.loc.l12,"L12#%d#\n",0);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",0);
		sprintf(vl_bloc.loc.l10,"L10#%s#\n",vl_loc.l06);				
  		sprintf(vl_bloc.loc.l09,"L09#%d#\n",vl_loc.l04);
		}
		else
		{
  		sprintf(vl_bloc.loc.l12,"L12#%d#\n",0);
  		sprintf(vl_bloc.loc.l11,"L11#%d#\n",0);
		sprintf(vl_bloc.loc.l06,"L06#%s#\n",vl_loc.l10);				
  		sprintf(vl_bloc.loc.l04,"L04#%d#\n",vl_loc.l09);
		}

	break;

  }

   if (vl_num_cause!=0)
    sprintf(vl_bloc.Bloc_Optio.Cause,"LNK#%s/%d/%d#\n",
		C_CODE_ESCOTA,
		vl_num_cause,
		vl_cle_cause);
  else
    strcpy(vl_bloc.Bloc_Optio.Cause,"");

 strcpy(pm_comment," ");
/* suppression de la transmission des commentaires 
 if ( XZAE155_LireCommentaires(va_fmc.Numero, va_fmc.Cle,ases_evt_genere_commentaires) != XDC_OK)
 {
       XZST_03EcritureTrace(XZSTC_WARNING, "Erreur a la recuperation des commentaires FMC") ;
 }
 else if (strlen(pm_comment) != 0)
 {
 	vl_taille=strlen(vl_bloc.Bloc_Optio.Comment)-6;
	if ( vl_taille < TAILLE_SUR)
 	{
		vl_bloc.Bloc_Optio.Comment[vl_taille+4]='\0';
		pm_comment[TAILLE_SUR-vl_taille-6]='\0';
 		sprintf (vl_bloc.Bloc_Optio.Comment,"%s%s#\n",vl_bloc.Bloc_Optio.Comment,pm_comment);
 	}
 }
*/
XZST_03EcritureTrace(XZSTC_WARNING, "1 l05=%s,vl_bloc.Localisation=%s.,sens=%d.",vl_loc.l05,vl_bloc.Localisation,vl_sens);
 if (!strcmp(vl_loc.l05,"U")) 
 {	/* si pas sur bretelle alors pas de LNP*/
	if (va_fmc.Type!=XZAEC_FMC_FermetureEch)
		sprintf (vl_bloc.Localisation,"");
	else
	{
	  	strcpy(vl_loc.l05,"B");
  		strcpy(vl_loc.l07,C_l07_B);
	}
	sprintf(vl_bloc.nbvoiesop,"");
 }
  sprintf(vl_bloc.loc.l05,"L05#%s#\n",vl_loc.l05);
  sprintf(vl_bloc.loc.l07,"L07#%s#\n",vl_loc.l07);

XZST_03EcritureTrace(XZSTC_WARNING, "2 l05=%s,vl_bloc.Localisation=%s.,sens=%d.",vl_loc.l05,vl_bloc.Localisation,vl_sens);

 if (XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_Jour_De_La_Semaine,&vl_Horodate) != XZSMC_OK)
                XZST_03EcritureTrace(XZSTC_WARNING, "Erreur a la lecture de l'heure courante") ;

XZST_03EcritureTrace(XZSTC_WARNING, "avt Ecriture strada %d l05=%s",va_fmc.Type,vl_loc.l05);
  /*ecrit le fichier strada et envoie le sur le PC strada*/
  if ( ases_evt_ecrit_strada(vl_bloc,va_fmc.Type) == XDC_NOK )
  {
                XZST_03EcritureTrace(XZSTC_WARNING, "Erreur a l'envoi du fichier") ;
                if (XZAC73_Ecrire_Exporter( va_fmc.Numero, va_fmc.Cle, vl_Horodate, XDC_ACT_ETAT_ECHEC) == XDC_NOK)
                		XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de mettre a jour l'action intergestionnaire") ;
                
  }
  else
  {
                if (XZAC73_Ecrire_Exporter( va_fmc.Numero, va_fmc.Cle, vl_Horodate, vl_type_horodate) == XDC_NOK)
                		XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de mettre a jour l'action intergestionnaire") ;
                
  }
XZST_03EcritureTrace(XZSTC_INFO, "avt Ecriture metacor %d",va_fmc.Type);

 /* s'il y a pas de restriction de circulation pour une restriction alors pas de cretaion de fichier metacor */
 if (!strcmp(vl_bloc_metacor.Bloc_Oblig.Type,"RES") && ((vl_bloc_metacor.Bloc_Oblig.rest[0]==' ') || (vl_bloc_metacor.Bloc_Oblig.rest[0]=='\0')) )
	{
	XZST_03EcritureTrace(XZSTC_INFO, "metacor : pas de restriction de circulation pour une restriction=> pas de generation de fichier metacor");
	vl_a_traiter_metacor=XDC_FAUX;
	}

 if ( vl_a_traiter_metacor)
	{	XZST_03EcritureTrace(XZSTC_WARNING, "Ecriture metacor");
	if ( ases_evt_ecrit_metacor(vl_bloc_metacor,C_PC_METACOR,"") != XDC_OK)
		XZST_03EcritureTrace(XZSTC_WARNING, "Ecriture metacor KO pour ESCOTA");
	if ( ases_evt_ecrit_metacor(vl_bloc_metacor,C_PC_METACOR_ASF,"_ASF") != XDC_OK)
		XZST_03EcritureTrace(XZSTC_WARNING, "Ecriture metacor KO pour ASF");
	}

  return (XDC_OK);

}



/* Fichier : @(#)ases_evt.c	1.38        Release : 1.38        Date : 04/04/11
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* ecriture du fichier strada d'une fmc
*
*****************************************************
* SEQUENCE D'APPEL :
* ases_evt_ecrit_strada
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
int ases_evt_ecrit_strada(T_Fichier	va_bloc, int va_type)
{
  FILE *pl_fd;
  FILE *pl_fp;
  char	vl_str[300];
  char  tl_str[300];
  char	pl_nom_fichier[70];
  int   vl_pos_fichier;
  int   vl_compteur;
  char	vl_ligne[300];
  char	pl_ligne[300];

        /* ecriture trace */
        XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : ases_evt_ecrit_strada" );
        XZST_03EcritureTrace(   XZSTC_INFO, "ases_evt_ecrit_strada:l05=%s",va_bloc.loc.l05 );


        /* Mise a jour du compteur des fichiers strada a generer */
        /* ----------------------------------------------------- */

        /* Lecture du compteur */
        if ( (pl_fp = fopen( CO_FIC_COMPTEUR, "r+")) == NULL )
        {
                XZST_03EcritureTrace(   XZSTC_WARNING, "erreur de lecture du fichier %s !", CO_FIC_COMPTEUR );
                return;
        }

        /* On lit les lignes du fichier compteur_evt.txt une par une */
        vl_pos_fichier=ftell(pl_fp);
        if (fgets(pl_ligne, CO_MAX_CARACTERE, pl_fp) != NULL)
          do {
                /* On ignore les lignes commencant par un # */
                if ( pl_ligne[0] == '#' )
                        continue;

                /* Pour les autres lignes on recherche la valeur du compteur */
                if (sscanf(pl_ligne,"%d", &vl_compteur) != 1)
                {
                        vl_compteur = 0;
                        /* ecriture trace */
                        XZST_03EcritureTrace(   XZSTC_WARNING, "le fichier %s n'a pas le bon format : les fichiers STRADA ont un nom incorrect !"
                                                                , CO_FIC_COMPTEUR);
                }
                break;
#ifdef _HPUX_SOURCE
          } while ( ((vl_pos_fichier=ftell(pl_fp))!=NULL) && (fgets(pl_ligne, CO_MAX_CARACTERE, pl_fp) != NULL) );
#else
          } while ( ((vl_pos_fichier=ftell(pl_fp))!=0) && (fgets(pl_ligne, CO_MAX_CARACTERE, pl_fp) != NULL) );
#endif 
       else {
          XZST_03EcritureTrace(   XZSTC_WARNING,"fichier vide : compteur remis a 0");
          vl_compteur = 9999999;
        }

        /* On gere le compteur */
        if (vl_compteur == 9999999 )
        {
                vl_compteur = 0;
/*                sprintf(pl_nom_fichier,"%s/S0000000.sit",C_PATH);*/
                sprintf(pl_nom_fichier,"%s/SESC0000.sit",C_PATH);
        }
        else
        {
                vl_compteur = vl_compteur + 1;

                /* generons le nom du fichier sur 7 chiffres a partir de vl_compteur */
/*                sprintf(pl_nom_fichier,"%s/S%07d.sit\0",C_PATH,vl_compteur);*/
                sprintf(pl_nom_fichier,"%s/SESC%04d.sit\0",C_PATH,vl_compteur%10000);

XZST_03EcritureTrace(   XZSTC_INFO, " nom du fichier = %s ", pl_nom_fichier);
        }


        /* Et on copie la nouvelle valeur du compteur dans le fichier avant de le fermer */
        fseek(pl_fp,vl_pos_fichier,SEEK_SET);
        fprintf(pl_fp,"%d",vl_compteur);
        fflush(pl_fp);
        fclose(pl_fp);


  /*ouverture du fichier*/
  if ((pl_fd=fopen(pl_nom_fichier,"w"))==NULL) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_ecrit_strada: pb creation fichier %s",
		pl_nom_fichier);
    return (XDC_NOK);
  }

  sprintf(vl_str,"MSE#%s#\n",C_DATEX_ESCOTA);
  fputs(vl_str,pl_fd);

  /*ecriture des blocs obligatoires*/
  fputs(va_bloc.Bloc_Oblig.SNM,pl_fd);
  fputs(va_bloc.Bloc_Oblig.ERF,pl_fd);
  fputs(va_bloc.Bloc_Oblig.DOB,pl_fd);
  fputs(va_bloc.Bloc_Oblig.PHR,pl_fd);
  sprintf(vl_str,"SNA#%s/%s#\n",C_DATEX_ESCOTA,C_CODE_ESCOTA);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"CLV#%d#\n",va_bloc.Bloc_Oblig.CLV);
  fputs(vl_str,pl_fd);
  fputs(va_bloc.Bloc_Oblig.INP,pl_fd);
  fputs(va_bloc.Bloc_Optio.Date_Debut,pl_fd);
  sprintf(vl_str,"LTV#%d#\n",va_bloc.Bloc_Oblig.LTV);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"VNM#%d#\n",va_bloc.Bloc_Oblig.VNM);
  fputs(vl_str,pl_fd);

  /*ecriture localisation*/
  fputs(va_bloc.loc.l01,pl_fd);
  fputs(va_bloc.loc.l02,pl_fd);
  fputs(va_bloc.loc.l03,pl_fd);
  fputs(va_bloc.loc.l04,pl_fd);
  fputs(va_bloc.loc.l05,pl_fd);
  fputs(va_bloc.loc.l06,pl_fd);
  fputs(va_bloc.loc.l07,pl_fd);
  fputs(va_bloc.loc.l08,pl_fd);
  fputs(va_bloc.loc.l09,pl_fd);
  fputs(va_bloc.loc.l10,pl_fd);
  fputs(va_bloc.loc.l11,pl_fd);
  fputs(va_bloc.loc.l12,pl_fd);

  /*ecriture des infos communes a toutes les fmc*/
  fputs(va_bloc.Bloc_Optio.Date_Fin,pl_fd);
  sprintf(vl_str,"CAN#%c#\n",va_bloc.Bloc_Optio.Fausse_Alerte);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"END#%c#\n",va_bloc.Bloc_Optio.Fin);
  fputs(vl_str,pl_fd);
  if (strcmp(va_bloc.Bloc_Optio.Cause,""))
    fputs(va_bloc.Bloc_Optio.Cause,pl_fd);

  sprintf(vl_str,"QIN#%d#\n",va_bloc.Bloc_Optio.Veracite);
  fputs(vl_str,pl_fd);
  /*DEM 901  Probalibility % = (11-veracite)*10; 1=>100% 5=>60%)*/
  sprintf(vl_str,"PRB#%d#\n",(11-va_bloc.Bloc_Optio.Veracite)*10);
  fputs(vl_str,pl_fd);

  fputs(va_bloc.Bloc_Optio.Voies,pl_fd);
  sprintf(vl_str,"FOR#%c#\n",va_bloc.Bloc_Optio.Prevision);
  fputs(vl_str,pl_fd);
 
/*a PN 30/10/01 : On ne remplit plus le DUV en attendant qu'on nous communique le bon format 
if ( va_bloc.Bloc_Optio.Duree < 6000)
  	sprintf(vl_str,"DUV#%02d%02d#\n",va_bloc.Bloc_Optio.Duree/60,va_bloc.Bloc_Optio.Duree % 60);
  else
	sprintf(vl_str,"DUR#L7#\n");
  fputs(vl_str,pl_fd);
  */
  if ( va_bloc.Bloc_Optio.Trafic != 0 )
  {
  	sprintf(vl_str,"MOT#%d#\n",va_bloc.Bloc_Optio.Trafic);
  	fputs(vl_str,pl_fd);
  }
 /* DEM701 */
  if ((va_bloc.Localisation[0] != '\0') && (va_bloc.Localisation[0] != ' '))
  {
  	sprintf(vl_str,"LNP#%s#\n",va_bloc.Localisation);
  	fputs(vl_str,pl_fd);
  }

  fputs(va_bloc.nbvoiesop,pl_fd);
  
  if (va_type != XZAEC_FMC_QueueBouchon)
  {
	sprintf(vl_str,"SEV#%d#\n",va_bloc.Bloc_Optio.Gravite);
	fputs(vl_str,pl_fd);
  }

  /*ecriture des infos specifiques a chaque fmc*/
  switch(va_type) {
	case XZAEC_FMC_Accident : 		
sprintf(vl_str,"NVE#%d#\n",va_bloc.Bloc_Optio.Bloc.Accident.Vehicules);
				  fputs(vl_str,pl_fd);
				  sprintf(vl_str,"NHL#%d#\n",va_bloc.Bloc_Optio.Bloc.Accident.PL);
				  fputs(vl_str,pl_fd);
				  sprintf(vl_str,"NBU#%d#\n",va_bloc.Bloc_Optio.Bloc.Accident.Cars);
				  fputs(vl_str,pl_fd);
				  sprintf(vl_str,"CAV#%d#\n",va_bloc.Bloc_Optio.Bloc.Accident.Rem);
				  fputs(vl_str,pl_fd);
				  sprintf(vl_str,"INJ#%d#\n",va_bloc.Bloc_Optio.Bloc.Accident.Blesses);
				  fputs(vl_str,pl_fd);
				  sprintf(vl_str,"DEA#%d#\n",va_bloc.Bloc_Optio.Bloc.Accident.Morts);
				  fputs(vl_str,pl_fd);
				  if (va_bloc.Bloc_Optio.Bloc.Accident.Matiere!=0) {
				    sprintf(vl_str,"LOA#DAN#\n");
				    fputs(vl_str,pl_fd);

				  }
			          break;

	case XZAEC_FMC_Delestage :
	case XZAEC_FMC_Deviation : 
				   break;

	case XZAEC_FMC_Meteo : sprintf(vl_str,"LEN#%1.3f#\n",(float) va_bloc.Bloc_Optio.Bloc.Meteo.Longueur/1000);
			       fputs(vl_str,pl_fd);
			       sprintf(vl_str,"VIS#%d#\n",va_bloc.Bloc_Optio.Bloc.Meteo.Visibilite);
			       fputs(vl_str,pl_fd);
			       break;

	case XZAEC_FMC_Contresens:
	case XZAEC_FMC_VehArrete:
	case XZAEC_FMC_VehPanne:
	case XZAEC_FMC_VehFeu: fputs(va_bloc.Bloc_Optio.Bloc.Vehicule.Type,pl_fd);
			       fputs(va_bloc.Bloc_Optio.Bloc.Vehicule.Matiere,pl_fd);
			       sprintf(vl_str,"WEI#%d#\n",va_bloc.Bloc_Optio.Bloc.Vehicule.Volume);
			       fputs(vl_str,pl_fd);
			       break;

	case XZAEC_FMC_Cirque:
	case XZAEC_FMC_ConvoiExc:
	case XZAEC_FMC_ConvoiMil:
	case XZAEC_FMC_VehLent : sprintf(vl_str,"LEN#%1.3f#\n",(float)va_bloc.Bloc_Optio.Bloc.VehicLent.Longueur/1000);
				 fputs(vl_str,pl_fd);
				 sprintf(vl_str,"IVS#%d#\n",va_bloc.Bloc_Optio.Bloc.VehicLent.Vitesse);
				 fputs(vl_str,pl_fd);
				 break;

	case XZAEC_FMC_Manifestation:
	case XZAEC_FMC_GreveESCOTA:
	case XZAEC_FMC_Escargot: 
				 break;

	case XZAEC_FMC_Basculement : sprintf(vl_str,"LEN#%1.3f#\n",(float)va_bloc.Bloc_Optio.Bloc.Basculement.Longueur/1000);
				     fputs(vl_str,pl_fd);
			   	     break;

	case XZAEC_FMC_Travaux: sprintf(vl_str,"LEN#%1.3f#\n",(float) va_bloc.Bloc_Optio.Bloc.Travaux.Longueur/1000);
				fputs(vl_str,pl_fd);
XZST_03EcritureTrace(XZSTC_WARNING,"LEN#%1.3f#\n",(float) va_bloc.Bloc_Optio.Bloc.Travaux.Longueur/1000);

				sprintf(vl_str,"MBY#%c#\n",(char) va_bloc.Bloc_Optio.Bloc.Travaux.Mobiles);
				fputs(vl_str,pl_fd);
				break;

	case XZAEC_FMC_ZoneStockage: sprintf(vl_str,"LEN#%1.3f#\n",(float) va_bloc.Bloc_Optio.Bloc.Travaux.Longueur/1000);
				     fputs(vl_str,pl_fd);
				     break;

	case XZAEC_FMC_IncSsConces: 
				    break;

	case XZAEC_FMC_QueueBouchon: sprintf(vl_str,"QUE#%1.3f#\n",(float) va_bloc.Bloc_Optio.Bloc.Bouchon.Longueur/1000);
				     fputs(vl_str,pl_fd);
				     if (va_bloc.Bloc_Optio.Bloc.Bouchon.Type < va_bloc.Bloc_Optio.Gravite)
				     	sprintf(vl_str,"SEV#%d#\n",va_bloc.Bloc_Optio.Bloc.Bouchon.Type);
				     else
				     	sprintf(vl_str,"SEV#%d#\n",va_bloc.Bloc_Optio.Gravite);
				     
				     fputs(vl_str,pl_fd);
				     break;

	case XZAEC_FMC_FermetureEch: break;

	default : break;
  }
  fputs(va_bloc.Bloc_Optio.Comment,pl_fd);
  /*fermeture du fichier*/
  fclose(pl_fd);

 
/*DEM1285 : suppression du noeud STRADA */
                
                         /*FTP*/
			system(C_SCRIPT_FTP);

			/* C_PATH contient le repertoire de travail du SAE ou sont transferes les fichiers trafic */
			sprintf(tl_str, "rm %s/S*.sit %s/fr*.sit",C_PATH,C_PATH);

			system(tl_str);
			
                

	sprintf( tl_str,"find %s -mtime +%d -exec rm {} \\;", C_PATH, CO_NB_JOUR_PURGE);
        system(tl_str);
  return (XDC_OK);
}


/* Fichier : @(#)ases_evt.c	1.38        Release : 1.38        Date : 04/04/11
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE 
******************************************************
* DESCRIPTION DE LA FONCTION :
* ecriture du fichier metacor d'une fmc
*
*****************************************************
* SEQUENCE D'APPEL :
* ases_evt_ecrit_metacor
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
int ases_evt_ecrit_metacor(T_Fichier_METACOR	va_bloc,char *pa_dest, char *pa_suffixe)
{
  FILE *pl_fd;
  FILE *pl_fp;
  char	vl_str[300];
  char  tl_str[300];
  char	pl_nom_fichier[70];
  int   vl_pos_fichier;
  int   vl_compteur;
  char	vl_ligne[300];
  char	pl_ligne[300];
  int i=0;
  char vl_Loca_init[11];
  char		vl_etat[11];
  char		vl_sens[3];

        /* ecriture trace */
   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : ases_evt_ecrit_metacor" );



  /* Si l'evt est un Conrole d'acc�s alors on g�n�re 4 fichier, un par bretelle */
  /* dans les autres cas on reste � un fichier par evt */
  if (!strcmp(va_bloc.Bloc_Oblig.Type,"CA") && strcmp(va_bloc.Bloc_Oblig.Loca_deb_type,C_AIRE))  {
  XZST_03EcritureTrace(   XZSTC_INFO, "CA = %s Acces.Sortie1:%d;Acces.Entree1:%d;Acces.Sortie2:%d;Acces.Entree2:%d",va_bloc.Bloc_Oblig.Ref,va_bloc.Bloc_Optio.Bloc.Acces.Sortie1,va_bloc.Bloc_Optio.Bloc.Acces.Entree1,va_bloc.Bloc_Optio.Bloc.Acces.Sortie2,va_bloc.Bloc_Optio.Bloc.Acces.Entree2);
  
  for (i=1;i<=4;i++)
  {              /* generons le nom du fichier  */
  sprintf(pl_nom_fichier,"%s%s/%s_%s-%d_%06d\0",C_PATH_METACOR,pa_suffixe,va_bloc.Bloc_Oblig.Type,va_bloc.Bloc_Oblig.Ref,i,va_bloc.Bloc_Oblig.Evo);


  XZST_03EcritureTrace(   XZSTC_WARNING, " nom du fichier = %s ", pl_nom_fichier);
  XZST_03EcritureTrace(   XZSTC_INFO, " nom du fichier = %s ", pl_nom_fichier);
  sprintf(vl_etat,"%s",va_bloc.Bloc_Oblig.etat);
  switch(i)  {
  	case 1 :
		va_bloc.Bloc_Oblig.Loca_deb_id[strlen(va_bloc.Bloc_Oblig.Loca_deb_id)-1]='3';
		sprintf(vl_sens,"1");
		if (!(va_bloc.Bloc_Optio.Bloc.Acces.Sortie1))
			sprintf(vl_etat,C_FIN);
		break;
  	case 2 :
		va_bloc.Bloc_Oblig.Loca_deb_id[strlen(va_bloc.Bloc_Oblig.Loca_deb_id)-1]='7';
		sprintf(vl_sens,"1");
		if (!(va_bloc.Bloc_Optio.Bloc.Acces.Entree1))
			sprintf(vl_etat,C_FIN);
		break;
  	case 3 :
		va_bloc.Bloc_Oblig.Loca_deb_id[strlen(va_bloc.Bloc_Oblig.Loca_deb_id)-1]='4';
		sprintf(vl_sens,"2");
		if (!(va_bloc.Bloc_Optio.Bloc.Acces.Sortie2))
			sprintf(vl_etat,C_FIN);
		break;
  	case 4 :
		va_bloc.Bloc_Oblig.Loca_deb_id[strlen(va_bloc.Bloc_Oblig.Loca_deb_id)-1]='8';
		sprintf(vl_sens,"2");
		if (!(va_bloc.Bloc_Optio.Bloc.Acces.Entree2))
			sprintf(vl_etat,C_FIN);
		break;
  	default :
		break;

  }


  /*ouverture du fichier*/
  if ((pl_fd=fopen(pl_nom_fichier,"w"))==NULL) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_ecrit_metacor: pb creation fichier %s",
		pl_nom_fichier);
    return (XDC_NOK);
  }

  /*ecriture des blocs obligatoires*/
  sprintf(vl_str,"type_evnt : %s\n",va_bloc.Bloc_Oblig.Type);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"ref : %s-%d\n",va_bloc.Bloc_Oblig.Ref,i);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"evo : %d\n",va_bloc.Bloc_Oblig.Evo);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"hd_deb_prev : %s\n",va_bloc.Bloc_Oblig.Debut_prevu);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"hd_fin_prev : %s\n",va_bloc.Bloc_Oblig.Fin_prevue);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"hd_deb : %s\n",va_bloc.Bloc_Oblig.Debut);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"hd_fin : %s\n",va_bloc.Bloc_Oblig.Fin);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"loca_deb_type : %s\n",va_bloc.Bloc_Oblig.Loca_deb_type);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"loca_deb_nom : %s\n",va_bloc.Bloc_Oblig.Loca_deb_nom);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"loca_deb_id : %s\n",va_bloc.Bloc_Oblig.Loca_deb_id);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"sns_deb_id : %s\n",vl_sens);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"pk_deb : %s\n",va_bloc.Bloc_Oblig.pk_deb);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"pr_deb : %s\n",va_bloc.Bloc_Oblig.pr_deb);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"loca_fin_type : %s\n",va_bloc.Bloc_Oblig.Loca_fin_type);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"loca_fin_nom : %s\n",va_bloc.Bloc_Oblig.Loca_deb_nom);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"loca_fin_id : %s\n",va_bloc.Bloc_Oblig.Loca_deb_id);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"sns_fin_id : %s\n",vl_sens);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"pk_fin : %s\n",va_bloc.Bloc_Oblig.pk_fin);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"pr_fin : %s\n",va_bloc.Bloc_Oblig.pr_fin);
  fputs(vl_str,pl_fd);

  sprintf(vl_str,"etat : %s\n",vl_etat);
  fputs(vl_str,pl_fd);

  /*ecriture des blocs optionels*/
  if (!strcmp(va_bloc.Bloc_Oblig.Type,"INC"))
	{
	sprintf(vl_str,"nb_bg : %d\n",va_bloc.Bloc_Optio.Bloc.Incident.Blesses_graves);
	fputs(vl_str,pl_fd);
	sprintf(vl_str,"nb_vl : %d\n",va_bloc.Bloc_Optio.Bloc.Incident.VL);
	fputs(vl_str,pl_fd);
	sprintf(vl_str,"nb_pl : %d\n",va_bloc.Bloc_Optio.Bloc.Incident.PL);
	fputs(vl_str,pl_fd);
	}
  /*fermeture du fichier*/
  fclose(pl_fd);
  }
  }
  else
  {


                /* generons le nom du fichier  */
  sprintf(pl_nom_fichier,"%s%s/%s_%s_%06d\0",C_PATH_METACOR,pa_suffixe,va_bloc.Bloc_Oblig.Type,va_bloc.Bloc_Oblig.Ref,va_bloc.Bloc_Oblig.Evo);


  XZST_03EcritureTrace(   XZSTC_WARNING, " nom du fichier = %s ", pl_nom_fichier);
        




  /*ouverture du fichier*/
  if ((pl_fd=fopen(pl_nom_fichier,"w"))==NULL) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_ecrit_metacor: pb creation fichier %s",
		pl_nom_fichier);
    return (XDC_NOK);
  }

  /*ecriture des blocs obligatoires*/
  sprintf(vl_str,"type_evnt : %s\n",va_bloc.Bloc_Oblig.Type);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"ref : %s\n",va_bloc.Bloc_Oblig.Ref);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"evo : %d\n",va_bloc.Bloc_Oblig.Evo);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"hd_deb_prev : %s\n",va_bloc.Bloc_Oblig.Debut_prevu);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"hd_fin_prev : %s\n",va_bloc.Bloc_Oblig.Fin_prevue);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"hd_deb : %s\n",va_bloc.Bloc_Oblig.Debut);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"hd_fin : %s\n",va_bloc.Bloc_Oblig.Fin);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"loca_deb_type : %s\n",va_bloc.Bloc_Oblig.Loca_deb_type);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"loca_deb_nom : %s\n",va_bloc.Bloc_Oblig.Loca_deb_nom);
  fputs(vl_str,pl_fd);
  if (!strcmp(va_bloc.Bloc_Oblig.Loca_deb_type,"SC")) /* A8 -> 8 */
  	sprintf(vl_str,"loca_deb_id : %s\n",&(va_bloc.Bloc_Oblig.Loca_deb_nom[1]));
  else	
  	sprintf(vl_str,"loca_deb_id : %s\n",va_bloc.Bloc_Oblig.Loca_deb_id);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"sns_deb_id : %s\n",va_bloc.Bloc_Oblig.sns_deb_id);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"pk_deb : %s\n",va_bloc.Bloc_Oblig.pk_deb);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"pr_deb : %s\n",va_bloc.Bloc_Oblig.pr_deb);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"loca_fin_type : %s\n",va_bloc.Bloc_Oblig.Loca_fin_type);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"loca_fin_nom : %s\n",va_bloc.Bloc_Oblig.Loca_fin_nom);
  fputs(vl_str,pl_fd);
  if (!strcmp(va_bloc.Bloc_Oblig.Loca_fin_type,"SC")) /* A8 -> 8 */
  	sprintf(vl_str,"loca_fin_id : %s\n",&(va_bloc.Bloc_Oblig.Loca_fin_nom[1]));
  else	
  	sprintf(vl_str,"loca_fin_id : %s\n",va_bloc.Bloc_Oblig.Loca_fin_id);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"sns_fin_id : %s\n",va_bloc.Bloc_Oblig.sns_fin_id);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"pk_fin : %s\n",va_bloc.Bloc_Oblig.pk_fin);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"pr_fin : %s\n",va_bloc.Bloc_Oblig.pr_fin);
  fputs(vl_str,pl_fd);
  /* si evt RES ou INC alors , on ajoute le champs 'rest' */
  if ( (!strcmp(va_bloc.Bloc_Oblig.Type,"RES")) || (!strcmp(va_bloc.Bloc_Oblig.Type,"INC"))) {
	sprintf(vl_str,"rest : %s\n",va_bloc.Bloc_Oblig.rest);
  	fputs(vl_str,pl_fd);
  }
  sprintf(vl_str,"etat : %s\n",va_bloc.Bloc_Oblig.etat);
  fputs(vl_str,pl_fd);

  /*ecriture des blocs optionels*/
  if (!strcmp(va_bloc.Bloc_Oblig.Type,"INC"))
	{
	sprintf(vl_str,"nb_bg : %d\n",va_bloc.Bloc_Optio.Bloc.Incident.Blesses_graves);
	fputs(vl_str,pl_fd);
	sprintf(vl_str,"nb_vl : %d\n",va_bloc.Bloc_Optio.Bloc.Incident.VL);
	fputs(vl_str,pl_fd);
	sprintf(vl_str,"nb_pl : %d\n",va_bloc.Bloc_Optio.Bloc.Incident.PL);
	fputs(vl_str,pl_fd);
	}
  /*fermeture du fichier*/
  fclose(pl_fd);
  }


       /* construit la commande test reseau de la machine demandee */
/*         sprintf(pl_ligne,"%s %s %u %u",SG_PING,pa_dest,*/
/*                                SG_BYTES_PING, SG_COUNT_PING);*/

/*        XZST_03EcritureTrace(XZSTC_INFO,"ligne %s",pl_ligne);*/

        /* execute la commande, */
/*	if ((pl_fp = popen(pl_ligne,"r")) != (FILE *)NULL)*/
/*        {*/
                /* recupere le resultat de cette commande */
/*                fgets(vl_ligne,sizeof(vl_ligne)-1,pl_fp) ;*/
/*                fgets(vl_ligne,sizeof(vl_ligne)-1,pl_fp) ;*/
/*                pclose(pl_fp) ;*/

/*                XZST_03EcritureTrace(XZSTC_INFO,"ligne ping = %s",vl_ligne);*/

                /*tout va bien ou ASF */
/*                if ((strstr(vl_ligne,SG_TOKEN_TST) != NULL)|| !strcmp(pa_suffixe,"_ASF"))*/
/*                {*/
                         /*FTP*/
/*			sprintf(tl_str, "%s%s",C_SCRIPT_FTP_METACOR,pa_suffixe);*/
/*			system(tl_str);*/

			/* C_PATH contient le repertoire de travail du SAE ou sont transferes les fichiers trafic */
			/*sprintf(tl_str, "rm %s%s/*_*_*",C_PATH_METACOR,pa_suffixe);

			system(tl_str);*/
			
/*                }*/
/*                else*/
/*                {*/
/*                        XZST_03EcritureTrace(   XZSTC_WARNING, "ases_evt_ftp : PC metacor HS");*/
/*			sprintf(tl_str, "rm %s%s/*_*_*",C_PATH_METACOR,pa_suffixe);*/
/*			system(tl_str);*/
/*			sprintf( tl_str,"find %s%s -mtime +%d -exec rm {} \\;", C_PATH_METACOR,pa_suffixe, CO_NB_JOUR_PURGE);*/
/*        		system(tl_str);*/
/*                        return XDC_OK;*/
/*                }*/
/*        }*/
/*        else*/
/*       {        XZST_03EcritureTrace(XZSTC_WARNING,*/
/*                     "erreur execution de %s",pl_ligne);*/
/*                        XZST_03EcritureTrace(   XZSTC_WARNING, "ases_evt_ftp : PC metacor HS");*/
/*			sprintf(tl_str, "rm %s%s/*_*_*",C_PATH_METACOR,pa_suffixe);*/
/*			system(tl_str);*/
/*			sprintf( tl_str,"find %s%s -mtime +%d -exec rm {} \\;", C_PATH_METACOR,pa_suffixe, CO_NB_JOUR_PURGE);*/
/*        		system(tl_str);*/
/*                 return XDC_NOK;*/
/*	}*/

	sprintf( tl_str,"find %s%s -mtime +%d -exec rm {} \\;", C_PATH_METACOR,pa_suffixe, CO_NB_JOUR_PURGE);
        system(tl_str);
  return (XDC_OK);
}


char * heurehiversete(int va_j,int va_m,int va_a)
{
static char	*tl_jour[]={"Dim","Lun","Mar","Mer","Jeu","Ven","Sam"};
static int tl_mois[]={1,4,4,0,2,5,0,3,6,1,4,6};

int vl_an=0, bl_jour=0, vl_num=0;
int bl_hiv=0;

va_a=va_a-2000;

vl_num=va_a+va_a/4+va_j+tl_mois[va_m-1]+5;

if ((va_a/4 == 0) && (va_m <=2))
	vl_num=vl_num-1;

vl_num=vl_num % 7;
XZST_03EcritureTrace(XZSTC_FONCTION,"vl_num=%d; va_j-25=%d\n",vl_num,va_j-25);
if ((va_m<=3) || (va_m>=11))
{
       bl_hiv = 1;}

  else bl_hiv = 0;
 
  if ((va_m==3) && (va_j-25>=vl_num))
               bl_hiv= 0;
   if ((va_m==10) && (va_j-25>=vl_num))
               bl_hiv= 1;


XZST_03EcritureTrace(XZSTC_FONCTION,"hivers=%d\n",bl_hiv);
if (bl_hiv)
return "P01";
else
return "P02";

}
