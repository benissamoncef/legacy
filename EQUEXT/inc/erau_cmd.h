								/*E*/
/*  Fichier : $Id: erau_cmd.h,v 1.5 2021/05/04 13:25:09 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2021/05/04 13:25:09 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER erau_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de commande de la tache TERAU
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	01 Dec 1994	: Creation
* Nagiel.E	version 1.2	24 Jan 1995	:
* Fontaine.C	version 1.3	22 Mai 1995	: Reprise entete, historique
* ABE		07/04/21	: Ajout PAU SONO DEM-SAE244 1.4
------------------------------------------------------ */

#ifndef erau_cmd
#define erau_cmd

/* fichiers inclus */

#include	"erau_don.h"
#include 	"etim_don.h"
#include	"xzae103.h"

/* definitions de constantes exportees */

extern XDY_NomMachine		vg_NomMachine;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

void	er_Init_IHM_Mes_RAU_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);

void	er_Cmd_RAU_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);

void	er_Init_IHM_Mes_RAU_SONO_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);

void	er_Cmd_SONO_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);

void	er_controle_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
void	er_CMD_LCR ( T_IPC_CONN			va_Cnx,
		     T_IPC_CONN_PROCESS_CB_DATA		data,
		     T_CB_ARG				arg);
 
void er_Presence_PCS ( XZAET_EPresence_Op_PCsimpl );

#endif
