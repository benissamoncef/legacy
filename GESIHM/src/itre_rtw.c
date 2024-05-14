/* fichier : $RCSfile: itre_rtw.c,v $   Release : $Revision: 1.33 $   Date : $Date: 2021/06/16 08:10:57 $	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : MTRE				*
* FICHIER : itre_rtw.c				*	
*************************************************
* DESCRIPTION :
* fonctions dont on peut demander l'execution
* a la tache relais
*
*************************************************
* HISTORIQUE :
*
* J.M. Guilhou	28 Sep 1994	Creation  V1.1
* F. VOLCIC	19 Mai 1995	Modif appel menu_LT (V1.12)
* B. Noel	19 Fev 1996	Demande d'init du type d'imprimante � Applix main courante
*				et Applix synoptique. (DEM 91)						V1.13
* Niepceron	26 Sep 1996	Ajout ITRE_AX_ACQT (DEM/424) 1.107
* Torregrossa	04 Dec 1996	Modif fichier genere pour fmc locales (680) 1.16
* Niepceron	10 mar 1999	Modif intergest pour fmc locales  1.17
* Lecrivain	16 Sep 2008	Ajout fonction de traitement de messages d'alarmes pour etat du Pirau (DEM 808) 1.18
* JMG    10/02/10        : portage Linux dem/934
* JMG	22/01/11	: ajout boutons alarmes et etats eqt 1.21
* JPL	23/12/11	Utilisation de symboles de constantes et primitives Migrazur  1.22
* JPL	26/12/11	Ajout fonctions de gestion du timer TdP pour les postes radio (DEM 1007)  1.23
* JMG	29/05/12	linux 1.24
* PNI	20/11/13	: correction du sendpoke pour timer TdP (DEM1007) 1.25
* PNI	15/12/14 	: Mise en place du timer pour activer socket DEM1116 1.27
* JMG	29/10/18	: RMVI DEM1315 1.28
* JMG	17/11/19	: ajout TDP surcharge DEM1365 1.29
* JMG	08/06.20 :;	EMS 1.30
* GR    15/07/20        : EMS
* PNI	28/01/21	: KEEP_ALIVE : ajout du sendpoke vers le second �cran SAE-231 1.31
* jmg	04/03/21	: reprise initMce pour API 1.32
* LCL	15/06/21	: Fix SEGFAULT sur reception RMVI SAE-295 1.33
* LCL	10/09/21	: Adaptations EMS / MAGISTRA 1.34
* LCL		20/12/21	: MOVIS P2 ems SAE-312
************************************************/

#include <stdlib.h>

#include "xdg.h"

#include "itre_rtw.h"

#define ERRBUFSIZE 300

/* definition des variables locales au module */
static char *	version = "$Id: itre_rtw.c,v 1.33 2021/06/16 08:10:57 pc2dpdy Exp $ : commentaire" ;

/*fichier contenant la liste des FMC locales*/
#define C_FICHIER	"../fichiers/dyn/ITMA_Liste_fmc_locales.txt"

#define CM_DISPO 1
#define CM_INHIB 2
#define CM_HS    3
#define CM_INDIS 4
#define CM_MARG  5

#define CM_CMD_DEL_SEMA "xzsx_cmd_sur.sh"
#define CM_CMD_LIVRE_MODERN "xlivre_modern1.sh"
#define	CM_CMD_ENVOI_VH		XDC_PATHEXE "/tienvoivh.ksh"
#define	CM_CMD_ENVOI_VH_ASF	XDC_PATHEXE "/tienvoivh_ASF.ksh"
#define	CM_CMD_EXPORT_VH	XDC_PATHEXE "/tiexportvh.ksh"
#define	REP_ALARME		"../fichiers/ala/"
#define	REP_FICHIER_MAGISTRA		"/usr/share/magistra/magistra-fichiers/fichiers/"


/* fichier : $RCSfile: itre_rtw.c,v $   Release : $Revision: 1.33 $   Date : $Date: 2021/06/16 08:10:57 $	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : MTRE				*
*************************************************
* DESCRIPTION :
* lance la main courante
*
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_lanceMce(
XDY_Datagroup va_dg,
char *pa_arg
)

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
    /*message de lancement de la main courante*/
    TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_NEWTASK),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_NEWTASK,
		    T_IPC_FT_STR, "ITMA_TMT_Menu_Ecran_textuel", /*nom macro*/
		    T_IPC_FT_STR, pa_arg, /* 1 argument*/
		    T_IPC_FT_INT4, 0,
		    NULL);

    TipcSrvFlush();

  return (XDC_OK);
}



/* fichier : $RCSfile: itre_rtw.c,v $   Release : $Revision: 1.33 $   Date : $Date: 2021/06/16 08:10:57 $	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : MTRE				*
*************************************************
* DESCRIPTION :
* initialise  Applix cote main courante
*
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_initMce(
XDY_Datagroup va_dg,
XDY_NomMachine va_machine
)

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{

char	vl_type_imprimante[2];
char vl_valeur[10]; /*LINUX*/
int va_canal_IHM;


XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_initMce IN *************************************************************");

  if (!strcmp(va_dg,vg_dg_dial))
    va_canal_IHM = vg_socEnvToAxDial;
  else
    va_canal_IHM = vg_socEnvToAxMce;


    /*messages de valorisation des variables systemes d'applix*/
    /*TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_SETVARCTX),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_SETVARCTX,
		    T_IPC_FT_STR, "CTX_DG_MCE",
		    T_IPC_FT_STR, vg_dg_mce,
		    NULL);
    TipcSrvFlush();*/
    itre_com_EcrireVarCtx(va_canal_IHM,"CTX_DG_MCE",vg_dg_mce,ITRE_COM_TRUE, va_machine);

  XZST_03EcritureTrace(   XZSTC_FONCTION, "envoi vg_dg_mce");
  /*  TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_SETVARCTX),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_SETVARCTX,
		    T_IPC_FT_STR, "vg_dg_dial",
		    T_IPC_FT_STR, vg_dg_dial, 
		    NULL);
    TipcSrvFlush();*/
    itre_com_EcrireVarCtx(va_canal_IHM,"vg_dg_dial",vg_dg_dial,ITRE_COM_TRUE, va_machine);
  XZST_03EcritureTrace(   XZSTC_FONCTION, "envoi vg_dg_dial");

    /*
    TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_SETVARCTX),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_SETVARCTX,
		    T_IPC_FT_STR, "vg_nom_site",
		    T_IPC_FT_STR, vg_nomSite, 
		    NULL);
    TipcSrvFlush();*/
    itre_com_EcrireVarCtx(va_canal_IHM,"vg_nom_site",vg_nomSite,ITRE_COM_TRUE, va_machine);

    /*
    TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_SETVARCTX),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_SETVARCTX,
		    T_IPC_FT_STR, "vg_poste",
		    T_IPC_FT_STR, vg_nomMachine, 
		    NULL);
    TipcSrvFlush(); */
    itre_com_EcrireVarCtx(va_canal_IHM,"vg_poste",vg_nomMachine,ITRE_COM_TRUE, va_machine);

    sprintf(vl_valeur,"%d",vg_type_imprimante);
    /*TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_SETVARCTX),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_SETVARCTX,
		    T_IPC_FT_STR, "vg_type_imprimante",
		    T_IPC_FT_STR, vl_valeur,
		    NULL);
    TipcSrvFlush();*/
    itre_com_EcrireVarCtx(va_canal_IHM,"vg_type_imprimante",vl_valeur,ITRE_COM_TRUE, va_machine);

    sprintf(vl_valeur,"%d",vg_type_machine);
    /*TipcSrvMsgWrite(va_dg,
		XDM_IdentMsg(XDM_IAX_SETVARCTX),
		FALSE,
		T_IPC_FT_INT4, ITRE_RTW_SETVARCTX,
	        T_IPC_FT_STR, "vg_type_machine",
		T_IPC_FT_STR, vl_valeur,
	        NULL);
    TipcSrvFlush();*/
    itre_com_EcrireVarCtx(va_canal_IHM,"vg_type_machine",vl_valeur,ITRE_COM_TRUE, va_machine);

    /*message de lancement de la main courante*/
    /*TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_NEWTASK),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_NEWTASK,
		    T_IPC_FT_STR, "ITMA_TDO",
		    T_IPC_FT_STR, "ITMA_TDO", 
		    T_IPC_FT_INT4, 0,
		    NULL);

    TipcSrvFlush();*/
    itre_com_NewTask(va_canal_IHM,"ITMA_TDO","ITMA_TDO",1,0,(char *)0,(int *)0, va_machine);
  XZST_03EcritureTrace(   XZSTC_FONCTION, "envoi itma_tdo");

  /*XZAH11_Init_IHM();*/ /*linux*/

  return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* lance la tache relais pour Applix cote synoptiques 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_lanceDial(
	  XDY_Datagroup va_dg,
	  char *va_appelant
          )

