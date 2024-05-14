/* Fichier : @(#)asts_export.c	1.10   Release : 1.10   Date : 07/03/07 *
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : asts_export.c			*
*************************************************
* DESCRIPTION :
* module externe de la tache serveur de donnees
* trafic pour le PC STRADA
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
*************************************************
* HISTORIQUE :
*
* VATHELOT	28 Avr 1997 : Creation 			V1.1
* NIEPCERON	30 Oct 1998 : remplacement de l'adr IP pour son nom v1.4
* NIEPCERON	05 Jan 1999 : Modification pour intergestionnaire dem/1724 v1.5
* NIEPCERON     03 Mar 1999 : horodate = derniere 6mn, suppression de fichier qd strada HS v1.6
* Niepceron     09 Aou 1999 : Modif catalog pour Strada-Net V2 dem/1767 v1.7
* Niepceron     05 jan 2006 : Ajout de la fonction asts_insere_ecrit_trafic DEL605 v1.8
* Niepceron     05 Avr 2007 : Correction du lib sens pour METACOR 1.9
* SFO           06/10/14 : Adaptation pour compilation sous linux
************************************************/
#include "asts_export.h"

/* definition des constantes */
#define CO_NB_JOUR_PURGE	3
#define CO_DELAI_FREQUENCE	360
#define CO_DELAI_PURGE		86400 * CO_NB_JOUR_PURGE
#define	CO_MAX_CARACTERE	100
#define	CO_NB_CHIFFRE		13

#define CO_FIC_FREQUENCE	XDC_PATH_STRADA "/frequence.txt"
#define CO_REP_TRAFOUT		XDC_PATH_STRADA "/trafout/"
#define CO_REP_TRAFOUT_M	XDC_PATH_STRADA "/trafout_M/"
#define CO_REP_TRAFOUT_M_ASF	XDC_PATH_STRADA "/trafout_M_ASF/"
#define CO_REP_CATOUT		XDC_PATH_STRADA "/catout/"
#define CO_FIC_COMPTEUR		XDC_PATH_STRADA "/compteur.txt"
#define CO_REP_STRADA		XDC_PATH_STRADA
#define	C_SCRIPT_FTP		XDC_PATH_STRADA "/ftp/traf_script_put"
#define	C_SCRIPT_FTP_M		XDC_PATH_STRADA "/ftp/traf_script_put_M"
#define	C_SCRIPT_FTP_M_ASF		XDC_PATH_STRADA "/ftp/traf_script_put_M_ASF"
#define	C_SCRIPT_FTP_CAT	XDC_PATH_STRADA "/ftp/catf_script_put"

#define CO_PATH_CATALOG		XDC_PATH_STRADA "/catout/"

/* dÅfinition des variables globales È l'application */
#define C_PC_STRADA		"STRADA"
#define C_PC_METACOR		"METACOR"
#define C_PC_METACOR_ASF		"METACOR_ASF"

#define C_DUREE_METACOR	"6"

#define SG_PING         "/etc/ping"
#define SG_TO_PING      8
#define SG_BYTES_PING   64
#define SG_COUNT_PING   1
#define SG_TOKEN_TST    "64"

#define C_DATEX_ESCOTA		"fr204"

/* definition des variables locales au module */
static char *	version = "@(#)asts_export.c	1.1 05/13/97 : commentaire" ;

struct Id_Station 	*pm_tete_station;

static FILE *pm_fd;
static XDY_Booleen	vm_entete=XDC_FAUX;
static XZSMT_Horodate	vm_HorodateZero = "";

/* declaration de fonctions internes */
int asts_ecrit_catalog(XDY_Mot,char *,char *,char *,int,int,char *,char *);
int asts_insere_ecrit_trafic(XDY_Horodate ,XDY_Eqt,XDY_Octet,XDY_Mot, XDY_Octet, XDY_Octet, XDY_Octet, XDY_Octet );

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

