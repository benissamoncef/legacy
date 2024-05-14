/*E Fichier : $Id: xzah02.h,v 1.1 2005/04/01 14:33:45 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2005/04/01 14:33:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAH02 * FICHIER XZAH02.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*
------------------------------------------------------
* HISTORIQUE :
*
* CHI   17/03/05        : Creation
*
*
------------------------------------------------------ */

#ifndef XZAH02
#define XZAH02

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
	#include "xzis.h"
	#include "xzahc.h"

/* definitions de constantes exportees */
#define XZAH02C_RPC_NAME "XZAH02"
#define XZAH02C_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int XZAH02_Envoyer_Bouchon_Chaussee_Segment(XDY_Booleen,XDY_Nom);
#endif
