/*E*/
/* Fichier : $Id: ereg_cfg.h,v 1.4 2013/08/28 09:19:46 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2013/08/28 09:19:46 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER ereg_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  
*
------------------------------------------------------
* HISTORIQUE :
*
* SDU		13/03/2012 : Creation DEM/1016
* JMG		12/04/2013 : lisage pente 
------------------------------------------------------ */

#ifndef ereg_cfg
#define ereg_cfg

/* fichiers inclus */
#include "xzst.h"
#include "xdy.h"
#include "xdc.h"
#include "xzao.h"


/* definitions de constantes exportees */
#define EREG_NB_PARAM_MAX				50
#define EREG_NUMERO_MAX_PARAM_ATTENDUS	32

#define EREG_NO_SCENARIO				-1

#define EREG_PRIORITE_PRV_MANU		0
#define EREG_PRIORITE_PRV_REGUL		254

/* definitions de constantes exportees */

#define EREG_PARAM_GEN_REG_NIVEAU_AUTO_SECTEUR_PR	XDC_REG_NIVEAU_AUTO_SECTEUR_PR
#define EREG_PARAM_GEN_REG_NIVEAU_AUTO_SECTEUR_VE	XDC_REG_NIVEAU_AUTO_SECTEUR_VE
#define EREG_PARAM_GEN_REG_NIVEAU_AUTO_SECTEUR_CA	XDC_REG_NIVEAU_AUTO_SECTEUR_CA
#define EREG_PARAM_GEN_REG_NIVEAU_AUTO_SECTEUR_VD	XDC_REG_NIVEAU_AUTO_SECTEUR_VD

#define EREG_PARAM_PAR_REG_DISTANCE_PRV_CRIT 		XDC_REG_DISTANCE_PRV_CRIT
#define EREG_PARAM_PAR_REG_DECRE_VIT_PRVAMONT_DEBCH XDC_REG_DECRE_VIT_PRVAMONT_DEBCH

#define EREG_PARAM_RDT_REG_ACTIVE_VAL_NEG 		XDC_REG_ACTIVE_VAL_NEG
#define EREG_PARAM_RDT_REG_ACTIVE_DEB_EXCESS 	XDC_REG_ACTIVE_DEB_EXCESS
#define EREG_PARAM_RDT_REG_ACTIVE_TAUX_EXCESS 	XDC_REG_ACTIVE_TAUX_EXCESS
#define	EREG_PARAM_RDT_REG_ACTIVE_QTO_NULS 		XDC_REG_ACTIVE_QTO_NULS
#define EREG_PARAM_RDT_REG_ACTIVE_QMAX_TONULS 	XDC_REG_ACTIVE_QMAX_TONULS
#define EREG_PARAM_RDT_REG_ACTIVE_SEUIL_NUL 	XDC_REG_ACTIVE_SEUIL_NUL
#define EREG_PARAM_RDT_REG_ACTIVE_SEUIL_MIN 	XDC_REG_ACTIVE_SEUIL_MIN
#define EREG_PARAM_RDT_REG_SEUIL_DEBIT_MAX 		XDC_REG_SEUIL_DEBIT_MAX
#define EREG_PARAM_RDT_REG_SEUIL_DEBIT_NUL 		XDC_REG_SEUIL_DEBIT_NUL
#define EREG_PARAM_RDT_REG_SEUIL_TO_INVALIDE 	XDC_REG_SEUIL_TO_INVALIDE
#define EREG_PARAM_RDT_REG_NBMAX_CYCLE_DEB_NUL 	XDC_REG_NBMAX_CYCLE_DEB_NUL
#define EREG_PARAM_RDT_REG_DEB_CHAUSSEE_MIN 	XDC_REG_DEB_CHAUSSEE_MIN
#define EREG_PARAM_RDT_REG_HEURE_DEB_JOUR 		XDC_REG_HEURE_DEB_JOUR
#define EREG_PARAM_RDT_REG_HEURE_FIN_JOUR 		XDC_REG_HEURE_FIN_JOUR

#define	EREG_PARAM_DET_REG_COEF_CALCUL_PENTE 		XDC_REG_COEF_CALCUL_PENTE
#define	EREG_PARAM_DET_REG_COEF_CALCUL_PENTE_FIN 	XDC_REG_COEF_CALCUL_PENTE_FIN

