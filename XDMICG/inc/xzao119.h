/*E*/
/*  Fichier : $Id: xzao119.h,v 1.2 1994/10/20 11:30:55 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/10/20 11:30:55 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao119.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	22 Sep 1994	: Creation
------------------------------------------------------ */

#ifndef xzao119
#define xzao119

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO119_RPC_NAME "XZAO;119"

#define XZAOC_XZAO119_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		NomTroncon;		
	XDY_NomAuto	NomAutoroute;
	XDY_PR		PRdebut;
	XDY_PR		PRfin;
	XDY_Booleen	Creation;
	
} XZAOT_ConfTroncon;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO119_Ajouter_Troncon (	XDY_Basedd,
					XZAOT_ConfTroncon,
					XDY_ResConf *);



#endif
