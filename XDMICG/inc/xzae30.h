/*E*/
/*  Fichier : 	$Id: xzae30.h,v 1.3 1994/10/10 12:19:08 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1994/10/10 12:19:08 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzao * FICHIER xzae30.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface des evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	07 Sep 1994	: Creation
* volcic	10 Oct 1994	: Modification (ajout include xdc.h, xdy.h) (v 1.2)
------------------------------------------------------*/

#ifndef xzae30
#define xzae30

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzae.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAEC_XZAE30_RPC_NAME "XZAE;50"

#define XZAEC_XZAE30_NB_PARM_RETURN 4

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAE30_Decrire_Pb_Concessionnaire_PA (	XDY_Horodate,
							XDY_Evt,
							XZAET_SousConcess *);



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
