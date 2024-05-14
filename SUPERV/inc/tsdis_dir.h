/*E*/ 
/*Fichier : $Id: tsdis_dir.h,v 1.2 2016/04/26 11:05:23 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2016/04/26 11:05:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SUPERV
------------------------------------------------------
* MODULE TSDIS * FICHIER tsdis_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Permet de definir les variables necessaires 
*	au module; TSDIS
*
------------------------------------------------------
* HISTORIQUE :
*
* T.Milleville	12 Jan 1995	: Creation
------------------------------------------------------*/

#ifndef TSDIS
#define TSDIS

/* fichiers inclus */
#include <rtworks/common.h>
#include <rtworks/ipc.h>
#ifdef _HPUX_SOURCE
#include <rtworks/ipc_proto.h>
#endif

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
void tsdis_test_pv_cb(
	T_IPC_CONN va_Conn,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_Arg);

void tsdis_sync_cb(
	T_IPC_CONN va_Conn,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_Arg);

void tsdis_datasrv_cb(
	T_IPC_CONN va_Conn,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_Arg);

#endif
