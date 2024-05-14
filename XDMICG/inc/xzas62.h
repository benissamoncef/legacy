/*E Fichier : $Id: xzas62.h,v 1.1 1999/02/26 14:40:17 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 14:40:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS62 * FICHIER XZAS62.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* niepceron	06 Jan 1999	: Creation
------------------------------------------------------ */

#ifndef XZAS62
#define XZAS62

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
#define XZAS62C_RPC_NAME "XZAS62"

#define XZAS62C_NB_PARM_RETURN 0
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/* SERVICE RENDU : 
*  XZAS62_Ajout_Mesure_DATEX : met en base les donnees de comptages.
*  Appelle XZAS62SP
*/
extern int XZAS62_Ajout_Mesure_DATEX (	XDY_Nom ,
					XDY_Sens ,
					XDY_Octet ,
					XDY_Horodate,
					XDY_Mot ,
					XDY_Booleen,
					XDY_Octet,
					XDY_Booleen,
					XDY_Octet,
					XDY_Booleen );
#endif
