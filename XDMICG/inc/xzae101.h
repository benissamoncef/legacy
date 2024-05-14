/*E Fichier : $Id: xzae101.h,v 1.3 1996/09/09 09:14:17 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1996/09/09 09:14:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE101 * FICHIER XZAE101.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	26 Aou 1996	: Creation
* Vathelot	04 sep 1996	: codage (v1.3)
------------------------------------------------------ */

#ifndef XZAE101
#define XZAE101

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
	#include "xzis.h"
	#include "xzag.h"
	#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAE101C_XZAE101_RPC_NAME		"XZAE;101"
#define XZAE101C_XZAE101_NB_PARAM_RETURN	0


/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* declaration de fonctions externes */

int XZAE101_Liste_FMC_Locales( XDY_Nom va_NomSite, XDY_FonctionInt va_FonctionUtilisateur);


#endif
