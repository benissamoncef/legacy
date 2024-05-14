/*E*/
/*Fichier : $Id: xzau16.h,v 1.2 1995/12/13 14:51:19 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/12/13 14:51:19 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAU * FICHIER xzau16.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Utilisateurs >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Mai 1995	: Creation
* volcic	29 Nov 1995	: Ajout includes
------------------------------------------------------ */

#ifndef xzau16
#define xzau16

/* fichiers inclus */
#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAUC_XZAU16_RPC_NAME "XZAU;16"

#define XZAUC_XZAU16_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAU16_Liste_Utilisateurs (	XDY_FonctionInt );

extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;


#endif
