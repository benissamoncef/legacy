/*E*/
/*Fichier : $Id: xzae1270.h,v 1.1 2007/03/26 10:18:08 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2007/03/26 10:18:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE127 * FICHIER XZAE127.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron     15/01/2007	: Creation pour PX-METACOR v1.1 DEM605
------------------------------------------------------ */

#ifndef XZAE127
#define XZAE127

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
#define XZAE1270C_RPC_NAME "XZAE1270"

#define XZAE1270C_NB_PARM_RETURN 5


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE127_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE1270_Infos_Metacor (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Horodate	*va_fin_prev_out,
					XDY_Octet	*va_type_loca_out,
					char 		*va_nom_loca_out,
					int 		*va_num_loca_out,
					char 	*va_ConfigVoies_out);

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
