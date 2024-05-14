/*E*/
/* Fichier : $Id: ereg_application.c,v 1.9 2020/12/02 10:55:11 gesconf Exp $      Release : $Revision: 1.9 $        Date : $Date: 2020/12/02 10:55:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER ereg_application.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de general de trace de la regulation >
*
------------------------------------------------------
* HISTORIQUE :
*
* SDU		15/03/2012 : Creation DEM/1016
* JMG		10/07/12 : correction ecrasement pilotage manu 1.2
* JMG		24/04/13 : trace WARNING
* JMG		17/06/13 : 130 RAPPEL = extinction
* JMG		03/08/15 ; 130 plus d extinction lot 22 DEM131 1.5
* JMG		03/09/15 ; correction 130 plus d extinction lot 22 DEM131 1.6
* PNI		5/11/19	: Prise en compte des pictos autres que limitation de vitesse DEM1363 1.7
* PNI		2/12/20	: horadate a nulle pour appel XZAT51 1.8
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */
#include "ereg_application.h"
#include "ereg_cfg.h"
#include "ereg_util.h"
#include "xzat51.h"
#include "xzac303.h"
#include "xzac802.h"

/* definitions de constantes */
#define CLE_TIMER_APPLICATION		"cle_timer_application"
#define CLE_TIMER_APPLICATION_SIMU		"cle_timer_application_simu"
#define CLE_TIMER_FAX_APPLICATION		"cle_timer_fax_application"

#define SG_PING         "/etc/ping"
#define SG_TO_PING      8
#define SG_BYTES_PING   64
#define SG_COUNT_PING   1
#define SG_TOKEN_TST    "64"

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
int	vg_InitAppli = XDC_NOK;
static vm_fax_ok = XDC_OK;
static int vm_periode = 6 * 60;
static char vm_cle[50];

/* declaration de fonctions internes */
void appli_process_cb();
void appli_check_fax_cb();
char *trim ( char * );

/*void appli_etat_prv_cb(T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_Data, T_CB_ARG pa_ARG); */
void appli_etat_dispo_prv_cb(T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_Data, T_CB_ARG pa_ARG);
void appli_cmd(T_ZONE_REGUL *pa_zone, T_COMMANDE va_commande, XDY_VitesseRegul va_vitesse, XDY_Bandeau va_bandeau);
void appli_maj_etat_prv(XDY_Etat_PRV, int *);

XDY_Entier vitesseTextToVitesse(XDY_VitesseRegulTxt );
void vitesseToVitesseText(XDY_Entier va_vitesseIn, XDY_VitesseRegulTxt va_vitesseOut);


/* definition variables externes */
extern XDY_Booleen 			vg_ModeSimu;
extern T_Liste_ZonesRegul 	*pg_Zones;
extern XDY_Basedd			vg_Base;
extern XDY_NomSite  		vg_NomSite;
extern XDY_NomMachine  		vg_NomMachine;
extern XDY_Horodate		 	vg_HorodateSimu;
extern XDY_District			vg_NumeroSite;
extern T_Param                  tg_params[EREG_NB_PARAM_MAX];

extern XDY_Booleen		vg_tebeg;

