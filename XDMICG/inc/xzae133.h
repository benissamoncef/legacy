/*E*/
/*Fichier : $Id: xzae133.h,v 1.1 1997/11/12 15:27:42 verdier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1997/11/12 15:27:42 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE133 * FICHIER XZAE133.h
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
------------------------------------------------------ */

#ifndef XZAE133
#define XZAE133

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
#define XZAE133C_RPC_NAME "XZAE133"

#define XZAE133C_NB_PARM_RETURN 3


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE133_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE133_Lire_Fiche_SousCnc_Pour_Strada (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Nom		va_nom_out,
					XDY_Booleen	*va_ferme_out,
					XDY_Booleen	*va_penurie_out);

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
