/*E*/
/* Fichier : $Id: ereg_util.c,v 1.7 2018/01/15 11:22:11 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2018/01/15 11:22:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER ereg_util.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de general de ajuste de la regulation >
*
------------------------------------------------------
* HISTORIQUE :
*
* SDU		13/03/2012 : Creation DEM/1016
* JMG		09/07/2012 : fin du forcage aussi sur terminier FMC 1.2
* JMG		11/11/12: correction calcul Pa
* JMG		17/05/13 : fonction reinit_contexte 1.4
* JMG		15/01/18 : correction POPR1 -> PODP1 1.7
------------------------------------------------------*/

/* fichiers inclus */
#include "ereg_util.h"
#include "ereg_cfg.h"
#include "ereg_valide.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzit.h"
#include "xzia.h"
#include "xzdd.h"
#include "xzaac.h"
#include "xzaec.h"

extern XDY_Booleen 			vg_ModeSimu;
extern XDY_NomSite 			vg_NomSite;
extern XDY_Booleen			vg_tebeg;
extern T_Liste_ZonesRegul	*pg_Zones;
extern XDY_District			vg_NumeroSite;
extern T_Param 			tg_params[EREG_NB_PARAM_MAX];
extern XDY_Horodate 	vg_Horodate_Msg[10];



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  reinit contexte des zones
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void reinit_contexte()
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR :
*
* CONDITION D'UTILISATION
*
* FONCTION
*
------------------------------------------------------*/
{
T_Liste_ZonesRegul *pl_liste;

	pl_liste = pg_Zones;

	while(pl_liste != NULL){
		pl_liste->zone.regulation_en_cours=XDC_FAUX;
		pl_liste->zone.fmc_regulation.numero=-1;
		pl_liste->zone.fmc_regulation.cle=-1;
		pl_liste->zone.horodate_forcage=0;
		pl_liste->zone.indicateur_forcage=XDC_FAUX;
		pl_liste->zone.regulation.contexte.chantiers = XDC_FAUX;
		pl_liste->zone.regulation.contexte.declenchement_horaire = XDC_FAUX;
		pl_liste->zone.regulation.contexte.evt_impactant = XDC_FAUX;
		pl_liste->zone.regulation.contexte.panne_PRV_critique = XDC_FAUX;
		pl_liste->zone.regulation.contexte.panne_PRV_non_critique = XDC_FAUX;
		pl_liste->zone.regulation.contexte.qualification_operateur = XDC_FAUX;
		pl_liste->zone.regulation.contexte.evt_mis_a_jour = XDC_FAUX;
		pl_liste->zone.regulation.contexte.scenario_ideal = XDC_FAUX;
		pl_liste->zone.regulation.contexte.alerte_deja_envoyee= XDC_FAUX;
		pl_liste->zone.regulation.horodate_regulation = 0;
		pl_liste->zone.regulation_proposee.contexte.chantiers = XDC_FAUX;
		pl_liste->zone.regulation_proposee.contexte.declenchement_horaire = XDC_FAUX;
		pl_liste->zone.regulation_proposee.contexte.panne_PRV_critique = XDC_FAUX;
		pl_liste->zone.regulation_proposee.contexte.panne_PRV_non_critique = XDC_FAUX;
		pl_liste->zone.regulation_proposee.contexte.qualification_operateur = XDC_FAUX;
		pl_liste->zone.regulation_proposee.contexte.scenario_ideal = XDC_FAUX;
		pl_liste->zone.regulation_proposee.horodate_regulation = 0;
		pl_liste->zone.regulation_proposee.indicateur_horaire = XDC_FAUX;
		pl_liste->zone.regulation_proposee.mesure = -1;
		pl_liste->zone.regulation_proposee.reponse_operateur = -1;
		strcpy(pl_liste->zone.regulation_proposee.transitoire,"");
		pl_liste->zone.regulation_proposee.scenario.id = -1;

		pl_liste = pl_liste->suivant;
	}

	XZST_03EcritureTrace( XZSTC_INFO, "OUT : reinit_contexte");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  recherche d'une ZONE dans la liste chainee des Zones a partir d'un numero de Zone
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Entier rechercher_zone(XDY_Eqt va_numeroZone, T_ZONE_REGUL **pa_result)
/*
* ARGUMENTS EN ENTREE :
*  XDY_Eqt      va_numeroZone 	: numero de la zone
*
* ARGUMENTS EN SORTIE :
*  T_ZONE_REGUL *pa_result  	: renvoie un pointeur vers la zone. NULL si non trouve
*
* CODE RETOUR :
*  XDC_OK 	: un element a ete trouve
*  XDC_NOK 	: aucun element trouve
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Recherche de la ZONE qui a le numero passe en parametre
*
------------------------------------------------------*/
{
T_Liste_ZonesRegul *pl_liste;

	XZST_03EcritureTrace( XZSTC_INFO, "IN : rechercher_zone numero=%d", va_numeroZone);

	pl_liste = pg_Zones;

	while(pl_liste != NULL){
		if (pl_liste->zone.numero_zone == va_numeroZone)
			break;

		pl_liste = pl_liste->suivant;
	}

	if (pl_liste != NULL){
		*pa_result = &(pl_liste->zone);
		XZST_03EcritureTrace( XZSTC_INFO, "OUT : rechercher_zone");
		return XDC_OK;
	}else
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "rechercher_zone : Attention aucune ZONE correspondant au numero %d n'a ete trouvee.", va_numeroZone);
		*pa_result = NULL;
		return XDC_NOK;
	}

	XZST_03EcritureTrace( XZSTC_INFO, "OUT : rechercher_zone");
}

