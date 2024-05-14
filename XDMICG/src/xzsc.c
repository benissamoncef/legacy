/*Fichier : $Id: xzsc.c,v 1.42 2021/05/04 13:28:02 pc2dpdy Exp $      Release : $Revision: 1.42 $        Date : $Date: 2021/05/04 13:28:02 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI * FICHIER xzsc.c
 ******************************************************
 * DESCRIPTION DU MODULE :
 *
 *   Description du module d'interface XZSCFINI
 *        - Declaration des fonctions externes
 *        - Declaration des variables externes
 *
 *   Reference DCG-021 chapitre 3.3
 *
 ******************************************************
 * HISTORIQUE :
 *
 *  T.Milleville  30 Aou 1994      : Creation
 *
 *  T.Milleville  09 Sep 1994 	  : Mod 1.0 -- Modification instruction
 *					TipcSrvCreate pour la nouvelle
 *					verson 3.0 de RTWorks .Ajout
 *					argument T_IPC_SRV_CONN_FULL
 *
 *  T.Milleville   25 Oct  1994 	  : Ajout de la fonction
 *					XZSC_17DureeAbsenceCourte  V 1.14
 *  T.Milleville   8 Fev  1995 	  : Ajout de TipcMsgDestroy apres
 *					chaque utilisation de TipcSrvMsgProcess
 *					V 1.15
 *  T.Milleville   10 Mar  1995 	  : Ajout de l'argument Nom du Site dans
 *	l'appel de la fonction XZSM_06... V1.16
 *  T.Milleville   21 Avr 95 	  : Ajout dans le module d'interface
 *	l'appel a la fonction XZSC_EnvoieSurveillance   V1.17
 *  T.Milleville   3 Mai 95 	  : Ajout de l'abonnement au datagroup
 *		XDC_NomMachine dans la fonction XZSC_01   V1.17
 *  T.Milleville   3 Mai 95 	  : Ajout de la callback sur reception
 *	du msh XDM_TSP_MIGRAZUR   V1.17
 *  T.Milleville   6 Juin  95 	  : Modification des parametres de Tmeout
 *	lors de la connexiion V1.18
 *  T.Milleville   9 Juin  95 	  : Ajout option Client_Connect_Timeout
 *		dans les parametres de connexion RTclient V1.19
 *  T.Milleville   19 Juin  95 	  : Ajout lecture des options RTworks
 *	pour un Client par fichiers V1.19
 *  T.Milleville   4 Aou   95 	  : Modification option de lancement du
 *	rtserver. Remplacement de start_always par start_on_demand
 *	et ajout de l'option server_start_max_tries a 2 si la tache
 *	est tsaplg.x  V1.20
 *  T.Milleville   28 Aou   95 	  : Modification  valeur tempo pour attente
 *	de messages V1.21
 *  T.Milleville   27 Sep   95 	  : Envoi du message TSE_STARTTMP sur le datagroup
 *	NSUPETAT_NomMachine et NSUPETAT_NomSite.Ajout d'un argument pour preciser
 *	le nom du Site dans la fonction XZSC_EnvoieSurveillance V1.25
 *  T.Milleville   12 Oct   95 	  : Ajout de traces V1.26
 *  P.Niepceron	  13 Dec 1995	  : Ajout option start_never dans declaration
 *	liste serveur de license 				v1.27
 *  P.Niepceron    22 Aou 1996	: Ajout type PCS v1.28
 *  P.Niepceron    08 Jan 1997	: Ajout type PC2 et PC2LT v1.29
 * JMG  8 oct 2007 : ajout XZSC22_NumeroSiteLocal v1.30
 * JMG  22/11/07  : suppression traces dans XZSC22_NumeroSiteLocal v1.31
 * JMG  29/09/09  : SECTO   DEM 887
 * JMG  26/06/10  : linux DEM/934
 * JPL  08/12/10  : Migration architecture HP ia64 (DEM 961) : suppression warnings  1.34
 * JPL  08/12/10  : Migration architecture HP ia64 (DEM 961) : suppr. '==' au lieu de '=' (!!! effet de bord !!!)  1.35
 * JPL  08/12/10  : Migration architecture HP ia64 (DEM 961) : Utiliser le type message obtenu par TipcMsgGetType  1.36
 * JMG  09/03/17  : REGIONALISATION - DEM1220 1.37
 * JPL  06/02/18  : Migration Linux 64 bits (DEM 1274) : Types des arguments des fonctions socket  1.38
 * RGR  26/03/19  : Mecanisme Keep Alive
 * RGR  24/05/19  : Passage trace XZSC_process_KEEPALIVE_cb a INFO au lieu de WARNING
 * JMg  07/03/20  : EMS 1.41
 * LCL  20/12/21  : MOVIS P2 ems SAE-312
 * LCL  19/09/22  : Fix probleme refresh RVQ pour noms topics ne se terminant pas par le nom du destinataire (IMODIF_TFM, ETDP_FCD, IHM_ADM, etc. par exemple)  JIRA-523
 * GGY  22/03/23  : Ajout fonction XZSC_AdresseIP, qui recupere l'adresse locale (10.xxx.xxx.xxx)
 * LCL	27/09/23	: Adaptations CNA DEM-483
*******************************************************/

/* fichiers inclus */

#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <rtworks/common.h>
#include <rtworks/ipc.h>

#include <xdc.h>
#include <xdg.h>
#include <xdm.h>
#include <xdy.h>
#include <xdf.h>

#include <errno.h>

#include <xzsa.h> 
#include <xzsc.h> 
#include <xzst.h>
#include <xzsm.h> 
#include <xzss.h> 
#include "xzsx.h"


/* declaration des constantes locales */
#define XZSCC_TEMPO_ALARME 90    /* duree d'attente de l'ensemble des messages
				    de synchronisation */

#define XZSCC_TIMEOUT_READ       	5.0   /* Timeout de cnx RTworks */
#define XZSCC_TIMEOUT_WRITE      	5.0   /*   ""            ""     */
#define XZSCC_TIMEOUT_KEEPALIVE 	30.0   /*   ""            ""     */
/* Lancement automatique du RTSERVER inhibe */
#define XZSCC_NEVER_RTSERVER	"start_never"	
/* Lancement automatique du RTSERVER autorise */
#define XZSCC_ALWAYS_RTSERVER	"start_always"	
#define XZSCC_DEMAND_RTSERVER	"start_on_demand"	

#define XZSCC_FIC_RES_RTWORKS	XDC_NSUP_PATHEXEC"/rtmigrazur.cmd"

#define XZSCC_TACHE_TSAPLG	"tsaplg.x"

#define egale(s1, s2)        (!strcmp ((s1), (s2)))

/* declaration des variables locales  */


XZSCT_NomMachine	XZSCV_NomMachine;	
XZSCT_NomTache		XZSCV_NomTache;
XZSCT_NomPeriph		XZSCV_NomPeriphSR;
int			XZSCV_TypeMachine;
XZSCT_NomMachine	XZSCV_NomMachineImp;
XZSCT_NomPeriph		XZSCV_NomImprimante;
XZSCT_NomPeriph		XZSCV_NomImprimanteSec;
char			XZSCV_PathTrace[80];
int			XZSCV_TailleFicTrace;
int			XZSCV_MaxTailleFicTrace;
int			XZSCV_InitOk = XDC_FAUX;
XZSCT_NomSite		XZSCV_NomSite;
int			XZSCV_PeriodeTestReseau;
int			XZSCV_Priorite;
int			XZSCV_TypeProfil;
int                     XZSCV_DureeAbsenceCourte; 
XDY_NomSite   vg_site;

#ifndef _TIBCO_EMS
T_INT4			vg_TypeMessage;
#endif

/* declaration de fonctions internes */
struct addrinfo {
    int              ai_flags;
    int              ai_family;
    int              ai_socktype;
    int              ai_protocol;
    size_t           ai_addrlen;
    struct sockaddr *ai_addr;
    char            *ai_canonname;
    struct addrinfo *ai_next;
};


/* fonctions internes callbacks */
void XZSC_SynchroMigrazur(
    T_IPC_CONN 			va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA 	pa_Data,
    T_CB_ARG 			pa_Arg);

#ifndef _TIBCO_EMS
void print_field_offsets( T_IPC_MSG msg );
void clone_message( T_IPC_MSG msg_source, T_IPC_MSG msg_clone );
void clone_message_JMS_vers_SS(T_IPC_MSG va_msg_source, char *va_grammaire);
T_PTR T_ENTRY print_one_field(T_IPC_MSG msg, T_IPC_MSG_TRAV trav, T_PTR arg);
T_PTR T_ENTRY append_one_field(T_IPC_MSG msg, T_IPC_MSG_TRAV trav, T_PTR arg);
int recherche_grammaire( T_INT4 va_NumType, char *va_grammaire );
#endif


/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Envoie les msg   XDM_TSP_STARTTMP et XDM_TSE_STARTTMP
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int XZSC_EnvoieSurveillance(
    XZSCT_NomTache va_NomTache,
    XZSCT_NomSite va_NomSite)

/*
 * ARGUMENTS EN ENTREE :
 *   va_NomTahe : Nom de la Tache lancee
 *   va_NomSite : Nom du Site vers lequel on envoie les informations
 *
 *
 * ARGUMENTS EN SORTIE :
 *    Aucun
 *
 *
 * CODE RETOUR :
 *   Aucun
 *
 * CONDITION D'UTILISATION
 *   Aucune
 *
 * FONCTION
 *   Envoie les msg XDM_TSP_STARTMP et XDM_TSE_STARTMP
 *	respectivement sur les dg NSUAPLG_NomMachine et
 *	NSUPETAT_NomMachine
 *
------------------------------------------------------*/
{
  int 	vl_ValRet	= XDC_OK;
  XZSCT_Datagroup		vl_datagroup	= "";
  int			vl_Priorite	= 100;
  T_IPC_MSG 		vl_Msg;


  XZST_03EcritureTrace(XZSTC_FONCTION ,"XZSC_EnvoieSurveillance : IN - Nom Tache %s",va_NomTache);
  /*B Envoi d'un message XDM_TSP_STARTTMP de creation
   * du Datagroup de Surveillance de la Tache sur le
   *       dategroup NSUPAPLG_NomMachine*/
  XDG_EncodeDG2(vl_datagroup,XDG_NSUPAPLG,XZSCV_NomMachine) ;
  if (!TipcSrvMsgWrite(vl_datagroup
      ,XDM_IdentMsg(XDM_TSE_STARTTMP)
      ,XDM_FLG_SRVMSGWRITE
      ,T_IPC_FT_STR
      ,va_NomTache
      ,T_IPC_FT_STR
      ,XZSCV_NomMachine
      ,NULL))
  {
    /* Affichage trace */
    XZST_03EcritureTrace(XZSTC_WARNING
        ,"XZSC_EnvoieSurveillance: Erreur TipcSrvMsgWrite\
        (vl_Datagroup,XDM_TSE_STARTTMP,XDM_FLG_SRVMSGWRITE,,%s)"                                 ,va_NomTache );
  }


  /*B Envoi d'un message XDM_TSE_STARTTMP de creation
   * du Datagroup de Surveillance de la Tache sur le
   *       dategroup NSUPETAT_NomSite*/
  /* Joint la datagroup XDG_NSUPETAT_NomSite */
  XDG_EncodeDG2(vl_datagroup,XDG_NSUPETAT,va_NomSite) ;
  if (!TipcSrvMsgWrite(vl_datagroup
      ,XDM_IdentMsg(XDM_TSE_STARTTMP)
      ,XDM_FLG_SRVMSGWRITE
      ,T_IPC_FT_STR
      ,va_NomTache
      ,T_IPC_FT_STR
      ,XZSCV_NomMachine
      ,NULL))
  {
    /* Affichage trace */
    XZST_03EcritureTrace(XZSTC_WARNING
        ,"XZSC_EnvoieSurveillance: Erreur TipcSrvMsgWrite\
        (vl_Datagroup,XDM_TSE_STARTTMP,XDM_FLG_SRVMSGWRITE,,%s)"                                 ,va_NomTache );
  }

  /* Envoi d'un message XDM_TSE_STARTTMP de creation
   * du Datagroup de Surveillance de la Tache sur le
   *       dategroup NSUPETAT_NomMachine*/
  /* Joint la datagroup XDG_NSUPETAT_NomMachine */
  XDG_EncodeDG2(vl_datagroup,XDG_NSUPETAT,XZSCV_NomMachine) ;
  if (!TipcSrvMsgWrite(vl_datagroup
      ,XDM_IdentMsg(XDM_TSE_STARTTMP)
      ,XDM_FLG_SRVMSGWRITE
      ,T_IPC_FT_STR
      ,va_NomTache
      ,T_IPC_FT_STR
      ,XZSCV_NomMachine
      ,NULL))
  {
    /* Affichage trace */
    XZST_03EcritureTrace(XZSTC_WARNING
        ,"XZSC_EnvoieSurveillance: Erreur TipcSrvMsgWrite\
        (vl_Datagroup,XDM_TSE_STARTTMP,XDM_FLG_SRVMSGWRITE,,%s)"                                 ,va_NomTache );
  }

  /*A armement du callback de reception du message de relance
   *	de l'application MIGRAZUR (cad des taches tsaplg.x
   *	ou tsetat.x ) */
  if (TipcSrvProcessCbCreate(TipcMtLookupByNum(XDM_TSP_MIGRAZUR),
      (T_IPC_CONN_PROCESS_CB_FUNC) XZSC_SynchroMigrazur,
      (T_CB_ARG) NULL) == T_FALSE)
  {
    /* erreur RTWorks */
    vl_ValRet = XZSCC_RTW_INV;
  }


  XZST_03EcritureTrace(XZSTC_FONCTION ,"XZSC_EnvoieSurveillance : OUT");

  return (vl_ValRet);


}	/* Fin XZSC_EnvoieSurveillance */


