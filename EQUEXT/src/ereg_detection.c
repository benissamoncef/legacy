/*E*/
/* Fichier : $Id: ereg_detection.c,v 1.9 2017/03/31 17:35:36 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2017/03/31 17:35:36 $
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TEREG * FICHIER ereg_detection.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * < Module de general de detection de la regulation >
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * SDU		13/03/2012 : Creation DEM/1016
* JMG		23/07/12 : correction conditions arret 1.2
 * LCL		03/08/12 : correction detection donnees invalide 1.4
* MAX		17/08/12 : correction lissage Q 1.5
* JMG		12/04/13 : lisage pente 1.6
* JMG		15/11/14 : linux 
* LCL		27/09/23 : Adaptations CNA DEM-483	
------------------------------------------------------*/

/* fichiers inclus */
#include "ereg_detection.h"
#include "ereg_cfg.h"
#include "ereg_util.h"
#include "xzit.h"

/* definitions de constantes */
#define CLE_TIMER_DETECTION		"cle_timer_detection"

#define EREG_NB_DATA_STATION_CYCLE_Q_PENTE	4
/*
#define DET_NIV_TRACE_1		XZSTC_INFO
#define DET_NIV_TRACE_2 	XZSTC_INFO
 */
int DET_NIV_TRACE_1;
int DET_NIV_TRACE_2;

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables globales */
XDY_Entier		vg_InitDetection = XDC_NOK;
XDY_Horodate 	vg_Horodate_Cycle;
XDY_Horodate 	vg_Horodate_Msg[10];
XDY_Entier		vg_JourSemaine_Cycle;
XDY_Entier		vg_idScenarioDefaut;
char			vg_donnee[10];

/* declaration de variables externes */
extern XDY_NomSite              vg_NomSite;
extern XDY_Booleen 			    vg_ModeSimu;
extern T_Liste_ZonesRegul 	    *pg_Zones;
extern XDY_Basedd				vg_Base;
extern XDY_Booleen				vg_SimuEnCours;
extern XDY_Booleen				vg_tebeg;
extern T_Param 					tg_params[EREG_NB_PARAM_MAX];
extern XDY_NomMachine 			vg_NomMachine;
extern XDY_Horodate				vg_HorodateSimu;
extern XDY_Horodate				vg_horodate_debut_simu;
extern XDY_Horodate				vg_horodate_fin_simu;
extern XDY_NomMachine		vg_machineSimu;

/* declaration de fonctions internes */
void det_ideal_cb(T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_Data, T_CB_ARG pa_ARG);
void det_simu_trafic_cb(T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_Data, T_CB_ARG pa_ARG);
void det_acquisition_cb(T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_Data, T_CB_ARG pa_ARG);
void null_cb(T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_Data, T_CB_ARG pa_ARG);
XDY_Entier verifier_conditions(T_Liste_Condition_Scenario *pa_listeConditions, XDY_Horodate va_horodate, XDY_Entier va_type_condition, XDY_Entier *pa_nbOk, XDY_Entier *pa_nbNOk, T_SCENARIO va_scenario);


/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  initialisation
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */
XDY_Entier det_init(char *va_scenarioDefaut){
	/*
	 * ARGUMENTS EN ENTREE :
	 *  Aucun
	 *
	 * ARGUMENTS EN SORTIE :
	 *  Aucun
	 *
	 * CODE RETOUR :
	 *  XDC_OK
	 *  XDC_NOK
	 *
	 * CONDITION D'UTILISATION
	 *  Doit etre invoque pour l'init du module
	 *
	 * FONCTION
	 *
------------------------------------------------------*/
	XDY_Entier		vl_indice_callback, vl_JourSemaine_Msg;
	XDY_Datagroup	vl_nomDG, vl_DG_REGUL_DETECTION;
	XDY_Horodate	vl_horodate;

	XZST_03EcritureTrace( XZSTC_WARNING, "JBE JE RENTRE DANS :: det_init :: ");
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : det_init");

	if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
			(int *) &vl_JourSemaine_Msg, (double *) &vl_horodate ) != XDC_OK )
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de recuperer l horodate courante");
	}
	vg_Horodate_Msg[0]=vl_horodate;
	vg_Horodate_Msg[1]=vl_horodate;
	vg_Horodate_Msg[2]=vl_horodate;
	vg_Horodate_Msg[3]=vl_horodate;
	vg_Horodate_Msg[4]=vl_horodate;
	vg_Horodate_Msg[5]=vl_horodate;
	vg_Horodate_Msg[6]=vl_horodate;

	/* Abonnement aux Datagroup */
	XDG_EncodeDG2 ( vl_nomDG, XDG_REGUL_DETECTION, vg_NomSite) ;
	strcpy(vl_DG_REGUL_DETECTION, vl_nomDG);
	XZST_03EcritureTrace(XZSTC_INFO, "Abonnement au Datagroup %s", vl_nomDG );
	if ( !XDG_JoindreDG( vl_nomDG ) )
	{
		XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", vl_nomDG );
		return XDC_NOK;
	}

	XZST_03EcritureTrace(DET_NIV_TRACE_1, "Armement CB det_acquisition_cb() sur XDM_EMES_6mn_Chaussee");
	if(TipcSrvProcessCbCreate(	XDM_IdentMsg(XDM_EMES_6mn_Chaussee ) ,
			det_acquisition_cb ,
			(T_CB_ARG)NULL) == NULL )
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "Erreur Armement Callback sur XDM_EMES_6mn_Chaussee.");
		return XDC_NOK;
	}

	if(TipcSrvProcessCbCreate(      XDM_IdentMsg(XDM_EMES_6mn_Voie),
			null_cb,
			(T_CB_ARG)NULL) == NULL )
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "Erreur Armement Callback sur XDM_EMES_6mn_Chaussee.");
		return XDC_NOK;
	}

	if (vg_ModeSimu == XDC_FAUX)
	{
		/* MODE TEMPS REEL */

		/* Abonnement au suject XDG_EMES_RADT_6MN<Secteur> */
		XDG_EncodeDG2 ( vl_nomDG, XDG_EMES_RADT_6MN, vg_NomSite) ;
		XZST_03EcritureTrace(DET_NIV_TRACE_1, "Abonnement au Datagroup %s", vl_nomDG );
		if ( !XDG_JoindreDG( vl_nomDG ) )
		{
			XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", vl_nomDG );
			return XDC_NOK;
		}
		/*si PR s'abonner a VE*/
		if (!strcmp(vg_NomSite, XDC_NOM_SITE_DP)) {
			XDG_EncodeDG2(vl_nomDG, XDG_EMES_RADT_6MN, XDC_NOM_SITE_VC);
			XDG_JoindreDG( vl_nomDG );
		}

		XZST_03EcritureTrace(XZSTC_WARNING, "Programmation TIMER pour Envoi Message XDM_REGUL_DETECTION toutes les %d minutes", getParamValue(EREG_PARAM_AJU_REG_FREQ_FONCT_MODU_AJUST));
		if(XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME ,
				(int *) &vg_JourSemaine_Cycle,
				(double *) &vl_horodate) != XDC_OK){
			XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de recuperer l horodate courante");
			return XDC_NOK;
		}
		if (XZSM_01DemanderProgrammation(
				vl_horodate+getParamValue(EREG_PARAM_AJU_REG_FREQ_FONCT_MODU_AJUST)*60,
				getParamValue(EREG_PARAM_AJU_REG_FREQ_FONCT_MODU_AJUST)*60,
				vl_DG_REGUL_DETECTION,
				XDM_REGUL_DETECTION,
				CLE_TIMER_DETECTION,
				XZSMC_SEC_VRAI) != XDC_OK)
		{
			XZST_03EcritureTrace( XZSTC_FATAL, "Erreur Programmation du timer XDM_REGUL_DETECTION non faite." );
			return XDC_NOK;
		}
	}

	/* Armement du timer (det_ideal_cb) sur XDG_REGUL_DETECTION pour le message XDM_REGUL_DETECTION */
	XZST_03EcritureTrace(XZSTC_WARNING, "JBE JE VAIS FAIRE :: le if pour det_ideal_cb ::");
	XZST_03EcritureTrace(DET_NIV_TRACE_1, "Armement CB det_ideal_cb() sur reception de XDM_REGUL_DETECTION");
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_REGUL_DETECTION),
			det_ideal_cb,
			(T_CB_ARG)NULL) == NULL )
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "Erreur Armement Callback sur XDM_REGUL_DETECTION." );
		return XDC_NOK;
	}

	if (vg_tebeg == XDC_VRAI){
		DET_NIV_TRACE_1 = XZSTC_INFO;
		DET_NIV_TRACE_2 = XZSTC_INFO;
	}else
	{
		DET_NIV_TRACE_1 = XZSTC_INFO;
		DET_NIV_TRACE_2 = XZSTC_MESSAGE;
	}

	vg_idScenarioDefaut = -1;
	if (va_scenarioDefaut != NULL){
		vg_idScenarioDefaut = 0;
		sscanf(va_scenarioDefaut, "%d", &vg_idScenarioDefaut);
		XZST_03EcritureTrace( XZSTC_INFO, "Initialisation d'un scenario par defaut : \"%s\" = %d", va_scenarioDefaut, vg_idScenarioDefaut);
	}else
		XZST_03EcritureTrace( XZSTC_INFO, "AUCUNE Initialisation d'un scenario par defaut");

	vg_InitDetection = XDC_OK;

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : det_init");
	return XDC_OK;
}

void null_cb( T_IPC_CONN      va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
		T_CB_ARG                        pa_ARG)
{
}

