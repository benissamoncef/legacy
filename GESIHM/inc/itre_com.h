/*Fichier :  $Id: itre_com.h,v 1.7 2021/06/08 18:17:37 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2021/06/08 18:17:37 $
*******************************************************
* STERIA * PROJET MIGRAZUR
*******************************************************
* MODULE MTRE          * FICHIER itre_com.h
*******************************************************
* AUTEUR JM Guilhou   * DATE DE CREATION 29/09/94
*******************************************************
* DESCRIPTION DU MODULE :
*
*
*******************************************************
* HISTORIQUE DES MODIFICATIONS :
* JBL		Nov 2007	: DEM 665
* JMG		24/06/19	: DEM1338 ODI-C 1.16
* jmg		04/03/21	: ajoput nomMachine dans ecrireVarCtx API 1.17
*******************************************************/

#ifndef MTRE_H
#define MTRE_H

/*fichiers inclus*/
#include "xdy.h"
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <time.h>
#include <xdc.h>

/* definitions des constantes exportees */

/* definitions des types exportes */

/* definitions des macros exportees */

/* declarations des donnes globales */

/* definition des contantes globales */

/* codes de message utilises par les primitives */
/* ces codes doivent etre identiques a ceux definis dans ITMA_TAR.h */
#define ITRE_COM_MSG_PID_PROCESS		0
#define ITRE_COM_MSG_QUIT			1
#define ITRE_COM_MSG_SEND_POKE		2
#define ITRE_COM_MSG_NEW_TASK		3
#define ITRE_COM_MSG_ECRIRE_VAR_CTX		4
#define ITRE_COM_MSG_LIRE_VAR_CTX		5
#define ITRE_COM_MSG_RET_VAR_CTX		6
#define ITRE_COM_MSG_RET_TASK		7
#define ITRE_COM_MSG_FIN_TACHE		8
#define ITRE_COM_MSG_EXEC			11
#define ITRE_COM_MSG_SEND			12
#define ITRE_COM_MSG_INFO_MESSAGE		13

/* masques de reception */
#define ITRE_COM_MASQUE_RET_TASK		(1<<ITRE_COM_MSG_RET_TASK)
#define ITRE_COM_MASQUE_RET_VAR_CTX	(1<<ITRE_COM_MSG_RET_VAR_CTX)
#define ITRE_COM_MASQUE_FIN_TACHE		(1<<ITRE_COM_MSG_FIN_TACHE)
#define ITRE_COM_MASQUE_TOUS		(ITRE_COM_MASQUE_RET_TASK | \
       			ITRE_COM_MASQUE_RET_VAR_CTX | ITRE_COM_MASQUE_FIN_TACHE)

/* definition des valeurs ITRE_COM_TRUE et ITRE_COM_FALSE, pour etre en
   accord avec true et false de Applix */
#define ITRE_COM_TRUE	1
#define ITRE_COM_FALSE	0

/* taille maximum d'une chaine emise par Applix */
#define ITRE_COM_MAX_STRLEN	2048

/*constantes "internes" de communication par socket*/
#define ITRE_COM_ATTENTE		-1
#define ITRE_COM_SANS_ATTENTE	0	
#define ITRE_COM_TO			2	
#define ITRE_COM_PCOM		3	
#define ITRE_COM_NO_HOST		106
#define ITRE_COM_EBUF		110
#define ITRE_COM_PBIND		102
#define ITRE_COM_NCIS		109

/* definition des types exportes */
typedef struct
{
  int codeMsg;   /*EXEC, SEND, RET_VAR_CTX, RET_TASK, FIN_TACHE*/
  union
    {
      char s[ITRE_COM_MAX_STRLEN];  /*code renvoye par la task Applix executee*/
      struct
	{
	  int i;  
	  char s[ITRE_COM_MAX_STRLEN];
	} is;  /*retourne pour RET_VAR_CTX*/
      struct 
	{
	  int numCmd; /*numero de poke pour retour Applix*/
	  int numFct; /*numero de la fonction a executer*/
	  char arg[ITRE_COM_MAX_STRLEN]; /*arguments*/
	  char nomMachine[ITRE_COM_MAX_STRLEN]; /* nom de la machine qui demande l'exec */
	} execMsg;
      struct
	{
	  int msgType;  /*type de message a envoyer*/
	  char dg[ITRE_COM_MAX_STRLEN]; /*datagroup a qui envoyer*/
	  char arg[ITRE_COM_MAX_STRLEN]; /*le message*/
	  char nomMachine[ITRE_COM_MAX_STRLEN]; /* nom de la machine qui demande l'exec */
	} sendMsg;
    } contenuMsg;
} ITRE_COM_TMsgDecode; /*structure du message recu d'Applix*/

/* prototype des fonctions exportees */
extern int itre_com_Connexion(int *fdSocket, int num_socket,XDY_NomMachine nom);
extern int itre_com_Deconnexion(int fdSocket);
extern int itre_com_ModeCommIHM(char *mode);

extern int itre_com_SendPoke(int fdSocket, int codePoke,
			    char *params[], int nParams, char *machine);
extern int itre_com_NewTask(int fdSocket, char *nomMacro, char *params,
			   int nParams, int synchrone, char *retour,
			   int *autreMsg, char *machine);
extern int itre_com_LireVarCtx(int fdSocket, char *nomVarCtx, char *valeur,
			      int *mode_trans, int *autreMsg);
extern int itre_com_EcrireVarCtx(int fdSocket, char *nomVarCtx, char *valeur,
				int mode_trans, char *nomMachine);
extern int itre_com_RecevoirMsg(int fdSocket, ITRE_COM_TMsgDecode *message,
			       int masqueReception, int *autreMsg);
extern int itre_com_InfoMessage(int fdSocket, char *nomMacro);

extern int itre_com_OuvreSocket(int *va_socket);
extern int itre_com_FermeSocket(int va_socket);
extern int itre_com_AttendConnexionClient(int va_sock_ecoute,
					int va_attente,
					int *va_sock_service,
					char *va_nom_client);
extern int itre_com_ConnexionServeur(char *va_host_serveur,
				   int va_port,
				   int va_attente,
				   int *va_socket_service);
extern int itre_com_ReceptionSocket(int va_socket_service,
				  char *va_message,
				  int *va_taille_message,
				  int va_attente);
extern int itre_com_EmissionSocket(int va_socket_service,
				 void *va_message,
				 int *va_taille_message,
				 int va_attente);
extern int itre_com_CoupeConnexion(int va_socket_service);
#endif

