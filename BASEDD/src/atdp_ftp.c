/*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : atdp_ftp.c				*
*************************************************
* DESCRIPTION :
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
*************************************************
* HISTORIQUE :
* JMG	17/11/08	creation 1.1 DEM 837
* SFO   06/10/14 : Adaptation pour compilation sous linux
* JMG	15/12/15 : export TDP ASF 1.3 DEM11551
* PNI	06/06/18 : correctif pour filtre donné ASF comprises entre 200 et 300 v1.4 DEM1297
************************************************/


#include "atdp_ftp.h"

#define C_PC_CCI             "CCI"

/* definition des constantes */

#define CO_REP_OUT		XDC_PATH_STRADA "/atdp/"

#define C_NOM_TRAFIC		"tdp"
#define C_FIC_DATA		"ESCOTA_TT_Data.xml"
#define C_FIC_SECTION		"ESCOTA_TT_Sections.xml"
#define C_SCRIPT_FTP 		XDC_PATH_STRADA "/ftp/cci_script_put"
#define C_SCRIPT_FTP_TDP	XDC_PATH_STRADA "/ftp/asftdp_script_put"
#define C_PATH_XML              XDC_PATH_STRADA "/xml"
#define C_PATH                  XDC_PATH_STRADA "/datexTDP"

#define SG_PING         "/etc/ping"
#define SG_TO_PING      8
#define SG_BYTES_PING   64
#define SG_COUNT_PING   1
#define SG_TOKEN_TST    "64"

/* definition des variables locales au module */

static char *	version = "@(#)atdp_ftp.c	1.1 1.1 11/17/08" ;
static char chemin_export[80];

static vm_init_done = 0;

/* declaration de fonctions internes */
void atdp_ecrit_fichier_date(char *, char *);
void atdp_ftp_genere();
void atdp_ftp_diffuse();
char *atdp_ftp_get_heure();


/* definition de fonctions externes */



/*A
*******************************************
*******************************************
* Arguments : Aucun
*******************************************/

int atdp_ftp()
{
  /*generation des fichiers a diffuser*/
  atdp_ftp_genere();

  /*diffusion FTP vers la CCI*/
  atdp_ftp_diffuse();

  /*liberation des ressources*/
  atdp_free_ressources();
}


