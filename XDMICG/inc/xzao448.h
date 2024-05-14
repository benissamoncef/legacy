/*E*/
/* Fichier : @(#)xzao448.h	1.1     Release : 1.1        Date : 05/03/12
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao448.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	05/03/12 : creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzao448
#define xzao448

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO448_RPC_NAME "XZAO448"

#define XZAOC_XZAO448_NB_PARM_RETURN 7

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO448_Rechercher_tous_les_PRV_amont (XDY_Autoroute	,
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
