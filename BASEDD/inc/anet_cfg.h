/*E*/
/*  Fichier : $Id: anet_cfg.h,v 1.2 2017/04/28 17:28:47 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2017/04/28 17:28:47 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE ANET * FICHIER anet_cfg.h
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees de configuration du composant ANET
*  de mise au neutre Temps de parcours des panneaux de la traversee de Toulon.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	31/03/17 : Creation (DEM 1221)  1.1
* JPL	26/04/17 : Ajout donnees de definition de TdP aux panneaux; ajout anet_cfg_init_2  1.2
-----------------------------------------------------------------------------------------*/

#include	<xzea.h>
#include	<xzad.h>


/* Definitions de constates */
/* ------------------------ */

#define		ANET_NB_LIG_PMV_SAGA		5
#define		ANET_MAX_LIG_TDP		3

#define		ANET_EQT_ETAT_INCONNU	((XDY_Etat_Eqt) -1)


/* Definitions de types exportes */
/* ----------------------------- */

typedef struct
{
	XDY_NomMachine		NomMachine;
	XDY_NomTache		NomTache;
	XDY_NomSite		NomSite;
	XDY_District		NumeroSite;
}
ANET_CONFIG_TACHE;



/* Donnees gerees par la tache pour les panneaux PMV et PMVA */

typedef struct
{
	XDY_Eqt			NumeroEqt;
	XDY_NomEqt		NomEqt;
	XDY_NomAuto		Autoroute;
	XDY_Autoroute		NumAuto;
	XDY_PR			PR;
	XDY_Sens		Sens;
	XDY_Octet		TypeSaga;
	XDY_Octet		NbLignes;
	XDY_Octet		NbCarLigne;
	XDY_Etat_Eqt		EtatEqt;
	XZEAT_Etat_Eqt_SAGA	Affiche;
	XZADT_Dest_TDP		DestTdP[ANET_MAX_LIG_TDP];
}
ANET_PMV_A_SAGA;



/* Definitions de constantes exportees */
/* ----------------------------------- */



/* Declarations de donnees globales */
/* -------------------------------- */

extern ANET_CONFIG_TACHE		vg_ConfigNeutreSaga;


/* Liste des panneaux (PMV et PMVA) du site*/

extern ANET_PMV_A_SAGA			*tg_Liste_PMV_SAGA;

extern int				vg_Nb_PMV_SAGA;



/* Declaration de fonctions externes */
/* --------------------------------- */

extern int	anet_cfg_init ( void );

extern int	anet_cfg_init_2 ( void );
