/*E*/
/*Fichier :  $Id: enix_don.h,v 1.2 2008/10/14 14:37:45 devgtie Exp $      Release : $Revision: 1.2 $        Date : $Date: 2008/10/14 14:37:45 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIX * FICHIER enix_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de gestion des donnees 
*  de la tache TENIX, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Moulin.D	version 1.1	07 Janv 2002	: Creation
* Moulin.D	version 1.2	23 Avr 2002	: Installation plate forme
* AAZ       verrsion1.3 07 Juin 2007    : GTC rénovée DEM 664
* JBL       Oct 2008 : Passage du type & numero alerte sur un XDY_Entier
* JBL		13 Oct 2008 : Suppression code inutilise (stockage interne alarmes et alertes)
------------------------------------------------------ */

#ifndef enix_don
#define enix_don
 
/* fichiers inclus */


#include "ex_mdon.h"
#include "etim_don.h"
#include "enix_cfg.h"
#include "enix_ctl.h"
#include "enix_cmd.h"
#include "xzec.h"
#include "xzet.h"
#include "xzaac.h"
#include "xzamc.h"

/* definitions de constantes exportees */
#define ENICC_REVEIL_15MN	900
#define ENIX_CMD_IS_ALIVE 0
#define ENIX_CMD_SEQUENCE 2
#define ENIX_CMD_MAITRE 3
#define ENIX_CMD_SYNCHRO 4
#define ENIX_CMD_DEMANDE_INIT 5
#define ENIX_CMD_DEMANDE_ARRET 6
/* AAZ 18/07/2007 */
#define ENIX_CMD_UNITAIRE 1
#define ENIX_CMD_DOMAINE 502

#define ENIX_REC_MAITRE 0
/* AAZ 18/07/2007 */
#define ENIX_REC_DISCEQT 3
#define ENIX_REC_DISCSEQ 4
#define ENIX_REC_EQUIPEMENT 7
#define ENIX_REC_DOMAINE 503
#define ENIX_REC_ALERTE 500
#define ENIX_REC_ALARME 501


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
	
} ENIX_DONNEES_Panneau;	

typedef struct {	
	XDY_Mot		TypeEquipement;
	XDY_Eqt		NumeroEquipement;
	XDY_Mot		TypeOuvrage;
        XDY_Mot         Domaine;
	XDY_Mot		RangComm2;
	XDY_Mot		RangEtat;
	XDY_Mot		RangEtat2;
	XDY_Mot		BitDiscordance;
	XDY_Mot		BitDiscordance2;
	XDY_Mot		EtatEquipement;
	XDY_Booleen		EtatDisc;
	XDY_Mot		Etat_Service;
	XDY_Mot     Mode;
	XDY_Booleen	InfoRecue;
	void *		Suivant;
} ENIX_DONNEES_Equipement;

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
	
} ENIX_DONNEES_FAV;	


typedef struct {
	
	XDY_Mot		NumSequence;
	XDY_Mot		Adresse;
	XDY_Mot		BitCommande;
	XDY_Mot		BitDiscordance;
	XDY_Booleen	EtatDisc;
	XDY_Entier	NumAction;
	XDY_Booleen	InfoRecue;
	XDY_Booleen	AttenteEtat;
	void *		Suivant;
	
} ENIX_DONNEES_Sequence;		

typedef struct {
	
	XDY_Eqt		NumeroCapteur;
	XDY_Mot		Type;
	XDY_Mot		NumBit;
	XDY_Mot		NumAlerte;
	XDY_Booleen	EtatCapteur;
	XDY_Mot		Etat_Service;
	void *		Suivant;
	
} ENIX_DONNEES_Capteur;	


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
    XDY_Mot                 NbrEquipement;

    /* A supprimer */
    XDY_Mot                 EtatEnergie;
    XDY_Mot                 EtatLocal;
    XDY_Mot                 EtatLampe;

    XDY_Mot                 DerniereSequence;
    XDY_Mot                 Etat_Service;
    XDY_Mot                 NumeroDomaine;
    XDY_Booleen             ModeDomaine;
    ENIX_DONNEES_Sequence   * ListeSequence;
    ENIX_DONNEES_Equipement * ListeEquipement;
    void *                  Suivant;
	
} ENIX_DONNEES_Tube;		

