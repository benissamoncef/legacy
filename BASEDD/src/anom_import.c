/* Fichier : $Id: anom_import.c,v 1.9 2019/02/05 14:36:59 gesconf Exp $ Release : $Revision: 1.9 $   Date : $Date: 2019/02/05 14:36:59 $	*
*************************************************
* STERIA * Projet MIGRAZUR			*
***********************************************1**
* MODULE  : BASEDD				*
* FICHIER : anom_import.c			*
*************************************************
* DESCRIPTION :
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
*************************************************
* HISTORIQUE :
*
* JMG	07/12/01 : creation 1.1
* JMG	11/11/12 : masque noma2 PMVA
* JMG	14/05/13 : modif xzae172
* PNI	05/01/19 : appel de XZAE73 � chaque reveil pour g�n�rer liste des astreintes 1.8 DEM1321
************************************************/

#include "anom_import.h"

/* definition des constantes */

#define C_FIC_FREQUENCE	XDC_PATH_STRADA "/frequence.txt"
#define C_REP_IN		XDC_PATH_STRADA "/nomade/in/"
#define C_REP_OUT		XDC_PATH_STRADA "/nomade/out/"
#define C_FREQ_IN	1
#define C_FREQ_OUT	150

#define C_NOM_VUE               C_REP_IN "demande_vue.txt"
#define C_NOM_MASK              C_REP_IN "demande_masques.txt"
#define C_NOM_SYNTHESE          C_REP_IN "demande_synthese.txt"
#define C_NOM_MAIL_IN		C_REP_IN "demande_mail.txt"
#define C_NOM_MAIL_SAE		C_REP_IN "mail_sae.txt"

#define C_NOM_EVT               C_REP_OUT "fmc"
#define C_NOM_TFM               C_REP_OUT "tfm"
#define C_NOM_REP_SYNTHESE      C_REP_OUT "reponse_synthese"
#define C_NOM_HEURE      	C_REP_OUT "heure_reception"
#define C_NOM_TDP      	C_REP_OUT "tdp"
#define C_NOM_ASTREINTES	C_REP_OUT "astreintes2"
#define C_NOM_MAIL_OUT		C_REP_OUT "mail"
#define C_NOM_TFM_SYNTHESE	C_REP_OUT "tfm_synthese"


/* d�finition des variables globales � l'application */
int anom_import_xzae170(char *, int , int , char *, char *, char *);
int anom_import_xzae171(char *);
int anom_import_xzae201(char *);
int anom_import_xzae172(char *, char *, char *, char *, char *, char *, char *, char *,char *, char *, char *, char *);
int anom_init_xzae173(char *, char *,char *, char *, char *, char *, char *, char *, char *, char *);

void anom_import_ftp();
void anom_reveil(int);

/* definition des variables locales au module */
static char *	version = "$Id: anom_import.c,v 1.9 2019/02/05 14:36:59 gesconf Exp $ $Revision: 1.9 $ $Date: 2019/02/05 14:36:59 $";
static char vg_vue[50]="";

static FILE *vg_fp;

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

