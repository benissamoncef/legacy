/*E*/
/*Fichier : $Id: xzap180.h,v 1.1 2017/03/31 17:21:02 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2017/03/31 17:21:02 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap180sp.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   30/0317 creation 1.1 lot 23
------------------------------------------------------ */

#ifndef xzap180
#define xzap180

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAP180C_RPC_NAME	"XZAP180"
#define XZAP180C_NB_PARM_RETURN	1
extern int XZAP180_Lire_Prop_Appel_Majeur ( XDY_Mot      , char *      );
extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		va_NumPA_in
*
* ARGUMENTS EN SORTIE :
*
* XZAOPT_PropTel 	va_PropTel
*
*/

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
