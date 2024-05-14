/*E*/
/* Fichier : $Id: xzao425.h,v 1.1 2000/12/11 14:39:20 gesconf Exp $      Release : $Revision: 1.1 $        Date :  $Date: 2000/12/11 14:39:20 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao425.h
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

#ifndef xzao425
#define xzao425

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO425_RPC_NAME "XZAO425"

#define XZAOC_XZAO425_NB_PARM_RETURN 8

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO425_Rechercher_les_PMV_ds_pert (XDY_Autoroute,
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
