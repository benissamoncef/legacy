/*E*/
/* Fichier : $Id: xzae65.h,v 1.4 2019/04/01 07:56:25 devgfi Exp $        $Revision: 1.4 $        $Date: 2019/04/01 07:56:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE65 * FICHIER XZAE65.h
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
* JPL	19/03/19 Ajout paramètres : horodate validation et état FMC (DEM 1327)  1.3
* JPL	19/03/19 Procédure dégroupée  1.4
------------------------------------------------------ */

#ifndef XZAE65
#define XZAE65

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
#define XZAE65C_RPC_NAME "XZAE65"

#define XZAE65C_NB_PARM_RETURN 1


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

extern int XZAE65_Ecrire_Fiche_Manifestation (	XDY_Entier	va_numero_in ,
						XDY_Octet	va_cle_in,
						XDY_Horodate	va_Horodate_in,
						XDY_Mot		va_Nombre_in,
						XDY_Booleen	va_Perception_in,
						XDY_Booleen	va_Violence_in,
						XDY_Mot		va_PosteOperateur_in,
						XDY_Octet	va_Prevision_Traiter_Clos_in);

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
