/*E*/
/*  Fichier : $Id: anet_tra.c,v 1.2 2018/05/29 08:33:14 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/05/29 08:33:14 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE ANET * FICHIER anet_tra.c
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de traitement des messages externes du composant ANET.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	07/04/17 : Creation; recueil des etats d'affichage des panneaux SAGA (DEM 1221)  1.1
* JPL	03/05/18 : Format de trace  1.2
-----------------------------------------------------------------------------------------*/

/* Fichiers inclus */
/* --------------- */

#include	"anet_cfg.h"
#include	"anet_tra.h"


/* definitions de constantes */


/* definitions de types locaux */


/* definition de macro locales */


/* Declaration de variables */
/* ------------------------ */

static char *version = "$Id: anet_tra.c,v 1.2 2018/05/29 08:33:14 devgfi Exp $ : anet_tra" ;


/* Declaration de variables internes */
/* --------------------------------- */

static	XDY_Datagroup	vm_DG_Init_SAGA;


/* Declaration de fonctions internes */
/* --------------------------------- */


/* Definition de fonctions externes */
/* -------------------------------- */



/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Memorisation de l'etat d'affichage des equipements PMV / PMVA SAGA.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int anet_MAJ_affichage_PMV_SAGA ( XZEAT_Etat_Eqt_SAGA	va_etat,
                                         int			*pa_resultat )

/*
* ARGUMENTS EN ENTREE :
*
*   va_etat           : Donnees d'etat d'affichage de l'equipement SAGA.
*
* ARGUMENTS EN SORTIE :
*
*   *pa_resultat       : Compte rendu d'execution.
*
* CODE RETOUR :
*
*   XDC_OK en cas d'execution correcte,
*   XDC_NOK sinon (equipement inconnu).
*
*
* CONDITION D'UTILISATION
*
* FONCTION
*
*   Mise a jour des donnnees d'etat d'affichage de l'equipement PMV / PMVA.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: anet_tra.c,v 1.2 2018/05/29 08:33:14 devgfi Exp $ : anet_MAJ_affichage_PMV_SAGA" ;

	int	vl_i_PMV = 0;

	XZST_03EcritureTrace ( XZSTC_FONCTION, "IN: %s numero %d", version, (int) va_etat.Numero);

	*pa_resultat = XDC_OK;

	/*A Rechercher le PMV dans la table par son numero */
	for ( vl_i_PMV = 0;  vl_i_PMV < vg_Nb_PMV_SAGA;  vl_i_PMV++ ) {
		if ( tg_Liste_PMV_SAGA[vl_i_PMV].NumeroEqt == va_etat.Numero ) {
			break;
		}
	}

	if ( vl_i_PMV >= vg_Nb_PMV_SAGA ) {
		/* L'equipement n'est pas un PMV ou un PMVA SAGA */
		*pa_resultat = XDC_NOK;
		return (XDC_NOK);
	}


	/*A Mettre a jour les donnees d'etat du panneau */
	tg_Liste_PMV_SAGA[vl_i_PMV].Affiche = va_etat;


	XZST_03EcritureTrace ( XZSTC_MESSAGE, "Etat PMV SAGA %d: %d-%d-%d-%d-%d %s/%s/%s/%s/%s Picto %s/%s",
	                          (int) tg_Liste_PMV_SAGA[vl_i_PMV].NumeroEqt,
	                          va_etat.Affichage1, va_etat.Affichage2, va_etat.Affichage3, va_etat.Affichage4, va_etat.Affichage5,
	                          va_etat.Texte1, va_etat.Texte2, va_etat.Texte3, va_etat.Texte4, va_etat.Texte5,
	                          va_etat.Picto, va_etat.Bandeau );

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Effectue l'initialisation du module de traitement des messages recus.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int anet_tra_init ()

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
*  Abonnement aux messages d'etat d'affichage des equipements PMV et PMVA SAGA.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: anet_tra.c,v 1.2 2018/05/29 08:33:14 devgfi Exp $ : anet_tra_init" ;

	int		vl_cr = XDC_OK;
	int		vl_res_abonnement = XDC_OK;


	/*A S'abonner aux messages d'etat d'affichage des equipements SAGA */
	XDG_EncodeDG2 (vm_DG_Init_SAGA, XDG_IABTINIT_SAGA, vg_ConfigNeutreSaga.NomMachine);

	vl_cr = XZEA180_Abt_Etat_Eqt_SAGA ( XZEAC_DEBUT_ABONNEMENT,
	                                    vm_DG_Init_SAGA,
	                                    (XDY_Fonction) anet_MAJ_affichage_PMV_SAGA,
	                                    XDC_NOM_SITE_DP,
	                                    &vl_res_abonnement );

	if ((vl_cr != XDC_OK)  ||  (vl_res_abonnement != XDC_OK)) {
		XZST_03EcritureTrace (XZSTC_WARNING, "%s: Echec abonnement aux messages d'etat");
		return (XDC_NOK);
	}

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Effectue les operations de terminaison du module de traitement des messages recus.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int anet_tra_fin ()

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
*  Desabonnement aux messages d'etat d'affichage des equipements PMV et PMVA SAGA.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: anet_tra.c,v 1.2 2018/05/29 08:33:14 devgfi Exp $ : anet_tra_fin" ;

	int		vl_cr = XDC_OK;
	int		vl_res_desabonnement = XDC_OK;


	/*A Se desabonner aux messages d'etat d'affichage des equipements SAGA */
	vl_cr = XZEA180_Abt_Etat_Eqt_SAGA ( XZEAC_FIN_ABONNEMENT,
	                                    vm_DG_Init_SAGA,
	                                    (XDY_Fonction) NULL,
	                                    XDC_NOM_SITE_DP,
	                                    &vl_res_desabonnement );

	if ((vl_cr != XDC_OK)  ||  (vl_res_desabonnement != XDC_OK)) {
		//XZST_03EcritureTrace (XZSTC_WARNING, "%s: Echec desabonnement aux messages d'etat");
		return (XDC_NOK);
	}

	return (XDC_OK);
}