int asts_export_init()
{
XZSMT_CleProg		vl_cle;
XZSMT_CleProg		vl_cle_purge;
XZSCT_Datagroup 	vl_Datagroup ;
int 			vl_JourSemaine;
double 			vl_HorodateSec; 
XZSCT_NomMachine 	pl_NomMachine	= "\0";
T_BOOL 			vl_etat;
FILE 			*pl_fp;
char			pl_param[CO_MAX_CARACTERE];
char			pl_ligne[CO_MAX_CARACTERE];
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



	XZST_03EcritureTrace(   XZSTC_INFO, "IN  : asts_export_init" );
 	
  	/* Recuperation du nom de la machine locale */
	XZSC_07NomMachine(pl_NomMachine) ;
 	
  	/* Lecture de la frequence d'exportation des donnees vers Strada */
	if ( (pl_fp = fopen(CO_FIC_FREQUENCE, "r")) == NULL ) {
		XZST_03EcritureTrace(   XZSTC_WARNING, "erreur de lecture du fichier %s !"
							,CO_FIC_FREQUENCE );
		return(XDC_NOK);
	}	


	/* On lit les lignes du fichier frequence.txt une par une */
	/* ------------------------------------------------------ */
	while ( fgets(pl_ligne, CO_MAX_CARACTERE, pl_fp) != NULL) {
		/* On ignore les lignes commencant par un # */
		if ( pl_ligne[0] == '#' )
			continue;
		
		/* Pour les autres lignes on recherche la frequence de notre tache */
		if (sscanf(pl_ligne,"%[^;];%d", pl_param, &vl_val)==2){
			if ( !strcmp( pl_param, "tasts.x") ) {
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
  	

	/* Initialisation de la liste chainee contenant les stations */
	pm_tete_station = NULL;
	
	if (XZAS55_Lect_RADT_A_Exporter((XDY_FonctionInt)asts_insere_station,&vl_val) != XDC_OK) {
	    XZST_03EcritureTrace(XZSTC_FATAL,"asts_export_init: pb dans l'appel a XZAS55_Lect_RADT_A_Exporter");
    	    return;
	}
  	
  	
  	/*A
	* Initialisation de la callback du timer de programmation
	* -------------------------------------------------------
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_TIMER_SRV_RADT), asts_export_ftp,(T_CB_ARG)NULL) == NULL) 
	{	
                XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AS_TIMER_SRV_RADT non cree.",version );
		return XDC_NOK;
	}
	
	
	/*A
	* Initialisation de la callback du timer de purge
	* -----------------------------------------------
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_TIMER_PURGE_RADT), asts_export_purge,(T_CB_ARG)NULL) == NULL) 
	{	
                XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AS_TIMER_PURGE_RADT non cree.",version );
		return XDC_NOK;
	}
	
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AG_EXPCATF), asts_export_catalog,(T_CB_ARG)NULL) == NULL) 
	{	
                XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AS_TIMER_PURGE_RADT non cree.",version );
		return XDC_NOK;
	}
	
  	
	/*A initialisation des timers */
	/* XZSM_00InitServices(); */
	
	/*A Lecture horodate courante systeme */
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
   
	   
	sprintf( vl_cle , "cle_timer_serveur_radt" );
	sprintf( vl_cle_purge, "cle_purge_radt");

	/*A on forme le datagroup de la tache */
	XDG_EncodeDG2(vl_Datagroup , XDG_AS_TIMER_SRV_RADT , pl_NomMachine );
	
   
	/*A Abonnement au datagroup pour que le timer nous envoie les messages */
	if (! XDG_JoindreDG(vl_Datagroup) )
	{	
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING, 
				"Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
			return XDC_NOK;
	}	
   
	/*A Abonnement au datagroup pour l'export catalog */
	if (! XDG_JoindreDG(XDG_AG_EXPCATF) )
	{	
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING, 
				"Abonnement sur %s non effectuee. \n " ,XDG_AG_EXPCATF);
			return XDC_NOK;
	}	
   
	/* armement du timer de programmation */
	/* ---------------------------------- */
#ifdef _HPUX_SOURCE
	if (XZSM_01DemanderProgrammation(	vl_HorodateSec,
						vl_frequence,
						vl_Datagroup,
						XDM_AS_TIMER_SRV_RADT,
						vl_cle,
						NULL) != XDC_OK)
#else
	if (XZSM_01DemanderProgrammation(	vl_HorodateSec,
						vl_frequence,
						vl_Datagroup,
						XDM_AS_TIMER_SRV_RADT,
						vl_cle,
						0) != XDC_OK)
#endif
	{
	/* Ecriture Trace */
        XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de donnees radt non declenchee. \n " );
        return XDC_NOK;
	} 
	
	

	/* Ecriture Trace */
	XZST_03EcritureTrace( XZSTC_INFO, "OUT : asts_export_init" );
	return XDC_OK;

}


/*A
*******************************************
* Fonction call back qui reagit au datagroup
* definit dans le timer de programmation
* Pour chaque station elle appelle
* la fonction client qui recherche les debits
* et genere le fichier au format datex sur
* le PC Strada
*******************************************
* Arguments : Aucun
*******************************************/

void asts_export_ftp()
{
struct Id_Station 	*pl_station_courante;
int			vl_fichier_deja_cree;
char			vl_ligne[CO_MAX_CARACTERE];
char			pl_ligne[CO_MAX_CARACTERE];
char			pl_locrds[CO_MAX_CARACTERE];
char			tl_str[CO_MAX_CARACTERE];
char			vl_code_DATEX[10] = "FR001";
FILE			*pl_fp;
char			vl_sens;
char			*vl_date;
char			pl_nom_fichier[CO_NB_CHIFFRE];
char			pl_fichier[100];
long int		vl_compteur;
int			vl_pos_fichier;
XDY_Entier		vl_val=0;
char			pl_strdate[25];
int			vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee1, vl_annee2;
int			vl_frequence;
XDY_Entier		vl_Q,vl_V,vl_TO;
	
	/* ecriture trace */
	XZST_03EcritureTrace(   XZSTC_INFO, "IN  : asts_export_ftp" );
	
	vl_fichier_deja_cree = XDC_FAUX;
	
	/* Mise a jour du compteur des fichiers strada a generer */
	/* ----------------------------------------------------- */
	
	/* Lecture du compteur */
	if ( (pl_fp = fopen( CO_FIC_COMPTEUR, "r+")) == NULL )
	{
		XZST_03EcritureTrace(   XZSTC_WARNING, "erreur de lecture du fichier %s !", CO_FIC_COMPTEUR );
		return;
	}	

	/* On lit les lignes du fichier compteur.txt une par une */
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
		strcpy(pl_nom_fichier,"M0000000");
	}
	else
	{
		vl_compteur = vl_compteur + 1;
XZST_03EcritureTrace(   XZSTC_INFO, " vl_compteur = %d", vl_compteur);

		/* generons le nom du fichier sur 7 chiffres a partir de vl_compteur */
		sprintf(pl_nom_fichier,"M%07d.dpm\0",vl_compteur);

XZST_03EcritureTrace(   XZSTC_INFO, " nom du fichier = %s", pl_nom_fichier);		
	}
	
	
	/* Et on copie la nouvelle valeur du compteur dans le fichier avant de le fermer */
	fseek(pl_fp,vl_pos_fichier,SEEK_SET);
	fprintf(pl_fp,"%d",vl_compteur);
	fflush(pl_fp);
 	fclose(pl_fp);
	
	
	
	/* Generons le fichier de trafic a destination du PC STRADA */
	/* -------------------------------------------------------- */
	
	/* On lit les stations de la liste chainee une par une */
	for( pl_station_courante = pm_tete_station; pl_station_courante !=NULL; pl_station_courante = pl_station_courante->Suivant)
	{
	XZST_03EcritureTrace(   XZSTC_INFO, "Test : numero de station = %d   sens %d ",pl_station_courante->Numero, pl_station_courante->Sens);

	/* initialisation de la localisation RDS */
	strcpy (pl_locrds,"#####"); 
	
	/* initialisation de la frequence */
	vl_frequence = 6 ;
	
		/* appel au client qui extrait les donnees des differentes stations */
		
		if (XZAS54_Donnees_RADT(pl_station_courante -> Numero, pl_station_courante -> Sens, &vl_Q, &vl_V, &vl_TO) != XDC_NOK)
		{

			XZST_03EcritureTrace(XZSTC_INFO,"%d %d %d",vl_Q,vl_V,vl_TO);
			if (vl_fichier_deja_cree == XDC_FAUX)
			{
				/* on forme le nom absolu du fichier */
				sprintf( pl_fichier, "%s", CO_REP_TRAFOUT);
				strncat( pl_fichier, pl_nom_fichier,CO_NB_CHIFFRE+1);
				
				XZST_03EcritureTrace(   XZSTC_INFO, "Test : nom du fichier genere %s", pl_fichier );
				
				/* on cree le fichier trafic pour strada */
				if ( (pl_fp = fopen( pl_fichier, "w+")) == NULL )
					{
					XZST_03EcritureTrace(   XZSTC_WARNING,
							"erreur de creation du fichier de donnees trafic pour strada !" );
					}

				/* on forme les lignes d'en-tete */
				sprintf(vl_ligne, "MSE#%s#\n",C_DATEX_ESCOTA);				
				fputs(vl_ligne, pl_fp);
				
			XZST_03EcritureTrace(XZSTC_INFO,"%s",vl_ligne);

				/* On recupere l'heure courante */			
				strcpy( pl_strdate, asts_get_heure());

				if (sscanf( pl_strdate, "%2d/%2d/%2d%2d %2d:%2d:%2d",
								&vl_jour,
								&vl_mois,
								&vl_annee1,
								&vl_annee2,
								&vl_heure,
								&vl_minute,
								&vl_seconde) != 7) {
								
					XZST_03EcritureTrace(   XZSTC_WARNING,
							"asts_export_ftp : le format de la date est incorrecte !" );			
				}
				
							
				sprintf(vl_ligne, "MST#%d%02d%02d%02d%02d%02dP01#\n", 
							vl_annee1,
							vl_annee2,
							vl_mois,
							vl_jour,
							vl_heure,
							vl_minute);
				fputs(vl_ligne, pl_fp);

				vl_fichier_deja_cree = XDC_VRAI;
			}
					
			vl_sens = (pl_station_courante->Sens == 1) ? 'P' : 'N' ;

			/* Puis une ligne par type de donnees pour chaque station */
			if (vl_Q != -1) {	
				sprintf(vl_ligne, "%s#FLO#%c#*#%d#VHH#K#%d#%s#\n", pl_station_courante->Nom, vl_sens,vl_frequence, vl_Q, pl_locrds);
				fputs(vl_ligne, pl_fp);
			}
			if (vl_V != -1) {
				sprintf(vl_ligne, "%s#AVS#%c#*#%d#KMH#K#%d#%s#\n", pl_station_courante->Nom, vl_sens,vl_frequence, vl_V, pl_locrds);
				fputs(vl_ligne, pl_fp);
			}
			if (vl_TO != -1) {
				sprintf(vl_ligne, "%s#OCC#%c#*#%d#PER#K#%d#%s#\n", pl_station_courante->Nom, vl_sens,vl_frequence, vl_TO, pl_locrds);
				fputs(vl_ligne, pl_fp);
			}
		}

	}

	fclose(pl_fp);

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
			 system(C_SCRIPT_FTP);

			 /* C_PATH contient le repertoire de travail du SAE ou sont transferes les fichiers trafic */
			 sprintf(tl_str, "rm %s/M*.dpm %s/fr*.sit",CO_REP_TRAFOUT,CO_REP_TRAFOUT);

			system(tl_str);

 		}
                else
                {
			XZST_03EcritureTrace(   XZSTC_WARNING, "asts_export_ftp : PC strada HS");
			sprintf(tl_str, "rm %s/M*.dpm %s/fr*.sit",CO_REP_TRAFOUT,CO_REP_TRAFOUT);
			system(tl_str);

                }
        }
        else
                XZST_03EcritureTrace(XZSTC_WARNING,
                     "erreur execution de %s",pl_ligne);

