/*E*/
/*  Fichier : 	$Id: xzac66.h,v 1.1 2004/11/16 14:14:18 gesconf Exp $  Release : $Revision: 1.1 $ Date : $Date: 2004/11/16 14:14:18 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac66.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	12/11/04	creation 
------------------------------------------------------ */

#ifndef xzac66
#define xzac66

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC66_RPC_NAME "XZAC;66"

#define XZACC_XZAC66_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC66_Commande_PAL ( XDY_Entier    ,
 				 XDY_Octet     ,
  				 XDY_Operateur ,
                                 XDY_Eqt       ,
                                 XDY_Horodate  ,
                                 XDY_Booleen   ,
                                 XDY_Octet     ,
                                 XDY_Entier    ,
                                 char       *,
                                 char      *,
                                 XDY_Octet      ,
                                 XDY_Octet      );

#endif
