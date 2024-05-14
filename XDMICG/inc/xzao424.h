/*E*/
/* Fichier : $Id: xzao424.h,v 1.1 2000/12/11 14:39:26 gesconf Exp $      Release : $Revision: 1.1 $        Date :  $Date: 2000/12/11 14:39:26 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao424.h
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

#ifndef xzao424
#define xzao424

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO424_RPC_NAME "XZAO424"

#define XZAOC_XZAO424_NB_PARM_RETURN 8

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO424_Rechercher_tous_les_PMV_amont (XDY_Autoroute,
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
