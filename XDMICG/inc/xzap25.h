/*E*/
/*Fichier : $Id: xzap25.h,v 1.1 1999/02/26 11:26:16 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 11:26:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE138 * FICHIER XZAP25.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	25/11/1998	: Creation (dem/1724) 1.1
------------------------------------------------------ */

#ifndef XZAP25
#define XZAP25

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
#define XZAP25C_RPC_NAME "XZAP;25"

#define XZAP25C_NB_PARM_RETURN 1


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAP25_Trafic : recupere les parametres
* de sortie de xzap;25
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/



extern int XZAP25_Trafic ( XDY_Mot      va_autoroute_in ,
                                        XDY_PR       va_PR_in,
                                        XDY_Sens       va_sens_in,
                                        XDY_Horodate       va_horodate_in,
                                        XDY_Octet      *va_trafic_out);


/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot      va_autoroute_in ,
* XDY_PR       va_PR_in,
* XDY_Sens       va_sens_in,
* XDY_Horodate       va_horodate_in,
*				
* PARAMETRES EN SORTIE :
* XDY_Octet      *va_trafic_out
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
* Extraire le niveau de trafic pour l'horodate donnee
*
*
* MECANISMES :
*
------------------------------------------------------*/

#endif