XZST_03EcritureTrace(XZSTC_WARNING,"Avant appel XZAS91");
	/* Creation du fichier de données pour metacor */
	if (XZAS91_Derniere_Donnees_RADT((XDY_FonctionInt)asts_insere_ecrit_trafic,&vl_val) != XDC_OK) {
	    XZST_03EcritureTrace(XZSTC_WARNING,"asts_export_init: pb dans l'appel a XZASXX");
    	    
	}
			
	XZST_03EcritureTrace(   XZSTC_WARNING, "asts_export_ftp : copie des fichiers créés dans le rep. d'export vers ASF");
	sprintf(tl_str, "cp %s/* %s",CO_REP_TRAFOUT_M,CO_REP_TRAFOUT_M_ASF);
	system(tl_str);
	



	/********** Transfert vers METACOR  *****************/
	/* construit la commande test reseau vers METACOR */
	 sprintf(pl_ligne,"%s %s %u %u",SG_PING,C_PC_METACOR,
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
			 system(C_SCRIPT_FTP_M);



 		}
                else
                {
			XZST_03EcritureTrace(   XZSTC_WARNING, "asts_export_ftp : PC metacor HS");
			sprintf(tl_str, "rm %s/*",CO_REP_TRAFOUT_M);
			system(tl_str);

                }
        }
        else
                XZST_03EcritureTrace(XZSTC_WARNING,
                     "erreur execution de %s",pl_ligne);


	/********** Transfert vers METACOR ASF *****************/
	/* construit la commande test reseau vers METACOR ASF */
	 sprintf(pl_ligne,"%s %s %u %u",SG_PING,C_PC_METACOR_ASF,
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
			 system(C_SCRIPT_FTP_M_ASF);


 		}
                else
                {
			XZST_03EcritureTrace(   XZSTC_WARNING, "asts_export_ftp : PC metacor HS");
			sprintf(tl_str, "rm %s/*",CO_REP_TRAFOUT_M_ASF);
			system(tl_str);

                }
        }
        else
                XZST_03EcritureTrace(XZSTC_WARNING,
                     "erreur execution de %s",pl_ligne);


	asts_export_purge();

	XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  : asts_export_ftp" );
}


