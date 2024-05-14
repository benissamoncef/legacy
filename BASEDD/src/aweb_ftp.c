/*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : aweb_evt.c				*
*************************************************
* DESCRIPTION :
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
*************************************************
* HISTORIQUE :
CLUCHAGUE 18/06/2004 V13 : Le fichier crise s'appelle ESCOTA_INFO_CRISE et non ESCOTA_CRISE
CLUCHAGUE 21/06/2004 V14 : info carte + crochet dermant dans le champ cdata de l'element titre
			   + pas de reference www.asf.fr 
Hilmarcher 01/07/2004 V15: modif horodate
JMG	20/10/04	export RDT pour calcul TDP par asf
JMG	15/12/04	ajout fichier ParcoursWeb 
PNI	15/12/05	generation tdp a partir du radt DEM545
PNI	16/08/06	modif entete fichier info crise + envoi d'un msg vide si suppression DEM577 1.9
JMG	16/09/08	refonte passerelle generique 1.10 DEM 836
JMG	16/11/08	table de verite des fermetures echangeur 1.11 DEM 836
JMG	26/11/08	messages "sur bretelle" pour les evts sur bretelle 1.12 DEM 836
JMG   17/12/08        parametre du flag de diff tdp v1.13 DEM 836
JMG   17/12/08        suppression "sur bretelle" pour evt obligatoire sur bretelle v1.14 DEM 836
PNI	04/03/09	: Un delestage ne produits pas de troncon noir cf LLE 04/03/09 DEM869 v1.15
PNI	04/03/09	: En cas de deviation, si l'ech de reinjection est sur une autre autoroute, alors on noircit uniquement le troncon contenant le PR de sortie (PR debut=PR fin) DEM8xx v1.16
PNI	21/12/09	: suppression de la correction de sens de l'A57 dans aweb_ftp_filtre DEM924 v1.16
PNI 	11/05/10	: Correctif dans la coloration des troncons v1.17
JPL	31/08/10	: Ajout du type Fmc "Zone de stockage" (DEM 942) 1.18
JPL	01/09/10	: Longueur des Travaux et Zone de stockage 1.19
PNI	09/02/11	: Supression du rm des fichierss déjà fait dans le script 
			  DEL974 : correction coloriage trc noir 1.20, 1.21
JPL	15/09/11	: Augmentation de la taille des donnees des messages de crise (DEM 993)  1.22
JMG	09/02/12	: correction consequence 1.23
PNI	23/07/11	: suppression tdp a partir du radt DEM1044 1.24
PNI	17/01/13	: modif pour compil linux  1.25
JMG	11/11/12	: correction faux bouchon 1 km 
JMG	15/02/13	: idem
SFO     06/10/14        : Adaptation pour compilation sous linux
PNI	04/07/16	: aweb_ftp_modif_couleur: prise en compte du sens du tronçon+ prise en compte FMC Délestage (correction sur DEM869) DEM 1178
************************************************/


#include "aweb_ftp.h"

#define C_PC_ASF             "ASFWEB"

/* definition des constantes */

#define CO_REP_OUT		XDC_PATH_STRADA "/web/"
#define CO_REP2_OUT		XDC_PATH_STRADA "/tdp/"
#define CO_REP_OUT2		XDC_PATH_STRADA "/web2/"

#define C_NOM_TRAFIC		"ESCOTA_TRAFIC_NIVEAU"
#define C_NOM_PERT		"ESCOTA_EVT_P_NIVEAU"
#define C_NOM_SERVICE		"ESCOTA_EVT_S_NIVEAU"
#define C_NOM_DATE		"ESCOTA_DATE_TRAFIC"
#define C_NOM_CRISE		"ESCOTA_INFO_CRISE"
#define C_NOM_RDT		"ESCOTA_CPTG6_"	/*TDPASF*/
#define C_NOM_PARCOURS		"ESCOTA_parcoursWeb_"
#define C_NOM_TDP		"ESCOTA_TDP_"
#define C_NOM_ZDP		XDC_PATH_STRADA "/web_zdp.txt"
#define C_NOM_CFGRDT		XDC_PATH_STRADA "/web_rdt.txt"
#define C_NOM_TDP_DIFF		XDC_PATH_STRADA "/web_tdp_diff.txt"

#define C_ENTETE		"Name,Value"
#define C_ENTETE_EVT		"Clip,X,Y,Xscale,Yscale,Rotate,symbole,txt"
#define C_PREFIX_PERTURBATION	"ep"
#define C_PREFIX_SERVICE	"es"

#define C_SCRIPT_FTP 		XDC_PATH_STRADA "/ftp/web_script_put"

#define SG_PING         "/etc/ping"
#define SG_TO_PING      8
#define SG_BYTES_PING   64
#define SG_COUNT_PING   1
#define SG_TOKEN_TST    "64"

#define C_ETAT_VERT	XDC_RADT_FLUIDE
#define C_ETAT_ORANGE	XDC_RADT_PRESAT
#define C_ETAT_ROUGE	XDC_RADT_SATURE
#define C_ETAT_NOIR	4

/* dÅfinition des variables globales È l'application */


/* definition des variables locales au module */
struct T_Troncon	*tm_troncons=NULL;

struct T_Icone		*tm_perturbations_nv1=NULL;
struct T_Icone		*tm_perturbations_nv2=NULL;
struct T_Icone		*tm_services_nv1=NULL;
struct T_Icone		*tm_services_nv2=NULL;

struct T_Comptage	*tm_stations=NULL; /*TDPASF*/
struct T_Comptage	*tm_stations_courant=NULL; /*TDPASF*/

struct T_Parcours	*tm_parcours=NULL;

struct T_ZDP		*tm_zdp=NULL;
struct T_ZDP		*tm_zdp_courant=NULL;

int vm_numero;
int  vm_local;
char vm_tdp_diff;
char vm_suppression[5];
int vm_duree;
char vm_texte[510 + 1];
char vm_titre[200 + 1];
char vm_horodate[30];
static int vm_un_sur_deux=0;
char vm_ligne[300];
char vm_str[50];
XZSMT_Horodate  vm_HorodateZero = "";

static char *	version = "@(#)aweb_ftp.c	1.26 1.26 08/28/13" ;


static char                     *tm_autoroutes[]={
	"",
	"A8",
	"A500",
	"A50",
	"A501",
	"A51",
	"A52",
	"A520",
	"A57"};

/* declaration de fonctions internes */
void	aweb_ftp_calcul();

int aweb_ftp_calcul_troncons(char * , int , char * ,
			int , XDY_Octet , XDY_Octet ,
			XDY_Octet , XDY_Octet , XDY_Octet ,
			XDY_Autoroute , XDY_Sens , XDY_PR ,
			XDY_PR );

int aweb_ftp_calcul_icones(char * , int , char * ,
			int , int , int , int ,
			int , char * ,
			char *, char *,
			int );

int aweb_ftp_modif_couleur(int, int, int);


void web_cumul_voies(XDY_Voie ,int *,int *, int *, int *, int *);


/*TDPASF*/
int aweb_ftp_calcul_rdt(char * , char * , XDY_Octet , int , int , int , int , int , int);

int aweb_calcul_crise(int, int, int, int, char *, char *, char*);

int aweb_calcul_comptage(char *, char *, int, int,int, int , int, int); /*TDPASF*/

int aweb_ftp_calcul_parcours(char *, char, int);

void aweb_ecrit_fichier_trafic (char *, int);
void aweb_ecrit_fichier_evt (char *,struct T_Icone *,int, int);
void aweb_ecrit_fichier_date(char *, char *);
void aweb_ecrit_fichier_crise(char *);
void aweb_ecrit_fichier_rdt(char *); /*TDPASF*/
void aweb_ecrit_fichier_parcours(char *);
void aweb_ftp_genere();
void aweb_ftp_diffuse();
char *aweb_ftp_get_heure();
char *aweb_neutralisations(XZLGT_Liste_FMC );
char *aweb_circulation(XZLGT_Liste_FMC );
char *aweb_restrictions(XZLGT_Liste_FMC );
char *aweb_consequence(XZLGT_Liste_FMC );

int aweb_ftp_colorie(XDY_Octet ,XDY_Autoroute ,XDY_Sens,XDY_PR , XDY_PR );

/* definition de fonctions externes */



/*A
*******************************************
*******************************************
* Arguments : Aucun
*******************************************/

int aweb_ftp()
{
  /*calcul des troncons et icones*/
  aweb_ftp_calcul();

  /*generation des fichiers a diffuser*/
  aweb_ftp_genere();

  /*diffusion FTP vers les ASF*/
  aweb_ftp_diffuse();

  /*liberation des ressources*/
  aweb_free_ressources();
}


/* Fichier : @(#)aweb_ftp.c	1.26       Release : 1.26        Date : 08/28/13
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE aweb_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* libere les ressources
*
******************************************************
* SEQUENCE D'APPEL :
* aweb_free_ressources();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
extern int aweb_free_ressources()
{
  struct T_Troncon	*pl_ex_troncon;
  struct T_Icone	*pl_ex_icone;
  struct T_Comptage	*pl_ex_station; /*TDPASF*/
  struct T_Parcours	*pl_ex_parcours;
  struct T_ZDP		*pl_ex_zdp;

  XZST_03EcritureTrace(XZSTC_FONCTION,"aweb_free_ressources: IN");
/*
  while (tm_zdp != NULL){
    pl_ex_zdp = tm_zdp;
    tm_zdp = tm_zdp->Suivant;
    free(pl_ex_zdp);
  }

  while (tm_troncons != NULL) {
    pl_ex_troncon = tm_troncons;
    tm_troncons = tm_troncons->Suivant;
    free(pl_ex_troncon);
  }
*/
  while (tm_perturbations_nv1!=NULL) {
    pl_ex_icone=tm_perturbations_nv1;
    tm_perturbations_nv1=tm_perturbations_nv1->Suivant;
    free(pl_ex_icone);
  }

  while (tm_perturbations_nv2!=NULL) {
    pl_ex_icone=tm_perturbations_nv2;
    tm_perturbations_nv2=tm_perturbations_nv2->Suivant;
    free(pl_ex_icone);
  }

  while (tm_services_nv1!=NULL) {
    pl_ex_icone=tm_services_nv1;
    tm_services_nv1=tm_services_nv1->Suivant;
    free(pl_ex_icone);
  }

  while (tm_services_nv2!=NULL) {
    pl_ex_icone=tm_services_nv2;
    tm_services_nv2=tm_services_nv2->Suivant;
    free(pl_ex_icone);
  }

  /*TDPASF*/
  /*
  while (tm_stations!=NULL) {
    pl_ex_station=tm_stations;
    tm_stations=tm_stations->Suivant;
    free(pl_ex_station);
  }

  while (tm_parcours!=NULL) {
    pl_ex_parcours=tm_parcours;
    tm_parcours=tm_parcours->Suivant;
    free(pl_ex_parcours);
  }
*/

  XZST_03EcritureTrace(XZSTC_FONCTION,"aweb_free_ressources: OUT");
  return (XDC_OK);
}


/* Fichier : @(#)aweb_ftp.c	1.26        Release : 1.26        Date : 08/28/13
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE AWEB
******************************************************
* DESCRIPTION DE LA FONCTION :
* 
*****************************************************
* SEQUENCE D'APPEL :
* 
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
void aweb_ftp_calcul()
{
  struct T_Troncon       *pl_liste;
  struct T_Comptage 	*pl_liste2;

  XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul :debut");
  XZST_03EcritureTrace(XZSTC_INFO,"aweb_ftp_calcul :traitement des troncons");

  /*initialisation a VERT des troncons*/
  pl_liste=tm_troncons;
  while (pl_liste!=NULL) {
    pl_liste->Etat = C_ETAT_VERT;
    pl_liste->Rouge = 45;
    pl_liste->Vert = 180;
    pl_liste->Bleu = 45;
    pl_liste=pl_liste->Suivant;
  }
 
  /* recuperation des derniers niveaux de trafic*/
  /*if (XZLG202_GetNiveaux() != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel XZLG202_GetNiveaux");
    return;
  }*/

  /*modif de la couleur des troncons*/
  if (XZLG203_ParcoursRADT(aweb_ftp_modif_couleur) != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel XZLG203_ParcoursRADT");
    return;
  }

  /*verrouille la memoire*/
  XZLG500_SemCall(-1,XZLGC_SEM_FMC);

  /*on filtre les evts a ne pas exporter*/
  if (aweb_ftp_filtre()!=XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel aweb_ftp_filtre");
    XZLG500_SemCall(1,XZLGC_SEM_FMC);
    return;
  }

  /*calcul des textes Web*/
  aweb_ftp_genere_texte();

  /*calcul des troncons impactes*/
  aweb_ftp_genere_icones();

  /*deverrouille la memoire*/
  XZLG500_SemCall(1,XZLGC_SEM_FMC);

  
  if (XZAE163_Genere_Textes() != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel XZAE163_Genere_Textes");
    return;
  }


  /*recuperation des icones pour le Web*/
  if (XZAE165_Lire_Icones((XDY_FonctionInt)aweb_ftp_calcul_icones,
				  (XDY_Octet)XDC_WEB_NIVEAU_1) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel XZAE165_Lire_Icones niveau 2");
    return;
  }

  /*recuperation des icones pour le Web*/
  if (XZAE165_Lire_Icones((XDY_FonctionInt)aweb_ftp_calcul_icones,
				  (XDY_Octet)XDC_WEB_NIVEAU_2) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel XZAE165_Lire_Icones niveau 2");
    return;
  }

  /*TDPASF*/
  /*lecture des donnees RDT pour le calcul des TDP*/
  if (vm_un_sur_deux==0) 
  {
    vm_un_sur_deux=1;
  XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul :radt");

    pl_liste2=tm_stations;
    while (pl_liste2!=NULL) {
    XZST_03EcritureTrace(XZSTC_INFO,"aweb_ftp_calcul : %d/%d",pl_liste2->Numero,pl_liste2->Sens);
#ifdef _HPUX_SOURCE
      if (XZLG205_GetRADT_Data_Station(pl_liste2->Numero,pl_liste2->Sens,aweb_ftp_calcul_rdt) != XDC_OK)
#else
	if (XZLG205_GetRADT_Data_Station(pl_liste2->Numero,pl_liste2->Sens,(XDY_FonctionInt)aweb_ftp_calcul_rdt) != XDC_OK)
#endif
      {
	  XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul : pb appel XZLG205_GetRADT_Data_Station");
	  return;
      }
      pl_liste2=pl_liste2->Suivant;
    }

/* Calcul des Tdp via rdt */
	/* DEM 545*/
   aweb_ftp_tdp_rdt();
}
  else 
    vm_un_sur_deux=0;

  
  XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_calcul :fin");
}

