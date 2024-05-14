/* Fichier : @(#)atdp_export.c	1.1   Release : 1.1   Date : 11/17/08	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : atdp_export.c			*
*************************************************
* DESCRIPTION :
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
*************************************************
* HISTORIQUE :
*
* JMG	16/01/01 : creation 1.1 DEM 837
* SFO   06/10/14 : Adaptation pour compilation sous linux
* JMG	15/12/15 : ajout export TDP vers ASF datex v2.3 1.3 DEM 1151
* JMG	05/04/16 : ajout import TDP des ASF 1.4
* JMG	05/09/18 : ajout import TDP de marseille LOT25 DEM1304 1.5
************************************************/
#include "atdp_export.h"


#define CO_FIC_FREQUENCE	XDC_PATH_STRADA "/frequence.txt"
#define CO_REP_OUT		XDC_PATH_STRADA "/atdp/"
#define CO_DELAI_FREQUENCE	360
#define CO_FIC_PARAM		XDC_PATH_STRADA	"/zdp_cci.txt"
#define CO_FIC_TDPASF_STATIQUE	XDC_PATH_STRADA "/escota_TT_Data.xml"
#define CO_FIC_TDPASF_DYN	XDC_PATH_STRADA	"/escota_TT_Sections.xml"

/* definition des variables locales au module */
static char *	version = "@(#)atdp_export.c	1.1 1.1 11/17/08 : commentaire" ;
static char chemin_export[80];

static FILE *pm_fd;

static XZLGT_Liste_Zdp	*vg_zdp;

XZLGT_Liste_Zdp vm_noeud;



/*******************************************
* Fonction d'initialisation pour la tache
* Elle lit les fichiers de parametrage
* et arme les deux timers :
* - le timer de programmation qui envoie 
* un datagroup
* - le timer de purge
*******************************************
* Arguments : Aucun
*******************************************/

int atdp_export_init()
{
XZSMT_CleProg		vl_cle;
XZSCT_Datagroup 	vl_Datagroup ;
int 			vl_JourSemaine;
double 			vl_HorodateSec; 
XZSCT_NomMachine 	pl_NomMachine	= "\0";
T_BOOL 			vl_etat;
FILE 			*pl_fp;
char			pl_param[200];
char			vl_cmd[200];
char			pl_ligne[200];
XDY_Entier		vl_val;
#ifdef _HPUX_SOURCE
int			vl_frequence=NULL;
#else
int			vl_frequence=0;
#endif
int			vl_station;
int			vl_sens;
XZIAT_destinataire	vl_destinataire;
char			pl_commande[150]="";
char			pl_erreur[200]="";
int			vl_code_retour;
int			vl_numero,vl_i;
XZLGT_Nombre_Mem	*vl_nombre_mem;
XZLGT_Liste_Zdp       *vl_liste;
int vl_indice;


	XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : atdp_export_init" );
 	
  	/* Recuperation du nom de la machine locale */
	XZSC_07NomMachine(pl_NomMachine) ;
 	
  	/* Lecture de la frequence d'exportation des donnees vers la CCI */
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
			if ( !strcmp( pl_param, "tatdp.x") ) {
				vl_frequence = vl_val;
				break;			
			}
		}
		else
			XZST_03EcritureTrace(   XZSTC_WARNING, "le fichier %s n'a pas le format demande !"
							,CO_FIC_FREQUENCE );	
	
	}