/******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 *
 * Fonction
 * Cette fonction trace les messages RTWorks recu par
 * la tache et ne faisant pas l'objet d'un traitement
 * particulier.
 * Les infos affichees par la trace sont:
 *    - l'emetteur du message
 *    - la taille du message
 *    - le numero et le nom du type du message
 *
 * Codes retour
 * Sans objet (callback)
 *
 * Mecanismes
 * RTS ???
 ******************************************************/

void XZSC_RecepMsgDefaut(
    T_IPC_CONN 			va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA 	pa_Data,
    T_CB_ARG 			pa_Arg)
{
#ifndef _TIBCO_EMS
  int 		vl_ValRet 	= XDC_OK;
  T_INT4 	vl_Taille	= 0;
  T_STR 	pl_Emetteur 	= "\0";
  T_STR 	pl_Destinataire	= "\0";

  T_STR 	pl_NomType	= "\0";
  T_INT4 	vl_NumType	= 0;
  char 		vl_grammaire[255];
  T_IPC_MT	vl_TypeMsg;

  XZST_03EcritureTrace(XZSTC_FONCTION,
      " XZSC_RecepMsgDefaut : IN \n");

  /*	TipcMsgPrint(pa_Data->msg, TutOut);*/

  /* Recuperation de la taille du message */
  if(!TipcMsgGetDataSize(pa_Data->msg,&vl_Taille))
  {
    XZST_03EcritureTrace(XZSTC_WARNING,"Recuperation Taille echoue !!");
    vl_ValRet = XZSCC_RTW_INV;
  }

  if(vl_ValRet == XDC_OK)
  {
    if(!TipcMsgGetSender(pa_Data->msg,&pl_Emetteur))
    {
      XZST_03EcritureTrace(XZSTC_WARNING,"Recuperation Emetteur echoue !!");
      vl_ValRet = XZSCC_RTW_INV;
    }
  }
  XZST_03EcritureTrace(XZSTC_WARNING,"Emetteur <%s>", pl_Emetteur);

  if(vl_ValRet == XDC_OK)
  {
    if(!TipcMsgGetDest(pa_Data->msg,&pl_Destinataire))
    {
      XZST_03EcritureTrace(XZSTC_WARNING,"Recuperation Destinataire  echoue !!");
      vl_ValRet = XZSCC_RTW_INV;
    }
  }

  if(vl_ValRet == XDC_OK)
  {
    /* Recuperation du type du message */
    if(!TipcMsgGetType(pa_Data->msg, &vl_TypeMsg))
    {
      XZST_03EcritureTrace(XZSTC_WARNING,"Recuperation Type Msg echoue !!");
      vl_ValRet = XZSCC_RTW_INV;
    }
    if(vl_ValRet == XDC_OK)
    {
      /* Recuperation du numero du type */
      if(TipcMtGetNum (vl_TypeMsg,
          &vl_NumType) != T_TRUE)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,"Recuperation Num du Type Msg echoue !!");
        vl_ValRet = XZSCC_RTW_INV;
      }
      else
      	vg_TypeMessage=vl_NumType;
    }
//    XZST_03EcritureTrace(XZSTC_WARNING,"Recuperation Num du Type Msg OK XXX %d", vl_NumType);

//    if (vl_NumType==-910)
//    {
    	/* C'est un message jms_stream */
	XZST_03EcritureTrace(XZSTC_WARNING,"Decodage message JMS_STREAM");

	/* Pour aller recuperer le type du message original */
	print_field_offsets(pa_Data->msg);
	if (vg_TypeMessage==0)
		vg_TypeMessage=vl_NumType;
	XZST_03EcritureTrace(XZSTC_WARNING," Type message trouve : [%d]", vg_TypeMessage);

	/* Je reposte le message à moi meme au bon format sauf si je suis deja l'emetteur */
	//XZST_03EcritureTrace(XZSTC_WARNING," strstr : %s", strstr(pl_Emetteur,XZSCV_NomMachine));
	if (strstr(pl_Emetteur,XZSCV_NomMachine))
		XZST_03EcritureTrace(XZSTC_WARNING," Message non reposte - %s est deja l'emetteur (%s)",XZSCV_NomMachine, pl_Emetteur);
	else
		clone_message_JMS_vers_SS(pa_Data->msg, vl_grammaire);

//    }
//    else
//    	print_field_offsets(pa_Data->msg);

  }

  if(vl_ValRet == XDC_OK)
  {
    /* ecriture d'un message dans le fichier trace */
    vl_ValRet = XZST_03EcritureTrace(XZSTC_WARNING,
        "Message traite; Emetteur: %s  Taille: %d  \
        Nom type: %s  Num type: %d\n",
        pl_Emetteur,
        vl_Taille,
        pl_NomType,
        vl_NumType);

  }

  XZST_03EcritureTrace(XZSTC_FONCTION,
      " XZSC_RecepMsgDefaut : OUT \n");
#endif
}	/* Fin XZSC_RecepMsgDefaut */

#ifndef _TIBCO_EMS
int recherche_grammaire( T_INT4 va_NumType, char *va_grammaire )
{
int i = 0;

	/* On parcourt le tableau : la derniere ligne est { "", 0, "" } avec { Nom, Id, Grammaire } */ 
	while (tg_Mtb[i].Id > 0)
	{
//XZST_03EcritureTrace(XZSTC_WARNING,"recherche_grammaire : Nom=[%s] Id=[%d] Grammaire=[%s]", tg_Mtb[i].Nom, tg_Mtb[i].Id, tg_Mtb[i].Grammaire);
		if ( tg_Mtb[i].Id == va_NumType )
		{
			strcpy( va_grammaire, tg_Mtb[i].Grammaire);
			return(1);
		}
		else
			i++;
		
	}
	return(0);
}


T_PTR T_ENTRY print_one_field(T_IPC_MSG msg, T_IPC_MSG_TRAV trav, T_PTR arg)
{
int vl_type;
int *counter_ptr = arg;
T_IPC_MSG vl_msg;

(*counter_ptr)++;
//	XZST_03EcritureTrace(XZSTC_WARNING," avant vl_type counter_ptr [%d]", *counter_ptr );
vl_type = (T_IPC_FT)(*trav->type_ptr);
if (vl_type==T_IPC_FT_INT4)
{
	T_INT4 vl_int;
	vl_int = *(T_INT4*)trav->value_ptr;
//	XZST_03EcritureTrace(XZSTC_WARNING," Nom Type T_IPC_FT_INT4 Value [%d]", vl_int );
}												
else if (vl_type==T_IPC_FT_INT2)
{
	T_INT2 vl_int;
	vl_int = *(T_INT2*)trav->value_ptr;
//	XZST_03EcritureTrace(XZSTC_WARNING," Nom Type T_IPC_FT_INT2 Value [%d]", vl_int );
}												
else if (vl_type==T_IPC_FT_INT2_ARRAY)
{
	/*T_INT2 vl_int;
	vl_int = *(T_INT2*)trav->value_ptr;*/
//	XZST_03EcritureTrace(XZSTC_WARNING,"  Nom Type T_IPC_FT_INT2_ARRAY" );
}												
else if (vl_type==T_IPC_FT_INT4_ARRAY)
{
	/*T_INT2 vl_int;
	vl_int = *(T_INT2*)trav->value_ptr;*/
//	XZST_03EcritureTrace(XZSTC_WARNING," Nom Type T_IPC_FT_INT4_ARRAY" );
}												
else if (vl_type==T_IPC_FT_REAL8_ARRAY)
{
	/*T_INT2 vl_int;
	vl_int = *(T_INT2*)trav->value_ptr;*/
//	XZST_03EcritureTrace(XZSTC_WARNING," Nom Type T_IPC_FT_REAL8_ARRAY" );
}												
else if (vl_type==T_IPC_FT_STR)
{
	char vl_str[255];

	strcpy(vl_str, (char*)trav->value_ptr);
/*	XZST_03EcritureTrace(XZSTC_WARNING," Nom Type T_IPC_FT_STR Value [%s]", vl_str );
*/
	XZST_03EcritureTrace(XZSTC_WARNING," counter_ptr [%d]", *counter_ptr );
	/* on recupere le 6ieme champ (mais le compteur a deja ete incremente */
	/* si ça n'est pas le sous-message, il est remis a zero ensuite */
	if (*counter_ptr==7)
	{
//		XZST_03EcritureTrace(XZSTC_WARNING," Nom Type T_IPC_FT_STR Value --> vgTypeMessage [%s]", vl_str );
		vg_TypeMessage=atoi(vl_str);
	}
}												
else if (vl_type==T_IPC_FT_REAL8)
{
	T_REAL8 vl_real;
	vl_real = *(T_REAL8*)trav->value_ptr;
//	XZST_03EcritureTrace(XZSTC_WARNING,"  Nom Type T_IPC_FT_REAL8 Value [%f]", vl_real );
												
}
else if (vl_type==T_IPC_FT_CHAR)
{
	T_CHAR vl_char;
		vl_char = *(T_CHAR*)trav->value_ptr;
//	XZST_03EcritureTrace(XZSTC_WARNING," Nom Type T_IPC_FT_CHAR Value [%c]", vl_char );

}
else if (vl_type==T_IPC_FT_MSG)
{
//	XZST_03EcritureTrace(XZSTC_WARNING," Nom Type T_IPC_FT_MSG" );
	T_INT4 vl_Taille = 0;
	T_IPC_MT vl_TypeMsg = 0;
	T_STR vl_Emetteur = "\0";
	T_STR vl_Destinataire = "\0";
	T_INT4 vl_NumType = 0;
	vg_TypeMessage=0;
	vl_msg = (T_IPC_MSG)trav->value_ptr;
	if(!TipcMsgGetDataSize(vl_msg,&vl_Taille))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"Recuperation Taille Msg echoue !!");
	}
	if(!TipcMsgGetSender(vl_msg,&vl_Emetteur))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"Recuperation Emetteur Msg echoue !!");
	}
	if(!TipcMsgGetDest(vl_msg,&vl_Destinataire))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"Recuperation Destinataire Msg echoue !!");
	}
	if(!TipcMsgGetType(vl_msg, &vl_TypeMsg))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"Recuperation Type Msg echoue !!");
	}
	if(!TipcMtGetNum(vl_TypeMsg, &vl_NumType))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"Recuperation Num du Type Msg echoue !!");
	}
	print_field_offsets(vl_msg);
}
else
{
	T_INT4 vl_int;
	vl_int = *(T_INT4*)trav->value_ptr;
//	XZST_03EcritureTrace(XZSTC_WARNING," Nom Type AUTRE Value [%d]", vl_int );

}
return NULL; /* continue traversal */
} /* print_one_field */

void print_field_offsets( T_IPC_MSG msg )
{
	int counter;

	vg_TypeMessage=0;
	counter = 1;
	(void)TipcMsgTraverse(msg, print_one_field, &counter);
} /* print_field_offsets */