/*A
*******************************************
* Fonction d'insertion d'un troncon
*******************************************
*******************************************/

int aweb_ftp_calcul_troncons(char * va_identifiant, int va_niveau, char * va_nom_vue,
			int va_etat, XDY_Octet va_station1, XDY_Octet va_station2,
			XDY_Octet va_station3, XDY_Octet va_station4, XDY_Octet va_station5,
			XDY_Autoroute va_autoroute, XDY_Sens va_sens, XDY_PR va_debut,
			XDY_PR va_fin)
{
struct T_Troncon       *pl_new_troncon;

	XZST_03EcritureTrace(XZSTC_INTERFACE,"aweb_ftp_calcul_troncons : id %s, vue %s, etat %d, niveau %d, stations =%d/%d%/%d/%d/%d",
					va_identifiant,
					va_nom_vue,
					va_etat,
					va_niveau,
					va_station1,
					va_station2,
					va_station3,
					va_station4,
					va_station5);

        if (( pl_new_troncon = (struct T_Troncon *) malloc (sizeof(struct T_Troncon))) != NULL)
        {
                strcpy(pl_new_troncon->Identifiant , va_identifiant);
                pl_new_troncon->Niveau = va_niveau;
                strcpy(pl_new_troncon->NomVue, va_nom_vue);
		pl_new_troncon->Station1 = va_station1;
		pl_new_troncon->Station2 = va_station2;
		pl_new_troncon->Station3 = va_station3;
		pl_new_troncon->Station4 = va_station4;
		pl_new_troncon->Station5 = va_station5;
		pl_new_troncon->Autoroute = va_autoroute;
		pl_new_troncon->Sens = va_sens;
		pl_new_troncon->Debut = va_debut;
		pl_new_troncon->Fin = va_fin;
		
		pl_new_troncon->Etat = va_etat;
		switch (va_etat)
		{
		case (XDC_WEB_ROUGE) : pl_new_troncon->Rouge = 255;
				       pl_new_troncon->Vert = 0;
				       pl_new_troncon->Bleu = 0;
				       break;

		case (XDC_WEB_NOIR) : pl_new_troncon->Rouge = 0;
				      pl_new_troncon->Vert = 0;
				      pl_new_troncon->Bleu = 0;
				      break;

                case (XDC_WEB_VERT) : pl_new_troncon->Rouge = 45;
				      pl_new_troncon->Vert = 180;
				      pl_new_troncon->Bleu = 45;
				      break;

                case (XDC_WEB_ORANGE) : pl_new_troncon->Rouge = 250;
					pl_new_troncon->Vert = 150;
					pl_new_troncon->Bleu = 90;
					break;
                }

                pl_new_troncon->Suivant = tm_troncons;
                tm_troncons = pl_new_troncon;
        }
        else
        {
                XZST_03EcritureTrace(   XZSTC_WARNING, "aweb_ftp_calcul_troncons : le malloc a echoue !" );
		return XDC_NOK;
        }

        return XDC_OK;
}


/*A
*******************************************
* Fonction d'insertion d'une icone
*******************************************
*******************************************/

int aweb_ftp_calcul_icones(char *va_identifiant, int va_niveau, char * va_nom_vue,
			int va_X, int va_Y, int va_facteurX, int va_facteurY,
			int va_rotation, char * va_icone,
			char *va_texte, char *va_horodate,
			int va_type)
{
struct T_Icone       *pl_new_icone;

	XZST_03EcritureTrace(XZSTC_INTERFACE,"aweb_ftp_calcul_icones : id %s, vue %s, icone %s, texte %s, niveau %d, type %d",
					va_identifiant,
					va_nom_vue,
					va_icone,
					va_texte,
					va_niveau,
					va_type);

        if (( pl_new_icone = (struct T_Icone *) malloc (sizeof(struct T_Icone))) != NULL)
        {
                strcpy(pl_new_icone->Identifiant , va_identifiant);
                pl_new_icone->Niveau = va_niveau;
                strcpy(pl_new_icone->NomVue, va_nom_vue);
		pl_new_icone->X = va_X;
		pl_new_icone->Y = va_Y;
		pl_new_icone->FacteurX = va_facteurX;
		pl_new_icone->FacteurY = va_facteurY;
		pl_new_icone->Rotation = va_rotation;
		strcpy(pl_new_icone->Icone,va_icone);
		strcpy(pl_new_icone->Texte,va_texte);
		strcpy(pl_new_icone->Horodate,va_horodate);

		if (va_type == XDC_WEB_PERTURBATION) 
		{
		  if (va_niveau == XDC_WEB_NIVEAU_1)
		  {
                    pl_new_icone->Suivant = tm_perturbations_nv1;
                    tm_perturbations_nv1 = pl_new_icone;
                  }
		  else
		  { 
		    pl_new_icone->Suivant = tm_perturbations_nv2;
		    tm_perturbations_nv2 = pl_new_icone;
                  }
		}
		else if (va_type == XDC_WEB_SERVICE)
		{
		  if (va_niveau == XDC_WEB_NIVEAU_1)
		  {
		    pl_new_icone->Suivant = tm_services_nv1;
		    tm_services_nv1 = pl_new_icone;
		  }
		  else
		  {
		    pl_new_icone->Suivant = tm_services_nv2;
		    tm_services_nv2 = pl_new_icone;
                  }
		}
        }
        else
        {
                XZST_03EcritureTrace(   XZSTC_WARNING, "aweb_ftp_calcul_icones : le malloc a echoue !" );
		return XDC_NOK;
        }

        return XDC_OK;
}


/* Fichier : @(#)aweb_ftp.c	1.26       Release : 1.26        Date : 08/28/13
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE aweb_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* 
*
******************************************************
* SEQUENCE D'APPEL :
* aweb_ecrit_fichier_date();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
void aweb_ecrit_fichier_date(char * va_nom, char *va_date)
{
  char vl_ligne[300];
  FILE *pl_fp;
  struct T_Troncon *pl_liste;

  XZST_03EcritureTrace(XZSTC_FONCTION,"aweb_ecrit_fichier_date : fichier a generer : %s",va_nom);

  if ( (pl_fp = fopen( va_nom, "w+")) == NULL )
  {
    XZST_03EcritureTrace(XZSTC_WARNING,
		"aweb_ecrit_fichier_date : impossible de creer le fichier %s", va_nom);
    return;
  }

  sprintf(vl_ligne,"%s\n",C_ENTETE);
  fputs(vl_ligne,pl_fp);

  sprintf(vl_ligne,"ESCOTA_date_trafic,%s",va_date);
  fputs(vl_ligne,pl_fp);

  fflush(pl_fp);
  fclose(pl_fp);

  XZST_03EcritureTrace(XZSTC_INFO,"aweb_ecrit_fichier_date : generation fichier terminee");
}


/* Fichier : @(#)aweb_ftp.c	1.26       Release : 1.26        Date : 08/28/13
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE aweb_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* generation des fichiers texte a exporter
*
******************************************************
* SEQUENCE D'APPEL :
* aweb_ftp_genere();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
void aweb_ftp_genere()
{
  char pl_nom_fichier[300];
  char pl_fin_fichier[300];
  char pl_fichier[300];
  char vl_date[30];
  int vl_jour,vl_mois,vl_annee,vl_heure,vl_minute,vl_seconde; 
  char vl_horodate[30];

  sprintf(pl_nom_fichier,"%s",CO_REP_OUT);
  strcat(pl_nom_fichier,C_NOM_TRAFIC);

  strcpy(vl_date,aweb_ftp_get_heure());

  if (sscanf( vl_date, "%2d/%2d/%4d %2d:%2d:%2d",
                                  &vl_jour,
                                  &vl_mois,
                                  &vl_annee,
                                  &vl_heure,
                                  &vl_minute,
                                  &vl_seconde) != 6) {
     XZST_03EcritureTrace(XZSTC_WARNING,
            "aweb_ftp_genere : le format de la date est incorrecte !" );
     return;
  }
    
  sprintf(pl_fin_fichier,"_%02d%02d%04d%02d%02d.TXT",
			vl_jour,
			vl_mois,
			vl_annee,
			vl_heure,
			vl_minute);

  sprintf(vl_horodate,"%02d/%02d/%04d %02d:%02d",
			vl_jour,
			vl_mois,
			vl_annee,
			vl_heure,
			vl_minute);

  sprintf(pl_fichier,"%s%d%s",pl_nom_fichier,XDC_WEB_NIVEAU_0,pl_fin_fichier);
  aweb_ecrit_fichier_trafic(pl_fichier, XDC_WEB_NIVEAU_0);

  sprintf(pl_fichier,"%s%d%s",pl_nom_fichier,XDC_WEB_NIVEAU_1,pl_fin_fichier);
  aweb_ecrit_fichier_trafic(pl_fichier, XDC_WEB_NIVEAU_1);
  
  sprintf(pl_fichier,"%s%d%s",pl_nom_fichier,XDC_WEB_NIVEAU_2,pl_fin_fichier);
  aweb_ecrit_fichier_trafic(pl_fichier, XDC_WEB_NIVEAU_2);

  sprintf(pl_nom_fichier,"%s",CO_REP_OUT);
  strcat(pl_nom_fichier,C_NOM_PERT);
  sprintf(pl_fichier,"%s%d%s",pl_nom_fichier,XDC_WEB_NIVEAU_1,pl_fin_fichier);
  aweb_ecrit_fichier_evt(pl_fichier,tm_perturbations_nv1,XDC_WEB_NIVEAU_1,XDC_WEB_PERTURBATION);

  sprintf(pl_nom_fichier,"%s",CO_REP_OUT);
  strcat(pl_nom_fichier,C_NOM_SERVICE);
  sprintf(pl_fichier,"%s%d%s",pl_nom_fichier,XDC_WEB_NIVEAU_1,pl_fin_fichier);
  aweb_ecrit_fichier_evt(pl_fichier,tm_services_nv1,XDC_WEB_NIVEAU_1,XDC_WEB_SERVICE);

  sprintf(pl_nom_fichier,"%s",CO_REP_OUT);
  strcat(pl_nom_fichier,C_NOM_PERT);
  sprintf(pl_fichier,"%s%d%s",pl_nom_fichier,XDC_WEB_NIVEAU_2,pl_fin_fichier);
  aweb_ecrit_fichier_evt(pl_fichier,tm_perturbations_nv2,XDC_WEB_NIVEAU_2,XDC_WEB_PERTURBATION);

  sprintf(pl_nom_fichier,"%s",CO_REP_OUT);
  strcat(pl_nom_fichier,C_NOM_SERVICE);
  sprintf(pl_fichier,"%s%d%s",pl_nom_fichier,XDC_WEB_NIVEAU_2,pl_fin_fichier);
  aweb_ecrit_fichier_evt(pl_fichier,tm_services_nv2,XDC_WEB_NIVEAU_2,XDC_WEB_SERVICE);

  sprintf(pl_nom_fichier,"%s",CO_REP_OUT);
  strcat(pl_nom_fichier,C_NOM_CRISE);
  strcat(pl_nom_fichier,pl_fin_fichier);
  aweb_ecrit_fichier_crise(pl_nom_fichier);

  /*TDPASF*/
  if (vm_un_sur_deux==1) 
  {
    XZST_03EcritureTrace(XZSTC_WARNING,"Ecrit fichier rdt");
    sprintf(pl_nom_fichier,"%s",CO_REP2_OUT);
    strcat(pl_nom_fichier,C_NOM_RDT);
    aweb_ecrit_fichier_rdt(pl_nom_fichier);
  
    sprintf(pl_nom_fichier,"%s",CO_REP2_OUT);
    strcat(pl_nom_fichier,C_NOM_PARCOURS);
    aweb_ecrit_fichier_parcours(pl_nom_fichier);
  }


  sprintf(pl_nom_fichier,"%s",CO_REP_OUT);
  strcat(pl_nom_fichier,C_NOM_DATE);
  strcat(pl_nom_fichier,pl_fin_fichier);
  aweb_ecrit_fichier_date(pl_nom_fichier,vl_horodate);

  XZST_03EcritureTrace(XZSTC_FONCTION,"aweb_ftp_genere : OUT");
}

