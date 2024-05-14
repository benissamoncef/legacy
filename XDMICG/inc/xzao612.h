/*E*/
/*Fichier :  $Id: xzao612.h,v 1.1 2008/02/13 13:24:32 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:32 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao612.h
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

#ifndef xzao612
#define xzao612

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO612_RPC_NAME "XZAO;612"

#define XZAOC_XZAO612_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct { XDY_Nom          NomSequence;
                 XDY_Booleen      Creation;
               } XZAOT_ConfSeqVEN;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO612_Ajouter_Sequence_VEN ( XDY_Basedd,
                                          XZAOT_ConfSeqVEN,
                                          XDY_ResConf *);


#endif

