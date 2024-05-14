/* Fichier : $Id: itre_com.c,v 1.17 2021/06/08 18:14:26 pc2dpdy Exp $        Release : $Revision: 1.17 $      Date : $Date: 2021/06/08 18:14:26 $
******************************************************************************
* STERIA * PROJET MIGRAZUR
******************************************************************************
* MODULE MTRE * FICHIER itre_com.c
******************************************************************************
* AUTEUR JM Guilhou       * DATE DE CREATION 29/09/94
******************************************************************************
* DESCRIPTION DU MODULE :
*
* Ce module contient toutes les fonctions necessaires
* a la communication entre le C et Applix
* connexion, deconnexion, envoi et reception de
* messages sur socket
*
*
******************************************************************************
* HISTORIQUE DES MODIFICATIONS :
* 
* JM GUILHOU	29/09/94	Creation	V1.1
* NIEPCERON	8/12/95		Ajout d'un teste de msg non vide avant envoie a applix v1.8
* JMG	26/06/10 : portage linux dem/934  1.9, 1.10
* JPL	12/11/12 : Separation de l'interface applix et de l'interface ASCII  1.11
* JPL	12/11/12 : Interface Java / Eclipse RCP en ASCII complete  1.12
* JPL	30/11/12 : Prise en compte de messages agglutines en reception ASCII  1.13
* JPL	06/03/13 : Codes de fonctions TEXTUELS echanges avec proto Java ... et plus si pas de Pb.  1.14
* JPL	16/02/18 : Migration Linux 64 bits (DEM 1274) : modification de champs de donnees socket obsoletes  1.15
* JMG	12/06/19 : ajout info_message DEM1338 ODI-C 1.16
* jmg	10/02/21 : MAGISTRA 1.17
* LCL 28/06/22 : Correctif envoi trame GTC trop longue pour variable dans itre_com_Applix_SendPoke 1.18
* LCL 12/07/22 : Correctif envoi messages au CI lorsque machine destinataire=VC ou DP et vg_nomSite=CI
* LCL 24/01/23 : Correctif nom machine vide sur envoi du Poke a Magistra
*****************************************************************************/

/* fichiers inclus */
#include <xzsem.h>
#include <itre_com.h>
#include <xdc.h>
#include <xzic.h>
#include "xzst.h"
extern int vg_titre;

extern XDY_NomMachine   vg_nomMachine;
extern XDY_NomSite      vg_nomSite;

struct chaineMsgDecode
{
  ITRE_COM_TMsgDecode message;
  struct chaineMsgDecode *suivant;
};


/* declarations des variables locales */
static int vm_mode_ASCII = FALSE;



/*****************************************************************************
******************************************************************************
**
**                   FONCTIONS INTERNES D'INTERFACE ASCII
**
******************************************************************************
*****************************************************************************/


/* declarations des variables locales */
static char vm_ASCII_buffer_envoi[ITRE_COM_MAX_STRLEN];


/* declarations des fonctions internes */
static int lireMsgASCIISocket(int fdSocket, ITRE_COM_TMsgDecode *message, int *autreMsg);


/******************************************************
* STERIA * PROJET MIGRAZUR
*******************************************************
* MODULE MTRE           FONCTION lireMsgASCIISocket
*******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Lecture dans une socket d'un message au format ASCII,
* puis decodage du code en tete.
*
* Formatage de la structure de reception en fonction du code message:
* - ITRE_COM_MSG_FIN_TACHE: pas de donnees complementaires
* - ITRE_COM_MSG_RET_VAR_CTX: is.i=transmission directe (XZGC_TRUE | XZGC_FALSE)
*                             is.s=valeur ou nom de fichier
* - ITRE_COM_MSG_RET_TASK: s=valeur de retour
*
******************************************************* 
* SEQUENCE D'APPEL :
*
* int lireMsgASCIISocket (int fdSocket, XZGC_TMsgDecode *message, int *autreMsg)
*
* PARAMETRES EN ENTREE :
*
* fdSocket	: descripteur de fichier pour la socket ouverte
*                 par XZGC_01Connexion
*
* PARAMETRES EN SORTIE :
*
* *message     	: contenu du message
* *autreMsg    	: indicateur de message restant
*
* VALEUR RENDUE :
*
* XZGC_OK	OK.
* XZGC_NOK	Erreur.
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
*
*******************************************************
* ALGORITHME :
*
*******************************************************/

static int lireMsgASCIISocket(int fdSocket, ITRE_COM_TMsgDecode *message, int *autreMsg)
{
	static char vl_ASCII_buffer_recep[ITRE_COM_MAX_STRLEN];
	static int vl_nb_car_recus = 0;				/* Taille donnees disponibles en reception */

	int vl_lgr_msg;
	char *pl_marque_fin;
	char *pl_lexon;

    /* A priori, aucun message ne suit le message recu */
    (*autreMsg) = XDC_FAUX;
 
    /* Si le tampon de stockage des donnees recues est vide alors */
    /* lire les donnees disponibles sur le canal de communication */
    if (vl_nb_car_recus == 0)
    {
	/*! remettre a zero le tampon de reception termine aussi la chaine ASCII recue */
	memset (vl_ASCII_buffer_recep, 0, sizeof (vl_ASCII_buffer_recep));
  //sem_wait(&vg_semaphore); 
	vl_nb_car_recus = read (fdSocket, vl_ASCII_buffer_recep, ITRE_COM_MAX_STRLEN-1);
  //sem_post(&vg_semaphore); 
	if (vl_nb_car_recus < 0)
	    return (XDC_NOK);

	XZST_03EcritureTrace (XZSTC_WARNING, "<<< IHM: '%s'", vl_ASCII_buffer_recep);
    }


    /* Delimiter le premier message termine par le marqueur de fin en tete du tampon */
    pl_marque_fin = strstr (vl_ASCII_buffer_recep, XZIC_MSG_ASC_FIN);
    if (pl_marque_fin != NULL)
    {
	(*pl_marque_fin) = '\0';
    }

    /* Decoder le premier message, en interpretant le premier champ (code) */
    pl_lexon = strtok (vl_ASCII_buffer_recep, XZIC_MSG_ASC_SEP_ELEM);
    if (pl_lexon == NULL)
    {
	message->codeMsg = ITRE_COM_MSG_FIN_TACHE;
    } else {
	sscanf (pl_lexon, "%d", &(message->codeMsg));
    }

    switch(message->codeMsg)
    {
     case ITRE_COM_MSG_FIN_TACHE:
	break;

     case ITRE_COM_MSG_RET_VAR_CTX:
	pl_lexon=strtok(NULL,XZIC_MSG_ASC_SEP_ELEM);
	sscanf(pl_lexon,"%d",&(message->contenuMsg.is.i));
	pl_lexon=strtok(NULL,XZIC_MSG_ASC_SEP_ELEM);
	strcpy(message->contenuMsg.is.s,pl_lexon);

	break;


     case ITRE_COM_MSG_RET_TASK:
	pl_lexon=strtok(NULL,XZIC_MSG_ASC_SEP_ELEM);
	strcpy(message->contenuMsg.s, pl_lexon);

	break;

     case ITRE_COM_MSG_EXEC:
	pl_lexon=strtok(NULL,XZIC_MSG_ASC_SEP_ELEM);
	sscanf(pl_lexon,"%d",&(message->contenuMsg.execMsg.numCmd));
	pl_lexon=strtok(NULL,XZIC_MSG_ASC_SEP_ELEM);
	sscanf(pl_lexon,"%d",&(message->contenuMsg.execMsg.numFct));
	pl_lexon=strtok(NULL,XZIC_MSG_ASC_SEP_ELEM);
	strcpy(message->contenuMsg.execMsg.arg, pl_lexon);

	break;

     case ITRE_COM_MSG_SEND:
	pl_lexon=strtok(NULL,XZIC_MSG_ASC_SEP_ELEM);
	sscanf(pl_lexon,"%d",&(message->contenuMsg.sendMsg.msgType));
	pl_lexon=strtok(NULL,XZIC_MSG_ASC_SEP_ELEM);
	strcpy(message->contenuMsg.sendMsg.dg,pl_lexon);
	pl_lexon=strtok(NULL,XZIC_MSG_ASC_SEP_ELEM);
	strcpy(message->contenuMsg.sendMsg.arg,pl_lexon);

	break;

     default:
	break;
    }


    /* Supprimer le message traite du tampon */
    if (pl_marque_fin == NULL)
    {
	vl_nb_car_recus = 0;
    }
    else
    {
	vl_lgr_msg = (pl_marque_fin - vl_ASCII_buffer_recep) + strlen (XZIC_MSG_ASC_FIN);
	vl_nb_car_recus = vl_nb_car_recus - vl_lgr_msg;

	/* et si des caracteres supplementaires le suivent alors  */
	/* les deplacer en debut de tampon et l'indiquer a l'appelant */
	if (vl_nb_car_recus > 0)
	{
	    memmove (vl_ASCII_buffer_recep, vl_ASCII_buffer_recep + vl_lgr_msg, vl_nb_car_recus);
	    vl_ASCII_buffer_recep[vl_nb_car_recus] = '\0';

	    (*autreMsg) = XDC_VRAI;
	}
    }

    return XDC_OK;
}