/*
* ARGUMENTS EN ENTREE :
* va_dg: datagroup de la tache relais cote synoptiques
* va_appelant: module appelant de cette fonction
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* FONCTION 
------------------------------------------------------*/
{
  char    vl_type_imprimante[2];
  char vl_valeur[10];
  int va_canal_IHM;
  T_IPC_MSG	vl_msg;


XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_lanceDial IN *************************************************************");
  if (!strcmp(va_dg,vg_dg_dial))
    va_canal_IHM = vg_socEnvToAxDial;
  else
    va_canal_IHM = vg_socEnvToAxMce;

    /*TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_SETVARCTX),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_SETVARCTX,
		    T_IPC_FT_STR, "vg_dg_mce",
		    T_IPC_FT_STR, vg_dg_mce, 
		    NULL);
    TipcSrvFlush();*/
    itre_com_EcrireVarCtx(va_canal_IHM,"vg_dg_mce",vg_dg_mce,ITRE_COM_TRUE,"");

    TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_SETVARCTX),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_SETVARCTX,
		    T_IPC_FT_STR, "vg_dg_dial",
		    T_IPC_FT_STR, vg_dg_dial, 
		    NULL);
    TipcSrvFlush();
    itre_com_EcrireVarCtx(va_canal_IHM,"vg_dg_dial",vg_dg_dial,ITRE_COM_TRUE,"");

    /*TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_SETVARCTX),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_SETVARCTX,
		    T_IPC_FT_STR, "vg_nom_site",
		    T_IPC_FT_STR, vg_nomSite, 
		    NULL);
    TipcSrvFlush();*/
    itre_com_EcrireVarCtx(va_canal_IHM,"vg_nom_site",vg_nomSite,ITRE_COM_TRUE,"");

    /*TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_SETVARCTX),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_SETVARCTX,
		    T_IPC_FT_STR, "vg_poste",
		    T_IPC_FT_STR, vg_nomMachine, 
		    NULL);
    TipcSrvFlush();*/
    itre_com_EcrireVarCtx(va_canal_IHM,"vg_poste", vg_nomMachine,ITRE_COM_TRUE,"");

    sprintf(vl_valeur,"%d",vg_type_imprimante);
    /*TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_SETVARCTX),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_SETVARCTX,
		    T_IPC_FT_STR, "vg_type_imprimante",
		    T_IPC_FT_STR, vl_valeur,
		    NULL);
    TipcSrvFlush();*/
    itre_com_EcrireVarCtx(va_canal_IHM,"vg_type_imprimante", vl_valeur,ITRE_COM_TRUE,"");

    sprintf(vl_valeur,"%d",vg_type_machine);
    /*TipcSrvMsgWrite(va_dg,
		XDM_IdentMsg(XDM_IAX_SETVARCTX),
		FALSE,
		T_IPC_FT_INT4, ITRE_RTW_SETVARCTX,
		T_IPC_FT_STR, "vg_type_machine",
		T_IPC_FT_STR, vl_valeur,
		NULL);
    TipcSrvFlush();*/
    itre_com_EcrireVarCtx(va_canal_IHM,"vg_type_machine", vl_valeur,ITRE_COM_TRUE,"");

    /* on lance la macro de gestion des FMC locales*/
    /* qui doit tourner meme si l'operateur n'est pas la*/
    /* sur les PC simplifies*/
    /*if (vg_type_machine==XZSAC_TYPEM_PCS)
      TipcSrvMsgWrite(va_dg,
			XDM_IdentMsg(XDM_IAX_NEWTASK),
			FALSE,
			T_IPC_FT_INT4, ITRE_RTW_NEWTASK,
			T_IPC_FT_STR, "ITMA_TLT_Gere_FMC_Locales",
			T_IPC_FT_STR, va_appelant,
			T_IPC_FT_INT4, 0,
			NULL);*/

    /*on lance la macro de login*/
    /*TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_NEWTASK),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_NEWTASK,
		    T_IPC_FT_STR, "ITMA_TLU_login",
		    T_IPC_FT_STR, va_appelant,
		    T_IPC_FT_INT4, 0,
		    NULL);
    TipcSrvFlush();*/
    itre_com_NewTask(va_canal_IHM,"ITMA_TLU_login",va_appelant,1,0,(char *)0,(int *)0,"");

    return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* lance la tache relais pour Applix cote LT 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_initLT(
	  XDY_Datagroup va_dg
          )

/*
* ARGUMENTS EN ENTREE :
* va_dg: datagroup de la tache relais cote synoptiques
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* FONCTION 
------------------------------------------------------*/
{
  T_IPC_MSG	vl_msg;

    TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_SETVARCTX),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_SETVARCTX,
		    T_IPC_FT_STR, "vg_dg_mce",
		    T_IPC_FT_STR, vg_dg_mce, 
		    NULL);
    TipcSrvFlush();

    TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_SETVARCTX),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_SETVARCTX,
		    T_IPC_FT_STR, "vg_dg_dial",
		    T_IPC_FT_STR, vg_dg_dial, 
		    NULL);
    TipcSrvFlush();

    TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_SETVARCTX),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_SETVARCTX,
		    T_IPC_FT_STR, "vg_nom_site",
		    T_IPC_FT_STR, vg_nomSite, 
		    NULL);
    TipcSrvFlush();

    TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_SETVARCTX),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_SETVARCTX,
		    T_IPC_FT_STR, "vg_poste",
		    T_IPC_FT_STR, vg_nomMachine, 
		    NULL);
    TipcSrvFlush();

    /*on lance le menu Applix pour les locaux techniques*/
    TipcSrvMsgWrite(va_dg,
		    XDM_IdentMsg(XDM_IAX_NEWTASK),
		    FALSE,
		    T_IPC_FT_INT4, ITRE_RTW_NEWTASK,
		    T_IPC_FT_STR, "ITMA_TLU_login_LT", /*nom macro*/
		    T_IPC_FT_STR, "ITMA_TLU_login_LT", /* 1 argument*/
		    T_IPC_FT_INT4, 0,
		    NULL);
    TipcSrvFlush();

    return (XDC_OK);
}


#ifndef _TIBCO_EMS
/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_IRAF_FMC_LOC
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_processFMCLoc(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg,
int va_canal_IHM
)

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
  #ifndef _TIBCO_EMS 
  T_REAL8 		*pl_horodate;
  T_REAL8 		*pl_debut;
  T_REAL8 		*pl_fin;
  T_STR	  		*pl_localisation;
  T_INT4		*pl_numero;
  T_INT2		*pl_cle;
  T_INT2		*pl_type;
  T_INT2		*pl_operation;
  T_INT2		*pl_datex;
  T_INT4		vl_nb_donnees,vl_i,vl_j,vl_k;
  #else
  XDY_Horodate 		pl_horodate[1000].;
  XDY_Horodate 		pl_debut[1000];
  XDY_Horodate 		pl_fin[1000];
  XDY_Texte       	pl_localisation[1000];
  XDY_entier		pl_numero[1000];
  XDY_Mot		pl_cle[1000];
  XDY_Mot		pl_type[1000];
  XDY_Mot		pl_operation[1000];
  XDY_Mot		pl_datex[1000];
  XDY_entier		vl_nb_donnees,vl_i,vl_j,vl_k;
  #endif
  char			*tl_params[1];
  char			vl_str[300];
  FILE			*pl_file;
  XDY_NomMachine        vl_machine;

  XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : itre_rtw_processFMCLoc");

  /*recuperation des donnees du message*/
  if (!TipcMsgRead(pa_data->msg,
			T_IPC_FT_INT4_ARRAY, &pl_numero, &vl_nb_donnees,
			T_IPC_FT_INT2_ARRAY, &pl_cle, &vl_i,
			T_IPC_FT_INT2_ARRAY, &pl_type, &vl_j,
			T_IPC_FT_REAL8_ARRAY, &pl_horodate, &vl_j,
			T_IPC_FT_STR_ARRAY, &pl_localisation, &vl_k,
			T_IPC_FT_REAL8_ARRAY, &pl_debut, &vl_j,
			T_IPC_FT_REAL8_ARRAY, &pl_fin, &vl_j,
			T_IPC_FT_INT2_ARRAY, &pl_operation, &vl_j,
			T_IPC_FT_INT2_ARRAY, &pl_datex, &vl_j,
			NULL)) {
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processFMCLoc: pb dans le decodage du message");
    return (XDC_NOK);
  }

  XZST_03EcritureTrace(XZSTC_DEBUG1,"itre_rtw_processFMCLoc: nb de FMC locales = %d",vl_nb_donnees);

  /*ouverture du fichier des FMC locales*/
  if ( (pl_file=fopen(C_FICHIER,"w"))==NULL) {
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processFMCLoc: pb dans l'ouverture du fichier %s",C_FICHIER);
    return (XDC_NOK);
  }

  /*ecriture de chaque FMC dans le fichier*/
  for (vl_i=0; vl_i<vl_nb_donnees; vl_i++) {
    sprintf(vl_str,"%d|%d|%d|%d|%s|%d|%d|%d|%d\n",
		pl_numero[vl_i],
		pl_cle[vl_i],
		pl_type[vl_i],
		(int)pl_horodate[vl_i],
		pl_localisation[vl_i],
		(int)pl_debut[vl_i],
		(int)pl_fin[vl_i],
		pl_operation[vl_i],
		pl_datex[vl_i]);
    fputs(vl_str,pl_file);
  }
  fclose(pl_file);
  
  /*transmission a Applix*/
  if (XZSC_GetDestinataire(pa_data, vl_machine)!=XDC_OK){
	XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processFMCLoc : pb dans recuperation machine destinataire du message");
	return (XDC_NOK);
  }
  sprintf(vl_str,"%d",vl_nb_donnees);
  tl_params[0]=vl_str;
  if (itre_com_SendPoke(va_canal_IHM,COM_CANAL_MTLT_LOCAL,tl_params,1,vl_machine)!= XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processFMCLoc: pb dans emission a Applix du poke COM_CANAL_MTLT_LOCAL");
    return (XDC_NOK);
  }

  XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : itre_rtw_processFMCLoc");
  return (XDC_OK);
}
#endif


