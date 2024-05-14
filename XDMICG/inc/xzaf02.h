/*E*/
/*  Fichier : 	$Id: xzaf02.h,v 1.4 1995/03/30 17:41:41 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/03/30 17:41:41 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzaf * FICHIER xzaf02.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'interface de la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.       12/01/95     : Creation
* C.T.       25/03/95     : Modif interface fonction
*                           prise en compte des evts sur 
*                           echangeur (V 1.3)
* C.T.       30/03/95     : Ajout de l'include de xzahc.h
*                           (V 1.4)
------------------------------------------------------*/

#ifndef xzaf02
#define xzaf02

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzahc.h"
#include "xzaf.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAFC_XZAF02_RPC_NAME "XZAF;02"

#define XZAFC_XZAF02_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAF02_Animer_Objet_Mobile ( XDY_Horodate,
					XDY_FonctionInt,
					XDY_FonctionInt);
						
						
extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
