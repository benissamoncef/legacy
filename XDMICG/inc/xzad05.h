/*E*/
/*Fichier : $Id: xzad05.h,v 1.1 1997/02/24 16:06:39 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1997/02/24 16:06:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAD05 * FICHIER XZAD05.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	24 fev 1997	: Creation (dem/)
------------------------------------------------------ */

#ifndef XZAD05
#define XZAD05

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

/* definitions de constantes exportees */
#define XZAD05C_RPC_NAME "XZAD05"

#define XZAD05C_NB_PARM_RETURN 1
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAD05_Dernier_TDP : recupere le dernier tdp 6mn pour horodate
donn≈e.
*  Appelle XZAD05SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAD05_Dernier_TDP (	XDY_Eqt  	va_Numero_in ,
				XDY_Horodate	va_Horodate_in,
				XDY_TDP		*pa_Temps_out);

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot               va_Numero_in   : numero de la zone
* XDY_Horodate	        va_Horodate_in  :
*

*				
* PARAMETRES EN SORTIE :
*XDY_TDP		pa_Temps_out : dernier temps 
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
*

* FONCTION :
*
*
*
* MECANISMES :
*
------------------------------------------------------*/
				
#endif
