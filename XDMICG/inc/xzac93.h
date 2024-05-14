/*E*/
/*X  Fichier : $Id: xzac93.h,v 1.1 2006/12/05 16:12:25 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2006/12/05 16:12:25 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac93.h
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

#ifndef xzac93
#define xzac93

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC93_RPC_NAME "XZAC93"

#define XZACC_XZAC93_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC93_Commande_BAD ( XDY_Entier    ,
 				 XDY_Octet     ,
  				 XDY_Operateur ,
                                 XDY_Eqt       ,
                                 XDY_Horodate  ,
                                 XDY_Booleen   ,
                                 XDY_Octet     ,
                                 XDY_Entier    ,
				 char *,
                                 XDY_Octet      ,
                                 XDY_Octet      );

#endif
