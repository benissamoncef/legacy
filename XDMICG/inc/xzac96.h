/*E*/
/*X  Fichier : %W%      Release : %I%        Date : %G%
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac96.h
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

#ifndef xzac96
#define xzac96

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC96_RPC_NAME "XZAC96"

#define XZACC_XZAC96_NB_PARM_RETURN 4

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC96_Lire_FMC_Data ( XDY_Entier, 
				XDY_Eqt       ,
                                 XDY_Entier *      ,
                                 XDY_Octet *      ,
                                 XDY_Entier *     ,
                                 XDY_Entier *     );

#endif
