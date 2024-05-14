/*E Fichier : $Id: enav_cmd.h,v 1.6 1995/10/02 14:51:04 volcic Exp $      Release : $Revision: 1.6 $        Date : $Date: 1995/10/02 14:51:04 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENAV * FICHIER enav_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le fichier d'include des callbacks nav
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E 	version 1.1	22 Nov 1994	: Creation
* Nagiel.E	version 1.2	21 Dec 1995	:
* Nagiel.E	version 1.3	21 Dec 1995	: RAS
* Mismer.D	version 1.4	03 Mai 1995	: Ajout Abt etat liaison
* Fontaine.C	version 1.5	18 Mai 1995	: Reprise entete, historique
* Fontaine.C	version 1.6	21 Sep 1995	: Ajout fonction init IHM matrice NAV
*
------------------------------------------------------ */

#ifndef enav_cmd
#define enav_cmd

/* fichiers inclus */

#include	"enav_don.h"
#include	"xzen.h"


/* definitions de constantes exportees */

extern XDY_NomMachine		vg_NomMachine;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern void	en_Init_IHM_contraste_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);

extern void	ep_Init_IHM_Liaison_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
extern void	en_contraste_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
extern void	en_Init_IHM_NAV_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);

extern void	en_Init_IHM_MAT_NAV_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);

extern void	en_Emission_MAT_NAV (
        ENAV_DONNEES_NAV *,
        XDY_Datagroup );

extern void	en_cmd_SCEN_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
extern void	en_cmd_Arret_SCEN_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
extern void	en_cmd_NAV_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
extern void	en_controle_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
extern void	en_journuit ( XZEAT_JourNuit , int );

extern void	en_cmd_Mat_NAV_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
        
extern void	en_CMD_LCR ( 
        T_IPC_CONN			va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA		data,
	T_CB_ARG				arg);
		     
 
#endif
