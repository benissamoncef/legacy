/*E*/
/* Fichier : $Id: ereg_ajuste.c,v 1.3 2013/04/12 09:35:46 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2013/04/12 09:35:46 $
 ------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
 ------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
 ------------------------------------------------------
 * MODULE TEREG * FICHIER ereg_ajuste.c
 ------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * < Module de general de ajuste de la regulation >
 *
 ------------------------------------------------------
 * HISTORIQUE :
 *
 * SDU		13/03/2012 : Creation DEM/1016
 * JMG		24/07/12 : correction VSR 1.2
 ------------------------------------------------------*/

/* fichiers inclus */
#include "ereg_ajuste.h"
#include "ereg_trace.h"
#include "xzia.h"

/* definitions de constantes */
#define AJUSTE_NIV_TRACE_1	XZSTC_INFO
#define AJUSTE_NIV_TRACE_2 	XZSTC_DEBUG1
#define AJUSTE_VITESSE_INV	200

/* declaration de variables externes */
extern T_Param tg_params[EREG_NB_PARAM_MAX];

/* definitions de types locaux */
typedef struct MAILLON_LISTE_CHAINEE {
	void *ptr;
	T_ZONE_REGUL *Zone;
	struct MAILLON_LISTE_CHAINEE *suivant;
	struct MAILLON_LISTE_CHAINEE *precedent;
} T_LISTE_CHAINEE;

typedef struct Evenement {
	XDY_Entier numero;
	XDY_Entier cle;
	XDY_Entier type;
	XDY_Booleen qualification;
	XDY_Voie etatVL;
	XDY_Voie etatVM1;
	XDY_Voie etatVM2;
	XDY_Voie etatVR;
	XDY_Voie etatVL_I;
	XDY_Voie etatVM1_I;
	XDY_Voie etatVM2_I;
	XDY_Voie etatVR_I;
	XDY_Booleen necessiteQualificationOP;
	XDY_Booleen enCours;
	XDY_Booleen misAJour;
} T_Evenement;

typedef struct Chantier {
	XDY_Entier numero;
	XDY_Entier cle;
	XDY_Entier type;
	XDY_Booleen enCours;
	XDY_VitesseRegul vitesse;
	XDY_PR prDeb;
	XDY_PR prFin;
} T_Chantier;

/* definition de macro locales */

/* declaration de variables locales */
T_LISTE_CHAINEE *pg_liste_evts_impactants = NULL;
T_LISTE_CHAINEE *pg_liste_chantiers = NULL;
T_ZONE_REGUL *pg_zone_en_cours;

/* declaration de variables externes */
extern XDY_Basedd vg_Base;
extern XDY_Booleen vg_ModeSimu;
extern XDY_Horodate vg_HorodateSimu;
extern XDY_NomSite vg_NomSite;
extern XDY_NomMachine vg_NomMachine;
extern T_Liste_ZonesRegul *pg_Zones;
extern T_Liste_PRV *pg_PRV;
extern XDY_Booleen vg_tebeg;
extern XDY_Horodate vg_Horodate_Cycle;

/* declaration de fonctions internes */
void ajuste_recherche_evt(T_ZONE_REGUL *pa_Zone);
void ajuste_recherche_chantiers(T_ZONE_REGUL *pa_Zone);
XDY_Booleen ajuste_analyse_evt_impactants(T_ZONE_REGUL *pa_zone);
XDY_Booleen ajuste_analyse_pannes(T_ZONE_REGUL *pa_Zone);
void ajuste_analyse_chantiers(T_ZONE_REGUL *pa_Zone);
int ajuste_verifie_vitesse_chantier(T_Chantier *pa_chantier,
		T_ZONE_REGUL *pa_zone, XDY_Booleen *pa_regulationModifiee);
int ajuste_verifie_prv_amont(T_Chantier *pa_chantier, T_ZONE_REGUL *pa_zone);
int ajuste_verifie_vitesse_fin(T_Chantier *pa_chantier, T_ZONE_REGUL *pa_zone);
int
		ajuste_verifie_decrement_fin(T_Chantier *pa_chantier,
				T_ZONE_REGUL *pa_zone);

/* fonctions internes */
int memoriser_evts_impactants(XDY_Entier va_numero, XDY_Entier va_cle,
		XDY_Entier va_type, XDY_Booleen va_qualification, XDY_Voie va_etatVL,
		XDY_Voie va_etatVM1, XDY_Voie va_etatVM2, XDY_Voie va_etatVR,
		XDY_Voie va_etatVL_I, XDY_Voie va_etatVM1_I, XDY_Voie va_etatVM2_I,
		XDY_Voie va_etatVR_I, XDY_Booleen va_avec_qualification,
		XDY_Entier *pa_cr) {
	T_Evenement *pl_evenement, *pl_ptrEvt;
	T_LISTE_CHAINEE *pl_liste_evts;

	XZST_03EcritureTrace(XZSTC_FONCTION,
			"IN : memoriser_evts_impactants %d %d", va_numero, va_cle);

	pl_evenement = malloc(sizeof(T_Evenement));

	pl_evenement->numero = va_numero;
	pl_evenement->cle = va_cle;
	pl_evenement->type = va_type;
	pl_evenement->qualification = va_qualification;
	pl_evenement->necessiteQualificationOP = va_avec_qualification;
	pl_evenement->etatVL = va_etatVL;
	pl_evenement->etatVM1 = va_etatVM1;
	pl_evenement->etatVM2 = va_etatVM2;
	pl_evenement->etatVR = va_etatVR;
	pl_evenement->etatVL_I = va_etatVL_I;
	pl_evenement->etatVM1_I = va_etatVM1_I;
	pl_evenement->etatVM2_I = va_etatVM2_I;
	pl_evenement->etatVR_I = va_etatVR_I;
	pl_evenement->enCours = XDC_VRAI;
	pl_evenement->misAJour = XDC_VRAI;

	/* rechercher evenement du cycle precedent */
	pl_liste_evts = pg_liste_evts_impactants;
	while (pl_liste_evts != NULL) {
		XZST_03EcritureTrace(XZSTC_INFO, "%d %d",
				((T_Evenement *) pl_liste_evts->ptr)->numero,
				((T_Evenement *) pl_liste_evts->ptr)->cle);
		if (((T_Evenement *) pl_liste_evts->ptr)->cle == pl_evenement->cle
				&& ((T_Evenement *) pl_liste_evts->ptr)->numero
						== pl_evenement->numero)
			break;
		else
			pl_liste_evts = pl_liste_evts->suivant;
	}

	if (pl_liste_evts != NULL) { /* evenement existant au cycle precedent*/
		pl_ptrEvt = (T_Evenement *) pl_liste_evts->ptr;
		/* Si Qualif ou emprise chaussee differente */
		if (pl_ptrEvt->qualification != pl_evenement->qualification
				|| pl_ptrEvt->etatVL != pl_evenement->etatVL
				|| pl_ptrEvt->etatVM1 != pl_evenement->etatVM1
				|| pl_ptrEvt->etatVM2 != pl_evenement->etatVM2
				|| pl_ptrEvt->etatVR != pl_evenement->etatVR
				|| pl_ptrEvt->etatVL_I != pl_evenement->etatVL_I
				|| pl_ptrEvt->etatVM1_I != pl_evenement->etatVM1_I
				|| pl_ptrEvt->etatVM2_I != pl_evenement->etatVM2_I
				|| pl_ptrEvt->etatVR_I != pl_evenement->etatVR_I) {

			pl_evenement->misAJour = XDC_VRAI;
			pl_evenement->enCours = XDC_VRAI;

			free(pl_liste_evts->ptr);
			pl_liste_evts->ptr = pl_evenement;

		} else {
			pl_ptrEvt->misAJour = XDC_FAUX;
			pl_ptrEvt->enCours = XDC_VRAI;
			free(pl_evenement);
		}

	} else { /* evenement inexistant */
		/* insertion en tete */
		pl_liste_evts = malloc(sizeof(T_LISTE_CHAINEE));
		pl_liste_evts->ptr = pl_evenement;
		pl_liste_evts->Zone = pg_zone_en_cours;
		pl_liste_evts->suivant = pg_liste_evts_impactants;
		pl_liste_evts->precedent = NULL;
		if (pg_liste_evts_impactants != NULL)
			pg_liste_evts_impactants->precedent = pl_liste_evts;
		pg_liste_evts_impactants = pl_liste_evts;
	}

	*pa_cr = XDC_OK;

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : memoriser_evts_impactants");
	return XDC_OK;
}