int appli_init()
/* ------------------------------------------------------
 * ARGUMENTS EN ENTREE :
 *  aucun
 *
 * ARGUMENTS EN SORTIE :
 *  aucun
 *
 * CODE RETOUR :
 *  aucun
 *
 * CONDITION D'UTILISATION
 *
 * FONCTION
 *
 ------------------------------------------------------*/
{
	int 				vl_indice_callback;
	int 				vl_Resultat;
	XDY_Booleen 		vl_Abonnement;
	XDY_Datagroup		vl_NomDG_etat, vl_NomDG_ietat_dispo, vl_NomDG_ietat_inieqt, vl_DG;
	XDY_Horodate		vl_horodate;
	XDY_Entier			vl_jourSemaine;
	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : appli_init");

	XDG_EncodeDG2 ( vl_DG, XDG_REGUL_APPLICATION, vg_NomSite) ;
	if ( !XDG_JoindreDG( vl_DG ) )
	{
	   XZST_03EcritureTrace(XZSTC_WARNING, "Abonnement au Datagroup %s non effectue ", vl_DG );
	}

	/*Armement du timer appli_process_cb sur XDG_REGUL_APPLICATION pour le msg XDM_REGUL_APPLICATION*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_REGUL_APPLICATION),
					appli_process_cb,
					(T_CB_ARG)NULL) == NULL)
	{
		XZST_03EcritureTrace( XZSTC_FATAL,
				"Erreur Armement Callback sur XDM_REGUL_APPLICATION." );
	}

	XZST_03EcritureTrace(XZSTC_INFO, "Programmation TIMER pour Envoi Message XDM_REGUL_APPLICATION");
	if(XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME ,
						(int *) &vl_jourSemaine,
						(double *) &vl_horodate) != XDC_OK){
			XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de recuperer l horodate courante") ;
	}

	/* TODO :  mettre la bonne  valeur pour le delai*/
	if(vg_ModeSimu == XDC_FAUX){
		strcpy(vm_cle,CLE_TIMER_APPLICATION);
	}
	else {
		strcpy(vm_cle,CLE_TIMER_APPLICATION_SIMU);
		vm_periode = tg_params[EREG_PARAM_APP_REG_CADENCE_MODE_SIMU].Valeur;
	}

	/*
	if (XZSM_01DemanderProgrammation( vl_horodate+vl_periode,
				vl_periode,
				vl_DG,
				XDM_REGUL_APPLICATION,
				vl_cle,
				XZSMC_SEC_VRAI) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL,
				"ERREUR Programmation du timer XDM_REGUL_APPLICATION non faite." );
	}
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_CHECK_FAX),
											appli_check_fax_cb,
											(T_CB_ARG)NULL) == NULL)
	{
		XZST_03EcritureTrace( XZSTC_FATAL,
				"Erreur Armement Callback sur XDM_CHECK_FAX." );
	}

	if(vg_ModeSimu == XDC_FAUX)
	{
		if (XZSM_01DemanderProgrammation( vl_horodate+tg_params[XDC_REG_DELAI_RECO_SERV_FAX_MESS].Valeur,
				tg_params[XDC_REG_DELAI_RECO_SERV_FAX_MESS].Valeur,
				vl_DG,
				XDM_CHECK_FAX,
				CLE_TIMER_FAX_APPLICATION,
				XZSMC_SEC_VRAI) != XDC_OK)
		{
			XZST_03EcritureTrace( XZSTC_FATAL,
					"ERREUR Programmation du timer XDM_REGUL_APPLICATION non faite." );
		}

		/*Abonnenement au DataGroup XDG_ETAT_PRV*/
		XDG_EncodeDG2 ( vl_NomDG_etat, XDG_ETAT_PRV, vg_NomSite) ;
		if ( !XDG_JoindreDG( vl_NomDG_etat ) )
		{
		   XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", vl_NomDG_etat );
		}

		/*A  Armement Callback sur Reception XDM_IETAT_DISPO_EQT */
		XZST_03EcritureTrace(XZSTC_INFO, "Armement CB appli_etat_dispo_prv_cb() sur XDM_IETAT_DISPO_EQT");
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IETAT_DISPO_EQT),
												appli_etat_dispo_prv_cb,
												(T_CB_ARG)NULL) == NULL)
		{
			XZST_03EcritureTrace( XZSTC_FATAL, "Erreur Armement Callback sur XDM_IETAT_DISPO_EQT.");
		}

		/*Abonnenement au DataGroup XDG_IETAT_DISPO_EQT*/
		XDG_EncodeDG2 ( vl_NomDG_ietat_dispo, XDG_IETAT_DISPO_EQT, vg_NomSite) ;
		if ( !XDG_JoindreDG( vl_NomDG_ietat_dispo ) )
		{
			XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", vl_NomDG_ietat_dispo );
		}

		/*Abonnenement au DataGroup XDG_IETAT_INIEQT*/
		XDG_EncodeDG2 ( vl_NomDG_ietat_inieqt, XDG_IETAT_INIEQT, vg_NomMachine) ;
		/*A joint ce datagroup */
		if ( !XDG_JoindreDG( vl_NomDG_ietat_inieqt ) )
		{
			XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", vl_NomDG_ietat_inieqt );
		}

		/*A Appel xzat26_Init_Etats_Eqts pour Initialisation des etats equipements*/
		if(xzat27_Init_Etats_EqtsIhm(XDC_EQT_PRV,
									0,
									vl_NomDG_ietat_dispo ) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING , "Probleme lors de l'initialisation des etats des equipements");
		}

		/* appel a xzea70_abt_etat_prv pour l'abonnement aux etats des prv  */
		if(XZEA70_Abt_Etat_PRV (XDC_VRAI,
								vl_NomDG_etat,
								appli_maj_etat_prv,
	    					   	vg_NomSite,
	    					   	&vl_Resultat ) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING , "Probleme lors de l abonnement a l etat du PRV : nom site %s", vg_NomSite);
		}

	}


	vg_InitAppli = XDC_OK;

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : appli_init");


	return XDC_OK;
}

