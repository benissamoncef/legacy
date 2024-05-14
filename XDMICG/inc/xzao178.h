/*E*/
/*  Fichier : $Id: xzao178.h,v 1.2 1997/05/06 19:33:42 mismer Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/05/06 19:33:42 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao178.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Sep 1994	: Creation
* Mismer.D	30 Apr 1997	: Ajout alerte monaco (DEM/1421)
------------------------------------------------------ */

#ifndef xzao178
#define xzao178

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO178_RPC_NAME "XZAO;178"

#define XZAOC_XZAO178_NB_PARM_RETURN 1

/* definitions de types exportes */
	
typedef struct {
	
	XDY_NomEqt	NomTube;
	XDY_NomAuto	Autoroute;
	XDY_NomEqt	NomServeur;
	XDY_PR		PR;
	XDY_PR		PRInfluence;
	XDY_Sens	Sens;
	XDY_Mot		AdresseEtat;
	XDY_Mot		AdresseCommande;
	XDY_Mot		AdresseDiscord;
	XDY_Mot		AdresseExtincteur;
	XDY_Mot		AdresseLit;
	XDY_Mot		AdresseOPCO;
	XDY_Mot		AdresseTelealarme;
	XDY_Mot		AdresseGabarit;
	XDY_Mot		AdresseVitesse;
	XDY_Mot		AdresseNiche;
	
} XZAOT_ConfTube;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO178_Ajouter_Tube (	XDY_Basedd,
					XZAOT_ConfTube,
					XDY_ResConf *);



#endif
