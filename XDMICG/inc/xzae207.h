/*E*/
/*Fichier : $Id: xzae207.h,v 1.1 2008/11/17 10:47:02 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/11/17 10:47:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE207 * FICHIER XZAE207.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	11/10/08	creation DEM/835 1.1
------------------------------------------------------ */

#ifndef XZAE207
#define XZAE207

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
#define XZAE207C_RPC_NAME "XZAE207"

#define XZAE207C_NB_PARM_RETURN 3


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE207_Lit_Nature :  FMC_NAT
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE207_Lit_Nature (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Octet	*va_code_nature_out,
					char	*va_libelle_nature_out);
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
