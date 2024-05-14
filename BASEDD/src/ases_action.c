/*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : ases_action.c				*
*************************************************
* DESCRIPTION :
* module externe de la tache serveur partie action
* pour le PC STRADA
*************************************************
* HISTORIQUE :
*
* JMG	02/03/04 creation (SAGA)
* JMG	05/09/05 ajout param en plus dans xzae127 1.2
* JMG 	20/09/05 parametre STA valorise a INP s'il est nul 1.3
* JMG	28/11/05 cle de l action est la bonne (et plus le CI) 1.4
* niepceron  11/12/06 1.5 : LTV =5 DEM732
* PNI         21/10/09 1.4 : ajout du caractère a dans le champ SNM pour différencier les action des evt DEM901
* JPL	18/11/10 : Suppression warnings mineurs pour compilation sur architecture HP ia64 (DEM 961)  1.9
* JMG	15/02/11 : ajout nom autoroute dans XZAE146
* SFO   06/10/14 : Adaptation pour compilation sous linux
* JMG	23/06/15 : correction vl_str -> 51 pour appel a ZAE146 1.13
* PNI	9/9/2015 : Remplacement Frontiere Italienne par Italie pour L07 lim. 35car DEM1145 1.13
* PNI	9/9/2015 : Remplacement LOC par P1.1 + integration heure hivers/ete DEM1145 1.13
************************************************/


#include "ases_evt.h"

#define CO_NB_JOUR_PURGE        3
#define CO_DELAI_FREQUENCE      360
#define CO_DELAI_PURGE          86400 * CO_NB_JOUR_PURGE
#define CO_NB_CHIFFRE           13

#define C_PC_STRADA             "STRADA"

/* definition des constantes */

#define CO_FIC_COMPTEUR         XDC_PATH_STRADA "/compteur_evt.txt"
#define CO_FIC_EVTSTR		XDC_PATH_STRADA "/evt_out.txt"
#define	CO_MAX_CARACTERE	80
#define TAILLE 200
#define C_FREQ_PURGE		86400 * 3
#define C_TAB_SEPAR		"\t"
#define	C_FREQ_VERIF_EVT	6
#define C_CODE_ESCOTA		"ESCOTA"
#define C_DATEX_ESCOTA		"fr204"

#define C_PATH 			XDC_PATH_STRADA	"/evtout"
#define C_SCRIPT_FTP 		XDC_PATH_STRADA "/ftp/sit_script_put"

#define NBR_PREMIER 	53
#define NBR_ELT_TABLE	59

#define C_l01		"F17"
#define C_l02		"P1.1"
#define C_l03		4
#define C_l07_P		"Aix en Provence -> Frontiere Italienne"
#define C_l07_N		"Frontiere italienne -> Aix en Provence"
#define C_l07_U		"Sens non défini"


#define SG_PING         "/etc/ping"
#define SG_TO_PING      8
#define SG_BYTES_PING   64
#define SG_COUNT_PING   1
#define SG_TOKEN_TST    "64"

/* dÅfinition des variables globales È l'application */


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
	"Le Luc -> Toulon"};

static char			*tm_l07_n[]={
	"",
	"Italie -> Aix en Provence",
	"MonacoA8 -> Monaco",
	"Toulon -> Marseille",
	"A52 -> Marseille",
	"Les Alpes -> Aix",
	"Aubagne -> Aix",
	"Auriol -> A52",
	"Toulon -> Le Luc"};




static char *	version = "@(#)ases_tfm.c	1.2 03/19/99 : commentaire" ;


/* declaration de fonctions internes */
void	ases_action_cb();
int	ases_action_genere_strada(XDY_Entier, XDY_Octet ,XDY_Horodate, XDY_Horodate, char *, XDY_Entier , XDY_Octet );

/* definition de fonctions externes */
extern char *heurehiversete(int, int, int);


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

