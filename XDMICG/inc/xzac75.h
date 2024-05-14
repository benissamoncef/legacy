/*E*/
/*  Fichier : 	$Id: xzac75.h,v 1.1 1999/02/26 11:34:51 gaborit Exp $  Release : $Revision: 1.1 $ Date : $Date: 1999/02/26 11:34:51 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac75.h
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

#ifndef xzac75
#define xzac75

/* fichiers inclus */
#include "xzsm.h"

/* definitions de constantes exportees */

#define XZACC_XZAC75_RPC_NAME "XZAC75"

#define XZACC_XZAC75_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC75_Ecrire_ExporterTFM ( XDY_Entier, XDY_Horodate, XDY_Octet );



#endif
