/*E*/
/*Fichier : $Id: xzas80.h,v 1.1 2001/04/19 16:26:49 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2001/04/19 16:26:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS80 * FICHIER XZAS80.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	16/01/01	: creation 1.1
------------------------------------------------------ */

#ifndef XZAS80
#define XZAS80

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
#define XZAS80C_RPC_NAME "XZAS80"

#define XZAS80C_NB_PARM_RETURN 0
/* definitions de types exportes */


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
* XZAS80 : reinitialise les troncons WEB  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAS80_Traiter_Troncons();

/*
*
* PARAMETRES EN ENTREE :
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
