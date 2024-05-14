/*E*/
/* Fichier : $Id: eimu_cmd.h,v 1.1 2020/11/04 09:25:38 pc2dpdy Exp $     Release : $Revision: 1.1 $      Date :  $Date: 2020/11/04 09:25:38 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEIMU * FICHIER eimu_cmd.h
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

#ifndef eimu_cmd
#define eimu_cmd

/* fichiers inclus */

#include	"eimu_don.h"
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
 
extern void	en_Init_IHM_IMU_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
extern void	en_cmd_IMU_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
extern void	en_controle_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
		     
 
#endif
