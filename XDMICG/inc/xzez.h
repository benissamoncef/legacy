/*E*/
/* Fichier : @(#)xzez.h	1.3   Release : 1.3   Date : 02/14/12   *
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEZ  * FICHIER xzez.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzez.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	03 Mar 1997	: Creation pour TDP (DEM/1417)
* VR            version 1.2 04 Nov 2011 : Ajout DestNoeud dans la structure XZEZT_Val_TDP (DEM/1014)
* VR		Ajout TDP PMVA (DEM/1014 PMA)
* JMG		ajout XZEZ11 LOT25 DEM1301 	1.4
* RG		Correction synchro revision	1.6
* GR 30/07/20   Ajout TDP_FCD DEM-SAE152 1.7     
* ABE 24/09/20  Modif appel de la xzez12 par tewsp DEM-SAE152 1.8
------------------------------------------------------ */

#ifndef XZEZ
#define XZEZ

/* fichiers inclus  */

#include <xdy.h>
#include <xdc.h>
#include "xzst.h"
#include "xdm.h"

/* definitions de constantes exportees */


/* definitions de types exportes */

typedef struct
 	{
	XDY_Eqt		Numero;
	XDY_Ligne_PMV	Dest;
	XDY_Ligne_PMV	DestAlt;
	XDY_Ligne_PMV   DestNoeud;
	}	XZEZT_Val_TDP;	

typedef struct
 	{
	XDY_Eqt		Numero;
	XDY_Eqt		Dest;
	XDY_Eqt		DestAlt;
	XDY_Eqt         DestNoeud;
	XDY_Eqt		Site;
	}	XZEZT_DDE_TDP;	

typedef struct
	{
	XDY_Eqt Numero;
	XDY_Ligne_PMVA Dest1_proche;
	XDY_Ligne_PMVA Dest1_loin;
	XDY_Ligne_PMVA Dest2_proche;
	XDY_Ligne_PMVA Dest2_loin;
	} XZEZT_Val_TDP_PMVA;
	
typedef struct
	{
	XDY_Eqt Numero;
	XDY_Eqt Dest1_proche;
	XDY_Eqt Dest1_loin;
	XDY_Eqt Dest2_proche;
	XDY_Eqt Dest2_loin;
	XDY_Eqt Site;
	} XZEZT_DDE_TDP_PMVA;

typedef struct S_XZEZT_Liste_Zdp
{
  XDY_ZDP                       zdp;
  struct S_XZEZT_Liste_Zdp*     suivant;
} XZEZT_Liste_Zdp;

typedef struct S_XZEZT_Liste_Destinations
{
  XDY_Destination                       destination;
  struct S_XZEZT_Liste_Destinations*    suivant;
} XZEZT_Liste_Destinations;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZEZ01_Dde_Valeur_TDP (	XDY_Datagroup,
					XDY_Fonction,
					XZEZT_DDE_TDP,
					int * );

extern int XZEZ02_Calcul_TDP (		XDY_Autoroute,
					XDY_PR,
					XDY_Sens,
					XDY_Eqt,
					XZEZT_Liste_Zdp	*,
					XZEZT_Liste_Destinations *,
					XZEZT_Liste_Zdp	*,
					XDY_TDP *, char * );

extern int XZEZ03_CompteRendu_TDP (	XDY_Entier,
					XDY_Booleen,
					XDY_Eqt,
					XDY_NomMachine );

extern int XZEZ04_Dde_Valeur_TDP_PMVA(XDY_Datagroup,
					XDY_Fonction,
					XZEZT_DDE_TDP_PMVA,
					int *);

extern int XZEZ05_Calcul_TDP_PMVA( XDY_Autoroute,
		XDY_PR,
		XDY_Sens,
		XDY_Eqt,
		XDY_Eqt,
		XDY_Eqt,
		XDY_Eqt,
		XZEZT_Liste_Zdp *,
		XZEZT_Liste_Destinations *,
		XDY_TDP *);

extern int XZEZ06_CompteRendu_TDP_PMVA( XDY_Entier,
			XDY_Booleen,
			XDY_Eqt,
			XDY_NomMachine);

extern int XZEZ11_Calcul_Traversee_Bouchon (       XDY_Autoroute                   ,
                                XDY_Sens                        ,
                                XDY_PR                          ,
                                XDY_PR                          ,
                                XZEZT_Liste_Zdp *               ,
                                XZEZT_Liste_Zdp *               ,
                                XDY_TDP *                       );

extern int XZEZ12_Envoi_TDP_FCD (XDY_Horodate,
                                 XDY_PR,
                                 XDY_PR,
				 XDY_Sens,
				 XDY_TDP,
				 XDY_Octet,
                                 XDY_NomAuto);

#endif
