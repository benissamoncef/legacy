/*E*/
/* Fichier : $Id: seta_sur.h,v 1.5 2011/01/11 21:02:11 gesconf Exp $        Release : $Revision: 1.5 $        Date : $Date: 2011/01/11 21:02:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETAT_SUR * FICHIER seta_sur.h
******************************************************
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
******************************************************
* HISTORIQUE :
*
* T.Milleville   14 Oct 1994: Creation
*
* T.Milleville   14 Dec  1995: Ajout constante  SEC_SURV_ACTIVE_SC
*	pour permettre une surveillance inter-SC sans pour cela
*	etre supervise (Remontee des etats machines)  V1.4
* JPL		11/01/11 : Migration architecture HP ia64 (DEM 971) : declaration 'seta_trait_mach_xxx'  1.5
*****************************************************/

#ifndef SETA_SUR_H
#define SETA_SUR_H

#include <rtworks/ipc.h>
#include "xzsc.h"

#define SEC_SURV_PASSIVE 0
#define SEC_SURV_ACTIVE 1

/*  Modif V1.4 */
#define SEC_SURV_ACTIVE_SC 2 


void seta_synchromachine_cb(T_IPC_CONN va_Conn,
		     T_IPC_CONN_PROCESS_CB_DATA pa_Data,
		     T_CB_ARG pa_Arg);

void seta_surv_st(XZSCT_NomMachine pa_NomMachine,
	   int va_NumOrdre);

void seta_surv_sc(XZSCT_NomMachine pa_NomMachine,
	   int va_NumOrdre);

void seta_surv_sd(XZSCT_NomMachine pa_NomMachine,
	   int va_NumOrdre);

void seta_arret_surv();

int seta_arme_surv(XZSCT_NomMachine pa_NomMachine);

int seta_trait_mach_surv(void (*pa_Traitement)());

int seta_trait_mach_superv(void (*pa_Traitement)());

void seta_dgstatus_cb (XZSCT_Datagroup pa_NomDatagroup,
		     int va_NbClient);

void seta_survie_cb(T_IPC_CONN va_Conn,
              T_IPC_CONN_PROCESS_CB_DATA pa_Data,
              T_CB_ARG pa_Arg);

void seta_msgvie_cb(T_IPC_CONN va_Conn,
              T_IPC_CONN_PROCESS_CB_DATA pa_Data,
              T_CB_ARG pa_Arg);

#endif
