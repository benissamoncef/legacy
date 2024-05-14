/*E Fichier : $Id: edai_cmd.h,v 1.4 1995/05/23 10:56:21 cornu Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/05/23 10:56:21 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDAI * FICHIER edai_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de commande de la 
*  tache TEDAI.
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	18 Oct 1994	: Creation
* Nagiel.E	version 1.2	22 Dec 1994	:
* Mismer.D	version 1.3	03 Mai 1995	: Ajout Abt etat liaison
* Fontaine.C	version 1.4	23 Mai 1995	: Reprise entete, historique
* GGY		20/11/23 : Ajout ed_Enregistrement_DAI_cb (DEM483)
------------------------------------------------------ */

#ifndef edai_cmd
#define edai_cmd

/* fichiers inclus */

#include	"edai_don.h"
/*#include	"xzex.h"
#include	"xzsm.h"
#include	"xzep.h"
#include	"xzec.h"*/

/* definitions de constantes exportees */

extern XDY_NomMachine		vg_NomMachine;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */


void	ed_Init_IHM_Mes_DAI_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
void	ed_controle_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);

void	ed_Conf_Voie_DAI_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);

void	ep_Init_IHM_Liaison_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
void	ed_CMD_LCR ( T_IPC_CONN			va_Cnx,
		     T_IPC_CONN_PROCESS_CB_DATA		data,
		     T_CB_ARG				arg);

void	ed_Enregistrement_DAI_cb (	T_IPC_CONN					va_Cnx,
        							T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
       								T_CB_ARG					pa_ARG);
		     
#endif
