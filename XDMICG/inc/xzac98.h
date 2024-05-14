/*E*/
/*X  Fichier : $Id: xzac98.h,v 1.1 2006/12/05 16:12:25 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2006/12/05 16:12:25 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac98.h
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

#ifndef xzac98
#define xzac98

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC98_RPC_NAME "XZAC98"

#define XZACC_XZAC98_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC98_Commande_Fin_BAF ( XDY_Eqt       ,
                                 XDY_Octet      ,
                                 XDY_Horodate  ,
                                 XDY_Booleen   ,
                                 XDY_Octet      ,
                                 XDY_Octet      ,
                                 XDY_Octet      ,
                                 XDY_Octet      ,
                                 XDY_Octet      ,
                                 XDY_Octet      );

#endif
