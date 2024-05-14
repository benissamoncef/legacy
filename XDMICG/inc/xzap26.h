/*E*/
/*Fichier : $Id: xzap26.h,v 1.1 2004/11/16 14:16:09 gesconf Exp $     Release : $Revision: 1.1 $        Date : $Date: 2004/11/16 14:16:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE138 * FICHIER XZAP26.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	02/12/01	creation
------------------------------------------------------ */

#ifndef XZAP26
#define XZAP26

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
#define XZAP26C_RPC_NAME "XZAP26"

#define XZAP26C_NB_PARM_RETURN 0


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
* propose extinction d'eqts  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/



extern int XZAP26_Extinction ( XDY_Entier      va_numero_pa_in ,
                                        XDY_Horodate       va_horodate_in,
                                        XDY_Entier      va_numero_evt_in ,
                                        XDY_Entier      va_cle_evt_in);


/*
*
* PARAMETRES EN ENTREE :
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
*

* FONCTION :
*
*
*
* MECANISMES :
*
------------------------------------------------------*/

#endif
