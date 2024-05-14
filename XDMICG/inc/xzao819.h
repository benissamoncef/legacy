/*E*/
/* Fichier : $Id: xzao819.h,v 1.1 2017/02/10 09:00:18 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date :  $Date: 2017/02/10 09:00:18 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao819.h
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

#ifndef xzao819
#define xzao819

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO819_RPC_NAME "XZAO819"

#define XZAOC_XZAO819_NB_PARM_RETURN 8

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO819_Rechercher_le_PMV_SAGA_amont (XDY_Autoroute,
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
