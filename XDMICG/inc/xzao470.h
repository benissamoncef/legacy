/*  Fichier : @(#)xzao470.h     1.1      Release : 1.1        Date : 30/01/12
------------------------------------------------------
* GTIE OJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao470.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* V. REYBIER     30/01/2012     : Creation (DEM/1014)
------------------------------------------------------ */

#ifndef xzao470
#define xzao470

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO470_RPC_NAME "XZAO470"

#define XZAOC_XZAO470_NB_PARM_RETURN 14

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO470_Lire_Liste_Destination_PMV ( XDY_Basedd,
						XDY_FonctionInt,
												XDY_Entier * );

												#endif

