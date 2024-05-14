/*E*/
/*Fichier : $Id: xzae129.h,v 1.3 2007/03/26 10:20:57 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2007/03/26 10:20:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE129 * FICHIER XZAE129.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Vathelot	12 Mai 1997	: Creation (dem/strada) 1.1
* Niepceron	23 Dec 1998	: Ajout nb PL dem/1724 1.2
* Niepceron	15 Jan 2007	: Ajout nb bless graves (PX_METACOR) DEM605 1.3
------------------------------------------------------ */

#ifndef XZAE129
#define XZAE129

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
#define XZAE129C_RPC_NAME "XZAE129"

#define XZAE129C_NB_PARM_RETURN 11


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE129_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE129_Lire_Fiche_Accident_Pour_Strada (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Octet	*va_vehicules_out,
					XDY_Octet	*va_PL_out,
					XDY_Octet	*va_cars_out,
					XDY_Octet       *va_rem_out,
					XDY_Octet	*va_blesses_out,
					XDY_Octet	*va_morts_out,
					XDY_Booleen	*va_matieres_out,
					XDY_Booleen	*va_veh_escota_out,
					XDY_Booleen	*va_homme_escota_out,
					XDY_Booleen	*va_perso_out,
					XDY_Octet	*va_blesses_graves_out);

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
