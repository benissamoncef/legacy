/*E*/
/*Fichier :  @(#)xzao551.h	1.1      Release : 1.1        Date : 02/02/12
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao551.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       02 Fev 2012     : Creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzao551
#define xzao551

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO551_RPC_NAME "XZAO551"

#define XZAOC_XZAO551_NB_PARM_RETURN 1 

/* definitions de types exportes */

typedef struct {
	XDY_Eqt prv;
	XDY_Eqt scenario;
	XDY_Eqt commande;
	XDY_Booleen prv_critique;
	XDY_Octet	autoroute;
	XDY_Entier	PR;
	XDY_Octet	sens;
} XZAOT_Commandes_Scenarios;


/* delaration de fonctions */

extern int XZAO551_Commandes_Scenarios (	XDY_Basedd,
				XDY_Entier,
				XDY_FonctionInt,
				XDY_Entier*);

#endif
