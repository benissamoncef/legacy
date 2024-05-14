/*E*/
/*Fichier :  $Id: xzao203.h,v 1.7 2008/02/13 13:24:29 devgtie Exp $      Release : $Revision: 1.7 $        Date : $Date: 2008/02/13 13:24:29 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao203.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	12 Oct 1994	: Creation
* volcic	02 Nov 1994	: Modification des structures (v1.2) 
*					XZAOT_ 
* volcic        10 Nov 1994     : Ajout Structures Sequences (v1.3)
* volcic        10 Nov 1994     : Modif Nom XZAO203 (v1.4)
* volcic        18 Nov 1994     : Modif Structure Scenarios (v1.5)
* mismer        24 Avr 1997     : Ajout colonne alerte + type gabarit,vitesse et niche (v1.6)
* JBL			Nov 2007		: DEM 662
------------------------------------------------------ */

#ifndef xzao203
#define xzao203

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO203_RPC_NAME "XZAO;203"

#define XZAOC_XZAO203_NB_PARM_RETURN 22

/* definitions de types exportes */

typedef struct {
	
	XDY_Eqt		Numero;
	XDY_NomEqt	NomServeur;
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
	
} XZAOT_GTC_NICE;

typedef struct {
	
	XDY_Eqt		Numero;
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
	
} XZAOT_Tube;		

typedef struct {
	
	XDY_Eqt		NumeroDemiEch;
	XDY_Mot		AdresseEtat;
	XDY_Mot		AdresseCommande;
	XDY_Mot		AdresseDiscordance;
	XDY_Mot		AdresseTelealarme;
	
} XZAOT_DemiEchangeur;		


typedef struct {
	
	XDY_Eqt		NumDemiEch;
	XDY_Octet	NumSequence;
	XDY_Mot		Adresse;
	XDY_Octet	BitCommande;
	XDY_Octet	BitDisc;
	
} XZAOT_SeqEch;		

typedef struct {
	
	XDY_Eqt		NumTube;
	XDY_Octet	NumSequence;
	XDY_Mot		Adresse;
	XDY_Octet	BitCommande;
	XDY_Octet	BitDisc;
	
} XZAOT_SeqTube;		

typedef struct {
	
	XDY_Eqt		NumMaitre;
	XDY_TypeEqt	TypeMaitre;
	XDY_Eqt		NumeroPnGTC;
	XDY_Octet	RangComm;
	XDY_Octet	RangComm2;
	XDY_Octet	RangEtat;
	XDY_Octet	RangEtat2;
	XDY_Octet	BitDiscordance;
	XDY_Octet	BitDiscordance2;
	
} XZAOT_PnGTC;		

typedef struct {
	
	XDY_Eqt		NumeroTube;
	XDY_Eqt		NumeroFAV;
	XDY_Octet	RangComm;
	XDY_Octet	RangComm2;
	XDY_Octet	RangEtat;
	XDY_Octet	RangEtat2;
	XDY_Octet	BitDiscordance;
	XDY_Octet	BitDiscordance2;
	
} XZAOT_FAVTube;		

typedef struct {
	
	XDY_Eqt		NumeroTube;
	XDY_Eqt		NumeroCapteur;
	XDY_Octet	Type;
	XDY_Octet	NumBit;
	XDY_Octet	NumAlerte;
	
} XZAOT_CapteurTube;		


/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO203_Liste_Eqt_GTC_NICE (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XZAOT_GTC_NICE * );

#endif
