/*E*/
/*  Fichier : $Id: ewsp_cmd.c,v 1.0 2022/01/18 08:00:00 pc2dpdy Exp $      Release : $Revision: 1.0 $        Date : $Date: 2022/01/18 08:00:00 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER ewsp_cmd.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module CMD de la tache TEWSP
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE		18/01/22	creation DEM-SAE367
* ABE		09/05/22	Ajout transfere droit d'enrichir au site de la localisation DEM-SAE367
* ABE		13/05/22	Ajout non traitement des messages de cause accident DEM-SAE367
* ABE		25/05/22	Remplacement creation evenement avec lien par changement type de la FMC existante de veh arrete a veh en panne DEM-SAE367
* ABE		31/05/22	Sauvegarde des infos horodate debut, origine creation, commentaire et cause de la FMC existante lors de la mise a jour par AVA DEM-SAE367
* ABE		12/07/22	Ajout d'un commentaire comportant l'id ava a la creation de la FMC DEM-SAE367
* ABE		06/01/22	Ajout d'un libelle d'epanneur pour les depanneurs AVA DEM-SAE450
* LCL		04/10/2023	Adaptation CNA DEM-483 : lecture site principal en base
* PNI 		24/11/22 : Les Fourgons et campingCar VL doivent être considérés en VL DEM-SAE-437
* PNI		25/10/23	: Ajout de l'id mission dans le commantaire, la fin de la mission ne termine pas la FMC, le debut FMC est le debut de la mission SAE-518
* PNI		05/03/23	: NE pas modifier la FMC si elle est terminée SAE-518
* GGY		26/02/24	: Ne pas maj via AVA si la voie est définie en Cone ou croixCone (DEM-589)
------------------------------------------------------*/

/* fichiers inclus */
#include "ewsp_cmd.h"
#include "ewsp_util.h"
#include "xzaa.h"
#include "xzae.h"
#include "xzsm.h"
#include "xzaac.h"
#include "xzdd.h"


#include "xzaec.h"
#include "xzst.h"
#include "xzit.h"
#include "xzia.h"


#include "xzac15.h"
#include "xzae34.h"
#include "xzae159.h"
#include "xzae99.h"
#include "xzae08.h"
#include "xzae11.h"
#include "xzae66.h"
#include "xzae96.h"
#include "xzae15.h"
#include "xzae136.h"
#include "xzae40.h"

/* definitions de constantes */
#define C_RAYON	"rayon"
#define C_PATH_RAYON "config_ava_rayon"
#define NUM_EQUIPEMENT_DP 90
#define NUM_EQUIPEMENT_VC 18
#define	NUM_POSTE_ENRICHISSEUR	94  // SPCI2
#define C_DATE_NULLE	-1
#define C_OPERATEUR 187 //187=ADMNIEP
#define	AVA_VOIE1 1
#define	AVA_VOIE2 2
#define	AVA_VOIE3 3
#define	AVA_VOIE4 4
#define	AVA_VOIE5 5
#define POSTE_CALCUL_PA_DP "PODP1"
#define POSTE_CALCUL_PA_VC "POVC1"
#define OUI_MODIFICATION_RETOUR_ALERTE 2
#define LIBELLE_DEPANNEUR "DEPANNEUR AVA"
/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables externes */
extern XDY_NomMachine	vg_NomMachine;
extern XDY_NomSite		vg_NomSite;
extern XDY_District		vg_NumeroSite;
extern XDY_Mot			vg_SitePrincipal;

/* declaration de variables locales */
T_Liste_AVA* 		pg_AVA_debut = NULL;
T_Liste_AVA* 		pg_AVA_fin = NULL;
XDY_Entier			vg_id_mission=0;
XDY_Texte			pg_localisation_ava="";

/* declaration de fonctions internes */
void ewsp_traite_doublon(XDY_Entier va_NumEvt,XDY_Octet va_CleEvt,XDY_Octet va_TypeEvt,XDY_PR va_PR,XDY_NomAuto va_NomAutoroute,XDY_Sens va_Sens);
int ewsp_lecture_conf_rayon(char  *va_NomFichier, char *va_PathFichier,XDY_Entier *va_rayon);
int ewsp_return_SAE_ptcar(char *va_ava_type_localisant, XDY_Octet *va_ptcar);
int ewsp_return_SAE_typeveh(T_AVA va_ava, XDY_Entier *va_typeveh);
int ewsp_return_SAE_voie(char *va_ava_position_voie, XDY_Entier *va_bau, XDY_Entier *va_voie1, XDY_Entier *va_voie2, XDY_Entier *va_voie3, XDY_Entier *va_voie4);
int ewsp_traite_FMC_AVA(T_AVA pl_AVA, 
						XDY_Entier va_NumEvt, 
						XDY_Octet va_CleEvt,
						XDY_PR va_PR, 
						XDY_Autoroute va_Autoroute, 
						XDY_Sens va_Sens, 
						XDY_Entier va_creation, 
						XDY_Entier va_avec_lien, 
						XDY_Entier va_numAction,
						XDY_Entier va_num_alerte, 
						XDY_Octet va_sit_alerte);
int validerFMC(	XDY_Entier va_numero, 
				XDY_Octet va_cle, 
				XDY_Octet va_type,
				XDY_Autoroute va_autoroute, 
				XDY_Sens va_sens, 
				XDY_PR va_pr,
				XDY_Octet va_ptcar_in, 
				XDY_Entier va_num_ptcar_in,
				XDY_Entier va_numero_cause, 
				XDY_Entier va_cle_cause,
				XDY_Horodate va_horodate_debut, 
				XDY_Entier va_position_voie_ava_bau, 
				XDY_Entier va_position_voie_ava_voie1, 
				XDY_Entier va_position_voie_ava_voie2, 
				XDY_Entier va_position_voie_ava_voie3, 
				XDY_Entier va_position_voie_ava_voie4, 
				XDY_Horodate va_horodate_fin, 
				XDY_Texte va_origine,
				XDY_Entier va_numero_alerte_in,
				XDY_Octet va_sit_alerte_in,
				XDY_Horodate va_horodate_validation,
				XDY_Entier va_creation);

int ewsp_supprime_AVA(XDY_Texte va_id_ava);
int ewsp_ins_liste_AVA (T_AVA va_ava);
/* definition de fonctions externes */


