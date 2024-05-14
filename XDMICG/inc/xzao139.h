/*E*/
/* Fichier : $Id: xzao139.h,v 1.6 2018/10/26 13:47:22 devgfi Exp $        $Revision: 1.6 $        $Date: 2018/10/26 13:47:22 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao139.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Sep 1994	: Creation
* PNI		06 Avr 2006 	: XDY_CodeEqt devient XDY_CodeCam v1.2 DEM565
* JPL		08/10/10  : Degroupee; plus de PR de debut et fin  (DEM 948)  1.3
* JPL		15/10/10  : Ajout de l'adresse IP  (DEM 954)  1.4
* JPL		14/12/10  : Ajout des champs Protocole, Port et Encoder  (DEM 954)  1.5
* JPL		26/10/18 : Ajout indicateur de suppression (DEM 1305)  1.6
------------------------------------------------------ */

#ifndef xzao139
#define xzao139

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO139_RPC_NAME "XZAO139"

#define XZAOC_XZAO139_NB_PARM_RETURN 1

/* definitions de types exportes */


typedef struct {

   	XDY_NomEqt	Nom;
   	XDY_CodeCam	Code;
   	XDY_NomEqt	NomLTVideo;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Booleen	Cycle;
	XDY_Booleen	Mobile;
	XDY_Booleen	Positionnable;
	XDY_AdresseIP	AdresseIP;
	XDY_Protocole	Protocole;
	XDY_Port	Port;
	XDY_Encoder	Encoder;
	XDY_Booleen	Supprime;

} XZAOT_ConfCamera;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO139_Ajouter_Camera (	XDY_Basedd,
					XZAOT_ConfCamera,
					XDY_ResConf *);



#endif