void clone_message_JMS_vers_SS(T_IPC_MSG va_msg_source, char *va_grammaire)
{
#define MAX_MALLOC 30
char	vl_grammaire[255];
int	vl_type;
char 	*vl_type_grammaire;		/* Nombre maximal de champs array dans un message */
int 	i;
char 	*vl_malloc[MAX_MALLOC];
int	vl_current_malloc;
T_IPC_FT vl_type_origine;
T_INT4 	vl_list_size;
T_INT4 	vl_buffer_size;
T_INT4 	*INT4;
T_INT2 	*INT2;
T_STR 	*STR;
T_REAL8	*REAL8;
T_CHAR 	*CHAR;
T_INT2 	*ARRAY_INT2;
T_INT4	*ARRAY_INT4;
T_STR	**ARRAY_STR;
T_REAL8	*ARRAY_REAL8;
T_IPC_MSG 	vl_msg_clone;
T_IPC_MT	vl_TypeMsg = 0;
XDY_Datagroup	pl_Destinataire="";

	XZST_03EcritureTrace(XZSTC_INFO,"clone_message_JMS_vers_SS : IN");

	vl_TypeMsg = XDM_IdentMsg(vg_TypeMessage);
	XZST_03EcritureTrace(XZSTC_WARNING," Type message trouve par XDM_IdentMsg: %d", vl_TypeMsg);
	vl_msg_clone = TipcMsgCreate( vl_TypeMsg );
	if (vl_msg_clone==NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING," Erreur creation message clone");
	}

	/* je le renvoie a moi meme */
	XDG_EncodeDG2 (pl_Destinataire,XZSCV_NomMachine,XZSCV_NomTache);
	XZST_03EcritureTrace(XZSTC_WARNING," Destinataire message ems clone sur DG rtserver [%s]",pl_Destinataire);
	if (!TipcMsgSetDest(vl_msg_clone, pl_Destinataire))
	{
		XZST_03EcritureTrace(XZSTC_WARNING," Erreur set dest message clone");
		return;
	}
	else
	{
		if ( !recherche_grammaire( vg_TypeMessage, vl_grammaire ))
		{
			XZST_03EcritureTrace(XZSTC_WARNING," Erreur recherche grammaire message !!!!");
			return;
		}
		XZST_03EcritureTrace(XZSTC_WARNING," Grammaire trouvee : [%s]",vl_grammaire);
	}

	/* init table des malloc */
	vl_current_malloc=0;
	for (i=0;i<MAX_MALLOC;i++)
		vl_malloc[i]=NULL;

	/* recuperation de la grammaire SmartSockets pour traiter les tableaux */
	vl_type_grammaire=strtok(vl_grammaire," ");

	while (vl_type_grammaire!=NULL)
	{
		if (!strcmp(vl_type_grammaire,"int4"))
			vl_type=T_IPC_FT_INT4;
		else if (!strcmp(vl_type_grammaire,"int4_array"))
			vl_type=T_IPC_FT_INT4_ARRAY;
		else if (!strcmp(vl_type_grammaire,"int2"))
			vl_type=T_IPC_FT_INT2;
		else if (!strcmp(vl_type_grammaire,"int2_array"))
			vl_type=T_IPC_FT_INT2_ARRAY;
		else if (!strcmp(vl_type_grammaire,"str"))
			vl_type=T_IPC_FT_STR;
		else if (!strcmp(vl_type_grammaire,"str_array"))
			vl_type=T_IPC_FT_STR_ARRAY;
		else if (!strcmp(vl_type_grammaire,"real8"))
			vl_type=T_IPC_FT_REAL8;
		else if (!strcmp(vl_type_grammaire,"real8_array"))
			vl_type=T_IPC_FT_REAL8_ARRAY;
		else if (!strcmp(vl_type_grammaire,"char"))
			vl_type=T_IPC_FT_CHAR;
//XZST_03EcritureTrace(XZSTC_WARNING,"recherche_grammaire : %s %d", vl_type_grammaire, vl_type);

		if (vl_type==T_IPC_FT_INT4)
		{
			INT4=malloc(sizeof(T_INT4));

			/* on stocke l'adresse allouee */
			vl_malloc[vl_current_malloc++]=(char*)INT4;

			TipcMsgNextInt4(va_msg_source, INT4);
			TipcMsgAppendInt4( vl_msg_clone, *INT4);
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendInt4 : %d", *INT4);
		}
		else if (vl_type==T_IPC_FT_INT4_ARRAY)
		{
			TipcMsgNextInt4(va_msg_source, &vl_list_size);
			if (&vl_list_size==NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"Erreur recuperation longueur tableau INT4");
				return;
			}
			ARRAY_INT4=(T_INT4 *)malloc((sizeof(T_INT4)*vl_list_size));

			/* on stocke l'adresse allouee */
			vl_malloc[vl_current_malloc++]=(char*)ARRAY_INT4;

			for (i=0;i<vl_list_size;i++)
				TipcMsgNextInt4(va_msg_source,&ARRAY_INT4[i]);

			TipcMsgAppendInt4Array(vl_msg_clone, ARRAY_INT4, i);
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendInt4 Array");
		}
		else if (vl_type==T_IPC_FT_INT2)
		{
			INT2=malloc(sizeof(T_INT2));

			/* on stocke l'adresse allouee */
			vl_malloc[vl_current_malloc++]=(char*)INT2;

			TipcMsgNextInt2(va_msg_source, INT2);
			TipcMsgAppendInt2(vl_msg_clone, *INT2);
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendInt2 : %d", *INT2);
		}
		else if (vl_type==T_IPC_FT_INT2_ARRAY)
		{
			TipcMsgNextInt4(va_msg_source, &vl_list_size);
			if (&vl_list_size==NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"Erreur recuperation longueur tableau INT2");
				return;
			}
			ARRAY_INT2=(short *)malloc((sizeof(short)*vl_list_size));

			/* on stocke l'adresse allouee */
			vl_malloc[vl_current_malloc++]=(char*)ARRAY_INT2;

			for (i=0;i<vl_list_size;i++)
				TipcMsgNextInt2(va_msg_source,&ARRAY_INT2[i]);

			TipcMsgAppendInt2Array(vl_msg_clone, ARRAY_INT2, i);
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendInt2 Array");
		}
		else if (vl_type==T_IPC_FT_STR)
		{
			/* Specifique XDM_API_POKE qui peut envoyer un array en quatrieme parametre alors que la grammaire indique str */
			if (TipcMsgNextType(va_msg_source, &vl_type_origine))
			{
				if ( vl_type_origine==T_IPC_FT_INT4)
			        {
				          /* on a un entier, c'est donc un tableau qu'il faut envoyer */
				          TipcMsgNextInt4(va_msg_source, &vl_list_size);
				          TipcMsgNextInt4(va_msg_source, &vl_buffer_size);
				          if (&vl_list_size==NULL)
				          {
					            XZST_03EcritureTrace(XZSTC_WARNING,"Erreur recuperation longueur tableau STR");
					            return;
				          }
				          if (&vl_buffer_size==NULL)
				          {
					            XZST_03EcritureTrace(XZSTC_WARNING,"Erreur recuperation taille chaine STR");
            					    return;
					  }
				          ARRAY_STR=(T_STR **)malloc((sizeof(T_STR *)*vl_list_size));

			/* on stocke l'adresse allouee */
			vl_malloc[vl_current_malloc++]=(char*)ARRAY_STR;

			for (i=0;i<vl_list_size;i++)
			{
				ARRAY_STR[i]=(T_STR *)malloc(sizeof(T_STR)*vl_buffer_size);

				/* on stocke l'adresse allouee */
				vl_malloc[vl_current_malloc++]=(char*)ARRAY_STR[i];

				TipcMsgNextStr(va_msg_source,ARRAY_STR[i]);
			}

				          TipcMsgAppendStrArray(vl_msg_clone, *ARRAY_STR, i);
			        }
				else if ( vl_type_origine==T_IPC_FT_STR)
			        {
				          /* On a bien une chaine */
				          STR=malloc(255*sizeof(T_STR));

				          /* on stocke l'adresse allouee */
				          vl_malloc[vl_current_malloc++]=(char*)STR;

				          TipcMsgNextStr(va_msg_source, STR);
				          TipcMsgAppendStr(vl_msg_clone, *STR);
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendStr : %s", *STR);
			        }
				else
					XZST_03EcritureTrace(XZSTC_WARNING," clone_message_JMS_vers_SS : Type inattendu (ni INT4, ni STR)");

			}
			else
				XZST_03EcritureTrace(XZSTC_WARNING," clone_message_JMS_vers_SS : Type CHAR/INT2 illisible pour conversion"); 

		}
		else if (vl_type==T_IPC_FT_STR_ARRAY)
		{
			/* Specifique StrArray pas gerable par la gateway : les strArray sont remplaces par une chaine simple concatenee entre des | */
			if (TipcMsgNextType(va_msg_source, &vl_type_origine))
			{
				if ( vl_type_origine==T_IPC_FT_STR)
			        {
					/* C'est la chaine concatenee : on la reposte telle quelle */
				        TipcMsgNextStr(va_msg_source, STR);
				        TipcMsgAppendStr(vl_msg_clone, *STR);
				}
				else
				{
					XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendStrArray NON GERE EN PROVENANCE DE LA GATEWAY");
				}

#ifdef _EXCLUDE			
			TipcMsgNextInt4(va_msg_source, &vl_list_size);
			TipcMsgNextInt4(va_msg_source, &vl_buffer_size);
			if (&vl_list_size==NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"Erreur recuperation longueur tableau STR");
				return;
			}
			if (&vl_buffer_size==NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"Erreur recuperation taille chaine STR");
				return;
			}

XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendStrArray debut 4");
//			ARRAY_STR=(T_STR **)malloc((sizeof(T_STR *)*vl_list_size));

XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendStrArray debut 5");
			/* on stocke l'adresse allouee */
//			vl_malloc[vl_current_malloc++]=(char*)ARRAY_STR;
T_STR strptr;
			for (i=0;i<vl_list_size;i++)
			{
XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendStrArray debut 6");
				ARRAY_STR[i]=(T_STR *)malloc(sizeof(char)*vl_buffer_size);
			ptr=(char*)(ARRAY_STR[i]);

				/* on stocke l'adresse allouee */
				vl_malloc[vl_current_malloc++]=(char*)ARRAY_STR[i];

				TipcMsgNextStr(va_msg_source,(T_STR*)&strptr);
XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendStrArray strptr=%s",strptr);
XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendStrArray ptr=%x",ptr);
//				memcpy((void*)ptr++,strptr,strlen(strptr)+1);
XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendStrArray ARRAY_STR[%d]=%s",i,*ARRAY_STR[i]);
			}
XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendStrArray debut 7");
for (i=1;i>vl_list_size;i++) 
XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendStrArray ARRAY_STR[%d]=%s",i,*ARRAY_STR[i]);

			TipcMsgAppendStrArray(vl_msg_clone, (T_STR*)ARRAY_STR, (T_INT4)vl_list_size);

#endif
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendStrArray fin");
			}
		}
		else if (vl_type==T_IPC_FT_REAL8)
		{
			REAL8=malloc(sizeof(T_REAL8));

			/* on stocke l'adresse allouee */
			vl_malloc[vl_current_malloc++]=(char*)REAL8;

			TipcMsgNextReal8(va_msg_source, REAL8);
			TipcMsgAppendReal8(vl_msg_clone, *REAL8);
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendReal8 : %f", *REAL8);
		}
		else if (vl_type==T_IPC_FT_REAL8_ARRAY)
		{
			TipcMsgNextInt4(va_msg_source, &vl_list_size);
			if (&vl_list_size==NULL)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"Erreur recuperation longueur tableau REAL8");
				return;
			}
			ARRAY_REAL8=(double *)malloc((sizeof(double)*vl_list_size));

			/* on stocke l'adresse allouee */
			vl_malloc[vl_current_malloc++]=(char*)ARRAY_REAL8;

			for (i=0;i<vl_list_size;i++)
				TipcMsgNextReal8(va_msg_source,&ARRAY_REAL8[i]);

			TipcMsgAppendReal8Array(vl_msg_clone, ARRAY_REAL8, i);
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendReal8Array");
		}
		else if (vl_type==T_IPC_FT_CHAR)
		{
			CHAR=malloc(sizeof(T_CHAR));

			/* on stocke l'adresse allouee */
			vl_malloc[vl_current_malloc++]=(char*)CHAR;

			/* La gateway transforme les CHAR en INT2 */
			/* Dans le message EMS, c'est un INT2 qu'il faut convertir en CHAR pour SmartSockets */
			if (TipcMsgNextType(va_msg_source, &vl_type_origine))
			{
				if ( vl_type_origine==T_IPC_FT_INT2)
					TipcMsgNextInt2(va_msg_source, (T_INT2*) CHAR);
				else
					TipcMsgNextChar(va_msg_source, CHAR);
				TipcMsgAppendChar(vl_msg_clone, *CHAR);
			}
			else
				XZST_03EcritureTrace(XZSTC_WARNING," clone_message_JMS_vers_SS : Type CHAR/INT2 illisible pour conversion"); 
//XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgAppendChar : chr(%d)", *CHAR);

		}


		vl_type_grammaire=strtok(NULL," ");

		if (vl_current_malloc>=MAX_MALLOC)
			XZST_03EcritureTrace(XZSTC_WARNING,"clone_message_JMS_vers_SS : Depassement nombre maximal de malloc grammaire messages atteint - fuite memoire possible"); 

	}

	/* Envoi du message */
	if (!TipcSrvMsgSend( vl_msg_clone, TRUE ))
	{
		XZST_03EcritureTrace(XZSTC_WARNING," clone_message_JMS_vers_SS : Erreur TipcSrvMsgSend message clone");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING," clone_message_JMS_vers_SS : TipcSrvMsgSend message effectue");
	}

	/* Liberation de la memoire allouee precedemment */
	vl_current_malloc=0;
	while (vl_malloc[vl_current_malloc]!=NULL)
	{
		free(vl_malloc[vl_current_malloc++]);
	}

	XZST_03EcritureTrace(XZSTC_INFO,"clone_message_JMS_vers_SS : OUT");
}


#endif

/******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 *
 * Fonction
 * Il s'agit de la fonction de sortie de la tache par
 * defaut (executee sur reception du message TSP_EXIT).
 * La fonction fermer le fichier de trace et rend la
 * main au shell appelant.
 *
 * Codes retour
 * Sans objet (callback)
 *
 * Mecanismes
 *
 ******************************************************/

void XZSC_Sortir()
{
  XZST_03EcritureTrace(XZSTC_FONCTION,
      " XZSC_Sortir : IN \n");

  /* fermeture du fichier de trace */
  XZST_02FermetureTrace();

  /* arret de la tache */
  exit(0);

  XZST_03EcritureTrace(XZSTC_FONCTION,
      " XZSC_Sortir : OUT \n");

}	/* Fin XZSC_Sortir */


/* autres fonctions internes */

/******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 *
 * Fonction
 * Cette fonction initialise les variables globales de
 * description du noyau de supervision de la tache
 *
 * Codes retour
 * XDC_OK      ok
 * Mecanismes
 *
 ******************************************************/