/*X*/
/* 
------------------------------------------------------
* SERVICE RENDU : 
* traitement du message ava de creation
* recherche potentiel de FMC doublon sur la zone, filtre a partir d'un rayon configurable
* proposition du traitement des doublons a l'operateur par message d'alerte
* creation de la FMC AVA
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ewsp_ava_cb_creation( T_IPC_CONN va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA pa_data,
		T_CB_ARG	pa_Arg )

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucune
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  

* 
------------------------------------------------------*/
{
   XZST_03EcritureTrace(XZSTC_FONCTION, "ewsp_ava_creation IN");
  #ifndef _TIBCO_EMS  
	T_STR vl_id_sca;
	T_STR vl_id_ava; 
	T_INT4 vl_id_mission;
	T_STR vl_horodate_debut;
	T_STR vl_horodate_fin;
	T_STR vl_statut_fin;
	T_STR vl_cause_annulation;
	T_STR vl_type_vehicule;
	T_STR vl_nom_autoroute;
	T_STR vl_sens;
	T_INT4 vl_pr;
	T_INT4 vl_distance;
	T_STR vl_type_localisant;
	T_STR vl_id_localisation;
	T_STR vl_position_sur_voies;
	T_STR vl_cause;
	T_STR vl_particularites;
	T_INT4 vl_occupant;
	T_STR vl_commentaires;
	T_STR vl_horodate_debut_prise_en_charge_client;
	T_STR vl_horodate_lancement_mission;
	T_STR vl_origine;
	T_STR vl_complement_origine;
#else
	char vl_id_sca[256];
	char vl_id_ava[256]; 
	XDY_Entier vl_id_mission;
	char vl_horodate_debut[256];
	char vl_horodate_fin[256];
	char vl_statut_fin[256];
	char vl_cause_annulation[256];
	char vl_type_vehicule[256];
	char vl_nom_autoroute[10];
	char vl_sens[20];
	XDY_Entier vl_pr;
	XDY_Entier vl_distance;
	char vl_type_localisant[256];
	char vl_id_localisation[256];
	char vl_position_sur_voies[256];
	char vl_cause[256];
	char vl_particularites[256];
	XDY_Entier vl_occupant;
	char vl_commentaires[256];
	char vl_horodate_debut_prise_en_charge_client[256];
	char vl_horodate_lancement_mission[256];
	char vl_origine[256];
	char vl_complement_origine[256];
#endif

	XDY_Entier vl_dangerosite;
	XDY_Entier vl_rayon=0;
	XDY_Entier va_Resultat_out=0;
	T_AVA pl_AVA;
	XDY_Texte vl_localisation_ava;
	XDY_PR	vl_PR;
	XDY_Autoroute vl_Autoroute;

	if (!TipcMsgRead(pa_data->msg,
		T_IPC_FT_STR,&vl_id_sca,
		T_IPC_FT_STR,&vl_id_ava,
		T_IPC_FT_INT4,&vl_id_mission,
		T_IPC_FT_STR,&vl_horodate_debut,
		T_IPC_FT_STR,&vl_horodate_fin,
		T_IPC_FT_STR,&vl_statut_fin,
		T_IPC_FT_STR,&vl_cause_annulation,
		T_IPC_FT_STR,&vl_type_vehicule,
		T_IPC_FT_STR,&vl_nom_autoroute,
		T_IPC_FT_STR,&vl_sens,
		T_IPC_FT_INT4,&vl_pr, 
		T_IPC_FT_INT4,&vl_distance, 
		T_IPC_FT_STR,&vl_type_localisant,
		T_IPC_FT_STR,&vl_id_localisation,
		T_IPC_FT_STR,&vl_position_sur_voies,
		T_IPC_FT_STR,&vl_cause,
		T_IPC_FT_STR,&vl_particularites,
		T_IPC_FT_INT4,&vl_occupant,
		T_IPC_FT_STR,&vl_commentaires,
		T_IPC_FT_STR,&vl_horodate_debut_prise_en_charge_client,
		T_IPC_FT_STR,&vl_horodate_lancement_mission,
		T_IPC_FT_STR,&vl_origine,
		T_IPC_FT_STR,&vl_complement_origine,
		T_IPC_FT_INT4,&vl_dangerosite,
		NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_ava_cb_creation: pb decodage msg AVA creation");
		return;
	}
	else
		XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_ava_cb_creation: msg AVA creation decode");

		XZST_03EcritureTrace(XZSTC_DEBUG1,"ewsp_ava_cb_creation:");
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_id_sca:%s",vl_id_sca);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_id_ava:%s",vl_id_ava);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_id_mission:%d",vl_id_mission);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_horodate_debut:%s",vl_horodate_debut);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_horodate_fin:%s",vl_horodate_fin);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_statut_fin:%s",vl_statut_fin);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_cause_annulation:%s",vl_cause_annulation);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_type_vehicule:%s",vl_type_vehicule);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_nom_autoroute:%s",vl_nom_autoroute);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_sens:%s",vl_sens);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_pr:%d",vl_pr);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_distance:%d",vl_distance);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_type_localisant:%s",vl_type_localisant);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_id_localisation:%s",vl_id_localisation);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_position_sur_voies:%s",vl_position_sur_voies);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_cause:%s",vl_cause);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_particularites:%s",vl_particularites);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_occupant:%d",vl_occupant);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_commentaires:%s",vl_commentaires);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_horodate_debut_prise_en_charge_client:%s",vl_horodate_debut_prise_en_charge_client);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_horodate_lancement_mission:%s.",vl_horodate_lancement_mission);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_origine:%s",vl_origine);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_complement_origine:%s",vl_complement_origine);
		XZST_03EcritureTrace(XZSTC_DEBUG1," vl_dangerosite:%d",vl_dangerosite);
 
		if(!strcmp(vl_cause,"Accident"))
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_ava_cb_creation Message de type accident, on ne prend pas en compte, arret du traitement");
			return;
		}

		strcpy(pl_AVA.id_sca, vl_id_sca);
		strcpy(pl_AVA.id_ava, vl_id_ava);
		pl_AVA.id_mission = vl_id_mission;
		strcpy(pl_AVA.horodate_debut, vl_horodate_debut);
		if (strlen(vl_horodate_lancement_mission)>17)
			strcpy(pl_AVA.horodate_debut, vl_horodate_lancement_mission); /*SAE-518*/
		strcpy(pl_AVA.horodate_fin, vl_horodate_fin);
		strcpy(pl_AVA.statut_fin, vl_statut_fin);
		strcpy(pl_AVA.cause_annulation, vl_cause_annulation);
		strcpy(pl_AVA.type_vehicule, vl_type_vehicule);
		strcpy(pl_AVA.nom_autoroute, vl_nom_autoroute);
		pl_AVA.sens=strcmp(vl_sens,"Sens1")?2:1;
		if(!strcmp(pl_AVA.nom_autoroute,"A57")) //Inversion du sens pour A57
		{
			if (pl_AVA.sens==XDC_SENS_SUD)
				pl_AVA.sens == XDC_SENS_NORD;
			else
				pl_AVA.sens == XDC_SENS_SUD;
		}
		pl_AVA.pr = vl_pr;
		pl_AVA.distance = vl_distance;
		strcpy(pl_AVA.type_localisant, vl_type_localisant);
		strcpy(pl_AVA.id_localisation, vl_id_localisation);
		strcpy(pl_AVA.position_sur_voies, vl_position_sur_voies);
		strcpy(pl_AVA.cause, vl_cause);
		strcpy(pl_AVA.particularites, vl_particularites);
		pl_AVA.occupant = vl_occupant;
		strcpy(pl_AVA.commentaires, vl_commentaires);
		strcpy(pl_AVA.horodate_debut_prise_en_charge_client, vl_horodate_debut_prise_en_charge_client);
		strcpy(pl_AVA.horodate_lancement_mission, vl_horodate_lancement_mission);
		strcpy(pl_AVA.origine, vl_origine);
		strcpy(pl_AVA.complement_origine, vl_complement_origine);
		pl_AVA.dangerosite = vl_dangerosite;
	
		XZST_03EcritureTrace(XZSTC_DEBUG1,"ewsp_ava_cb_creation:");
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.id_sca:%s",pl_AVA.id_sca);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.id_ava:%s",pl_AVA.id_ava);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.id_mission:%d",pl_AVA.id_mission);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.horodate_debut:%s",pl_AVA.horodate_debut);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.horodate_fin:%s",pl_AVA.horodate_fin);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.statut_fin:%s",pl_AVA.statut_fin);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.cause_annulation:%s",pl_AVA.cause_annulation);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.type_vehicule:%s",pl_AVA.type_vehicule);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.nom_autoroute:%s",pl_AVA.nom_autoroute);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.sens:%d",pl_AVA.sens);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.pr:%d",pl_AVA.pr);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.distance:%d",pl_AVA.distance);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.type_localisant:%s",pl_AVA.type_localisant);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.id_localisation:%s",pl_AVA.id_localisation);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.position_sur_voies:%s",pl_AVA.position_sur_voies);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.cause:%s",pl_AVA.cause);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.particularites:%s",pl_AVA.particularites);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.occupant:%d",pl_AVA.occupant);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.commentaires:%s",pl_AVA.commentaires);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.horodate_debut_prise_en_charge_client:%s",pl_AVA.horodate_debut_prise_en_charge_client);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.horodate_lancement_mission:%s",pl_AVA.horodate_lancement_mission);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.origine:%s",pl_AVA.origine);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.complement_origine:%s",pl_AVA.complement_origine);
		XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.dangerosite:%d",pl_AVA.dangerosite);
	
	/* localisation */
	sprintf(vl_localisation_ava,"%s-%u-%d",pl_AVA.nom_autoroute,pl_AVA.sens,(pl_AVA.pr*1000)+pl_AVA.distance);

	/* Calcul du PR */
	vl_PR = (vl_pr*1000) + vl_distance;

	/* lecture du rayon pour la recherche de doublon */
	if (ewsp_lecture_conf_rayon(C_PATH_RAYON, XDC_PATH_DRA,  &vl_rayon))
		XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_ava_cb_creation: rayon de recherche doublon =%d",vl_rayon);
	else
		XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_ava_cb_creation: pb de lecture du fichier de config rayon =%d",vl_rayon);
	
	vg_id_mission = pl_AVA.id_mission; //pour passage fonction traitement doublon
	strcpy(pg_localisation_ava,vl_localisation_ava); //pour passage fonction traitement doublon

	/* Recherche de FMC veh_ donblon a partir de la localisation AVA dans un rayon */
	xzae159_Rechercher_Veh_dans_rayon(  vl_nom_autoroute ,
                                        vl_PR,
				                        pl_AVA.sens,
                                        vl_rayon,
                                        (XDY_FonctionInt)ewsp_traite_doublon,
						                &va_Resultat_out
										);

	/* Recupere le numero de l'autoroute a partir du nom */
	xzae34_Lire_Numero_Autoroute(vl_nom_autoroute,&vl_Autoroute);

	if (va_Resultat_out==0) //Pas de doublon creation de la FMC AVA
	{
		/* creation ava sans cause et sans lien */
		if (ewsp_traite_FMC_AVA(pl_AVA,
								0,//evt
								0,//cle
								vl_PR,
								vl_Autoroute,
								pl_AVA.sens,
								XDC_OUI ,//creation
								XDC_NON, //modification de la fmc sae existante par ava
								0,
								0, //num alerte
								siteGestionFromNumAuto(vl_Autoroute, vl_PR)  //site alerte
								) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_ava_cb_creation: pb appel a ewsp_traite_FMC_AVA");
		}
	}
	else
	{
		if (ewsp_ins_liste_AVA(pl_AVA)!= XDC_OK)
		{
			XZST_03EcritureTrace(   XZSTC_WARNING, "ewsp_ava_cb_creation : pb insertion de l'element AVA:%s dans la liste AVA",pl_AVA.id_ava);
		}
	}


   XZST_03EcritureTrace(XZSTC_FONCTION, "ewsp_ava_cb_creation OUT");
}

/* 
------------------------------------------------------
* SERVICE RENDU : 
* Traitement du message de mofication AVA
* Recherche de la FMC SAE a partir du numero AVA
* si la fmc n'existe pas : Appel de creation AVA 
* sinon Modification de la FMC AVA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ewsp_ava_cb_modification(T_IPC_CONN va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA pa_data,
		T_CB_ARG	pa_Arg )

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucune
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* 
------------------------------------------------------*/
{
	XZST_03EcritureTrace(XZSTC_FONCTION, "ewsp_ava_cb_modification IN");

	  #ifndef _TIBCO_EMS  
	T_STR vl_id_sca;
	T_STR vl_id_ava; 
	T_INT4 vl_id_mission;
	T_STR vl_horodate_debut;
	T_STR vl_horodate_fin;
	T_STR vl_statut_fin;
	T_STR vl_cause_annulation;
	T_STR vl_type_vehicule;
	T_STR vl_nom_autoroute;
	T_STR vl_sens;
	T_INT4 vl_pr;
	T_INT4 vl_distance;
	T_STR vl_type_localisant;
	T_STR vl_id_localisation;
	T_STR vl_position_sur_voies;
	T_STR vl_cause;
	T_STR vl_particularites;
	T_INT4 vl_occupant;
	T_STR vl_commentaires;
	T_STR vl_horodate_debut_prise_en_charge_client;
	T_STR vl_horodate_lancement_mission;
	T_STR vl_origine;
	T_STR vl_complement_origine;
#else
	char vl_id_sca[256];
	char vl_id_ava[256]; 
	XDY_Entier vl_id_mission;
	char vl_horodate_debut[256];
	char vl_horodate_fin[256];
	char vl_statut_fin[256];
	char vl_cause_annulation[256];
	char vl_type_vehicule[256];
	char vl_nom_autoroute[10];
	char vl_sens[256];
	XDY_Entier vl_pr;
	XDY_Entier vl_distance;
	char vl_type_localisant[256];
	char vl_id_localisation[256];
	char vl_position_sur_voies[256];
	char vl_cause[256];
	char vl_particularites[256];
	XDY_Entier vl_occupant;
	char vl_commentaires[256];
	char vl_horodate_debut_prise_en_charge_client[256];
	char vl_horodate_lancement_mission[256];
	char vl_origine[256];
	char vl_complement_origine[256];
#endif

	XDY_Entier vl_dangerosite;
	XDY_Entier vl_numero_fmc;
	XDY_Octet	vl_cle;
	T_AVA pl_AVA;
	XDY_PR	vl_PR;
	XDY_Entier vl_NumEvtCause=0;
	XDY_Octet vl_CleEvtCause=0;
	XDY_Autoroute vl_Autoroute;
	XDY_Entier vl_numAction=0;

	if (!TipcMsgRead(pa_data->msg,
		T_IPC_FT_STR,&vl_id_sca,
		T_IPC_FT_STR,&vl_id_ava,
		T_IPC_FT_INT4,&vl_id_mission,
		T_IPC_FT_STR,&vl_horodate_debut,
		T_IPC_FT_STR,&vl_horodate_fin,
		T_IPC_FT_STR,&vl_statut_fin,
		T_IPC_FT_STR,&vl_cause_annulation,
		T_IPC_FT_STR,&vl_type_vehicule,
		T_IPC_FT_STR,&vl_nom_autoroute,
		T_IPC_FT_STR,&vl_sens,
		T_IPC_FT_INT4,&vl_pr, 
		T_IPC_FT_INT4,&vl_distance, 
		T_IPC_FT_STR,&vl_type_localisant,
		T_IPC_FT_STR,&vl_id_localisation,
		T_IPC_FT_STR,&vl_position_sur_voies,
		T_IPC_FT_STR,&vl_cause,
		T_IPC_FT_STR,&vl_particularites,
		T_IPC_FT_INT4,&vl_occupant,
		T_IPC_FT_STR,&vl_commentaires,
		T_IPC_FT_STR,&vl_horodate_debut_prise_en_charge_client,
		T_IPC_FT_STR,&vl_horodate_lancement_mission,
		T_IPC_FT_STR,&vl_origine,
		T_IPC_FT_STR,&vl_complement_origine,
		T_IPC_FT_INT4,&vl_dangerosite,
		NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_ava_cb_modification: pb decodage msg AVA creation");
		return;
	}
	else
		XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_ava_cb_modification: msg AVA modification decode");

	strcpy(pl_AVA.id_sca, vl_id_sca);
	strcpy(pl_AVA.id_ava, vl_id_ava);
	pl_AVA.id_mission = vl_id_mission;
	strcpy(pl_AVA.horodate_debut, vl_horodate_debut);
	if (strlen(vl_horodate_lancement_mission)>17)
		strcpy(pl_AVA.horodate_debut, vl_horodate_lancement_mission); /* SAE-518 vl_horodate_lancement_mission*/
	XZST_03EcritureTrace(XZSTC_DEBUG1,"pl_AVA.horodate_debut=%d;vl_horodate_lancement_mission=%s;%d",pl_AVA.horodate_debut,vl_horodate_lancement_mission,strlen(vl_horodate_lancement_mission));
	strcpy(pl_AVA.horodate_fin, vl_horodate_fin);
	strcpy(pl_AVA.statut_fin, vl_statut_fin);
	strcpy(pl_AVA.cause_annulation, vl_cause_annulation);
	strcpy(pl_AVA.type_vehicule, vl_type_vehicule);
	strcpy(pl_AVA.nom_autoroute, vl_nom_autoroute);
	pl_AVA.sens=strcmp(vl_sens,"Sens1")?XDC_SENS_NORD:XDC_SENS_SUD;
	if(!strcmp(pl_AVA.nom_autoroute,"A57")) //Inversion du sens pour A57
	{
		if (pl_AVA.sens==XDC_SENS_SUD)
			pl_AVA.sens == XDC_SENS_NORD;
		else
			pl_AVA.sens == XDC_SENS_SUD;
	}
	pl_AVA.pr = vl_pr;
	pl_AVA.distance = vl_distance;
	strcpy(pl_AVA.type_localisant, vl_type_localisant);
	strcpy(pl_AVA.id_localisation, vl_id_localisation);
	strcpy(pl_AVA.position_sur_voies, vl_position_sur_voies);
	strcpy(pl_AVA.cause, vl_cause);
	strcpy(pl_AVA.particularites, vl_particularites);
	pl_AVA.occupant = vl_occupant;
	strcpy(pl_AVA.commentaires, vl_commentaires);
	strcpy(pl_AVA.horodate_debut_prise_en_charge_client, vl_horodate_debut_prise_en_charge_client);
	strcpy(pl_AVA.horodate_lancement_mission, vl_horodate_lancement_mission);
	strcpy(pl_AVA.origine, vl_origine);
	strcpy(pl_AVA.complement_origine, vl_complement_origine);
	pl_AVA.dangerosite = vl_dangerosite;

	XZST_03EcritureTrace(XZSTC_DEBUG1,"ewsp_ava_cb_modification:");
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.id_sca:%s",pl_AVA.id_sca);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.id_ava:%s",pl_AVA.id_ava);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.id_mission:%d",pl_AVA.id_mission);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.horodate_debut:%s",pl_AVA.horodate_debut);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.horodate_fin:%s",pl_AVA.horodate_fin);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.statut_fin:%s",pl_AVA.statut_fin);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.cause_annulation:%s",pl_AVA.cause_annulation);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.type_vehicule:%s",pl_AVA.type_vehicule);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.nom_autoroute:%s",pl_AVA.nom_autoroute);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.sens:%d",pl_AVA.sens);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.pr:%d",pl_AVA.pr);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.distance:%d",pl_AVA.distance);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.type_localisant:%s",pl_AVA.type_localisant);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.id_localisation:%s",pl_AVA.id_localisation);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.position_sur_voies:%s",pl_AVA.position_sur_voies);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.cause:%s",pl_AVA.cause);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.particularites:%s",pl_AVA.particularites);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.occupant:%d",pl_AVA.occupant);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.commentaires:%s",pl_AVA.commentaires);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.horodate_debut_prise_en_charge_client:%s",pl_AVA.horodate_debut_prise_en_charge_client);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.horodate_lancement_mission:%s",pl_AVA.horodate_lancement_mission);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.origine:%s",pl_AVA.origine);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.complement_origine:%s",pl_AVA.complement_origine);
	XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.dangerosite:%d",pl_AVA.dangerosite);

	if(!strcmp(vl_cause,"Accident"))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_ava_cb_modification Message de type accident, on ne prend pas en compte, arret du traitement");
		return;
	}

	/* Calcul du PR */
	vl_PR = (vl_pr*1000) + vl_distance;

	/* Recupere le numero de l'autoroute a partir du nom */
	xzae34_Lire_Numero_Autoroute(vl_nom_autoroute,&vl_Autoroute);
	XZST_03EcritureTrace(XZSTC_WARNING, "autoroute=%d",vl_Autoroute);

	/* Recherche du nmeruo de la FMC a partir de son numero AVA */
	xzae99_Rechercher_NumFMC_NumAction_par_idAVA(pl_AVA.id_ava,&vl_numero_fmc, &vl_cle, &vl_NumEvtCause, &vl_CleEvtCause, &vl_numAction);
	XZST_03EcritureTrace(XZSTC_WARNING, "Nouvelle FMC : numero=%d cle=%d",vl_numero_fmc,vl_cle);

	XZST_03EcritureTrace(XZSTC_WARNING, "evt:%d/%d cause:%d/%d action :%d localisation :%d-%u-%d",vl_numero_fmc,vl_cle,vl_NumEvtCause,vl_CleEvtCause,vl_numAction,vl_Autoroute,pl_AVA.sens,vl_PR);

	/* traite FMC sans creation (avec lien sinon on ecrase le lien)*/
	if (ewsp_traite_FMC_AVA(pl_AVA,
							vl_numero_fmc,
							vl_cle,
							vl_PR,
							vl_Autoroute,
							pl_AVA.sens,
							XDC_NON,//creation
							XDC_OUI, //modification de la fmc sae existante par ava
							vl_numAction,
							0, //num alerte
							siteGestionFromNumAuto(vl_Autoroute, vl_PR) /* site alerte */
							) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_ava_cb_modification: pb appel a ewsp_traite_FMC_AVA");
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "ewsp_ava_cb_modification OUT");
}