/* Fichier : @(#)aweb_ftp.c	1.26       Release : 1.26        Date : 08/28/13
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE aweb_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* 
*
******************************************************
* SEQUENCE D'APPEL :
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
void aweb_ecrit_fichier_trafic(char *va_nom, int va_niveau)
{
  char vl_ligne[300];
  FILE *pl_fp;
  struct T_Troncon *pl_liste;

  XZST_03EcritureTrace(XZSTC_INTERFACE,"aweb_ecrit_fichier_trafic : fichier a generer : %s(%d)",va_nom,va_niveau);

  if ( (pl_fp = fopen( va_nom, "w+")) == NULL )
  {
    XZST_03EcritureTrace(XZSTC_WARNING,
		"aweb_ecrit_fichier_trafic : impossible de creer le fichier %s",va_nom);
    return;
  }

  sprintf(vl_ligne,"%s\n",C_ENTETE);
  fputs(vl_ligne,pl_fp);

  pl_liste = tm_troncons;
  while (pl_liste != NULL)
  {
    XZST_03EcritureTrace(XZSTC_INFO,"aweb_ecrit_fichier : %s / %d",
				pl_liste->Identifiant,pl_liste->Niveau);
    if (pl_liste->Niveau==va_niveau) {
      sprintf(vl_ligne,"n%d_%sr,%d\n",va_niveau,pl_liste->Identifiant,pl_liste->Rouge);
      fputs(vl_ligne,pl_fp);
      sprintf(vl_ligne,"n%d_%sv,%d\n",va_niveau,pl_liste->Identifiant,pl_liste->Vert);
      fputs(vl_ligne,pl_fp);
      sprintf(vl_ligne,"n%d_%sb,%d\n",va_niveau,pl_liste->Identifiant,pl_liste->Bleu);
      fputs(vl_ligne,pl_fp);
    }
    pl_liste = pl_liste->Suivant;
  }

  fflush(pl_fp);
  fclose(pl_fp);

  XZST_03EcritureTrace(XZSTC_INFO,"aweb_ecrit_fichier : generation fichier terminee");
  return;
}


/* Fichier : @(#)aweb_ftp.c	1.26       Release : 1.26        Date : 08/28/13
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE aweb_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* 
*
******************************************************
* SEQUENCE D'APPEL :
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
void aweb_ecrit_fichier_evt(char *va_nom, struct T_Icone *pa_liste, int va_niveau,int va_type)
{
  char vl_ligne[300];
  FILE *pl_fp;
  struct T_Icone *pl_liste;
  char vl_prefixe[5];

  XZST_03EcritureTrace(XZSTC_FONCTION,"aweb_ecrit_fichier_evt : fichier a generer : %s",va_nom);

  if ( (pl_fp = fopen( va_nom, "w+")) == NULL )
  {
    XZST_03EcritureTrace(XZSTC_WARNING,
		"aweb_ecrit_fichier_evt : impossible de creer le fichier %s", va_nom);
    return;
  }

  if (va_type == XDC_WEB_PERTURBATION)
    strcpy(vl_prefixe,C_PREFIX_PERTURBATION); 
  else
    strcpy(vl_prefixe,C_PREFIX_SERVICE);

  sprintf(vl_ligne,"%s\n",C_ENTETE_EVT);
  fputs(vl_ligne,pl_fp);

  pl_liste = pa_liste;
  while (pl_liste != NULL)
  {
    sprintf(vl_ligne,"%s%d_%s_%s,%d,%d,0.%d,0.%d,%d,%s,\"%s\\r\\n%s\"\n",
			vl_prefixe,
			va_niveau,
			pl_liste->NomVue,
			pl_liste->Identifiant,
			pl_liste->X,
			pl_liste->Y,
			pl_liste->FacteurX,
			pl_liste->FacteurY,
			pl_liste->Rotation,
			pl_liste->Icone,
			pl_liste->Horodate,
			pl_liste->Texte);
    fputs(vl_ligne,pl_fp);
    pl_liste = pl_liste->Suivant;
  }

  fflush(pl_fp);
  fclose(pl_fp);

  XZST_03EcritureTrace(XZSTC_INFO,"aweb_ecrit_fichier_evt : generation fichier terminee");
  return;
}


/* Fichier : @(#)aweb_ftp.c	1.26       Release : 1.26        Date : 08/28/13
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE aweb_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* diffusion vers les ASF
*
******************************************************
* SEQUENCE D'APPEL :
* aweb_ftp_diffuse();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
void aweb_ftp_diffuse()
{
  char vl_ligne[600];

  /*FTP*/
  system(C_SCRIPT_FTP);

  /*suppression des fichiers du repertoire*/
/* v1.20 : non, c'est fait dans le C_SCRIPT_FTP */
  /*sprintf(vl_ligne,"rm %s/*.TXT %s/*.txt",CO_REP_OUT,CO_REP_OUT);*/
  /*system(vl_ligne);*/
      
}


/*A
*******************************************
* Fonction qui retourne la date courante
*******************************************
* Arguments : Aucun
*******************************************/

char * aweb_ftp_get_heure()
{
XZSMT_Horodate  vl_Horodate     = "";
double          vl_HorodateSec  = 0.0;
int             vl_JourSemaine  = 0;



        XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_HorodateSec);
        XZSM_11ConversionHorodate(vl_HorodateSec, &vm_HorodateZero);

        return(vm_HorodateZero);

}


/* Fichier : @(#)aweb_ftp.c	1.26       Release : 1.26        Date : 08/28/13
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE aweb_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* 
*
******************************************************
* SEQUENCE D'APPEL :
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
void aweb_ecrit_fichier_crise(char *va_nom)
{
  char vl_ligne[600];
  FILE *pl_fp;
  /*char vl_date[30];
  int vl_jour,vl_mois,vl_annee,vl_heure,vl_minute,vl_seconde;*/

  XZST_03EcritureTrace(XZSTC_FONCTION,"aweb_ecrit_fichier_crise : fichier a generer : %s",va_nom);

  vm_numero=0;
  vm_local=0;
  /*recuperation du texte du message crise*/
  XZAE166_Lire_Messages((XDY_FonctionInt)aweb_calcul_crise);

  /*si pas de message de crise*/
  if (vm_numero==0) return;

  if ( (pl_fp = fopen( va_nom, "w+")) == NULL )
  {
    XZST_03EcritureTrace(XZSTC_WARNING,
		"aweb_ecrit_fichier_crise : impossible de creer le fichier %s", va_nom);
    return;
  }

  /*generation du fichier XML*/
  sprintf(vl_ligne,"%s\n","<?xml version=\'1.0\' encoding=\'iso-8859-1\'?>"); /* DEM577 */
  fputs(vl_ligne,pl_fp);
  sprintf(vl_ligne,"%s\n","<info_crise>");
  fputs(vl_ligne,pl_fp);
	/* Si on remplit le msg que s'il n'est pas supprimer (contournement du disfonctionnement du champ suppression) DEM577 */
  if (!strcmp( vm_suppression, "non" ))
  {
  sprintf(vl_ligne,"<message source=\"ESCOTA\" lang=\"fr\" id=\"ESCOTA%d\" suppression=\"%s\">\n",
		vm_numero, vm_suppression);
  fputs(vl_ligne,pl_fp);
  
  if (vm_local==1)
  {
	sprintf(vl_ligne,"<carte>0</carte>\n");
	fputs(vl_ligne,pl_fp);
  }
  sprintf(vl_ligne,"<carte>1_8</carte>\n");
  fputs(vl_ligne,pl_fp);
  sprintf(vl_ligne,"<carte>2_aix</carte>\n");
  fputs(vl_ligne,pl_fp);
  sprintf(vl_ligne,"<carte>2_cotedazur</carte>\n");
  fputs(vl_ligne,pl_fp);
  
  sprintf(vl_ligne,"<horodate>%s</horodate>\n",vm_horodate);
  fputs(vl_ligne,pl_fp);
  
  sprintf(vl_ligne,"<delai_validite>%d</delai_validite>\n",vm_duree);
  fputs(vl_ligne,pl_fp);

  sprintf(vl_ligne,"<titre><![CDATA[\n%s\n]]>",vm_titre);
  fputs(vl_ligne,pl_fp);

  sprintf(vl_ligne,"</titre>\n");
  fputs(vl_ligne,pl_fp);

  sprintf(vl_ligne,"<texte><![CDATA[\n%s\n]]></texte>\n",vm_texte);
  fputs(vl_ligne,pl_fp);

  sprintf(vl_ligne,"</message>\n");
  fputs(vl_ligne,pl_fp);
  }

  sprintf(vl_ligne,"</info_crise>\n");
  fputs(vl_ligne,pl_fp);


  fflush(pl_fp);
  fclose(pl_fp);

  XZST_03EcritureTrace(XZSTC_INFO,"aweb_ecrit_fichier_crise : generation fichier terminee");
  return;
}




int aweb_calcul_crise(int va_numero, int va_suppression, int va_local, 
			int va_duree, char *va_titre, char *va_texte, char *va_horodate)
{  
  int i=0;
  vm_numero = va_numero;
  if (va_suppression==0)
    strcpy(vm_suppression,"non");
  else
    strcpy(vm_suppression,"oui");
  vm_local = va_local;
  vm_duree = va_duree;
  while (va_texte[i]!='\0')
  {
    if (va_texte[i]=='#')
      vm_texte[i]='\n';
    else
      vm_texte[i]=va_texte[i];
    i++;
  }
  vm_texte[i]='\0';
  strcpy(vm_titre,va_titre);
  strcpy(vm_horodate,va_horodate);
  return (XDC_OK);
}



/*TDPASF*/
/* Fichier : @(#)aweb_ftp.c	1.26       Release : 1.26        Date : 08/28/13
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE aweb_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* 
*
******************************************************
* SEQUENCE D'APPEL :
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
void aweb_ecrit_fichier_rdt(char *va_nom)
{
  char vl_ligne[300];
  FILE *pl_fp;
  struct T_Comptage *pl_liste;
  
  
  
  pl_liste = tm_stations;
  sprintf(vl_ligne,"%s",pl_liste->Horodate);
  sprintf(vm_ligne,"%s",pl_liste->Horodate);
  strcat(va_nom,vl_ligne);
  strcat(va_nom,".TXT");

  XZST_03EcritureTrace(XZSTC_INTERFACE,"aweb_ecrit_fichier_rdt : fichier a generer : %s",va_nom);

  if (tm_stations==NULL)
    return;

  if ( (pl_fp = fopen( va_nom, "w+")) == NULL )
  {
    XZST_03EcritureTrace(XZSTC_WARNING,
		"aweb_ecrit_fichier_rdt : impossible de creer le fichier %s", va_nom);
    return;
  }

  strcat(vl_ligne,"\n");
  fputs(vl_ligne,pl_fp);

  while (pl_liste != NULL)
  {
    sprintf(vl_ligne,"%s\t%d\t%d\t%d\t%d\t%d\t%d\n",
			pl_liste->Identifiant,
			pl_liste->Sens,
			pl_liste->Validite,
			pl_liste->Debit,
			pl_liste->Debit_PL,
			pl_liste->Vitesse,
			pl_liste->Taux);
    fputs(vl_ligne,pl_fp);
    pl_liste = pl_liste->Suivant;
  }

  fflush(pl_fp);
  fclose(pl_fp);

  XZST_03EcritureTrace(XZSTC_INFO,"aweb_ecrit_fichier_rdt : generation fichier terminee");
  return;
}


/*TDPASF*/
/*A
*******************************************
* Fonction d'insertion d'une mesure
*******************************************
*******************************************/

