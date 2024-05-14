/*E*/
/*  Fichier : $Id: xzao69.h,v 1.1 1994/10/05 19:13:39 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:13:39 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao69.h
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

#ifndef xzao69
#define xzao69

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO69_RPC_NAME "XZAO;69"

#define XZAOC_XZAO69_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_NomEqt	NomStation;  	
   	XDY_Sens	Sens;
   	XDY_Octet	Boucle;
   	XDY_Octet	BoucleInv;
   	XDY_Octet	Voie;
	XDY_Booleen	Poids;
	XDY_Booleen	Creation;

} XZAOT_ConfPtMes;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO69_Ajouter_Pt_Mesure (	XDY_Basedd,
					XZAOT_ConfPtMes,
					XDY_ResConf *);


#endif
