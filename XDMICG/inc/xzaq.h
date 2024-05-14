/*E*/
/* Fichier : $Id: xzaq.h,v 1.2 1995/09/20 19:26:22 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/09/20 19:26:22 $
------------------------------------------------------
* STERIA  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzaq * FICHIER xzaq.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface indice qualite >
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit       18/01/95     : Creation
* gaborit       20/09/95	: modif arg XZAQ01_Mise_A_Jour_Etat XDY_Booleen->XDY_Octet (1.2)
------------------------------------------------------*/

#ifndef xzaq
#define xzaq

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAQC_XZAQ01_RPC_NAME "XZAQ;01"
#define XZAQC_XZAQ02_RPC_NAME "XZAQ;02"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAQ01_Mise_A_Jour_Etat (	XDY_Nom,
					XDY_Nom,
					XDY_Nom,
					XDY_Octet);
				
						
extern int XZAQ02_Arret_SUPERV ();
						
extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif

