/*E*/
/*Fichier : $Id: xzao125.h,v 1.4 2007/11/19 17:10:35 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2007/11/19 17:10:35 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao125.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* JMG   31/10/07        : ajout site de gestion  1.2
------------------------------------------------------ */

#ifndef xzao125
#define xzao125

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO125_RPC_NAME "XZAO;125"

#define XZAOC_XZAO125_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Eqt		NumeroEqt;
	XDY_NomEqt	NomEqt;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Octet	NumeroAutoroute;
	XDY_District	SiteGestion;
} XZAOT_EqtSyn;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO125_Liste_Eqt_Synoptique (	XDY_Basedd,
						XDY_Nom,
						XDY_Mot,
						XDY_Octet,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