XDY_Entier rechercher_condition(XDY_Entier va_numero, T_Liste_Condition_Zone *pa_liste, T_CONDITION **pa_result){
T_Liste_Condition_Zone	*pl_liste;

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : rechercher_condition numero=%d", va_numero);

	pl_liste = pa_liste;

	while(pl_liste != NULL){
/*		XZST_03EcritureTrace( XZSTC_INFO,"COND %d", pl_liste->condition.id); */
		if (pl_liste->condition.id == va_numero)
			break;

		pl_liste = pl_liste->suivant;
	}

	if (pl_liste != NULL){
		*pa_result = &(pl_liste->condition);
		XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : rechercher_condition");
		return XDC_OK;
	}else
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "rechercher_condition : Attention aucune Condition correspondant au numero %d n'a ete trouvee.", va_numero);
		*pa_result = NULL;
		return XDC_NOK;
	}

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : rechercher_condition");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  recherche d'un PRV dans une liste de PRV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Entier rechercher_prv(XDY_Eqt va_numEquipement, T_Liste_PRV *pa_listePRV, T_PRV **pa_result){
/*
* ARGUMENTS EN ENTREE :
*  XDY_Eqt      va_numEquipement : numero d eqt
*  T_Liste_PRV* pa_listePRV      : liste chainee des PRV
*
* ARGUMENTS EN SORTIE :
*  T_PRV**       pa_result        : renvoie un pointeur vers le PRV trouv���. NULL si non trouv���
*
* CODE RETOUR :
*  XDC_OK : un PRV a ete trouve
*  XDC_NOK : aucun PRV trouve
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Recherche du PRV qui a le num���ro d'equipement pass��� dans en parametre
*
------------------------------------------------------*/
T_Liste_PRV	*pl_listePRV;

XZST_03EcritureTrace( XZSTC_FONCTION, "IN : rechercher_PRV num=%d", va_numEquipement);

	pl_listePRV = pa_listePRV;

	while(pl_listePRV != NULL){
/*		XZST_03EcritureTrace( XZSTC_INFO,"PRV %d", pl_listePRV->prv.numero_prv); */
		if (pl_listePRV->prv.numero_prv == va_numEquipement)
			break;

		pl_listePRV = pl_listePRV->suivant;
	}

	if (pl_listePRV != NULL){
		*pa_result = &(pl_listePRV->prv);
		XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : rechercher_PRV");
		return XDC_OK;
	}else
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "rechercher_PRV : Attention aucun PRV correspondant au numEqt %d n'a ete trouve.", va_numEquipement);
		*pa_result = NULL;
		return XDC_NOK;
	}

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : rechercher_PRV");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  recherche d'une STATION dans une liste chainee de Stations apartir d'un numero de station
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Entier rechercher_station(XDY_Entier va_numStation, XDY_Octet va_sens, T_Liste_Station *pa_liste, T_STATION **pa_result){
/*
* ARGUMENTS EN ENTREE :
*  XDY_Entier va_numStation 	: numero de la station
*  T_Liste_Station *pa_liste    : liste chainee
*
* ARGUMENTS EN SORTIE :
*  T_STATION **pa_result        : renvoie un pointeur vers la Station trouvee. NULL si non trouve
*
* CODE RETOUR :
*  XDC_OK :
*  XDC_NOK :
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Recherche de la Station qui a le numero passe en parametre
*
------------------------------------------------------*/
T_Liste_Station	*pl_liste;

XZST_03EcritureTrace( XZSTC_FONCTION, "IN : rechercher_station num=%d(%d)", va_numStation,va_sens);

	pl_liste = pa_liste;

	while(pl_liste != NULL){
		if ((pl_liste->station.station == va_numStation) && (pl_liste->station.sens == va_sens))
			break;

		pl_liste = pl_liste->suivant;
	}

	if (pl_liste != NULL){
		*pa_result = &(pl_liste->station);
		XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : rechercher_station");
		return XDC_OK;
	}else
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "rechercher_station : Attention aucune STATION correspondant au numero %d n'a ete trouvee.", va_numStation);
		*pa_result = NULL;
		return XDC_NOK;
	}

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : rechercher_station");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  indique si un PRV est critique pour un scenario donn���
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Booleen est_prv_critique_pour_scenario(T_PRV va_Prv, T_SCENARIO va_scenario){
/*
* ARGUMENTS EN ENTREE :
*  T_PRV 	  va_Prv 		: PRV concern���
*  T_SCENARIO va_scenario	: Scenario Concern���
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  XDC_VRAI si le PRV est critique pour le sc���nario
*  XDC_FAUX si le PRV n'est pas critique pour le sc���nario
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Recherche parmis les commandes du scenario celle qui concerne le PRV et renvoi le booleen critique
*
------------------------------------------------------*/
T_Liste_Commande *pl_listeCommande;

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : est_PRV_critique_pour_scenario ");

	pl_listeCommande = va_scenario.ListeCommandes;
	while (pl_listeCommande != NULL){
		XZST_03EcritureTrace( XZSTC_INFO,"%d %d",
				pl_listeCommande->commande.vitesse,
				pl_listeCommande->commande.critique);

		if (pl_listeCommande->commande.equipement == va_Prv.numero_prv)
			break;

		pl_listeCommande = pl_listeCommande->suivant;
	}
	if (pl_listeCommande != NULL) {
		XZST_03EcritureTrace( XZSTC_INFO,"%d %d",
				pl_listeCommande->commande.vitesse,
				pl_listeCommande->commande.critique);

		return pl_listeCommande->commande.critique;
	}
	XZST_03EcritureTrace( XZSTC_WARNING,"commande non trouvee prv %d, scenario %s",
						va_Prv.numero_prv, va_scenario.libelle);
	return (FALSE);
	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : est_PRV_critique_pour_scenario ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  indique si un scenario contient un PRV critique en panne
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Booleen scenario_has_prv_critique_en_panne(T_SCENARIO *pa_scenario){
/*
* ARGUMENTS EN ENTREE :
*  T_SCENARIO *pa_scenario	: pointeur vers le Scenario Concern���
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  XDC_VRAI si un PRV critique en panne a ���t��� trouve
*  XDC_FAUX si aucun PRV critique en panne a ���t��� trouve
*
* CONDITION D'UTILISATION
*	aucune
*
* FONCTION
*
------------------------------------------------------*/
T_Liste_Commande *pl_listeCommande;

	XZST_03EcritureTrace( XZSTC_INFO, "IN : est_PRV_critique_pour_scenario %d",
				pa_scenario->id);

	pl_listeCommande = pa_scenario->ListeCommandes;
	while (pl_listeCommande != NULL){
		XZST_03EcritureTrace( XZSTC_INFO, "prv %d etat %d critique %d",
				pl_listeCommande->commande.prv->numero_prv,
				pl_listeCommande->commande.prv->etat_de_marche,
				pl_listeCommande->commande.critique);
		if (pl_listeCommande->commande.critique == XDC_VRAI)
			if (pl_listeCommande->commande.prv->etat_de_marche != XDC_VRAI)
				return XDC_VRAI;

		pl_listeCommande = pl_listeCommande->suivant;
	}
	return XDC_FAUX;

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : est_PRV_critique_pour_scenario ");
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  recherche d'un SCENARIO dans une liste chainee de Zones a partir d'un ID scenario
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Entier rechercher_scenario(XDY_Eqt va_idScenario, T_Liste_ZonesRegul *pa_listeZones, T_SCENARIO *pa_result){
/*
* ARGUMENTS EN ENTREE :
*  XDY_Eqt      va_idScenario 	: id du scenario
*  T_Liste_Zone *pa_listeZones  : liste chainee des Zones
*
* ARGUMENTS EN SORTIE :
*  T_SCENARIO*       pa_result        : renvoie un pointeur vers le Scenario. NULL si non trouve
*
* CODE RETOUR :
*  XDC_OK 	: un element a ete trouve
*  XDC_NOK 	: aucun element trouve
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Recherche du SCENARIO qui a l'id passe dans en parametre
*
------------------------------------------------------*/
T_Liste_ZonesRegul	*pl_liste;
T_Liste_Scenario 	*pl_listeScenarios;

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : rechercher_scenario id=%d", va_idScenario);

	pl_liste = pa_listeZones;

	while(pl_liste != NULL){
		pl_listeScenarios = pl_liste->zone.ListeScenario;

		while (pl_listeScenarios != NULL){
			if (pl_listeScenarios->scenario.id == va_idScenario)
				break;

			pl_listeScenarios = pl_listeScenarios->suivant;
		}
		if (pl_listeScenarios != NULL)
			break;
	}

	if (pl_listeScenarios != NULL){
		*pa_result = pl_listeScenarios->scenario;
		return XDC_OK;
	}else
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "rechercher_scenario : Attention aucun SCENARIO correspondant a l'id %d n'a ete trouv��.", va_idScenario);
		/* *pa_result = NULL; */
		return XDC_OK;
	}

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : rechercher_scenario");
}


