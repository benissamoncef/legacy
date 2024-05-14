/*E*/
/*Fichier : $Id: xzao163.h,v 1.5 2007/11/19 17:10:36 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2007/11/19 17:10:36 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao163.h
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
* volcic	08 Dec 1994	: Modif Code (v1.3)
* Mismer	20 Jan 1998	: Ajout type equipement  (v1.4)
* JMG   31/10/07        : ajout site de gestion  1.5
------------------------------------------------------ */

#ifndef xzao163
#define xzao163

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO163_RPC_NAME "XZAO;163"

#define XZAOC_XZAO163_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Eqt		NumeroMod;
	XDY_Eqt		NumeroNAV;
	char		Code[3];

} XZAOT_Module;

typedef struct {
	
	XDY_Eqt		NumeroNAV;
	XDY_Octet	NumeroType;
	XDY_NomEqt	NomServeur;
	XDY_Octet	Liaison;
	XDY_Rgs		Rgs;
	XDY_Octet	Type;
	XDY_District	SiteGestion;	
} XZAOT_NAV;


/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO163_Liste_NAV_Machine (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
