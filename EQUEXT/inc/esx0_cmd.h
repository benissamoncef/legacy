/*E Fichier : $Id: esx0_cmd.h,v 1.2 1995/05/23 11:42:32 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/23 11:42:32 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESX0 * FICHIER esx0_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure des donnees du fichier de commandes de la
*   tache TESX0, protocole de la tache TESAT.
*
------------------------------------------------------
* HISTORIQUE :
*
* Gabert.P	version 1.1	21 Dec 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
*
------------------------------------------------------ */


/* fichiers inclus */

#include	"esx0_don.h"
#include	"xzex.h"
#include	"xzsm.h"
#include	"xzep.h"
#include	"xzec.h"

#ifndef esx0_cmd
#define esx0_cmd

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/* RTW */
void	ed_fic_acc_cb ( 
        /* T_IPC_CONN */ int 			va_Cnx,
        /* T_IPC_CONN_PROCESS_CB_DATA */ int     pa_Data,
        /* T_CB_ARG */ int                       pa_ARG);
 
void	ed_fic_tra_cb ( 
        /* T_IPC_CONN */ int			va_Cnx,
        /* T_IPC_CONN_PROCESS_CB_DATA  */ int     pa_Data,
        /* T_CB_ARG */ int                       pa_ARG);

void	ed_fic_tvx_cb ( 
        /* T_IPC_CONN */ int			va_Cnx,
        /* T_IPC_CONN_PROCESS_CB_DATA  */ int     pa_Data,
        /* T_CB_ARG */ int                       pa_ARG);

void	ed_fic_mto_cb ( 
        /* T_IPC_CONN */ int			va_Cnx,
        /* T_IPC_CONN_PROCESS_CB_DATA  */ int     pa_Data,
        /* T_CB_ARG */ int                       pa_ARG);

void	ed_fic_pmv_cb ( 
        /* T_IPC_CONN */ int			va_Cnx,
        /* T_IPC_CONN_PROCESS_CB_DATA  */ int     pa_Data,
        /* T_CB_ARG */ int                       pa_ARG);

void	ed_fic_fer_cb ( 
        /* T_IPC_CONN */ int			va_Cnx,
        /* T_IPC_CONN_PROCESS_CB_DATA  */ int     pa_Data,
        /* T_CB_ARG */ int                       pa_ARG);


#endif
