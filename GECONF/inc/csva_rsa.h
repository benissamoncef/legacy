/*E Fichier : $Id: csva_rsa.h,v 1.2 2016/05/25 17:07:41 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2016/05/25 17:07:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE RSA * FICHIER RSA.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*
*	Module MRSA de TCSVA contenant le callback permettant
*  de restaurer une base archiver sur bande dans la base 
*  sauvegarde/restauration.
* DCG-002/0.1
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	06 Jan 1995	: Creation
* JPL		24/05/2016	: Remplacement nom periph. par CRSAC_PERIPH_RSTR_DEFAUT (DEM 1165)  1.2
------------------------------------------------------ */

#ifndef RSA
#define RSA

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

/* definitions de constantes exportees */

#define CRSAC_FIC_TRACE_REST		XDC_PATHCNFFICDYN "/" XDF_BILAN_REST_BASE_HIST
#define CRSAC_FIC_TRACE_REST_TEMPO	XDC_REP_TMP "/" XDF_BILAN_REST_BASE_HIST
#define CRSAC_SITE_SAUVEGARDE		"CI"
#define CRSAC_PERIPH_RSTR_DEFAUT	"/dev/rmt/0mn"
#define CRSAC_BASE_SVG 			"SVG"
#define CRSAC_NOMFIC_DAT		"ARCHIVAGE_HISTOR"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

extern XZIAT_destinataire 	CSVAV_NOMMACHINE;

/* delaration de fonctions externes */
/*
*	crsa_restarc_cb est declenchee sur reception du message RTWks XDM_CSV_RES.
*	Ce CallBack va effectuer une restauration des bases de la bande dans la base
*  sauvegarde/restauration.
*/
extern void crsa_restarc_cb (	T_IPC_CONN 			,
        			T_IPC_CONN_PROCESS_CB_DATA      ,
        			T_CB_ARG                        ) ;

#endif
