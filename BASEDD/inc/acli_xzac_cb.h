/*E*/
/*Fichier :  @(#)acli_xzac_cb.h	1.2      Release : 1.2        Date : 02/13/08
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE ACLI_XZAC * FICHIER ACLI_XZAC.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
* JBL		Nov 2007	:	DEM 663
* JMG		11/11/1é	: xzac802 et 303
------------------------------------------------------ */

#ifndef ACLI_XZAC
#define ACLI_XZAC

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
	#include        "xzst.h"

	#include	"asql_com.h"
	#include	"acli_dir.h"
	#include	"xzac92.h"
	#include	"xzac98.h"
	#include	"xzac01.h"
	#include	"xzac802.h"
	#include	"xzac61.h"
	#include	"xzac82.h"
	#include	"xzac84.h"
	#include	"xzac93.h"
	#include	"xzac99.h"
	#include	"xzac06.h"
	#include	"xzac63.h"
	#include	"xzac100.h"
	#include	"xzac0101.h"
	#include	"xzac102.h"
	#include	"xzac0103.h"
	#include	"xzac104.h"
/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void acli_xzac92_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
extern void acli_xzac98_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
extern void acli_xzac01_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
extern void acli_xzac61_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
extern void acli_xzac82_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
extern void acli_xzac84_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
extern void acli_xzac93_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
extern void acli_xzac99_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
extern void acli_xzac06_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
extern void acli_xzac63_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
extern void acli_xzac100_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
extern void acli_xzac101_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
extern void acli_xzac102_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
extern void acli_xzac103_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
extern void acli_xzac104_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
extern void acli_xzac802_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void acli_xzac303_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);

#endif
