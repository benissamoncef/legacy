/*E*/
/*  Fichier : 	$Id: xzap33.h,v 1.1 2005/09/14 14:25:02 gesconf Exp $     Release : $Revision: 1.1 $        Date : $Date: 2005/09/14 14:25:02 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzap03.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG       31 Aug 2005     : Creation
------------------------------------------------------ */

#ifndef xzap33
#define xzap33

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP33_RPC_NAME "XZAP33"

#define XZAOC_XZAP03_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP33_Ajout_Prop_BAD  (	XDY_Mot,
					XDY_Mot,
					XDY_District,
					XDY_Commentaire,
					XDY_Octet,
					XDY_Eqt,
					XDY_Commentaire,
					XDY_Nom,
					XDY_Scenario,
					XDY_Booleen ) ;



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
