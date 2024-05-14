/*E*/
/*Fichier :  $Id: xzac631.h,v 1.1 2008/02/13 13:24:22 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:22 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac631.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JBL		Nov 2007	:	DEM 662
------------------------------------------------------ */

#ifndef xzac631
#define xzac631

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC631_RPC_NAME "XZAC631"

#define XZACC_XZAC631_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC631_Commande_Fin_Signalisation ( XDY_Eqt       ,
                                 XDY_Octet      ,
                                 XDY_Horodate  ,
                                 XDY_Booleen   );

#endif