void appli_analyse(T_ZONE_REGUL *pa_Zone)
/* ------------------------------------------------------
 * ARGUMENTS EN ENTREE :
 *  T_ZONE_REGUL *va_Zone
 *
 * ARGUMENTS EN SORTIE :
 *  aucun
 *
 * CODE RETOUR :
 *  aucun
 *
 * CONDITION D'UTILISATION
 * Historisation des donnees d'entree par appel
 * 					a la fonction trace_process
 * FONCTION
 *
 ------------------------------------------------------*/
{
	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : appli_analyse ");


	if (vg_InitAppli == XDC_NOK){
		XZST_03EcritureTrace( XZSTC_FATAL, "appli_analyse  : INITIALISATION NON FAITE");
		exit(0);
	}
	XZST_03EcritureTrace( XZSTC_DEBUG1, "##### APPLICATION ANALYSE zone%d #####", pa_Zone->numero_zone);

	/* La regulation proposee devient la regulation en vigueur*/
	if (updateRegulationProposee(pa_Zone, pa_Zone->regulation_proposee) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "appli_analyse : Impossible de mettre a jour la regulation avec la regulation proposee");
	}

	XZST_03EcritureTrace( XZSTC_INFO, "      ... La Regulation Proposee devient la regulation Courante : Scenario %d #####", pa_Zone->regulation.scenario.id);

	/*if (trace_process(EREG_TRACE_TYPE_APPLICATION,
					pa_Zone->numero_zone,
					0,
					0,
					pa_Zone->fmc_regulation.numero,pa_Zone->fmc_regulation.cle,
					pa_Zone->regulation_proposee.scenario.id,
					0,
					"Appli analyse : %s ",pa_Zone->regulation_proposee.scenario.libelle) == XDC_NOK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "appli_analyse : Impossible de tracer le process");
	}*/
	if (vg_tebeg) {
		XZST_03EcritureTrace( XZSTC_WARNING,"arret apres validation!!!");
				return ;
	}

	updateFmcRegulation(pa_Zone,TRUE);

	appli_process(pa_Zone);

	XZST_03EcritureTrace( XZSTC_DEBUG1, "##### APPLICATION ANALYSE TERMINEE #####");
	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : appli_analyse");
}

void appli_cmd(T_ZONE_REGUL *pa_zone, T_COMMANDE va_commande, XDY_VitesseRegul va_vitesse, XDY_Bandeau va_bandeau)
/* ------------------------------------------------------
 * ARGUMENTS EN ENTREE :
 * T_PRV vl_PRV
 *
 * ARGUMENTS EN SORTIE :
 *  aucun
 *
 * CODE RETOUR :
 *  aucun
 *
 * CONDITION D'UTILISATION
 * envoi la commande au prv sur le terrain
 * appel a la fonction xzac550_piloter_prv
 * et mise a jour de l'etat du prv sur la zone
 *
 * FONCTION
 *
 ------------------------------------------------------*/
{
	T_Liste_ZonesRegul *tmpZone = NULL;
	XDY_Evt vl_evt;
	XDY_Operateur vl_Operateur;

	XDY_Horodate  vl_HeureLancement;
	XDY_Octet     vl_Site;
	XDY_Octet     vl_Priorite;
	XDY_Entier    vl_DistanceEvt;
	XDY_Octet     vl_site_origine;
	XDY_VitesseRegulTxt	vl_vitesseTexte;
	int vl_JourSemaine;
	XDY_Entier vl_action=0;
	XDY_Booleen	vl_130_rappel = XDC_FAUX;

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : appli_cmd %d %d %s",
						va_commande.prv->numero_prv, va_vitesse, va_bandeau);

	if (vg_InitAppli == XDC_NOK){
		XZST_03EcritureTrace( XZSTC_FATAL, "appli_cmd  : INITIALISATION NON FAITE");
		exit(0);
	}

	if ((va_vitesse==0) && (!strcmp(va_bandeau,"")))
		vl_130_rappel = XDC_VRAI;
	if ((va_vitesse==130) && (!strcmp(va_bandeau,"RAPPEL")))
		vl_130_rappel = XDC_VRAI;

/*	if (pa_zone->pilotage_manuel == XDC_VRAI){
		XZST_03EcritureTrace( XZSTC_WARNING, "appli_cmd  : ANNNULATION tentative d'envoi des commandes alors que la zone est en pilotage MANUEL");
		return;
	}
*/
	if(vg_ModeSimu == XDC_FAUX)
	{
		if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
		                        (int *) &vl_JourSemaine, (double *) &vl_HeureLancement ) != XDC_OK )
		{
			XZST_03EcritureTrace(   XZSTC_WARNING , "Probleme lors de la lecture de l'heure en Sec");
		    return ;
		}


		vl_evt.numero = pa_zone->fmc_regulation.numero;
		vl_evt.cle = pa_zone->fmc_regulation.cle;

		vl_Operateur = 1;
		vl_Priorite = EREG_PRIORITE_PRV_REGUL;
		vl_DistanceEvt = 0;
		vl_site_origine= vg_NumeroSite;
		vitesseToVitesseText(va_vitesse, vl_vitesseTexte);

		XZST_03EcritureTrace( XZSTC_INFO," %d %d %d",
					pa_zone->regulation.mesure,
					pa_zone->regulation_proposee.scenario.id,
					pa_zone->scenarioNominal->id);

	
		if ((vl_130_rappel==XDC_FAUX) && (pa_zone->regulation.mesure != EREG_MESURE_ARRET) &&
				(pa_zone->regulation.scenario.id != pa_zone->scenarioNominal->id)){
				/* (pa_zone->regulation_proposee.scenario.id != pa_zone->scenarioNominal->id)){*/
			XZST_03EcritureTrace( XZSTC_INFO,"  avant XZAC802_Commande_PRV");
			if(XZAC802_Commande_PRV (vl_evt.numero,
							  	  vl_evt.cle,
							  	  vl_Operateur,
							  	  va_commande.prv->numero_prv,
							  	  vl_HeureLancement,
							  	  XZSC_22NumeroSiteLocal(),
							  	  va_bandeau,
							  	  vl_vitesseTexte,
							  	  vl_Priorite,
							  	  vl_DistanceEvt,
							  	  vl_site_origine, &vl_action) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "appli_cmd : Impossible de commander le PRV  numeroPRV : %d numEvt : %d",
										va_commande.equipement,
										vl_evt.numero);
			}
		}
		else {
			XZST_03EcritureTrace( XZSTC_INFO,"  avant XZAC303_Commande_Fin_PRV");
			if(XZAC303_Commande_Fin_PRV(vl_evt.numero,
							  	  vl_evt.cle,
							  	  vl_Operateur,
							  	  va_commande.prv->numero_prv,
							  	  vl_HeureLancement,
							  	  XZSC_22NumeroSiteLocal(),
							  	  va_bandeau,
							  	  vl_vitesseTexte,
							  	  vl_Priorite,
							  	  vl_site_origine) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "appli_cmd : Impossible de commander le PRV  numeroPRV : %d numEvt : %d",
										va_commande.equipement,
										vl_evt.numero);
			}

		}
		XZST_03EcritureTrace( XZSTC_INFO, "Commande %s (%s) envoyee au PRV %d",
											vl_vitesseTexte,
											va_bandeau,
											va_commande.equipement);

	}
	else if(vg_ModeSimu == XDC_VRAI)
	{
		/*mise a jour de l'etat du PRV sur la zone avec la valeur de commande*/
		strcpy(va_commande.prv->bandeau,va_bandeau);
		va_commande.prv->vitesse_courante = va_vitesse;
		va_commande.prv->etat_de_marche = XDC_VRAI;
		vitesseToVitesseText(va_vitesse, va_commande.prv->vitesse_texte);
	}

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : appli_cmd");
}

