/*E*/
/* Fichier : $Id: xzae41.h,v 1.3 2011/12/02 14:13:43 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2011/12/02 14:13:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE41 * FICHIER XZAE41.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
* JMG   11/10/08        CREATION        DEM 835
* JPL	10/11/11  Procedure degroupee  (pour DEM 1006)  1.2
* JPL	10/11/11  Declaration des parametres additionnels (champs conditions) de la proc. stockee  (DEM 1006)  1.3
------------------------------------------------------ */

#ifndef XZAE41
#define XZAE41

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
#define XZAE41C_RPC_NAME "XZAE41"

#define XZAE41C_NB_PARM_RETURN 17


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE41_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE41_Lire_Fiche_Accident (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Octet	*va_vehicules_out,
					XDY_Octet	*va_PL_out,
					XDY_Octet	*va_cars_out,
					XDY_Octet       *va_rem_out,
					XDY_Octet       *va_motos_out,
					XDY_Octet	*va_blessesLegers_out,
					XDY_Octet	*va_blessesGraves_out,
					XDY_Octet	*va_morts_out,
					XDY_Booleen	*va_matieres_out,
					XDY_Booleen	*va_veh_escota_out,
					XDY_Booleen	*va_homme_escota_out,
					XDY_Booleen	*va_perso_out,
					XDY_Booleen	*va_degats_out);

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
