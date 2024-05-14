/*E*/
/*Fichier : $Id: xzao78.h,v 1.6 2018/10/25 10:53:03 pc2dpdy Exp $	Release : $Revision: 1.6 $        Date : $Date: 2018/10/25 10:53:03 $------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao78.h
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
* gaborit	25 jan 1995	: modif entete
* JMG   31/10/07        : ajout site de gestion  1.4
* JMG	27/08/18	: ajout adresse_ip; port suppression SAGA DEM1306 1.5
------------------------------------------------------ */

#ifndef xzao78
#define xzao78

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO78_RPC_NAME "XZAO78"

#define XZAOC_XZAO78_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Eqt		NumeroPMV;
	XDY_NomEqt	NomServeur;
	XDY_Octet	Liaison;
	XDY_Rgs		RGS;
	XDY_Octet	NumeroTypePMV;
	XDY_District	SiteGestion;
	XDY_AdresseIP   AdresseIP;
	XDY_PortIP	Port;
	XDY_Octet	TypeLCR;
	char		IdLCR[50];
} XZAOT_PMV;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO78_Lire_Liste_PMV (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
