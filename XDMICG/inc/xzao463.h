/*E*/
/*Fichier : @(#)xzao463.h	1.2      Release : 1.2        Date : 15/11/11
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao463.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	15/11/11: Creation (DEM/1014)
------------------------------------------------------ */

#ifndef xzao463
#define xzao463

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO463_RPC_NAME "XZAO4630"

#define XZAOC_XZAO463_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	XDY_Eqt		NumeroPMV;
	XDY_Eqt		NumeroDestProche;
	XDY_Nom		NomDestProche;
	XDY_Eqt		NumeroDestLointain;
	XDY_Nom		NomDestLointain;
	XDY_Eqt		NumeroDestNoeud;
	XDY_Nom		NomDestNoeud;
	
} XZAOT_PMV_DEST_TDP;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO463_Lire_Liste_PMV_DEST(	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
