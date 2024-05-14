/*E*/
/*X  Fichier : $Id: xzac126.h,v 1.1 2018/10/19 18:30:32 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/10/19 18:30:32 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac126.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	23/07/18	creation RVA LOT25 DEM1303 1.1
------------------------------------------------------ */

#ifndef xzac126
#define xzac126

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC126_RPC_NAME "XZAC126"

#define XZACC_XZAC126_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC126_Premier_RVA ( XDY_Entier    ,
 				 XDY_Octet     ,
                                 XDY_Entier  *     );

#endif
