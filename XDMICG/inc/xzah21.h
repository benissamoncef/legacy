/*E Fichier : $Id: xzah21.h,v 1.2 1995/03/30 17:41:45 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/03/30 17:41:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAH * FICHIER XZAH21.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	10 jan 1995	: Creation
* torregrossa	30 mar 1995	: Suppression du fichier xzah_utils.h
*                                 (V 1.2)
------------------------------------------------------ */

#ifndef XZAH21
#define XZAH21

/* fichiers inclus */
	/* Include systeme */
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
	#include "xzag.h"
	#include "asql_com.h"
	#include "xzahc.h"
	#include "xzed.h"
	#include "xzer.h"

	
/* definitions de constantes exportees */

#define XZAH21C_XZAH21_RPC_NAME "XZAH;21"
#define XZAH21C_XZAH21_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*	XZAH21 : Envoie a EQUEXT les voies radt ou dai inversees
------------------------------------------------------
* SEQUENCE D'APPEL :
*
* PARAMETRES EN ENTREE :
*
* XDY_NomMachine	va_NomMachine_in		
*
* PARAMETRES EN SORTIE : aucun
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
* INTERFACE :
*
*
* MECANISMES :
*
------------------------------------------------------*/
extern int XZAH21_VoiesDaiRadt(XDY_NomMachine);

#endif
