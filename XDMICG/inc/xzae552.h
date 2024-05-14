/*E*/
/*Fichier : @(#)xzae552.h	1.1      Release : 1.1        Date : 02/26/99
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE552 * FICHIER xzae552.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Cree une FMC à partir d'un evennement DATEX recu.
*
------------------------------------------------------
* HISTORIQUE :
*
* F. lizot      12 Fev 2004     : Creation (SAGA)
------------------------------------------------------ */

#ifndef XZAE552
#define XZAE552

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
#define XZAE552C_RPC_NAME "XZAE552"

#define XZAE552C_NB_PARM_RETURN 1


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE552_Creation_FMC: 
*  Cree une FMC à partir d'un evennement DATEX recu.
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE552_Creation_FMC(char *va_emetteur_in,
				char *va_reference_in,
				XDY_Octet   va_cle_in,
				XDY_Octet   va_site_in,
				char *va_texte_alt_in,
				int  *va_FMC_out);

/*
*
* PARAMETRES EN ENTREE :
*
* char *va_emetteur_in          code du partenaire qui a emis le message
* char *va_reference_in         reference datex associee
* int   va_cle_in               cle a utiliser pour creer la FMC
* int   va_site_in              site a utiliser pour creer la FMC
* char *va_texte_alt_in         texte a utiliser pour creer la FMC
*				
* PARAMETRES EN SORTIE :
*
* int  *va_FMC_out              num&ro de la FMC creee
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
