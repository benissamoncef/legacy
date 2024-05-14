/*E*/
/*  Fichier : $Id: aneu_cfg.h,v 1.1 2013/02/27 09:11:01 devgfi Exp $      Release : $Revision: 1.1 $        Date : $Date: 2013/02/27 09:11:01 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE TANEU * FICHIER aneu_cfg.h
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees de configuration de la tache TANEU.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	22/01/13 : Creation (DEM 1061)  1.1
*
-----------------------------------------------------------------------------------------*/


/* Definitions de types exportes */
/* ----------------------------- */

typedef struct
{
	XDY_NomMachine		NomMachine;
	XDY_NomTache		NomTache;
	XDY_NomSite		NomSite;
	XDY_District		NumeroSite;
}
ANEU_CONFIG_TACHE;



/* Donnees gerees par la tache pour les panneaux PMV et PMVA */

typedef struct
{
	XDY_Eqt			NumeroEqt;
	XDY_NomEqt		NomEqt;
	XDY_Autoroute		NumAuto;
	XDY_PR			PR;
	XDY_Sens		Sens;
	XDY_Etat_Eqt		EtatEqt;
	XDY_Entier		NumAction;
}
ANEU_DONNEES_EQUIPEMENT;



/* Definitions de constantes exportees */
/* ----------------------------------- */



/* Declarations de donnees globales */
/* -------------------------------- */

extern ANEU_CONFIG_TACHE	vg_ConfigNeutr;


/* Liste des PMV du site*/

extern ANEU_DONNEES_EQUIPEMENT		*tg_Liste_PMV;

extern int				vg_Nbr_PMV;



/* Liste des PMVA du site*/

extern ANEU_DONNEES_EQUIPEMENT		*tg_ListePMVA;

extern int				vg_Nb_PMVA;



/* Declaration de fonctions externes */
/* --------------------------------- */

extern int	aneu_cfg_init ( void );
