/*E*/
/*Fichier : $Id: xzae200.h,v 1.1 2004/11/16 14:15:54 gesconf Exp $  Release : $Revision: 1.1 $        Date : $Date: 2004/11/16 14:15:54 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzae200.h
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

#ifndef XZAE200
#define XZAE200

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

#define XZAE200C_RPC_NAME "XZAE200"

#define XZAE200C_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAE200_Lire_Infos_TFM ( XDY_FonctionInt va_FonctionUtilisateur_in,
					XDY_Entier	va_cle_in,
					XDY_Octet      va_niveau_in);

#endif
