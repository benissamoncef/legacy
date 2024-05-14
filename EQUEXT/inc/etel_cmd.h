/*E Fichier : @(#)etel_cmd.h	1.3      Release : 1.3        Date : 05/22/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE etel_cmd * FICHIER etel_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Declaration des variables et fonctions du fichier de commande de la tache TETEL
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1	30 Nov 1994	: Creation
* Fontaine.C    Version 1.2     10 Mar 1995     : Declaration d'ESCAPE
* Fontaine      Version 1.3     04 Mai 1995     : Reprise entete et suppression ESCAPE
* Mismer.D      Version 1.7     22 Mai 1995     : Changement nom fonction EnvoyerTrame
* Diaz.L	Version 1.8     09 Oct 1998     : Ajout PABX
*------------------------------------------------------ */

#ifndef etel_cmd
#define etel_cmd

/* fichiers inclus */

#include	"etel_don.h"
#include	"xzex.h"
#include	"xzsm.h"
#include	"xzep.h"
#include	"xzec.h"

/* definitions de constantes exportees */

/* extern XDY_NomMachine		vg_NomMachine; */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
XDY_NomMachine                       vg_NomMachine;

/* delaration de fonctions externes */

void	ed_init_MAA () ;

void	ed_init_etat_AUTOTEL ( 
        T_IPC_CONN 		 	va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
        T_CB_ARG 			pa_ARG);
 
void	ed_cmd_AUTOTEL ( 
        T_IPC_CONN 		        va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG			pa_ARG);
        
void	ed_controle_cb ( 
        T_IPC_CONN 		        va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG			pa_ARG);


int  EnvoyerTrame_Tel ( char*	trame,
		    int		type_msg,
		    char*	adr_rgs,
		    int 	nocmd,
		    int 	priorite,
		    int		socket);
		    
void	ed_cmd_ALCATEL ( 
        T_INT4 	 		va_CodeCde,
        T_STR			va_NumTel,
        T_INT2 			va_NumeroPO);     

#endif
