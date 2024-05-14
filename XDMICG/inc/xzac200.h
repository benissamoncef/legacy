/*E*/
/*X  Fichier : $Id: xzac200.h,v 1.1 2018/10/19 18:30:45 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/10/19 18:30:45 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac200.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* PNI	23/08/06	creation RVA LOT25 DEM1303
------------------------------------------------------ */

#ifndef xzac200
#define xzac200

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC200_RPC_NAME "XZAC200"

#define XZACC_XZAC200_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC200_Inserer_Texte ( XDY_Entier    ,
 				 XDY_Octet     ,
                                 char *      );

#endif
