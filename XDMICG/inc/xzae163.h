/*E Fichier : $Id: xzae163.h,v 1.1 2008/11/17 10:44:31 pc2dpdy Exp $  Release : $Revision: 1.1 $        Date : $Date: 2008/11/17 10:44:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE163 * FICHIER XZAE163.h
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

#ifndef XZAE163
#define XZAE163

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

#define XZAE163C_XZAE163_RPC_NAME		"XZAE163"
#define XZAE163C_XZAE163_NB_PARAM_RETURN	0


/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*	
*
*  Appelle XZAE163 :  traiter icones WEB
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE163_Genere_Textes( );

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