void appli_process(T_ZONE_REGUL *pa_Zone){
/* ------------------------------------------------------
 * ARGUMENTS EN ENTREE :
 *	T_SCENARIO *va_scenario : le scenario a faire appliquer
 * ARGUMENTS EN SORTIE :
 *  aucun
 *
 * CODE RETOUR :
 *  aucun
 *
 * CONDITION D'UTILISATION
 * Compare l'affichage sur le terrain avec
 * 		   l'affichage requis pour le scenario donné
 *
 * FONCTION
 *
 ------------------------------------------------------*/
	T_Liste_Commande	*pl_ListeCommandes = NULL;
	XDY_Octet 			vl_DispoPRV;
	XDY_Octet 			vl_TypePRV;
	XDY_Entier 			vl_NumEvt;
	XDY_Octet 			vl_CleEvt;
	XDY_Octet 			vl_Priorite;
	XDY_Entier 			vl_DistanceEvt;
	XDY_Entier			vl_Action;
	XDY_Octet			vl_SiteAction;
	XDY_Booleen			vl_TransitoireAppliquee = XDC_FAUX;
	XDY_Booleen			vl_CommandeAEnvoyer = XDC_FAUX;
	XDY_Booleen			vl_UneCommandeEnvoyee = XDC_FAUX;
	XDY_Booleen			vl_PRV_Manu_Ne_Pas_Modifier = XDC_FAUX;

	XDY_VitesseRegul	vl_VitesseCouranteAmont;
	XDY_VitesseRegulTxt	vl_VitesseCouranteTxt;
	XDY_VitesseRegul	vl_VitesseCourante;
	XDY_VitesseRegul	vl_VitesseCible;
	float			    vl_PRCible;
	XDY_VitesseRegul	vl_VitesseAEnvoyer;

	XDY_Bandeau			vl_Bandeau;
	XDY_Bandeau			vl_BandeauCourant;
	XDY_Horodate			vl_horodate;
	int				vl_jourSemaine;
	char vl_DG[50];
	char vl_transitoire[100] = "Regulation Transitoire : ";
	char vl_vitessesNominales[100] = "";
	char vl_vitessesEnvoyees[100] = "";
	char vl_vitessesTerrain[100] = "";
	char tmp[100];

	XDY_VitesseRegulTxt vl_temp;
	XDY_Booleen	vl_130_rappel=XDC_FAUX;


	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : appli_process zone %d", pa_Zone->numero_zone);
	XZST_03EcritureTrace( XZSTC_INFO, "##### APPLICATION PROCESS ZONE %d / vitesse Amont=%d #####", pa_Zone->numero_zone, pa_Zone->vitesse_amont);

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



	pl_ListeCommandes = pa_Zone->regulation.scenario.ListeCommandes;
	/*pour le premier panneau*/
	vl_VitesseCouranteAmont = pa_Zone->vitesse_amont;
	/*vl_VitesseCouranteAmont = -1; */

	XZST_03EcritureTrace( XZSTC_INFO, "num PRV: %d", pl_ListeCommandes->commande.prv->numero_prv);

	/*pour chaque Panneau*/
	while(pl_ListeCommandes != NULL)
	{
		vl_CommandeAEnvoyer = XDC_FAUX;
		vl_VitesseCible = pl_ListeCommandes->commande.vitesse;
		vl_VitesseAEnvoyer = vl_VitesseCible;
		vl_PRCible = (float)(pl_ListeCommandes->commande.prv->pr_prv)/1000.0;

		XZST_03EcritureTrace( XZSTC_INFO, "prv %d PR %3.1f",pl_ListeCommandes->commande.prv->numero_prv, vl_PRCible);
		sprintf(tmp, "%s %3.1f(%dkm/h)", vl_vitessesNominales, vl_PRCible, vl_VitesseCible);
		strcpy(vl_vitessesNominales, tmp);

		/*recuperation des informations d'utilisation et de la priorite du PRV afin de savoir si on est en pilotage manuel ou non*/
		if(XZAT51_Utilisation_PRV (0,
					pl_ListeCommandes->commande.equipement,
										&vl_DispoPRV,
										vl_VitesseCouranteTxt,
										vl_BandeauCourant,
										&vl_NumEvt,
										&vl_CleEvt,
										&vl_Priorite,
										&vl_DistanceEvt,
										&vl_Action,
										&vl_SiteAction) != XDC_OK)
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de recuperer les infos d'Utilisation du PRV : %d",
						pl_ListeCommandes->commande.equipement);
			strcpy(vl_VitesseCouranteTxt, pl_ListeCommandes->commande.prv->vitesse_texte);
			strcpy(vl_BandeauCourant, pl_ListeCommandes->commande.prv->bandeau);
		}
		vl_VitesseCourante = vitesseTextToVitesse(vl_VitesseCouranteTxt);


		if(vg_ModeSimu == XDC_FAUX)
		{
			vl_VitesseCourante = pl_ListeCommandes->commande.prv->vitesse_courante;
			strcpy(vl_BandeauCourant, pl_ListeCommandes->commande.prv->bandeau );

			XZST_03EcritureTrace( XZSTC_INFO, "PRV %d  : priorite %d // action = %d // evt = %d",
									pl_ListeCommandes->commande.prv->numero_prv,
									vl_Priorite, vl_Action, vl_NumEvt);
		}

		rtrim(vl_BandeauCourant);

		sprintf(tmp, "%s %3.1f(%dkm/h)", vl_vitessesTerrain, vl_PRCible, vl_VitesseCourante);
		strcpy(vl_vitessesTerrain, tmp);

		strcpy(vl_Bandeau, "" );

		if ((vl_VitesseCourante !=-1) && (vl_VitesseCourante !=0) && ( 
				(vl_VitesseCourante - vl_VitesseAEnvoyer > 20) ||
				( (vl_VitesseCourante==0) && (130 - vl_VitesseAEnvoyer > 20))
						) )
		{
			vl_VitesseAEnvoyer = vl_VitesseCourante -20;
			/*sequence transitoire memorisee*/
			vitesseToVitesseText(vl_VitesseAEnvoyer, vl_temp);
			sprintf(vl_transitoire, "%s %3.1f(%skm/h)", vl_transitoire, vl_PRCible, vl_temp);
			vl_TransitoireAppliquee = XDC_VRAI;
			/*vl_CommandeAEnvoyer = XDC_VRAI;*/
		}

		if ( (vl_VitesseCouranteAmont != -1) && (vl_VitesseCouranteAmont - vl_VitesseAEnvoyer > 20))
		{
			vl_VitesseAEnvoyer = vl_VitesseCouranteAmont - 20;
			/*sequence  transitoire memorisee*/
			vitesseToVitesseText(vl_VitesseAEnvoyer, vl_temp);
			sprintf(vl_transitoire, "%s %3.1f(%skm/h)", vl_transitoire, vl_PRCible, vl_temp);
			vl_TransitoireAppliquee = XDC_VRAI;
			/*vl_CommandeAEnvoyer = XDC_VRAI;*/
		}

		if(vl_VitesseAEnvoyer == vl_VitesseCouranteAmont)
			strcpy(vl_Bandeau , "RAPPEL");

		XZST_03EcritureTrace( XZSTC_INFO, "PRV %d  : Vitesse Cible=%d(%s), VitesseCourante=%d(%s) VitesseAEnvoyer=%d,appli_process=%d",
				pl_ListeCommandes->commande.prv->numero_prv,
				vl_VitesseCible,
				vl_Bandeau,
				vl_VitesseCourante,
				vl_BandeauCourant,vl_VitesseAEnvoyer,pa_Zone->regulation.appli_process);

		vl_VitesseCouranteAmont = vl_VitesseAEnvoyer;

		/*traitement specifique 130 RAPPEL*/
		
		if ( (vl_VitesseAEnvoyer==130) && (!strcmp(vl_Bandeau,"RAPPEL"))) {
		  vl_130_rappel  = XDC_VRAI;
		  if (vl_VitesseCourante==0) {
			vl_VitesseCourante=130;
			strcpy(vl_BandeauCourant,"RAPPEL");
		  }
		  else
		    vl_CommandeAEnvoyer = XDC_VRAI;
		}
		else
			vl_130_rappel = XDC_FAUX;
		

		if (pl_ListeCommandes->commande.prv->etat_de_marche==XDC_VRAI) {
		if (vl_VitesseAEnvoyer != vl_VitesseCourante ||
			strcmp(vl_Bandeau, vl_BandeauCourant) ||
			( (pa_Zone->regulation.mesure == EREG_MESURE_ARRET ) && (pa_Zone->fmc_regulation.numero != -1) )) /*&& pa_Zone->regulation.appli_process== XDC_FAUX))*/
		{
			vl_CommandeAEnvoyer = XDC_VRAI;
		}
		}

		/* test si pilotage Manuel */
		vl_PRV_Manu_Ne_Pas_Modifier = XDC_FAUX;
		if(( vl_Priorite >= EREG_PRIORITE_PRV_MANU) &&
				(vl_Priorite < EREG_PRIORITE_PRV_REGUL))
		{
			if (vl_NumEvt != 0 && vl_CleEvt != 0 &&
					(vl_NumEvt != pa_Zone->fmc_regulation.numero ||	vl_CleEvt != pa_Zone->fmc_regulation.cle))
			{
				/* marquer la zone en pilotage manu  */
				pa_Zone->pilotage_manuel = XDC_VRAI;
				if (pa_Zone->indicateur_forcage == XDC_FAUX){
					/* ne pas envoyer la commande */
					vl_CommandeAEnvoyer = XDC_FAUX;
					vl_PRV_Manu_Ne_Pas_Modifier = XDC_VRAI;
					vl_TransitoireAppliquee = XDC_VRAI; /* a laisser pour mise a jour FMC */
				}
			}
		}
		

		if (vl_CommandeAEnvoyer == XDC_VRAI) {
			/* envoi de commandes prv */
			vl_UneCommandeEnvoyee = XDC_VRAI;
			if (vl_130_rappel==XDC_VRAI)
				/* lot 22*/
				/* appli_cmd(pa_Zone, pl_ListeCommandes->commande, 0, ""); */
				appli_cmd(pa_Zone, pl_ListeCommandes->commande, vl_VitesseAEnvoyer, vl_Bandeau);
			else
				appli_cmd(pa_Zone, pl_ListeCommandes->commande, vl_VitesseAEnvoyer, vl_Bandeau);
		}

		if (vl_PRV_Manu_Ne_Pas_Modifier == XDC_VRAI)
			sprintf(tmp, "%s %3.1f(%dkm/h)", vl_vitessesEnvoyees, vl_PRCible,vl_VitesseCourante );
		else
			sprintf(tmp, "%s %3.1f(%dkm/h)", vl_vitessesEnvoyees,vl_PRCible, vl_VitesseAEnvoyer);

		strcpy(vl_vitessesEnvoyees, tmp);
		vl_VitesseCouranteAmont = vl_VitesseAEnvoyer;
		pl_ListeCommandes = pl_ListeCommandes->suivant;
	} /* Fin Boucle PRV */