int aweb_ftp_calcul_rdt(char * va_horodate, char * va_identifiant, XDY_Octet va_sens, int va_validite,
			int va_debit, int va_debit_PL, int va_vitesse, int va_taux, int va_numero)
{
struct T_Comptage       *pl_new_station;

	XZST_03EcritureTrace(XZSTC_INTERFACE,"aweb_ftp_calcul_rdt : id %s(%d), sens %d, debit %d, debitPL %d, vit %d, taux %d",
					va_identifiant,
					va_numero,
					va_sens,
					va_debit,
					va_debit_PL,
					va_vitesse,
					va_taux);

	pl_new_station=tm_stations;

	while (pl_new_station!=NULL) {
		if (pl_new_station->Numero==va_numero) {
                  strcpy(pl_new_station->Identifiant , va_identifiant);
                  pl_new_station->Validite = va_validite;
                  pl_new_station->Debit = va_debit;
                  pl_new_station->Debit_PL = va_debit_PL;
                  pl_new_station->Vitesse = va_vitesse;
                  pl_new_station->Taux = va_taux;
                  strcpy(pl_new_station->Horodate, va_horodate);
	          return (XDC_OK);	
		}
	  pl_new_station = pl_new_station->Suivant;
        }
        return XDC_NOK;
}



/* Fichier : @(#)aweb_ftp.c	1.26       Release : 1.26        Date : 08/28/13
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE aweb_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* 
*
******************************************************
* SEQUENCE D'APPEL :
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
void aweb_ecrit_fichier_parcours(char *va_nom)
{
  char vl_ligne[300];
  FILE *pl_fp;
  struct T_Parcours *pl_liste;
  struct T_ZDP *pl_liste2;
  
  strcat(va_nom,vm_ligne);
  strcat(va_nom,".TXT");

  XZST_03EcritureTrace(XZSTC_INTERFACE,"aweb_ecrit_fichier_parcours : fichier a generer : %s",va_nom);
  /* lecture du flag de diffusion dans le fichier */
  if ( (pl_fp = fopen(C_NOM_TDP_DIFF ,"r")) == NULL) {
	XZST_03EcritureTrace(XZSTC_WARNING,"aweb_charge_zdp : pb ouverture %s",
				C_NOM_TDP_DIFF);
        vm_tdp_diff='N';
  }
  else {
    fgets(vl_ligne, 200, pl_fp);

    if (sscanf(vl_ligne,"%c",&vm_tdp_diff)!=1) {
    	XZST_03EcritureTrace(XZSTC_WARNING,"aweb_charge_zdp :pb decodage %s",
			vl_ligne);
    	vm_tdp_diff='N';
    }
  }

  fclose(pl_fp);

  pl_liste2=tm_zdp;

  pl_liste = tm_parcours;

  /*
  if (tm_parcours==NULL)
    return;
*/
  if (pl_liste2==NULL)
    return;
  if ( (pl_fp = fopen( va_nom, "w+")) == NULL )
  {
    XZST_03EcritureTrace(XZSTC_WARNING,
		"aweb_ecrit_fichier_parcours : impossible de creer le fichier %s",va_nom);
    return;
  }

  while (pl_liste2!=NULL) {
    sprintf(vl_ligne,"%s;%c;0\n",
			pl_liste2->Libelle,
			vm_tdp_diff);

    if (pl_liste2->Libelle[0] != '*' )
         fputs(vl_ligne,pl_fp);
    pl_liste2 = pl_liste2->Suivant;
  }
 
/*
  while (pl_liste != NULL)
  {
    sprintf(vl_ligne,"%s;%c;%d\n",
			pl_liste[vl_indice].Identifiant,
			pl_liste[vl_indice].A_Diffuser,
			pl_liste[vl_indice].Temps);
    fputs(vl_ligne,pl_fp);
    pl_liste = pl_liste[vl_indice].Suivant;
  }
*/
  fflush(pl_fp);
  fclose(pl_fp);

  XZST_03EcritureTrace(XZSTC_INFO,"aweb_ecrit_fichier_parcours : generation fichier terminee");
  return;
}


/*A
*******************************************
* Fonction d'insertion d'un parcours
*******************************************
*******************************************/

int aweb_ftp_calcul_parcours(char * va_identifiant, char va_a_diffuser, int va_temps)
{
struct T_Parcours       *pl_new_parcours;

	XZST_03EcritureTrace(XZSTC_INFO,"aweb_ftp_calcul_parcours : id %s, a diffuser %c, temps %d",
					va_identifiant,
					va_a_diffuser,
					va_temps);

        if (( pl_new_parcours = (struct T_Parcours *) malloc (sizeof(struct T_Parcours))) != NULL)
        {
                strcpy(pl_new_parcours->Identifiant , va_identifiant);
                pl_new_parcours->A_Diffuser = va_a_diffuser;
                pl_new_parcours->Temps = va_temps;
		pl_new_parcours->Suivant = tm_parcours;
		tm_parcours = pl_new_parcours;
        }
        else
        {
                XZST_03EcritureTrace(   XZSTC_WARNING, "Aweb_ftp_calcul_parcours : le malloc a echoue !" );
		return XDC_NOK;
        }

        return XDC_OK;
}


/*A
*******************************************
* Fonction d'insertion d'un parcours
*******************************************
*******************************************/

int aweb_ftp_tdp_rdt()
{
char vl_ligne[300];
char vl_lignel[2048];
char vl_ligner[2048];
char vl_lignet[2048];
XDY_Entier vl_temps=0,vl_temps_r=0,vl_temps_t=0,vl_h=0,vl_m=0,vl_s=0;
char pl_nom_fichier[500]="";
char pl_nom_fichier_r[500]="";
char pl_nom_fichier_t[500]="";
FILE *pl_fpr;
FILE *pl_fpt;
FILE *pl_fpl;
FILE *pl_fp;
int vl_jour,vl_mois,vl_annee,vl_heure,vl_minute,vl_seconde; 
char vl_horodate[30];
char vl_date[30];
struct T_ZDP *pl_liste;

  strcpy(vl_date,aweb_ftp_get_heure());

  if (sscanf( vl_date, "%2d/%2d/%4d %2d:%2d:%2d",
                                  &vl_jour,
                                  &vl_mois,
                                  &vl_annee,
                                  &vl_heure,
                                  &vl_minute,
                                  &vl_seconde) != 6) {
     XZST_03EcritureTrace(XZSTC_WARNING,
            "aweb_ftp_genere : le format de la date est incorrecte !" );
     return;
  }

	/* Creation fichier tdp reconst */
/* DEM1044
sprintf(pl_nom_fichier,"/tmp/%s%04d%02d%02d",C_NOM_TDP,vl_annee,vl_mois,vl_jour);
  	XZST_03EcritureTrace(XZSTC_INFO,"Open1 %s ",pl_nom_fichier);
	if ( (pl_fpl = fopen(pl_nom_fichier , "r")) == NULL )
  	{
    	XZST_03EcritureTrace(XZSTC_WARNING,
		"aweb_ftp_tdp_rdt : Creation du fichier journalier%s",pl_nom_fichier);
	fclose(pl_fpl);
  	XZST_03EcritureTrace(XZSTC_INFO,"Close1");
  	XZST_03EcritureTrace(XZSTC_INFO,"Open2 %s ",pl_nom_fichier);
	if ( (pl_fpl = fopen(pl_nom_fichier , "w")) == NULL )
  		{
    		XZST_03EcritureTrace(XZSTC_WARNING,
		"aweb_ftp_tdp_rdt : impossible de creer le fichier %s", pl_nom_fichier);
		fclose(pl_fpl);
  		XZST_03EcritureTrace(XZSTC_INFO,"Close 2.0");
    		return;
  		}

*/

  pl_liste=tm_zdp;
/* DEM1044
  sprintf (vl_lignel,"# Temps de Parcours RADT : %04d/%02d/%02d\t %s",vl_annee,vl_mois,vl_jour);
  while (pl_liste != NULL) {
	if (pl_liste->A_Calculer)
	  sprintf (vl_lignel,"%s%s\t",vl_lignel,pl_liste->Libelle);
    pl_liste = pl_liste->Suivant;
  }
	sprintf(vl_lignel,"%s\n",vl_lignel);
    	fputs(vl_lignel,pl_fpl);
    	fclose(pl_fpl);

  	XZST_03EcritureTrace(XZSTC_INFO,"Close 2.1");

	}
	else
		fclose(pl_fpl);
*/
	/* Creation fichier tdp reconst */
/* DEM1044
	sprintf(pl_nom_fichier_r,"/tmp/%sr_%04d%02d%02d",C_NOM_TDP,vl_annee,vl_mois,vl_jour);
  	XZST_03EcritureTrace(XZSTC_INFO,"Open1 %s ",pl_nom_fichier);
	if ( (pl_fpr = fopen(pl_nom_fichier_r , "r")) == NULL )
  	{
    	XZST_03EcritureTrace(XZSTC_WARNING,
		"aweb_ftp_tdp_rdt : Creation du fichier journalier%s",pl_nom_fichier_r);
	fclose(pl_fpr);
  	XZST_03EcritureTrace(XZSTC_INFO,"Close1");
  	XZST_03EcritureTrace(XZSTC_INFO,"Open2 %s ",pl_nom_fichier);
	if ( (pl_fpr = fopen(pl_nom_fichier_r , "w")) == NULL )
  		{
    		XZST_03EcritureTrace(XZSTC_WARNING,
		"aweb_ftp_tdp_rdt : impossible de creer le fichier %s", pl_nom_fichier_r);
		fclose(pl_fpr);
  		XZST_03EcritureTrace(XZSTC_INFO,"Close 2.0");
    		return;
  		}

	pl_liste=tm_zdp;

  	sprintf (vl_ligner,"# Temps de Parcours RADT reconstitué: %04d/%02d/%02d\t",vl_annee,vl_mois,vl_jour);
  	while (pl_liste != NULL) {
		if (pl_liste->A_Calculer)
	  		sprintf (vl_ligner,"%s%s\t",vl_ligner,pl_liste->Libelle);
    		pl_liste = pl_liste->Suivant;
  		}
	sprintf(vl_ligner,"%s\n",vl_ligner);
    	fputs(vl_ligner,pl_fpr);
    	fclose(pl_fpr);

  	XZST_03EcritureTrace(XZSTC_INFO,"Close 2.1");

	}
	else
		fclose(pl_fpr);
*/

	/* Creation fichier tdp badge */
    	/* DEM1044
	sprintf(pl_nom_fichier_t,"/tmp/%st_%04d%02d%02d",C_NOM_TDP,vl_annee,vl_mois,vl_jour);
  	XZST_03EcritureTrace(XZSTC_INFO,"Open1 %s ",pl_nom_fichier);
	if ( (pl_fpt = fopen(pl_nom_fichier_t , "r")) == NULL )
  	{
    	XZST_03EcritureTrace(XZSTC_WARNING,
		"aweb_ftp_tdp_rdt : Creation du fichier journalier%s",pl_nom_fichier_t);
	fclose(pl_fpt);
  	XZST_03EcritureTrace(XZSTC_INFO,"Close1");
  	XZST_03EcritureTrace(XZSTC_INFO,"Open2 %s ",pl_nom_fichier_t);
	if ( (pl_fpt = fopen(pl_nom_fichier_t , "w")) == NULL )
  		{
    		XZST_03EcritureTrace(XZSTC_WARNING,
		"aweb_ftp_tdp_rdt : impossible de creer le fichier %s", pl_nom_fichier_t);
		fclose(pl_fpt);
  		XZST_03EcritureTrace(XZSTC_INFO,"Close 2.0");
    		return;
  		}

	pl_liste=tm_zdp;

  	sprintf (vl_lignet,"# Temps de Parcours  badge: %04d/%02d/%02d\t",vl_annee,vl_mois,vl_jour);
  	while (pl_liste != NULL) {
		if (pl_liste->A_Calculer)
	  		sprintf (vl_lignet,"%s%s\t",vl_lignet,pl_liste->Libelle);
    		pl_liste = pl_liste->Suivant;
  		}
	sprintf(vl_lignet,"%s\n",vl_lignet);
    	fputs(vl_lignet,pl_fpt);
    	fclose(pl_fpt);

  	XZST_03EcritureTrace(XZSTC_INFO,"Close 2.1");

	}
	else
		fclose(pl_fpt);


  	XZST_03EcritureTrace(XZSTC_INFO,"Open3 %s",pl_nom_fichier);
  	if ( (pl_fpl = fopen(pl_nom_fichier , "a")) == NULL )
  	{
    	XZST_03EcritureTrace(XZSTC_WARNING,
		"aweb_ftp_tdp_rdt : impossible d'ouvrir le fichier journalier");
    	fclose(pl_fpl);
  	XZST_03EcritureTrace(XZSTC_WARNING,"Close 3.0");
	return;
  	}

  	XZST_03EcritureTrace(XZSTC_INFO,"Open3 %s",pl_nom_fichier_r);
  	if ( (pl_fpr = fopen(pl_nom_fichier_r , "a")) == NULL )
  	{
    	XZST_03EcritureTrace(XZSTC_WARNING,
		"aweb_ftp_tdp_rdt : impossible d'ouvrir le fichier journalier");
    	fclose(pl_fpr);
  	XZST_03EcritureTrace(XZSTC_WARNING,"Close 3.0");
	return;
  	}

  	XZST_03EcritureTrace(XZSTC_INFO,"Open3 %s",pl_nom_fichier_t);
  	if ( (pl_fpt = fopen(pl_nom_fichier_t , "a")) == NULL )
  	{
    	XZST_03EcritureTrace(XZSTC_WARNING,
		"aweb_ftp_tdp_rdt : impossible d'ouvrir le fichier journalier");
    	fclose(pl_fpt);
  	XZST_03EcritureTrace(XZSTC_WARNING,"Close 3.0");
	return;
  	}

    	sprintf(pl_nom_fichier,"/tmp/%s",C_NOM_TDP);
  	XZST_03EcritureTrace(XZSTC_INFO,"Open4 %s",pl_nom_fichier);
  	if ( (pl_fp = fopen(pl_nom_fichier , "w")) == NULL )
  	{
    	XZST_03EcritureTrace(XZSTC_WARNING,
		"aweb_ftp_tdp_rdt : impossible de creer le fichier %s", pl_nom_fichier);
    	fclose(pl_fp);
  	XZST_03EcritureTrace(XZSTC_WARNING,"Close4");
	return;
  	}

	pl_liste=tm_zdp;
	sprintf(vl_lignel,"%04d%02d%02d %02d:%02d",vl_annee,vl_mois,vl_jour,vl_heure,vl_minute);
	sprintf(vl_ligner,"%04d%02d%02d %02d:%02d",vl_annee,vl_mois,vl_jour,vl_heure,vl_minute);
	sprintf(vl_lignet,"%04d%02d%02d %02d:%02d",vl_annee,vl_mois,vl_jour,vl_heure,vl_minute);
	while (pl_liste!=NULL) {
	  if (pl_liste->A_Calculer) {
		XZST_03EcritureTrace(   XZSTC_WARNING, "aweb_ftp_tdp_rdt : appel XZAS100_TdP_RADT(%d,%d,%d,%d)",
					pl_liste->Autoroute,
					pl_liste->Debut,
					pl_liste->Fin,
					pl_liste->Sens);

	    if (XZAS100_TdP_RADT(pl_liste->Autoroute,
				pl_liste->Debut,
				pl_liste->Fin,
				pl_liste->Sens,&vl_temps) !=  XDC_OK ) {
		XZST_03EcritureTrace(   XZSTC_WARNING, "aweb_ftp_tdp_rdt : echec de XZAS100_TdP_RADT(%d,%d,%d,%d)",
					pl_liste->Autoroute,
					pl_liste->Debut,
					pl_liste->Fin,
					pl_liste->Sens);
		vl_temps=0;
	     }
             vl_h=vl_temps/3600;
	     vl_m=(vl_temps%3600)/60;
	     vl_s=(vl_temps%3600)%60;
	     sprintf(vl_lignel,"%s\t%d",vl_lignel,vl_temps);
	     sprintf(vl_ligne,"%s :\t%d; %02dh%02dm%02ds",pl_liste->Libelle,vl_temps,vl_h,vl_m,vl_s);
	    if (XZAS101_TdP_Recon_RADT(pl_liste->Autoroute,
				pl_liste->Debut,
				pl_liste->Fin,
				pl_liste->Sens,&vl_temps_r) !=  XDC_OK ) {
		XZST_03EcritureTrace(   XZSTC_WARNING, "aweb_ftp_tdp_rdt : echec de XZAS101_TdP_RADT_Recon(%d,%d,%d,%d)",
					pl_liste->Autoroute,
					pl_liste->Debut,
					pl_liste->Fin,
					pl_liste->Sens);
		vl_temps=0;
	     }
             vl_h=vl_temps_r/3600;
	     vl_m=(vl_temps_r%3600)/60;
	     vl_s=(vl_temps_r%3600)%60;
	     sprintf(vl_ligner,"%s\t%d",vl_ligner,vl_temps_r);
	     sprintf(vl_ligne,"%s\t%d; %02dh%02dm%02ds",vl_ligne,vl_temps_r,vl_h,vl_m,vl_s);

	    if (XZAS103_TdP(pl_liste->Autoroute,
				pl_liste->Debut,
				pl_liste->Fin,
				pl_liste->Sens,&vl_temps_t) !=  XDC_OK ) {
		XZST_03EcritureTrace(   XZSTC_WARNING, "aweb_ftp_tdp_rdt : echec de XZAS103_TdP(%d,%d,%d,%d)",
					pl_liste->Autoroute,
					pl_liste->Debut,
					pl_liste->Fin,
					pl_liste->Sens);
		vl_temps_t=0;
	     }
             vl_h=vl_temps_t/3600;
	     vl_m=(vl_temps_t%3600)/60;
	     vl_s=(vl_temps_t%3600)%60;
	     sprintf(vl_lignet,"%s\t%d",vl_lignet,vl_temps_t);
	     sprintf(vl_ligne,"%s\t%d; %02dh%02dm%02ds\n",vl_ligne,vl_temps_t,vl_h,vl_m,vl_s);
	     fputs(vl_ligne,pl_fp);
	   }
	   pl_liste = pl_liste->Suivant;
        }

  	fflush(pl_fp);
  	fclose(pl_fp);
  	XZST_03EcritureTrace(XZSTC_INFO,"Close 5");

	sprintf(vl_lignel,"%s\n",vl_lignel);
    	fputs(vl_lignel,pl_fpl);
  	fflush(pl_fpl);
  	fclose(pl_fpl);

	sprintf(vl_ligner,"%s\n",vl_ligner);
    	fputs(vl_ligner,pl_fpr);
  	fflush(pl_fpr);
  	fclose(pl_fpr);

	sprintf(vl_lignet,"%s\n",vl_lignet);
    	fputs(vl_lignet,pl_fpt);
  	fflush(pl_fpt);
  	fclose(pl_fpt);
  	XZST_03EcritureTrace(XZSTC_INFO,"Close 6");
*/

        return XDC_OK;
}






