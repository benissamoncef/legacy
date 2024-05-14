/*E*/
/*Fichier : $Id: xzae159.h,v 1.1 2012/07/04 10:58:14 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:58:14 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzae159.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  ABE	20/01/22	: Creation SAE-DEM367
------------------------------------------------------ */

#ifndef xzae159
#define xzae159

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

#define XZAE159C_RPC_NAME "XZAE159"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int xzae159_Rechercher_Veh_dans_rayon (XDY_NomAuto   va_autoroute_in ,
                                        XDY_PR		    va_pr_in,
				                        XDY_Sens		va_sens_in,
                                        XDY_Entier      va_rayon_in,
                                        XDY_FonctionInt pa_FonctionUtilisateur_in,
						                XDY_Entier 	    *va_Resultat_out);

#endif
