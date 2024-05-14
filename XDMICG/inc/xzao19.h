/*E Fichier : $Id: xzao19.h,v 1.8 2012/07/30 13:45:13 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2012/07/30 13:45:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzao19.h
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
* C.T.          13 Jan 1995     : Ajout de la date en entree (V1.2)
* JMG		14 nov 1997	: ajout type de nav en sortie 1.3
* JPL		14/01/11 : Migration architecture HP ia64 (DEM 971) : condition d'inclusion <> autres fichiers 1.7
* PNI           30/07/2012      : XDY_Octet *pa_TypeNav_Out 1.8 DEM1048
------------------------------------------------------ */

#ifndef xzao19_
#define xzao19_

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAO19_RPC_NAME "XZAO;19"
#define XZAOC_XZAO19_NB_PARM_RETURN 8

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

extern tg_Connexion  *XZAGV_Connexion;
extern tg_Context    * XZAGV_Context;

/* delaration de fonctions externes */
extern int XZAO19_NAV_amont(XDY_Autoroute,
				XDY_PR,
				XDY_Sens,
				XDY_Mot,
				
				XDY_Mot *,
				XDY_District *,
				XDY_Autoroute *,
				XDY_PR *,
				XDY_Sens *,
				XDY_Entier *,
				XDY_Entier *,
				XDY_Octet *,
				XDY_Horodate);
#endif