int genererTexteAlerteReveil(T_REGULATION va_regulation, char *pa_result){

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : genererTexteAlerteReveil ");

	/*Construction du TexteAlerte en fonction des booleens du contexte*/
	if(va_regulation.contexte.scenario_ideal == XDC_VRAI)
	{
		if(va_regulation.contexte.declenchement_horaire  == XDC_VRAI)
		{
			sprintf(pa_result,"Scenario %s et declenchement Horaire#%d",
					va_regulation.scenario.libelle,
					va_regulation.scenario.id);
		}
		else
		{
			sprintf(pa_result,"Scenario propose : %s#%d",
					va_regulation.scenario.libelle,
					va_regulation.scenario.id);
		}
	}
	else if(va_regulation.contexte.evt_impactant == XDC_VRAI)
	{
		if(va_regulation.contexte.qualification_operateur == XDC_VRAI)
		{
			sprintf(pa_result,"Evenement Impactant : '%s' propose#%d",
					va_regulation.scenario.libelle,
					va_regulation.scenario.id);
		}
		else
		{
			sprintf(pa_result,"Evenement Impactant : '%s' propose#%d",
					va_regulation.scenario.libelle,
					va_regulation.scenario.id);
		}

	}
	else if(va_regulation.contexte.panne_PRV_critique == XDC_VRAI)
	{
		sprintf(pa_result,"Panne de PRV critique : '%s' propose#%d",
				va_regulation.scenario.libelle,
				va_regulation.scenario.id);
	}
	else if(va_regulation.contexte.panne_PRV_non_critique == XDC_VRAI)
	{
		sprintf(pa_result,"Panne de PRV non critique : '%s' propose#%d",
				va_regulation.scenario.libelle,
				va_regulation.scenario.id);
	}
	else if (va_regulation.contexte.chantiers == XDC_VRAI)
	{
		if (va_regulation.contexte.panne_PRV_non_critique == XDC_VRAI)
		{
			sprintf(pa_result,"Travaux en cours et Panne de PRV critique : '%s' propose #%d",
					va_regulation.scenario.libelle,
					va_regulation.scenario.id);
		}
		else
		{
			sprintf(pa_result,"Travaux en cours : '%s' propose#%d",
					va_regulation.scenario.libelle,
					va_regulation.scenario.id);
		}
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "Erreur de construction du texteAlerte");
		strcpy(pa_result,"");
		return XDC_NOK;
	}


	sprintf(pa_result, "%s#%d#%d",
				pa_result,
				calculerContexte(va_regulation.contexte),
				va_regulation.mesure);

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : genererTexteAlerteReveil ");
	return XDC_OK;
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen isRegulationEnCours (T_ZONE_REGUL *pa_zone)


