/*E*/
/*Fichier : $Id: xzad04.h,v 1.4 1997/05/15 09:58:05 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1997/05/15 09:58:05 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAD04 * FICHIER XZAD04.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	24 Fev 1997	: Creation (dem/tdp) 1.2
------------------------------------------------------ */

#ifndef XZAD04
#define XZAD04

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
#define XZAD04C_RPC_NAME "XZAD04"

#define XZAD04C_NB_PARM_RETURN 0
/* definitions de types exportes */

typedef struct {
        XDY_TDP 	Numero;
        XDY_Horodate 	HorodateFin;
        } XZAD04_Ligne;


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAD04_Liste_Franchissements : recupere la liste de 
franchissement pour un site ,une fmc et horodate donn≈s.
*  Appelle XZAD04SP
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAD04_Liste_Franchissements (	XDY_Octet       va_NumeroSite_in ,
					XDY_Horodate	va_Horodate_in,
					XDY_Entier	va_NoFMC_in,
					XDY_Octet	va_CleFMC_in,
					XDY_FonctionInt va_fct_in);

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Octet               va_NumeroSite_in   : numero du Site
* XDY_Datetime          va_Horodate_in
* XDY_TDP		va_NoFMC_in
* XDY_Octet		va_CleFMC_in
* XDY_FonctionInt va_fct_in

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