int anom_import_init()
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
int			vl_frequence=NULL;
int			vl_station;
int			vl_sens;
XZIAT_destinataire	vl_destinataire;
char			pl_commande[150]="";
char			pl_erreur[200]="";
int			vl_code_retour;



	XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : anom_import_init" );
 	
  	/* Recuperation du nom de la machine locale */
	XZSC_07NomMachine(pl_NomMachine) ;
 	
  	/*A
	* Initialisation de la callback du timer de programmation
	* -------------------------------------------------------
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_NOMADE_TIMER), anom_import_ftp,(T_CB_ARG)NULL) == NULL) 
	{	
                XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_NOMADE_TIMER non cree.",version );
		return XDC_NOK;
	}
	/*if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_NOMADE_REVEIL), anom_reveil, (T_CB_ARG)NULL) == NULL)
	{
		XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_NOMADE_REVEIL non cree.",version );
		return XDC_NOK;
	}*/
 	XZSM_00InitServices();

	/*A Lecture horodate courante systeme */
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
   
	sprintf( vl_cle , "cle_timer_nomade" );
   
	/*A Abonnement au datagroup pour que le timer nous envoie les messages */
	if (! XDG_JoindreDG(XDG_NOMADE_TIMER) )
	{	
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING, 
				"Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
			return XDC_NOK;
	}	
   
	/* armement du timer de programmation */
	/* ---------------------------------- */
	if (XZSM_01DemanderProgrammation(	vl_HorodateSec,
						C_FREQ_IN,
						XDG_NOMADE_TIMER,
						XDM_NOMADE_TIMER,
						vl_cle,
						NULL) != XDC_OK)
	{
	/* Ecriture Trace */
        XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer nomade non declenchee. \n " );
        return XDC_NOK;
	} 

	/*sprintf( vl_cle , "cle_timer_reveil");

	
	if (XZSM_01DemanderProgrammation(       vl_HorodateSec,
						C_FREQ_OUT,
						XDG_NOMADE_TIMER,
						XDM_NOMADE_REVEIL,
						vl_cle,
						NULL) != XDC_OK)
	{
	XZST_03EcritureTrace( XZSTC_WARNING, "Programmation du timer nomade non declenchee. \n " );
	return XDC_NOK;
	}*/
	

	/*suppression d'eventuels anciens fichiers a transmettre*/
/*	sprintf(pl_ligne,"rm %s/*",C_REP_IN);
	system(pl_ligne);*/

	/*generation fichier des astreintes*/
  	vg_fp = fopen (C_NOM_ASTREINTES, "w+");
  	if (vg_fp==NULL)
  	{
    		XZST_03EcritureTrace(   XZSTC_WARNING,
			" anom_init : impossible d'ouvrir le fichier %s",C_NOM_ASTREINTES);
    		return;
  	}
  	/*XZAE173*/
  	if (XZAE173_Recup_Astreintes((XDY_FonctionInt)anom_init_xzae173) != XDC_OK)
  	{
    		XZST_03EcritureTrace(   XZSTC_WARNING," anom_init : appel a anom_init_xzae173 a echoue");
    		fclose(vg_fp);
    		return;
  	}
  	fclose(vg_fp);

	system("tigz_astreinte.ksh");
	
	/* Ecriture Trace */
	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : anom_import_init" );
	return XDC_OK;

}


/*A
*******************************************
* Fonction call back qui reagit au datagroup
* definit dans le timer de programmation
* pour traiter les demandes du 
* SAE NOMADE
*******************************************
* Arguments : Aucun
*******************************************/

