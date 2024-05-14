/*E*/
/*Fichier : $Id: xzao204.h,v 1.2 1997/02/06 09:19:52 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/02/06 09:19:52 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao204.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* niepceron	16 Jan 1997	: Creation dem/1373
		06 Fev 1997	: Ajout champs Inverse dem/1373 1.2
------------------------------------------------------ */

#ifndef xzao204
#define xzao204

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO204_RPC_NAME "XZAO;204"

#define XZAOC_XZAO204_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	NomFAV;
	XDY_Eqt		NumeroFAV;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Octet	NumeroAutoroute;
	XDY_Octet	Voie;
	XDY_Booleen	Inverse;

} XZAOT_FAVSyn;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO204_Liste_FAV_Synoptique (	XDY_Basedd,
						XDY_Nom,
						XDY_Booleen,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
