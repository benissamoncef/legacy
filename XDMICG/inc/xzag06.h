/*E Fichier : $Id: xzag06.h,v 1.1 1994/11/15 11:56:37 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/15 11:56:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAG06 * FICHIER XZAG06.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Appartient a XZAG 
* Permet d effectuer la purge de la base historique des donnees
* ayant plus de 5 ans.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	07 Nov 1994	: Creation
------------------------------------------------------ */

#ifndef XZAG06
#define XZAG06

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
#define XZAG06C_RPC_NAME "XZAG;06"

#define XZAG06C_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int XZAG06_Purge_Base_Historique (XDY_Horodate	);

#endif
