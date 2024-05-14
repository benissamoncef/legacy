/*E*/
/*  Fichier : @(#)xzap02.h	$Id: xzap02.h,v 1.7 2017/10/10 08:28:04 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2017/10/10 08:28:04 $
------------------------------------------------------
* GTIE   PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao FICHIER xzap02.h
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
* niepceron	10 Mar 1997	: Ajout destination et destination alternat tdp (1.3)
* reybier	30 Jan 2012	: Ajou destination noeud TdP etendus (DEM 1014) 1.4
* JMG	26/06/17 : convergence 1.7
* GGY	01/08/23 : Ajout Flash_In (DEM473)
------------------------------------------------------ */

#ifndef xzap02
#define xzap02

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP02_RPC_NAME "XZAP02"

#define XZAOC_XZAP02_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP02_Ajout_Prop_PMV  (	XDY_Mot,
                          		XDY_Mot,
                          		XDY_District,
                          		XDY_Phrase,
                          		XDY_Octet,
                          		XDY_Eqt,
                          		XDY_Commentaire,
                          		XDY_Ligne_PMV,
                          		XDY_Ligne_PMV,
                          		XDY_Ligne_PMV,
                          		XDY_Ligne_PMV,
                          		XDY_Ligne_PMV,
                          		XDY_Ligne_PMV,
                          		XDY_Nom,
                          		XDY_TxtPicto,
					XDY_NomPicto,
					XDY_Mot,
					XDY_Mot,
					XDY_Mot,
					XDY_Booleen , 
					int,
					XDY_Booleen);



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
