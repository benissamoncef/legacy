/*E*/
/*Fichier : @(#)xzao115.h	1.4	     Release : 1.4        Date : 01/25/95
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao115.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	14 Oct 1994	: Modif XDY_FonctionInt
* PNI         10 mai 2006     : XDY_CodeEqt devient XDY_CodeCam v1.2 DEM565
------------------------------------------------------ */

#ifndef xzao115
#define xzao115

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO115_RPC_NAME "XZAO115"

#define XZAOC_XZAO115_NB_PARM_RETURN 4

/* definitions de types exportes */

typedef struct {

   	XDY_Eqt		Numero;
	XDY_NomEqt	NomServeur;
   	XDY_Octet	Liaison;
   	XDY_Rgs		RGS;
   	
} XZAOT_SysVideo;

typedef struct {
	
	XDY_Eqt		Numero;
	XDY_Octet	LiaisonsFixes;
	XDY_Octet	LiaisonsCycles;

} XZAOT_LTVideo;

typedef struct {

   	XDY_Eqt		Numero;
	XDY_Eqt		NumeroLTVideo;
	XDY_CodeCam	Code;
   	XDY_Booleen	Cycle;
	XDY_Booleen	Mobile;
	XDY_Booleen	Positionnable;
	
} XZAOT_Camera;
	
typedef struct {

   	XDY_Eqt		Numero;
	XDY_CodeCam	Code;
   	XDY_NomEqt	MachineOperateur;
	
} XZAOT_Moniteur;

typedef struct {

   	XDY_Eqt		Numero;
	XDY_CodeEqt	Code;
   	
} XZAOT_Magneto;	


/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO115_Lire_SystemeVideo (	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XZAOT_SysVideo *);

#endif
