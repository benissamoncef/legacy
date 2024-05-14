/*E*/
/* Fichier : $Id: xzao180.h,v 1.4 2018/11/06 21:38:11 devgfi Exp $        $Revision: 1.4 $        $Date: 2018/11/06 21:38:11 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao180.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Sep 1994	: Creation
* mismer        24 Avr 1997     : Ajout colonne alerte + type gabarit,vitesse et niche (v1.2)
* JPL		26/10/18 : Ajout indicateur de suppression (DEM 1305)  1.3
* JPL		06/11/18 : Procédure dégroupée  1.4
------------------------------------------------------ */

#ifndef xzao180
#define xzao180

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO180_RPC_NAME "XZAO180"

#define XZAOC_XZAO180_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	NomCapteur;
	XDY_NomAuto	Autoroute;
	XDY_NomEqt	NomTube;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Octet	Type;
	XDY_Octet	NumBit;
	XDY_Octet	NumAlerte;
	XDY_Booleen	Supprime;
	
} XZAOT_ConfCaptTun;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO180_Ajouter_Capteur_Tunnel (	XDY_Basedd,
						XZAOT_ConfCaptTun,
						XDY_ResConf *);



#endif
