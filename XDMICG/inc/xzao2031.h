/*E*/
/*Fichier :  $Id: xzao2031.h,v 1.1 2008/02/13 13:24:29 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:29 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao0203.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ	08 Nov 2007	: Creation (copie xzao203) DEM 662
------------------------------------------------------ */

#ifndef xzao2031
#define xzao2031

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO2031_RPC_NAME "XZAO2031"

#define XZAOC_XZAO2031_NB_PARM_RETURN 36

/* definitions de types exportes */

typedef struct {
    XDY_Eqt		Numero;
	XDY_NomEqt	NomServeur;
		
} XZAOT_GTC_NICE_RENOVEE;


typedef struct {
	
	XDY_NomEqt	 Ouvrage;
	XDY_NomEqt  NomOuvrage;
	
} XZAOT_OUV;		

typedef struct {
	
	XDY_NomEqt	   Domaine;
	XDY_Octet      Type;
	XDY_Eqt		   Numero;
	XDY_Etat_Eqt   Etat;
	
} XZAOT_DOM;


/* AAZ 17/10/2007 */

typedef struct {
    
	XDY_NomEqt	EQT;
	XDY_TypeEqt	TypeMaitre;
	XDY_Eqt		Maitre;
	XDY_TypeEqt	Type;
	XDY_Eqt		Numero;
	XDY_NomEqt	Domaine;
	
} XZAOT_EQT;		

typedef struct {
	
	XDY_NomEqt	Domaine;
	XDY_Octet   TypeEquipement;
	XDY_Eqt		Numero;
	XDY_Octet	NumSequence;
	
} XZAOT_SEQ;


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO2031_Liste_Eqt_GTC_NICE_RENOVEE (  XDY_Basedd,
                                                  XDY_NomEqt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XDY_FonctionInt,
                                                  XZAOT_GTC_NICE_RENOVEE *);

#endif
