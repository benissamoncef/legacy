/*E Fichier : @(#)xzaa.h	1.6      Release : 1.6        Date : 03/04/2012
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAA * FICHIER XZAA.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	03/04/2012	: Creation (DEM/1016)
------------------------------------------------------ */

#ifndef XZAA30
#define XZAA30

/* fichiers inclus */
	#include        <stdio.h>
	#include        <string.h>
	#include        <ctype.h>

	/* include system necessaire pour Open Server et Open Client. */

	#include "asql_com.h"
	#include <ctpublic.h>
	#include <cstypes.h>

	/* Include service migrazur */
	 #include "xdy.h"
	 #include "xdm.h"
	 #include "xzst.h"
	 #include "xzsc.h"

/* definitions de constantes exportees */

#define XZAAC_XZAA30_RPC_NAME "XZAA;30"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAA30_Liste_Infos_Alerte_Regulation (XDY_Basedd,
										   XDY_Entier,
										   XDY_Entier*,
										   XDY_Entier*,
										   XDY_Entier*);

#ifdef _HPUX_SOURCE
extern tg_Connexion  *XZAGV_Connexion;
#endif
#endif
