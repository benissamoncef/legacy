/*E Fichier : $Id: xzae162.h,v 1.1 2008/11/17 10:43:56 pc2dpdy Exp $  Release : $Revision: 1.1 $        Date : $Date: 2008/11/17 10:43:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE162 * FICHIER XZAE162.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.1 DEM 836
------------------------------------------------------ */

#ifndef XZAE162
#define XZAE162

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

#define XZAE162C_XZAE162_RPC_NAME		"XZAE162"
#define XZAE162C_XZAE162_NB_PARAM_RETURN	0


/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*	
*
*  Appelle XZAE162 :  traiter icones WEB
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE162_Calcul_Troncons(XDY_Autoroute va_autoroute,
			XDY_Octet	va_sens,
			XDY_PR		va_debut,
			XDY_Octet	va_type_pt_car,
			XDY_Octet	va_num_pt_car,
			XDY_Horodate	va_horodate,
			XDY_Nom		va_nom,
			XDY_Nom		va_icone,
			XDY_Octet	va_priorite,
			XDY_Octet	va_type);

/*
*
* PARAMETRES EN ENTREE : aucun
* PARAMETRES EN SORTIE : aucun
* VALEUR RENDUE :
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
------------------------------------------------------*/

#endif
