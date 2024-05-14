/*E*/
/* Fichier : $Id: xzae567.h,v 1.1 2012/07/04 10:56:58 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:56:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE567 * FICHIER XZAE567.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       25 Avril 2012     : Creation (DEM/1016)
------------------------------------------------------ */
#ifndef XZAE567
#define XZAE567

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
#define XZAE567C_RPC_NAME "XZAE567"

#define XZAE567C_NB_PARM_RETURN 6


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU :
*  XZAE567_Get_FMC_Regulation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE567_Get_FMC_Regulation ( 	XDY_Horodate 			va_horodate_in,
									XDY_Eqt 				va_zone_in,
									XDY_Entier				*va_Numero_out,
									XDY_Octet  				*va_Cle_out,
									XDY_Entier              *va_Contexte_out,
									XDY_Entier              *va_Scenario_out,
									XDY_Entier              *va_Scenario_cible_out,
									XDY_Entier              *va_Mesure_out,
									XDY_Entier              *va_Indicateur_Horaire_out,
									XDY_Entier              *va_Zone_out,
									char*                   va_Transitoire_out);
/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt		va_numero_in
* XDY_Sens		va_cle_in
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
