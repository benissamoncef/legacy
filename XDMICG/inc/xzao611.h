/*E*/
/*Fichier :  $Id: xzao611.h,v 1.1 2008/02/13 13:24:31 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:31 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao611.h
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

#ifndef xzao611
#define xzao611

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO611_RPC_NAME "XZAO;611"

#define XZAOC_XZAO611_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct { XDY_Nom          NomSequence;
                 XDY_Booleen      Creation;
               } XZAOT_ConfSeqECL;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO611_Ajouter_Sequence_ECL ( XDY_Basedd,
                                          XZAOT_ConfSeqECL,
                                          XDY_ResConf *);


#endif

