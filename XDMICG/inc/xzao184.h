/*E*/
/* Fichier : $Id: xzao184.h,v 1.4 2018/11/07 17:25:16 devgfi Exp $        $Revision: 1.4 $        $Date: 2018/11/07 17:25:16 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao184.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic	27 Sep 1994	: Creation
* Mismer        05 Fev 1997     : Ajout colonne voie et inverse (v1.2)
* JPL		26/10/18	: Ajout indicateur de suppression (DEM 1305)  1.3
* JPL		07/11/18	: Procédure dégroupée  1.4
------------------------------------------------------ */

#ifndef xzao184
#define xzao184

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO184_RPC_NAME "XZAO184"

#define XZAOC_XZAO184_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	NomPnGTC;
	XDY_NomAuto	Autoroute;
	XDY_NomEqt	NomMaitre;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Voie	Voie;
	XDY_Octet	Inverse;
	XDY_Octet	RangComm;
	XDY_Octet	RangComm2;
	XDY_Octet	RangEtat;
	XDY_Octet	RangEtat2;
	XDY_Octet	BitDiscordance;
	XDY_Octet	BitDiscordance2;
	XDY_Booleen	Supprime;
	
} XZAOT_ConfPnGTC;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO184_Ajouter_PnGTC (	XDY_Basedd,
					XZAOT_ConfPnGTC,
					XDY_ResConf *);



#endif
