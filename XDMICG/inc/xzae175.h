/*E*/
/*Fichier : @(#)xzae175.h        1.0      Release : 1.0        Date : 06/02/
------------------------------------------------------
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAM01 * FICHIER XZAE175.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
------------------------------------------------------ */


#ifndef XZAE175
#define XZAE175

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
#define XZAE175_RPC_NAME "XZAE175"

#define XZAE175_NB_PARM_RETURN 4


/* definitions de types exportes */


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU :
*  XZAE175_Get_6Min_Sequence : description de la fonction
*  Appelle XZAE175
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE175_Get_6Min_Sequence (XDY_FonctionInt pa_FonctionUtilisateur_in);

/*
*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*
* CODE RETOUR :
*
* 0     XDC_OK
* 1     XDC_NOK
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