/*****************************************************************************
* ESCOTA * PROJET MIGRAZUR / PASTRE
******************************************************************************
* MODULE MTRE           FONCTION itre_com_ASCII_SendPoke
******************************************************************************
* DESCRIPTION DE LA FONCTION :
*
* Envoi du message (ITRE_COM_MSG_SEND_POKE, codePoke, params) dans la socket.
*
******************************************************************************
* SEQUENCE D'APPEL :
*/

static int itre_com_ASCII_SendPoke (int fdSocket, int codePoke,
                                       char *params[], int nParams, char *va_machine)
{
    static char *vl_SEPAR = XZIC_MSG_ASC_SEP_ELEM;

    int vl_lg_msg, vl_lg_param;
    int i;
    int vl_cr;

    /* Composition du message: ITRE_COM_MSG_SEND_POKE, codePoke, params */
/*
    vl_lg_msg = sprintf (vm_ASCII_buffer_envoi, "%d%s%d", ITRE_COM_MSG_SEND_POKE,
                            vl_SEPAR, codePoke);
*/
    vl_lg_msg = sprintf (vm_ASCII_buffer_envoi, "%s%s%d", "SENDPOKE",
                            vl_SEPAR, codePoke);
    /* Ajout des parametres */
    for (i = 0;  i < nParams;  i++)
    {
	vl_lg_param = sprintf (vm_ASCII_buffer_envoi + vl_lg_msg, "%s%s", vl_SEPAR, params[i]);
	vl_lg_msg = vl_lg_msg + vl_lg_param;
    }
    /* Ajout du marqueur de fin */
    strcat (vm_ASCII_buffer_envoi, XZIC_MSG_ASC_FIN);
    vl_lg_msg = vl_lg_msg + strlen (XZIC_MSG_ASC_FIN);

    /* Envoi du message */
    vl_cr = itre_com_EmissionSocket (fdSocket, vm_ASCII_buffer_envoi, &vl_lg_msg, ITRE_COM_ATTENTE) ;
    return (vl_cr) ;
}



/*****************************************************************************
* ESCOTA * PROJET MIGRAZUR / PASTRE
******************************************************************************
* MODULE MTRE           FONCTION itre_com_ASCII_NewTask
******************************************************************************
* DESCRIPTION DE LA FONCTION :
*
* Envoi du message (ITRE_COM_MSG_NEW_TASK, synchrone, nomMacro, params)
* dans la socket.
*
******************************************************************************
* SEQUENCE D'APPEL :
*/

static int itre_com_ASCII_NewTask (int fdSocket, char *nomMacro,
                                      char *params, int nParams,
                                      int synchrone, char *retour, int *autreMsg, char *machine)
{
    static char *vl_SEPAR = XZIC_MSG_ASC_SEP_ELEM;

    int vl_lg_msg;
    int vl_cr;

    /* Composition du message: ITRE_COM_MSG_NEW_TASK, synchrone, nomMacro, params */
/*
    vl_lg_msg = sprintf (vm_ASCII_buffer_envoi, "%d%s%d%s%s%s%s%s", ITRE_COM_MSG_NEW_TASK, vl_SEPAR,
                            synchrone, vl_SEPAR, nomMacro, vl_SEPAR, params, XZIC_MSG_ASC_FIN);
*/
    vl_lg_msg = sprintf (vm_ASCII_buffer_envoi, "%s%s%d%s%s%s%s%s", "NEWTASK", vl_SEPAR,
                            synchrone, vl_SEPAR, nomMacro, vl_SEPAR, params, XZIC_MSG_ASC_FIN);

    /* Envoi du message */
    vl_cr = itre_com_EmissionSocket (fdSocket, vm_ASCII_buffer_envoi, &vl_lg_msg, ITRE_COM_ATTENTE) ;
    return (vl_cr) ;
}
static int itre_com_ASCII_InfoMessage (int fdSocket, char *nomMacro
                                      )
{
    static char *vl_SEPAR = XZIC_MSG_ASC_SEP_ELEM;

    int vl_lg_msg;
    int vl_cr;

    /* Composition du message: ITRE_COM_MSG_NEW_TASK, synchrone, nomMacro, params */
    vl_lg_msg = sprintf (vm_ASCII_buffer_envoi, "%s%s%d%s%s%s%s%s", "INFOMESSAGE", vl_SEPAR, nomMacro, XZIC_MSG_ASC_FIN);

    /* Envoi du message */
    vl_cr = itre_com_EmissionSocket (fdSocket, vm_ASCII_buffer_envoi, &vl_lg_msg, ITRE_COM_ATTENTE) ;
    return (vl_cr) ;
}






/*****************************************************************************
* ESCOTA * PROJET MIGRAZUR / PASTRE
******************************************************************************
* MODULE MTRE           FONCTION itre_com_ASCII_LireVarCtx
******************************************************************************
* DESCRIPTION DE LA FONCTION :
*
* Envoi du message (ITRE_COM_MSG_LIRE_VAR_CTX, nomVarCtx) dans la socket.
*
******************************************************************************
* SEQUENCE D'APPEL :
*/

static int itre_com_ASCII_LireVarCtx (int fdSocket, char *nomVarCtx, char *valeur,
                                         int *transDirecte, int *autreMsg)
{
    static char *vl_SEPAR = XZIC_MSG_ASC_SEP_ELEM;

    ITRE_COM_TMsgDecode message;
    int vl_lg_msg;
    int vl_cr;
  
    /* Composition et envoi du message: ITRE_COM_MSG_LIRE_VAR_CTX, nomVarCtx */
    vl_lg_msg = sprintf (vm_ASCII_buffer_envoi, "%d%s%s%s", ITRE_COM_MSG_LIRE_VAR_CTX, vl_SEPAR,
                            nomVarCtx, XZIC_MSG_ASC_FIN) ;
    vl_cr = itre_com_EmissionSocket (fdSocket, vm_ASCII_buffer_envoi, &vl_lg_msg, ITRE_COM_ATTENTE) ;
  
    /* Attendre la reponse ITRE_COM_MSG_RET_VAR_CTX */
    vl_cr = itre_com_RecevoirMsg (fdSocket, &message, ITRE_COM_MASQUE_RET_VAR_CTX, autreMsg);
    if (vl_cr != XDC_OK)
	return XDC_NOK;
  
    /*cd preparer les valeurs de retour */
    strcpy (valeur, message.contenuMsg.is.s);
    *transDirecte = message.contenuMsg.is.i;
  
    return (XDC_OK);
}



/*****************************************************************************
* ESCOTA * PROJET MIGRAZUR / PASTRE
******************************************************************************
* MODULE MTRE           FONCTION itre_com_ASCII_EcrireVarCtx
******************************************************************************
* DESCRIPTION DE LA FONCTION :
*
* Envoi du message ITRE_COM_ECRIRE_VAR_CTX dans la socket.
*
******************************************************************************
* SEQUENCE D'APPEL :
*/

