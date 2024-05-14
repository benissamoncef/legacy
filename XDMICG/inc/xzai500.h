/*E*/
/*Fichier : @(#)xzai500.h	1.1      Release : 1.1        Date : 02/26/99
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAI500 * FICHIER xzai500.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Enregistre en base un message transmis par un partenaire 
* pour diffusion directe
* Si ce message est lie a une FMC, il est enregistre
* comme une action TFM via XZAC19 et XZAC20.
* Sinon, le message est enregistre dans la table TFM_DTX.
*
------------------------------------------------------
* HISTORIQUE :
*
* F. lizot      12 Fev 2004     : Creation (SAGA)
------------------------------------------------------ */

#ifndef XZAI500
#define XZAI500

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
#define XZAI500C_RPC_NAME "XZAI500"

#define XZAI500C_NB_PARM_RETURN 0


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAI500_Enregistre_Message_TFM : 
* Enregistre en base un message transmis par un partenaire
* pour diffusion directe
* Si ce message est lie a une FMC, il est enregistre
* comme une action TFM via XZAC19 et XZAC20.
* Sinon, le message est enregistre dans la table TFM_DTX.
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAI500_Enregistre_Message_TFM (char *va_code_partenaire_in,
				   	   char *va_reference_datex_in,
					   char *va_lnk_in,
					   XDY_Horodate va_HeureLancement_in,
					   char *va_texte_message_1_in,
					   char *va_texte_message_2_in);

/*
*
* PARAMETRES EN ENTREE :
*
* char *va_code_partenaire_in   code du partenaire qui a emis le message
* char *va_reference_datex_in   reference datex associee
* char *va_lnk_in               reference datex FMC associee
* XDY_Horodate va_HeureLancement_in heure de lancement
* char *va_texte_message_1_in   texte a diffuser (debut du message)
* char *va_texte_message_2_in   texte a diffuser (fin du message)
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
