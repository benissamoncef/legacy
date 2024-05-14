/*E Fichier : @(#)acli_xzac03_cb.h	1.1      Release : 1.1        Date : 11/16/04
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE ACLI_XZAC03 * FICHIER ACLI_XZAC03.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
------------------------------------------------------ */

#ifndef ACLI_XZAC03
#define ACLI_XZAC03

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
	#include	"xzac03.h"
/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void acli_xzac03_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 

#endif
