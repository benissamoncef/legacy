/*E Fichier : $Id: acli_hcl.h,v 1.6 2012/05/29 10:20:16 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2012/05/29 10:20:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE AHCL * FICHIER AHCL.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	31 Oct 1994	: Creation
* gaborit	11 jan 1995	: ajout ahcl21 et ahcl26
* gaborit	16 jan 1995	: ajout ahcl27
* gaborit	16 jan 1995	: ajout ahcl27
* gaborit       13 sep 1995	: modif cle et delais timer (1.4)
* guilhou	21/02/2012	: ajout ahcl980 dem/1015
----------------------------------------------------- */

#ifndef AHCL
#define AHCL

/* fichiers inclus */
	/* includes systeme */
	
	#include        <stdio.h>

	/* includes Utilitaires */
	
		/* rtworks */
	#include        <rtworks/common.h>
	#include        <rtworks/ipc.h>
	#include        <rtworks/ipc_proto.h>
	
		/* sybase*/
	
	#include 	"cstypes.h"
	#include	"ctpublic.h"
	
	/* includes applicatifs Migrazur */
	
	#include        "xdc.h"
	#include        "xdy.h"
	#include        "xzag.h"
	#include        "xzsc.h"
	#include        "xzsm.h"
	#include	"asql_com.h"
	#include	"xzah10.h"
	#include	"xzah21.h"
	#include	"xzah27.h"
	#include	"acli_dir.h"
	
/* definitions de constantes exportees */
#define AHCLC_TIMERCLE_AHCL21   "Cle non cycli. Radt Dai"
#define AHCLC_TIMERDELAI_AHCL21 30.0

#define AHCLC_TIMERCLE_AHCL27   "Cle pas cycl. Satir"
#define AHCLC_TIMERDELAI_AHCL27 15.0

#define ACLIC_XZAE141_RPC_NAME	"XZAE141"
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void ahcl11 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);
extern void ahcl21 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);
extern void ahcl26 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);
extern void ahcl27 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);
extern void ahcl99 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);
extern void ahcl980 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);

#endif
