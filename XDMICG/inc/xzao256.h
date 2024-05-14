/*E*/
/* Fichier : @(#)xzao256.h	1.1     Release : 1.1        Date : 21/12/11
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao256.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  VR	21/12/11	: Creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzao256
#define xzao256

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO256_RPC_NAME "XZAO256"

#define XZAOC_XZAO256_NB_PARM_RETURN 7

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO256_Rechercher_tous_les_PRV_amont (XDY_Autoroute	,
						XDY_Sens	,
						XDY_PR		,
						XDY_PR		,
						XDY_District	,
						XDY_Entier	,
						XDY_Horodate	,
						char		*,
						XDY_Octet	,
						XDY_FonctionInt ,
						XDY_Entier 	* );

#endif
