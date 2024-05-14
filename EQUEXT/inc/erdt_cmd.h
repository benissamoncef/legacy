/*E Fichier : $Id: erdt_cmd.h,v 1.8 1996/10/09 20:41:29 mismer Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/10/09 20:41:29 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERDT * FICHIER erdt_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de commande de la tache TERDT
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	20 Dec 1994	: Creation
* Mismer.D	version 1.2	03 Mai 1995	: Ajout Abt etat liaison 
* Fontaine.C	version 1.3	22 Mai 1995	: Reprise entete
* Volcic.F	version 1.4	07 Jun 1994	: Ajout Mode Lent
* Volcic.F	version 1.5	01 Jun 1994	: Ajout Acknoledge
* Volcic.F	version 1.6	06 Sep 1996	: Modif Extension Ouest (RADT_W)
* Mismer.D	version 1.7	19 Oct 1996	: Modif Extension Ouest Suite(RADT_W)
* Mismer.D	version 1.8	04 Oct 1996	: Modif pour mesures pesage (DEM/1227)
*
------------------------------------------------------ */

#ifndef erdt_cmd
#define erdt_cmd

/* fichiers inclus */

#include	"erdt_don.h"
#include	"erdt_cal.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

void	er_controle_cb ( 
        T_IPC_CONN,
        T_IPC_CONN_PROCESS_CB_DATA,
        T_CB_ARG );
 
void	er_EMes_6mn_Voisin_cb ( 
        T_IPC_CONN,
        T_IPC_CONN_PROCESS_CB_DATA,
        T_CB_ARG);
 
void	er_EMes_6gl_Voisin_cb ( 
        T_IPC_CONN ,
        T_IPC_CONN_PROCESS_CB_DATA,
        T_CB_ARG);
 
void	er_Conf_Voie_RADT_cb ( 
        T_IPC_CONN ,
        T_IPC_CONN_PROCESS_CB_DATA,
        T_CB_ARG);
 
void	er_Dem_Indiv_cb ( 
        T_IPC_CONN,
        T_IPC_CONN_PROCESS_CB_DATA,
        T_CB_ARG);
 
void	er_Init_IHM_6gl_Voie_cb ( 
        T_IPC_CONN,
        T_IPC_CONN_PROCESS_CB_DATA,
        T_CB_ARG);

void	er_Init_IHM_6gl_Chaussee_cb ( 
        T_IPC_CONN,
        T_IPC_CONN_PROCESS_CB_DATA,
        T_CB_ARG);

void	er_Init_IHM_6mn_parVoie_cb (
        T_IPC_CONN,
        T_IPC_CONN_PROCESS_CB_DATA,
        T_CB_ARG);

void	ep_Init_IHM_Liaison_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);
 
void	er_journuit ( XZEAT_JourNuit , int );
 
void	er_etat_liaison ( XZSET_EComm_Site );
 
void	er_CMD_LCR ( T_IPC_CONN,
		     T_IPC_CONN_PROCESS_CB_DATA,
		     T_CB_ARG);

void	er_Mode_Lent_cb ( T_IPC_CONN,
		     	  T_IPC_CONN_PROCESS_CB_DATA,
		     	  T_CB_ARG);

void	er_Acknoledge_cb ( T_IPC_CONN,
		     	  T_IPC_CONN_PROCESS_CB_DATA,
		     	  T_CB_ARG);

void	er_Acknoledge_Pesage_cb ( 	T_IPC_CONN,
		     	  		T_IPC_CONN_PROCESS_CB_DATA,
		     	  		T_CB_ARG);

extern int  er_EnvoyerTrame ( 	XZEXT_Trame,
		    	 	int,
		    	 	char *,
		    	 	int,
		    	 	int,
		    	 	int);
#endif
