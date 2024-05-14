/* fichier : $RCSfile: itre_rtw.h,v $   Release : $Revision: 1.16 $   Date : $Date: 2021/06/08 18:20:02 $	*
*************************************************
* STERIA * Projet MARIANE			*
*************************************************
* MODULE  : MTRE				*
* FICHIER : itre_rtw.h				*	
*************************************************
* DESCRIPTION :
* fonctions dont on peut demander l'execution
* a la tache relais
*
*************************************************
* HISTORIQUE :
*
* J.M. Guilhou	  28 Sep 1994	Creation V1.1
* J.P. Lecrivain  17 Nov 1994	Ajout vg_nomSite V1.2
* Guilhou	  19 dec 1994	interface lanceDial V1.4
* Noel 		  19 Fev 1996   Ajout de la variable vg_type_imprimante		V1.7
* Guilhou	22 aut 1996 	Ajout itre_rtw_processFMCLoc et itre_rtw_processFIN (RADT) V1.8
* Niepceron	27 aut 1996 	Ajout itre_rtw_processACQT  (dem/424) V1.9
* Niepceron	17 Sep 2008 	Ajout itre_rtw_processAlarme (DEM 808) V1.10
* JMG		xx/12/2010	Portage linux : fichiers "includes"  1.11
* JPL		23/12/2011	Ajout fonctions de gestion du Timer TdP (DEM 1007)  1.12
* JMG	24/02/12	: ajout vg_appli_est_mono_ecran linux 1.13
* JMG	29/10/18	: ajogut DEM1315 RMVI 1.14
* JMG	17/11/19 :	TDP surcharge DEM1365 1.15
* LCL	26/03/21 :	itre serveur Magistra
************************************************/

#ifndef MTRE
#define MTRE

/***********************************************/
/* Acces aux services                          */
/***********************************************/
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rtworks/common.h>
#include <rtworks/ipc.h>
#ifdef _HPUX_SOURCE
#include <rtworks/ipc_proto.h>
#endif
#include "xzst.h"
#include "itre_com.h"
#include "xzsa.h"
#include "xzic.h"
#include "xzae103.h"


/***********************************************/
/* Constantes 				       */
/***********************************************/
#define ITRE_RTW_JOINDG        1
#define ITRE_RTW_SENDPOKE        2
#define ITRE_RTW_NEWTASK 	3
#define ITRE_RTW_SETVARCTX       4
#define ITRE_RTW_GETVARCTX       5
#define ITRE_RTW_LEAVEDG       6

/***********************************************/
/* Type 				       */
/***********************************************/

/***********************************************/
/* Macros 				       */
/***********************************************/

/***********************************************/
/* acces au variables globales 		       */
/***********************************************/
extern int vg_socRecFromAx, vg_socEnvToAx;
extern int vg_socRecFromAxMce, vg_socEnvToAxMce;
extern int vg_socRecFromAxDial, vg_socEnvToAxDial;
extern XDY_Datagroup	vg_dg_mce,vg_dg_dial;
extern XDY_NomMachine	vg_nomMachine;
extern XDY_NomSite	vg_nomSite;
extern int		vg_type_imprimante;	/* le type est initialise dans itre_dir.c */
extern int		vg_type_machine;
extern int		vg_operateurPresent;
extern int		vg_appli_est_mono_ecran;

/***********************************************/
/* acces aux fonctions                         */
/***********************************************/

