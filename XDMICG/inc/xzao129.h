/*E*/
/*Fichier : $Id: xzao129.h,v 1.3 1994/11/29 10:46:50 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1994/11/29 10:46:50 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao129.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
------------------------------------------------------ */

#ifndef xzao129
#define xzao129

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO129_RPC_NAME "XZAO;129"

#define XZAOC_XZAO129_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Octet	NumeroPeage;
	XDY_Nom		NomPeage;
	XDY_PR		PRsens1;
	XDY_Mot		LongueurSens1;
	XDY_PR		PRsens2;
	XDY_Mot		LongueurSens2;
	XDY_Octet	NumeroAutoroute;

} XZAOT_Peage;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO129_Liste_Peages (	XDY_Basedd,
					XDY_Nom,
					XDY_Mot,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