#define EREG_PARAM_AJU_REG_TYPE_EVT_IMPACT 							XDC_REG_TYPE_EVT_IMPACT
#define EREG_PARAM_AJU_REG_FREQ_FONCT_MODU_AJUST 					XDC_REG_FREQ_FONCT_MODU_AJUST /* Frequence en minutes */
#define EREG_PARAM_AJU_REG_DECR_VIT_FINCHANT_PRV_AVAL_PLEIN_VOIE 	XDC_REG_DECR_VIT_FINCHANT_PRV_AVAL_PLEIN_VOIE

#define EREG_PARAM_VAL_REG_DELAI_LAT_GEN_ALERTE 	XDC_REG_DELAI_LAT_GEN_ALERTE
#define EREG_PARAM_VAL_REG_NIV_AUTO_MOD_VALID 		XDC_REG_NIV_AUTO_MOD_VALID
#define EREG_PARAM_VAL_REG_DELAI_LAT_GEN_ALERTE 	XDC_REG_DELAI_LAT_GEN_ALERTE
#define EREG_PARAM_VAL_REG_DUREE_APP_SCEN_FORCE 	XDC_REG_DUREE_APP_SCEN_FORCE

#define EREG_PARAM_APP_REG_DELAI_NON_REPONSE_TEPRV 		XDC_REG_DELAI_NON_REPONSE_TEPRV
#define EREG_PARAM_APP_REG_DELAI_LAT_TEPRV_PERTE_REGUl 	XDC_REG_DELAI_LAT_TEPRV_PERTE_REGUl
#define EREG_PARAM_APP_REG_DELAI_RECO_SERV_FAX_MESS 	XDC_REG_DELAI_RECO_SERV_FAX_MESS
#define EREG_PARAM_APP_REG_CADENCE_MODE_SIMU 			XDC_REG_CADENCE_MODE_SIMU

#define EREG_CONDITION_SIMPLE 		0
#define EREG_CONDITION_COMPLEXE 	1

#define EREG_CONDITIONS_NB_MAX_FILLES	4
#define EREG_CONDITIONS_NULL	0


typedef struct Liste_Generique {
	void *element;
	struct Liste_Generique *suivant;
} T_Liste_Generique;

#define Liste_Generique_Suivant(p) \
 if (p!=NULL) p=p->suivant

#define Liste_Generique_Inserer_En_Tete(nouveau, tete) \
		nouveau->suivant = tete; tete = nouveau

typedef struct {
	XDY_Entier Q;
	XDY_Entier TO;
	XDY_Booleen Q_valide;
	XDY_Booleen TO_valide;
}T_Donnees;


typedef struct {
	XDY_Eqt numero_prv;
	XDY_TypeEqt type_prv;
	XDY_VitesseRegul vitesse_nominale;
	XDY_Entier zone_prv;
	XDY_Booleen critique_prv;
/*	XDY_Autoroute autoroute_prv; */
	XDY_PR pr_prv;
	XDY_Sens sens_prv;
	XDY_VitesseRegul vitesse_courante; 	/* Vitesse remontee par l'equipement : mis a�jour par CB*/
	XDY_Bandeau bandeau; 				/* Bandeau remonte par l'equipement : mis a�jour par CB*/
	XDY_Booleen etat_de_marche; 		/* Etat remonte par l'equipement : mis a�jour par CB*/
	XDY_VitesseRegulTxt vitesse_texte; 	/* Vitesse texte remontee par l'equipement : mis a jour par CB*/
} T_PRV;

typedef struct {
	XDY_Eqt	equipement;
	XDY_Entier	vitesse;
	XDY_Booleen critique;
	XDY_Bandeau vitesse_bandeau;
	T_PRV *prv;
} T_COMMANDE;

typedef struct S_T_COMMANDE {
	T_COMMANDE commande;
	struct S_T_COMMANDE *suivant;
} T_Liste_Commande;

#define EREG_NB_DATA_STATION_MAX		5
#define EREG_NB_DATA_CALCUL_PENTE		4
#define EREG_NB_DATA_STATION_CYCLE_Q	5
#define EREG_NB_DATA_STATION_CYCLE_TO	3
#define EREG_NB_DATA_STATION_CYCLE_PENTE	8

typedef struct {
	XDY_Entier station;
	XDY_Entier zone_station;
	XDY_Sens sens;
	T_Donnees data[EREG_NB_DATA_STATION_MAX];
	T_Donnees data_pente[EREG_NB_DATA_STATION_CYCLE_PENTE];
	double QL1;
	double QL2;
	double QL3;
	double QL4;
	XDY_Entier positionDerniereStationRecue;
	XDY_Entier positionDerniereStationRecuePente;
	XDY_Horodate horodateDerniereStationRecue;
	double data_lisseeQ;
	double data_lisseeTO;
	XDY_Booleen data_lisseeQ_valide;
	XDY_Booleen data_lisseeTO_valide;
	XDY_Entier pente;
	XDY_Booleen pente_valide;
	XDY_Booleen comptage_panne;
	XDY_Entier station_comptage;
} T_STATION;

