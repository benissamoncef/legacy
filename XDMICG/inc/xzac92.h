/*E*/
/*X  Fichier : $Id: xzac92.h,v 1.1 2006/12/05 16:12:37 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2006/12/05 16:12:37 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac92.h
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

#ifndef xzac92
#define xzac92

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC92_RPC_NAME "XZAC92"

#define XZACC_XZAC92_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC92_Commande_BAF ( XDY_Entier    ,
 				 XDY_Octet     ,
  				 XDY_Operateur ,
                                 XDY_Eqt       ,
                                 XDY_Horodate  ,
                                 XDY_Booleen   ,
                                 XDY_Octet     ,
                                 XDY_Entier    ,
                                 XDY_Octet      ,
                                 XDY_Octet      ,
                                 XDY_Octet      ,
                                 XDY_Octet      ,
                                 XDY_Octet      ,
                                 XDY_Octet      ,
                                 XDY_Octet      ,
                                 XDY_Octet      );

#endif
