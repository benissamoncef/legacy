/*E*/
/* Fichier : $Id: xzao182.h,v 1.3 2018/11/07 10:00:25 devgfi Exp $        $Revision: 1.3 $        $Date: 2018/11/07 10:00:25 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao182.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Sep 1994	: Creation
* JPL		26/10/18 : Ajout indicateur de suppression (DEM 1305)  1.2
* JPL		07/11/18 : Procédure dégroupée  1.3
------------------------------------------------------ */

#ifndef xzao182
#define xzao182

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO182_RPC_NAME "XZAO182"

#define XZAOC_XZAO182_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	NomFAV;
	XDY_NomAuto	Autoroute;
	XDY_NomEqt	NomTube;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Voie	Voie;
	XDY_Octet	RangComm;
	XDY_Octet	RangComm2;
	XDY_Octet	RangEtat;
	XDY_Octet	RangEtat2;
	XDY_Octet	BitDiscordance;
	XDY_Octet	BitDiscordance2;
	XDY_Booleen	Supprime;
	
} XZAOT_ConfFAV;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO182_Ajouter_FAV (	XDY_Basedd,
					XZAOT_ConfFAV,
					XDY_ResConf *);



#endif
