/*E*/
/*Fichier : @(#)xzas88.h	1.2      Release : 1.2        Date : 12/03/97
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS88 * FICHIER XZAS88.h
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

#ifndef XZAS88
#define XZAS88

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
#define XZAS88C_RPC_NAME "XZAS88"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
* lecture de la config des stations a radt 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAS88_Lect_RADT(XDY_FonctionInt pa_FonctionUtilisateur_in,
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
