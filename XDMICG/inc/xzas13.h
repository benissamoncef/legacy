/*E Fichier : $Id: xzas13.h,v 1.1 1997/05/15 09:59:08 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1997/05/15 09:59:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS13 * FICHIER XZAS13.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Appartient a TACLI : permet de declencher l animation des
* courbes des synoptiques.
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	03 Fev 1997	: Creation
------------------------------------------------------ */

#ifndef XZAS13
#define XZAS13

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
extern int XZAS13_Init_TDP (	XDY_Datagroup,
					XDY_Eqt	,
					XDY_Horodate); 

extern int XZAS13_Init_TDP_CI (	XDY_Datagroup,
					XDY_Eqt	,
					XDY_Horodate); 



#endif
