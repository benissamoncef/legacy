/*E*/
/*Fichier : $Id: xzae138.h,v 1.3 2007/03/26 10:23:29 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2007/03/26 10:23:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE138 * FICHIER XZAE138.h
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
* Niepceron	04 Mar 1999	: Ajout pr tete 1.2
* Niepceron     17/01/2007	: Ajout arg pour PX-METACOR v1.3 DEM605
------------------------------------------------------ */

#ifndef XZAE138
#define XZAE138

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
#define XZAE138C_RPC_NAME "XZAE138"

#define XZAE138C_NB_PARM_RETURN 7


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE138_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE138_Lire_Fiche_Basculement_Pour_Strada (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Entier	*va_longueur_out,
					XDY_Entier      *va_prtete_out,
					XDY_Octet	*va_type_loca_out,
					char 		*va_nom_loca_out,
					int 		*va_num_loca_out,
					char		*va_ConfigVoies_out,
					char		*va_ConfigVoiesSuite_out);

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
