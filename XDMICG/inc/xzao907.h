/*E*/
/*  Fichier : @(#)xzao907.h	1.1      Release : 1.1        Date : 10/05/94
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao907.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	11/05/2021 : Création DEM-SAE244 1.1
------------------------------------------------------ */

#ifndef xzao907								
#define xzao907

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAO907_RPC_NAME "XZAO907"

#define XZAO907_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO907_Trouve_PAU(	XDY_District va_Site_in,
				XDY_Texte va_Autoroute_in,
				XDY_Sens va_Sens_in,
				XDY_PR va_PR_Debut_in,
				XDY_PR va_PR_Fin_in,
				XDY_District va_site_origine_in,
				XDY_Entier va_DistanceEvt_in,
				XDY_Entier *va_NumEqt_out);
						



#endif
