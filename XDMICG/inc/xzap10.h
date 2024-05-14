/*E*/
/*  Fichier : @(#)xzap10.h	$Id: xzap10.h,v 1.2 1994/10/10 12:20:30 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/10/10 12:20:30 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzap10.h
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

#ifndef xzap10
#define xzap10

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP10_RPC_NAME "XZAP;10"

#define XZAOC_XZAP10_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP10_Ajout_Prop_Fax_Texte(	XDY_Mot,
					XDY_Mot,
					XDY_Texte ) ;



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