int XZSC_InitDescNoyau()
{

  int 			TailleNomMachine 	= sizeof(XZSCV_NomMachine);
  /* valeur de retour de la fonction */
  int 			vl_ValRet 		= 	XDC_OK;
  /* description du noyau de supervision de la machine */
  XZSXT_Machine	Machine;


  XZST_03EcritureTrace(XZSTC_FONCTION,
      " XZSC_InitDescNoyau : IN \n");

  /* recherche du nom de la machine */
  if(gethostname(XZSCV_NomMachine, TailleNomMachine) == -1)
  {
    vl_ValRet = XZSCC_NOM_INV;
  }

  /* **** recuperation de l'alias de /etc/host pour la validation  
   *
   *if(XZSC_NomAlias(XZSCV_NomMachine) == -1)*/

  if(vl_ValRet == XDC_OK)
  {
    /* initialisation des variables globales de la tache */
    vl_ValRet = XZSX_01MachineLecture(XZSCV_NomMachine, XDC_NSUP_PATHOPER, &Machine);

    if(vl_ValRet == XDC_OK)
    {
      /* test si les noms machine coincident */
      if(!strcmp(XZSCV_NomMachine, Machine.NomMachine))
      {
        /* Affectation de la structure retournee */
        strcpy(XZSCV_NomPeriphSR, Machine.NomPeriphSR);
        strcpy(XZSCV_NomMachineImp, Machine.NomMachineImp);
        strcpy(XZSCV_NomImprimante, Machine.NomImprimante);
        strcpy(XZSCV_NomImprimanteSec, Machine.NomImprimanteSec);
        strcpy(XZSCV_NomSite, Machine.NomSite);
        strcpy(XZSCV_PathTrace, Machine.PathTrace);
        XZSCV_MaxTailleFicTrace = Machine.MaxTailleFicTrace;
        XZSCV_PeriodeTestReseau = Machine.PeriodeTestReseau;
        XZSCV_TypeMachine = Machine.TypeMachine;
        XZSCV_Priorite = Machine.Priorite;
        XZSCV_TypeProfil = Machine.TypeProfil;
        XZSCV_DureeAbsenceCourte = Machine.DureeAbsenceCourte;
      }
      else
      {
        /* incoherence fichier configuration */
        vl_ValRet = XZSCC_FIC_INV;
      }
    }
  }

  XZST_03EcritureTrace(XZSTC_FONCTION,
      " XZSC_InitDescNoyau : OUT \n");

  return(vl_ValRet);

}	/* Fin XZSC_InitDescNoyau */


void XZSC_ErreurRTW(T_IPC_CONN conn, T_CONN_ERROR_CB_DATA data, T_CB_ARG arg)
{
  XZST_03EcritureTrace (XZSTC_WARNING, "SrvError; TutErrNumGet = %d; errno = %d \n", TutErrNumGet(), errno);
}





/******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 *
 * Fonction
 * Cette fonction connecte la tache au RTServer avec les
 * options contenues dans le fichier de configuration
 * NSUP_Machine.
 * Si la tache n'existe pas dans le fichier, les options
 * par defaut sont utilisees.
 *
 * Codes retour
 * XDC_OK      ok
 *
 * Mecanismes
 *
 ******************************************************/

int XZSC_ConnexionTache(char *NomApplication)
{

  /* valeur de retour de la fonction */
  int 		vl_ValRet		= XDC_OK;
  /* description du noyau de supervision de la tache */
  XZSXT_Tache 	Tache;
  /* Chaine a envoyer a l'interpreteur de commande RTWorks */
  char 	CommandeRTW[100]	= "\0";
  int		i			= 0;
  char * 	vl_NomServer		= NULL;
  char 	vl_SepVirgule[2]	= ",";
  char 	vl_CmdVirgule[2]	= "\0";
  char 	vl_CmdServer[256]	= "\0";
  char * 	vl_NomLicense		= NULL;
  char 	vl_CmdLicense[256]	= "\0";

  T_CB pl_FonctionErreurRTW;
  T_CB_PRIORITY vl_Priorite;

  XZST_03EcritureTrace(XZSTC_FONCTION,
      " XZSC_ConnexionTache : IN  Nom Appli %s \n",NomApplication);

  vl_ValRet = XZSX_02TacheLecture(XZSCV_NomMachine,
      XZSCV_NomTache,
      XDC_NSUP_PATHOPER,
      &Tache);

  if(vl_ValRet != XZSXC_NOM_INV)
  {
    XZSCV_TailleFicTrace = Tache.TailleFicTrace;

    /* Activation des options RTWorks */

    /*A Si la Machine est un SD ou un SC et que la Tache
     *	est TSAPLG, alors on permet a celle-ci de lancer
     *	le RTSERVER et le RTLM. Ajout de l'option start_always
     **/
    if ( (	(XZSCV_TypeMachine == XZSCC_TYPEM_SD) ||
        (XZSCV_TypeMachine == XZSCC_TYPEM_SC) ||
        (XZSCV_TypeMachine == XZSCC_TYPEM_PC2LT) ||
        (XZSCV_TypeMachine == XZSCC_TYPEM_PC2) ||
        (XZSCV_TypeMachine == XZSCC_TYPEM_PCS)) &&
        (!strcmp((char*)XZSCV_NomTache, (char*)XZSCC_TACHE_TSAPLG)) )
    {
      /*A
       *	Recuperation du Nom des Servers independamment
       **/
      vl_NomServer = strtok(Tache.ListeServer,vl_SepVirgule);
      while ( vl_NomServer != NULL)
      {
        sprintf(vl_CmdServer, "%s%s%s:%s",
            vl_CmdServer,
            vl_CmdVirgule,
            XZSCC_DEMAND_RTSERVER, vl_NomServer);
        /*sprintf(vl_CmdServer, "%s%s%s:%s",
					vl_CmdServer,
			 		vl_CmdVirgule,
				XZSCC_ALWAYS_RTSERVER, vl_NomServer);*/
        vl_NomServer =
            strtok(NULL,vl_SepVirgule);
        strcpy(vl_CmdVirgule,vl_SepVirgule);
      }
      /*A
       *	Recuperation du Nom des Servers de license independamment
       **/
      strcpy ( vl_CmdVirgule, "\0" );
      vl_NomLicense = strtok(Tache.ListeLicense,vl_SepVirgule);
      while ( vl_NomLicense != NULL)
      {
        sprintf(vl_CmdLicense, "%s%s%s:%s",
            vl_CmdLicense,
            vl_CmdVirgule,
            XZSCC_DEMAND_RTSERVER, vl_NomLicense);
        /*sprintf(vl_CmdLicense, "%s%s%s:%s",
					vl_CmdLicense,
			 		vl_CmdVirgule,
				XZSCC_ALWAYS_RTSERVER, vl_NomLicense);*/
        vl_NomLicense =
            strtok(NULL,vl_SepVirgule);
        strcpy(vl_CmdVirgule,vl_SepVirgule);
      }
    }
    else
    {
      /*
       *	Recuperation du Nom des Servers independamment
       **/
      vl_NomServer = strtok(Tache.ListeServer,vl_SepVirgule);
      while ( vl_NomServer != NULL)
      {
        sprintf(vl_CmdServer, "%s%s%s:%s",
            vl_CmdServer,
            vl_CmdVirgule,
            XZSCC_NEVER_RTSERVER, vl_NomServer);
        vl_NomServer =
            strtok(NULL,vl_SepVirgule);
        strcpy(vl_CmdVirgule,vl_SepVirgule);
      }
      /*A
       *	Recuperation du Nom des Servers de License independamment
       **/
      strcpy ( vl_CmdVirgule, "\0" );
      vl_NomLicense = strtok(Tache.ListeLicense,vl_SepVirgule);
      while ( vl_NomLicense != NULL)
      {
        sprintf(vl_CmdLicense, "%s%s%s:%s",
            vl_CmdLicense,
            vl_CmdVirgule,
            XZSCC_NEVER_RTSERVER, vl_NomLicense);
        vl_NomLicense =
            strtok(NULL,vl_SepVirgule);
        strcpy(vl_CmdVirgule,vl_SepVirgule);
      }
    }
	
#ifndef _TIBCO_EMS 

    /*A Option liste RTservers */
    sprintf(CommandeRTW, "setopt server_names %s",vl_CmdServer);
    printf ("%s\n",CommandeRTW);
    TutCommandParseStr(CommandeRTW);

    /*A Option liste RTlm */
    sprintf(CommandeRTW, "setopt lm_names %s", vl_CmdLicense);
    printf ("%s\n",CommandeRTW);
    TutCommandParseStr(CommandeRTW);

    /*A Option application  */
    sprintf(CommandeRTW, "setopt application %s", NomApplication);
    printf ("%s\n",CommandeRTW);
    TutCommandParseStr(CommandeRTW);

    /*A Option protocol */
    TutCommandParseStr("setopt default_protocols tcp") ;

    /*A Positionnement des autres options en utilisant un fichier
     *			de ressources */
    TutCommandParseFile(XZSCC_FIC_RES_RTWORKS);

    /* positionnement des timeout de connexion RTserver */
    /*sprintf(CommandeRTW, "setopt server_read_timeout %d",XZSCC_TIMEOUT_READ);
      TutCommandParseStr(CommandeRTW);

      sprintf(CommandeRTW, "setopt server_write_timeout %d",XZSCC_TIMEOUT_WRITE);
      TutCommandParseStr(CommandeRTW);

      sprintf(CommandeRTW, "setopt server_keep_alive_timeout %d",XZSCC_TIMEOUT_KEEPALIVE);
      TutCommandParseStr(CommandeRTW);*/

    /* Lancement Rtserver si tache tsaplg.x */
    if  (!strcmp((char*)XZSCV_NomTache, (char*)XZSCC_TACHE_TSAPLG))
    {
      /* Positionement de l'option server_start_max_tries a 2 */
      sprintf(CommandeRTW, "setopt server_start_max_tries %d", 2);
      TutCommandParseStr(CommandeRTW);
    }

    /* Connexion de la tache au RTServer */
    /*  Modification 1.0 */
    if(TipcSrvCreate(T_IPC_SRV_CONN_FULL) == T_FALSE)
    {
      printf("\n******************\nechec connexion RTserver\n********************\n");
      vl_ValRet = XZSCC_RTW_INV;
  	  XZST_03EcritureTrace(XZSTC_FONCTION,"XZSC_ConnexionTache : OUT  Code Retour %d \n",vl_ValRet);
  	return(vl_ValRet);
    }
#else 
	vl_NomServer = strtok(Tache.ListeServer,vl_SepVirgule);
	XZST_03EcritureTrace(XZSTC_WARNING,"Tache.ListeServer = %s vl_NomServer = %s",Tache.ListeServer, vl_NomServer);
	if (XZEMS_02Connexion(vl_NomServer) == XDC_NOK)
	{
		vl_ValRet = XZSCC_RTW_INV;
		XZST_03EcritureTrace(XZSTC_FONCTION,
			" XZSC_ConnexionTache : OUT  Code Retour %d \n",vl_ValRet);
		return(vl_ValRet);
	}
#endif

      /* Mise en place de l'auto-flush a chaque envoi d'un
	    message RTWorks */
      TipcSrvSetAutoFlushSize(0);

      /*  Supprime option de relance RTserver */
      if  (!strcmp((char*)XZSCV_NomTache, (char*)XZSCC_TACHE_TSAPLG))
      {
        /* Positionement de l'option server_start_max_tries a 1 */
        sprintf(CommandeRTW, "setopt server_start_max_tries %d", 1);
        TutCommandParseStr(CommandeRTW);
      }

      /* desabonnement de la tache du datagroup _time */
      XDG_QuitterDG("_time");

#ifndef _TIBCO_EMS 
      /* mise en place du call-back d'erreur */
      pl_FonctionErreurRTW = TipcSrvErrorCbCreate (XZSC_ErreurRTW, NULL);

      if (pl_FonctionErreurRTW != NULL)
      {
        TutCbGetPriority (TipcSrvErrorCbLookup(TipcCbSrvError, NULL), &vl_Priorite);
        if (!TutCbSetPriority (pl_FonctionErreurRTW, 1 + vl_Priorite))
        {

          printf("\n******************\nechec TutCbSetPriority\n********************\n");

        }
      }
      else
      {
        printf("\n******************\nechec TipcSrvErrorCbCreate\n********************\n");

      }
#endif 

  }
  else
  /* fichier XDF_NSUP_NomMachine inexistant */
  {
    printf("\n********************\nfichier XDF_NSUP_NomMachine inexistant\n********************\n");
    vl_ValRet = XZSCC_FIC_INV;
  }

  XZST_03EcritureTrace(XZSTC_FONCTION,
      " XZSC_ConnexionTache : OUT  Code Retour %d \n",vl_ValRet);
  return(vl_ValRet);

}	/* Fin XZSC_ConnexionTache */


/*X*/
/* Fichier : @(#)xzsc.c	1.23      Release : 1.23        Date : 09/21/95
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 *
 * Fonction
 * Cette fonction met en place la fonction de sortie
 * de la tache (fonction par defaut ou fonction utilisateur
 * activee sur reception du message XDM_TSP_EXIT.
 *
 * Codes retour
 * XDC_OK      ok
 *
 * Mecanismes
 *
 ******************************************************/

int XZSC_FonctionSortie(void (*FonctionUtil)())
{
  int vl_ValRet = XDC_OK;

  XZST_03EcritureTrace(XZSTC_FONCTION,
      " XZSC_FonctionSortie : IN \n");

  if(FonctionUtil == NULL)
  {
    /* La fonction de sortie est la fonction de sortie par defaut */
    FonctionUtil = XZSC_Sortir;
  }

  /* armement du callback de sortie sur reception du message TSP_EXIT */
  if (TipcSrvProcessCbCreate(TipcMtLookupByNum(XDM_TSP_EXIT),
      (T_IPC_CONN_PROCESS_CB_FUNC) FonctionUtil,
      (T_CB_ARG) NULL) == T_FALSE)
  {
    /* erreur RTWorks */
    vl_ValRet = XZSCC_RTW_INV;
  }

  /* detournement des signaux d'arret d'un processus vers la fonction de sortie */
  if (signal(SIGTERM, FonctionUtil) == (void (*)())-1)
  {
    printf("*** erreur detournement SIGTERM ***");
  }

  XZST_03EcritureTrace(XZSTC_FONCTION,
      " XZSC_FonctionSortie : OUT  Code Retour %d  \n", vl_ValRet);

  return(vl_ValRet);

}	/* Fin XZSC_FonctionSortie */


/*X*/
/* Fichier : @(#)xzsc.c	1.23      Release : 1.23        Date : 09/21/95
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 *
 * Fonction
 * Cette fonction met en place le systeme de reception
 * de l'horodate.
 *
 * Codes retour
 * XDC_OK      ok
 *
 * Mecanismes
 *
 ******************************************************/

