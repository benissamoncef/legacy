/*E*/
/*Fichier :  $Id: xzac0103.h,v 1.1 2008/02/13 13:24:19 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:19 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac0103.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JBL		Nov 2007	: DEM 662
------------------------------------------------------ */

#ifndef xzac103
#define xzac103

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC103_RPC_NAME "XZAC;103"

#define XZACC_XZAC103_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC103_Commande_Seq_ECL ( XDY_Entier    ,
 				 XDY_Octet     ,
  				 XDY_Operateur ,
                                 XDY_Octet     ,
                                 XDY_Eqt       ,
                                 XDY_Horodate  ,
                                 XDY_Booleen   ,
                                 XDY_Octet     ,
                                 XDY_Entier ,
				 XDY_Octet    );

#endif
