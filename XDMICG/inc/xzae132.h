/*E*/
/*Fichier : $Id: xzae132.h,v 1.1 1997/11/12 15:27:41 verdier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1997/11/12 15:27:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE132 * FICHIER XZAE132.h
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

#ifndef XZAE132
#define XZAE132

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
#define XZAE132C_RPC_NAME "XZAE132"

#define XZAE132C_NB_PARM_RETURN 2


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE132_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE132_Lire_Fiche_VehLent_Pour_Strada (	XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
					XDY_Mot		*va_longueur_out,
					XDY_Octet	*va_vitesse_out);

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
