/*E*/
/*Fichier : $Id: xzai07.h,v 1.1 1994/11/02 19:58:40 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/02 19:58:40 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAI * FICHIER xzai07.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Info TFM >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Nov 1994	: Creation
------------------------------------------------------*/

#ifndef xzai07
#define xzai07

/* fichiers inclus */

#include "xdy.h"
#include "asql_com.h"
#include "xzat.h"

/* definitions de constantes exportees */

#define XZAIC_XZAI07_RPC_NAME "XZAI;07"

#define XZAIC_XZAI07_NB_PARM_RETURN 2

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;

extern int XZAI07_Rechercher_Message_A_Diffuser (	XDY_Evt,
							XDY_Horodate *,
							XDY_Booleen *); 



#endif
