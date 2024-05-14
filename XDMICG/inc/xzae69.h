/*E*/
/*  Fichier : 	$Id: xzae69.h,v 1.1 1994/10/12 19:10:52 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/12 19:10:52 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzao * FICHIER xzae69.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface des evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	12 Oct 1994	: Creation
------------------------------------------------------*/

#ifndef xzae69
#define xzae69

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzae.h"
#include "xzaec.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAEC_XZAE69_RPC_NAME "XZAE;69"

#define XZAEC_XZAE69_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAE69_Liste_Causes (XDY_Evt,
				XDY_Horodate,
				XDY_Fonction );

											
						
extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