/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_IAX_FINPOSTE
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_processFIN(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg,
int va_canal_IHM
)

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{ 
  char 	*pl_params;
  char	vl_str[300]="Le CI a demand� votre fin de poste. Vous allez etre deconnecte";
  char  vl_arg[20]="itre_rtw";
  XDY_NomMachine        vl_machine;

  XZST_03EcritureTrace (XZSTC_FONCTION, "IN : itre_rtw_processFIN");

  /*on fait un info message*/
  if (XZSC_GetDestinataire(pa_data, vl_machine)!=XDC_OK){
        XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processFIN : pb dans recuperation machine destinataire du message");
        return (XDC_NOK);
  }
  pl_params=vl_str;
  if (itre_com_NewTask(vg_socEnvToAxMce,"INFO_MESSAGE@",pl_params,2,0,(char *)0,(int *)0,vl_machine)!=XDC_OK)
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processFIN: pb dans l'appel a itre_com_NewTask");

  /*on attend un peu*/
  sleep(1);

  /*on se casse*/
  XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : itre_rtw_processFIN");
  itre_dir_fin_communication();

  return (XDC_OK);
}  


/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_IPOSIT_INDIC_EQT
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_processEQT(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg,
int va_canal_IHM
)

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{ 
  T_INT4	vl_etat4 ;
#ifdef _TIBCO_EMS
  XDY_Entier	va_etat;
  XDY_Mot	vl_type,vl_no;
#else
  T_INT4	va_etat;
  T_INT2	vl_type,vl_no;
#endif
  char vl_st_rouge[3];
  char 	*tl_params[3];
  XDY_NomMachine        vl_machine;

  XZST_03EcritureTrace (XZSTC_FONCTION, "IN : itre_rtw_processEQT");

  /*recuperation des donnees du message*/
  if (!TipcMsgRead(pa_data->msg,
			T_IPC_FT_INT2,&vl_type,
			T_IPC_FT_INT2,&vl_no,
			T_IPC_FT_INT4,&va_etat,
			NULL)) {
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processEQT: pb dans le decodage du message");
    return (XDC_NOK);
  }
XZST_03EcritureTrace(XZSTC_DEBUG1,"Message correctement lu");
   /*A Determination de l'etat de l'equipement : hs inhibe indispo marginal dispo */
    if ( (va_etat & XDC_EQT_HS) == XDC_EQT_HS )
       vl_etat4 = CM_HS ;
    else
    if ( (va_etat & XDC_EQT_INHIBE) == XDC_EQT_INHIBE )
       vl_etat4 = CM_INHIB;
    else
    if ( ((va_etat & XDC_EQT_CRITIQUE) == XDC_EQT_CRITIQUE) ||
         ((va_etat & XDC_EQT_MAJEUR) == XDC_EQT_MAJEUR) ||
         ((va_etat & XDC_EQT_DESACTIVE) == XDC_EQT_DESACTIVE) )
       vl_etat4 = CM_INDIS;
    else
    if ((va_etat & XDC_EQT_MINEUR) == XDC_EQT_MINEUR)
       vl_etat4 = CM_MARG;
    else
       vl_etat4 = CM_DISPO;

  if ( vl_etat4 == CM_INDIS )
	sprintf(vl_st_rouge,"%d", 2); /*rouge*/
  else if (vl_etat4 == CM_MARG) 
    sprintf(vl_st_rouge,"%d", 1); /*orange*/
  else
    return (XDC_OK); 

  /*on fait un info message*/
  tl_params[0]=vl_st_rouge;
  if (XZSC_GetDestinataire(pa_data, vl_machine)!=XDC_OK){
        XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processEQT : pb dans recuperation machine destinataire du message");
        return (XDC_NOK);
  }

  if (itre_com_SendPoke(vg_socEnvToAxDial,COM_CANAL_MTMT_EQT,tl_params,1,vl_machine) != XDC_OK) 
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processEQT: pb dans l'appel a itre_com_SendPoke");

  XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : itre_rtw_processEQT");
  return (XDC_OK);
}


 
/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_IPOSIT_INDIC_ALARME
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_processALA(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg,
int va_canal_IHM
)

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
#ifdef _TIBCO_EMS
  XDY_Mot	vl_rouge ;
#else
  T_INT2	vl_rouge ;
#endif
  char vl_st_rouge[3];
  char 	*tl_params[3];
  XDY_NomMachine        vl_machine;

  XZST_03EcritureTrace (XZSTC_FONCTION, "IN : itre_rtw_processALA");

  /*recuperation des donnees du message*/
  if (!TipcMsgRead(pa_data->msg,
			T_IPC_FT_INT2, &vl_rouge, 
			NULL)) {
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processALA: pb dans le decodage du message");
    return (XDC_NOK);
  }
  /*on fait un info message*/
  sprintf(vl_st_rouge,"%d", vl_rouge);
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processALA: %s",vl_st_rouge);
  tl_params[0]=vl_st_rouge;
  if (XZSC_GetDestinataire(pa_data, vl_machine)!=XDC_OK){
        XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processALA : pb dans recuperation machine destinataire du message");
        return (XDC_NOK);
  }

  if (itre_com_SendPoke(vg_socEnvToAxDial,COM_CANAL_MTMT_ALARME,tl_params,1,vl_machine) != XDC_OK) 
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processALA: pb dans l'appel a itre_com_SendPoke");


  XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : itre_rtw_processALA");

  return (XDC_OK);
}  

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_IAX_ACQT
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_processACQT(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg,
int va_canal_IHM
)

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{ 
#ifdef _TIBCO_EMS
  XDY_Entier	vl_position, vl_numero;
  XDY_Mot	vl_cle;
#else
  T_INT4	vl_position, vl_numero;
  T_INT2	vl_cle;
#endif
  XDY_NomMachine        vl_machine;

  XZST_03EcritureTrace (XZSTC_FONCTION, "IN : itre_rtw_processACQT");

  /*recuperation des donnees du message*/
  if (!TipcMsgRead(pa_data->msg,
			T_IPC_FT_INT4, &vl_position, 
			T_IPC_FT_INT4, &vl_numero,
			T_IPC_FT_INT2, &vl_cle,
			NULL)) {
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processACQT: pb dans le decodage du message");
    return (XDC_NOK);
  }

  if (XZSC_GetDestinataire(pa_data, vl_machine)!=XDC_OK){
        XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processACQT : pb dans recuperation machine destinataire du message");
        return (XDC_NOK);
  }

  if (itre_rtw_processACQT_titre(vl_position,vl_numero,vl_cle,vl_machine) != XDC_OK)
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processACQT : pb dans l'appel a itre_rtw_processACQT_titre");

  XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : itre_rtw_processACQT");

  return (XDC_OK);
}  

/********************************************************
 * Deport du traitement de int itre_rtw_processACQT 
 * afin de pouvoir l'appeler explicitement
 * au lieu qu'il soit appele uniquement sur 
 * le callback de reception du message 4337 XDM_IAX_ACQT
 ********************************************************
 */
int itre_rtw_processACQT_titre( XDY_Entier vl_position, 
                                XDY_Entier vl_numero, 
                                XDY_Mot vl_cle,
                                XDY_NomMachine vl_machine)
{
  XDY_Nom	vl_st_position, vl_st_numero, vl_st_cle;
  char 	*tl_params[3];

  XZST_03EcritureTrace (XZSTC_FONCTION, "IN : itre_rtw_processACQT_titre position=%d FMC=%d/%d", vl_position, vl_numero, vl_cle);
  /*on fait un info message*/
  sprintf(vl_st_position,"%d", vl_position);
  sprintf(vl_st_numero,"%d", vl_numero);
  sprintf(vl_st_cle,"%d", vl_cle);
  tl_params[0]=vl_st_position;
  tl_params[1]=vl_st_numero;
  tl_params[2]=vl_st_cle;

  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTLT_ACQT,tl_params,3,vl_machine) != XDC_OK) 
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processACQT_titre: pb dans l'appel a itre_com_SendPoke");

  XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : itre_rtw_processACQT_titre");

}



