/*E*/
/*Fichier : $Id: xzae170.h,v 1.1 2004/11/16 14:15:41 gesconf Exp $  Release : $Revision: 1.1 $        Date : $Date: 2004/11/16 14:15:41 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzae170.h
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

#ifndef XZAE170
#define XZAE170

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

#define XZAE170C_RPC_NAME "XZAE170"

#define XZAE170C_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAE170_Recup_Liste_Evt ( XDY_FonctionInt va_FonctionUtilisateur_in);

#endif
