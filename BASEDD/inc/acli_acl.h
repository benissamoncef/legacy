/*E Fichier : $Id: acli_acl.h,v 1.4 2020/06/09 14:20:31 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2020/06/09 14:20:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE ACL * FICHIER ACL.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	24 Oct 1994	: Creation
* JMG	18/03/11 : ajout ACLIC_AACL11_RPC_NAME 1.3
* LCL	16/09/19 : ajout ACLIC_AACL31_RPC_NAME COCKPIT LOT27 DEM1351	1.4	
------------------------------------------------------ */

#ifndef ACL
#define ACL

/* fichiers inclus */
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
/* definitions de constantes exportees */

#define ACLIC_AACL01_RPC_NAME "XZAA;01"

#define ACLIC_AACL02_RPC_NAME "XZAD01"

#define ACLIC_AACL11_RPC_NAME "XZAA11"

#define ACLIC_AACL31_RPC_NAME "XZAE199"



/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void aacl01 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void aacl02      (T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void aacl01_localisee    (T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void aacl01_cockpit    (T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);

#endif
