/*E*/
/*Fichier : $Id: xzao130.h,v 1.2 1994/10/20 11:31:08 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/10/20 11:31:08 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao130.h
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

#ifndef xzao130
#define xzao130

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO130_RPC_NAME "XZAO;130"

#define XZAOC_XZAO130_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Mot		NumeroPortion;
	XDY_Mot		NumeroStation;
	XDY_Sens	Sens;
	XDY_PR		PRdebut;
	XDY_PR		PRfin;
	XDY_Octet	NumeroAutoroute;

} XZAOT_Influence;


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO130_Liste_Influences (	XDY_Basedd,
					XDY_Nom,
					XDY_Mot,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
