/*E*/
/* Fichier : $Id: xzap05.h,v 1.4 1994/11/25 10:13:19 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1994/11/25 10:13:19 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzap05.h
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
* volcic        25 Nov 1994     : Modification XDY_Scenario (v1.4)
------------------------------------------------------ */

#ifndef xzap05
#define xzap05

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP05_RPC_NAME "XZAP;05"

#define XZAOC_XZAP05_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP05_Ajout_Prop_Echangeur(	XDY_Mot,
					XDY_Mot,
					XDY_District,
					XDY_Phrase,
					XDY_Octet,
					XDY_Mot,
					XDY_Commentaire,
					XDY_Nom,
					XDY_Octet,
					XDY_Booleen ) ;



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
