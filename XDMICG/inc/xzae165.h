/*E*/
/*Fichier : $Id: xzae165.h,v 1.1 2001/04/19 16:27:04 gesconf Exp $  Release : $Revision: 1.1 $        Date : $Date: 2001/04/19 16:27:04 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzae165.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.1
------------------------------------------------------ */

#ifndef XZAE165
#define XZAE165

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

#define XZAE165C_RPC_NAME "XZAE165"

#define XZAE165C_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAE165_Lire_Icones ( XDY_FonctionInt va_FonctionUtilisateur_in,
					XDY_Octet      va_niveau_in);

#endif
