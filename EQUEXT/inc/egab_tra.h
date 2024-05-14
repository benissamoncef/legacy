/*Fichier :  $Id: egab_tra.h,v 1.2 2018/10/26 16:03:14 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/10/26 16:03:14 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDAI * FICHIER egab_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de traitement de la 
*  tache TEDAI. 
*
------------------------------------------------------
* HISTORIQUE :
*
* 
* JMG	16/05/18 creation 1.1
* JMG	26/10/18 ajout cb LCR 1.2
------------------------------------------------------ */

#ifndef egab_tra
#define egab_tra

/* fichiers inclus */

#include "ex_msta.h"


/* definitions de constantes exportees a deplace ds xdc.h */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
int  em_traitement_trame(int va_socket);
extern void    ep_CMD_LCR ( T_IPC_CONN                 va_Cnx,
                     T_IPC_CONN_PROCESS_CB_DATA         data,
		                          T_CB_ARG                           arg);

#endif
