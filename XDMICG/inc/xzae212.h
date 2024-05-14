/*E*/
/*Fichier : $Id: xzae212.h,v 1.3 2019/09/12 12:35:33 devgfi Exp $     $Revision: 1.3 $        $Date: 2019/09/12 12:35:33 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE212 * FICHIER XZAE212.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   08/02/16 creation SAGA 1.0
* JPL	05/09/19 Ajout paramètre code de nature incendie (DEM 1352)  1.3
------------------------------------------------------ */

#ifndef XZAE212
#define XZAE212

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
#define XZAE212C_RPC_NAME "XZAE212"

#define XZAE212C_NB_PARM_RETURN 1


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE212_Ecrire_Fiche_Incendie (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Horodate	va_horodate_in,
					XDY_Octet	va_feu_maitrise_in,
					XDY_Octet	va_code_nature_in,
					XDY_Entier	va_PosteOperateur_in,
					XDY_Octet	va_prevision_traiter_clos_in);

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier		va_numero_in
* XDY_District		va_cle_in
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
