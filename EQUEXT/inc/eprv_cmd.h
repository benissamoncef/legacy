/*E Fichier : $Id: eprv_cmd.h,v 1.1 2012/07/04 14:27:49 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 14:27:49 $
------------------------------------------------------
* GTIE/GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER eprv_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   C'est le fichier incluant les Callbacks PRV
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	19/12/11		: Creation (DEM/1016)
------------------------------------------------------ */

#ifndef prv_cmd
#define eprv_cmd

/* fichiers inclus */

#include	"eprv_don.h"
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
#define rtrim(c) \
        int q;q=strlen(c); while(q>1 && c[--q] == ' ') c[q]='\0';if (c[0]==' ') c[0]='\0'

/* declarations de donnees globales */

/* delaration de fonctions externes */


void	ep_Init_IHM_PRV_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 


void	ep_cmd_PRV_cb ( T_IPC_CONN			va_Cnx,
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
