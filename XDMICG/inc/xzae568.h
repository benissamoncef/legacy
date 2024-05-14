/*E*/
/* Fichier : $Id: xzae568.h,v 1.1 2016/09/29 15:38:48 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2016/09/29 15:38:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE568 * FICHIER XZAE568.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       25 Avril 2012     : Creation (DEM/1016)
------------------------------------------------------ */
#ifndef XZAE568
#define XZAE568

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
#define XZAE568C_RPC_NAME "XZAE568"

#define XZAE568C_NB_PARM_RETURN 2


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU :
*  XZAE568_Get_FMC_Saga
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAE568_Get_FMC_Saga ( char 				*va_numero_saga_in,
				XDY_Entier				*va_Numero_out,
				XDY_Octet  				*va_Cle_out,
				XDY_Entier                              *va_Numero_Tete_out);
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
