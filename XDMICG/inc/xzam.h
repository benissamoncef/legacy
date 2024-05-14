/*E Fichier : $Id: xzam.h,v 1.5 2010/12/07 12:02:47 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/12/07 12:02:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE AMCL * FICHIER AMCL.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	24 Oct 1994	: Creation
* gaborit	02 Oct 1995	: Ajout d'un argument à xzam01(1.2)
* guilhou       1 oct 2007    : ajout site de gestion 1.3
* JMG	12/12/10	: portage linux 1.4
------------------------------------------------------ */

#ifndef AMCL
#define AMCL

/* fichiers inclus */
	/* include system */
	#include        <stdio.h>
	#include        <string.h>
	#include        <ctype.h>

	/* include system necessaire pour Open Server et Open Client. */
#ifdef _HPUX_SOURCE
	#include        <ospublic.h>
	#include        <oserror.h>
#endif

	/* Include service migrazur */
	 #include "xdy.h"
	 #include "xdm.h"
	 #include "xzst.h"
	 #include "xzsc.h"
/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int XZAM01_Ajout_Alarme (XDY_Horodate ,
				XDY_TypeEqt,
				XDY_Eqt	,
				XDY_TypeAlarme,
				XDY_Booleen,
				XDY_NomMachine,
				XDY_District) ;

#endif
