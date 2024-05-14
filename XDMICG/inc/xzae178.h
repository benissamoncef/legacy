/*E*/
/*Fichier : $Id: xzae178.h,v 1.1 2008/11/17 10:45:20 pc2dpdy Exp $  Release : $Revision: 1.1 $        Date : $Date: 2008/11/17 10:45:20 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzae178.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.1 DEM 836
------------------------------------------------------ */

#ifndef XZAE178
#define XZAE178

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

#define XZAE178C_RPC_NAME "XZAE178"

#define XZAE178C_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAE178_Lire_Config_Web ( XDY_FonctionInt va_FonctionUtilisateur_in);

#endif
