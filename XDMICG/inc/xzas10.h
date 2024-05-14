/*E Fichier : $Id: xzas10.h,v 1.2 1995/01/27 20:54:42 torregrossa Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/27 20:54:42 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS10 * FICHIER XZAS10.h
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

#ifndef XZAS10
#define XZAS10

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
#define XZAS10C_RPC_NAME "XZAS10"

#define XZAS10C_NB_PARM_RETURN 0
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/* SERVICE RENDU : 
*  XZAS10_Ajout_Mesure_6mn : met en base les donnees de comptages.
*  Appelle XZAS10SP
*/
extern int XZAS10_Ajout_Mesure_6mn (	XDY_Mot ,
					XDY_Sens ,
					XDY_Octet ,
					XDY_Horodate,
					XDY_Mot ,
					XDY_Booleen,
					XDY_Octet,
					XDY_Booleen,
					XDY_Octet,
					XDY_Booleen,
					XDY_Octet,
					XDY_Booleen );
#endif
