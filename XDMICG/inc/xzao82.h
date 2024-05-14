/*E*/
/*Fichier : $Id: xzao82.h,v 1.7 2020/11/03 18:04:37 pc2dpdy Exp $	Release : $Revision: 1.7 $        Date : $Date: 2020/11/03 18:04:37 $------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao82.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	09 Dec 1994	: Modif pour Equext (v1.3)
* volcic	09 Dec 1994	: Modif entete (v1.4)
* JMG   31/10/07        : ajout site de gestion 1.5
* JMG	01/06/09	: ajout IP 1.6
* CGR	11/09/20	: ajout deuxieme adresse IP DEM-SAE175 1.7
------------------------------------------------------ */

#ifndef xzao82
#define xzao82

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO82_RPC_NAME "XZAO;82"

#define XZAOC_XZAO82_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {

   	XDY_Eqt		NumeroDAI;
   	XDY_NomEqt	NomServeur;
   	XDY_Rgs		Rgs; 
   	XDY_Octet	LiaisonMaitre;
   	XDY_Octet	LiaisonEsclave;
   	XDY_District	SiteGestion;	
	XDY_PortIP	Port;
	XDY_AdresseIP	AdresseIP;
	XDY_PortIP      PortE;
	XDY_AdresseIP   AdresseIPE;
	XDY_PortIP	Port2;
	XDY_AdresseIP	Adresse2IP;
	XDY_PortIP	Port2E;
	XDY_AdresseIP	Adresse2IPE;
} XZAOT_DAI;

typedef struct {

   	XDY_Eqt		NumeroDAI;
   	XDY_Octet	NumeroAnalyseur;
	XDY_Mot		NumeroEqt;
	XDY_Octet	NombreVoies;
        XDY_District	SiteGestion;
} XZAOT_Analyseur;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO82_Lire_Liste_DAI (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_Entier * );
#endif
