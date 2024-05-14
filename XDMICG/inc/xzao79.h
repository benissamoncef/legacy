/*E*/
/*Fichier : $Id: xzao79.h,v 1.6 2019/01/07 11:29:36 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2019/01/07 11:29:36 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao79.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	14 Oct 1994	: Modif XDY_FonctionInt
* gaborit	25 jan 1995	: modif entete
* JMG	31/10/07	: ajout site de gestion 
* JMG	02/09/18	: ajout IP suppression SAGA DEM1306 1.5
* JMG	04/11/18	: ajout typelcr 1.6 DEM1306
* GGY	15/06/23	: Correction type du port XDY_Port => XDY_PortIP dans la xzao79 (DEM471)
------------------------------------------------------ */

#ifndef xzao79
#define xzao79

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO79_RPC_NAME "XZAO79"

#define XZAOC_XZAO79_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Eqt			NumeroPicto;
	XDY_NomEqt		NomServeur;
	XDY_Octet		Liaison;
	XDY_Rgs			RGS;
	XDY_Octet		NumeroTypePicto;
	XDY_District	SiteGestion;	
	XDY_AdresseIP	AdresseIP;
	XDY_PortIP		Port;
	XDY_Octet		TypeLCR;
	char			IdLCR[20];
} XZAOT_Picto;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO79_Lire_Liste_Picto (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
