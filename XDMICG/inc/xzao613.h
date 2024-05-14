/*E*/
/*Fichier :  $Id: xzao613.h,v 1.1 2008/02/13 13:24:32 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:32 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao613.h
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

#ifndef xzao613
#define xzao613

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO613_RPC_NAME "XZAO;613"

#define XZAOC_XZAO613_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct { XDY_Nom          NomSequence;
                 XDY_Booleen      Creation;
               } XZAOT_ConfSeqENR;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO613_Ajouter_Sequence_ENR ( XDY_Basedd,
                                          XZAOT_ConfSeqENR,
                                          XDY_ResConf *);


#endif

