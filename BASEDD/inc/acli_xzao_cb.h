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
* ABE	23/09/2020 : Creation DEM-SAE152 V1.1		
------------------------------------------------------ */

#ifndef ACLI_XZAO
#define ACLI_XZAO

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
	#include	"xzao960.h"
/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void acli_xzao960_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);

#endif