#ifdef _HPUX_SOURCE
	if (vl_frequence==NULL) {
#else
	if (vl_frequence==0) {
#endif		
	  XZST_03EcritureTrace(   XZSTC_WARNING, "pas de frequence dans le fichier %s : utilisation freq par default %d"
	  				       , CO_FIC_FREQUENCE
	  				       , CO_DELAI_FREQUENCE);
	  vl_frequence=	CO_DELAI_FREQUENCE;
	}			    
	  				       
	  				       
  	fclose(pl_fp);
  	

  	/*A
	* Initialisation de la callback du timer de programmation
	* -------------------------------------------------------
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ATDP_TIMER), atdp_export_ftp,(T_CB_ARG)NULL) == NULL) 
	{	
                XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AW_TIMER non cree.",version );
		return XDC_NOK;
	}
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ATDP_TIMER_ASF), atdp_import_ftp,(T_CB_ARG)NULL) == NULL) 
	{	
                XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AW_TIMER non cree.",version );
		return XDC_NOK;
	}
	
 	XZSM_00InitServices();

	/*A Lecture horodate courante systeme */
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
   
	   
	sprintf( vl_cle , "cle_timer_cci" );
   
	/*A Abonnement au datagroup pour que le timer nous envoie les messages */
	if (! XDG_JoindreDG(XDG_ATDP_TIMER) )
	{	
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING, 
				"Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
			return XDC_NOK;
	}	
   
	/* armement du timer de programmation */
	/* ---------------------------------- */
	vl_HorodateSec+=vl_frequence;
#ifdef _HPUX_SOURCE
	if (XZSM_01DemanderProgrammation(	vl_HorodateSec,
						vl_frequence,
						XDG_ATDP_TIMER,
						XDM_ATDP_TIMER,
						vl_cle,
						NULL) != XDC_OK)
#else
	if (XZSM_01DemanderProgrammation(	vl_HorodateSec,
						vl_frequence,
						XDG_ATDP_TIMER,
						XDM_ATDP_TIMER,
						vl_cle,
						0) != XDC_OK)
#endif
	{
	/* Ecriture Trace */
        XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de donnees web non declenchee. \n " );
        return XDC_NOK;
	} 
	if (XZSM_01DemanderProgrammation(	0,
						60,
						XDG_ATDP_TIMER,
						XDM_ATDP_TIMER_ASF,
						"cle_import_asf",
						0) != XDC_OK)
	{
	/* Ecriture Trace */
        XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de donnees web non declenchee. \n " );
        return XDC_NOK;
	} 
	
	/*suppression d'eventuels anciens fichiers a transmettre*/
	sprintf(pl_ligne,"rm %s/*.TXT %s/*.txt",CO_REP_OUT,CO_REP_OUT);
	system(pl_ligne);

	/*initialisation de la memoire partagee*/
	if (XZLG994_Accede_Memoire_Partagee_Nombre() != XDC_OK) {
	  XZST_03EcritureTrace( XZSTC_WARNING, "atdp_export_init: pb appel a XZLG994_Accede_Memoire_Partagee_Nombre");
	  return (XDC_NOK);
	}

	/*recupere le semaphore de tagen*/
	if (XZLG503_SemGet("/produits/migrazur/appliSD/exec/tagen.x",XZLGC_SEM_TDP) != XDC_OK) {
	  XZST_03EcritureTrace( XZSTC_WARNING, "atdp_export_init: pb appel a XZLG503_SemGet");
	  return (XDC_NOK);
	}

	/*recupere le nombre de ZDP*/
	vl_nombre_mem = XZLG993_Get_Nombre();

	if (XZLG995_Accede_Memoire_Partagee_TDP() != XDC_OK) {
	  XZST_03EcritureTrace( XZSTC_WARNING, "atdp_export_init: pb appel a XZLG995_Accede_Memoire_Partagee_TDP");
	  return (XDC_NOK);
        }
	vg_zdp=XZLG992_Get_ZDP();

	vl_liste=vg_zdp;
	for (vl_indice=0;vl_indice<vl_nombre_mem->Nombre_ZDP;vl_indice++) {
	  XZST_03EcritureTrace( XZSTC_WARNING, "atdp_export_init: libelle ZDP %s // nom = %s",vl_liste[vl_indice].Zdp.Libelle, vl_liste[vl_indice].Nom);
        }

	XZST_03EcritureTrace( XZSTC_WARNING, "atdp_export_init : nombre ZDP = %d",vl_nombre_mem->Nombre_ZDP);

	/* Lecture des zdp a exporter vers la CCI*/
	if ( (pl_fp = fopen(CO_FIC_PARAM, "r")) == NULL ) {
		XZST_03EcritureTrace(   XZSTC_WARNING, "erreur de lecture du fichier %s !"
					,CO_FIC_PARAM );
		return(XDC_NOK);
	}
	
       /* On lit les lignes du fichier frequence.txt une par une */
       /* ------------------------------------------------------ */
       while ( fgets(pl_ligne, 200, pl_fp) != NULL) {
	      /* On ignore les lignes commencant par un # */
	      if ( pl_ligne[0] == '#' )
		       continue;

	      /* Pour les autres lignes on recherche la frequence de notre tache */
	      if (sscanf(pl_ligne,"%d", &vl_numero)==1){
		/*on memorise qu on doit l emettre*/
		if (XZLG103_FiltreZone(vl_numero,TRUE)!= XDC_OK) {
		  XZST_03EcritureTrace(   XZSTC_WARNING,"atdp_export_init : pb appel a XZLG103_FiltreZone");
		}
	      } 
	      else
		       XZST_03EcritureTrace(   XZSTC_WARNING, "le fichier %s n'a pas le format demande !"
			       ,CO_FIC_FREQUENCE );
	}

	 XZST_03EcritureTrace(   XZSTC_WARNING, "XML %d",vl_nombre_mem->Nombre_ZDP+1);

	atdp_xml_init(vl_nombre_mem->Nombre_ZDP);

	 /*on lit au demarrage le fichier ASF_TT_Sections.xml*/
	         atdp_xml_decode_statique();

	/* Ecriture Trace */
	XZST_03EcritureTrace( XZSTC_WARNING, "OUT : atdp_export_init" );

	atdp_export_ftp();
	return XDC_OK;

}

void atdp_import_ftp(){

  XZST_03EcritureTrace( XZSTC_WARNING, "atdp_import_ftp");
  /*import ASF*/
  atdp_xml_decode_dyn();

  /*import marseille*/
  atdp_xml_decode_dyn_marseille();
}

/*A
*******************************************
* Fonction call back qui reagit au datagroup
* definit dans le timer de programmation
*******************************************
* Arguments : Aucun
*******************************************/

void atdp_export_ftp()
{
  int vl_cr;

  /* Ecriture Trace */
  XZST_03EcritureTrace( XZSTC_FONCTION, "In : atdp_export_ftp");

  /*bloque le semaphore*/
  /*XZLG500_SemCall(-1,XZLGC_SEM_TDP);*/

  /*generation des fichiers*/
  atdp_ftp();

  /*libere le semaphore*/
  /*XZLG500_SemCall(1,XZLGC_SEM_TDP);*/
}


/*A
*******************************************
* Fonction qui retourne la date courante
*******************************************
* Arguments : Aucun
*******************************************/

char * atdp_get_heure()
{
static XZSMT_Horodate	vl_HorodateZero = "";
XZSMT_Horodate  vl_Horodate     = "";
double          vl_HorodateSec  = 0.0;
int             vl_JourSemaine  = 0;

	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_HorodateSec);
	XZSM_11ConversionHorodate(vl_HorodateSec, &vl_HorodateZero);
	return(vl_HorodateZero);
}

