/*E*/
/*X  Fichier : $Id: xzac61.h,v 1.1 2006/12/05 16:12:25 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2006/12/05 16:12:25 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac61.h
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

#ifndef xzac61
#define xzac61

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC61_RPC_NAME "XZAC;61"

#define XZACC_XZAC61_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC61_Commande_Fin_PMV ( XDY_Eqt       ,
                                 XDY_Octet      ,
                                 XDY_Horodate  ,
                                 XDY_Octet      );

#endif
