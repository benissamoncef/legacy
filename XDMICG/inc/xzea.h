/*E*/
/*Fichier :  $Id: xzea.h,v 1.24 2021/05/03 14:05:18 pc2dpdy Exp $      Release : $Revision: 1.24 $        Date : $Date: 2021/05/03 14:05:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE xzea  * FICHIER xzea.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < fichier de donnees commun aux fichier xzea* >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	12 Sep 1994	: Creation
* Mismer.D	version 1.2	12 Oct 1994	: Ajout xzea28
* Mismer.D	version 1.3	21 Nov 1994	: Ajout primitives abonnement GTC de NICE
* Volcic.F	version 1.4	21 Dec 1994	: Ajout primitives abonnement RADT
* Lecrivain	version 1.5	30 Dec 1994	: Ajout abonnement autotel et operator, Ajout xzea25
* Mismer.D	version 1.6	20 Fev 1995	: Ajout primitives abonnement Etat Matrice NAV
* Volcic.F	version 1.7	15 Mar 1995	: Ajout primitives abonnement Etat Liaison Dataviews
* Volcic.F	version 1.8	16 Mar 1995	:
* Volcic.F	version 1.9	16 Mar 1995	:
* Fontaine.C	version 1.10	24 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.11	17 Sep 1996	: Modif Extension Ouest (RADT_W)
* Mismer.D	version 1.12	04 Oct 1996	: Modif pour pesage RADT (DEM/1227)
* Mismer.D	version 1.13	21 Nov 1996	: Modif pour METEO (DEM/1232)
* Mismer.D	version 1.14	21 Fev 1997	: Modif pour TDP (DEM/1396)
* Mismer.D	version 1.15	19 Jan 1998	: Modif pour PAL (DEM/1539)
* Mismer.D	version 1.16	21 Jan 1998	: Modif pour FAU et FAC (DEM/1544)
* JMG		version 1.17	13/09/05	: PMVA BAF BAD
* JBL		Nov 2007					: DEM 662
* VR		Dec 2011					: Ajout equipement PRV (DEM/1016)
* JMG		16/04./16 : ajout SAGA 1.20
* LCL		22/02/18  : ajout CTRL_FEUX 1.21 DEM1284
* LCL		11/05/18  : ajout CAP GABEB 1.22 DEM1283 
* ABE		15/12/20  : ajout IMU SAE-DEM155 1.23 
* ABE		02/04/21  : ajout PAU SONO DEM-SAE244 1.24
* GGY		23/11/23  : Ajout XZEAT_Etat_PIC (DEM483)
------------------------------------------------------ */

#ifndef xzea
#define xzea

/* fichiers inclus */
#include <xdy.h>
#include <xdc.h>
#include <xdm.h>
#include <xdg.h>
#include "xzst.h"
#include "xzec.h"
#include "xzea01.h"
#include "xzea02.h"
#include "xzea03.h"
#include "xzea04.h"
#include "xzea05.h"
#include "xzea06.h"
#include "xzea07.h"
#include "xzea08.h"
#include "xzea09.h"
#include "xzea10.h"
#include "xzea11.h"
#include "xzea12.h"
#include "xzea13.h"
#include "xzea14.h"
#include "xzea15.h"
#include "xzea16.h"
#include "xzea18.h"
#include "xzea22.h"
#include "xzea23.h"
#include "xzea24.h"
#include "xzea25.h"
#include "xzea26.h"
#include "xzea27.h"
#include "xzea28.h"
#include "xzea29.h"
#include "xzea30.h"
#include "xzea31.h"
#include "xzea32.h"
#include "xzea33.h"
#include "xzea34.h"
#include "xzea35.h"
#include "xzea36.h"
#include "xzea37.h"
#include "xzea38.h"
#include "xzea71.h"

/* GTC renovee */
#include "xzea42.h"
#include "xzea43.h"
#include "xzea44.h"
#include "xzea45.h"
#include "xzea46.h"
#include "xzea47.h"
#include "xzea48.h"
#include "xzea49.h"
#include "xzea50.h"
#include "xzea51.h"
#include "xzea52.h"
#include "xzea53.h"
#include "xzea54.h"
#include "xzea55.h"
#include "xzea180.h"

/* definitions de constantes exportees */

#define	XZEAC_DEBUT_ABONNEMENT		XDC_DEBUT_ABONNEMENT_EQUEXT
#define	XZEAC_FIN_ABONNEMENT		XDC_FIN_ABONNEMENT_EQUEXT
#define	XZEAC_FERMER_ABONNEMENT		XDC_FERMER_ABONNEMENT_EQUEXT
#define	XZEAC_FORMAT_FICHIER_HISTORIQUE_DONNEES	"\
Numero_equipement=%d\n\
Validite=%d\n\
Date_de_debut=%s\n\
Heure_de_debut=%s\n\
Date_de_fin=%s\n\
Heure_de_fin=%s\n\
Type_de_pluie=%d\n"

/* definitions de types exportes */

