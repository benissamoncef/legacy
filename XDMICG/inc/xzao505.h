/*E*/
/*  Fichier : @(#)xzao505.h	1.1      Release : 1.1        Date : 10/05/94
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao505.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	24/05/2021 : Création DEM-SAE155 1.1
------------------------------------------------------ */

#ifndef xzao505								
#define xzao505

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC505_RPC_NAME "XZAO505"

#define XZAOC505_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO505_Recherche_IMU(	XDY_Texte va_NomBICE_in,
				int *va_NumEqt_out);
						



#endif
