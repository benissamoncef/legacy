/*E*/
/*  Fichier : 	$Id: xzaz01.h,v 1.2 1995/03/07 17:29:38 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/03/07 17:29:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE XZAZ * FICHIER XZAZ.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* reception de travaux de satir
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	17/01/95	: Creation
* C.T.	06/03/95	: Modif interface de la fonction (V 1.2)
------------------------------------------------------ */

#ifndef XZAZ01
#define XZAZ01

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzaz.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAZC_XZAZ01_RPC_NAME "XZAZ;01"

#define XZAZC_XZAZ01_NB_PARM_RETURN 2

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAZ01_Ecrire_Travaux_Satir( XZAZT_Travaux , XDY_Evt*);
						
						
extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
