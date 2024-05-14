/*E*/
/*Fichier : $Id: xzao780.h,v 1.4 2018/10/25 10:53:08 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2018/10/25 10:53:08 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao780.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	09/06/05: Creation
* JMG   31/10/07        : ajout site de gestion  1.2
* LCL	02/10/12	: Suppression SAGA DEM1306 1.3
------------------------------------------------------ */

#ifndef xzao780
#define xzao780

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO780_RPC_NAME "XZAO780"

#define XZAOC_XZAO780_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	XDY_Eqt		NumeroPMVA;
	XDY_NomEqt	NomServeur;
	XDY_Octet	Liaison;
	XDY_Rgs		RGS;
	XDY_Octet	NumeroTypePMVA;
	XDY_District	SiteGestion;
	XDY_AdresseIP	AdresseIP;
	XDY_PortIP	Port;
	XDY_Octet	TypeLCR;
	char		IdLCR[50];
} XZAOT_PMVA;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO780_Lire_Liste_PMVA (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
