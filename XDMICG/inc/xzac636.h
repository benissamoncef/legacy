/*E*/
/*Fichier :  $Id: xzac636.h,v 1.1 2008/02/13 13:24:24 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:24 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac636.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* J.B.	10/12/07	: Creation a partir xzac63 DEM 662
------------------------------------------------------ */

#ifndef xzac636
#define xzac636

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC636_RPC_NAME "XZAC636"

#define XZACC_XZAC636_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC636_Commande_Fin_Exploitation ( XDY_Eqt       ,
                                 XDY_Octet      ,
                                 XDY_Horodate  ,
                                 XDY_Booleen   );

#endif
