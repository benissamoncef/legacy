/*E*/
/*  Fichier : $Id: xzao176.h,v 1.1 1994/10/05 19:12:54 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:12:54 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao176.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Sep 1994	: Creation
------------------------------------------------------ */

#ifndef xzao176
#define xzao176

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO176_RPC_NAME "XZAO;176"

#define XZAOC_XZAO176_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	NomDemiEch;
	XDY_NomAuto	Autoroute;
	XDY_NomEqt	NomServeur;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Mot		AdresseEtat;
	XDY_Mot		AdresseCommande;
	XDY_Mot		AdresseDiscordance;
	XDY_Mot		AdresseTelealarme;
	
} XZAOT_ConfDemiEch;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO176_Ajouter_Demi_Ech (	XDY_Basedd,
					XZAOT_ConfDemiEch,
					XDY_ResConf *);



#endif