/* Fichier : @(#)atdp_ftp.c	1.1      Release : 1.1        Date :  11/17/08
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE atdp_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* libere les ressources
*
******************************************************
* SEQUENCE D'APPEL :
* atdp_free_ressources();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
extern int atdp_free_ressources()
{
  return (XDC_OK);
}



/* Fichier : @(#)atdp_ftp.c	1.9       Release : 1.9        Date : 11/06/06
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE atdp_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* generation des fichiers texte a exporter
*
******************************************************
* SEQUENCE D'APPEL :
* atdp_ftp_genere();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
void atdp_ftp_genere()
{
  char pl_nom_fichier[300];
  char vl_date[30];
  int vl_jour,vl_mois,vl_annee,vl_heure,vl_minute,vl_seconde; 
  char vl_horodate[30];
  char vl_str[300];
  XDY_TDP	vl_tdp;
  XDY_Octet vl_valide;
  int vl_indice;
  int vl_indice_H;
  XZLGT_Liste_Zdp	*vl_liste;
  XZLGT_Nombre_Mem	*vl_nombre_mem;
  FILE *pl_fp;
  XDY_Horodate vl_datetime;
  char pl_fin_fichier[50]="";

  strcpy(vl_date,atdp_ftp_get_heure());
  strcpy( chemin_export, C_PATH);


  if (sscanf( vl_date, "%2d/%2d/%4d %2d:%2d:%2d",
                                  &vl_jour,
                                  &vl_mois,
                                  &vl_annee,
                                  &vl_heure,
                                  &vl_minute,
                                  &vl_seconde) != 6) {
     XZST_03EcritureTrace(XZSTC_WARNING,
            "atdp_ftp_genere : le format de la date est incorrecte !" );
     return;
  }
    
  sprintf(pl_nom_fichier,"%s%s%04d%02d%02d%02d%02d.txt",
			CO_REP_OUT,
			C_NOM_TRAFIC,
			vl_annee,
			vl_mois,
			vl_jour,
			vl_heure,
			vl_minute);

  if ( (pl_fp = fopen( pl_nom_fichier, "w+")) == NULL )
  {
    XZST_03EcritureTrace(XZSTC_WARNING,
		"atdp_ftp_genere : impossible de creer le fichier %s",pl_nom_fichier);
    return;
  }
  XZST_03EcritureTrace(XZSTC_WARNING,"atdp_ftp_genere: fichier %s cree",pl_nom_fichier);

  /*acces a la memoire partagee*/
  vl_liste = XZLG992_Get_ZDP();
  vl_nombre_mem = XZLG993_Get_Nombre();

  XZST_03EcritureTrace(XZSTC_WARNING,"atdp_ftp_genere: vl_nombre_mem->Nombre_ZDP = %d",
				vl_nombre_mem->Nombre_ZDP);

  /*en tete du fichier XML SECTION*/
  /*a l init*/
  if (vm_init_done==0) {
    atdp_xml_write2(vl_liste[1],2,"glob");
    for (vl_indice=0;vl_indice<vl_nombre_mem->Nombre_ZDP;vl_indice++) {
       if ((vl_liste[vl_indice].Zdp.Code_Zone < 200) || (vl_liste[vl_indice].Zdp.Code_Zone >300))/* pas les zones ASF*/
         /*fichiers XML SECTION par ZDP*/
         atdp_xml_write2(vl_liste[vl_indice],1,"zdp");
    }
    /*export XML*/
    sprintf(vl_str,"%s/concat_tdp.ksh %s %s",
		  C_PATH_XML,
		  chemin_export,
		  C_FIC_SECTION);
    system(vl_str);
    vm_init_done=1;
  }

  /*recherche de la plus grande horodate de data*/
  vl_indice_H = 0;
  for (vl_indice=0;vl_indice<vl_nombre_mem->Nombre_ZDP;vl_indice++) {
    if (vl_liste[vl_indice].Tdp.Horodate> vl_liste[vl_indice_H].Tdp.Horodate)
      vl_indice_H = vl_indice;
  }
  /*en tete du fichierXML DATA*/
  atdp_xml_write2(vl_liste[vl_indice_H],4,"glob");
  for (vl_indice=0;vl_indice<vl_nombre_mem->Nombre_ZDP;vl_indice++) {
    if ((vl_liste[vl_indice].Zdp.Code_Zone < 200) || (vl_liste[vl_indice].Zdp.Code_Zone >300))/* pas les zones ASF*/
      /*fichiers XML DATA par ZDP*/
      atdp_xml_write2(vl_liste[vl_indice],3,"zdp");
  }
  /*export XML*/ 
      sprintf(vl_str,"%s/concat_tdp_data.ksh %s %s",
                    C_PATH_XML, 
		    chemin_export, 
		   C_FIC_DATA); 
  system(vl_str);

  for (vl_indice=0;vl_indice<vl_nombre_mem->Nombre_ZDP;vl_indice++) {
      /*ecriture dans le fichier*/
      if (vl_liste[vl_indice].Valide) {
      XZLG300_trim(vl_liste[vl_indice].Zdp.Libelle);
      sprintf(vl_str,"%s;%d;%d;%d;%d\n",
		vl_liste[vl_indice].Zdp.Libelle,
		vl_liste[vl_indice].Zdp.Sens,
		vl_liste[vl_indice].Zdp.TDP_Normal,
		vl_liste[vl_indice].Tdp.Tdp,
		vl_liste[vl_indice].Tdp.Validite);
      fputs(vl_str,pl_fp);
    }
  }
  fflush(pl_fp);
  fclose(pl_fp);

  XZST_03EcritureTrace(XZSTC_FONCTION,"atdp_ftp_genere : OUT");
}

/* Fichier : @(#)atdp_ftp.c	1.9       Release : 1.9        Date : 11/06/06
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE atdp_evt
******************************************************
* DESCRIPTION DE LA FONCTION :
* diffusion vers les ASF
*
******************************************************
* SEQUENCE D'APPEL :
* atdp_ftp_diffuse();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
void atdp_ftp_diffuse()
{
  char vl_ligne[600];

  /*FTP*/
  system(C_SCRIPT_FTP);
  system(C_SCRIPT_FTP_TDP);

  /*suppression des fichiers du repertoire*/
  /*sprintf(vl_ligne,"rm %s/*.TXT %s/*.txt",CO_REP_OUT,CO_REP_OUT);
  system(vl_ligne);*/
      
}


/*A
*******************************************
* Fonction qui retourne la date courante
*******************************************
* Arguments : Aucun
*******************************************/

char * atdp_ftp_get_heure()
{
static XZSMT_Horodate  vl_HorodateZero = "";
XZSMT_Horodate  vl_Horodate     = "";
double          vl_HorodateSec  = 0.0;
int             vl_JourSemaine  = 0;



        XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_HorodateSec);
        XZSM_11ConversionHorodate(vl_HorodateSec, &vl_HorodateZero);

        return(vl_HorodateZero);

}


