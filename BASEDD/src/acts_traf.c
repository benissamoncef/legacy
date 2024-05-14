/* Fichier : acts_traf.c   Release : 1.1   Date : 05/15/97	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : acts_traf.c				*
*************************************************
* DESCRIPTION :
* module externe de la tache client trafic
* pour le PC STRADA
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
*************************************************
* HISTORIQUE :
*
* PENOT	05 Mai 1997 : Creation 			V1.1
* PENOT 30 Sep 1997 : Strada 2eme generation	V1.2
* Niepceron 	05 Jan 1998 : intergestionnaire dem/1724 v1.4
* Niepceron 	22 Mar 1999 : correction + suppression traces  v1.5
* Niepceron     09 Aou 1999 : Modif catalog pour Strada-Net V2 dem/1767 v1.6
* Niepceron     12 Avr 2006 : correction classe veh v1.8 DEM569
* SFO           06/10/14 : Adaptation pour compilation sous linux
************************************************/


#include "acts_traf.h"

/* definition des constantes */

#define CO_DELAI_FREQUENCE	10
#define CO_FIC_FREQUENCE	"frequence.txt"

#define CO_FIC_EQTCAT		XDC_PATH_STRADA "/cat_eqt_strada.txt"
#define CO_FIC_EQTSTR		"cat_eqt_strada.txt"
#define CO_FIC_STATIONS		"eqt_strada.txt"

#define	CO_MAX_CARACTERE	80
#define TAILLE 			200

#define C_PATH XDC_PATH_STRADA "/trafin/"
#define	C_SCRIPT_FTP_CAT	XDC_PATH_STRADA "/ftp/catc_script_get"

#define CO_PATH_CATALOG		XDC_PATH_STRADA "/catin/"
#define C_PATH_ERR		XDC_PATH_STRADA "/trafinerr/"

#define SG_PING         "/etc/ping"
#define SG_TO_PING      8
#define SG_BYTES_PING   64
#define SG_COUNT_PING   1
#define SG_TOKEN_TST    "64"
#define C_PC_STRADA		"STRADA"

/* dÅfinition des variables globales È l'application */

liste_eqt 	pm_eqt = NULL; /* liste utilisee pour stocker les codes equipement 	*/

char		pm_file[100];  /* nom courant du fichier traite 			*/

/* definition des variables locales au module */

static char *	version = "@(#)acts_traf.c	1.1 05/15/97 : commentaire" ;

/* + les stations*/

/* declaration de fonctions internes */
int acts_traf_init();
int acts_traf_ftp();
int acts_traf_decode_readfile_update(char *pa_name, XDY_Horodate va_Horodate, XDY_Sens va_Sens, 
	XDY_Voie va_Voie, XDY_Octet va_TypeDonnees, int va_Donnees, int va_Unite, int va_Classe, int va_frequence,loc_rds pa_loc);
int acts_traf_decode_readfile (char *);
void acts_traf_decode_XZIS22();
void acts_traf_decode();
void acts_traf_freemem();
int acts_traf_ftpdel(); 
void acts_import_cat_erreur();


/* definition de fonctions externes */

/*A
*******************************************
* Fonction d'initialisation pour la tache
* serveur de donnees trafic
* Elle lit les fichiers de parametrage
* et arme les deux timers :
* - le timer de programmation qui envoie 
* un datagroup
* - le timer de purge
*******************************************
* Arguments : Aucun
*******************************************/

int acts_traf_init() {

XZSMT_CleProg		vl_cle;
XZSCT_Datagroup 	vl_Datagroup ;
int 			vl_JourSemaine;
double 			vl_HorodateSec; 
XZSCT_NomMachine 	pl_NomMachine	= "\0";
T_BOOL 			vl_etat;
FILE 			*pl_fp;
char			pl_param[CO_MAX_CARACTERE];
char			pl_ligne[CO_MAX_CARACTERE];
char 			vl_str[TAILLE];
int			vl_val;
int			vl_code;
int			vl_frequence = 0; 
liste_eqt		pl_eqt;



	
	XZST_03EcritureTrace(   XZSTC_INFO, "IN  : acts_traf_init" );
 	
  	/* Recuperation du nom de la machine locale */
	XZSC_07NomMachine(pl_NomMachine) ;
  	
  	/* Lecture de la frequence d'inportation des donnees du PC Strada */
  	sprintf(vl_str,"%s/%s",XDC_PATH_STRADA,CO_FIC_FREQUENCE);
	if ( (pl_fp = fopen(vl_str, "r")) == NULL )
	{
		XZST_03EcritureTrace(   XZSTC_WARNING, "IN : acts_traf_init : erreur de lecture du fichier %s !", CO_FIC_FREQUENCE);
		return;
	}	

	/* On lit les lignes du fichier une par une */
	while ( fgets(pl_ligne, CO_MAX_CARACTERE, pl_fp) != NULL)
	{
		/* On ignore les lignes commencant par un # ou ne contenant rien */
		if ( pl_ligne[0] == '#' || pl_ligne[0] == '\n' ) 
		{
			continue;
		}

		/* Pour les autres lignes on recherche la frequence de notre tache */
		if(sscanf(pl_ligne,"%[^;];%d", pl_param, &vl_val) == 2) {
#ifdef _HPUX_SOURCE
			if ( strcmp( pl_param, "tacts.x") == NULL ) {
#else
			if ( strcmp( pl_param, "tacts.x") == 0 ) {
#endif
				vl_frequence = vl_val;
				break;			
			}
		}
		else {
			XZST_03EcritureTrace(   XZSTC_WARNING, "IN : acts_traf_init : Le fichier %s n'a pas le bon format !", CO_FIC_FREQUENCE);
		}
	}	
	
	if (! vl_frequence) {	/* dans le cas ou le fichier frequence.txt n'a pas de valeur on l'initialise avec 6 minutes */ 
		XZST_03EcritureTrace(   XZSTC_WARNING, "IN : acts_traf_init : La frequence du client Strada donnees n'a pu etre lue dans le fichier %s !", CO_FIC_FREQUENCE);
		vl_frequence = CO_DELAI_FREQUENCE;
	}

  	fclose(pl_fp);
  	
XZST_03EcritureTrace(   XZSTC_WARNING, "IN : acts_traf_init : La frequence est %d", vl_frequence);


  	/* Lecture du fichier eqt_strada.txt et initialisation de la liste chainee contenant les codes equipement */
  	sprintf(vl_str,"%s/%s",XDC_PATH_STRADA,CO_FIC_STATIONS);
	if ( (pl_fp = fopen(vl_str, "r")) == NULL ) {
		XZST_03EcritureTrace(   XZSTC_WARNING, "IN : acts_traf_init : erreur de lecture du fichier %s !", CO_FIC_STATIONS);
		return;
	}	


	/* On lit les lignes du fichier une par une */
	while (fgets(pl_ligne, CO_MAX_CARACTERE, pl_fp) != NULL) {
		/* On ignore les lignes commencant par un # */
		if (  pl_ligne[0] == '#') {
			continue;
		}
		
		/* Pour les autres lignes on cree la liste chainee */

		if(sscanf(pl_ligne,"%d\t%s",&vl_code ,vl_str)==2) {
XZST_03EcritureTrace(   XZSTC_INFO, "IN : acts_traf_init : initialisation liste chainee: %d   %s", vl_code,vl_str); 
			
			pl_eqt            = (liste_eqt) malloc (sizeof(struct code_eqt));
			pl_eqt -> suivant = pm_eqt;
			pl_eqt -> string  = strdup(vl_str);
			pl_eqt -> number  = vl_code;
			
			/* initialisation des autres champs */
			pl_eqt -> h_1    = XDC_RADT_INCONNU;
			pl_eqt -> h_1_b  = FALSE;
			pl_eqt -> h_2    = XDC_RADT_INCONNU;
			pl_eqt -> h_2_b  = FALSE;
			
			pl_eqt -> q_1_b  = FALSE;
			pl_eqt -> q_2_b  = FALSE;
			pl_eqt -> v_1_b  = FALSE;
			pl_eqt -> v_2_b  = FALSE;
			pl_eqt -> to_1_b = FALSE;
			pl_eqt -> to_2_b = FALSE;
			
			pl_eqt -> q_1  = -1;
			pl_eqt -> q_2  = -1;
			pl_eqt -> v_1  = -1;
			pl_eqt -> v_2  = -1;
			pl_eqt -> to_1 = -1;
			pl_eqt -> to_2 = -1;
						
			pl_eqt -> e_1  = FALSE;
			pl_eqt -> e_2  = FALSE;

			pm_eqt = pl_eqt;
		}
	}	
	
  	fclose(pl_fp);
  	
	
	
	XZST_03EcritureTrace(   XZSTC_INFO, "IN : acts_traf_init : Lecture du fichier %s",CO_FIC_STATIONS);
  	
  	
  	/*A
	* Initialisation de la callback du timer de programmation 
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AG_IMPCAT), acts_import_cat,(T_CB_ARG)NULL) == NULL)  {	
                XZST_03EcritureTrace( XZSTC_FATAL,"IN : acts_traf_init : %s : Callback sur XDM_AG_IMPCAT non cree.",version );
		return ;
	}
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AG_EXPCATC), acts_export_cat,(T_CB_ARG)NULL) == NULL)  {	
                XZST_03EcritureTrace( XZSTC_FATAL,"IN : acts_traf_init : %s : Callback sur XDM_AG_EXPCATC non cree.",version );
		return ;
	}
	
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_TIMER_CLT_RADT), acts_traf_decode,(T_CB_ARG)NULL) == NULL)  {	
                XZST_03EcritureTrace( XZSTC_FATAL,"IN : acts_traf_init : %s : Callback sur XDM_AS_TIMER_CLT_RADT non cree.",version );
		return ;
	}
  	
	/*A initialisation des timers */
	/* XZSM_00InitServices(); */
	
	/*A Lecture horodate courante systeme */
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
   
	sprintf( vl_cle , "cle_timer_client_radt" );

	/*A on forme le datagroup de la tache */
	XDG_EncodeDG2(vl_Datagroup , XDG_AS_TIMER_CLT_RADT , pl_NomMachine );
   
	/*A Abonnement au datagroup pour que le timer nous envoie les messages */
	if (! XDG_JoindreDG(vl_Datagroup) ) {	
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING, 
				"IN : acts_traf_init : Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
			return;
	}	
	if (! XDG_JoindreDG(XDG_AG_IMPCAT) ) {	
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING, 
				"IN : acts_traf_init : Abonnement sur %s non effectuee. \n " ,XDG_AG_IMPCAT);
			return;
	}	
	if (! XDG_JoindreDG(XDG_AG_EXPCATC) ) {	
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING, 
				"IN : acts_traf_init : Abonnement sur %s non effectuee. \n " ,XDG_AG_EXPCATC);
			return;
	}	
   
	/* armement du timer de programmation */
