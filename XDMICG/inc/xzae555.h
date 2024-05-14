/*E*/
/*Fichier : $Id: xzae555.h,v 1.1 2008/11/17 10:54:23 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/11/17 10:54:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE555 * FICHIER XZAE555.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        CREATION        DEM 835
------------------------------------------------------ */

#ifndef XZAE555
#define XZAE555

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
#define XZAE555C_RPC_NAME "XZAE555"

#define XZAE555C_NB_PARM_RETURN 3


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE555_Bouchon_Conseq : nb de bouchons consequence
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE555_Bouchon_Conseq (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Octet       *va_nb_out,
					XDY_Entier	*va_num_out,
					XDY_Octet	*va_cle_out);
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