/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_RMVI_STATUS
*************************************************
* SEQUENCE D'APPEL :
*/
extern int itre_rtw_processRMVI_status(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
)
{
  int vl_status;
#ifndef _TIBCO_EMS
  T_STR vl_texte;
#else
  char  vl_texte[200];
#endif
  XDY_NomMachine vl_machine;
  char  *tl_params[2];
  char vl_st_status[20];

  XZST_03EcritureTrace(XZSTC_FONCTION,"IN : itre_rtw_processRMVI_status");

  /*recuperation des donnees du message*/
  if (TipcMsgRead(pa_data->msg,	T_IPC_FT_INT4, &vl_status, 
				T_IPC_FT_STR, &vl_texte,
				NULL))
  {
    XZST_03EcritureTrace(XZSTC_FONCTION,
                            "itre_rtw_processRMVI_status: status=%d texte=%s",
                            vl_status, vl_texte);
  }
  else
  {
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processRMVI_status: pb dans le decodage du message");
    return (XDC_NOK);
  }
  sprintf(vl_st_status,"%d",vl_status);
  tl_params[0]=vl_st_status;
  tl_params[1]=vl_texte;
  if (XZSC_GetDestinataire(pa_data, vl_machine)!=XDC_OK){
        XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processRMVI_status : pb dans recuperation machine destinataire du message");
        return (XDC_NOK);
  }
  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_RMVI_STATUS,tl_params,2,vl_machine) != XDC_OK)
  	XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processRMVI_status pb dans l'appel a itre_com_SendPoke");

 XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : itre_rtw_processRMVI_status");
 return (XDC_OK);
}

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_RMVI_RETOUR
*************************************************
* SEQUENCE D'APPEL :
*/
extern int itre_rtw_processRMVI_retour(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
)
{
#ifndef _TIBCO_EMS
  T_STR vl_requete;
#else
  XDY_Nom vl_requete;
#endif
XDY_NomMachine vl_machine;

#ifndef _TIBCO_EMS
  T_STR *pl_id;
 T_STR *pl_type;
 T_INT4 *pl_tdp;
 T_INT4 *pl_taux;
 T_STR *pl_position;
 T_INT4 *pl_vitesse;
 T_INT4 *pl_dlp;
 T_INT4 *pl_note;
 T_INT4 vl_nb1, vl_nb2, vl_nb3, vl_nb4, vl_nb5, vl_nb6, vl_nb7, vl_nb8;
#else
 XDY_Texte pl_id[50];
 XDY_Texte pl_type[50];
 XDY_Entier pl_tdp[50];
 XDY_Entier pl_taux[50];
 XDY_Nom pl_position[50];
 XDY_Entier pl_vitesse[50];
 XDY_Entier pl_dlp[50];
 XDY_Entier pl_note[50];
 XDY_Entier vl_nb1, vl_nb2, vl_nb3, vl_nb4, vl_nb5, vl_nb6, vl_nb7, vl_nb8;
#endif
  int vl_indice;
  char  *tl_params[10];
  XDY_Nom	vl_st_id, vl_st_tdp, vl_st_taux, vl_st_position, vl_st_vitesse, vl_st_dlp, vl_st_note;
  XDY_Nom 	vl_st_indice;
 if (!TipcMsgRead(pa_data->msg,
 		T_IPC_FT_STR, &vl_requete,
		T_IPC_FT_STR_ARRAY, &pl_id, &vl_nb1,
		T_IPC_FT_STR_ARRAY, &pl_type, &vl_nb2,
		T_IPC_FT_INT4_ARRAY, &pl_tdp, &vl_nb3,
		T_IPC_FT_INT4_ARRAY, &pl_taux, &vl_nb4,
		T_IPC_FT_STR_ARRAY, &pl_position, &vl_nb5,
		T_IPC_FT_INT4_ARRAY, &pl_vitesse, &vl_nb6,
		T_IPC_FT_INT4_ARRAY, &pl_dlp, &vl_nb7,
		T_IPC_FT_INT4_ARRAY, &pl_note, &vl_nb8,
		NULL))
  {
 XZST_03EcritureTrace (XZSTC_WARNING, "itre_rtw_processRMVI_retour : pb lecture message");
 return (XDC_NOK);
  }

 XZST_03EcritureTrace (XZSTC_WARNING, "itre_rtw_processRMVI_retour : %d",vl_nb1);
  for (vl_indice=0; vl_indice < vl_nb1; vl_indice++)
  {
    sprintf(vl_st_id ,"%s",pl_id[vl_indice]);
    sprintf(vl_st_tdp ,"%d",pl_tdp[vl_indice]);
    sprintf(vl_st_taux ,"%d",pl_taux[vl_indice]);
    sprintf(vl_st_position ,"%s",pl_position[vl_indice]);
 XZST_03EcritureTrace (XZSTC_WARNING, "itre_rtw_processRMVI_retour : %s",vl_st_position);
    sprintf(vl_st_vitesse ,"%d",pl_vitesse[vl_indice]);
    sprintf(vl_st_dlp ,"%d",pl_dlp[vl_indice]);
    sprintf(vl_st_note ,"%d",pl_note[vl_indice]);
    sprintf(vl_st_indice,"%d",vl_indice);
    tl_params[0]=vl_requete;
    tl_params[1]=vl_st_id;
    tl_params[2]=pl_type[vl_indice];
    tl_params[3]=vl_st_tdp;
    tl_params[4]=vl_st_taux;
    tl_params[5]=vl_st_position;
    tl_params[6]=vl_st_vitesse;
    tl_params[7]=vl_st_dlp;
    tl_params[8]=vl_st_note;
    tl_params[9]=vl_st_indice;
    
    if (XZSC_GetDestinataire(pa_data, vl_machine)!=XDC_OK){
          XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processRMVI_retour : pb dans recuperation machine destinataire du message");
          return (XDC_NOK);
    }
    if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_RMVI_RETOUR,tl_params,10,vl_machine) != XDC_OK) 
      XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processRMVI_retour : pb dans l'appel a itre_com_SendPoke");
  }
 XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : itre_rtw_processRMVI_retour");
 return (XDC_OK);
}

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_RMVI_RETOUR
*************************************************
* SEQUENCE D'APPEL :
*/
extern int itre_rtw_processRMVI_EMS_retour(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
)
{
#ifndef _TIBCO_EMS
  T_STR vl_requete;
#else
  XDY_Nom vl_requete;
#endif
XDY_NomMachine vl_machine;

 XDY_Texte pl_id[50];
 XDY_Texte pl_type[50];
 XDY_Texte pl_position[50];
#ifndef _TIBCO_EMS
 T_INT4 *pl_tdp;
 T_INT4 *pl_taux;
 T_INT4 *pl_vitesse;
 T_INT4 *pl_dlp;
 T_INT4 *pl_note;
  T_STR		*vl_str_id;
  T_STR		*vl_str_type;
  T_STR		*vl_str_position;
 T_INT4 vl_nb1, vl_nb2, vl_nb3, vl_nb4, vl_nb5, vl_nb6, vl_nb7, vl_nb8;
#else
 XDY_Entier pl_tdp[50];
 XDY_Entier pl_taux[50];
 XDY_Entier pl_vitesse[50];
 XDY_Entier pl_dlp[50];
 XDY_Entier pl_note[50];
  char		vl_str_id[1024];
  char		vl_str_type[1024];
  char 		vl_str_position[1024];
 XDY_Entier vl_nb1, vl_nb2, vl_nb3, vl_nb4, vl_nb5, vl_nb6, vl_nb7, vl_nb8;
#endif
  int vl_indice;
  char  *tl_params[10];
  XDY_Nom	vl_st_id, vl_st_tdp, vl_st_taux, vl_st_position, vl_st_vitesse, vl_st_dlp, vl_st_note;
  XDY_Nom 	vl_st_indice;

  char		*token;

 XZST_03EcritureTrace (XZSTC_WARNING, "itre_rtw_processRMVI_EMS_retour : IN");
 if (!TipcMsgRead(pa_data->msg,
 		T_IPC_FT_STR, &vl_requete,
		T_IPC_FT_STR, &vl_str_id,
		T_IPC_FT_STR, &vl_str_type,
		T_IPC_FT_INT4_ARRAY, &pl_tdp, &vl_nb3,
		T_IPC_FT_INT4_ARRAY, &pl_taux, &vl_nb4,
		T_IPC_FT_STR, &vl_str_position,
		T_IPC_FT_INT4_ARRAY, &pl_vitesse, &vl_nb6,
		T_IPC_FT_INT4_ARRAY, &pl_dlp, &vl_nb7,
		T_IPC_FT_INT4_ARRAY, &pl_note, &vl_nb8,
		NULL))
  {
 XZST_03EcritureTrace (XZSTC_WARNING, "itre_rtw_processRMVI_EMS_retour : pb lecture message");
 return (XDC_NOK);
  }

  vl_indice=0;
  token=strtok((char*)vl_str_id,"|");
  while (token!=NULL)
  {
  	strcpy(pl_id[vl_indice++],token);
	token=strtok(NULL,"|");
  }
  vl_indice=0;
  token=strtok((char*)vl_str_type,"|");
  while (token!=NULL)
  {
  	strcpy(pl_type[vl_indice++],token);
	token=strtok(NULL,"|");
  }
  vl_indice=0;
  token=strtok((char*)vl_str_position,"|");
  while (token!=NULL)
  {
  	strcpy(pl_position[vl_indice++],token);
	token=strtok(NULL,"|");
  }

  for (vl_indice=0; vl_indice < vl_nb3; vl_indice++)
  {
    sprintf(vl_st_id ,"%s",pl_id[vl_indice]);
    sprintf(vl_st_tdp ,"%d",pl_tdp[vl_indice]);
    sprintf(vl_st_taux ,"%d",pl_taux[vl_indice]);
    sprintf(vl_st_position ,"%s",pl_position[vl_indice]);
 XZST_03EcritureTrace (XZSTC_WARNING, "itre_rtw_processRMVI_EMS_retour : %s",vl_st_position);
    sprintf(vl_st_vitesse ,"%d",pl_vitesse[vl_indice]);
    sprintf(vl_st_dlp ,"%d",pl_dlp[vl_indice]);
    sprintf(vl_st_note ,"%d",pl_note[vl_indice]);
    sprintf(vl_st_indice,"%d",vl_indice);
    tl_params[0]=vl_requete;
    tl_params[1]=vl_st_id;
    tl_params[2]=pl_type[vl_indice];
    tl_params[3]=vl_st_tdp;
    tl_params[4]=vl_st_taux;
    tl_params[5]=vl_st_position;
    tl_params[6]=vl_st_vitesse;
    tl_params[7]=vl_st_dlp;
    tl_params[8]=vl_st_note;
    tl_params[9]=vl_st_indice;
    
    if (XZSC_GetDestinataire(pa_data, vl_machine)!=XDC_OK){
          XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processRMVI_EMS_retour : pb dans recuperation machine destinataire du message");
          return (XDC_NOK);
    }
    if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_RMVI_RETOUR,tl_params,10,vl_machine) != XDC_OK) 
      XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processRMVI_EMS_retour : pb dans l'appel a itre_com_SendPoke");
  }
 XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : itre_rtw_processRMVI_EMS_retour");
 return (XDC_OK);
}