/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR :
*  Aucun
*
* CONDITION D'UTILISATION
*  Doit etre invoque avant les modules specifique aux equipements
*
* FONCTION
*   creation de la liste des zones
*
------------------------------------------------------*/
{
	return (pa_zone->fmc_regulation.numero > 0);
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int creerFMCRegulation (T_ZONE_REGUL *pa_zone, T_Contexte va_contexte, XDY_Entier va_num_alerte)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
* Fonction de creation de FMC
*
* FONCTION
*
------------------------------------------------------*/
{
	XDY_Entier	vl_numeroFMC;
	XDY_Octet	vl_cleFMC ;
	XDY_Entier vl_contexte;
	XZITT_Destinataire vl_destinataire;

	/*si une FMC est deja en cours sur la zone*/
	if (pa_zone->fmc_regulation.numero > 0) {
		XZST_03EcritureTrace(XZSTC_FONCTION, "pas besoin de creer une autre FMC regulation");
		return (XDC_OK);
	}


	XZST_03EcritureTrace(XZSTC_INFO, "+++++ creerFMCRegulation : autoroute_prv: %d sens_prv: %d pr_debut: %d pr_fin: %d chantiers: %d declenchement_horaire: %d evt_impactant: %d panne_PRV_critique: %d panne_PRV_non_critique: %d qualification_operateur: %d scenario_ideal: %d id: %d indicateur_horaire: %d mesure: %d transitoire: %s",
							pa_zone->autoroute_prv,
							pa_zone->sens_prv,
							pa_zone->pr_debut,
							pa_zone->pr_fin,
							va_contexte.chantiers,
							va_contexte.declenchement_horaire,
							va_contexte.evt_impactant,
							va_contexte.panne_PRV_critique,
							va_contexte.panne_PRV_non_critique,
							va_contexte.qualification_operateur,
							va_contexte.scenario_ideal,
							pa_zone->scenarioNominal->id,
							pa_zone->regulation_proposee.indicateur_horaire,
							pa_zone->regulation_proposee.mesure,
							pa_zone->regulation_proposee.transitoire
							);
	vl_contexte = calculerContexte(va_contexte);

	/*Appel la fonction de creation de fmc regulation*/
	if (vg_ModeSimu==XDC_FAUX) {
	if (XZAE560_Creer_FMC_Regulation(pa_zone->autoroute_prv,
									pa_zone->sens_prv,
									pa_zone->pr_debut,
									pa_zone->pr_fin,
									vl_contexte,
									pa_zone->regulation_proposee.scenario.id,
									pa_zone->regulation_proposee.scenario.id,
									pa_zone->regulation_proposee.indicateur_horaire,
									pa_zone->regulation_proposee.mesure,
									pa_zone->regulation_proposee.transitoire,
									pa_zone->numero_zone,
									va_num_alerte,
									&vl_numeroFMC,
									&vl_cleFMC) != XDC_OK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "creerFMCRegulation : Impossible d appeler XZAE560_Creer_FMC_Regulation");
					return XDC_NOK;
				}
	
		pa_zone->fmc_regulation.numero = vl_numeroFMC;
		pa_zone->fmc_regulation.cle = vl_cleFMC;
		XZST_03EcritureTrace(XZSTC_FONCTION, "FMC CREE : numero=%d, cle=%d",
				pa_zone->fmc_regulation.numero,
				pa_zone->fmc_regulation.cle);

		if (pa_zone->fmc_regulation.numero > -1) {
			strcpy(vl_destinataire.site,vg_NomSite);
			XZIT08_Modif_Liste_FMC(pa_zone->fmc_regulation, vl_destinataire ,XDC_REV_FMC_CRE);
		}
	}	

	appli_analyse(pa_zone);

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : creerFMCRegulation");
	return XDC_OK;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int terminerFmcRegulation (T_ZONE_REGUL *pa_zone, XDY_Booleen va_avec_PA)


/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR :
*  Aucun
*
* CONDITION D'UTILISATION
*  Doit etre invoque avant les modules specifique aux equipements
*
* FONCTION
*   creation de la liste des zones
*
------------------------------------------------------*/
{	XZITT_Destinataire vl_destinataire;
	XDY_Evt         NumEvt;
	char vl_poste[20];
	XDY_Horodate vl_HorodateSec;
	int  vl_JourSemaine;

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : terminerFmcRegulation ");

	if (vg_ModeSimu==XDC_VRAI) {
	  return (XDC_OK);
	}

	NumEvt.numero = pa_zone->fmc_regulation.numero;
	NumEvt.cle = pa_zone->fmc_regulation.cle;

	 pa_zone->indicateur_forcage = XDC_FAUX;

	if (pa_zone->fmc_regulation.numero != -1) {
	if (XZAE566_Terminer_FMC_Regulation(pa_zone->fmc_regulation.numero,
										pa_zone->fmc_regulation.cle)!= XDC_OK)
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "terminerFmcRegulation: pb appel XZAE566_Terminer_FMC_Regulation Numero: %d\n Cle:%d\n",
					pa_zone->fmc_regulation.numero,
					pa_zone->fmc_regulation.cle);
		}

		strcpy(vl_destinataire.site,vg_NomSite);
		XZIT08_Modif_Liste_FMC(pa_zone->fmc_regulation, vl_destinataire ,XDC_REV_FMC_FIN);

		XZST_03EcritureTrace( XZSTC_WARNING, "terminerFmcRegulation: appel XZAE566_Terminer_FMC_Regulation OK Numero: %d\n Cle:%d\n",
						pa_zone->fmc_regulation.numero,
						pa_zone->fmc_regulation.cle);

		pa_zone->fmc_regulation.numero = -1;
	}

	if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
							(int *) &vl_JourSemaine, (double *) &vl_HorodateSec ) != XDC_OK )
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la lecture de l'heure %d en Sec", vl_HorodateSec );
		return XDC_NOK;
	}

	sprintf(vl_poste,"PO%s1",vg_NomSite);
	if (va_avec_PA)
		XZDD02_CalculerPA(vl_HorodateSec,-1,"PODP1",NumEvt,XZAEC_FMC_Regulation,XZDDC_PA_SANS_TFM,XZDDC_MODE_OPERATIONNEL);
	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : terminerFmcRegulation");


	return (XDC_OK);
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int updateFmcRegulation (T_ZONE_REGUL *pa_zone, XDY_Booleen va_avec_PA)


