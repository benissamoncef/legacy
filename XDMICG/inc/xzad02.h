/*E*/
/*Fichier : $Id: xzad02.h,v 1.2 2012/02/14 14:28:38 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2012/02/14 14:28:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAD02 * FICHIER XZAD02.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	03 Oct 1996	: Creation (dem/1232)
*  jmg		13/01/2012	: tdp etendu dem/1014
------------------------------------------------------ */

#ifndef XZAD02
#define XZAD02

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
#define XZAD02C_RPC_NAME "XZAD02"

#define XZAD02C_NB_PARM_RETURN 0
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAD02_Ajout_TDP_6mn : met en base les temps de parcours 6mn.
*  Appelle XZAD02SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAD02_Ajout_TDP_6mn (	XDY_Eqt  	va_Numero_in ,
				XDY_Horodate	va_Horodate_in,
				XDY_TDP		va_Temps_in,
				XDY_Octet	va_Validite_in,
				XDY_TDP         va_TempsRADT_in,
				XDY_Octet       va_ValiditeRADT_in);
				
#endif
