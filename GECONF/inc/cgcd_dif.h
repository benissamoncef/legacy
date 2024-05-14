/*E Fichier : $Id: cgcd_dif.h,v 1.8 1999/02/26 10:47:15 gaborit Exp $      Release : $Revision: 1.8 $        Date : $Date: 1999/02/26 10:47:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE MDIF * FICHIER MDIF.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de livraison de tcgcd gerant le configuration.
* (DCG-002/0.1 page 35).
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	13 Dec 1994	: Creation
* niepceron	08 Jun 1995	: Ajout des constantes relatives aux fic ds .../inf V1.3
* niepceron	29 Jan 1996	: Ajout du ssysteme DIE v1.5
* niepceron	05 Fev 1996	: Ajout du ssysteme MOD v1.6
* niepceron     21 Oct 1996	: Ajout des ssystemes OPER UTIL et NTFM v1.7
* niepceron	27 Jan 1999	: Ajout des ssystemes STRA 1.8 dem/1728
------------------------------------------------------ */

#ifndef MDIF
#define MDIF

/* fichiers inclus */
	/* includes systeme */
	#include <stdio.h>
	/* includes Utilitaires */
	
		/* rtworks */
	#include        <rtworks/common.h>
	#include        <rtworks/ipc.h>
	#include        <rtworks/ipc_proto.h>
	
	/* includes applicatifs Migrazur */
	#include "xzcg.h"
	#include "xzss.h"
	#include "xzsa.h"
	#include "xzdd.h"
	#include "xdc.h"
	#include "xdf.h"
	#include "cgcd_liv.h"

/* definitions de constantes exportees */

#define CDIFC_LIVRAISON		CLIVC_LIVRAISON	
#define CDIFC_DIFFUSION		CLIVC_DIFFUSION	

#define CDIFC_DIFF_SCNT_SOURCE 	XDC_PATHCNFFIC	 "/snt" 
#define CDIFC_DIFF_SCNT_OPERA	XDC_PATHFIC	 "/snt"

#define CDIFC_DIFF_SNAV_SOURCE 	XDC_PATHCNFFIC	 "/nav" 
#define CDIFC_DIFF_SNAV_OPERA	XDC_PATHFIC	 "/nav"

#define CDIFC_DIFF_LPIC_SOURCE	XDC_PATHCNFFIC	 "/pic"
#define CDIFC_DIFF_LPIC_OPERA	XDC_PATHFIC	 "/pic"

#define CDIFC_DIFF_MPMV_SOURCE	XDC_PATHCNFFIC	 "/pmv"
#define CDIFC_DIFF_MPMV_OPERA	XDC_PATHFIC	 "/pmv"

#define CDIFC_DIFF_EQAL_SOURCE	XDC_PATHCNFFIC	 "/eqa"
#define CDIFC_DIFF_EQAL_OPERA	XDC_PATHFIC	 "/eqa"

#define CDIFC_DIFF_SYNP_SOURCE	XDC_PATHCNFFIC	 "/syn"
#define CDIFC_DIFF_SYNP_OPERA	XDC_PATHFIC	 "/syn"

#define CDIFC_DIFF_RSAD_SOURCE	XDC_PATHCNFFIC	 "/bdc"
#define CDIFC_DIFF_RSAD_OPERA	XDC_PATHCNFFIC	 "/bdc"

#define CDIFC_DIFF_FAID_SOURCE	XDC_PATHCNFFIC	 "/aid"
#define CDIFC_DIFF_FAID_OPERA	XDC_PATHFIC	 "/aid"

#define CDIFC_DIFF_FINF_SOURCE  XDC_PATHCNFFIC   "/inf"
#define CDIFC_DIFF_FINF_OPERA   XDC_PATHFIC      "/inf"

#define CDIFC_DIFF_PIGN_SOURCE	XDC_PATHCNFFIC	 "/ign"
#define CDIFC_DIFF_PIGN_OPERA	XDC_PATHFIC	 "/ign"

#define CDIFC_DIFF_DIE_SOURCE	XDC_PATHCNFFIC	 "/die"
#define CDIFC_DIFF_DIE_OPERA	XDC_PATHFIC	 "/die"

#define CDIFC_DIFF_MOD_SOURCE	XDC_PATHCNFFIC	 "/mod"
#define CDIFC_DIFF_MOD_OPERA	XDC_PATHFIC	 "/mod"

#define CDIFC_DIFF_MFBO_SOURCE	XDC_PATHCNFFIC	 "/mbo"
#define CDIFC_DIFF_MFBO_OPERA	XDC_PATHFIC	 "/mbo"

#define CDIFC_DIFF_DEFT_SOURCE	XDC_PATHCNFFIC	 "/dft"
#define CDIFC_DIFF_DEFT_OPERA	XDC_PATHFIC	 "/dft"

