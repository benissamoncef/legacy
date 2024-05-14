/* Fichier : @(#)aweb_export.c	1.5  Release : 1.5   Date : 08/31/10	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : aweb_export.c			*
*************************************************
* DESCRIPTION :
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
*************************************************
* HISTORIQUE :
*
* JMG	16/01/01 : creation 1.1
* JMG	17/11/08 : refonte avec utilisation de tagen 1.2 DEM 836
* PNI	06/10/09 : Ajout trace et reinit timer v1.4
* JPL	31/08/10 : Ajout du type Fmc "Zone de stockage" (DEM 942) 1.5
* SFO   06/10/14 : Adaptation pour compilation sous linux
************************************************/
#include "aweb_export.h"
#include "xzlg.h"


/* definition des constantes */

#define CO_FIC_FREQUENCE	XDC_PATH_STRADA "/frequence.txt"
#define CO_REP_OUT		XDC_PATH_STRADA "/web/"
#define CO_DELAI_FREQUENCE	30

/* dÅfinition des variables globales È l'application */

/* definition des variables locales au module */
static char *	version = "@(#)aweb_export.c	1.1 1.1 04/19/01 : commentaire" ;

static FILE *pm_fd;
static int vm_last_checksum;
static int vm_last_occurence;
static int vm_frequence;
static int vm_go=XDC_FAUX;
static double vm_HorodateSec=0; 
static XZSMT_Horodate vm_HorodateZero = "";


int aweb_export_charge_textes(XDY_Mot , char *,XDY_Octet ,char *, XDY_Octet );

T_Type_FMC              tg_types_fmc[]=
{
        {       XZAEC_FMC_Accident,             "",  0  ,       ""},
        {       XZAEC_FMC_Delestage,            ""   , 0,      ""},
        {       XZAEC_FMC_Deviation,            ""   , 0,      ""},
        {       XZAEC_FMC_PosteOperateur,       ""   , 0 ,                  ""},
        {       XZAEC_FMC_Meteo,                ""   , 0,          ""},
        {       XZAEC_FMC_Contresens,           ""   , 0,     ""},
        {       XZAEC_FMC_VehArrete,            ""   , 0,     ""},
        {       XZAEC_FMC_VehPanne,             ""   , 0,      ""},
        {       XZAEC_FMC_VehFeu,               ""   , 0,        ""},
        {       XZAEC_FMC_Cirque,               ""   , 0,         ""},
        {       XZAEC_FMC_ConvoiExc,            ""   , 0,      ""},
        {       XZAEC_FMC_ConvoiMil,            ""   , 0,     ""},
        {       XZAEC_FMC_VehLent,              ""   , 0,       ""},
        {       XZAEC_FMC_Manifestation,        ""   , 0,          ""},
        {       XZAEC_FMC_GreveESCOTA,          ""   , 0,           ""},
        {       XZAEC_FMC_Escargot,             ""   , 0,       ""},
        {       XZAEC_FMC_Basculement,          ""   , 0,         ""},
        {       XZAEC_FMC_Travaux,              ""   , 0,        ""},
        {       XZAEC_FMC_IncSsConces,          ""   , 0,      ""},
        {       XZAEC_FMC_QueueBouchon,         ""   , 0,        ""},
        {       XZAEC_FMC_FermetureEch,         ""   , 0,       ""},
        {       XZAEC_FMC_Eboulement,           ""   , 0,          ""},
        {       XZAEC_FMC_Incendie,             ""   , 0,       ""},
        {       XZAEC_FMC_Inondation,           ""   , 0,     ""},
        {       XZAEC_FMC_Obstacle,             ""   , 0,          ""},
        {       XZAEC_FMC_ChausseDeter,         ""   , 0,   ""},
        {       XZAEC_FMC_FlaqueHuile,          ""   , 0,   ""},
        {       XZAEC_FMC_AnimalErrant,         ""   , 0,         ""},
        {       XZAEC_FMC_FrontiereFermee,      ""   , 0,      ""},
        {       XZAEC_FMC_AltercationUsager,    ""   , 0, ""},
        {       XZAEC_FMC_Agression,            ""   , 0,      ""},
        {       XZAEC_FMC_MalaiseUsager,        ""   , 0,        ""},
        {       XZAEC_FMC_EvtHorsReseau,        ""   , 0,          ""},
        {       XZAEC_FMC_AireFermee,           ""   , 0,    ""},
        {       XZAEC_FMC_Salage,               ""   , 0,         ""},
        {       XZAEC_FMC_HoldUpVol,            ""   , 0,         ""},
        {       XZAEC_FMC_TeteBouchon,          ""   , 0,                   ""},
        {       XZAEC_FMC_PietonChaussee,       ""   , 0,        ""},
        {       XZAEC_FMC_Autre,                ""   , 0,          ""},
        {       XZAEC_FMC_VehMarcheArr,         ""   , 0, ""},
        {       XZAEC_FMC_TraitChaussees,       ""   , 0,   ""},
        {       XZAEC_FMC_EchDeconseille,       ""   , 0,        ""},
        {       XZAEC_FMC_ChausseeGliss,        ""   , 0, ""},
        {       XZAEC_FMC_BasculTravaux,        ""   , 0,     ""},
        {       XZAEC_FMC_ZoneStockage,         ""   , 0,     ""}
};