int ases_action_init()
{
XZSMT_CleProg		vl_cle;
XZSCT_Datagroup 	vl_Datagroup ;
int 			vl_JourSemaine;
double 			vl_HorodateSec = (double) 0.0;
XZSCT_NomMachine 	pl_NomMachine	= "\0";
T_BOOL 			vl_etat;
int			vl_val;
char			vl_str1[80];
char			*vl_chaine;
int			vl_ValRet;
XDY_NomSite		pl_NomSite;

  	/* Recuperation du nom de la machine locale */
	XZSC_07NomMachine(pl_NomMachine) ;
  	
	/*A
        * Initialisation de la callback du timer de purge
        * -----------------------------------------------
        */

	/*abonnement au dg pour les validations de fmc */
	XDG_JoindreDG(XDG_AS_SRVEVT_STRADA);

	/* armement du timer de programmation */
#ifdef _HPUX_SOURCE
	if (XZSM_01DemanderProgrammation(       vl_HorodateSec,
						C_FREQ_VERIF_EVT,
						XDG_AS_SRVEVT_STRADA,
						XDM_AS_SRVEVT_STRADA,
						vl_cle,
						NULL) != XDC_OK)
#else
	if (XZSM_01DemanderProgrammation(       vl_HorodateSec,
						C_FREQ_VERIF_EVT,
						XDG_AS_SRVEVT_STRADA,
						XDM_AS_SRVEVT_STRADA,
						vl_cle,
						0) != XDC_OK)
#endif
	{
	  XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer de verif des tfms non declenchee." );
	  return (XDC_NOK);
  	}

	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_SRVEVT_STRADA), ases_action_cb,(T_CB_ARG)NULL) == NULL)
	{
		XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AS_SRVEVT_STRADA non cree.",version );
		return XDC_NOK;
	}

	tg_fmc=NULL;
	tg_fmc_courant=NULL;

	/* Ecriture Trace */
	XZST_03EcritureTrace( XZSTC_INFO, "OUT : ases_tfm_init   cle : %s" ,vl_cle);
	return XDC_OK;

}



