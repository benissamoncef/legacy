/*E*/
/*Fichier : @(#)xzao116.h	1.1      Release : 1.1        Date : 10/11/94
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao116.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	14 Oct 1994	: Modif XDY_FonctionInt (v1.2)
* volcic	14 Oct 1994	: Modif XDY_NomEqt MachineOperateur (v1.3)
------------------------------------------------------ */

#ifndef xzao116
#define xzao116

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO116_RPC_NAME "XZAO;116"

#define XZAOC_XZAO116_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {

   	XDY_Eqt		Numero;
   	XDY_NomEqt	NomServeur;
   	XDY_Octet	Liaison;
   	XDY_Rgs		RGS;
   	XDY_Eqt		MachineOperateur;
   	XDY_Tel		NumeroRepondeur;
   	
} XZAOT_MAA;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO116_Lire_MAA (	XDY_Basedd,
				XDY_NomEqt,
				XDY_FonctionInt,
				XDY_Entier * );

#endif
