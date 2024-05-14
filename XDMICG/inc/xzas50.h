/*E Fichier : $Id: xzas50.h,v 1.1 1996/10/09 17:35:24 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/10/09 17:35:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS50 * FICHIER XZAS50.h
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

#ifndef XZAS50
#define XZAS50

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
#define XZAS50C_RPC_NAME "XZAS50"

#define XZAS50C_NB_PARM_RETURN 0
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/* SERVICE RENDU : 
*  XZAS50_Ajout_Mesure_KC : met en base les donnees de comptages.
*  Appelle XZAS50SP
*/
extern int XZAS50_Ajout_Mesure_KC (	XDY_Mot  	va_Station_in ,
				XDY_Sens 	va_Sens_in,
				XDY_Octet 	va_Voie_in,
				XDY_Horodate	va_Horodate_in,
				XDY_Mot		*pa_tKC_in);
				
#endif
