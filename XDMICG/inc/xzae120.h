/*E*/
/*Fichier : $Id: xzae120.h,v 1.2 1997/11/12 15:27:23 verdier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/11/12 15:27:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE120 * FICHIER XZAE120.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	12 Mai 1997	: Creation (dem/strada) 1.1
------------------------------------------------------ */

#ifndef XZAE120
#define XZAE120

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
#define XZAE120C_RPC_NAME "XZAE120"

#define XZAE120C_NB_PARM_RETURN 0


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XXZAE120_Creation_Evt_ADA :
* creation en base d'une ligne de synthese strada  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE120_Creation_Evt_ADA (	char	va_reference_in[50] ,
					int	va_version_in,
					XDY_Horodate	va_date_in,
					char		va_texte_in[250],
					char		va_type_in[50]);

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
