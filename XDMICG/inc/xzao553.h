/*E*/
/* Fichier : $Id: xzao553.h,v 1.4 2018/11/05 13:06:24 devgfi Exp $        $Revision: 1.4 $        $Date: 2018/11/05 13:06:24 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao553.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       14 Nov 2011     : Creation (DEM/1014)
* JPL	19/07/16	: Nom correct de la fonction  1.3
* JPL	26/10/18	: Ajout indicateur de suppression (DEM 1305)  1.4
------------------------------------------------------ */

#ifndef xzao553
#define xzao553

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO553_RPC_NAME "XZAO553"

#define XZAOC_XZAO553_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	XDY_Eqt			Numero;
	XDY_Nom			Nom;
	XDY_NomAuto		Autoroute;
	XDY_Sens		Sens;
	XDY_PR			PR_Debut;
	XDY_PR			PR_Fin;
	XDY_Mot			Vitesse;
	XDY_Booleen		Inhibition;
	XDY_Booleen		Mode_Horaire;
   	XDY_NomEqt		NomServeur;
	XDY_Booleen		Supprime;
} XZAOT_ConfZoneReg;        

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO553_Ajouter_Zone_Reg ( XDY_Basedd,
                                        XZAOT_ConfZoneReg,
                                        XDY_ResConf *);

#endif
