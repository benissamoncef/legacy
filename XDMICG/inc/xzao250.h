/*E*/
/*Fichier : 	$Id: xzao250.h,v 1.1 2005/09/13 18:11:14 gesconf Exp $  Release : $Revision: 1.1 $ Date : $Date: 2005/09/13 18:11:14 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao250.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	09/06/05: Creation
------------------------------------------------------ */

#ifndef xzao250
#define xzao250

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO250_RPC_NAME "XZAO250"

#define XZAOC_XZAO250_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	NomPMV;
	XDY_Eqt		NumeroPMV;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Octet	NumeroAutoroute;
} XZAOT_PMVASyn;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO250_Liste_PMVA_Synoptique (	XDY_Basedd,
						XDY_Nom,
						XDY_Mot,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