//if (vl_TransitoireAppliquee == XDC_VRAI){
	if (vl_UneCommandeEnvoyee == XDC_VRAI) {
		XZST_03EcritureTrace( XZSTC_INFO, ".. Mise a jour de la FMC, Transitoire ou PRV Manuel %s ", vl_vitessesEnvoyees);
		strcpy(pa_Zone->regulation.transitoire, vl_vitessesEnvoyees);
		updateFmcRegulation(pa_Zone,TRUE);

		if (pa_Zone->fmc_regulation.numero != -1) {
			if (trace_process(EREG_TRACE_TYPE_APPLICATION,
						pa_Zone->numero_zone,
						0,
						0,
						pa_Zone->fmc_regulation.numero,pa_Zone->fmc_regulation.cle,
						pa_Zone->regulation_proposee.scenario.id,
						0,
						0,0,
						vl_vitessesEnvoyees) == XDC_NOK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "appli_analyse : Impossible de tracer le process");
			}
		}

		genererReveil(pa_Zone, vl_vitessesEnvoyees);

		XZST_03EcritureTrace( XZSTC_INFO, "  => Vitesses Terrain   = %s", vl_vitessesTerrain);
		XZST_03EcritureTrace( XZSTC_INFO, "  => Vitesses Attendues = %s", vl_vitessesNominales);
		XZST_03EcritureTrace( XZSTC_INFO, "  => Vitesses Envoyees  = %s", vl_vitessesEnvoyees);

	}

	/* marquage de la regulation de la zone comme ayant ete mise a jour sur le terrain*/
	pa_Zone->regulation.appli_process = XDC_VRAI;

	if(XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME ,
						(int *) &vl_jourSemaine,
						(double *) &vl_horodate) != XDC_OK){
			XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de recuperer l horodate courante") ;
	}

	
	XDG_EncodeDG2 ( vl_DG, XDG_REGUL_APPLICATION, vg_NomSite) ;
	if (XZSM_01DemanderProgrammation( vl_horodate+vm_periode,
				0,
				vl_DG,
				XDM_REGUL_APPLICATION,
				vm_cle,
				XZSMC_SEC_VRAI) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL,
				"ERREUR Programmation du timer XDM_REGUL_APPLICATION non faite." );
	}

	XZST_03EcritureTrace( XZSTC_FONCTION, "##### APPLICATION PROCESS TERMINE #####");

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : appli_process");

}

