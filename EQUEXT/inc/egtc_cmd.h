/*E*/
/*Fichier : $Id: egtc_cmd.h,v 1.4 1996/12/05 22:37:08 mismer Exp $       Release : $Revision: 1.4 $     Date : $Date: 1996/12/05 22:37:08 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEGTC * FICHIER egtc_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier de commande de la 
* tache TEGTC
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	19 Dec 1994	: Creation
* Mismer.D	version 1.2	03 Mai 1995	: Ajout Abt etat liaison 
* Fontaine.C	version 1.3	19 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.4 	15 Nov 1996	: Modif pour alerte DAI tunnel (DEM/1309)
*
------------------------------------------------------ */

#ifndef egtc_cmd
#define egtc_cmd

/* fichiers inclus */

#include	"egtc_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */


void	eg_Init_IHM_GTC_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
void	eg_Init_IHM_JourNuit_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
  
void	ep_Init_IHM_Liaison_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
void	eg_controle_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
		     
void	eg_CMD_LCR ( 
	T_IPC_CONN			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA	data,
	T_CB_ARG			arg);
		     
void	eq_commande_sortie_TOR ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
        
#endif
