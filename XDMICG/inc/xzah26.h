/*E Fichier : $Id: xzah26.h,v 1.1 1995/01/11 17:04:11 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/11 17:04:11 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAH * FICHIER xzah26.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Ce module appartient a TACLI. Il permet de demander l'init voies RADT et DAI
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	11 Jan 1995     : creation
------------------------------------------------------ */

#ifndef XZAH26
#define XZAH26

/* fichiers inclus */
	/* include system */
	#include        <stdio.h>
	#include        <string.h>
	#include        <ctype.h>

	/* Include service migrazur */
	 #include "xdc.h"
	 #include "xdy.h"
	 #include "xdm.h"
	 
	 #include "xzst.h"
	 #include "xzsc.h"
	 
	 
/* definitions de constantes exportees */

/* definitions de types exportes */


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int XZAH26_InitRadtDai ( XDY_NomMachine ); 
#endif