/* 
------------------------------------------------------
* SERVICE RENDU : 
* Traitement du retour des alertes doublon
* creation de la fmc ava avec ou sans lien avec la fmc identifier comme possible doublon
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ewsp_ava_cb_retour_alerte(T_IPC_CONN va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA pa_data,
		T_CB_ARG	pa_Arg )

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucune
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* 
------------------------------------------------------*/
{
#ifndef _TIBCO_EMS  
	T_INT4 vl_id_mission;
	T_INT4 vl_NumEvtSAE=0;
	T_INT4 vl_CleEvtSAE=0;
	T_INT4 vl_modification_existant=0; //booleen pour activer la modification du type la fmc existante
	T_INT4 vl_alerte_numero=0;
	T_INT4 vl_alerte_sit=0;
#else
	XDY_Entier vl_id_mission;
	XDY_Entier vl_NumEvtSAE=0;
	XDY_Entier vl_CleEvtSAE=0;
	XDY_Entier vl_modification_existant=0; //booleen pour activer la modification du type la fmc existante
	XDY_Entier vl_alerte_numero=0;
	XDY_Entier vl_alerte_sit=0;
#endif
	T_Liste_AVA	*pl_AVA;
	XDY_Entier vl_NumEvt;
	XDY_Entier vl_CleEvt;
	XDY_PR	vl_PR;
	XDY_Autoroute vl_Autoroute;	

	XDY_Entier  vl_numero_fmc;
	XDY_Octet	vl_cle;
	XDY_Entier  vl_numAction=0;
	XDY_Octet   vl_cle_evt_cause=0;

	XZST_03EcritureTrace(XZSTC_FONCTION, "ewsp_ava_cb_retour_alerte IN");

	if (!TipcMsgRead(pa_data->msg,
		T_IPC_FT_INT4,&vl_id_mission,
		T_IPC_FT_INT4,&vl_NumEvtSAE, //La fmc potentiellement en doublon
		T_IPC_FT_INT4,&vl_CleEvtSAE,
		T_IPC_FT_INT4,&vl_modification_existant,
		T_IPC_FT_INT4,&vl_alerte_numero,
		T_IPC_FT_INT4,&vl_alerte_sit,
		NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_ava_cb_retour_alerte: pb decodage msg AVA retour alerte");
		return;
	}
	else
		XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_ava_cb_retour_alerte: msg AVA retour alerte decode Id mission:%d NumEvt:%d CleEvt:%d modification =%d Numero alerte=%d Site alerte=%d",
			vl_id_mission,vl_NumEvtSAE,vl_CleEvtSAE,vl_modification_existant,vl_alerte_numero,vl_alerte_sit);
	
	for (pl_AVA = pg_AVA_debut; (pl_AVA != NULL) ; pl_AVA = pl_AVA -> suivant)
	{	
		XZST_03EcritureTrace ( XZSTC_FONCTION, "ewsp_ava_cb_retour_alerte pl_ava.ava.id_mission:%d",pl_AVA->ava.id_mission);
		if(pl_AVA->ava.id_mission==vl_id_mission)
		{
			XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp_ava_cb_retour_alerte Element AVA trouve dans la liste");
			break;
		}
	}
	if (pl_AVA == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_ava_cb_retour_alerte Element AVA non trouve dans la liste");
		return;
	}

	/* Calcul du PR */
	vl_PR = (pl_AVA->ava.pr*1000) + pl_AVA->ava.distance;

	/* Recupere le numero de l'autoroute a partir du nom */
	xzae34_Lire_Numero_Autoroute(pl_AVA->ava.nom_autoroute,&vl_Autoroute);

	/* creation ava */
	if (ewsp_traite_FMC_AVA(pl_AVA->ava,
						vl_modification_existant?vl_NumEvtSAE:0,//evt SAE
						vl_modification_existant?vl_CleEvtSAE:0,//cle SAE
						vl_PR,
						vl_Autoroute,
						(XDY_Sens)pl_AVA->ava.sens,
						vl_modification_existant?XDC_NON:XDC_OUI, //On ne cree que si on decide de ne pas faire evoluer le type de la fmc existante
						vl_modification_existant?OUI_MODIFICATION_RETOUR_ALERTE:0, //Si==1 on met 2 pour creer le commentaire avec le num id_ava
						0,
						vl_alerte_numero,
						(XDY_Octet)vl_alerte_sit) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_ava_cb_retour_alerte: pb appel a ewsp_traite_FMC_AVA");
	}

	ewsp_supprime_AVA(pl_AVA->ava.id_ava); //Suppression de l'element dans la liste

	XZST_03EcritureTrace(XZSTC_FONCTION, "ewsp_ava_cb_retour_alerte OUT");

	return;
}

/* 
------------------------------------------------------
* SERVICE RENDU : 
* Traitement des FMC identifiees comme potentiel doublon
* envoie d'une alerte par FMC remonte a l'operateur pour choix si creation de la fmc AVA avec lien sur doublon ou non
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ewsp_traite_doublon(XDY_Entier va_NumEvt,XDY_Octet va_CleEvt,XDY_Octet va_TypeEvt,XDY_PR va_PR,XDY_NomAuto va_NomAutoroute,XDY_Sens va_Sens)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucune
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* 
------------------------------------------------------*/
{
	int				vl_JourSemaine;
	XDY_Horodate	vl_HorodateSec;
	XDY_Texte		vl_str;
	XDY_Texte		vl_localisation;
	T_Liste_AVA		*pl_AVA ;
	XDY_District 	vl_SiteGestion;
	XDY_Eqt			vl_EqtAlerte;

	XZST_03EcritureTrace(XZSTC_FONCTION, "ewsp_traite_doublon IN");

	/* concat de la localisation */	
	rtrim(va_NomAutoroute);

	/* Recherche du district de localisation */
	vl_SiteGestion = siteGestionFromNomAuto(va_NomAutoroute, va_PR);
	if ( vl_SiteGestion == XDC_DP )
		vl_EqtAlerte = NUM_EQUIPEMENT_DP;
	else
		vl_EqtAlerte = NUM_EQUIPEMENT_VC;

	sprintf(vl_localisation,"%s-%u-%d",va_NomAutoroute,va_Sens,va_PR); 
	
	/* creation du message d'alerte */
	sprintf(vl_str,"2 FMC proches: FMC %d/%d %s - dep. AVA %d %s",va_NumEvt,va_CleEvt,vl_localisation,vg_id_mission,pg_localisation_ava);
	
	XZST_03EcritureTrace(XZSTC_WARNING, "Message alerte sur eqt %d :\n%s",vl_EqtAlerte,vl_str);
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);	
	if (xzaa01_Creer_Alerte(vl_HorodateSec,
			XDC_EQT_MAC,  
			vl_EqtAlerte,
			XZAAC_ALERTE_AVA_POTENTIEL_DOUBLON,
			vl_str,
			vl_SiteGestion) != XDC_OK )
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_traite_doublon envoie d'alerte");
		}
		else
			XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_traite_doublon OUT");


	XZST_03EcritureTrace(XZSTC_FONCTION, "ewsp_traite_doublon OUT");
}

/* 
------------------------------------------------------
* SERVICE RENDU : 
* Lecture du fichier de configuration rayon pour recherche potentiel doublon dans un rayon configurable
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ewsp_lecture_conf_rayon(char  *va_NomFichier, char *va_PathFichier,XDY_Entier *va_rayon)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* 
------------------------------------------------------*/

