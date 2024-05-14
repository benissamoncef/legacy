/*E*/
/*  Fichier : 	$Id: xzaf03.h,v 1.2 1995/01/17 12:03:44 torregrossa Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/17 12:03:44 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzaf * FICHIER xzaf03.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'interface de la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.       13/01/95     : Creation
* C.T.       17/01/95     : Aucune (V1.2)
------------------------------------------------------*/

#ifndef xzaf03
#define xzaf03

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

#define XZAFC_XZAF03_RPC_NAME "XZAF;03"

#define XZAFC_XZAF03_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAF03_Animer_Neut_Voies (	XDY_FonctionInt);
						
						
extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
