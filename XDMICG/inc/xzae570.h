/*E*/
/* Fichier : %W%        Release : %I%        Date : %G%
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE570 * FICHIER XZAE570.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  recherche bouchons proches echangeur
*  
*
------------------------------------------------------
* HISTORIQUE :
*
* 
* JMG	31/08/18 : correction entete GABEB 1.2
------------------------------------------------------ */

#ifndef XZAE570
#define XZAE570

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
#define XZAEC_XZAE570_RPC_NAME "XZAE570"

#define XZAOC_XZAE570_NB_PARM_RETURN 1


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
					
extern int XZAE570_Bouchons_Proches( XDY_Horodate 	va_Horodate_in, 
			 XDY_Autoroute 	va_Autoroute_in,
				 XDY_PR 		va_PR_in,
				 XDY_Octet 		va_mode_in,
				 XDY_Eqt		va_ech_in,
				 XDY_FonctionInt pa_FonctionUtilisateur_in
									);
/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt		va_numero_in
* XDY_Sens		va_cle_in
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
