/*E*/
/*  Fichier : $Id: anet_cfg.c,v 1.5 2021/06/15 17:31:15 devgfi Exp $      Release : $Revision: 1.5 $        Date : $Date: 2021/06/15 17:31:15 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE ANET * FICHIER anet_cfg.c
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de configuration du composant ANET.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	03/04/17 : Creation; lecture des equipements SAGA et de leur etat (DEM 1221)  1.1
* JPL	27/04/17 : Lecture des destinations et zones de mesures en fichier local  1.2
* JPL	10/05/17 : Suppression des PMV[A] sans destination configuree  1.3
* JPL	16/05/17 : Activation des zones de Temps de parcours des destinations  1.4
-----------------------------------------------------------------------------------------*/

/* Fichiers inclus */
/* --------------- */

#include	<xdy.h>
#include	<xdc.h>
#include	<xdf.h>
#include	<xzst.h>
#include	<xzsm.h>

#include	<xzao000.h>
#include	<xzao730.h>
#include	<xzao790.h>

#include	"anet_cyc.h"
#include	"anet_cfg.h"


/* definitions de constantes */

#define	ANET_NB_MAX_PMV_SAGA	50

#define	CM_PATH_DEST_PMV_SAGA	XDC_PATHFIC  "/dra/"
#define	CM_FIC_DEST_PMV_SAGA	CM_PATH_DEST_PMV_SAGA  "destination_pmv_saga.txt"

#define	CM_SEPAR_CHAMPS_DEST	XZAO_TAB_SEPAR
#define	CM_SEPAR_ZONES_DEST	","


/* definitions de types locaux */


/* definition de macro locales */


/* Declaration de variables */
/* ------------------------ */

static char *version = "$Id: anet_cfg.c,v 1.5 2021/06/15 17:31:15 devgfi Exp $ : anet_cfg" ;


ANET_CONFIG_TACHE		vg_ConfigNeutreSaga;


ANET_PMV_A_SAGA	*		tg_Liste_PMV_SAGA = NULL;

int				vg_Nb_PMV_SAGA = 0;


XDY_Booleen			vg_Init_A_Faire = XDC_VRAI;


/* Declaration de variables internes */
/* --------------------------------- */


/* Declaration de fonctions internes */
/* --------------------------------- */


/* Definition de fonctions externes */
/* -------------------------------- */

extern	int	XZAD_Destination_De_Nom (char *, XDY_Destination **);
extern	int	XZAD_ZDP_De_Code (XDY_Entier, XDY_ZDP **);



/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Creation de la liste des PMV / PMVA.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int anet_ajouter_PMV_SAGA ( XZAOT_ConfEqtSAGA	va_Eqt )