void appli_check_fax_cb()
{
  char vl_ligne[50];
  FILE 	*pl_fp=NULL;
  T_Liste_ZonesRegul      *pl_Zone = NULL;

  /* ping la machine serveur de fax*/
  sprintf(vl_ligne,"%s SP%s %u %u",SG_PING,vg_NomSite,
  	SG_BYTES_PING, SG_COUNT_PING);
   if ((pl_fp = popen(vl_ligne,"r")) != (FILE *)NULL)
   {
      fgets(vl_ligne,sizeof(vl_ligne)-1,pl_fp) ;
      fgets(vl_ligne,sizeof(vl_ligne)-1,pl_fp) ;
      pclose(pl_fp) ;

      /*tout va bien*/
      if ((strstr(vl_ligne,SG_TOKEN_TST) != NULL)) {
	XZST_03EcritureTrace( XZSTC_DEBUG1,"serveur de fax %s OK");
	vm_fax_ok=XDC_OK;
      }
      else {
	XZST_03EcritureTrace( XZSTC_FONCTION,"serveur de fax %s HS");
	if (vm_fax_ok == XDC_NOK) {
	  XZST_03EcritureTrace( XZSTC_WARNING,"2 sequences HS => passer en degrade");
	  pl_Zone = pg_Zones;
	  while(pl_Zone != NULL)
	  {
	  	passer_zone_degrade(&pl_Zone->zone, XDC_VRAI,"Pb SPPR");
		pl_Zone = pl_Zone->suivant;
	  }
	}
      }
   }
}