{
	FILE *pFile;
	fpos_t position;
	char vl_line_output[200];
	char vl_ligne[200];		
	int cpt;	
	vl_line_output[0]='\0';
	char PathFichier[256]="";
	
	/* concat dossier + fichier */
	strcat(PathFichier,(const char*)va_PathFichier);
	strcat(PathFichier,"/");
	strcat(PathFichier,(const char*)va_NomFichier);	

	XZST_03EcritureTrace(XZSTC_FONCTION,"ewsp_lecture_conf_rayon: IN");

	/* ouvre le fichier xml */
   	if ( (pFile = fopen(PathFichier, "r+"))== NULL )
	{
		XZST_03EcritureTrace(XZSTC_FATAL, "ewsp_lecture_conf_rayon erreur de lecture %s !",PathFichier);
		return -1;
	}	
	
	while ( fgets(vl_ligne, 200, pFile) != NULL) 
	{
			if ( vl_ligne[0] == '#' )
			continue;

		/* si le nom correspond au destinataire en parametre */
		if (!strncmp(vl_ligne,C_RAYON,5))
		{				
			if (sscanf(vl_ligne,"rayon %d",va_rayon)==1)
			{			
				fclose (pFile);
				XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_lecture_conf_rayon va_rayon=%d",*va_rayon);
				XZST_03EcritureTrace(XZSTC_FONCTION,"ewsp_lecture_conf_rayon: OUT");
				return 1;
			}
		}		

	}

	/* si on ne trouve pas le destinataire dans le fichier */
	XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_lecture_conf_rayon rayon non trouvee");		

	fclose(pFile);	
	XZST_03EcritureTrace(XZSTC_FONCTION,"ewsp_lecture_conf_rayon: OUT");
    
	return -2;		
}

/* 
------------------------------------------------------
* SERVICE RENDU : 
* Ajout d'un element AVA dans la liste
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ewsp_ins_liste_AVA (T_AVA va_ava)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* 
------------------------------------------------------*/
{
	T_Liste_AVA *pl_NewAVA;
	T_Liste_AVA *pl_AVA;
	
	XZST_03EcritureTrace ( XZSTC_FONCTION,	"IN: ewsp_ins_liste_AVA : numeroAVA=%s, localisation=%s-%u-%d",
			va_ava.id_ava,
			va_ava.nom_autoroute,
			va_ava.sens,
			va_ava.pr);

	/*A Allocation memoire pour la nouvelle structure */
	if ( (pl_NewAVA = (T_Liste_AVA *) malloc ( sizeof(T_Liste_AVA))) == NULL)
	{
		/* Ecriture trace */
		XZST_03EcritureTrace ( XZSTC_WARNING, " OUT: ewsp_ins_liste_AVA Allocation memoire impossible");
		return XDC_NOK;
	}

	/*A Initialise la structure avec les valeurs passees en parametre */
	strcpy(pl_NewAVA->ava.id_sca, va_ava.id_sca);
	strcpy(pl_NewAVA->ava.id_ava, va_ava.id_ava);
	pl_NewAVA->ava.id_mission = va_ava.id_mission;
	strcpy(pl_NewAVA->ava.horodate_debut, va_ava.horodate_debut);
	strcpy(pl_NewAVA->ava.horodate_fin, va_ava.horodate_fin);
	strcpy(pl_NewAVA->ava.statut_fin, va_ava.statut_fin);
	strcpy(pl_NewAVA->ava.cause_annulation, va_ava.cause_annulation);
	strcpy(pl_NewAVA->ava.type_vehicule, va_ava.type_vehicule);
	strcpy(pl_NewAVA->ava.nom_autoroute, va_ava.nom_autoroute);
	pl_NewAVA->ava.sens = (XDY_Sens)va_ava.sens;
	pl_NewAVA->ava.pr = va_ava.pr;
	pl_NewAVA->ava.distance = va_ava.distance;
	strcpy(pl_NewAVA->ava.type_localisant, va_ava.type_localisant);
	strcpy(pl_NewAVA->ava.id_localisation, va_ava.id_localisation);
	strcpy(pl_NewAVA->ava.position_sur_voies, va_ava.position_sur_voies);
	strcpy(pl_NewAVA->ava.cause, va_ava.cause);
	strcpy(pl_NewAVA->ava.particularites, va_ava.particularites);
	pl_NewAVA->ava.occupant = va_ava.occupant;
	strcpy(pl_NewAVA->ava.commentaires, va_ava.commentaires);
	strcpy(pl_NewAVA->ava.horodate_debut_prise_en_charge_client, va_ava.horodate_debut_prise_en_charge_client);
	strcpy(pl_NewAVA->ava.horodate_lancement_mission, va_ava.horodate_lancement_mission);
	strcpy(pl_NewAVA->ava.origine, va_ava.origine);
	strcpy(pl_NewAVA->ava.complement_origine, va_ava.complement_origine);
	pl_NewAVA->ava.dangerosite = va_ava.dangerosite;

	pl_NewAVA->suivant = NULL;

	/* Ajout de l element dans la liste chainee */
	if(pg_AVA_debut == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"First");
		/* premier element de la liste */
		pg_AVA_debut = pl_NewAVA;
      	pl_NewAVA->precedent = NULL;
	}
	else
	{     
	 /* on relie le dernier element de la liste a ce nouvel element */
      pg_AVA_fin->suivant = pl_NewAVA; /* chainage avant */
      pl_NewAVA->precedent = pg_AVA_fin; /* chainage arriere */
   	}
    pg_AVA_fin = pl_NewAVA;

		/* debug remove */
		for (pl_AVA = pg_AVA_debut; (pl_AVA != NULL) ; pl_AVA = pl_AVA -> suivant)
		{	
				XZST_03EcritureTrace(XZSTC_DEBUG1,	"ewsp_ins_liste_AVA print element ava ajoute");
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.id_sca:%s",pl_AVA->ava.id_sca);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA.id_ava:%s",pl_AVA->ava.id_ava);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.id_mission:%d",pl_AVA->ava.id_mission);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.horodate_debut:%s",pl_AVA->ava.horodate_debut);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.horodate_fin:%s",pl_AVA->ava.horodate_fin);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.statut_fin:%s",pl_AVA->ava.statut_fin);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.cause_annulation:%s",pl_AVA->ava.cause_annulation);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.type_vehicule:%s",pl_AVA->ava.type_vehicule);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.nom_autoroute:%s",pl_AVA->ava.nom_autoroute);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.sens:%d",pl_AVA->ava.sens);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.pr:%d",pl_AVA->ava.pr);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.distance:%d",pl_AVA->ava.distance);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.type_localisant:%s",pl_AVA->ava.type_localisant);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.id_localisation:%s",pl_AVA->ava.id_localisation);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.position_sur_voies:%s",pl_AVA->ava.position_sur_voies);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.cause:%s",pl_AVA->ava.cause);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.particularites:%s",pl_AVA->ava.particularites);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.occupant:%d",pl_AVA->ava.occupant);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.commentaires:%s",pl_AVA->ava.commentaires);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.horodate_debut_prise_en_charge_client:%s",pl_AVA->ava.horodate_debut_prise_en_charge_client);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.horodate_lancement_mission:%s",pl_AVA->ava.horodate_lancement_mission);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.origine:%s",pl_AVA->ava.origine);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.complement_origine:%s",pl_AVA->ava.complement_origine);
				XZST_03EcritureTrace(XZSTC_DEBUG1," pl_AVA->ava.dangerosite:%d",pl_AVA->ava.dangerosite);
		}

	XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp_ins_liste_AVA OUT");
}


/* 
------------------------------------------------------
* SERVICE RENDU : 
* supprime d'un element AVA dans la liste
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int ewsp_supprime_AVA(XDY_Texte va_id_ava)

/*
* ARGUMENTS EN ENTREE :
* XDY_Entier XDY_Texte : l'id de l'element a supprimer
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* 
------------------------------------------------------*/
{
	T_Liste_AVA *pl_AVA	= NULL;
	int vl_AvaTrouve = XDC_FAUX;

	XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp_supprime_AVA IN");
	/* recherche du AVA dans la liste */
	for (pl_AVA = pg_AVA_debut; 
	(pl_AVA != NULL) && (!vl_AvaTrouve);
	pl_AVA = pl_AVA -> suivant)
	{
		if (!strcmp(pl_AVA->ava.id_ava,va_id_ava))
		{
			/* suppression du AVA de la liste */
			if (pl_AVA == pg_AVA_debut)
			{
				pg_AVA_debut = pl_AVA -> suivant;
			}
			else
			{
			/* on relie l'element precedent a l'element suivant */
				((T_Liste_AVA *)(pl_AVA -> precedent)) -> suivant = pl_AVA -> suivant; /* chainage avant */
				/* si element a suprimer n est pas le dernier de la liste */
				/*    on recalcule le chainage arriere */
				/* sinon maj de pg_AVA_fin */
				if (pl_AVA != pg_AVA_fin)
					((T_Liste_AVA *)(pl_AVA -> suivant)) -> precedent = pl_AVA -> precedent; /* chainage arriere */
				else
					pg_AVA_fin = pl_AVA -> precedent;
			}
			free(pl_AVA);
			vl_AvaTrouve = XDC_VRAI;
		}
	}

	XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp_supprime_AVA OUT");
	return(XDC_OK);
}	/* ewsp_supprimeAVA */	


/* 
------------------------------------------------------
* SERVICE RENDU : 
* libere la memoire AVA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
void ewsp_ava_freemem()

/*
* ARGUMENTS EN ENTREE :
* 	Aucun
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* 
------------------------------------------------------*/
{
	T_Liste_AVA	* pl_AVA ;

	XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp_ava_freemem IN");
	for (pl_AVA = pg_AVA_debut; (pl_AVA != NULL) ; pl_AVA = pl_AVA -> suivant)
	{	
		XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_ava_freemem Free ava:\n   id_sca:%s\n   id_ava:%s\n   id_mission:%d\n   horodate_debut:%s\n   horodate_fin:%s\n   statut_fin:%s\n   cause_annulation:%s\n   type_vehicule:%s\n   nom_autoroute:%s\n   sens:%s\n   pr:%d\n   distance:%d\n   type_localisant:%s\n   id_localisation:%s\n   position_sur_voies:%s\n   cause:%s\n   particularites:%s\n   occupant:%d\n   commentaires:%s\n   horodate_debut_prise_en_charge_client:%s\n   horodate_lancement_mission:%s\n   origine:%s\n   complement_origine:%s\n   dangerosite:%d",
			pl_AVA->ava.id_sca,pl_AVA->ava.id_ava,pl_AVA->ava.id_mission,pl_AVA->ava.horodate_debut,pl_AVA->ava.horodate_fin,pl_AVA->ava.statut_fin,pl_AVA->ava.cause_annulation,pl_AVA->ava.type_vehicule,pl_AVA->ava.nom_autoroute,pl_AVA->ava.sens,pl_AVA->ava.pr,pl_AVA->ava.distance,pl_AVA->ava.type_localisant,
			pl_AVA->ava.id_localisation,pl_AVA->ava.position_sur_voies,pl_AVA->ava.cause,pl_AVA->ava.particularites,pl_AVA->ava.occupant,pl_AVA->ava.commentaires,pl_AVA->ava.horodate_debut_prise_en_charge_client,pl_AVA->ava.horodate_lancement_mission,pl_AVA->ava.origine,pl_AVA->ava.complement_origine,pl_AVA->ava.dangerosite);
		free(pl_AVA);
	}	
	XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp_ava_freemem OUT");
}


