/*E*/
/*Fichier :  $Id: xzac633.h,v 1.1 2008/02/13 13:24:23 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:23 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac633.h
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

#ifndef xzac633
#define xzac633

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC633_RPC_NAME "XZAC633"

#define XZACC_XZAC633_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC633_Commande_Fin_Ventilation ( XDY_Eqt       ,
                                 XDY_Octet      ,
                                 XDY_Horodate  ,
                                 XDY_Booleen   );

#endif
