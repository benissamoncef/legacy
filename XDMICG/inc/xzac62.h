/*E*/
/*  Fichier : 	@(#)xzac62.h	1.1  Release : 1.1 Date : 11/16/04
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac62.h
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

#ifndef xzac62
#define xzac62

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC62_RPC_NAME "XZAC;62"

#define XZACC_XZAC62_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC62_Commande_Fin_NAV ( XDY_Eqt       ,
                                 XDY_Octet      ,
                                 XDY_Horodate  ,
                                 XDY_Booleen   ,
                                 char      *);

#endif
