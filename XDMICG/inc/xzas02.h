/*E Fichier : $Id: xzas02.h,v 1.2 1996/10/04 10:17:34 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/10/04 10:17:34 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS02 * FICHIER XZAS02.h
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
* niepceron	23/09/96	: Ajout de la plage (ana3) v1.2
------------------------------------------------------ */

#ifndef XZAS02
#define XZAS02

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
extern int XZAS02_Inrets (    XDY_Datagroup   va_datagroup_specif,
                                XDY_Autoroute   va_Autoroute,
                                XDY_PR          va_PR_Debut,
                                XDY_PR          va_PR_Fin,
                                XDY_Horodate    va_HorodateDebut,
				XDY_Octet	va_Plage); 
#endif