void anom_import_ftp()
{
  int vl_mask;
  FILE *pl_fp;
  FILE *pl_fp_out;
  char vl_vue[50];
  char vl_ligne[200];
  XDY_Datagroup   vl_dg;
  int vl_numero,vl_cle;
  int vl_flag_nomade=0;
  int vl_flag_photo=0;
  char vl_destinataire[20];
  char vl_texte[255];
  XZSMT_Horodate  vl_HorodateZero = "";
  XZSMT_Horodate  vl_Horodate     = "";
  double          vl_HorodateSec  = 0.0;
  int             vl_JourSemaine  = 0;

  /* Ecriture Trace */
  XZST_03EcritureTrace( XZSTC_FONCTION, "In : anom_import_ftp");

  system("tscmd.x \"chmod a+rwx /produits/migrazur/appliSD/fichiers/stra/nomade/in/*\"");
  system("tscmd.x \"chmod a+rwx /produits/migrazur/appliSD/fichiers/stra/nomade/out/*\"");
  strcpy(vl_dg,"");
  XDG_EncodeDG2(vl_dg,XDG_ISYN_SERVEUR,"SSCI");

  if ( (pl_fp = fopen( C_NOM_VUE, "r")) != NULL) {
    vl_flag_nomade=0;
    if (fgets(vl_ligne,sizeof(vl_ligne)-1,pl_fp)!=NULL) {
    if (sscanf(vl_ligne,"%[^#]#",vl_vue)==1) {
      /*si demande vue courante*/
      if (!strcmp(vl_vue,"COURANTE"))  {
	/*memoriser qu'il faut faire une photo*/
	vl_flag_nomade=1;
	vl_flag_photo=1;
        sprintf(vl_ligne,"rm %s",C_NOM_VUE);
        system(vl_ligne);
      }
      else {
        /*si vue demande differente de la derniere vue affichee*/
        if (strcmp(vl_vue,vg_vue)) {
          XZST_03EcritureTrace( XZSTC_WARNING," anom_import_ftp : vue a afficher = %s",vl_vue);
	  strcpy(vg_vue,vl_vue);
	   XZIS26_VUE(vl_vue);
           if (!TipcSrvMsgWrite( vl_dg
				    ,XDM_IdentMsg(XDM_ISYN_NOMADE_VUE )
				    ,XDM_FLG_SRVMSGWRITE
				    ,T_IPC_FT_STR, vl_vue
				    ,NULL) )
           {
             XZST_03EcritureTrace(XZSTC_WARNING ,"anom_import_ftp : TipcSrvMsgWrite(XDM_ISYN_NOMADE_VUE) a echoue");
           }
           else
            TipcSrvFlush();
        }
      }
    }
    }
    fclose(pl_fp);
  }

  if ( (pl_fp = fopen( C_NOM_MASK, "rb")) != NULL) {
    vl_flag_nomade=0;
    sleep (1) ;
    fgets(vl_ligne,sizeof(vl_ligne)-1,pl_fp);
    fclose(pl_fp);
    sscanf(vl_ligne,"%d",&vl_mask);
    XZST_03EcritureTrace( XZSTC_WARNING," anom_import_ftp : mask %d ", vl_mask);
    if (vl_mask==1) vl_mask=26;
    XZIS25_MASQUE(vl_mask);
    if (!TipcSrvMsgWrite( vl_dg
				,XDM_IdentMsg(XDM_ISYN_NOMADE_MASK)
				,XDM_FLG_SRVMSGWRITE
				,T_IPC_FT_INT4, vl_mask
				,NULL ) )
    {
      XZST_03EcritureTrace(XZSTC_WARNING ,"anom_import_ftp : TipcSrvMsgWrite(XDM_ISYN_NOMADE_MASK) a echoue");
    }
    else TipcSrvFlush();

    /*suppression du fichier*/
    sprintf(vl_ligne,"rm %s",C_NOM_MASK);
    system(vl_ligne);
  }

  /*on regarde s il y a un mail du PC NOMADE*/
  if ( (pl_fp = fopen( C_NOM_MAIL_IN, "r")) != NULL) {
    /*cle de verification du bon format du fichier*/
    fgets(vl_ligne,sizeof(vl_ligne)-1,pl_fp);
    sscanf(vl_ligne,"%d",&vl_cle);

    if (vl_cle<45) {
    /*recuperation du destinataire*/
    fgets(vl_ligne,sizeof(vl_ligne)-1,pl_fp);
    sscanf(vl_ligne,"%s",vl_destinataire);

    pl_fp_out = fopen(C_NOM_MAIL_SAE,"w");
    fputs("Message en provenance du PC NOMADE\n\n",pl_fp_out);
    XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_HorodateSec);
    XZSM_11ConversionHorodate(vl_HorodateSec, &vl_HorodateZero);
    vl_HorodateZero[16]='\0';
    fprintf(pl_fp_out,"Date d'emission : %s\n\n", vl_HorodateZero);
    fputs("Texte du message : \n\n",pl_fp_out);

    /*texte du mail*/
    while ( (fgets(vl_ligne,sizeof(vl_ligne)-1,pl_fp)!=NULL)){
      fputs(vl_ligne,pl_fp_out);
    }
    fclose(pl_fp);
    fclose(pl_fp_out);

    sprintf(vl_ligne,"xdialog -display %s:0.0 -fg black -bg white -t MAIL_NOMADE -O Acquitter -f %s &",
				vl_destinataire,
				C_NOM_MAIL_SAE);
    XZST_03EcritureTrace(XZSTC_WARNING ,"anom_import_ftp: xdialog = %s",vl_ligne);
    system(vl_ligne);
    /*suppression du fichier*/
    sprintf(vl_ligne,"rm %s",C_NOM_MAIL_IN);
    system(vl_ligne);
    }
    else
      fclose(pl_fp);
  }

  /*si demande du SAE nomade (hors synthese)*/
  if (vl_flag_nomade==1) {
    /*generer copie ecran+TFM+FMC+TDP*/
    anom_reveil(vl_flag_photo);
  }

  if ( (pl_fp = fopen( C_NOM_SYNTHESE, "r")) != NULL) {
    fgets(vl_ligne,sizeof(vl_ligne)-1,pl_fp);
    fclose(pl_fp);
    sscanf(vl_ligne,"%d#%d", &vl_numero, &vl_cle);
    if (vl_cle<=45) {
    sprintf(vl_ligne,"rm %s",C_NOM_SYNTHESE);
    system(vl_ligne);
    XZST_03EcritureTrace( XZSTC_WARNING," anom_import_ftp : evt = %d/%d",vl_numero,vl_cle);

    /*calcul de la synthese de la FMC*/
    XDG_EncodeDG2(vl_dg,XDG_IAX_TEXTE,"SPCI");
    sprintf(vl_ligne,"tanom|%d|%d",vl_numero,vl_cle);
    if (!TipcSrvMsgWrite (vl_dg,
		XDM_IdentMsg(XDM_IAX_NEWTASK),
		XDM_FLG_SRVMSGWRITE,
		T_IPC_FT_INT4, COM_NEWTASK,
		T_IPC_FT_STR,"ITMA_TSE_Synthese_Word",
		T_IPC_FT_STR,vl_ligne,
		T_IPC_FT_INT4, 0,
		NULL) )
      XZST_03EcritureTrace(XZSTC_WARNING ,"anom_import_ftp : TipcSrvMsgWrite(XDM_IAX_NEWTASK) sur %s a echoue",
			vl_dg);
    else
    TipcSrvFlush();
   }
  }
}


