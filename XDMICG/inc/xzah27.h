/*E Fichier : $Id: xzah27.h,v 1.1 1995/01/17 16:04:15 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/17 16:04:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAH * FICHIER XZAH27.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	16 Jan 1995	: creation
------------------------------------------------------ */

#ifndef XZAH27
#define XZAH27

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
	#include "xzst.h"
	#include "xzsc.h"
	#include "xzag.h"
	#include "asql_com.h"
	#include "xzes.h"
	
/* definitions de constantes exportees */

#define XZAH27C_XZAH27_RPC_NAME "XZAH;27"
#define XZAH27C_XZAH27_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int XZAH27_Satir();
#endif
