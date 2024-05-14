/*E*/
/* Fichier : $Id: seta_con.h,v 1.2 1994/10/24 17:19:22 milleville Exp $	Release : $Revision: 1.2 $        Date : $Date: 1994/10/24 17:19:22 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_CON * FICHIER seta_con.h
******************************************************
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  14 Oct 1994	: Creation
*
******************************************************/

#ifndef SETA_CON
#define SETA_CON

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

void seta_stmodei_cb(T_IPC_CONN ca_Cnx,
	      T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	      T_CB_ARG Arg);

void seta_stmoder_cb(T_IPC_CONN ca_Cnx,
	      T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	      T_CB_ARG Arg);


#endif
