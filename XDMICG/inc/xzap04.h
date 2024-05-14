/*E*/
/* Fichier : 	$Id: xzap04.h,v 1.5 2009/10/22 15:55:22 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2009/10/22 15:55:22 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzap04.h
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
* volcic     	25 Nov 1994     : Modification XDY_Scenario (v1.4)
* JMG		23/09/08	: ajout domaine 1.5 DEM895
------------------------------------------------------ */

#ifndef xzap04
#define xzap04

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP04_RPC_NAME "XZAP;04"

#define XZAOC_XZAP04_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP04_Ajout_Prop_Tunnel  (	XDY_Mot,
					XDY_Mot,
					XDY_District,
					XDY_Phrase,
					XDY_Octet,
					XDY_Mot,
					XDY_Commentaire,
					XDY_Nom,
					XDY_Octet,
					XDY_Booleen,
					XDY_Octet) ;


extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
