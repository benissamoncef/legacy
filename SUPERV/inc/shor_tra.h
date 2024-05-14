/* Fichier : $Id: shor_tra.h,v 1.2 1994/11/22 16:42:48 milleville Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/11/22 16:42:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SUPERV
------------------------------------------------------
* MODULE SHOR_TRA * FICHIER shor_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Interface du module SHOR_TRA
*
------------------------------------------------------
* HISTORIQUE :
*
* Milleville.T	10 Aug 1994	: Creation
------------------------------------------------------ */

#ifndef SHOR_TRA
#define SHOR_TRA

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern void sh_actualiserhoro_cb(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);

extern void sh_synchroniser_cb(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);

extern void sh_actualisersyste_cb(
        T_IPC_CONN va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);

#endif
