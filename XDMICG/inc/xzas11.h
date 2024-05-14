/*E Fichier : $Id: xzas11.h,v 1.2 1995/01/27 20:54:45 torregrossa Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/27 20:54:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS11 * FICHIER XZAS11.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	08 Nov 1994	: Creation
* C.T.		27 Jan 1995	: Modif nom proc appelee V 1.2
------------------------------------------------------ */

#ifndef XZAS11
#define XZAS11

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
	
/* definitions de constantes exportees */
#define XZAS11C_RPC_NAME "XZAS11"

#define XZAS11C_NB_PARM_RETURN 0
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*  XZAS11_Ajout_Mesure_Poids : met en base les donnees de comptages poids.
*  Appelle XZAS11SP
*/
extern int XZAS11_Ajout_Mesure_Poids (	XDY_Mot	,
					XDY_Sens,
					XDY_Horodate,
					XDY_Entier,
					XDY_Entier);
#endif
