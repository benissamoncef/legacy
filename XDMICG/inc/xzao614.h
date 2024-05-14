/*E*/
/*Fichier :  $Id: xzao614.h,v 1.1 2008/02/13 13:24:32 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:32 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao614.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ      06 Sep 2007 : Creation DEM 662
------------------------------------------------------ */

#ifndef xzao614
#define xzao614

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO614_RPC_NAME "XZAO;614"

#define XZAOC_XZAO614_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct { XDY_Nom          NomSequence;
                 XDY_Booleen      Creation;
               } XZAOT_ConfSeqINC;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO614_Ajouter_Sequence_INC ( XDY_Basedd,
                                          XZAOT_ConfSeqINC,
                                          XDY_ResConf *);


#endif

