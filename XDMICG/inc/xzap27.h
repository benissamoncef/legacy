/*E*/
/* Fichier : $Id: xzap27.h,v 1.1 2004/05/04 10:08:48 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2004/05/04 10:08:48 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzap27.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	27/04/04	creation	1.1
------------------------------------------------------ */

#ifndef xzap27
#define xzap27

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP27_RPC_NAME "XZAP27"

#define XZAOC_XZAP27_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP27_Ajout_Prop_PAL  (	XDY_Mot,
					XDY_Mot,
					XDY_District,
					XDY_Commentaire,
					XDY_Octet,
					XDY_Eqt,
					XDY_Commentaire,
					XDY_Nom,
					char *,
					XDY_Booleen ) ;



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
