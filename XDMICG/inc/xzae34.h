/*E*/
/*Fichier : $Id: xzae34.h,v 1.1 2012/07/04 10:58:14 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:58:14 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzae34.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  ABE	01/01/22	: Creation SAE-DEM367
------------------------------------------------------ */

#ifndef xzae34
#define xzae34

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
#include "xzae.h"
#include "xzag.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAE34C_RPC_NAME "XZAE34"
#define XZAE34C_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int xzae34_Lire_Numero_Autoroute (	XDY_NomAuto	va_Nom_Autoroute_in,
                                            XDY_Autoroute		*va_Num_Autoroute_out);

#endif
