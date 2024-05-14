/*E*/
/*  Fichier : $Id: xzao453.h,v 1.2 2007/11/19 17:10:37 pc2dpdy Exp $     Release : $Revision: 1.2 $        Date : $Date: 2007/11/19 17:10:37 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao453.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	: Creation
* JMG   31/10/07        : ajout site de gestion 
------------------------------------------------------ */

#ifndef xzao453
#define xzao453

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO453_RPC_NAME "XZAO453"

#define XZAOC_XZAO453_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Eqt		NumeroMod;
	XDY_Eqt		NumeroBAD;
	char		Code[3];

} XZAOT_ModuleBAD;

typedef struct {
	
	XDY_Eqt		NumeroBAD;
	XDY_NomEqt	NomServeur;
	XDY_Octet	Liaison;
	XDY_Rgs		Rgs;
	XDY_Octet	Type;
	XDY_District	SiteGestion;
} XZAOT_BAD;


/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO453_Liste_BAD_Machine (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
