/*E*/
/*  Fichier : 	$Id: acli_eqt.h,v 1.1 1995/08/10 18:43:14 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/08/10 18:43:14 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE acli * FICHIER acli_eqt.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'interface de l'initialisation des equipements
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.       16/02/95     : Creation
------------------------------------------------------*/

#ifndef ACLI_EQT
#define ACLI_EQT

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define ACLIC_EQT_RPC_NAME "XZAT;26"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAT26_Init_Etats_Eqts (	XDY_Octet,
					XDY_Entier,
					XDY_NomMachine);
					
						
						
extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
