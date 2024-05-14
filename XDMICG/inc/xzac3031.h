/*E*/
/*  Fichier : 	@(#)xzac3031.h
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac3031.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	13/11/23	: creation (DEM483) 
------------------------------------------------------ */

#ifndef xzac3031
#define xzac3031

/* fichiers inclus */
/* definitions de constantes exportees */
#define XZACC_XZAC3031_RPC_NAME "XZAC3031"
#define XZACC_XZAC3031_NB_PARM_RETURN 1

/* definitions de types exportes */
/* definition de macro exportees */
/* declarations de donnees globales */
/* delaration de fonctions externes */

extern int XZAC3031_Commande_Fin_PIC (	XDY_Horodate		va_HeureLancementFin_in,
										XDY_Entier			va_NumEvt_in,
										XDY_Octet			va_CleEvt_in,
										XDY_Operateur		va_Operateur_in,
										XDY_Eqt				va_NumEqt_in,
										XDY_Site			va_Site_in,
										XDY_Bandeau_PIC		va_Bandeau_in,
										XDY_Booleen			va_Flash_in,
										XDY_Picto			va_Picto_in,
										XDY_Octet			va_Priorite_in,
										XDY_Site			va_Site_Origine_in,
										XDY_Entier			va_NumeroAction_out);

#endif
