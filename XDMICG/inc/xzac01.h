/*E*/
/*X  Fichier : @(#)xzac01.h	1.1      Release : 1.1        Date : 12/05/06
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac01.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* PNI	23/08/06	creation
* VR	04/11/11	Ajout type XDY_Mot (DEM/1014)
------------------------------------------------------ */

#ifndef xzac01
#define xzac01

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC01_RPC_NAME "XZAC;01"

#define XZACC_XZAC01_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC01_Commande_PMV ( XDY_Entier    ,
 				 XDY_Octet     ,
  				 XDY_Operateur ,
                                 XDY_Eqt       ,
                                 XDY_Horodate  ,
                                 XDY_Octet     ,
				 char *,
				 char *,
				 char *,
				 char *,
				 char *,
				 char *,
                                 XDY_Octet      ,
                                 XDY_Octet      ,
                                 XDY_Octet      ,
                                 XDY_Octet      ,
				 char *,
				 char *,
                                 XDY_Octet      ,
                                 XDY_Octet      ,
                                 XDY_Entier    ,
				 char *,
				 char *,
                                 XDY_Mot      ,
                                 XDY_Mot      ,
                                 XDY_Octet      ,
                                 XDY_Octet      ,
								 XDY_Mot);

#endif
