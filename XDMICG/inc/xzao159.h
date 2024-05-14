/*E*/
/*  Fichier : $Id: xzao159.h,v 1.1 1994/10/05 19:12:40 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:12:40 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao159.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Sep 1994	: Creation
------------------------------------------------------ */

#ifndef xzao159
#define xzao159

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO159_RPC_NAME "XZAO;159"

#define XZAOC_XZAO159_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Scenario	Scenario;
	XDY_Nom		NomType;
	XDY_Booleen	AutoFichier;
	XDY_Booleen	Creation;
	
} XZAOT_ConfScenCn;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO159_Ajouter_Scen_Connu (	XDY_Basedd,
					XZAOT_ConfScenCn,
					XDY_ResConf *);



#endif
