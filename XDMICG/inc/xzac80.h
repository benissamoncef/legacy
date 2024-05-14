/*E*/
/*Fichier : @(#)xzac80.h	1.1      Release : 1.1        Date : 02/26/99
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC80 * FICHIER xzac80.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Insere une action DATEX si la FMC associee existe dans MIGRAZUR
*  (insertion dans les tables ACT_GEN et ACT_ADA)
*
------------------------------------------------------
* HISTORIQUE :
*
* F. lizot      04 Mar 2004     : Creation (SAGA)
------------------------------------------------------ */

#ifndef XZAC80
#define XZAC80

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
	#include "xzae.h"
	#include "xzag.h"
	#include "asql_com.h"

/* definitions de constantes exportees */
#define XZAC80C_RPC_NAME "XZAC80"

#define XZAC80C_NB_PARM_RETURN 1


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAC80_Ajouter_Action_DATEX : 
*  Insere une action DATEX si la FMC associee existe dans MIGRAZUR
*  (insertion dans les tables ACT_GEN et ACT_ADA)
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAC80_Ajouter_Action_DATEX (char *va_emetteur_in,
				        char *va_reference_in,
					char *va_lnk_in,
					XDY_Horodate va_horodate_debut_in,
					XDY_Horodate va_horodate_fin_in,
					char *va_libelle_in,
					int  *va_numeroAction_out);

/*
*
* PARAMETRES EN ENTREE :
*
* char *va_emetteur_in         code du partenaire qui a emis le message
* char *va_reference_in        reference datex associee
* char *va_lnk_in              numero de FMC cause
* XDY_Horodate va_horodate_debut_in  horodate de debut
* XDY_Horodate va_horodate_fin_in  horodate de fin
* va_libelle_in                commentaires
*				
* PARAMETRES EN SORTIE :
* va_numeroAction_out          numero de l'action creee
*
* VALEUR RENDUE :
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
