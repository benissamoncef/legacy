/*E*/
/*  Fichier : $Id: aneu_cfg.c,v 1.1 2013/02/27 09:09:53 devgfi Exp $      Release : $Revision: 1.1 $        Date : $Date: 2013/02/27 09:09:53 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE TANEU * FICHIER aneu_cfg.c
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de configuration de la tache TANEU.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	22/01/13 : Creation (DEM 1061)  1.1
*
-----------------------------------------------------------------------------------------*/

/* Fichiers inclus */
/* --------------- */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>
#include	<xzsm.h>

#include	<xzat01.h>

#include	"aneu_dir.h"
#include	"aneu_cfg.h"


/* definitions de constantes */


/* definitions de types locaux */


/* definition de macro locales */


/* Declaration de variables */
/* ------------------------ */

static char *version = "$Id: aneu_cfg.c,v 1.1 2013/02/27 09:09:53 devgfi Exp $ : aneu_cfg" ;


ANEU_CONFIG_TACHE		vg_ConfigNeutr;


ANEU_DONNEES_EQUIPEMENT	*	tg_Liste_PMV  = NULL;
int				vg_Nbr_PMV = 0;

ANEU_DONNEES_EQUIPEMENT	*	tg_ListePMVA = NULL;
int				vg_Nb_PMVA = 0;


/* Declaration de variables internes */
/* --------------------------------- */


/* Declaration de fonctions internes */
/* --------------------------------- */


/* Definition de fonctions externes */
/* -------------------------------- */



/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Creation de la liste des PMV.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int aneu_ajouter_PMV ( int		va_nb_PMV,
                              int		va_i_PMV,
                              XZATT_EtatEqt	va_PMV )

/*
* ARGUMENTS EN ENTREE :
*
*   va_nb_PMV         : Nombre total d'equipements lus en base ;
*   va_i_PMV          : Numero (rang) de l'equipement fourni ;
*   va_PMV            : Donnees de l'equipement PMV.
*
* ARGUMENTS EN SORTIE :
*
*   Aucun
*
* CODE RETOUR :
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Au premier appel, creation de la table des PMV pour la tache.
*   Ajout du PMV a la table.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: aneu_cfg.c,v 1.1 2013/02/27 09:09:53 devgfi Exp $ : aneu_ajouter_PMV" ;

	XZST_03EcritureTrace ( XZSTC_FONCTION, "IN: %s", version);

	/*A Au premier appel, allouer la memoire pour la table complete des PMV */
	if (va_i_PMV == 0)
	{
		tg_Liste_PMV = (ANEU_DONNEES_EQUIPEMENT *) calloc ( (size_t) va_nb_PMV, sizeof (ANEU_DONNEES_EQUIPEMENT) );
		if ( tg_Liste_PMV == NULL )
		{
			XZST_03EcritureTrace (XZSTC_FATAL, "OUT: %s Allocation memoire impossible", version);
		}
	}

	/*A Si la table n'a pu etre creee, aucun PMV n'est pris en compte */
	if ( tg_Liste_PMV == NULL )
	{
		return (XDC_NOK);
	}


	/*A Ajouter le PMV dans la table et mettre a jour ses donnees */
	tg_Liste_PMV[va_i_PMV].NumeroEqt = va_PMV.NumeroEqt;
	strcpy (tg_Liste_PMV[va_i_PMV].NomEqt, va_PMV.NomEqt);
	tg_Liste_PMV[va_i_PMV].NumAuto = va_PMV.NumAuto;
	tg_Liste_PMV[va_i_PMV].PR = va_PMV.PR;
	tg_Liste_PMV[va_i_PMV].Sens = va_PMV.Sens;
	tg_Liste_PMV[va_i_PMV].EtatEqt = va_PMV.EtatEqt;

	tg_Liste_PMV[va_i_PMV].NumAction = 0;

	vg_Nbr_PMV = vg_Nbr_PMV + 1;


	XZST_03EcritureTrace ( XZSTC_INFO, "PMV %d, <%s>, Loc %d-%d-%d ETAT=%d",
	                          (int) tg_Liste_PMV[va_i_PMV].NumeroEqt,
	                          tg_Liste_PMV[va_i_PMV].NomEqt,
	                          (int) tg_Liste_PMV[va_i_PMV].NumAuto,
	                          (int) tg_Liste_PMV[va_i_PMV].Sens,
	                          (int) tg_Liste_PMV[va_i_PMV].PR,
	                          (int) tg_Liste_PMV[va_i_PMV].EtatEqt);

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Creation de la liste des PMVA.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int aneu_ajouter_PMVA ( int		va_nb_PMVA,
                               int		va_i_PMVA,
                               XZATT_EtatEqt	va_PMVA )
