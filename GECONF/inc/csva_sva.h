/*E Fichier : $Id: csva_sva.h,v 1.5 2016/05/25 17:07:56 devgfi Exp $      Release : $Revision: 1.5 $        Date : $Date: 2016/05/25 17:07:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE SVA * FICHIER SVA.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module conforme a la DCG-002/0.1.
* Contient le callback de sauvegarde hebdomadaire des bases.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	06 Jan 1995	: Creation
* C.T.		17 Mar 1998	: modif capacite dat
* JPL		24/05/2016	: Remplacement nom periph. par CSVAC_PERIPH_SVG_DEFAUT (DEM 1165)  1.5
------------------------------------------------------ */

#ifndef SVA
#define SVA

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
	#include        "xzsc.h"
	#include        "xzsm.h"
	#include        "xzcg.h"
	#include        "xzia.h"
	#include        "xzagc.h"
	#include        "xzca.h"

/* definitions de constantes exportees */
#define CSVAC_BASE_SVG 			"SVG"
#define CSVAC_SAUVEGARDE_CFG		XZCGC_SAUVEGARDE_CFG
#define CSVAC_SITE_SAUVEGARDE		"CI"
#define CSVAC_PERIPH_SVG_DEFAUT		"/dev/rmt/0mn"

#define CSVAC_SVG_GLOB			XZAGC_SVG_GLOB
#define CSVAC_FIC_TRACE_SVG		XDC_PATHCNFFICDYN "/" XDF_BILAN_SVG_BASE_HIST
#define CSVAC_FIC_TRACE_REST		XDC_PATHCNFFICDYN "/" XDF_BILAN_REST_BASE_HIST

#define CSVAC_FIC_TRACE_SVG_REST_TEMPO	XDC_REP_TMP "/" XDF_BILAN_REST_BASE_HIST
#define CSVAC_BASE_HEBDO		"hebdo"
#define CSVAC_CAPACITE_DAT		1433600 /* =  2048000*0.70 */
#define CSVAC_NOMFIC_DAT		"ARCHIVAGE_HISTOR"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
extern XZIAT_destinataire 	CSVAV_NOMMACHINE;

/* delaration de fonctions externes */
/*
*	csva_sauvebd_cb est declenchee sur reception du message RTWks XDM_CSV_ARC.
*	Ce CallBack va effectuer une sauvegarde des bases sur bande et vide le journal
* des transactions,
* previent l administrateur de l etat de la sauvegarde.
*	
*/

extern void csva_sauvebd_cb (	T_IPC_CONN 			va_Cnx,
        			T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        			T_CB_ARG                        pa_ARG) ;

#endif
