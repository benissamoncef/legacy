/*E*/
/* Fichier : @(#)exa0_don.h	1.5      Release : 1.5        Date : 05/06/97
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEXA0 * FICHIER exa0_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de donnees de la tache 
*  TEXA0, protocole de la gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	22 Nov 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
* Mismer.D	version 1.3	29 Mai 1995	: Modif declaration fonction ea_sup_liste_cmd
* Mismer.D	version 1.4	06 Sep 1995	: Ajout vg_Etat_AprilNet
* Mismer.D      version 1.5     24 Avr 1997     : Ajout alerte monaco (DEM/1421)
* SFO           06-10-2014 : suppression stropts pour compil sous linux
------------------------------------------------------ */

#ifndef exa0_don
#define exa0_don
#define HP9000

/* fichiers inclus */


#include <lms1.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <time.h>
#ifdef _HPUX_SOURCE
#include <stropts.h>
#endif
#include <poll.h>

#include <sys/times.h>
#include "ex_mdon.h"
#include "xzec.h"
#include "xzet.h"
#include "xzsm.h"
#include "xzaac.h"
#include "xzamc.h"

/* definitions de constantes exportees */

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
	
} EXA0_DONNEES_GENERALE;


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
	void *			Suivant;
	
} EXA0_DONNEES_Tube;		

typedef struct {
	
	XDY_Eqt			NumeroDemiEch;
	XDY_Mot			AdresseEtat;
	XDY_Mot			AdresseCommande;
	XDY_Mot			AdresseDiscord;
	XDY_Mot			AdresseTelealarme;
	XDY_Mot			NbrPanFAV;
	void *			Suivant;
	
} EXA0_DONNEES_Echangeur;		


typedef struct {
	
	errblT *		pErrBlk;
	lmireeT *		Evenement;
	lmoreeT *		pLmoree;
	char **			NomEvenement;
	
} EXA0_AST_PARAM;		


typedef struct {
	
	double			Horodate;
	int 			Type;
	int			Adresse;
	int			NbrCmd;
	XDY_Texte		Msg;
	short			TabCmd[XZETC_MAX_PANNEAU/XZETC_NB_ETAT_PAN];
	void *			Suivant;
	
} EXA0_ENG_CMD;		


typedef char EXA0_DESCRI_VARIABLE[32];		
typedef char EXA0_NOM_VARIABLE[32];		

/* definition de macro exportees */

/* declarations de donnees globales */

extern int			vg_Mode_Fonct;
extern XDY_NomMachine		vg_NomMachine;
extern EXA0_DONNEES_Tube	*pg_debutTube;
extern EXA0_DONNEES_Echangeur	*pg_debutEchangeur;
extern EXA0_ENG_CMD		*pg_debutCmd;

extern short			vg_TabFlag[XZETC_TAILLE_FLAG];
extern EXA0_DONNEES_GENERALE    vg_ConfigAdresse;
extern XZEXT_MASK *		vg_SockMask;
extern T_INT4			vg_SockRT;
extern int			vg_SockTENIC;
extern int			vg_SockApril;
extern int			vg_SockSrvApr;
extern int			vg_Etat_AprilNet;
extern struct pollfd		*vg_SockLms1;

extern errblT  vg_ErrApril;
extern lmoinaT vg_AssocApril;
extern lmidevT vg_VarTelealarme;
extern lmidevT vg_VarSignalisation;
extern lmidevT vg_VarETOR;
extern lmidevT vg_VarITOR;
extern lmidevT vg_VarAlerte;

extern short * pg_Telealarme;
extern short * pg_Signalisation;
extern short * pg_ETOR;
extern short * pg_ITOR;
extern short * pg_Alerte;

extern short * pg_ResultTelealarme;
extern short * pg_ResultSignalisation;
extern short * pg_ResultETOR;
extern short * pg_ResultITOR;
extern short * pg_ResultAlerte;

/* delaration de fonctions externes */

EX_DECLA_INSERER   ( EXA0_DONNEES_Tube,  ea_ins_liste_Tube )
EX_DECLA_INSERER   ( EXA0_DONNEES_Echangeur, ea_ins_liste_Ech )
EX_DECLA_INSERER   ( EXA0_ENG_CMD, ea_ins_liste_cmd )
extern void  ea_sup_liste_cmd ( XDY_Horodate );	

#endif
