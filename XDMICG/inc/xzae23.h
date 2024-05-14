/*E*/
/*  Fichier : 	$Id: xzae23.h,v 1.5 1998/12/03 13:49:12 gaborit Exp $      Release : $Revision: 1.5 $        Date : $Date: 1998/12/03 13:49:12 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzao * FICHIER xzae23.h
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
* niepceron	03 Dec 1998	: XZAOC_XZAP23_... devient XZAOC_XZAE23_... v1.3
------------------------------------------------------*/

#ifndef xzae23
#define xzae23

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAE23_RPC_NAME "XZAE;51"

#define XZAOC_XZAE23_NB_PARM_RETURN 5

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAE23_Decrire_Bouchon_PA  (	XDY_Horodate,
					XDY_Evt,
					XDY_PR *,
					XDY_Octet *,
					XDY_Entier *,
					XDY_Evt *);



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
