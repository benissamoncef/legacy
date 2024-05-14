/*E*/
/* Fichier : @(#)ereg_cfg.c	1.4      Release : 1.4        Date : 08/28/13
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER ereg_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de general de creation des fichiers de config des equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* SDU		13/03/2012 : Creation DEM/1016
* JMG		11/11/12 : gestion sens
* JMG		15/07/13 : lissage
* SFO           06/10/14 : Adaptation pour compilation sous linux
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.6
* PNI	26/11/20	: Appel de XZAT51 avec horodate nulle SAE201 1.7
* LCL	28/09/23 	: Adaptations CNA DEM-483	
------------------------------------------------------*/

/* fichiers inclus */
#include "ereg_cfg.h"
#include "ereg_dir.h"
#include "ereg_util.h"
#include "xzao551.h"
#include "xzat51.h"
#include "xzao561.h"
#include "xzao570.h"

/* definitions de constantes */
#define CFG_NIV_TRACE_1	XZSTC_INFO
#define CFG_NIV_TRACE_2 XZSTC_DEBUG1

/* definitions de types locaux */

/* definition de macro locales */


/* declaration de variables locales */
int vl_InitCfg = XDC_NOK;

/* declaration de variables globales */
XDY_Basedd			vg_Base = XDC_BASE_CFT;
T_Liste_ZonesRegul* pg_Zones = NULL;
T_Liste_PRV* 		pg_PRV = NULL;
T_Param 			tg_params[EREG_NB_PARAM_MAX];
XDY_Entier			vg_ModeRegul;
XDY_District		vg_NumeroSite;
//T_Param 					tg_params[EREG_NB_PARAM_MAX];

/* declaration de variables externes */
extern XDY_NomSite  vg_NomSite;
extern XDY_Booleen 	vg_ModeSimu;
extern XDY_Horodate	vg_HorodateSimu;
int config_params(XZAOT_Terdt va_terdt, int va_numAppel);

/* declaration de fonctions internes */
int creer_liste_zones (XDY_Eqt va_Numero,
							char *va_Nom,
							XDY_Autoroute va_Autoroute,
							XDY_Sens va_Sens,
							XDY_PR va_Debut,
							XDY_PR va_Fin,
							XDY_VitesseRegul va_Vitesse_amont,
							XDY_Booleen va_Inhibition, XDY_Booleen va_ModeHoraire);

int creer_liste_PRV (XDY_Eqt va_Numero,
						XDY_NomAuto va_nom_autoroute,
						XDY_PR va_PR,
						XDY_Sens va_Sens,
						XDY_VitesseRegul va_Vitesse_nominale,
						XDY_Booleen va_Critique,
						XDY_Entier va_Type,
						XDY_Entier va_Zone);

int creer_liste_station(XZAOT_Comptage va_Station);

int creer_liste_scenarios(XDY_Entier va_numero, char *va_libelle, XDY_Octet va_niveau,
						XDY_Booleen va_nominal, XDY_Booleen va_degrade,
						XDY_Entier va_zone, XDY_Booleen va_inhibition);

int creer_liste_params(XZAOT_Terdt va_param);

int cfg_init()
{
	int			vl_ValRet = XDC_OK;
	int 		i = 0;
	XDY_Basedd      vl_Basedd_in;
	XDY_Entier     vl_Resultat;

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : cfg_init Simu." );

	/*A
	 * Connexion a la base de donnees
	 */
	if ( ( vl_ValRet = XZAG02_Connexion_Base( vg_NomSite ) ) != XDC_OK )
	{
	   XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de se connecter a la base erreur : %d.", vl_ValRet );
	   return XDC_NOK;
	}

	if (vg_ModeSimu==XDC_FAUX)
		strcpy(vg_Base, XDC_BASE_CFG);
	else
		strcpy(vg_Base, XDC_BASE_CFT);

	XZST_03EcritureTrace( CFG_NIV_TRACE_1, "Base DD = %s",vg_Base);

	vg_NumeroSite=XZSC_22NumeroSiteLocal();
	XZST_03EcritureTrace( CFG_NIV_TRACE_1,"Site Local = %d" ,vg_NumeroSite);

	/*chargement des parametres*/
	for (i=0; i<EREG_NB_PARAM_MAX; i++){
		tg_params[i].Initialise = XDC_FAUX;
		tg_params[i].Numero = i;
	}


	if (XZAO570_Params_Regulation(0, vg_ModeSimu, creer_liste_params, vg_Base) != XDC_OK) {
		XZST_03EcritureTrace( XZSTC_WARNING, "pb appel XZAO570_Params_Regulation");
		return (XDC_NOK);
	}

	/*Verfication des parametres de 1 a EREG_NB_PARAM_MAX*/
	for (i=1; i<EREG_NB_PARAM_MAX; i++){
		if (tg_params[i].Numero <= EREG_NUMERO_MAX_PARAM_ATTENDUS &&
				tg_params[i].Initialise == XDC_FAUX)
		{
			XZST_03EcritureTrace( XZSTC_FATAL, "Un des parametres n'a pas ete initialise : %d", i);
			return (XDC_NOK);
		}
		else if ((tg_params[i].Numero > EREG_NUMERO_MAX_PARAM_ATTENDUS &&
				tg_params[i].Initialise == XDC_VRAI))
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "Le parametre %s a ete initialise mais ne sera pas utilise", tg_params[i].Nom);
		}
	}

	/* Chargement des parametres */
	if (XZAO570_Params_Regulation ( 0, 0, config_params, vg_Base) != XDC_OK){
		XZST_03EcritureTrace( XZSTC_WARNING, "pb appel XZAO570_Params_Regulation");
		return (XDC_NOK);
	}

	/* Chargement de la liste des zones */
#ifdef _HPUX_SOURCE
	if (XZAO557_Liste_Zones(vg_NumeroSite, creer_liste_zones, vg_Base, &vl_Resultat) != XDC_OK) {
#else
	  if (XZAO557_Liste_Zones(vg_NumeroSite, (XDY_FonctionInt)creer_liste_zones, vg_Base, &vl_Resultat) != XDC_OK) {
#endif
		XZST_03EcritureTrace( XZSTC_WARNING, "pb appel XZAO557_Liste_Zones");
		return (XDC_NOK);
	}

	switch(vg_NumeroSite) {
	case XDC_DP :
		vg_ModeRegul = getParamValue(EREG_PARAM_GEN_REG_NIVEAU_AUTO_SECTEUR_PR);
		break;
	case XDC_VC :
		vg_ModeRegul = getParamValue(EREG_PARAM_GEN_REG_NIVEAU_AUTO_SECTEUR_CA);
		break;
	default :
		vg_ModeRegul = /*XDC_REG_MODE_AUTO*/XDC_REG_MODE_MANUEL;
		break;
	}

	XZST_03EcritureTrace( XZSTC_INFO, "OUT : cfg_init mode = %d",vg_ModeRegul );
	return (XDC_OK);
}

