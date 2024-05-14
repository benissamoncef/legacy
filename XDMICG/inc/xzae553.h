/*E*/
/*Fichier : @(#)xzae553.h	1.1      Release : 1.1        Date : 14/04/04
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC80 * FICHIER xzae553.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture et ecriture des evenements
*
------------------------------------------------------
* HISTORIQUE :
*
* F.Lizot  14/04/2004 :  Creation (SAGA)
------------------------------------------------------ */

#ifndef XZAE553
#define XZAE553

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
#define XZAE553C_RPC_NAME "XZAE553"

#define XZAE553C_NB_PARM_RETURN 0


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE553_Creer_Evenement_DATEX : 
*  Cree un evennement DATEX associe a une FMC ESCOTA
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE553_Creer_Evenement_DATEX (char *va_emetteur_in,
				          char *va_reference_in,
				          XDY_Entier  va_evenement_in,
				          XDY_Octet   va_cle_in);

/*
*
* PARAMETRES EN ENTREE :
*
* char *va_emetteur_in         code du partenaire qui a emis le message
* char *va_reference_in        reference datex associee
* XDY_Entier va_evenement_in   numero de la FMC associee 
* XDY_Octet  va_cle_in         cle de la FMC associee 
*				
* PARAMETRES EN SORTIE :
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
