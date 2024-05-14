/*E*/
/* Fichier : $Id: xzao413.h,v 1.1 1998/01/09 14:12:35 verdier Exp $      Release : $Revision: 1.1 $        Date :  $Date: 1998/01/09 14:12:35 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao413.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	10/12/1997	: Creation (DEM/1534)
------------------------------------------------------ */

#ifndef xzao413
#define xzao413

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO413_RPC_NAME "XZAO413"

#define XZAOC_XZAO413_NB_PARM_RETURN 11

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO413_Lire_Liste_Zone_Influence_PMV (XDY_Basedd,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
