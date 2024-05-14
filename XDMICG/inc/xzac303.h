/*E*/
/*  Fichier : 	@(#)xzac303.h	1.1  Release : 1.1 Date : 05/03/12
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac303.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	05/03/12	: creation (DEM/1016) 
------------------------------------------------------ */

#ifndef xzac303
#define xzac303

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC303_RPC_NAME "XZAC303"

#define XZACC_XZAC303_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC303_Commande_Fin_PRV ( XDY_Entier    va_NumEvt_in,
										XDY_Octet     va_CleEvt_in,
										XDY_Operateur va_Operateur_in,
										XDY_Eqt       va_NumEqt_in,
										XDY_Horodate  va_HeureLancementFin_in,
										XDY_Octet      va_Site_in,
										XDY_Bandeau	va_Bandeau_in,
										XDY_VitesseRegulTxt	va_Vitesse_in,
										XDY_Octet     va_Priorite_in,
										XDY_Octet      va_Site_Origine_in);

#endif
