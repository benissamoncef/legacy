/*E*/
/*  Fichier : 	@(#)xzac901.h	1.1  Release : 1.1 Date : 05/03/12
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac901.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	26/02/18	: creation CTRL_FEUX 1.1
------------------------------------------------------ */

#ifndef xzac901
#define xzac901

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC901_RPC_NAME "XZAC901"

#define XZACC_XZAC901_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC901_Commande_Fin_CFE ( XDY_Entier    	va_NumEvt_in,
					XDY_Octet     	va_CleEvt_in,
					XDY_Operateur 	va_Operateur_in,
					XDY_Eqt       	va_NumEqt_in,
					XDY_Horodate  	va_HeureLancementFin_in,
					XDY_Octet      	va_Site_in,
					XDY_Octet	va_PlanDeFeux_in,
					XDY_LibellePlanDeFeux   va_LibellePlanDeFeux_in,
					XDY_Octet     	va_Priorite_in,
					XDY_Octet      	va_Site_Origine_in);

#endif
