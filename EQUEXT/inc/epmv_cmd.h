/* Fichier : epmv_cmd.h
------------------------------------------------------
* GTIE/GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epmv_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   C'est le fichier incluant les Callbacks PMV
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	13 Sep 1994	: Creation
* Nagiel.E	version 1.2	23 Nov 1994	: Ajout de ep_cmd_PICTO_cb
* Mismer.D	version 1.3	08 Mar 1995	:
* Mismer.D	version 1.4	03 Mai 1995	: Ajout Abt etat liaison 
* Fontaine.C	version 1.5	18 Mai 1995	: Reprise entete, historique
* JPL		16/11/22 : Declaration de la fonction ep_maj_contraste_tous  SAE-429
------------------------------------------------------ */

#ifndef epmv_cmd
#define epmv_cmd

/* fichiers inclus */

#include	"epmv_don.h"
#include	"xzex.h"
#include	"xzsm.h"
#include	"xzep.h"
#include	"xzec.h"
/*#include	"xzat.h"*/

/* definitions de constantes exportees */

extern XDY_NomMachine		vg_NomMachine;
extern int			vg_Contexte;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */


void	ep_Init_IHM_PMV_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
void	ep_Init_IHM_Picto_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
void	ep_cmd_PICTO_cb ( T_IPC_CONN		va_Cnx,
		     T_IPC_CONN_PROCESS_CB_DATA		data,
		     T_CB_ARG				arg);

void	ep_cmd_PMV_cb ( T_IPC_CONN			va_Cnx,
		     T_IPC_CONN_PROCESS_CB_DATA		data,
		     T_CB_ARG				arg);

void	ep_contraste_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
void	ep_controle_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
		    
void	ep_journuit ( XZEAT_JourNuit va_contexte, int va_resultat);

void	ep_maj_contraste_tous ( int va_Contexte );

void	ep_Init_IHM_contraste_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
void	ep_Init_IHM_Liaison_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
void	ep_CMD_LCR ( T_IPC_CONN			va_Cnx,
		     T_IPC_CONN_PROCESS_CB_DATA		data,
		     T_CB_ARG				arg);
		     
#endif