/* 
------------------------------------------------------
* SERVICE RENDU : 
* Valide une FMC en y ajoutant la cause et la consequence
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int validerFMC(	XDY_Entier va_numero, 
				XDY_Octet va_cle, 
				XDY_Octet va_type,
				XDY_Autoroute va_autoroute, 
				XDY_Sens va_sens, 
				XDY_PR va_pr,
				XDY_Octet va_ptcar_in, 
				XDY_Entier va_num_ptcar_in,
				XDY_Entier va_numero_cause, 
				XDY_Entier va_cle_cause,
				XDY_Horodate va_horodate_debut, 
				XDY_Entier va_position_voie_ava_bau, 
				XDY_Entier va_position_voie_ava_voie1, 
				XDY_Entier va_position_voie_ava_voie2, 
				XDY_Entier va_position_voie_ava_voie3, 
				XDY_Entier va_position_voie_ava_voie4, 
				XDY_Horodate va_horodate_fin, 
				XDY_Texte va_origine,
				XDY_Entier va_numero_alerte_in,
				XDY_Octet va_sit_alerte_in,
				XDY_Horodate va_horodate_validation,
				XDY_Entier va_creation)

/*
* ARGUMENTS EN ENTREE :
* 	XDY_Entier va_numero
	XDY_Octet va_type
	XDY_Autoroute va_autoroute
	XDY_Sens va_sens 
	XDY_PR va_pr
	XDY_Octet va_ptcar_in
	XDY_Entier va_numero_cause
	XDY_Entier va_cle_cause
	XDY_Horodate va_horodate
	XDY_Entier va_position_voie_ava_bau : XDC_VOIE_BAU_NORMALE / XDC_VOIE_BLOQUEE
	XDY_Entier va_position_voie_ava_voiex : XDC_VOIE_SENS_NORMAL / XDC_VOIE_BLOQUEE
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* 
------------------------------------------------------*/
{
	XDY_Octet vl_autoroute,vl_sens,vl_ptcar,vl_typeptcar,vl_position;
	XDY_PR vl_pr;
	XDY_Octet vl_vr,vl_vm2,vl_vm1,vl_vl, vl_bau, vl_vri,vl_vm2i,vl_vm1i,vl_vli, vl_baui;
	XDY_Octet vl_bauet, vl_baueti,vl_dis, vl_disr;
	XDY_Entier vl_num_cause;
	int vl_cle_cause;
	XDY_Entier vl_fiche_appel_out;
	int vl_res=0, vl_ret=0;
	XDY_Octet vl_sens_in;
	XDY_Entier vl_poste_enrichisseur;
	int nb_entree, nb_sorties;
	XDY_Octet va_ptcar_FMC; 
	XDY_Entier va_num_ptcar_FMC;

	XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp valider FMC IN");
	/*recuperation de la config des voies */
	
	vl_sens_in=va_sens;
	if(va_ptcar_in == XDC_POINT_CARACT_BIFURCATION) //Si autre que section courante
	{
		if(va_sens==XDC_SENS_SUD)
			vl_sens_in=XDC_SENS_SORTIE_SUD;
		if(va_sens==XDC_SENS_NORD)
			vl_sens_in=XDC_SENS_SORTIE_NORD;
	}

	if (va_creation)
	{
		vl_ret = XZAE66_Lire_Config_Voies (	va_autoroute, va_pr, vl_sens_in, va_num_ptcar_in, va_ptcar_in, 0, va_type,
											&vl_autoroute, &vl_pr, &vl_sens, &vl_ptcar, &vl_typeptcar, &vl_position,
											&vl_vr, &vl_vm2, &vl_vm1, &vl_vl, &vl_bau, &vl_vri, &vl_vm2i, &vl_vm1i, &vl_vli, &vl_baui,
											&vl_bauet, &vl_baueti, &vl_dis, &vl_disr,&vl_res);

		XZST_03EcritureTrace ( XZSTC_WARNING,"ewsp valider FMC XZAE66 res=%d ret=%d",vl_res, vl_ret);
	}
	else 
	{
		vl_ret = XZAE220_Lire_Config_Voies (va_numero, va_cle,
											&vl_vr, &vl_vm2, &vl_vm1, &vl_vl, &vl_bau, &vl_vri, &vl_vm2i, &vl_vm1i, &vl_vli, &vl_baui,
											&va_ptcar_FMC, &va_num_ptcar_FMC, &nb_entree, &nb_sorties);
	
		XZST_03EcritureTrace (XZSTC_WARNING, "XZAE220_Lire_Config_Voies");		
	}

	XZST_03EcritureTrace (XZSTC_WARNING, "Etat Voies Avant modif AVA : vl_vr=%d. vl_vm2=%d. vl_vm1=%d. vl_vl=%d. vl_bau=%d. vl_vri=%d. vl_vm2i=%d. vl_vm1i=%d. vl_vli=%d. vl_baui=%d.",
							vl_vr, vl_vm2, vl_vm1, vl_vl, vl_bau, vl_vri, vl_vm2i, vl_vm1i, vl_vli, vl_baui);

	/* application de l'etat de la voie AVA */
	/* Si la voie en cours n'est pas Cone (=5) ou CroixCone (=6) */
	if (va_position_voie_ava_voie1==XDC_VOIE_BLOQUEE)//Voie 1
	{	
		if ((vl_vl != XDC_VOIE_NEUTRALISEE) && (vl_vl != XDC_VOIE_BLOQUEE_NEUTRALISEE))
		{
			vl_vl=XDC_VOIE_BLOQUEE;
		}
	}
	
	if (va_position_voie_ava_voie2==XDC_VOIE_BLOQUEE) //Voie 2
	{
		if(vl_vm1 == XDC_VOIE_INEXISTANTE && vl_vm2 == XDC_VOIE_INEXISTANTE)
		{
			if ((vl_vr != XDC_VOIE_NEUTRALISEE) && (vl_vr != XDC_VOIE_BLOQUEE_NEUTRALISEE))
			{
				vl_vr=XDC_VOIE_BLOQUEE;
			}
		}
		else
		{
			if ((vl_vm1 != XDC_VOIE_NEUTRALISEE) && (vl_vm1 != XDC_VOIE_BLOQUEE_NEUTRALISEE))
			{
				vl_vm1=XDC_VOIE_BLOQUEE;
			}
		}
	}
	
	if (va_position_voie_ava_voie3==XDC_VOIE_BLOQUEE) //Voie 3
	{
		if(vl_vm2 == XDC_VOIE_INEXISTANTE)
		{
			if ((vl_vr != XDC_VOIE_NEUTRALISEE) && (vl_vr != XDC_VOIE_BLOQUEE_NEUTRALISEE))
			{
				vl_vr=XDC_VOIE_BLOQUEE;
			}
		}
		else
		{
			if ((vl_vm2 != XDC_VOIE_NEUTRALISEE) && (vl_vm2 != XDC_VOIE_BLOQUEE_NEUTRALISEE))
			{
				vl_vm2=XDC_VOIE_BLOQUEE;
			}
		}
	}
	
	if (va_position_voie_ava_voie4==XDC_VOIE_BLOQUEE) //Voie 4
	{
		if ((vl_vr != XDC_VOIE_NEUTRALISEE) && (vl_vr != XDC_VOIE_BLOQUEE_NEUTRALISEE))
		{
			vl_vr=XDC_VOIE_BLOQUEE;
		}
	}
	
	if (va_position_voie_ava_bau==XDC_VOIE_BLOQUEE) //BAU, fosse, refuge
	{
		if ((vl_bau != XDC_VOIE_NEUTRALISEE) && (vl_bau != XDC_VOIE_BLOQUEE_NEUTRALISEE))
		{
			vl_bau=XDC_VOIE_BLOQUEE;
		}
	}

	XZST_03EcritureTrace (XZSTC_WARNING, "Etat Voies Apres modif AVA : vl_vr=%d. vl_vm2=%d. vl_vm1=%d. vl_vl=%d. vl_bau=%d. vl_vri=%d. vl_vm2i=%d. vl_vm1i=%d. vl_vli=%d. vl_baui=%d.",
									 vl_vr, vl_vm2, vl_vm1, vl_vl, vl_bau, vl_vri, vl_vm2i, vl_vm1i, vl_vli, vl_baui);

	vl_poste_enrichisseur = NUM_POSTE_ENRICHISSEUR;

	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",va_numero);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",va_cle);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",vl_poste_enrichisseur);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",XDC_FMC_ETAT_TRAI);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",va_type);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%lf",(XDY_Horodate)C_DATE_NULLE);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%lf",va_horodate_debut);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%lf",(XDY_Horodate)C_DATE_NULLE);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%lf",(va_horodate_fin == 0)?(XDY_Horodate) C_DATE_NULLE:va_horodate_fin);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%lf",(va_horodate_fin > va_horodate_debut) ?va_horodate_fin:va_horodate_debut);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",va_numero_cause); /* num cause*/
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",va_cle_cause); /*cle cause*/
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",va_numero_alerte_in); /*num alerte*/
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%s",va_origine);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",vl_poste_enrichisseur);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",XZAEC_FMC_CONFIRMEE);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",va_autoroute);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",va_pr);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",vl_sens_in);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",va_num_ptcar_in);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",va_ptcar_in);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",0);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",vl_vr );
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",vl_vm2 );
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",vl_vm1 );
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",vl_vl );
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",vl_bau );
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",vl_vri );
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",vl_vm2i);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",vl_vm1i);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",vl_vli );
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",vl_baui);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",XDC_FAUX);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",XDC_FAUX);
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",0); /*degats domaine*/
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",va_type); /*type anterieur*/
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",0); /*fausse alerte*/
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",0); /*longueur*/
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%s",""); /*com evt init*/
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",va_sit_alerte_in); /*site alerte*/
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",0); /*majeur*/
	XZST_03EcritureTrace(   XZSTC_DEBUG1, "%d",0); /*ecoulement*/

	if (XZAE11_Valider_Fiche_MC(va_numero,
								va_cle,
								vl_poste_enrichisseur,
								XDC_FMC_ETAT_TRAI,
								XZAEC_FMC_VehPanne,
								(XDY_Horodate)C_DATE_NULLE,
								va_horodate_debut,
								(XDY_Horodate)C_DATE_NULLE,
								(va_horodate_fin == 0)?(XDY_Horodate) C_DATE_NULLE:va_horodate_fin,
								va_horodate_validation,
								va_numero_cause, /* num cause*/
								va_cle_cause, /*cle cause*/
								va_numero_alerte_in, /*num alerte*/
								va_origine,//Origine_creation
								vl_poste_enrichisseur,
								XZAEC_FMC_CONFIRMEE,
								va_autoroute,
								va_pr,
								(XDY_Sens)vl_sens_in,
								va_num_ptcar_in, //type pt cara
								va_ptcar_in,//numero point cara
								0,
								vl_vr ,
								vl_vm2 ,
								vl_vm1 ,
								vl_vl ,
								vl_bau ,
								vl_vri ,
								vl_vm2i,
								vl_vm1i,
								vl_vli ,
								vl_baui,
								XDC_FAUX,
								XDC_FAUX,
								0, /*degats domaine*/
								va_type, /*type anterieur*/
								0, /*fausse alerte*/
								0, /*longueur*/
								"", /*com evt init*/
								va_sit_alerte_in, /*site alerte*/
								0, /*majeur*/
								0  /*ecoulement*/ ) != XDC_OK) 
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "pb appel XZAE11_Valider_Fiche_MC queue");
		XZST_03EcritureTrace( XZSTC_FONCTION, "ewsp valider FMC OUT");
		return (XDC_NOK);
	}	
	XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp valider FMC OUT");
	return (XDC_OK);
}

/* 
------------------------------------------------------
* SERVICE RENDU : 
* Return l'enum du pt caracteristique a partir du type localisant d'ava
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ewsp_return_SAE_ptcar(char *va_ava_type_localisant, XDY_Octet *va_ptcar)

/*
* ARGUMENTS EN ENTREE :
* 	
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* 
------------------------------------------------------*/
{

	XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp_return_SAE_ptcar IN");

	if (!strcmp(va_ava_type_localisant,"SectionCourante"))
	{
		*va_ptcar = XDC_POINT_CARACT_AUCUN;
	}
	/*else if (!strcmp(va_ava_type_localisant,"BretelleEchangeur"))
	{
		*va_ptcar = XDC_POINT_CARACT_ECHANGEUR;
	}*/ //Seul bifurcation gere, demande d'ESCOTA
	else if (!strcmp(va_ava_type_localisant,"BretelleBifurcation"))
	{
		*va_ptcar = XDC_POINT_CARACT_BIFURCATION;
	}
	/*else if (!strcmp(va_ava_type_localisant,"BretelleAire"))
	{
		*va_ptcar = XDC_POINT_CARACT_AIRE;
	}*/ ////Seul bifurcation gere, demande d'ESCOTA
	else 
		return XDC_NOK;
	
	
	XZST_03EcritureTrace ( XZSTC_WARNING,	"ewsp_return_SAE_ptcar AVA type localisant = %s, SAE pt carac = %d",va_ava_type_localisant,*va_ptcar);
	XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp_return_SAE_ptcar OUT");
	return XDC_OK;
}

