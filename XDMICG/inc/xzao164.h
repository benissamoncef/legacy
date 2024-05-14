/*E*/
/*Fichier : $Id: xzao164.h,v 1.4 1994/11/18 15:49:33 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1994/11/18 15:49:33 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao164.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	16 Oct 1994	: Modif XDY_FonctionInt (v1.2)
* volcic	18 Oct 1994	: Suppr parametre XDY_NomEqt (v1.3)
* volcic	18 Nov 1994	: Ajout du type de scenario dans XZAOT_Scenario (v1.4)
------------------------------------------------------ */

#ifndef xzao164
#define xzao164

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO164_RPC_NAME "XZAO;164"

#define XZAOC_XZAO164_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Octet	NumeroScen;
	XDY_Booleen	AutoFichier;
	XDY_Scenario	Type;
	
} XZAOT_ScenConnu;

typedef struct {
	
	XDY_Octet	NumeroType;
	XDY_Booleen	FlashMod4;
	XDY_Octet	NbBarrieres;
	
} XZAOT_TypeNAV;


/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO164_Liste_Types_NAV (	XDY_Basedd,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
