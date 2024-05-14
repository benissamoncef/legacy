/*E*/
/*  Fichier : 	$Id: xzae27.h,v 1.5 1996/08/29 18:02:39 verdier Exp $      Release : $Revision: 1.5 $        Date : $Date: 1996/08/29 18:02:39 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzao * FICHIER xzae27.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface des evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	07 Sep 1994	: Creation
* volcic	10 Oct 1994	: Modification (ajout include xdc.h, xdy.h) (v 1.2)
* BG		03 Mai 1995	: Ajout d'un argument d'appel proc stockee (v1.4)
------------------------------------------------------*/

#ifndef xzae27
#define xzae27

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

#define XZAEC_XZAE27_RPC_NAME "XZAE;45"

#define XZAEC_XZAE27_NB_PARM_RETURN 36

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAE27_Decrire_Pb_Veh_PA  (	XDY_Horodate,
					XDY_Evt,
					XZAET_Vehicule *);



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
