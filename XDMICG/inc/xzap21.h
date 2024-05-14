/*E*/
/*  Fichier : $Id: xzap21.h,v 1.2 1994/11/25 10:13:23 volcic Exp $      Release : $Revision: 1.2 $       Date : $Date: 1994/11/25 10:13:23 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzap21.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	15 Nov 1994	: Creation
* volcic	25 Nov 1994	: Suppression parametre Resultat (v1.2)
------------------------------------------------------ */

#ifndef xzap21
#define xzap21

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAPC_XZAP21_RPC_NAME "XZAP;21"

#define XZAPC_XZAP21_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP21_Placer_Evt_File_Attente  (	XDY_Mot,
						XDY_Mot,
						XDY_Octet,
						XDY_Octet,
						XDY_Entier ) ;



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
