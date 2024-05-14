/*E*/
/*  Fichier : @(#)xzao802.h	1.1      Release : 1.1        Date : 10/05/94
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao802.h
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

#ifndef xzao802								
#define xzao802

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAO802_RPC_NAME "XZAO802"

#define XZAO802_NB_PARM_RETURN 2

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO802_Tunnel_Oppose(	XDY_Autoroute  	va_autoroute_in,
					XDY_Sens va_sens_in,
					XDY_PR	va_PR_in,
					XDY_PR *va_PR_Debut_out,
					XDY_PR *va_PR_Fin_out );
						



#endif
