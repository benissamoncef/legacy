/*E*/
/*Fichier : $Id: xzao126.h,v 1.2 1994/10/20 11:31:03 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/10/20 11:31:03 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao126.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
------------------------------------------------------ */

#ifndef xzao126
#define xzao126

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO126_RPC_NAME "XZAO;126"

#define XZAOC_XZAO126_NB_PARM_RETURN 0

/* definitions de types exportes */
typedef struct {
	
	XDY_Mot		NumeroPortion;
	XDY_Autoroute	NumeroAutoroute;
	XDY_PR		PRdebut;
	XDY_PR		PRfin;
	XDY_Booleen	DistrictRecouvrement;
	XDY_Booleen	Bifurcation;
	XDY_PR		PRBifurcation;

} XZAOT_Portion;


/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO126_Liste_Portions_Troncon (	XDY_Basedd,
						XDY_Nom,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
