/*E*/
/*X  Fichier : $Id: xzac84.h,v 1.1 2006/12/05 16:12:25 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2006/12/05 16:12:25 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac84.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* PNI	23/08/06	creation
------------------------------------------------------ */

#ifndef xzac84
#define xzac84

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC84_RPC_NAME "XZAC84"

#define XZACC_XZAC84_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC84_Commande_Fin_PMVA ( XDY_Eqt       ,
                                 XDY_Octet      ,
                                 XDY_Horodate  ,
                                 XDY_Octet      );

#endif