/*A
*******************************************
* Fonction call back qui reagit au datagroup
* deginit dans le timer de purge
* Elle supprime les fichiers qui ont
* plus de trois jours
*******************************************
* Arguments : Aucun
*******************************************/

void asts_export_purge()
{
char		pl_commande[100]="";

	/* ecriture trace */
	XZST_03EcritureTrace(   XZSTC_INFO, "IN  : asts_export_purge" );
	
	/* suppression des fichiers qui ont ete cres il y a plus de trois jours */
	sprintf( pl_commande,"find %s -mtime +%d -exec rm {} \\;", CO_REP_TRAFOUT, CO_NB_JOUR_PURGE);
	system(pl_commande);
	
	/* ecriture trace */
	XZST_03EcritureTrace(   XZSTC_INFO, "OUT  : asts_export_purge" );

}


/*A
*******************************************
* Fonction d'insertion d'une station dans
* la liste chainee contenant la liste des
* stations selectionnees.
* L'insertion se fait en tete de liste.
* La liste des stations etant globale
* au module.
*******************************************
* Arguments :
* int va_station : le numero de la station
* int sens : le sens de la station
*******************************************/

int asts_insere_station(int va_station,char *va_nom,int va_sens)
{
struct Id_Station 	*pl_new_station;
char			tl_nom[20];
int 			vl_i;

	if (( pl_new_station = (struct Id_Station *) malloc (sizeof(struct Id_Station))) != NULL)
	{
		pl_new_station->Numero = va_station;
		pl_new_station->Sens = va_sens;
		for (vl_i=0;va_nom[vl_i] != ' ' && va_nom[vl_i] != '\0' && va_nom[vl_i] != '\t';vl_i++) {
			tl_nom[vl_i]= va_nom[vl_i];
		}
		tl_nom[vl_i]='\0';
		
		strcpy(pl_new_station->Nom, tl_nom);
		pl_new_station->Suivant = pm_tete_station;

		pm_tete_station = pl_new_station;
	}
	else
	{
		XZST_03EcritureTrace(   XZSTC_WARNING, "asts_insere_station : le malloc a echoue !" );	
	}
		
	return XDC_OK;
}



