/*E*/
/*  Fichier : 	$Id: xzag09.h,v 1.2 1995/09/12 19:27:22 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/09/12 19:27:22 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzag * FICHIER xzag09.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Sauvegarde et restauration de base de donnees
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	21/12/1994	: Creation
* C.T.	12/09/1995	: Ajout constantes
------------------------------------------------------*/

#ifndef xzag09
#define xzag09

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzag.h"
#include "stdio.h"

/* definitions de constantes exportees */
#define XZAGC_MES_RST_OK "Restauration reussie"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAG09_Restaurer_Base  (	XDY_NomSite ,
					char *,
					XZAGT_Volume ,
					char *,
					char *,
					char *,
					XZAGT_Bande ,
					char *);



#endif