#ifdef _HPUX_SOURCE
	if (XZSM_01DemanderProgrammation(	vl_HorodateSec,
						vl_frequence,
						vl_Datagroup,
						XDM_AS_TIMER_CLT_RADT,
						vl_cle,
						NULL) != XDC_OK) {
#else
	if (XZSM_01DemanderProgrammation(	vl_HorodateSec,
						vl_frequence,
						vl_Datagroup,
						XDM_AS_TIMER_CLT_RADT,
						vl_cle,
						0) != XDC_OK) {
#endif
		/* Ecriture Trace */
        	XZST_03EcritureTrace( XZSTC_WARNING, "IN : acts_traf_init : Programmation du timer de donnees radt non declenchee. \n " );
        	return ;
	} 

	/* Ecriture Trace */
	XZST_03EcritureTrace( XZSTC_INFO, "OUT : acts_traf_init" );
	return ;

}


/*A
*******************************************
* Fonction qui recupere les donnees trafic
* sur le PC Strada et les place dans le 
* repertoire client trafic
*******************************************
* Arguments : Aucun
*******************************************/

int acts_traf_ftp() {
char			vl_ligne[CO_MAX_CARACTERE];
char			pl_ligne[CO_MAX_CARACTERE];
FILE			*pl_fp;

	
	/* execution d'un script de recuperation des fichiers */
/* construit la commande test reseau de la machine demandee */
  sprintf(pl_ligne,"%s %s %u %u",SG_PING,C_PC_STRADA,
				SG_BYTES_PING, SG_COUNT_PING);

  XZST_03EcritureTrace(XZSTC_INFO,"ligne %s",pl_ligne);

  /* execute la commande, */
  if ((pl_fp = popen(pl_ligne,"r")) != (FILE *)NULL)
   {
                /* recupere le resultat de cette commande */
                fgets(vl_ligne,sizeof(vl_ligne)-1,pl_fp) ;
                fgets(vl_ligne,sizeof(vl_ligne)-1,pl_fp) ;
                pclose(pl_fp) ;

		XZST_03EcritureTrace(XZSTC_INFO,"ligne ping = %s",vl_ligne);

		/*tout va bien*/
                if (strstr(vl_ligne,SG_TOKEN_TST) != NULL)
                {
			 /*FTP*/
			system(C_SCRIPT_TRAF); 
 		}
                else
                {
			XZST_03EcritureTrace(   XZSTC_WARNING, "acts_traf_ftp : PC strada HS");
			return (XDC_NOK);
                }
    }
    else
   {
                XZST_03EcritureTrace(XZSTC_WARNING,
                     "erreur execution de %s",pl_ligne);
			return (XDC_NOK);
    }
  

	
	/* ecriture trace */
	XZST_03EcritureTrace(   XZSTC_INFO, "IN  : acts_traf_ftp" );
	
	return (XDC_OK);
}


/*A
*******************************************
* Fonction appelee lorsqu'un fichier 
* donnees trafic du PC Strada a ete
* traite. Cette fonction permet d'effacer alors
* le fichier sur le PC Strada
*******************************************
* Arguments : Aucun
*******************************************/

int acts_traf_ftpdel() {
	
	
	/* ecriture trace */
	XZST_03EcritureTrace(   XZSTC_INFO, "IN  : acts_traf_ftpdel" );
	
	return (XDC_OK);
}


