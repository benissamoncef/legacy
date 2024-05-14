/*E Fichier : $Id: xzah01.h,v 1.2 1996/09/06 11:22:51 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/09/06 11:22:51 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAH * FICHIER XZAH01.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	24 Oct 1994	: Creation
* niepceron	27 Aou 1996	: Ajout du site en arg  v1.2
------------------------------------------------------ */

#ifndef XZAH01
#define XZAH01

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

#define XZAH01C_XZAH01_RPC_NAME "XZAH;01"
#define XZAH01C_XZAH01_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int XZAH01_Envoyer_Bouchon_Chaussee_Portion(XDY_Booleen,XDY_Nom);
#endif