XDY_Entier getParamValue(XDY_Eqt va_numero){
XDY_Entier vl_result;

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : getParamValue");
	vl_result = -1;

	if (va_numero >= EREG_NB_PARAM_MAX)
		XZST_03EcritureTrace( XZSTC_WARNING, "getParamValue : numero de parametre %d INCORRECT", va_numero);
	else
	{
		if (va_numero > EREG_NUMERO_MAX_PARAM_ATTENDUS)
			XZST_03EcritureTrace( XZSTC_WARNING, "getParamValue : numero de parametre %d Hors de la plage attentue %d", va_numero, EREG_NUMERO_MAX_PARAM_ATTENDUS);

		if (tg_params[va_numero].Initialise == XDC_VRAI){
			vl_result = tg_params[va_numero].Valeur;
		}
		else
			XZST_03EcritureTrace( XZSTC_WARNING, "getParamValue : numero de parametre %d Hors de la plage attentue %d", va_numero, EREG_NUMERO_MAX_PARAM_ATTENDUS);
	}
	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : getParamValue=%d", vl_result);
	return vl_result;
}

int creer_liste_conditions_zone(XZAOT_Condition_Scenarios va_condition){

	XZST_03EcritureTrace( XZSTC_INTERFACE, "IN creer_liste_conditions_zone : numero %d(%s), zone %d, station %d",
			va_condition.Numero,
			va_condition.Libelle,
			va_condition.Zone,
			va_condition.Station);

	T_Liste_ZonesRegul *pl_Zone;
	T_Liste_Condition_Zone   *pl_NewCondition;
	int i;

	/*parcours de toutes les zones*/
	pl_Zone = pg_Zones;
	while (pl_Zone!= NULL)
	{
		if (pl_Zone->zone.numero_zone == va_condition.Zone)
			break;

		pl_Zone=pl_Zone->suivant;
	}

	if (pl_Zone == NULL){
		XZST_03EcritureTrace ( XZSTC_WARNING, "OUT creer_liste_conditions : ATTENTION Aucune Zone existante (%d) trouvée pour la condition %d(%s) !",
				va_condition.Zone,
				va_condition.Numero,
				va_condition.Libelle);
		return XDC_NOK;
	}

	/*A Allocation memoire pour la nouvelle structure */
	if ( (pl_NewCondition = (T_Liste_Condition_Zone *) malloc ( sizeof(T_Liste_Condition_Zone))) == NULL)
	{
		/* Ecriture trace */
		XZST_03EcritureTrace ( XZSTC_FATAL, " OUT: creer_liste_conditions_zone Allocation memoire impossible");
		return XDC_NOK;
	}

	/*A Initialise la structure avec les valeurs passees en parametre */
	pl_NewCondition->condition.id = va_condition.Numero;
	pl_NewCondition->condition.id_scenario = va_condition.Scenario;
	pl_NewCondition->condition.num_station = va_condition.Station;
	pl_NewCondition->condition.sens_station = (XDY_Sens)va_condition.Sens;
	strcpy(pl_NewCondition->condition.libelle, va_condition.Libelle);
	pl_NewCondition->condition.station = NULL;
	pl_NewCondition->condition.type_condition = va_condition.Type_horaire_trafic;
	pl_NewCondition->condition.type_condition_horaire = va_condition.Type_condition_horaire;
	pl_NewCondition->condition.type_condition_horaire_jour = va_condition.Type_jour;
	if (va_condition.Condition_simple == EREG_CONDITION_SIMPLE)
		pl_NewCondition->condition.condition_simple = XDC_VRAI;
	else
		pl_NewCondition->condition.condition_simple = XDC_FAUX;

	pl_NewCondition->condition.seuil = va_condition.Seuil;
	pl_NewCondition->condition.type_donnee_trafic = va_condition.Donnee_trafic;
	pl_NewCondition->condition.operateur_comparaison = va_condition.Operateur;
	pl_NewCondition->condition.horodate_debut = va_condition.Horodate_debut;
	pl_NewCondition->condition.horodate_fin = va_condition.Horodate_fin;

	pl_NewCondition->condition.heure_debut = va_condition.Heure_debut;
	pl_NewCondition->condition.heure_fin = va_condition.Heure_fin;
	pl_NewCondition->condition.minute_debut = va_condition.Minute_debut;
	pl_NewCondition->condition.minute_fin = va_condition.Minute_fin;

	pl_NewCondition->condition.id_condition_alternative = va_condition.Cond_alternative;
	pl_NewCondition->condition.liste_id_conditions_filles[0] = va_condition.Conjonction1;
	pl_NewCondition->condition.liste_id_conditions_filles[1] = va_condition.Conjonction2;
	pl_NewCondition->condition.liste_id_conditions_filles[2] = va_condition.Conjonction3;
	pl_NewCondition->condition.liste_id_conditions_filles[3] = va_condition.Conjonction4;
	pl_NewCondition->condition.liste_conditions_filles = NULL;

	if (pl_NewCondition->condition.type_condition != XDC_REG_CONDITION_TRAF_ACTIV &&
			pl_NewCondition->condition.type_condition != XDC_REG_CONDITION_TRAF_DESACTIV &&
			pl_NewCondition->condition.type_condition != XDC_REG_CONDITION_TRAF_HORAIRE)
	{
		XZST_03EcritureTrace(XZSTC_FATAL, "Type de Condition %d Inconnu",
				pl_NewCondition->condition.type_condition);
		return XDC_NOK;
	}

	if (pl_NewCondition->condition.condition_simple==XDC_VRAI)
		XZST_03EcritureTrace(CFG_NIV_TRACE_1, "+++++ CREATION CONDITION %s Simple %d",
							pl_NewCondition->condition.type_condition==XDC_REG_CONDITION_TRAF_ACTIV?"Activation":
									pl_NewCondition->condition.type_condition==XDC_REG_CONDITION_TRAF_DESACTIV?"Desactivation":"Horaire",
							pl_NewCondition->condition.id);
	else
		XZST_03EcritureTrace(CFG_NIV_TRACE_1, "+++++ CREATION CONDITION %s Complexe[%d,%d,%d,%d] %d",
							pl_NewCondition->condition.type_condition==XDC_REG_CONDITION_TRAF_ACTIV?"Activation":
									pl_NewCondition->condition.type_condition==XDC_REG_CONDITION_TRAF_DESACTIV?"Desactivation":"Horaire",
							pl_NewCondition->condition.liste_id_conditions_filles[0],
							pl_NewCondition->condition.liste_id_conditions_filles[1],
							pl_NewCondition->condition.liste_id_conditions_filles[2],
							pl_NewCondition->condition.liste_id_conditions_filles[3],
							pl_NewCondition->condition.id);

	if (pl_NewCondition->condition.type_condition == XDC_REG_CONDITION_TRAF_HORAIRE){
		XZSMT_Horodate vl_Horodate_debut="vide";
		XZSMT_Horodate vl_Horodate_fin="vide";
		if (pl_NewCondition->condition.horodate_debut != 0)
			if(XZSM_11ConversionHorodate( pl_NewCondition->condition.horodate_debut, &vl_Horodate_debut) != XDC_OK){
				XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de convertir l'horodate courante en chaine");
				return XDC_NOK;
			}
		if (pl_NewCondition->condition.horodate_fin != 0)
			if(XZSM_11ConversionHorodate( pl_NewCondition->condition.horodate_fin, &vl_Horodate_fin) != XDC_OK){
				XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de convertir l'horodate courante en chaine");
				return XDC_NOK;
			}
		XZST_03EcritureTrace(CFG_NIV_TRACE_1, "Horodate de %s a %s", vl_Horodate_debut, vl_Horodate_fin);
	}

	/*A Ajout a la liste si existante, creation sinon */
	/* Insertion en Tete */
	pl_NewCondition->suivant = pl_Zone->zone.ListeConditions;
	pl_Zone->zone.ListeConditions = pl_NewCondition;

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : creer_liste_condition %d", pl_NewCondition->condition.id);
	return (XDC_OK);
}

