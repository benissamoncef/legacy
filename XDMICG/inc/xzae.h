/*E*/
/*  Fichier : 	$Id: xzae.h,v 1.14 1999/02/26 11:42:24 gaborit Exp $      Release : $Revision: 1.14 $        Date : $Date: 1999/02/26 11:42:24 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE XZAE * FICHIER XZAE.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	07 Sep 1994	: Creation
* C.T.  	10 Oct 1994	: Ajout de l'include de xzaec.h
* OM		05 Dec 1994	: Ajout du champ AbrPtCar dans XZAET_Evenement
* C.T.  	23 Jan 1995	: Ajout du champ Position dans XZAET_Evenement (1.12)
* P.N.		16 Jun 1998	: Ajout du champ Presdang dans XZAET_Vehicule (1.13)
* P.N.		13 Jan 1999	: Ajout des champs datex,gravite,trafic,bloquant,duree dans XZAET_Evenement (dem/1724) (1.14)
------------------------------------------------------ */

#ifndef XZAE
#define XZAE

/* fichiers inclus */

#include "xzaec.h"

/* definitions de constantes exportees */

#define XZAEC_VehiculeCar 	3

/* definitions de types exportes */

typedef struct {

	XDY_Entier	Numero;
	XDY_Octet	Cle;
	XDY_Octet	Site;
	XDY_Octet	Type;
	XDY_Horodate	DebutPrevu;
	XDY_Horodate	Debut;
	XDY_Horodate	FinPrevue;
	XDY_Horodate	Fin;
	XDY_Horodate	Validation;
	XDY_Entier	NumCause;
	XDY_Octet	CleCause;
	XDY_Octet	TypeCause;
	XDY_Alerte	NumAlerte;
	XDY_Horodate	HoroAlerte;
	XDY_Nom		TypeAlerte;
	XDY_Nom		Origine;
	XDY_Booleen	Confirme;
	XDY_Autoroute	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Octet	PointCar;
	XDY_Nom		NomPtCar;
	XDY_Abrev	AbrPtCar;
	XDY_Octet	NumPtCar;
	XDY_Octet	Position;
	XDY_Octet	Datex;
	XDY_Octet	Trafic;
	XDY_Octet	Gravite;
	XDY_Booleen	Bloquant;
	XDY_Octet	Duree;
	
} XZAET_Evenement;	
	
typedef struct {

	XDY_Voie	VR;
	XDY_Voie	VM2;
	XDY_Voie	VM1;
	XDY_Voie	VL;
	XDY_Voie	BAU;
	XDY_Voie	VR_I;
	XDY_Voie	VM2_I;
	XDY_Voie	VM1_I;
	XDY_Voie	VL_I;
	XDY_Voie	BAU_I;
	
} XZAET_Voies;	
	
typedef struct {

	XDY_Octet	Numero;
	XDY_Horodate	Horodate;
	XDY_Horodate	HoroPerime;
	XDY_Commentaire	Texte;

} XZAET_Commentaire;

typedef struct {

	XDY_Octet	VL;
	XDY_Octet	PL;
	XDY_Octet	Remorque;
	XDY_Octet	Car;
	XDY_Octet	Moto;
	XDY_Octet	BlesseLeg;
	XDY_Octet	BlesseGra;
	XDY_Octet	Mort;
	XDY_Booleen	VehEscota;
	XDY_Booleen	MatDang;
	XDY_Booleen	HomEscota;
	XDY_Booleen	Personnalite;
	XDY_Booleen	DegatsDom;
	
} XZAET_Accident;

typedef struct {

	XDY_Octet	PassagersBus;
	XDY_Booleen	Bebe;
	XDY_Booleen	PersAgee;			
	XDY_Booleen	Animaux;

} XZAET_ComplAcc;	
	
typedef struct {

	XDY_PR		PRFin;
	XDY_Octet	Perturbation;
	XDY_Mot		Visibilite;
	XDY_Entier	Longueur;

} XZAET_Meteo;

typedef struct {

	XDY_Octet	Type;
	XDY_Octet	Passagers;
	XDY_Booleen	Bebe;
	XDY_Booleen	PersAgee;			
	XDY_Booleen	Animaux;
	XDY_Booleen	VehEscota;
	XDY_Octet	Chargement;
	XDY_MatDang	CodeMat;
	XDY_Entier	Enleve;
	XDY_TexteVeh	Texte;
	XDY_Booleen	Presdang;

} XZAET_Vehicule;	

typedef struct {

	XDY_Mot		Nombre;
	XDY_Booleen	Perception;
	XDY_Booleen	Violence;

} XZAET_Manifestation;	
	
typedef struct {

	XDY_PR		PR;
	XDY_Entier	Longueur;
	
} XZAET_Basculement;	

typedef struct {

	XDY_Booleen	Nature;
	XDY_Octet	Type;
	XDY_Booleen	Continu;
	XDY_Horodate	HoroDebC;
	XDY_Horodate	HoroFinC;
	XDY_Booleen	Mobile;
	XDY_Autoroute	Autoroute;
	XDY_PR		PRqueueM;
	XDY_PR		PRteteM;
	XDY_Sens	SensM;
	XDY_PR		PRtete;
	XDY_Booleen	VoiesRet;
	XDY_Entier	Longueur;
	XDY_Entier	LongueurM;
	
} XZAET_Travaux;


typedef struct {

	XDY_Nom		Nom;
	XDY_Booleen	Ferme;
	XDY_Booleen	Penurie;
	XDY_Octet	Carburant;
	
} XZAET_SousConcess;

typedef struct {

	XDY_Booleen	Entree1;
	XDY_Booleen	Entree2;
	XDY_Booleen	Sortie1;
	XDY_Booleen	Sortie2;
	
} XZAET_FermetureEchangeur;
		

#endif