typedef struct {
	
	XDY_Eqt			NumeroDemiEch;
	XDY_Mot			AdresseEtat;
	XDY_Mot			AdresseCommande;
	XDY_Mot			AdresseDiscord;
	XDY_Mot			AdresseTelealarme;
    XDY_Mot                 NbrEquipement;

    /* A supprimer */
    XDY_Mot                 EtatEnergie;
    XDY_Mot                 EtatLocal;
    XDY_Mot                 EtatLampe;

    XDY_Mot                 DerniereSequence;
    XDY_Mot                 Etat_Service;
    XDY_Mot                 NumeroDomaine;
    XDY_Booleen             ModeDomaine;
    ENIX_DONNEES_Sequence   * ListeSequence;
    ENIX_DONNEES_Equipement * ListeEquipement;
    void *                  Suivant;
	
} ENIX_DONNEES_Echangeur;		

typedef struct {
    int						NumeroAbsolu;
    XDY_Eqt                 Numero;
    XDY_Mot                 AdresseEtat;
    XDY_Mot                 NbrEquipement;
    XDY_Mot                 DerniereSequence;
    XDY_Mot                 Etat_Service;
    XDY_Mot		            TypeOuvrage;
    XDY_Mot                 NumeroDomaine;
    XDY_Booleen             ModeDomaine;
	XDY_Booleen				InfoRecue;
    ENIX_DONNEES_Sequence   * ListeSequence;
    ENIX_DONNEES_Equipement * ListeEquipement;
    void *                  Suivant;
    
} ENIX_DONNEES_SIG;

typedef struct {
    int						NumeroAbsolu;
    XDY_Eqt                 Numero;
    XDY_Mot                 AdresseEtat;
    XDY_Mot                 NbrEquipement;
    XDY_Mot                 DerniereSequence;
    XDY_Mot                 Etat_Service;
    XDY_Mot		            TypeOuvrage;
    XDY_Mot                 NumeroDomaine;
    XDY_Booleen             ModeDomaine;
	XDY_Booleen				InfoRecue;
    ENIX_DONNEES_Sequence   * ListeSequence;
    ENIX_DONNEES_Equipement * ListeEquipement;
    void *                  Suivant;
    
} ENIX_DONNEES_ECL;

typedef struct {
    int						NumeroAbsolu;
    XDY_Eqt                 Numero;
    XDY_Mot                 AdresseEtat;
    XDY_Mot                 NbrEquipement;
    XDY_Mot                 DerniereSequence;
    XDY_Mot                 Etat_Service;
    XDY_Mot		            TypeOuvrage;
    XDY_Mot                 NumeroDomaine;
    XDY_Booleen             ModeDomaine;
	XDY_Booleen				InfoRecue;
    ENIX_DONNEES_Sequence   * ListeSequence;
    ENIX_DONNEES_Equipement * ListeEquipement;
    void *                  Suivant;
    
} ENIX_DONNEES_VEN;

typedef struct {
    int						NumeroAbsolu;
    XDY_Eqt                 Numero;
    XDY_Mot                 AdresseEtat;
    XDY_Mot                 NbrEquipement;
    XDY_Mot                 DerniereSequence;
    XDY_Mot                 Etat_Service;
    XDY_Mot		            TypeOuvrage;
    XDY_Mot                 NumeroDomaine;
    XDY_Booleen             ModeDomaine;
	XDY_Booleen				InfoRecue;
    ENIX_DONNEES_Sequence   * ListeSequence;
    ENIX_DONNEES_Equipement * ListeEquipement;
    void *                  Suivant;
    
} ENIX_DONNEES_ENR;

typedef struct {
    int						NumeroAbsolu;
    XDY_Eqt                 Numero;
    XDY_Mot                 AdresseEtat;
    XDY_Mot                 NbrEquipement;
    XDY_Mot                 DerniereSequence;
    XDY_Mot                 Etat_Service;
    XDY_Mot		            TypeOuvrage;
    XDY_Mot                 NumeroDomaine;
    XDY_Booleen             ModeDomaine;
	XDY_Booleen				InfoRecue;
    ENIX_DONNEES_Sequence   * ListeSequence;
    ENIX_DONNEES_Equipement * ListeEquipement;
    void *                  Suivant;
    
} ENIX_DONNEES_INC;

