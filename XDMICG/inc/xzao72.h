/*E*/
/* Fichier : $Id: xzao72.h,v 1.7 2018/11/06 17:37:10 devgfi Exp $        $Revision: 1.7 $        $Date: 2018/11/06 17:37:10 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao72.h
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
* JPL	19/07/16	: Nom correct de la fonction  1.4
* LCL 	03/09/18	: Suppression SAGA DEM1306 1.5
* JPL	26/10/18 : Ajout indicateur de suppression (DEM 1305)  1.7
------------------------------------------------------ */

#ifndef xzao72
#define xzao72

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO72_RPC_NAME "XZAO72"

#define XZAOC_XZAO72_NB_PARM_RETURN 1

/* definitions de types exportes */
   	
typedef struct {

   	XDY_NomEqt	NomPMV;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PR;
   	XDY_Sens	Sens;
   	XDY_NomEqt	NomServeur;
   	XDY_Octet	LiaisonPMV;
   	XDY_Rgs		RgsPMV;
   	XDY_Nom		TypePMV;
   	XDY_Octet	LiaisonPicto;
   	XDY_Rgs		RgsPicto;
   	XDY_Nom		TypePicto;
	XDY_NomSite	NomSite;   	
	XDY_AdresseIP	AdresseIP;
	XDY_PortIP	PortIP;
   	XDY_Octet	TypeLCR;
	char		IdLCR[50];
	XDY_Booleen	Supprime;
} XZAOT_ConfPMV;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO72_Ajouter_PMVPicto (	XDY_Basedd,
					XZAOT_ConfPMV,
					XDY_ResConf *);

#endif