void det_acquisition_cb(	T_IPC_CONN 	va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
		T_CB_ARG                        pa_ARG)
{
	XDY_Entier			vl_indice = 0;
	T_Liste_ZonesRegul 	*pl_zone = NULL;
	T_Liste_Station 	*pl_station = NULL;

	XDY_Horodate 	vl_horodate, vl_horodate_courante;
	int 			vl_JourSemaine_courant;
	XDY_Mot			vl_Numero;
	XDY_Mot		vl_Periode;
	XDY_Mot		vl_Sens;
	XDY_Mot		vl_Voie;
	XDY_Mot		vl_Mesure_Q;
	XDY_Mot		vl_Mesure_V;
	XDY_Mot		vl_Mesure_TO;
	XDY_Mot		vl_Mesure_PL;
	XDY_Mot		vl_Validite_Q;
	XDY_Mot		vl_Validite_V;
	XDY_Mot		vl_Validite_TO;
	XDY_Mot		vl_Validite_PL;
	XDY_Entier		vl_DernierePosition;
	XDY_Entier		vl_DernierePositionPente;
	XDY_Entier		vl_tmpPos,vl_tmpPosLissee;
	XDY_Entier		vl_cumul_Q;
	XDY_Entier		vl_cumul_TO;
	XDY_Entier		vl_donneesPente[4];
	XDY_Entier		vl_donneesPenteLissee[EREG_NB_DATA_STATION_CYCLE_PENTE];
	XDY_Booleen     vl_donneesPenteValides;
	XDY_Booleen     vl_donneesPenteLisseeValides;
	double          vl_pente;
	double          vl_penteLissee;
	double vl_QLissee = 0.0;
	double vl_TOLissee = 0.0;


	int i = 0, cptQ = 0, cptTO = 0;
	int cpt_pente = 0;
	XZST_03EcritureTrace( XZSTC_DEBUG2, "IN : det_acquisition_cb");
	//lecture donnees trafic
	if(! TipcMsgRead(pa_Data->msg,T_IPC_FT_REAL8,&vl_horodate,
			T_IPC_FT_INT2,&vl_Numero,
			T_IPC_FT_INT2,&vl_Periode,
			T_IPC_FT_INT2,&vl_Sens,
			T_IPC_FT_INT2,&vl_Voie,
			T_IPC_FT_INT2,&vl_Mesure_Q,
			T_IPC_FT_INT2,&vl_Validite_Q,
			T_IPC_FT_INT2,&vl_Mesure_V,
			T_IPC_FT_INT2,&vl_Validite_V,
			T_IPC_FT_INT2,&vl_Mesure_TO,
			T_IPC_FT_INT2,&vl_Validite_TO,
			T_IPC_FT_INT2,&vl_Mesure_PL,
			T_IPC_FT_INT2,&vl_Validite_PL
			,NULL))
	{
		XZST_03EcritureTrace(DET_NIV_TRACE_1, "det_acquisition_cb (mode temps reel): Impossible de recuperer le message");
		return;
	}
	else
	{
		/*XZST_03EcritureTrace(DET_NIV_TRACE_2," - - - Donnees recues (mode Temps Reel): station=%d, voie=%d, sens=%d, Q=%d(validite=%d), TO=%d(validite=%d)",*/
			if ( (vl_Numero==75) )
			XZST_03EcritureTrace(XZSTC_WARNING," - - - Donnees recues : station=%d, voie=%d, sens=%d, Q=%d(validite=%d), TO=%d(validite=%d) H=%lf P=%d",
					vl_Numero,
					vl_Voie,
					vl_Sens,
					vl_Mesure_Q,
					vl_Validite_Q,
					vl_Mesure_TO,
					vl_Validite_TO,vl_horodate,vl_Periode);

		if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
				(int *) &vl_JourSemaine_courant, (double *) &vl_horodate_courante ) != XDC_OK )
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de recuperer l horodate courante");
		}
		pl_zone = pg_Zones;

		/* On recherche la station correspondante*/
		while(pl_zone != NULL)
		{
			pl_station = pl_zone->zone.ListeStation ;

			/* verification que la zone ne soit pas dégradée
			 * si oui : on n'effectue aucun traitement des données trafic  */
			/*
			if(pl_zone->zone.degrade)
				break;
			else{*/

				while(pl_station != NULL)
				{

					if ( (pl_station->station.station == vl_Numero) &&
							(pl_station->station.sens == vl_Sens) )
					{
						if (vl_Validite_Q != XDC_RADT_INCONNU)
							XZST_03EcritureTrace(DET_NIV_TRACE_2," + + + + TRAITEMENT POUR STATION %d (sens %d) + + + +", pl_station->station.station, pl_station->station.sens);
						else
							XZST_03EcritureTrace(DET_NIV_TRACE_2," (#) DONNEE RECUE NON VALIDE");

						/* on verifie que la donnee n a pas deja ete recue*/
						if (pl_station->station.horodateDerniereStationRecue==vl_horodate) {
							XZST_03EcritureTrace(XZSTC_INFO,"donnee deja recue");
							break;
						}
						vg_Horodate_Msg[pl_zone->zone.numero_zone] = vl_horodate_courante;

						pl_station->station.horodateDerniereStationRecue = vl_horodate;
						/* Station trouvee */
						vl_DernierePosition = pl_station->station.positionDerniereStationRecue;
						vl_DernierePositionPente = pl_station->station.positionDerniereStationRecuePente;
						vl_DernierePosition ++;
						vl_DernierePositionPente ++;
						if (vl_DernierePosition == EREG_NB_DATA_STATION_CYCLE_Q) /*v1.5*/
							vl_DernierePosition = 0;
						if (vl_DernierePositionPente == EREG_NB_DATA_STATION_CYCLE_PENTE)
							vl_DernierePositionPente = 0;

						if ( (vl_Numero==75) )
						  XZST_03EcritureTrace(XZSTC_WARNING," (#) Memorisation des donnees pente en rang [%d]", vl_DernierePositionPente);
						/* Memorisation des valeurs Q et TO
						 * avec flag indiquant si valeur valide ou non
						 * */

						if(vl_Validite_Q == XDC_RADT_INCONNU)
						{
							pl_station->station.data[vl_DernierePosition].Q_valide = XDC_FAUX;
							pl_station->station.data[vl_DernierePosition].Q = 0;
						}
						else{
							pl_station->station.data[vl_DernierePosition].Q_valide = XDC_VRAI;
							pl_station->station.data[vl_DernierePosition].Q = vl_Mesure_Q;
						}

						if(vl_Validite_TO == XDC_RADT_INCONNU)
						{
							pl_station->station.data[vl_DernierePosition].TO_valide = XDC_FAUX;
							pl_station->station.data[vl_DernierePosition].TO = 0;
						}
						else{
							pl_station->station.data[vl_DernierePosition].TO_valide = XDC_VRAI;
							pl_station->station.data[vl_DernierePosition].TO = vl_Mesure_TO;
						}

						pl_station->station.data_pente[vl_DernierePositionPente] = 
								pl_station->station.data[vl_DernierePosition];

						if (vl_Validite_Q != XDC_RADT_INCONNU && vl_Validite_TO != XDC_RADT_INCONNU)
							XZST_03EcritureTrace(DET_NIV_TRACE_2," - - det_acquisition_cb (mode temps reel) : donnees traitees : station : %d \t "
									"Q  : (%s) = %d \t "
									"TO : (%s) = %d ",
									pl_station->station.station,
									pl_station->station.data[vl_DernierePosition].Q_valide==XDC_VRAI ? "VALIDE" : "INVALIDE",
											pl_station->station.data[vl_DernierePosition].Q,
											pl_station->station.data[vl_DernierePosition].TO_valide==XDC_VRAI ? "VALIDE" : "INVALIDE",
													pl_station->station.data[vl_DernierePosition].TO);
						else{
							XZST_03EcritureTrace(DET_NIV_TRACE_2," - - det_acquisition_cb (mode temps reel) : donnees traitees : inconnues ");
						}

						/* Comptage du nombre de valeurs valides sur les 5 derniers cycles pour Q*/
						cptQ = 0;
						cpt_pente =0;
						vl_cumul_Q = 0;
						vl_tmpPos = vl_DernierePosition;
						vl_tmpPosLissee = vl_DernierePositionPente;

						XZST_03EcritureTrace(DET_NIV_TRACE_2," [+] parcours des donnees Q: ");

						vl_donneesPenteValides = XDC_VRAI;
						vl_donneesPenteLisseeValides = XDC_VRAI;

						for(i=0; i <EREG_NB_DATA_STATION_CYCLE_PENTE; i++) {
							if (pl_station->station.data_pente[vl_tmpPosLissee].Q_valide == XDC_FAUX) {
								vl_donneesPenteLissee[i] = -1;
								vl_donneesPenteLisseeValides = XDC_FAUX;
								XZST_03EcritureTrace(DET_NIV_TRACE_2," donne Q recu invalide ");
							}
							else {
								vl_donneesPenteLissee[i] = pl_station->station.data_pente[vl_tmpPosLissee].Q;
							}
							vl_tmpPosLissee --;
							if (vl_tmpPosLissee< 0)
								vl_tmpPosLissee= EREG_NB_DATA_STATION_CYCLE_PENTE - 1;
						}

						if (vl_Numero==75) {
							for (i=0; i < 8; i++)
							  XZST_03EcritureTrace(XZSTC_WARNING,"vl_donneesPenteLissee[%d] = %d",
										i,vl_donneesPenteLissee[i]);
						}

						/*QL1 = moyenne Q1 a Q5*/
						pl_station->station.QL1 = (vl_donneesPenteLissee[0] > -1 ? vl_donneesPenteLissee[0] : 0) +
								(vl_donneesPenteLissee[1] > -1 ? vl_donneesPenteLissee[1] : 0) +
								(vl_donneesPenteLissee[2] > -1 ? vl_donneesPenteLissee[2] : 0) +
								(vl_donneesPenteLissee[3] > -1 ? vl_donneesPenteLissee[3] : 0) +
								(vl_donneesPenteLissee[4] > -1 ? vl_donneesPenteLissee[4] : 0) ;
						cptQ = (vl_donneesPenteLissee[0] > -1 ? 1 : 0) +
							(vl_donneesPenteLissee[1] > -1 ? 1 : 0) +
							(vl_donneesPenteLissee[2] > -1 ? 1 : 0) +
							(vl_donneesPenteLissee[3] > -1 ? 1 : 0) +
							(vl_donneesPenteLissee[4] > -1 ? 1 : 0);
						if (cptQ > 0)
							pl_station->station.QL1 = pl_station->station.QL1 / cptQ;
						else
							pl_station->station.QL1 = -1;


						/*QL2 = moyenne Q2 a Q6*/
						
						pl_station->station.QL2 = (vl_donneesPenteLissee[1] > -1 ? vl_donneesPenteLissee[1] : 0)	 +
								(vl_donneesPenteLissee[2] > -1 ? vl_donneesPenteLissee[2] : 0) +
								(vl_donneesPenteLissee[3] > -1 ? vl_donneesPenteLissee[3] : 0) +
								(vl_donneesPenteLissee[4] > -1 ? vl_donneesPenteLissee[4] : 0) +
								(vl_donneesPenteLissee[5] > -1 ? vl_donneesPenteLissee[5] : 0) ;
						cptQ = (vl_donneesPenteLissee[1] > -1 ? 1 : 0) +
							(vl_donneesPenteLissee[2] > -1 ? 1 : 0) +
							(vl_donneesPenteLissee[3] > -1 ? 1 : 0) +
							(vl_donneesPenteLissee[4] > -1 ? 1 : 0) +
							(vl_donneesPenteLissee[5] > -1 ? 1 : 0);
						if (cptQ > 0)
							pl_station->station.QL2 = pl_station->station.QL2 / cptQ;
						else
							pl_station->station.QL2 = -1;

						/*QL3 = moyenne Q3 a Q7*/
						pl_station->station.QL3 = (vl_donneesPenteLissee[2] > -1 ? vl_donneesPenteLissee[2] : 0)	 +
								(vl_donneesPenteLissee[3] > -1 ? vl_donneesPenteLissee[3] : 0) +
								(vl_donneesPenteLissee[4] > -1 ? vl_donneesPenteLissee[4] : 0) +
								(vl_donneesPenteLissee[5] > -1 ? vl_donneesPenteLissee[5] : 0) +
								(vl_donneesPenteLissee[6] > -1 ? vl_donneesPenteLissee[6] : 0) ;
						cptQ = (vl_donneesPenteLissee[2] > -1 ? 1 : 0) +
							(vl_donneesPenteLissee[3] > -1 ? 1 : 0) +
							(vl_donneesPenteLissee[4] > -1 ? 1 : 0) +
							(vl_donneesPenteLissee[5] > -1 ? 1 : 0) +
							(vl_donneesPenteLissee[6] > -1 ? 1 : 0);
						if (cptQ > 0)
							pl_station->station.QL3 = pl_station->station.QL3 / cptQ;
						else
							pl_station->station.QL3 = -1;


						/*QL4 = moyenne Q4 a Q8*/
						pl_station->station.QL4 = (vl_donneesPenteLissee[3] > -1 ? vl_donneesPenteLissee[3] : 0)	 +
								(vl_donneesPenteLissee[4] > -1 ? vl_donneesPenteLissee[4] : 0) +
								(vl_donneesPenteLissee[5] > -1 ? vl_donneesPenteLissee[5] : 0) +
								(vl_donneesPenteLissee[6] > -1 ? vl_donneesPenteLissee[6] : 0) +
								(vl_donneesPenteLissee[7] > -1 ? vl_donneesPenteLissee[7] : 0) ;
						cptQ = (vl_donneesPenteLissee[3] > -1 ? 1 : 0) +
							(vl_donneesPenteLissee[4] > -1 ? 1 : 0) +
							(vl_donneesPenteLissee[5] > -1 ? 1 : 0) +
							(vl_donneesPenteLissee[6] > -1 ? 1 : 0) +
							(vl_donneesPenteLissee[7] > -1 ? 1 : 0);
						if (cptQ > 0)
							pl_station->station.QL4 = pl_station->station.QL4 / cptQ;
						else
							pl_station->station.QL4 = -1;
						
						if (vl_Numero==75) {
							XZST_03EcritureTrace(XZSTC_WARNING,"QL1 = %f, QL2 = %f, QL3 = %f, QL4=%f",
								pl_station->station.QL1,
								pl_station->station.QL2,
								pl_station->station.QL3,
								pl_station->station.QL4);
						}

						i = (pl_station->station.QL1 > 0 ? 1 : 0) +
							(pl_station->station.QL2> 0 ? 1 : 0) +
							(pl_station->station.QL3> 0 ? 1 : 0) +
							(pl_station->station.QL4> 0 ? 1 : 0) ;
						if (i==4) /* si les 4 QLx sont valides alors la pente est valide*/
						/*if (i>0)*/
						  vl_donneesPenteLisseeValides = XDC_VRAI;
						else
						  vl_donneesPenteLisseeValides= XDC_FAUX;
						
						cptQ=0;
						for(i=0; i < EREG_NB_DATA_STATION_CYCLE_Q; i++)
						{
							/* Qualification des donnees pour le calcul de la pente*/
							if (i < EREG_NB_DATA_CALCUL_PENTE)
							{
								if (pl_station->station.data[vl_tmpPos].Q_valide == XDC_FAUX)
								{
									vl_donneesPenteValides = XDC_FAUX;

									XZST_03EcritureTrace(DET_NIV_TRACE_2," donne Q recu invalide ");
								}
								else
								{
									vl_donneesPente[i] = pl_station->station.data[vl_tmpPos].Q;
								}
							}

							if(pl_station->station.data[vl_tmpPos].Q_valide == XDC_VRAI)
							{
								cptQ ++;
								vl_cumul_Q += pl_station->station.data[vl_tmpPos].Q;

								pl_station->station.data_lisseeQ = ((double) vl_cumul_Q) / ((double) cptQ);
								pl_station->station.data_lisseeQ_valide = XDC_VRAI;

								if ( (vl_Numero==75))
								XZST_03EcritureTrace(XZSTC_WARNING," - - - Lissage des donnees Q : cpt=%d, cumul=%d, lissage=%f",
										cptQ,
										vl_cumul_Q,
										pl_station->station.data_lisseeQ);

								XZST_03EcritureTrace(DET_NIV_TRACE_2," donne Q recu valide : %d (indice pente %d)",
										vl_donneesPente[i],
										i);
							}
							else
							{
								XZST_03EcritureTrace(DET_NIV_TRACE_2," DONNEE Q INVALIDE");
							}

							if ( (vl_Numero==75) )
							XZST_03EcritureTrace(XZSTC_INFO," + calcul Qlisse + indicePosition=%d / Q(%s)=%d / cpt=%d / cumul=%d",
									vl_tmpPos,
									pl_station->station.data[vl_tmpPos].Q_valide==XDC_VRAI ? "VALIDE" : "INVALIDE",
											pl_station->station.data[vl_tmpPos].Q,
											cptQ,
											vl_cumul_Q);

							vl_tmpPos --;
							if (vl_tmpPos < 0)
								vl_tmpPos = EREG_NB_DATA_STATION_MAX - 1;
						}

                                                /* Aucune donnee valide pour calculer le Q lisse : donnee lissee invalide */
                                                if ( cptQ == 0 )
                                                {
                                                        //on incrémente le compteur pour indiquer que cela fait trop de cycle que les donnees recus sont invalides
                                                        pl_station->station.data_lisseeQ = 0;
                                                        pl_station->station.data_lisseeQ_valide = XDC_FAUX;

							if ( (vl_Numero==75) )
                                                        XZST_03EcritureTrace(XZSTC_INFO,"#### Station %d : Donnee lissee Q invalide ####",
                                                                        pl_station->station.station );

                                                        pl_station->station.comptage_panne = XDC_VRAI;
                                                }

						/* Comptage du nombre de valeurs valides sur les 3 derniers cycles pour TO*/
						cptTO = 0;
						vl_cumul_TO = 0;
						vl_tmpPos = vl_DernierePosition;
						XZST_03EcritureTrace(DET_NIV_TRACE_2," [+] parcours des donnees TO: ");
						for(i=0; i < EREG_NB_DATA_STATION_CYCLE_TO; i++)
						{
							if(pl_station->station.data[vl_tmpPos].TO_valide == XDC_VRAI)
							{
								cptTO ++;
								vl_cumul_TO += pl_station->station.data[vl_tmpPos].TO;

								pl_station->station.data_lisseeTO = ((double) vl_cumul_TO) / ((double) cptTO);
								pl_station->station.data_lisseeTO_valide = XDC_VRAI;

								if ( (vl_Numero==75) )
								XZST_03EcritureTrace(XZSTC_INFO, "- - - Lissage des donnees TO : cpt=%d, cumul=%d, lissage=%f",
										cptTO,
										vl_cumul_TO,
										pl_station->station.data_lisseeTO);
							}
							if ( (vl_Numero==75) )
							XZST_03EcritureTrace(XZSTC_INFO," + calcul TOlisse + indicePosition=%d / TO(%s)=%d / cpt=%d / cumul=%d",
									vl_tmpPos,
									pl_station->station.data[vl_tmpPos].TO_valide==XDC_VRAI ? "VALIDE" : "INVALIDE",
											pl_station->station.data[vl_tmpPos].TO,
											cptTO,
											vl_cumul_TO);

							vl_tmpPos --;
							if (vl_tmpPos < 0)
								vl_tmpPos = EREG_NB_DATA_STATION_MAX - 1;
						}

                                                /* Aucune donnee valide pour calculer le TO lisse : donnee lissee invalide */
                                                if ( cptTO == 0 )
                                                {
                                                        pl_station->station.data_lisseeTO = 0;
                                                        pl_station->station.data_lisseeTO_valide = XDC_FAUX;

							if ( (vl_Numero==75) )
                                                        XZST_03EcritureTrace(XZSTC_INFO,"#### Station %d : Donnee lissee TO invalide ####",
                                                                        pl_station->station.station );

                                                        pl_station->station.comptage_panne = XDC_VRAI;
                                                }

                                                /* Si ni donnee lissee Q valide, ni donnee lissee TO valide, alors la station est en panne */
                                                if((pl_station->station.data_lisseeQ_valide == XDC_FAUX) && (pl_station->station.data_lisseeTO_valide == XDC_FAUX))
                                                {
                                                        pl_station->station.comptage_panne = XDC_VRAI;
							if ( (vl_Numero==75) )
                                                        XZST_03EcritureTrace(XZSTC_WARNING,"#*#*# station : %d (EN PANNE)",
                                                                        pl_station->station.station);
                                                }
                                                else
                                                        pl_station->station.comptage_panne = XDC_FAUX;

						if ( (vl_Numero==75) )
						XZST_03EcritureTrace(XZSTC_INFO," - - -moyenne TO : vl_cumul_TO=%d, cptTO=%d", vl_cumul_TO, cptTO);

						// Calcul de la pente du debit
						vl_pente = 0.0;
						vl_penteLissee = 0.0;

						if (vl_donneesPenteLisseeValides == XDC_VRAI) {
							vl_penteLissee = ((float)(tg_params[XDC_REG_COEF_CALCUL_PENTE].Valeur)/100  
										* (pl_station->station.QL1 - pl_station->station.QL4)) +
									((float)(tg_params[XDC_REG_COEF_CALCUL_PENTE_FIN].Valeur)/100 * 
											(pl_station->station.QL2 - pl_station->station.QL3));

							pl_station->station.pente = (XDY_Entier) vl_penteLissee;
							pl_station->station.pente_valide = XDC_VRAI;

								if ( (vl_Numero==75) )
							XZST_03EcritureTrace(XZSTC_WARNING," + + Calcul de la Pente Lissee + + (%d * (%f - %f)) + (%d * (%f - %f )) = %d",
									tg_params[XDC_REG_COEF_CALCUL_PENTE].Valeur,
									pl_station->station.QL1,
									pl_station->station.QL4,
									tg_params[XDC_REG_COEF_CALCUL_PENTE_FIN].Valeur,
									pl_station->station.QL2,
									pl_station->station.QL3,
									pl_station->station.pente);
						}
						else {
							if ( (vl_Numero==75) )
							XZST_03EcritureTrace(XZSTC_WARNING," + + Calcul de la Pente lissee IMPOSSIBLE + + : (%d * (%f - %f)) + (%d * (%f - %f ))",
									tg_params[XDC_REG_COEF_CALCUL_PENTE].Valeur,
									pl_station->station.QL1,
									pl_station->station.QL4,
									tg_params[XDC_REG_COEF_CALCUL_PENTE_FIN].Valeur,
									pl_station->station.QL2,
									pl_station->station.QL3);
							pl_station->station.pente_valide = XDC_FAUX;

						}

						if (vl_donneesPenteValides == XDC_VRAI){
							vl_pente = ((float)(tg_params[XDC_REG_COEF_CALCUL_PENTE].Valeur)/100  * (vl_donneesPente[0] - vl_donneesPente[3])) +
									((float)(tg_params[XDC_REG_COEF_CALCUL_PENTE_FIN].Valeur)/100 * (vl_donneesPente[1] - vl_donneesPente[2]));

							/*
							pl_station->station.pente = (XDY_Entier) vl_pente;
							pl_station->station.pente_valide = XDC_VRAI;
							*/							

								if ( (vl_Numero==75) )
							XZST_03EcritureTrace(XZSTC_WARNING," + + Calcul de la Pente + + (%d * (%d - %d)) + (%d * (%d - %d )) = %d",
									tg_params[XDC_REG_COEF_CALCUL_PENTE].Valeur,
									vl_donneesPente[0],
									vl_donneesPente[3],
									tg_params[XDC_REG_COEF_CALCUL_PENTE_FIN].Valeur,
									vl_donneesPente[1],
									vl_donneesPente[2],
									vl_pente);
						}
						else{
							if ( (vl_Numero==75) )
							XZST_03EcritureTrace(XZSTC_WARNING," + + Calcul de la Pente IMPOSSIBLE + + : (%d * (%d - %d)) + (%d * (%d - %d ))",
									tg_params[XDC_REG_COEF_CALCUL_PENTE].Valeur,
									vl_donneesPente[0],
									vl_donneesPente[3],
									tg_params[XDC_REG_COEF_CALCUL_PENTE_FIN].Valeur,
									vl_donneesPente[1],
									vl_donneesPente[2]);
							/*pl_station->station.pente_valide = XDC_FAUX;*/
						}

						pl_station->station.positionDerniereStationRecue = vl_DernierePosition;
						pl_station->station.positionDerniereStationRecuePente = vl_DernierePositionPente;
						break;
					}
					pl_station = pl_station->suivant;

				} /*Fin While Station*/
			/*}*/
			if (pl_station != NULL)
				break;

			pl_zone = pl_zone->suivant;

		}/*Fin While Zone*/

		if (pl_station == NULL){
			XZST_03EcritureTrace(DET_NIV_TRACE_2,"det_acquisition_cb : Station %d NON Trouvee pour le Sens %d", vl_Numero, vl_Sens);
		}


	}
	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : det_acquisition \n");
}


