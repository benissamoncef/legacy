/*E*/
/*  Fichier : $Id: xzao166.h,v 1.2 1997/05/06 19:33:40 mismer Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/05/06 19:33:40 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao166.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Sep 1994	: Creation
* mismer        24 Avr 1997     : Ajout colonne alerte  (v1.2)
------------------------------------------------------ */

#ifndef xzao166
#define xzao166

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO166_RPC_NAME "XZAO;166"

#define XZAOC_XZAO166_NB_PARM_RETURN 1

/* definitions de types exportes */


typedef struct {
	
	XDY_NomEqt	Nom;
	XDY_NomMachine	NomMachine;
	XDY_Mot		LongueurITOR;
	XDY_Mot		AdresseITOR;
	XDY_Mot		LongueurETOR;
	XDY_Mot		AdresseETOR;
	XDY_Mot		LongTelealarme;
	XDY_Mot		AdresseTelealarme;
	XDY_Mot		LongTelecomEch;
	XDY_Mot		AdresseTelecomEch;
	XDY_Mot		LongTelecomTub;
	XDY_Mot		AdresseTelecomTub;
	XDY_Mot		LongTelesignEch;
	XDY_Mot		AdresseTelesignEch;
	XDY_Mot		LongTelesignTub;
	XDY_Mot		AdresseTelesignTub;
	XDY_Mot		LongSeqEch;
	XDY_Mot		AdresseSeqEch;
	XDY_Mot		LongSeqTub;
	XDY_Mot		AdresseSeqTub;
	XDY_Mot		LongAlerteTub;
	XDY_Mot		AdresseAlerteTub;
	
} XZAOT_ConfGTCNICE;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO166_Ajouter_GTC_NICE (	XDY_Basedd,
					XZAOT_ConfGTCNICE,
					XDY_ResConf *);



#endif
