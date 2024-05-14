/*E Fichier : $Id: xzas06.h,v 1.5 1998/09/22 13:14:31 gaborit Exp $      Release : $Revision: 1.5 $        Date : $Date: 1998/09/22 13:14:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS06 * FICHIER XZAS06.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Appartient a TACLI : permet de declencher l animation des
* courbes des synoptiques.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	09 Nov 1994	: Creation
* niepceron	29 Aou 1996	: Ajout de XZAS06_Fonction_De_T_CI v1.2
* niepceron	02 Jul 1998	: Ajout de xzsm.h (dem/1699) v1.3
------------------------------------------------------ */

#ifndef XZAS06
#define XZAS06

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
extern int XZAS06_Fonction_De_T (	XDY_Datagroup,
					XDY_Eqt	,
					XDY_Sens,
					XDY_Voie,
					XDY_Horodate,
					XDY_Octet); 


extern int XZAS06_Fonction_De_T_CI (    XDY_Datagroup,
					XDY_Eqt ,
					XDY_Sens,
					XDY_Voie,
					XDY_Horodate,
				       	XDY_Octet);

#endif