XDY_Entier verifier_une_condition(T_CONDITION *pa_condition, XDY_Horodate va_horodate, XDY_Booleen va_alternative,
		XDY_Booleen *pa_estOk, XDY_Booleen *pa_estNOk, T_SCENARIO va_scenario ){
	T_STATION *pl_station = NULL;
	XDY_Octet vl_operateur;
	XDY_Booleen vl_donnee_test_valide;
	double	vl_donnee_test;
	XDY_Entier vl_donnee, vl_seuil;
	strcpy(vg_donnee ," ");
	XDY_Booleen vl_flag_Qvalide, vl_flag_TOvalide, vl_flag_Pentevalide;
	vl_flag_Qvalide = XDC_FAUX;
	vl_flag_TOvalide = XDC_FAUX;
	vl_flag_Pentevalide = XDC_FAUX;

	int vl_r;
	int vl_jour;
	int vl_mois;
	int vl_annee;
	int vl_heure;
	int vl_minute;
	int vl_seconde;
	int vl_min, vl_max, vl_courant;

	XZSMT_Horodate vl_Horodate_courante, vl_horodate_debut, vl_horodate_fin;
	XDY_Entier vl_nb_ok_complexe;
	XDY_Entier vl_nb_nok_complexe;
	char buffer[2000];

	XZST_03EcritureTrace( XZSTC_DEBUG1, "IN verifier_une_condition %d", pa_condition->id);

	*pa_estOk = XDC_FAUX;
	*pa_estNOk = XDC_FAUX;

	strcpy(buffer, "");

	/* Conditions COMPLEXES*/
	if (pa_condition->condition_simple == XDC_FAUX)
	{
		XZST_03EcritureTrace(DET_NIV_TRACE_2,"         Condition COMPLEXE %d",pa_condition->id);
		if (pa_condition->liste_conditions_filles==NULL) {
		  XZST_03EcritureTrace(XZSTC_WARNING,"pas de conditions filles");
		  return (XDC_NOK);
		}
		if (pa_condition->id>500)
		  return XDC_NOK;
		if (verifier_conditions((T_Liste_Condition_Scenario *)pa_condition->liste_conditions_filles,
				va_horodate,
				pa_condition->type_condition,
				&vl_nb_ok_complexe,
				&vl_nb_nok_complexe,
				va_scenario) != XDC_OK){
			XZST_03EcritureTrace(XZSTC_WARNING,"    Impossible de verifier les sous conditions de la condition complexe %d(%s)",
					pa_condition->id,
					pa_condition->libelle);
		}else{
			if(vl_nb_nok_complexe > 0){
				*pa_estNOk = XDC_VRAI;
				XZST_03EcritureTrace(DET_NIV_TRACE_2,"    KO : Condition Complexe NON Verifiee");
			}
			else if(vl_nb_ok_complexe > 0){
				*pa_estOk = XDC_VRAI;
				XZST_03EcritureTrace(DET_NIV_TRACE_2,"    OK : Condition Complexe Verifiee");
			}
		}
	}else
	{
		/* Conditions SIMPLES*/
		if (pa_condition->type_condition == EREG_CONDITION_TYPE_HORAIRE)
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,"    Test Condition HORAIRE %d(%s)",
					pa_condition->id,
					pa_condition->libelle);

			if(XZSM_11ConversionHorodate( va_horodate, &vl_Horodate_courante) != XDC_OK)
			{
				XZST_03EcritureTrace( DET_NIV_TRACE_2, "Impossible de convertir l'horodate courante en chaine");
				return XDC_NOK;
			}
			if(XZSM_11ConversionHorodate( pa_condition->horodate_debut, &vl_horodate_debut) != XDC_OK)
			{
				XZST_03EcritureTrace( DET_NIV_TRACE_2, "Impossible de convertir l'horodate courante en chaine");
				return XDC_NOK;
			}
			if(XZSM_11ConversionHorodate( pa_condition->horodate_fin, &vl_horodate_fin) != XDC_OK)
			{
				XZST_03EcritureTrace( DET_NIV_TRACE_2, "Impossible de convertir l'horodate courante en chaine");
				return XDC_NOK;
			}

			sscanf( vl_Horodate_courante, "%d/%d/%d %d:%d:%d",
					&vl_jour, &vl_mois, &vl_annee, &vl_heure, &vl_minute, &vl_seconde);

			//comparaison de l'horodate courante avec la plage horaire
			//condition HORAIRE HORODATE
			if (pa_condition->type_condition_horaire == EREG_CONDITION_TYPE_HORAIRE_HORODATE)
			{
				XZST_03EcritureTrace(DET_NIV_TRACE_2,"# # plage horaire, 1, horodate # #");
				
				XZST_03EcritureTrace(XZSTC_DEBUG1,"horodate_debut : [%f] horodate : [%f](%s)\t horodate_fin : [%f]\n",
						pa_condition->horodate_debut,
						va_horodate,
						vl_Horodate_courante,
						pa_condition->horodate_fin);
				
				if( (pa_condition->horodate_debut <= va_horodate) &&
						(va_horodate <= pa_condition->horodate_fin) )
				{
					/*XZST_03EcritureTrace(XZSTC_INFO,"**** plage horaire heure ****");*/
					vl_min = pa_condition->heure_debut*60 + pa_condition->minute_debut;
					vl_max = pa_condition->heure_fin*60 +  pa_condition->minute_fin;
					vl_courant = vl_heure*60 + vl_minute;

					if ( (vl_courant >= vl_min) && (vl_courant <= vl_max))
					{
						*pa_estOk = XDC_VRAI;
						XZST_03EcritureTrace(DET_NIV_TRACE_2,"    OK : Condition Verifiee");
					}
					else{
						*pa_estNOk = XDC_VRAI;
						XZST_03EcritureTrace(DET_NIV_TRACE_2,"    KO : Condition NON Verifiee");
					}
				}
				else
					*pa_estNOk = XDC_VRAI;
			}
			//comparaison de l'heure courante avec la plage : "heure debut - heure fin"
			//condition HORAIRE HEURE
			else if (pa_condition->type_condition_horaire == EREG_CONDITION_TYPE_HORAIRE_HEURE)
			{
				XZST_03EcritureTrace(DET_NIV_TRACE_2,"# # plage horaire, 3, tous les jours # #");
				vl_min = pa_condition->heure_debut*60 + pa_condition->minute_debut;
				vl_max = pa_condition->heure_fin*60 +  pa_condition->minute_fin;
				vl_courant = vl_heure*60 + vl_minute;


				XZST_03EcritureTrace(DET_NIV_TRACE_2,"heure debut %d *courant (%d)* heure fin %d", vl_min, vl_courant, vl_max);

				if ( (vl_courant >= vl_min) && (vl_courant <= vl_max))
				{
					*pa_estOk = XDC_VRAI;
					XZST_03EcritureTrace(DET_NIV_TRACE_2,"    OK : Condition Verifiee");
				}
				else{
					*pa_estNOk = XDC_VRAI;
					XZST_03EcritureTrace(DET_NIV_TRACE_2,"    KO : Condition NON Verifiee");
				}
			}

			//condition HORAIRE JOUR + HORODATE
			else if (pa_condition->type_condition_horaire == XDC_REG_HORAIRE_HEURE_JOUR)
			{
				XZST_03EcritureTrace(DET_NIV_TRACE_2,"# # plage horaire, 2, Jours + Horodate # #");

				XZST_03EcritureTrace(DET_NIV_TRACE_2,"jour semaine cycle : %d\n",
						vg_JourSemaine_Cycle);


				/* tests plage horodate */
				if( (pa_condition->horodate_debut <= va_horodate) &&
						(va_horodate <= pa_condition->horodate_fin) )
				{
					XZST_03EcritureTrace(DET_NIV_TRACE_2,"# - horodate OK - #");
					/* tests du jour */
					if( pa_condition->type_condition_horaire_jour == vg_JourSemaine_Cycle )
					{
						XZST_03EcritureTrace(DET_NIV_TRACE_2,"# - jour  OK - #");
						vl_min = pa_condition->heure_debut*60 + pa_condition->minute_debut;
						vl_max = pa_condition->heure_fin*60 +  pa_condition->minute_fin;
						vl_courant = vl_heure*60 + vl_minute;
						/* test plage horraire */
						if ( (vl_courant >= vl_min) && (vl_courant <= vl_max))
						{
							*pa_estOk = XDC_VRAI;
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"    OK : Condition Verifiee");
						}
						else{
							*pa_estNOk = XDC_VRAI;
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"    KO : Condition NON Verifiee");
						}
					}
				}
			}
			/* Jours ouvrés : du lundi 0 au vendredi 4 */
			else if(pa_condition->type_condition_horaire_jour == XDC_REG_JOURS_OUVRES)
			{
				XZST_03EcritureTrace(DET_NIV_TRACE_2,"# # plage horaire, 4, Type jours (jours ouvres) # #");


				/* test plage horodate */
				if( (pa_condition->horodate_debut <= va_horodate) &&
						(va_horodate <= pa_condition->horodate_fin) )
				{
					XZST_03EcritureTrace(DET_NIV_TRACE_2,"# - horodate OK - #");

					/* test jours ouvres */
					if( (vg_JourSemaine_Cycle>=1) && (vg_JourSemaine_Cycle<=5) )
					{
						XZST_03EcritureTrace(DET_NIV_TRACE_2,"# - jours ouvres OK - #");
						vl_min = pa_condition->heure_debut*60 + pa_condition->minute_debut;
						vl_max = pa_condition->heure_fin*60 +  pa_condition->minute_fin;
						vl_courant = vl_heure*60 + vl_minute;
						/* test plage horraire */
						if ( (vl_courant >= vl_min) && (vl_courant <= vl_max))
						{
							*pa_estOk = XDC_VRAI;
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"    OK : Condition Verifiee");
						}
						else{
							*pa_estNOk = XDC_VRAI;
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"    KO : Condition NON Verifiee");
						}
					}
				}

			}
			else if(pa_condition->type_condition_horaire_jour == XDC_REG_JOURS_WE)
			{
				XZST_03EcritureTrace(DET_NIV_TRACE_2,"# # plage horaire, 4, Type jours (week end) # #");


				/* test plage horodate */
				if( (pa_condition->horodate_debut <= va_horodate) &&
						(va_horodate <= pa_condition->horodate_fin) )
				{
					XZST_03EcritureTrace(DET_NIV_TRACE_2,"# - horodate OK - #");

					/* test jours ouvres */
					if( (vg_JourSemaine_Cycle == 6) || (vg_JourSemaine_Cycle == 0) )
					{
						XZST_03EcritureTrace(DET_NIV_TRACE_2,"# - week end OK - #");
						vl_min = pa_condition->heure_debut*60 + pa_condition->minute_debut;
						vl_max = pa_condition->heure_fin*60 +  pa_condition->minute_fin;
						vl_courant = vl_heure*60 + vl_minute;
						/* test plage horraire */
						if ( (vl_courant >= vl_min) && (vl_courant <= vl_max))
						{
							*pa_estOk = XDC_VRAI;
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"    OK : Condition Verifiee");
						}
						else{
							*pa_estNOk = XDC_VRAI;
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"    KO : Condition NON Verifiee");
						}
					}
				}
			}
			else{
				XZST_03EcritureTrace(XZSTC_WARNING,"Attention cas Type de Condition Horaire %d non traite",
						pa_condition->type_condition_horaire_jour);
			}
		}/* Fin Traitement Condition Horaire */
		/*else{	XZST_03EcritureTrace(XZSTC_WARNING,"Attention cas Type de Condition Horaire %d non traite",	pa_condition->type_condition_horaire);}*/
		else
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,"    Test Condition TRAFIC %d(%s)",
					pa_condition->id,
					pa_condition->libelle);

			/*recherche de la station de la condition*/
			pl_station = pa_condition->station;

			if (pl_station != NULL)
			{
				XZST_03EcritureTrace(XZSTC_DEBUG1,"      test si station %d en panne",
						pl_station->station);

				if(pl_station->comptage_panne == XDC_VRAI)
				{
					XZST_03EcritureTrace(DET_NIV_TRACE_2,"    >> Station de Comptage en PANNE, verification condition alternative : %d", pl_station->station);
					//verification de la condition alternative
					if(pa_condition->condition_alternative != NULL)
					{
						if (verifier_une_condition(pa_condition->condition_alternative,
								va_horodate,
								XDC_VRAI,
								pa_estOk,
								pa_estNOk,
								va_scenario) != XDC_OK){
							XZST_03EcritureTrace(XZSTC_WARNING,"    Impossible de verifier la condition alternative %d(%s)",
									pa_condition->condition_alternative->id,
									pa_condition->condition_alternative->libelle);
						}else{
							if(*pa_estOk == XDC_VRAI){
								XZST_03EcritureTrace(DET_NIV_TRACE_2,"    OK : Condition Alternative Verifiee");
							}
							if(*pa_estNOk == XDC_VRAI){
								XZST_03EcritureTrace(DET_NIV_TRACE_2,"    KO : Condition Alternative NON Verifiee");
							}
						}
					}else
					{
						XZST_03EcritureTrace(DET_NIV_TRACE_2,"       Pas de Condition Alternative !");
						*pa_estNOk == XDC_VRAI;
					}


				}
				else {
					XZST_03EcritureTrace(XZSTC_DEBUG1,"      test de la validite des donnees de la station");

					vl_operateur = pa_condition->operateur_comparaison;
					vl_donnee = pa_condition->type_donnee_trafic;
					vl_seuil = pa_condition->seuil;

					//Test de validité des données
					if( (vl_donnee == XDC_REG_DONNEE_Q) && (pl_station->data_lisseeQ_valide == XDC_FAUX) )
					{
						XZST_03EcritureTrace(XZSTC_DEBUG1,"      Condition Q %d(%s) NON verifiable : Donnees Station %d INVALIDES",
								pa_condition->id,
								pa_condition->libelle,
								pl_station->station);
						XZST_03EcritureTrace(DET_NIV_TRACE_2,"        >> verification condition alternative : ");
						//verification de la condition alternative
						if(pa_condition->condition_alternative != NULL)
						{
							if (verifier_une_condition(pa_condition->condition_alternative,
									va_horodate,
									XDC_VRAI,
									pa_estOk,
									pa_estNOk,
									va_scenario) != XDC_OK)
							{
								XZST_03EcritureTrace(DET_NIV_TRACE_2,"        Impossible de verifier la condition alternative %d(%s)",
										pa_condition->condition_alternative->id,
										pa_condition->condition_alternative->libelle);
							}else{
								if(*pa_estOk == XDC_VRAI){
									XZST_03EcritureTrace(DET_NIV_TRACE_2,"        OK : Condition Alternative Verifiee");
								}
								if(*pa_estNOk == XDC_VRAI){
									XZST_03EcritureTrace(DET_NIV_TRACE_2,"        KO : Condition Alternative NON Verifiee");
								}
							}
						}else
						{
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"        >> Pas de Condition Alternative !");
							*pa_estNOk = XDC_VRAI;
						}
					}
					else if( (vl_donnee == XDC_REG_DONNEE_TO) && (pl_station->data_lisseeTO_valide == XDC_FAUX) )
					{
						XZST_03EcritureTrace(DET_NIV_TRACE_2,"      Condition TO %d(%s) NON verifiable : Donnees Station %d INVALIDES",
								pa_condition->id,
								pa_condition->libelle,
								pl_station->station);
						XZST_03EcritureTrace(DET_NIV_TRACE_2,"        >> verification condition alternative : ");
						//verification de la condition alternative
						if(pa_condition->condition_alternative != NULL)
						{
							if (verifier_une_condition(pa_condition->condition_alternative,
									va_horodate,
									XDC_VRAI,
									pa_estOk,
									pa_estNOk,
									va_scenario) != XDC_OK)
							{
								XZST_03EcritureTrace(DET_NIV_TRACE_2,"        Impossible de verifier la condition alternative %d(%s)",
										pa_condition->condition_alternative->id,
										pa_condition->condition_alternative->libelle);
							}else{
								if(*pa_estOk == XDC_VRAI){
									XZST_03EcritureTrace(DET_NIV_TRACE_2,"        OK : Condition Alternative Verifiee");
								}
								if(*pa_estNOk == XDC_VRAI){
									XZST_03EcritureTrace(DET_NIV_TRACE_2,"        KO : Condition Alternative NON Verifiee");
								}
							}
						}else
						{
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"        >> Pas de Condition Alternative !");
							*pa_estNOk = XDC_VRAI;
						}
					}
					else if( (vl_donnee == XDC_REG_DONNEE_PENTE) && (pl_station->pente_valide == XDC_FAUX) )
					{
						XZST_03EcritureTrace(XZSTC_DEBUG1,"      Condition pente %d(%s) NON verifiable : Donnees Station %d INVALIDES",
								pa_condition->id,
								pa_condition->libelle,
								pl_station->station);
						XZST_03EcritureTrace(XZSTC_DEBUG1,"        >> verification condition alternative : ");
						//verification de la condition alternative
						if(pa_condition->condition_alternative != NULL)
						{
							if (verifier_une_condition(pa_condition->condition_alternative,
									va_horodate,
									XDC_VRAI,
									pa_estOk,
									pa_estNOk,
									va_scenario) != XDC_OK)
							{
								XZST_03EcritureTrace(DET_NIV_TRACE_2,"        Impossible de verifier la condition alternative %d(%s)",
										pa_condition->condition_alternative->id,
										pa_condition->condition_alternative->libelle);
							}else{
								if(*pa_estOk == XDC_VRAI){
									XZST_03EcritureTrace(XZSTC_DEBUG1,"        OK : Condition Alternative Verifiee");
								}
								if(*pa_estNOk == XDC_VRAI){
									XZST_03EcritureTrace(XZSTC_DEBUG1,"        KO : Condition Alternative NON Verifiee");
								}
							}
						}else
						{
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"        >> Pas de Condition Alternative !");
							*pa_estNOk = XDC_VRAI;
						}
					}
					else{
						XZST_03EcritureTrace(XZSTC_DEBUG1,"      test des donnees ...");
						if (vl_donnee == XDC_REG_DONNEE_Q)
						{
							vl_donnee_test = pl_station->data_lisseeQ;
							vl_donnee_test_valide = pl_station->data_lisseeQ_valide;
							sprintf(vg_donnee, "(%f)", vl_donnee_test);
							XZST_03EcritureTrace(XZSTC_DEBUG1,"*-*-*-* VG_DONNEE %s *-*-*-*", vg_donnee);
							XZST_03EcritureTrace(XZSTC_DEBUG1,"*-*-*-* VL_DONNEE_TEST %f *-*-*-*", vl_donnee_test);


							XZST_03EcritureTrace(XZSTC_DEBUG1,"      test sur valeur Q lissee = %f(%s) et seuil =%d",
									vl_donnee_test,
									vl_donnee_test_valide==XDC_VRAI?"Valide":"Invalide",
											vl_seuil);

						}else if (vl_donnee == XDC_REG_DONNEE_TO){
							vl_donnee_test = pl_station->data_lisseeTO;
							vl_donnee_test_valide = pl_station->data_lisseeTO_valide;
							sprintf(vg_donnee, "(%f)", vl_donnee_test);
							XZST_03EcritureTrace(XZSTC_DEBUG1,"*-*-*-* VG_DONNEE %s *-*-*-*", vg_donnee);
							XZST_03EcritureTrace(XZSTC_DEBUG1,"*-*-*-* VL_DONNEE_TEST %f *-*-*-*", vl_donnee_test);

							XZST_03EcritureTrace(XZSTC_DEBUG1,"      test sur valeur TO lissee = %f(%s) et seuil =%d",
									vl_donnee_test,
									vl_donnee_test_valide==XDC_VRAI?"Valide":"Invalide",
											vl_seuil);

						}else if (vl_donnee == XDC_REG_DONNEE_PENTE){
							vl_donnee_test = (double)pl_station->pente;
							sprintf(vg_donnee, "(%f)", vl_donnee_test);
							XZST_03EcritureTrace(XZSTC_DEBUG1,"*-*-*-* VG_DONNEE %s *-*-*-*", vg_donnee);
							XZST_03EcritureTrace(XZSTC_DEBUG1,"*-*-*-* VL_DONNEE_TEST %f *-*-*-*", vl_donnee_test);

							XZST_03EcritureTrace(DET_NIV_TRACE_2,"      test sur valeur PENTE = %f et seuil =%d",
									vl_donnee_test,
									vl_seuil);

						} else
						{
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"Test Condition %d Impossible : Type de la donnee Incoherent : %d",
									pa_condition->id,
									vl_donnee);
						}

						if (vl_operateur == XDC_REG_OPERATEUR_INF)
						{
							if (vl_donnee_test < vl_seuil)
							{
								XZST_03EcritureTrace(XZSTC_DEBUG1,"      test OK car valeur %f INFERIEURE au seuil %d",
										vl_donnee_test, vl_seuil);
								*pa_estOk = XDC_VRAI;
							}
							else
							{
								XZST_03EcritureTrace(XZSTC_DEBUG1,"      test NOK car valeur %f NON INFERIEURE au seuil %d",
										vl_donnee_test, vl_seuil);
								*pa_estNOk = XDC_VRAI;
							}
						}else if (vl_operateur == XDC_REG_OPERATEUR_SUP)
						{
							if (vl_donnee_test > vl_seuil)
							{
								XZST_03EcritureTrace(XZSTC_DEBUG1,"      test OK car valeur %f SUPERIEURE au seuil %d",
										vl_donnee_test, vl_seuil);
								*pa_estOk = XDC_VRAI;
							}
							else
							{
								XZST_03EcritureTrace(XZSTC_DEBUG1,"      test NOK car valeur %f NON SUPERIEURE au seuil %d",
										vl_donnee_test, vl_seuil);
								*pa_estNOk = XDC_VRAI;
							}
						}else if (vl_operateur == XDC_REG_OPERATEUR_EGAL)
						{
							if (vl_donnee_test == (double)vl_seuil)
							{
								XZST_03EcritureTrace(DET_NIV_TRACE_2,"      test OK car valeur %f EGALE au seuil %d",
										vl_donnee_test, vl_seuil);
								*pa_estOk = XDC_VRAI;
							}
							else
							{
								XZST_03EcritureTrace(DET_NIV_TRACE_2,"      test NOK car valeur %f NON EGALE au seuil %d",
										vl_donnee_test, vl_seuil);
								*pa_estNOk = XDC_VRAI;
							}
						}else{
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"Test Condition %d Impossible : Opérateur de comparaison Incoherent : %d",
									pa_condition->id,
									vl_operateur);
						}
					}
				}
			} /* fin station est nulle */
			else
			{
				XZST_03EcritureTrace(DET_NIV_TRACE_1,"    Pas de Station Associee !!!!!!!!!!!!!!");

			}
		} /* Fin Traitement Condition Activation ou Desactivation*/
	}/* Fin Traitement Condition Simple*/

	XZST_03EcritureTrace(XZSTC_DEBUG1,"    Resultat Condition %d(%s) : OK=%s / NOK=%s",
			pa_condition->id,
			pa_condition->libelle,
			*pa_estOk == XDC_VRAI?"VRAI":"FAUX",
					*pa_estNOk == XDC_VRAI?"VRAI":"FAUX");

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT verifier_une_condition");
	return XDC_OK;
}

