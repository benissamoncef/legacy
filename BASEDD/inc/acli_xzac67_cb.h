/*E Fichier : $Id: acli_xzac67_cb.h,v 1.1 2004/11/16 14:08:56 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2004/11/16 14:08:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE ACLI_XZAC67 * FICHIER ACLI_XZAC67.h
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

#ifndef ACLI_XZAC67
#define ACLI_XZAC67

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
	#include	"xzac67.h"
/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void acli_xzac67_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 

#endif
