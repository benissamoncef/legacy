/*E Fichier : $Id: cgcd_util.h,v 1.1 1995/01/25 10:40:24 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/25 10:40:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE MUTI * FICHIER MUTI.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de TCGCD contenant les utilitaires de la tache.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	18 Jan 1995	: Creation
------------------------------------------------------ */

#ifndef MUTI
#define MUTI

/* fichiers inclus */
	/* includes systeme */
	
	#include        <stdio.h>

	/* includes Utilitaires */
	
		/* rtworks */
	#include        <rtworks/common.h>
	#include        <rtworks/ipc.h>
	#include        <rtworks/ipc_proto.h>
	
	/* includes applicatifs Migrazur */
		
	#include        "xdc.h"
	#include        "xdy.h"
	#include        "xdg.h"
	#include        "xdf.h"	
	#include        "xzsm.h"
	#include        "xzsc.h"
	#include        "xzia.h"

/* definitions de constantes exportees */

#define CUTIC_TYPE_SYSTEM 	(XZSMC_TYPE_SYSTEME)

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

extern XZIAT_destinataire 	CGCDV_NOMMACHINE;
extern int			CGCDV_CHECKLIST;
extern FILE			*CGCDG_Fich_BilanConfig ;
extern FILE			*CGCDG_Fich_CHECKLIST	;
extern char 			CGCDG_Base_Courante[4];

/* delaration de fonctions externes */
/*
*  cuti_Recup_Horodate : Fonction permettant de recuperer l horodate courante
*  sous forme de chaine : utilisee pour initialiser
*  les Callbacks de la tache TCGCD.
*/
extern int cuti_Recup_Horodate(XZSMT_Horodate);

/*
* cuti_BilanConf_Trace  permet d ecrire dans le fichier bilan config 
* le message passe en argument
*/
extern int cuti_BilanConf_Trace(char *);

/*  
*  cuti_Chgmt_Base_Courante permet de changer de base courante.
*  Pour le passage de la Base courante a CFT, il n y a pas de verification.
*  Pour passer la Base courante a CFG, il faut que le tableau de checklist soit
*  Valider : pour tous les types de Verif, le flag doit etre positionne a XDC_OK.
*  On envoie un message a l IHM de l administrateur pour l informer de toute demande.
*/
extern int cuti_Chgmt_Base_Courante(char *);

/*  
*	cuti_CanalComInit_cb est declenchee sur reception du message RTWks XDM_CGD_CANAL.
*  Permet de positionne la Globale CGCDV_NOMMACHINE vers la quelle on envoie les messgae 
*  d administration (via xzia08).
*
*/
extern void cuti_CanalComInit_cb (	T_IPC_CONN,
        				T_IPC_CONN_PROCESS_CB_DATA,
        				T_CB_ARG);
/*
*	cuti_InitChecklist_cb est declenchee sur reception du message RTWks XDM_CGD_CHECK.
*  Permet de positionne la Globale CGCDV_CHECKLIST permettant d activer toutes les verifications checklist
*  de TCGCD.
*/

extern void cuti_InitChecklist_cb (	T_IPC_CONN,
        			T_IPC_CONN_PROCESS_CB_DATA,
        			T_CB_ARG);
/*
*	cuti_ChgmtBaseCour_cb permet de changer de base courante.
*/

extern void cuti_ChgmtBaseCour_cb (	T_IPC_CONN 			va_Cnx,
        			T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        			T_CB_ARG                        pa_ARG); 

#endif
