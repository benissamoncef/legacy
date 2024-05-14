/*E*/
/*Fichier :  $Id: eniw_don.h,v 1.2 2002/04/24 08:49:02 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2002/04/24 08:49:02 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIW * FICHIER eniw_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de gestion des donnees 
*  de la tache TENIW, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Moulin.D	version 1.1	07 Janv 2002	: Creation
* Moulin.D	version 1.2	23 Avr 2002	: Installation plate forme
*
------------------------------------------------------ */

#ifndef eniw_don
#define eniw_don
 
/* fichiers inclus */


#include "ex_mdon.h"
#include "etim_don.h"
#include "eniw_cfg.h"
#include "eniw_ctl.h"
#include "eniw_cmd.h"
#include "xzec.h"
#include "xzet.h"
#include "xzaac.h"
#include "xzamc.h"

/* definitions de constantes exportees */
#define ENICC_REVEIL_15MN	900
#define ENIW_CMD_IS_ALIVE 0
#define ENIW_CMD_PANNEAU 1
#define ENIW_CMD_SEQUENCE 2
#define ENIW_CMD_MAITRE 3
#define ENIW_CMD_SYNCHRO 4
#define ENIW_CMD_DEMANDE_INIT 5
#define ENIW_CMD_DEMANDE_ARRET 6

#define ENIW_REC_MAITRE 0
#define ENIW_REC_TELEALARME 1
#define ENIW_REC_ETOR 2
#define ENIW_REC_DISCPAN 3
#define ENIW_REC_DISCSEQ 4
#define ENIW_REC_CO 5
#define ENIW_REC_ALERTE 6
#define ENIW_REC_PANNEAU 7



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
	
} ENIW_DONNEES_Panneau;	

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
	
} ENIW_DONNEES_FAV;	


typedef struct {
	
	XDY_Mot		NumSequence;
	XDY_Mot		Adresse;
	XDY_Mot		BitCommande;
	XDY_Mot		BitDiscordance;
	XDY_Booleen	EtatDisc;
	XDY_Entier	NumAction;
	void *		Suivant;
	
} ENIW_DONNEES_Sequence;		

typedef struct {
	
	XDY_Eqt		NumeroCapteur;
	XDY_Mot		Type;
	XDY_Mot		NumBit;
	XDY_Mot		NumAlerte;
	XDY_Booleen	EtatCapteur;
	XDY_Mot		Etat_Service;
	void *		Suivant;
	
} ENIW_DONNEES_Capteur;	

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
	XDY_Mot			EtatEnergie;
	XDY_Mot			EtatLocal;
	XDY_Mot			EtatLampe;
	XDY_Mot			DerniereSequence;
	XDY_Mot			Etat_Service;
	ENIW_DONNEES_Sequence *	ListeSequence;
	ENIW_DONNEES_Panneau *	ListePanneau;
	ENIW_DONNEES_Capteur *	ListeCapteur;
	void *			Suivant;
	
} ENIW_DONNEES_Tube;		

typedef struct {
	
	XDY_Eqt			NumeroDemiEch;
	XDY_Mot			AdresseEtat;
	XDY_Mot			AdresseCommande;
	XDY_Mot			AdresseDiscord;
	XDY_Mot			AdresseTelealarme;
	XDY_Mot			NbrPanFAV;
	XDY_Mot			EtatEnergie;
	XDY_Mot			EtatLocal;
	XDY_Mot			EtatLampe;
	XDY_Mot			DerniereSequence;
	XDY_Mot			Etat_Service;
	ENIW_DONNEES_Sequence *	ListeSequence;
	ENIW_DONNEES_Panneau *	ListePanneau;
	void *			Suivant;
	
} ENIW_DONNEES_Echangeur;		



		

/* definition de macro exportees */

/* declarations de donnees globales */

extern 	ENIW_DONNEES_Tube	*pg_debutTube;
extern 	ENIW_DONNEES_Echangeur	*pg_debutEchangeur;
extern 	XZEXT_MASK *		vg_SockMask;
extern 	T_INT4			vg_SockRT;
extern 	int			vg_SockTWWI0;
extern 	int			vg_SockTWWI1;
extern 	int			vg_SockTETIM;

/* delaration de fonctions externes */

EX_DECLA_INSERER   ( ENIW_DONNEES_Tube,  et_ins_liste_Tube )
EX_DECLA_INSERER   ( ENIW_DONNEES_Echangeur, et_ins_liste_Ech )

#endif
