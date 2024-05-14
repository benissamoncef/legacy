/*E*/
/* Fichier : $Id: xzao419.h,v 1.2 2004/11/16 14:16:05 gesconf Exp $      Release : $Revision: 1.2 $        Date :  $Date: 2004/11/16 14:16:05 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao419.h
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

#ifndef xzao419
#define xzao419

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO419_RPC_NAME "XZAO419"

#define XZAOC_XZAO419_NB_PARM_RETURN 8

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO419_Rechercher_le_PMV_amont (XDY_Autoroute,
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
					XDY_Entier,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