/* Fichier : @(#)ases_tfm.c	1.2        Release : 1.2        Date : 03/19/99
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* verifie si des actions sont a exporter
*****************************************************
* SEQUENCE D'APPEL :
* ases_action_cb
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
void ases_action_cb()
{
  XZST_03EcritureTrace(XZSTC_INFO,"ases_action_cb: debut de fonction !");
  
  if (  XZAC81_Rechercher_Dernieres_Actions(ases_action_genere_strada) == XDC_NOK )
  	XZST_03EcritureTrace(XZSTC_WARNING,"Erreur a l'appel de XZAC81_Rechercher_Dernieres_Actions");
   return ;
}





/* Fichier : @(#)ases_tfm.c	1.2        Release : 1.2        Date : 03/19/99
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* generation du fichier strada d'une action
*
*****************************************************
* SEQUENCE D'APPEL :
* ases_action_genere_strada
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
int ases_action_genere_strada(XDY_Entier va_action,
				XDY_Octet va_niveau,
				XDY_Horodate va_succes,
				XDY_Horodate va_fin,
				char *pa_texte,
				XDY_Entier va_fmc,
				XDY_Octet va_cle)
{
  T_Fichier		vl_bloc;
  XDY_Horodate		vl_debut,vl_fin,vl_validation;
  XDY_Nom		vl_fausse_alerte,vl_fin_flag;
  XDY_Entier		vl_duree;
  XDY_Entier		vl_num_cause;
  XDY_Octet		vl_cle_cause;
  XDY_Nom		vl_type_cause;
  XDY_Octet		vl_veracite,vl_trafic;
  XDY_Nom		vl_prevision;
  XZSMT_Horodate	vl_horo_chaine;
  int                   vl_jour, vl_mois, vl_minute, vl_heure, vl_seconde, vl_annee;
  int                   vl_jourc, vl_moisc, vl_minutec, vl_heurec, vl_secondec, vl_anneec;
  int			vl_annee1,vl_annee2;
  XDY_Booleen		vl_veh_esc,vl_hom_esc,vl_perso;
  XDY_Mot		vl_plan;
  char			tl_string_comment[400];
  char			tl_string[400],tl_buffer[15],tl_loc_float[15];
  int			vl_i;
  int			vl_visibilite, vl_type,vl_nombre,vl_perception,vl_violence;
  int			vl_bebes,vl_agees,vl_animaux,vl_escota;
  char			tl_matiere[100],tl_observations[100],tl_nom[100];
  XDY_Octet		vl_octet1, vl_octet2, vl_octet3, vl_octet4;
  XDY_Booleen		vl_bool1,vl_bool2,vl_bool3,vl_bool4;
  XDY_Entier		vl_entier1=1,vl_entier2,vl_entier3,vl_entier4;
  XDY_Mot		vl_mot1,vl_mot2,vl_mot3;
  char			vl_localisation[10];
  char			vl_str1[51],vl_str2[51],vl_str3[51];
  loc_rds		vl_loc;
  XDY_Entier		vl_pr,vl_pr_inf,vl_pr_sup;
  XDY_Octet		vl_sens;
  XDY_Octet		vl_autoroute;
  XDY_Horodate	vl_Horodate;
  int vl_Jour_De_La_Semaine;
  XDY_Octet		vl_type_horodate=XDC_ACT_ETAT_SUCCES;
  int			vl_indice=0;
   XDY_Octet		vl_nbvoiesop;
   XDY_Octet		vl_gravite;
  char                  vl_reference[36]; /* Ajout SAGA */

  
  XZST_03EcritureTrace(XZSTC_INFO,"ases_action_genere_strada: IN action:%d,niv:%d,succes:%lf,fin:%lf,texte:%s,fmc:%d,cle:%d",
		va_action, va_niveau,va_succes,va_fin,pa_texte,va_fmc,va_cle);

  /*remplissage des champs dont on a deja les infos*/
  sprintf(vl_bloc.Bloc_Oblig.SNM,"SNM#%s/a%d/%d#\n"
				  ,C_CODE_ESCOTA
				  ,va_action
				  ,va_niveau);
  strcpy(vl_bloc.Bloc_Oblig.ERF,"ERF#1#\n");
  vl_bloc.Bloc_Oblig.CLV=1;
  vl_bloc.Bloc_Oblig.LTV=CM_V_ALERT_C;
  vl_bloc.Bloc_Oblig.VNM=1; 

  if ( !strncmp(pa_texte,"envoi patrouille",10) || !strncmp(pa_texte,"message fourgon",10) )
	{
  	sprintf(vl_bloc.Bloc_Oblig.DOB,"DOB#RMT#\n");
  	sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#RMV#\n");
	}
  else
	{
  	sprintf(vl_bloc.Bloc_Oblig.DOB,"DOB#OPA#\n");
  	sprintf(vl_bloc.Bloc_Oblig.PHR,"PHR#CLE#\n");
	}

  /*recherche en base les infos communes a l'tfm qui doivent etre exportees*/
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
  if (XZAE127_Infos_obligatoires(va_fmc,
				va_cle,
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
				vl_reference) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_action_genere_strada: pb appel a XZAE127_Infos_obligatoires %d %d",
				va_fmc,
				va_cle);
    return (XDC_NOK);
  }
  vl_bloc.Bloc_Oblig.VNM = vl_entier1;
  
  if (XZAE146_Recherche_Loc_Rds(vl_pr,
  				vl_autoroute,
   				vl_loc.l01,
 				&vl_entier1 ,
  				&vl_entier2 ,
				vl_str1,
				vl_str2,
				&vl_entier3,
  				&vl_entier4,vl_str3 ) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_action_genere_strada: pb appel a XZAE146_Recherche_Loc_Rds %d %d",
				vl_pr,
				va_cle);
    return (XDC_NOK);
  }
  
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
  }
  
  


  vl_loc.l08=0;
  
  XZST_03EcritureTrace(XZSTC_INFO,"ases_action_genere_strada: pr 1: %d   pr 2: %d",vl_loc.l11,vl_loc.l12);
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
  
  sprintf(vl_bloc.loc.l11,"L11#%d#\n",vl_loc.l11);
  
  sprintf(vl_bloc.loc.l12,"L12#%d#\n",vl_loc.l12);
  
