/*
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao963.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JPL	23/01/2024 : Creation, similaire a XZAO962
------------------------------------------------------ */

#ifndef xzao963
#define xzao963

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO963_RPC_NAME "XZAO963"

#define XZAOC_XZAO963_NB_PARM_RETURN 3

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO963_Calcul_TDP_FCD_Pur (	XDY_Autoroute,
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
