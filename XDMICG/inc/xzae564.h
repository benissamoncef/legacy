/*E*/
/* Fichier : $Id: xzae564.h,v 1.1 2012/07/04 10:56:32 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:56:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE564 * FICHIER XZAE564.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       02 Fev 2012     : Creation (DEM/1016)
------------------------------------------------------ */

#ifndef XZAE564
#define XZAE564

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
#define XZAEC_XZAE564_RPC_NAME "XZAE564"

#define XZAE564C_NB_PARM_RETURN 3


/* definitions de types exportes */
extern tg_Connexion  *XZAGV_Connexion;


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE564_Bouchon_Conseq : nb de bouchons consequence
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
					

int XZAE564_Update_FMC_Regulation ( XDY_Entier	va_Numero_in,
									XDY_Octet  va_Cle_in,
									XDY_Eqt	va_Scenario_in,
									XDY_Eqt	va_Contexte_in,
									XDY_Eqt	va_Indicateur_Horaire_in,
									XDY_Eqt	va_Mesure_in,
									char*		va_Transitoire_in,
									XDY_Eqt	va_Zone_in,
									XDY_Eqt		va_Operateur_in,
									XDY_Horodate	va_Horodate_in,
									XDY_Eqt		va_Scenario_cible_in
									);

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
