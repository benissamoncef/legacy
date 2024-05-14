/*E*/
/*Fichier : $Id: xzae155.h,v 1.1 1999/03/19 16:35:32 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/03/19 16:35:32 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzae155.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* niepceron	05 Mar 1999	: Creation 
------------------------------------------------------ */

#ifndef XZAE55
#define XZAE155

/* fichiers inclus */
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

#define XZAE155C_RPC_NAME "XZAE155"

#define XZAE155C_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAE155_LireCommentaires (	XDY_Entier	va_NumEvt_in,
					XDY_Octet	va_CleEvt_in,
					XDY_FonctionInt va_FonctionUtilisateur_in);

#endif