/*
* ARGUMENTS EN ENTREE :
*
*   va_Eqt            : Donnees de l'equipement SAGA.
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
*   Au premier appel, creation de la table des PMV / PMVA pour la tache.
*   Ajout du PMV / PMVA a la table.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: anet_cfg.c,v 1.5 2021/06/15 17:31:15 devgfi Exp $ : anet_ajouter_PMV_SAGA" ;

	XZST_03EcritureTrace ( XZSTC_FONCTION, "IN: %s", version);

	/*A Traiter uniquement les PMV et PMVA */
	if ( (va_Eqt.TypeSAGA != XDC_SAGA_PMV_1_18) &&
	     (va_Eqt.TypeSAGA != XDC_SAGA_PMV_2_18) &&
	     (va_Eqt.TypeSAGA != XDC_SAGA_PMV_2_15) &&
	     (va_Eqt.TypeSAGA != XDC_SAGA_PMV_3_15) &&
	     (va_Eqt.TypeSAGA != XDC_SAGA_PMV_2_18_P) &&
	     (va_Eqt.TypeSAGA != XDC_SAGA_PMV_3_15_P) &&
	     (va_Eqt.TypeSAGA != XDC_SAGA_PMVA) )
	{
		return (XDC_OK);
	}


	/*A Au premier appel, allouer la memoire pour la table complete des PMV SAGA */
	if (vg_Nb_PMV_SAGA == 0)
	{
		tg_Liste_PMV_SAGA = (ANET_PMV_A_SAGA *) calloc ( (size_t) ANET_NB_MAX_PMV_SAGA, sizeof (ANET_PMV_A_SAGA) );
		if ( tg_Liste_PMV_SAGA == NULL )
		{
			XZST_03EcritureTrace (XZSTC_FATAL, "OUT: %s Allocation memoire impossible", version);
		}
	}

	/*A Si la table n'a pu etre creee, aucun PMV n'est pris en compte */
	if ( tg_Liste_PMV_SAGA == NULL )
	{
		return (XDC_NOK);
	}


	/*A Ajouter le PMV dans la table et mettre a jour ses donnees */
	tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NumeroEqt = va_Eqt.Numero;
	strcpy (tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NomEqt, va_Eqt.NomEqtSAGA);

	tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].TypeSaga = va_Eqt.TypeSAGA;
	switch (va_Eqt.TypeSAGA) {
		case XDC_SAGA_PMV_1_18 :
			tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NbLignes = 1;
			tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NbCarLigne = 18;
			break;
		case XDC_SAGA_PMV_2_18 :
			tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NbLignes = 2;
			tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NbCarLigne = 18;
			break;
		case XDC_SAGA_PMV_2_15 :
			tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NbLignes = 2;
			tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NbCarLigne = 15;
			break;
		case XDC_SAGA_PMV_3_15 :
			tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NbLignes = 3;
			tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NbCarLigne = 15;
			break;
		case XDC_SAGA_PMV_2_18_P :
			tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NbLignes = 2;
			tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NbCarLigne = 18;
			break;
		case XDC_SAGA_PMV_3_15_P :
			tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NbLignes = 3;
			tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NbCarLigne = 15;
			break;
		case XDC_SAGA_PMVA :
			tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NbLignes = 3;
			tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NbCarLigne = 15;
			break;
		default :
			tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NbLignes = 1;
			tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NbCarLigne = 15;
			break;
	}

	tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].EtatEqt = ANET_EQT_ETAT_INCONNU;
	tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].Affiche.Numero = 0;			/* Marque d'affichage inconnu */
	memset (tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].DestTdP, 0, ANET_MAX_LIG_TDP * sizeof (XZADT_Dest_TDP));


	XZST_03EcritureTrace ( XZSTC_INFO, "PMV SAGA %d <%s>, type %d",
	                          (int) tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NumeroEqt,
	                          tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].NomEqt,
	                          (int) tg_Liste_PMV_SAGA[vg_Nb_PMV_SAGA].TypeSaga);

	vg_Nb_PMV_SAGA = vg_Nb_PMV_SAGA + 1;

	return (XDC_OK);
}




