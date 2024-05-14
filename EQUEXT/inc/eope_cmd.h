/*E Fichier : $Id: eope_cmd.h,v 1.3 1995/05/22 17:12:42 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/22 17:12:42 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TEOPE * FICHIER eope_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Declaration des variables et fonctions du fichier de commande de la tache TEOPE
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1 02 Dec 1994	: Creation
* Fontaine.C    Version 1.2 04 Mai 1995 : Reprise entete
* Mismer.D      Version 1.3 22 Mai 1995 : Changement nom fonction EnvoyerTrame
------------------------------------------------------ */

#ifndef eope_cmd
#define eope_cmd

/* fichiers inclus */

#include	"eope_don.h"
#include	"xzex.h"
#include	"xzsm.h"
#include	"xzep.h"
#include	"xzec.h"

/* definitions de constantes exportees */

/* extern XDY_NomMachine		vg_NomMachine; */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

void	ed_init_etat_OPER ( 
        T_IPC_CONN 		 	va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
        T_CB_ARG 			pa_ARG);
 
void	ed_cmd_OPER ( 
        T_IPC_CONN 		        va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG			pa_ARG);
        
void	ed_controle_cb ( 
        T_IPC_CONN 		        va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG			pa_ARG);


int  EnvoyerTrame_Operator ( char*	trame,
		    int		type_msg,
		    char*	adr_rgs,
		    int 	nocmd,
		    int 	priorite,
		    int		socket);
		    
     
#endif