/* 
------------------------------------------------------
* SERVICE RENDU : 
* Return l'enum du type de veh a partir du type veh d'ava
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int ewsp_return_SAE_typeveh(T_AVA va_ava, XDY_Entier *va_typeveh)

/*
* ARGUMENTS EN ENTREE :
* 	
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* 
------------------------------------------------------*/
{

	XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp_return_SAE_typeveh IN");

	if (strstr(va_ava.particularites,"ObjetTracte"))
	{
		*va_typeveh = XZAEC_VEH_Remorque;
	}
	else if ((!strcmp(va_ava.type_vehicule,"VL")) || !strcmp(va_ava.type_vehicule,"Fourgon") || !strcmp(va_ava.type_vehicule,"CCVL")) /*SAE-437*/
	{
		*va_typeveh = XZAEC_VEH_VL;
	}
	else if (!strcmp(va_ava.type_vehicule,"Bus"))
	{
		*va_typeveh = XZAEC_VEH_Car;
	}
	else if (!strcmp(va_ava.type_vehicule,"Moto"))
	{
		*va_typeveh = XZAEC_VEH_Moto;
	}	
	else if (!strcmp(va_ava.type_vehicule,"PL")  || !strcmp(va_ava.type_vehicule,"CCPL"))
	{
		*va_typeveh = XZAEC_VEH_PL;
	}
	else 
		return XDC_NOK;

	
	
	XZST_03EcritureTrace ( XZSTC_WARNING,	"ewsp_return_SAE_typeveh AVA type veh = %s, SAE type veh = %d",va_ava.type_vehicule,*va_typeveh);
	XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp_return_SAE_typeveh OUT");
	return XDC_OK;
}

/* 
------------------------------------------------------
* SERVICE RENDU : 
* Return l'enum de la voie impacte a partir du poisition voie ava
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int ewsp_return_SAE_voie(char *va_ava_position_voie, 
						XDY_Entier *va_bau, 
						XDY_Entier *va_voie1, 
						XDY_Entier *va_voie2, 
						XDY_Entier *va_voie3, 
						XDY_Entier *va_voie4)

/*
* ARGUMENTS EN ENTREE :
* 	
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* 
------------------------------------------------------*/
{

	XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp_return_SAE_voie IN");

	*va_bau = XDC_VOIE_BAU_NORMALE;
	*va_voie1 = XDC_VOIE_SENS_NORMAL;
	*va_voie2 = XDC_VOIE_SENS_NORMAL;
	*va_voie3 = XDC_VOIE_SENS_NORMAL;
	*va_voie4 = XDC_VOIE_SENS_NORMAL;

	if (strstr(va_ava_position_voie,"Inconnue"))
	{
		/* On ne fait rien */
	}
	if (strstr(va_ava_position_voie,"BAU"))
	{
		*va_bau = XDC_VOIE_BLOQUEE;
	}
	if (strstr(va_ava_position_voie,"Refuge"))
	{
		*va_bau = XDC_VOIE_BLOQUEE;
	}
	if (strstr(va_ava_position_voie,"Fosse"))
	{
		*va_bau = XDC_VOIE_BLOQUEE;
	}
	if (strstr(va_ava_position_voie,"Voie1"))
	{
		*va_voie1 = XDC_VOIE_BLOQUEE;
	}
	if (strstr(va_ava_position_voie,"Voie2"))
	{
		*va_voie2 = XDC_VOIE_BLOQUEE;
	}
	if (strstr(va_ava_position_voie,"Voie3"))
	{
		*va_voie3= XDC_VOIE_BLOQUEE;
	}	
	if (strstr(va_ava_position_voie,"Voie4"))
	{
		*va_voie4 = XDC_VOIE_BLOQUEE;
	}
	
	
	XZST_03EcritureTrace ( XZSTC_WARNING,	"ewsp_return_SAE_voie AVA position sur voie = %s, bau = %d voie1 = %d voie2 = %d voie3 = %d voie4 = %d",
								va_ava_position_voie,
								*va_bau,
								*va_voie1,
								*va_voie2,
								*va_voie3,
								*va_voie4);
	XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp_return_SAE_voie OUT");
	return XDC_OK;
}