/*A
*******************************************
* Fonction call back qui reagit au datagroup
* definit dans le timer de programmation
* pour traiter la generation automatique
* des fichiers a transmettre au 
* SAE NOMADE
*******************************************
* Arguments : Aucun
*******************************************/

void anom_reveil(int va_mode_photo)
{
  XZSMT_Horodate  vl_HorodateZero = "";
  XZSMT_Horodate  vl_Horodate     = "";
  double          vl_HorodateSec  = 0.0;
  int             vl_JourSemaine  = 0;

  XZST_03EcritureTrace(   XZSTC_WARNING,"anom_reveil : in");

  /*generation du fichier contenant l heure */
  vg_fp = fopen (C_NOM_HEURE,"w+");
  if (vg_fp==NULL)
  {
    XZST_03EcritureTrace(   XZSTC_WARNING," anom_reveil : impossible d'ouvrir le fichier %s", C_NOM_HEURE);
    return;
  }
  XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_HorodateSec);
  XZSM_11ConversionHorodate(vl_HorodateSec, &vl_HorodateZero);
  vl_HorodateZero[16]='\0';
  fputs(vl_HorodateZero,vg_fp);
  fclose(vg_fp);

  vg_fp = fopen (C_NOM_EVT,"w+");
  if (vg_fp==NULL) 
  {
    XZST_03EcritureTrace(   XZSTC_WARNING," anom_reveil : impossible d'ouvrir le fichier %s",C_NOM_EVT);
    return;
  }

  /*XZAE170*/
  if (XZAE170_Recup_Liste_Evt((XDY_FonctionInt)anom_import_xzae170) != XDC_OK)
  {
    XZST_03EcritureTrace(   XZSTC_WARNING," anom_reveil : appel a XZAE170_Recup_Liste_Evt a echoue");
    fclose(vg_fp);
    return;
  }
  fclose(vg_fp);


  /*generation du fichier des derniers messages TFM*/
  vg_fp = fopen (C_NOM_TFM, "w+");
  if (vg_fp==NULL)
  {
    XZST_03EcritureTrace(   XZSTC_WARNING," anom_reveil : impossible d'ouvrir le fichier %s",C_NOM_TFM);
    return;
  }
  /*XZAE171*/
  if (XZAE171_Recup_Msg_TFM((XDY_FonctionInt)anom_import_xzae171) != XDC_OK)
  {
    XZST_03EcritureTrace(   XZSTC_WARNING," anom_reveil : appel a XZAE171_Recup_Msg_TFM a echoue");
    fclose(vg_fp);
    return;
  }
  fclose(vg_fp);

  /*generation du fichier contenant la synthese des messages TFM*/
  vg_fp = fopen (C_NOM_TFM_SYNTHESE,"w+");
  if (vg_fp==NULL)
  {
    XZST_03EcritureTrace(   XZSTC_WARNING," anom_reveil : impossible d'ouvrir le fichier %s",C_NOM_TFM_SYNTHESE);
    return;
  }
  /*XZAE201*/
  if (XZAE201_TFM_Synthese((XDY_FonctionInt)anom_import_xzae201) != XDC_OK)
  {
    XZST_03EcritureTrace(   XZSTC_WARNING," anom_reveil : appel a XZAE201_Synthese_TFM a echoue");
    fclose(vg_fp);
    return;
  }
  fclose(vg_fp);

  /*generation du fichier contenant les temps de parcours*/
  vg_fp = fopen (C_NOM_TDP, "w+");
  if (vg_fp==NULL)
  {
    XZST_03EcritureTrace(   XZSTC_WARNING," anom_reveil : impossible d'ouvrir le fichier %s",C_NOM_TDP);
    return;
  }

  /*XZAE172*/
  if (XZAE172_Recup_TDP((XDY_FonctionInt)anom_import_xzae172) != XDC_OK)
  {
    XZST_03EcritureTrace(   XZSTC_WARNING," anom_reveil : appel a XZAE172_Recup_TDP a echoue");
    fclose(vg_fp);
    return;
  }
  fclose(vg_fp);

  /*compression des fichiers texte generes*/
  system("tigz.ksh >>/dev/null 2>>/dev/null");

  /*copie du synoptique*/
  if (va_mode_photo==1)
    system("tigif.ksh >>/dev/null 2>>/dev/null");

	/*generation fichier des astreintes*/
  	vg_fp = fopen (C_NOM_ASTREINTES, "w+");
  	if (vg_fp==NULL)
  	{
    		XZST_03EcritureTrace(   XZSTC_WARNING,
			" anom_init : impossible d'ouvrir le fichier %s",C_NOM_ASTREINTES);
    		return;
  	}
  	/*XZAE173*/
  	if (XZAE173_Recup_Astreintes((XDY_FonctionInt)anom_init_xzae173) != XDC_OK)
  	{
    		XZST_03EcritureTrace(   XZSTC_WARNING," anom_init : appel a anom_init_xzae173 a echoue");
    		fclose(vg_fp);
    		return;
  	}
  	fclose(vg_fp);

	system("tigz_astreinte.ksh");


}


