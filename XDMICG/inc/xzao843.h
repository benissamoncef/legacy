/*E*/
/* Fichier : @(#)xzao843.h	1.1      Release : 1.1        Date :  12/11/00
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao843.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   30/0317 creation 1.1 DEM1191
------------------------------------------------------ */

#ifndef xzao843
#define xzao843

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO843_RPC_NAME "XZAO843"

#define XZAOC_XZAO843_NB_PARM_RETURN 8

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO843_Rechercher_les_PMVA_SAGA_ds_pert (XDY_Autoroute,
					XDY_Sens,
					XDY_PR,
					XDY_PR,
					XDY_Entier,
					XDY_Entier,
					XDY_PR,
					XDY_PR,
					XDY_Autoroute,
					XDY_Autoroute,
					XDY_PR,
					XDY_PR,
					XDY_District,
					XDY_Entier,
					XDY_Horodate,
					char *,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
