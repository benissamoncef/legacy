/*E*/
/*Fichier : $Id: xzad08.h,v 1.2 2011/03/07 16:27:48 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2011/03/07 16:27:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAD08 * FICHIER XZAD08.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	24 Fev 1997	: Creation (dem/tdp) 1.1
* JPL		07/03/11 : Migration architecture HP ia64 (DEM 975) : declaration correcte XZAD08_Liste_Franchissements  1.2
------------------------------------------------------ */

#ifndef XZAD08
#define XZAD08

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
#define XZAD08C_RPC_NAME "XZAD08"

#define XZAD08C_NB_PARM_RETURN 0
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAD08_Liste_Franchissements : recupere la liste de 
franchissement haut.
*  Appelle XZAD08SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAD08_Liste_Franchissements(XDY_FonctionInt va_fct_in);

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_FonctionInt va_fct_in

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