/*liste des evts*/
int anom_import_xzae170(char *va_etat, int va_numero, int va_cle, char *va_type, char *va_debut, char *va_local)
{
  char vl_ligne[500];

  sprintf(vl_ligne,"%s#%d#%d#%s#%s#%s\n",
			va_etat,
			va_numero,
			va_cle,
			va_type,
			va_debut,
			va_local);
  fputs(vl_ligne,vg_fp);
  return (XDC_OK);
}

/*messages TFM*/
int anom_import_xzae171(char *va_msg)
{
  fputs(va_msg,vg_fp);
  fputs("\n",vg_fp);
  return (XDC_OK);
}

/*synthese des messages TFM*/
int anom_import_xzae201(char *va_msg)
{
  fputs(va_msg,vg_fp);
  fputs("\n",vg_fp);
  return (XDC_OK);
}

int anom_import_xzae172(char *va_date, char *va_tdp11,char *va_tdp12,
			char *va_tdp21, char *va_tdp22,
			char *va_tdp31, char *va_tdp32,
			char *va_tdp41, char *va_tdp42,
			char *va_tdp51, char *va_tdp52, char *va_tdp53)
{
  char vl_ligne[500];
  sprintf(vl_ligne,"%s#%s#%s#%s#%s#%s#%s#%s#%s#%s#%s#%s#\n",
		va_date,
		va_tdp11,
		va_tdp12,
		va_tdp21,
		va_tdp22,
		va_tdp31,
		va_tdp32,
		va_tdp41,
		va_tdp42,
		va_tdp51,va_tdp52,va_tdp53);
  fputs(vl_ligne,vg_fp);
  return (XDC_OK);
}