int XZSC_InitHorodate()
{
  int vl_ValRet = XDC_OK; /* Valeur de retour de la fonction */

#ifndef _TIBCO_EMS

  XZST_03EcritureTrace(XZSTC_FONCTION,
      " XZSC_InitHorodate : IN \n");

  /* Autorisation de reception de l'horodate */
  /* Modification pour MIGRAZUR
   *	Mode emission XZSMC_SYSTE   */
  XZSM_06AutoriserMajHorodate(XZSMC_SYSTE,XZSCV_NomSite);

  /* Demande de l'horodate */
  vl_ValRet = XZSM_05DemanderDiffusionMode(XZSCV_NomTache,
      XZSCV_NomMachine);

  XZST_03EcritureTrace(XZSTC_FONCTION,
      " XZSC_InitHorodate : OUT  code retour %d  \n", vl_ValRet);

#endif


  return(vl_ValRet);

} 	/* fin XZSC_InitHorodate */


/*X*/
/* Fichier : @(#)xzsc.c	1.23      Release : 1.23        Date : 09/21/95
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 *
 * Fonction
 * Cette fonction met en place le systeme de traces.
 *
 * Codes retour
 * XDC_OK      ok
 *
 * Mecanismes
 *
 ******************************************************/

int XZSC_InitTrace()
{
  int vl_ValRet = XDC_OK; /* Valeur de retour de la fonction */

  XZST_03EcritureTrace(XZSTC_FONCTION,
      " XZSC_InitTrace : IN \n");

  /* ouverture du fichier trace */
  vl_ValRet = XZST_01OuvertureTrace();

  if(vl_ValRet == XDC_OK)
  {
    /* autorisation telecommande du niveau de trace */
    vl_ValRet = XZST_07AutoriserCmdNivVisuTrace();
  }

  if(vl_ValRet == XDC_OK)
  {
    /* forcage du niveau de trace a FATAL et WARNING */
    vl_ValRet = XZST_08ForceNivVisuTraceLocal(
        XZSTV_NiveauTrace, XZSTV_VisuTrace);
  }

#ifndef _TIBCO_EMS

  if (vl_ValRet == XDC_OK)
  {
    /* Orientation des messages recus vers les fichiers traces */
    if(TipcSrvDefaultCbCreate((T_IPC_CONN_PROCESS_CB_FUNC) XZSC_RecepMsgDefaut,
        (T_CB_ARG) NULL) == NULL)
    {
      vl_ValRet = XZSCC_RTW_INV;
    }
  }
#endif

  XZST_03EcritureTrace(XZSTC_FONCTION,
      " XZSC_InitTrace : OUT  Code Retour %d \n",vl_ValRet);

  return(vl_ValRet);

}	/* Fin XZSC_InitTrace */



/*X*/
/* Fichier : @(#)xzsc.c	1.23      Release : 1.23        Date : 09/21/95
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 *
 * Fonction
 * Cette fonction synchronise la tache en attendant les
 * messages de synchronisation envoyes par TSETAT.
 *
 * Codes retour
 * XDC_OK      ok
 *
 * Mecanismes
 *
 ******************************************************/

int XZSC_Synchronisation()
{
  int 		vl_ValRet 	= XDC_OK; /* Valeur de retour de la fonction */
  int 		vl_FinAttente	= 0;
  T_IPC_MSG 	vl_Msg;
  int 		vl_MAJMRecu	= 0;
  int		vl_MAJERecu	= 0;
  int		vl_MAJCRecu	= 0;
  int		vl_MAJDRecu	= 0;


  XZST_03EcritureTrace(XZSTC_FONCTION,
      "  XZSC_Synchronisation : IN \n");
#ifndef _TIBCO_EMS
  /* armement du time-out d'attente des messages de synchronisation */
  if (XZSS_14ArmerTimeout(XZSCC_TEMPO_ALARME) != XDC_OK)
  {

    /* erreur armement time-out */
  }

  vl_MAJMRecu = vl_MAJERecu = vl_MAJCRecu = vl_MAJDRecu = XDC_FAUX;

  /* boucle d'attente */
  do
  {
    /* test si reception du message XDM_TSE_MAJM */
    if ((vl_Msg = TipcSrvMsgSearchType(0.0, TipcMtLookupByNum(XDM_TSE_MAJM))) != NULL)
    {
      vl_MAJMRecu = XDC_VRAI;
      if (!TipcSrvMsgProcess(vl_Msg))
      {
        XZST_03EcritureTrace(XZSTC_WARNING,
            "XZSC_Synchronisation: erreur execution cb XDM_TSE_MAMJ");
      }
      TipcMsgDestroy(vl_Msg);
    }

    /* test si reception du message XDM_TSE_MAJE */
    if ((vl_Msg = TipcSrvMsgSearchType(0.0, TipcMtLookupByNum(XDM_TSE_MAJE))) != NULL)
    {
      vl_MAJERecu = XDC_VRAI;
      if (!TipcSrvMsgProcess(vl_Msg))
      {
        XZST_03EcritureTrace(XZSTC_WARNING,
            "XZSC_Synchronisation: erreur execution cb XDM_TSE_MAJE");
      }
      TipcMsgDestroy(vl_Msg);
    }

    /* test si reception du message XDM_TSE_MAJC */
    if ((vl_Msg = TipcSrvMsgSearchType(0.0, TipcMtLookupByNum(XDM_TSE_MAJC))) != NULL)
    {
      vl_MAJCRecu = XDC_VRAI;
      if (!TipcSrvMsgProcess(vl_Msg))
      {
        XZST_03EcritureTrace(XZSTC_WARNING,
            "XZSC_Synchronisation: erreur execution cb XDM_TSE_MAJC");
      }
      TipcMsgDestroy(vl_Msg);
    }

    /* test si reception du message XDM_TSE_MAJD */
    if ((vl_Msg = TipcSrvMsgSearchType(0.0, TipcMtLookupByNum(XDM_TSE_MAJD))) != NULL)
    {
      vl_MAJDRecu = XDC_VRAI;
      if (!TipcSrvMsgProcess(vl_Msg))
      {
        XZST_03EcritureTrace(XZSTC_WARNING,
            "XZSC_Synchronisation: erreur execution cb XDM_TSE_MAJD");
      }
      TipcMsgDestroy(vl_Msg);
    }

    /* consultation du time-out */
    vl_FinAttente = XZSS_15ConsulterTimeout();

  } while (!(vl_MAJMRecu && vl_MAJERecu && vl_MAJCRecu && vl_MAJDRecu) &&
      !(vl_FinAttente));


  /* Suppression de l'alarme */
  XZSS_14ArmerTimeout(0);

  /* si les messages n'ont pas tous ete recus, la tache est arretee */
  if (!(vl_MAJMRecu && vl_MAJERecu && vl_MAJCRecu && vl_MAJDRecu))
  {
    XZSC_Sortir();
  }

  XZST_03EcritureTrace(XZSTC_FONCTION,
      "  XZSC_Synchronisation : OUT  Code Retour %d \n",vl_ValRet);

#endif
  return(vl_ValRet);

}	/* Fin XZSC_Synchronisation */




/* definition de fonctions externes */



/*X*/
/* Fichier : @(#)xzsc.c	1.23      Release : 1.23        Date : 09/21/95
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * NomTache	  : nom de la tache provenant du "main"
 * 	       	de la tache.
 * AutoriserTime	  : XZSCC_AUTORISER ou XZSCC_INTERDIR
 *		(XZSCC_INTERDIR pour tache temporaire).
 * AutoriserTrace  : XZSCC_AUTORISER ou XZSCC_INTERDIR.
 * AutoriserSynchro: XZSCC_AUTORISER ou XZSCC_INTERDIR
 *		(XZSCC_INTERDIR pour tache temporaire).
 * FonctionUtil	  : pointeur sur la fonction utilisateur
 *		de sortie de la tache.
 *
 * Fonction
 * Cette fonction teste si l'indicateur XZSCV_InitOk
 * est positionne a 0. Sinon elle renvoie XDC_NOK.
 *
 * Elle initialise la variable XZSCV_NomTache avec
 * "NomTache"et desabonne la tache du datagroup "_time".
 *
 * Cette fonction initialise les variables globales de
 * la tache en recuperant leur valeur dans le fichier
 * XDF_NSUP_NomMachine par la fonction
 * sx_machine_lecture (structure SXT_Machine retournee)
 * (DCG Tome 2.1 - Description des modules communs) :
 * - Nom de la machine : variable XZSCV_NomMachine
 * - Nom du peripherique de sauvegarde : variable
 *   XZSCV_NomPeriphSR
 * - Nom de la machine d'impression : variable
 *   XZSCV_NomMachineImp
 * - Nom de l'imprimante : variable XZSCV_NomImprimante
 * - Nom du site : variable XZSCV_NomSite
 * - Chemin repertoire trace : variable XZSCV_PathTrace
 * - Taille maximale fichier trace : variable
 *   XZSCV_MaxTailleFicTrace
 * - Periode des tests du reseau : variable
 *   XZSCV_PeriodeTestReseau
 * - Type de la machine : variable XZSCV_TypeMachine
 *
 * Elle recherche les options RTworks de la tache dans
 * le fichier de configuration de la tache
 * XDF_NSUP_NomMachine par la fonction sx_tache_lecture
 * (structure SXT_Tache retournee)
 * (DCG Tome 2.1  - Description des modules communs) et
 * initialise les variables suivantes :
 * - La taille du fichier trace : variable
 *   XZSCV_TailleFicTrace
 *
 * Les options sont ensuite activees et la tache se
 * connecte au serveur RTworks.
 *
 * Si le nom de la tache n'existe pas (tache temporaire
 * non geree par le noyau de supervision) , les options
 * RTworks retenues seront celles d'une des taches du
 * fichier de configuration. Ces options retenues sont en
 * fait les memes pour toutes les taches de la machine.
 *
 * Elle arme un callback sur reception du message
 * XDM_TSP_EXIT.
 * Si le pointeur sur la fonction utilisateur est different
 * de NULL alors le callback est la fonction utilisateur :
 * FonctionUtil( (IN)(T_MSG_HEADER *) 	msg_header,
 *		(OUT)(char *)		msg_data)
 * Les arguments sont specifiques a un callback IPC.
 *
 * Si le pointeur sur la fonction utilisateur est NULL
 * alors le callback est la fonction suivante :
 * XZSC_Sortir(  (IN) (T_MSG_HEADER *) 	msg_header,
 * 		(IN) (char *)		 msg_data)
 * 	Ce callback interne appelle la primitive
 *	XZST_02 pour fermer le fichier trace si
 *	celui-ci est ouvert et effectue un exit.
 *	Les arguments sont specifiques a un
 *	callback IPC.
 * Elle appelle la primitive XZSE_10 pour permettre la
 * mise a jour des modes de la station et de l'etat de
 * la machine.
 *
 * Si AutoriserTrace = XZSCC_AUTORISER
 *    Elle appelle la primitive XZST_01 pour ouvrir le
 *    fichier trace.
 *    Elle appelle la primitive XZST_07 pour permettre
 *    la telecommande du niveau de trace.
 *    Elle appelle la primitive XZST_08 pour forcer le
 *    niveaux de trace a XZSTC_FATAL | XZSTC_WARNING.
 * Fsi
 * Si AutoriserTime = XZSCC_AUTORISER
 *    Elle appelle la primitive XZSM_06 pour autoriser
 *    la reception de l'horodate .
 * Fsi
 *
 * De plus la fonction oriente les messages recus vers
 * les fichiers traces. Elle utilise pour cela la fonction
 * TipcSetDefaultReceive avec la fonction XZSC_RecepMsgDefaut.
 * XZSC_RecepMsgDefaut ( (IN) (T_MSG_HEADER *) msg_header,
 *			(IN) (char *)	      msg_data)
 * Ce callback interne appelle la primitive XZST_03 pour
 * ecrire dans le fichier trace si celui-ci est ouvert.
 *
 * Redirection de la sortie standard (stdout) et de la
 * sortie d'erreur standard (stderr) sur /dev/console
 * (mecanisme SYS8)
 *
 * La fonction s'abonne ensuite au datagroup
 * XDG_NomMachine_NomTache.
 * Si AutoriserSynchro = XZSCC_AUTORISER
 *       Armement d'une alarme de 10 secondes, avec
 *	la fonction XZSC_Alarme.
 *       Attente non bloquante des messages du RTserver
 *	(mecanisme RTS4).
 *       Tant que l'on n'a pas recu les messages du type :
 *                       XDM_TSE_MAJM
 *                       XDM_TSE_MAJE
 *                       XDM_TSE_MAJC
 *                       XDM_TSE_MAJD
 *       Ftantque.
 *       Suppression de l'alarme.
 * Fsi
 *
 * XZSC_Alarme( )
 * Ce callback interne appelle la primitive XZST_02
 * pour fermer le fichier trace si celui-ci est ouvert,
 * supprime l'alarme et effectue un exit de la tache.
 * Si AutoriserTime = XZSCC_AUTORISER
 *        Elle appelle la primitive XZSM_05 pour effectuer
 *	 une demande d'horodate .
 * Fsi
 *
 * La fonction positionne l'indicateur XZSCV_InitOk a 1
 *
 * Codes retour
 * XZSCC_NOM_INV		Nom passe en argument egal a NULL.
 * XDC_NOK 	Initialisation deja effectuee.
 * XZSCC_FIC_INV 	Erreur lecture fichier de
 *			configuration du noyau.
 * XDC_OK		ok
 *
 * Mecanismes
 * Rtworks : RTS3 , RTS9 et RTS10
 * Systeme : SYS8
 ******************************************************/

