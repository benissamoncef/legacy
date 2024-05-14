/*E*/
/*Fichier : $Id: xzao251.h,v 1.1 2012/07/04 10:57:45 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:57:45 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao251.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  VR	21/12/11	: Creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzao251
#define xzao251

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO251_RPC_NAME "XZAO251"

#define XZAOC_XZAO251_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	NomPMV;
	XDY_Eqt		NumeroPMV;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Octet	NumeroAutoroute;
} XZAOT_PRVSyn;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO251_Liste_PRV_Synoptique (	XDY_Basedd,
						XDY_Nom,
						XDY_Mot,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