/*
* ARGUMENTS EN ENTREE :
*
*   va_nb_PMVA        : Nombre total d'equipements lus en base ;
*   va_i_PMVA         : Numero (rang) de l'equipement fourni ;
*   va_PMVA           : Donnees de l'equipement PMVA.
*
* ARGUMENTS EN SORTIE :
*
*   Aucun
*
* CODE RETOUR :
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Au premier appel, creation de la table des PMVA pour la tache.
*   Ajout du PMVA a la table.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: aneu_cfg.c,v 1.1 2013/02/27 09:09:53 devgfi Exp $ : aneu_ajouter_PMVA" ;

	XZST_03EcritureTrace ( XZSTC_FONCTION, "IN: %s", version);

	/*A Au premier appel, allouer la memoire pour la table complete des PMVA */
	if (va_i_PMVA == 0)
	{
		tg_ListePMVA = (ANEU_DONNEES_EQUIPEMENT *) calloc ( (size_t) va_nb_PMVA, sizeof (ANEU_DONNEES_EQUIPEMENT) );
		if ( tg_ListePMVA == NULL )
		{
			XZST_03EcritureTrace (XZSTC_FATAL, "OUT: %s Allocation memoire impossible", version);
		}
	}

	/*A Si la table n'a pu etre creee, aucun PMVA n'est pris en compte */
	if ( tg_ListePMVA == NULL )
	{
		return (XDC_NOK);
	}


	/*A Ajouter le PMVA dans la table et mettre a jour ses donnees */
	tg_ListePMVA[va_i_PMVA].NumeroEqt = va_PMVA.NumeroEqt;
	strcpy (tg_ListePMVA[va_i_PMVA].NomEqt, va_PMVA.NomEqt);
	tg_ListePMVA[va_i_PMVA].NumAuto = va_PMVA.NumAuto;
	tg_ListePMVA[va_i_PMVA].PR = va_PMVA.PR;
	tg_ListePMVA[va_i_PMVA].Sens = va_PMVA.Sens;
	tg_ListePMVA[va_i_PMVA].EtatEqt = va_PMVA.EtatEqt;

	tg_ListePMVA[va_i_PMVA].NumAction = 0;

	vg_Nb_PMVA = vg_Nb_PMVA + 1;


	XZST_03EcritureTrace ( XZSTC_INFO, "PMVA %d, <%s>, Loc %d-%d-%d ETAT=%d",
	                          (int) tg_ListePMVA[va_i_PMVA].NumeroEqt,
	                          tg_ListePMVA[va_i_PMVA].NomEqt,
	                          (int) tg_ListePMVA[va_i_PMVA].NumAuto,
	                          (int) tg_ListePMVA[va_i_PMVA].Sens,
	                          (int) tg_ListePMVA[va_i_PMVA].PR,
	                          (int) tg_ListePMVA[va_i_PMVA].EtatEqt);

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Effectue l'initialisation de la tache.
*
*  Lecture en base de donnees des equipements PMV et PMVA (avec leur etat).
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int aneu_cfg_init ()

/*
* ARGUMENTS EN ENTREE :
*
*   Aucun
*
* ARGUMENTS EN SORTIE :
*
*   Aucun
*
* CODE RETOUR :
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Lecture en base de la liste des equipements PMV et PMVA
*   et remplissage des tables associees.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: aneu_cfg.c,v 1.1 2013/02/27 09:09:53 devgfi Exp $ : aneu_cfg_init" ;

	int	vl_cr = XDC_OK;
	int	vl_cr_init = XDC_OK;

	/*A
	** Initialiser les tables des equipements panneaux
	*/

	vl_cr = XZAT01_Liste_Eqt_Dispo ( (XDY_TypeEqt) XDC_EQT_PMV,
	                                    (XDY_District) vg_ConfigNeutr.NumeroSite,
	                                    (XDY_Etat_Eqt) XDC_EQT_HS,
	                                    (XDY_Etat_Eqt) XDC_EQT_NON_HS,
	                                    (XDY_Etat_Eqt) 0,
	                                    (XDY_Etat_Eqt) 0,
	                                    (XDY_Horodate) XZSMC_HORODATE_NUL,
	                                    (XDY_FonctionInt) aneu_ajouter_PMV );
	if (vl_cr != XDC_OK) {
		vl_cr_init = XDC_NOK;
	}

	vl_cr = XZAT01_Liste_Eqt_Dispo ( (XDY_TypeEqt) XDC_EQT_PMVA,
	                                    (XDY_District) vg_ConfigNeutr.NumeroSite,
	                                    (XDY_Etat_Eqt) XDC_EQT_HS,
	                                    (XDY_Etat_Eqt) XDC_EQT_NON_HS,
	                                    (XDY_Etat_Eqt) 0,
	                                    (XDY_Etat_Eqt) 0,
	                                    (XDY_Horodate) XZSMC_HORODATE_NUL,
	                                    (XDY_FonctionInt) aneu_ajouter_PMVA );
	if (vl_cr != XDC_OK) {
		vl_cr_init = XDC_NOK;
	}

	return (vl_cr_init);
}
