/*E Fichier : $Id: xzao500.h,v 1.4 2011/01/14 16:11:07 gesconf Exp $     Release : $Revision: 1.4 $       Date : $Date: 2011/01/14 16:11:07 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzao500.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMg	27/04/04	creation	1.1
* PCL	21/09/04	rajout parametres	1.2
* PCL	19/11/04	rajout parametre num, cle 1.3
* JPL	14/01/11 : Migration architecture HP ia64 (DEM 971) : condition d'inclusion <> autres fichiers 1.4
------------------------------------------------------ */

#ifndef xzao500_
#define xzao500_

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAO500_RPC_NAME "XZAO500"
#define XZAOC_XZAO500_NB_PARM_RETURN 6

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

extern tg_Connexion  *XZAGV_Connexion;
extern tg_Context    * XZAGV_Context;

/* delaration de fonctions externes */
extern int XZAO500_PAL_amont(XDY_Autoroute,
				XDY_PR,
				XDY_Sens,
				XDY_Mot,
				XDY_PR,
				XDY_District,
				char *,
				XDY_FonctionInt,
				XDY_Horodate,
				XDY_Entier      *,
				XDY_PR,
				XDY_Booleen,
				XDY_Entier,
				XDY_Octet);
#endif
