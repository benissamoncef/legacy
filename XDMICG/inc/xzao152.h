/*E*/
/*Fichier : $Id: xzao152.h,v 1.3 1994/11/29 10:26:24 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1994/11/29 10:26:24 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao152.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	16 Oct 1994	: Modif XDY_FocntionInt
------------------------------------------------------ */

#ifndef xzao152
#define xzao152

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO152_RPC_NAME "XZAO;152"

#define XZAOC_XZAO152_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Autoroute	NumeroAutoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;

} XZAOT_Portail;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO152_Liste_Portails (	XDY_Basedd,
					XDY_Nom,
					XDY_Mot,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