/*A
********************************************
* fonction  de acts_traf_decode_readfile permettant  
* de faire un update de la liste des equipements
*******************************************
* Arguments : le nom du fichier
*******************************************/
int acts_traf_decode_readfile_update(char *pa_name, XDY_Horodate va_Horodate, XDY_Sens va_Sens, 
					XDY_Voie va_Voie, XDY_Octet va_TypeDonnees, int va_Donnees, int va_Unite, int va_Classe, int va_frequence,loc_rds pa_loc) {

liste_eqt	pl_eqt = pm_eqt;
	
	if (va_TypeDonnees==XDC_RADT_V)
		if (va_Unite!= XINC_VITESSE_K) {
          		XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile_update : incoherence unites de mesure fichier %s equipement %s",pm_file,pa_name);
          		return (FALSE);
          	}
	
	if (va_TypeDonnees==XDC_RADT_Q)
		if (va_Unite!= XINC_VITESSE_V) {
          		XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile_update : incoherence unites de mesure fichier %s equipement %s",pm_file,pa_name);
          		return (FALSE);
          	}
          	
         if (va_TypeDonnees==XDC_RADT_TO)
		if (va_Unite!= XINC_VITESSE_P) {
          		XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile_update : incoherence unites de mesure fichier %s equipement %s",pm_file,pa_name);
          		return (FALSE);
          	}


 	while (strcmp(pl_eqt -> string , pa_name))  {
         	if ((pl_eqt = pl_eqt -> suivant) == NULL) {
          		XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile_update : l'equipement %s n'est pas reference",pa_name);
          		return (-1);
          	}
	}

							
	/*                          cas ou l'horodate n'a pas change                     */
			
	/* voie sens nord */
        if (va_Sens == XDC_SENS_NORD && pl_eqt -> h_1 == va_Horodate) {
		pl_eqt -> e_1 = TRUE;
				
		if (va_TypeDonnees == XDC_RADT_Q) {
         		pl_eqt -> q_1        = va_Donnees;
          		pl_eqt -> q_1_b      = XDC_RADT_CORRECT;
          		pl_eqt -> q_1_classe = va_Classe;
          		pl_eqt -> q_f_1      = va_frequence;
          		
          		pl_eqt -> q_1_loc.option = pa_loc.option;
        		
        		pl_eqt -> q_1_loc.l02    = pa_loc.l02;
        		pl_eqt -> q_1_loc.l05    = pa_loc.l05;
        		
        		strcpy(pl_eqt -> q_1_loc.l01,pa_loc.l01);
        		strcpy(pl_eqt -> q_1_loc.l03,pa_loc.l03);
        		strcpy(pl_eqt -> q_1_loc.l04,pa_loc.l04);
        		strcpy(pl_eqt -> q_1_loc.l06,pa_loc.l06);

          	}
          			
          	else if (va_TypeDonnees == XDC_RADT_V) {
          		pl_eqt -> v_1        = va_Donnees;
          		pl_eqt -> v_1_b      = XDC_RADT_CORRECT;
           		pl_eqt -> v_1_classe = va_Classe;
          		pl_eqt -> v_f_1      = va_frequence;
        									
        		pl_eqt -> v_1_loc.option = pa_loc.option;
        		
        		pl_eqt -> v_1_loc.l02    = pa_loc.l02;
        		pl_eqt -> v_1_loc.l05    = pa_loc.l05;
        		
        		strcpy(pl_eqt -> v_1_loc.l01,pa_loc.l01);
        		strcpy(pl_eqt -> v_1_loc.l03,pa_loc.l03);
        		strcpy(pl_eqt -> v_1_loc.l04,pa_loc.l04);
        		strcpy(pl_eqt -> v_1_loc.l06,pa_loc.l06);
          		
         	}
          				
          	else {
          		pl_eqt -> to_1        = va_Donnees;
          		pl_eqt -> to_1_b      = XDC_RADT_CORRECT;
          		pl_eqt -> to_1_classe = va_Classe;
          		pl_eqt -> to_f_1      = va_frequence;
          		
          		pl_eqt -> to_1_loc.option = pa_loc.option;
        		
        		pl_eqt -> to_1_loc.l02    = pa_loc.l02;
        		pl_eqt -> to_1_loc.l05    = pa_loc.l05;
        		
        		strcpy(pl_eqt -> to_1_loc.l01,pa_loc.l01);
        		strcpy(pl_eqt -> to_1_loc.l03,pa_loc.l03);
        		strcpy(pl_eqt -> to_1_loc.l04,pa_loc.l04);
        		strcpy(pl_eqt -> to_1_loc.l06,pa_loc.l06);
          		

          	}
          			
          	return;
          }
          		
          /* voie sens sud */
          else if (va_Sens == XDC_SENS_SUD && pl_eqt -> h_2 == va_Horodate) {
			pl_eqt -> e_2 = TRUE;

	
			if (va_TypeDonnees == XDC_RADT_Q) {
          			pl_eqt -> q_2        = va_Donnees;
          			pl_eqt -> q_2_b      = XDC_RADT_CORRECT;
          			pl_eqt -> q_2_classe = va_Classe;
          			pl_eqt -> q_f_2      = va_frequence;
          			
          			pl_eqt -> q_2_loc.option = pa_loc.option;
        		
        			pl_eqt -> q_2_loc.l02    = pa_loc.l02;
        			pl_eqt -> q_2_loc.l05    = pa_loc.l05;
        		
        			strcpy(pl_eqt -> q_2_loc.l01,pa_loc.l01);
        			strcpy(pl_eqt -> q_2_loc.l03,pa_loc.l03);
        			strcpy(pl_eqt -> q_2_loc.l04,pa_loc.l04);
        			strcpy(pl_eqt -> q_2_loc.l06,pa_loc.l06);
          		}
          			
          		else if (va_TypeDonnees == XDC_RADT_V) {
          			pl_eqt -> v_2        = va_Donnees;
          			pl_eqt -> v_2_b      = XDC_RADT_CORRECT;
          			pl_eqt -> v_2_classe = va_Classe;
          			pl_eqt -> v_f_2      = va_frequence;
          			
          			pl_eqt -> v_2_loc.option = pa_loc.option;
        		
        			pl_eqt -> v_2_loc.l02    = pa_loc.l02;
        			pl_eqt -> v_2_loc.l05    = pa_loc.l05;
        		
        			strcpy(pl_eqt -> v_2_loc.l01,pa_loc.l01);
        			strcpy(pl_eqt -> v_2_loc.l03,pa_loc.l03);
        			strcpy(pl_eqt -> v_2_loc.l04,pa_loc.l04);
        			strcpy(pl_eqt -> v_2_loc.l06,pa_loc.l06);          			
          		}
          				
          		else {
          			pl_eqt -> to_2        = va_Donnees;
          			pl_eqt -> to_2_b      = XDC_RADT_CORRECT;
          			pl_eqt -> to_2_classe = va_Classe;
            			pl_eqt -> to_f_2      = va_frequence;
           			
          			pl_eqt -> to_2_loc.option = pa_loc.option;
        		
        			pl_eqt -> to_2_loc.l02    = pa_loc.l02;
        			pl_eqt -> to_2_loc.l05    = pa_loc.l05;
        		
        			strcpy(pl_eqt -> to_2_loc.l01,pa_loc.l01);
        			strcpy(pl_eqt -> to_2_loc.l03,pa_loc.l03);
        			strcpy(pl_eqt -> to_2_loc.l04,pa_loc.l04);
        			strcpy(pl_eqt -> to_2_loc.l06,pa_loc.l06);
       			}
          			
          		return ;
          	}


	   /*                               cas ou l'horodate a change                          */

	   /* voie sens nord */
          if (va_Sens == XDC_SENS_NORD) {
           	if (pl_eqt -> h_1_b == FALSE || (va_Horodate>pl_eqt -> h_1)) {
          		pl_eqt -> e_1 = TRUE;
          		
          		pl_eqt -> h_1 = va_Horodate;
          		pl_eqt -> h_1_b = TRUE;
	
          		pl_eqt -> q_1_b    = FALSE;
          		pl_eqt -> v_1_b    = FALSE;
          		pl_eqt -> to_1_b   = FALSE;
          		pl_eqt -> q_1      = -1;
            		pl_eqt -> q_f_1    = -1;
        		pl_eqt -> v_1      = -1;
        		pl_eqt -> v_f_1    = -1;
          		pl_eqt -> to_1     = -1;
          		pl_eqt -> to_f_1   = -1;
          			
          		if (va_TypeDonnees == XDC_RADT_Q){
          			pl_eqt -> q_1    = va_Donnees;
          			pl_eqt -> q_1_b  = XDC_RADT_CORRECT;
          			pl_eqt -> q_1_classe = va_Classe;
          			pl_eqt -> q_f_1      = va_frequence;
          										
        			pl_eqt -> q_1_loc.option = pa_loc.option;
        		
        			pl_eqt -> q_1_loc.l02    = pa_loc.l02;
        			pl_eqt -> q_1_loc.l05    = pa_loc.l05;
        		
        			strcpy(pl_eqt -> q_1_loc.l01,pa_loc.l01);
        			strcpy(pl_eqt -> q_1_loc.l03,pa_loc.l03);
        			strcpy(pl_eqt -> q_1_loc.l04,pa_loc.l04);
        			strcpy(pl_eqt -> q_1_loc.l06,pa_loc.l06);
          		}
          			
          		else if (va_TypeDonnees == XDC_RADT_V) {
          			pl_eqt -> v_1   = va_Donnees;
				pl_eqt -> v_1_b = XDC_RADT_CORRECT;
				pl_eqt -> v_1_classe = va_Classe;
          			pl_eqt -> v_f_1      = va_frequence;
          										
        			pl_eqt -> v_1_loc.option = pa_loc.option;
        		
        			pl_eqt -> v_1_loc.l02    = pa_loc.l02;
        			pl_eqt -> v_1_loc.l05    = pa_loc.l05;
        		
        			strcpy(pl_eqt -> v_1_loc.l01,pa_loc.l01);
        			strcpy(pl_eqt -> v_1_loc.l03,pa_loc.l03);
        			strcpy(pl_eqt -> v_1_loc.l04,pa_loc.l04);
        			strcpy(pl_eqt -> v_1_loc.l06,pa_loc.l06);
          		}
          				
          		else {
          			pl_eqt -> to_1    = va_Donnees;
          			pl_eqt -> to_1_b  = XDC_RADT_CORRECT;
          			pl_eqt -> to_1_classe = va_Classe;

          			pl_eqt -> to_f_1      = va_frequence;	
          									
        			pl_eqt -> to_1_loc.option = pa_loc.option;
        		
        			pl_eqt -> to_1_loc.l02    = pa_loc.l02;
        			pl_eqt -> to_1_loc.l05    = pa_loc.l05;
        		
        			strcpy(pl_eqt -> to_1_loc.l01,pa_loc.l01);
        			strcpy(pl_eqt -> to_1_loc.l03,pa_loc.l03);
        			strcpy(pl_eqt -> to_1_loc.l04,pa_loc.l04);
        			strcpy(pl_eqt -> to_1_loc.l06,pa_loc.l06);
          		}
          			
          		return;
            	}
          }
          
            /* voie sens sud */
            
          if (pl_eqt -> h_2_b == FALSE || (va_Horodate>pl_eqt -> h_2)) {	
          	
            	pl_eqt -> e_2 = TRUE;
            	pl_eqt -> h_2 = va_Horodate;
          	pl_eqt -> h_2_b = TRUE;

            	pl_eqt -> q_2_b  = FALSE;
            	pl_eqt -> v_2_b  = FALSE;
            	pl_eqt -> to_2_b = FALSE;
            	pl_eqt -> q_2      = -1;
            	pl_eqt -> q_f_2    = -1;
		pl_eqt -> v_2      = -1;
		pl_eqt -> v_f_2    = -1;
            	pl_eqt -> to_2     = -1;
            	pl_eqt -> to_f_2   = -1;
            	
          	if (va_TypeDonnees == XDC_RADT_Q) {
          		pl_eqt -> q_2   = va_Donnees;
          		pl_eqt -> q_2_b = XDC_RADT_CORRECT;
          		pl_eqt -> q_2_classe = va_Classe;
          		pl_eqt -> q_f_2      = va_frequence;
          									
        		pl_eqt -> q_2_loc.option = pa_loc.option;
        		
        		pl_eqt -> q_2_loc.l02    = pa_loc.l02;
        		pl_eqt -> q_2_loc.l05    = pa_loc.l05;
        		
        		strcpy(pl_eqt -> q_2_loc.l01,pa_loc.l01);
        		strcpy(pl_eqt -> q_2_loc.l03,pa_loc.l03);
        		strcpy(pl_eqt -> q_2_loc.l04,pa_loc.l04);
        		strcpy(pl_eqt -> q_2_loc.l06,pa_loc.l06);
         	}
          			
          	else if (va_TypeDonnees == XDC_RADT_V) {
          		pl_eqt -> v_2   = va_Donnees;
          		pl_eqt -> v_2_b = XDC_RADT_CORRECT;
          		pl_eqt -> v_2_classe = va_Classe;
          		pl_eqt -> v_f_2      = va_frequence;
          									
        		pl_eqt -> v_2_loc.option = pa_loc.option;
        		
        		pl_eqt -> v_2_loc.l02    = pa_loc.l02;
        		pl_eqt -> v_2_loc.l05    = pa_loc.l05;
        		
        		strcpy(pl_eqt -> v_2_loc.l01,pa_loc.l01);
        		strcpy(pl_eqt -> v_2_loc.l03,pa_loc.l03);
        		strcpy(pl_eqt -> v_2_loc.l04,pa_loc.l04);
        		strcpy(pl_eqt -> v_2_loc.l06,pa_loc.l06);
          	}
          				
          	else {
          		pl_eqt -> to_2   = va_Donnees;
          		pl_eqt -> to_2_b = XDC_RADT_CORRECT;
          		pl_eqt -> to_2_classe = va_Classe;
            		pl_eqt -> to_f_2      = va_frequence;
           									
        		pl_eqt -> to_2_loc.option = pa_loc.option;
        		
        		pl_eqt -> to_2_loc.l02    = pa_loc.l02;
        		pl_eqt -> to_2_loc.l05    = pa_loc.l05;
        		
        		strcpy(pl_eqt -> to_2_loc.l01,pa_loc.l01);
        		strcpy(pl_eqt -> to_2_loc.l03,pa_loc.l03);
        		strcpy(pl_eqt -> to_2_loc.l04,pa_loc.l04);
        		strcpy(pl_eqt -> to_2_loc.l06,pa_loc.l06);
          	}
          			
          	return (TRUE);
          }	
          
          /* l'horodate est postrieure, il ne sera pas pris en compte */
      	  XZST_03EcritureTrace(   XZSTC_INFO, "IN  : acts_traf_decode_readfile_update : l'horodate du fichier %s est anterieur, les informations ne seront pas prises en compte sur l'equipement %s",pm_file, pl_eqt -> string );
	  return;
          	
}

