/*E*/
/* Fichier : $Id: xzao16.h,v 1.4 2011/01/14 16:11:17 gesconf Exp $        Release : $Revision: 1.4 $        Date : $Date: 2011/01/14 16:11:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzao16.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	05 Sep 1994	: Creation
* JPL		14/01/11 : Migration architecture HP ia64 (DEM 971) : condition d'inclusion <> autres fichiers 1.4
------------------------------------------------------ */

#ifndef xzao16_
#define xzao16_

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */
#define XZAOC_XZAO16_RPC_NAME "XZAO;16"
#define XZAOC_XZAO16_NB_PARM_RETURN 7
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

extern tg_Connexion  *XZAGV_Connexion;
extern tg_Context    * XZAGV_Context;

/* delaration de fonctions externes */
extern int XZAO16_Aire_Amont(XDY_Autoroute,
				XDY_PR,
				XDY_Sens,
				XDY_Mot,
				XDY_Octet *,
				XDY_Nom	,
				XDY_Abrev,
				XDY_Autoroute 	*,
				XDY_PR 		*,
				XDY_Sens 	*,
				XDY_Entier 	*);
#endif