static int itre_com_ASCII_EcrireVarCtx (int fdSocket, char *nomVarCtx, char *valeur,
                                           int transDirecte, char *nomMachine)
{
    static char *vl_SEPAR = XZIC_MSG_ASC_SEP_ELEM;

    int vl_lg_msg;
    int vl_cr;

    /* Composition et envoi du message :                            */
    /* ITRE_COM_MSG_ECRIRE_VAR_CTX, nomVarCtx, transDirecte, valeur */
/*
    vl_lg_msg = sprintf (vm_ASCII_buffer_envoi, "%d%s%s%s%d%s%s%s", ITRE_COM_MSG_ECRIRE_VAR_CTX, vl_SEPAR,
                            nomVarCtx, vl_SEPAR, transDirecte, vl_SEPAR, valeur, XZIC_MSG_ASC_FIN);
*/
    vl_lg_msg = sprintf (vm_ASCII_buffer_envoi, "%s%s%s%s%d%s%s%s", "SETVARCTX", vl_SEPAR,
                            nomVarCtx, vl_SEPAR, transDirecte, vl_SEPAR, valeur, XZIC_MSG_ASC_FIN);

    vl_cr = itre_com_EmissionSocket (fdSocket, vm_ASCII_buffer_envoi, &vl_lg_msg, ITRE_COM_ATTENTE) ;
    return (vl_cr) ;
}










/*****************************************************************************
******************************************************************************
**
**                    FONCTIONS INTERNES D'INTERFACE APPLIX
**
******************************************************************************
*****************************************************************************/

#ifndef _MAGISTRA
#include <elfapi.h>

extern elfData AxMemRead();

/* declarations des fonctions internes */
static int ecrireMsgApplixSocket (int fdSocket, elfData msgData);
#endif


/******************************************************
* STERIA * PROJET MIGRAZUR
*******************************************************
* MODULE MTRE           FONCTION ecrireMsgApplixSocket 
*******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Cette fonction est interne.
*
* Elle traduit une donnee elfData au format contigu,
* appelle XZGS_08EmissionSocket, et permet de tracer finement la trame
* envoyee a des fins de mise au point.
* La fonction libere l'espace alloue pour la donnee elf en entree.
*
******************************************************* 
* SEQUENCE D'APPEL :
*
* int ecrireMsgApplixSocket (int fdSocket, elfData msgData)
*
* PARAMETRES EN ENTREE :
* fdSocket	: descripteur de fichier pour la socket ouverte
*		  par XZGC_01Connexion
* msgData	: message au format non contigu
*
* PARAMETRES EN SORTIE :
*
* msgData est liberee dans cette fonction.
* 
* VALEUR RENDUE :
*
* XZGC_OK	OK.
* XZGC_NOK	Erreur.
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
*
*******************************************************
* ALGORITHME :
*
*******************************************************/
#ifndef _MAGISTRA
static int ecrireMsgApplixSocket (int fdSocket, elfData msgData)
{
  char *buffer;
  int buflen;

  /*cd transformer msgData en format contigu */
  buflen=AxMemWrite(msgData, 0, &buffer);
  AxFreeData(msgData);
  /* Test de msh non vide	Modif v1.8 */
  if (buflen > 0)
    {
    if(itre_com_EmissionSocket(fdSocket, buffer, &buflen, ITRE_COM_ATTENTE)!=XDC_OK)
      {
      free(buffer);
      return XDC_NOK;
      }
    }
  else
    {
    XZST_03EcritureTrace(XZSTC_WARNING,"ecrireMsgApplixSocket: tentative d'envoi de message vide");
    free(buffer);
    return XDC_NOK;
    }

  free(buffer);
  return XDC_OK;
}
#endif



/******************************************************
* STERIA * PROJET MIGRAZUR
*******************************************************
* MODULE MTRE           FONCTION lireMsgApplixSocket
*******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Lecture d'un message au format d'echange Applix dans la socket,
* puis decodage du code en tete.
*
* Formatage de la structure de reception en fonction du code message:
* - ITRE_COM_MSG_FIN_TACHE: pas de donnees complementaires
* - ITRE_COM_MSG_RET_VAR_CTX: is.i=transmission directe (XZGC_TRUE | XZGC_FALSE)
*                             is.s=valeur ou nom de fichier
* - ITRE_COM_MSG_RET_TASK: s=valeur de retour
*
******************************************************* 
* SEQUENCE D'APPEL :
*
* static int lireMsgApplixSocket (int fdSocket, XZGC_TMsgDecode *message)
*
* PARAMETRES EN ENTREE :
*
* fdSocket	: descripteur de fichier pour la socket ouverte
*                 par XZGC_01Connexion
*
* PARAMETRES EN SORTIE :
*
* *message     	: contenu du message
*
* VALEUR RENDUE :
*
* XZGC_OK	OK.
* XZGC_NOK	Erreur.
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
*
*******************************************************
* ALGORITHME :
*
*******************************************************/
#ifndef _MAGISTRA
static int lireMsgApplixSocket(int fdSocket, ITRE_COM_TMsgDecode *message)
{
  static char buffer[ITRE_COM_MAX_STRLEN];
  int32 buflen;
  int ret;
  elfData msgData0, msgData;
  int i;

    /* lecture en deux temps : d'abord un premier entier qui vaut */
    /* la taille en octets de la suite du message, que l'on lit ensuite */
    memset(buffer,0,sizeof(buffer));
    //sem_wait(&vg_semaphore); 
    i=read(fdSocket,&buflen,sizeof(int32));
    //sem_post(&vg_semaphore); 
    if (i<0)
	return XDC_NOK;

    ret=itre_com_ReceptionSocket(fdSocket, buffer, &buflen, ITRE_COM_ATTENTE);
    if (ret!=XDC_OK)
	return XDC_NOK;

    /*formatage des donnees Applix*/
    msgData0=AxRPCRead(buffer);
    msgData=AxArrayFromArray(msgData0,1);

    /*cd interpretation du premier champ (code) pour decoder le message */
    message->codeMsg=AxIntFromArray(msgData, 0);

    switch(message->codeMsg)
    {
     case ITRE_COM_MSG_FIN_TACHE:
	break;
     
     case ITRE_COM_MSG_RET_VAR_CTX:
	message->contenuMsg.is.i=AxIntFromArray(msgData, 1);
	strcpy(message->contenuMsg.is.s, AxStrFromArray(msgData, 2));

	break;

     case ITRE_COM_MSG_RET_TASK:
	strcpy(message->contenuMsg.s, AxStrFromArray(msgData, 1));

	break;

     case ITRE_COM_MSG_EXEC:
	message->contenuMsg.execMsg.numCmd=AxIntFromArray(msgData,1);
	message->contenuMsg.execMsg.numFct=AxIntFromArray(msgData,2);
	strcpy(message->contenuMsg.execMsg.arg, AxStrFromArray(msgData,3));

	break;

     case ITRE_COM_MSG_SEND:
	message->contenuMsg.sendMsg.msgType=AxIntFromArray(msgData,1);
	strcpy(message->contenuMsg.sendMsg.dg,AxStrFromArray(msgData,2));
	strcpy(message->contenuMsg.sendMsg.arg,AxStrFromArray(msgData,3));

	break;

     default:
	break;
    }

    AxFreeData(msgData);
    return XDC_OK;
}
#endif



/*****************************************************************************
* ESCOTA * PROJET MIGRAZUR / PASTRE
******************************************************************************
* MODULE MTRE           FONCTION itre_com_Applix_SendPoke
******************************************************************************
* DESCRIPTION DE LA FONCTION :
*
* Envoi du message (ITRE_COM_MSG_POKE, codePoke, params) dans la socket.
*
******************************************************************************
* SEQUENCE D'APPEL :
*/