/*A
********************************************
* fonction  de acts_traf_decode permettant de 
* lire un fichier de donnees trafic Strada
*******************************************
* Arguments : le nom du fichier
*******************************************/

int acts_traf_decode_readfile (char *pa_file) {

liste_eqt		pl_eqt;
loc_rds			pl_loc;
XZSMT_Horodate		pl_strdate;
 
FILE 			* pl_fD;
char 			* pl_string;
char			* pl_name;
char 			tl_name[TAILLE];
char 			pl_buffer[TAILLE];
char 			tl_buffer[TAILLE];
char 			tl_localisation[TAILLE];
char 			pl_buftok[TAILLE];
char			pl_mse[6];
char			vl_signe;

int			vl_annee;
int			vl_mois;
int			vl_jour;
int			vl_heure;
int			vl_minute;
int			vl_gmt;
int			vl_frequence;

XDY_Eqt			vl_NumStation;
XDY_Sens          	vl_Sens;
XDY_Voie          	vl_Voie;
XDY_Horodate 		vl_HorodateDebut;
XDY_Octet         	vl_TypeDonnees;

int 			vl_Donnees;
int			vl_Unite;
int			vl_Classe;

int			vl_count;
int			vl_i,vl_j,vl_k,vl_l;

	strcpy(pm_file,pa_file);
	
  	if ((pl_fD = fopen (pa_file,"r")) != NULL) {

XZST_03EcritureTrace(   XZSTC_INFO, "IN  : acts_traf_decode_readfile : lecture du fichier %s",pa_file);
      	
	if ( fgets(pl_buffer,sizeof(pl_buffer),pl_fD) != NULL )     /* leture entete du fichier ligne 1 */
      		{
      			strcpy(pl_buftok,pl_buffer);
      			
      			pl_string = strtok(pl_buftok,"#");
      			
      			if (strcmp(pl_string,"MSE") != 0) {
      				/* le fichier n'a pas le bon format, il ne sera pas pris en compte */
      				XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s n'a pas le bon format MSE",pa_file );
				return XDC_NOK;
      			}
      			
      			pl_string = strtok(NULL,"#");
      			
      			if ( strlen(pl_string) != 5 ) {
      				/* le fichier n'a pas le bon format, il ne sera pas pris en compte */
      				XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s n'a pas le bon format MSE code emetteur %s", pa_file, pl_string);
				return XDC_NOK;
      			}
      				
      			strcpy(pl_mse,pl_string);	
      			
     		}
     		     		
    		if (fgets(pl_buffer,sizeof(pl_buffer),pl_fD) != NULL)    /* leture entete du fichier ligne 2 */
   		{	
   			strcpy(pl_buftok,pl_buffer);
      			
      			pl_string = strtok(pl_buftok,"#");
   		      	pl_string = strtok(NULL,"#");

   			if (strlen(pl_string) != 15) {
      				/* le fichier n'a pas le bon format, il ne sera pas pris en compte */
      				XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s n'a pas le bon format MST (nombre de caracteres)",pa_file );
				return XDC_NOK;
      			}
      			
      			if (sscanf(pl_buffer,"MST#%04d%02d%02d%02d%02d%c%02d#\n", 	&vl_annee,
      										&vl_mois,
      										&vl_jour,
      										&vl_heure,
      										&vl_minute,
      										&vl_signe,
      										&vl_gmt     ) != 7) {
      				/* le fichier n'a pas le bon format, il ne sera pas pris en compte */
      				XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s n'a pas le bon format MST",pa_file );
				return XDC_NOK;
      			}
      			
      			if ( vl_annee < 0 || vl_annee > 2100 ) {
      				/* le fichier n'a pas le bon format, il ne sera pas pris en compte */
      				XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s n'a pas le bon format MST : annee incorrect ",pa_file );
				return XDC_NOK;
      			}
      			if ( vl_mois < 1 || vl_mois > 12 ) {
      				/* le fichier n'a pas le bon format, il ne sera pas pris en compte */
      				XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s n'a pas le bon format MST : mois incorrect ",pa_file );
				return XDC_NOK;
      			}
      			if ( vl_jour < 1 || vl_jour > 31 ) {
      				/* le fichier n'a pas le bon format, il ne sera pas pris en compte */
      				XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s n'a pas le bon format MST : jour incorrect ",pa_file );
				return XDC_NOK;
      			}
      			
      			if ( vl_heure < 0 || vl_heure > 24 ) {
      				/* le fichier n'a pas le bon format, il ne sera pas pris en compte */
      				XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s n'a pas le bon format MST : heure incorrect ",pa_file );
				return XDC_NOK;
      			}
      			
      			if ( vl_minute < 0 || vl_minute > 59 ) {
      				/* le fichier n'a pas le bon format, il ne sera pas pris en compte */
      				XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s n'a pas le bon format MST : minutes incorrect ",pa_file );
				return XDC_NOK;
      			}
      			if ( vl_signe != 'P' && vl_signe != 'N' ) {
      				/* le fichier n'a pas le bon format, il ne sera pas pris en compte */
      				XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s n'a pas le bon format MST : signe incorrect ",pa_file );
				return XDC_NOK;
      			}
      			
      			if ( vl_gmt < 0 || vl_gmt > 24 ) {
      				/* le fichier n'a pas le bon format, il ne sera pas pris en compte */
      				XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s n'a pas le bon format MST : gmt incorrect ",pa_file );
				return XDC_NOK;
      			}
      			
      			
      			sprintf(pl_strdate,"%2d/%2d/%4d %2d:%2d:00", 	vl_jour,
      									vl_mois,
      									vl_annee,
      									vl_heure,
      									vl_minute);
         				
      			 XZSM_13ConversionHorodateSec(pl_strdate,&vl_HorodateDebut);
    /* XZST_03EcritureTrace(   XZSTC_INFO, "IN  : acts_traf_decode_readfile :	horodate : %s : %lf ",pl_strdate,vl_HorodateDebut); */ 
      			 
      			 /* l'horodate est donnee pour GMT = +1 et il faut pouvoir la convertir lorsque celle-ci
      			    est differente */
      			 if (vl_signe == 'N')
      			 	vl_gmt = - vl_gmt;
      			 	
      			 vl_HorodateDebut= vl_HorodateDebut+ (vl_gmt - 1) * 3600 +10; /* 3600 nbre de seconde par heure */
      		
      		}
    
    	  	while (fgets(pl_buffer,sizeof(pl_buffer),pl_fD) != NULL)   /* lecture des donnees trafic du fichier */
        	{
          	/* on ignore les lignes commencant par un # ou ne contenant rien */
          		if ( (pl_buffer[0] == '#') || (pl_buffer[0] == '\n') )
            			continue;
            			
            		strcpy (tl_buffer,pl_buffer);
            		          	

	
            	/* lecture du premier champ   : Equipement                      */			
          		pl_name = strtok (pl_buffer,"#");
          		strcpy(tl_name,pl_name);
          		
          	/* lecture du deuxieme champ, lecture 'non case sensitive'  Type */			
          		pl_string = strtok (NULL,"#");
          		
          		if (!strcasecmp (pl_string,"FLO"))          	/* FLO  ->  XDC_RADT_Q    debit             */
          			vl_TypeDonnees = XDC_RADT_Q;
 
          		else if (!strcasecmp (pl_string,"AVS"))  	/* AVS  ->  XDC_RADT_V    vitesse moyenne   */
          			vl_TypeDonnees = XDC_RADT_V;

          		else if (!strcasecmp (pl_string,"OCC"))		/* OCC  ->  XDC_RADT_TO   taux d'occupation */
          			vl_TypeDonnees =  XDC_RADT_TO;

          		else  {
          			XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s a un ou des champs Type(s) non repertorie(s)",pa_file );
				continue;
          		}
         		
          	/* lecture du troisieme champ  : Sens                            */			
          		pl_string = strtok (NULL,"#");   
        		
          		if (!strcasecmp (pl_string,"P"))	
          			vl_Sens = XDC_SENS_SUD ;
          			
          		else if (!strcasecmp (pl_string,"N"))	
          			vl_Sens = XDC_SENS_NORD ;
          			
          		else
          		{
          			XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s a un ou des champs Sens non repertorie(s)",pa_file );
				continue;
          		}
          		
          		
          	/* lecture du quatrieme champ  : Voie                            */			
          		pl_string = strtok (NULL,"#");    	
          			
          		if (!strcasecmp (pl_string,"*"))	
          			vl_Voie = XDC_RADT_CHAUSSEE;		/*  *  -> XDC_RADT_CHAUSSEE  toute la chaussee  */
          			
         		else if (!strcasecmp (pl_string,"1"))	
          			vl_Voie = XDC_RADT_V1;			/*  1  -> XDC_RADT_V1        voie 1             */
          		
          		else if (!strcasecmp (pl_string,"2"))	
          			vl_Voie = XDC_RADT_V2;			/*  2  -> XDC_RADT_V2        voie 2             */
          		
          		else if (!strcasecmp (pl_string,"3"))	
          			vl_Voie = XDC_RADT_V3;			/*  3  -> XDC_RADT_V3        voie 3             */
          			
          		else if (!strcasecmp (pl_string,"4"))	
          			vl_Voie = XDC_RADT_V4;			/*  4  -> XDC_RADT_V4        voie 4             */
			
          		else {
          			XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s a un ou des champs Voie(s) non repertorie(s)",pa_file );
				continue;
          		}
          		          		
          		/* on ne traite que les donnees chaussee dans Strada 1 */
          		if (strcmp (pl_string,"*")) {
          			XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s comporte des donnees sur des voies independantes (cf 1 2 3 ...)",pa_file );
				continue;
          		}

          	/* lecture du cinquieme champ  : Frequence */
          		pl_string = strtok (NULL,"#");
          		
          		strcpy(pl_buffer , pl_string);


          		if (sscanf(pl_buffer,"%d\0", 	&vl_frequence) != 1) {
          			XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s comporte des donnees sans indication de la frequence",pa_file );
				continue;
          		}
        		
          	/* lecture du sixieme champ  : Unite                 	*/
          		pl_string = strtok (NULL,"#");    	
          			
          		if (!strcasecmp (pl_string,"KMH")) 	
          			vl_Unite = XINC_VITESSE_K;			/*  1  -> KMH km/h	             	*/
          			
         		else if (!strcasecmp (pl_string,"PER"))	
          			vl_Unite = XINC_VITESSE_P;			/*  2  -> PER Pourcentage           	*/
          		
          		else if (!strcasecmp (pl_string,"VHH"))	
          			vl_Unite = XINC_VITESSE_V;			/*  3  -> VHH vehicule par heure      	*/
          		
			
          		else
          		{
          			XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s a un ou des champs Unite(s) non repertorie(s): %s",pa_file ,pl_string);
				continue;
          		}
          	

          	/* lecture du septieme champ  : Classe de vehicule 	*/
			pl_string = strtok (NULL,"#");    	
          			
          		if (!strcasecmp (pl_string,"K")) 	
          			vl_Classe = 1;			/*  1  -> K   tous vehicules	             	*/
          			
         		else if (!strcasecmp (pl_string,"KX1"))	
          			vl_Classe = 2;			/*  2  -> KX1 Vehicules legers           	*/
          		
          		else if (!strcasecmp (pl_string,"KX2"))	
          			vl_Classe = 3;			/*  3  -> KX2 Poids lourds	      		*/
          		
			
          		else
          		{
          			XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : le fichier %s a un ou des champs Classe(s) non repertorie(s)",pa_file );
				continue;
          		}
         
          	
         	/* lecture du huitieme champ  : Donnee                 	*/
          				
          		pl_string = strtok (NULL,"#");
          		
          		if (sscanf(pl_string,"%d", &vl_Donnees) != 1)
          		{
          			XZST_03EcritureTrace(   XZSTC_WARNING, 
          				"IN  : acts_traf_decode_readfile : le fichier %s a un ou des champs Donnee(s) n'ayant pas le bon format",pa_file );
				continue;
          		}
      
          		
          	/* lecture de la localisation RDS optionnelle */
          	vl_count =0;

          	for (vl_i = 0 ; vl_count < 8 ; vl_i ++) {
          		if (tl_buffer[vl_i] == '#')
          			vl_count ++;
          	}


          	for (vl_j = 0 ; tl_buffer[vl_i+vl_j] != '\0' ; vl_j ++)
          	
          		tl_localisation[vl_j] = tl_buffer[vl_i+vl_j];
          		
          	tl_localisation[vl_j] = '\0';
         	
          	if (strlen (tl_localisation) > 7) {
          		if (sscanf(tl_localisation,"%[^#]#%d#%[^#]#%[^#]#%d#%[^#]#\n", 	 pl_loc.l01,
          										&pl_loc.l02,
           										 pl_loc.l03,
           										 pl_loc.l04,
           										&pl_loc.l05,
     											 pl_loc.l06) != 6) {
      				XZST_03EcritureTrace(   XZSTC_INFO, "IN  : acts_traf_decode_readfile : localisation incorrecte");						
          			pl_loc.option = FALSE;
          			continue;
          		}
          		else 
          			pl_loc.option = TRUE;
          	}
          		
          	else {
          		pl_loc.option = FALSE;
            		pl_loc.l01[0] = '\0';
          		pl_loc.l02    = -1;
           		pl_loc.l03[0] = '\0';
			pl_loc.l04[0] = '\0';
          		pl_loc.l05    = -1;
          		pl_loc.l06[0] = '\0';
          	}

          		
        /*  	if (pl_loc.option == TRUE) {
          		XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode_readfile : localisation - %s - %d - %s - %s - %d - %s ", pl_loc.l01,
        									pl_loc.l02,
                								pl_loc.l03,
        									pl_loc.l04,
        									pl_loc.l05,
        									pl_loc.l06);	
        	} */
          		
          	
          	/* up_date de la liste chainee des equipements */
        
/* XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts nom: %s - horodate: %s - sens: %d - voie: %d - type: %d - donnÅe: %d - unitÅ: %d - classe : %d - frequence %d",
        			                  tl_name,pl_strdate,vl_Sens,vl_Voie,vl_TypeDonnees,vl_Donnees,vl_Unite,vl_Classe,vl_frequence); */
        			
          	acts_traf_decode_readfile_update(tl_name,vl_HorodateDebut,vl_Sens,vl_Voie,vl_TypeDonnees,vl_Donnees,vl_Unite,vl_Classe,vl_frequence,pl_loc);
          	
        }
      	fclose (pl_fD); 
      	
      	for (pl_eqt=pm_eqt;pl_eqt!=NULL;pl_eqt=pl_eqt->suivant) {
      		XZST_03EcritureTrace(   XZSTC_INFO, "IN acts_traf_decode_readfile : equipement defini par stada sens 1 ******** %s - %d ********",pl_eqt -> string,
      										pl_eqt -> number);
      		XZST_03EcritureTrace(   XZSTC_INFO, "IN acts_traf_decode_readfile : q_1 %d, v_1 %d, to_1 %d",
							pl_eqt -> q_1,
							pl_eqt -> v_1,
							pl_eqt -> to_1 );     /* visualisation des donnees recuperees */
		XZST_03EcritureTrace(   XZSTC_INFO, "IN acts_traf_decode_readfile : equipement defini par stada sens 2 ******** %s - %d ********",pl_eqt -> string,
      										pl_eqt -> number);
      		XZST_03EcritureTrace(   XZSTC_INFO, "IN acts_traf_decode_readfile : q_2 %d, v_2 %d, to_2 %d",
							pl_eqt -> q_2,
							pl_eqt -> v_2,
							pl_eqt -> to_2 );     /* visualisation des donnees recuperees */
								
		/*  if ( pl_eqt -> v_1_loc.option == TRUE) { 
			XZST_03EcritureTrace(   XZSTC_WARNING, "IN acts_traf_decode_readfile : localisation eqt_v  sens 1 %s %d %s %s %d %s", pl_eqt -> v_1_loc.l01,
        									pl_eqt -> v_1_loc.l02,
                								pl_eqt -> v_1_loc.l03,
        									pl_eqt -> v_1_loc.l04,
        									pl_eqt -> v_1_loc.l05,
        									pl_eqt -> v_1_loc.l06);
        	 } 
        	 
        	 if ( pl_eqt -> q_1_loc.option == TRUE) { 
			XZST_03EcritureTrace(   XZSTC_WARNING, "IN acts_traf_decode_readfile :  localisation eqt_q  sens 1 %s %d %s %s %d %s", pl_eqt -> q_1_loc.l01,
        									pl_eqt -> q_1_loc.l02,
                								pl_eqt -> q_1_loc.l03,
        									pl_eqt -> q_1_loc.l04,
        									pl_eqt -> q_1_loc.l05,
        									pl_eqt -> q_1_loc.l06);
        	 }
        	 
        	 if ( pl_eqt -> to_1_loc.option == TRUE) { 
			XZST_03EcritureTrace(   XZSTC_WARNING, "IN acts_traf_decode_readfile : localisation eqt_to sens 1 %s %d %s %s %d %s", pl_eqt -> to_1_loc.l01,
        									pl_eqt -> to_1_loc.l02,
                								pl_eqt -> to_1_loc.l03,
        									pl_eqt -> to_1_loc.l04,
        									pl_eqt -> to_1_loc.l05,
        									pl_eqt -> to_1_loc.l06);
        	 }
        									
		
		if ( pl_eqt -> v_2_loc.option == TRUE) { 
			XZST_03EcritureTrace(   XZSTC_INFO, "IN acts_traf_decode_readfile : acts_traf_decode_readfile : localisation eqt_v  sens 2 %s %d %s %s %d %s", pl_eqt -> v_2_loc.l01,
        									pl_eqt -> v_2_loc.l02,
                								pl_eqt -> v_2_loc.l03,
        									pl_eqt -> v_2_loc.l04,
        									pl_eqt -> v_2_loc.l05,
        									pl_eqt -> v_2_loc.l06);
        	}
        	
        	if ( pl_eqt -> q_2_loc.option == TRUE) { 
			XZST_03EcritureTrace(   XZSTC_INFO, "IN acts_traf_decode_readfile : acts_traf_decode_readfile : localisation eqt_q  sens 1 %s %d %s %s %d %s", pl_eqt -> q_2_loc.l01,
        									pl_eqt -> q_2_loc.l02,
                								pl_eqt -> q_2_loc.l03,
        									pl_eqt -> q_2_loc.l04,
        									pl_eqt -> q_2_loc.l05,
        									pl_eqt -> q_2_loc.l06);
        	} 
        	 
        	if ( pl_eqt -> to_2_loc.option == TRUE) { 
			XZST_03EcritureTrace(   XZSTC_INFO, "IN acts_traf_decode_readfile : acts_traf_decode_readfile : localisation eqt_to sens 1 %s %d %s %s %d %s", pl_eqt -> to_2_loc.l01,
        									pl_eqt -> to_2_loc.l02,
                								pl_eqt -> to_2_loc.l03,
        									pl_eqt -> to_2_loc.l04,
        									pl_eqt -> to_2_loc.l05,
        									pl_eqt -> to_2_loc.l06);
        	} */
        	
	}
      	
      	XZST_03EcritureTrace(   XZSTC_INFO, "IN  : acts_traf_decode_readfile : Le fichier %s de donnees trafic Strada a ete traite",pa_file);
      	return XDC_OK;
    		
    	}
    	
    	/* Le fichier n'a pu etre ouvert */
    	XZST_03EcritureTrace(   XZSTC_WARNING, "IN  : acts_traf_decode : le fichier %s n'a pu etre lu",pa_file );
  	return XDC_NOK ;

}


