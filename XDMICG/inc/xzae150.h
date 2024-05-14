/*E*/
/*Fichier : $Id: xzae150.h,v 1.1 1999/02/26 14:35:46 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 14:35:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE150 * FICHIER XZAE150.h
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

#ifndef XZAE150
#define XZAE150

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
#define XZAE150C_RPC_NAME "XZAE150"

#define XZAE150C_NB_PARM_RETURN 3


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE150_Message_Datex : renvoie le dernier Message 
*  radiodiffuse recu.
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE150_Message_Datex (	XDY_Entier	va_NumEvt_in,
					XDY_Octet	va_CleEvt_in,
					char		*va_emetteur_out,
					char		*va_Texte_out);

/*
*
* PARAMETRES EN ENTREE :
* int 		va_NumEvt_in
* tinyint 	va_CleEvt_in
*				
* PARAMETRES EN SORTIE :
* char		*va_Texte_out,
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
