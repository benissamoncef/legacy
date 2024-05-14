/*E*/
/*  Fichier : 	$Id: xzac73.h,v 1.1 1999/02/26 11:34:50 gaborit Exp $  Release : $Revision: 1.1 $ Date : $Date: 1999/02/26 11:34:50 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac73.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* niepceron       09 Dec 1998     : Creation
------------------------------------------------------ */

#ifndef xzac73
#define xzac73

/* fichiers inclus */
#include "xzsm.h"

/* definitions de constantes exportees */

#define XZACC_XZAC73_RPC_NAME "XZAC73"

#define XZACC_XZAC73_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC73_Ecrire_Exporter ( XDY_Entier, XDY_Octet, XDY_Horodate, XDY_Octet );



#endif
