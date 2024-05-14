/*
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao962.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* LPE  19/08/2020	: Calcul TDP_FCD DEM-SAE152 1.1
* ABE  25/09/2020	: Correction après recette DEM-SAE152 1.2
* LPE  18/11/2020	: Ajout numero et cle FMC DEM-SAE152 1.3
------------------------------------------------------ */

#ifndef xzao962
#define xzao962

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO962_RPC_NAME "XZAO962"

#define XZAOC_XZAO962_NB_PARM_RETURN 3

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO962_Calcul_TDP_FCD (	XDY_Autoroute,
					XDY_PR,
 					XDY_Sens,
 					XDY_Autoroute,
 					XDY_PR,
 					XDY_Sens,
					XDY_Entier,
					XDY_Octet,
 					XDY_TDP * va_TDP_out,
					XDY_Octet * va_validite_out,		
					XDY_Booleen * va_validite_faible_out);

#endif
