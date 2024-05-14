/*E*/
/*  Fichier : $Id: xzao121.h,v 1.2 1994/10/20 11:30:58 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/10/20 11:30:58 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao121.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	22 Sep 2194	: Creation
------------------------------------------------------ */

#ifndef xzao121
#define xzao121

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO121_RPC_NAME "XZAO;121"

#define XZAOC_XZAO121_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		NomPortion;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Octet	Vitesse;
	XDY_Booleen	Creation;
	
} XZAOT_ConfPnPol;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO121_Ajouter_Panneau_Police (	XDY_Basedd,
						XZAOT_ConfPnPol,
						XDY_ResConf *);



#endif
