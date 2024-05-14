/*E*/
/*X  Fichier : @(#)xzac802.h	1.1      Release : 1.1        Date : 05/03/12
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac802.h
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

#ifndef xzac802
#define xzac802

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC802_RPC_NAME "XZAC802"

#define XZACC_XZAC802_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */


extern int XZAC802_Commande_PRV ( XDY_Entier    va_NumEvt_in,
                                 XDY_Octet     va_CleEvt_in,
                                 XDY_Operateur va_Operateur_in,
                                 XDY_Eqt       va_NumEqt_in,
                                 XDY_Horodate  va_HeureLancement_in,
                                 XDY_Octet      va_Site_in,
								 XDY_Bandeau	va_Bandeau_in,
								 XDY_VitesseRegulTxt	va_Vitesse_in,
                                 XDY_Octet     va_Priorite_in,
                                 XDY_Entier    va_DistanceEvt_in,
                                 XDY_Octet      va_site_origine_in,
                                 XDY_Entier		*va_NumeroAction_out);
#endif
