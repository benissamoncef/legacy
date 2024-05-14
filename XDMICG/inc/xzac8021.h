/*E*/
/*X  Fichier : @(#)xzac8021.h	1.1      Release : 1.1        Date : 05/03/12
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac8021.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	04/12/23	: creation (DEM483)
------------------------------------------------------ */
#ifndef xzac8021
#define xzac8021

/* fichiers inclus */
/* definitions de constantes exportees */
#define XZACC_XZAC8021_RPC_NAME "XZAC8021"
#define XZACC_XZAC8021_NB_PARM_RETURN 1

/* definitions de types exportes */
/* definition de macro exportees */
/* declarations de donnees globales */
/* delaration de fonctions externes */

extern int XZAC8021_Commande_PIC (	XDY_Entier			va_NumEvt_in,
									XDY_Octet			va_CleEvt_in,
									XDY_Operateur		va_Operateur_in,
									XDY_Eqt				va_NumEqt_in,
									XDY_Horodate		va_HeureLancement_in,
									XDY_Octet			va_Site_in,
									XDY_Bandeau_PIC		va_Bandeau_in,
									XDY_Booleen			va_Flash_in,
									XDY_Picto_PIC		va_Picto_in,
									XDY_Octet			va_Priorite_in,
									XDY_Entier			va_DistanceEvt_in,
									XDY_Octet			va_site_origine_in,
									XDY_Entier			*va_NumeroAction_out);
#endif
