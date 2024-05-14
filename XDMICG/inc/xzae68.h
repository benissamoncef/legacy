/*E*/
/*  Fichier : 	$Id: xzae68.h,v 1.1 1994/10/12 19:10:47 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/12 19:10:47 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzao * FICHIER xzae68.h
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

#ifndef xzae68
#define xzae68

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

#define XZAEC_XZAE68_RPC_NAME "XZAE;68"

#define XZAEC_XZAE68_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAE68_ListeConsequences (	XDY_Evt,
					XDY_Horodate,
					XDY_Octet,
					XDY_Fonction );

											
						
extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
