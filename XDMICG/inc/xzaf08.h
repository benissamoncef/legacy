/*E*/
/*  Fichier : 	$Id: xzaf08.h,v 1.2 1995/01/17 12:03:49 torregrossa Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/17 12:03:49 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzaf * FICHIER xzaf08.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'interface de la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.       16/01/95     : Creation
* C.T.       17/01/95     : Aucune (V1.2)
------------------------------------------------------*/

#ifndef xzaf08
#define xzaf08

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzaf.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAFC_XZAF08_RPC_NAME "XZAF;08"

#define XZAFC_XZAF08_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAF08_Animer_Voies_RADT_Inversees ( XDY_FonctionInt);
						
						
extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
