/*E*/
/*  Fichier : 	$Id: xzag08.h,v 1.4 1995/09/12 19:27:19 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/09/12 19:27:19 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzag * FICHIER xzag08.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Sauvegarde et restauration de base de donnees
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	20/12/1994	: Creation
* C.T.	12/09/1995	: Ajout constantes 
* C.T.	12/09/1995	: Modif constante XZAGC_MES_SVG_OK 
* C.T.	12/09/1995	: Suppression constante XZAGC_NB_CARACT_A_LIRE 
------------------------------------------------------*/

#ifndef xzag08
#define xzag08

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzag.h"
#include "stdio.h"

/* definitions de constantes exportees */
#define	 XZAGC_MES_SVG_OK	"Sauvegarde reussie"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAG08_Sauvegarder_Base  (	XDY_NomSite ,
					char *,
					XZAGT_Volume ,
					char *,
					char *,
					char *,
					XDY_Entier, 
					XZAGT_Bande ,
					char *);



#endif
