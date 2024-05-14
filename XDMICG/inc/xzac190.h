/*E*/
/*X  Fichier : $Id: xzac190.h,v 1.1 2018/10/19 18:30:39 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/10/19 18:30:39 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac190.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* PNI	23/08/06	creation DEM1303 RVA 1.1
------------------------------------------------------ */

#ifndef xzac190
#define xzac190

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC190_RPC_NAME "XZAC190"

#define XZACC_XZAC190_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC190_Envoyer_RVA ( XDY_Entier    ,
 				 XDY_Octet     ,
  				 XDY_Entier ,
                                 XDY_Octet       ,
                                 XDY_Octet       ,
                                 XDY_Horodate  ,
                                 XDY_Horodate  ,
                                 XDY_Mot   ,
                                 XDY_Entier *      );

#endif