static int itre_com_Applix_SendPoke (int fdSocket, int codePoke,
                                        char *params[], int nParams, char *va_machine)
{
#ifndef _MAGISTRA
  elfData msgData=NULL;
#endif
  register int i, n;
  XDY_Datagroup vl_dg;
  /* 11000 caractères pour les remontees d'etat d'equipements de la GTC */
  char vl_array[10][11000];
  char vl_machine[100];


  
  if (vg_titre)
  {
  	/* Initialisation tableau de chaines */
	for (i=0;i<10;i++)
		memset(vl_array[i]," ",11000);

    strcpy(vl_machine, va_machine);

    for (i=0;i<nParams;i++)
      strcpy(vl_array[i],params[i]);

  if (!strcmp(va_machine,XDC_NOM_SITE_VC) || !strcmp(va_machine,XDC_NOM_SITE_DP))
    if (!strcmp(vg_nomSite, XDC_NOM_SITE_CI))
      strcpy(vl_machine,XDC_NOM_SITE_CI);

  if (!strcmp(vl_machine,""))
  /* Si on n'a pas de nom de machine (ne devrait pas arriver mais il semble que ça arrive), on surcharge avec le nom du site */
    strcpy(vl_machine, vg_nomSite);

  XDG_EncodeDG2(vl_dg, XDG_API, vg_nomSite);

  XZST_03EcritureTrace(XZSTC_WARNING,"itre_com_Applix_SendPoke : Envoi %d sur topic %s param %s vers machine %s",XDM_API_POKE,vl_dg,params[0],vl_machine);

	if (nParams>1)
	  	TipcSrvMsgWrite(vl_dg,
	  		XDM_IdentMsg(XDM_API_POKE),
			XDM_FLG_SRVMSGWRITE,
			T_IPC_FT_INT4, COM_SENDPOKE,
			T_IPC_FT_INT4, codePoke,
			T_IPC_FT_STR_ARRAY, vl_array[0], nParams, 11000,
			T_IPC_FT_STR, vl_machine, NULL);
	else
	  	TipcSrvMsgWrite(vl_dg,
	  		XDM_IdentMsg(XDM_API_POKE),
			XDM_FLG_SRVMSGWRITE,
			T_IPC_FT_INT4, COM_SENDPOKE,
			T_IPC_FT_INT4, codePoke,
			T_IPC_FT_STR, vl_array[0],
			T_IPC_FT_STR, vl_machine, NULL);
  	return (XDC_OK);

  }

#ifndef _MAGISTRA
  n=0;
  msgData=AxMakeArray(nParams+2);
  AxAddIntToArray(msgData, n++, ITRE_COM_MSG_SEND_POKE);
  AxAddIntToArray(msgData, n++, codePoke);
  for(i=0; i<nParams; i++)
  {
    AxAddStrToArray(msgData, n++, params[i]);
  }
  /*cd envoi du message */
  return ecrireMsgApplixSocket(fdSocket, msgData);
#endif
}



/*****************************************************************************
* ESCOTA * PROJET MIGRAZUR / PASTRE
******************************************************************************
* MODULE MTRE           FONCTION itre_com_Applix_NewTask
******************************************************************************
* DESCRIPTION DE LA FONCTION :
*
* Envoi du message (ITRE_COM_MSG_NEW_TASK, nomMacro, params, synchrone)
* dans la socket.
*
******************************************************************************
* SEQUENCE D'APPEL :
*/

static int itre_com_Applix_NewTask (int fdSocket, char *nomMacro,
                                       char *params, int nParams,
                                       int synchrone, char *retour, int *autreMsg, char *machine)
{
#ifndef _MAGISTRA
  elfData msgData=NULL;
#endif
  register int i, n;
  int ret;
  ITRE_COM_TMsgDecode message;
  XDY_Datagroup vl_dg;

  if (vg_titre)
  {
    XDG_EncodeDG2(vl_dg, XDG_API, vg_nomSite);
    TipcSrvMsgWrite(vl_dg,
	XDM_IdentMsg(XDM_API_NEWTASK),
	XDM_FLG_SRVMSGWRITE,
	T_IPC_FT_STR, nomMacro,
	T_IPC_FT_STR,params,
	T_IPC_FT_INT4, synchrone,
	T_IPC_FT_STR,machine);
    TipcSrvFlush();
  return (XDC_OK); 
  }
#ifndef _MAGISTRA
  /*cd composition du message: ITRE_COM_MSG_NEW_TASK, synchrone, nomMacro, params*/
  n=0;
  msgData=AxMakeArray(nParams+3);
  AxAddIntToArray(msgData, n++, ITRE_COM_MSG_NEW_TASK);
  AxAddIntToArray(msgData, n++, synchrone); /*0 new_task; 1 pend_for_new_task*/
  AxAddStrToArray(msgData, n++, nomMacro);
  AxAddStrToArray(msgData, n++, params);

  /*cd envoi du message */
  if((ret=ecrireMsgApplixSocket(fdSocket, msgData))!=XDC_OK)
  {
    /*
    fprintf(stderr,"erreur dans itre_com_newtask");
    */
    return XDC_NOK;
  }
  return (XDC_OK); 
#endif
}


static int itre_com_Applix_InfoMessage (int fdSocket, char *nomMacro
                                       )
{
#ifndef _MAGISTRA
  elfData msgData=NULL;
#endif
  register int i, n;
  int ret;
  ITRE_COM_TMsgDecode message;

#ifndef _MAGISTRA
  /*cd composition du message: ITRE_COM_MSG_NEW_TASK, synchrone, nomMacro, params*/
  n=0;
  msgData=AxMakeArray(3);
  AxAddIntToArray(msgData, n++, ITRE_COM_MSG_INFO_MESSAGE);
  AxAddStrToArray(msgData, n++, nomMacro);

  /*cd envoi du message */
  if((ret=ecrireMsgApplixSocket(fdSocket, msgData))!=XDC_OK)
  {
    /*
    fprintf(stderr,"erreur dans itre_com_newtask");
    */
    return XDC_NOK;
  }
#endif
  return (XDC_OK); 
}





/*****************************************************************************
* ESCOTA * PROJET MIGRAZUR / PASTRE
******************************************************************************
* MODULE MTRE           FONCTION itre_com_Applix_LireVarCtx
******************************************************************************
* DESCRIPTION DE LA FONCTION :
*
* Envoi du message (ITRE_COM_MSG_LIRE_VAR_CTX, nomVarCtx) dans la socket.
*
******************************************************************************
* SEQUENCE D'APPEL :
*/

static int itre_com_Applix_LireVarCtx (int fdSocket, char *nomVarCtx, char *valeur,
                                          int *transDirecte, int *autreMsg)
{
  int ret;
  ITRE_COM_TMsgDecode message;

#ifndef _MAGISTRA
  /*cd composition et envoi du message: ITRE_COM_MSG_LIRE_VAR_CTX, nomVarCtx */
  ret=ecrireMsgApplixSocket(fdSocket,
		      AxBuildArray("is", ITRE_COM_MSG_LIRE_VAR_CTX, nomVarCtx));
  
  /*cd attendre la reponse ITRE_COM_MSG_RET_VAR_CTX */
  if(itre_com_RecevoirMsg(fdSocket, &message,
			 ITRE_COM_MASQUE_RET_VAR_CTX, autreMsg)!=XDC_OK)
    return XDC_NOK;
  
  /*cd preparer les valeurs de retour */
  strcpy(valeur, message.contenuMsg.is.s);
  *transDirecte=message.contenuMsg.is.i;
#endif 
  return XDC_OK;
}



/*****************************************************************************
* ESCOTA * PROJET MIGRAZUR / PASTRE
******************************************************************************
* MODULE MTRE           FONCTION itre_com_Applix_EcrireVarCtx
******************************************************************************
* DESCRIPTION DE LA FONCTION :
*
* Envoi du message ITRE_COM_ECRIRE_VAR_CTX dans la socket.
*
******************************************************************************
* SEQUENCE D'APPEL :
*/

