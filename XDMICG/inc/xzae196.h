		/*E*/
/*Fichier : $Id: xzae196.h,v 1.2 2020/11/03 18:04:33 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2020/11/03 18:04:33 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE196 * FICHIER XZAE196.h
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
* ABE	15/10/2020 : 	Ajout version station dans les param out DEM-SAE130 1.2
------------------------------------------------------ */

#ifndef XZAE196
#define XZAE196

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
#define XZAE196C_RPC_NAME "XZAE196"

#define XZAE196C_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE196_Infos_obligatoires : recupere les infos 
*  de coordonée en fonction du PR/axe
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE196_Lire_Code_SIREDO (XDY_Eqt va_numero_station_in ,
                                     XDY_Sens va_sens_in,
					char * va_code_siredo_out,
					char * va_version_station_out);
/*
*
* PARAMETRES EN ENTREE :
*
*				
* PARAMETRES EN SORTIE :
* XDY_Eqt	va_numero_station_in
* XDY_sens	va_sens_in	

* VALEUR RENDUE :
* char * va_code_siredo_out
* char * va_version_station_out
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
* Retourne le code SIREDO et la version de la station pour un numero de station et un sens donnée
*
*
* MECANISMES :
*
------------------------------------------------------*/

#endif
