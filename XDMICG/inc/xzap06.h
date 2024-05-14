/*E*/
/*  Fichier : @(#)xzap06.h	$Id: xzap06.h,v 1.3 1994/10/10 12:20:18 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1994/10/10 12:20:18 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzap06.h
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

#ifndef xzap06
#define xzap06

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP06_RPC_NAME "XZAP;06"

#define XZAOC_XZAP06_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP06_Ajout_Prop_Envoi_TFM(	XDY_Mot,
					XDY_Mot,
					XDY_District,
					XDY_Horodate,
					XDY_Nom,
					XDY_Phrase ) ;



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
