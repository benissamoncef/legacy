/*E*/
/*X  Fichier : @(#)xzac903.h	1.1      Release : 1.1        Date : 04/12/20
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac903.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* CGR	04/12/20	: creation IMU 1.1 DEM-SAE155
------------------------------------------------------ */

#ifndef xzac903
#define xzac903

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC903_RPC_NAME "XZAC903"

#define XZACC_XZAC903_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */


extern int XZAC903_Commande_IMU ( XDY_Entier    va_NumEvt_in,
                                  XDY_Octet     va_CleEvt_in,
                                  XDY_Operateur va_Operateur_in,
                                  XDY_Eqt       va_NumEqt_in,
                                  XDY_Horodate  va_HeureLancement_in,
                                  XDY_Octet     va_Site_in,
				  XDY_ChaineIMU	*va_ChaineIMU_in,
				  XDY_Entier	va_Intervalle_in,
				  XDY_Entier	va_Cycles_in,
				  XDY_Entier	va_Duree_in,
                                  XDY_Octet     va_Priorite_in,
                                  XDY_Entier    va_DistanceEvt_in,
                                  XDY_Octet     va_site_origine_in,
                                  XDY_Entier	*va_NumeroAction_out);

#endif	
