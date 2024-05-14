/*E*/
/*  Fichier : $Id: xzas100.h,v 1.2 2009/10/05 10:06:55 gesconf Exp $        Release : $Revision: 1.2 $        Date : $Date: 2009/10/05 10:06:55 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS100 * FICHIER XZAS100.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* PNI	15/12/2005	: Creation
------------------------------------------------------ */

#ifndef XZAS100
#define XZAS100

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
#define XZAS100C_RPC_NAME "XZAS100"

#define XZAS100C_NB_PARM_RETURN 3

#define XZAS101C_RPC_NAME "XZAS101"

#define XZAS101C_NB_PARM_RETURN 3
#define XZAS103C_RPC_NAME "XZAS103"

#define XZAS103C_NB_PARM_RETURN 3
/* definitions de types exportes */


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAS100_TdP_RADT : recupere le temps de parcours
*  pour une autoroute, PRdebut, PRfin et un sens donnes È l'heure courante
*  Appelle XZAS;100SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAS100_TdP_RADT (	XDY_Autoroute  	va_autoroute_in ,
				XDY_PR		va_PRdeb_in,
				XDY_PR		va_PRfin_in,
				XDY_Sens	va_sens_in,
				XDY_Entier	*va_tdp_out);


/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Autoroute  	va_autoroute_in ,
* XDY_PR		va_PRdeb_in,
* XDY_PR		va_PRfin_in,
* XDY_Sens	va_sens_in,
*				
* PARAMETRES EN SORTIE :
* XDY_Entier	*va_tdp_out
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
* - prametre obligatoire sont obligatoires
*

* FONCTION :
*
* Calcule d'un temps de parcours basé sur les données RADT
*
*
* MECANISMES :
*
------------------------------------------------------*/

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAS101_TdP_RADT : recupere le temps de parcours
*  pour une autoroute, PRdebut, PRfin et un sens donnes È l'heure courante
*  Appelle XZAS;101SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAS101_TdP_Recon_RADT (	XDY_Autoroute  	va_autoroute_in ,
				XDY_PR		va_PRdeb_in,
				XDY_PR		va_PRfin_in,
				XDY_Sens	va_sens_in,
				XDY_Entier	*va_tdp_out);


/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Autoroute  	va_autoroute_in ,
* XDY_PR		va_PRdeb_in,
* XDY_PR		va_PRfin_in,
* XDY_Sens	va_sens_in,
*				
* PARAMETRES EN SORTIE :
* XDY_Entier	*va_tdp_out
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
* - prametre obligatoire sont obligatoires
*

* FONCTION :
*
* Calcule d'un temps de parcours basé sur les données RADT
*
*
* MECANISMES :
*
------------------------------------------------------*/

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAS103_TdP : recupere le temps de parcours
*  pour une autoroute, PRdebut, PRfin et un sens donnes È l'heure courante
*  Appelle XZAS;100SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAS103_TdP (	XDY_Autoroute  	va_autoroute_in ,
				XDY_PR		va_PRdeb_in,
				XDY_PR		va_PRfin_in,
				XDY_Sens	va_sens_in,
				XDY_Entier	*va_tdp_out);


/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Autoroute  	va_autoroute_in ,
* XDY_PR		va_PRdeb_in,
* XDY_PR		va_PRfin_in,
* XDY_Sens	va_sens_in,
*				
* PARAMETRES EN SORTIE :
* XDY_Entier	*va_tdp_out
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
* - prametre obligatoire sont obligatoires
*

* FONCTION :
*
* Calcule d'un temps de parcours basé sur les données RADT
*
*
* MECANISMES :
*
------------------------------------------------------*/


#endif
