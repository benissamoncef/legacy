/*E*/
/*X  Fichier : $Id: xzac63.h,v 1.1 2006/12/05 16:12:26 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2006/12/05 16:12:26 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac63.h
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

#ifndef xzac63
#define xzac63

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC63_RPC_NAME "XZAC;63"

#define XZACC_XZAC63_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC63_Commande_Fin_Tunnel ( XDY_Eqt       ,
                                 XDY_Octet      ,
                                 XDY_Horodate  ,
                                 XDY_Booleen   );

#endif
