/*E*/
/*Fichier :  $Id: xzac.h,v 1.3 2010/11/30 10:21:10 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2010/11/30 10:21:10 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER XZAC.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Nov 1994	: Creation
* JBL		Nov 2007	: DEM 662
* JPL		30/11/10	: Migration architecture HP ia64 (DEM 961) : fichiers en-tete systeme inclus  1.3
------------------------------------------------------ */

#ifndef XZAC
#define XZAC

/* fichiers inclus */


#include <fcntl.h>

#if defined (__hp9000s300)
#include <tcio.h>
#endif

#include "xdy.h"
#include "asql_com.h"
#include "xzat.h"
#include "xzac22.h"
#include "xzac23.h"
#include "xzac25.h"
#include "xzac26.h"

/* definitions de constantes exportees */


/* definitions de types exportes */


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;


#endif