int memoriser_chantier(XDY_Entier va_numero, XDY_Entier va_cle,
		XDY_Entier va_type, XDY_PR va_prDebut, XDY_PR va_prFin,
		XDY_Entier va_vitesse, XDY_Entier *pa_cr) {
	T_Chantier *pl_chantier, *pl_ptrChantier;
	T_LISTE_CHAINEE *pl_liste_chantiers;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : memoriser_chantier");

	pl_chantier = malloc(sizeof(T_Chantier));

	pl_chantier->numero = va_numero;
	pl_chantier->cle = va_cle;
	pl_chantier->type = va_type;
	pl_chantier->enCours = XDC_VRAI;
	pl_chantier->prDeb = va_prDebut;
	pl_chantier->prFin = va_prFin;
	pl_chantier->vitesse = va_vitesse;

	/* chantier inexistant */
	/* insertion en tete */
	pl_liste_chantiers = malloc(sizeof(T_LISTE_CHAINEE));
	pl_liste_chantiers->ptr = pl_chantier;
	pl_liste_chantiers->Zone = pg_zone_en_cours;
	pl_liste_chantiers->suivant = pg_liste_chantiers;
	pl_liste_chantiers->precedent = NULL;
	if (pg_liste_chantiers != NULL)
		pg_liste_chantiers->precedent = pl_liste_chantiers;
	pg_liste_chantiers = pl_liste_chantiers;

	*pa_cr = XDC_OK;

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : memoriser_chantier");
	return XDC_OK;
}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Ajustement du scenario de regulation
 ------------------------------------------------------
 * SEQUENCE D'APPEL :
 */
int ajuste_analyse(T_ZONE_REGUL *pa_Zone) {
	/*
	 * ARGUMENTS EN ENTREE :
	 *  T_ZONE_REGUL *pa_Zone : pointeur sur la zone de regulation � ajuster
	 *
	 * ARGUMENTS EN SORTIE :
	 *  Aucun
	 *
	 * CODE RETOUR :
	 *  XDC_OK
	 *  XDC_NOK
	 *
	 * CONDITION D'UTILISATION
	 *  aucune
	 *
	 * FONCTION
	 *   recherche des evenements impactants
	 *   recherche des chantier
	 *   analyse des evenement impactants et ajustement du scenario choisi si besoin
	 *   analyse des chantiers et ajustement du scenario choisi si besoin
	 *
	 ------------------------------------------------------*/
	T_Liste_ZonesRegul *pl_maillon;
	T_Liste_PRV *pl_listePRV = NULL;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : ajuste_analyse zone=%s",
			pa_Zone->nom_zone);

	XZST_03EcritureTrace(XZSTC_DEBUG1, "##### AJUSTEMENT #####");
	if (vg_tebeg) {
	 XZST_03EcritureTrace( XZSTC_WARNING,"arret apres detection!!!");
	 return (XDC_OK);
	 }

	if (pa_Zone->regulation_proposee.scenario.nominal != XDC_VRAI) {
		ajuste_recherche_evt(pa_Zone);

		ajuste_recherche_chantiers(pa_Zone);

		if (ajuste_analyse_evt_impactants(pa_Zone) == XDC_VRAI) {
			/* poursuivre */
			if (ajuste_analyse_pannes(pa_Zone) == XDC_VRAI) {
				ajuste_analyse_chantiers(pa_Zone);
			}
		}
	}

	XZST_03EcritureTrace(XZSTC_INFO, "AJUSTEMENT SCENARIO \t scenario propose : %d",
								pa_Zone->regulation_proposee.scenario.id);

XZST_03EcritureTrace(XZSTC_INFO,"AFFICHAGE DU CONTEXTE : chantier=%s, declenchement H=%s, evt impact=%s, panne PRV Crit = %s, panne PRV non Crit = %s, qualif ope = %s, scenario ideal = %s",
				pa_Zone->regulation_proposee.contexte.chantiers ==XDC_VRAI ? "VRAI" : "FAUX",
				pa_Zone->regulation_proposee.contexte.declenchement_horaire ==XDC_VRAI ? "VRAI" : "FAUX",
				pa_Zone->regulation_proposee.contexte.evt_impactant ==XDC_VRAI ? "VRAI" : "FAUX",
				pa_Zone->regulation_proposee.contexte.panne_PRV_critique ==XDC_VRAI ? "VRAI" : "FAUX",
				pa_Zone->regulation_proposee.contexte.panne_PRV_non_critique ==XDC_VRAI ? "VRAI" : "FAUX",
				pa_Zone->regulation_proposee.contexte.qualification_operateur ==XDC_VRAI ? "VRAI" : "FAUX",
				pa_Zone->regulation_proposee.contexte.scenario_ideal ==XDC_VRAI ? "VRAI" : "FAUX");


	valide_scenario(pa_Zone);

	XZST_03EcritureTrace(XZSTC_DEBUG1, "##### FIN AJUSTEMENT #####");
	return XDC_OK;

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : ajuste_analyse");

}

