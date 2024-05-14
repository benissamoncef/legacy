/*E*/
/*  Fichier : $Id: xzap44.h,v 1.1 2007/12/13 11:36:41 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2007/12/13 11:36:41 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzap09.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	17/10/07	creation
------------------------------------------------------ */

#ifndef xzap44
#define xzap44

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP44_RPC_NAME "XZAP44"

#define XZAOC_XZAP44_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP44_Ajout_Dest_Mail  (	XDY_Mot,
					XDY_Mot,
					XDY_PR,
					XDY_Autoroute,
					XDY_Octet,
					XDY_Commentaire ) ;



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
