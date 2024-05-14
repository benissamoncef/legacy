/*E*/
/*Fichier : @(#)xzao409.h	1.2      Release : 1.2        Date : 04/20/99
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao409.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* mismer       	10 Fev 1997     : Creation (DEM/1395)
* niepceron	20 Avr 1999	: Modif pour animation zdp evol 15 dem/1730 v1.2

------------------------------------------------------ */

#ifndef xzao409
#define xzao409

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO409_RPC_NAME "XZAO409"

#define XZAOC_XZAO409_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	NomZDP;
	XDY_Eqt		NumeroZDP;
	XDY_PR		PR_Debut;
	XDY_PR		PR_Fin;
	XDY_Sens	Sens;
	XDY_Octet	NumeroAutoroute;
	XDY_Mot		SeuilBas;
	XDY_Mot		SeuilHaut;
	XDY_Mot		tdp_max;

} XZAOT_ZDPSyn;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO409_Liste_ZDP_Synoptique (	XDY_Basedd,
						XDY_Nom,
						XDY_Mot,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
