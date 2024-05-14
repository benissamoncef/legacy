/*E Fichier : $Id: xzah07.h,v 1.3 1996/09/06 11:23:03 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1996/09/06 11:23:03 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAH * FICHIER XZAH.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	24 Oct 1994	: Creation
* torregrossa	30 mar 1995	: supression du fichier
*                                 xzah_utils.h en include (V 1.2)
* niepceron	27 Aou 1996	: Ajout du site en arg  v1.3
------------------------------------------------------ */

#ifndef XZAH07
#define XZAH07

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
	#include "xzis.h"
	#include "xzahc.h"

	
/* definitions de constantes exportees */

#define XZAH07C_XZAH07_RPC_NAME "XZAH;07"
#define XZAH07C_XZAH07_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int XZAH07_Envoyer_Objet_Mobile_Portion(	XDY_Booleen,XDY_Nom);
#endif
