/*E*/
/*Fichier : $Id: xzae153.h,v 1.1 1999/02/26 14:35:48 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 14:35:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE153 * FICHIER XZAE153.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	23 Dec 1998	: creation
------------------------------------------------------ */

#ifndef XZAE153
#define XZAE153

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
#define XZAE153C_RPC_NAME "XZAE153"

#define XZAE153C_NB_PARM_RETURN 6


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE153_Localisation_PR : converti la localisation
*  RDS en Autoroute PR sens
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE153_Localisation_PR (	char		*va_l1_in,
					char		*va_l2_in,
					XDY_Octet       va_l3_in,
					int             va_l4_in,
					char            *va_l5_in,
					char            *va_l6_in,
					char            *va_l7_in,
					int             va_l8_in,
					int             va_l9_in,
					char            *va_l10_in,
					int             va_l11_in,
					int             va_l12_in,
					char		*va_libelle_out,
					XDY_Octet	*va_Autoroute_out,
					XDY_PR		*va_PR_out,
					XDY_Sens	*va_Sens_out,
					XDY_Octet       *va_Autoroutetete_out,
					XDY_PR          *va_PRtete_out);

/*
*
* PARAMETRES EN ENTREE :
*
* char		*va_L1_in,
  char		*va_l2_in,
* tinyint         va_l3_in,
* int             va_l4_in,
* char            *va_l5_in,
* char            *va_l6_in,
* char            *va_l7_in,
* int             va_l8_in,
* int             va_l9_in,
* char            *va_l10_in,
* int             va_l11_in,
* int             va_l12_in,
*				
* PARAMETRES EN SORTIE :
* char		*va_libelle_out,
* XDY_Octet	*va_Autoroute_out,
* XDY_PR		*va_PR_out,
* XDY_Sens	*va_Sens_out,
* XDY_Octet       *va_Autoroutetete_out,
* XDY_PR          *va_PRtete_out
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