void ajuste_recherche_evt(T_ZONE_REGUL *pa_Zone)
/*
 * ARGUMENTS EN ENTREE :
 *  T_ZONE_REGUL *pa_Zone : pointeur sur la zone de regulation � concern�e
 *
 * ARGUMENTS EN SORTIE :
 *  Aucun
 *
 * CODE RETOUR :
 *  Aucun
 *
 * CONDITION D'UTILISATION
 *  aucune
 *
 * FONCTION
 *	recherche les evenements en cours sur la zone de regulation via XZAE562_Evts_impactants_En_Cours
 *	la fonction memoriser_evts_impactants est utilis�e pour memoriser les evenements renvoy�s par la procdeure stockee XZAE562
 ------------------------------------------------------*/
{
	XDY_Entier vl_result;
	T_LISTE_CHAINEE *pl_liste_evts;
	T_LISTE_CHAINEE *pl_liste_evts_suivant;
	T_Evenement *pl_evt;
	int		vl_jourSemaine;
	XDY_Horodate	vl_horodate;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : ajuste_recherche_evt zone [%s]",
			pa_Zone->nom_zone);

	/* memorisation pour la fct memoriser_evts_impactants()*/
	pg_zone_en_cours = pa_Zone;

	/* Parcours des evenemet pour les noter non en cours*/
	pl_liste_evts = pg_liste_evts_impactants;
	while (pl_liste_evts != NULL) {
		((T_Evenement *) pl_liste_evts->ptr)->enCours = XDC_FAUX;
		pl_liste_evts = pl_liste_evts->suivant;
	}

	if(vg_ModeSimu == XDC_FAUX) {
		if(XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME ,
					(int *) &vl_jourSemaine,
					(double *) &vl_horodate) != XDC_OK){
			XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de recuperer l horodate courante") ;
			return;
		}
	}
	else
		vl_horodate = vg_HorodateSimu;

	XZST_03EcritureTrace(XZSTC_INFO, "562 : %d %d %d %d",
			pa_Zone->autoroute_prv, pa_Zone->sens_prv, pa_Zone->pr_debut, pa_Zone->pr_fin);

	/*XZAE562_Evts_impactants_En_Cours(vg_Horodate_Cycle, pa_Zone->autoroute_prv,
			pa_Zone->sens_prv, pa_Zone->pr_debut, pa_Zone->pr_fin,
			memoriser_evts_impactants);*/
	XZAE562_Evts_impactants_En_Cours(vl_horodate, pa_Zone->autoroute_prv,
				pa_Zone->sens_prv, pa_Zone->pr_debut, pa_Zone->pr_fin,
				memoriser_evts_impactants);


	/* Parcours des evenements non mis à jour*/
	pl_liste_evts = pg_liste_evts_impactants;
	while (pl_liste_evts != NULL) {
		pl_evt = (T_Evenement *) pl_liste_evts->ptr;
		if (pl_evt->enCours == XDC_FAUX) {

			pl_liste_evts_suivant = pl_liste_evts->suivant;
			/* suppression maillon pl_liste_evts*/
			if (pl_liste_evts->precedent == NULL) {
				/* suppression en tete */
				pg_liste_evts_impactants = pl_liste_evts->suivant;
				if (pg_liste_evts_impactants != NULL)
					pg_liste_evts_impactants->precedent = NULL;
			} else {
				/* suppression */
				pl_liste_evts->precedent->suivant = pl_liste_evts->suivant;
				if (pl_liste_evts->suivant != NULL)
					pl_liste_evts->suivant->precedent
							= pl_liste_evts->precedent;
			}
			free(pl_liste_evts->ptr);
			free(pl_liste_evts);

			pl_liste_evts = pl_liste_evts_suivant;
		} else
			pl_liste_evts = pl_liste_evts->suivant;
	}

	pg_zone_en_cours = NULL;

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : ajuste_recherche_evt");
}

