/*E Fichier : @(#)acli_tcl.h	1.7      Release : 1.7        Date : 02/13/08
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE TCL * FICHIER TCL.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	05 Oct 1994	: Creation
* Torregrossa	29 Jan 1998	: Ajout ACLIC_ATCL28_RPC_NAME 1.4-1.5
* JMG		11/10/05	: ajout PMVA BAF BAD 1.6
* AAZ       27/08/2007  : ajout SIG, ECL, VEN, ENR et INC
* VR        15/12/2011  : ajout PRV (DEM/1016)
* JMG		26/04/16 : ajout SAGA 1.9 DEM1191
* JMG 		04/03/18 : ajout controleurs feux CTRL_FEUX 1.10 DEM1284
* ABE	28/01/21 : ajout ACLIC_ATCL70_RPC_NAME 1.11 DEM-SAE155
* ABE		07/04/21 : ajout atcl80 pour PAU SONO DEM-SAE244 1.12
------------------------------------------------------ */

#ifndef TCL
#define TCL

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
	#include        "xzis.h"
	#include        "xzec.h"
	#include	"asql_com.h"
	
/* definitions de constantes exportees */

#define ACLIC_ATCL22_RPC_NAME "XZAT;22"
#define ACLIC_ATCL23_RPC_NAME "XZAT;23"
#define ACLIC_ATCL24_RPC_NAME "XZAT;24"
#define ACLIC_ATCL25_RPC_NAME "XZAT;25"
#define ACLIC_ATCL26_RPC_NAME "XZAT;26"
#define ACLIC_ATCL27_RPC_NAME "XZAT;26"
#define ACLIC_ATCL28_RPC_NAME "XZAT;27"

#define ACLIC_ATCL250_RPC_NAME	"XZAT250"
#define ACLIC_ATCL251_RPC_NAME	"XZAT251"
#define ACLIC_ATCL30_RPC_NAME	"XZAT30"
#define ACLIC_ATCL31_RPC_NAME	"XZAT31"
#define ACLIC_ATCL51_RPC_NAME	"XZAT520"

/* AAZ */
#define ACLIC_ATCL40_RPC_NAME "XZAT40"
#define ACLIC_ATCL41_RPC_NAME "XZAT41"
#define ACLIC_ATCL42_RPC_NAME "XZAT42"
#define ACLIC_ATCL43_RPC_NAME "XZAT43"
#define ACLIC_ATCL44_RPC_NAME "XZAT44"
#define ACLIC_ATCL45_RPC_NAME "XZAT45"
#define ACLIC_ATCL60_RPC_NAME "XZAT530"
#define ACLIC_ATCL70_RPC_NAME "XZAT540"
#define ACLIC_ATCL80_RPC_NAME "XZAT550"

/* AAZ */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void atcl22 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl23 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl24 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl25 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl26 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl27 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl28 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl250 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl251 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl30 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl31 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl51 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
/* AAZ */
extern void atcl40 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl41 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl42 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl43 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl44 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl45 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl60 	(T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
extern void atcl70 	(T_IPC_CONN va_Cnx,T_IPC_CONN_PROCESS_CB_DATA pa_Data,T_CB_ARG pa_ARG);
extern void atcl80 	(T_IPC_CONN va_Cnx,T_IPC_CONN_PROCESS_CB_DATA pa_Data,T_CB_ARG pa_ARG);
/* AAZ */
#endif
