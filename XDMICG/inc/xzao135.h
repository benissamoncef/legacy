/*E*/
/*Fichier : $Id: xzao135.h,v 1.4 1994/11/29 10:26:23 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1994/11/29 10:26:23 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao135.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	13 Oct 1994	: Modification nom fonction (v 1.2)
* volcic	29 Nov 1994	: Ajout parametre d'entree (v 1.4)
------------------------------------------------------ */

#ifndef xzao135
#define xzao135

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO135_RPC_NAME "XZAO;135"

#define XZAOC_XZAO135_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Mot		NumeroPassage;
	XDY_Nom		NomPassage;
	XDY_PR		PR;
	XDY_Octet	NumeroAutoroute;
	XDY_Mot		GabaritSens1;
	XDY_Mot		GabaritSens2;

} XZAOT_Passage;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO135_Lire_Passages_Superieurs (	XDY_Basedd,
						XDY_Nom,
				  		XDY_Mot,
				  		XDY_FonctionInt,
						XDY_Entier * );

#endif