/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR :
*  Aucun
*
* CONDITION D'UTILISATION
*  Doit etre invoque avant les modules specifique aux equipements
*
* FONCTION
*   creation de la liste des zones
*
------------------------------------------------------*/
{
	XDY_Horodate	vl_HorodateSec;
	int				vl_JourSemaine;
	XZSMT_Horodate	vl_Horodate_courante;
	XZITT_Destinataire vl_destinataire;
	XDY_Evt         NumEvt;
	char vl_poste[10];

	if (vg_ModeSimu==XDC_VRAI) {
		return (XDC_OK);
	}
	if (pa_zone->fmc_regulation.numero == -1)
		return (XDC_OK);

	if ( pa_zone->regulation.scenario.id == -1)
		return (XDC_OK);

	if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
							(int *) &vl_JourSemaine, (double *) &vl_HorodateSec ) != XDC_OK )
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la lecture de l'heure %d en Sec", vl_HorodateSec );
		return XDC_NOK;
	}
	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : updateFmcRegulation ");

	if(XZSM_11ConversionHorodate( vl_HorodateSec, &vl_Horodate_courante) != XDC_OK){
		XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de convertir l'horodate courante en chaine");
		return XDC_NOK;
	}
	XZST_03EcritureTrace( XZSTC_INFO,"horo %s",vl_Horodate_courante);
		if (XZAE564_Update_FMC_Regulation (pa_zone->fmc_regulation.numero,
											pa_zone->fmc_regulation.cle,
											pa_zone->regulation.scenario.id,
											calculerContexte(pa_zone->regulation.contexte),
											pa_zone->regulation.indicateur_horaire,
											pa_zone->regulation.mesure,
											pa_zone->regulation.transitoire,
											pa_zone->numero_zone,
											NULL,
											vl_HorodateSec,
											pa_zone->regulation.scenario.id)!= XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "updateFmcRegulation: pb appel XZAE564_Update_FMC_Regulation Numero: %d \n Cle: %d \n Scenario: %d \n Contexte: %d \n Indicateur_Horaire: %d \n Mesure: %d \n Transitoire: %s \n Zone: %d \n Scenario_cible: %d \n",
				pa_zone->fmc_regulation.numero,
				pa_zone->fmc_regulation.cle,
				pa_zone->regulation.scenario.id,
				calculerContexte(pa_zone->regulation.contexte) ,
				pa_zone->regulation.indicateur_horaire ,
				pa_zone->regulation.mesure,
				pa_zone->regulation.transitoire,
				pa_zone->numero_zone,
				pa_zone->regulation.scenario.id);
		return (XDC_NOK);
	}
		XZST_03EcritureTrace( XZSTC_INFO, "updateFmcRegulation: *** XZAE564_Update_FMC_Regulation OK *** :\n Numero: %d \n Cle: %d \n Scenario: %d \n Contexte: %d \n Indicateur_Horaire: %d \n Mesure: %d \n Transitoire: %s \n Zone: %d \n Scenario_cible: %d \n",
						pa_zone->fmc_regulation.numero,
						pa_zone->fmc_regulation.cle,
						pa_zone->regulation.scenario.id,
						calculerContexte(pa_zone->regulation.contexte) ,
						pa_zone->regulation.indicateur_horaire ,
						pa_zone->regulation.mesure,
						pa_zone->regulation.transitoire,
						pa_zone->numero_zone,
						pa_zone->regulation.scenario.id);

	strcpy(vl_destinataire.site,vg_NomSite);
	XZIT08_Modif_Liste_FMC(pa_zone->fmc_regulation, vl_destinataire ,XDC_REV_FMC_MOD);

	NumEvt.numero = pa_zone->fmc_regulation.numero;
	NumEvt.cle = pa_zone->fmc_regulation.cle;

	if ( (va_avec_PA)  && (pa_zone->regulation.mesure != EREG_MESURE_ARRET) ) {
	sprintf(vl_poste,"PO%s1",vg_NomSite);
	if (strlen(pa_zone->regulation.transitoire) >0)
		XZDD02_CalculerPA(vl_HorodateSec,-1,"PODP1",NumEvt,XZAEC_FMC_Regulation,XZDDC_PA_SANS_TFM,XZDDC_MODE_OPERATIONNEL);
	}

	XZST_03EcritureTrace( XZSTC_INFO, "OUT : updateFmcRegulation");

	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int updateFmcRegulationProposee (T_ZONE_REGUL *pa_zone)


/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR :
*  Aucun
*
* CONDITION D'UTILISATION
*  Doit etre invoque avant les modules specifique aux equipements
*
* FONCTION
*   creation de la liste des zones
*
------------------------------------------------------*/
{
	XDY_Horodate	vl_HorodateSec;
	int				vl_JourSemaine;
	XZSMT_Horodate	vl_Horodate_courante;
	XZITT_Destinataire vl_destinataire;

	if (vg_ModeSimu==XDC_VRAI) {
		return (XDC_OK);
	}

	if (pa_zone->fmc_regulation.numero == -1)
		return (XDC_OK);

	if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
							(int *) &vl_JourSemaine, (double *) &vl_HorodateSec ) != XDC_OK )
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la lecture de l'heure %d en Sec", vl_HorodateSec );
		return XDC_NOK;
	}
	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : updateFmcRegulation ");

	if(XZSM_11ConversionHorodate( vl_HorodateSec, &vl_Horodate_courante) != XDC_OK){
		XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de convertir l'horodate courante en chaine");
		return XDC_NOK;
	}
	XZST_03EcritureTrace( XZSTC_INFO,"horo %s",vl_Horodate_courante);
		if (XZAE564_Update_FMC_Regulation (pa_zone->fmc_regulation.numero,
											pa_zone->fmc_regulation.cle,
											pa_zone->regulation_proposee.scenario.id,
											calculerContexte(pa_zone->regulation_proposee.contexte),
											pa_zone->regulation_proposee.indicateur_horaire,
											pa_zone->regulation_proposee.mesure,
											pa_zone->regulation_proposee.transitoire,
											pa_zone->numero_zone,
											NULL,
											vl_HorodateSec,
											pa_zone->regulation_proposee.scenario.id)!= XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "updateFmcRegulation: pb appel XZAE564_Update_FMC_Regulation Numero: %d \n Cle: %d \n Scenario: %d \n Contexte: %d \n Indicateur_Horaire: %d \n Mesure: %d \n Transitoire: %s \n Zone: %d \n Operateur: %d \n Horodate: %ld \n Scenario_cible: %d \n",
				pa_zone->fmc_regulation.numero,
				pa_zone->fmc_regulation.cle,
				pa_zone->regulation_proposee.scenario.id,
				calculerContexte(pa_zone->regulation_proposee.contexte) ,
				pa_zone->regulation_proposee.indicateur_horaire ,
				pa_zone->regulation_proposee.mesure,
				pa_zone->regulation_proposee.transitoire,
				pa_zone->numero_zone,
				NULL,
				vl_HorodateSec,
				pa_zone->regulation_proposee.scenario.id);
	}
		XZST_03EcritureTrace( XZSTC_INFO, "updateFmcRegulation: *** XZAE564_Update_FMC_Regulation OK *** :\n Numero: %d \n Cle: %d \n Scenario: %d \n Contexte: %d \n Indicateur_Horaire: %d \n Mesure: %d \n Transitoire: %s \n Zone: %d \n Operateur: %d \n Horodate: %ld \n Scenario_cible: %d \n",
						pa_zone->fmc_regulation.numero,
						pa_zone->fmc_regulation.cle,
						pa_zone->regulation_proposee.scenario.id,
						calculerContexte(pa_zone->regulation.contexte) ,
						pa_zone->regulation_proposee.indicateur_horaire ,
						pa_zone->regulation_proposee.mesure,
						pa_zone->regulation_proposee.transitoire,
						pa_zone->numero_zone,
						NULL,
						vl_HorodateSec,
						pa_zone->regulation_proposee.scenario.id);

	strcpy(vl_destinataire.site,vg_NomSite);
	XZIT08_Modif_Liste_FMC(pa_zone->fmc_regulation, vl_destinataire ,XDC_REV_FMC_MOD);

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : updateFmcRegulation");

	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int reinitTempoForcage (T_ZONE_REGUL *pa_zone)
