/*E*/
/* Fichier : $Id: xzao161.h,v 1.8 2018/11/06 20:54:03 devgfi Exp $        $Revision: 1.8 $        $Date: 2018/11/06 20:54:03 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao161.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Sep 1994	: Creation
* volcic	19 Oct 1994	: Modif ConfModNAV (v1.2)
* volcic	20 Oct 1994	: Suppr param sortie XDY_Mot (v1.3)
* volcic	14 Nov 1994	: Ajout PRinfluence (v1.4)
* C.T.		21 Jan 1998	: Ajout TypeNAV (1545) (v1.5)
* JMG   31/10/07        : ajout site de gestion 1.6
* JPL	26/10/18 : Ajout indicateur de suppression (DEM 1305)  1.7
* JPL	06/11/18 : Procédure dégroupée  1.8
------------------------------------------------------ */

#ifndef xzao161
#define xzao161

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO161_RPC_NAME "XZAO161"

#define XZAOC_XZAO161_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	NomMod;
	char		Code[3];
	XDY_NomAuto	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
        XDY_NomSite     NomSite;	

} XZAOT_ConfModNAV;

typedef struct {
	
	XDY_NomEqt	NomNAV;
	XDY_NomAuto	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_NomEqt	NomServeur;
	XDY_Octet	Liaison;
	XDY_Rgs		Rgs;
	XDY_Octet	Type;
	XDY_PR		PRinfluence;
	XZAOT_ConfModNAV Modules[22];
	
} XZAOT_ConfNAV;

typedef struct {
	
	XDY_NomEqt	NomNAV;
	XDY_NomAuto	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_NomEqt	NomServeur;
	XDY_Octet	Liaison;
	XDY_Rgs		Rgs;
	XDY_Nom		Type;
	XDY_PR		PRinfluence;
	XDY_NomEqt	NomMod;
	char		CodeMod[3];
	XDY_NomAuto	AutoMod;
	XDY_PR		PRMod;
	XDY_Sens	SensMod;
	XDY_Octet	TypeNAV;
	XDY_NomSite	NomSite;	
	XDY_Booleen	Supprime;
} XZAOT_AjoutNAV;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO161_Ajouter_NAV (XDY_Basedd,
				XZAOT_AjoutNAV,
				XDY_ResConf *);



#endif
