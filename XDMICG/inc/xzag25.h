/*E Fichier : $Id: xzag25.h,v 1.3 1997/05/15 09:58:31 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1997/05/15 09:58:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAG25 * FICHIER XZAG25.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Torregrossa	10 Aou 1995	: Creation
* Niepceron	09 Oct 1996	: Ajout de XZAG52 (dem/1227) v1.2
* Niepceron     27 Fev 1997	: Ajout de XZAG55 (dem/1396) v1.3
------------------------------------------------------ */

#ifndef XZAG25
#define XZAG25

/* fichiers inclus */
	/* Include systeme */
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <memory.h>
	
	/* Include Sybase */
	#include <ctpublic.h>
	#include <cstypes.h>

	/* Include applicatif Migrazur */
	#include "xdy.h"
	#include "xdc.h"
	#include "xzag.h"
	#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAG11C_RPC_NAME "XZAG;11"
#define XZAG25C_RPC_NAME "XZAG;25"
#define XZAG26C_RPC_NAME "XZAG;26"
#define XZAG27C_RPC_NAME "XZAG;27"
#define XZAG28C_RPC_NAME "XZAG;28"
#define XZAG29C_RPC_NAME "XZAG;29"
#define XZAG30C_RPC_NAME "XZAG;30"
#define XZAG31C_RPC_NAME "XZAG;31"
#define XZAG52C_RPC_NAME "XZAG;52"
#define XZAG55C_RPC_NAME "XZAG;55"

#define XZAG11C_NB_PARM_RETURN 1
#define XZAG25C_NB_PARM_RETURN 0
#define XZAG26C_NB_PARM_RETURN 0
#define XZAG27C_NB_PARM_RETURN 0
#define XZAG28C_NB_PARM_RETURN 0
#define XZAG29C_NB_PARM_RETURN 0
#define XZAG30C_NB_PARM_RETURN 0
#define XZAG31C_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int XZAG25_Purge_Evts_Base_Historique (XDY_Horodate, XDY_Booleen *);
extern int XZAG_Purge_MorceauHistorise (CS_CHAR *, XDY_Octet, char *, CS_DATETIME *);
extern int XZAG_Purge_Base_Historique (XDY_Horodate);
#endif