/*A
*******************************************
* Fonction qui libere les ressources 
* utilisees par la liste chainee contenant
* la liste des stations
*******************************************
* Arguments : Aucun
*******************************************/

void asts_export_arret()
{
struct Id_Station *pl_station_courante;
struct Id_Station *pl_liste_tempo;

for(pl_station_courante = pm_tete_station; pl_station_courante != NULL ; pl_station_courante = pl_liste_tempo)
	{
	pl_liste_tempo = pl_station_courante->Suivant;
	free(pl_station_courante);
	}

}




/*A
*******************************************
* Fonction qui retourne la date courante
*******************************************
* Arguments : Aucun
*******************************************/

char * asts_get_heure()
{
XZSMT_Horodate  vl_Horodate     = "";
double          vl_HorodateSec  = 0.0;
int             vl_JourSemaine  = 0;



	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_HorodateSec);
	XZSM_11ConversionHorodate(vl_HorodateSec-fmod(vl_HorodateSec,360), &vm_HorodateZero);
	
	return(vm_HorodateZero);

}


/*A
*******************************************
* Fonction call back pour l'export catalog
*******************************************
* Arguments : Aucun
*******************************************/

void asts_export_catalog()
{
char			vl_ligne[CO_MAX_CARACTERE];
char			pl_ligne[CO_MAX_CARACTERE];
FILE			*pl_fp;
  char			vl_str[256];
  XZSMT_Horodate	vl_horo_chaine;
  int                   vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
  XDY_Horodate	vl_Horodate;
  int vl_Jour_De_La_Semaine,vl_val;
  XZIAT_destinataire vl_dest;
char			pl_nom_fichier[CO_NB_CHIFFRE];
char			pl_fichier[100];
long int		vl_compteur;
int			vl_pos_fichier;

	/* ecriture trace */
	XZST_03EcritureTrace(   XZSTC_INFO, "IN  : asts_export_catalog" );
	strcpy(vl_dest.site,"CI");
 	vm_entete=XDC_FAUX;
 	if (XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_Jour_De_La_Semaine,&vl_Horodate) != XZSMC_OK)
                XZST_03EcritureTrace(XZSTC_WARNING, "Erreur a la lecture de l'heure courante") ;
                
	sprintf(vl_str, "rm %s/M*.cat ",CO_PATH_CATALOG);
	system(vl_str);
	
	/* Mise a jour du compteur des fichiers strada a generer */
	/* ----------------------------------------------------- */
	
	/* Lecture du compteur */
	if ( (pl_fp = fopen( CO_FIC_COMPTEUR, "r+")) == NULL )
	{
		XZST_03EcritureTrace(   XZSTC_WARNING, "erreur de lecture du fichier %s !", CO_FIC_COMPTEUR );
		return;
	}	

	/* On lit les lignes du fichier compteur.txt une par une */
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
		strcpy(pl_nom_fichier,"M0000000");
	}
	else
	{
		vl_compteur = vl_compteur + 1;
XZST_03EcritureTrace(   XZSTC_INFO, " vl_compteur = %d", vl_compteur);

		/* generons le nom du fichier sur 7 chiffres a partir de vl_compteur */
		sprintf(pl_nom_fichier,"%sM%07d.cat\0",CO_PATH_CATALOG,vl_compteur);

XZST_03EcritureTrace(   XZSTC_INFO, " nom du fichier = %s", pl_nom_fichier);		
	}
	
	
  /*ouverture du fichier*/
  if ((pm_fd=fopen(pl_nom_fichier,"w"))==NULL) {
    XZST_03EcritureTrace(XZSTC_WARNING,"asts_export_catalog: pb creation fichier %s",
		pl_nom_fichier);
    return ;
  }
  sprintf(vl_str,"MSE#%s#\n",C_DATEX_ESCOTA);
  fputs(vl_str,pm_fd);
	
  XZSM_11ConversionHorodate(vl_Horodate,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute,
                        &vl_seconde ) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"asts_export_catalog: format date incorrect");
    return ;
  }
  sprintf(vl_str,"MST#%04d%02d%02d%02d%02dP01#\n",
                        vl_annee,
                        vl_mois,
                        vl_jour,
                        vl_heure,
                        vl_minute);
                        
  fputs(vl_str,pm_fd);
  
                        
   if (XZAS70_Loc_RADT_A_Exporter((XDY_FonctionInt)asts_ecrit_catalog,&vl_val) != XDC_OK) {
	    XZST_03EcritureTrace(XZSTC_FATAL,"asts_export_catalog: pb dans l'appel a XZAS70_Loc_RADT_A_Exporter");
	}
  fclose(pm_fd);
	
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
 		char 			tl_message[256];
 
 			sprintf(tl_message,"Impossibilite de transferer le fichier catalogue DATEX");
			XZIA08_AffMsgAdm(vl_dest,tl_message);

			XZST_03EcritureTrace(   XZSTC_WARNING, "asts_export_catalog : PC strada HS");
                }
    }
    else
   {
   char 			tl_message[256];
 
		 sprintf(tl_message,"Impossibilite de transferer le fichier catalogue DATEX");
		XZIA08_AffMsgAdm(vl_dest,tl_message);

                XZST_03EcritureTrace(XZSTC_WARNING,
                     "erreur execution de %s",pl_ligne);
    }
  
	/* ecriture trace */
	XZST_03EcritureTrace(   XZSTC_INFO, "OUT  : asts_export_catalog" );
	
	/* On sort de la tache pour la reinitialisation */
	exit(0);

}

