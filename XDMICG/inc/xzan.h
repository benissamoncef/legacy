/*E*/
/*  Fichier : 	$Id: xzan.h,v 1.4 2010/11/30 10:21:15 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 2010/11/30 10:21:15 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* volcic	08 Dec 1994	: Mise en conformite (v1.2)
* volcic	11 Jan 1995	: Mise en conformite (v1.3)
* JPL		30/11/10	: Migration architecture HP ia64 (DEM 961) : fichiers en-tete systeme inclus  1.4
------------------------------------------------------ */

#ifndef XZAN
#define XZAN

/* fichiers inclus */


#include <fcntl.h>

#if defined (__hp9000s300)
#include <tcio.h>
#endif

#include "xdy.h"
#include "asql_com.h"
#include "xzat.h"
#include "xzao.h"
#include "xzan01.h"
#include "xzan02.h"
#include "xzan36.h"

/* definitions de constantes exportees */

#define XZANC_CAL_NON_VIDE	3

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;


#endif
