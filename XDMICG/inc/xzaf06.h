/*E*/
/*  Fichier : 	$Id: xzaf06.h,v 1.3 1995/05/29 18:56:34 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/29 18:56:34 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzaf * FICHIER xzaf06.h
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
* C.T.       29/05/95     : Ajout de la date de formation en entree (V1.3)
------------------------------------------------------*/

#ifndef xzaf06
#define xzaf06

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAFC_XZAF06_RPC_NAME "XZAF;06"

#define XZAFC_XZAF06_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAF06_Animer_Voies_Neutralisees (	XDY_Horodate, 
						XDY_FonctionInt);
						
						
extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