void appli_process_cb()
/* ------------------------------------------------------
 * ARGUMENTS EN ENTREE :
 *
 * ARGUMENTS EN SORTIE :
 *  aucun
 *
 * CODE RETOUR :
 *  aucun
 *
 * CONDITION D'UTILISATION
 * Compare l'affichage sur le terrain avec
 * 		   l'affichage requis pour chaque zone
 *
 * FONCTION
 *
 ------------------------------------------------------*/
{
	T_Liste_ZonesRegul 	*pl_Zone = NULL;

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : appli_process_cb");

	/*Compare l'affichage sur le terrain avec l'affichage requis pour chaque zone*/
	/*Traitement effectue Zone par Zone*/
	XZST_03EcritureTrace( XZSTC_INFO, "(((((((( APPLICATION PROCESS CB )))))))");

	pl_Zone = pg_Zones;
	while(pl_Zone != NULL)
	{
		appli_process(&(pl_Zone->zone));

		pl_Zone = pl_Zone->suivant;
	}
	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : appli_process_cb");
}

#if 0
void appli_etat_prv_cb(T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_Data, T_CB_ARG pa_ARG)
/* ------------------------------------------------------
 * ARGUMENTS EN ENTREE :
 *  Message RTworks
 *
 * ARGUMENTS EN SORTIE :
 *  aucun
 *
 * CODE RETOUR :
 *  aucun
 *
 * CONDITION D'UTILISATION
 * Recuperation de la vitesse et du texte affiche sur le prv
 * et mise a jour de l'etat du prv pour la zone
 *
 * FONCTION
 *
 ------------------------------------------------------*/
{
	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : appli_etat_prv_cb");

	T_Liste_ZonesRegul *pl_zones = NULL;
	T_Liste_PRV *pl_listePRV = NULL;
	XDY_Etat_PRV vl_prv;
	int vl_res;

	XZST_03EcritureTrace(XZSTC_INFO, "((((((((((( CB ETAT PRV ))))))))))))");

	/*Recuperation des donnees du msg XDM_ETAT_PRV*/
	if(! TipcMsgRead(pa_Data->msg,T_IPC_FT_INT2,&vl_prv.Numero,
								  T_IPC_FT_STR,&vl_prv.Bandeau,
								  T_IPC_FT_STR,&vl_prv.Vitesse,
								  NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "appli_etat_prv_cb : Impossible de recuperer le message");
		return;
	}
	else
	{
		appli_maj_etat_prv(vl_prv, &vl_res);
	}
	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : appli_etat_prv_cb");
}
#endif



void appli_maj_etat_prv(XDY_Etat_PRV va_etat_prv, int *va_cr)
{
	T_Liste_ZonesRegul *pl_zones = NULL;
	T_Liste_PRV *pl_listePRV = NULL;

	XZST_03EcritureTrace(XZSTC_INFO, "prv = %d, vitesse=%s, texte affiche=%s",
			va_etat_prv.Numero,
			va_etat_prv.Vitesse, va_etat_prv.Bandeau);
	pl_zones = pg_Zones;
	//Recherche de la zone du PRV

	while(pl_zones != NULL)
	{
		pl_listePRV = pl_zones->zone.ListePRV;
		while(pl_listePRV != NULL)
		{
			if(pl_listePRV->prv.numero_prv == va_etat_prv.Numero)
			{
				pl_listePRV->prv.vitesse_courante = vitesseTextToVitesse(va_etat_prv.Vitesse);
				strcpy(pl_listePRV->prv.bandeau , trim(va_etat_prv.Bandeau));
				strcpy(pl_listePRV->prv.vitesse_texte , va_etat_prv.Vitesse);

				XZST_03EcritureTrace(XZSTC_WARNING,"mise a jour de l'etat du prv : numeroPRV=%d, Bandeau=%s, vitesse=%d",
						va_etat_prv.Numero,
						pl_listePRV->prv.bandeau,
						pl_listePRV->prv.vitesse_courante);
				*va_cr = XDC_OK;
				return;
			}
			pl_listePRV = pl_listePRV->suivant;
		}
		pl_zones = pl_zones->suivant;
	}
	*va_cr= XDC_NOK;
}



