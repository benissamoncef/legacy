/*E*/
/*  Fichier : @(#)xzap01.h	$Id: xzap01.h,v 1.3 1994/10/10 12:19:33 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1994/10/10 12:19:33 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzap01.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	17 Aug 1994	: Creation
* volcic	10 Oct 1994	: Modification (ajout include xdc.h, xdy.h) (v 1.2)
------------------------------------------------------ */

#ifndef xzap01
#define xzap01

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP01_RPC_NAME "XZAP;01"

#define XZAOC_XZAP01_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP01_Creer_PA ( XDY_Horodate,
			     XDY_Eqt,
			     XDY_Entier,
			     XDY_Octet,
			     XDY_Mot *);

extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
