/*E*/
/*Fichier : $Id: xzao128.h,v 1.4 1994/11/28 16:45:48 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1994/11/28 16:45:48 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao128.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	28 Nov 1994	: Ajout parametre en entree (v1.4)
------------------------------------------------------ */

#ifndef xzao128
#define xzao128

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO128_RPC_NAME "XZAO;128"

#define XZAOC_XZAO128_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		NomAire;
	XDY_Octet	NumeroAire;
	XDY_Octet	NumeroAutoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Mot		Capacite_VL;
	XDY_Mot		Capacite_PL;

} XZAOT_Aire;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO128_Liste_Aires_Troncon (XDY_Basedd,
					XDY_Nom,
					XDY_Mot,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
