/*E*/
/* Fichier : $Id: spng_tra.h,v 1.3 1995/07/20 14:01:26 milleville Exp $        Release : $Revision: 1.3 $        Date : $Date: 1995/07/20 14:01:26 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SPNG_TRA * FICHIER spng_tra.h
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module contient les deux fonctions qui sont activees
* sur reception d'un message RTWORKS, il s'agit de la
* fonction d'armement d'un test reseau, et de la fonction
* de desarmement de test reseau.
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 30 SEp 1994	: Creation
*
******************************************************/

#ifndef SPNG_TRA
#define SPNG_TRA

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern void sg_arme_test_cb(T_IPC_CONN va_Conn,
                   T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	           T_CB_ARG pa_Arg);

extern void sg_arret_test_cb(T_IPC_CONN va_Conn,
	           T_IPC_CONN_PROCESS_CB_DATA pa_Data,
		   T_CB_ARG pa_Arg);

extern void sg_verif_test_cb(T_IPC_CONN va_Conn,
	           T_IPC_CONN_PROCESS_CB_DATA pa_Data,
		   T_CB_ARG pa_Arg);

extern void se_test_res(XZSCT_NomMachine pa_NomMachine, int *pa_EtatMachine);

#endif
