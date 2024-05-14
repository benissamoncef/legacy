/*E*/
/* Fichier : @(#)xzao443.h	1.1      Release : 1.1        Date :  12/11/00
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao443.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	27/09/00 : creation
------------------------------------------------------ */

#ifndef xzao443
#define xzao443

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO443_RPC_NAME "XZAO443"

#define XZAOC_XZAO443_NB_PARM_RETURN 8

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO443_Rechercher_les_PMVA_ds_pert (XDY_Autoroute,
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
