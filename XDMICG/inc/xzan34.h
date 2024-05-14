/*E Fichier : $Id: xzan34.h,v 1.1 1994/11/15 11:43:46 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/15 11:43:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAN34 * FICHIER XZAN34.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	04 Nov 1994	: Creation
------------------------------------------------------ */

#ifndef XZAN34
#define XZAN34

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
#define XZAN34C_RPC_NAME "XZAN;34"

#define XZAN34C_NB_PARM_RETURN 0
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/********************************
*  XZAN34_Epurer_Calendrier_Astreinte : est appele par un callback de tacli
*  sur reception de message XDM_AN_PURGE sur le dg XDG_AN_site.
*  Ce callback ne doit etre arme que sur le CI.
*  Le cb est d≈clench≈ de maniere cyclique (il se rearme pour se declencher 24h 
*  plus tard).
*  Appelle XZAN34SP
*********************************
*/
extern int XZAN34_Epurer_Calendrier_Astreinte ();

#endif
