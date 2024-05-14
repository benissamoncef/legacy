/*E*/
/*  Fichier : $Id: xzao63.h,v 1.2 1994/12/15 15:23:29 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/12/15 15:23:29 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao63.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	22 Sep 1994	: Creation
* volcic	29 Nov 1994	: Modif XZAOT_Influence (v1.2)
------------------------------------------------------ */

#ifndef xzao63
#define xzao63

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO63_RPC_NAME "XZAO;63"

#define XZAOC_XZAO63_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_NomAuto	Autoroute;
   	XDY_NomEqt	NomStation;
   	XDY_Sens	Sens;
   	XDY_PR		PRdebut;
   	XDY_PR		PRfin;
   	XDY_Booleen	Creation;

} XZAOT_ConfInfluence;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO63_Ajouter_Influence_Station ( 	XDY_Basedd,
						XZAOT_ConfInfluence,
						XDY_ResConf *);


#endif