typedef struct S_T_STATION {
	T_STATION		station;
	struct S_T_STATION 	*suivant;
} T_Liste_Station;

#define EREG_CONDITION_TYPE_ACTIVATION			XDC_REG_CONDITION_TRAF_ACTIV
#define EREG_CONDITION_TYPE_DESACTIVATION		XDC_REG_CONDITION_TRAF_DESACTIV
#define EREG_CONDITION_TYPE_HORAIRE				XDC_REG_CONDITION_TRAF_HORAIRE

#define EREG_CONDITION_TYPE_HORAIRE_JOUR_WE			XDC_REG_JOURS_WE
#define EREG_CONDITION_TYPE_HORAIRE_JOUR_OUVRES		XDC_REG_JOURS_OUVRES

#define EREG_CONDITION_TYPE_HORAIRE_HORODATE		XDC_REG_HORAIRE_HORODATE	/*plage*/
#define EREG_CONDITION_TYPE_HORAIRE_HEURE_JOUR		XDC_REG_HORAIRE_HEURE_JOUR	/*lundi 8h 12h*/
#define EREG_CONDITION_TYPE_HORAIRE_HEURE			XDC_REG_HORAIRE_HEURE		/*12h 14h ts les jours*/
#define EREG_CONDITION_TYPE_HORAIRE_HEURE_TYPE		XDC_REG_HORAIRE_TYPE_HEURE	/*week ends 8h 10h*/

/* definitions de types exportes */
typedef struct CONDITION{
	XDY_Entier		id;
	XDY_Entier 		type_condition; 			 /* EREG_CONDITION_TYPE_HORAIRE, EREG_CONDITION_TYPE_ACVTIVATION, EREG_CONDITION_TYPE_DESACTIVATION */
	XDY_Entier		type_condition_horaire;		 /* EREG_CONDITION_TYPE_HORAIRE_HORODATE, EREG_CONDITION_TYPE_HORAIRE_HEURE_JOUR, EREG_CONDITION_TYPE_HORAIRE_HEURE, EREG_CONDITION_TYPE_HORAIRE_HEURE_TYPE */
	XDY_Entier		type_condition_horaire_jour; /* EREG_CONDITION_TYPE_HORAIRE_WE, EREG_CONDITION_TYPE_HORAIRE_OUVRES */
	XDY_Entier  	id_scenario;
	XDY_Entier		num_station;
	XDY_Sens		sens_station;
	char			libelle[80];
	T_STATION   	*station; 					 /* pointeur sur la station */
	XDY_Booleen		condition_simple;

/* Champs pour les conditions Trafic*/
	XDY_Entier		seuil;
	XDY_Entier  	type_donnee_trafic;			/* XDC_REG_DONNEE_Q, XDC_REG_DONNEE_TO, XDC_REG_DONNEE_PENTE */
	XDY_Octet		operateur_comparaison;		/* XDC_REG_OPERATEUR_INF, XDC_REG_OPERATEUR_SUP, XDC_REG_OPERATEUR_EGAL */

/* Champs pour les conditions Horaires*/
	XDY_Horodate	horodate_debut;
	XDY_Horodate	horodate_fin;
	XDY_Entier		heure_debut;
	XDY_Entier  	minute_debut;
	XDY_Entier		heure_fin;
	XDY_Entier		minute_fin;

/* Champs pour les conditions SIMPLES/COMPLEXES*/
	XDY_Entier  		liste_id_conditions_filles[EREG_CONDITIONS_NB_MAX_FILLES];
/*	struct CONDITION	*liste_conditions_filles[EREG_CONDITIONS_NB_MAX_FILLES]; */
	void				*liste_conditions_filles;
	XDY_Entier			id_condition_alternative;
	struct CONDITION	*condition_alternative;
} T_CONDITION;


typedef struct S_T_CONDITION_SCENARIO {
	T_CONDITION *condition;
	struct S_T_CONDITION_SCENARIO *suivant;
} T_Liste_Condition_Scenario;

typedef struct S_T_CONDITION_ZONE {
	T_CONDITION condition;
	struct S_T_CONDITION_ZONE *suivant;
} T_Liste_Condition_Zone;


