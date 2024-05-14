/*E Fichier : $Id: xzae102.h,v 1.2 1996/09/09 09:14:15 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/09/09 09:14:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE102 * FICHIER XZAE102.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	26 Aou 1996	: Creation
* Niepceron	03 Sep 1996	: Suppression du site dans les argument v1.2
* Vathelot	05 Sep 1996	: ajout du nombre de parametre de retour v1.2
------------------------------------------------------ */

#ifndef XZAE102
#define XZAE102

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

#define XZAE102C_XZAE102_RPC_NAME		"XZAE;102"
#define XZAE102C_XZAE102_NB_PARAM_RETURN	0


/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*	
*
*  Appelle XZAE;102 : Mets les travaux prevus --> "en cours"
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE102_Travaux_a_mettre_en_cours( );

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
*	Appelle XZAE;102 et pour chaque ligne retournee appelle va_FonctionUtilisateur
*
------------------------------------------------------*/

#endif
