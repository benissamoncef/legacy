/*E*/
/*  Fichier : $Id: xzao84.h,v 1.3 2012/06/20 10:22:42 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2012/06/20 10:22:42 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao84.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	22 Sep 1994	: Creation  1.1, 1.2
* JPL		14/06/2012	: Ajout nom sortie et indic. bifurcation; proc. stockée dégroupée (DEM 1033)  1.3
------------------------------------------------------ */

#ifndef xzao84
#define xzao84

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO84_RPC_NAME "XZAO84"

#define XZAOC_XZAO84_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_Octet	Numero;
   	XDY_Nom		NomEchangeur;
   	XDY_Abrev	Abreviation;
   	XDY_Sortie	NomSortie;
   	XDY_Booleen	Bifurcation;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PR;	
	XDY_PR		PRentreeSens1;	
	XDY_Octet	VoiesEntreeSens1;
	XDY_Booleen	PeageEntreeSens1;
	XDY_PR		PRentreeSens2;	
	XDY_Octet	VoiesEntreeSens2;
	XDY_Booleen	PeageEntreeSens2;
	XDY_PR		PRsortieSens1;	
	XDY_Octet	VoiesSortieSens1;
	XDY_Booleen	PeageSortieSens1;
	XDY_PR		PRsortieSens2;	
	XDY_Octet	VoiesSortieSens2;
	XDY_Booleen	PeageSortieSens2;
	XDY_Booleen	Creation;
	
} XZAOT_ConfEchang;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO84_Ajouter_Echangeur (	XDY_Basedd,
					XZAOT_ConfEchang,
					XDY_ResConf *);

#endif
