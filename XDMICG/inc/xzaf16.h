/*E*/
/*  Fichier : 	$Id: xzaf16.h,v 1.1 1995/01/23 19:28:26 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/23 19:28:26 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzaf * FICHIER xzaf14.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'interface de la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.       23/01/95     : Creation
------------------------------------------------------*/

#ifndef xzaf23
#define xzaf23

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzaf.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAFC_XZAF16_RPC_NAME "XZAF;16"

#define XZAFC_XZAF16_NB_PARM_RETURN 4

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAF16_Lire_Niveaux_Station( XDY_Mot, 
					XDY_Horodate, 
					XDY_Sens *, 
					XDY_Octet *, 
					XDY_Sens *, 
					XDY_Octet *);
						
						
extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