/* 
------------------------------------------------------
* SERVICE RENDU : 
* creer la fmc ava
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int ewsp_traite_FMC_AVA(T_AVA pl_AVA,
						XDY_Entier va_NumEvt,
						XDY_Octet va_CleEvt,
						XDY_PR va_PR,
						XDY_Autoroute va_Autoroute,
						XDY_Sens va_Sens,
						XDY_Entier va_creation,
						XDY_Entier va_modification_existant,
						XDY_Entier va_numAction,
						XDY_Entier va_num_alerte,
						XDY_Octet va_sit_alerte)

/*
* ARGUMENTS EN ENTREE :
* 	
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* 
------------------------------------------------------*/
{

	XDY_Octet vl_pt_car;
	XDY_Entier vl_position_voie_ava_bau;
	XDY_Entier vl_position_voie_ava_voie1;
	XDY_Entier vl_position_voie_ava_voie2;
	XDY_Entier vl_position_voie_ava_voie3;
	XDY_Entier vl_position_voie_ava_voie4;
	XDY_Entier vl_type_veh;
	XDY_Entier vl_NumEvt;
	XDY_Octet vl_CleEvt;
	XDY_Entier vl_fiche_appel;
	XDY_Evt vl_evt;
	XDY_Horodate vl_horodate_debut=0.0;
	XDY_Horodate vl_horodate_debut_prise_en_charge_client=0.0;
	XDY_Horodate vl_horodate_lancement_mission=0.0;
	XDY_Horodate vl_horodate_fin=0.0;
	XDY_Horodate vl_horodate;
	XDY_Texte vl_origine="";
	XDY_Texte vl_origine_trunc="";
	int		vl_JourSemaine;
	XDY_Entier vl_poste_enrichisseur;
	XDY_NomMachine vl_poste_calcul_pa="";
	char		vl_dg[20];
	T_Liste_AVA *pl_AVA_mem;
	XDY_Entier va_droit_enrichir;

	//param xzae40
	XDY_Entier   va_Numero_out;
	XDY_Octet    va_Cle_out;
	XDY_Octet    va_Site_out;
	XDY_Octet    va_Type_out;
	XDY_Horodate va_DebutPrevu_out;
	XDY_Horodate va_Debut_out;
	XDY_Horodate va_FinPrevu_out;
	XDY_Horodate va_Fin_out;
	XDY_Horodate va_Validation_out;
	XDY_Entier   va_NumCause_out;
	XDY_Octet    va_CleCause_out;
	XDY_Mot      va_TypeCause_out;
	XDY_Entier   va_NumAlerte_out ;
	XDY_Horodate va_HoroAlerte_out;
	XDY_Nom      va_TypeAlerte_out;
	XDY_Nom      va_Origine_out;
	XDY_Booleen  va_Confirme_out;
	XDY_Octet    va_Autoroute_out;
	XDY_PR       va_PR_out;
	XDY_Octet    va_Sens_out;
	XDY_Octet    va_PointCar_out;
	XDY_Nom      va_NomPtCar_out;
	XDY_Octet    va_VR_out;
	XDY_Octet    va_VM2_out;
	XDY_Octet    va_VM1_out;
	XDY_Octet    va_VL_out;
	XDY_Octet    va_BAU_out;
	XDY_Octet    va_VR_I_out;
	XDY_Octet    va_VM2_I_out;
	XDY_Octet    va_VM1_I_out;
	XDY_Octet    va_VL_I_out;
	XDY_Octet    va_BAU_I_out;
	XDY_Octet    va_AutorouteCause_out;
	XDY_PR       va_PRCause_out;
	XDY_Octet    va_SensCause_out;
	XDY_Horodate va_HoroCause_out;
	XDY_Octet    va_Position_out;
	XDY_Octet    va_Degats_out;
	XDY_Mot      va_TypeAnterieur_out;
	XDY_Octet    va_FausseAlerte_out;
	XDY_Octet    va_BAU_Etroite_out;
	XDY_Octet    va_BAU_I_Etroite_out;
    XDY_Entier   va_Longueur_out;
	XDY_Nom      va_AbrPtCar_out;
	XDY_Octet    va_NumPtCar_out;
	XDY_Octet    va_Degrade_out;
	XDY_Entier   va_NumEvtInit_out;
	XDY_Octet    va_CleEvtInit_out;
	XDY_Commentaire   va_ComEvtInit_out;
	XDY_Octet    va_TypeEvtInit_out;
	XDY_Octet    va_AutorouteEvtInit_out;
	XDY_PR       va_PREvtInit_out;
	XDY_Octet    va_SensEvtInit_out;
	XDY_Horodate va_HoroEvtInit_out;
	XDY_Octet    va_SiteAlerte_out;
	XDY_Octet    va_Datex_out;
	XDY_Octet    va_Trafic_out;
	XDY_Octet    va_Gravite_out;
	XDY_Booleen  va_Bloquant_out;
	XDY_Octet    va_Duree_out;
	XDY_Octet  	 va_Majeur_out;
	XDY_Octet    va_Ecoulement_out;
	

	int vl_sit_old;
	char vl_Texte[255]="";
	int vl_longueur=0;

	XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp_traite_FMC_AVA IN");

	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_horodate);	

	XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_traite_FMC_AVA id_ava:%s evt:%d/%d Localisation:%d-%u-%d Creation:%d modification:%d alerte:%d sit alerte:%d",
										pl_AVA.id_ava, 
										va_NumEvt, 
										va_CleEvt,  
										va_Autoroute,  
										va_Sens, 
										va_PR, 
										va_creation,  
										va_modification_existant,
										va_num_alerte,
										va_sit_alerte);

	/* Mapping AVA - SAE pt caracteristique*/
	if (ewsp_return_SAE_ptcar(pl_AVA.type_localisant,&vl_pt_car) != XDC_OK)
	{
			XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_traite_FMC_AVA type localisant err, section courante selectionnee");
			vl_pt_car=0;
	}
	else 
			XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_traite_FMC_AVA pt carac = %d",vl_pt_car);

	/* Mapping AVA - SAE type veh*/
	if (ewsp_return_SAE_typeveh(pl_AVA,&vl_type_veh) != XDC_OK)
	{
			XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_traite_FMC_AVA type veh err, VL selectionnee");
			vl_type_veh=XZAEC_VEH_VL;
	}
	else 
			XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_traite_FMC_AVA type veh = %d",vl_type_veh);

	/* Mapping AVA - SAE enum de la voie impactee*/
	if (ewsp_return_SAE_voie(pl_AVA.position_sur_voies,
							&vl_position_voie_ava_bau,
							&vl_position_voie_ava_voie1,
							&vl_position_voie_ava_voie2,
							&vl_position_voie_ava_voie3,
							&vl_position_voie_ava_voie4) != XDC_OK)
	{
			XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_traite_FMC_AVA  position sur voie err, voie lente selectionnee");
			vl_position_voie_ava_voie1=XDC_VOIE_BLOQUEE;
	}
	else 
			XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_traite_FMC_AVA bau = %d voie1 = %d voie2 = %d voie3 = %d voie4 = %d",
												vl_position_voie_ava_bau,
												vl_position_voie_ava_voie1,
												vl_position_voie_ava_voie2,
												vl_position_voie_ava_voie3,
												vl_position_voie_ava_voie4);
	
	XZSM_13ConversionHorodateSec(pl_AVA.horodate_debut,&vl_horodate_debut);
	XZSM_13ConversionHorodateSec(pl_AVA.horodate_debut_prise_en_charge_client,&vl_horodate_debut_prise_en_charge_client);
	XZSM_13ConversionHorodateSec(pl_AVA.horodate_lancement_mission,&vl_horodate_lancement_mission);
	XZSM_13ConversionHorodateSec(pl_AVA.horodate_fin,&vl_horodate_fin);

	/* Mise a jour horodate de fin si mission annulee */
	if (!strcmp(pl_AVA.statut_fin,"Annule"))
	{
		/* Si l'horodate de fin n'est pas renseignee */
		if (!strcmp(pl_AVA.horodate_fin,""))
			/* on met horodate de fin a la date courante */
			vl_horodate_fin=vl_horodate;			
	}

	/* Recherche du poste enrichisseur */
	vl_poste_enrichisseur = NUM_POSTE_ENRICHISSEUR;


	XZST_03EcritureTrace(XZSTC_DEBUG1,"ewsp_traite_FMC_AVA horodate_debut:%lf\nhorodate prise en charge:%lf\nhorodate lancement mission :%lf\n horodate fin :%lf",
			vl_horodate_debut,vl_horodate_debut_prise_en_charge_client,vl_horodate_lancement_mission,vl_horodate_fin);

	/* cas ou on recois un message de modification d'une fmc qui n'a pas ete cree */
	if(va_NumEvt == 0 && va_CleEvt ==0 && va_creation == 0)
	{		
		for (pl_AVA_mem = pg_AVA_debut; (pl_AVA_mem != NULL) ; pl_AVA_mem = pl_AVA_mem -> suivant) // On cherche si la fmc en modif est dans la memoire en attente de qualification de l'alerte doublon
		{	
			if (!strcmp(pl_AVA.id_ava,pl_AVA_mem->ava.id_ava)) // On trouve dans la memoire en attente de qualifcication la fmc en demande de modif, on la met a jour et on sort
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_traite_FMC_AVA Demande de modification d'une FMC ava %s en attente de qualification d'alerte, mis a jour des donnees ava en memoire et arret",pl_AVA.id_ava);
				strcpy(pl_AVA_mem->ava.id_sca, pl_AVA.id_sca);
				strcpy(pl_AVA_mem->ava.id_ava, pl_AVA.id_ava);
				pl_AVA_mem->ava.id_mission = pl_AVA.id_mission;
				strcpy(pl_AVA_mem->ava.horodate_debut, pl_AVA.horodate_debut);
				strcpy(pl_AVA_mem->ava.horodate_fin, pl_AVA.horodate_fin);
				strcpy(pl_AVA_mem->ava.statut_fin, pl_AVA.statut_fin);
				strcpy(pl_AVA_mem->ava.cause_annulation, pl_AVA.cause_annulation);
				strcpy(pl_AVA_mem->ava.type_vehicule, pl_AVA.type_vehicule);
				strcpy(pl_AVA_mem->ava.nom_autoroute, pl_AVA.nom_autoroute);
				pl_AVA_mem->ava.sens = (XDY_Sens)pl_AVA.sens;
				pl_AVA_mem->ava.pr = pl_AVA.pr;
				pl_AVA_mem->ava.distance = pl_AVA.distance;
				strcpy(pl_AVA_mem->ava.type_localisant, pl_AVA.type_localisant);
				strcpy(pl_AVA_mem->ava.id_localisation, pl_AVA.id_localisation);
				strcpy(pl_AVA_mem->ava.position_sur_voies, pl_AVA.position_sur_voies);
				strcpy(pl_AVA_mem->ava.cause, pl_AVA.cause);
				strcpy(pl_AVA_mem->ava.particularites, pl_AVA.particularites);
				pl_AVA_mem->ava.occupant = pl_AVA.occupant;
				strcpy(pl_AVA_mem->ava.commentaires, pl_AVA.commentaires);
				strcpy(pl_AVA_mem->ava.horodate_debut_prise_en_charge_client, pl_AVA.horodate_debut_prise_en_charge_client);
				strcpy(pl_AVA_mem->ava.horodate_lancement_mission, pl_AVA.horodate_lancement_mission);
				strcpy(pl_AVA_mem->ava.origine, pl_AVA.origine);
				strcpy(pl_AVA_mem->ava.complement_origine, pl_AVA.complement_origine);
				pl_AVA_mem->ava.dangerosite = pl_AVA.dangerosite;
				
				return XDC_OK; //on sort
			}
		}
		va_creation = 1; // Sortie du for on a pas trouve dans la memoire on cree la fmc ava sur message de modification pour pas perdre la donnee
	}

	if (va_creation)
	{
		/*creation FMC AVA*/
		if (XZAE08_Creer_Fiche(XZAEC_FMC_VehPanne,
			vl_poste_enrichisseur,
			XDC_OUI,
			vl_horodate_debut,
			(XDY_Horodate)C_DATE_NULLE,
			vg_NumeroSite,
			(XDY_Octet)XDC_FAUX, 
			&vl_NumEvt, 
			&vl_CleEvt,
			&vl_fiche_appel, 
			va_num_alerte,
			va_sit_alerte)!= XDC_OK) 
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_traite_FMC_AVA: pb appel a XZAE08_Creer_Fiche AVA");
		}
	}
	else
	{	// Si on creer pas la fmc, les num/cle sont en param
		vl_NumEvt=va_NumEvt;
		vl_CleEvt=va_CleEvt;
	}

	//Ajout en commentaire de la fmc du numero AVA EVOL 3, si creation ou modification de l'existant pour la premiere fois
	//Ajout de l'id mission en commentaire
	
	vl_Texte[0]='\0';
	strncpy(vl_Texte,pl_AVA.id_ava,255-1);
	vl_longueur=strlen(vl_Texte);
	strncat(vl_Texte," ",255-1-1-vl_longueur);
	vl_longueur=strlen(vl_Texte);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"texte 2 : %s;%d;%d",vl_Texte,vl_longueur,pl_AVA.id_mission);
	sprintf(vl_Texte,"%s%d",vl_Texte,pl_AVA.id_mission);
	vl_Texte[254]='\0';
	XZST_03EcritureTrace(XZSTC_DEBUG1,"texte 3 : %s;%d",vl_Texte,vl_longueur);

	if (va_creation || va_modification_existant==2)
	{
		if(XZAE67_Ecrire_Commentaires(vl_NumEvt, 				//XDY_Entier      va_numero_in
									vl_CleEvt, 					//XDY_Octet       va_cle_in
									0, 							//XDY_Entier      va_Numero_in, pour modifier un com existant
									vl_horodate_debut, 			//XDY_Horodate    va_Horodate_in
									(XDY_Horodate)C_DATE_NULLE, 	//XDY_Horodate    va_HoroPerime_in
									vl_Texte,				//char            *va_Texte_in
									NUM_POSTE_ENRICHISSEUR		//XDY_Mot 		  va_PosteOperateur_in
		)!= XDC_OK) 
		{
			XZST_03EcritureTrace(   XZSTC_WARNING, "ewsp_traite_FMC_AVA : pb appel a XZAE67_Ecrire_Commentaires");
		}
	}

	if(!va_creation || va_modification_existant)
	{
		/* On recupere les informations de la FMC d'origine en cas de mise jour par le message AVA d'une FMC SAE */
		if (XZAE40_Lire_Fiche_MC((XDY_Horodate)C_DATE_NULLE,vl_NumEvt,vl_CleEvt,&va_Numero_out,&va_Cle_out,&va_Site_out,&va_Type_out,
		&va_DebutPrevu_out, &va_Debut_out, &va_FinPrevu_out, &va_Fin_out, &va_Validation_out, &va_NumCause_out, &va_CleCause_out,
		&va_TypeCause_out, &va_NumAlerte_out, &va_HoroAlerte_out, va_TypeAlerte_out, va_Origine_out, &va_Confirme_out, &va_Autoroute_out,
		&va_PR_out, &va_Sens_out, &va_PointCar_out, va_NomPtCar_out, &va_VR_out, &va_VM2_out, &va_VM1_out, &va_VL_out,
		&va_BAU_out, &va_VR_I_out, &va_VM2_I_out, &va_VM1_I_out, &va_VL_I_out, &va_BAU_I_out, &va_AutorouteCause_out, &va_PRCause_out, &va_SensCause_out,
		&va_HoroCause_out, &va_Position_out, &va_Degats_out, &va_TypeAnterieur_out, &va_FausseAlerte_out, &va_BAU_Etroite_out, &va_BAU_I_Etroite_out, &va_Longueur_out,
		va_AbrPtCar_out, &va_NumPtCar_out, &va_Degrade_out,	&va_NumEvtInit_out, &va_CleEvtInit_out, va_ComEvtInit_out, &va_TypeEvtInit_out, &va_AutorouteEvtInit_out,
        &va_PREvtInit_out, &va_SensEvtInit_out, &va_HoroEvtInit_out, &va_SiteAlerte_out, &va_Datex_out, &va_Trafic_out, &va_Gravite_out,
		&va_Bloquant_out, &va_Duree_out, &va_Majeur_out, &va_Ecoulement_out ) != XDC_OK) 
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_traite_FMC_AVA: pb appel a XZAE40_Lire_Fiche_MC(%d/%d)",vl_NumEvt,vl_CleEvt);
			return ;
		}
		XZST_03EcritureTrace(XZSTC_DEBUG1,"ewsp_traite_FMC_AVA retour xzae40 : cause:%d/%d",va_NumCause_out,va_CleCause_out);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"ewsp_traite_FMC_AVA retour xzae40 : horodate_debut:%lf",va_Debut_out);
		XZST_03EcritureTrace(XZSTC_DEBUG1,"ewsp_traite_FMC_AVA retour xzae40 : origine:%s",va_Origine_out);
	}
