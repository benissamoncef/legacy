/*E*/
/*Fichier : $Id: xzao149.h,v 1.4 1995/03/24 17:56:09 niepceron Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/03/24 17:56:09 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao149.h
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
* volcic	28 Nov 1994	: Modif XZAOT_Echangeur (v1.3)
* Niepceron	24 Mar 1995	: Modif XZAOT_Echangeur (v1.4)
------------------------------------------------------ */

#ifndef xzao149
#define xzao149

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO149_RPC_NAME "XZAO;149"

#define XZAOC_XZAO149_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom_Ech		NomEchangeur;
	XDY_Octet	NumeroAutoroute;
	XDY_PR		PR;
	XDY_Booleen	Peage_Entree_Sens1;
	XDY_Entier	Existance_Entree_Sens1;
	XDY_Booleen	Peage_Entree_Sens2;
	XDY_Entier	Existance_Entree_Sens2;
	XDY_Booleen	Peage_Sortie_Sens1;
	XDY_Entier	Existance_Sortie_Sens1;
	XDY_Booleen	Peage_Sortie_Sens2;
	XDY_Entier	Existance_Sortie_Sens2;

} XZAOT_Echangeur;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO149_Liste_Echangeurs_Synoptique (XDY_Basedd,
						XDY_Mot,
						XDY_Nom,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
