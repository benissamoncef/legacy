/*E Fichier : $Id: acli_mcl.h,v 1.1 1994/11/03 09:52:48 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 09:52:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE MCL * FICHIER MCL.h
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
------------------------------------------------------ */

#ifndef MCL
#define MCL

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

/* definitions de constantes exportees */

#define ACLIC_AMCL01_RPC_NAME "XZAM;01"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void amcl01 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA ,T_CB_ARG);
#endif
