/*E*/
/*  Fichier : $Id: xzad.h,v 1.4 2017/05/17 09:28:48 devgfi Exp $      Release : $Revision: 1.4 $        Date : $Date: 2017/05/17 09:28:48 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : XDMICG
-------------------------------------------------------------------------------------------
* MODULE XZAD * FICHIER xzad.h
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de calcul des Temps de Parcours.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	13/02/13 : Creation (DEM 1061)  1.1
* JPL	02/02/17 : Inclusion fichier de constantes xzadc.h (DEM 1215)  1.2
* JPL	24/04/17 : Declaration types XZADT_Liste_ZDP, XZADT_Dest_TDP et fonction XZAD_Calcul_TDP (pour DEM 1221)  1.3
* JPL	16/05/17 : Declaration fonctions XZAD_Activer_Graphe et XZAD110_Demarrer_TDP  1.4
-----------------------------------------------------------------------------------------*/

#ifndef XZAD_H
#define XZAD_H

#include	"xdy.h"

#include	"xzadc.h"


/* Definitions de constantes exportees */
/* ----------------------------------- */

/* Valeurs de codes d'erreur */

#define		XZADC_ERR_INVALIDE	11
#define		XZADC_ERR_INCONNU	12



/* Definitions de types exportes */
/* ----------------------------- */

	/* Donnees d'elaboration des Temps de Parcours */
	/* ------------------------------------------- */

/* Liste de zones de mesures (graphe) Temps de Parcours */

typedef struct S_XZADT_Liste_ZDP
{
	XDY_ZDP				*p_zdp;
	struct S_XZADT_Liste_ZDP	*suivant;
} XZADT_Liste_ZDP;



/* Moyens de definition d'un Temps de Parcours */

typedef struct S_XZADT_Dest_TDP
{
	XDY_Destination			*destination;
	XZADT_Liste_ZDP			*graphe;
} XZADT_Dest_TDP;



	/* Donnees de resultats Temps de Parcours */
	/* -------------------------------------- */

/* Donnees de Temps de Parcours d'un PMV */

typedef struct
{
	XDY_Destination		Dest_Proche;
	XDY_TDP			TDP_Proche;
	XDY_Destination		Dest_Loin;
	XDY_TDP			TDP_Loin;
	XDY_Destination		Dest_Noeud;
	XDY_TDP			TDP_Noeud;
} XZADT_Temps_De_Parcours_PMV;



/* Donnees de Temps de Parcours d'un PMVA */

typedef struct
{
	XDY_Destination		Dest1_Proche;
	XDY_TDP			TDP1_Proche;
	XDY_Destination		Dest1_Loin;
	XDY_TDP			TDP1_Loin;

	XDY_Destination		Dest2_Proche;
	XDY_TDP			TDP2_Proche;
	XDY_Destination		Dest2_Loin;
	XDY_TDP			TDP2_Loin;
} XZADT_Temps_De_Parcours_PMVA;



/* Declarations de donnees globales */
/* -------------------------------- */



/* Declarations de fonctions externes */
/* ---------------------------------- */

extern int	XZAD101_Initialiser_TDP (XDY_District va_site);

extern int	XZAD102_Terminer_TDP ();


extern int	XZAD103_Temps_De_Parcours_PMV (XDY_Eqt va_Numero_PMV, XZADT_Temps_De_Parcours_PMV *pa_TDP);

extern int	XZAD104_Temps_De_Parcours_PMVA (XDY_Eqt va_Numero_PMVA, XZADT_Temps_De_Parcours_PMVA *pa_TDP);

extern int	XZAD110_Demarrer_TDP ();


extern void	XZAD_Activer_Graphe ( XZADT_Liste_ZDP * );

extern int	XZAD_Calcul_TDP ( XDY_Autoroute, XDY_PR, XDY_Sens, XDY_Destination *, XZADT_Liste_ZDP *, XDY_TDP *, XDY_Booleen *);

#endif