XZST_03EcritureTrace(XZSTC_DEBUG1,"ewsp_traite_FMC_AVA va_Fin_out=%d.",va_Fin_out);
	vl_sit_old=va_sit_alerte;//init du sit
	/* SAE-518 si FMC non Terminée par l'opérateur */
	if ( va_Fin_out==0.0 ) 
	if(!va_creation) 
	{
		/* Dans le cas ou la FMC au SAE est sur un site et que la localisation AVA la deplace sur un autre site */
		/* on recupere le PR/autoroute actuel de la FMC dans le SAE pour comparer le site avec le site du PR du message AVA */
		XZST_03EcritureTrace(XZSTC_DEBUG1,"ewsp_traite_FMC_AVA autoroute_old:%d, pr_old:%d",va_Autoroute_out,va_PR_out);

		/* On recupere le sit qui correspond a la localisation dans le SAE */
		vl_sit_old=siteGestionFromNumAuto(va_Autoroute_out, va_PR_out);
		if(va_sit_alerte!=vl_sit_old)	
			XZST_03EcritureTrace(XZSTC_DEBUG1,"ewsp_traite_FMC_AVA la fmc etait sur un ancien site:%d",vl_sit_old);

		// Transfert du droit d'enrichir au CI pour pouvoir modifier la FMC */ 
		if(xzae15_Transmettre_Droit_Enrichir (vl_NumEvt,	//va_NumEvt_in
										vl_CleEvt,			//va_CleEvt_in
										vg_SitePrincipal,   //va_Site_in
										0,    				//va_PosteOperateur_in
										0,    				//va_Sans_Rafraichir_in
										1,    				//va_Sans_Reveil_in
										vl_sit_old==XDC_VC?XDC_NOM_SITE_VC:XDC_NOM_SITE_DP,		//va_NomSiteLocal_in
										&va_droit_enrichir)!=XDC_OK)    //va_Resultat_out
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_traite_FMC_AVA Enrichissement non transfere au CI");
		}
		else
			XZST_03EcritureTrace(XZSTC_INFO,"ewsp_traite_FMC_AVA Enrichissement transfere au CI FMC:%d/%d",vl_NumEvt,vl_CleEvt);
	}//Doit etre appele avant XZAE96 qui remet le sit de FMC_VEH au site local, et la XZAE15 a besoin de FMC_VEH.sit == FMC_GEN.sit pour transfere les droits

	/* Ajout id_ava, dangerosite,type veh, occupant dans FMC_VEH */
	XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_traite_FMC_AVA: FMC AVA %d/%d",vl_NumEvt,vl_CleEvt);
	if(XZAE96_Ecrire_Fiche_Vehicule(vl_NumEvt,
			vl_CleEvt,
			vl_type_veh,
			pl_AVA.occupant,
			0,//bebe_in
			0,//personne agee
			0,//animaux
			0,//Veh Escota
			0,//chargement
			"",//code mat
			0,//Depannage
			0,//site depannage
			pl_AVA.commentaires,//Commentaire
			1,//Numero Veh (la xzae08 met 1 dans cette colonne)
			vl_poste_enrichisseur,//Poste operateur
			"",//Marque
			"",//type
			"",//couleur
			"",//Immatriculation
			0,//code pays
			"",//Pays
			"",//nom
			"",//ville
			"",//adresse
			"",//code danger
			0,//Code nature panne
			"",//nature panne
			"",//positionnement
			0,//veh roulant
			"",//demande enlev par
			pl_AVA.id_mission,//fiche appel
			!strstr(pl_AVA.particularites,"MatiereDangereuse")?0:1,// presence matiere dangereuse
			0,//fax envoye
			0,//code marque
			0,//code couleur
			0,//code enleve demande par
			vg_NumeroSite,//site in
			vg_NumeroSite,//site creation
			0,//nb bebe
			0, //nb age
			0,//flag association
			pl_AVA.id_ava,//id_ava
			pl_AVA.dangerosite//dangerosite
			) != XDC_OK)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING, "ewsp_traite_FMC_AVA: pb appel a XZAE96_Ecrire_Fiche_Vehicule AVA");
		return (XDC_NOK);			
	}

	/* Concatenation de l'origine */
	if (!va_modification_existant)
	{
		strcpy(vl_origine,"AVA ");
		strcat(vl_origine,pl_AVA.origine);
		strcat(vl_origine," ");
		strcat(vl_origine,pl_AVA.complement_origine);
		strncpy(vl_origine_trunc,vl_origine,25);
	}
	else
	{
		strcpy(vl_origine,va_Origine_out); //Si on modifie la FMC du SAE, on garde son origine
		strncpy(vl_origine_trunc,vl_origine,25);
	}
	XZST_03EcritureTrace(   XZSTC_WARNING, "ewsp_traite_FMC_AVA origine =%s",vl_origine_trunc);

	/* Ajout localisation FMC, pt caracteristique, etat voie, FMC cause*/  /* SAE-518 si FMC non Terminée par l'opérateur */
	if (va_Fin_out==0.0) 
	if(validerFMC( vl_NumEvt,
		vl_CleEvt,  
		va_creation?XZAEC_FMC_VehPanne:va_Type_out, // Si on est sur une creation, sinon on prend le type retour de la xzae40
		va_Autoroute,  
		va_Sens,
		va_PR,
		vl_pt_car,
		atoi(pl_AVA.id_localisation),
		va_modification_existant?va_NumCause_out:0, //Si on modifie la FMC du SAE on garde sa cause
		va_modification_existant?va_CleCause_out:0, //Si on modifie la FMC du SAE on garde sa cause
		//va_modification_existant?va_Debut_out:vl_horodate_debut, //Si on modifie la FMC du SAE, on garde son horodate
		vl_horodate_debut,
		vl_position_voie_ava_bau,
		vl_position_voie_ava_voie1,
		vl_position_voie_ava_voie2,
		vl_position_voie_ava_voie3,
		vl_position_voie_ava_voie4,
		0, /* SAE-518 vl_horodate_fin,*/
		vl_origine_trunc,
		va_num_alerte,
		vl_sit_old,
		vl_horodate,
		va_creation )!=XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_traite_FMC_AVA: pb appel a validerFMC");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_traite_FMC_AVA: FMC AVA %d/%d validee",vl_NumEvt,vl_CleEvt);
	} 

	// Transfert du droit d'enrichir au site de la localisation */
	/* SAE-518 si FMC non Terminée par l'opérateur */
	if (va_Fin_out==0.0) 
	{
	if(xzae15_Transmettre_Droit_Enrichir (	 vl_NumEvt,			//va_NumEvt_in
                                     vl_CleEvt,				//va_CleEvt_in
                                     va_sit_alerte,    		//va_Site_in
                                     0,    					//va_PosteOperateur_in
                                     0,    					//va_Sans_Rafraichir_in
                                     1,    					//va_Sans_Reveil_in
									 "CI",						//va_NomSiteLocal_in
                                     &va_droit_enrichir)!= XDC_OK)    //va_Resultat_out
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"ewsp_traite_FMC_AVA Enrichissement non transfere sur site %d",va_sit_alerte);
	}
	else
		XZST_03EcritureTrace(XZSTC_INFO,"ewsp_traite_FMC_AVA Enrichissement transfere site:%d FMC:%d/%d",va_sit_alerte,vl_NumEvt,vl_CleEvt);


	if (va_creation)
	{
		/* Generer le reveil de creation de FMC */
		genererReveil(	vl_NumEvt,
						vl_CleEvt, 
						XDC_REV_FMC_CRE,
						siteGestionFromNumAuto(va_Autoroute,va_PR));

		if (siteGestionFromNumAuto(va_Autoroute,va_PR) != vg_SitePrincipal)
			genererReveil(	vl_NumEvt,
							vl_CleEvt, 
							XDC_REV_FMC_CRE,
							vg_SitePrincipal);
	}
	else
	{
		/* Generer le reveil de modification de FMC */
		genererReveil(	vl_NumEvt,
						vl_CleEvt, 
						XDC_REV_FMC_MOD,
						siteGestionFromNumAuto(va_Autoroute,va_PR));

		if (siteGestionFromNumAuto(va_Autoroute,va_PR) != vg_SitePrincipal)
			genererReveil(	vl_NumEvt,
							vl_CleEvt, 
							XDC_REV_FMC_MOD,
							vg_SitePrincipal);
	}
	}
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%d",vl_NumEvt);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%d",vl_CleEvt);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%d",va_numAction); //numero d'action 
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%d",vg_NumeroSite);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%s",pl_AVA.id_sca);//va_NomDepanneur_in);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%s","");//va_Resultat_in,);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%lf",vl_horodate_debut_prise_en_charge_client);//va_HoroAppel_in);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%lf",vl_horodate_lancement_mission);//va_HoroArrivee_in);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%d",C_OPERATEUR);//va_Operateur_in);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%lf",vl_horodate_fin);//va_HoroDepart_in);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%d",strcmp(pl_AVA.statut_fin,"Annule")?0:vl_horodate_fin);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%d",0);//va_renfort_in);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%d",0);//va_patrouille_presente_in);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%d",0);//va_garagiste_contact_pc_in);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%d",strcmp(pl_AVA.statut_fin,"Annule")?0:1);//va_annul_intervention_in);//va_annul_intervention_in);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%d",0);//va_code_type_astreinte_in);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%d",0);//va_interlocuteur_in,);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"%d",vg_NumeroSite);

	if (XZAC15_Creer_Modifier_Action(vl_NumEvt,
									vl_CleEvt,
									va_numAction, //numero d'action
									vg_NumeroSite,
									LIBELLE_DEPANNEUR,//va_NomDepanneur_in
									"",//va_Resultat_in,
									vl_horodate_lancement_mission,//va_HoroAppel_in
									vl_horodate_debut_prise_en_charge_client,//va_HoroArrivee_in
									C_OPERATEUR,//va_Operateur_in
									vl_horodate_fin,//va_HoroDepart_in
									strcmp(pl_AVA.statut_fin,"Annule")?0:vl_horodate_fin,//va_HoroAnnul_in
									0,//va_renfort_in
									0,//va_patrouille_presente_in
									0,//va_garagiste_contact_pc_in
									strcmp(pl_AVA.statut_fin,"Annule")?0:1,//va_annul_intervention_in
									0,//va_code_type_astreinte_in
									0,//va_interlocuteur_in,
									vl_horodate_debut_prise_en_charge_client==0?vg_NumeroSite:va_sit_alerte//va_site_responsable_in
									//Il y a un bug sur l'affichage DP et VC du depannage dans le synoptique
									//Si le site responsable ne change pas en meme temp qu'on ajoute une heure de success(prise en charge)
									//l'affichage ne ce met pas a jour, donc on le laisse a 43 et on le passe au site concerne quand on a une heure success
									) != XDC_OK )
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_traite_FMC_AVA: pb appel a XZAC15_Creer_Modifier_Action");
	}

	/* si dangerosite, on lance le PA */
	if ((pl_AVA.dangerosite) && (vl_horodate_fin==0.0))
	{
		/* FMC creee sur le CI, plan d'action calculer sur DP/VC, la replication n'a pas le temps de se faire */
		sleep(3);

		vl_evt.cle=vl_CleEvt;
		vl_evt.numero=vl_NumEvt;
		/* determination du Site et dg pour le PA */
		strcpy(vl_dg, "XDG_LX_IE_");
		if (va_sit_alerte==XDC_VC)
		{
			strcpy(vl_poste_calcul_pa,POSTE_CALCUL_PA_VC);
			strcat(vl_dg, XDC_NOM_SITE_VC);
		}
		else if (va_sit_alerte==XDC_DP)
		{
			strcpy(vl_poste_calcul_pa,POSTE_CALCUL_PA_DP);
			strcat(vl_dg, XDC_NOM_SITE_DP);
		}
		else
			XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_traite_FMC_AVA: pb calcul PA site alerte = %d",va_sit_alerte);

		//XZDD02_CalculerPA(vl_horodate_debut,-1,vl_poste_calcul_pa,vl_evt,XZAEC_FMC_VehPanne,XZDDC_PA_SANS_TFM,XZDDC_MODE_OPERATIONNEL);

		XZST_03EcritureTrace(XZSTC_INTERFACE,
		"ewsp_traite_FMC_AVA message calcul PA : Horodate = %lf, NumPosteOperateur = %d, NomPosteOperateur = %s, NumEvt = %d, NumEvtCle = %d, TypeEvt = %d, TypePA = %d, Mode = %d",
		vl_horodate, -1, vl_poste_calcul_pa, vl_evt.numero, vl_evt.cle, XZAEC_FMC_VehPanne, XZDDC_PA_SANS_TFM, XZDDC_MODE_OPERATIONNEL);

		/*A
		**  ecriture d'un message de type XDM_DIN_PA sur le datagroup choisi
		*/
		XZST_03EcritureTrace(XZSTC_WARNING,
			"ewsp_traite_FMC_AVA : envoi d'un message de type XDM_DIN_PA sur le datagroup %s", vl_dg);
		if (TipcSrvMsgWrite(vl_dg,
			XDM_IdentMsg(XDM_DIN_PA),
			XDM_FLG_SRVMSGWRITE,
			T_IPC_FT_REAL8, vl_horodate,
			T_IPC_FT_INT2, -1,
			T_IPC_FT_STR, vl_poste_calcul_pa,
			T_IPC_FT_INT4, vl_evt.numero,
			T_IPC_FT_CHAR, (char) vl_evt.cle,
			T_IPC_FT_CHAR, (char) XZAEC_FMC_VehPanne,
			T_IPC_FT_CHAR, (char) XZDDC_PA_SANS_TFM,
			NULL) != TRUE)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "ewsp_traite_FMC_AVA echec dans l'ecriture du message XDM_DIN_PA");
			XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp_traite_FMC_AVA OUT");
			return(XDC_NOK);
		}

	}

	XZST_03EcritureTrace ( XZSTC_FONCTION,	"ewsp_traite_FMC_AVA OUT");
	return XDC_OK;
}