/*A
*******************************************
* Fonction Utilisateur pour l'ecriture du catalog
*******************************************
* Arguments : Aucun
*******************************************/

int asts_ecrit_catalog(XDY_Mot va_numero,
			char *va_nom,
			char *va_table,
			char *va_version,
			int va_loc_rds,
			int va_distance,
			char *va_sens,
			char *va_lib_sens)
{
  char			vl_str[256];
  int			vl_long;

  vl_long=strlen(va_nom);
  while ((vl_long>1) && (va_nom[vl_long-1]==' ')) 
   vl_long--;

  if (va_nom[vl_long-1]==' ')
  	va_nom[vl_long-1]='\0';
  else
  	va_nom[vl_long]='\0';
  vl_long=strlen(va_version);
  while ((vl_long>1) && (va_version[vl_long-1]==' ')) 
   vl_long--;

  if (va_version[vl_long-1]==' ')
  	va_version[vl_long-1]='\0';
  else
  	va_version[vl_long]='\0';
  vl_long=strlen(va_lib_sens);
  while ((vl_long>1) && (va_lib_sens[vl_long-1]==' ')) 
   vl_long--;

  if (va_lib_sens[vl_long-1]==' ')
  	va_lib_sens[vl_long-1]='\0';
  else
  	va_lib_sens[vl_long]='\0';
  	
  sprintf(vl_str,"%s#FLO#%s#*#6#VHH#K#%s#%s#%d#%s#%d##\n",va_nom,va_sens,va_table,va_version,va_loc_rds,va_lib_sens,va_distance);
  XZST_03EcritureTrace(XZSTC_INFO,"asts_ecrit_catalog:ligne:%s.",vl_str);
  fputs(vl_str,pm_fd);
  sprintf(vl_str,"%s#AVS#%s#*#6#KMH#K#%s#%s#%d#%s#%d##\n",va_nom,va_sens,va_table,va_version,va_loc_rds,va_lib_sens,va_distance);
  XZST_03EcritureTrace(XZSTC_INFO,"asts_ecrit_catalog:ligne:%s.",vl_str);
  fputs(vl_str,pm_fd);
  sprintf(vl_str,"%s#OCC#%s#*#6#PER#K#%s#%s#%d#%s#%d##\n",va_nom,va_sens,va_table,va_version,va_loc_rds,va_lib_sens,va_distance);
  XZST_03EcritureTrace(XZSTC_INFO,"asts_ecrit_catalog:ligne:%s.",vl_str);
  fputs(vl_str,pm_fd);
  
return XDC_OK;
}


