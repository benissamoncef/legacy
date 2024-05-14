/*E*/
/*Fichier : @(#)xzae550.h	1.1      Release : 1.1        Date : 01/04/04
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC80 * FICHIER xzae550.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture et ecriture des evenements
*
------------------------------------------------------
* HISTORIQUE :
*
* F.Lizot  01/04/2004 :  Creation (SAGA)
------------------------------------------------------ */

#ifndef XZAE550
#define XZAE550

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
#define XZAE550C_RPC_NAME "XZAE550"

#define XZAE550C_NB_PARM_RETURN 2


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE550_Chercher_FMC_DATEX : 
*  Recherche la FMC associee a un evennement DATEX
*  Retourne 0,0 si la FMX n'existe pas
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE550_Chercher_FMC_DATEX (char *va_emetteur_in,
				       char *va_reference_in,
				       XDY_Entier  *va_evenement_out,
				       XDY_Octet   *va_cle_out);

/*
*
* PARAMETRES EN ENTREE :
*
* char *va_emetteur_in         code du partenaire qui a emis le message
* char *va_reference_in        reference datex associee
*				
* PARAMETRES EN SORTIE :
* XDY_Entier *va_evenement_out  numero de la FMC associee ou 0 si elle n'existe pas
* XDY_Octet  *va_cle_out        cle de la FMC associee ou 0 si elle n'existe pas
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
