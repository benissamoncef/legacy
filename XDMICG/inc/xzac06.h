/*E*/
/*X  Fichier : $Id: xzac06.h,v 1.1 2006/12/05 16:12:26 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2006/12/05 16:12:26 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac06.h
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

#ifndef xzac06
#define xzac06

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC06_RPC_NAME "XZAC;06"

#define XZACC_XZAC06_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC06_Commande_Tunnel ( XDY_Entier    ,
 				 XDY_Octet     ,
  				 XDY_Operateur ,
                                 XDY_Eqt       ,
                                 XDY_Horodate  ,
                                 XDY_Booleen   ,
                                 XDY_Octet     ,
                                 XDY_Entier ,
				 XDY_Octet    );

#endif