int XZSC_01InitCnxSup( char *NomApplication,
    XZSCT_NomTache va_NomTache,
    int AutoriserTime,
    int AutoriserTrace,
    int AutoriserSynchro,
    void (*FonctionUtil)(),
    char *DestiStd)
{
  static char *version="$Id: xzsc.c,v 1.42 2021/05/04 13:28:02 pc2dpdy Exp $ : XZSC_01InitCnxSup";

  int			vl_IndiceMsg	= 0;
  XZSCT_Datagroup	DGMachineTache 	= "\0";
  int			vl_ValRet 	= XDC_OK;
  char *		pl_P ;

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      "   XZSC_01InitCnxSup : in  \
      Nom Appli %s -- Nom Tache %s -- Autoriser Time %d -- \
      Autoriser Trace  %d -- Autoriser Synchro %d -- \
      Std out %s \n",NomApplication, va_NomTache, AutoriserTime,
      AutoriserTrace, AutoriserSynchro, DestiStd);
printf(
      "   XZSC_01InitCnxSup : in  \
      Nom Appli %s -- Nom Tache %s -- Autoriser Time %d -- \
      Autoriser Trace  %d -- Autoriser Synchro %d -- \
      Std out %s \n",NomApplication, va_NomTache, AutoriserTime,
      AutoriserTrace, AutoriserSynchro, DestiStd);

  if(XZSCV_InitOk == XDC_FAUX)
  {
    if (va_NomTache != NULL)
    {
      /* eventuellement, ne recupere que la partie */
      strncpy(XZSCV_NomTache
          ,((pl_P = strrchr(va_NomTache,'/')) == NULL ? va_NomTache:pl_P+1)
          ,sizeof(XZSCV_NomTache)-1
      ) ;
      vl_ValRet = XZSC_InitDescNoyau();

      /* creation des types de message RTWorks */
      if(vl_ValRet == XDC_OK)
      {
        for (vl_IndiceMsg = 0; tg_Mtb[vl_IndiceMsg].Nom[0] != '\0' ; vl_IndiceMsg++)
        {
          TipcMtCreate(tg_Mtb[vl_IndiceMsg].Nom,
              tg_Mtb[vl_IndiceMsg].Id,
              tg_Mtb[vl_IndiceMsg].Grammaire);
        }
      }
#if 0
      /* Redirection de la sortie standard et de la sortie d'erreur
	    standard */
      if ((vl_ValRet == XDC_OK) && (DestiStd != NULL))
      {
        if((freopen(DestiStd, "w", stdout) == NULL) ||
            (freopen(DestiStd, "w", stderr) == NULL))
        {
          vl_ValRet = XZSCC_SYS_INV;
        }
      }
#endif

      if (vl_ValRet == XDC_OK)
      {
        /* Connexion de la tache */
        if (NomApplication == NULL)
        {
          vl_ValRet = XZSC_ConnexionTache(XDC_NOM_APPLICATION);
        }
        else
        {
          vl_ValRet = XZSC_ConnexionTache(XDC_NOM_APPLICATION);
          /*
	       vl_ValRet = XZSC_ConnexionTache(NomApplication);*/
        }
      }
      if (vl_ValRet == XDC_OK)
      {
        /* Mise en place de la fonction de sortie utilisateur */
        vl_ValRet = XZSC_FonctionSortie(FonctionUtil);
      }

      if (vl_ValRet == XDC_OK)
      {
        /* Mise a jour des modes de la station et
         *			de l'etat de la machine */
        vl_ValRet = XZSE_10AutoriseMajModeEtat();
      }

#if 0
      /* Redirection de la sortie standard et de la sortie d'erreur
	    standard */
      if ((vl_ValRet == XDC_OK) && (DestiStd != NULL))
      {
        if((freopen(DestiStd, "w", stdout) == NULL) ||
            (freopen(DestiStd, "w", stderr) == NULL))
        {
          vl_ValRet = XZSCC_SYS_INV;
        }
      }
#endif

      /* Abonnement de la fonction au datagroup XDG_NomMachine_NomTache */
      if (vl_ValRet == XDC_OK)
      {
        XDG_EncodeDG2 (DGMachineTache, XZSCV_NomMachine,
            XZSCV_NomTache);
        if ( XDG_JoindreDG(DGMachineTache) != T_TRUE)
        {
	        XZST_03EcritureTrace(XZSTC_WARNING,"KO Abonnement de la fonction au datagroup XDG_NomMachine_NomTache %s\n",DGMachineTache);
          vl_ValRet = XZSCC_RTW_INV;
        }
      }
      /* Abonnement de la fonction au datagroup XDG_NomMachine */
      if (vl_ValRet == XDC_OK)
      {
        if ( XDG_JoindreDG(XZSCV_NomMachine) != T_TRUE)
        {
	  //printf("KO2");
          vl_ValRet = XZSCC_RTW_INV;
        }
     }

      /* Affectation de l'indicateur d'initialisation */
      if(vl_ValRet == XDC_OK)
      {
        XZSCV_InitOk = XDC_VRAI;
      }

      /* Mise en place eventuelle du systeme de trace */
      
      if ((vl_ValRet == XDC_OK) && (AutoriserTrace == XZSCC_AUTORISER))
      {
        vl_ValRet = XZSC_InitTrace();
      }
      
      /*B Envoie des msg permettant de fournir aux taches
       *	tsaplg et tsetat le fait qu'une mouvelle taches est
       *	surveillee  */
      XZSC_EnvoieSurveillance(va_NomTache, XZSCV_NomSite);
      /* Mise en place eventuelle du systeme de reception de l'horodate */
      if ((vl_ValRet == XDC_OK) && (AutoriserTime == XZSCC_AUTORISER))
      {
        vl_ValRet = XZSC_InitHorodate();
      }

      /* synchronisation eventuelle de la tache */
      if ((vl_ValRet == XDC_OK) && (AutoriserSynchro == XZSCC_AUTORISER))
      {
        vl_ValRet = XZSC_Synchronisation();
      }
	else 
      {
      	printf("PB XZSC_InitHorodate	");
      }
      /*A Memorisation de l'administartion Systeme */
      XZSA_20InitAdmin();

    }
    else
    {
      /* Nom de la tache indefini */
      vl_ValRet = XZSCC_NOT_INV;
    }
  }
  else
  {
    /* initialisation deja effectuee */
    vl_ValRet = XZSCC_INI_INV;
  }

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      "XZSC_01InitCnxSup : OUT   Code Retour  %d \n", vl_ValRet);
  return(vl_ValRet);

}	/* Fin XZSC_01InitCnxSup */


/*X*/
/* Fichier : @(#)xzsc.c	1.23      Release : 1.23        Date : 09/21/95
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * Priorite :	Priorite de la machine (significatif
 *		pour un SD ou un SC).
 *
 * Fonction
 * Cette fonction retourne le contenu de la variable
 * XZSCV_Priorite.
 *
 * Codes retour
 * XDC_NOK	Initialisation non effectuee
 *		(XZSCV_InitOk = 0).
 * XDC_OK	ok
 *
 * Mecanismes
 ******************************************************/

int XZSC_02Priorite(int *Priorite)
{   
  static char *version="$Id: xzsc.c,v 1.42 2021/05/04 13:28:02 pc2dpdy Exp $ : XZSC_02Priorite";

  int vl_ValRet = XDC_OK; /* valeur de retour de la fonction */


  XZST_03EcritureTrace(XZSTC_INTERFACE,
      "    XZSC_02Priorite : IN \n");


  if (XZSCV_InitOk == XDC_VRAI)
  {
    *Priorite = XZSCV_Priorite;
  }
  else
  {
    vl_ValRet = XZSCC_INI_INV;
  }

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      "    XZSC_02Priorite : OUT   Code Retour %d -- Priorite %d \n",
      vl_ValRet, *Priorite);

  return(vl_ValRet);

}	/* Fin XZSC_02Priorite */


/*X*/
/* Fichier : @(#)xzsc.c	1.23      Release : 1.23        Date : 09/21/95
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * Periode :	Periode des tests du reseau pour une
 *		machine de type SD ou SC.
 *
 * Fonction
 * Cette fonction retourne le contenu de la variable
 * XZSCV_PeriodeTestReseau.
 *
 * Codes retour
 * XDC_NOK	Initialisation non effectuee
 *		(XZSCV_InitOk = 0).
 * XDC_OK	ok
 *
 * Mecanismes
 ******************************************************/

int XZSC_03PeriodeTestReseau(int *Periode)
{   
  static char *version="$Id: xzsc.c,v 1.42 2021/05/04 13:28:02 pc2dpdy Exp $ : XZSC_03PeriodeTestReseau";

  int vl_ValRet = XDC_OK; /* valeur de retour de la fonction */


  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_03PeriodeTestReseau : IN  \n");


  if (XZSCV_InitOk == XDC_VRAI)
  {
    *Periode = XZSCV_PeriodeTestReseau;
  }
  else
  {
    vl_ValRet = XZSCC_INI_INV;
  }

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_03PeriodeTestReseau : OUT   Code Retour %d -- Periode %d \n",
      vl_ValRet, *Periode );

  return(vl_ValRet);

}	/* Fin XZSC_03PeriodeTestReseau */



/*X*/
/* Fichier : @(#)xzsc.c	1.23      Release : 1.23        Date : 09/21/95
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * NomSite :	Nom du site de la machine.
 *
 * Fonction
 * Cette fonction retourne le contenu de la variable
 * XZSCV_NomSite.
 *
 * Codes retour
 * XDC_NOK 	Initialisation non effectuee
 *			(XZSCV_InitOk = 0).
 * XDC_OK		ok
 *
 * Mecanismes
 ******************************************************/

int XZSC_04NomSite(XZSCT_NomSite NomSite)
{
  static char *version="$Id: xzsc.c,v 1.42 2021/05/04 13:28:02 pc2dpdy Exp $ : XZSC_04NomSite";

  int vl_ValRet = XDC_OK; /* valeur de retour de la fonction */


  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_04NomSite : IN \n");


  if (XZSCV_InitOk == XDC_VRAI)
  {
    strcpy(NomSite, XZSCV_NomSite);
  }
  else
  {
    vl_ValRet = XZSCC_INI_INV;
  }

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_04NomSite : OUT  Code Retour %d -- Site %s \n",
      vl_ValRet, NomSite);

  return(vl_ValRet);

}	/* Fin XZSC_04NomSite */



/*X*/
/* Fichier : @(#)xzsc.c	1.23      Release : 1.23        Date : 09/21/95
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * TypeMachine :	Type de la machine XZSCC_TYPEM_ST,
 *		XZSCC_TYPEM_SD ou XZSCC_TYPEM_SC.
 *
 * Fonction
 * Cette fonction retourne le contenu de la variable
 * XZSCV_TypeMachine.
 *
 * Codes retour
 * XDC_NOK		Initialisation non effectuee
 *			(XZSCV_InitOk = 0).
 * XDC_OK 		ok
 *
 * Mecanismes
 ******************************************************/

int XZSC_05TypeMachine(int *TypeMachine)
{
  static char *version="$Id: xzsc.c,v 1.42 2021/05/04 13:28:02 pc2dpdy Exp $ : XZSC_05TypeMachine";

  int vl_ValRet = XDC_OK; /* valeur de retour de la fonction */


  XZST_03EcritureTrace(XZSTC_DEBUG1, " XZSC_05TypeMachine : IN \n");

  if (XZSCV_InitOk == XDC_VRAI)
  {
    *TypeMachine = XZSCV_TypeMachine;
  }
  else
  {
    vl_ValRet = XZSCC_INI_INV;
  }

  XZST_03EcritureTrace(XZSTC_DEBUG1, " XZSC_05TypeMachine : OUT \
      Type Mach %d -- Code Retour %d \n", *TypeMachine,vl_ValRet);

  return(vl_ValRet);

}	/* Fin XZSC_05TypeMachine */


/*X*/
/* Fichier : @(#)xzsc.c	1.23      Release : 1.23        Date : 09/21/95
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * NomTache : Nom de la tache.
 *
 * Fonction
 * Cette fonction retourne le contenu de la variable
 * XZSCV_NomTache.
 *
 * Codes retour
 * XDC_NOK	Initialisation non effectuee
 *		(XZSCV_InitOk = 0).
 * XDC_OK	ok
 *
 * Mecanismes
 ******************************************************/

int XZSC_06NomTache(XZSCT_NomTache NomTache)
{
  static char *version="$Id: xzsc.c,v 1.42 2021/05/04 13:28:02 pc2dpdy Exp $ : XZSC_06NomTache";

  int vl_ValRet = XDC_OK; /* valeur de retour de la fonction */


  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_06NomTache : IN \n");

  if (XZSCV_InitOk == XDC_VRAI)
  {
    strcpy(NomTache, XZSCV_NomTache);
  }
  else
  {
    vl_ValRet = XZSCC_INI_INV;
  }

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_06NomTache : OUT  Code Retour %d -- Tache %s \n",
      vl_ValRet, NomTache);

  return(vl_ValRet);

}	/* Fin XZSC_06NomTache */



/*X*/
/* Fichier : @(#)xzsc.c	1.23      Release : 1.23        Date : 09/21/95
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * NomMachine : Nom de la machine.
 *
 * Fonction
 * Cette fonction retourne le contenu de la variable
 * XZSCV_NomMachine.
 *
 * Codes retour
 * XDC_NOK	Initialisation non effectuee
 *		(XZSCV_InitOk = 0).
 * XDC_OK	ok
 *
 * Mecanismes
 ******************************************************/

