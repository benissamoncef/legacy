/*E*/
/*Fichier : $Id: xzao124.h,v 1.4 1996/09/03 11:55:17 verdier Exp $      Release : $Revision: 1.4 $        Date : $Date: 1996/09/03 11:55:17 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao124.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* gaborit	25 jan 1995	: modif entete
------------------------------------------------------ */

#ifndef xzao124
#define xzao124

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO124_RPC_NAME "XZAO;124"

#define XZAOC_XZAO124_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	NomPMV;
	XDY_Eqt		NumeroPMV;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Octet	NumeroAutoroute;
	XDY_Octet	Lignes;
	XDY_Nom		NomTypePicto;

} XZAOT_PMVSyn;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO124_Liste_PMV_Synoptique (	XDY_Basedd,
						XDY_Nom,
						XDY_Mot,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
