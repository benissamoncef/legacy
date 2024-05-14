/*E*/
/*  Fichier : 	@(#)xzat51.h	1.2  Release : 1.2 Date : 05/03/12
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat51.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       05 Mars 2012     : Creation (DEM/1016)
* PNI 27/11/20 : Correct du nb de param 1.3
------------------------------------------------------ */

#ifndef xzat51
#define xzat51

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZATC_XZAT51_RPC_NAME "XZAT51"

#define XZATC_XZAT51_NB_PARM_RETURN 9

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */


int XZAT51_Utilisation_PRV  ( 	XDY_Horodate	va_Horodate_in,
				XDY_Mot		va_NumEqt_in,
				XDY_Octet 	*va_DispoPRV_out,
				XDY_VitesseRegulTxt	va_Vitesse_out,
				XDY_Bandeau		va_Bandeau_out,
				XDY_Entier 	*va_NumEvt_out,
				XDY_Octet 	*va_CleEvt_out,
				XDY_Octet 	*va_Priorite_out,
				XDY_Entier 	*va_DistanceEvt_out ,
				XDY_Entier *va_Action_out,
				XDY_Octet *va_SiteAction_out);


#endif
