/*E*/
/* Fichier : $Id: xzao450.h,v 1.4 2020/05/19 13:38:54 pc2dpdy Exp $        $Revision: 1.4 $        $Date: 2020/05/19 13:38:54 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao450.h
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
* LCL	11/06/19	: BAF/IP DEM1332 1.4
------------------------------------------------------ */

#ifndef xzao450
#define xzao450

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO450_RPC_NAME "XZAO450"

#define XZAOC_XZAO450_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	NomMod;
	char		Code[3];
	XDY_NomAuto	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	

} XZAOT_ConfModBAF;

typedef struct {
	
	XDY_NomEqt	NomBAF;
	XDY_NomAuto	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_NomEqt	NomServeur;
	XDY_Octet	Liaison;
	XDY_Rgs		Rgs;
	XDY_Octet	Type;
/*	XDY_PR		PRinfluence;*/
	XZAOT_ConfModBAF Modules[10];
        XDY_NomSite     NomSite;        
	
} XZAOT_ConfBAF;

typedef struct {
	
	XDY_NomEqt	NomBAF;
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
/*	XDY_Octet	TypeBAF;*/
        XDY_NomSite     NomSite;        
	XDY_Booleen	Supprime;
	XDY_AdresseIP	Adresse_IP;
	XDY_PortIP	Port;
	
} XZAOT_AjoutBAF;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO450_Ajouter_BAF (XDY_Basedd,
				XZAOT_AjoutBAF,
				XDY_ResConf *);



#endif
