/*E Fichier : $Id: xzas12.h,v 1.2 1995/01/27 20:54:48 torregrossa Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/27 20:54:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS12 * FICHIER XZAS12.h
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

#ifndef XZAS12
#define XZAS12

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

#define XZAS12C_RPC_NAME "XZAS12"
#define XZAS12C_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*  XZAS12_Ajout_Niveau_Service : met en base les niveaux de service des autoroutes.
*  Appelle XZAS12SP
*/
extern int XZAS12_Ajout_Niveau_Service (XDY_Mot,
					XDY_Sens,
					XDY_Horodate,
					XDY_Octet);
#endif