typedef	XDY_EMes_RADT		XZEAT_EMes_RADT;
typedef XDY_ENiveau_Service	XZEAT_ENiveau_Service;
typedef XDY_EMes_DAI		XZEAT_EMes_DAI;
typedef XDY_EMes_Poids		XZEAT_EMes_Poids;
typedef XDY_EMes_Classe_DIV	XZEAT_EMes_Classe_DIV;
typedef XDY_Etat_PMV		XZEAT_Etat_PMV;
typedef XDY_Etat_Picto		XZEAT_Etat_Picto;
typedef XDY_Etat_PAL		XZEAT_Etat_PAL;
typedef XDY_Etat_GTC		XZEAT_Etat_GTC;
typedef XDY_Etat_MAA		XZEAT_Etat_MAA;	
typedef XDY_Etat_PAU		XZEAT_Etat_PAU;	
typedef XDY_Etat_Magneto	XZEAT_Etat_Magneto;	
typedef XDY_Etat_Camera		XZEAT_Etat_Camera;	
typedef XDY_Etat_ES80		XZEAT_Etat_ES80;
typedef XDY_Etat_Autotel	XZEAT_Etat_Autotel;	
typedef XDY_Etat_Operator	XZEAT_Etat_Operator;	

typedef struct
	{
	XDY_TypeEqt		TypeEqt;
	XDY_Etat_NAV		Etat;
	}	XZEAT_Etat_NAV;

typedef struct
 	{
	XDY_TypeEqt	TypeEqt;
	XDY_Eqt		Numero;
	XDY_Octet	Contraste;
	}	XZEAT_Contraste;	

typedef struct
 	{
	XDY_Octet	JourNuit;
	}	XZEAT_JourNuit;
	
typedef struct
 	{
	XDY_Eqt		NumTube;
	XDY_Eqt		NumCapt;
	XDY_Octet	EtatCapt;
	}	XZEAT_Capteur;	

typedef struct
 	{
	XDY_TypeEqt	TypeEqt;
	XDY_Eqt		NumEqt;
	XDY_Eqt		NumPanneau;
	XDY_Octet	EtatPanneau;
	XDY_Octet	EtatDisc;
	}	XZEAT_Panneau;

typedef struct
 	{
 	XDY_Entier  Domaine;
	XDY_TypeEqt	TypeEqt; /* Type du maitre */
	XDY_Eqt		NumEqt; /* Numero du maitre */
	XDY_Entier	TypeEquipement;
	XDY_Eqt		NumEquipement;
	XDY_Octet	EtatEquipement;
	XDY_Octet	EtatTechnique;
	XDY_Octet	Mode;
	XDY_Octet	EtatDisc;
	XDY_Octet	ModeRafale;
	}	XZEAT_Equipement;

typedef struct
 	{
	XDY_Entier	TypeOuvrage;
	XDY_Entier 	IdOuvrage;
	XDY_Entier  Domaine;
	XDY_Entier	Mode;
	XDY_Entier	IdScenario;
	}	XZEAT_Domaine;

typedef struct
 	{
	XDY_TypeEqt	TypeEqt;
	XDY_Eqt		NumEqt;
	XDY_Octet	Energie;
	XDY_Octet	Local;
	XDY_Octet	Lampe;
	}	XZEAT_Telealarme;

typedef struct
 	{
	XDY_TypeEqt	TypeEqt;
	XDY_Eqt		NumEqt;
	XDY_Octet	SeqCourante;
	XDY_Octet	SeqChange;
	XDY_Octet	EtatSeq;
	}	XZEAT_Sequence;

typedef struct
 	{
	XDY_Eqt		NumEqt;
	char		Matrice[XDC_Lon_Matrice];
	XDY_Mot		Params[XDC_Lon_Params_Matrice];
	}	XZEAT_Etat_Mat_NAV;

typedef struct
        {
        XDY_Eqt         Eqt;
        XDY_TypeEqt     TypeEqt;
        XDY_Booleen     Etat;
        }       XZEAT_Etat_Liaison;

typedef struct
        {
	XDY_Horodate	Horodate;	
	XDY_Eqt		Numero;	
	XDY_Sens	Sens;
	XDY_Voie	Voie;
        XDY_Mot		Mesures[XDC_NB_CLASSE_SILHOUETTE];
        }       XZEAT_EMES_RADT_SILHOUETTE;

typedef struct
        {
	XDY_Horodate	Horodate;	
	XDY_Eqt		Numero;	
	XDY_Sens	Sens;
	XDY_Voie	Voie;
        XDY_Mot		Mesures[XDC_NB_CLASSE_ESSIEU];
        }       XZEAT_EMES_RADT_ESSIEU;

typedef struct
        {
	XDY_Horodate	Horodate;	
	XDY_Eqt		Numero;	
	XDY_Sens	Sens;
	XDY_Voie	Voie;
        XDY_Mot		Mesures[XDC_NB_CLASSE_POIDS];
        }       XZEAT_EMES_RADT_POIDS;


typedef XDY_Etat_PMVA		XZEAT_Etat_PMVA;
typedef XDY_Etat_PRV		XZEAT_Etat_PRV;
typedef XDY_Etat_Eqt_SAGA	XZEAT_Etat_Eqt_SAGA;
typedef XDY_Etat_CFE		XZEAT_Etat_CFE;
typedef XDY_Etat_IMU		XZEAT_Etat_IMU;
typedef XDY_Etat_SONO		XZEAT_Etat_SONO;
typedef XDY_Etat_PIC		XZEAT_Etat_PIC;

typedef struct
	{
	XDY_TypeEqt		TypeEqt;
	XDY_Etat_BAF		Etat;
	}	XZEAT_Etat_BAF;
typedef struct
	{
	XDY_TypeEqt		TypeEqt;
	XDY_Etat_BAD		Etat;
	}	XZEAT_Etat_BAD;
	


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

#endif
