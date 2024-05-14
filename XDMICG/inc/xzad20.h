/*E*/
/*Fichier : $Id: xzad20.h,v 1.1 2020/02/13 14:04:28 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2020/02/13 14:04:28 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAD20 * FICHIER XZAD20.h
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

#ifndef XZAD20
#define XZAD20

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
#define XZAD20C_RPC_NAME "XZAD20"

#define XZAD20C_NB_PARM_RETURN 0
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAD20_Ajout_TDP_6mn : met en base les temps de parcours 6mn.
*  Appelle XZAD20SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAD20_Reveil_Operateur (	XDY_Entier  	va_Numero_in ,
				XDY_Octet	va_Cle_in);
				
#endif