/*A
*******************************************
* Fonction Utilisateur pour l'ecriture des données trafic pour metacor
*******************************************
* Arguments : Aucun
*******************************************/

int asts_insere_ecrit_trafic(XDY_Horodate va_horodate,
			XDY_Eqt	va_station,
			XDY_Octet va_sens,
			XDY_Mot va_Q,
			XDY_Octet va_V,
			XDY_Octet va_TO,
			XDY_Octet va_PL,
			XDY_Octet va_validite)
{
  FILE *pl_fd;
  int                   vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
   XZSMT_Horodate	vl_horo_chaine;
 char			vl_str[256],vl_date_fic[30], vl_date[30];
  int			vl_long;
  char	pl_nom_fichier[256];

  XZST_03EcritureTrace(XZSTC_FONCTION,"asts_insere_ecrit_trafic IN vl_horodate %f, vl_station %d, vl_sens %d, vl_Q %d, vl_V %d, vl_TO %d, vl_PL %d, vl_validite %d",va_horodate, va_station, va_sens, va_Q, va_V, va_TO, va_PL, va_validite);
  XZSM_11ConversionHorodate(va_horodate,&vl_horo_chaine);
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
  if (vl_annee!=1970) {
    sprintf(vl_date_fic,"%02d%02d%02d%02d%04d",
                        vl_minute, vl_heure,vl_jour,vl_mois,vl_annee);
    sprintf(vl_date,"%02d/%02d/%04d %02d:%02d",
			vl_jour,vl_mois,vl_annee,vl_heure,vl_minute);
    }
  else
    {
	XZST_03EcritureTrace(XZSTC_WARNING,"asts_insere_ecrit_trafic : Date donnees null");
	return (XDC_NOK);
    }
 
   
               /* generons le nom du fichier  */
  sprintf(pl_nom_fichier,"%s%s_%s\0",CO_REP_TRAFOUT_M,vl_date_fic,C_DUREE_METACOR);

  XZST_03EcritureTrace(   XZSTC_INFO, " nom du fichier = %s ", pl_nom_fichier);
        

  /*ouverture du fichier*/
  if ((pl_fd=fopen(pl_nom_fichier,"a"))==NULL) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_evt_ecrit_metacor: pb creation fichier %s",
		pl_nom_fichier);
    return (XDC_NOK);
  }

  if ( va_validite != 1)
	va_validite =0;
  
  	/* Ecriture des données */

  fputs("{\n",pl_fd);
  switch (va_sens)
  {
  case XDC_SENS_ENTREE_SUD :
  	XZST_03EcritureTrace(   XZSTC_INFO, " entre sud : %d",va_sens);
	sprintf(vl_str,"st_id : %04d_S%d_E\n",va_station,XDC_SENS_SUD);
	break;
  case XDC_SENS_SORTIE_SUD :
  	XZST_03EcritureTrace(   XZSTC_INFO, " sortie sud : %d",va_sens);
  	sprintf(vl_str,"st_id : %04d_S%d_S\n",va_station,XDC_SENS_SUD);
	break;
  case XDC_SENS_SUD : 	
  	XZST_03EcritureTrace(   XZSTC_INFO, " sud : %d",va_sens);
  	sprintf(vl_str,"st_id : %04d_S%d\n",va_station,XDC_SENS_SUD);
	break;
  case XDC_SENS_ENTREE_NORD :
  	XZST_03EcritureTrace(   XZSTC_INFO, " entre nord : %d",va_sens);
  	sprintf(vl_str,"st_id : %04d_S%d_E\n",va_station,XDC_SENS_NORD);
	break;
  case XDC_SENS_SORTIE_NORD :
  	XZST_03EcritureTrace(   XZSTC_INFO, " sortie nord : %d",va_sens);
  	sprintf(vl_str,"st_id : %04d_S%d_S\n",va_station,XDC_SENS_NORD);
	break;
  case XDC_SENS_NORD : 	
  	XZST_03EcritureTrace(   XZSTC_INFO, " nord : %d",va_sens);
  	sprintf(vl_str,"st_id : %04d_S%d\n",va_station,XDC_SENS_NORD);
	break;
  default :  
  	XZST_03EcritureTrace(   XZSTC_INFO, " Autre!!!! : %d",va_sens);
  	sprintf(vl_str,"st_id : %04d_S%d\n",va_station,va_sens);
	break;
  }

  fputs(vl_str,pl_fd);
  sprintf(vl_str,"hd : %s\n",vl_date);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"dur : %s\n",C_DUREE_METACOR);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"val : %d\n",va_validite);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"Qtv : %d\n",va_Q);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"Qpl : %d\n",(int)(va_Q*va_PL/100));
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"PCpl : %d\n",va_PL);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"TO : %d\n",va_TO);
  fputs(vl_str,pl_fd);
  sprintf(vl_str,"V : %d\n",va_V);
  fputs(vl_str,pl_fd);
  fputs("}\n",pl_fd);

  fclose(pl_fd);
    XZST_03EcritureTrace(XZSTC_FONCTION,"asts_insere_ecrit_trafic OUT");

return XDC_OK;
}