/* fichier : $RCSfile: itre_rtw.h,v $   Release : $Revision: 1.16 $   Date : $Date: 2021/06/08 18:20:02 $	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : MTRE				*
*************************************************
* DESCRIPTION :
* lance la main courante
*
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_lanceMce(XDY_Datagroup va_dg, char *pa_arg
);

/*
* PARAMETRE EN ENTREE :
* va_dg: datagroup de la tache coupleur
* pa_arg: chaine des arguments avec separateur
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/



/* fichier : $RCSfile: itre_rtw.h,v $   Release : $Revision: 1.16 $   Date : $Date: 2021/06/08 18:20:02 $	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : MTRE				*
*************************************************
* DESCRIPTION :
* Initialise Applix cote main courante
*
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_initMce(XDY_Datagroup va_dg,
XDY_NomMachine va_machine
);

/*
* PARAMETRE EN ENTREE :
* va_dg: datagroup de la tache coupleur
* pa_arg: chaine des arguments avec separateur
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/



/* fichier : $RCSfile: itre_rtw.h,v $   Release : $Revision: 1.16 $   Date : $Date: 2021/06/08 18:20:02 $	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : MTRE				*
*************************************************
* DESCRIPTION :
* Initialise Applix cote locaux techniques
*
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_initLT(XDY_Datagroup va_dg
);

/*
* PARAMETRE EN ENTREE :
* va_dg: datagroup de la tache coupleur
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/



/* fichier : $RCSfile: itre_rtw.h,v $   Release : $Revision: 1.16 $   Date : $Date: 2021/06/08 18:20:02 $	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : MTRE				*
*************************************************
* DESCRIPTION :
* lance la tache relais cote synoptique
* affiche la fenetre de login
*
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_lanceDial(XDY_Datagroup va_dg,
			char *va_appelant
);

/*
* PARAMETRE EN ENTREE :
* va_dg: datagroup de la tache relais
* va_appelant: module appelant 
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/


/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_IRAF_FMC_LOC
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_processFMCLoc(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg,
int va_canal_IHM
);

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/



/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_IAX_FINPOSTE
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_processFIN(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg,
int va_canal_IHM
);

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/



/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_IAX_ACQT
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_processACQT(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg,
int va_canal_IHM
);

extern int itre_rtw_processACQT_titre( 
	XDY_Entier, 
	XDY_Entier, 
	XDY_Mot, 
	XDY_NomMachine
);

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/

extern int itre_rtw_processTDP(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg,
int va_canal_IHM
);



/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_AM_alarme
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_processAlarme(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
);

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/



/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* traite le message en provenance du RT server
* XDM_ITDP_TIMER
*************************************************
* SEQUENCE D'APPEL :
*/

extern int itre_rtw_process_Timer_TdP(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
);

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/

extern int itre_Timer_TdP_activer(
int va_en_fonction
);



/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* fonction utilisateur passe a
* XZAE103_Abt_Presence_Op_PCsimpl
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_presence_PCS(XZAET_EPresence_Op_PCsimpl	va_presence
);

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* appel suppression semaphore sur message magistra
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_process_del_sema(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
);

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* applique le pra sur le po sur message magistra
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_process_pra_po(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
);

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* appel configuration modern sur message magistra
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_process_conf_modern(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
);

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* appel fonctions remorque mobile sur message magistra
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_process_fct_remorque(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
);

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* appel fonctions remorque CA mobile sur message magistra
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_process_fct_remorque_CA(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
);

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* appel viabilite hivernal
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_process_viabilite_hivernal(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
);

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* appel viabilite hivernal ASF
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_process_viabilite_hivernal_ASF(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
);

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* appel viabilite hivernal pour transfert ftp sur message magistra
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_process_viabilite_hivernal_export(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
);

/*************************************************
* MODULE  : MTRE                                *
*************************************************
* DESCRIPTION :
* appel lt alarmes sur message magistra
*************************************************
* SEQUENCE D'APPEL :
*/

extern void itre_rtw_process_lt_alarme(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
);

/*
* PARAMETRE EN ENTREE :
*
* PARAMETRE EN SORTIE :
*
* VALEUR DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITION LIMITES :
************************************************/


extern int itre_rtw_processRMVI_status(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
);
extern int itre_rtw_processRMVI_retour(
T_IPC_CONN va_cnx,
T_IPC_CONN_PROCESS_CB_DATA pa_data,
T_CB_ARG pa_arg
);


#endif
