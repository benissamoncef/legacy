/*E Fichier : $Id: MSRA.h,v 1.1 1994/09/28 10:54:48 niepceron Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/09/28 10:54:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GESIHM
------------------------------------------------------
* MODULE MSRA * FICHIER MSRA.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Creation et Mise a jour des Variables d'animations 
*   graphiques
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	12 Sep 1994	: Creation
------------------------------------------------------ */

#ifndef MSRA
#define MSRA

/* fichiers inclus */
#include "std.h"         
#include "math.h"        
#include "dvstd.h"       /* public types & constants */
#include "dvtools.h"     /* constants used by T routines */
#include "dvGR.h"	 /* constants used by window mgt & GR routines */
#include "VOstd.h"       /* constants used by VO & VOob routines */
/*#include "hashtypes.h"*/
#include "VTfundecl.h"
#include "Tfundecl.h"   /* T routines (screens, drawports & views) */
#include "VOfundecl.h"   /* VO routines (objects) */
#include "VUerfundecl.h" /* VUer routines (event handling routines) */

#include "xzst.h"
#include "xzsc.h"
#include "xzis.h"
#include "xdy.h"

#include "MSVU_hashtypes.h"

/* definitions de constantes exportees */

#define CM_RAF_TRONCON  0
#define CM_RAF_DISTRICT 1
#define CM_RAF_GENERALE 2


/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* declarations de donnees externes */


extern VIEW  vm_view_en_cours;   /* View du synoptique en cours */

/* delaration de fonctions externes */

#endif