XZST_03EcritureTrace(XZSTC_INFO,"ases_action_genere_strada: l11: %s - l12 : %s",vl_bloc.loc.l11,vl_bloc.loc.l12);


  sprintf(vl_bloc.Bloc_Optio.Cause,"LNK#%s/%d/%d#\n",
		C_CODE_ESCOTA,
		va_fmc,
		va_cle);
  
 if (XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_Jour_De_La_Semaine,&vl_Horodate) != XZSMC_OK)
                XZST_03EcritureTrace(XZSTC_WARNING, "Erreur a la lecture de l'heure courante") ;
  XZSM_11ConversionHorodate(vl_Horodate,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jourc,
                        &vl_moisc,
                        &vl_anneec,
                        &vl_heurec,
                        &vl_minutec,
                        &vl_seconde ) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_action_genere_strada: format date incorrect");
    return (XDC_NOK);
  }
  sprintf(vl_bloc.Bloc_Oblig.INP,"INP#%04d%02d%02d%02d%02d%s#\n",
                        vl_anneec,
                        vl_moisc,
                        vl_jourc,
                        vl_heurec,
                        vl_minutec,
			heurehiversete(vl_jourc,vl_moisc,vl_anneec) );

  XZSM_11ConversionHorodate(va_succes,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date succes: %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
			&vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_action_genere_strada: format date incorrect");
    return (XDC_NOK);
  }
  if (vl_annee != 1970) {
    sprintf(vl_bloc.Bloc_Optio.Date_Debut,"STA#%04d%02d%02d%02d%02d%s#\n",
			vl_annee,
			vl_mois,
			vl_jour,
			vl_heure,
			vl_minute,
			heurehiversete(vl_jour,vl_mois,vl_annee) );
  }
  else {
    sprintf(vl_bloc.Bloc_Optio.Date_Debut,"STA#%04d%02d%02d%02d%02d%s#\n",
			vl_anneec,
			vl_moisc,
			vl_jourc,
			vl_heurec,
			vl_minutec,
			heurehiversete(vl_jourc,vl_moisc,vl_anneec) );
  }
    
  /* S'il y a une date de fin alors on termine l'action exporter */
  if ( va_fin != 0.0 )
  {	XZST_03EcritureTrace(XZSTC_INFO,"Evt fini!");
  	vl_type_horodate=XDC_ACT_ETAT_FIN;
  }
  	
  XZSM_11ConversionHorodate(va_fin,&vl_horo_chaine);
  XZST_03EcritureTrace(XZSTC_INFO,"date : %s",vl_horo_chaine);
  if (sscanf( vl_horo_chaine, "%2d/%2d/%4d %2d:%2d",
                        &vl_jour,
                        &vl_mois,
                        &vl_annee,
                        &vl_heure,
                        &vl_minute) != 5) {
    XZST_03EcritureTrace(XZSTC_WARNING,"ases_action_genere_strada: format date incorrect");
    return (XDC_NOK);
  }
  if (vl_annee!=1970)
  {
    sprintf(vl_bloc.Bloc_Optio.Date_Fin,"STO#%04d%02d%02d%02d%02d%s#\n",
                        vl_annee,
                        vl_mois,
                        vl_jour,
                        vl_heure,
                        vl_minute,
			heurehiversete(vl_jour,vl_mois,vl_annee) );
  vl_bloc.Bloc_Optio.Fin='Y';
  vl_bloc.Bloc_Oblig.VNM += 1;
  }
  else
  {
    strcpy(vl_bloc.Bloc_Optio.Date_Fin,"");
   vl_bloc.Bloc_Optio.Fin='N';
  }

  XZST_03EcritureTrace(XZSTC_INFO,"%s %s %c %s %s",
			vl_bloc.Bloc_Optio.Date_Debut,
			vl_bloc.Bloc_Optio.Date_Fin,
			vl_bloc.Bloc_Optio.Fin,
			vl_bloc.Bloc_Optio.Cause,
			vl_bloc.Localisation);
 sprintf (vl_bloc.Bloc_Optio.Comment,"SUR#");
 
 /* Suppresion des espaces de fin */
 vl_indice=strlen(pa_texte)-1;
 while ((vl_indice >= 0) && (pa_texte[vl_indice]==' '))
 {
    pa_texte[vl_indice]='\0';
    vl_indice--;
 }

 
 vl_indice=0;
 while 	( pa_texte[vl_indice]!='\0' )
 {
 if (pa_texte[vl_indice] == '\n')
 	vl_bloc.Bloc_Optio.Comment[vl_indice+4]=' ';
 else
 	vl_bloc.Bloc_Optio.Comment[vl_indice+4]=pa_texte[vl_indice];
 vl_indice++;
 }
 
 vl_bloc.Bloc_Optio.Comment[vl_indice+4]='#';
 vl_bloc.Bloc_Optio.Comment[vl_indice+5]='\0';
	    	
 if (XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_Jour_De_La_Semaine,&vl_Horodate) != XZSMC_OK)
                XZST_03EcritureTrace(XZSTC_WARNING, "Erreur a la lecture de l'heure courante") ;

  /*ecrit le fichier strada et envoie le sur le PC strada*/
  if ( ases_tfm_ecrit_strada(vl_bloc) == XDC_NOK )
  {
                XZST_03EcritureTrace(XZSTC_WARNING, "Erreur a l'envoi du fichier") ;
  }

  return (XDC_OK);
}