int anom_init_xzae173(char *va_nom, char *va_explication_tel1,char *va_tel1,
			char *va_explication_tel2, char *va_tel2,
			char *va_radio, char *va_nom_type_astreinte,
			char *va_nom_sous_type_astreinte, char *va_date_debut_formattee,
			char *va_date_fin_formattee)
{
  char vl_ligne[500];
  char vl_nom[50];
  char vl_explication_tel1[50];
  char vl_tel1[50];
  char vl_explication_tel2[50];
  char vl_tel2[50];
  char vl_radio[50];
  char vl_nom_type_astreinte[50];
  char vl_nom_sous_type_astreinte[50];
  char vl_date_debut_formattee[50];
  char vl_date_fin_formattee[50];

  if (va_nom[0]=='\0') strcpy(vl_nom," "); else strcpy(vl_nom,va_nom);
  if (va_explication_tel1[0]=='\0') strcpy(vl_explication_tel1," "); else strcpy(vl_explication_tel1,va_explication_tel1);
  if (va_tel1[0]=='\0') strcpy(vl_tel1," "); else strcpy(vl_tel1,va_tel1);
  if (va_explication_tel2[0]=='\0') strcpy(vl_explication_tel2," "); else strcpy(vl_explication_tel2,va_explication_tel2);
  if (va_tel2[0]=='\0') strcpy(vl_tel2," "); else strcpy(vl_tel2,va_tel2);
  if (va_radio[0]=='\0') strcpy(vl_radio," "); else strcpy(vl_radio,va_radio);
  if (va_nom_type_astreinte[0]=='\0') strcpy(vl_nom_type_astreinte," "); else strcpy(vl_nom_type_astreinte,va_nom_type_astreinte);
  if (va_nom_sous_type_astreinte[0]=='\0') strcpy(vl_nom_sous_type_astreinte," "); else strcpy(vl_nom_sous_type_astreinte,va_nom_sous_type_astreinte);
  if (va_date_debut_formattee[0]=='\0') strcpy(vl_date_debut_formattee," "); else strcpy(vl_date_debut_formattee,va_date_debut_formattee);
  if (va_date_fin_formattee[0]=='\0') strcpy(vl_date_fin_formattee," "); else strcpy(vl_date_fin_formattee,va_date_fin_formattee);
  sprintf(vl_ligne,"%s#%s#%s#%s#%s#%s#%s#%s#%s#%s#\n",
		vl_nom,
		vl_explication_tel1,
		vl_tel1,
		vl_explication_tel2,
		vl_tel2,
		vl_radio,
		vl_nom_type_astreinte,
		vl_nom_sous_type_astreinte,
		vl_date_debut_formattee,
		vl_date_fin_formattee);
  fputs(vl_ligne,vg_fp);
  return (XDC_OK);
}
