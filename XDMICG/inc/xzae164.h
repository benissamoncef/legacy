/*E Fichier : $Id: xzae164.h,v 1.1 2001/04/19 16:26:50 gesconf Exp $    Release : $Revision: 1.1 $        Date : $Date: 2001/04/19 16:26:50 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE164 * FICHIER XZAE164.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.1
------------------------------------------------------ */

#ifndef XZAE164
#define XZAE164

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

#define XZAE164C_XZAE164_RPC_NAME		"XZAE164"
#define XZAE164C_XZAE164_NB_PARAM_RETURN	0


/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*	
*
*  Appelle XZAE164 :  reinit icones WEB
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE164_Init_Icones( );

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
