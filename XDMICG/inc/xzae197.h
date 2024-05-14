		/*E*/
/*Fichier : $Id: xzae197.h,v 1.2 2020/07/20 14:26:33 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/07/20 14:26:33 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE197 * FICHIER XZAE197.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	02/07/2020 :	Creation DEM130 V1.1
* ABE 20/07/2020 : 	DEM 130 V1.2 correction malloc pour radio
------------------------------------------------------ */

#ifndef XZAE197
#define XZAE197

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

/* Include Sybase */
#include <ctpublic.h>
#include <cstypes.h>
	
/* Include applicatif Migrazur */
#include "xdy.h"
#include "xdc.h"
#include "xzae.h"
#include "xzag.h"
#include "asql_com.h"

/* definitions de constantes exportees */
#define XZAE197C_RPC_NAME "XZAE197"

#define XZAE197C_NB_PARM_RETURN 1
#define RADIO_LENGTH 11

/* definitions de types exportes */
typedef struct {
	int size;
	char **radio;
	XDY_Horodate *heure_succes;
	XDY_PR *pr;
	XDY_Sens *sens;
	XDY_Autoroute *autoroute;										
	
} XZAE_LOC_GPS;	

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE197_Infos_obligatoires : recupere les infos 
*  de coordonée en fonction du PR/axe
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE197_get_pat_radio_date_depart(XZAE_LOC_GPS *loc_gps_out);

extern int XZAE197_free_memory (XZAE_LOC_GPS *loc_gps_out);

/*
*
* PARAMETRES EN ENTREE :
*
*				
* PARAMETRES EN SORTIE :
* char* 	va_radio_out
* CS_DATETIME	va_heure_succes_out

* VALEUR RENDUE :
*
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
*
*

* FONCTION :
*
* REtourn la liste des radio et la date de départ des patrouilleurs en action
*
*
* MECANISMES :
*
------------------------------------------------------*/

#endif
