/*E Fichier : $Id: xzan37.h,v 1.1 1994/11/30 12:37:09 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/30 12:37:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAN37 * FICHIER XZAN37.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module interface Annuaire 
*********************************
*  XZAN37_Nom_Fonction : Retourne le nom de l'interlocuteur 
*  associ≈ » une fonction.
*
*  Appelle XZAN37SP
*********************************  
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	30 Nov 1994	: Creation
------------------------------------------------------ */

#ifndef XZAN37
#define XZAN37

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
#define XZAN37C_RPC_NAME "XZAN;37"

#define XZAN37C_NB_PARM_RETURN 1
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/********************************
*  XZAN37_Nom_Fonction : Retourne le nom de l'interlocuteur 
*  associ≈ » une fonction.
*  Appelle XZAN37SP
*********************************
*/
extern int XZAN37_Nom_Fonction (XDY_Octet,XDY_Nom);

#endif