/*A
********************************************
* fonction  de acts_traf_decode permettant  
* de faire un update des donnees des equipements
* dans le synoptique
*******************************************
* Arguments : NON
*******************************************/
void acts_traf_decode_XZIS22() {
liste_eqt	pl_eqt ;
									
for(pl_eqt = pm_eqt ; pl_eqt != NULL ; pl_eqt = pl_eqt -> suivant) 
	{
							
		if (pl_eqt -> e_1 == TRUE) {
			XZIS22_Donnees_Trafic_Strada  (	pl_eqt -> number,
							XDC_SENS_NORD,
							XDC_RADT_CHAUSSEE,
							pl_eqt -> h_1,
							pl_eqt -> q_1,
							pl_eqt -> q_1_b,
							pl_eqt -> v_1,
							pl_eqt -> v_1_b,
							pl_eqt -> to_1,
							pl_eqt -> to_1_b  );
			XZAS62_Ajout_Mesure_DATEX  (	pl_eqt -> string,
							XDC_SENS_NORD,
							XDC_RADT_CHAUSSEE,
							pl_eqt -> h_1,
							pl_eqt -> q_1,
							pl_eqt -> q_1_b,
							pl_eqt -> v_1,
							pl_eqt -> v_1_b,
							pl_eqt -> to_1,
							pl_eqt -> to_1_b  );
		
			/* remise a zero de l'etat 1 */
			pl_eqt -> e_1 = FALSE;
		}

		if (pl_eqt -> e_2 == TRUE) {
			XZIS22_Donnees_Trafic_Strada  (	pl_eqt -> number,
							XDC_SENS_SUD,
							XDC_RADT_CHAUSSEE,
							pl_eqt -> h_2,
							pl_eqt -> q_2,
							pl_eqt -> q_2_b,
							pl_eqt -> v_2,
							pl_eqt -> v_2_b,
							pl_eqt -> to_2,
							pl_eqt -> to_2_b   );
			XZAS62_Ajout_Mesure_DATEX  (	pl_eqt -> string,
							XDC_SENS_SUD,
							XDC_RADT_CHAUSSEE,
							pl_eqt -> h_2,
							pl_eqt -> q_2,
							pl_eqt -> q_2_b,
							pl_eqt -> v_2,
							pl_eqt -> v_2_b,
							pl_eqt -> to_2,
							pl_eqt -> to_2_b   );
			
			/* remise a zero de l'etat 2 */
			pl_eqt -> e_2 = FALSE;
		}
		
	}
}


