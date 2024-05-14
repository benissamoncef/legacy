/*E*/
/* Fichier : @(#)xzao442.h	1.1     Release : 1.1        Date : 09/13/05
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao442.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	21/06/05 : creation
------------------------------------------------------ */

#ifndef xzao442
#define xzao442

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO442_RPC_NAME "XZAO442"

#define XZAOC_XZAO442_NB_PARM_RETURN 7

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO442_Rechercher_tous_les_BAD_amont (XDY_Autoroute	,
						XDY_Sens	,
						XDY_PR		,
						XDY_PR		,
						XDY_District	,
						XDY_Entier	,
						XDY_Horodate	,
						char		*,
						int		,
						XDY_FonctionInt ,
						XDY_Entier 	* );

#endif