/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_AM_alarme
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_processAlarme(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
)

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
  XDY_Horodate 		vl_Horodate_double;	/* double 	*/
  XDY_TypeEqt		vl_TypeEqt;		/* unsigned char*/
  XDY_Eqt		vl_Eqt;			/* short	*/
  XDY_TypeAlarme	vl_TypeAlarme;		/* short*/
  XDY_Booleen		vl_Etat;		/* booleen  bit	*/

#ifndef _TIBCO_EMS
  T_STR			vl_NomMachine;
#else
  XDY_NomMachine	vl_NomMachine;
#endif
XDY_NomMachine	vl_machine;

XDY_Nom	vl_st_etat;
char 	*tl_params[3];

  XZST_03EcritureTrace (XZSTC_FONCTION, "IN : itre_rtw_processAlarme");

  /*recuperation des donnees du message*/
  if (TipcMsgRead(pa_data->msg,	T_IPC_FT_REAL8, &vl_Horodate_double, 
				T_IPC_FT_CHAR, &vl_TypeEqt,
				T_IPC_FT_INT2, &vl_Eqt,
				T_IPC_FT_INT2, &vl_TypeAlarme,
				T_IPC_FT_CHAR, &vl_Etat, 
				T_IPC_FT_STR, &vl_NomMachine,
				NULL))
  {
    XZST_03EcritureTrace(XZSTC_DEBUG2,
                            "itre_rtw_processAlarme: TypeEqt=%d Eqt=%d TypeAlarme=%d Etat=%d Machine=%s\n",
                            vl_TypeEqt, vl_Eqt, vl_TypeAlarme, vl_Etat, vl_NomMachine);
  }
  else
  {
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processAlarme: pb dans le decodage du message");
    return (XDC_NOK);
  }

  /* Si c'est un message d'etat du Pirau alors */
  if (vl_TypeEqt == XDC_EQT_RAU)
  {
    /* envoyer l'etat du Pirau cote synoptique */
    sprintf(vl_st_etat, "%d", vl_Etat);
    tl_params[0]=vl_st_etat;
    if (XZSC_GetDestinataire(pa_data, vl_machine)!=XDC_OK){
          XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processAlarme : pb dans recuperation machine destinataire du message");
          return (XDC_NOK);
    }

    if (itre_com_SendPoke(vg_socEnvToAxDial,COM_CANAL_MTMT_ETAT_PIRAU,tl_params,1,vl_machine) != XDC_OK) 
      XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processAlarme: pb dans l'appel a itre_com_SendPoke");
  }

  XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : itre_rtw_processAlarme");

  return (XDC_OK);
}




/*************************************************
* MODULE  : MTRE                                 *
**************************************************
* DESCRIPTION :
* Fonctions interne de gestion du timer TdP.
*
**************************************************
* SEQUENCE D'APPEL :
*/

/* Dates en secondes des reveils du timer Temps de Parcours */
#define		CM_TIMER_TDP_REVEIL1	(7 * 60)
#define		CM_TIMER_TDP_REVEIL2	(13 * 60)
#define		CM_TIMER_TDP_REVEIL3	(20 * 60)
#define		CM_TIMER_TDP_REVEIL4	(28 * 60)
#define		CM_TIMER_TDP_REVEIL5	(36 * 60)
#define		CM_TIMER_TDP_REVEIL6	(43 * 60)
#define		CM_TIMER_TDP_REVEIL7	(51 * 60)
#define		CM_TIMER_TDP_REVEIL8	(58 * 60)


static XDY_Booleen vm_Timer_TdP_actif = XDC_FAUX;

static int itre_programmer_Timer_TdP()
{
    XDY_Datagroup	vl_dg_timer_tdp;
    XZSMT_CleProg	vl_cle_timer_tdp;
    XDY_Horodate	vl_DateEnSeconde, vl_DateReveil;
    int			vl_JourSemaine;
    long		vl_Maintenant = 0;
    long		vl_heure = 0, vl_seconde = 0;

    XZST_03EcritureTrace (XZSTC_FONCTION, "IN : itre_programmer_Timer_TdP");

    /* Lire l'horodate actuelle et determiner celle du prochain declenchement de reveil */
    XZSM_07LireHorodate (XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_DateEnSeconde);

    /*! au demarrage, une marge de quelques secondes assure de ne pas la manquer */
    vl_DateEnSeconde = vl_DateEnSeconde + 5.0;

    vl_Maintenant = (long) vl_DateEnSeconde;
    vl_seconde = vl_Maintenant % 3600;
    vl_heure = vl_Maintenant - vl_seconde;

    if (vl_seconde < CM_TIMER_TDP_REVEIL1) {
	vl_DateReveil = (XDY_Horodate) (vl_heure + CM_TIMER_TDP_REVEIL1);
    } else if (vl_seconde < CM_TIMER_TDP_REVEIL2) {
	vl_DateReveil = (XDY_Horodate) (vl_heure + CM_TIMER_TDP_REVEIL2);
    } else if (vl_seconde < CM_TIMER_TDP_REVEIL3) {
	vl_DateReveil = (XDY_Horodate) (vl_heure + CM_TIMER_TDP_REVEIL3);
    } else if (vl_seconde < CM_TIMER_TDP_REVEIL4) {
	vl_DateReveil = (XDY_Horodate) (vl_heure + CM_TIMER_TDP_REVEIL4);
    } else if (vl_seconde < CM_TIMER_TDP_REVEIL5) {
	vl_DateReveil = (XDY_Horodate) (vl_heure + CM_TIMER_TDP_REVEIL5);
    } else if (vl_seconde < CM_TIMER_TDP_REVEIL6) {
	vl_DateReveil = (XDY_Horodate) (vl_heure + CM_TIMER_TDP_REVEIL6);
    } else if (vl_seconde < CM_TIMER_TDP_REVEIL7) {
	vl_DateReveil = (XDY_Horodate) (vl_heure + CM_TIMER_TDP_REVEIL7);
    } else if (vl_seconde < CM_TIMER_TDP_REVEIL8) {
	vl_DateReveil = (XDY_Horodate) (vl_heure + CM_TIMER_TDP_REVEIL8);
    } else {	/* l'heure est presque atteinte */
	vl_DateReveil = (XDY_Horodate) (vl_heure + 3600 + CM_TIMER_TDP_REVEIL1);
    }

    /* Programmer le declenchement du reveil a l'horodate voulue */
    XDG_EncodeDG2 (vl_dg_timer_tdp, XDG_ITDP_TIMER, vg_nomMachine);
    sprintf (vl_cle_timer_tdp, "%s_%s", XDG_ITDP_TIMER, vg_nomMachine);
    if (XZSM_01DemanderProgrammation (vl_DateReveil,
                                      0,
                                      vl_dg_timer_tdp,
                                      XDM_ITDP_TIMER,
                                      vl_cle_timer_tdp,
                                      2) != XDC_OK)
    {
	XZST_03EcritureTrace(XZSTC_WARNING,"Timer TdP sur %d non enclenche", XDM_ITDP_TIMER);
    }
    else
    {
	XZST_03EcritureTrace(XZSTC_DEBUG2,"Timer TdP programme a %lf", vl_DateReveil);
    }

    XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : itre_programmer_Timer_TdP");

    return (XDC_OK);
}


/*************************************************
* DESCRIPTION :
* Active / desactive la fonction de timer TdP.
*
**************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_Timer_TdP_activer(int va_en_fonction)
{
    XDY_Booleen		vl_etat_actuel_Timer_TdP = vm_Timer_TdP_actif;
    XDY_Datagroup	vl_dg_timer_tdp;

    XZST_03EcritureTrace (XZSTC_INFO, "itre_Timer_TdP_activer: %d", va_en_fonction);

    /* Definir le nouvel etat du timer TdP */
    if (va_en_fonction == XDC_FAUX) {
	vm_Timer_TdP_actif = XDC_FAUX;
    } else {
	vm_Timer_TdP_actif = XDC_VRAI;
    }

    /* si le timer DEVIENT actif, programmer le prochain reveil */
    if ( (vl_etat_actuel_Timer_TdP == XDC_FAUX)  &&  (vm_Timer_TdP_actif == XDC_VRAI) )
    {
	itre_programmer_Timer_TdP ();
    }
    /* si le timer devient inactif, annuler la programmation du reveil */
    if ( /*(vl_etat_actuel_Timer_TdP == XDC_VRAI)  &&*/  (vm_Timer_TdP_actif == XDC_FAUX) )
    {
	XDG_EncodeDG2 (vl_dg_timer_tdp, XDG_ITDP_TIMER, vg_nomMachine);
	XZSM_02AnnulerProgrammation (XZSMC_CLEPROG_NUL, vl_dg_timer_tdp, 0);
    }

    return (XDC_OK);
}


