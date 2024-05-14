/*E Fichier : $Id: cgcd_vce.h,v 1.14 2019/05/15 16:48:42 devgtie Exp $      Release : $Revision: 1.14 $        Date : $Date: 2019/05/15 16:48:42 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE MVCE * FICHIER MVCE.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module conforme a la DCG-002/0.1.
* Contient les callbacks de verification et duplication de 
* la base.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	13 Oct 1994	: Creation
***********
* MERCIER	22 Dec 1994	: Ajout	de		V 1.3
*	CVCEC_SAUVEGARDE_CFG et CVCEC_RESTAURATION_DANS_CFT
*  CVCEC_SITE_SAUVEGARDE
***********
* MERCIER	15 Jan 1995	: Suppressiond de CVCEC_VERIF_TYPE V 1.4
* C.T.		17 Mar 1998	: Modif capacite bande V1.5
* JPL		24/05/2016	: Remplacement nom periph. par CVCEC_PERIPH_SVG_DEFAUT (DEM 1165)  1.12
* RGR		03/05/2019	: Retrait ipc_proto.h						   1.13
------------------------------------------------------ */

#ifndef MVCE
#define MVCE

/* fichiers inclus */
	/* includes systeme */
	
	#include        <stdio.h>

	/* includes Utilitaires */
	
		/* rtworks */
	#include        <rtworks/common.h>
	#include        <rtworks/ipc.h>
	
	/* includes applicatifs Migrazur */
	
	
	#include        "xdc.h"
	#include        "xdy.h"
	#include        "xdg.h"
	#include        "xzsc.h"
	#include        "xzsm.h"
	#include        "xzcg.h"
	#include        "xzia.h"
	#include        "xzagc.h"
	#include  	"cgcd_util.h"

/* definitions de constantes exportees */
/* Tableau de verification */

#define CVCEC_VERIF_SCNT		XZCGC_VERIF_SCNT	
#define CVCEC_VERIF_SNAV		XZCGC_VERIF_SNAV	
#define CVCEC_VERIF_DEFT		XZCGC_VERIF_DEFT
#define CVCEC_VERIF_SIHM		XZCGC_VERIF_SIHM	
#define CVCEC_VERIF_RESE		XZCGC_VERIF_RESE	
#define CVCEC_VERIF_EQAL		XZCGC_VERIF_EQAL	
#define CVCEC_VERIF_EQUI		XZCGC_VERIF_EQUI	
#define CVCEC_VERIF_RSAD		XZCGC_VERIF_RSAD	
#define CVCEC_VERIF_SGBD		XZCGC_VERIF_SGBD	
#define CVCEC_VERIF_MACH		XZCGC_VERIF_MACH	
#define CVCEC_VERIF_GLOBAL		XZCGC_VERIF_GLOBAL

/*******/

#define CVCEC_SAUVEGARDE_CFG		XZCGC_SAUVEGARDE_CFG
#define CVCEC_RESTAURATION_DANS_CFT	XZCGC_RESTAURATION_DANS_CFT
#define CVCEC_SITE_SAUVEGARDE		"CI"
#define CVCEC_PERIPH_SVG_DEFAUT		"/dev/rmt/0mn"
/*
#define CVCEC_DEVICE_SAUVEGARDE         "/dev/rmt/c4t8d0BESTn"
*/
#define CVCEC_DEVICE_SAUVEGARDE         "/dev/rmt/0mn"

#define CVCEC_NOM_VOL_SAUVEGARDE_AUTO	"CFGCFT"
#define CVCEC_SVG_GLOB			XZAGC_SVG_GLOB
#define CVCEC_FIC_TRACE_SVG		XDC_PATHCNFFICDYN "/" XDF_BILAN_SVG_BASE
#define CVCEC_FIC_TRACE_REST		XDC_PATHCNFFICDYN "/" XDF_BILAN_REST_BASE

#define CVCEC_FIC_TRACE_SVG_REST_TEMPO	XDC_REP_TMP "/" XDF_BILAN_REST_BASE
#define CVCEC_BASE_CFG			"CFG"
#define CVCEC_BASE_CFT			"CFT"
#define CVCEC_CAPACITE_DAT		1433600 /* =  2048000*0.70 */
#define CVCEC_NOMFIC_DAT		"SVG_CFG_AVANT_VER"

/* definitions de globales exportes */

extern XZIAT_destinataire CGCDV_NOMMACHINE;
extern  char CGCDG_Base_Courante[4];

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void cvce_VerifModif_cb(	T_IPC_CONN ,
        			T_IPC_CONN_PROCESS_CB_DATA,
        			T_CB_ARG);
extern void cvce_majbd_cb(	T_IPC_CONN ,
        			T_IPC_CONN_PROCESS_CB_DATA,
        			T_CB_ARG);

extern int cvce_Prerequis_ActionSurBase (); 
#endif
