/*E*/
/*  Fichier : 	$Id: xzaf15.h,v 1.3 1995/05/29 15:20:00 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/29 15:20:00 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzaf * FICHIER xzaf15.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'interface de la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.       17/01/95     : Creation
* C.T.       29/05/95     : Ajout fonction sur echangeur
*                           en argument en entree
* C.T.       29/05/95     : correction erreur de compilation
*                           (V 1.3)
------------------------------------------------------*/

#ifndef xzaf15
#define xzaf15

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAF15_Animer_Formation (	XDY_Horodate,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XDY_FonctionInt);
						
						
extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
