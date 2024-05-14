/*E*/
/*  Fichier : @(#)xzap08.h	$Id: xzap08.h,v 1.3 2014/11/14 08:31:56 devgfi Exp $      $Revision: 1.3 $        $Date: 2014/11/14 08:31:56 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzap08.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	31 Aug 1994	: Creation
* volcic	10 Oct 1994	: Modification (ajout include xdc.h, xdy.h) (v 1.2)
* JPL		09/10/2014	: Degroupee; ajout du sens (DEM 1100)  1.3
* JPL		02/07/2021	: Extension du parametre "Remarque" (SAE-287)  1.4
------------------------------------------------------ */

#ifndef xzap08
#define xzap08

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP08_RPC_NAME "XZAP08"

#define XZAOC_XZAP08_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP08_Ajout_Prop_Tel  (	XDY_Mot,
					XDY_Mot,
					XDY_District,
					XDY_Commentaire,
					XDY_Commentaire,
					XDY_Nom,
					XDY_PR,
					XDY_Autoroute,
					XDY_Sens,
					XDY_Octet ) ;



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
