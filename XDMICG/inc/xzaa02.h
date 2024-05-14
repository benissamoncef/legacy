/*E*/
/*Fichier : $Id: xzaa02.h,v 1.1 2012/07/04 10:58:14 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:58:14 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAA * FICHIER xzaa02.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  ABE	04/04/22	: Creation SAE-DEM367
------------------------------------------------------ */

#ifndef xzaa02
#define xzaa02

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
#include "xzaa.h"
#include "xzag.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAA02C_RPC_NAME "XZAA;02"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int xzaa02_Archive_Fiche_Alerte (XDY_Entier va_NumAlerte_in,
                                XDY_Octet va_Qualification_in,
                                XDY_Entier va_NumEvt_in,
                                XDY_Octet va_CleEvt_in,
                                XDY_NomSite va_NomSiteLocal_in);

#endif
