/*E*/
/* Fichier : $Id: xzao440.h,v 1.1 2005/09/13 18:12:12 gesconf Exp $     Release : $Revision: 1.1 $        Date : $Date: 2005/09/13 18:12:12 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao440.h
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

#ifndef xzao440
#define xzao440

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO440_RPC_NAME "XZAO440"

#define XZAOC_XZAO440_NB_PARM_RETURN 7

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO440_Rechercher_tous_les_PMVA_amont (XDY_Autoroute	,
						XDY_Sens	,
						XDY_PR		,
						XDY_PR		,
						XDY_District	,
						XDY_Entier	,
						XDY_Horodate	,
						char		*,
						XDY_Octet	,
						XDY_FonctionInt ,
						XDY_Entier 	* );

#endif