XDY_Entier verifier_conditions(T_Liste_Condition_Scenario *pa_listeConditions, XDY_Horodate va_horodate, XDY_Entier va_type_condition, XDY_Entier *pa_nbOk, XDY_Entier *pa_nbNOk, T_SCENARIO va_scenario )
{
	XDY_Entier vl_nb_ok;
	XDY_Entier vl_nb_nok;
	XDY_Booleen vl_ok;
	XDY_Booleen vl_nok;
	XDY_Entier vl_type_cond;

	 XZST_03EcritureTrace( XZSTC_WARNING, "verifier_conditions");
	XZST_03EcritureTrace( XZSTC_WARNING, "IN verifier_conditions %d", pa_listeConditions->condition->id);

	/* traitement des conditions d'activation */
	vl_nb_ok = 0;
	vl_nb_nok = 0;
	while (pa_listeConditions != NULL){
		if (pa_listeConditions->condition->type_condition != va_type_condition){
			pa_listeConditions = pa_listeConditions->suivant;
			continue;
		}

		if (pa_listeConditions==NULL)
		  break;

		XZST_03EcritureTrace(DET_NIV_TRACE_2,"    Test Condition %d(%s)",
				pa_listeConditions->condition->id,
				pa_listeConditions->condition->libelle);

		if (verifier_une_condition(pa_listeConditions->condition,
				va_horodate,
				XDC_FAUX,
				&vl_ok,
				&vl_nok,
				va_scenario) != XDC_OK){
			XZST_03EcritureTrace(XZSTC_WARNING,"Impossible de Verifier la condition %d(%s)",
					pa_listeConditions->condition->id,
					pa_listeConditions->condition->libelle);
			return XDC_NOK;
		}else
		{
			if (vl_ok == XDC_VRAI)
			{
				vl_nb_ok ++;
				vl_type_cond = pa_listeConditions->condition->type_condition;

				XZST_03EcritureTrace(XZSTC_DEBUG1,"*-*-*-* STATION %d *-*-*-*", pa_listeConditions->condition->num_station);
				XZST_03EcritureTrace(XZSTC_DEBUG1,"*-*-*-* SENS %d *-*-*-*", pa_listeConditions->condition->station->sens);

				if (trace_process(EREG_TRACE_TYPE_DETECTION,
						va_scenario.zone_scenario,
						pa_listeConditions->condition->id,
						0,
						0,0,
						va_scenario.id,
						0,
						pa_listeConditions->condition->num_station,
						pa_listeConditions->condition->station->sens,
						"%s %s ", vl_type_cond == EREG_CONDITION_TYPE_ACTIVATION ?  "activation" :
								vl_type_cond == EREG_CONDITION_TYPE_DESACTIVATION ? "desactivation" : "horaire",
										vg_donnee) != XDC_OK)
				{
					XZST_03EcritureTrace(DET_NIV_TRACE_1,"Impossible d effectuer l historisation");
				}
			}
			if (vl_nok == XDC_VRAI)
			{
				vl_nb_nok ++;
			}
		}

		XZST_03EcritureTrace(DET_NIV_TRACE_2,"    Resultat Condition %d(%s) : OK=%d / NOK=%d",
				pa_listeConditions->condition->id,
				pa_listeConditions->condition->libelle,
				vl_nb_ok,
				vl_nb_nok);

		pa_listeConditions = pa_listeConditions->suivant;

	} //fin parcours des conditions

	XZST_03EcritureTrace(DET_NIV_TRACE_2,"    Resultat Final : OK=%d / NOK=%d",
			vl_nb_ok,
			vl_nb_nok);

	*pa_nbOk = vl_nb_ok;
	*pa_nbNOk = vl_nb_nok;

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT verifier_conditions %d OK / %d NOK",
			*pa_nbNOk,
			*pa_nbNOk);

	return XDC_OK;
}