int creer_liste_conditions_scenario(XZAOT_Condition_Scenarios va_condition){
T_Liste_ZonesRegul *pl_Zone;
T_Liste_Scenario   *pl_Scenario;
T_CONDITION   *pl_ConditionZone;
T_Liste_Condition_Scenario   *pl_NewCondition;
T_Liste_Scenario   *pl_liste_scenarios;

	XZST_03EcritureTrace( XZSTC_INFO, "IN creer_liste_conditions_scenario : numero %d(%s), scenario %d, station %d(%d), zone %d",
			va_condition.Numero,
			va_condition.Libelle,
			va_condition.Scenario,
			va_condition.Station,
			va_condition.Sens,
			va_condition.Zone);

	/*parcours de toutes les zones*/
	pl_Zone = pg_Zones;
	while (pl_Zone!= NULL)
	{
		if (pl_Zone->zone.numero_zone == va_condition.Zone)
			break;

		pl_Zone=pl_Zone->suivant;
	}

	if (pl_Zone == NULL){
			XZST_03EcritureTrace ( XZSTC_WARNING, "OUT creer_liste_conditions_scenario : ATTENTION Aucune Zone existante (%d) trouvée pour la condition %d(%s) !",
					va_condition.Zone,
					va_condition.Numero,
					va_condition.Libelle);
			return XDC_NOK;
	}
	else{
		if (va_condition.Scenario == EREG_NO_SCENARIO){
			XZST_03EcritureTrace ( XZSTC_WARNING, "OUT creer_liste_conditions_scenario : Condition %d(%s) sans Scenario  !",
					va_condition.Numero,
					va_condition.Libelle);
			return XDC_NOK;
		}else
		{
			/* on recherche le scenario sur cette zone*/
			pl_liste_scenarios = pl_Zone->zone.ListeScenario;

			while (pl_liste_scenarios != NULL){
				if (pl_liste_scenarios->scenario.id == va_condition.Scenario)
					break;
				pl_liste_scenarios = pl_liste_scenarios->suivant;
			}

			if (pl_liste_scenarios == NULL)
			{
				XZST_03EcritureTrace ( XZSTC_WARNING, "OUT creer_liste_conditions_scenario : ATTENTION Aucun scenario existant (%d) trouvé pour la condition %d(%s) !",
						va_condition.Scenario,
						va_condition.Numero,
						va_condition.Libelle);
				return XDC_NOK;
			}

			pl_Scenario = pl_liste_scenarios;
		}
	}

	/* ajout de la Condition dans la liste des conditions du scenario correspondant */

	/* recherche de la condition dans la zone */
	pl_ConditionZone = NULL;
	if (rechercher_condition(va_condition.Numero, pl_Zone->zone.ListeConditions, &pl_ConditionZone) != XDC_OK)
	{
		XZST_03EcritureTrace ( XZSTC_WARNING, " OUT: creer_liste_conditions_scenario La condition %d n'a pas ete chargee", va_condition.Numero);
		return XDC_NOK;
	}

	/*A Allocation memoire pour la nouvelle structure */
	if ( (pl_NewCondition = (T_Liste_Condition_Scenario *) malloc ( sizeof(T_Liste_Condition_Scenario))) == NULL)
	{
		/* Ecriture trace */
		XZST_03EcritureTrace ( XZSTC_FATAL, " OUT: creer_liste_conditions_scenario Allocation memoire impossible");
		return XDC_NOK;
	}

	pl_NewCondition->condition = pl_ConditionZone;

	/* Insertion en tete */
	pl_NewCondition->suivant = pl_Scenario->scenario.conditions;
	pl_Scenario->scenario.conditions = pl_NewCondition;

	if (pl_NewCondition->condition->type_condition == EREG_CONDITION_TYPE_HORAIRE)
		pl_Scenario->scenario.has_conditions_horaire = XDC_VRAI;

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : creer_liste_condition_scenario");
	return (XDC_OK);
}

int creer_liste_commandes(XZAOT_Commandes_Scenarios va_commande) {
T_Liste_ZonesRegul *pl_Zones;
T_Liste_Scenario   *pl_listeScenarios;
T_Liste_Commande   *pl_Commande;
T_Liste_Commande   *pl_NewCommande;
T_PRV	*pl_tmp_PRV=NULL;

	XZST_03EcritureTrace( XZSTC_FONCTION, "creer_liste_commandes :  scenario %d, prv %d, vitesse %d, critique %d",
			va_commande.scenario,
			va_commande.prv,
			va_commande.commande,
			va_commande.prv_critique);

	XZST_03EcritureTrace( XZSTC_MESSAGE, "Parcours de toutes les zones ");
	/*parcours de toutes les zones*/
	pl_Zones = pg_Zones;
	while (pl_Zones!= NULL)
	{
		pl_listeScenarios = pl_Zones->zone.ListeScenario;

		/*parcours de tous les scenarios de la zone*/
		/*si c est mon scenario va_scenario*/

		XZST_03EcritureTrace( XZSTC_MESSAGE, "Parcours de tous les scenarios de la zone %s", pl_Zones->zone.nom_zone);
		while (pl_listeScenarios!= NULL)
		{
			if (pl_listeScenarios->scenario.id == va_commande.scenario)
				break;
			pl_listeScenarios=pl_listeScenarios->suivant;
		}

			/* pointeur vers le PRV d'une commande*/
			if (rechercher_prv(va_commande.prv, pl_Zones->zone.ListePRV, &pl_tmp_PRV) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "Erreur : le PRV %d est Introuvable", va_commande.prv);
				return (XDC_OK);
			}

		if (pl_listeScenarios != NULL )
		{
			/*ajout de la commande dans scenario.ListeCommandes;
			 */

			/*A Allocation memoire pour la nouvelle structure */
			if ( (pl_NewCommande = (T_Liste_Commande *) malloc ( sizeof(T_Liste_Commande))) == NULL)
			{
				/* Ecriture trace */
				XZST_03EcritureTrace ( XZSTC_WARNING, " OUT: creer_liste_commandes Allocation memoire impossible");
				return XDC_NOK;
			}

			/*A Initialise la structure avec les valeurs passees en parametre */
			pl_NewCommande->commande.vitesse = va_commande.commande;
			pl_NewCommande->commande.critique = va_commande.prv_critique;
			//pl_NewCommande->commande.bandeau = va_bandeau;
			pl_NewCommande->commande.equipement = va_commande.prv;
			pl_NewCommande->commande.prv = pl_tmp_PRV;
		/*	pl_NewCommande->commande.prv = NULL;*/

			/*A Ajout a la liste si existante, creation sinon */
			XZST_03EcritureTrace(XZSTC_MESSAGE, "CREATION COMMANDE dans la zone %s et le scenario %d(%s) pour le PRV %d",
													pl_Zones->zone.nom_zone,
													pl_listeScenarios->scenario.id,
													pl_listeScenarios->scenario.libelle,
													pl_NewCommande->commande.equipement);

			if (pl_listeScenarios->scenario.ListeCommandes == NULL)
			{
				pl_listeScenarios->scenario.ListeCommandes = pl_NewCommande;
				pl_listeScenarios->scenario.ListeCommandes->suivant = NULL;
			}
			else
			{
				pl_Commande = pl_listeScenarios->scenario.ListeCommandes;
				while (pl_Commande->suivant != NULL)
					pl_Commande = pl_Commande->suivant;

				pl_Commande->suivant = pl_NewCommande;
				pl_NewCommande->suivant = NULL;
			}
			break;
		}

		pl_Zones=pl_Zones->suivant;

	}

	if (pl_listeScenarios == NULL)
	{
		/*trace warning*/
		XZST_03EcritureTrace ( XZSTC_WARNING, "La Commande sur l'equipement %d n'appartient a aucun Scenario connu %d",
				va_commande.prv,
				va_commande.scenario);
		return XDC_NOK;
	}


	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : creer_liste_commandes");
	return (XDC_OK);
}



int creer_liste_scenarios(XDY_Entier va_numero, char *va_libelle, XDY_Octet va_niveau,
						XDY_Booleen va_nominal, XDY_Booleen va_degrade,
						XDY_Entier va_zone, XDY_Booleen va_inhibition)
{
	T_Liste_Scenario *pl_NewScenario;
	T_Liste_Scenario *pl_Scenario;
	T_Liste_ZonesRegul *pl_Zones;
	XDY_Entier	vl_Resultat;

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN creer_liste_scenarios :  numero %d, libelle %s, inhibe %d, nominal %d, degarde %d, niveau %d, zone %d",
			va_numero,
			va_libelle,
			va_inhibition,
			va_nominal,
			va_degrade,
			va_niveau,
			va_zone);

	/*Recherche de la zone de regulation*/
	pl_Zones = pg_Zones;
	while (pl_Zones!= NULL)
	{
		if (pl_Zones->zone.numero_zone == va_zone)
			break;
		pl_Zones=pl_Zones->suivant;
	}

	if (pl_Zones == NULL)
	{
		/*trace warning*/
		XZST_03EcritureTrace ( XZSTC_WARNING, "OUT creer_liste_scenarios: le scenario %d n'appartient a aucune zone (%d)!",
				va_numero,
				va_zone);
		return XDC_NOK;
	}

	/*A Allocation memoire pour la nouvelle structure */
	if (va_inhibition == XDC_FAUX){

		if (( pl_NewScenario = (T_Liste_Scenario *) malloc ( sizeof(T_Liste_Scenario) )) == NULL)
		{
			/* Ecriture trace */
			XZST_03EcritureTrace ( XZSTC_WARNING, " OUT: creer_liste_scenarios Allocation memoire impossible");
			return XDC_NOK;
		}

		/*A Initialise la structure avec les valeurs passees en parametre */
		pl_NewScenario->scenario.ListeCommandes=NULL;
/*		pl_NewScenario->scenario.conditions_activ = NULL;
		pl_NewScenario->scenario.conditions_horaire = NULL;
		pl_NewScenario->scenario.conditions_desactiv =NULL;
*/
		pl_NewScenario->scenario.conditions=NULL;
		pl_NewScenario->scenario.degrade =va_degrade;
		pl_NewScenario->scenario.id = va_numero;
/*		pl_NewScenario->scenario.inhibe = va_inhibition; */
		strcpy(pl_NewScenario->scenario.libelle, va_libelle);
		pl_NewScenario->scenario.niveau = va_niveau;
		pl_NewScenario->scenario.nominal = va_nominal;
		pl_NewScenario->scenario.has_conditions_horaire = FALSE;
		pl_NewScenario->scenario.zone_scenario = va_zone;


		/*A Ajout a la liste des scenarios de la zone si existante, creation sinon */
		XZST_03EcritureTrace(CFG_NIV_TRACE_1, "+++++ CREATION SCENARIO %d (%s) dans la zone %d (%s)",
				pl_NewScenario->scenario.id,
				pl_NewScenario->scenario.libelle,
				pl_Zones->zone.numero_zone,
				pl_Zones->zone.nom_zone);

		if (pl_Zones->zone.ListeScenario == NULL)
		{
			pl_Zones->zone.ListeScenario = pl_NewScenario;
			pl_Zones->zone.ListeScenario->suivant = NULL;
			pl_Zones->zone.ListeScenario->precedent = NULL;
		}
		else
		{
			pl_Scenario = pl_Zones->zone.ListeScenario;
			while (pl_Scenario->suivant != NULL)
				pl_Scenario = pl_Scenario->suivant;

			pl_Scenario->suivant = pl_NewScenario;
			pl_NewScenario->precedent = pl_Scenario;
			pl_NewScenario->suivant = NULL;
		}


		/*chargement des commandes pour chaque scenario	 */
		if (XZAO551_Commandes_Scenarios( vg_Base, va_numero, creer_liste_commandes,&vl_Resultat)!= XDC_OK)
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "pb appel XZAO551_Commandes_Scenarios");
					return (XDC_NOK);
		}

		/*chargement des conditions*/
		if (XZAO561_Conditions_Scenarios( vg_Base, va_zone, va_numero, creer_liste_conditions_scenario,&vl_Resultat)!= XDC_OK)
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "pb appel XZAO561_Conditions_Scenarios");
			return (XDC_NOK);
		}
	}

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : creer_liste_scenarios");
	return (XDC_OK);
}

int creer_liste_params(XZAOT_Terdt va_param)
{

	XZST_03EcritureTrace ( XZSTC_FONCTION,"creer_liste_params : numero=%d, type=%d, valeur=%d, nom=%s",
						va_param.Numero,
						va_param.Type,
						va_param.Valeur,
						va_param.Nom);

	strcpy(tg_params[va_param.Numero].Nom,va_param.Nom);
	tg_params[va_param.Numero].Numero = va_param.Numero;
	tg_params[va_param.Numero].Type = va_param.Type;
	tg_params[va_param.Numero].Valeur = va_param.Valeur;
	tg_params[va_param.Numero].Initialise = XDC_VRAI;

	XZST_03EcritureTrace ( XZSTC_FONCTION,"creer_liste_params : out");
	return (XDC_OK);
}

/* Creation de la fonction uilisateur de creation de la liste des zones*/
int creer_liste_zones (XDY_Eqt va_Numero,
							char *va_Nom,
							XDY_Autoroute va_Autoroute,
							XDY_Sens va_Sens,
							XDY_PR va_Debut,
							XDY_PR va_Fin,
							XDY_VitesseRegul va_Vitesse_amont,
							XDY_Booleen va_Inhibition,
							XDY_Booleen va_ModeHoraire)
{
	/*LCL*/
	XZST_03EcritureTrace ( XZSTC_FONCTION,
			"IN creer_liste_zones : va_Numero=%d, va_Nom=%s, va_Autoroute=%d, va_Sens=%d,va_Debut=%d,va_Fin=%d,va_Vitesse_amont=%d,va_Inhibition=%d, mode horaire %d",
				va_Numero,
				va_Nom,
				va_Autoroute,
				va_Sens,
				va_Debut,
				va_Fin,
				va_Vitesse_amont,
				va_Inhibition,
				va_ModeHoraire);

	T_Liste_Scenario 	*pl_listeScenarios;
	T_Liste_Commande	*pl_listeCommandes;
	T_Liste_Condition_Zone   *pl_listeConditions;
	XDY_Entier     		vl_Resultat ;
	T_PRV				*pl_tmp_PRV;
	T_STATION			*pl_tmp_STATION;
	T_Liste_ZonesRegul 	*pl_NewZone;
	T_Liste_ZonesRegul 	*pl_Zones;
	int i;
	T_Liste_Condition_Scenario   *pl_conditionScenario;
	T_CONDITION					*pl_condition;

	if (va_Inhibition == XDC_VRAI) {
		XZST_03EcritureTrace ( CFG_NIV_TRACE_2, "ZONE %d (%s) inhibee => ne pas traiter", va_Numero, va_Nom);
		return (XDC_OK);
	}

	/*A Allocation memoire pour la nouvelle structure */
	if (( pl_NewZone = (T_Liste_ZonesRegul *) malloc ( sizeof(T_Liste_ZonesRegul) )) == NULL)
	{
		/* Ecriture trace */
		XZST_03EcritureTrace ( XZSTC_WARNING, " OUT: creer_liste_zones Allocation memoire impossible");
		return XDC_NOK;
	}

	/*A Initialise la structure avec les valeurs passees en parametre */
	pl_NewZone->zone.numero_zone = va_Numero;
	strcpy(pl_NewZone->zone.nom_zone, va_Nom);
	rtrim(pl_NewZone->zone.nom_zone);
	pl_NewZone->zone.autoroute_prv = va_Autoroute;
	pl_NewZone->zone.sens_prv = va_Sens;
	pl_NewZone->zone.pr_debut = va_Debut;
	pl_NewZone->zone.pr_fin = va_Fin;
	pl_NewZone->zone.vitesse_amont = va_Vitesse_amont;
	pl_NewZone->zone.inhibition = va_Inhibition;
	pl_NewZone->zone.mode_horaire = va_ModeHoraire;
	pl_NewZone->zone.degrade = XDC_FAUX;
	pl_NewZone->zone.fmc_regulation.numero=-1;
	pl_NewZone->zone.fmc_regulation.cle = -1;
	pl_NewZone->zone.regulation_en_cours=XDC_FAUX;
	pl_NewZone->zone.horodate_forcage=0;
	pl_NewZone->zone.indicateur_forcage=XDC_FAUX;
	pl_NewZone->zone.scenarioNominal = NULL;
	pl_NewZone->zone.ListePRV=NULL;
	pl_NewZone->zone.ListeStation=NULL;
	pl_NewZone->zone.ListeScenario=NULL;
	pl_NewZone->zone.ListeConditions=NULL;
	pl_NewZone->zone.regulation.contexte.chantiers = XDC_FAUX;
	pl_NewZone->zone.regulation.contexte.declenchement_horaire = XDC_FAUX;
	pl_NewZone->zone.regulation.contexte.evt_impactant = XDC_FAUX;
	pl_NewZone->zone.regulation.contexte.panne_PRV_critique = XDC_FAUX;
	pl_NewZone->zone.regulation.contexte.panne_PRV_non_critique = XDC_FAUX;
	pl_NewZone->zone.regulation.contexte.qualification_operateur = XDC_FAUX;
	pl_NewZone->zone.regulation.contexte.evt_mis_a_jour = XDC_FAUX;
	pl_NewZone->zone.regulation.contexte.scenario_ideal = XDC_FAUX;
	pl_NewZone->zone.regulation.contexte.alerte_deja_envoyee= XDC_FAUX;
	pl_NewZone->zone.regulation.horodate_regulation = 0;
	pl_NewZone->zone.regulation.indicateur_horaire = XDC_FAUX;
	pl_NewZone->zone.regulation.mesure = -1;
	pl_NewZone->zone.regulation.reponse_operateur = -1;
	strcpy(pl_NewZone->zone.regulation.transitoire,"");
	pl_NewZone->zone.regulation.scenario.id = -1;
	pl_NewZone->zone.regulation.scenario.ListeCommandes = NULL;
	pl_NewZone->zone.regulation.scenario.conditions = NULL;
	strcpy(pl_NewZone->zone.regulation.alerte_envoyee, "");
	pl_NewZone->zone.regulation.sous_type_alerte_envoyee = -1;

	pl_NewZone->zone.regulation_proposee.contexte.chantiers = XDC_FAUX;
	pl_NewZone->zone.regulation_proposee.contexte.declenchement_horaire = XDC_FAUX;
	pl_NewZone->zone.regulation_proposee.contexte.panne_PRV_critique = XDC_FAUX;
	pl_NewZone->zone.regulation_proposee.contexte.panne_PRV_non_critique = XDC_FAUX;
	pl_NewZone->zone.regulation_proposee.contexte.qualification_operateur = XDC_FAUX;
	pl_NewZone->zone.regulation_proposee.contexte.scenario_ideal = XDC_FAUX;
	pl_NewZone->zone.regulation_proposee.horodate_regulation = 0;
	pl_NewZone->zone.regulation_proposee.indicateur_horaire = XDC_FAUX;
	pl_NewZone->zone.regulation_proposee.mesure = -1;
	pl_NewZone->zone.regulation_proposee.reponse_operateur = -1;
	strcpy(pl_NewZone->zone.regulation_proposee.transitoire,"");
	pl_NewZone->zone.regulation_proposee.scenario.id = -1;
	pl_NewZone->zone.regulation_proposee.scenario.ListeCommandes = NULL;
	pl_NewZone->zone.regulation_proposee.contexte.evt_mis_a_jour = XDC_FAUX;
	pl_NewZone->zone.regulation_proposee.contexte.alerte_deja_envoyee= XDC_FAUX;
	pl_NewZone->zone.regulation_proposee.scenario.conditions = NULL;
	strcpy(pl_NewZone->zone.regulation.alerte_envoyee, "");
	pl_NewZone->zone.regulation.sous_type_alerte_envoyee = -1;

	pl_NewZone->zone.pilotage_manuel = XDC_FAUX;
	pl_NewZone->zone.ListeAlertes = NULL;
	pl_NewZone->suivant = NULL;

	/*A Ajout a la liste si existante, creation sinon */
	XZST_03EcritureTrace(CFG_NIV_TRACE_1, "+++++ CREATION ZONE %d (%s)", pl_NewZone->zone.numero_zone, pl_NewZone->zone.nom_zone);

	pl_NewZone->suivant = pg_Zones;
	pg_Zones = pl_NewZone;

	XZST_03EcritureTrace(CFG_NIV_TRACE_2, "___CHARGEMENT des CONDITIONS de la ZONE %d (%s)___", pl_NewZone->zone.numero_zone, pl_NewZone->zone.nom_zone);
	/*chargement de toutes les conditions applicables sur la zone*/
	if (XZAO561_Conditions_Scenarios( vg_Base, va_Numero, EREG_NO_SCENARIO, creer_liste_conditions_zone ,&vl_Resultat)!= XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "pb appel XZAO561_Conditions_Scenarios");
		return (XDC_NOK);
	}

	XZST_03EcritureTrace(CFG_NIV_TRACE_2, "___CHARGEMENT des PRV de la ZONE %d (%s)___", pl_NewZone->zone.numero_zone, pl_NewZone->zone.nom_zone);
	/*chargement des PRV de la zone*/
#ifdef _HPUX_SOURCE
	if (XZAO556_PRV_Zone(va_Numero, creer_liste_PRV,vg_Base, &vl_Resultat) != XDC_OK) {
#else
	  if (XZAO556_PRV_Zone(va_Numero, (XDY_FonctionInt)creer_liste_PRV,vg_Base, &vl_Resultat) != XDC_OK) {
#endif
		XZST_03EcritureTrace ( XZSTC_WARNING, " pb appel a XZAO556_PRV_Zone");
		return XDC_NOK;
	}

	XZST_03EcritureTrace(CFG_NIV_TRACE_2, "___CHARGEMENT des STATIONS de la ZONE %d (%s)___", pl_NewZone->zone.numero_zone, pl_NewZone->zone.nom_zone);
	/*Chargement de la liste des stations pour chaque zone*/
	if (XZAO563_Comptage(va_Numero, creer_liste_station, vg_Base, &vl_Resultat)!= XDC_OK) {
		XZST_03EcritureTrace( XZSTC_WARNING, "pb appel XZAO563_Comptage");
		return (XDC_NOK);
	}

	XZST_03EcritureTrace(CFG_NIV_TRACE_2, "___CHARGEMENT des SCENARIOS de la ZONE %d (%s)___", pl_NewZone->zone.numero_zone, pl_NewZone->zone.nom_zone);
	/*chargement des scenarios de la zone*/
#ifdef _HPUX_SOURCE
	if (XZAO558_Scenarios_Zone(va_Numero, creer_liste_scenarios,vg_Base, &vl_Resultat) != XDC_OK) {
#else
	  if (XZAO558_Scenarios_Zone(va_Numero, (XDY_FonctionInt)creer_liste_scenarios,vg_Base, &vl_Resultat) != XDC_OK) {
#endif
		XZST_03EcritureTrace ( XZSTC_WARNING, " pb appel a XZAO558_Scenarios_Zone");
		return XDC_NOK;
	}

	XZST_03EcritureTrace(CFG_NIV_TRACE_1, "_____INITIALISATION DES POINTEURS de la ZONE %d (%s)___", pl_NewZone->zone.numero_zone, pl_NewZone->zone.nom_zone);
	/* Initialisation des pointeurs entre les differentes structures */
	/* pointeur vers Station des conditions de la zone */
	pl_listeConditions = pl_NewZone->zone.ListeConditions;
	while (pl_listeConditions != NULL){
		XZST_03EcritureTrace(CFG_NIV_TRACE_2, "_____PTR ... de la CONDITION %d_____", pl_listeConditions->condition.id);
		/* pointeur vers la Station des Conditions d'activation*/
		if (rechercher_station(pl_listeConditions->condition.num_station, 
					pl_listeConditions->condition.sens_station,
					pl_NewZone->zone.ListeStation, &pl_tmp_STATION) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "ERREUR : la Station %d(%d) est Introuvable", pl_listeConditions->condition.num_station,
						pl_listeConditions->condition.sens_station	);
			return XDC_NOK;
		}else
			pl_listeConditions->condition.station = pl_tmp_STATION;

		XZST_03EcritureTrace(XZSTC_INFO, "******* La Station %d(%d) de la Conditions %d(%s) a ete associee avec la Station %d(%d) de la Zone %d ********",
				pl_listeConditions->condition.num_station,
				pl_listeConditions->condition.sens_station,
				pl_listeConditions->condition.id,
				pl_listeConditions->condition.libelle,
				pl_listeConditions->condition.station->station,
				pl_listeConditions->condition.station->sens,
				pl_listeConditions->condition.station->zone_station);

		/* Condition Alternative */
		if (pl_listeConditions->condition.id_condition_alternative > EREG_CONDITIONS_NULL)
		{
			XZST_03EcritureTrace(CFG_NIV_TRACE_2, "_____PTR CONDITION ALTERNATIVE %d_____", pl_listeConditions->condition.id_condition_alternative);
			if (rechercher_condition(pl_listeConditions->condition.id_condition_alternative,
									pl_NewZone->zone.ListeConditions,
									&(pl_listeConditions->condition.condition_alternative) ) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_FATAL, "Impossible de Charger la condition alternative %d",
						pl_listeConditions->condition.id_condition_alternative);
			}
		}

		if (pl_listeConditions->condition.condition_simple == XDC_FAUX){
			/* Conditions Filles */
			for (i=0; i<EREG_CONDITIONS_NB_MAX_FILLES; i++){
				if (pl_listeConditions->condition.liste_id_conditions_filles[i] > EREG_CONDITIONS_NULL)
				{
					XZST_03EcritureTrace(CFG_NIV_TRACE_2, "_____PTR CONDITION FILLE %d_____", pl_listeConditions->condition.liste_id_conditions_filles[i]);
					if (rechercher_condition(pl_listeConditions->condition.liste_id_conditions_filles[i],
												pl_NewZone->zone.ListeConditions,
												&pl_condition) != XDC_OK)
					{
						XZST_03EcritureTrace(XZSTC_FATAL, "Impossible de Charger la condition fille %d",
								pl_listeConditions->condition.liste_id_conditions_filles[i]);
					}else
					{
						pl_conditionScenario = (T_Liste_Condition_Scenario *) malloc(sizeof(T_Liste_Condition_Scenario));
						pl_conditionScenario->condition = pl_condition;
						pl_conditionScenario->suivant = pl_listeConditions->condition.liste_conditions_filles;
						pl_listeConditions->condition.liste_conditions_filles = pl_conditionScenario;
					}
				}
			}
		}
		pl_listeConditions = pl_listeConditions->suivant;
	}

	pl_listeScenarios = pl_NewZone->zone.ListeScenario;

	pl_NewZone->zone.scenarioNominal = NULL;
	while (pl_listeScenarios != NULL){
		XZST_03EcritureTrace(CFG_NIV_TRACE_2, "_____PTR ... du SCENARIO %d_____", pl_listeScenarios->scenario.id);
		/* pointeur vers le scenario nomnal d'une zone*/
		if (pl_listeScenarios->scenario.nominal == XDC_VRAI){
			pl_NewZone->zone.scenarioNominal = &(pl_listeScenarios->scenario);
		}

/*		pl_listeCommandes = pl_listeScenarios->scenario.ListeCommandes;
		while (pl_listeCommandes != NULL){
			if (rechercher_prv(pl_listeCommandes->commande.equipement, pl_NewZone->zone.ListePRV, &pl_tmp_PRV) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "Erreur : le PRV %d est Introuvable", pl_listeCommandes->commande.equipement);
			}else
				pl_listeCommandes->commande.prv = pl_tmp_PRV;

			pl_listeCommandes = pl_listeCommandes->suivant;
		}
*/
		pl_listeScenarios = pl_listeScenarios->suivant;
	}

	if (pl_NewZone->zone.scenarioNominal == NULL){
		XZST_03EcritureTrace(XZSTC_WARNING, "ERREUR : Il n'y a pas de Scenario Nominal pour la Zone %d (%s)",
								pl_NewZone->zone.numero_zone,
								pl_NewZone->zone.nom_zone);
		return XDC_NOK;

	}


	XZST_03EcritureTrace ( XZSTC_FONCTION,"creer_liste_zones sortie");
	return (XDC_OK);
}



/* Creation de la fonction uilisateur de creation de la liste des PRV*/
int creer_liste_PRV (XDY_Eqt va_Numero,
						XDY_NomAuto va_nom_autoroute,
						XDY_PR va_PR,
						XDY_Sens va_Sens,
						XDY_VitesseRegul va_Vitesse_nominale,
						XDY_Booleen va_Critique,
						XDY_Entier va_Type,
						XDY_Entier va_Zone)
{
	T_Liste_PRV *pl_NewPRV;
	T_Liste_PRV *pl_PRV;
	T_Liste_ZonesRegul *pl_Zones;
	XDY_Octet 			vl_DispoPRV;
	XDY_Octet 			vl_TypePRV;
	XDY_Entier 			vl_NumEvt;
	XDY_Octet 			vl_CleEvt;
	XDY_Octet 			vl_Priorite;
	XDY_Entier 			vl_DistanceEvt;
	XDY_Entier			vl_Action;
	XDY_Octet			vl_SiteAction;
	XDY_Bandeau			vl_Bandeau;
	XDY_VitesseRegulTxt	vl_VitesseCourante;
	XDY_Horodate                    vl_horodate;
        int                             vl_jourSemaine;

	XZST_03EcritureTrace ( XZSTC_FONCTION,	"IN: creer_liste_PRV : numero=%d, autoroute=%s, type=%d, va_Vitesse_nominale=%d, va_Zone=%d,va_Critique=%d,va_PR=%d,va_Sens=%d",
			va_Numero,
			va_nom_autoroute,
			va_Type,
			va_Vitesse_nominale,
			va_Zone,
			va_Critique,
			va_PR,va_Sens);

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


	/*A Recherche de la zone de regulation d'appartenance du PRV */
	pl_Zones = pg_Zones;
	while (pl_Zones!= NULL)
	{
		if (pl_Zones->zone.numero_zone == va_Zone)
			break;
		pl_Zones=pl_Zones->suivant;
	}

	if (pl_Zones == NULL)
	{
		/*trace warning*/
		XZST_03EcritureTrace ( XZSTC_WARNING, "creer_liste_PRV : le PRV %d n'appartient a aucune zone existante (%d)!", va_Numero, va_Zone);
		return XDC_NOK;
	}

	/*A Allocation memoire pour la nouvelle structure */
	if ( (pl_NewPRV = (T_Liste_PRV *) malloc ( sizeof(T_Liste_PRV))) == NULL)
	{
		/* Ecriture trace */
		XZST_03EcritureTrace ( XZSTC_WARNING, " OUT: creer_liste_PRV Allocation memoire impossible");
		return XDC_NOK;
	}

	/*A Initialise la structure avec les valeurs passees en parametre */
	pl_NewPRV->prv.numero_prv = va_Numero;
	pl_NewPRV->prv.type_prv = va_Type;
	pl_NewPRV->prv.vitesse_nominale = va_Vitesse_nominale;
	pl_NewPRV->prv.zone_prv = va_Zone;
	pl_NewPRV->prv.critique_prv = va_Critique;
/*	pl_NewPRV->prv.autoroute_prv = va_Autoroute;*/
	pl_NewPRV->prv.pr_prv = va_PR;
	pl_NewPRV->prv.sens_prv = va_Sens;
	pl_NewPRV->prv.etat_de_marche = XDC_FAUX;
	pl_NewPRV->prv.vitesse_courante = -1;
	strcpy(pl_NewPRV->prv.bandeau,"");

	/*recuperation de l'etat d'affichage des PRV a l'horodate de simulation*/
	if(XZAT51_Utilisation_PRV (0,
								va_Numero,
								&vl_DispoPRV,
								vl_VitesseCourante,
								vl_Bandeau,
								&vl_NumEvt,
								&vl_CleEvt,
								&vl_Priorite,
								&vl_DistanceEvt,
								&vl_Action,
								&vl_SiteAction) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d utiliser le PRV numero : %d", va_Numero);
	}
	else{
		if ((vl_DispoPRV & ( XDC_EQT_MAJEUR
							| XDC_EQT_CRITIQUE
							| XDC_EQT_DESACTIVE
							| XDC_EQT_HS
							| XDC_EQT_INHIBE))== 0 )
			pl_NewPRV->prv.etat_de_marche = XDC_VRAI;
		else
			XZST_03EcritureTrace ( XZSTC_INFO, "PRV %d EN PANNE", pl_NewPRV->prv.numero_prv);
	}

	if (vl_Priorite == EREG_PRIORITE_PRV_MANU){
		pl_Zones->zone.pilotage_manuel = XDC_VRAI;
	}

	/*A Ajout a la liste si existante, creation sinon */
	XZST_03EcritureTrace(XZSTC_MESSAGE, "+++++ CREATION PRV %d dans la zone %d(%s) - %d",
			pl_NewPRV->prv.numero_prv,
			pl_Zones->zone.numero_zone,
			pl_Zones->zone.nom_zone,
			pl_NewPRV->prv.etat_de_marche);

	if (pl_Zones->zone.ListePRV == NULL)
	{
		pl_Zones->zone.ListePRV = pl_NewPRV;
		pl_Zones->zone.ListePRV->suivant = NULL;
	}
	else
	{
		pl_PRV = pl_Zones->zone.ListePRV;
		while (pl_PRV->suivant != NULL)
			pl_PRV = pl_PRV->suivant;

		pl_PRV->suivant = pl_NewPRV;
		pl_NewPRV->suivant = NULL;
	}

	XZST_03EcritureTrace ( XZSTC_FONCTION,"OUT: creer_liste_PRV");
	return (XDC_OK);
}


/* Creation de la fonction uilisateur de creation de la liste des PRV*/
int creer_liste_station (XZAOT_Comptage va_Station)
{
int i;
	XZST_03EcritureTrace ( XZSTC_INTERFACE,"creer_liste_station : va_Station=%d, va_Zone=%d",
			va_Station.Station,va_Station.Zone);

	T_Liste_Station *pl_NewStation;
	T_Liste_Station *pl_Station;
	T_Liste_ZonesRegul *pl_Zones;


	/*A Allocation memoire pour la nouvelle structure */
	if ( (pl_NewStation = (T_Liste_Station *) malloc ( sizeof(T_Liste_Station)) ) == NULL)
	{
		/* Ecriture trace */
		XZST_03EcritureTrace ( XZSTC_WARNING, " OUT: creer_liste_station Allocation memoire impossible");
		return XDC_NOK;
	}

	/*A Initialise la structure avec les valeurs passees en parametre */
	pl_NewStation->station.station = va_Station.Station;
	pl_NewStation->station.zone_station = va_Station.Zone;
	pl_NewStation->station.sens = va_Station.Sens;
	pl_NewStation->station.positionDerniereStationRecue = -1;
	pl_NewStation->station.positionDerniereStationRecuePente = -1;
	pl_NewStation->station.horodateDerniereStationRecue = -1;
	pl_NewStation->station.data_lisseeQ = 0;
	pl_NewStation->station.data_lisseeQ_valide = XDC_FAUX;
	pl_NewStation->station.data_lisseeTO = 0;
	pl_NewStation->station.data_lisseeTO_valide = XDC_FAUX;
	pl_NewStation->station.pente = 0;
	pl_NewStation->station.QL1 = -1;
	pl_NewStation->station.QL2 = -1;
	pl_NewStation->station.QL3 = -1;
	pl_NewStation->station.QL4 = -1;
	pl_NewStation->station.pente_valide = XDC_FAUX;

	for (i=0; i<EREG_NB_DATA_STATION_MAX; i++){
		pl_NewStation->station.data[i].Q = 0;
		pl_NewStation->station.data[i].TO = 0;
		pl_NewStation->station.data[i].Q_valide = XDC_FAUX;
		pl_NewStation->station.data[i].TO_valide = XDC_FAUX;
	}
	for (i=0; i<EREG_NB_DATA_STATION_CYCLE_PENTE; i++){
		pl_NewStation->station.data_pente[i].Q = 0;
		pl_NewStation->station.data_pente[i].TO = 0;
		pl_NewStation->station.data_pente[i].Q_valide = XDC_FAUX;
		pl_NewStation->station.data_pente[i].TO_valide = XDC_FAUX;
	}
	/*A Recherche de la zone de regulation d'appartenance de la Station */
	pl_Zones = pg_Zones;
	while (pl_Zones!= NULL)
	{
		if (pl_Zones->zone.numero_zone == pl_NewStation->station.zone_station)
			break;
		pl_Zones=pl_Zones->suivant;
	}

	if (pl_Zones == NULL)
	{
		/*trace warning*/
		XZST_03EcritureTrace ( XZSTC_WARNING, "Erreur, la station %d n'appartient a aucune zone !", va_Station.Station);
		return XDC_NOK;
	}

		/*A Ajout a la liste si existante, creation sinon */
		XZST_03EcritureTrace(XZSTC_FONCTION, "++++ CREATION STATION id: %d dans la zone : %d ", pl_NewStation->station.station, pl_Zones->zone.numero_zone);
		pl_NewStation->suivant = pl_Zones->zone.ListeStation;
		pl_Zones->zone.ListeStation = pl_NewStation;

	XZST_03EcritureTrace ( XZSTC_FONCTION,"OUT: creer_liste_station");
	return (XDC_OK);
}


int	config_params ( XZAOT_Terdt  va_terdt,
					 int		 va_NumAppel )
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR :
*   Aucun
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*   Ecrire les parametres dans un tableau
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int			vl_ValRet = XDC_OK;

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : config_params");

   /* Remplissage du tableau */

   tg_params[va_terdt.Numero].Numero = va_terdt.Numero;
   tg_params[va_terdt.Numero].Type =   va_terdt.Type;
   strcpy (tg_params[va_terdt.Numero].Nom,	va_terdt.Nom);
   tg_params[va_terdt.Numero].Valeur = va_terdt.Valeur;

   XZST_03EcritureTrace ( XZSTC_INFO, "Config REG PARAMS : %d %d %s %d",
												   tg_params[va_terdt.Numero].Numero,
												   tg_params[va_terdt.Numero].Type,
												   tg_params[va_terdt.Numero].Nom,
												   tg_params[va_terdt.Numero].Valeur );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : config_params" );
   return ( vl_ValRet );

}