static int itre_com_Applix_EcrireVarCtx (int fdSocket, char *nomVarCtx, char *valeur,
                                            int transDirecte, char *nomMachine)
{
  XDY_Datagroup vl_dg;
  if (vg_titre)
  {
	XZST_03EcritureTrace (XZSTC_WARNING, "itre_com_Applix_EcrireVarCtx nomMachine=[%s] nomVarCtx=[%s]", nomMachine, nomVarCtx);

  	XDG_EncodeDG2(vl_dg, XDG_API, vg_nomSite);
  	TipcSrvMsgWrite(vl_dg,
  		XDM_IdentMsg(XDM_API_SETVARCTX),
		XDM_FLG_SRVMSGWRITE,
		T_IPC_FT_STR, nomMachine,
		T_IPC_FT_STR, nomVarCtx,
		T_IPC_FT_STR, valeur, NULL);
  	TipcSrvFlush();
  	return (XDC_OK);
}

#ifndef _MAGISTRA
  /*cd composition et envoi du message: ITRE_COM_MSG_ECRIRE_VAR_CTX, nomVarCtx,
    transDirecte, valeur */
  return ecrireMsgApplixSocket(fdSocket,
			 AxBuildArray("isis", ITRE_COM_MSG_ECRIRE_VAR_CTX,
				      nomVarCtx, transDirecte, valeur));
#endif
}









/*****************************************************************************
******************************************************************************
**
**            FONCTIONS GENERIQUES D'INTERFACE DE L'APPLICATION
**
******************************************************************************
*****************************************************************************/



/******************************************************
* STERIA * PROJET MIGRAZUR
*******************************************************
* MODULE MTRE           FONCTION itre_com_SendPoke
*******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Cette fonction est une primitive.
*
* Envoi du message (ITRE_COM_MSG_POKE, codePoke, params) dans la socket.
*
*
******************************************************* 
* SEQUENCE D'APPEL :
*
* extern int itre_com_SendPoke(int fdSocket, int codePoke,
*                             char *params[], int nParams)
*
* PARAMETRES EN ENTREE :
*
* fdSocket	: descripteur de fichier pour la socket ouverte
* codePoke	: code du poke a emettre
* params	: donnees du poke
* nParams	: nombre de donnees
*
* PARAMETRES EN SORTIE :
* 
* VALEUR RENDUE : 
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
*
*******************************************************
* ALGORITHME :
*
*******************************************************/
extern int itre_com_SendPoke(int fdSocket, int codePoke,
			    char *params[], int nParams, char *va_machine)
{
    int vl_cr;

    if (vm_mode_ASCII) {
	vl_cr = itre_com_ASCII_SendPoke (fdSocket, codePoke, params, nParams, va_machine);
    } else {
	vl_cr = itre_com_Applix_SendPoke(fdSocket, codePoke, params, nParams, va_machine);
    }
    return (vl_cr);
}



/******************************************************
* STERIA * PROJET MIGRAZUR
*******************************************************
* MODULE MTRE           FONCTION itre_com_NewTask
*******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Cette fonction est une primitive.
*
* Envoi du message (ITRE_COM_MSG_NEW_TASK, synchrone, nomMacro, params) dans
* la socket.
*
* Si le parametre synchrone vaut ITRE_COM_TRUE, la fonction attend la fin de la
* tache 
*
******************************************************* 
* SEQUENCE D'APPEL :
*
* extern int itre_com_NewTask(int fdSocket, char *nomMacro, char *params[],
*                            int nParams, int synchrone, char *retour,
*                            int *autreMsg)
*
* PARAMETRES EN ENTREE :
*
* fdSocket	: descripteur de fichier pour la socket ouverte
* nomMacro	: nom de la macro a executer
* params	: parametres d'entree de la macro
* nParams	: nombre de parametres
* synchrone	: TRUE si la fonction attend la fin de la tache
*		  FALSE si l'execution est asynchrone
*
* PARAMETRES EN SORTIE :
* 
* VALEUR RENDUE : 
*
* OK	OK.
* NOK	Erreur
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
*
*******************************************************
* ALGORITHME :
*
*******************************************************/
extern int itre_com_NewTask(int fdSocket, char *nomMacro, char *params,
			   int nParams, int synchrone, char *retour,
			   int *autreMsg, char *machine)
{
    int vl_cr;

    if (vm_mode_ASCII) {
	vl_cr = itre_com_ASCII_NewTask (fdSocket, nomMacro, params, nParams,
	                                   synchrone, retour, autreMsg, machine);
    } else {
	vl_cr = itre_com_Applix_NewTask(fdSocket, nomMacro, params, nParams,
	                                   synchrone, retour, autreMsg, machine);
    }
    return (vl_cr);
}
extern int itre_com_InfoMessage(int fdSocket, char *nomMacro)
{
    int vl_cr;

    if (vm_mode_ASCII) {
	vl_cr = itre_com_ASCII_InfoMessage (fdSocket, nomMacro );
    } else {
	vl_cr = itre_com_Applix_InfoMessage(fdSocket, nomMacro );
    }
    return (vl_cr);
}




/******************************************************
* STERIA * PROJET MIGRAZUR
*******************************************************
* MODULE MTRE           FONCTION itre_com_LireVarCtx
*******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Cette fonction est une primitive.
*
* Envoi du message (ITRE_COM_MSG_LIRE_VAR_CTX, nomVarCtx) dans la socket.
*
* Le resultat est renvoye dans un message (ITRE_COM_MSG_RET_VAR_CTX, resultat,
* transDirecte).
*
******************************************************* 
* SEQUENCE D'APPEL :
*
* extern int itre_com_LireVarCtx(int fdSocket, char *nomVarCtx,
*                               char *valeur, int *transDirecte, int *autreMsg)
*
* PARAMETRES EN ENTREE :
*
* fdSocket	: descripteur de fichier pour la socket ouverte
* nomVarCtx	: nom de la variable de contexte
*
* PARAMETRES EN SORTIE :
*
* *valeur	: valeur de la variable de contexte, ou nom du fichier
*		  contenant cette valeur
* *transDirecte	: mode de transmission du resultat:
*		  TRUE: valeur contient alors la valeur de la variable
*		  FALSE: valeur contient alors le nom du fichier
*		  contenant la valeur de la variable de contexte
* 
* VALEUR RENDUE : 
*
* OK	OK.
* NOK	Erreur.
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
*
*******************************************************
* ALGORITHME :
*
*******************************************************/
extern int itre_com_LireVarCtx(int fdSocket, char *nomVarCtx, char *valeur,
			      int *transDirecte, int *autreMsg)
{
    int vl_cr;

    if (vm_mode_ASCII) {
	vl_cr = itre_com_ASCII_LireVarCtx (fdSocket, nomVarCtx, valeur, transDirecte, autreMsg);
    } else {
	vl_cr = itre_com_Applix_LireVarCtx (fdSocket, nomVarCtx, valeur, transDirecte, autreMsg);
    }
    return (vl_cr);
}



/******************************************************
* STERIA * PROJET MIGRAZUR
*******************************************************
* MODULE MTRE           FONCTION itre_com_EcrireVarCtx
*******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Cette fonction est une primitive.
*
* Envoi du message (ITRE_COM_ECRIRE_VAR_CTX, nomVarCtx, valeur, transDirecte dans
* la socket.
*
******************************************************* 
* SEQUENCE D'APPEL :
*
* extern int itre_com_EcrireVarCtx(int fdSocket, char *nomVarCtx, char *valeur,
*                                 int transDirecte)
*
* PARAMETRES EN ENTREE :
*
* fdSocket	: descripteur de fichier pour la socket ouverte
* nomVarCtx	: nom de la variable de contexte
* resultat	: valeur de la variable de contexte, ou nom du fichier
*		  contenant cette valeur
* transDirecte	: mode de transmission de la valeur:
*		  ITRE_COM_TRUE: valeur contient alors la valeur
*		  de la variable de contexte
*		  ITRE_COM_FALSE: valeur contient alors le nom du
*		  fichier contenant la valeur de la variable de contexte
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE : 
*
* XDC_OK	OK.
* XDC_NOK	Erreur.
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
*
*******************************************************
* ALGORITHME :
*
*******************************************************/
extern int itre_com_EcrireVarCtx(int fdSocket, char *nomVarCtx, char *valeur,
				int transDirecte,char *nomMachine)
{
    int vl_cr;

    if (vm_mode_ASCII) {
	vl_cr = itre_com_ASCII_EcrireVarCtx (fdSocket, nomVarCtx, valeur, transDirecte,nomMachine);
    } else {
	vl_cr = itre_com_Applix_EcrireVarCtx(fdSocket, nomVarCtx, valeur, transDirecte, nomMachine);
    }
    return (vl_cr);
}