/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Recherche d'un PMV / PMVA par son nom dans la liste.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int anet_PMV_De_Nom (	XDY_Nom			pa_Nom,
				ANET_PMV_A_SAGA		**ppa_PMV )
{
	int	i;

	(*ppa_PMV) = NULL;

	for (i = 0;  i < vg_Nb_PMV_SAGA;  i++) {
		if (strcmp (tg_Liste_PMV_SAGA[i].NomEqt, pa_Nom) == 0) {
			(*ppa_PMV) = &tg_Liste_PMV_SAGA[i];
			return (XDC_OK);
		}
	}

	return (XDC_NOK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Lecture du fichier de configuration des destinations PMV et PMVA SAGA.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int anet_cfg_dest_PMV_SAGA (char *va_fichier_dest)

/*
* ARGUMENTS EN ENTREE :
*
*   va_fichier_dest   : Nom complet du fichier des destinations et zones de mesure.
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
*   Lecture du fichier des destinations PMV et PMVA SAGA et zones de mesure.
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: anet_cfg.c,v 1.5 2021/06/15 17:31:15 devgfi Exp $ : anet_cfg_dest_PMV_SAGA" ;

	FILE		*pl_F_dest;
	XDY_Texte	vl_Ligne;
	char		*pl_curseur;
	int		vl_retcode = 0;
	XDY_Entier	vl_Resultat = 0;
	int		vl_cr;
	int		vl_NbCarsLus;

	XDY_Nom		vl_Nom_Panneau;
	XDY_Nom		vl_Nom_Destination;
	int		vl_Type_Destination;
	char		*pl_Code_Zone;
	int		vl_Code_Zone;

	ANET_PMV_A_SAGA		*pl_PMV = NULL;
	XDY_Destination		*pl_Destination = NULL;
	XDY_ZDP			*pl_ZDP = NULL;
	XZADT_Liste_ZDP		*pl_Graphe = NULL, *pl_zone;

	XZST_03EcritureTrace (XZSTC_INTERFACE, "IN: %s", version);

	if ( (pl_F_dest = fopen (va_fichier_dest, "r")) == NULL) {
		XZST_03EcritureTrace (XZSTC_WARNING, "Fichier %s inaccessible", va_fichier_dest);
		return (XDC_NOK);
	}

	/* Pour chaque ligne du fichier */
	vl_retcode = XZAO000_LireLigneFich (pl_F_dest, vl_Ligne, sizeof (vl_Ligne), &vl_Resultat);
	while ( (vl_retcode == XDC_OK)  &&  (vl_Resultat != XZAO_EOF) ) {
		for (pl_curseur = vl_Ligne;  isspace (*pl_curseur);  pl_curseur++) {
		}

		/* Si la ligne n'est ni vide ni commentee alors */
		if ( (*pl_curseur != XZAO_FIN_STRING)  &&  (*pl_curseur != XDF_COMMENTAIRE) ) {
			vl_cr = XDC_NOK;
			if (sscanf (pl_curseur, "%[^\t]\t%[^\t]\t%d\t%n",
			               vl_Nom_Panneau, vl_Nom_Destination, &vl_Type_Destination, &vl_NbCarsLus) < 3)
			{
				XZST_03EcritureTrace (XZSTC_WARNING, "Declaration incorrecte: %s", pl_curseur);
			} else {
				XDY_Supprimer_Espaces (vl_Nom_Panneau);
				anet_PMV_De_Nom (vl_Nom_Panneau, &pl_PMV);

				XDY_Supprimer_Espaces (vl_Nom_Destination);
				XZAD_Destination_De_Nom (vl_Nom_Destination, &pl_Destination);

				if (pl_PMV == NULL) {
					XZST_03EcritureTrace (XZSTC_WARNING, "PMV[A] SAGA inconnu: %s", vl_Nom_Panneau);
				} else if (pl_Destination == NULL) {
					XZST_03EcritureTrace (XZSTC_WARNING, "PMV %s: destination %s inconnue",
					                         vl_Nom_Panneau, vl_Nom_Destination);
				} else if ( (vl_Type_Destination < 1)  ||  (vl_Type_Destination > pl_PMV->NbLignes) ) {
					XZST_03EcritureTrace (XZSTC_WARNING, "PMV %s: type destination %d errone",
					                         vl_Nom_Panneau, vl_Type_Destination);
				} else {
					vl_cr = XDC_OK;
				}
			}

			if (vl_cr == XDC_OK) {
				/* Analyser les zones de mesure de temps de parcours et construire leur liste  */
				pl_Graphe = NULL;
				pl_Code_Zone = strtok (pl_curseur + vl_NbCarsLus, CM_SEPAR_ZONES_DEST);
				while ( (vl_cr == XDC_OK)  &&  (pl_Code_Zone != NULL) ) {
					if (sscanf (pl_Code_Zone, "%d", &vl_Code_Zone) < 1  ||
					    XZAD_ZDP_De_Code ((XDY_Entier) vl_Code_Zone, &pl_ZDP) != XDC_OK)
					{
						XZST_03EcritureTrace (XZSTC_WARNING, "PMV %s: code zone %s errone",
						                         vl_Nom_Panneau, pl_Code_Zone);
						vl_cr = XDC_NOK;
					} else if ((pl_zone = (XZADT_Liste_ZDP *) calloc ((size_t) 1, sizeof(XZADT_Liste_ZDP))) == NULL) {
						XZST_03EcritureTrace (XZSTC_WARNING, "Allocation memoire impossible !");
						vl_cr = XDC_NOK;
					} else {
						pl_zone->p_zdp = pl_ZDP;
						pl_zone->suivant = pl_Graphe;
						pl_Graphe = pl_zone;
					}
					pl_Code_Zone = strtok (NULL, CM_SEPAR_ZONES_DEST);
				}
				if ( (vl_cr == XDC_OK)  &&  (pl_Graphe == NULL) ) {
					XZST_03EcritureTrace (XZSTC_WARNING, "PMV %s: aucune zone pour destination %s",
					                         vl_Nom_Panneau, vl_Nom_Destination);
					vl_cr = XDC_NOK;
				}
			}

			if (vl_cr == XDC_OK) {
				/* Si la ligne de configuration est correcte alors memoriser la destination et le graphe */
				pl_PMV->DestTdP[vl_Type_Destination - 1].destination = pl_Destination;
				pl_PMV->DestTdP[vl_Type_Destination - 1].graphe = pl_Graphe;

				{	char vl_l_zones[40] = "";
					for (pl_zone = pl_Graphe;  pl_zone != NULL;  pl_zone = pl_zone->suivant) {
						sprintf (vl_l_zones + strlen (vl_l_zones), " %d", pl_zone->p_zdp->Code_Zone);
					}
					XZST_03EcritureTrace (XZSTC_INFO, "PMV %s: destination %d %s, zones%s",
								 vl_Nom_Panneau, vl_Type_Destination, vl_Nom_Destination, vl_l_zones);
				}
			}
		}

		vl_retcode = XZAO000_LireLigneFich (pl_F_dest, vl_Ligne, sizeof (vl_Ligne), &vl_Resultat);
	}

	fclose (pl_F_dest);

	XZST_03EcritureTrace (XZSTC_INTERFACE, "OUT: anet_cfg_dest_PMV_SAGA");

	return (XDC_OK);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Effectue l'initialisation de la tache.
*
*  Lecture en base de donnees des equipements PMV et PMVA.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int anet_cfg_init ()

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
	static char *version = "$Id: anet_cfg.c,v 1.5 2021/06/15 17:31:15 devgfi Exp $ : anet_cfg_init" ;

	int		vl_cr = XDC_OK;
	int		vl_cr_init = XDC_OK;
	XDY_Entier	vl_Resultat = 0;

	/*A
	** Initialiser les tables des equipements panneaux
	*/

	vl_cr = XZAO790_Liste_Eqt_SAGA ( (XDY_FonctionInt) anet_ajouter_PMV_SAGA,
	                                    XDC_BASE_CFG,
	                                    &vl_Resultat );
	if (vl_cr != XDC_OK) {
		vl_cr_init = XDC_NOK;
	}

	vl_cr = XZAT01_Liste_Eqt_Dispo ( (XDY_TypeEqt) XDC_EQT_SAGA,
	                                    (XDY_District) vg_ConfigNeutreSaga.NumeroSite,
	                                    (XDY_Etat_Eqt) XDC_EQT_HS,
	                                    (XDY_Etat_Eqt) XDC_EQT_NON_HS,
	                                    (XDY_Etat_Eqt) 0,
	                                    (XDY_Etat_Eqt) 0,
	                                    (XDY_Horodate) XZSMC_HORODATE_NUL,
	                                    (XDY_FonctionInt) anet_MAJ_etat_PMV_SAGA );
	if (vl_cr != XDC_OK) {
		vl_cr_init = XDC_NOK;
	}

	vg_Init_A_Faire = XDC_FAUX;

	return (vl_cr_init);
}




/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Complement d'initialisation de la tache.
*
*  Lecture de la configuration des destinations et zones de mesure.
*
*  ! Doit etre effectuee APRES l'initialisation du module XZAD,
*    (et donc lecture des destinations et points de mesure) !
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int anet_cfg_init_2 ()
{
	int	vl_nb_PMV = 0;
	int	vl_i_PMV = 0;
	int	vl_nb_destinations = 0;
	int	i;
	int	vl_cr_config;

	/* Lire la configuration des destinations Temps de parcours des PMV SAGA */
	vl_cr_config = anet_cfg_dest_PMV_SAGA (CM_FIC_DEST_PMV_SAGA);

	for (vl_i_PMV = 0;  vl_i_PMV < vg_Nb_PMV_SAGA;  vl_i_PMV++) {
		vl_nb_destinations = 0;
		for (i = 0;  i < ANET_MAX_LIG_TDP;  i++) {
			if (NULL != tg_Liste_PMV_SAGA[vl_i_PMV].DestTdP[i].destination) {
				vl_nb_destinations = vl_nb_destinations + 1;
			}
		}

		/* si une destination est configuree pour ce PMV alors le conserver */
		if (vl_nb_destinations > 0) {
			tg_Liste_PMV_SAGA[vl_nb_PMV] = tg_Liste_PMV_SAGA[vl_i_PMV];
			XZST_03EcritureTrace (XZSTC_DEBUG1, " + PMV %s: %d destinations",
			                         tg_Liste_PMV_SAGA[vl_i_PMV].NomEqt, vl_nb_destinations);
			vl_nb_PMV = vl_nb_PMV + 1;
		}
	}

	/* Mettre a jour le nombre de PMV concernes par les Temps de parcours */
	vg_Nb_PMV_SAGA = vl_nb_PMV;


	/* Activer les zones de Temps de parcours de toutes les destinations des panneaux SAGA */
	for (vl_i_PMV = 0;  vl_i_PMV < vg_Nb_PMV_SAGA;  vl_i_PMV++) {
		for (i = 0;  i < ANET_MAX_LIG_TDP;  i++) {
			XZAD_Activer_Graphe (tg_Liste_PMV_SAGA[vl_i_PMV].DestTdP[i].graphe);
		}
	}

	return (vl_cr_config);
}
