/*E*/
/* Fichier : $Id: enic_don.h,v 1.6 1997/05/06 19:58:11 mismer Exp $      Release : $Revision: 1.6 $        Date : $Date: 1997/05/06 19:58:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIC * FICHIER enic_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de gestion des donnees 
*  de la tache TENIC, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	03 Nov 1994	: Creation
* Mismer.D	version 1.2	01 Fev 1995	:
* Mismer.D	version 1.3	17 Mar 1995	:
* Mismer.D	version 1.4	18 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.5	04 Oct 1996	: Correction type NumAction (DEM/1172)
* Mismer.D      version 1.6     24 Avr 1997     : Ajout colonne alerte (DEM/1421)
*
------------------------------------------------------ */

#ifndef enic_don
#define enic_don

/* fichiers inclus */


#include "ex_mdon.h"
#include "etim_don.h"
#include "enic_cfg.h"
#include "enic_ctl.h"
#include "enic_cmd.h"
#include "xzec.h"
#include "xzet.h"
#include "xzaac.h"
#include "xzamc.h"

/* definitions de constantes exportees */
#define ENICC_REVEIL_15MN	900

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
	
} XZAOT_GTC_NICE;


typedef struct {
	
	XDY_Eqt		NumeroPanneau;
	XDY_Mot		RangComm;
	XDY_Mot		RangComm2;
	XDY_Mot		RangEtat;
	XDY_Mot		RangEtat2;
	XDY_Mot		BitDiscordance;
	XDY_Mot		BitDiscordance2;
	XDY_Mot		EtatPanneau;
	XDY_Booleen	EtatDisc;
	XDY_Mot		Etat_Service;
	void *		Suivant;
	
} ENIC_DONNEES_Panneau;	

typedef struct {
	
	XDY_Eqt		NumeroFAV;
	XDY_Mot		RangComm;
	XDY_Mot		RangComm2;
	XDY_Mot		RangEtat;
	XDY_Mot		RangEtat2;
	XDY_Mot		BitDiscordance;
	XDY_Mot		BitDiscordance2;
	XDY_Mot		EtatFAV;
	XDY_Mot		Etat_Service;
	void *		Suivant;
	
} ENIC_DONNEES_FAV;	


typedef struct {
	
	XDY_Mot		NumSequence;
	XDY_Mot		Adresse;
	XDY_Mot		BitCommande;
	XDY_Mot		BitDiscordance;
	XDY_Booleen	EtatDisc;
	XDY_Entier	NumAction;
	void *		Suivant;
	
} ENIC_DONNEES_Sequence;		

typedef struct {
	
	XDY_Eqt		NumeroCapteur;
	XDY_Mot		Type;
	XDY_Mot		NumBit;
	XDY_Mot		NumAlerte;
	XDY_Booleen	EtatCapteur;
	XDY_Mot		Etat_Service;
	void *		Suivant;
	
} ENIC_DONNEES_Capteur;	

typedef struct {
	
	XDY_Eqt			Numero;
	XDY_Mot			AdresseEtat;
	XDY_Mot			AdresseCommande;
	XDY_Mot			AdresseDiscord;
	XDY_Mot			AdresseExtincteur;
	XDY_Mot			AdresseLit;
	XDY_Mot			AdresseOPCO;
	XDY_Mot			AdresseTelealarme;
	XDY_Mot			AdresseGabarit;
	XDY_Mot			AdresseVitesse;
	XDY_Mot			AdresseNiche;
	XDY_Mot			NbrPanFAV;
	XDY_Mot			EtatTelealarme;
	XDY_Mot			DerniereSequence;
	XDY_Mot			Etat_Service;
	ENIC_DONNEES_Sequence *	ListeSequence;
	ENIC_DONNEES_Panneau *	ListePanneau;
	ENIC_DONNEES_Capteur *	ListeCapteur;
	void *			Suivant;
	
} ENIC_DONNEES_Tube;		

typedef struct {
	
	XDY_Eqt			NumeroDemiEch;
	XDY_Mot			AdresseEtat;
	XDY_Mot			AdresseCommande;
	XDY_Mot			AdresseDiscord;
	XDY_Mot			AdresseTelealarme;
	XDY_Mot			NbrPanFAV;
	XDY_Mot			EtatTelealarme;
	XDY_Mot			DerniereSequence;
	XDY_Mot			Etat_Service;
	ENIC_DONNEES_Sequence *	ListeSequence;
	ENIC_DONNEES_Panneau *	ListePanneau;
	void *			Suivant;
	
} ENIC_DONNEES_Echangeur;		



		

/* definition de macro exportees */

/* declarations de donnees globales */

extern 	ENIC_DONNEES_Tube	*pg_debutTube;
extern 	ENIC_DONNEES_Echangeur	*pg_debutEchangeur;
extern 	XZEXT_MASK *		vg_SockMask;
extern 	T_INT4			vg_SockRT;
extern 	int			vg_SockTEXA0;
extern 	int			vg_SockTETIM;

/* delaration de fonctions externes */

EX_DECLA_INSERER   ( ENIC_DONNEES_Tube,  et_ins_liste_Tube )
EX_DECLA_INSERER   ( ENIC_DONNEES_Echangeur, et_ins_liste_Ech )

#endif
