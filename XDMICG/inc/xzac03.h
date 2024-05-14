/*E*/
/*  Fichier : 	@(#)xzac03.h	1.1  Release : 1.1 Date : 11/16/04
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac03.h
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

#ifndef xzac03
#define xzac03

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC03_RPC_NAME "XZAC;03"

#define XZACC_XZAC03_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC03_Commande_NAV ( XDY_Entier    ,
 				 XDY_Octet     ,
  				 XDY_Operateur ,
                                 XDY_Eqt       ,
                                 XDY_Horodate  ,
                                 XDY_Booleen   ,
                                 XDY_Octet     ,
                                 XDY_Entier    ,
                                 char      *,
                                 XDY_Octet      ,
                                 XDY_Octet      );

#endif
