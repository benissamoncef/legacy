/*E*/
/* Fichier : $Id: epal_cmd.h,v 1.1 1998/01/30 15:25:22 mismer Exp $     Release : $Revision: 1.1 $      Date :  $Date: 1998/01/30 15:25:22 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPAL * FICHIER epal_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le fichier d'include des callbacks pal
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D 	version 1.1	15 Jan 1998	: Creation
*
------------------------------------------------------ */

#ifndef epal_cmd
#define epal_cmd

/* fichiers inclus */

#include	"epal_don.h"
#include	"xzen.h"


/* definitions de constantes exportees */

extern XDY_NomMachine		vg_NomMachine;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern void	en_Init_IHM_contraste_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);

extern void	ep_Init_IHM_Liaison_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
extern void	en_contraste_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
extern void	en_Init_IHM_PAL_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
extern void	en_cmd_PAL_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
extern void	en_controle_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
extern void	en_journuit ( XZEAT_JourNuit , int );

extern void	en_CMD_LCR ( 
        T_IPC_CONN			va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA		data,
	T_CB_ARG				arg);
		     
 
#endif
