/*E Fichier : $Id: xzaa.h,v 1.6 2011/04/04 15:31:19 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2011/04/04 15:31:19 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAA * FICHIER XZAA.h
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
* gaborit	26 jan 1995	: ajout xzaa10_Equation (1.2)
* gaborit	27 jan 1995	: ajout horodate a xzaa10_Equation (1.3)
* JMG   31/10/07        : ajout site de gestion  1.4
* JMG	12/12/10	: portage linux 1.5
* JMG	18/03/11	: ajout xzaa01_Creer_Alerte_Localisee 1.6
------------------------------------------------------ */

#ifndef XZAA
#define XZAA

/* fichiers inclus */
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
extern int xzaa01_Creer_Alerte (XDY_Horodate,
                                XDY_TypeEqt,
                                XDY_Eqt,
                                XDY_TypeAlerte ,
                                XDY_TexteAlerte,
				XDY_District);

extern int xzaa10_Equation (	XDY_Octet,
				XDY_Entier,
				XDY_Entier,
				XDY_Entier,
				XDY_Entier,
				XDY_Entier,
				XDY_Entier,
				XDY_Entier,
				XDY_Entier,
				XDY_Entier,
				XDY_Entier,
				float,
				float,
				XDY_Horodate,
				XDY_Eqt) ;

extern int xzaa01_Creer_Alerte_Localisee (XDY_Horodate,
                                XDY_TypeEqt,
                                XDY_Eqt,
                                XDY_TypeAlerte ,
                                XDY_TexteAlerte,
                                XDY_District,
                                char *,
                                XDY_PR,
                                int);

#endif
