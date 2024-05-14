/*E*/
/*  Fichier : $Id: xzao88.h,v 1.1 1994/10/05 19:13:58 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:13:58 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao88.h
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

#ifndef xzao88
#define xzao88

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO88_RPC_NAME "XZAO;88"

#define XZAOC_XZAO88_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_Octet	Numero;
   	XDY_Nom		NomPeage;
   	XDY_Abrev	Abreviation;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PRsens1;
	XDY_PR		PRsens2;
	XDY_Mot		LongueurSens1;
	XDY_Mot		LongueurSens2;
	XDY_Booleen	Creation;
	
} XZAOT_ConfPeage;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO88_Ajouter_Peage (	XDY_Basedd,
					XZAOT_ConfPeage,
					XDY_ResConf *);



#endif
