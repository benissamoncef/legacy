/*E*/
/*  Fichier : $Id: xzap07.h,v 1.7 2020/07/23 12:34:13 gesconf Exp $      Release : $Revision: 1.7 $        Date : $Date: 2020/07/23 12:34:13 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzap07.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	31 Aug 1994	: Creation
* volcic	10 Oct 1994	: Modification (ajout include xdc.h, xdy.h) (v 1.2)
* volcic	05 Dec 1994	: Modification entete (v 1.6)
* PNI		23/07/200/20	: Ajout SMS DEM1301 v1.7
------------------------------------------------------ */

#ifndef xzap07
#define xzap07

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

#define XZAOC_XZAP07_RPC_NAME "XZAP07"

#define XZAOC_XZAP07_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP07_Ajout_Prop_Fax  (	XDY_Mot,
					XDY_Mot,
					XDY_District,
					XDY_Phrase,
					char *,
					XDY_Nom,
					char * ,
					XDY_Octet) ;



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
