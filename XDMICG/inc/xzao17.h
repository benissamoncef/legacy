/*E Fichier : $Id: xzao17.h,v 1.5 2012/07/26 11:41:47 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2012/07/26 11:41:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzao17.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	06 Sep 1994	: Creation
* JPL		14/01/11 : Migration architecture HP ia64 (DEM 971) : condition d'inclusion <> autres fichiers 1.4
* JPL		25/06/12 : Procedure degroupee; ajout parametres nom sortie usager et indic. bifurcation (DEM 1033)  1.5
------------------------------------------------------ */

#ifndef xzao17_
#define xzao17_

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAO17_RPC_NAME "XZAO17"
#define XZAOC_XZAO17_NB_PARM_RETURN 9

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

extern tg_Connexion  *XZAGV_Connexion;
extern tg_Context    * XZAGV_Context;

/* delaration de fonctions externes */
extern int XZAO17_Sortie_Amont(XDY_Autoroute,
				XDY_PR ,
				XDY_Sens,
				XDY_Mot,

				XDY_Octet 	*,
				XDY_Nom	,
				XDY_Abrev,
				XDY_Sortie,
				XDY_Octet 	*,
				XDY_Autoroute 	*,
				XDY_PR 		*,
				XDY_Sens 	*,
				XDY_Entier 	*);
#endif
