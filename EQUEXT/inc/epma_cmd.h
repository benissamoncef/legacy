/* Fichier : epma_cmd.h
------------------------------------------------------
* GTIE/GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epma_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   C'est le fichier incluant les Callbacks PMVA
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
* JPL	10/02/23 : Declaration de la fonction ep_maj_contraste_tous  (SAE-429)
------------------------------------------------------ */

#ifndef epma_cmd
#define epma_cmd

/* fichiers inclus */

#include	"epma_don.h"
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


void	ep_Init_IHM_PMVA_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 


void	ep_cmd_PMVA_cb ( T_IPC_CONN			va_Cnx,
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
