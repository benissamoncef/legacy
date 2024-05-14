/*E*/
/* Fichier : @(#)xzap03.h	$Id: xzap03.h,v 1.4 1994/12/05 18:21:23 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1994/12/05 18:21:23 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzap03.h
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
------------------------------------------------------ */

#ifndef xzap03
#define xzap03

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP03_RPC_NAME "XZAP;03"

#define XZAOC_XZAP03_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP03_Ajout_Prop_NAV  (	XDY_Mot,
					XDY_Mot,
					XDY_District,
					XDY_Commentaire,
					XDY_Octet,
					XDY_Eqt,
					XDY_Commentaire,
					XDY_Nom,
					XDY_Scenario,
					XDY_Booleen ) ;



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
