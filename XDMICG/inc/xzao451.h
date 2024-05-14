/*E*/
/* Fichier : $Id: xzao451.h,v 1.3 2018/11/05 12:58:10 devgfi Exp $        $Revision: 1.3 $        $Date: 2018/11/05 12:58:10 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao451.h
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
* JPL	26/10/18	: Ajout indicateur de suppression (DEM 1305)  1.3
------------------------------------------------------ */

#ifndef xzao451
#define xzao451

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO451_RPC_NAME "XZAO451"

#define XZAOC_XZAO451_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	NomMod;
	char		Code[3];
	XDY_NomAuto	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	

} XZAOT_ConfModBAD;

typedef struct {

        XDY_Eqt         NumeroMod;
        XDY_Eqt         NumeroBAD;
        char            Code[3];

} XZAOT_ModuleBAD;

typedef struct {
	
	XDY_NomEqt	NomBAD;
	XDY_NomAuto	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_NomEqt	NomServeur;
	XDY_Octet	Liaison;
	XDY_Rgs		Rgs;
	XDY_Octet	Type;
/*	XDY_PR		PRinfluence;*/
	XZAOT_ConfModBAD Modules[10];
	
} XZAOT_ConfBAD;

typedef struct {
	
	XDY_NomEqt	NomBAD;
	XDY_NomAuto	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_NomEqt	NomServeur;
	XDY_Octet	Liaison;
	XDY_Rgs		Rgs;
	XDY_Octet		Type;
	/*XDY_PR		PRinfluence;*/
	XDY_NomEqt	NomMod;
	char		CodeMod[3];
	XDY_NomAuto	AutoMod;
	XDY_PR		PRMod;
	XDY_Sens	SensMod;
/*	XDY_Octet	TypeBAD;*/
	XDY_NomSite	NomSite;	
	XDY_Booleen	Supprime;
} XZAOT_AjoutBAD;


typedef struct {

        XDY_Eqt         NumeroBAD;
        XDY_NomEqt      NomServeur;
        XDY_Octet       Liaison;
        XDY_Rgs         Rgs;
        XDY_Octet       Type;
	XDY_District	SiteGestion;
} XZAOT_BAD;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO451_Ajouter_BAD (XDY_Basedd,
				XZAOT_AjoutBAD,
				XDY_ResConf *);



#endif
