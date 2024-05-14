/*E*/
/*Fichier : $Id: xzao132.h,v 1.3 1994/11/29 10:26:19 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1994/11/29 10:26:19 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao132.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	29 Nov 1994	: Ajout parametre d'entree (v1.3)
------------------------------------------------------ */

#ifndef xzao132
#define xzao132

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO132_RPC_NAME "XZAO;132"

#define XZAOC_XZAO132_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Mot		NumeroReseau;
	XDY_Nom		NomReseau;
	XDY_PR		PR;
	XDY_Octet	NumeroAutoroute;
	XDY_Octet	TypeReseau;

} XZAOT_Reseau;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO132_Liste_Reseaux (	XDY_Basedd,
					XDY_Nom,
					XDY_Mot,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
