/*E*/
/* Fichier : $Id: ecfe_cmd.h,v 1.1 2019/01/29 11:05:34 pc2dpdy Exp $     Release : $Revision: 1.1 $      Date :  $Date: 2019/01/29 11:05:34 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TECFE * FICHIER ecfe_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le fichier d'include des callbacks pal
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	01/03/18 :	Creation CTRL_FEUX 1.1 DEM1284
*
------------------------------------------------------ */

#ifndef ecfe_cmd
#define ecfe_cmd

/* fichiers inclus */

#include	"ecfe_don.h"
#include	"xzen.h"


/* definitions de constantes exportees */

extern XDY_NomMachine		vg_NomMachine;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern void	ep_Init_IHM_Liaison_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
extern void	en_Init_IHM_CFE_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
extern void	en_cmd_CFE_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
extern void	en_controle_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
#endif
