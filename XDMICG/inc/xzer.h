/*E*/
/* Fichier : $Id: xzer.h,v 1.3 1995/08/01 18:10:52 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/08/01 18:10:52 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZER  * FICHIER xzer.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzer.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	12 Dec 1994	: Creation
* Fontaine.C	version 1.2	24 Mai 1995	: Reprise entete
* Volcic.F	version 1.3	01 Aou 1995	: Ajout XZER03_Acknoledge_6min_QVTO
*
------------------------------------------------------ */

#ifndef xzer
#define xzer

/* fichiers inclus */
#include <xdy.h>
#include <xdc.h>
#include <xdm.h>
#include "xzst.h"
#include "xzec.h"


/* definitions de constantes exportees */

#define	XZERC_DEBUT_ABONNEMENT		XDC_DEBUT_ABONNEMENT_EQUEXT
#define	XZERC_FIN_ABONNEMENT		XDC_FIN_ABONNEMENT_EQUEXT
#define	XZERC_FERMER_ABONNEMENT		XDC_FERMER_ABONNEMENT_EQUEXT

  /* Constantes pour le calcul des niveaux de service */
  
#define	XZERC_NIV_NONRENSEIGNE		-1
#define	XZERC_NIV_FLUIDE		1
#define	XZERC_NIV_PRESATURE		2
#define	XZERC_NIV_SATURE		3

/* definitions de types exportes */

typedef	XDY_EMes_RADT		XZERT_EMes_RADT;
typedef XDY_ENiveau_Service	XZERT_ENiveau_Service;
typedef XDY_EMes_Poids		XZERT_EMes_Poids;
typedef XDY_EMes_Classe_DIV	XZERT_EMes_Classe_DIV;

typedef struct
 {
	XDY_Eqt		NoEqtRADT;
	XDY_Sens	Sens;
	XDY_Voie	EtatVoie[5];
} XZERT_Config_Voies;
	
	
/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZER01_Config_Voies_RADT (  XZERT_Config_Voies ,
				XDY_NomMachine );

extern int XZER02_Demande_Individuelle (XDY_Eqt ,
					XDY_Entier ,
					XDY_NomMachine );
extern int XZER03_Acknoledge_6min_QVTO (XDY_Eqt ,
					XDY_Horodate );
#endif
