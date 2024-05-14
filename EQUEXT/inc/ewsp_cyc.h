		/*E*/
/*  Fichier : $Id: ewsp_cyc.h,v 1.5 2020/09/22 08:06:01 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2020/09/22 08:06:01 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER ewsp_cyc.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier cyclique de la tache TERAU
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	21/09/16	creation
* LCL	12/06/19	ODI-C DEM1338 1.1
* LCL	06/09/19	LOT27 COCKPIT DEM1351 1.2
* ABE	12/08/2020	Ajout taitement données 6min TDP FCD DEM-SAE152 1.3
------------------------------------------------------ */

#ifndef ewsp_cyc
#define ewsp_cyc

/* fichiers inclus */
#include	<xdy.h>
#include	<xdc.h>

#include 	"ex_msok.h"


/* definitions de constantes exportees */
#define EWSPC_REVEIL		10
#define EWSP_TDP_FCD_REVEIL	360
#define ADRESSE_SERVEUR_ODIC_WS "http://ws-odic.vinci-autoroutes.com/Satisfaction/Declenche"
#define MAX_TIME_TRANSFER 	5     /* -m/--max-time <seconds> Maximum time allowed for the transfer */
#define WORKPATH 		"/tmp/"
#define WORKFILE_JSON 		"odi-c_satisfaction_response.json"
#define WORKFILE_TXT 		"odi-c_satisfaction_response.txt"
#define STATUT_STR_ERREUR 	"\"Message\":\""
#define STATUT_STR_SUCCES 	"\"IdSatisfaction\":\""
#define VAL_CLE_DOSSIER_COCKPIT_PUSH    "/produits/push-gateway/data"
#define VAL_CLE_DOSSIER_TDP_PUSH    "/produits/push-gateway/dataTDP"

#define CLE_JSON_ID             "id"
#define CLE_JSON_TYPE           "type"
#define CLE_JSON_AXE            "roadNumber"
#define CLE_JSON_PROBA          "probability"
#define CLE_JSON_SENS           "direction"
#define CLE_JSON_NUMPR_DEB      "pr"
#define CLE_JSON_OFFSET_DEB     "distance"
#define CLE_JSON_NUMPR_FIN      "prfin"
#define CLE_JSON_OFFSET_FIN     "distancefin"
#define VAL_CLE_JSON_VEH_ARR    "VehicleObstruction"
#define VAL_CLE_JSON_OBSTACLE   "GeneralObstruction"
#define VAL_CLE_JSON_ACCIDENT   "Accident"
#define VAL_CLE_JSON_BOUCHON    "AbnormalTraffic"
#define VAL_CLE_JSON_PROGRAMME  "probable"
#define VAL_CLE_JSON_SIGNALE    "riskOf"
#define VAL_CLE_JSON_CONFIRME   "certain"
#define VAL_CLE_JSON_SENS_SUD   "aligned"
#define VAL_CLE_JSON_SENS_NORD  "opposite"
#define VAL_CLE_JSON_SENS_INC   "both"




/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
void ewsp_cyc_60();
void ewsp_odi_c();
void ewsp_fcd();
#endif
