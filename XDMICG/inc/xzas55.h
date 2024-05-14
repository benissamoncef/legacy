/*E*/
/*Fichier : $Id: xzas55.h,v 1.2 1997/12/03 11:04:40 vathelot Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/12/03 11:04:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS55 * FICHIER XZAS55.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Vathelot	02 Mai 1997	: Creation (dem/strada) 1.1
------------------------------------------------------ */

#ifndef XZAS55
#define XZAS55

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
	#include "xzag.h"
	#include "asql_com.h"

/* definitions de constantes exportees */
#define XZAS55C_RPC_NAME "XZAS55"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
* lecture de la config des stations a radt a exporter vers strada  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAS55_Lect_RADT_A_Exporter (XDY_FonctionInt pa_FonctionUtilisateur_in,
				      XDY_Entier        *va_Resultat_out);

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt		va_station_in
* XDY_Sens		va_sens_in
*				
* PARAMETRES EN SORTIE :
*
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

* FONCTION :
*
*
*
* MECANISMES :
*
------------------------------------------------------*/
#endif
