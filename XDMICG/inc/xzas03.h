/*E Fichier : $Id: xzas03.h,v 1.2 1998/09/22 13:14:32 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1998/09/22 13:14:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS03 * FICHIER XZAS03.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Appartient a TACLI : permet de declencher l animation des
* courbes des synoptiques.
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	14/01/95	: Creation
* niepceron	02 Jul 1998	: Ajout de xzsm.h (dem/1699) v1.2
------------------------------------------------------ */

#ifndef XZAS03
#define XZAS03

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
	 #include "xzsm.h"
	 
	 
/* definitions de constantes exportees */

/* definitions de types exportes */


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int XZAS03_Bouchon (    XDY_Datagroup   va_datagroup_specif,
                                XDY_Autoroute   va_Autoroute,
                                XDY_PR          va_PR_Debut,
                                XDY_PR          va_PR_Fin,
                                XDY_Horodate    va_HorodateDebut ); 
#endif