/*A
*******************************************
* Fonction call back qui reagit au datagroup
* deginit dans le timer de purge
* Elle decode les fichiers donnees trafic
* et les transmet au synoptique
*******************************************
* Arguments : Aucun
*******************************************/

void acts_traf_decode()
{
FILE 			*pl_fD=NULL;
char 			tl_buffer[TAILLE+1];
char 			tl_str[TAILLE];
char 			tl_commande[256];
char 			tl_message[256];

	/* rapatriement des fichiers a decoder */
	acts_traf_ftp();

	/* lecture des fichiers donnees trafic Strada */
	sprintf(tl_str, "ls %sM*.dpm %sm*.dpm 2>&-",C_PATH,C_PATH);

	if ( (pl_fD = popen(tl_str,"r")) != NULL)
	{
		while(fgets(tl_buffer,TAILLE,pl_fD) != NULL)
		{
			/* lecture du fichier Strada */
			if(sscanf(tl_buffer,"%s\n",tl_str)==1)
				if ( acts_traf_decode_readfile(tl_str) == XDC_NOK )
				{
				/* Erreur sur le decodage du fichier */
				sprintf(tl_commande,"cp %s %s",tl_str,C_PATH_ERR);
				system(tl_commande);
                		sprintf(tl_message,"Fichier DATEX incorrect : %s",tl_str);
                		XZIA08_AffMsgAdm("PODFA",tl_message);
                		XZIA08_AffMsgAdm("SDCI1",tl_message);
                		sprintf(tl_message,"Fichier DATEX incorrect : Le fichier est copie dans %s",C_PATH_ERR);
                		XZIA08_AffMsgAdm("PODFA",tl_message);
                		XZIA08_AffMsgAdm("SDCI1",tl_message);
				
				}

			/* faire un ftp et effacer le repertoire du PC Strada */
			acts_traf_ftpdel();
		}
		pclose(pl_fD);
	}
	
	/* lancement de la procedure de lancement de XZIS22_Donnees_Trafic_Strada */
	acts_traf_decode_XZIS22();
	
	XZST_03EcritureTrace(   XZSTC_INFO, "IN  : acts_traf_decode : Les informations ont ete transferees au synoptique trafic Strada");


	/* C_PATH contient le repertoire de travail du SAE ou sont transferes les fichiers trafic */
	sprintf(tl_str, "rm %sM*.dpm %sm*.dpm",C_PATH,C_PATH); 
	
	/* effacer tous les fichiers trafic apres traitement */
 	system(tl_str);
	
}