int aweb_charge_zdp() {
FILE *pl_fp;
char pl_ligne[200];
char vl_libelle[100];
int vl_autoroute;
XDY_PR	vl_debut;
XDY_PR	vl_fin;
int	vl_sens;
int	vl_filtre;


  if ( (pl_fp = fopen(C_NOM_ZDP , "r")) == NULL )
  {
	XZST_03EcritureTrace(XZSTC_WARNING,"aweb_charge_zdp : pb ouverture %s",
				C_NOM_ZDP);
    return (XDC_NOK);
  }

  while ( fgets(pl_ligne, 200, pl_fp) != NULL) {
	/* On ignore les lignes commencant par un # */
	if ( pl_ligne[0] == '#' )
		continue;

	if (sscanf(pl_ligne,"%[^;];%d;%d;%d;%d;%d",vl_libelle,
			&vl_autoroute,&vl_debut,&vl_fin,&vl_sens,&vl_filtre)!= 6) {
		XZST_03EcritureTrace(XZSTC_WARNING,"aweb_charge_zdp :pb decodage %s",
						pl_ligne);
		fclose(pl_fp);
		return (XDC_NOK);
	}

	/*on met tout ca en memoire*/
	if (tm_zdp==NULL) {
		XZST_03EcritureTrace(XZSTC_INFO,"aweb_charge_zdp :%s(%d)(%d/%d%/%d/%d)",vl_libelle,vl_filtre,
						vl_autoroute,vl_debut,vl_fin,vl_sens);
		tm_zdp=(struct T_ZDP *)malloc(sizeof(struct T_ZDP));
		strcpy(tm_zdp->Libelle,vl_libelle);
		tm_zdp->Debut=vl_debut;
		tm_zdp->Fin=vl_fin;
		tm_zdp->Autoroute=vl_autoroute;
		tm_zdp->Sens=vl_sens;
		tm_zdp->A_Calculer=((vl_filtre==0)?FALSE:TRUE);
		tm_zdp->Suivant = NULL;
		tm_zdp_courant=tm_zdp;
	}
	else {
		XZST_03EcritureTrace(XZSTC_INFO,"aweb_charge_zdp :%s(%d)",vl_libelle,vl_filtre);
	  tm_zdp_courant->Suivant = (struct T_ZDP *)malloc(sizeof(struct T_ZDP));
	  tm_zdp_courant = tm_zdp_courant->Suivant;
	  tm_zdp_courant->Debut=vl_debut;
	  strcpy(tm_zdp_courant->Libelle,vl_libelle);
	  tm_zdp_courant->Fin=vl_fin;
	  tm_zdp_courant->Autoroute=vl_autoroute;
          tm_zdp_courant->A_Calculer=((vl_filtre==0)?FALSE:TRUE);
	  tm_zdp_courant->Sens=vl_sens;
	  tm_zdp_courant->Suivant = NULL;
	}
  }
  fclose(pl_fp);
  return (XDC_OK);
}

int aweb_charge_rdt() {
FILE *pl_fp;
char pl_ligne[200];
char vl_libelle[100];
int		vl_numero;
int	vl_sens;

  if ( (pl_fp = fopen(C_NOM_CFGRDT , "r")) == NULL )
  {
	XZST_03EcritureTrace(XZSTC_WARNING,"aweb_charge_rdt : pb ouverture %s",
				C_NOM_CFGRDT);
    return (XDC_NOK);
  }

  while ( fgets(pl_ligne, 200, pl_fp) != NULL) {
	/* On ignore les lignes commencant par un # */
	if ( pl_ligne[0] == '#' )
		continue;

	if (sscanf(pl_ligne,"%d;%d",
			&vl_numero,&vl_sens)!= 2) {
		XZST_03EcritureTrace(XZSTC_WARNING,"aweb_charge_rdt :pb decodage %s",
						pl_ligne);
		fclose(pl_fp);
		return (XDC_NOK);
	}

	/*on met tout ca en memoire*/
	if (tm_stations==NULL) {
		tm_stations=(struct T_Comptage *)malloc(sizeof(struct T_Comptage));
		tm_stations->Sens=vl_sens;
		tm_stations->Numero=vl_numero;
		tm_stations->Suivant = NULL;
		tm_stations_courant=tm_stations;
	}
	else {
	  tm_stations_courant->Suivant = (struct T_Comptage *)malloc(sizeof(struct T_Comptage));
	  tm_stations_courant = tm_stations_courant->Suivant;
	  tm_stations_courant->Sens=vl_sens;
	  tm_stations_courant->Numero=vl_numero;
	  tm_stations_courant->Suivant = NULL;
	}
  }
  fclose(pl_fp);
  return (XDC_OK);
}


int aweb_ftp_modif_couleur(int va_station, int va_sens, int va_niveau)
{
  struct T_Troncon	*pl_liste;

  pl_liste=tm_troncons;
  XZST_03EcritureTrace(XZSTC_INTERFACE,"aweb_ftp_modif_couleur: station %d, sens %d, niv=%d",
			va_station,va_sens,va_niveau);

  /*parcours les tronçons pour mise a jour si station associee*/
  while (pl_liste!=NULL) {
    if ( ((pl_liste->Station1==va_station) || (pl_liste->Station2==va_station) ||
       (pl_liste->Station3==va_station) || (pl_liste->Station4==va_station) ||
       (pl_liste->Station5==va_station) ) && (pl_liste->Sens=va_sens)) {
	 XZST_03EcritureTrace(XZSTC_DEBUG1,"aweb_ftp_modif_couleur: station TROUVEE");
	 /*si le troncon est vert*/
	 if (pl_liste->Etat==C_ETAT_VERT) {
		pl_liste->Etat=va_niveau;
	 }
	 else if (pl_liste->Etat==C_ETAT_ORANGE) {
		if (va_niveau==XDC_RADT_SATURE)  {
			pl_liste->Etat=C_ETAT_ROUGE;
		}
	 }

                switch (pl_liste->Etat)
                {
                case (C_ETAT_ROUGE) : pl_liste->Rouge = 255;
                                       pl_liste->Vert= 0;
                                       pl_liste->Bleu = 0;
                                       break;

                case (C_ETAT_VERT) : pl_liste->Rouge = 45;
                                      pl_liste->Vert = 180;
                                      pl_liste->Bleu = 45;
                                      break;

                case (C_ETAT_ORANGE) : pl_liste->Rouge = 250;
                                        pl_liste->Vert = 150;
                                        pl_liste->Bleu = 90;
                                        break;
                }

     }
     pl_liste=pl_liste->Suivant;
  }
  return (XDC_OK);
}