void det_ideal_cb(T_IPC_CONN 					va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
		T_CB_ARG                        pa_ARG)
{
	XDY_Eqt				vl_numero_zone;
	T_Liste_ZonesRegul 	*pl_zone = NULL;
	T_Liste_Scenario 	*pl_listeScenario = NULL;
	T_SCENARIO 			*pl_scenario = NULL;
	T_SCENARIO			pl_sce_tst;
	T_Liste_Condition_Scenario 	*pl_condition = NULL;
	/*	T_SCENARIO 			vl_scenario_nominal;*/
	T_SCENARIO 			vl_scenario;
	T_SCENARIO 			vl_scenario_ideal;
	T_STATION 			vl_station;
	T_STATION 			*pl_station;
	XDY_Entier			vl_nbOK;
	XDY_Entier 			vl_nbKO;
	XDY_Booleen			vl_testerHoraire;
	XDY_Horodate			vl_date;

	XZST_03EcritureTrace( XZSTC_DEBUG1, "IN : det_ideal_cb");

	if (XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,
			(int *) &vg_JourSemaine_Cycle, (double *) &vg_Horodate_Cycle ) != XDC_OK )
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de recuperer l horodate courante");
	}

	XZSMT_Horodate vl_Horodate="vide";
	if (vg_SimuEnCours) {
		XZSM_11ConversionHorodate( vg_HorodateSimu, &vl_Horodate);
		vl_date = vg_HorodateSimu;
	}
	else {
		if(XZSM_11ConversionHorodate( vg_Horodate_Cycle, &vl_Horodate) != XDC_OK)
			XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de convertir l'horodate courante en chaine");

		vl_date = vg_Horodate_Cycle;
	}

	XZST_03EcritureTrace( DET_NIV_TRACE_1, "(((((((( DETECTION CB %s )))))))",vl_Horodate);

	/* recherche d'un scenario activable pour la zone */
	pl_zone = pg_Zones;

	while(pl_zone != NULL)
	{
		passer_zone_degrade(&pl_zone->zone, XDC_FAUX,"");

		XZST_03EcritureTrace( DET_NIV_TRACE_1, ">>>>>>>>> DETECTION ZONE %d (%s)",
				pl_zone->zone.numero_zone,
				pl_zone->zone.nom_zone);

		/* Reinitialisation regulation Proposee */
		XZST_03EcritureTrace( DET_NIV_TRACE_1, "ZONE %d (%s) : reinitialisation de la regulation proposee",
				pl_zone->zone.numero_zone,
				pl_zone->zone.nom_zone);

		/* Reinitialisation Contexte */
		pl_zone->zone.regulation_proposee.contexte.chantiers = XDC_FAUX;
		pl_zone->zone.regulation_proposee.contexte.declenchement_horaire = XDC_FAUX;
		pl_zone->zone.regulation_proposee.contexte.evt_impactant = XDC_FAUX;
		pl_zone->zone.regulation_proposee.contexte.panne_PRV_critique = XDC_FAUX;
		pl_zone->zone.regulation_proposee.contexte.panne_PRV_non_critique = XDC_FAUX;
		pl_zone->zone.regulation_proposee.contexte.qualification_operateur = XDC_FAUX;
		pl_zone->zone.regulation_proposee.contexte.scenario_ideal = XDC_FAUX;

		/* proposition scenario ideal */
		pl_zone->zone.regulation_proposee.indicateur_horaire = XDC_FAUX;
		pl_zone->zone.regulation_proposee.mesure = EREG_MESURE_DEFAUT;
		pl_zone->zone.regulation_proposee.reponse_operateur = XDC_ALT_INCONNUE;
		pl_zone->zone.regulation_proposee.scenario.id = -1;

		/* Recherche du scenario Ideal */
		pl_listeScenario = pl_zone->zone.ListeScenario;
		pl_scenario = NULL;
		vl_testerHoraire = XDC_FAUX;

		/* On va se positionner sur le plus regule*/
		if (pl_listeScenario != NULL)
			while(pl_listeScenario->suivant != NULL){
				pl_listeScenario = pl_listeScenario->suivant;
			}

		while(pl_listeScenario != NULL)
		{
			/*		if (pl_listeScenario->scenario.id == 11)
				pl_sce_tst = pl_listeScenario->scenario;*/

			/*			XZST_03EcritureTrace( XZSTC_INFO, "vg_idScenarioDefaut = %d, pl_listeScenario->precedent = %s",
					vg_idScenarioDefaut,
					pl_listeScenario->precedent == NULL ?"NULL":"not null"); */
			/* Forcage SCENARIO plus REGULE */
			if (vg_idScenarioDefaut == 0){
				XZST_03EcritureTrace( XZSTC_WARNING, "*******************************************************");
				XZST_03EcritureTrace( XZSTC_WARNING, "*********** FORCAGE SCENARIO %2d AU DEMARAGE ***********", pl_listeScenario->scenario.id);
				XZST_03EcritureTrace( XZSTC_WARNING, "*******************************************************");
				pl_scenario = &(pl_listeScenario->scenario);
				break;
			}

			if (vg_idScenarioDefaut > 0 && vg_idScenarioDefaut == pl_listeScenario->scenario.id){
				XZST_03EcritureTrace( XZSTC_WARNING, "*******************************************************");
				XZST_03EcritureTrace( XZSTC_WARNING, "*********** FORCAGE SCENARIO %2d AU DEMARAGE ***********", vg_idScenarioDefaut);
				XZST_03EcritureTrace( XZSTC_WARNING, "*******************************************************");
				pl_scenario = &(pl_listeScenario->scenario);
				break;
			}
			if (vg_idScenarioDefaut == 0 && pl_listeScenario->precedent != NULL){
				/*				XZST_03EcritureTrace( XZSTC_INFO, "pl_listeScenario->precedent->precedent = %s",
						pl_listeScenario->precedent->precedent == NULL ?"NULL":"not null");*/
				if (pl_listeScenario->precedent->precedent == NULL){
					XZST_03EcritureTrace( XZSTC_WARNING, "*******************************************************");
					XZST_03EcritureTrace( XZSTC_WARNING, "*********** FORCAGE SCENARIO %2d AU DEMARAGE ***********", pl_listeScenario->scenario.id);
					XZST_03EcritureTrace( XZSTC_WARNING, "*******************************************************");
					pl_scenario = &(pl_listeScenario->scenario);
					break;
				}
			}


			XZST_03EcritureTrace( DET_NIV_TRACE_1, "ZONE %d (%s) : Test Scenario %d (%s)",
					pl_zone->zone.numero_zone,
					pl_zone->zone.nom_zone,
					pl_listeScenario->scenario.id,
					pl_listeScenario->scenario.libelle);

			vl_scenario = pl_listeScenario->scenario;
			vl_station = pl_zone->zone.ListeStation->station;

			/* verification de l'etat du scenarios */
			if  (vl_scenario.degrade == XDC_VRAI ){
				XZST_03EcritureTrace(XZSTC_DEBUG1,"Scenario %d Ignore : DEGRADE",vl_scenario.id );
				pl_listeScenario = pl_listeScenario->precedent;
				continue;
			}

			/* verification de l'etat du scenarios */
			if  (vl_scenario.nominal == XDC_VRAI ){
				XZST_03EcritureTrace(XZSTC_DEBUG1,"Scenario %d Ignore : NOMINAL",vl_scenario.id );
				pl_listeScenario = pl_listeScenario->precedent;
				continue;
			}

			/* declenchement horaire force, verification de condition de declenchement horaire */
			if(pl_zone->zone.mode_horaire == XDC_VRAI)
				if (vl_scenario.has_conditions_horaire == XDC_FAUX){
					XZST_03EcritureTrace(XZSTC_DEBUG1,"Scenario %d Ignore : Zone %s en mode declenchement horaire force et pas de condition d'horaire pour scenario",pl_zone->zone.nom_zone);
					pl_listeScenario = pl_listeScenario->precedent;
					continue;
				}

			/* parcours des conditions de declenchement et verif des conditions */


			/* traitement des conditions d'activation */
			/* SI conditions */
			if (vl_scenario.conditions == NULL)
				XZST_03EcritureTrace(XZSTC_WARNING," ATTENTION IL N'Y A PAS DE CONDITIONS POUR CE SCENARIO");
			else
			{
				XZST_03EcritureTrace(XZSTC_DEBUG1,"!!! SCENARIO PRECEDENT %d !!! SCENARIO TESTE %d !!!",
						pl_zone->zone.regulation.scenario.id,
						//pl_scenario->id,
						vl_scenario.id);

				if (pl_zone->zone.regulation.scenario.id == vl_scenario.id)
					//if (pl_scenario->id == vl_scenario.id)
				{
					XZST_03EcritureTrace(XZSTC_DEBUG1,"!!! SCENARIO IDENTIQUE A PRECEDENT !!!");
					// SI conditions de desactivation
					if (verifier_conditions(vl_scenario.conditions, vl_date, EREG_CONDITION_TYPE_DESACTIVATION, &vl_nbOK, &vl_nbKO, vl_scenario) == XDC_OK)
					{
						// SI (au moins) une condition de desactivation n'a pas ete verifiee
						if (vl_nbKO > 0 || vl_nbOK == 0)
						{
							// Au moins une condition de desactivation n'a pas ete verifiee
							// => SCENARIO OK
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"    Conditions de DESACTIVATION NON Verifiees");
							// on a notre scenario
							pl_scenario = &(pl_listeScenario->scenario);
							break;
						}
						else
						{
							// toutes les conditions de desactivation ont ete verifiee
							// => SCENARIO NOK
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"    Conditions de DESACTIVATION Verifiees");
							vl_testerHoraire = XDC_VRAI;
						}
					}
				}else
				{
					XZST_03EcritureTrace(XZSTC_INFO,"!!! SCENARIO DIFFERENT QUE PRECEDENT !!!");
					// SI conditions d'activation
					if (verifier_conditions(vl_scenario.conditions, vl_date, EREG_CONDITION_TYPE_ACTIVATION,&vl_nbOK, &vl_nbKO, vl_scenario) == XDC_OK)
					{

						// SI (au moins) une condition d'activation a ete verifiee
						if (vl_nbOK > 0)
						{
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"    Conditions d'ACTIVATION Verifiees");
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"... verification des Conditions de desactivation : ");

							// SI conditions de desactivation
							if (verifier_conditions(vl_scenario.conditions, vl_date, EREG_CONDITION_TYPE_DESACTIVATION, &vl_nbOK, &vl_nbKO, vl_scenario) == XDC_OK)
							{
								// SI (au moins) une condition de desactivation n'a pas ete verifiee
								if (vl_nbKO > 0 || vl_nbOK == 0)
								{
									// Au moins une condition de desactivation n'a pas ete verifiee
									// => SCENARIO OK
									XZST_03EcritureTrace(DET_NIV_TRACE_2,"    Conditions de DESACTIVATION NON Verifiees");
									// on a notre scenario
									pl_scenario = &(pl_listeScenario->scenario);

									break;
								}
								else
								{
									// toutes les conditions de desactivation ont ete verifiee
									// => SCENARIO NOK
									XZST_03EcritureTrace(DET_NIV_TRACE_2,"    Conditions de DESACTIVATION Verifiees");
									vl_testerHoraire = XDC_VRAI;
								}
							}
							// Erreur verification des conditions de desactivations
							else
							{
								XZST_03EcritureTrace(XZSTC_WARNING,"Impossible de Verifier les Conditions de Desactivation du Scenario ");
							}
						}
						else
						{
							// Pas de condition d'activation satisfaite
							// => SCENARIO NOK
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"    Conditions d'ACTIVATION Non Verifiees");
							vl_testerHoraire = XDC_VRAI;
						}


					}// Erreur verification des conditions d'activation
					else
					{
						XZST_03EcritureTrace(XZSTC_WARNING,"Impossible de Verifier les Conditions d'Activation du Scenario");
					}
				}


				if (vl_testerHoraire == XDC_VRAI)
				{
					// SI conditions horaire
					if (verifier_conditions(vl_scenario.conditions, vl_date, EREG_CONDITION_TYPE_HORAIRE, &vl_nbOK, &vl_nbKO,vl_scenario) == XDC_OK)
					{
						// Si (au moins) une condition horaire a été verifiée
						if (vl_nbOK > 0)
						{
							// => SCENARIO OK
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"    Conditions HORAIRES Verifiees");

							// on a notre scenario
							pl_scenario = &(pl_listeScenario->scenario);
							pl_zone->zone.regulation_proposee.indicateur_horaire = XDC_VRAI;

							break;
						}
						else
						{
							// Aucune condition d'Activation n'a ete verifiee
							// => SCENARIO NOK
							XZST_03EcritureTrace(DET_NIV_TRACE_2,"    Conditions HORAIRES Non Verifiees");
						}
					}
					// pas de conditions horaires
					else
					{
						// on a notre scenario
						pl_scenario = &(pl_listeScenario->scenario);



						break;
					}
				}
			}
			pl_listeScenario = pl_listeScenario->precedent;
		} /* Fin Boucle Scenarios*/

		if (pl_scenario == NULL){
			XZST_03EcritureTrace(DET_NIV_TRACE_2,"Zone %d(%s) Aucun Scenario Detecte : Scenario Nominal Utilise",
					pl_zone->zone.numero_zone,
					pl_zone->zone.nom_zone);

			pl_scenario = pl_zone->zone.scenarioNominal;
		}

		// initialisation contexte
		pl_zone->zone.regulation_proposee.scenario = *pl_scenario;
		pl_zone->zone.regulation_proposee.contexte.scenario_ideal = XDC_VRAI;


		/*	pl_zone->zone.regulation_proposee.scenario = pl_sce_tst;*/


		XZST_03EcritureTrace(DET_NIV_TRACE_1,"*************************************************");
		XZST_03EcritureTrace(DET_NIV_TRACE_1,"Zone %d(%s) Scenario Propose = %d (%s)",
				pl_zone->zone.numero_zone,
				pl_zone->zone.nom_zone,
				pl_zone->zone.regulation_proposee.scenario.id,
				pl_zone->zone.regulation_proposee.scenario.libelle);
		XZST_03EcritureTrace(DET_NIV_TRACE_1,"AFFICHAGE DU CONTEXTE : chantier=%s, declenchement H=%s, evt impact=%s, panne PRV Crit = %s, panne PRV non Crit = %s, qualif ope = %s, scenario ideal = %s",
				pl_zone->zone.regulation_proposee.contexte.chantiers ==XDC_VRAI ? "VRAI" : "FAUX",
						pl_zone->zone.regulation_proposee.contexte.declenchement_horaire ==XDC_VRAI ? "VRAI" : "FAUX",
								pl_zone->zone.regulation_proposee.contexte.evt_impactant ==XDC_VRAI ? "VRAI" : "FAUX",
										pl_zone->zone.regulation_proposee.contexte.panne_PRV_critique ==XDC_VRAI ? "VRAI" : "FAUX",
												pl_zone->zone.regulation_proposee.contexte.panne_PRV_non_critique ==XDC_VRAI ? "VRAI" : "FAUX",
														pl_zone->zone.regulation_proposee.contexte.qualification_operateur ==XDC_VRAI ? "VRAI" : "FAUX",
																pl_zone->zone.regulation_proposee.contexte.scenario_ideal ==XDC_VRAI ? "VRAI" : "FAUX");
		XZST_03EcritureTrace(DET_NIV_TRACE_1,"*************************************************");

		//Historisation
		if(pl_zone->zone.regulation.scenario.id != pl_zone->zone.regulation_proposee.scenario.id) {
			if (trace_process(EREG_TRACE_TYPE_DETECTION,
					pl_zone->zone.numero_zone,
					0,
					0,
					0,0,
					pl_zone->zone.regulation_proposee.scenario.id,
					0,
					0,0,
					"scenario ideal") != XDC_OK)
			{
				XZST_03EcritureTrace(DET_NIV_TRACE_1,"Impossible d effectuer l historisation nom zone : %s \n de la regulation proposee", pl_zone->zone.nom_zone, pl_zone->zone.regulation_proposee.scenario.libelle);
			}
		}

		//passer a l ajustement
		ajuste_analyse(pl_zone);

		pl_zone = pl_zone->suivant;
	} /* Fin Boucle Zones*/

	vg_idScenarioDefaut = -1;
	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : det_ideal_cb");
}



