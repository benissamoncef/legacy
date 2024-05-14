/*E*/
/*Fichier :  $Id: eniw_cmd.h,v 1.2 2002/04/24 08:48:37 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2002/04/24 08:48:37 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIW * FICHIER eniw_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de commande de la
* tache TENIW, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Moulin.D	version 1.1	07 Janv 2002	: Creation
* Moulin.D	version 1.2	23 Avr 2002	: Installation plate forme
*
------------------------------------------------------ */
 
#ifndef eniw_cmd
#define eniw_cmd

/* fichiers inclus */

#include	"eniw_don.h"

/* definitions de constantes exportees */

extern XDY_NomMachine		vg_NomMachine;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
extern int vg_maitre_actif;

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
