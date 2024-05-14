/*E*/
/*Fichier :  $Id: xzac634.h,v 1.1 2008/02/13 13:24:23 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:23 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac634.h
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

#ifndef xzac634
#define xzac634

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC634_RPC_NAME "XZAC634"

#define XZACC_XZAC634_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC634_Commande_Fin_Energie ( XDY_Eqt       ,
                                 XDY_Octet      ,
                                 XDY_Horodate  ,
                                 XDY_Booleen   );

#endif