/* definition de fonctions externes */

/*A
*******************************************
* Fonction d'initialisation pour la tache
* Elle lit les fichiers de parametrage
* et arme les deux timers :
* - le timer de programmation qui envoie 
* un datagroup
* - le timer de purge
*******************************************
* Arguments : Aucun
*******************************************/

int aweb_export_init()
{
XZSMT_CleProg		vl_cle;
XZSCT_Datagroup 	vl_Datagroup ;
int 			vl_JourSemaine;
double 			vl_HorodateSec; 
XZSCT_NomMachine 	pl_NomMachine	= "\0";
T_BOOL 			vl_etat;
FILE 			*pl_fp;
char			pl_param[200];
char			pl_ligne[200];
XDY_Entier		vl_val;
int			vl_station;
int			vl_sens;
XZIAT_destinataire	vl_destinataire;
char			pl_commande[150]="";
char			pl_erreur[200]="";
int			vl_code_retour;



	XZST_03EcritureTrace(   XZSTC_INTERFACE, "IN  : aweb_export_init" );
 	
  	/* Recuperation du nom de la machine locale */
	XZSC_07NomMachine(pl_NomMachine) ;
 	
  	/* Lecture de la frequence d'exportation des donnees vers les ASF */
	if ( (pl_fp = fopen(CO_FIC_FREQUENCE, "r")) == NULL ) {
		XZST_03EcritureTrace(   XZSTC_WARNING, "erreur de lecture du fichier %s !"
							,CO_FIC_FREQUENCE );
		return(XDC_NOK);
	}	


	/* On lit les lignes du fichier frequence.txt une par une */
	/* ------------------------------------------------------ */
	while ( fgets(pl_ligne, 200, pl_fp) != NULL) {
		/* On ignore les lignes commencant par un # */
		if ( pl_ligne[0] == '#' )
			continue;
		
		/* Pour les autres lignes on recherche la frequence de notre tache */
		if (sscanf(pl_ligne,"%[^;];%d", pl_param, &vl_val)==2){
			if ( !strcmp( pl_param, "taweb.x") ) {
				vm_frequence = vl_val;
				break;			
			}
		}
		else {
			XZST_03EcritureTrace(   XZSTC_WARNING, "le fichier %s n'a pas le format demande !"
							,CO_FIC_FREQUENCE );	
			vm_frequence =0;
		}
	}
		
	if (vm_frequence==0) {
	  XZST_03EcritureTrace(   XZSTC_WARNING, "pas de frequence dans le fichier %s : utilisation freq par default %d"
	  				       , CO_FIC_FREQUENCE
	  				       , CO_DELAI_FREQUENCE);
	  vm_frequence=	CO_DELAI_FREQUENCE;
	}			    
	  				       
	  				       
  	fclose(pl_fp);
  	

  	/*A
	* Initialisation de la callback du timer de programmation
	* -------------------------------------------------------
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AW_TIMER), aweb_export_ftp,(T_CB_ARG)NULL) == NULL) 
	{	
                XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AS_TIMER_SRV_RADT non cree.",version );
		return XDC_NOK;
	}
	
 	XZSM_00InitServices();

	/*A Lecture horodate courante systeme */
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vm_HorodateSec);
   
	   
	sprintf( vl_cle , "cle_timer_web" );

#ifdef _HPUX_SOURCE
       XZSM_02AnnulerProgrammation( vl_cle,NULL,NULL);
