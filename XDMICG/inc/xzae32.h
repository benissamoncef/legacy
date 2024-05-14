/*E*/
/*  Fichier : 	$Id: xzae32.h,v 1.3 1994/10/10 12:19:29 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1994/10/10 12:19:29 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzao * FICHIER xzae32.h
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

#ifndef xzae32
#define xzae32

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

#define XZAEC_XZAE32_RPC_NAME "XZAE;52"

#define XZAEC_XZAE32_NB_PARM_RETURN 4

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAE32_Decrire_Fermeture_Echangeur_PA (	XDY_Horodate,
							XDY_Evt,
							XZAET_FermetureEchangeur *);



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