/******************************************************
* STERIA * PROJET MIGRAZUR
*******************************************************
* MODULE MTRE           FONCTION itre_com_RecevoirMsg
*******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Cette fonction est une primitive.
*
* Lecture d'un message dans la socket.
* Decodage du code en tete.
* Formatage de la structure de reception en fonction du code message:
* - ITRE_COM_MSG_FIN_TACHE: pas de donnees complementaires
* - ITRE_COM_MSG_RET_VAR_CTX: is.i=transmission directe (XZGC_TRUE | XZGC_FALSE)
*                             is.s=valeur ou nom de fichier
* - ITRE_COM_MSG_RET_TASK: s=valeur de retour
*
******************************************************* 
* SEQUENCE D'APPEL :
*
* extern int itre_com_RecevoirMsg(int fdSocket, XZGC_TMsgDecode *message,
*				 int masqueReception, int *autreMsg)
*
* PARAMETRES EN ENTREE :
*
* fdSocket	: descripteur de fichier pour la socket ouverte
*                 par XZGC_01Connexion
* masqueReception:indique le ou les types de message attendus par un champ
*		  de bits (XZGC_MASQUE_REC_...)
*
* PARAMETRES EN SORTIE :
*
* *message     	: contenu du message
* *autreMsg	: XZGC_TRUE indique si des messages d'un autre type n'entrant
*		  pas dans le masque specifie ont ete recus: il faudra alors
*		  aller les chercher avec XZGC_08DepilerMsg.
* 
* VALEUR RENDUE : 
*
* XZGC_OK	OK.
* XZGC_NOK	Erreur.
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
*
*******************************************************
* ALGORITHME :
*
*******************************************************/
extern int itre_com_RecevoirMsg(int fdSocket,ITRE_COM_TMsgDecode *message,
			       int masqueReception, int *autreMsg)
{
    int vl_cr;

    while(1)
    {
	*autreMsg = XDC_FAUX;

	/*cd lecture bloquante d'un message */
#ifdef _HPUX_SOURCE
	vl_cr = lireMsgApplixSocket(fdSocket, message);
#else
	vl_cr = lireMsgASCIISocket(fdSocket, message, autreMsg);
#endif

	if(vl_cr != XDC_OK)
		return XDC_NOK;
	return (XDC_OK);
    }
}



/******************************************************
 * STERIA * PROJET  MIGRAZUR 
 *******************************************************
 * MODULE  MTRE  FONCTION  itre_com_OuvreSocket 
 *******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 * Cree et ouvre une socket de type SOCK_STREAM dans le domaine AF_INET
 *
 ******************************************************* 
 * SEQUENCE D'APPEL :
 *
 * extern int itre_com_OuvreSocket (pa_idsocket)
 *
 * PARAMETRES EN ENTREE :
 *
 *
 * PARAMETRES EN SORTIE :
 *
 * int *pa_idsocket : pointeur sur un descripteur de la socket mis a jour
 * 
 * VALEUR RENDUE : 
 *
 * XZGS_OK : Ok
 * XZGS_NOK : Nok
 *
 * CONDITION D'ERREUR :
 *
 * NEANT
 *
 * CONDITION LIMITES :
 *
 * NEANT
 *
 *******************************************************
 * ALGORITHME :
 *
 *
 * Creer une socket de type SOCK_STREAM dans le domaine AF_INET
 *
 *
 *******************************************************/

extern int itre_com_OuvreSocket(pa_socket)
     int *pa_socket;
{
  int vl_socket;
  
  /* creation de la socket */
  vl_socket=socket( AF_INET, SOCK_STREAM, 0);
  *pa_socket=vl_socket;
  if ( vl_socket < 0 ) 
    return(XDC_NOK);
  return(XDC_OK);
}



/******************************************************
 * STERIA * PROJET  MIGRAZUR 
 *******************************************************
 * MODULE  MTRE  FONCTION  itre_com_FermeSocket 
 *******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 * Ferme la socket identifiee par id_socket
 *
 ******************************************************* 
 * SEQUENCE D'APPEL :
 *
 * extern int itre_com_FermeSocket (va_idsocket)
 *
 * PARAMETRES EN ENTREE :
 *
 *
 * PARAMETRES EN SORTIE :
 *
 * int va_idsocket : descripteur de la socket a fermer 
 * 
 * VALEUR RENDUE : 
 *
 * XZGS_OK : Ok
 * XZGS_NOK : Nok
 *
 * CONDITION D'ERREUR :
 *
 * NEANT
 *
 * CONDITION LIMITES :
 *
 * NEANT
 *
 *******************************************************
 * ALGORITHME :
 *
 *
 *
 *
 *******************************************************/
extern int itre_com_FermeSocket(va_socket)
     int va_socket;
{
  close(va_socket);
  return(XDC_OK);
}



/******************************************************
 * ESCOTA * PROJET  MIGRAZUR
 *******************************************************
 * MODULE  MTRE  FONCTION  itre_com_ModeCommIHM
 *******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 * Definit le mode de communication de la tache relais avec l'IHM
 *
 ******************************************************* 
 * SEQUENCE D'APPEL :
 *
 * extern int itre_com_ModeCommIHM (va_mode)
 *
 * PARAMETRES EN ENTREE :
 *
 * char *va_mode : mode de communication avec l'IHM
 *
 * PARAMETRES EN SORTIE :
 *
 * 
 * VALEUR RENDUE : 
 *
 * XZGS_OK : Ok
 *
 * CONDITION D'ERREUR :
 *
 * NEANT
 *
 * CONDITION LIMITES :
 *
 * NEANT
 *
 *******************************************************
 * ALGORITHME :
 *
 *
 *
 *
 *******************************************************/
extern int itre_com_ModeCommIHM (va_mode)
     char *va_mode;
{
    vm_mode_ASCII = FALSE;
    if (strcasecmp (va_mode, "ascii") == 0) {
	vm_mode_ASCII = TRUE;
    }

    return(XDC_OK);
}



/******************************************************
 * STERIA * PROJET MIGRAZUR
 *******************************************************
 * MODULE MTRE           FONCTION itre_com_Connexion
 *******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 * Cette fonction est une primitive.
 *
 * Il faut appeler cette fonction avant toutes les autres .
 *
 ******************************************************* 
 * SEQUENCE D'APPEL :
 *
 * itre_com_Connexion(int *fdSocket, int num_socket, XDY_NomMachine nom)
 *
 * PARAMETRES EN ENTREE :
 *
 * PARAMETRES EN SORTIE :
 *
 * *fdSocket	: descripteur de fichier pour la socket
 * 
 * VALEUR RENDUE :
 *
 * ITRE_COM	Ok
 * ITRE_COM_NOK	Erreur.
 *
 * CONDITION D'ERREUR :
 *
 * CONDITION LIMITES :
 *
 *******************************************************
 * ALGORITHME :
 *
 *******************************************************/
extern int  itre_com_Connexion(int *fdSocket, int num_socket,XDY_NomMachine nom)
{
  char nomHostServeur[FILENAME_MAX];
  
  strcpy(nomHostServeur, nom);
  if(itre_com_ConnexionServeur(nomHostServeur, num_socket,
			     ITRE_COM_ATTENTE, fdSocket)!=XDC_OK)
    return XDC_NOK;
  
 return XDC_OK;
}