/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_ITDP_TIMER
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_process_Timer_TdP(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
)

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
    char                  *tl_params[1];
    char                  vl_str[300]="tdprva";
    XDY_NomMachine        vl_machine;

    XZST_03EcritureTrace (XZSTC_FONCTION, "IN : itre_rtw_process_Timer_TdP");

    /* le message concretise un top du timer, sans donnees; */
    /* si le timer est toujours actif, propager le signal a l'IHM cote main courante */
    if (vm_Timer_TdP_actif == XDC_VRAI)
    {   
	if (XZSC_GetDestinataire(pa_data, vl_machine)!=XDC_OK){
	      XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_process_Timer_TdP : pb dans recuperation machine destinataire du message");
	      return (XDC_NOK);
	}

	tl_params[0]=vl_str; 
	if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTDP_TIMER,tl_params,1,vl_machine) != XDC_OK)
	{
	    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_process_Timer_TdP: pb dans l'appel a itre_com_SendPoke");
	}
        else XZST_03EcritureTrace(XZSTC_INFO,"itre_rtw_process_Timer_TdP: TOP TIMER TDP");

	/* puis le reprogrammer a la date du prochain reveil */
	itre_programmer_Timer_TdP ();
    }

    XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : itre_rtw_process_Timer_TdP");

    return (XDC_OK);
}


/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_process_Timer_KEEPALIVE(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
)

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
    char                  *tl_params[1]={""};
    XDY_NomMachine        vl_machine;

    XZST_03EcritureTrace (XZSTC_FONCTION, "IN : itre_rtw_process_Timer_KEEPALIVE");

	if (XZSC_GetDestinataire(pa_data, vl_machine)!=XDC_OK){
	      XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_process_Timer_KEEPALIVE : pb dans recuperation machine destinataire du message");
	      return (XDC_NOK);
	}
	if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_KEEP_ALIVE_TIMER,tl_params,1,"") != XDC_OK)
	{
	    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_process_Timer_KEEPALIVE: pb dans l'appel a itre_com_SendPoke");
	}
        else XZST_03EcritureTrace(XZSTC_INFO,"itre_rtw_process_Timer_KEEPALIVE: TOP TIMER ");

	if (itre_com_SendPoke(vg_socEnvToAxDial,COM_CANAL_KEEP_ALIVE_TIMER,tl_params,1,vl_machine) != XDC_OK)
	{
	    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_process_Timer_KEEPALIVE: pb dans l'appel a itre_com_SendPoke");
	}
        else XZST_03EcritureTrace(XZSTC_INFO,"itre_rtw_process_Timer_KEEPALIVE: TOP TIMER ");



    XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : itre_rtw_process_Timer_KEEPALIVE");

    return (XDC_OK);
}





/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* fonction utilisateur passe a
* XZAE103_Abt_Presence_Op_PCsimpl
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_presence_PCS(
XZAET_EPresence_Op_PCsimpl      va_presence
)

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
  char	vl_variable[30];
  char	vl_valeur[5];
  char 	*tl_params[2];
  XDY_NomMachine        vl_machine;

  XZST_03EcritureTrace(XZSTC_FONCTION,"IN: itre_rtw_presence_PCS");

  /*trace*/
  XZST_03EcritureTrace(XZSTC_DEBUG1,"itre_rtw_presence_PCS: presence %d, site %s",
			va_presence.Presence,
			va_presence.NomSite);

  /*fabrique le nom de la variable a envoyer a Applix*/
  sprintf(vl_variable,"vg_operateur_%s",va_presence.NomSite);
  sprintf(vl_valeur,"%d",va_presence.Presence);
  tl_params[0]=vl_variable;
  tl_params[1]=vl_valeur;

  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_MTMT_ETAT_PCSIMPL,tl_params,2,"") != XDC_OK) 
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_presence_PCS: pb dans l'appel a itre_com_SendPoke");
  if (itre_com_SendPoke(vg_socEnvToAxDial,COM_CANAL_MTMT_ETAT_PCSIMPL,tl_params,2,"") != XDC_OK)
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_presence_PCS: pb dans l'appel a itre_com_SendPoke");

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT itre_rtw_presence_PCS");

}



/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_IAX_ACQT
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_processTDP(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg,
int va_canal_IHM
)

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{ 
#ifdef _TIBCO_EMS
  XDY_Entier	vl_courant, vl_surcharge;
  XDY_Mot	vl_numero;
#else
  T_INT4	vl_courant, vl_surcharge;
  T_INT2	vl_numero;
#endif
  char 	*tl_params[3];
  char vl_cmd[50];
  XZST_03EcritureTrace (XZSTC_FONCTION, "IN : itre_rtw_processTDP");
  XDY_NomMachine        vl_machine;

  /*recuperation des donnees du message*/
  if (!TipcMsgRead(pa_data->msg,
			T_IPC_FT_INT2, &vl_numero,
			T_IPC_FT_INT4, &vl_courant,
			T_IPC_FT_INT4, &vl_surcharge,
			NULL)) {
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processTDP: pb dans le decodage du message");
    return (XDC_NOK);
  }
  /*on fait un info message*/
  sprintf(vl_cmd,"%d,%d,%d",vl_numero,vl_courant,vl_surcharge);
  tl_params[0]=vl_cmd;

  if (XZSC_GetDestinataire(pa_data, vl_machine)!=XDC_OK){
        XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processTDP : pb dans recuperation machine destinataire du message");
        return (XDC_NOK);
  }
  if (itre_com_SendPoke(vg_socEnvToAxMce,COM_CANAL_TDP_SURCHARGE,tl_params,1,vl_machine) != XDC_OK) 
    XZST_03EcritureTrace(XZSTC_WARNING,"itre_rtw_processTDP: pb dans l'appel a itre_com_SendPoke");


  XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : itre_rtw_processTDP");

  return (XDC_OK);
}

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* appel suppression semapgore sur message magistra
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_process_del_sema(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
)
/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
  char vl_str[255];
  char vl_cmd[255];  
  char vl_nom_semaphore[255];
  int vl_num_equip;
  int vl_num_type_alarme;
  FILE *vl_commande=NULL;
  char  vl_errorBuffer [ERRBUFSIZE+1] = "";
  #ifndef _TIBCO_EMS
    T_STR vl_poste_maitre;
    T_STR vl_nom_tache_equip;
  #else
    char  vl_poste_maitre[200];
    char  vl_nom_tache_equip[200];
  #endif
  if (!TipcMsgRead(pa_data->msg, 
		T_IPC_FT_STR,&vl_poste_maitre,
		T_IPC_FT_STR,&vl_nom_tache_equip,
    T_IPC_FT_INT4,&vl_num_equip,
    T_IPC_FT_INT4,&vl_num_type_alarme,
    NULL))
  {
    XZST_03EcritureTrace (XZSTC_WARNING, "pb lecture message dans itre_rtw_process_del_sema");
    return;
  }

  sprintf(vl_nom_semaphore,"Ala_%s_%s_%03d_%03d",vl_poste_maitre,vl_nom_tache_equip,vl_num_equip,vl_num_type_alarme);
  sprintf(vl_cmd,"/bin/rm -f %s/ala/%s",XDC_PATHFIC,vl_nom_semaphore);
  sprintf(vl_str,"%s %s \"%s\"",CM_CMD_DEL_SEMA,vl_poste_maitre,vl_cmd);
  XZST_03EcritureTrace (XZSTC_WARNING,"delete semaphore cmd=%s",vl_str);
  //system(vl_str);
  vl_commande = popen(vl_str,"r");
  if ( vl_commande != NULL) 
  {
    fgets(vl_errorBuffer,ERRBUFSIZE,vl_commande);
    XDY_Supprimer_Espaces_De_Fin(vl_errorBuffer);

    if(strlen(vl_errorBuffer)!=0)
    {
      XZST_03EcritureTrace (XZSTC_WARNING,"%s",vl_errorBuffer);
    }
  }
  return;
}

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* applique le pra sur le po sur message magistra
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_process_pra_po(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
)
/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
    int	vl_ValRet=-1; /* compte-rendu d'appel */
    int vl_pcloseReturnCode=0;
    char vl_cmd[255];
    FILE *vl_commande=NULL;
    char  vl_errorBuffer [ERRBUFSIZE+1] = "";
  #ifndef _TIBCO_EMS
    T_STR vl_site;
    T_STR vl_poste_perateur;
  #else
    char  vl_poste_perateur[200];
    char  vl_site[200];
  #endif

  XDY_Datagroup vl_dg;
  XDY_NomSite vl_nomSite;
  if((vl_ValRet = XZSC_04NomSite(vl_nomSite)) != XDC_OK)
  {
      XZST_03EcritureTrace (XZSTC_WARNING, "pb encodage XDG_MGA_REMOTE_CMD");
      return;
  }
  XDG_EncodeDG2(vl_dg, XDG_MGA_REMOTE_CMD, vl_nomSite);

  if (!TipcMsgRead(pa_data->msg, 
		T_IPC_FT_STR, &vl_poste_perateur,
		T_IPC_FT_STR, &vl_site,
    NULL))
  {
    XZST_03EcritureTrace (XZSTC_WARNING, "pb lecture message dans itre_rtw_process_pra_po");
    return;
  }
  sprintf(vl_cmd,"xzsx_cmd_sur.sh %s \"/produits/migrazur/appliSD/exec/sae_mco_cht_site -k %s\" 2>&1", vl_poste_perateur, vl_site);
  XZST_03EcritureTrace (XZSTC_WARNING, "PRA PO cmd:%s",vl_cmd);
    //vl_ValRet=system(vl_cmd);
  vl_commande = popen(vl_cmd,"r");
  if ( vl_commande != NULL) 
  {  

    fgets(vl_errorBuffer,ERRBUFSIZE,vl_commande);
    XDY_Supprimer_Espaces_De_Fin(vl_errorBuffer);

    if(!strcmp(vl_errorBuffer,""))
    {
        vl_ValRet = 0;
    }
    else
    {
      XZST_03EcritureTrace (XZSTC_WARNING,"%s",vl_errorBuffer);
      vl_ValRet = -1;
    }
  }
  else 
    vl_ValRet = -1;

  vl_pcloseReturnCode=pclose(vl_commande);

  TipcSrvMsgWrite(vl_dg,
    XDM_IdentMsg(XDM_MGA_CMD_RETOUR),
    XDM_FLG_SRVMSGWRITE,
    T_IPC_FT_INT4, XDM_MGA_CMD_CFG_PRA_PO,
    T_IPC_FT_STR, vl_poste_perateur,
    T_IPC_FT_INT4, vl_ValRet,
    NULL);

}


