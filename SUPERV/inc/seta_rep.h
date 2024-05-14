/*E*/
/* Fichier : $Id: seta_rep.h,v 1.2 1994/10/24 17:19:53 milleville Exp $	Release : $Revision: 1.2 $        Date : $Date: 1994/10/24 17:19:53 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_REP * FICHIER seta_rep.h
******************************************************
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
******************************************************
* HISTORIQUE :
*
* L.GEORGES	23 Mar 1994	: Cration
******************************************************/

#ifndef SETA_REP
#define SETA_REP

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern void seta_synopag_cb(T_IPC_CONN va_Conn,
              T_IPC_CONN_PROCESS_CB_DATA pa_Data,
              T_CB_ARG pa_Arg);

extern void seta_synopss_cb(T_IPC_CONN va_Conn,
              T_IPC_CONN_PROCESS_CB_DATA pa_Data,
              T_CB_ARG pa_Arg);

extern void seta_synoptc_cb(T_IPC_CONN va_Conn,
              T_IPC_CONN_PROCESS_CB_DATA pa_Data,
              T_CB_ARG pa_Arg);

extern void seta_synopcz_cb(T_IPC_CONN va_Conn,
              T_IPC_CONN_PROCESS_CB_DATA pa_Data,
              T_CB_ARG pa_Arg);

extern void seta_synopts_cb(T_IPC_CONN va_Conn,
              T_IPC_CONN_PROCESS_CB_DATA pa_Data,
              T_CB_ARG pa_Arg);

extern void seta_diffetat_cb(T_IPC_CONN va_Conn,
              T_IPC_CONN_PROCESS_CB_DATA pa_Data,
              T_CB_ARG pa_Arg);

extern void seta_diffetat(XZSCT_Datagroup pa_NomDatagroup);

#endif