/******************************************************
 * STERIA * MIGRAZUR 
 *******************************************************
 * MODULE MTRE           FONCTION itre_com_Deconnexion
 *******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 * Cette fonction est une primitive.
 *
 * Envoi du message de deconnexion ITRE_COM_MSG_QUIT dans la socket.
 *
 * Fermeture de la socket.
 *
 *
 ******************************************************* 
 * SEQUENCE D'APPEL :
 *
 * extern int itre_com_Deconnexion(int fdSocket)
 *
 * PARAMETRES EN ENTREE :
 *
 * fdSocket	: descripteur de fichier pour la socket ouverte
 *                 par itre_com_01Connexion
 *
 * PARAMETRES EN SORTIE :
 * 
 * VALEUR RENDUE : 
 *
 * ITRE_COM_OK	OK.
 * ITRE_COM_NOK	Erreur.
 *
 * CONDITION D'ERREUR :
 *
 * CONDITION LIMITES :
 *
 *******************************************************
 * ALGORITHME :
 *
 *******************************************************/
extern int itre_com_Deconnexion(int fdSocket)
{
  /* composition et envoi du message de deconnexion: ITRE_COM_MSG_QUIT */

  itre_com_SendPoke (fdSocket, ITRE_COM_MSG_QUIT, NULL, 0,"");

  if(itre_com_CoupeConnexion(fdSocket)!=XDC_OK)
    return XDC_NOK;
  
  return XDC_OK;
}



/******************************************************
 * STERIA * PROJET  MIGRAZUR 
 *******************************************************
 * MODULE  MTRE  FONCTION  itre_com_AttendConnexionClient 
 *******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 *
 * Cette fonction attend la connexion d'un client sur le port attache a la socket
 * et retourne la socket de service associee au client 
 * ainsi que le nom host du client connecte.  
 *
 *
 *
 ******************************************************* 
 * SEQUENCE D'APPEL :
 *
 * extern int itre_com_AttendConnexionClient (va_socket_ecoute,va_attente,pa_socket_service,pa_nom_client )
 *
 * PARAMETRES EN ENTREE :
 *
 * int socket_ecoute	: descripteur de la socket d'ecoute.
 * int attente		: entier indiquant la duree de l'attente en secondes.	
 *
 *
 * PARAMETRES EN SORTIE :
 *
 * int *socket_service	: descipteur de la socket de service avec le client.
 * char *nom_client	: Nom du client connecte.
 *
 *
 * VALEUR RENDUE : 
 *
 * XZGS_OK	: Ok
 * XZGS_NOK	: Nok
 * XZGS_TO	: Time-out sur l'attente.
 *
 * CONDITION D'ERREUR :
 *
 *
 * CONDITION LIMITES :
 *
 * NEANT
 *
 *******************************************************
 * ALGORITHME :
 *
 *
 *
 *******************************************************/

extern int itre_com_AttendConnexionClient(va_sock_ecoute,va_attente,pa_sock_service,pa_nom_client)
     int va_sock_ecoute;
     int va_attente;
     int *pa_sock_service;
     char *pa_nom_client;
{
  int vl_taille;
  struct sockaddr_in vl_addr_client;
  struct hostent *pl_client;
  fd_set vl_fdread;
  struct timeval vl_time;
  int vl_retaccpt;
  int vl_ret=0;
  
  /* traitement du delai d'attente */
  switch(va_attente)
    {
    case ITRE_COM_SANS_ATTENTE:
      /*cd socket non bloquante */
      fcntl(va_sock_ecoute,F_SETFL,O_NDELAY);
      break;
    case ITRE_COM_ATTENTE:
      break;
    default:
      /*cd timeout sur l'accept */
      FD_ZERO(&vl_fdread);
      FD_SET(va_sock_ecoute,&vl_fdread);
      vl_time.tv_sec=va_attente;
      vl_time.tv_usec=0;
#ifdef _HPUX_SOURCE
      vl_ret=select(va_sock_ecoute+1,(int *)&vl_fdread,NULL,NULL,&vl_time);
#else
      vl_ret=select(va_sock_ecoute+1,&vl_fdread,(fd_set *)0,(fd_set *)0,&vl_time);
#endif
      if (vl_ret==0)
	return ITRE_COM_TO;
      if (vl_ret==-1)
	return XDC_NOK;
      
      break;
    }
  vl_taille=sizeof(struct sockaddr);
  /*cd accept la connexion d'un client */
  vl_retaccpt=accept(va_sock_ecoute,(struct sockaddr *)&vl_addr_client,&vl_taille);
  if (vl_retaccpt<0)
    return(XDC_NOK);
  
  *pa_sock_service=vl_retaccpt;
  /*cd positionnement de la socket en non bloquant */
  fcntl(*pa_sock_service,F_SETFL,O_NDELAY);
  /*cd recuperation du nom du client connecte */
  pl_client=gethostbyaddr((char * )&vl_addr_client.sin_addr.s_addr,4,AF_INET);
  strcpy(pa_nom_client,pl_client->h_name);
  return(XDC_OK);
}



/******************************************************
 * STERIA * PROJET  MIGRAZUR 
 *******************************************************
 * MODULE  MTRE  FONCTION  itre_com_ConnexionServeur 
 *******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 * Envoi une demande de connexion au serveur identifie par son nom host sur le port donne.
 * si le flag attente=XZGS_ATTENTE le processus appelant est bloque tant que la connexion 
 * n'est pas effective.
 * Des que la connexion est etablie, la fonction renvoi l'identification de la
 * socket de service connectee au serveur.
 *
 ******************************************************* 
 * SEQUENCE D'APPEL :
 *
 * extern int itre_com_ConnexionServeur (pa_nomhostserveur,va_port,va_attente,pa_socket_service)
 *
 * PARAMETRES EN ENTREE :
 *
 * char *pa_nomhostserveur 	: Nom du serveur de communication 
 * int va_port		: Numero de port de communication
 * int va_attente	: flag indiquant l'attente ou non de la fonction
 *
 * PARAMETRES EN SORTIE :
 *
 * int *pa_socket_service : descripteur de la socket de service.
 
 * VALEUR RENDUE : 
 *
 * XZGS_OK    : Ok
 * XZGS_NOK   : Nok
 *
 * CONDITION D'ERREUR :
 *
 * NEANT
 *
 * CONDITION LIMITES :
 *
 * NEANT
 *
 *******************************************************
 * ALGORITHME :
 *
 *
 *
 *******************************************************/
int itre_com_ConnexionServeur(pa_host_serveur,va_port,va_attente,pa_socket_service) 
     char *pa_host_serveur;
     int va_port;
     int va_attente;
     int *pa_socket_service;
{
  int vl_sock;
  int vl_ret;
  struct sockaddr_in vl_serveur;
  struct hostent *pl_serveur;
  
  /*cd creation de la socket */
  vl_sock=socket(AF_INET,SOCK_STREAM,0);
  if (vl_sock==-1)
  {
    /*
    printf(">erreur socket %d\n",errno);
    */
    return (XDC_NOK);
  }
  
  /*cd test sur le delai d'attente */
  if (va_attente==ITRE_COM_SANS_ATTENTE)
    fcntl(vl_sock,F_SETFL,O_NDELAY);
  /*cd recuperation de l'adresse IP du serveur de communication  */
  
  pl_serveur=gethostbyname(pa_host_serveur);
  
  if (pl_serveur==(struct hostent *)NULL)
    return(ITRE_COM_NO_HOST);
  
  memset((char *)&vl_serveur,0,sizeof(vl_serveur));
  /*cd remplissage de la structure de communication */
  memcpy(&vl_serveur.sin_addr,pl_serveur->h_addr_list[0],(unsigned)pl_serveur->h_length);
  vl_serveur.sin_family = AF_INET;
  vl_serveur.sin_port = htons(va_port);
  
  /*cd Demande de connexion au serveur */
  vl_ret=connect(vl_sock,(struct sockaddr *)&vl_serveur,sizeof(vl_serveur));
  
  if (vl_ret !=0)
  {
    /*ferme la socket puisque la connexion a echoue*/
    itre_com_FermeSocket(vl_sock);
    return(XDC_NOK);
  }
  
  *pa_socket_service=vl_sock;
  
  /*cd configuration non bloquante de la socket par defaut */
  fcntl(vl_sock,F_SETFL,O_NDELAY);
  return(XDC_OK);	
}