/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* appel configuration modern sur message magistra
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_process_conf_modern(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
)
/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
  FILE *vl_commande=NULL;
  char  vl_errorBuffer [ERRBUFSIZE+1] = "";
  XZST_03EcritureTrace (XZSTC_WARNING, "Appel conf modern");
  //system(CM_CMD_LIVRE_MODERN);
  vl_commande = popen(CM_CMD_LIVRE_MODERN,"r");
  if ( vl_commande != NULL) 
  {
    fgets(vl_errorBuffer,ERRBUFSIZE,vl_commande);
    XDY_Supprimer_Espaces_De_Fin(vl_errorBuffer);

    if(strlen(vl_errorBuffer)!=0)
    {
      XZST_03EcritureTrace (XZSTC_WARNING,"%s",vl_errorBuffer);
    }
  }
}

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* appel fonctions remorque mobile sur message magistra
*************************************************
* SEQUENCE D'APPEL :
*/
extern void itre_rtw_process_fct_remorque(
  T_IPC_CONN va_cnx,
  T_IPC_CONN_PROCESS_CB_DATA pa_data,
  T_CB_ARG pa_arg
)
/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
  char vl_cmd[255];
  FILE *vl_commande=NULL;
  char  vl_errorBuffer [ERRBUFSIZE+1] = "";
  #ifndef _TIBCO_EMS
    T_STR vl_portdisplay;
  #else
    char  vl_portdisplay[200];
  #endif

  if (!TipcMsgRead(pa_data->msg, 
		T_IPC_FT_STR,&vl_portdisplay,
    NULL))
  {
    XZST_03EcritureTrace (XZSTC_WARNING, "pb lecture message dans itre_rtw_process_fct_remorque");
    return;
  }

  sprintf(vl_cmd,"/produits/migrazur/appliSD/fichiers/util/remorque-loc.sh %s", vl_portdisplay);
  XZST_03EcritureTrace (XZSTC_WARNING, "Appel fonction remorque cmd=%s",vl_cmd);
  /* le path du script PMV sur remorque, qui appel un JAR sur deppuis la machine POCDDP1*/
  //system(vl_cmd);
  vl_commande = popen(vl_cmd,"r");
  if ( vl_commande != NULL) 
  {
    fgets(vl_errorBuffer,ERRBUFSIZE,vl_commande);
    XDY_Supprimer_Espaces_De_Fin(vl_errorBuffer);

    if(strlen(vl_errorBuffer)!=0)
    {
      XZST_03EcritureTrace (XZSTC_WARNING,"%s",vl_errorBuffer);
    }
  }
}


/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* appel fonctions remorque CA mobile sur message magistra
*************************************************
* SEQUENCE D'APPEL :
*/
extern void itre_rtw_process_fct_remorque_CA(
  T_IPC_CONN va_cnx,
  T_IPC_CONN_PROCESS_CB_DATA pa_data,
  T_CB_ARG pa_arg
)
/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
  char vl_cmd[255];
  FILE *vl_commande=NULL;
  char  vl_errorBuffer [ERRBUFSIZE+1] = "";
  #ifndef _TIBCO_EMS
    T_STR vl_portdisplay;
  #else
    char  vl_portdisplay[200];
  #endif

  if (!TipcMsgRead(pa_data->msg, 
		T_IPC_FT_STR,&vl_portdisplay,
    NULL))
  {
    XZST_03EcritureTrace (XZSTC_WARNING, "pb lecture message dans itre_rtw_process_fct_remorque_CA");
    return;
  }

  sprintf(vl_cmd,"/produits/migrazur/appliSD/fichiers/util/remorque-loc-CA.sh %s", vl_portdisplay);
  XZST_03EcritureTrace (XZSTC_WARNING, "Appel fonction remorque CA cmd=%s",vl_cmd);
  /* le path du script PMV sur remorque, qui appel un JAR sur deppuis la machine POCDDP1 */
  //system(vl_cmd);
  vl_commande = popen(vl_cmd,"r");
  if ( vl_commande != NULL) 
  {
    fgets(vl_errorBuffer,ERRBUFSIZE,vl_commande);
    XDY_Supprimer_Espaces_De_Fin(vl_errorBuffer);

    if(strlen(vl_errorBuffer)!=0)
    {
      XZST_03EcritureTrace (XZSTC_WARNING,"%s",vl_errorBuffer);
    }
  }
}

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* appel trasnfert viabilite hivernal
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_process_viabilite_hivernal(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
)
/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
    int	vl_ValRet=-1; /* compte-rendu d'appel */
    int vl_pcloseReturnCode=0;
    char vl_cmd[255];
    int vl_num_action;
    int vl_num_site;
    FILE *vl_commande=NULL;
    char  vl_errorBuffer [ERRBUFSIZE+1] = "";
  #ifndef _TIBCO_EMS
    T_STR vl_poste_perateur;
  #else
    char vl_poste_perateur[255];
  #endif
  XDY_Datagroup vl_dg;
  XDY_NomSite vl_nomSite;

  if((vl_ValRet = XZSC_04NomSite(vl_nomSite)) != XDC_OK)
  {
      XZST_03EcritureTrace (XZSTC_WARNING, "pb encodage XDG_MGA_REMOTE_CMD");
      return;
  }
  XDG_EncodeDG2(vl_dg, XDG_MGA_REMOTE_CMD, vl_nomSite);

  if (!TipcMsgRead(pa_data->msg, 
		T_IPC_FT_STR, &vl_poste_perateur,
    T_IPC_FT_INT4, &vl_num_action,
    T_IPC_FT_INT4, &vl_num_site,
    NULL))
  {
    XZST_03EcritureTrace (XZSTC_WARNING, "pb lecture message dans itre_rtw_process_viabilite_hivernal");
    return;
  }
  /* stderr est redirigée dans stdout au cas ou le mot erreur ne se trouve que dans stderr*/
  sprintf(vl_cmd,"%s %d %d 2>&1",CM_CMD_ENVOI_VH, vl_num_action, vl_num_site);
  XZST_03EcritureTrace (XZSTC_WARNING, "Appel fonction viabilite hivernale vl_cmd=%s",vl_cmd);
  //vl_ValRet=system(vl_cmd);
  vl_commande = popen(vl_cmd,"r");
  if ( vl_commande != NULL) 
  {   

    fgets(vl_errorBuffer,ERRBUFSIZE,vl_commande);
    XDY_Supprimer_Espaces_De_Fin(vl_errorBuffer);

    if(!strcmp(vl_errorBuffer,""))
    {
        vl_ValRet = 0;
    }
    else
    {
      XZST_03EcritureTrace (XZSTC_WARNING,"%s",vl_errorBuffer);
      vl_ValRet = -1;
    }
  }
  else 
    vl_ValRet = -1;

  if (WIFEXITED(vl_pcloseReturnCode))
    XZST_03EcritureTrace (XZSTC_INFO,"retour pclose = %d",WEXITSTATUS(vl_pcloseReturnCode));
  else
    XZST_03EcritureTrace (XZSTC_INFO,"retour pclose = OK");

  vl_pcloseReturnCode=pclose(vl_commande);
  
  TipcSrvMsgWrite(vl_dg,
  XDM_IdentMsg(XDM_MGA_CMD_RETOUR),
  XDM_FLG_SRVMSGWRITE,
  T_IPC_FT_INT4, XDM_MGA_CMD_HIVERNAGE,
  T_IPC_FT_STR, vl_poste_perateur,
  T_IPC_FT_INT4, vl_ValRet,
  NULL);
}