XDY_Booleen ajuste_analyse_evt_impactants(T_ZONE_REGUL *pa_zone) {
	/*
	 * ARGUMENTS EN ENTREE :
	 *  aucun
	 *
	 * ARGUMENTS EN SORTIE :
	 *  Aucun
	 *
	 * CODE RETOUR :
	 *  XDC_OK
	 *  XDC_NOK
	 *
	 * CONDITION D'UTILISATION
	 *  aucune
	 *
	 * FONCTION
	 * analyse les nouveaux evenements impactants ou ceuw mis � jour non encore trait�s.
	 ------------------------------------------------------*/
	T_LISTE_CHAINEE *pl_liste;
	T_Evenement *pl_evt;
	T_ZONE_REGUL *pl_zone;
	int vl_cr;
	XDY_Booleen vl_evt_impactant_sans_qualif = XDC_FAUX;
	XDY_Booleen vl_presence_evt_mis_a_jour = XDC_FAUX;
	XDY_Booleen vl_evt_impactant = XDC_FAUX;
	XDY_Booleen vl_poursuivre = XDC_VRAI;
	XDY_Nom vl_trace;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : ajuste_analyse_evt_impactants");
	XZST_03EcritureTrace(AJUSTE_NIV_TRACE_1, "   ... analyse Evts");

	/* Parcours des Evenements */
	pl_liste = pg_liste_evts_impactants;
	while (pl_liste != NULL) {
		if (pl_liste->Zone->numero_zone == pa_zone->numero_zone) {
			pl_evt = (T_Evenement *) pl_liste->ptr;
			pl_zone = pl_liste->Zone;

			/*evt sans besoin de qualif operateur*/
			if (pl_evt->necessiteQualificationOP == XDC_FAUX) {
				vl_evt_impactant_sans_qualif = XDC_VRAI;

				XZST_03EcritureTrace(XZSTC_WARNING, ".-.-. ajuste_analyse_evt_impactants : tracer le process d'ajustement.-.-.");
				if (trace_process(EREG_TRACE_TYPE_AJUSTEMENT,
						pa_zone->numero_zone, 0, 0, pl_evt->numero,
						pl_evt->cle, (*(pl_zone->scenarioNominal)).id, 0,
						0,0,
						"Evt Impactant sans qualification operateur") == XDC_NOK) {
					XZST_03EcritureTrace(XZSTC_WARNING,
							"ajuste_analyse_evt_impactants : Impossible de tracer le process d'ajustement");
				}
			}
			else {
				vl_evt_impactant = XDC_VRAI;
				if (pl_evt->misAJour == XDC_VRAI) { /*evt a qualifier et qui a ete mis a jour*/
					vl_presence_evt_mis_a_jour = XDC_VRAI;
					pl_zone->regulation_proposee.contexte.alerte_deja_envoyee = XDC_FAUX;
					if (trace_process(EREG_TRACE_TYPE_AJUSTEMENT,
							pa_zone->numero_zone, 0, 0, pl_evt->numero,
							pl_evt->cle, (*(pa_zone->scenarioNominal)).id, 0,
							0,0,
							"Evt Impactant avec qualification operateur") == XDC_NOK) {
						XZST_03EcritureTrace(XZSTC_WARNING,
								"ajuste_analyse_evt_impactants : Impossible de tracer le process d'ajustement");
					}
				}
				else {
					XZST_03EcritureTrace(XZSTC_WARNING,"prop %d courant %d",
							pa_zone->regulation_proposee.scenario.id,
							pa_zone->regulation.scenario.id);
					if ( (pa_zone->regulation_proposee.scenario.id != pa_zone->regulation.scenario.id) &&
							(pa_zone->regulation.scenario.id == (*(pa_zone->scenarioNominal)).id) )
						vl_presence_evt_mis_a_jour = XDC_VRAI;
				}
			}
			XZST_03EcritureTrace(AJUSTE_NIV_TRACE_1,
					"     > Evt %d/%d  - Qualif OP %s - maj %s ",
					pl_evt->numero, pl_evt->cle,
					pl_evt->necessiteQualificationOP == XDC_VRAI ? "VRAI"
							: "FAUX", pl_evt->misAJour == XDC_VRAI ? "VRAI"
							: "FAUX");
		}
		/* Suivant */
		pl_liste = pl_liste->suivant;
	}

	if (vl_evt_impactant_sans_qualif == XDC_VRAI) {
		XZST_03EcritureTrace(AJUSTE_NIV_TRACE_1,
				">>>> AJUSTEMENT : Presence Evenement Impactant : DEMANDE ARRET REGULATION");

		pl_zone->regulation_proposee.scenario = *(pl_zone->scenarioNominal);
		pl_zone->regulation_proposee.contexte.evt_impactant = XDC_VRAI;
		pl_zone->regulation_proposee.contexte.qualification_operateur
				= XDC_FAUX;
		pl_zone->regulation_proposee.mesure = EREG_MESURE_ARRET;
		pl_zone->regulation_proposee.indicateur_horaire = XDC_FAUX;
		pl_zone->regulation_proposee.reponse_operateur = XDC_ALT_INCONNUE;
		pl_zone->regulation_proposee.contexte.scenario_ideal = XDC_FAUX;
		/*trace*/
/*		if (trace_process(EREG_TRACE_TYPE_AJUSTEMENT, pa_zone->numero_zone, 0,
				0, pl_evt->numero, pl_evt->cle,
				(*(pl_zone->scenarioNominal)).id, 0,
				"Evt Impactant sans qualification operateur") == XDC_NOK) {
			XZST_03EcritureTrace(XZSTC_WARNING,
					"ajuste_analyse_evt_impactants : Impossible de tracer le process d'ajustement");
		}
*/
		vl_poursuivre = XDC_FAUX;
	} else if (vl_evt_impactant == XDC_VRAI){
		XZST_03EcritureTrace(AJUSTE_NIV_TRACE_1,
				">>>> AJUSTEMENT : Evt impactant Mis A Jour: DEMANDE QUALIF OPERATEUR");
		if (vl_presence_evt_mis_a_jour==XDC_VRAI) {
			pl_zone->regulation_proposee.scenario = *(pl_zone->scenarioNominal);
			pl_zone->regulation_proposee.contexte.evt_impactant = XDC_VRAI;
			pl_zone->regulation_proposee.contexte.qualification_operateur
				= XDC_VRAI;
			pl_zone->regulation_proposee.contexte.scenario_ideal = XDC_FAUX;
			pl_zone->regulation_proposee.mesure = EREG_MESURE_ARRET;
		}
		pl_zone->regulation_proposee.indicateur_horaire = XDC_FAUX;
		pl_zone->regulation_proposee.reponse_operateur = XDC_ALT_INCONNUE;
		pl_zone->regulation_proposee.contexte.evt_mis_a_jour = vl_presence_evt_mis_a_jour;
		if (vl_presence_evt_mis_a_jour)
			vl_poursuivre = XDC_FAUX;
		else
			vl_poursuivre = XDC_VRAI;
	} else
		vl_poursuivre = XDC_VRAI;

	XZST_03EcritureTrace(XZSTC_FONCTION,
			"OUT : ajuste_analyse_evt_impactants = %d", vl_poursuivre);

	return vl_poursuivre;
}

void ajuste_recherche_chantiers(T_ZONE_REGUL *pa_Zone) {
	/*
	 * ARGUMENTS EN ENTREE :
	 *  T_ZONE_REGUL *pa_Zone : pointeur sur la zone de regulation � traiter
	 *
	 * ARGUMENTS EN SORTIE :
	 *  Aucun
	 *
	 * CODE RETOUR :
	 *  Aucun
	 *
	 * CONDITION D'UTILISATION
	 *  aucune
	 *
	 * FONCTION
	 *	recherche les chantiers en cours sur la zone de regulation via XZAE563_Chantiers_En_Cours
	 *	la fonction memoriser_chantier est utilis�e pour memoriser les chantiers renvoy�s par la procedure stockee XZAE563
	 ------------------------------------------------------*/
	XDY_Entier vl_result;
	T_LISTE_CHAINEE *pl_liste_chantiers;
	T_LISTE_CHAINEE *pl_liste_chantiers_suivant;
	int	vl_jourSemaine;
	XDY_Horodate	vl_horodate;

	XZST_03EcritureTrace(XZSTC_INFO,
			"IN : ajuste_recherche_chantiers zone [%s]", pa_Zone->nom_zone);

	/* memorisation pour la fct memoriser_chantier()*/
	pg_zone_en_cours = pa_Zone;

	/* liberation pg_liste_chantiers*/
	while (pg_liste_chantiers != NULL) {
		/* suppression en tete*/
		pl_liste_chantiers = pg_liste_chantiers;
		pg_liste_chantiers = pg_liste_chantiers->suivant;

		free((T_Chantier *) pl_liste_chantiers->ptr);
		free(pl_liste_chantiers);
	}

	if(vg_ModeSimu == XDC_FAUX) {
		if(XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME ,
					(int *) &vl_jourSemaine,
					(double *) &vl_horodate) != XDC_OK){
			XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de recuperer l horodate courante") ;
			return;
		}
	}
	else
		vl_horodate = vg_HorodateSimu;

	/*XZAE563_Chantiers_En_Cours(vg_Horodate_Cycle, pa_Zone->autoroute_prv,
			pa_Zone->sens_prv, pa_Zone->pr_debut, pa_Zone->pr_fin,
			memoriser_chantier);*/
	XZAE563_Chantiers_En_Cours(vl_horodate, pa_Zone->autoroute_prv,
				pa_Zone->sens_prv, pa_Zone->pr_debut, pa_Zone->pr_fin,
				memoriser_chantier);


	pg_zone_en_cours = NULL;

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : ajuste_recherche_chantiers");
}

