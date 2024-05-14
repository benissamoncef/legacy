/*E*/
/* Fichier : $Id: xzao825.h,v 1.1 2017/02/10 09:01:21 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date :  $Date: 2017/02/10 09:01:21 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao825.h
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

#ifndef xzao825
#define xzao825

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO825_RPC_NAME "XZAO825"

#define XZAOC_XZAO825_NB_PARM_RETURN 8

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO825_Rechercher_les_PMV_SAGA_ds_pert (XDY_Autoroute,
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
