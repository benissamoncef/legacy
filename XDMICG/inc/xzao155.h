/*E*/
/*  Fichier : $Id: xzao155.h,v 1.1 1994/10/05 19:12:37 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:12:37 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao155.h
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

#ifndef xzao155
#define xzao155

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO155_RPC_NAME "XZAO;155"

#define XZAOC_XZAO155_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Scenario	Scenario;
	XDY_Nom		Nom;
	XDY_Booleen	Creation;
	
} XZAOT_ConfScenario;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO155_Ajouter_Scenario (	XDY_Basedd,
					XZAOT_ConfScenario,
					XDY_ResConf *);


#endif
