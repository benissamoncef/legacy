		/*E*/
/*Fichier : $Id: xzae202.h,v 1.1 2020/10/26 09:38:51 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2020/10/26 09:38:51 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE202 * FICHIER XZAE202.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	21/10/2020 :	Creation SAE-DEM130 V1.1
------------------------------------------------------ */

#ifndef XZAE202
#define XZAE202

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
#define XZAE202C_RPC_NAME "XZAE202"

#define XZAE202C_NB_PARM_RETURN 3

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE202_Get_FMC_Type : recupere le type de FMC et la FMC cause pour un numero et cle 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE202_Get_FMC_Type(XDY_Entier      va_numero_in ,
                                XDY_Octet       va_cle_in,
				XDY_Entier	*va_numero_cause_out,
				XDY_Octet	*va_cle_cause_out,
                                XDY_Octet       *va_type_out);
/*
*
* PARAMETRES EN ENTREE :
* XDY_Entier      va_numero_in
* XDY_Octet       va_cle_in	
*
*				
* PARAMETRES EN SORTIE :
* XDY_Mot         *va_type_out

* VALEUR RENDUE :
* EXP..FMC_GEN.Type
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
* 2     XDC_ARG_INV
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  XZAE202_Get_FMC_Type : recupere le type de FMC et la cause pour un numero et cle 
*
*
* MECANISMES :
*
------------------------------------------------------*/

#endif
