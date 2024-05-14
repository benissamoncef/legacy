/*E*/
/*X  Fichier : @(#)xzac900.h	1.1      Release : 1.1        Date : 05/03/12
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac900.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	26/02/18	: creation CTRL_FEUX 1.1 DEM1284
------------------------------------------------------ */

#ifndef xzac900
#define xzac900

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC900_RPC_NAME "XZAC900"

#define XZACC_XZAC900_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */


extern int XZAC900_Commande_CFE ( XDY_Entier    va_NumEvt_in,
                                  XDY_Octet     va_CleEvt_in,
                                  XDY_Operateur va_Operateur_in,
                                  XDY_Eqt       va_NumEqt_in,
                                  XDY_Horodate  va_HeureLancement_in,
                                  XDY_Octet     va_Site_in,
				  XDY_Octet	va_PlanDeFeux_in,
				  XDY_LibellePlanDeFeux va_LibellePlanDex_in,
                                  XDY_Octet     va_Priorite_in,
                                  XDY_Entier    va_DistanceEvt_in,
                                  XDY_Octet     va_site_origine_in,
                                  XDY_Entier	*va_NumeroAction_out);
#endif
