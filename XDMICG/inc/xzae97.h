/*E*/
/*  Fichier : 	$Id: xzae97.h,v 1.1 1996/08/22 11:47:29 gesconf Exp $    Release : $Revision: 1.1 $        Date :$Date: 1996/08/22 11:47:29 $ 
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzao * FICHIER xzae97.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface des evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* guilhou	22 aou 1996	: Creation
------------------------------------------------------*/

#ifndef xzae97
#define xzae97

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP97_RPC_NAME "XZAE;97"

#define XZAOC_XZAP97_NB_PARM_RETURN 2

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAE97_Recuperer_Numero_Queue  (	XDY_Horodate,
						XDY_Evt,
						XDY_Evt *);



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
