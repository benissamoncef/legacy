/*Fichier : $Id: xzac5501.h
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac5501.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	13/11/23	: Creation (DEM483)
------------------------------------------------------ */

#ifndef xzac5501
#define xzac5501

/* fichiers inclus */
/* definitions de constantes exportees */
#define XZACC_XZAC5501_RPC_NAME "XZAC5501"
#define XZACC_XZAC5501_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC5501_Commande_PIC (	XDY_Entier		va_NumEvt_in,
									XDY_Octet		va_CleEvt_in,
									XDY_Operateur	va_Operateur_in,
									XDY_Eqt			va_NumEqt_in,
									XDY_Horodate	va_HeureLancement_in,
									XDY_Octet		va_Site_in,
									XDY_Bandeau_PIC	va_Bandeau_in,
									XDY_Booleen		va_Flash_in,
									XDY_Picto		va_Picto_in,
									XDY_Octet		va_Priorite_in,
									XDY_Entier		va_DistanceEvt_in,
									XDY_Octet		va_site_origine_in,
									XDY_Entier		va_NumeroAction_out);

#endif
