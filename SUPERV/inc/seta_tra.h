/*E*/
/* Fichier : $Id: seta_tra.h,v 1.7 2011/01/11 21:03:52 gesconf Exp $	$Revision: 1.7 $        Date : $Date: 2011/01/11 21:03:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_TRA * FICHIER seta_tra.h
******************************************************
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  14 Oct 1994 	: Creation
* JPL		11/01/11 : Migration architecture HP ia64 (DEM 971) : declaration 'seta_etat_init'  1.7
******************************************************/

#ifndef SETA_TRA
#define SETA_TRA

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

int seta_etat_init(XZSCT_NomMachine pa_NomMachine,
		   int		va_NumOrdre,
		   int		va_Etat);

void seta_etamach_cb(T_IPC_CONN va_Conn,
                   T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	           T_CB_ARG pa_Arg);

void seta_etassys_cb(T_IPC_CONN va_Conn,
	           T_IPC_CONN_PROCESS_CB_DATA pa_Data,
		   T_CB_ARG pa_Arg);

void seta_etatach_cb(T_IPC_CONN va_Conn,
	           T_IPC_CONN_PROCESS_CB_DATA pa_Data,
		   T_CB_ARG pa_Arg);

void seta_etaperi_cb(T_IPC_CONN va_Conn,
	           T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	           T_CB_ARG pa_Arg);

void seta_etainte_cb(T_IPC_CONN va_Conn,
                   T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	           T_CB_ARG pa_Arg);


void seta_etamode_cb(T_IPC_CONN va_Conn,
	           T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	           T_CB_ARG pa_Arg);

void seta_etatrac_cb(T_IPC_CONN va_Conn,
	           T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	           T_CB_ARG pa_Arg);

void seta_etattmp_cb(T_IPC_CONN va_Conn,
	           T_IPC_CONN_PROCESS_CB_DATA pa_Data,
                   T_CB_ARG pa_Arg);

void seta_etares_cb(T_IPC_CONN va_Conn,
	           T_IPC_CONN_PROCESS_CB_DATA pa_Data,
                   T_CB_ARG pa_Arg);

void seta_etappli_cb(T_IPC_CONN va_Conn,
	           T_IPC_CONN_PROCESS_CB_DATA pa_Data,
                   T_CB_ARG pa_Arg);

void seta_etatout_cb(T_IPC_CONN va_Conn,
	           T_IPC_CONN_PROCESS_CB_DATA pa_Data,
                   T_CB_ARG pa_Arg);

void seta_bascule_cb(T_IPC_CONN va_Conn,
	           T_IPC_CONN_PROCESS_CB_DATA pa_Data,
                   T_CB_ARG pa_Arg);
int seta_mach_init(
	XZSCT_NomMachine pa_NomMachine,
	int              va_NumOrdre,
	int              va_EtatMachine);

#endif