/*A
*******************************************
* Fonction liberant la memoire dynamique
* allouee pour la liste chainee contenant
* le code des equipements
*******************************************
* Arguments : Aucun
*******************************************/
void acts_traf_freemem()
{
liste_eqt	pl_eqt1 ;
liste_eqt	pl_eqt2 ;

	for (pl_eqt1 = pm_eqt;pl_eqt1 != NULL; pl_eqt1 = pl_eqt2)
	{
		pl_eqt2 =  pl_eqt1 -> suivant;
		free (pl_eqt1 -> string);
		free (pl_eqt1);
	}
}



/*A
*******************************************
* Fonction call back pour l'export catalog
*******************************************
* Arguments : Aucun
*******************************************/

void acts_import_cat()
{
char			vl_ligne[CO_MAX_CARACTERE];
char			pl_ligne[CO_MAX_CARACTERE];
FILE			*pl_fp;
  XZSCT_Datagroup         vl_Dg ;
  char			vl_str[256],vl_strw[256],vl_emetteur[36],*pl_buffer;
  int                   vl_numero,vl_loc_rds,vl_distance,vl_sensp,vl_sens;
  char			vl_sensrds[10],vl_nom[36],vl_nomp[36],vl_table[4];
  FILE *pl_fD,*pl_fd,*pl_fdw;
char 			tl_str[TAILLE+1];
char 			tl_buffer[TAILLE+1];
int 			vl_indice=0;
char 			vl_version[36];

	/* ecriture trace */
  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : acts_import_cat" );
  
  XZST_03EcritureTrace(   XZSTC_INFO, "suppression de l'ancien catalogue");
  sprintf(vl_str,"rm -f %s/*",CO_PATH_CATALOG);
  system(vl_str);
  XZST_03EcritureTrace(   XZSTC_INFO, "execution de %s",C_SCRIPT_FTP_CAT);
  
/* construit la commande test reseau de la machine demandee */
  sprintf(pl_ligne,"%s %s %u %u",SG_PING,C_PC_STRADA,
				SG_BYTES_PING, SG_COUNT_PING);

  XZST_03EcritureTrace(XZSTC_INFO,"ligne %s",pl_ligne);

  /* execute la commande, */
  if ((pl_fp = popen(pl_ligne,"r")) != (FILE *)NULL)
   {
                /* recupere le resultat de cette commande */
                fgets(vl_ligne,sizeof(vl_ligne)-1,pl_fp) ;
                fgets(vl_ligne,sizeof(vl_ligne)-1,pl_fp) ;
                pclose(pl_fp) ;

		XZST_03EcritureTrace(XZSTC_INFO,"ligne ping = %s",vl_ligne);

		/*tout va bien*/
                if (strstr(vl_ligne,SG_TOKEN_TST) != NULL)
                {
			 /*FTP*/
			  system(C_SCRIPT_FTP_CAT);
 		}
                else
                {
			XZST_03EcritureTrace(   XZSTC_WARNING, "asts_import_catalog : PC strada HS");
    			acts_import_cat_erreur();
    			return;
                }
    }
    else
   {
                XZST_03EcritureTrace(XZSTC_WARNING,
                     "erreur execution de %s",pl_ligne);
    		acts_import_cat_erreur();
    		return;
    }
  XZST_03EcritureTrace(   XZSTC_INFO, "fin execution de %s",C_SCRIPT_FTP_CAT);
	/* lecture des fichiers donnees trafic Strada */
	sprintf(tl_str, "ls %s* 2>&-",CO_PATH_CATALOG);
  	XZST_03EcritureTrace(   XZSTC_INFO, "Execution de %s",tl_str);

	if ( (pl_fD = popen(tl_str,"r")) != NULL)
	{
   	  /*ouverture du fichier*/
  	  XZST_03EcritureTrace(   XZSTC_INFO, "Ouverture de %s",CO_FIC_EQTCAT);
  	  if ((pl_fdw=fopen(CO_FIC_EQTCAT,"w"))==NULL) {
    		XZST_03EcritureTrace(XZSTC_WARNING,"asts_export_catalog: pb creation fichier %s",
		CO_FIC_EQTCAT);
   		acts_import_cat_erreur();
   		return ;
 	  }
 	  XZST_03EcritureTrace(   XZSTC_INFO, "Fin Ouverture de %s",CO_FIC_EQTCAT);
  	 fputs("# numero,nom,sens,loc_rds,distance,emetteur\n",pl_fdw);
 	  
	  while(fgets(tl_buffer,TAILLE,pl_fD) != NULL)
	  {
		XZST_03EcritureTrace(   XZSTC_INFO, "ligne = %s",tl_buffer);
		
		/* lecture du fichier Strada */
		if(sscanf(tl_buffer,"%s\n",tl_str)==1)
		{	
  			XZST_03EcritureTrace(   XZSTC_INFO, "Ouverture de %s",tl_str);
  			if ((pl_fd=fopen(tl_str,"r"))==NULL) {
   				 XZST_03EcritureTrace(XZSTC_INFO,"acts_import_cat: pb ouverture fichier %s",
					tl_str);
    				acts_import_cat_erreur();
   				 break ;
  				}
  
  			XZST_03EcritureTrace(   XZSTC_INFO, "fin Ouverture de %s, lecture",tl_str);
  
  			if (fgets(vl_str, 255, pl_fd) == NULL)
  			{
    				XZST_03EcritureTrace(XZSTC_WARNING,"Fichier catalog incorrect");
    				acts_import_cat_erreur();
   				 return;
  			}
  			else
  			{
				XZST_03EcritureTrace(   XZSTC_INFO, "ligne;%s.",vl_str);
				pl_buffer = vl_str;
				if (sscanf(pl_buffer,"MSE#%[^#]#",vl_emetteur)!=1) {      			
      					/* l'attribut MSE du fichier n'a pas le bon format, il ne sera pas pris en compte */
      					XZST_03EcritureTrace(   XZSTC_WARNING, " L'attribut MSE du catalog n'a pas le bon format ou n'existe pas\n%s",pl_buffer );
    					acts_import_cat_erreur();
					return;
      				}
  
  			}
  			/* Lecture de l'heure de generation */
  			if (fgets(vl_str, 255, pl_fd) == NULL)
  			{
    				XZST_03EcritureTrace(XZSTC_WARNING,"Fichier catalog incorrect");
    				acts_import_cat_erreur();
    				return;
  			}
			XZST_03EcritureTrace(   XZSTC_WARNING, "ligne;%s.",vl_str);
   
  			strcpy(vl_nomp,"");
  			vl_sensp=0;
  
 			 while (fgets(vl_str, 255, pl_fd) != NULL)
  			{
 				XZST_03EcritureTrace(   XZSTC_INFO, "ligne;%s.",vl_str);
  				pl_buffer = strtok (vl_str,"#");
  				strcpy(vl_nom,pl_buffer);
  				pl_buffer = strtok (NULL,"#");
  				/*nature mesure: on saute */
  				pl_buffer = strtok (NULL,"#");
  				strcpy(vl_sensrds,pl_buffer);
  				pl_buffer = strtok (NULL,"#");
  				/*voie: on saute */
  				pl_buffer = strtok (NULL,"#");
  				/*periode: on saute */
  				pl_buffer = strtok (NULL,"#");
  				/*unite: on saute */
  				pl_buffer = strtok (NULL,"#");
  				/*classe: on saute */
  				pl_buffer = strtok (NULL,"#");
  				strcpy(vl_table,pl_buffer);
  				pl_buffer = strtok (NULL,"#");
  				strcpy(vl_version,pl_buffer);
  				pl_buffer = strtok (NULL,"#");
  				vl_loc_rds=atoi(pl_buffer);
  				pl_buffer = strtok (NULL,"#");
  				/* libelle du sens : on saute */
  				pl_buffer = strtok (NULL,"#");
  				vl_distance=atoi(pl_buffer);
  				if ( vl_sensrds[0] == 'P')
  					vl_sens=XDC_SENS_SUD;
  				else
  					vl_sens=XDC_SENS_NORD;
  	
  				vl_indice++;
        			sprintf(vl_strw,"%d\t%s\t%d\t%d\t%d\t%s\n",
                       			 vl_indice,
                       			 vl_nom,
                        			vl_sens,
                       			 vl_loc_rds,
                        			vl_distance,
                        			vl_emetteur);
                        
 				XZST_03EcritureTrace(   XZSTC_INFO, "lignew;%s.",vl_strw);
 				if ( strcmp(vl_nom,vl_nomp) || (vl_sens!=vl_sensp) )
 				   {
        				fputs(vl_strw,pl_fdw);
        				strcpy(vl_nomp,vl_nom);
        				vl_sensp=vl_sens;
           			}
  			}
  
  			fclose(pl_fd);
		}	
	  }

	}

  fclose(pl_fdw);
  sprintf(vl_str,"rcp %s podfa:%s/%s",CO_FIC_EQTCAT,XDC_PATH_STRADA,CO_FIC_EQTSTR);
  system(vl_str);
  
  /*A Envoi a podfa l'ouverture de la fenetre */
    XDG_EncodeDG2(vl_Dg , XDG_IAX_TEXTE , "SDCI1" );
   if (!TipcSrvMsgWrite(vl_Dg
                        ,XDM_IdentMsg(XDM_IAX_NEWTASK) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT4,COM_NEWTASK
			,T_IPC_FT_STR,"ITMA_STRA_Catalog_Stations"
			,T_IPC_FT_STR,"tacts.x"
			,T_IPC_FT_INT4,0
			,NULL) )
     {
     /* Affichage Trace */
     XZST_03EcritureTrace(XZSTC_WARNING
             ,"MSOP_Envoyer:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
	     ,vl_Dg ,XDM_IdentMsg(XDM_IAX_NEWTASK) ) ;
     return ;
     }
 
    XDG_EncodeDG2(vl_Dg , XDG_IAX_TEXTE , "PODFA" );
   if (!TipcSrvMsgWrite(vl_Dg
                        ,XDM_IdentMsg(XDM_IAX_NEWTASK) 
                        ,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_INT4,COM_NEWTASK
			,T_IPC_FT_STR,"ITMA_STRA_Catalog_Stations"
			,T_IPC_FT_STR,"tacts.x"
			,T_IPC_FT_INT4,0
			,NULL) )
     {
     /* Affichage Trace */
     XZST_03EcritureTrace(XZSTC_WARNING
             ,"MSOP_Envoyer:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
	     ,vl_Dg ,XDM_IdentMsg(XDM_IAX_NEWTASK) ) ;
     return ;
     }
 
	/* ecriture trace */
	XZST_03EcritureTrace(   XZSTC_INFO, "OUT  : acts_import_cat" );

}


/*A
*******************************************
* Fonction d'envoi msg erreur a l'as
*******************************************
* Arguments : Aucun
*******************************************/

void acts_import_cat_erreur()
{
 char 			tl_message[256];
 
 sprintf(tl_message,"Impossibilite de recuperer le fichier catalogue DATEX");
 XZIA08_AffMsgAdm("PODFA",tl_message);
 XZIA08_AffMsgAdm("SDCI1",tl_message);

}
/*A
*******************************************
* Fonction call back pour l'export catalog
*******************************************
* Arguments : Aucun
*******************************************/

void acts_export_cat()
{
	
	/* On sort de la tache pour la reinitialisation */
	exit(0);

}