/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR :
*  Aucun
*
* CONDITION D'UTILISATION
*  Doit etre invoque avant les modules specifique aux equipements
*
* FONCTION
*   creation de la liste des zones
*
------------------------------------------------------*/
{
XDY_Horodate	vl_HorodateSec;
int				vl_JourSemaine;

	if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
							(int *) &vl_JourSemaine, (double *) &vl_HorodateSec ) != XDC_OK )
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la lecture de l'heure %d en Sec", vl_HorodateSec );
		return XDC_NOK;
	}
	pa_zone->horodate_forcage = vl_HorodateSec;
	return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int annulerForcage (T_ZONE_REGUL *pa_zone)


/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR :
*  Aucun
*
* CONDITION D'UTILISATION
*  Doit etre invoque avant les modules specifique aux equipements
*
* FONCTION
*   creation de la liste des zones
*
------------------------------------------------------*/
{
	pa_zone->indicateur_forcage = FALSE;
	pa_zone->regulation_proposee.mesure = EREG_MESURE_DEFAUT;
	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int declencherForcage (T_ZONE_REGUL *pa_zone)
/*
* ARGUMENTS EN ENTREE :
*	T_ZONE_REGUL *pa_zone
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR :
*  Aucun
*
* CONDITION D'UTILISATION
*
* FONCTION
*   declenche un forcage
*
------------------------------------------------------*/
{
	int vl_jour;
	XDY_Horodate vl_horo;

	pa_zone->indicateur_forcage = TRUE;
	pa_zone->regulation_proposee.mesure = EREG_MESURE_FORCAGE;

	if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
	                       &vl_jour,  &vl_horo ) != XDC_OK )
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de recuperer l horodate courante");
		return (XDC_NOK);
	}
	pa_zone->horodate_forcage = vl_horo;
	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int declencherArret (T_ZONE_REGUL *pa_zone)
