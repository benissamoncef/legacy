/*E*/
/*Fichier : $Id: xzao76.h,v 1.6 2019/01/07 11:48:57 pc2dpdy Exp $	Release : $Revision: 1.6 $        Date : $Date: 2019/01/07 11:48:57 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao76.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	14 Oct 1994	: Modif XDY_FonctionInt (v1.2)
* volcic        18 Sep 1994     : Modification Structure RADT (v1.3)
* JMG   31/10/07        : ajout site de gestion  1.4
* JMG	31/01/18	: ajout IP et suppression ";" DEM1306 1.6
------------------------------------------------------ */

#ifndef xzao76
#define xzao76

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO76_RPC_NAME "XZAO76"

#define XZAOC_XZAO76_NB_PARM_RETURN 0

/* definitions de types exportes */
   	
typedef struct {

   	XDY_Eqt		NumeroStation;
   	XDY_NomEqt	NomServeur;
   	XDY_Octet	Liaison;
   	XDY_Rgs		RGS;
   	XDY_NomFich	Seuils;
   	XDY_NomFich	Alertes;
   	XDY_Booleen	DonneesIndividuelles;
	XDY_Eqt		NumStationAmontS1;
   	XDY_Eqt		NumStationAvalS1;
	XDY_Eqt		NumStationAmontS2;
   	XDY_Eqt		NumStationAvalS2;
	XDY_District	SiteGestion;   	
	XDY_AdresseIP	AdresseIP;
	XDY_PortIP	Port;
	XDY_Octet	Type;
	char		IdLCR[10];
} XZAOT_RADT;

typedef struct {

   	XDY_Eqt		NumeroStation;  	
   	XDY_Sens	Sens;
   	XDY_Octet	Boucle;
   	XDY_Octet	BoucleInv;
   	XDY_Octet	Voie;
	XDY_Booleen	Poids;

} XZAOT_PtMes;

typedef struct {

   	XDY_Eqt		NumeroStation;  	
   	XDY_Sens	Sens;
   	XDY_Sens	SensVoie[5];

} XZAOT_Chaussee;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO76_Lire_Liste_RADT (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
