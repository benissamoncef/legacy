/*E*/
/* Fichier : $Id: xzao67.h,v 1.4 2019/01/14 13:35:33 pc2dpdy Exp $        $Revision: 1.4 $        $Date: 2019/01/14 13:35:33 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao67.h
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
* JPL	19/10/18 : Ajout indicateur de suppression (DEM 1305)  1.3
* JMG	31/10/18 : ajout IP DEM1306 1.4
------------------------------------------------------ */

#ifndef xzao67
#define xzao67

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO67_RPC_NAME "XZAO67"

#define XZAOC_XZAO67_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_NomEqt	NomStation;
   	XDY_NomAuto	NomAutoroute;
   	XDY_PR		PR;
   	XDY_Sens	Sens;
   	XDY_NomEqt	NomServeur;
   	XDY_Octet	Liaison;
   	XDY_Rgs		RGS;
   	XDY_NomFich	Seuils;
   	XDY_NomFich	Alertes;
   	XDY_Booleen	DonneesIndividuelles;
	XDY_NomSite	NomSite;
   	XDY_Booleen	Supprime;
	XDY_Octet	Type;
	XDY_AdresseIP	AdresseIP;
	XDY_PortIP	Port;
	XDY_Nom		IdLCR;
} XZAOT_ConfRADT;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO67_Ajouter_Station_RADT (XDY_Basedd,
					XZAOT_ConfRADT,
					XDY_ResConf *);

#endif
