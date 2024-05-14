/*E*/
/*  Fichier : 	$Id: xzae20.h,v 1.13 1999/02/26 11:42:07 gaborit Exp $      Release : $Revision: 1.13 $        Date : $Date: 1999/02/26 11:42:07 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzao * FICHIER xzae20.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface des evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	08 Sep 1994	: Creation
* volcic	10 Oct 1994	: Modification (ajout include xdc.h, xdy.h) (v 1.2)
* volcic	14 Oct 1994	: Modification (ajout include xdc.h, xdy.h) (v 1.3)
* volcic	14 Oct 1994	: Ajout 4 parametres de sorties (v 1.4)
* mercier	05 Dec 1994	: Ajout de 2 param  de sorties 	V 1.5
* torregrossa	25 Mar 1996	: Ajout de 1 param  de sorties 	V 1.6
* torregrossa	25 Sep 1996	: Ajout de 1 param  de sorties 	(FMC10) V 1.7
* torregrossa	04 Oct 1996	: Ajout de 1 param  de sorties 	(FMC3) V 1.8
* torregrossa	04 Oct 1996	: Ajout de 1 param  de sorties 	V 1.9
* torregrossa	04 Oct 1996	: Ajout de 2 param  de sorties 	(SAD4) V 1.10
* torregrossa	15 Dec 1997	: Ajout de 3 param  de sorties 	(1531) V 1.11
* torregrossa	12 Jan 1998	: Ajout de 1 param  de sorties 	(1532) V 1.12
* niepceron	13 Jan 1999	: Ajout de 5 param  de sorties, intergestionnaire (1624) v1.13
------------------------------------------------------*/

#ifndef xzae20
#define xzae20

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

#define XZAEC_XZAE20_RPC_NAME "XZAE;40"

#define XZAEC_XZAE20_NB_PARM_RETURN 53

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAE20_Decrire_Evt_PA (	XDY_Horodate,
					XDY_Evt,
					XDY_Fonction,
					XZAET_Evenement *,
					XZAET_Voies * );
				
						
						
extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif

