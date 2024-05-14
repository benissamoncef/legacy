/*E*/
/* Fichier : $Id: ereg_valide.c,v 1.5 2020/12/02 10:55:10 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2020/12/02 10:55:10 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER ereg_valide.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de general de valide de la regulation >
*
------------------------------------------------------
* HISTORIQUE :
*
* SDU		13/03/2012 : Creation DEM/1016
* JMG		11/07/12 : alerte ARRET sur panne PRV critique 1.2
* GR            09/07/2020 : EMS 1.3
* PNI		02/12/2020 : horadate a null pour appel XZAT51 1.4
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */
#include "ereg_valide.h"
#include "ereg_cfg.h"
#include "ereg_util.h"
#include "xzae567.h"
#include "xzaac.h"
#include "xzae560.h"
#include "xzit.h"

/* definitions de constantes */


#define VALID_NIV_TRACE_1	XZSTC_INFO
#define VALID_NIV_TRACE_2 	XZSTC_DEBUG1

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
int	vg_InitValide = XDC_NOK;

/* declaration de variables globales */
XDY_Datagroup		vg_NomDG;
extern T_Param 		tg_params[EREG_NB_PARAM_MAX];
/* declaration de variables externes */
extern XDY_Basedd			vg_Base ;
extern T_Liste_ZonesRegul* 	pg_Zones;
extern XDY_Booleen 			vg_ModeSimu;
extern XDY_Entier  			vg_ModeRegul;
extern XDY_NomSite 			vg_NomSite;
extern XDY_District			vg_NumeroSite;
extern XDY_Booleen		  vg_tebeg;
extern XDY_Horodate		 	vg_HorodateSimu;

/* declaration de fonctions internes */
void valide_sollicite(T_ZONE_REGUL *);
void valide_declenchement(T_ZONE_REGUL *);
void valide_ajustement(T_ZONE_REGUL *);
void valide_arret(T_ZONE_REGUL *);
void valide_change_contexte(T_ZONE_REGUL *);
void valide_acquittement_cb (T_IPC_CONN,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);

void	valide_traite_fmc_cb (T_IPC_CONN 	va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
		T_CB_ARG                        pa_ARG);

int creerFMCRegulation (T_ZONE_REGUL *pa_zone, T_Contexte va_contexte, XDY_Entier);

XDY_Entier acquitter_Alerte(T_ZONE_REGUL *pa_zone,
		XDY_TexteAlerte pa_texte_alerte,
		XDY_Entier va_type_validation, XDY_Entier);

int genererAlerte (T_ZONE_REGUL *pa_zone,
						XDY_Octet va_sous_type_alerte,
						XDY_Entier va_type);

int genererReveil (T_ZONE_REGUL *pa_zone,
					char *va_transit);

XDY_Entier checkPilotageManuel (T_ZONE_REGUL *pa_zone, XDY_Booleen va_avecGenerationAlerte);



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
*
------------------------------------------------------*/
int valide_init(){
	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : valide_init ");
	T_Liste_ZonesRegul *pl_zones=NULL;
	XDY_Entier	vl_numero;
	XDY_Octet	vl_cle;
	XDY_Entier vl_contexte, vl_scenario, vl_mesure, vl_indicateur_horaire;
	XDY_Entier vl_scenario_cible;
	XDY_Entier vl_zone;
	char vl_transitoire[201];
	int vl_JourSemaine;
	XDY_Horodate vl_horo_courante;
	T_Liste_Scenario 	*pl_listeScenario = NULL;
	XDY_Horodate vl_mon_horo;

	if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
	                        (int *) &vl_JourSemaine, (double *) &vl_horo_courante ) != XDC_OK )
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la lecture de l'heure %d en Sec", vl_horo_courante );
		return XDC_NOK;
	}

	/*En mode temps reel*/
	if (vg_ModeSimu == XDC_FAUX)
	{
		vl_mon_horo = vl_horo_courante;
		/*A  Armement Callback sur Reception XDM_ICREER_ALERTE */
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ACQUITTE_REGUL),
					valide_acquittement_cb,(T_CB_ARG)NULL) == NULL)
		{
			   XZST_03EcritureTrace( XZSTC_WARNING, "Callback sur XDM_ICREER_ALERTE non declenchee.");
		}

		/*A  Armement Callback sur Reception XDM_EREG_FMC */
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EREG_FMC),
						valide_traite_fmc_cb,(T_CB_ARG)NULL) == NULL)
		{
			   XZST_03EcritureTrace( XZSTC_WARNING, "Callback sur XDM_EREG_FMC non declenchee.");
		}

		/*A Abonnement aux Datagroup XDG_EREG_FMC*/
		XDG_EncodeDG2 ( vg_NomDG, XDG_EREG_FMC, vg_NomSite) ;
	   	/*joint ce datagroup*/
	   	if ( !XDG_JoindreDG( vg_NomDG ) )
	   	{
	      	   XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", vg_NomDG );
	      	   return XDC_NOK;
		}

	}
	else
	{
		vl_mon_horo = vg_HorodateSimu;
		XZST_03EcritureTrace( VALID_NIV_TRACE_2, "Mode Simulation");
	}


	/*recuperation d une fmc regulation en cours*/
	pl_zones = pg_Zones;
	while (pl_zones != NULL) {
		vl_numero = -1;
		if (XZAE567_Get_FMC_Regulation(vl_mon_horo, pl_zones->zone.numero_zone,
					&vl_numero,&vl_cle, &vl_contexte,&vl_scenario,&vl_scenario_cible, &vl_mesure,
					&vl_indicateur_horaire,&vl_zone,vl_transitoire) != XDC_OK) {
			XZST_03EcritureTrace(XZSTC_WARNING,"pb appel a XZAE567_Get_FMC_Regulation");
		}else {
			if (vl_numero != -1) {
				pl_zones->zone.fmc_regulation.numero = vl_numero;
				pl_zones->zone.fmc_regulation.cle = vl_cle;
				pl_zones->zone.regulation_en_cours = XDC_VRAI;
				pl_zones->zone.regulation.mesure = vl_mesure;
				pl_zones->zone.regulation.indicateur_horaire = vl_indicateur_horaire;
				pl_zones->zone.regulation.contexte.checksum = vl_contexte;
//				pl_zones->zone.regulation.scenario.id = vl_scenario;

				pl_listeScenario = pl_zones->zone.ListeScenario;
				if (pl_listeScenario != NULL) {
							while(pl_listeScenario->suivant != NULL){
								if (pl_listeScenario->scenario.id == vl_scenario)  {
									pl_zones->zone.regulation.scenario = pl_listeScenario->scenario;
									break;
								}
								pl_listeScenario = pl_listeScenario->suivant;
							}
				}

				pl_zones->zone.regulation.horodate_regulation = 0;

				if (pl_zones->zone.regulation.mesure == EREG_MESURE_FORCAGE) {
					pl_zones->zone.indicateur_forcage = XDC_VRAI;
				}
			}
		//	else{
				/* Ajout Proposition ARRET REGUL pour init correcte du systeme*/
	//			declencherArret(&(pl_zones->zone));
		//		updateRegulationProposee(&(pl_zones->zone), pl_zones->zone.regulation_proposee);
	//		}
		}
		pl_zones=pl_zones->suivant;
	}

	vg_InitValide = XDC_OK;

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : valide_init");
	return XDC_OK;
}



int valide_scenario(T_ZONE_REGUL *pa_zone)
{
XDY_Horodate 	vl_horo_courante;
XDY_Entier 		vl_JourSemaine;
XDY_Booleen		vl_AppliquerChangement = XDC_FAUX;
XDY_Entier      vl_numero;
XDY_Octet       vl_cle;
XDY_Entier vl_contexte, vl_scenario, vl_mesure, vl_indicateur_horaire;
XDY_Entier vl_scenario_cible;
XDY_Entier vl_zone;
char vl_transitoire[201];

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN valide_scenario : zone=%d(%s)",
				pa_zone->numero_zone,
				pa_zone->nom_zone);

	XZST_03EcritureTrace( VALID_NIV_TRACE_1, "#### VALIDATION SCENARIO : Zone:%d Simu=%s Mode=%s ####",
							pa_zone->numero_zone,
							vg_ModeSimu==XDC_VRAI?"VRAI":"FAUX",
							vg_ModeRegul==XDC_REG_MODE_MANUEL?"MANU":vg_ModeRegul==XDC_REG_MODE_AUTO?"AUTO":"SEMI-AUTO");

	if (vg_InitValide == XDC_NOK){
		XZST_03EcritureTrace( XZSTC_FATAL, "valide_scenario  : INITIALISATION NON FAITE");
		exit(0);
	}

	if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
	                        (int *) &vl_JourSemaine, (double *) &vl_horo_courante ) != XDC_OK )
	{
		XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la lecture de l'heure %d en Sec", vl_horo_courante );
		return XDC_NOK;
	}


	if (pa_zone->degrade &&  pa_zone->regulation_proposee.mesure == EREG_MESURE_ARRET) 
	{
		XZST_03EcritureTrace( VALID_NIV_TRACE_2, "     ..DEGRADE => Arret Demande et Retour au Scenario Nominal : valide_arret()");
		if (pa_zone->regulation_proposee.contexte.panne_PRV_critique) {
			if (pa_zone->indicateur_forcage == XDC_VRAI)
				genererAlerte(pa_zone, EREG_VALIDE_SOUS_TYPE_ALERTE_TERMINER_FORCAGE, XZAVC_REG_TRACE_VALIDATION);
			else
				genererAlerte(pa_zone, EREG_VALIDE_SOUS_TYPE_ALERTE_ARRET,XZAVC_REG_TRACE_VALIDATION);
		}
		valide_arret(pa_zone);
		return (XDC_OK);
	}

	/*Mode temps reel*/

	if(pa_zone->indicateur_forcage == XDC_VRAI &&
		pa_zone->regulation.scenario.id != pa_zone->scenarioNominal->id &&
			scenario_has_prv_critique_en_panne(&(pa_zone->regulation.scenario)))
	{
		XZST_03EcritureTrace( VALID_NIV_TRACE_2, "     ..scenario force : prv non critique en panne");
		if (pa_zone->regulation_proposee.contexte.panne_PRV_non_critique == XDC_FAUX){
			pa_zone->regulation_proposee.contexte.panne_PRV_non_critique = XDC_VRAI;
			pa_zone->regulation_proposee.contexte.scenario_ideal = XDC_FAUX;
			genererAlerte(pa_zone, EREG_VALIDE_SOUS_TYPE_ALERTE_TERMINER_FORCAGE, XZAVC_REG_TRACE_VALIDATION);
		} else
			XZST_03EcritureTrace( VALID_NIV_TRACE_2, "     ..scenario force : prv critique en panne. ALERTE DEJA LEVEE");
	}


	//if (vg_ModeSimu == XDC_FAUX)
//	{
		XZST_03EcritureTrace( VALID_NIV_TRACE_2, "     ..Mode temps reel");

		/*Si un forcage operateur est en cours*/
		if (pa_zone->indicateur_forcage == XDC_VRAI)
		{
			/*on re verifie au cas ou*/
			if (XZAE567_Get_FMC_Regulation(vg_HorodateSimu, pa_zone->numero_zone,
					&vl_numero,&vl_cle, &vl_contexte,&vl_scenario,&vl_scenario_cible, &vl_mesure,
					&vl_indicateur_horaire,&vl_zone,vl_transitoire) == XDC_OK) {
				if (vl_numero == -1) {
					pa_zone->indicateur_forcage = XDC_FAUX;
					pa_zone->horodate_forcage = vl_horo_courante;
				}
			}
			XZST_03EcritureTrace(VALID_NIV_TRACE_1, "     ..Forcage en cours %lf %lf",
					pa_zone->horodate_forcage, vl_horo_courante);
			vl_AppliquerChangement = XDC_FAUX;

			if(pa_zone->horodate_forcage + getParamValue(EREG_PARAM_VAL_REG_DUREE_APP_SCEN_FORCE)*60  < vl_horo_courante )
			{
				XZST_03EcritureTrace( VALID_NIV_TRACE_1, "     ..Forcage depuis plus de %d minutes",
							getParamValue(EREG_PARAM_VAL_REG_DUREE_APP_SCEN_FORCE));
				genererAlerte(pa_zone, EREG_VALIDE_SOUS_TYPE_ALERTE_TERMINER_FORCAGE, XZAVC_REG_TRACE_VALIDATION);
			}
			else
				XZST_03EcritureTrace( VALID_NIV_TRACE_1, "Forcage depuis moins de '%d' minutes : Pas de traitement de validation du scenario",
						getParamValue(EREG_PARAM_VAL_REG_DUREE_APP_SCEN_FORCE));

		}
//	}

	XZST_03EcritureTrace( VALID_NIV_TRACE_2, "     ..tous Modes");




	if (pa_zone->indicateur_forcage == XDC_FAUX)
	{
		XZST_03EcritureTrace(VALID_NIV_TRACE_2, "     ..Pas de Forcage en cours");
		XZST_03EcritureTrace( VALID_NIV_TRACE_1,"pa_zone->regulation.scenario.id %d",pa_zone->regulation.scenario.id);
		XZST_03EcritureTrace( VALID_NIV_TRACE_1,"pa_zone->regulation_proposee.scenario.id %d",	pa_zone->regulation_proposee.scenario.id);
		XZST_03EcritureTrace( XZSTC_DEBUG1,"contexte %d",isNewContext(pa_zone));
		XZST_03EcritureTrace( XZSTC_DEBUG1,"courant %lf, regul %lf",	vl_horo_courante,pa_zone->regulation.horodate_regulation);
		XZST_03EcritureTrace( XZSTC_DEBUG1,"delta %lf",(float)getParamValue(XDC_REG_DELAI_LAT_DETEC_EVOL)*60);
		if( ( (pa_zone->regulation.scenario.id != pa_zone->regulation_proposee.scenario.id) &&
			  ( (vl_horo_courante-pa_zone->regulation.horodate_regulation) >= (float)(getParamValue(XDC_REG_DELAI_LAT_DETEC_EVOL)*60) )
			)
		    ||
			isNewContext(pa_zone))
		{
			XZST_03EcritureTrace( VALID_NIV_TRACE_1, "     ..scenario actif terrain different de scenario propose ou nouveau contexte" );
			vl_AppliquerChangement = XDC_VRAI;
		}
		else
		{
			if (pa_zone->regulation.scenario.id == pa_zone->regulation_proposee.scenario.id)
			{
				XZST_03EcritureTrace( VALID_NIV_TRACE_1, "     ..scenario identique");
			}
			else
			{
				XZST_03EcritureTrace( VALID_NIV_TRACE_1, "     ..scenario different mais delai XDC_REG_DELAI_LAT_DETEC_EVOL (%dmn) non atteint",getParamValue(XDC_REG_DELAI_LAT_DETEC_EVOL));
			}
		}
	}
	if (vl_AppliquerChangement == XDC_VRAI){
		valide_sollicite(pa_zone);
	}else


	XZST_03EcritureTrace( XZSTC_DEBUG1, "######## FIN VALIDATION ########");

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : valide_scenario");
}


void valide_sollicite(T_ZONE_REGUL *pa_zone)
{
	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : valide_sollicite ");

	if (vg_InitValide == XDC_NOK){
		XZST_03EcritureTrace( XZSTC_FATAL, "valide_sollicite  : INITIALISATION NON FAITE");
		exit(0);
	}

	XZST_03EcritureTrace( XZSTC_DEBUG1, "#### VALIDATION SOLLICITATION ####");

	/* Traitement premier lancement */
	/* Si il n'y a pas de scenario en cours dans la regulation */
	if ( (isRegulationEnCours(pa_zone) == XDC_FAUX) && (pa_zone->regulation_proposee.scenario.nominal == XDC_VRAI)) {
		XZST_03EcritureTrace( VALID_NIV_TRACE_1, "     ..regulation non demarree et scenario nominal");
		pa_zone->regulation_proposee.mesure = EREG_MESURE_ARRET;
		valide_arret(pa_zone);
	}
	else if (isRegulationEnCours(pa_zone) == XDC_FAUX){
		XZST_03EcritureTrace( VALID_NIV_TRACE_1, "     ..Premier lancement : valide_declenchement()");
		valide_declenchement(pa_zone);
	}else
	{
		if (pa_zone->regulation_proposee.scenario.nominal) {
			XZST_03EcritureTrace( VALID_NIV_TRACE_1,"proposition du scenario nominal => on force a EREG_MESURE_ARRET");
			pa_zone->regulation_proposee.mesure = EREG_MESURE_ARRET;
		}

		/*Si arret de la regulation*/
		if(pa_zone->regulation_proposee.mesure == EREG_MESURE_ARRET
				&& pa_zone->regulation_proposee.scenario.nominal == XDC_VRAI)
		{
			XZST_03EcritureTrace( VALID_NIV_TRACE_2, "     ..Arret Demande et Retour au Scenario Nominal : valide_arret()");
			valide_arret(pa_zone);
		}
		/*Si le scenario n est pas nominal*/
		/*Si il n existe pas de FMC en cours sur la zone*/
		else if (pa_zone->regulation_proposee.scenario.nominal != XDC_VRAI && isRegulationEnCours(pa_zone) == XDC_FAUX)
		{
			XZST_03EcritureTrace( VALID_NIV_TRACE_2, "     ..le scenario n'est pas nominal et pas de FMC en cours : valide_declenchement()");
			valide_declenchement(pa_zone);
		}
		/* Si scenario autre mais meme contexte*/
		else if (pa_zone->regulation_proposee.scenario.id != pa_zone->regulation.scenario.id &&
					isNewContext(pa_zone) == XDC_FAUX)
		{
			XZST_03EcritureTrace( VALID_NIV_TRACE_2, "     ..Autre scenario dans la meme contexte : valide_ajustement()");
			valide_ajustement(pa_zone);
		}
		/*Si meme scenario mais contexte different*/
		else if (pa_zone->regulation_proposee.scenario.id != pa_zone->regulation.scenario.id)
		{
			XZST_03EcritureTrace( VALID_NIV_TRACE_2, "     ..Meme scenario mais dans un autre contexte (sans controle : valide_change_contexte())");
			valide_change_contexte(pa_zone);
		}
	}
	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : valide_sollicite");
}


void valide_declenchement(T_ZONE_REGUL *pa_zone)
{
	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : valide_declenchement ");

	if (vg_InitValide == XDC_NOK){
		XZST_03EcritureTrace( XZSTC_FATAL, "valide_declenchement  : INITIALISATION NON FAITE");
		exit(0);
	}

	XZST_03EcritureTrace( XZSTC_DEBUG1, "#### VALIDATION DECLENCHEMENT ####");
	pa_zone->regulation_proposee.mesure = EREG_MESURE_DECLENCHEMENT;

	checkPilotageManuel(pa_zone, XDC_VRAI);
	if (pa_zone->pilotage_manuel == XDC_VRAI){
		XZST_03EcritureTrace( XZSTC_WARNING, "PRV en pilotage manuel dans la zone de regulation : Operation Abandonnee");
	}else
	{
		/* Mode manuel ou Semi Auto*/
		if ( (vg_ModeRegul == XDC_REG_MODE_MANUEL) || (vg_ModeRegul == XDC_REG_MODE_SEMIAUTO) ||
		     ( (vg_ModeRegul == XDC_REG_MODE_AUTO) && (pa_zone->regulation_proposee.contexte.evt_impactant == XDC_VRAI) 
			 && (pa_zone->regulation_proposee.contexte.qualification_operateur==XDC_VRAI)))
		{
			/* Alerte sur la zone*/
			if (genererAlerte(pa_zone, EREG_VALIDE_SOUS_TYPE_ALERTE_DECLENCHEMENT, XZAVC_REG_TRACE_VALIDATION) == XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "valide_declenchement : Impossible de generer une alerte");
			}

		}
		/* Mode Auto*/
		else if (vg_ModeRegul == XDC_REG_MODE_AUTO)
		{
			/* Creation d une FMC*/
			if (creerFMCRegulation(pa_zone, pa_zone->regulation_proposee.contexte,0) == XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "valide_declenchement : Creation FMC impossible ");
			}

			/*Proposition au module d application*/
			appli_analyse(pa_zone);

			/*Positionnement d un reveil sur la FMC*/
			//todo mettre les bonnes valeurs
			/*if (genererReveil(pa_zone, va_type, va_texte) == XDC_NOK) */
			if (genererReveil(pa_zone,"") == XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "valide_declenchement : Positionnement du reveil sur la FMC impossible ");
			}

		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "Aucun mode %d trouve", vg_ModeRegul);
		}

	}

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : valide_declenchement");
}

void valide_ajustement(T_ZONE_REGUL *pa_zone)
{
	if (vg_InitValide == XDC_NOK){
		XZST_03EcritureTrace( XZSTC_FATAL, "valide_ajustement  : INITIALISATION NON FAITE");
		exit(0);
	}

	XZST_03EcritureTrace( XZSTC_DEBUG1, "#### VALIDATION AJUSTEMENT ####");

	/*si ajustement car evt impactant mais sans mise a jour */
	/*alors ne pas solliciter l operateur pour rien*/
	if (    (pa_zone->regulation_proposee.contexte.evt_impactant == XDC_VRAI) &&
			(pa_zone->regulation_proposee.contexte.evt_mis_a_jour == XDC_FAUX)
	   ){
		XZST_03EcritureTrace(XZSTC_INFO,"evt impactant sans mise a jour => pas besoin d alerte");
		return ;
	}

	checkPilotageManuel(pa_zone, XDC_VRAI);
	if (pa_zone->pilotage_manuel == XDC_VRAI){
		XZST_03EcritureTrace( XZSTC_WARNING, "PRV en pilotage manuel dans la zone de regulation : Operation Abandonnee");
	}else
	{
		/* Mode manuel*/
		if (vg_ModeRegul == XDC_REG_MODE_MANUEL)
		{
			/* Alerte sur la zone*/
			if (genererAlerte(pa_zone, EREG_VALIDE_SOUS_TYPE_ALERTE_AJUSTEMENT, XZAVC_REG_TRACE_VALIDATION) == XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "valide_ajustement : Impossible de generer une alerte");
			}

		}
		else if ((vg_ModeRegul == XDC_REG_MODE_AUTO) || (vg_ModeRegul == XDC_REG_MODE_SEMIAUTO))
		{
			/*Positionnement d un reveil sur la FMC*/
		/*	if (genererReveil(pa_zone,XZAVC_REG_TRACE_VALIDATION,"") == XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "valide_ajustement : Positionnement du reveil sur la FMC impossible");
			}*/

			/*Proposition au module d application*/
			if (appli_analyse(pa_zone) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "valide_ajustement :Proposition au module d application impossible");
			}

			/*mettre a jour la FMC avec la nouvelle regulation*/
			updateFmcRegulation(pa_zone, TRUE);
		}
		else
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "Aucun mode %d trouve", vg_ModeRegul);
		}
	}

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : valide_ajustement");
}

void valide_arret(T_ZONE_REGUL *pa_zone)
{
	XZST_03EcritureTrace( XZSTC_INFO, "IN : valide_arret ");

	if (vg_InitValide == XDC_NOK){
		XZST_03EcritureTrace( XZSTC_FATAL, "valide_arret  : INITIALISATION NON FAITE");
		exit(0);
	}

	XZST_03EcritureTrace( XZSTC_DEBUG1, "#### VALIDATION ARRET ####");

	if (pa_zone->regulation_proposee.scenario.id != pa_zone->scenarioNominal->id)
		XZST_03EcritureTrace(XZSTC_WARNING, "ATTENTION : Arret demande, mais scenario propose n'est pas le NOMINAL");
	else
	{
		/*Si une Validation operateur n est pas requise*/
		if ((pa_zone->regulation_proposee.contexte.qualification_operateur == XDC_FAUX) &&
				(pa_zone->regulation_proposee.contexte.evt_impactant == XDC_VRAI))
		{
			XZST_03EcritureTrace(VALID_NIV_TRACE_2, "     ..Validation operateur non requise");

			updateFmcRegulation(pa_zone, FALSE);
			/*Termine la FMC*/
			if (terminerFmcRegulation(pa_zone, TRUE) == XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "valide_arret :terminer la FMC impossible");
			}

			/*Proposition au module d application*/
			if (appli_analyse(pa_zone) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "valide_arret :Proposer le scenario a appliquer impossible");
			}
		}
		else
		{
			XZST_03EcritureTrace(VALID_NIV_TRACE_2, "     ..Validation Operateur requise");

			/* Mode manuel ou Mode semiauto*/
			if ((vg_ModeRegul == XDC_REG_MODE_MANUEL) || (vg_ModeRegul == XDC_REG_MODE_SEMIAUTO))
			{
				/* Alerte sur la zone si regulation demarree*/
				if ( pa_zone->fmc_regulation.numero > 0) {
					if (genererAlerte(pa_zone, EREG_VALIDE_SOUS_TYPE_ALERTE_ARRET, XZAVC_REG_TRACE_VALIDATION) == XDC_NOK)
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "valide_arret :Alerte sur la zone impossible");
					}
				}
				else {
					/*Proposition au module d application*/
					if (appli_analyse(pa_zone) != XDC_OK)
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "valide_arret :Proposer le scenario a appliquer impossible");
					}
				}
			}
			/* Mode Auto*/
			else if (vg_ModeRegul == XDC_REG_MODE_AUTO)
			{

				/*Proposition au module d application*/
				if (appli_analyse(pa_zone) != XDC_OK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "valide_arret :Proposer le scenario a appliquer impossible");
				}
				/*Termine la FMC*/
				if (terminerFmcRegulation(pa_zone, TRUE) == XDC_NOK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "valide_arret :Terminer la FMC impossible");
				}

			}
			else
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "Aucun mode %d trouve", vg_ModeRegul);
			}
		}
	}

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : valide_arret");
}

void valide_change_contexte(T_ZONE_REGUL *pa_zone)
{
	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : valide_change_contexte ");

	if (vg_InitValide == XDC_NOK){
		XZST_03EcritureTrace( XZSTC_FATAL, "valide_change_contexte  : INITIALISATION NON FAITE");
		exit(0);
	}

	XZST_03EcritureTrace( VALID_NIV_TRACE_1, "#### VALIDATION CHANGEMENT CONTEXTE ####");

	checkPilotageManuel(pa_zone, XDC_VRAI);
	if (pa_zone->pilotage_manuel == XDC_VRAI){
		XZST_03EcritureTrace( XZSTC_WARNING, "PRV en pilotage manuel dans la zone de regulation : Operation Abandonnee");
	}else
	{
		/* Alerte sur la zone*/
		if (genererAlerte(pa_zone, EREG_VALIDE_SOUS_TYPE_ALERTE_CHANGEMENT_CONTEXTE, XZAVC_REG_TRACE_VALIDATION) == XDC_NOK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "valide_change_contexte :Alerte sur la zone impossible");
		}

	}
	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : valide_change_contexte");
}



XDY_Entier acquitter_Alerte(T_ZONE_REGUL *pa_zone,
							XDY_TexteAlerte pa_texte_alerte,
							XDY_Entier va_type_validation,
							XDY_Entier va_num_alerte){

XDY_Booleen va_alerte_validee;
T_Liste_Generique *pl_listeGenerique;
T_Liste_Generique *pl_listeGenerique2;
T_Liste_Generique *pl_a_supprimer;
T_REGULATION *pl_regulation;

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : acquitter_Alerte ");

	va_alerte_validee = (va_type_validation == XDC_ALT_VRAIE || va_type_validation == XDC_ALT_VRAIE_EVT);

	XZST_03EcritureTrace( VALID_NIV_TRACE_1, "///// Acquitement DE l'ALERTE : %s /////",
			va_alerte_validee==XDC_VRAI?"POSITIF":"NEGATIF");

	/* On Recherche l'alerte qui a ete acquitee */
	pl_listeGenerique = pa_zone->ListeAlertes;
	while (pl_listeGenerique != NULL){
		if ( !strcmp(((T_REGULATION *)pl_listeGenerique->element)->alerte_envoyee, pa_texte_alerte) )
			break;
		Liste_Generique_Suivant(pl_listeGenerique);
	}

	if (pl_listeGenerique == NULL){
		XZST_03EcritureTrace( XZSTC_WARNING, "acquitter_Alerte : L'alerte \"%s\" n'existe pas", pa_texte_alerte);
		return XDC_NOK;
	}

	pl_regulation = (T_REGULATION *)pl_listeGenerique->element;

	XZST_03EcritureTrace( VALID_NIV_TRACE_1, "      ..Traitement Acquitement %s pour Alerte Type %d",
			va_alerte_validee==XDC_VRAI?"POSITIF":"NEGATIF",
					pl_regulation->sous_type_alerte_envoyee);

	if ((pl_regulation->sous_type_alerte_envoyee==EREG_VALIDE_SOUS_TYPE_ALERTE_ARRET) &&
			(va_alerte_validee==XDC_FAUX) &&
			(pl_regulation->contexte.evt_impactant == XDC_VRAI) &&
			(pl_regulation->contexte.qualification_operateur == XDC_VRAI)) {
		XZST_03EcritureTrace( XZSTC_INFO,"on poursuit tel quel");
	} else {
	switch (pl_regulation->sous_type_alerte_envoyee) {
		case EREG_VALIDE_SOUS_TYPE_ALERTE_DECLENCHEMENT:
			if (va_alerte_validee == XDC_VRAI){
				checkPilotageManuel(pa_zone, XDC_VRAI);
				if (pa_zone->pilotage_manuel == XDC_VRAI){
					XZST_03EcritureTrace( XZSTC_WARNING, "PRV en pilotage manuel dans la zone de regulation : Operation Abandonnee");
				}else
				{
					//creation FMC regulation(contexte + scenario)
					creerFMCRegulation(pa_zone, pa_zone->regulation_proposee.contexte, va_num_alerte);
					/*envoie du scenario au module d application*/
					//appli_analyse(pa_zone);
				}
			}else
			{
				/*declenchement du forcage de la zone*/
				pa_zone->regulation_proposee.scenario = *(pa_zone->scenarioNominal);
				declencherForcage(pa_zone);

				//creation FMC regulation(forcage et scenario nominal)
				if (creerFMCRegulation(pa_zone, pa_zone->regulation_proposee.contexte, va_num_alerte) != XDC_OK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "acquitter_Alerte : appel a creerFMCRegulation renvoie une erreur");
					appli_analyse(pa_zone);
				}else
				{
					/*envoie du scenario nominal au module d application*/
					appli_analyse(pa_zone);
				}
			}
			break;

		case EREG_VALIDE_SOUS_TYPE_ALERTE_TERMINER_FORCAGE:
			if (va_alerte_validee == XDC_VRAI){
				/*annulation du forcage de la zone*/
				annulerForcage(pa_zone);

				/*envoie du scenario au module d application*/
				appli_analyse(pa_zone);

				if (pa_zone->regulation_proposee.scenario.id == pa_zone->scenarioNominal->id) {
				if (terminerFmcRegulation(pa_zone, TRUE) != XDC_OK)
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "acquitter_Alerte : appel a terminerFmcRegulation renvoie une erreur");
					}
				} else {
					/*Mise a jour de la FMC regulation en cours(contexte + scenario de l alerte)*/
					if (updateFmcRegulation(pa_zone, TRUE) != XDC_OK)
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "acquitter_Alerte : appel a updateFmcRegulation renvoie une erreur");
					}
				}

			}else
				reinitTempoForcage(pa_zone);

			break;

		case EREG_VALIDE_SOUS_TYPE_ALERTE_AJUSTEMENT:
		case EREG_VALIDE_SOUS_TYPE_ALERTE_CHANGEMENT_CONTEXTE:
			if (pl_regulation->sous_type_alerte_envoyee==EREG_VALIDE_SOUS_TYPE_ALERTE_AJUSTEMENT)
				pa_zone->regulation_proposee.mesure = EREG_MESURE_AJUSTEMENT;
			if (va_alerte_validee == XDC_VRAI){
				checkPilotageManuel(pa_zone, XDC_VRAI);
				if (pa_zone->pilotage_manuel == XDC_VRAI){
					XZST_03EcritureTrace( XZSTC_WARNING, "PRV en pilotage manuel dans la zone de regulation : Operation Abandonnee");
				}else
				{
					if (updateFmcRegulation(pa_zone, TRUE) != XDC_OK)
					{
						XZST_03EcritureTrace(XZSTC_WARNING, "acquitter_Alerte : appel a updateFmcRegulation renvoie une erreur");
					}else
					{
						/*envoie du scenario au module d application*/
						appli_analyse(pa_zone);
					}
				}
			}else
			{
				/*Declenchement du forcage sur la zone*/
				pa_zone->regulation_proposee.scenario = *(pa_zone->scenarioNominal);
				declencherForcage(pa_zone);

				if (updateFmcRegulation(pa_zone, TRUE) != XDC_OK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "acquitter_Alerte : appel a updateFmcRegulation renvoie une erreur");
				}else
				{
					/*envoie du scenario au module d application*/
					appli_analyse(pa_zone);
				}
			}
			break;

		case EREG_VALIDE_SOUS_TYPE_ALERTE_ARRET:
			if (va_alerte_validee == XDC_VRAI){

				pa_zone->regulation.mesure = EREG_MESURE_ARRET;
				appli_analyse(pa_zone);

				if (updateFmcRegulation(pa_zone, FALSE) != XDC_OK)
					XZST_03EcritureTrace(XZSTC_WARNING, "acquitter_Alerte : appel a updateFmcRegulation renvoie une erreur");

				/*Fin de la FMC regulation*/
				if (terminerFmcRegulation(pa_zone, TRUE) != XDC_OK)
					XZST_03EcritureTrace(XZSTC_WARNING, "acquitter_Alerte : appel a terminerFmcRegulation renvoie une erreur");
			}else
			{
				/*Declenchement du forcage sur la zone*/
				/* JMG 12/12/13 : on ne force plus le scenario nominal si arret refuse par l operateur*/
				/* on gere un forcage du scenario courant*/
				/*pa_zone->regulation_proposee.scenario = *(pa_zone->scenarioNominal);*/
				declencherForcage(pa_zone);

				if (updateFmcRegulation(pa_zone, TRUE) != XDC_OK)
				{
					XZST_03EcritureTrace(XZSTC_WARNING, "acquitter_Alerte : appel a updateFmcRegulation renvoie une erreur");
				}else
				{
					/*envoie du scenario au module d application*/
					/* PNI 08/04/2013 : plus d'analyse, application du process */
					/*appli_analyse(pa_zone);*/
					;
				}
			}
			break;

		default:
			XZST_03EcritureTrace(XZSTC_WARNING, " valide_acquittement_cb: Sous Type Alerte %d INCONNU", pl_regulation->sous_type_alerte_envoyee);
			break;
	}
	}

	/* On Recherche l'alerte qui a ete acquitee */
	pl_listeGenerique = pa_zone->ListeAlertes;
	while (pl_listeGenerique != NULL){
		if ( !strcmp(((T_REGULATION *)pl_listeGenerique->element)->alerte_envoyee, pa_texte_alerte) ) {
			pl_a_supprimer = pl_listeGenerique;

			/*suppression en tete*/
			if (pl_a_supprimer == pa_zone->ListeAlertes) {
				pa_zone->ListeAlertes = pa_zone->ListeAlertes->suivant;
				free(pl_a_supprimer);
				return (XDC_OK);
			}
			else {
				pl_listeGenerique2 = pa_zone->ListeAlertes;
				while (pl_listeGenerique2->suivant != NULL){
					if ( !strcmp(((T_REGULATION *)pl_listeGenerique2->suivant->element)->alerte_envoyee, pa_texte_alerte) ) {
						pl_listeGenerique2->suivant = pl_listeGenerique2->suivant->suivant;
						free(pl_a_supprimer);
						return (XDC_OK);
					}
					Liste_Generique_Suivant(pl_listeGenerique2);
				}
			}
			break;
		}
		Liste_Generique_Suivant(pl_listeGenerique);
	}

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : acquitter_Alerte ");
	return XDC_OK;

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	valide_acquittement_cb (T_IPC_CONN 	va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
		T_CB_ARG                        pa_ARG)

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
*  Fonction appelee lors de la reception du message
*  XDM_ICREER_ALERTE
*
* FONCTION
*
------------------------------------------------------*/
{

#ifndef _TIBCO_EMS
T_STR vl_texte_in;
T_INT4	vl_numAlerte;
T_INT2 vl_acquitement_in;
T_INT4 vl_numEqt_in;
#else
XDY_TxtAlarme vl_texte_in; 
XDY_Entier	vl_numAlerte;
XDY_Mot vl_acquitement_in;
XDY_Entier vl_numEqt_in;
#endif
 

XDY_Entier vl_acquitement;
XDY_Eqt vl_numEqt;
XDY_TexteAlerte vl_texte;

T_ZONE_REGUL *pl_zone;

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : valide_acquittement_cb ");

	if (vg_InitValide == XDC_NOK){
		XZST_03EcritureTrace( XZSTC_FATAL, "valide_acquittement_cb  : INITIALISATION NON FAITE");
		exit(0);
	}

	XZST_03EcritureTrace(VALID_NIV_TRACE_1,"<<<<<<<<<<< AQUITEMENT ALERTE >>>>>>>>>>>>>");

	/*Recuperation des donnees du message*/
	//todo mettre legs bonnes valeurs
	if(TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4,&vl_numEqt_in,
								T_IPC_FT_INT2,&vl_acquitement_in,
								T_IPC_FT_STR, &vl_texte_in,
								T_IPC_FT_INT4, &vl_numAlerte,
								NULL))
	{
		vl_numEqt = (XDY_Eqt)vl_numEqt_in;
		vl_acquitement = (XDY_Entier)vl_acquitement_in;
		strcpy(vl_texte, vl_texte_in);

		XZST_03EcritureTrace(XZSTC_INFO," Message recupere %d %d %s %d",
						vl_numEqt,vl_acquitement,vl_texte, vl_numAlerte);

		if (rechercher_zone((XDY_Eqt)vl_numEqt, &pl_zone) != XDC_OK)
			XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de retrouver la zone %d", vl_numEqt);

	}else
	{
		XZST_03EcritureTrace(XZSTC_WARNING, " valide_acquittement_cb: Impossible de recuperer le message");
		return;
	}

	acquitter_Alerte(pl_zone, vl_texte, vl_acquitement, vl_numAlerte);
	trace_process(XZAVC_REG_TRACE_ALERTE,
					pl_zone->numero_zone,
					0,
					vl_numAlerte,
					pl_zone->fmc_regulation.numero==-1?0:pl_zone->fmc_regulation.numero,
					pl_zone->fmc_regulation.numero==-1?0:pl_zone->fmc_regulation.cle,
					pl_zone->regulation_proposee.scenario.id,
					vl_acquitement,
					0,0,
					"Acquitement ");


	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : valide_acquittement_cb");

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void valide_traite_fmc_cb (T_IPC_CONN 	va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
		T_CB_ARG                        pa_ARG)

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
*  Fonction appelee lors de la reception du message
*  XDM_EREG_FMC
*
* FONCTION
*
------------------------------------------------------*/
{
	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : valide_traite_fmc_cb ");

#ifdef _TIBCO_EMS
    XDY_Entier vl_NumeroFMC;
    XDY_Entier vl_CleFMC;
    XDY_Entier vl_ScenarioFMC;
    XDY_Entier vl_IndicateurFMC;
    XDY_Entier vl_NumeroZone;
#else
	T_INT4 vl_NumeroFMC;
    T_INT4 vl_CleFMC;
    T_INT4 vl_ScenarioFMC;
    T_INT4 vl_IndicateurFMC;
    T_INT4 vl_NumeroZone;
#endif
    T_SCENARIO			vl_scenario;
    T_ZONE_REGUL *pl_zone;
	T_Liste_ZonesRegul	*pl_liste;
	T_Liste_Scenario 	*pl_listeScenarios;



	if (vg_InitValide == XDC_NOK){
		XZST_03EcritureTrace( XZSTC_FATAL, "valide_traite_fmc_cb  : INITIALISATION NON FAITE");
		exit(0);
	}
	XZST_03EcritureTrace(VALID_NIV_TRACE_1,"<<<<<<<<<<< valide_traite_fmc_cb >>>>>>>>>>>>>");

	/*Recuperation du message*/
	if(!TipcMsgRead(pa_Data->msg,T_IPC_FT_INT4,&vl_NumeroFMC,
								T_IPC_FT_INT4,&vl_CleFMC,
								T_IPC_FT_INT4,&vl_NumeroZone,
								T_IPC_FT_INT4,&vl_ScenarioFMC,
								T_IPC_FT_INT4,&vl_IndicateurFMC,
								NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING, " valide_traite_fmc_cb: Impossible de recuperer le message");
		return;
	}

	XZST_03EcritureTrace(VALID_NIV_TRACE_1,"valide_traite_fmc_cb: Message recupere %d %d %d %d %d :",
								vl_NumeroFMC,
								vl_CleFMC,
								vl_NumeroZone,
								vl_ScenarioFMC,
								vl_IndicateurFMC);


	rechercher_zone(vl_NumeroZone, &pl_zone);

	pl_liste = pg_Zones;
	//pl_liste = pa_listeZones;

	/*recherche le scenario de la FMC dans la liste des scenarios*/
	while(pl_liste != NULL)
	{
		if(pl_liste->zone.numero_zone == pl_zone->numero_zone)
			pl_listeScenarios = pl_liste->zone.ListeScenario;

		while (pl_listeScenarios != NULL)
		{
			if (pl_listeScenarios->scenario.id == vl_ScenarioFMC)
				break;

			pl_listeScenarios = pl_listeScenarios->suivant;
		}
		if (pl_listeScenarios != NULL)
			break;
		pl_liste=pl_liste->suivant;
	}


	if (vl_IndicateurFMC == 1) /* Fini */
	{	/* scebnario nominal et contexte=ARRET*/
		pl_zone->fmc_regulation.numero = vl_NumeroFMC;
		pl_zone->fmc_regulation.cle = vl_CleFMC;
		pl_zone->indicateur_forcage = XDC_FAUX;

		declencherArret(pl_zone);

		/*Historisation*/
		if (trace_process(XZAVC_REG_TRACE_VALIDATION,
								vl_NumeroZone,
								0,
								0,
								vl_NumeroFMC, vl_CleFMC,
								(*(pl_zone->scenarioNominal)).id,
								0,
								0,0
				, "fin FMC REGULATION") != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "valide_traite_fmc_cb : Impossible de tracer le process de validation cb");
		}

		appli_analyse(pl_zone);
		updateFmcRegulation(pl_zone, FALSE);

		terminerFmcRegulation(pl_zone, TRUE);
		pl_zone->fmc_regulation.numero = -1;
		pl_zone->fmc_regulation.cle = -1;
	}else
	{
		if (pl_listeScenarios != NULL)
			vl_scenario = pl_listeScenarios->scenario;
		else {
			XZST_03EcritureTrace( XZSTC_WARNING, "recherche le scenario de la FMC : Attention aucun SCENARIO correspondant a l'id %d n'a ete trouvee.",
				vl_ScenarioFMC);
			return;
		}

		if (pl_zone->degrade==XDC_VRAI) {
			pl_zone->fmc_regulation.numero = vl_NumeroFMC;
			pl_zone->fmc_regulation.cle= vl_CleFMC;
			if (trace_process(XZAVC_REG_TRACE_FORCAGE,
								vl_NumeroZone,
								0,
							0,
							vl_NumeroFMC, vl_CleFMC,
							(*(pl_zone->scenarioNominal)).id,
								0,
							0,0
						, "Fin FMC car zone degradee") != XDC_OK)
			{
						XZST_03EcritureTrace(XZSTC_WARNING, "valide_traite_fmc_cb : Impossible de tracer le process de validation cb");
			}
			terminerFmcRegulation(pl_zone, TRUE);
			return;
		}
		/*Historisation*/
			if (trace_process(XZAVC_REG_TRACE_FORCAGE,
					vl_NumeroZone,
					0,
					0,
					vl_NumeroFMC, vl_CleFMC,
					vl_scenario.id,
					0,
					0,0
					, "Mise a Jour FMC FORCAGE") != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "valide_traite_fmc_cb : Impossible de tracer le process de validation cb");
		}

		/*recherche le scenario de la FMC dans la liste des scenarios*/
		if (pl_zone->regulation_proposee.scenario.id == vl_ScenarioFMC) {
			XZST_03EcritureTrace(XZSTC_INFO,"scenario FMC = scenario propose, donc pas de forcage");
			pl_zone->fmc_regulation.numero = vl_NumeroFMC;
			pl_zone->fmc_regulation.cle = vl_CleFMC;
			/*pl_zone->regulation.scenario = vl_scenario;*/
			appli_analyse(pl_zone);
		}
		else {
			pl_zone->fmc_regulation.numero = vl_NumeroFMC;
			pl_zone->fmc_regulation.cle = vl_CleFMC;
			declencherForcage(pl_zone);

			pl_zone->regulation_proposee.scenario = vl_scenario;

			pl_zone->regulation.scenario = vl_scenario;
			appli_analyse(pl_zone);
		}
	}


	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : valide_traite_fmc_cb");

}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int genererAlerte (T_ZONE_REGUL *pa_zone,
				   XDY_Octet va_sous_type_alerte,
				   XDY_Entier va_type)


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
	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : genererAlerte ");

	XDY_TexteAlerte	vl_TexteAlerte;
	XDY_TexteAlerte	vl_TexteAlerteComplet;
	XDY_Horodate	vl_horodate;
	XDY_Entier		vl_jourSemaine;
	T_Liste_Generique *pl_listeGenerique;
	T_REGULATION *pl_regulation;

		/*Construction du TexteAlerte en fonction des booleens du contexte*/
		if (genererTexteAlerteReveil(pa_zone->regulation_proposee, vl_TexteAlerte) != XDC_OK){
			XZST_03EcritureTrace(XZSTC_WARNING,"genererAlerte : genererTexteAlerteReveil renvoie une erreur");
		}

		/*Recherche de l'horodate courant*/
		if(XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,(int *) &vl_jourSemaine,(double *) &vl_horodate) != XDC_OK)
		{
			XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de recuperer l horodate courante");
			return XDC_NOK;
		}

		if (va_sous_type_alerte == EREG_VALIDE_SOUS_TYPE_ALERTE_AJUSTEMENT)
			sprintf(vl_TexteAlerteComplet, "AJUSTEMENT REGULATION Zone %d : %s",
					pa_zone->numero_zone,
					vl_TexteAlerte);
		else if (va_sous_type_alerte == EREG_VALIDE_SOUS_TYPE_ALERTE_ARRET)
			sprintf(vl_TexteAlerteComplet, "ARRET REGULATION Zone %d : %s",
					pa_zone->numero_zone,
					vl_TexteAlerte);
		else if (va_sous_type_alerte == EREG_VALIDE_SOUS_TYPE_ALERTE_CHANGEMENT_CONTEXTE)
			sprintf(vl_TexteAlerteComplet, "CHANGEMENT CONTEXTE REGULATION Zone %d : %s",
					pa_zone->numero_zone,
					vl_TexteAlerte);
		else if (va_sous_type_alerte == EREG_VALIDE_SOUS_TYPE_ALERTE_DECLENCHEMENT)
			sprintf(vl_TexteAlerteComplet, "DECLENCHEMENT REGULATION Zone %d : %s",
					pa_zone->numero_zone,
					vl_TexteAlerte);
		else if (va_sous_type_alerte == EREG_VALIDE_SOUS_TYPE_ALERTE_TERMINER_FORCAGE)
			sprintf(vl_TexteAlerteComplet, "TERMINER FORCAGE REGULATION Zone %d : %s",
					pa_zone->numero_zone,
					vl_TexteAlerte);
		else{
			XZST_03EcritureTrace(XZSTC_WARNING, "genererAlerte : sous type d'alerte %d INCONNU", va_sous_type_alerte);
			sprintf(vl_TexteAlerteComplet, "REGULATION Zone %d : %s",
					pa_zone->numero_zone,
					vl_TexteAlerte);
		}

		/* NE PAS GENERER SIL EXISTE DEJA LA MEME ALERTE*/
		pl_listeGenerique = pa_zone->ListeAlertes;
		while (pl_listeGenerique != NULL){
			if ( !strcmp(((T_REGULATION *)pl_listeGenerique->element)->alerte_envoyee, vl_TexteAlerteComplet) ){
				XZST_03EcritureTrace(XZSTC_INFO,"genererAlerte : La meme alerte a deja ete envoyee et non acquitee");
				return XDC_OK;
			}
			Liste_Generique_Suivant(pl_listeGenerique);
		}

		XZST_03EcritureTrace(XZSTC_INFO, "genererAlerte : %d",pa_zone->regulation_proposee.contexte.alerte_deja_envoyee);

		XZST_03EcritureTrace(XZSTC_INFO,"_____GENERATION ALERTE : %s ______", vl_TexteAlerteComplet);
		if (vg_ModeSimu == XDC_FAUX) {
			if (xzaa01_Creer_Alerte(vl_horodate,
								XDC_EQT_ZONE_REGUL,
								pa_zone->numero_zone,
								XZAAC_ALERTE_REGULATION,
								vl_TexteAlerteComplet,
								vg_NumeroSite) != XDC_OK){
			XZST_03EcritureTrace(XZSTC_WARNING, "genererAlerte : Impossible de Creer l'Alerte");
			return (XDC_NOK);
		  }
		}
		
			/* Memorisation de l'alerte */
			if ((pl_listeGenerique = (T_Liste_Generique *)malloc(sizeof(T_Liste_Generique))) == NULL){
				XZST_03EcritureTrace(XZSTC_FATAL, "genererAlerte : Impossible d'allouer de la memoire");
			}
			if ((pl_listeGenerique->element = malloc(sizeof(T_REGULATION))) == NULL){
				XZST_03EcritureTrace(XZSTC_FATAL, "genererAlerte : Impossible d'allouer de la memoire");
			}

			pa_zone->regulation_proposee.sous_type_alerte_envoyee = va_sous_type_alerte;
			strcpy(pa_zone->regulation_proposee.alerte_envoyee, vl_TexteAlerteComplet);
			*((T_REGULATION *)pl_listeGenerique->element) = pa_zone->regulation_proposee;

		/*	if (pa_zone->regulation_proposee.contexte.evt_impactant) {
				XZST_03EcritureTrace(XZSTC_INFO, "genererAlerte : mise a vrai regulation_proposee.contexte.alerte_deja_envoyee");
				pa_zone->regulation_proposee.contexte.alerte_deja_envoyee = XDC_VRAI;
			}*/
			/* insertion en tete */
			Liste_Generique_Inserer_En_Tete(pl_listeGenerique, pa_zone->ListeAlertes);
		

		/*Historisation*/
		if (trace_process(XZAVC_REG_TRACE_ALERTE,
				pa_zone->numero_zone,
				0,
				va_sous_type_alerte,
				0,0,
				pa_zone->regulation_proposee.scenario.id,
				0,
				0,0,
				vl_TexteAlerteComplet) == XDC_NOK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "genererAlerte : Impossible de tracer le process de generation d'alerte");
		}


	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : genererAlerte");
	return (XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  generer alerte
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int genererReveil (T_ZONE_REGUL *pa_zone,
					char *va_transit)
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

	XDY_TexteAlerte	vl_TexteAlerte="";
	XZITT_Destinataire vl_destinataire;

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : genererReveil ");

		if ((pa_zone->fmc_regulation.numero > 0)) {

			if (strlen(va_transit)>0)
				sprintf (vl_TexteAlerte, "%s", va_transit);
			else if (pa_zone->pilotage_manuel == XDC_VRAI){
				strcpy(vl_TexteAlerte, "OPERATION ABANDONNEE : Un PRV au moins est en pilotage MANUEL");
			}
			else {
				/*Construction du TexteAlerte en fonction des booleens du contexte*/
				if (genererTexteAlerteReveil(pa_zone->regulation_proposee, vl_TexteAlerte) != XDC_OK){
					XZST_03EcritureTrace(XZSTC_WARNING,"genererAlerte : genererTexteAlerteReveil renvoie une erreur");
				}
			}
			strcpy(vl_destinataire.site,vg_NomSite);

			XZST_03EcritureTrace( XZSTC_INFO, "______Generation REVEIL sur FMC %d/%d : %s_______",
					pa_zone->fmc_regulation.numero,
					pa_zone->fmc_regulation.cle, vl_TexteAlerte);
			if (vg_ModeSimu == XDC_FAUX) {
				XZIT08_Modif_Liste_FMC(pa_zone->fmc_regulation, 
						vl_destinataire ,vl_TexteAlerte);
				strcpy(vl_destinataire.site,XDC_NOM_SITE_CI);
				XZIT08_Modif_Liste_FMC(pa_zone->fmc_regulation, 
						vl_destinataire ,vl_TexteAlerte);
			}

			/*Historisation*/
			if (trace_process(XZAVC_REG_TRACE_REVEIL,
				pa_zone->numero_zone,
				0,
				0,
				pa_zone->fmc_regulation.numero,
				pa_zone->fmc_regulation.cle,
				pa_zone->regulation.scenario.id,
				0,
				0,0,
				vl_TexteAlerte) == XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "genererAlerte : Impossible de tracer le process de generation de reveil");
			}
		}

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : genererReveil");
	return (XDC_OK);
}

/*------------------------------------------------------
* SERVICE RENDU :
*  v�rifie si un ds PRV de la zone est en pilotage manuel
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Entier checkPilotageManuel (T_ZONE_REGUL *pa_zone, XDY_Booleen va_avecGenerationAlerte){
/*
* ARGUMENTS EN ENTREE :
*  T_ZONE_REGUL      pa_zone 	: zone
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*  XDC_OK 	: la fonction s'est bien passee
*  XDC_NOK 	: la fonction s'est mal passee
*
* CONDITION D'UTILISATION
*
* FONCTION
* Interroge l'etat de chaque PRV de la zone.
* Si un des PRV est en pilotage manuel, l'information est remontee sur la zone
------------------------------------------------------*/
T_Liste_PRV 		*pl_liste;
XDY_Octet 			vl_DispoPRV;
XDY_Entier 			vl_NumEvt;
XDY_Octet 			vl_CleEvt;
XDY_Octet 			vl_Priorite;
XDY_Entier 			vl_DistanceEvt;
XDY_Entier			vl_Action;
XDY_Octet			vl_SiteAction;
XDY_VitesseRegulTxt	vl_VitesseCourante;
XDY_Bandeau			vl_Bandeau;
XDY_Booleen			vl_PilotageManuelAvant;
XDY_Horodate                    vl_horodate;
        int                             vl_jourSemaine;

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


	vl_PilotageManuelAvant = pa_zone->pilotage_manuel;
	pa_zone->pilotage_manuel = XDC_FAUX;
	for (pl_liste = pa_zone->ListePRV; pl_liste != NULL; pl_liste=pl_liste->suivant)
	{
		if(XZAT51_Utilisation_PRV (0,
										pl_liste->prv.numero_prv,
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
			XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de recuperer les infos d'Utilisation du PRV : %d", pl_liste->prv.numero_prv);
			return XDC_NOK;
		}else
		{
			if (vl_NumEvt != 0 &&
				vl_CleEvt != 0 &&
					(vl_NumEvt != pa_zone->fmc_regulation.numero ||
					 vl_CleEvt != pa_zone->fmc_regulation.cle) &&
				vl_Priorite == EREG_PRIORITE_PRV_MANU){
				XZST_03EcritureTrace( XZSTC_INFO, "PRV %d en priorite 0 : MANUELLE", pl_liste->prv.numero_prv);
				pa_zone->pilotage_manuel = XDC_VRAI;
			}
		}

	}

	if (va_avecGenerationAlerte == XDC_VRAI &&
			pa_zone->pilotage_manuel == XDC_VRAI &&
			vl_PilotageManuelAvant == XDC_FAUX)
		genererReveil(pa_zone, "");

	return XDC_OK;
}
