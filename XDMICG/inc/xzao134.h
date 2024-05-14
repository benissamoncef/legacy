/*E*/
/*Fichier : $Id: xzao134.h,v 1.3 1994/11/29 10:26:21 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1994/11/29 10:26:21 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao134.h
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
* volcic	29 Nov 1994	: Ajout parametre d'entree (v1.3)
------------------------------------------------------ */

#ifndef xzao134
#define xzao134

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO134_RPC_NAME "XZAO;134"

#define XZAOC_XZAO134_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_PR		PR;									
	XDY_Sens	Sens;
	XDY_Octet	Vitesse;
	XDY_Octet	NumeroAutoroute;

} XZAOT_PnPol;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO134_Liste_Panneaux_Police (	XDY_Basedd,
						XDY_Nom,
						XDY_Mot,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
