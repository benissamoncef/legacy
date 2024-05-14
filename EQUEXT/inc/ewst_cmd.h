/*E*/
/*  Fichier : $Id: ewst_cmd.h,v 1.1 2018/01/11 14:26:51 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/01/11 14:26:51 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER ewst_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache TERAU
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	12/04/22	creation DEM-SAE375
*
------------------------------------------------------ */

#ifndef ewst_cmd
#define ewst_cmd

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include "ewst_dir.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
void cb_tewst_appel_tel(T_IPC_CONN va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA pa_data,
		T_CB_ARG	pa_Arg);

void cb_tewst_retour_appel(T_IPC_CONN va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA pa_data,
		T_CB_ARG	pa_Arg);
int tewst_lecture_ini(XDY_Texte va_param, XDY_Texte va_IniFileData);
#endif