#else
       XZSM_02AnnulerProgrammation( vl_cle,(char *) NULL,0);
#endif
   
	/*A Abonnement au datagroup pour que le timer nous envoie les messages */
	if (! XDG_JoindreDG(XDG_AW_TIMER) )
	{	
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING, 
				"Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
			return XDC_NOK;
	}	
   
	/* armement du timer de programmation */
	/* ---------------------------------- */
/*	vm_HorodateSec+=vm_frequence;
	if (XZSM_01DemanderProgrammation(	vm_HorodateSec,
						0,
						XDG_AW_TIMER,
						XDM_AW_TIMER,
						vl_cle,
						NULL) != XDC_OK)
	{*/
	/* Ecriture Trace */
/*        XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de donnees web non declenchee. \n " );
        return XDC_NOK;
	}
	else
	  XZST_03EcritureTrace( XZSTC_INFO, "XZSM_01DemanderProgrammation OK");*/
	
	/*suppression d'eventuels anciens fichiers a transmettre*/
	sprintf(pl_ligne,"rm %s/*.TXT %s/*.txt",CO_REP_OUT,CO_REP_OUT);
	system(pl_ligne);

	/*chargement en memoire des textes et icones Web*/
	if (XZAE178_Lire_Config_Web(aweb_export_charge_textes)!= XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel XZAE168_Lire_Config_Web");
		return;
	}

	/*chargement en memoire de la liste des troncons*/
        if (XZAS81_Lire_Troncons((XDY_FonctionInt)aweb_ftp_calcul_troncons,
		(XDY_Octet)XDC_WEB_NIVEAU_0) != XDC_OK)
	{
	        XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel XZAS81_Lire_Troncons");
		return;
	}
        if (XZAS81_Lire_Troncons((XDY_FonctionInt)aweb_ftp_calcul_troncons,
		(XDY_Octet)XDC_WEB_NIVEAU_1) != XDC_OK)
	{
	        XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel XZAS81_Lire_Troncons");
		return;
	}
        if (XZAS81_Lire_Troncons((XDY_FonctionInt)aweb_ftp_calcul_troncons,
		(XDY_Octet)XDC_WEB_NIVEAU_2) != XDC_OK)
	{
	        XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel XZAS81_Lire_Troncons");
		return(XDC_NOK);
	}

	vm_last_checksum=0;
	vm_last_occurence=0;

	/*recupere le semaphore de tagen*/
	if (XZLG503_SemGet("/produits/migrazur/appliSD/exec/tagen.x",XZLGC_SEM_FMC) != XDC_OK) {
		  XZST_03EcritureTrace( XZSTC_WARNING, "atdp_export_init: pb appel a XZLG503_SemGet");
		    return (XDC_NOK);
	}
	if (XZLG503_SemGet("/produits/migrazur/appliSD/exec/tagen.x",XZLGC_SEM_RDT) != XDC_OK) {
		  XZST_03EcritureTrace( XZSTC_WARNING, "atdp_export_init: pb appel a XZLG503_SemGet");
		    return (XDC_NOK);
	}

	
	/*recup des evts en cours sur le reseau*/
	if (XZLG986_Accede_Memoire_Partagee_FMC() != XDC_OK) {
	      XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel  XZLG986_Accede_Memoire_Partagee_FMC") ;
		return(XDC_NOK);
	}

	if (XZLG994_Accede_Memoire_Partagee_Nombre() != XDC_OK) {
	  XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel  XZLG994_Accede_Memoire_Partagee_Nombre");
	  return (XDC_NOK);
        }

	if (XZLG981_Accede_Memoire_Partagee_RDT() != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel  XZLG981_Accede_Memoire_Partagee_RDT");
		return(XDC_NOK);
	}

	/*chargement en memoire des stations RADT a exporter*/
	if (aweb_charge_rdt() != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel aweb_charge_rdt");
		return (XDC_NOK);
	}

	/*chargement en memoire des zones TDP a generer via RADT*/
	if (aweb_charge_zdp() != XDC_OK) {
		XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel aweb_charge_zdp");
		return (XDC_NOK);
	}

	/*on declenche asap une premiere generation*/
	vm_go=XDC_VRAI;
	aweb_export_ftp();
	vm_go=XDC_FAUX;

	/* Ecriture Trace */
	XZST_03EcritureTrace( XZSTC_INFO, "OUT : aweb_export_init" );
	return XDC_OK;

}