/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* appel trasnfert viabilite hivernal ASF
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_process_viabilite_hivernal_ASF(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
)
/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
    int	vl_ValRet=-1; /* compte-rendu d'appel */
    int vl_pcloseReturnCode=0;
    char vl_cmd[255];
    int vl_num_action;
    int vl_num_site;
    FILE *vl_commande=NULL;
    char  vl_errorBuffer [ERRBUFSIZE+1] = "";
  #ifndef _TIBCO_EMS
    T_STR vl_poste_perateur;
    T_STR vl_path_fichier;
  #else
    char vl_poste_perateur[255];
    char vl_path_fichier[255];
  #endif
  XDY_Datagroup vl_dg;
  XDY_NomSite vl_nomSite;
  
  if((vl_ValRet = XZSC_04NomSite(vl_nomSite)) != XDC_OK)
  {
      XZST_03EcritureTrace (XZSTC_WARNING, "pb encodage XDG_MGA_REMOTE_CMD");
      return;
  }
  XDG_EncodeDG2(vl_dg, XDG_MGA_REMOTE_CMD, vl_nomSite);

  if (!TipcMsgRead(pa_data->msg, 
		T_IPC_FT_STR, &vl_poste_perateur,
		T_IPC_FT_STR, &vl_path_fichier,
    NULL))
  {
    XZST_03EcritureTrace (XZSTC_WARNING, "pb lecture message dans itre_rtw_process_viabilite_hivernal_ASF");
    return;
  }
  sprintf(vl_cmd,"%s %s 2>&1",CM_CMD_ENVOI_VH_ASF, vl_path_fichier);
  XZST_03EcritureTrace (XZSTC_WARNING, "Appel fonction viabilite hivernale ASF vl_cmd=%s",vl_cmd);
  //vl_ValRet=system(vl_cmd);
  vl_commande = popen(vl_cmd,"r");
  if ( vl_commande != NULL) 
  {   

    fgets(vl_errorBuffer,ERRBUFSIZE,vl_commande);
    XDY_Supprimer_Espaces_De_Fin(vl_errorBuffer);

    if(!strcmp(vl_errorBuffer,""))
    {
        vl_ValRet = 0;
    }
    else
    {
      XZST_03EcritureTrace (XZSTC_WARNING,"%s",vl_errorBuffer);
      vl_ValRet = -1;
    }
  }
  else 
    vl_ValRet = -1;

  vl_pcloseReturnCode=pclose(vl_commande);

  TipcSrvMsgWrite(vl_dg,
  XDM_IdentMsg(XDM_MGA_CMD_RETOUR),
  XDM_FLG_SRVMSGWRITE,
  T_IPC_FT_INT4, XDM_MGA_CMD_HIVERNAGE_ASF,
  T_IPC_FT_STR, vl_poste_perateur,
  T_IPC_FT_INT4, vl_ValRet,
  NULL);
}

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* appel viabilite hivernal pour transfert ftp sur message magistra
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_process_viabilite_hivernal_export(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
)
/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
    int	vl_ValRet=-1; /* compte-rendu d'appel */
    int vl_pcloseReturnCode=0;
    char vl_cmd[255];
    int vl_num_action;
    int vl_num_site;
    FILE *vl_commande=NULL;
    char  vl_errorBuffer [ERRBUFSIZE+1] = "";
  #ifndef _TIBCO_EMS
    T_STR vl_poste_perateur;
    T_STR vl_path_fichier;
  #else
    char vl_poste_perateur[255];
    char vl_path_fichier[255];
  #endif
  XDY_Datagroup vl_dg;
  XDY_NomSite vl_nomSite;
  
  if((vl_ValRet = XZSC_04NomSite(vl_nomSite)) != XDC_OK)
  {
      XZST_03EcritureTrace (XZSTC_WARNING, "pb encodage XDG_MGA_REMOTE_CMD");
      return;
  }
  XDG_EncodeDG2(vl_dg, XDG_MGA_REMOTE_CMD, vl_nomSite);

  if (!TipcMsgRead(pa_data->msg, 
		T_IPC_FT_STR, &vl_poste_perateur,
		T_IPC_FT_STR, &vl_path_fichier,
    NULL))
  {
    XZST_03EcritureTrace (XZSTC_WARNING, "pb lecture message dans itre_rtw_process_viabilite_hivernal_export");
    return;
  }
  sprintf(vl_cmd,"%s %s 2>&1",CM_CMD_EXPORT_VH, vl_path_fichier);
  XZST_03EcritureTrace (XZSTC_WARNING, "Appel fonction viabilite hivernale export vl_cmd=%s",vl_cmd);
  //vl_ValRet=system(vl_cmd);
  vl_commande = popen(vl_cmd,"r");
  if ( vl_commande != NULL) 
  {   

    fgets(vl_errorBuffer,ERRBUFSIZE,vl_commande);
    XDY_Supprimer_Espaces_De_Fin(vl_errorBuffer);

    if(!strcmp(vl_errorBuffer,""))
    {
        vl_ValRet = 0;
    }
    else
    {
      XZST_03EcritureTrace (XZSTC_WARNING,"%s",vl_errorBuffer);
      vl_ValRet = -1;
    }
  }
  else 
    vl_ValRet = -1;
    
    vl_pcloseReturnCode=pclose(vl_commande);

  TipcSrvMsgWrite(vl_dg,
  XDM_IdentMsg(XDM_MGA_CMD_RETOUR),
  XDM_FLG_SRVMSGWRITE,
  T_IPC_FT_INT4, XDM_MGA_CMD_HIVERNAGE_EXPORT,
  T_IPC_FT_STR, vl_poste_perateur,
  T_IPC_FT_INT4, vl_ValRet,
  NULL);
}



/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* appel lt alarmes sur message magistra
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_process_lt_alarme(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
)
/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/
{
  int	vl_ValRet=-1; /* compte-rendu d'appel */
  int vl_pcloseReturnCode=0;
  char vl_cmd[255];
  FILE *vl_rep_alarme;
  FILE *vl_commande=NULL;
  char  vl_errorBuffer [ERRBUFSIZE+1] = "";
  #ifndef _TIBCO_EMS
    T_STR vl_NomMachine;
    T_STR vl_poste_perateur;
  #else
    char  vl_NomMachine[200];
    char  vl_poste_perateur[200];
  #endif

  XDY_Datagroup vl_dg;
  XDY_NomSite vl_nomSite;
  
  if((vl_ValRet = XZSC_04NomSite(vl_nomSite)) != XDC_OK)
  {
      XZST_03EcritureTrace (XZSTC_WARNING, "pb encodage XDG_MGA_REMOTE_CMD");
      return;
  }
  XDG_EncodeDG2(vl_dg, XDG_MGA_REMOTE_CMD, vl_nomSite);

  if (!TipcMsgRead(pa_data->msg, 
		T_IPC_FT_STR,&vl_poste_perateur,
		T_IPC_FT_STR,&vl_NomMachine,
    NULL))
  {
    XZST_03EcritureTrace (XZSTC_WARNING, "pb lecture message dans itre_rtw_process_lt_alarme");
    return;
  }
    
  sprintf(vl_cmd,"rcp PODFA:/users/gesconf/migrazur/fichiers/deq/alarme.txt %sdeq/",REP_FICHIER_MAGISTRA);
  XZST_03EcritureTrace (XZSTC_WARNING, "Appel alarme LT copie alarme.txt vl_cmd=%s",vl_cmd);
  system(vl_cmd);

  if (vl_rep_alarme = fopen(REP_ALARME, "r")) 
  {
    fclose(vl_rep_alarme);
  }
  else
  {
    sprintf(vl_cmd,"mkdir %s",REP_ALARME);
    system(vl_cmd);
  }

  sprintf(vl_cmd,"rcp %s:/produits/migrazur/appliSD/fichiers/ala/Alarmes_%s %sala/ 2>&1",vl_NomMachine,vl_NomMachine,REP_FICHIER_MAGISTRA);
  XZST_03EcritureTrace (XZSTC_WARNING, "Appel alarme LT copie fichier Alarmes vl_cmd=%s",vl_cmd);
  //vl_ValRet=system(vl_cmd);
  vl_commande = popen(vl_cmd,"r");
  if ( vl_commande != NULL) 
  {   

    fgets(vl_errorBuffer,ERRBUFSIZE,vl_commande);
    XDY_Supprimer_Espaces_De_Fin(vl_errorBuffer);

    if(!strcmp(vl_errorBuffer,""))
    {
        vl_ValRet = 0;
    }
    else
    {
      XZST_03EcritureTrace (XZSTC_WARNING,"%s",vl_errorBuffer);
      vl_ValRet = -1;
    }
  }
  else 
    vl_ValRet = -1;
    
  vl_pcloseReturnCode=pclose(vl_commande);

  TipcSrvMsgWrite(vl_dg,
  XDM_IdentMsg(XDM_MGA_CMD_RETOUR),
  XDM_FLG_SRVMSGWRITE,
  T_IPC_FT_INT4, XDM_MGA_CMD_ALARME_LT,
  T_IPC_FT_STR, vl_poste_perateur,
  T_IPC_FT_INT4, vl_ValRet,
  NULL);
}
