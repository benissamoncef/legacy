/*E*/
/*  Fichier : $Id: xzap29.h,v 1.1 2007/03/26 10:31:09 gesconf Exp $      Release : $Revision: 1.1 $       Date : $Date: 2007/03/26 10:31:09 $
------------------------------------------------------
* GTIE   PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao FICHIER xzap29.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	 Creation
------------------------------------------------------ */

#ifndef xzap29
#define xzap29

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP29_RPC_NAME "XZAP29"

#define XZAOC_XZAP29_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP29_Ajout_Prop_BAF  (	XDY_Mot,
                          		XDY_Mot,
                          		XDY_District,
                          		XDY_Phrase,
                          		XDY_Octet,
                          		XDY_Eqt,
                          		XDY_Commentaire,
                          		XDY_Octet,
                          		XDY_Octet,
                          		XDY_Octet,
                          		XDY_Octet,
                          		XDY_Octet,
                          		XDY_Octet,
                             		XDY_Nom,
					XDY_Booleen );



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