XDY_Booleen ajuste_analyse_pannes(T_ZONE_REGUL *pa_Zone) {
	/*
	 * ARGUMENTS EN ENTREE :
	 *  T_ZONE_REGUL *pa_Zone : pointeur sur la zone de regulation � traiter
	 *
	 * ARGUMENTS EN SORTIE :
	 *  Aucun
	 *
	 * CODE RETOUR :
	 *  XDC_OK
	 *  XDC_NOK
	 *
	 * CONDITION D'UTILISATION
	 *  aucune
	 *
	 * FONCTION
	 *
	 ------------------------------------------------------*/
	T_Liste_PRV *pl_listePRV;
	T_SCENARIO *pl_Scenario;
	T_Liste_Scenario *pl_listeScenario;
	XDY_Octet vl_DispoPRV; /* XDY_ETAT_EQT */
	XDY_Octet vl_TypePRV;
	XDY_Bandeau vl_Bandeau;
	XDY_VitesseRegulTxt vl_Vitesse;
	XDY_Entier vl_NumEvt;
	XDY_Octet vl_CleEvt;
	XDY_Octet vl_Priorite;
	XDY_Entier vl_DistanceEvt;
	XDY_Entier vl_Action;
	XDY_NomSite vl_SiteAction;
	XDY_Booleen vl_prvMarche = XDC_FAUX;
	XDY_Booleen vl_poursuivre = XDC_VRAI;
	XDY_Booleen vl_prv_critique_hs = XDC_FAUX;
	char vl_str[50];
	XZIAT_destinataire vl_destinataire;

	XZST_03EcritureTrace(XZSTC_INFO, "IN : ajuste_analyse_pannes [%s]",
			pa_Zone->nom_zone);
	XZST_03EcritureTrace(XZSTC_DEBUG1, "   ... analyse des pannes");
	pl_listePRV = pa_Zone->ListePRV;


	while (pl_listePRV != NULL) {
		if (vg_ModeSimu == XDC_VRAI)
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,"numero_prv : %d", pl_listePRV->prv.numero_prv);
			XZST_03EcritureTrace(XZSTC_DEBUG1,"date simu : %lf", vg_HorodateSimu);

			if (XZAT100_Utilisation_PRV(vg_HorodateSimu,
					pl_listePRV->prv.numero_prv, &vl_DispoPRV, &vl_TypePRV,
					&vl_Bandeau, &vl_Vitesse, &vl_NumEvt, &vl_CleEvt,
					&vl_Priorite, &vl_DistanceEvt, &vl_Action, &vl_SiteAction)
					!= XDC_OK) {
				XZST_03EcritureTrace(XZSTC_WARNING,
						"ajuste_analyse_pannes  : Erreur dans XZAT100_Utilisation_PRV");
				vl_prvMarche = XDC_VRAI;
			} else
				vl_prvMarche = (vl_DispoPRV == XDC_OK);
		} else
			vl_prvMarche = pl_listePRV->prv.etat_de_marche;


		if (vl_prvMarche == XDC_FAUX) {
			XZST_03EcritureTrace(XZSTC_INFO, "PRV %d (%d) etat %d en PANNE",
					pl_listePRV->prv.numero_prv, pl_listePRV->prv.pr_prv,
					vl_prvMarche);

			/* Etat NON OK*/

			/* Si PRV critique pour le scenario nominal */
			if (pl_listePRV->prv.critique_prv == XDC_VRAI) {
			/*
			if (est_prv_critique_pour_scenario(pl_listePRV->prv,
					*(pa_Zone->scenarioNominal)) == XDC_VRAI) {*/
				XZST_03EcritureTrace(XZSTC_INFO,
						"PRV critique pour le scenario nominal, en panne",
						pl_listePRV->prv.numero_prv);
				sprintf(vl_str,"PRV %d panne",pl_listePRV->prv.numero_prv);
				/*Todo : passer la regul en mode degradé*/
				pa_Zone->regulation_proposee.contexte.panne_PRV_critique
						= XDC_VRAI;
				pa_Zone->regulation_proposee.contexte.scenario_ideal
						= XDC_FAUX;

				pa_Zone->regulation_proposee.scenario
						= *(pa_Zone->scenarioNominal);

				pa_Zone->regulation_proposee.mesure = EREG_MESURE_ARRET;
				vl_poursuivre = XDC_FAUX;

				if (pa_Zone->degrade==XDC_FAUX) {
					passer_zone_degrade(pa_Zone, XDC_VRAI,vl_str);

					if (trace_process(EREG_TRACE_TYPE_AJUSTEMENT,
							pa_Zone->numero_zone, 0, 0, 0, 0,
							pa_Zone->regulation_proposee.scenario.id, 0,
							0,0,
							"PANNE du PRV %d CRITIQUE pour scenario nominal",
							pl_listePRV->prv.numero_prv) == XDC_NOK) {
						XZST_03EcritureTrace(XZSTC_WARNING,
								"ajuste_analyse_evt_impactants : Impossible de tracer le process d'ajustement");
					}
				}

				return vl_poursuivre;
			}

			/* Todo : Si PRV critique pour scenario propose */
			else if (est_prv_critique_pour_scenario(pl_listePRV->prv,
					pa_Zone->regulation_proposee.scenario)) {
				pl_listeScenario = pa_Zone->ListeScenario;
				/* on se positionne sur le scenario le plus regule mais inferieur au niveau de regulation du scenario propose */
				while (pl_listeScenario != NULL) {
					if (pl_listeScenario->suivant != NULL)
						if (pl_listeScenario->suivant->scenario.niveau
								>= pa_Zone->regulation_proposee.scenario.niveau)
							break;
					pl_listeScenario = pl_listeScenario->suivant;
				}

				while (pl_listeScenario != NULL) {
					if (pl_listeScenario->scenario.degrade == XDC_VRAI) {
						/* on est sur un niveau moins regulé (cf boucle precedente)*/
						if (est_prv_critique_pour_scenario(pl_listePRV->prv,
								pl_listeScenario->scenario) == XDC_FAUX) {
							pa_Zone->regulation_proposee.scenario
									= pl_listeScenario->scenario;
							pa_Zone->regulation_proposee.contexte.panne_PRV_non_critique
									= XDC_VRAI;
							pa_Zone->regulation_proposee.mesure
									= EREG_MESURE_CHGT_CONTEXTE;
							pa_Zone->regulation_proposee.indicateur_horaire
									= XDC_FAUX;
							pa_Zone->regulation_proposee.reponse_operateur
									= XDC_ALT_INCONNUE;
							pa_Zone->regulation_proposee.contexte.scenario_ideal
									= XDC_FAUX;
							if (trace_process(
									EREG_TRACE_TYPE_AJUSTEMENT,
									pa_Zone->numero_zone,
									0,
									0,
									0,
									0,
									pa_Zone->regulation_proposee.scenario.id,
									0,
									0,0,
									"PANNE du PRV %d CRITIQUE pour scenario propose",
									pl_listePRV->prv.numero_prv) == XDC_NOK) {
								XZST_03EcritureTrace(XZSTC_WARNING,
										"ajuste_analyse_evt_impactants : Impossible de tracer le process d'ajustement");
							}
							break;
						}
					}
					pl_listeScenario = pl_listeScenario->precedent;
				}

			}
		}
		pl_listePRV = pl_listePRV->suivant;
	}

	//on remet la zone a NON DEGRADEE au cas ou
	//car pas de PRV critique en panne
	if (pa_Zone->degrade == XDC_VRAI) {
		pa_Zone->degrade = XDC_FAUX;
		sprintf(vl_str,"Zone %s en mode NORMAL", pa_Zone->nom_zone);
		strcpy(vl_destinataire.site, vg_NomSite);
		if (vg_ModeSimu == XDC_FAUX)
			XZIA_06AffMsgSyst(vl_destinataire, vl_str);
		if (trace_process(
				EREG_TRACE_TYPE_AJUSTEMENT,
				pa_Zone->numero_zone,
				0,
				0,
				0,
				0,
				pa_Zone->regulation_proposee.scenario.id,
				0,
				0,0,
				vl_str) == XDC_NOK) {
			XZST_03EcritureTrace(XZSTC_WARNING,
					"ajuste_analyse_evt_impactants : Impossible de tracer le process d'ajustement");
		}
	}


	return vl_poursuivre;
}

