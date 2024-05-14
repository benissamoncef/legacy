/*E*/
/*  Fichier : $Id: xzao86.h,v 1.2 1995/03/10 15:53:18 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/03/10 15:53:18 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao86.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	22 Sep 1994	: Creation
* C.T.		10 Mar 1995	: Modif structure (V 1.2)
------------------------------------------------------ */

#ifndef xzao86
#define xzao86

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO86_RPC_NAME "XZAO;86"

#define XZAOC_XZAO86_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_Octet	Numero;
   	XDY_Nom		NomAire;
   	XDY_Abrev	Abreviation;
   	XDY_NomAuto	Autoroute;
   	XDY_Sens	Sens;
   	XDY_PR		PRentree;
	XDY_PR		PRsortie;
	XDY_Booleen	Creation;
	XDY_Mot		CapaciteVL;
	XDY_Mot		CapacitePL;
	
} XZAOT_ConfAire;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO86_Ajouter_Aire (	XDY_Basedd,
					XZAOT_ConfAire,
					XDY_ResConf *);



#endif
