/*E*/
/*Fichier : $Id: xzas53.h,v 1.2 1997/05/15 09:59:12 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/05/15 09:59:12 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS53 * FICHIER XZAS53.h
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

#ifndef XZAS53
#define XZAS53

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
#define XZAS53C_RPC_NAME "XZAS53"

#define XZAS53C_NB_PARM_RETURN 0
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/* SERVICE RENDU : 
*  XZAS53_Ajout_Mesure_MET : met en base les donnees meteo.
*  Appelle XZAS53SP
*/
extern int XZAS53_Ajout_Mesure_MET (	XDY_Eqt  	va_Numero_in ,
				XDY_Horodate	va_jour_in,
				XDY_Booleen	va_Dispo_in,
				XDY_Octet	va_Code_in,
				XDY_Horodate    va_Horo_debut_in,
				XDY_Horodate    va_Horo_fin_in);
				
#endif