void ajuste_analyse_chantiers(T_ZONE_REGUL *pa_Zone) {
	T_Liste_PRV *pl_listePRV=NULL;
	T_LISTE_CHAINEE *pl_listeChantiers;
	T_Liste_Commande *pl_listeCommandes;
	XDY_Entier vl_verifOK = XDC_OK;
	T_PRV vl_PRV;
	XDY_Booleen vl_regulationModifieeGlobale = XDC_FAUX;
	XDY_Booleen vl_regulationModifieeChantier = XDC_FAUX;
	T_Chantier *pl_chantier;

	XZST_03EcritureTrace(XZSTC_INFO,
			"IN : ajuste_analyse_chantiers zone [%s]", pa_Zone->nom_zone);
	XZST_03EcritureTrace(AJUSTE_NIV_TRACE_1, "   ... analyse des chantiers");

	pl_listeChantiers = pg_liste_chantiers;

	pl_listePRV = pa_Zone->ListePRV;

	while (pl_listeChantiers != NULL) {

		if (pl_listeChantiers->Zone->numero_zone = pa_Zone->numero_zone) {
			vl_verifOK = XDC_OK;
			pl_chantier = (T_Chantier *) (pl_listeChantiers->ptr);

			XZST_03EcritureTrace(AJUSTE_NIV_TRACE_1,
					"     > chantier PR %d a %d : vitesse=%d",
					pl_chantier->prDeb, pl_chantier->prFin,
					pl_chantier->vitesse);

			vl_regulationModifieeChantier = XDC_FAUX;
			vl_verifOK = ajuste_verifie_vitesse_chantier(pl_chantier, pa_Zone,
					&vl_regulationModifieeChantier);
			if (vl_regulationModifieeChantier == XDC_VRAI) {
				vl_regulationModifieeGlobale = XDC_VRAI;
				if (trace_process(EREG_TRACE_TYPE_AJUSTEMENT,
						pa_Zone->numero_zone, 0, 0, pl_chantier->numero,
						pl_chantier->cle,
						pa_Zone->regulation_proposee.scenario.id, 0,
						0,0,
						"Ajustement  provoque par Chantier") == XDC_NOK) {
					XZST_03EcritureTrace(XZSTC_WARNING,
							"ajuste_analyse_evt_impactants : Impossible de tracer le process d'ajustement");
				};
			}

			if (pa_Zone->regulation_proposee.scenario.nominal == XDC_FAUX) {
				/* Scenario autre que nominal propos� */
				vl_verifOK = ajuste_verifie_prv_amont(
						(T_Chantier *) pl_listeChantiers->ptr, pa_Zone);

				if (vl_verifOK == XDC_OK) {
					/* verifier vitesse fin chantier*/
					if (ajuste_verifie_vitesse_fin(
							(T_Chantier *) pl_listeChantiers->ptr, pa_Zone)
							== XDC_OK) {
						ajuste_verifie_decrement_fin(
								(T_Chantier *) pl_listeChantiers->ptr, pa_Zone);
					}
				}
			} else
				break;
		}
		pl_listeChantiers = pl_listeChantiers->suivant;
	}

	if (vl_regulationModifieeGlobale == XDC_VRAI) {
		pa_Zone->regulation_proposee.mesure = EREG_MESURE_AJUSTEMENT;
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : ajuste_analyse_chantiers");
}

void calculer_vitesses_scenario_entre_2_PR(T_SCENARIO *pa_scenario,
		XDY_PR va_pr1, XDY_PR va_pr2, XDY_VitesseRegul *pa_vitesseMin,
		XDY_VitesseRegul *pa_vitesseMax) {
	T_Liste_Commande *pl_listeCommandes;
	XDY_PR vl_pr_min;
	XDY_PR vl_pr_max;

	XZST_03EcritureTrace(
			XZSTC_FONCTION,
			"IN : calculer_vitesses_scenario_entre_2_PR scenario=%s, pr(%d/%d)",
			pa_scenario->libelle, va_pr1, va_pr2);

	if (va_pr1 < va_pr2) {
		vl_pr_min = va_pr1;
		vl_pr_max = va_pr2;
	} else {
		vl_pr_min = va_pr2;
		vl_pr_max = va_pr1;
	}

	*pa_vitesseMin = AJUSTE_VITESSE_INV;
	*pa_vitesseMax = AJUSTE_VITESSE_INV;

	pl_listeCommandes = pa_scenario->ListeCommandes;
	while (pl_listeCommandes != NULL) {
		if (pl_listeCommandes->commande.prv->pr_prv >= vl_pr_min
				&& pl_listeCommandes->commande.prv->pr_prv <= vl_pr_max) {
			if (*pa_vitesseMin == AJUSTE_VITESSE_INV)
				*pa_vitesseMin = pl_listeCommandes->commande.vitesse;
			if (*pa_vitesseMax == AJUSTE_VITESSE_INV)
				*pa_vitesseMax = pl_listeCommandes->commande.vitesse;

			if (pl_listeCommandes->commande.vitesse > *pa_vitesseMax)
				*pa_vitesseMax = pl_listeCommandes->commande.vitesse;
			if (pl_listeCommandes->commande.vitesse < *pa_vitesseMin)
				*pa_vitesseMin = pl_listeCommandes->commande.vitesse;
		}

		pl_listeCommandes = pl_listeCommandes->suivant;
	}

	XZST_03EcritureTrace(XZSTC_FONCTION,
			"OUT : calculer_vitesses_courante_entre_2_PR");

}
int ajuste_verifie_vitesse_chantier(T_Chantier *pa_chantier,
		T_ZONE_REGUL *pa_zone, XDY_Booleen *pa_regulationModifiee) {
	XDY_VitesseRegul vl_vitesseChantier;
	T_Liste_Scenario *pl_listeScenarios;
	T_Liste_Commande *pl_listeCommandes;
	T_PRV vl_prv;
	XDY_VitesseRegul vl_vitesseMin;
	XDY_VitesseRegul vl_vitesseMax;
	XDY_Booleen vl_scenarioOK;
	XDY_PR vl_pr_min, vl_pr_max;
	int vl_niveau = 99999;

	T_SCENARIO *pl_scenarioTrouve = NULL;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : ajuste_verifie_vitesse_chantier");
	XZST_03EcritureTrace(AJUSTE_NIV_TRACE_1,
			"     ... ajustement vitesse dans un chantier");

	vl_vitesseChantier = pa_chantier->vitesse;
	*pa_regulationModifiee = XDC_FAUX;

	vl_pr_min = pa_chantier->prDeb < pa_chantier->prFin ? pa_chantier->prDeb
			: pa_chantier->prFin;
	vl_pr_max = pa_chantier->prDeb < pa_chantier->prFin ? pa_chantier->prFin
			: pa_chantier->prDeb;

	/*	vl_vitesseMin = vl_vitesseMax = vl_vitesseChantier;*/
	calculer_vitesses_scenario_entre_2_PR(
			&(pa_zone->regulation_proposee.scenario), pa_chantier->prDeb,
			pa_chantier->prFin, &vl_vitesseMin, &vl_vitesseMax);
	if (vl_vitesseMin != AJUSTE_VITESSE_INV && vl_vitesseChantier
			< vl_vitesseMin) {
		/*Vitesse inferieure à celle du scenario courant*/
		XZST_03EcritureTrace(
				AJUSTE_NIV_TRACE_1,
				"         vitesse chantier %d < %d vitesse min du scenario %d (%s)",
				vl_vitesseChantier, vl_vitesseMin,
				pa_zone->regulation_proposee.scenario.id,
				pa_zone->regulation_proposee.scenario.libelle);

		/* rechercher un scenario degardé sans PRV critique en panne et plus regule dont la vitesse est inférieure*/
		pl_listeScenarios = pa_zone->ListeScenario;
		pl_scenarioTrouve = NULL;
		while (pl_listeScenarios != NULL) {
		/*	XZST_03EcritureTrace(AJUSTE_NIV_TRACE_1,
					"scenario %s(%d) de niveau %d(%d)",
					pl_listeScenarios->scenario.libelle,
					pa_zone->regulation_proposee.scenario.niveau,
					pl_listeScenarios->scenario.niveau,
					pl_listeScenarios->scenario.degrade);*/
			/* SI Scenario degradé et plus regule*/
			if ((pl_listeScenarios->scenario.degrade == XDC_VRAI)
					&& (pl_listeScenarios->scenario.niveau < vl_niveau)
					&& (pl_listeScenarios->scenario.niveau
							> pa_zone->regulation_proposee.scenario.niveau)) {
				XZST_03EcritureTrace(AJUSTE_NIV_TRACE_1, "on traite");

				pl_listeCommandes = pl_listeScenarios->scenario.ListeCommandes;

				vl_scenarioOK = XDC_FAUX;
				while (pl_listeCommandes != NULL) {
				/*	XZST_03EcritureTrace(AJUSTE_NIV_TRACE_1,
							"analyse PRV %d (%d-%d)",
							pl_listeCommandes->commande.prv->pr_prv, vl_pr_min,
							vl_pr_max);*/
					/*le PRV est-il dans le chantier */
					if ((pl_listeCommandes->commande.prv->pr_prv >= vl_pr_min)
							&& (pl_listeCommandes->commande.prv->pr_prv
									<= vl_pr_max)) {
						XZST_03EcritureTrace(AJUSTE_NIV_TRACE_1,
								"analyse PRV %d",
								pl_listeCommandes->commande.prv->pr_prv);

						/* le scenario ne doit pas avoir de PRV critique en panne */
						if (pl_listeCommandes->commande.prv->critique_prv
								== XDC_VRAI
								&& pl_listeCommandes->commande.prv->etat_de_marche
										== XDC_FAUX) {
							XZST_03EcritureTrace(AJUSTE_NIV_TRACE_1,
									"PRV critique en panne, on oublie");
							vl_scenarioOK = XDC_FAUX;
						}

						XZST_03EcritureTrace(
								AJUSTE_NIV_TRACE_1,
								"vitesse PRV (pr %d) %d || vitesse chantier %d",
								pl_listeCommandes->commande.prv->pr_prv,
								pl_listeCommandes->commande.vitesse,
								vl_vitesseChantier);

						/* La vitesse doir etre inferieure ou egale à celle du chantier*/
						if (pl_listeCommandes->commande.vitesse
								> vl_vitesseChantier) {
							vl_scenarioOK = XDC_FAUX;
							break;
						} else {
							vl_scenarioOK = XDC_VRAI;

						}
					}
					pl_listeCommandes = pl_listeCommandes->suivant;
				}

				if (vl_scenarioOK == XDC_VRAI) {
					pl_scenarioTrouve = &(pl_listeScenarios->scenario);
					vl_niveau = pl_listeScenarios->scenario.niveau;
				}
			}
			pl_listeScenarios = pl_listeScenarios->suivant;
		}

		if (pl_scenarioTrouve != NULL) {
			pa_zone->regulation_proposee.scenario = *pl_scenarioTrouve;
			pa_zone->regulation_proposee.contexte.qualification_operateur
					= XDC_FAUX;
			pa_zone->regulation_proposee.mesure = EREG_MESURE_AJUSTEMENT;
			pa_zone->regulation_proposee.contexte.chantiers = XDC_VRAI;
			pa_zone->regulation_proposee.indicateur_horaire = XDC_FAUX;
			pa_zone->regulation_proposee.reponse_operateur = XDC_ALT_INCONNUE;
			pa_zone->regulation_proposee.contexte.scenario_ideal = XDC_FAUX;

			*pa_regulationModifiee = XDC_VRAI;
			XZST_03EcritureTrace(AJUSTE_NIV_TRACE_1,
					"   => Changement de Scenario pour la zone %s : %s",
					pa_zone->nom_zone,
					pa_zone->regulation_proposee.scenario.libelle);

		} else {
			if (pa_zone->regulation_proposee.scenario.id
					!= pa_zone->scenarioNominal->id) {
				pa_zone->regulation_proposee.scenario
						= *(pa_zone->scenarioNominal);
				pa_zone->regulation_proposee.contexte.qualification_operateur
						= XDC_FAUX;
				pa_zone->regulation_proposee.mesure = EREG_MESURE_ARRET;
				pa_zone->regulation_proposee.reponse_operateur
						= XDC_ALT_INCONNUE;
				pa_zone->regulation_proposee.contexte.chantiers = XDC_VRAI;
				pa_zone->regulation_proposee.indicateur_horaire = XDC_FAUX;
				pa_zone->regulation_proposee.contexte.scenario_ideal = XDC_FAUX;

				/* Todo : sans qualification operateur !*/
				XZST_03EcritureTrace(AJUSTE_NIV_TRACE_1,
						"   => passage nominal pour la zone %s ",
						pa_zone->nom_zone);
				*pa_regulationModifiee = XDC_VRAI;
			} else {
				XZST_03EcritureTrace(AJUSTE_NIV_TRACE_1,
						"   => on reste sur le nominal pour la zone %s ",
						pa_zone->nom_zone);
				*pa_regulationModifiee = XDC_FAUX;
			}
		}

	}

	return XDC_OK;

	XZST_03EcritureTrace(XZSTC_FONCTION,
			"OUT : ajuste_verifie_vitesse_chantier");
}

int ajuste_verifie_prv_amont(T_Chantier *pa_chantier, T_ZONE_REGUL *pa_zone) {
	T_Liste_ZonesRegul *tmpZone = NULL;
	T_Liste_Scenario *pl_listeScenario;
	T_Liste_Commande *pl_listeCommandes;
	T_SCENARIO vl_scenario;
	T_SCENARIO *pl_scenario;
	T_Liste_PRV *pl_liste;

	T_PRV *vl_PRV;
	int vl_vitesse_amont = -1;
	XDY_Sens vl_sens;
	int vl_pr_max = 0;
	int vl_niveau = -1;
	int vl_pr_min;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : ajuste_verifie_prv_amont");
	vl_sens = pa_zone->sens_prv;

	vl_scenario = pa_zone->regulation_proposee.scenario;
	vl_pr_min = pa_chantier->prDeb < pa_chantier->prFin ? pa_chantier->prDeb
			: pa_chantier->prFin;
	vl_pr_max = pa_chantier->prDeb < pa_chantier->prFin ? pa_chantier->prFin
			: pa_chantier->prDeb;

	pl_liste = pa_zone->ListePRV;
	while ( pl_liste!= NULL) {
		XZST_03EcritureTrace(XZSTC_INFO, "PRV %d - chantier %d %d",
				pl_liste->prv.pr_prv, vl_pr_min, vl_pr_max);

		/* TODO : PRV en amont du chantier sur la zone située entre 0 et 400m */
		if (((pl_liste->prv.pr_prv < vl_pr_min)
				&& (pl_liste->prv.pr_prv > vl_pr_min - 400)
				&& (vl_sens == XDC_SENS_SUD))
				|| ((pl_liste->prv.pr_prv > vl_pr_max)
						&& (pl_liste->prv.pr_prv < vl_pr_max + 400)
						&& (vl_sens == XDC_SENS_NORD))) {
			/* TODO : vitesse amont == vitesse scenario */
			pl_listeCommandes = vl_scenario.ListeCommandes;
			while (pl_listeCommandes != NULL) {
				if (pl_listeCommandes->commande.prv->numero_prv
						== pl_liste->prv.numero_prv) {
					vl_vitesse_amont = pl_listeCommandes->commande.vitesse;
					XZST_03EcritureTrace(XZSTC_INFO,
							"vitesse amont %d - chantier %d", vl_vitesse_amont,
							pa_chantier->vitesse);
					break;
				}
				pl_listeCommandes = pl_listeCommandes->suivant;
			}
		}

		if ((vl_vitesse_amont != -1) && (vl_vitesse_amont
				- pa_chantier->vitesse
				!= tg_params[XDC_REG_DECRE_VIT_PRVAMONT_DEBCH].Valeur)) {
			XZST_03EcritureTrace(XZSTC_INFO, "recherche scenario moins regule");
			pl_listeScenario = pa_zone->ListeScenario;
			/*TODO : recherche scenario moins regule imposant bonne vitesse sur PRV amont*/
			while (pl_listeScenario != NULL) {
				if ((pl_listeScenario->scenario.niveau
						< pa_zone->regulation_proposee.scenario.niveau)
						&& (pl_listeScenario->scenario.degrade == XDC_VRAI)
						&& (pl_listeScenario->scenario.niveau > vl_niveau)) {
					XZST_03EcritureTrace(XZSTC_INFO,
							"scenario '%s' moins regule (%d)",
							pl_listeScenario->scenario.libelle,
							pl_listeScenario->scenario.niveau);

					pl_listeCommandes
							= pl_listeScenario->scenario.ListeCommandes;

					/*imposant bonne vitesse sur PRV amont ? */
					while (pl_listeCommandes != NULL) {
						if ((pl_listeCommandes->commande.prv->numero_prv
								== pl_liste->prv.numero_prv)
								&& (pl_listeCommandes->commande.vitesse
										== pa_chantier->vitesse
												+ tg_params[XDC_REG_DECRE_VIT_PRVAMONT_DEBCH].Valeur)) {
							vl_vitesse_amont
									= pl_listeCommandes->commande.vitesse;
							XZST_03EcritureTrace(XZSTC_INFO,
									"bonne vitesse %d sur PRV amont",
									vl_vitesse_amont);

							/*on le memorise*/
							pl_scenario = &(pl_listeScenario->scenario);
							vl_niveau = pl_listeScenario->scenario.niveau;

						}
						pl_listeCommandes = pl_listeCommandes->suivant;
					}
				}
				pl_listeScenario = pl_listeScenario->suivant;
			}

			/*un scenario trouve ?*/
			if (vl_niveau != -1) {
				XZST_03EcritureTrace(XZSTC_INFO,
						"scenario moins regule %s trouve", pl_scenario->libelle);
				pa_zone->regulation_proposee.scenario = *pl_scenario;
				pa_zone->regulation_proposee.contexte.qualification_operateur
						= XDC_FAUX;
				pa_zone->regulation_proposee.mesure = EREG_MESURE_AJUSTEMENT;
				pa_zone->regulation_proposee.contexte.chantiers = XDC_VRAI;
				pa_zone->regulation_proposee.indicateur_horaire = XDC_FAUX;
				pa_zone->regulation_proposee.reponse_operateur
						= XDC_ALT_INCONNUE;
				pa_zone->regulation_proposee.contexte.scenario_ideal = XDC_FAUX;
			} else {
				XZST_03EcritureTrace(XZSTC_INFO,
						"pas de scenario moins regule trouve");
				pa_zone->regulation_proposee.scenario
						= *(pa_zone->scenarioNominal);
				pa_zone->regulation_proposee.contexte.qualification_operateur
						= XDC_FAUX;
				pa_zone->regulation_proposee.mesure = EREG_MESURE_ARRET;
				pa_zone->regulation_proposee.reponse_operateur
						= XDC_ALT_INCONNUE;
				pa_zone->regulation_proposee.contexte.chantiers = XDC_VRAI;
				pa_zone->regulation_proposee.indicateur_horaire = XDC_FAUX;
				pa_zone->regulation_proposee.contexte.scenario_ideal = XDC_FAUX;
			}
		}

		pl_liste = pl_liste->suivant;
	}

	return XDC_OK;

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : ajuste_verifie_prv_amont");
}

int ajuste_verifie_vitesse_fin(T_Chantier *pa_chantier, T_ZONE_REGUL *pa_zone) {
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : ajuste_verifie_vitesse_fin");

	return XDC_OK;

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : ajuste_verifie_vitesse_fin");
}

int ajuste_verifie_decrement_fin(T_Chantier *pa_chantier, T_ZONE_REGUL *pa_zone) {
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : ajuste_verifie_decrement_fin");

	return XDC_OK;

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : ajuste_verifie_decrement_fin");
}

