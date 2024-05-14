/*E*/
/*Fichier :  $Id: xzao610.h,v 1.1 2008/02/13 13:24:31 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:31 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao610.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ      05 Sep 2007 : Creation DEM 662
------------------------------------------------------ */

#ifndef xzao610
#define xzao610

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO610_RPC_NAME "XZAO;610"

#define XZAOC_XZAO610_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct { XDY_Nom          NomSequence;
                 XDY_Booleen      Creation;
               } XZAOT_ConfSeqSIG;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO610_Ajouter_Sequence_SIG ( XDY_Basedd,
                                          XZAOT_ConfSeqSIG,
                                          XDY_ResConf *);


#endif

