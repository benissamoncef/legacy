/*E*/
/*Fichier : $Id: xzac550.h,v 1.1 2012/07/04 10:54:56 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:54:56 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac550.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	06/03/12	: creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzac550
#define xzac550

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC550_RPC_NAME "XZAC550"

#define XZACC_XZAC550_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC550_Commande_PRV ( XDY_Entier    ,
								 XDY_Octet     ,
								 XDY_Operateur ,
                                 XDY_Eqt       ,
                                 XDY_Horodate  ,
                                 XDY_Octet     ,
								 XDY_Bandeau,
								 XDY_VitesseRegulTxt,
                                 XDY_Octet      ,
                                 XDY_Entier    ,
                                 XDY_Octet);

#endif
