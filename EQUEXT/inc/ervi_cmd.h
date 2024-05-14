/*E*/
/* Fichier : $Id: ervi_cmd.h,v 1.3 2020/07/07 14:08:39 pc2dpdy Exp $     Release : $Revision: 1.3 $      Date :  $Date: 2020/07/07 14:08:39 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TECFE * FICHIER ervi_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* c'est le fichier d'include des callbacks pal
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	14/11/18 creation DEM1315 1.1 RMVI
* LCL	21/01/19 augmentation constante NB_MAX_ELMTS 1.2
*
------------------------------------------------------ */

#ifndef ervi_cmd
#define ervi_cmd

/* fichiers inclus */
#include "xdy.h"
#include "xdc.h"
#include "xdf.h"
#include "xzst.h"
#include "xzst.h"
#include <rtworks/common.h>
#include <rtworks/ipc.h>
#include "xzst.h"



/* definitions de constantes exportees */

extern XDY_NomMachine		vg_NomMachine;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern void	en_demande_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);

/* definition de constantes */
#define ADRESSE_SERVEUR_WS "http://194.5.181.203:20442/GEOLOC_WS/RMVI"
#define MAX_TIME_TRANSFER 5	/* -m/--max-time <seconds> Maximum time allowed for the transfer */
#define WORKPATH "/produits/migrazur/appliSD/tmp/"
#define WORKFILE_JSON "rmvi_response.json"
#define WORKFILE_TXT "rmvi_response.txt"
#define STATUT_STR_ERREUR "\"status\":\"echec\""
#define STATUT_STR_SUCCES "\"status\":\"succes\""
#define NB_MAX_ELMTS 200

#define STATUT_OK 	0
#define STATUT_ERREUR 	1
#define STATUT_TIMEOUT 	2
#define STATUT_EMPTY 	3

#endif