int XZSC_07NomMachine(XZSCT_NomMachine NomMachine)
{
  static char *version="$Id: xzsc.c,v 1.42 2021/05/04 13:28:02 pc2dpdy Exp $ : XZSC_07NomMachine";

  int vl_ValRet = XDC_OK; /* valeur de retour de la fonction */


  XZST_03EcritureTrace(XZSTC_DEBUG1, "  XZSC_07NomMachine : IN \n");


  if (XZSCV_InitOk == XDC_VRAI)
  {
    strcpy(NomMachine, XZSCV_NomMachine);
  }
  else
  {
    vl_ValRet = XZSCC_INI_INV;
  }

  XZST_03EcritureTrace(XZSTC_DEBUG1, "  XZSC_07NomMachine : OUT \
      Nom Mach %s -- Code Retour %d \n", NomMachine,vl_ValRet);

  return(vl_ValRet);

}	/* Fin XZSC_07NomMachine */



/*X*/
/* Fichier : @(#)xzsc.c	1.23      Release : 1.23        Date : 09/21/95
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * NomPeriph : 	Nom du peripherique de sauvegarde /
 *		restauration.
 *
 * Fonction
 * Cette fonction retourne le contenu de la variable
 * XZSCV_NomPeriphSR.
 *
 * Codes retour
 * XDC_NOK	Initialisation non effectuee
 *		(XZSCV_InitOk = 0).
 * XDC_OK	 ok
 *
 * Mecanismes
 ********************************************************/

int XZSC_08NomPeriphSR(XZSCT_NomPeriph NomPeriph)
{
  static char *version="$Id: xzsc.c,v 1.42 2021/05/04 13:28:02 pc2dpdy Exp $ : XZSC_08NomPeriphSR";

  int vl_ValRet = XDC_OK; /* valeur de retour de la fonction */


  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_08NomPeriphSR  : IN \n");

  if (XZSCV_InitOk == XDC_VRAI)
  {
    strcpy(NomPeriph, XZSCV_NomPeriphSR);
  }
  else
  {
    vl_ValRet = XZSCC_INI_INV;
  }

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_08NomPeriphSR  : OUT   Code Retour %d -- Nom Peri %s \n",		vl_ValRet, NomPeriph);

  return(vl_ValRet);

}	/* Fin XZSC_08NomPeriphSR */



/*X*/
/* Fichier : @(#)xzsc.c	1.23      Release : 1.23        Date : 09/21/95
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * NomMachineImp :	Nom de la machine pilotant
 *			l'imprimante.
 *
 * Fonction
 * Cette fonction retourne le contenu de la variable
 * XZSCV_NomMachineImp.
 *
 * Codes retour
 * XDC_NOK	Initialisation non effectuee
 *		(XZSCV_InitOk = 0).
 * XDC_OK	ok
 *
 * Mecanismes
 ********************************************************/

int XZSC_10NomMachineImp(XZSCT_NomMachine NomMachineImp)
{
  static char *version="$Id: xzsc.c,v 1.42 2021/05/04 13:28:02 pc2dpdy Exp $ : XZSC_10NomMachineImp";

  int vl_ValRet = XDC_OK; /* valeur de retour de la fonction */


  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_10NomMachineImp  : IN  \n");


  if (XZSCV_InitOk == XDC_VRAI)
  {
    strcpy(NomMachineImp, XZSCV_NomMachineImp);
  }
  else
  {
    vl_ValRet = XZSCC_INI_INV;
  }

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_10NomMachineImp  : OUT  Code Retour %d --  Imp %s \n",
      vl_ValRet, NomMachineImp);

  return(vl_ValRet);

}	/* Fin XZSC_10NomMachineImp */



/*X*/
/* Fichier : @(#)xzsc.c	1.23      Release : 1.23        Date : 09/21/95
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * NomImprimante :	Nom de l'imprimante affectee a
 *			la station de travail.
 *
 * Fonction
 * Cette fonction retourne le contenu de la variable
 * XZSCV_NomImprimante
 *
 * Codes retour
 * XDC_NOK	Initialisation non effectuee
 *		(XZSCV_InitOk = 0).
 * XDC_OK	ok
 *
 * Mecanismes
 ********************************************************/

int XZSC_11NomImprimante(XZSCT_NomPeriph NomImprimante)
{
  static char *version="$Id: xzsc.c,v 1.42 2021/05/04 13:28:02 pc2dpdy Exp $ : XZSC_11NomImprimante";

  int vl_ValRet = XDC_OK; /* valeur de retour de la fonction */


  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_11NomImprimante  : IN \n");


  if (XZSCV_InitOk == XDC_VRAI)
  {
    strcpy(NomImprimante, XZSCV_NomImprimante);
  }
  else
  {
    vl_ValRet = XZSCC_INI_INV;
  }

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_11NomImprimante  : OUT   Code Retour %d -- Impr %s \n",
      vl_ValRet, NomImprimante);

  return(vl_ValRet);

}	/* Fin XZSC_11NomImprimante */




/*X*/
/* Fichier : @(#)xzsc.c	1.23      Release : 1.23        Date : 09/21/95
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * PathTrace :	 Chemin du repertoire des fichiers
 *		 traces.
 * TailleFic :	 Taille du fichier trace.
 * MaxTailleFic : Taille maximale des fichiers traces
 *		 de la machine.
 *
 * Fonction
 * Cette fonction retourne le contenu des variables
 * XZSCV_PathTrace, XZSCV_TailleFicTrace et
 * XZSCV_MaxTailleFicTrace.
 *
 * Codes retour
 * XDC_NOK	Initialisation non effectuee
 *		(XZSCV_InitOk = 0).
 * XDC_OK	ok
 *
 * Mecanismes
 ********************************************************/

int XZSC_13InfoFicTrace(char *PathTrace,
    int *TailleFic,
    int *MaxTailleFic)
{

  int vl_ValRet = XDC_OK; /* valeur de retour de la fonction */


  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_13InfoFicTrace : IN \n");


  if (XZSCV_InitOk == XDC_VRAI)
  {
    strcpy(PathTrace, XZSCV_PathTrace);
    *TailleFic = XZSCV_TailleFicTrace;
    *MaxTailleFic = XZSCV_MaxTailleFicTrace;
  }
  else
  {
    vl_ValRet = XZSCC_INI_INV;
  }

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_13InfoFicTrace : OUT   Code Retour %d -- \
      Path %s -- Taille %d -- Max Taille %d \n",
      vl_ValRet, PathTrace, *TailleFic, *MaxTailleFic);

  return(vl_ValRet);

}	/* Fin XZSC_13InfoFicTrace */


/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Permet de recuperer le nom de l'imprimante de secours
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int  	XZSC_15NomImprimanteSecours(
    XZSCT_NomPeriph NomImprimanteSecours)

/*
 * ARGUMENTS EN ENTREE :
 *   Aucun
 *
 *
 * ARGUMENTS EN SORTIE :
 *   Nom de l'imprimante de Secours
 *
 *
 * CODE RETOUR :
 *   XDC_OK,  XZSCC_INI_INV
 *
 * CONDITION D'UTILISATION
 *    Aucune
 *
 * FONCTION
 *
------------------------------------------------------*/
{

  int vl_ValRet = XDC_OK; /* valeur de retour de la fonction */


  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_15NomImprimanteSecours : IN  \n");


  if (XZSCV_InitOk == XDC_VRAI)
  {
    strcpy(NomImprimanteSecours, XZSCV_NomImprimanteSec);
  }
  else
  {
    vl_ValRet = XZSCC_INI_INV;
  }

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_15NomImprimanteSecours : OUT   Code Retour %d \
      Impr Sec %s \n", vl_ValRet, NomImprimanteSecours);

  return(vl_ValRet);

}	/*  XZSC_15NomImprimanteSecours */



/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Permet de recuperer le nom de l'imprimante de secours
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int     XZSC_16ProfilMachine(
    XZSCT_TypeProfil * TypeProfil)

/*
 * ARGUMENTS EN ENTREE :
 *   Aucun
 *
 *
 * ARGUMENTS EN SORTIE :
 *   Type du Profil
 *
 *
 * CODE RETOUR :
 *   XDC_OK,  XZSCC_INI_INV
 *
 * CONDITION D'UTILISATION
 *    Aucune
 *
 * FONCTION
 *
------------------------------------------------------*/
{

  int vl_ValRet = XDC_OK; /* valeur de retour de la fonction */


  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_16ProfilMachine : IN \n");


  if (XZSCV_InitOk == XDC_VRAI)
  {
    *TypeProfil = XZSCV_TypeProfil;
  }
  else
  {
    vl_ValRet = XZSCC_INI_INV;
  }

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      " XZSC_16ProfilMachine : OUT  Code Retour %d -- Profil %s \n",
      vl_ValRet, *TypeProfil);

  return(vl_ValRet);


}       /*  XZSC_16_ProfilMachine **/


/*X*/
/* Fichier : @(#)xzsc.c	1.23      Release : 1.23        Date : 09/21/95
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * Duree :
 *
 * Fonction
 * Cette fonction retourne le contenu de la variable
 * XZSCV_DureeAbsenceCourte.
 *
 * Codes retour
 * XDC_NOK       Initialisation non effectuee
 *               (XZSCV_InitOk = 0).
 * XDC_OK        ok
 *
 * Mecanismes
 ******************************************************/

int XZSC_17DureeAbsenceCourte(int *Duree)
{

  int vl_ValRet = XDC_OK; /* valeur de retour de la fonction */

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      "XZSC_17DureeAbsenceCourte  : IN \n");

  if (XZSCV_InitOk == XDC_VRAI)
  {
    *Duree = XZSCV_DureeAbsenceCourte;
  }
  else
  {
    vl_ValRet = XDC_NOK;
  }

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      "XZSC_17DureeAbsenceCourte  : OUT  -- Abscence Courte %d \
      -- Code Retour %d \n",*Duree,vl_ValRet);

  return(vl_ValRet);

}	/* Fin XZSC_17DureeAbsenceCourte */




/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Permet de recuperer le nom de l'imprimante de secours
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int     XZSC_20TestConnexion()

/*
 * ARGUMENTS EN ENTREE :
 *   Aucun
 *
 *
 * ARGUMENTS EN SORTIE :
 *	Aucun
 *
 * CODE RETOUR :
 *   XDC_OK,  XDC_NOK
 *
 * CONDITION D'UTILISATION
 *    Aucune
 *
 * FONCTION
 *
------------------------------------------------------*/
{

  int vl_ValRet = XDC_OK; /* valeur de retour de la fonction */

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      "XZSC_20TestConnexion  : IN \n");

vl_ValRet = XDC_OK;
#ifndef _TIBCO_EMS
  if (TipcSrvIsRunning())
  {
    vl_ValRet = XDC_OK;
  }
  else
  {
    vl_ValRet = XDC_NOK;
  }
#endif

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      "XZSC_20TestConnexion  : OUT   Code Retour %d  \n",
      vl_ValRet);

  return(vl_ValRet);


}       /*  XZSC_20TestConnexion */


/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Permet de recuperer le nom de l'imprimante de secours
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int    XZSC_21NomMachineRTserver(
    XZSCT_NomMachine	va_NomMachine)

/*
 * ARGUMENTS EN ENTREE :
 *   Aucun
 *
 *
 * ARGUMENTS EN SORTIE :
 * 	Nom du Rtserver sur lequel on est connecte
 *
 * CODE RETOUR :
 *   XDC_OK 	: ok
 *   XDC_NOK 	: Pb avec primitive System
 *   XZSCC_INI_INV	: Initialisation non effectuee
 *   XZSCC_RTW_INV	: Pb avec primitive RTWorks
 *
 * CONDITION D'UTILISATION
 *    Aucune
 *
 * FONCTION
 *
------------------------------------------------------*/
{

  /* valeur de retour de la fonction */
  int 			vl_ValRet 	= XDC_OK;
  T_INT4		vl_Socket	= 0;
  struct sockaddr_in 	vl_Addr;
#if defined (__ia64)
  int 			vl_LongAddr 	= sizeof(struct sockaddr_in);
#else
  socklen_t		vl_LongAddr 	= sizeof(struct sockaddr_in);
#endif
  struct hostent * 	vl_Host;


#ifndef _TIBCO_EMS
  XZST_03EcritureTrace(XZSTC_INTERFACE,
      "XZSC_21... Appel aucun argument \n");

  if (XZSCV_InitOk == XDC_VRAI)
  {

    /*A
     **  	Recuperation de l'identificateur de Socket
     **/
    if ( !TipcSrvGetSocket( &vl_Socket))
    {
      XZST_03EcritureTrace(XZSTC_WARNING,
          "XZSC21... Recuperation Socket a echouee \n");

      return (XZSCC_RTW_INV);
    }

    /*
     **  Recuperation du Nom du RTServer
     **/
    if (getpeername ( vl_Socket, (struct sockaddr *) &vl_Addr, &vl_LongAddr ) )
    {
      XZST_03EcritureTrace(XZSTC_WARNING,
          "XZSC_21... Recuperation Addresse a echouee %d \n",
          errno);

      return (XDC_NOK);
    }
    vl_Host = gethostbyaddr( (char *) &(vl_Addr.sin_addr),
        sizeof(struct in_addr), AF_INET);
    strcpy( (char*)va_NomMachine, vl_Host->h_name);
  }
  else
  {
    vl_ValRet =  XZSCC_INI_INV;
  }

  XZST_03EcritureTrace(XZSTC_INTERFACE,
      "XZSC_21... Nom RTServer %s -- Code Retour %d \n",
      va_NomMachine, vl_ValRet);

#endif
  return(vl_ValRet);


}       /*  XZSC_21NomMachineRTserver */


