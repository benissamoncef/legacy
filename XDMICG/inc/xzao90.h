/*E*/
/*  Fichier : $Id: xzao90.h,v 1.1 1994/10/05 19:14:00 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:14:00 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao90.h
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

#ifndef xzao90
#define xzao90

/* definitions de constantes exportees */

#define XZAOC_XZAO90_RPC_NAME "XZAO;90"

#define XZAOC_XZAO90_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_Nom		NomPS;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PR;
	XDY_Mot		GabaritSens1;
	XDY_Mot		GabaritSens2;
	XDY_Booleen	Creation;
	
} XZAOT_ConfPS;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO90_Ajouter_PS (	XDY_Basedd,
				XZAOT_ConfPS,
				XDY_ResConf *);




#endif
