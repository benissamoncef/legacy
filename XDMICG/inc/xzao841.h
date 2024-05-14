/*E*/
/* Fichier : @(#)xzao841.h	1.1     Release : 1.1        Date : 09/13/05
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao841.h
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

#ifndef xzao841
#define xzao841

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO841_RPC_NAME "XZAO841"

#define XZAOC_XZAO841_NB_PARM_RETURN 7

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO841_Rechercher_tous_les_BAF_SAGA_amont (XDY_Autoroute	,
						XDY_Sens	,
						XDY_PR		,
						XDY_PR		,
						XDY_District	,
						XDY_Entier	,
						XDY_Horodate	,
						char		*,
						int		,
						int	,
						XDY_FonctionInt ,
						XDY_Entier 	* );

#endif