/******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 * Adresse chaine Nom de l'alias
 *
 * Fonction
 * Cette fonction retourne le nom de l'alias contenu dans /etc/hosts
 * ou si nis+ est actif le nom de l'alias dans la base NIS ou si il
 * n'y a pas d'alias le nom de la machine.
 *
 * Codes retour
 * XDC_NOK	erreur
 * XDC_OK	ok
 *
 * Mecanismes
 ********************************************************/

int XZSC_NomAlias(char *pa_NomAlias)
{

  int vl_ValRet = XDC_NOK; /* valeur de retour de la fonction */
  struct  hostent * pl_hostent; /* identifiant de la machine  */

  char vl_NomMachine[50]	= "\0";
  char **pl_p;  /* tableau de pointeur sur alias */


  XZST_03EcritureTrace(XZSTC_FONCTION ,
      "XZSC_NomAlias : IN \n");


  /* recherche du nom de la machine */
  if ( gethostname(vl_NomMachine, 50) == -1)
  {
    vl_ValRet = XDC_NOK;
  }

  /* initialisation de liste */
  sethostent(0);
  while ( (pl_hostent=gethostent()) != NULL)
  {
    if ( !strcmp(vl_NomMachine,pl_hostent->h_name))
    {
      pl_p = pl_hostent->h_aliases;
      /* on prend le dernier alias dans tous les cas*/
      /* tant que le contenu du pointeur n'est pas null */
      while(*pl_p)
      {
        XZST_03EcritureTrace(XZSTC_INFO,
            "XZSC_NomAlias: nom machine %s, alias %s "
            , pl_hostent->h_name,*pl_p);
        strcpy(pa_NomAlias,*pl_p++);
      }


      /* ATTENTION NE PAS INSERER DE BREAK, L'ALIAS N'EST TROUVE
	    QU'A LA DEUXIEME OCCURENCE DU NOM DE MACHINE */

      vl_ValRet = XDC_OK;
    }
  }
  endhostent(); /* liberation de ressources */

  XZST_03EcritureTrace(XZSTC_FONCTION ,
      "XZSC_NomAlias : OUT   Code Retour %d \n",vl_ValRet);

  return(vl_ValRet);

}	/* Fin XZSC_NomAlias */



/*------------------------------------------------------
 * SERVICE RENDU :
 *  Permet de reenregister les taches temporaires
 *	deja lancees, si les taches tsaplg.x et tsetat.x
 *	se relancent
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void XZSC_SynchroMigrazur(
    T_IPC_CONN 			va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA 	pa_Data,
    T_CB_ARG 			pa_Arg)

/*
 * ARGUMENTS EN ENTREE :
 *   Arguments d'une callback
 *
 *
 * ARGUMENTS EN SORTIE :
 *   Aucun
 *
 *
 * CODE RETOUR :
 *   Aucun
 *
 * CONDITION D'UTILISATION
 *   Aucune
 *
 * FONCTION
 *   Sur reception du msg XDM_TSP_MIGRAZUR, si la tache est
 *	temporaire alors on appel al fonction de synchro surveillance
 *	permettant d'enregistrer cette tache au niveau de tsaplg.x
 *	et de tsetat.x
 *
------------------------------------------------------*/
{

  int	vl_ValRet	= XDC_NOK;
#ifndef _TIBCO_EMS
  T_STR	vl_NomSite	= "";
#else
  XDY_NomSite	vl_NomSite      = "";
#endif
  XZST_03EcritureTrace(XZSTC_FONCTION ,
      " XZSC_SynchroMigrazur : IN \n");

  if (!TipcMsgRead(pa_Data -> msg,
      T_IPC_FT_STR,&vl_NomSite,
      NULL))
  {
    XZST_03EcritureTrace(XZSTC_WARNING,
        " Erreur sur reception XDM_TSP_MIGRAZUR \n");
  }

  XZST_03EcritureTrace(XZSTC_MESSAGE,
      " Reception XDM_TSP_MIGRAZUR pour la tache %s -- sur machien %s -- envoye par le site %s \n",
      XZSCV_NomTache, XZSCV_NomMachine, vl_NomSite);

  /*Modifiaction de test 21/09/95. On envoie qulque soit le type de t&ache */
  XZSC_EnvoieSurveillance(XZSCV_NomTache,vl_NomSite);


  /*A Envoie de l'etat  de la Tache */
  XZSE_12EnvoieEtatTache(XZSCV_NomMachine,XZSCV_NomTache,
      XZSEC_ETAT_OK, 10);


  XZST_03EcritureTrace(XZSTC_FONCTION ,
      " XZSC_SynchroMigrazur : OUT \n");

}	/* Fin XZSC_SynchroMigrazur */



/******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 *
 * Fonction
 *
 * Codes retour
 * XDC_NOK	Initialisation non effectuee
 *		(XZSCV_InitOk = 0).
 * XDC_OK	ok
 *
 * Mecanismes
 ******************************************************/

XDY_District XZSC_22NumeroSiteLocal()
{   

  XDY_NomSite vl_site;
  XDY_District vl_numero_site=0;

  int vl_ValRet = XDC_OK; /* valeur de retour de la fonction */


  XZST_03EcritureTrace(XZSTC_FONCTION,
      "    XZSC_22NumeroSiteLocal : IN \n");

  vl_ValRet = XZSC_04NomSite(vl_site);

  if (vl_ValRet != XDC_OK) {
    XZST_03EcritureTrace(XZSTC_WARNING, "XZSC_22NumeroSiteLocal : pb appel a XZSC_04NomSite");
    return (vl_numero_site);
  }

  if (XZSCV_InitOk == XDC_VRAI)
  {
    if (!strcmp(vl_site,XDC_NOM_SITE_CI)) vl_numero_site=XDC_CI;
    else if (!strcmp(vl_site,XDC_NOM_SITE_VC)) vl_numero_site=XDC_VC;
    else if (!strcmp(vl_site,XDC_NOM_SITE_DP)) vl_numero_site=XDC_DP;
    else if (!strcmp(vl_site,XDC_NOM_SITE_CA)) vl_numero_site=XDC_CA;
  }

  XZST_03EcritureTrace(XZSTC_DEBUG1,
      "    XZSC_22NumeroSiteLocal : OUT   NumeroSite %d \n",
      vl_numero_site);

  return(vl_numero_site);
}



char *XZSC23_Nom_From_Numero(XDY_District va_numero)
{
  XDY_NomSite	vl_site;
  XDY_NomSite vl_mon_site;

  XZSC_04NomSite(vl_mon_site);
  switch(va_numero)
  {
    case XDC_CI: strcpy(vg_site,XDC_NOM_SITE_CI);
    break;
    case XDC_VC: strcpy(vg_site,XDC_NOM_SITE_VC);
    break;
    case XDC_DP: strcpy(vg_site,XDC_NOM_SITE_DP);
    break;
    case XDC_CA: strcpy(vg_site,XDC_NOM_SITE_CA);
    break;
    default: strcpy(vg_site,vl_mon_site);
    break;
  }
  return (vg_site);
}

/*****************************************************************************
* Service permettant de programmer un timer toutes les 25 min sur un site donne
******************************************************************************/

extern int XZSC_30ProgKeepAlive(XZSCT_NomSite va_NomSite)
{

  XDY_Datagroup vl_dg = "";
  int vl_ValRet = XDC_OK;
  XZSMT_CleProg vl_cle_timer = "";

  XDG_EncodeDG2(vl_dg, XDG_KEEPALIVE, va_NomSite);

  XDG_EncodeDG2(vl_cle_timer, vl_dg, "timer");

  if ( (vl_ValRet = XZSM_01DemanderProgrammation(0, 60*25,vl_dg, XDM_TSE_KEEPALIVE, vl_cle_timer, 10)) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Erreur demande programmation KEEP ALIVE \n");
    vl_ValRet = XDC_NOK;
  }

  return vl_ValRet;
}
extern int XZSC_30ProgKeepAliveHIS(XZSCT_NomSite va_NomSite)
{

  XDY_Datagroup vl_dg = "";
  int vl_ValRet = XDC_OK;
  XZSMT_CleProg vl_cle_timer = "";

  XDG_EncodeDG2(vl_dg, XDG_KEEPALIVE, va_NomSite);

  XDG_EncodeDG2(vl_cle_timer, vl_dg, "timer");

  if ( (vl_ValRet = XZSM_01DemanderProgrammation(0, 30,vl_dg, XDM_TSE_KEEPALIVE, vl_cle_timer, 10)) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Erreur demande programmation KEEP ALIVE \n");
    vl_ValRet = XDC_NOK;
  }

  return vl_ValRet;
}


/*****************************************************************************
* Service permettant de s'abonner au Data Group KEEP ALIVE sur un site donne
* La CallBack associee fait uniquement une trace si besoin
******************************************************************************/

extern int XZSC_31AbtKeepAlive(XZSCT_NomSite va_NomSite)
{

  XDY_Datagroup vl_dg = "";
  int vl_ValRet = XDC_OK;

  /* Abonnement au data group keep alive */
  XDG_EncodeDG2(vl_dg, XDG_KEEPALIVE, va_NomSite);

  if( !XDG_JoindreDG( vl_dg ) )
  {
    XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", vl_dg );
    vl_ValRet = XZSCC_RTW_INV;
  }

  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_KEEPALIVE),XZSC_process_KEEPALIVE_cb,(T_CB_ARG) NULL) == NULL)
  {
    XZST_03EcritureTrace(XZSTC_FATAL, "Rattachement a la callback XZSC_process_KEEPALIVE_cb non effectue");
    vl_ValRet = XZSCC_RTW_INV;
  }
  return vl_ValRet;
}


extern void  XZSC_process_KEEPALIVE_cb(T_IPC_CONN                 va_cnx,
                                       T_IPC_CONN_PROCESS_CB_DATA pa_data,
                                       T_CB_ARG                   pa_arg)

{
  XZST_03EcritureTrace (XZSTC_INFO, "Execution XZSC_process_KEEPALIVE_cb : Callback vide");
}

extern int XZSC_GetDestinataire(
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_STR *va_Destinataire
)
{
XDY_Datagroup   vl_dg_dest;
XDY_Datagroup	vl_dg_admin_site;
char            *vl_ch;

//        XZST_03EcritureTrace(XZSTC_WARNING,"XZSC_GetDestinataire : IN");

        strcpy( (char *)va_Destinataire, "" );
#ifdef _TIBCO_EMS
        /* Determiner le destinataire */
        TipcMsgGetDest(pa_data->msg, &vl_dg_dest);

//        XZST_03EcritureTrace(XZSTC_WARNING,"TipcMsgGetDest : [%s]",vl_dg_dest);

        /*extraire le nom de la machine cible*/
	      XDG_EncodeDG2 (vl_dg_admin_site, XDG_IAVERT_ADMINISTRATEUR, XZSCV_NomSite);
        if (egale(vl_dg_dest,vl_dg_admin_site))
                strcpy((char *)va_Destinataire, XZSCV_NomSite);
        else
        {
          vl_ch=strrchr(vl_dg_dest,'_');
          if (vl_ch==NULL)
                  strcpy((char *)va_Destinataire, XZSCV_NomSite);
          else
          {
                  if ((!strncmp (vl_ch+1,"PO",2)) ||
                      (!strncmp (vl_ch+1,"SS",2)) ||
                      (!strncmp (vl_ch+1,"SD",2)) ||
                      (!strncmp (vl_ch+1,"SP",2)))
                  {
                          /* c'est le nom de la machine */
                          strcpy((char *)va_Destinataire,vl_ch+1);
                  }
                  else
                  {
                        strcpy((char *)va_Destinataire, "");
                        /* Si ni PO, ni SS, ni SD, ni SP - IMODIF_TFM, ETDP_FCD, IHM_ADM, etc. par exemple - Alors c'est le site qui est destinataire */
                        strcpy((char *)va_Destinataire,XZSCV_NomSite);
                  }
          }
        }
//        XZST_03EcritureTrace(XZSTC_WARNING,"XZSC_GetDestinataire : va_Destinataire en sortie : [%s]", va_Destinataire);
#endif
//        XZST_03EcritureTrace(XZSTC_WARNING,"XZSC_GetDestinataire : OUT");

        return(XDC_OK);
}


/******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 * Fonction
 * Recuperation de l'adresse IP locale
 *
 * Codes retour
 * XDC_NOK	erreur
 * XDC_OK	ok
 ******************************************************
 */
int XZSC_AdresseIP (XDY_Texte va_IP)
{
  XZST_03EcritureTrace(XZSTC_FONCTION, ": In \n");
  struct addrinfo hints, *res, *p;
  int status;
  XDY_AdresseIP ipstr[16];
  char whoami[128];

  XZST_03EcritureTrace(XZSTC_FONCTION, "XZSC_AdresseIP : IN \n");

  memset(&hints, 0, sizeof(hints));

  if (gethostname(whoami, 128)==0)
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, "hostname: %s", whoami);
  }

  if ((status = getaddrinfo(whoami, NULL, &hints, &res)) != 0)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "getaddrinfo: %s", gai_strerror(status));
    return (XDC_NOK);
  }
  
  for(p = res;p != NULL; p = p->ai_next)
  {
    void *addr;
    
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
    addr = &(ipv4->sin_addr);

    // Convertit IP en string 
    inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
    
    XZST_03EcritureTrace(XZSTC_FONCTION, "IP trouvee avant conversion: %s", ipstr);

    if (strncmp(ipstr, "127.0", 5 ))
    {
      strcpy(va_IP, ipstr);
    }
  }
  // Libere les ressources
  freeaddrinfo(res); 
  
  XZST_03EcritureTrace(XZSTC_FONCTION, "IP trouvee: %s", va_IP);

  XZST_03EcritureTrace(XZSTC_FONCTION, "XZSC_AdresseIP : OUT \n");
	return (XDC_OK);
}
