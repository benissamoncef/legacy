/*E*/
/*Fichier : $Id: xzad03.h,v 1.1 1997/05/15 09:58:03 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1997/05/15 09:58:03 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAD03 * FICHIER XZAD03.h
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
------------------------------------------------------ */

#ifndef XZAD03
#define XZAD03

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
#define XZAD03C_RPC_NAME "XZAD03"

#define XZAD03C_NB_PARM_RETURN 0
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAD03_Ajout_TDP_1h : met en base les temps de parcours 1h.
*  Appelle XZAD03SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAD03_Ajout_TDP_1h (	XDY_Eqt  	va_Numero_in ,
				XDY_Horodate	va_Horodate_in,
				XDY_TDP		va_Temps_in,
				XDY_Octet	va_Validite_in,
				XDY_TDP		va_Appariement_in);
				
#endif