int aweb_ftp_filtre() 
{
  XZLGT_Liste_FMC *pl_liste;
  XDY_Entier vl_nb;
  XDY_Booleen vl_perception;
  XDY_Booleen vl_nature;
  XDY_Nom vl_nom;
  XDY_Nom vl_hierarchie;
  XDY_Entier vl_numero;
  XDY_PR vl_pr;
  XDY_PR vl_prf;
  XDY_PR vl_prd;
 double vl_longueur;
  XDY_Octet vl_niveau;
  XDY_Octet vl_nb_conseq;
  XDY_Sens vl_sens_corrige;
  XDY_Entier vl_num;
  XDY_Octet vl_cle;
  XZLGT_Nombre_Mem vl_nombre_mem;
  int vl_indice;
  int vl_index;
  int vl_trouve;

/*  pl_liste=XZLG99_GetListe();*/
  /*accede a la memoire partagee des FMC*/
  pl_liste=XZLG985_Get_FMC();

  for (vl_indice=0;vl_indice<XZLGC_NB_MAX_FMC;vl_indice++) {
    XZST_03EcritureTrace(XZSTC_INTERFACE,"aweb_ftp_filtre: %d/%d/%ds%d aut:%d",
				pl_liste[vl_indice].Evt.numero,pl_liste[vl_indice].Evt.cle,
				pl_liste[vl_indice].Type,pl_liste[vl_indice].Sens,pl_liste[vl_indice].Autoroute);
    pl_liste[vl_indice].A_Traiter=FALSE;

    if (pl_liste[vl_indice].Evt.numero==0)
      break;

    if (XZLG55_IsBouchonConsequence(pl_liste[vl_indice].Evt.numero,pl_liste[vl_indice].Evt.cle,&vl_nb_conseq,&vl_num,&vl_cle)!= XDC_OK) {
	      XZST_03EcritureTrace(XZSTC_WARNING,"aweb_ftp_filtre : pb appel a XZLG55_IsBouchonConsequence");
    }
    if (vl_nb_conseq>0) {
	pl_liste[vl_indice].Evt_Consequence.numero=vl_num;
	pl_liste[vl_indice].Evt_Consequence.cle=vl_cle;
    }
    else   {
	pl_liste[vl_indice].Evt_Consequence.numero=0;
	pl_liste[vl_indice].Evt_Consequence.cle=0;
    }

    /*ceux a conserver de toute facon*/
    if ( (pl_liste[vl_indice].Type==XZAEC_FMC_Accident) ||
	 (pl_liste[vl_indice].Type==XZAEC_FMC_Contresens) ||
	 (pl_liste[vl_indice].Type==XZAEC_FMC_VehMarcheArr) ||
	 (pl_liste[vl_indice].Type==XZAEC_FMC_VehFeu) ||
	 (pl_liste[vl_indice].Type==XZAEC_FMC_Salage) ||
	 (pl_liste[vl_indice].Type==XZAEC_FMC_AireFermee) ||
	 (pl_liste[vl_indice].Type==XZAEC_FMC_Incendie) ||
	 (pl_liste[vl_indice].Type==XZAEC_FMC_Inondation) ||
	 (pl_liste[vl_indice].Type==XZAEC_FMC_AnimalErrant) ||
	 (pl_liste[vl_indice].Type==XZAEC_FMC_PietonChaussee) ) {
	pl_liste[vl_indice].A_Traiter=TRUE; 
	pl_liste[vl_indice].PR_Fin=pl_liste[vl_indice].PR_Debut;
    }

    else if ( (pl_liste[vl_indice].Type==XZAEC_FMC_Manifestation) || (pl_liste[vl_indice].Type==XZAEC_FMC_Escargot)) {
	XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre: manif = %d",pl_liste[vl_indice].Bloc_Optio.Bloc.Manif.Nombre);
        if ((pl_liste[vl_indice].Type==XZAEC_FMC_Manifestation) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Manif.Nombre>=50)) 
	  pl_liste[vl_indice].A_Traiter=TRUE;
	if ((pl_liste[vl_indice].Type==XZAEC_FMC_Escargot) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Manif.Nombre>=10)) pl_liste[vl_indice].A_Traiter=TRUE;
	pl_liste[vl_indice].PR_Fin=pl_liste[vl_indice].PR_Debut;
  }
	else if ( (pl_liste[vl_indice].Type==XZAEC_FMC_VehArrete) || (pl_liste[vl_indice].Type==XZAEC_FMC_VehPanne)) {
      if ( (pl_liste[vl_indice].VR == XDC_VOIE_BLOQUEE_NEUTRALISEE) || (pl_liste[vl_indice].VR ==XDC_VOIE_BLOQUEE)  || (pl_liste[vl_indice].VR ==XDC_VOIE_NEUTRALISEE) ||
           (pl_liste[vl_indice].VM2 == XDC_VOIE_BLOQUEE_NEUTRALISEE) || (pl_liste[vl_indice].VM2 ==XDC_VOIE_BLOQUEE)  || (pl_liste[vl_indice].VM2 ==XDC_VOIE_NEUTRALISEE) ||
           (pl_liste[vl_indice].VM1 == XDC_VOIE_BLOQUEE_NEUTRALISEE) || (pl_liste[vl_indice].VM1 ==XDC_VOIE_BLOQUEE)  || (pl_liste[vl_indice].VM1 ==XDC_VOIE_NEUTRALISEE) ||
           (pl_liste[vl_indice].VL == XDC_VOIE_BLOQUEE_NEUTRALISEE) || (pl_liste[vl_indice].VL ==XDC_VOIE_BLOQUEE)  || (pl_liste[vl_indice].VL ==XDC_VOIE_NEUTRALISEE) )
	pl_liste[vl_indice].A_Traiter=TRUE;
	pl_liste[vl_indice].PR_Fin=pl_liste[vl_indice].PR_Debut;
    }

    else if ( (pl_liste[vl_indice].Type==XZAEC_FMC_Eboulement) || (pl_liste[vl_indice].Type==XZAEC_FMC_Obstacle) ||
	      (pl_liste[vl_indice].Type==XZAEC_FMC_FlaqueHuile) || (pl_liste[vl_indice].Type==XZAEC_FMC_ChausseeGliss) ) {
	XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre: pl_liste[vl_indice].VL=%d",
				pl_liste[vl_indice].VL);
      if ( (pl_liste[vl_indice].VR == XDC_VOIE_BLOQUEE_NEUTRALISEE) || (pl_liste[vl_indice].VR ==XDC_VOIE_BLOQUEE)  || (pl_liste[vl_indice].VR ==XDC_VOIE_NEUTRALISEE) ||
           (pl_liste[vl_indice].VM2 == XDC_VOIE_BLOQUEE_NEUTRALISEE) || (pl_liste[vl_indice].VM2 ==XDC_VOIE_BLOQUEE)  || (pl_liste[vl_indice].VM2 ==XDC_VOIE_NEUTRALISEE) ||
           (pl_liste[vl_indice].VM1 == XDC_VOIE_BLOQUEE_NEUTRALISEE) || (pl_liste[vl_indice].VM1 ==XDC_VOIE_BLOQUEE)  || (pl_liste[vl_indice].VM1 ==XDC_VOIE_NEUTRALISEE) ||
           (pl_liste[vl_indice].VL == XDC_VOIE_BLOQUEE_NEUTRALISEE) || (pl_liste[vl_indice].VL ==XDC_VOIE_BLOQUEE)  || (pl_liste[vl_indice].VL ==XDC_VOIE_NEUTRALISEE) ) {
	pl_liste[vl_indice].A_Traiter=TRUE;
	XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre: EVT A TRAITER !");
	pl_liste[vl_indice].PR_Fin=pl_liste[vl_indice].PR_Debut;
      }
      else {
        /*regarder si bouchon consequence*/
	if (vl_nb_conseq>0) {
		pl_liste[vl_indice].A_Traiter=TRUE;
		pl_liste[vl_indice].PR_Fin=pl_liste[vl_indice].PR_Debut;
        }
      }
    }

    else if ( (pl_liste[vl_indice].Type==XZAEC_FMC_Travaux) ||
              (pl_liste[vl_indice].Type==XZAEC_FMC_ZoneStockage) ||
              (pl_liste[vl_indice].Type==XZAEC_FMC_BasculTravaux) ) {
	XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre: longueur TRV = %d",pl_liste[vl_indice].Bloc_Optio.Bloc.Travaux.Longueur);
      if (pl_liste[vl_indice].Bloc_Optio.Bloc.Travaux.Longueur >=1000) {
	pl_liste[vl_indice].A_Traiter=TRUE;
      }
    }

    else if ( (pl_liste[vl_indice].Type==XZAEC_FMC_Delestage) ||  (pl_liste[vl_indice].Type==XZAEC_FMC_Deviation)) {
      pl_liste[vl_indice].A_Traiter=TRUE;

        /*colorier en noir les troncons entre les 2 PR*/
	vl_sens_corrige=pl_liste[vl_indice].Sens;
XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre: sens noir : %d",vl_sens_corrige);
	if ( (pl_liste[vl_indice].Sens==XDC_SENS_SORTIE_NORD) ||
         (	pl_liste[vl_indice].Sens==XDC_SENS_ENTREE_NORD))
	     vl_sens_corrige=XDC_SENS_NORD;
	  else if ( (pl_liste[vl_indice].Sens==XDC_SENS_SORTIE_SUD ) ||
		(pl_liste[vl_indice].Sens==XDC_SENS_ENTREE_SUD))
      		vl_sens_corrige=XDC_SENS_SUD;
	  else if (pl_liste[vl_indice].Sens==XDC_SENS_INCONNU)
		vl_sens_corrige=XDC_SENS_NORD;
	
	XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre:  NOIR entre %d et %d,%d",
				pl_liste[vl_indice].PR_Debut,
				pl_liste[vl_indice].PR_Fin,vl_sens_corrige );
	
	vl_prf=pl_liste[vl_indice].PR_Fin;
	vl_prd=pl_liste[vl_indice].PR_Debut;
	if ( pl_liste[vl_indice].PR_Fin==0 ) /*réinjection sur autre autoroute */
	{ /* alors PR fin = PR debut+1 */
		if (((vl_sens_corrige==XDC_SENS_SUD) && (pl_liste[vl_indice].Autoroute!=8)) || ((vl_sens_corrige==XDC_SENS_NORD) && (pl_liste[vl_indice].Autoroute==8)))
			vl_prf= ++vl_prd;
		else
			vl_prf= --vl_prd;


		XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre:réinjection sur autre autoroute=> PR fin = PR debut  NOIR entre %d et %d",
				vl_prd,
				vl_prf);
	}
/*DEM924: 	if (((vl_sens_corrige==XDC_SENS_SUD) && (pl_liste[vl_indice].Autoroute!=8)) || ((vl_sens_corrige==XDC_SENS_NORD) && (pl_liste[vl_indice].Autoroute==8)))
			vl_sens_corrige=XDC_SENS_SUD;
		else
			vl_sens_corrige= XDC_SENS_NORD;*/
XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre: sens corrige noir : %d",vl_sens_corrige);

          if ( (!(pl_liste[vl_indice].Prevision)) && (pl_liste[vl_indice].Type!=XZAEC_FMC_Delestage) ) /*Pas noircir en cas de delestage PNI*/
	(int) aweb_ftp_colorie((XDY_Octet) XDC_WEB_NOIR,pl_liste[vl_indice].Autoroute,vl_sens_corrige,vl_prd,vl_prf);
    }

    else if (pl_liste[vl_indice].Type==XZAEC_FMC_QueueBouchon) {
	XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre: PR debut =%d; PR fin =%d, longueur=%d, cause=%d",
					pl_liste[vl_indice].PR_Debut,pl_liste[vl_indice].PR_Fin,
						pl_liste[vl_indice].Bloc_Optio.Bloc.Bouchon.Longueur,
						pl_liste[vl_indice].Evt_Cause.numero);

	XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre:  ROUGE entre %d et %d",
				pl_liste[vl_indice].PR_Debut,
				pl_liste[vl_indice].PR_Fin);
        (int) aweb_ftp_colorie(XDC_WEB_ROUGE,pl_liste[vl_indice].Autoroute,
			pl_liste[vl_indice].Sens,
			pl_liste[vl_indice].PR_Debut,pl_liste[vl_indice].PR_Fin);

	pl_liste[vl_indice].A_Traiter=FALSE;

        /*if (pl_liste[vl_indice].Evt_Cause.numero>0)
          pl_liste[vl_indice].A_Traiter=FALSE;
	else
	  pl_liste[vl_indice].A_Traiter=TRUE;*/
    }

    else if ( ( pl_liste[vl_indice].Type==XZAEC_FMC_FermetureEch) || (pl_liste[vl_indice].Type==XZAEC_FMC_EchDeconseille)) {
      pl_liste[vl_indice].A_Traiter=TRUE;
      pl_liste[vl_indice].PR_Fin=pl_liste[vl_indice].PR_Debut;
    }

    else if ( (pl_liste[vl_indice].Type==XZAEC_FMC_Meteo) || (pl_liste[vl_indice].Type==XZAEC_FMC_TraitChaussees) ||
	      (pl_liste[vl_indice].Type==XZAEC_FMC_ChausseeGliss)
            ) {
	  pl_liste[vl_indice].A_Traiter=TRUE;
    }

    else if (pl_liste[vl_indice].Type==XZAEC_FMC_IncSsConces) {
      pl_liste[vl_indice].A_Traiter=TRUE;
      pl_liste[vl_indice].PR_Fin=pl_liste[vl_indice].PR_Debut;
    }

  /*si evt pas commence*/
  if (pl_liste[vl_indice].Prevision) {
    XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre: evt en prevision => on le vire");
    pl_liste[vl_indice].A_Traiter=FALSE;
  }

  /*si evt fini*/
  XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre: FIN = %c",pl_liste[vl_indice].Bloc_Optio.Fin);
  if (pl_liste[vl_indice].Bloc_Optio.Fin=='Y') {
    XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre: evt fini => on le vire");
    pl_liste[vl_indice].A_Traiter=FALSE;
  }

  /*si evt non confirme*/
  XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre: vERACITE = %d",pl_liste[vl_indice].Bloc_Optio.Veracite);
  if (pl_liste[vl_indice].Bloc_Optio.Veracite==5) {
    XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre: evt pas confirme (%d) => on le vire");
    pl_liste[vl_indice].A_Traiter=FALSE;
  }

  if (pl_liste[vl_indice].A_Traiter) 
    XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre : FMC %d/%d gardee",
				pl_liste[vl_indice].Evt.numero,
				pl_liste[vl_indice].Evt.cle);
  else
    XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre : FMC %d/%d  VIREE",
				pl_liste[vl_indice].Evt.numero,
				pl_liste[vl_indice].Evt.cle);
  /*je memorise sa diffusion par taweb*/
  XZLG10_Memorise_Diffusion_FMC(pl_liste[vl_indice].Evt.numero,pl_liste[vl_indice].Evt.cle,XZLGC_DIFF_CLIENT_1);

  }

  /*boucle specifique sur les bouchons*/
  for (vl_indice=0;vl_indice<XZLGC_NB_MAX_FMC;vl_indice++) {
    if (pl_liste[vl_indice].Evt.numero==0)
      break;

    if (pl_liste[vl_indice].Type!=XZAEC_FMC_QueueBouchon)
      continue;

    vl_trouve=0;
    /*le bouchon a une cause*/
    if (pl_liste[vl_indice].Evt_Cause.numero>0) {
      XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre: cause bouchon = %d", pl_liste[vl_indice].Evt_Cause.numero);
      /*cette cause est elle sur le synoptique Web Trafic*/
      for (vl_index=0;vl_index<XZLGC_NB_MAX_FMC;vl_index++) {
        if (pl_liste[vl_index].Evt.numero==0)
	  break;
        if ( (pl_liste[vl_index].Evt.numero==pl_liste[vl_indice].Evt_Cause.numero) &&
	   (pl_liste[vl_index].Evt.cle==pl_liste[vl_indice].Evt_Cause.cle)) {
	  vl_trouve=1;
	  XZST_03EcritureTrace(XZSTC_INFO,"aaweb_ftp_filtre: cause bouchon trouvee %d",pl_liste[vl_index].A_Traiter);
	  if ((!pl_liste[vl_index].A_Traiter) || (pl_liste[vl_index].Bloc_Optio.Fin=='Y'))
	    pl_liste[vl_indice].A_Traiter=TRUE;
	  break;
	}
      }
      if (vl_trouve==0)
	pl_liste[vl_indice].A_Traiter=TRUE;
    }
    else /*bouchon sans cause*/
      pl_liste[vl_indice].A_Traiter=TRUE;
  }
  return (XDC_OK);
}