/*
* ARGUMENTS EN ENTREE :
*	T_ZONE_REGUL *pa_zone
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR :
*  Aucun
*
* CONDITION D'UTILISATION
*
* FONCTION
*   declenche un Arret de la Regulation
*
------------------------------------------------------*/
{
	pa_zone->regulation_proposee.scenario = *(pa_zone->scenarioNominal);
	pa_zone->regulation_proposee.mesure = EREG_MESURE_ARRET;
	return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int updateRegulationProposee (T_ZONE_REGUL *pa_zone,
								T_REGULATION va_regulation)


/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR :
*  Aucun
*
* CONDITION D'UTILISATION
*  Doit etre invoque avant les modules specifique aux equipements
*
* FONCTION
*   creation de la liste des zones
*
------------------------------------------------------*/
{
	XDY_Horodate 	vl_horo_courante;
	XDY_Entier 		vl_JourSemaine;

	if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
	                        (int *) &vl_JourSemaine, (double *) &vl_horo_courante ) != XDC_OK )
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la lecture de l'heure %d en Sec", vl_horo_courante );
		return XDC_NOK;
	}

	pa_zone->regulation = va_regulation;
	if (pa_zone->regulation.scenario.id == (*(pa_zone->scenarioNominal)).id)
		pa_zone->regulation.horodate_regulation=0;
	else
		pa_zone->regulation.horodate_regulation = vl_horo_courante;
	pa_zone->regulation.appli_process = XDC_FAUX;
	pa_zone->regulation.contexte.checksum = calculerContexte(pa_zone->regulation.contexte);
	return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Calcule de la valeur Entiere du contexte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Entier calculerContexte (T_Contexte va_contexte){
/*
* ARGUMENTS EN ENTREE :
* T_Contexte va_contexte
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR :
*  XDY_Entier : la valeur du contexte
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Calcul de la valeur entiere du contexte par application des masques de bit
*
------------------------------------------------------*/
XDY_Entier vl_resultat;

	if (va_contexte.scenario_ideal == XDC_VRAI)
		return (va_contexte.declenchement_horaire * XDC_REG_MASK_CONTEXT_DECL_HORAIRE |
					va_contexte.scenario_ideal * XDC_REG_MASK_CONTEXT_SCENARIO_IDEAL);

	vl_resultat =  va_contexte.chantiers * XDC_REG_MASK_CONTEXT_CHANTIERS |
					va_contexte.declenchement_horaire * XDC_REG_MASK_CONTEXT_DECL_HORAIRE |
					va_contexte.evt_impactant * XDC_REG_MASK_CONTEXT_EVT_IMPACTANTS |
					va_contexte.panne_PRV_critique * XDC_REG_MASK_CONTEXT_PANNE_PRV_CRITIQUE |
					va_contexte.panne_PRV_non_critique * XDC_REG_MASK_CONTEXT_PANNE_PRV_NON_CRITIQUE |
					va_contexte.qualification_operateur * XDC_REG_MASK_CONTEXT_QUALIF_OPERATEUR |
					va_contexte.scenario_ideal * XDC_REG_MASK_CONTEXT_SCENARIO_IDEAL;
	return vl_resultat;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  comparer contexte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XDY_Booleen comparerContexte (T_Contexte va_contexte1, T_Contexte va_contexte2)


/*
* ARGUMENTS EN ENTREE :
* XDY_Entier va_Station
* XDY_Entier va_Zone
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR :
*  Aucun
*
* CONDITION D'UTILISATION
*  Doit etre invoque avant les modules specifique aux equipements
*
* FONCTION
*   creation de la liste des zones
*
------------------------------------------------------*/
{
	return calculerContexte(va_contexte1) == calculerContexte(va_contexte2);
}


/*------------------------------------------------------
* SERVICE RENDU :
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void passer_zone_degrade(T_ZONE_REGUL *pa_zone, XDY_Booleen va_forcage_degrade, char *va_str)
/*
* ARGUMENTS EN ENTREE :
*  T_ZONE_REGUL      pa_zone 	: zone
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Passe la zone en mode dégradée si toutes les stations de la zone n'ont pas de données trafic
*   et si la zone est en mode horaire forcée
------------------------------------------------------*/
{
	T_Liste_Station 	*pl_station = NULL;
	XDY_Entier			i=0, nb_station=0;
	XDY_Booleen			vl_data_presente = XDC_FAUX;
	T_Liste_PRV	*pl_prv=NULL;
	XDY_Booleen vl_zoneDegrade = XDC_FAUX;
	XDY_Entier		vl_JourSemaine_Cycle;
	XDY_Horodate 	vl_Horodate_Courant;
	char	vl_str[50];
	char	vl_str2[50]="";
	XZIAT_destinataire vl_destinataire;

	XZST_03EcritureTrace( XZSTC_WARNING, "entree dans passer_zone_degrade");
	if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
								(int *) &vl_JourSemaine_Cycle, (double *) &vl_Horodate_Courant ) != XDC_OK )
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de recuperer l horodate courante");
	}

	if (va_forcage_degrade == XDC_VRAI) {
		vl_zoneDegrade = XDC_VRAI;
		strcpy(vl_str2,va_str);
	}
	else if (pa_zone->mode_horaire == XDC_VRAI)
		vl_zoneDegrade = XDC_FAUX;
	else if(vl_Horodate_Courant - vg_Horodate_Msg[pa_zone->numero_zone] >= tg_params[XDC_REG_DELAI_LAT_TERDT].Valeur*60)
		vl_zoneDegrade = XDC_VRAI;
	else
	{
		/* parcours de toutes les stations de la zone
		 * et si la derniere données trafic de chaque station est invalides alors la zone est en mode dégradée */

		for (pl_station = pa_zone->ListeStation; pl_station != NULL; pl_station = pl_station->suivant)
		{
			if( (pl_station->station.data[pl_station->station.positionDerniereStationRecue].Q_valide != XDC_FAUX) ||
				(pl_station->station.data[pl_station->station.positionDerniereStationRecue].TO_valide != XDC_FAUX) )
			{
				vl_data_presente = XDC_VRAI;
				break;
			}
		}

		if (vl_data_presente==XDC_FAUX) {
			vl_zoneDegrade = XDC_VRAI;
			strcpy(vl_str2," Pas RADT");	
		}
	}

	if (vl_zoneDegrade == XDC_VRAI){
		if (pa_zone->degrade == XDC_FAUX) {
		pa_zone->degrade = XDC_VRAI;
		/* Information transmise au module SAE 'administration' */
		sprintf(vl_str,"Zone %s en mode DEGRADE %s", pa_zone->nom_zone,vl_str2);
		XZST_03EcritureTrace( XZSTC_WARNING, "zone num : %d en mode degrade (%s)", pa_zone->numero_zone,vl_str2);
		strcpy(vl_destinataire.site, vg_NomSite);
		if (vg_ModeSimu==XDC_FAUX) {
			XZIA_06AffMsgSyst(vl_destinataire, vl_str);
		}

		if (trace_process(
				EREG_TRACE_TYPE_AJUSTEMENT,
				pa_zone->numero_zone,
				0,
				0,
				0,
				0,
				pa_zone->regulation_proposee.scenario.id,
				0,0,0,vl_str) == XDC_NOK) {
			XZST_03EcritureTrace(XZSTC_WARNING,
					"ajuste_analyse_evt_impactants : Impossible de tracer le process d'ajustement");
		}
		}
	} 
	else
	{ /*que si pas de prv critique en panne pour le scenario nominal*/
		if (!scenario_has_prv_critique_en_panne(pa_zone->scenarioNominal)) {
			if (pa_zone->degrade==XDC_VRAI) {
				pa_zone->degrade = XDC_FAUX;
				sprintf(vl_str,"Zone %s en mode NORMAL", pa_zone->nom_zone);
				XZST_03EcritureTrace( XZSTC_WARNING, "zone num : %d en mode normal", pa_zone->numero_zone);
				strcpy(vl_destinataire.site, vg_NomSite);
				if (vg_ModeSimu==XDC_FAUX) {
					XZIA_06AffMsgSyst(vl_destinataire, vl_str);
				}

				if (trace_process(
					EREG_TRACE_TYPE_AJUSTEMENT,
					pa_zone->numero_zone,
					0,
					0,
					0,
					0,
					pa_zone->regulation_proposee.scenario.id,
					0,0,0,
					vl_str) == XDC_NOK) {
						XZST_03EcritureTrace(XZSTC_WARNING,
						"ajuste_analyse_evt_impactants : Impossible de tracer le process d'ajustement");
				}
			}
		}
		else {  /*PRV critique en panne pour le scenario nominal*/
		  if (pa_zone->degrade==XDC_FAUX) {
			pa_zone->degrade = XDC_VRAI;
			sprintf(vl_str,"Zone %s en mode DEGRADE",pa_zone->nom_zone);
			XZST_03EcritureTrace( XZSTC_WARNING, "zone num : %d en mode degrade",pa_zone->numero_zone);
			strcpy(vl_destinataire.site, vg_NomSite);
			if (vg_ModeSimu==XDC_FAUX) {
				XZIA_06AffMsgSyst(vl_destinataire, vl_str);
			}
			pa_zone->regulation_proposee.contexte.panne_PRV_critique=XDC_VRAI;
			pa_zone->regulation_proposee.contexte.scenario_ideal=XDC_FAUX;
			pa_zone->regulation_proposee.scenario=*(pa_zone->scenarioNominal);
			pa_zone->regulation_proposee.mesure = EREG_MESURE_ARRET;

			if (trace_process(EREG_TRACE_TYPE_AJUSTEMENT,
				pa_zone->numero_zone, 0, 0, 0, 0,
				pa_zone->regulation_proposee.scenario.id, 0,
				0,0,
				"PANNE de PRV CRITIQUE pour scenario nominal"
				) == XDC_NOK) {
					XZST_03EcritureTrace(XZSTC_WARNING,
						"ajuste_analyse_evt_impactants : Impossible de tracer le process d'ajustement");
			}
			if (trace_process(EREG_TRACE_TYPE_AJUSTEMENT,
				pa_zone->numero_zone, 0, 0, 0, 0,
				pa_zone->regulation_proposee.scenario.id, 0,
				0,0,vl_str)== XDC_NOK) {
					XZST_03EcritureTrace(XZSTC_WARNING,
						"ajuste_analyse_evt_impactants : Impossible de tracer le process d'ajustement");
			}
		  }
		}
	}
}

