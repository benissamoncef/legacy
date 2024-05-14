/*E*/
/*Fichier :  $Id: xzac635.h,v 1.1 2008/02/13 13:24:23 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:23 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac635.h
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

#ifndef xzac635
#define xzac635

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC635_RPC_NAME "XZAC635"

#define XZACC_XZAC635_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC635_Commande_Fin_Incendie ( XDY_Eqt       ,
                                 XDY_Octet      ,
                                 XDY_Horodate  ,
                                 XDY_Booleen   );

#endif
