/*E*/
/* Fichier : $Id: seta_ini.h,v 1.4 1995/05/29 15:01:03 milleville Exp $	Release : $Revision: 1.4 $        Date : $Date: 1995/05/29 15:01:03 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_INI * FICHIER seta_ini.h
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
* T.Milleville  09 Avr 1995 	: Ajout fonction seta_init_starttmp_cb V1.4
*
******************************************************/

#ifndef SETA_INI
#define SETA_INI

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void seta_init_cb(T_IPC_CONN va_Cnx,
		      T_IPC_CONN_PROCESS_CB_DATA pa_Data,
		      T_CB_ARG pa_Arg);

extern void seta_init_starttmp_cb(T_IPC_CONN va_Cnx,
		      T_IPC_CONN_PROCESS_CB_DATA pa_Data,
		      T_CB_ARG pa_Arg);


extern int seta_init();


#endif