int aweb_ftp_colorie(XDY_Octet va_couleur,XDY_Autoroute va_autoroute,XDY_Sens va_sens,
			XDY_PR va_debut, XDY_PR va_fin)
{
  XDY_PR vl_min;
  XDY_PR vl_max;
  struct T_Troncon *pl_liste;
XZST_03EcritureTrace(XZSTC_INFO,"aweb_ftp_colorie: couleur:%d,autoroute:%d,sens:%d, %d,%d.",va_couleur,va_autoroute,va_sens,
			va_debut, va_fin);
  if (va_debut>va_fin) {
    vl_min=va_fin;
    vl_max=va_debut;
  }
  else {
    vl_min=va_debut;
    vl_max=va_fin;
  }
  pl_liste=tm_troncons;
 
  while (pl_liste!=NULL) {
    if ( (pl_liste->Autoroute==va_autoroute) && (pl_liste->Sens==va_sens) &&
/* DEM974 */
(( ((pl_liste->Debut <= vl_min) && (pl_liste->Fin>vl_min)) ||
  ((pl_liste->Debut < vl_max) && (pl_liste->Fin>=vl_max)) ||
	    ( (pl_liste->Debut >= vl_min) && (pl_liste->Fin<=vl_max)) ) ))


/*	 (  ( (pl_liste->Debut <= vl_min) && (pl_liste->Fin>=vl_max)) ||
	    ( (pl_liste->Debut >= vl_min) && (pl_liste->Fin<=vl_max)) ||
	    ( (pl_liste->Debut < vl_min) && (pl_liste->Fin>=vl_max))
         )
       )*/
 { XZST_03EcritureTrace(XZSTC_INFO,"aweb_ftp_colorie: troncon trouve : %s",pl_liste->Identifiant);
       if (va_couleur==XDC_WEB_NOIR) {
	 pl_liste->Etat=C_ETAT_NOIR;
	 pl_liste->Rouge = 0;
	 pl_liste->Vert = 0;
  	 pl_liste->Bleu = 0;
       }
       else if (va_couleur==XDC_WEB_ROUGE) {
	 if (pl_liste->Etat != C_ETAT_NOIR) {
	   pl_liste->Etat = C_ETAT_ROUGE;
	   pl_liste->Rouge = 255;
	   pl_liste->Vert = 0;
  	   pl_liste->Bleu = 0;
	} 
       }
       else if (va_couleur==XDC_WEB_ORANGE) {
	 if ( (pl_liste->Etat != C_ETAT_NOIR) && (pl_liste->Etat !=C_ETAT_ROUGE)) {
	   pl_liste->Etat = C_ETAT_ORANGE;
	   pl_liste->Rouge = 250;
	   pl_liste->Vert = 150;
  	   pl_liste->Bleu = 90;
	 }
       }
      }
    pl_liste=pl_liste->Suivant;
  }
  return (XDC_OK);
}

	


int aweb_ftp_genere_texte()
{
  char vl_texte1[50]="";
  char vl_texte2[50]="";
  char vl_texte[90]="";
  XZLGT_Liste_FMC *pl_liste;
  XDY_PR vl_min;
  int vl_indice;
  XDY_PR vl_max;
  int vl_longueur;

  pl_liste=XZLG985_Get_FMC();

  for (vl_indice=0;vl_indice<XZLGC_NB_MAX_FMC;vl_indice++) {
    if (pl_liste[vl_indice].Evt.numero==0)
      break;

    if (pl_liste[vl_indice].A_Traiter) {
    strcpy(vl_texte1, tg_types_fmc[pl_liste[vl_indice].Type-1].Texte_Web);

    if (pl_liste[vl_indice].Type==XZAEC_FMC_FermetureEch) 
    {
      /*E1 E2 S1 S2*/
      strcpy(vl_texte1,"Fermeture echangeur");
      if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_NON) )
        strcpy(vl_texte1,"Fermeture echangeur");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_OUI) )
        strcpy(vl_texte1,"Sortie fermee");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_NON) )
        strcpy(vl_texte1,"Sortie fermee");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_OUI) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_OUI) )
        strcpy(vl_texte1,"Ent & sortie fermee");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_NON) )
        strcpy(vl_texte1,"Sortie fermee");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_OUI) )
        strcpy(vl_texte1,"Sorties fermees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_OUI) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_NON) )
        strcpy(vl_texte1,"Ent. et sortie fermees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_OUI) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_OUI) )
        strcpy(vl_texte1,"Fermeture echangeur");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_NON) )
        strcpy(vl_texte1,"Entree fermee");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_OUI) )
        strcpy(vl_texte1,"Ent. et sortie fermees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_OUI) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_NON) )
        strcpy(vl_texte1,"Entrees fermees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_OUI) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_OUI) )
        strcpy(vl_texte1,"Ent. et sortis fermees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_NON) )
        strcpy(vl_texte1,"Ent. & sortie fermees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_OUI) )
        strcpy(vl_texte1,"Ent. et sorties fermees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_OUI) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_NON) )
        strcpy(vl_texte1,"Ent. et sorties fermees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_OUI) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_OUI) )
        strcpy(vl_texte1,"Fermeture echangeur");
    }
    else if (pl_liste[vl_indice].Type==XZAEC_FMC_EchDeconseille)
    {
      /*E1 E2 S1 S2*/
      strcpy(vl_texte1,"Echangeur deconseille");
      if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_NON) )
        strcpy(vl_texte1,"Echangeur deconseille");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_OUI) )
        strcpy(vl_texte1,"Sortie deconseillee");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_NON) )
        strcpy(vl_texte1,"Sortie deconseillee");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_OUI) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_OUI) )
        strcpy(vl_texte1,"Ent & sortie deconseillees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_OUI) )
        strcpy(vl_texte1,"Sorties deconseillees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_OUI) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_NON) )
        strcpy(vl_texte1,"Ent. et sortie deconseillees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_OUI) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_OUI) )
        strcpy(vl_texte1,"Ent. et sorties deconseillees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_NON) )
        strcpy(vl_texte1,"Entree deconseillee");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_OUI) )
        strcpy(vl_texte1,"Ent. et sorties deconseillees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_OUI) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_NON) )
        strcpy(vl_texte1,"Entrees deconseillees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_NON) )
        strcpy(vl_texte1,"Ent & sortie deconseillees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_OUI) )
        strcpy(vl_texte1,"Ent. et sorties deconseillees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_OUI) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_NON) )
        strcpy(vl_texte1,"Ent. et sortie deconseillees");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_OUI) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_NON) )
        strcpy(vl_texte1,"Entree deconseillee");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_OUI) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_OUI) )
        strcpy(vl_texte1,"Echangeur deconseille");
      else if ( (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree1==XDC_OUI) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Entree2==XDC_NON) && 
	   (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie1==XDC_NON) && (pl_liste[vl_indice].Bloc_Optio.Bloc.Echangeur.Sortie2==XDC_OUI) )
        strcpy(vl_texte1,"Ent. et sortie deconseillees");
    }
    else if ( (pl_liste[vl_indice].Type==XZAEC_FMC_Meteo) ||(pl_liste[vl_indice].Type==XZAEC_FMC_TraitChaussees) ||
	       (pl_liste[vl_indice].Type==XZAEC_FMC_ChausseeGliss)){
      strcpy(vl_texte1,pl_liste[vl_indice].Bloc_Optio.Bloc.Meteo.Nature); /*libelle nature meteo*/
    }
    else if ( pl_liste[vl_indice].Type==XZAEC_FMC_VehPanne)
		switch(pl_liste[vl_indice].Bloc_Optio.Bloc.Vehicule.TypeVehicule) {
			case (XZAEC_VEH_PL) : strcpy(vl_texte1,"PL en panne");
						break;
			case (XZAEC_VEH_VL) : strcpy(vl_texte1,"VL en panne");
					break;
			case (XZAEC_VEH_Car) : strcpy(vl_texte1,"Car en panne");
					break;
			case (XZAEC_VEH_Moto) : strcpy(vl_texte1,"Moto en panne");
					break;
			case (XZAEC_VEH_Remorque) : strcpy(vl_texte1,"Remorque en panne");
					break;
			default : strcpy(vl_texte1,"Vehicule en panne");
					break;
		}
    else if ( pl_liste[vl_indice].Type==XZAEC_FMC_IncSsConces) {
      if (( !pl_liste[vl_indice].Bloc_Optio.Bloc.Concession.Fermeture) && (!pl_liste[vl_indice].Bloc_Optio.Bloc.Concession.Penurie))
	strcpy(vl_texte1,"PAS DE CARBURANT");
      else if (pl_liste[vl_indice].Bloc_Optio.Bloc.Concession.Fermeture)
	strcpy(vl_texte1,"STATION FERMEE");
      else if (pl_liste[vl_indice].Bloc_Optio.Bloc.Concession.Carburant==1) 
	strcpy(vl_texte1,"PAS DE SUPER");
      else if (pl_liste[vl_indice].Bloc_Optio.Bloc.Concession.Carburant==2) 
	strcpy(vl_texte1,"PAS DE GAZOLE");
      else if (pl_liste[vl_indice].Bloc_Optio.Bloc.Concession.Carburant==3) 
	strcpy(vl_texte1,"PAS DE SANS PLOMB 95");
      else if (pl_liste[vl_indice].Bloc_Optio.Bloc.Concession.Carburant==4) 
	strcpy(vl_texte1,"PAS DE SANS PLOMB 98");
      else if (pl_liste[vl_indice].Bloc_Optio.Bloc.Concession.Carburant==5) 
	strcpy(vl_texte1,"PAS DE GPL");
      else
	strcpy(vl_texte1,"PAS DE CARBURANT");
    }

     /* if (pl_liste[vl_indice].PR_Debut>pl_liste[vl_indice].PR_Fin) {
	vl_min=(int)floor((pl_liste[vl_indice].PR_Fin+500)/1000);
	vl_max=(int)floor((pl_liste[vl_indice].PR_Debut+500)/1000);
      }
      else {*/
	vl_max=(int)floor((pl_liste[vl_indice].PR_Fin+500)/1000);
	vl_min=(int)floor((pl_liste[vl_indice].PR_Debut+500)/1000);
    /*  }*/
    sprintf(vl_texte,"%s au km %d", vl_texte1, vl_min);
    switch(pl_liste[vl_indice].Type) 
    {
	case XZAEC_FMC_Accident:
	  /*if (pl_liste[vl_indice].Type_Point_Car==XDC_POINT_CARACT_ECHANGEUR) {
            sprintf(vl_texte1," sur bretelle");
	    strcat(vl_texte,vl_texte1);
		}*/
		strcat(vl_texte,aweb_neutralisations(pl_liste[vl_indice]));
		strcat(vl_texte,aweb_circulation(pl_liste[vl_indice]));
		break;

	case XZAEC_FMC_Meteo:
	case XZAEC_FMC_TraitChaussees:
	case XZAEC_FMC_ChausseeGliss:
	  if (pl_liste[vl_indice].Type_Point_Car==XDC_POINT_CARACT_ECHANGEUR) {
            sprintf(vl_texte1," sur bretelle");
	    strcat(vl_texte,vl_texte1);
	    break;
	  }
		sprintf(vl_texte1,"-%d",vl_max);
		strcat(vl_texte,vl_texte1);
		break;

	case XZAEC_FMC_VehPanne:
	  /*if (pl_liste[vl_indice].Type_Point_Car==XDC_POINT_CARACT_ECHANGEUR) {
            sprintf(vl_texte1," sur bretelle");
	    strcat(vl_texte,vl_texte1);
		}*/
		strcat(vl_texte,aweb_neutralisations(pl_liste[vl_indice]));
		strcat(vl_texte,aweb_circulation(pl_liste[vl_indice]));
		break;

	case XZAEC_FMC_QueueBouchon:
	vl_longueur = (double)pl_liste[vl_indice].Bloc_Optio.Bloc.Bouchon.Longueur;
	vl_longueur = vl_longueur / 1000;
	if ((int)floor(vl_longueur)==0)
	  vl_longueur=1;
	if (pl_liste[vl_indice].Type_Point_Car==XDC_POINT_CARACT_ECHANGEUR) {
          sprintf(vl_texte1," sur bretelle");
	  strcat(vl_texte,vl_texte1);
	  break;
	}
	else {
	  sprintf(vl_texte1," sur %d km",(int)floor(vl_longueur));
		strcat(vl_texte,vl_texte1);
		break;
	}

	case XZAEC_FMC_Travaux:
	case XZAEC_FMC_ZoneStockage:
	case XZAEC_FMC_BasculTravaux:
	vl_longueur = (double)pl_liste[vl_indice].Bloc_Optio.Bloc.Travaux.Longueur;
	vl_longueur = vl_longueur / 1000;
	  if (pl_liste[vl_indice].Type_Point_Car==XDC_POINT_CARACT_ECHANGEUR) {
            sprintf(vl_texte1," sur bretelle");
	    strcat(vl_texte,vl_texte1);
	    break;
	  }
	sprintf(vl_texte1," sur %d km",(int)floor(vl_longueur));
		strcat(vl_texte,vl_texte1);
		strcat(vl_texte,aweb_restrictions(pl_liste[vl_indice]));
		break;

	default:
	  if ( (pl_liste[vl_indice].Type_Point_Car==XDC_POINT_CARACT_ECHANGEUR)  &&
	       (pl_liste[vl_indice].Type != XZAEC_FMC_FermetureEch) &&
	       (pl_liste[vl_indice].Type != XZAEC_FMC_IncSsConces) ) {
            sprintf(vl_texte1," sur bretelle");
	    strcat(vl_texte,vl_texte1);
		}
		break;

    }
    strcat(vl_texte,aweb_consequence(pl_liste[vl_indice]));
    strcpy(pl_liste[vl_indice].Nom1,vl_texte);
    }
  }
  return (XDC_OK);
}


