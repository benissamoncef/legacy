/*E Fichier : $Id: xzag05.h,v 1.1 1994/11/15 11:56:25 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/15 11:56:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAG05 * FICHIER XZAG05.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	07 Nov 1994	: Creation
------------------------------------------------------ */

#ifndef XZAG05
#define XZAG05

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

#define XZAG05C_RPC_NAME "XZAG;05"

#define XZAG05C_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int XZAG05_Purge_Base_Exploitation (XDY_Horodate	);
#endif
