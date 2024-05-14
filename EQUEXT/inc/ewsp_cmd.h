/*E*/
/*  Fichier : $Id: ewsp_cmd.h,v 1.0 2022/01/18 08:00:00 pc2dpdy Exp $      Release : $Revision: 1.0 $        Date : $Date: 2022/01/18 08:00:00 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER ewsp_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier commande de la tache TEWSP
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	18/01/22	creation
------------------------------------------------------ */

#ifndef ewsp_cmd
#define ewsp_cmd

/* fichiers inclus */
#include	<xdy.h>
#include	<xdc.h>
#include <rtworks/ipc.h>

/* definitions de constantes exportees */

/* definitions de types exportes */

/* structure utilisee pour stocker les info AVA*/
	typedef struct {
		char id_sca[256];
		char id_ava[256]; 
		XDY_Entier id_mission;
		char horodate_debut[256];
		char horodate_fin[256];
		char statut_fin[256];
		char cause_annulation[256];
		char type_vehicule[256];
		char nom_autoroute[10];
		XDY_Sens sens;
		XDY_Entier pr;
		XDY_Entier distance;
		char type_localisant[256];
		char id_localisation[256];
		char position_sur_voies[256];
		char cause[256];
		char particularites[256];
		XDY_Entier occupant;
		char commentaires[256];
		char horodate_debut_prise_en_charge_client[256];
		char horodate_lancement_mission[256];
		char origine[256];
		char complement_origine[256];
		XDY_Entier dangerosite;
	}T_AVA;

typedef struct S_T_AVA {
	T_AVA		ava;
	struct S_T_AVA 	*suivant;
	struct S_T_AVA 	*precedent;
} T_Liste_AVA;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void ewsp_ava_cb_creation( T_IPC_CONN va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA pa_data,
		T_CB_ARG	pa_Arg );
extern void ewsp_ava_cb_modification( T_IPC_CONN va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA pa_data,
		T_CB_ARG	pa_Arg );
extern void ewsp_ava_cb_retour_alerte( T_IPC_CONN va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA pa_data,
		T_CB_ARG	pa_Arg );
		
#endif