#define CDIFC_DIFF_FADM_SOURCE	XDC_PATHCNFFIC	 "/adm"
#define CDIFC_DIFF_FADM_OPERA	XDC_PATHFIC	 "/adm"

#define CDIFC_DIFF_DORA_SOURCE	XDC_PATHCNFFIC	 "/dra"
#define CDIFC_DIFF_DORA_OPERA	XDC_PATHFIC	 "/dra"

#define CDIFC_DIFF_OPER_SOURCE  XDC_PATHCNFFIC   "/opr"
#define CDIFC_DIFF_OPER_OPERA   XDC_PATHFIC      "/opr"

#define CDIFC_DIFF_UTIL_SOURCE  XDC_PATHCNFFIC   "/util"
#define CDIFC_DIFF_UTIL_OPERA   XDC_PATHFIC      "/util"

#define CDIFC_DIFF_NTFM_SOURCE  XDC_PATHCNFFIC   "/ntfm"
#define CDIFC_DIFF_NTFM_OPERA   XDC_PATHFIC      "/ntfm"

#define CDIFC_DIFF_STRA_SOURCE  XDC_PATHCNFFIC   "/stra"
#define CDIFC_DIFF_STRA_OPERA   XDC_PATHFIC      "/stra"

#define CDIFC_DIFF_EXEC_BASEDD_SOURCE 	XDC_PATHCNFEXE 	
#define CDIFC_DIFF_EXEC_BASEDD_OPERA	XDC_PATHEXE

#define CDIFC_DIFF_EXEC_EQUEXT_SOURCE 	XDC_PATHCNFEXE 
#define CDIFC_DIFF_EXEC_EQUEXT_OPERA	XDC_PATHEXE

#define CDIFC_DIFF_EXEC_GECONF_SOURCE 	XDC_PATHCNFEXE 
#define CDIFC_DIFF_EXEC_GECONF_OPERA	XDC_PATHEXE

#define CDIFC_DIFF_EXEC_GESIHM_SOURCE 	XDC_PATHCNFEXE 
#define CDIFC_DIFF_EXEC_GESIHM_OPERA	XDC_PATHEXE
	

#define CDIFC_DIFF_EXEC_MAINTE_SOURCE 	XDC_PATHCNFEXE 
#define CDIFC_DIFF_EXEC_MAINTE_OPERA	XDC_PATHEXE

#define CDIFC_DIFF_EXEC_SAIDEC_SOURCE 	XDC_PATHCNFEXE 
#define CDIFC_DIFF_EXEC_SAIDEC_OPERA	XDC_PATHEXE

#define CDIFC_DIFF_EXEC_SIMTST_SOURCE 	XDC_PATHCNFEXE 
#define CDIFC_DIFF_EXEC_SIMTST_OPERA	XDC_PATHEXE


#define CDIFC_DIFF_EXEC_SUPERV_SOURCE 	XDC_PATHCNFEXE 
#define CDIFC_DIFF_EXEC_SUPERV_OPERA	XDC_PATHEXE

#define CDIFC_DIFF_EXEC_XDMICG_SOURCE 	XDC_PATHCNFEXE 
#define CDIFC_DIFF_EXEC_XDMICG_OPERA	XDC_PATHEXE

#define CDIFC_DEBUT_NOM_SERV		"SD"
#define CDIFC_LONG_DEBUT_NOM_SERV	2


/* definitions de types exportes */

/* definitions de globales exportes */

extern XZIAT_destinataire CGCDV_NOMMACHINE;

/* definition de macro exportees */

/* declarations de donnees globales */

	/* File descripteur du tableau checklist*/
	extern FILE *CGCDG_Fich_CHECKLIST;

	/* Poiteur sur le tableau checklist*/
	extern CLIVT_Elt_Tab_Checklist CLIVG_Tab_Checklist[XZCGC_NB_VERIF];

/* delaration de fonctions externes */

/*
* SERVICE RENDU : 
*	cdif_FctUtil_Gest_ligne_bl permet de traiter 
*  une ligne du Bon de livraison conformement a la fonctionalite
*  de diffusion des fichiers par tcgcd.x (page35 du DCG-002/0.1).
*/

extern int cdif_FctUtil_Gest_ligne_bl(XZCGT_ComposantBL ,int * );

/*
* SERVICE RENDU : 
*	cdif_cb : Callback de la tache TCGCD effectuant la diffusion.
* SEQUENCE D'APPEL :
*/

extern void cdif_cb(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG );

/*
*	cdif_Diff_Taches permet de transferer les taches d un sous systeme
* 
*/
extern int cdif_Diff_Taches	(	XZSCT_NomTache,
        				int ,
        				void **);

#endif
