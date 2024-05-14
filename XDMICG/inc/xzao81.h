/*E*/
/* Fichier : $Id: xzao81.h,v 1.4 2018/11/06 19:06:07 devgfi Exp $        $Revision: 1.4 $        $Date: 2018/11/06 19:06:07 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao81.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	22 Sep 1994	: Creation
* JMG   31/10/07        : ajout site de gestion
* JPL	26/10/18 : Ajout indicateur de suppression (DEM 1305)  1.3
* JPL	06/11/18 : Procédure dégroupée  1.4
------------------------------------------------------ */

#ifndef xzao81
#define xzao81

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO81_RPC_NAME "XZAO81"

#define XZAOC_XZAO81_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_NomEqt	NomAnalyseur;
   	XDY_NomEqt	NomDAI;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PRdebut;   	  	 	
   	XDY_PR		PRfin;	
	XDY_Sens	Sens;
	XDY_Octet	NumeroAnalyseur;
	XDY_Octet	VoiesSurveillees;
	XDY_NomSite	NomSite;
	XDY_Booleen	Supprime;
} XZAOT_ConfAnal;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO81_Ajouter_Analyseur (	XDY_Basedd,
					XZAOT_ConfAnal,
					XDY_ResConf *);



#endif
