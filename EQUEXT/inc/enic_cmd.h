/*E*/
/* Fichier : $Id: enic_cmd.h,v 1.2 1995/05/22 15:46:45 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/22 15:46:45 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIC * FICHIER enic_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de commande de la
* tache TENIC, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	03 Nov 1994	: Creation
* Fontaine.C	version 1.2	22 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef enic_cmd
#define enic_cmd

/* fichiers inclus */

#include	"enic_don.h"

/* definitions de constantes exportees */

extern XDY_NomMachine		vg_NomMachine;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern	void	et_Init_IHM_TNIC_cb ( 	T_IPC_CONN,
        				T_IPC_CONN_PROCESS_CB_DATA,
        				T_CB_ARG );

extern	void	et_cmd_sigt_cb	    ( 	T_IPC_CONN,
        				T_IPC_CONN_PROCESS_CB_DATA,
        				T_CB_ARG );

extern	void	et_cmd_sige_cb 	    ( 	T_IPC_CONN,
        				T_IPC_CONN_PROCESS_CB_DATA,
        				T_CB_ARG );

extern	void	et_cmd_seqt_cb      ( 	T_IPC_CONN,
        				T_IPC_CONN_PROCESS_CB_DATA,
        				T_CB_ARG );

extern	void	et_cmd_seqe_cb      ( 	T_IPC_CONN,
        				T_IPC_CONN_PROCESS_CB_DATA,
        				T_CB_ARG );

extern	void	et_controle_cb 	    ( 	T_IPC_CONN 			va_Cnx,
        				T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        				T_CB_ARG                        pa_ARG);



#endif
