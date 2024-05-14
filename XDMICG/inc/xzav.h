/*E*/
/*Fichier :  @(#)xzav.h	1.25      Release : 1.25        Date : 22/03/12
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAV * FICHIER XZAV.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		22/03/12	: Creation (DEM/1016)
------------------------------------------------------ */

#ifndef _XZAV_COMPLET_
#define _XZAV_COMPLET_

/* fichiers inclus */


#include <fcntl.h>

#if defined (__hp9000s300)
#include <tcio.h>
#endif

#include "xdy.h"
#include "asql_com.h"
#include "xzat.h"
#include "xzav01.h"
#include "xzav03.h"
#include "xzav04.h"




/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
