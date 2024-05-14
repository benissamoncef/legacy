/*E*/
/*Fichier : $Id: xzas70.h,v 1.1 1999/02/26 14:40:18 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 14:40:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS70 * FICHIER XZAS70.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron 	05/01/1999	: creation
------------------------------------------------------ */

#ifndef XZAS70
#define XZAS70

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
#define XZAS70C_RPC_NAME "XZAS70"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
* lecture de la localisation des stations a radt a exporter vers strada  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAS70_Loc_RADT_A_Exporter (XDY_FonctionInt pa_FonctionUtilisateur_in,
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
