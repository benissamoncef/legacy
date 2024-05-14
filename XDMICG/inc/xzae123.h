/*E Fichier : @(#)xzae123.h	1.2      Release : 1.2        Date : 09/09/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE123 * FICHIER XZAE123.h
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

#ifndef XZAE123
#define XZAE123

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

#define XZAE123C_XZAE123_RPC_NAME		"XZAE123"
#define XZAE123C_XZAE123_NB_PARAM_RETURN	0


/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*	
*
*  Appelle XZAE;123 : Mets les travaux prevus --> "en cours"
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE123_Lect_FMC_A_Exporter(XDY_FonctionInt pa_FonctionUtilisateur_in,
					 XDY_Entier     *va_Resultat_out);

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
*	Appelle XZAE;123 et pour chaque ligne retournee appelle va_FonctionUtilisateur
*
------------------------------------------------------*/

#endif