/*A
*******************************************
* Fonction call back qui reagit au datagroup
* definit dans le timer de programmation
*******************************************
* Arguments : Aucun
*******************************************/

void aweb_export_ftp()
{
  int vl_cr;
  XZLGT_Nombre_Mem *vl_nb;
  int                     vl_JourSemaine;
  double                  vl_HorodateSec;

  /* Ecriture Trace */
  XZST_03EcritureTrace( XZSTC_INFO, "In : aweb_export_ftp");
  XZST_03EcritureTrace( XZSTC_WARNING, "In : aweb_export_ftp memo date");
  XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);

  /*traiter MES_NIV_DER*/
  if (XZAS80_Traiter_Troncons() != XDC_OK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "aweb_export_ftp : pb appel XZAS80");
  }

  /*reinitialiser l'etat des icones WEB*/
  if (XZAE164_Init_Icones() != XDC_OK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "aweb_export_ftp : pb appel XZAE164");
  }

  vl_nb=XZLG993_Get_Nombre();

  XZST_03EcritureTrace( XZSTC_INFO, "aweb_export_ftp : checksum = %d, last = %d, occurence = %d",
			vl_nb->Checksum,
			vm_last_checksum,
			vm_last_occurence);

  if (vl_nb->Checksum==vm_last_checksum) {
    vm_last_occurence++;
    if (vm_last_occurence>3) {
	XZST_03EcritureTrace( XZSTC_FATAL,"aweb_export_ftp : la memoire partagee est MORTE !");
	return;
    }
  }
  else {
    vm_last_checksum=vl_nb->Checksum;
    vm_last_occurence=0;
  }
  

  /*generation des fichiers*/
 
 if (!vm_go)
  {   XZST_03EcritureTrace( XZSTC_WARNING,"Out : aweb_export_ftp : vm_go=XDC_FAUX");
      vm_go=XDC_VRAI;
   }

else
{	XZST_03EcritureTrace( XZSTC_WARNING,"Out : aweb_export_ftp : vm_go=XDC_VRAI");
      vm_go=XDC_FAUX;
      aweb_ftp();
}

	/* armement du timer de programmation */
	/* ---------------------------------- */
	vm_HorodateSec+=vm_frequence;
#ifdef _HPUX_SOURCE
	 XZSM_02AnnulerProgrammation( "cle_timer_web",NULL,NULL);
         if (XZSM_01DemanderProgrammation(	vm_HorodateSec,
						0,
						XDG_AW_TIMER,
						XDM_AW_TIMER,
						"cle_timer_web",
						NULL) != XDC_OK)
#else
	 XZSM_02AnnulerProgrammation( "cle_timer_web",(char *) NULL,0);
         if (XZSM_01DemanderProgrammation(	vm_HorodateSec,
						0,
						XDG_AW_TIMER,
						XDM_AW_TIMER,
						"cle_timer_web",
						0) != XDC_OK)
#endif
	{
	/* Ecriture Trace */
        XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de donnees web non declenchee. \n " );
	} 
	else
	  XZST_03EcritureTrace( XZSTC_WARNING, "XZSM_01DemanderProgrammation OK %d",vm_frequence);

}


/*A
*******************************************
* Fonction qui retourne la date courante
*******************************************
* Arguments : Aucun
*******************************************/

char * aweb_get_heure()
{
XZSMT_Horodate  vl_Horodate     = "";
double          vl_HorodateSec  = 0.0;
int             vl_JourSemaine  = 0;

	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_HorodateSec);
	XZSM_11ConversionHorodate(vl_HorodateSec, &vm_HorodateZero);
	return(vm_HorodateZero);
}



int aweb_export_charge_textes(XDY_Mot va_type, char *va_texte_web,XDY_Octet va_priorite,char *va_icone, XDY_Octet va_type_web)
{
  XZST_03EcritureTrace(XZSTC_INTERFACE,"aweb_export_charge_textes: %d %s %d %s %d",
				va_type,va_texte_web,va_priorite,va_icone,va_type_web);
  tg_types_fmc[va_type-1].Priorite=va_priorite;
  tg_types_fmc[va_type-1].Type_Web=va_type_web;
  strcpy(tg_types_fmc[va_type-1].Texte_Web,va_texte_web);;
  strcpy(tg_types_fmc[va_type-1].Icone,va_icone);
  return (XDC_OK);
}