/******************************************************
 * STERIA * PROJET  MIGRAZUR 
 *******************************************************
 * MODULE  MTRE  FONCTION  itre_com_ReceptionSocket 
 *******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 * Met le process appelant en attente pendant 'attente' secondes maximum d'un message sur la
 * socket de service ou bloquant (XZGS_ATTENTE) ou non bloquant (XZGS_SANS_ATTENTE).
 * Cette fonction renvoie le message recu ainsi que la taille du message 
 *
 ******************************************************* 
 * SEQUENCE D'APPEL :
 *
 * extern int itre_com_ReceptionSocket (va_socket_service,pa_message,pa_taille_message,va_attente)
 *
 * PARAMETRES EN ENTREE :
 *
 * int va_socket_service : descripteur de la socket de service.
 * int va_attente	: flag indiquant l'attente ou non de la fonction
 *
 * PARAMETRES EN SORTIE :
 *
 * char *pa_message	 : message recu
 * int *pa_taille_message : taille du message recu 
 * 
 * VALEUR RENDUE : 
 *
 * XZGS_OK    : Ok
 * XZGS_NOK   : Nok
 * XZGS_TO    : Sortie sur Time-out
 *
 * CONDITION D'ERREUR :
 *
 * NEANT
 *
 * CONDITION LIMITES :
 *
 * NEANT
 *
 *******************************************************
 * ALGORITHME :
 *
 *
 *
 *******************************************************/
int itre_com_ReceptionSocket(va_socket_service,pa_message,pa_taille_message,va_attente) 
     int va_socket_service;
     char *pa_message;
     int *pa_taille_message;
     int va_attente;
{
  struct timeval vl_time,*pl_Time;
  fd_set vl_fdread;
  int vl_cr;
  int vl_nc;

  /*cd test sur le flag d'attente */
  switch(va_attente)
    {
    case ITRE_COM_ATTENTE:
      pl_Time=NULL;
      break;
    case ITRE_COM_SANS_ATTENTE:
      break;
    default:
      vl_time.tv_sec=va_attente;
      vl_time.tv_usec=0;
      pl_Time=&vl_time;
      break;
    }

  FD_ZERO(&vl_fdread);
  FD_SET(va_socket_service,&vl_fdread);
  
  if (va_attente != ITRE_COM_SANS_ATTENTE)
    {
#ifdef _HPUX_SOURCE
      vl_cr=select(va_socket_service+1,(int *)&vl_fdread,NULL,NULL,pl_Time);
#else
      vl_cr=select(va_socket_service+1,&vl_fdread,(fd_set *)0,(fd_set *)0,pl_Time);
#endif
      switch(vl_cr)
	{
	case 0:
	  /* sortie sur timeout lecture */
	  return(ITRE_COM_TO);
	case -1:
	  /*erreur sur select */
	  return(XDC_NOK);
	default:
	  break;
	}
    }
  memset(pa_message,0,(unsigned)*pa_taille_message);
  
  //sem_wait(&vg_semaphore); 
  vl_nc=read(va_socket_service,pa_message,(unsigned)*pa_taille_message);
  //sem_post(&vg_semaphore); 

  if (vl_nc <= 0)
    /* erreur sur le read */
    return(XDC_NOK);
  
  *pa_taille_message=vl_nc;
  return(XDC_OK);
}



/******************************************************
 * STERIA * PROJET  MIGRAZUR 
 *******************************************************
 * MODULE  MTRE  FONCTION  itre_com_EmissionSocket 
 *******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 * Emet un message sur une socket de service. Si la socket est pleine, le process est bloque 
 * pendant 'attente' secondes, bloque si ITRE_COM_ATTENTE ou non bloquant si ITRE_COM_SANS_ATTENTE.
 *
 ******************************************************* 
 * SEQUENCE D'APPEL :
 *
 * extern int itre_com_EmissionSocket (va_socket_service,pa_message,pa_taille_message,va_attente)
 *
 * PARAMETRES EN ENTREE :
 *
 * int va_socket_service : descripteur de la socket de service.
 * int va_attente	: flag indiquant l'attente ou non de la fonction
 * char *pa_message	: message a envoyer.
 * int *pa_taille_message : taille du message a envoyer 
 *
 * PARAMETRES EN SORTIE :
 * int *pa_taille_message : taille du message effectivement envoye.
 *
 * 
 * VALEUR RENDUE : 
 *
 * XZGS_OK    : Ok
 * XZGS_NOK   : Nok
 * XZGS_TO    : Sortie sur Time-out
 * XZGS_PCOM  : Probleme de communication
 *
 * CONDITION D'ERREUR :
 *
 * NEANT
 *
 * CONDITION LIMITES :
 *
 * NEANT
 *
 *******************************************************
 * ALGORITHME :
 *
 *
 *
 *******************************************************/
int itre_com_EmissionSocket(va_socket_service,pa_message,pa_taille_message,va_attente) 
     int va_socket_service;
     void *pa_message;
     int *pa_taille_message;
     int va_attente;
{
  struct timeval vl_time,*pl_Time;
  fd_set vl_fdwrite;
  int vl_nb_ecrit,vl_cr,vl_nc;
  
  
  /*cd test sur le flag d'attente */
  switch(va_attente)
    {
    case ITRE_COM_ATTENTE:
      pl_Time=NULL;
      break;
    case ITRE_COM_SANS_ATTENTE:
      break;
    default:
      vl_time.tv_sec=va_attente;
      vl_time.tv_usec=0;
      pl_Time=&vl_time;
      break;
    }
  
  FD_ZERO(&vl_fdwrite);
  FD_SET(va_socket_service,&vl_fdwrite);
  
  if (va_attente != ITRE_COM_SANS_ATTENTE)
    {
#ifdef _HPUX_SOURCE
      vl_cr=select(va_socket_service+1,NULL,(int *)&vl_fdwrite,NULL,pl_Time);
#else
      vl_cr=select(va_socket_service+1,(fd_set *)0,&vl_fdwrite,(fd_set *)0,pl_Time);
#endif
      switch(vl_cr)
	{
	case 0:
	  /* sortie sur timeout ecriture */
	  return(ITRE_COM_TO);
	case -1:
	  /*erreur sur select */
	  return(XDC_NOK);
	default:
	  break;
	}
    }
  /*
  fprintf(stderr,"avant write ... ");
  */
  //sem_wait(&vg_semaphore); 
  vl_nc=write(va_socket_service,pa_message,(unsigned)*pa_taille_message);
  //sem_post(&vg_semaphore); 
  /*
  fprintf(stderr,"code retour %d\n",vl_nc);
  */
  *pa_taille_message=vl_nc;
  if (vl_nc < 0)
    return(ITRE_COM_PCOM);
  else
    {
      return(XDC_OK);
    }
}



/******************************************************
 * STERIA * PROJET  MIGRAZUR 
 *******************************************************
 * MODULE  MTRE  FONCTION  itre_com_CoupeConnexion 
 *******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 * Coupe la connexion avec une socket de service connectee.
 *
 *
 ******************************************************* 
 * SEQUENCE D'APPEL :
 *
 * extern int itre_com_CoupeConnexion (va_socket_service)
 *
 * PARAMETRES EN ENTREE :
 *
 * int va_socket_service : descripteur de la connexion a couper.
 *
 * PARAMETRES EN SORTIE :
 *
 *
 * VALEUR RENDUE : 
 *
 * XZGS_OK    : Ok
 * XZGS_NOK   : Nok
 *
 * CONDITION D'ERREUR :
 *
 * NEANT
 *
 * CONDITION LIMITES :
 *
 * NEANT
 *
 *******************************************************
 * ALGORITHME :
 *
 *
 *
 *******************************************************/
int itre_com_CoupeConnexion( va_socket_service)
     int va_socket_service;
{
  int vl_ret;
  
  /*cd shutdown de la socket dans les deux sens (lecture/ecriture) */
  vl_ret=shutdown(va_socket_service,2);
  if (vl_ret)
    return(XDC_NOK);
  else
    return(XDC_OK);
}
