/*E*/
/*  Fichier : $Id: xzap43.h,v 1.1 2007/12/13 11:36:21 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2007/12/13 11:36:21 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzap43.h
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

#ifndef xzap43
#define xzap43

/* fichiers inclus */

#include <string.h>
#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"
#include "xzap10.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP43_RPC_NAME "XZAP43"

#define XZAOC_XZAP43_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP43_Ajout_Prop_Mail  (	XDY_Mot,
					XDY_Mot,
					XDY_District,
					XDY_Phrase,
					char *,
					XDY_Nom,
					char * ) ;



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
