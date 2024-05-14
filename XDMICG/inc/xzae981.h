/*E*/
/*  Fichier : @(#)xzae981sp.h	1.1      Release : 1.1        Date : 08/29/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae981.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*
*
------------------------------------------------------
* HISTORIQUE :
*
* SDU  14/02/12 : Creation
-----------------------------------------------------*/

#ifndef xzae981
#define xzae981

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzae.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAEC_XZAE981_RPC_NAME "XZAE981"

#define XZAEC_XZAE981_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAE980_Recharche_FMC_Liee (
				XDY_Evt_Commu 	   va_Id_Commu,
				XDY_Evt 	       *pa_NumEvt,
				XDY_Booleen	       *pa_Encours,
				XDY_Synthese_Commu *pa_Synthese,
				XDY_Tendance_Commu *pa_Tendance);

extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;


#endif