typedef struct  {
	XDY_Entier	id;
	char		libelle[80];
	XDY_Booleen	nominal;
	XDY_Booleen degrade;
/*	XDY_Booleen inhibe; */
	XDY_Entier niveau;
	XDY_Entier zone_scenario;
/*	T_Liste_Condition *conditions_activ;
	T_Liste_Condition *conditions_desactiv;
	T_Liste_Condition *conditions_horaire;*/
	T_Liste_Condition_Scenario *conditions;
	XDY_Booleen has_conditions_horaire;
	T_Liste_Commande	*ListeCommandes;
	XDY_Booleen activable;
} T_SCENARIO;


typedef struct {
	XDY_Booleen scenario_ideal;	/*=> scenario ideal*/
	XDY_Booleen declenchement_horaire; /*=>scenario ideal (dec. horaire)*/
	XDY_Booleen evt_impactant; /*Evenement impactant avec ou sans qualification*/
	XDY_Booleen qualification_operateur; /*suivant ça*/
	XDY_Booleen panne_PRV_critique; /*panne de PRV critique*/
	XDY_Booleen panne_PRV_non_critique; /*panne de PRV non critique*/
	XDY_Booleen chantiers; /*chantiers ou chantiers et panne de PRV non critique*/
	XDY_Octet	checksum;
	XDY_Booleen evt_mis_a_jour;
	XDY_Booleen alerte_deja_envoyee;
}T_Contexte;

typedef struct {
	T_SCENARIO	scenario;
	T_Contexte	contexte;
	XDY_Entier	mesure; /* Constantes EREG_MESURE_xxx */
	XDY_Octet	reponse_operateur;
	XDY_Booleen indicateur_horaire;
	XDY_Horodate horodate_regulation;
	char		transitoire[100];
	XDY_TexteAlerte alerte_envoyee;
	XDY_Entier sous_type_alerte_envoyee;
	XDY_Booleen			appli_process;
} T_REGULATION;

#define EREG_MESURE_DEFAUT 			3
#define EREG_MESURE_ARRET  			1
#define EREG_MESURE_FORCAGE  		2
#define EREG_MESURE_DECLENCHEMENT  	3
#define EREG_MESURE_AJUSTEMENT  	4
#define EREG_MESURE_CHGT_CONTEXTE  	5

typedef struct S_T_PRV {
	T_PRV		prv;
	struct S_T_PRV 	*suivant;
} T_Liste_PRV;


typedef struct S_T_SCENARIO {
	T_SCENARIO scenario;
	struct S_T_SCENARIO *suivant;
	struct S_T_SCENARIO *precedent;
} T_Liste_Scenario;

typedef struct {
	XDY_Eqt 			numero_zone ;
	char 			nom_zone[50];
	XDY_Autoroute 		autoroute_prv;
	XDY_Sens 			sens_prv;
	XDY_PR 				pr_debut;
	XDY_PR 				pr_fin;
	XDY_VitesseRegul 	vitesse_amont;
	XDY_Booleen 		degrade;
	XDY_Booleen 		inhibition;
	XDY_Booleen 		mode_horaire;
	XDY_Evt				fmc_regulation;
	XDY_Booleen 		regulation_en_cours;
	XDY_Horodate		horodate_forcage;
	XDY_Booleen 		indicateur_forcage; /* Scenario Force*/
/*	T_Contexte			*contexte;  		/* Pointeur sur le contexte a utiliser pour la creation mise a jour de FMC */
	T_SCENARIO 			*scenarioNominal;
	T_REGULATION 		regulation;
	T_REGULATION 		regulation_proposee;
	T_Liste_PRV			*ListePRV;
	T_Liste_Station		*ListeStation;
	T_Liste_Scenario	*ListeScenario;
	T_Liste_Condition_Zone	*ListeConditions;
	T_Liste_Generique	*ListeAlertes;
	XDY_Booleen			pilotage_manuel;
} T_ZONE_REGUL;

typedef struct S_T_ZONEREGUL {
	T_ZONE_REGUL			zone;
	struct S_T_ZONEREGUL 	*suivant;
} T_Liste_ZonesRegul;

typedef struct {
   	XDY_Eqt				Numero;
   	XDY_TypeEqt			Type;
   	XDY_NomParamRegul	Nom;
	XDY_Entier			Valeur;
	XDY_Booleen			Initialise;
} T_Param;


/* definition de macro exportees */

/* declarations de variables globales */

/* delaration de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  initialisation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cfg_init();

/*
* ARGUMENTS EN ENTREE :
*  
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
*   Chargement de la liste des zones puis de la liste des PRV
*
------------------------------------------------------*/

XDY_Entier getParamValue(XDY_Eqt va_numero);

#endif
