/*E*/
/*  Fichier : $Id: xzap42.h,v 1.1 2007/12/13 10:50:39 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2007/12/13 10:50:39 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE xzao * FICHIER xzap42.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Plan d'actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	17/01/07	creation
------------------------------------------------------ */

#ifndef xzap42
#define xzap42

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAOC_XZAP42_RPC_NAME "XZAP42"

#define XZAOC_XZAP42_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAP42_Ajout_Prop_Mail_Texte(	XDY_Mot,
					XDY_Mot,
					XDY_Texte ) ;



extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;



#endif