void det_simu_trafic_cb(T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_Data, T_CB_ARG pa_ARG)
{
	T_Liste_Station *pl_station = NULL;
	T_Liste_ZonesRegul 	*pl_zone = NULL;
	XDY_Entier	vl_Q;
	XDY_Entier	vl_V;
	XDY_Entier	vl_TO;
	XDY_Octet	vl_Validite_Q;
	XDY_Octet	vl_Validite_V;
	XDY_Octet	vl_Validite_TO;
	XDY_Entier 	vl_Periode;
	XDY_Datagroup	vl_nomDG;
	XZSMT_Horodate vl_Horodate_debut, vl_horodate_fin, vl_horodate_simu;

	vl_Periode = 6;
	pl_zone = pg_Zones;

	XZST_03EcritureTrace( XZSTC_DEBUG1, "date de simulation : %lf",vg_HorodateSimu);

	XDG_EncodeDG2 ( vl_nomDG, XDG_REGUL_DETECTION, vg_NomSite);

	vg_HorodateSimu = vg_HorodateSimu + 6*60;
	XZIT20_Fin_Simulation(vg_machineSimu);

	if(XZSM_11ConversionHorodate(vg_HorodateSimu , &vl_horodate_simu) != XDC_OK)
	{
		XZST_03EcritureTrace( DET_NIV_TRACE_2, "Impossible de convertir l'horodate courante en chaine");
	}
	XZST_03EcritureTrace( XZSTC_WARNING, "det_simu_trafic_cb date de simulation : %s",vl_horodate_simu);

	if(XZSM_11ConversionHorodate(vg_horodate_fin_simu , &vl_horodate_fin) != XDC_OK)
	{
		XZST_03EcritureTrace( DET_NIV_TRACE_2, "Impossible de convertir l'horodate courante en chaine");
	}


	while(pl_zone != NULL)
	{
		pl_station = pl_zone->zone.ListeStation ;

		while(pl_station != NULL)
		{
			//recuperation en base des donnees trafics
			XZST_03EcritureTrace( XZSTC_DEBUG1, "station %d sens %d",
					pl_station->station.station,
					pl_station->station.sens);

			if (XZAS540_Liste_Donnees_Trafic(pl_station->station.station,
					pl_station->station.sens,
					vg_HorodateSimu,
					&vl_Q,
					&vl_V,
					&vl_TO,
					&vl_Validite_Q,
					&vl_Validite_V,
					&vl_Validite_TO)!= XDC_OK)
			{
				XZST_03EcritureTrace( XZSTC_WARNING, "pb appel XZAS540_Liste_Donnees_Trafic");
			}
			else{
				XZST_03EcritureTrace( XZSTC_DEBUG1, "Q = %d/%d, V=%d/%d, TO=%d/%d",
						vl_Q,vl_Validite_Q,
						vl_V,vl_Validite_V,
						vl_TO,vl_Validite_TO);

				if (!TipcSrvMsgWrite(vl_nomDG,
						XDM_IdentMsg(XDM_EMES_6mn_Chaussee),
						XDM_FLG_SRVMSGWRITE,
						T_IPC_FT_REAL8,vg_HorodateSimu,
						T_IPC_FT_INT2,pl_station->station.station,
						T_IPC_FT_INT2,vl_Periode,
						T_IPC_FT_INT2,pl_station->station.sens,
						T_IPC_FT_INT2,XDC_RADT_CHAUSSEE,
						T_IPC_FT_INT2,vl_Q,
						T_IPC_FT_INT2,vl_Validite_Q,
						T_IPC_FT_INT2,vl_V,
						T_IPC_FT_INT2,vl_Validite_V,
						T_IPC_FT_INT2,vl_TO,
						T_IPC_FT_INT2,vl_Validite_TO,
						T_IPC_FT_INT2,-1,
						T_IPC_FT_INT2,-1,
						NULL) )
				{
					XZST_03EcritureTrace( XZSTC_DEBUG1, "message non envoye");
				}
				else
				{
					XZST_03EcritureTrace( XZSTC_DEBUG1, "message envoye");
					XZST_03EcritureTrace( XZSTC_DEBUG1, "HorodateSimu :%s station : %d sens : %d",
							vl_horodate_simu,
							pl_station->station.station,
							pl_station->station.sens);
					XZST_03EcritureTrace( XZSTC_DEBUG1, "DONNEE MSG Q = %d/%d, V=%d/%d, TO=%d/%d",
							vl_Q,vl_Validite_Q,
							vl_V,vl_Validite_V,
							vl_TO,vl_Validite_TO);

					TipcSrvFlush();
				}
				pl_station = pl_station->suivant;
			}
		}
		pl_zone = pl_zone->suivant;
	}

	//envoyer le msg XDM_REGUL_DETECTION a XDG_REGUL_DETECTION
	if (!TipcSrvMsgWrite(vl_nomDG,
			XDM_IdentMsg(XDM_REGUL_DETECTION),
			XDM_FLG_SRVMSGWRITE,
			NULL) )
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "message non envoye");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "message XDM_REGUL_DETECTION envoye sur %s",
				vl_nomDG);
		TipcSrvFlush();
	}


	XZST_03EcritureTrace( XZSTC_DEBUG1, "**horodate simu** : %s",vl_horodate_simu);

	XZST_03EcritureTrace( XZSTC_DEBUG1, "**horodate fin simu : %s",vl_horodate_fin);
	if(vg_HorodateSimu >= vg_horodate_fin_simu)
	{
		vg_Horodate_Cycle = vg_HorodateSimu;
		XZST_03EcritureTrace( XZSTC_INFO, "HORODATE SIMULATION : %lf",vg_Horodate_Cycle);

		XZIT20_Fin_Simulation(vg_machineSimu);
		vg_SimuEnCours = FALSE;

		/*supprimer le timer de lecture des donnees trafic*/
		XZSM_02AnnulerProgrammation(CLE_TIMER_DETECTION, XDG_EMES_RADT_6MN, XDM_EMES_6mn_Chaussee);
		/*supprimer le timer de recherche du scenario ideal*/
		XZSM_02AnnulerProgrammation(CLE_TIMER_DETECTION, XDG_REGUL_SIMU, XDM_REGUL_DETECTION);
	}





}