void appli_etat_dispo_prv_cb(T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_Data, T_CB_ARG pa_ARG)
/* ------------------------------------------------------
 * ARGUMENTS EN ENTREE :
 *  Message RTworks
 *
 * ARGUMENTS EN SORTIE :
 *  aucun
 *
 * CODE RETOUR :
 *  aucun
 *
 * CONDITION D'UTILISATION
 * Recuperation du type, du numero et de l'etat du prv
 * et mise a jour de l'etat du prv pour la zone
 *
 * FONCTION
 *
 ------------------------------------------------------*/
{
	XZST_03EcritureTrace( XZSTC_INTERFACE, "IN : appli_etat_dispo_prv_cb");

	//XDY_Etat_PRV vl_prv;
#ifndef _TIBCO_EMS
	T_INT2 vl_type, vl_num;
	T_INT4 vl_etat_eqt;
#else
	XDY_Mot vl_type, vl_num;
	XDY_Entier vl_etat_eqt;
#endif
	T_Liste_ZonesRegul *tmpZone = NULL;
	T_Liste_PRV *pl_prv= NULL;

	/*Recuperation des donnees du msg XDM_IETAT_DISPO_EQT*/
	if(! TipcMsgRead(pa_Data->msg,T_IPC_FT_INT2,&vl_type,
								  T_IPC_FT_INT2,&vl_num,
								  T_IPC_FT_INT4,&vl_etat_eqt,NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "appli_etat_dispo_prv_cb : Impossible de recuperer le message");
		return;
	}
	else
	{
		if (vl_type!=XDC_EQT_PRV)
			return;

		XZST_03EcritureTrace(XZSTC_INFO," numero=%d, type=%d, etat=%d",	vl_num, vl_type, vl_etat_eqt);
		tmpZone = pg_Zones;
		while(tmpZone != NULL)
		{
			pl_prv = tmpZone->zone.ListePRV;
			while(pl_prv != NULL)
			{

				if (pl_prv->prv.numero_prv == vl_num)
				{
					if ((vl_etat_eqt & ( XDC_EQT_MAJEUR
										| XDC_EQT_CRITIQUE
										| XDC_EQT_DESACTIVE
										| XDC_EQT_HS
										| XDC_EQT_INHIBE))== 0 )
						pl_prv->prv.etat_de_marche = XDC_VRAI;
					else
						pl_prv->prv.etat_de_marche = XDC_FAUX;

					XZST_03EcritureTrace(XZSTC_INFO,"mise a jour de l'etat du prv : numeroPRV=%d etat=%d", vl_num,pl_prv->prv.etat_de_marche );
					break;
				}
				pl_prv = pl_prv->suivant;
			}
			tmpZone = tmpZone->suivant;
		}
	}

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : appli_etat_dispo_prv_cb");
}


XDY_Entier vitesseTextToVitesse(XDY_VitesseRegulTxt v_vitesseText)
{
	XDY_Entier vl_vitesse=0;

	if(!strncmp(v_vitesseText , "L50",3))
		vl_vitesse = 50;
	else if(!strncmp(v_vitesseText , "L70",3))
		vl_vitesse = 70;
	else if(!strncmp(v_vitesseText , "L90",3))
		vl_vitesse = 90;
	else if(!strncmp(v_vitesseText , "L110",4))
		vl_vitesse = 110;
	else if(!strncmp(v_vitesseText , "L130",4))
		vl_vitesse = 130;

	return vl_vitesse;
}

void vitesseToVitesseText(XDY_Entier va_vitesseIn, XDY_VitesseRegulTxt va_vitesseOut)
{
	XDY_VitesseRegulTxt vl_vitesseText="";

	if(va_vitesseIn == 50)
		strcpy(va_vitesseOut, "L50");
	else if(va_vitesseIn == 70)
		strcpy(va_vitesseOut , "L70");
	else if(va_vitesseIn == 90)
		strcpy(va_vitesseOut , "L90");
	else if(va_vitesseIn == 110)
		strcpy(va_vitesseOut , "L110");
	else if(va_vitesseIn == 130)
		strcpy(va_vitesseOut , "L130");
	else
		strcpy(va_vitesseOut,"L0");
}


char *trim ( char *s )
{
  int i = 0;
  int j = strlen ( s ) - 1;
  int k = 0;

while ( isspace ( s[i] ) && s[i] != '\0' )
    i++;

      while ( isspace ( s[j] ) && j >= 0 )
  j--;

    while ( i <= j )
	s[k++] = s[i++];

  s[k] = '\0';

    return s;
}
