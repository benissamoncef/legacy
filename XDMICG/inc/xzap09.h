/*E*/
/*  Fichier : @(#)xzap09.h	$Id: xzap09.h,v 1.5 2018/10/19 17:40:06 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2018/10/19 17:40:06 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzap09.h
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
* volcic	05 Dec 1994	: Ajout parametre d'entree Explication (v 1.3)
* JPL		09/10/2014	: Degroupee; ajout du sens (DEM 1100)  1.4
* JMG		02/10/18	: ajout SMS LOT25 DEM1301 1.5
------------------------------------------------------ */

#ifndef xzap09
#define xzap09

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP09_RPC_NAME "XZAP09"

#define XZAOC_XZAP09_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP09_Ajout_Dest_Fax  (	XDY_Mot,
					XDY_Mot,
					XDY_PR,
					XDY_Autoroute,
					XDY_Sens,
					XDY_Octet,
					XDY_Commentaire,
					XDY_Octet) ;



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
