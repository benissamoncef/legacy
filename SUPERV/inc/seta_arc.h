/*E*/
/* Fichier : $Id: seta_arc.h,v 1.3 1994/11/22 16:42:30 milleville Exp $	Release : $Revision: 1.3 $        Date : $Date: 1994/11/22 16:42:30 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_ARC * FICHIER seta_arc.h
******************************************************
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
******************************************************
* HISTORIQUE :
*
* T.MIlleville  14 Oct 1994 	: Creation
******************************************************/

#ifndef SETA_ARC
#define SETA_ARC

/* fichiers inclus */
#include "xzsc.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void seta_archiver_cb(T_IPC_CONN va_Conn,
                          T_IPC_CONN_PROCESS_CB_DATA pa_Data,
                          T_CB_ARG pa_Arg);

extern int seta_diffuser_msg(XZSCT_NomMachine pa_NomMachine,
 		            int              va_NumOrdre,
		            void             **pa_DonneesMsg);

extern void seta_mesg_cb(T_IPC_CONN va_Conn,
                       T_IPC_CONN_PROCESS_CB_DATA pa_Data,
                       T_CB_ARG pa_Arg);

extern void **seta_message(int va_NumTexte, int va_NbChaine, ...);

#endif
