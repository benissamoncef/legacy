/*E*/
/* Fichier : $Id: xzao144.h,v 1.4 2019/05/21 11:05:48 pc2dpdy Exp $        $Revision: 1.4 $        $Date: 2019/05/21 11:05:48 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao144.h
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
* JPL		06/11/18 : Procédure dégroupée  1.3
* JMG		06/05/19 : ajout type DEM1334 1.4
------------------------------------------------------ */

#ifndef xzao144
#define xzao144

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO144_RPC_NAME "XZAO144"

#define XZAOC_XZAO144_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_NomEqt	Nom;
   	XDY_NomEqt	NomPIRAU;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PR;
   	XDY_Sens	Sens;
   	XDY_CodeEqt	CodeAntenne;
   	XDY_CodeEqt	CodePAU;
	XDY_Booleen	Supprime;
   	XDY_Octet	Type;	
} XZAOT_ConfPAU;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO144_Ajouter_PAU (	XDY_Basedd,
					XZAOT_ConfPAU,
					XDY_ResConf *);


#endif
