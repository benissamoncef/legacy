/*E*/
/* Fichier : %W%        Release : %I%        Date : %G%
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE571 * FICHIER XZAE571.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       02 Fev 2012     : Creation (DEM/1016)
------------------------------------------------------ */

#ifndef XZAE571
#define XZAE571

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
#define XZAEC_XZAE571_RPC_NAME "XZAE571"

#define XZAE571C_NB_PARM_RETURN 0


/* definitions de types exportes */
extern tg_Connexion  *XZAGV_Connexion;


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE571_Terminer_FMC 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
					

int XZAE571_Terminer_FMC ( XDY_Entier	va_Numero_in,
				XDY_Octet  va_Cle_in);

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