#if 0
/*------------------------------------------------------
* SERVICE RENDU :
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void passer_zone_degrade(T_ZONE_REGUL *pa_zone, XDY_Booleen va_forcage_degrade)
/*
* ARGUMENTS EN ENTREE :
*  T_ZONE_REGUL      pa_zone 	: zone
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Passe la zone en mode dégradée si toutes les stations de la zone n'ont pas de données trafic
*   et si la zone est en mode horaire forcée
------------------------------------------------------*/
{
	T_Liste_Station 	*pl_station = NULL;
	XDY_Entier			i=0, nb_station=0;
	XDY_Booleen			vl_data_presente = XDC_FAUX;
	T_Liste_PRV	*pl_prv=NULL;
	XDY_Booleen vl_zoneDegrade = XDC_FAUX;
	XDY_Entier		vl_JourSemaine_Cycle;
	XDY_Horodate 	vl_Horodate_Cycle;
	char	vl_str[50];

	if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
		                        (int *) &vl_JourSemaine_Cycle, (double *) &vl_Horodate_Cycle ) != XDC_OK )
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de recuperer l horodate courante");
	}

	/* parcours de toutes les stations de la zone
	 * et si les données trafic sont invalides alors la zone est en mode dégradée  */

	pl_station = pa_zone->ListeStation ;

	XZST_03EcritureTrace( XZSTC_INFO,"vl_Horodate_Cycle %lf , vg_Horodate_Msg %lf",vl_Horodate_Cycle,vg_Horodate_Msg[pa_zone->numero_zone]);
	XZST_03EcritureTrace( XZSTC_INFO,"tg_params[XDC_REG_DELAI_LAT_TERDT].Valeur %d",tg_params[XDC_REG_DELAI_LAT_TERDT].Valeur);
	while ((pl_station != NULL) && (!vl_data_presente))
 	{
		for (i=0; i<EREG_NB_DATA_STATION_MAX; i++)
		{
			if(vl_Horodate_Cycle - vg_Horodate_Msg[pa_zone->numero_zone] >= tg_params[XDC_REG_DELAI_LAT_TERDT].Valeur*60)
			{
				vl_data_presente = XDC_FAUX;
			}
			else if( (pl_station->station.data[i].Q_valide != XDC_FAUX) ||
				(pl_station->station.data[i].TO_valide != XDC_FAUX) )
			{
				vl_data_presente = XDC_VRAI;
				break;
			}
		}
		pl_station = pl_station->suivant;
	}

	if (pa_zone->mode_horaire == XDC_VRAI)
			vl_zoneDegrade = XDC_FAUX;
	else if (!vl_data_presente) {
			XZST_03EcritureTrace( XZSTC_INFO,"aucune donnee RADT =>zone degradee");
			vl_zoneDegrade = XDC_VRAI;
	}

	if( (vl_zoneDegrade == XDC_VRAI) || (va_forcage_degrade == XDC_VRAI) ){
		pa_zone->degrade = XDC_VRAI;
		/* Information transmise au module SAE 'administration' */
		sprintf(vl_str,"Zone %s en mode DEGRADE", pa_zone->nom_zone);
		XZST_03EcritureTrace( XZSTC_WARNING, "zone num : %d en mode degrade", pa_zone->numero_zone);
		if (vg_ModeSimu==XDC_FAUX) {
			XZIA_06AffMsgSyst(XDC_MACHINE_GECONF, vl_str);
		}
	}
}
#endif

/*------------------------------------------------------
* SERVICE RENDU :
*  indique si le contexte de regulation a change
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Booleen isNewContext (T_ZONE_REGUL *pa_zone)
/*
* ARGUMENTS EN ENTREE :
*  T_ZONE_REGUL      pa_zone 	: zone
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*  XDC_OK 	: un element a ete trouve
*  XDC_NOK 	: aucun element trouve
*
* CONDITION D'UTILISATION
*
* FONCTION
*
------------------------------------------------------*/
{
	XDY_Booleen vl_res=XDC_FAUX;

	if ((pa_zone->regulation.contexte.chantiers != pa_zone->regulation_proposee.contexte.chantiers) ||
			  (pa_zone->regulation.contexte.declenchement_horaire != pa_zone->regulation_proposee.contexte.declenchement_horaire) ||
			  ( (pa_zone->regulation.contexte.evt_impactant != pa_zone->regulation_proposee.contexte.evt_impactant) &&
					  (pa_zone->regulation.contexte.evt_impactant!=XDC_VRAI) &&
					  (pa_zone->regulation_proposee.contexte.evt_impactant!=XDC_FAUX))
			  ||
			  (pa_zone->regulation.contexte.panne_PRV_critique != pa_zone->regulation_proposee.contexte.panne_PRV_critique) ||
			  (pa_zone->regulation.contexte.panne_PRV_non_critique != pa_zone->regulation_proposee.contexte.panne_PRV_non_critique) ||
			  (pa_zone->regulation.contexte.scenario_ideal != pa_zone->regulation_proposee.contexte.scenario_ideal) )
		 vl_res = XDC_VRAI;

	if ((vl_res) && (pa_zone->regulation.contexte.evt_impactant!=XDC_VRAI)) {
		if ((pa_zone->regulation.contexte.qualification_operateur != pa_zone->regulation_proposee.contexte.qualification_operateur))
			vl_res= XDC_VRAI;
	}
	return (vl_res);
}

