/*E Fichier : $Id: xzas51.h,v 1.1 1996/10/09 17:35:25 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/10/09 17:35:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS51 * FICHIER XZAS51.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	03 Oct 1996	: Creation
------------------------------------------------------ */

#ifndef XZAS51
#define XZAS51

/* fichiers inclus */
	/* Include systeme */
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
#define XZAS51C_RPC_NAME "XZAS51"

#define XZAS51C_NB_PARM_RETURN 0
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/* SERVICE RENDU : 
*  XZAS51_Ajout_Mesure_EC : met en base les donnees de comptages.
*  Appelle XZAS51SP
*/
extern int XZAS51_Ajout_Mesure_EC (	XDY_Mot  	va_Station_in ,
				XDY_Sens 	va_Sens_in,
				XDY_Octet 	va_Voie_in,
				XDY_Horodate	va_Horodate_in,
				XDY_Mot		*pa_tEC_in);
				
#endif