typedef struct {
    int						NumeroAbsolu;
    XDY_Eqt                 Numero;
    XDY_Mot                 AdresseEtat;
    XDY_Mot                 NbrEquipement;
    XDY_Mot                 DerniereSequence;
    XDY_Mot                 Etat_Service;
    XDY_Mot		            TypeOuvrage;
    XDY_Mot                 NumeroDomaine;
    XDY_Booleen             ModeDomaine;
	XDY_Booleen				InfoRecue;
    ENIX_DONNEES_Sequence   * ListeSequence;
    ENIX_DONNEES_Equipement * ListeEquipement;
    void *                  Suivant;
    
} ENIX_DONNEES_EXP;

typedef struct {
    int						NumeroAbsolu;
    XDY_Eqt                 Numero;
    XDY_Mot                 AdresseEtat;
    XDY_Mot                 NbrEquipement;
    XDY_Mot                 DerniereSequence;
    XDY_Mot                 Etat_Service;
    XDY_Mot		            TypeOuvrage;
    XDY_Mot                 NumeroDomaine;
    XDY_Booleen             ModeDomaine;
	XDY_Booleen				InfoRecue;
    ENIX_DONNEES_Sequence   * ListeSequence;
    ENIX_DONNEES_Equipement * ListeEquipement;
    void *                  Suivant;
    
} ENIX_DONNEES_SYS;

typedef struct {
    int						NumeroAbsolu;
    XDY_Eqt                 Numero;
    XDY_Mot                 AdresseEtat;
    XDY_Mot                 NbrEquipement;
    XDY_Mot                 DerniereSequence;
    XDY_Mot                 Etat_Service;
    XDY_Mot		            TypeOuvrage;
    XDY_Mot                 NumeroDomaine;
    XDY_Booleen             ModeDomaine;
	XDY_Booleen				InfoRecue;
    ENIX_DONNEES_Sequence   * ListeSequence;
    ENIX_DONNEES_Equipement * ListeEquipement;
    void *                  Suivant;
    
} ENIX_DONNEES_LTN;

typedef struct {
    XDY_Mot     NumeroDomaine;
    XDY_Mot     EtatDomaine;
    XDY_Booleen EtatDisc;
    XDY_Mot     Etat_Service;
    XDY_Booleen Mode;
	XDY_Booleen	InfoRecue;
    void *      Suivant;
    
} ENIX_DONNEES_Domaine; 

/* definition de macro exportees */

/* declarations de donnees globales */

extern 	ENIX_DONNEES_Tube	*pg_debutTube;
extern 	ENIX_DONNEES_Echangeur	*pg_debutEchangeur;
extern  ENIX_DONNEES_SIG        *pg_debutSIG;
extern  ENIX_DONNEES_ECL        *pg_debutECL;
extern  ENIX_DONNEES_VEN        *pg_debutVEN;
extern  ENIX_DONNEES_ENR        *pg_debutENR;
extern  ENIX_DONNEES_INC        *pg_debutINC;
extern  ENIX_DONNEES_EXP        *pg_debutEXP;
extern  ENIX_DONNEES_SYS        *pg_debutSYS;
extern  ENIX_DONNEES_LTN        *pg_debutLTN;
extern 	XZEXT_MASK *		vg_SockMask;
extern 	T_INT4			vg_SockRT;
extern 	int			vg_SockTWWI0;
extern 	int			vg_SockTWWI1;
extern 	int			vg_SockTETIM;

/* delaration de fonctions externes */

EX_DECLA_INSERER   ( ENIX_DONNEES_Tube,      et_ins_liste_Tube )
EX_DECLA_INSERER   ( ENIX_DONNEES_Echangeur, et_ins_liste_Ech )
EX_DECLA_INSERER   ( ENIX_DONNEES_SIG,       et_ins_liste_SIG )
EX_DECLA_INSERER   ( ENIX_DONNEES_ECL,       et_ins_liste_ECL )
EX_DECLA_INSERER   ( ENIX_DONNEES_VEN,       et_ins_liste_VEN )
EX_DECLA_INSERER   ( ENIX_DONNEES_ENR,       et_ins_liste_ENR )
EX_DECLA_INSERER   ( ENIX_DONNEES_INC,       et_ins_liste_INC )
EX_DECLA_INSERER   ( ENIX_DONNEES_EXP,       et_ins_liste_EXP )
EX_DECLA_INSERER   ( ENIX_DONNEES_SYS,       et_ins_liste_SYS )
EX_DECLA_INSERER   ( ENIX_DONNEES_LTN,       et_ins_liste_LTN )

#endif
