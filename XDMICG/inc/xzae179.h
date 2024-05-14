/*E*/
/*Fichier : $Id: xzae179.h,v 1.1 2008/11/17 10:45:23 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/11/17 10:45:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE179 * FICHIER XZAE179.h
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
* JMG		11/10/08 : parametres en plus DEM 836 1.2
------------------------------------------------------ */

#ifndef XZAE179
#define XZAE179

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
#define XZAE179C_RPC_NAME "XZAE179"

#define XZAE179C_NB_PARM_RETURN 5


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

extern int XZAE179_Restrictions_Basculement (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Octet       va_VR_in,
					XDY_Octet       va_VM2_in,
					XDY_Octet       va_VM1_in,
					XDY_Octet       va_VL_in,
					XDY_Octet       va_BAU_in,
					XDY_Octet       va_VR_I_in,
					XDY_Octet       va_VM2_I_in,
					XDY_Octet       va_VM1_I_in,
					XDY_Octet       va_VL_I_in,
					XDY_Octet       va_BAU_I_in,
					char            *va_restrictions1_out,
					char      *va_restrictions2_out,
					XDY_Octet 	*va_type_loc_out       ,
					char		*va_nom_loc_out,
					XDY_Entier	*va_num_loc_out);
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
