/*E*/
/*  Fichier : 	$Id: xzac67.h,v 1.1 2004/11/16 14:14:24 gesconf Exp $  Release : $Revision: 1.1 $ Date : $Date: 2004/11/16 14:14:24 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac67.h
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

#ifndef xzac67
#define xzac67

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC67_RPC_NAME "XZAC;67"

#define XZACC_XZAC67_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC67_Commande_Fin_PAL ( XDY_Eqt       ,
                                 XDY_Octet      ,
                                 XDY_Horodate  ,
                                 XDY_Booleen   ,
                                 char       *,
                                 char      *);

#endif