int aweb_ftp_genere_icones()
{
  XZLGT_Liste_FMC *pl_liste;
  int vl_indice;

  pl_liste = XZLG985_Get_FMC();

  for (vl_indice=0;vl_indice<XZLGC_NB_MAX_FMC;vl_indice++) {
    if (pl_liste[vl_indice].Evt.numero==0)
      break;

    if (pl_liste[vl_indice].A_Traiter) {
      XZST_03EcritureTrace(XZSTC_INFO,"aweb_ftp_genere_icones : id %d/%d, %s (%d/%d s%d)",
					pl_liste[vl_indice].Evt.numero,
					pl_liste[vl_indice].Evt.cle,
					pl_liste[vl_indice].Nom1,
					pl_liste[vl_indice].PR_Debut,
					pl_liste[vl_indice].PR_Fin,
					pl_liste[vl_indice].Sens);
      XZAE162_Calcul_Troncons(pl_liste[vl_indice].Autoroute,
				pl_liste[vl_indice].Sens,
				pl_liste[vl_indice].PR_Debut,
				pl_liste[vl_indice].Type_Point_Car,
				pl_liste[vl_indice].Numero_Point_Car,
				pl_liste[vl_indice].HValidation,
				pl_liste[vl_indice].Nom1,
				tg_types_fmc[pl_liste[vl_indice].Type-1].Icone,
				tg_types_fmc[pl_liste[vl_indice].Type-1].Priorite,
				tg_types_fmc[pl_liste[vl_indice].Type-1].Type_Web);
    }
  }

  return (XDC_OK);
}



char *aweb_neutralisations(XZLGT_Liste_FMC pa_liste)
{ 
  int vl_tot=0;
  int vl_neut=0;
  int vl_bloq=0;
  int vl_circ=0;
  int vl_double=0;

  if (pa_liste.Type_Point_Car==XDC_POINT_CARACT_ECHANGEUR) {
    return ("\\r\\nsur bretelle");
  }

  web_cumul_voies(pa_liste.VL,&vl_tot,&vl_circ,&vl_neut,&vl_bloq,&vl_double);
  web_cumul_voies(pa_liste.VM1,&vl_tot,&vl_circ,&vl_neut,&vl_bloq,&vl_double);
  web_cumul_voies(pa_liste.VM2,&vl_tot,&vl_circ,&vl_neut,&vl_bloq,&vl_double);
  web_cumul_voies(pa_liste.VR,&vl_tot,&vl_circ,&vl_neut,&vl_bloq,&vl_double);
  
  if (vl_neut>0) return ("");
  if (vl_circ==0) return ("\\r\\ntoutes voies bloquees");
  if (vl_bloq==1) {
    vl_tot;
    vl_bloq=0;
    web_cumul_voies(pa_liste.VL,&vl_tot,&vl_circ,&vl_neut,&vl_bloq,&vl_double);
    if (vl_bloq>0) return ("\\r\\nvoie lente occupee");
    web_cumul_voies(pa_liste.VM1,&vl_tot,&vl_circ,&vl_neut,&vl_bloq,&vl_double);
    if (vl_bloq>0) return ("\\r\\nvoie mediane occupee");
    web_cumul_voies(pa_liste.VM1,&vl_tot,&vl_circ,&vl_neut,&vl_bloq,&vl_double);
    if (vl_bloq>0) return ("\\r\\nvoie mediane occupee");
    return ("\\r\\nvoie rapide occupee");
  }
  else if (vl_bloq>0) {
    sprintf(vm_str,"\\r\\n%d voies occupees sur %d",vl_bloq,vl_tot);
    return (vm_str);
  }
  return ("");
}
 


char *aweb_circulation(XZLGT_Liste_FMC pa_liste)
{
  int vl_tot=0;
  int vl_neut=0;
  int vl_bloq=0;
  int vl_circ=0;
  int vl_double=0;

  web_cumul_voies(pa_liste.VL,&vl_tot,&vl_circ,&vl_neut,&vl_bloq,&vl_double);
  web_cumul_voies(pa_liste.VM1,&vl_tot,&vl_circ,&vl_neut,&vl_bloq,&vl_double);
  web_cumul_voies(pa_liste.VM2,&vl_tot,&vl_circ,&vl_neut,&vl_bloq,&vl_double);
  web_cumul_voies(pa_liste.VR,&vl_tot,&vl_circ,&vl_neut,&vl_bloq,&vl_double);

  if (vl_neut==0) return ("");

  if (vl_circ==0) return ("");
  
  sprintf(vm_str,"\\r\\ncirculation sur %d voies",vl_circ);

  return (vm_str);
}



char *aweb_restrictions(XZLGT_Liste_FMC pa_liste)
{
  int vl_tot=0;
  int vl_neut=0;
  int vl_bloq=0;
  int vl_circ=0;
  int vl_double=0;

  XZST_03EcritureTrace(XZSTC_INFO,"aweb_restrictions: evt = %d, VL=%d, VM1=%d,VM2=%d, VR=%d",
			pa_liste.Evt.numero,
			pa_liste.VL,
			pa_liste.VM1,
			pa_liste.VM2,
			pa_liste.VR);
  web_cumul_voies(pa_liste.VL,&vl_tot,&vl_circ,&vl_neut,&vl_bloq,&vl_double);
  web_cumul_voies(pa_liste.VM1,&vl_tot,&vl_circ,&vl_neut,&vl_bloq,&vl_double);
  web_cumul_voies(pa_liste.VM2,&vl_tot,&vl_circ,&vl_neut,&vl_bloq,&vl_double);
  web_cumul_voies(pa_liste.VR,&vl_tot,&vl_circ,&vl_neut,&vl_bloq,&vl_double);

  if ( (pa_liste.VL_I==XDC_VOIE_SENS_INVERSE) ||
       (pa_liste.VM1_I==XDC_VOIE_SENS_INVERSE) ||
       (pa_liste.VM2_I==XDC_VOIE_SENS_INVERSE) ||
       (pa_liste.VR_I==XDC_VOIE_SENS_INVERSE) 
      )
      return ("\\r\\ncirculation a double sens");


  if (vl_double>0) return ("\\r\\ncirculation a double sens");

  if (vl_neut==0) return ("");

  if (vl_circ==vl_tot) return ("");

  sprintf(vm_str,"\\r\\n%d voies circulees sur %d",vl_circ,vl_tot);
  return (vm_str);
}



char *aweb_consequence(XZLGT_Liste_FMC pa_liste)
{
  XDY_Entier vl_longueur;
  XDY_PR vl_pr;
  XDY_Mot vl_niveau;
  XDY_Entier vl_num;
  XDY_Octet vl_cle;
  int vl_indice;
  int vl_trouve;
  XZLGT_Liste_FMC *pl_liste;

  XZST_03EcritureTrace(XZSTC_INFO,"aweb_consequence: evt conseq = %d %d",pa_liste.Evt.numero,pa_liste.Evt_Consequence.numero);

  if ((pa_liste.Evt_Consequence.numero>0) && ((pa_liste.Evt.numero != pa_liste.Evt_Consequence.numero))) {
    vl_trouve=0;
    pl_liste=XZLG985_Get_FMC();
    /*recherche mon bouchon dans la liste*/
    for (vl_indice=0;vl_indice<XZLGC_NB_MAX_FMC;vl_indice++) {
      if (pl_liste[vl_indice].Evt.numero==0)
	break;

      if ( (pl_liste[vl_indice].Evt.numero==pa_liste.Evt_Consequence.numero) &&
	 (pl_liste[vl_indice].Evt.cle==pa_liste.Evt_Consequence.cle) ) {
	XZST_03EcritureTrace(XZSTC_INFO,"aweb_consequence: fini =%c",
					pl_liste[vl_indice].Bloc_Optio.Fin);
	vl_trouve=1;
        /*si bouchon consequence fini*/
	if (pl_liste[vl_indice].Bloc_Optio.Fin=='Y')
	  return ("");
	else
	  break;
      }
    }
    if (vl_trouve==0)
      return ("");
    if (XZLG51_GetFmcBouchon(pa_liste.Evt_Consequence.numero,pa_liste.Evt_Consequence.cle,
	    &vl_pr,&vl_niveau,&vl_longueur,
	    &vl_num,&vl_cle)==XDC_OK)  {
      if ((int)floor(vl_longueur/1000)==0)
        strcpy(vm_str,"\\r\\nConsequence : bouchon sur 1 km");
      else
        sprintf(vm_str,"\\r\\nConsequence : bouchon sur %d km",(int)floor(vl_longueur/1000));
    }
    else
      sprintf(vm_str,"\\r\\nConsequence : bouchon");
    return (vm_str);
  }
  else
    return ("");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* cumul le nb de voies totales et en circulation
* a pertie de l'etat de la voie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void web_cumul_voies(XDY_Voie x,int *pa_tot,int *pa_cir, int *pa_neut, int *pa_bloq, int *pa_double)

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
	     (*pa_tot)++;
		break;
        case    XDC_VOIE_SENS_INVERSE   :
		(*pa_double)++;
		(*pa_tot)++;
                break;
        case    XDC_VOIE_BLOQUEE        :
		(*pa_bloq)++;
		(*pa_tot)++;
                break;
        case    XDC_VOIE_NEUTRALISEE    :
		(*pa_neut)++;
		(*pa_tot)++;
                break;
        case    XDC_VOIE_BLOQUEE_NEUTRALISEE :
		(*pa_neut)++;
             (*pa_tot)++;
                break;
        default:
                break;
        }
}

