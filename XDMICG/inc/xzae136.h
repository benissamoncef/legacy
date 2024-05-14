/*E*/
/*Fichier : $Id: xzae136.h,v 1.3 2007/03/26 10:22:16 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2007/03/26 10:22:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE136 * FICHIER XZAE136.h
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
* Niepceron	07 Jan 1999	: modif datex dem/1724 1.2
* Niepceron	15 Jan 2007	: Modif pour PX-METACOR v1.3 DEM605
------------------------------------------------------ */

#ifndef XZAE136
#define XZAE136

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
#define XZAE136C_RPC_NAME "XZAE136"

#define XZAE136C_NB_PARM_RETURN 10


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE136_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE136_Lire_Fiche_Bouchon_Pour_Strada (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Mot		*va_type_out,
					XDY_Entier	*va_longueur_out,
					XDY_Octet	*va_autoroute_out,
					XDY_Entier	*va_PR_out,
					XDY_Octet	*va_sens_out,
					XDY_Entier      *va_numero_out,
					XDY_Octet       *va_cle_out,
					XDY_Octet	*va_type_loca_out,
					char 		*va_nom_loca_out,
					int 		*va_num_loca_out);

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
