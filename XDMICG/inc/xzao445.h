/*E*/
/* Fichier : @(#)xzao445.h	1.1     Release : 1.1        Date : 09/13/05
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao445.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	21/06/05 : creation
* JMG	26/11/16	filtre peage amont lot 23 1.2
------------------------------------------------------ */

#ifndef xzao445
#define xzao445

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO445_RPC_NAME "XZAO445"

#define XZAOC_XZAO445_NB_PARM_RETURN 4

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO445_Rechercher_Peages_Amont (XDY_Autoroute	,
						XDY_Sens	,
						XDY_PR		,
						XDY_PR		,
						XDY_District	,
						XDY_Entier	,
						XDY_Horodate	,
						char		*,
						XDY_FonctionInt ,
						XDY_Entier 	* );

#endif
