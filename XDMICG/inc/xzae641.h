/*E*/
/*  Fichier : 	$Id: xzae641.h,v 1.1 2008/11/17 10:55:06 pc2dpdy Exp $    Release : $Revision: 1.1 $        Date :$Date: 2008/11/17 10:55:06 $ 
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzao * FICHIER xzae641.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface des evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        CREATION        DEM 835
------------------------------------------------------*/

#ifndef xzae641
#define xzae641

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP641_RPC_NAME "XZAE641"

#define XZAOC_XZAP641_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAE641_Memorise_Diffusion  (	XDY_Entier,
						XDY_Octet,
						XDY_Octet );



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
