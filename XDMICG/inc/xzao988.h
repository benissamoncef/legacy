/*E*/
/*Fichier :  $Id: xzao988.h,v 1.1 2008/02/13 13:24:37 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:37 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao988.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic    27 Sep 1994    : Creation DEM 662
------------------------------------------------------ */

#ifndef xzao988
#define xzao988

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO988_RPC_NAME "XZAO;988"

#define XZAOC_XZAO988_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
    XDY_Nom        NomSequence;
    XDY_Octet      Type;
    XDY_NomEqt     NomENR;
    XDY_Booleen    Creation;
} XZAOT_ConfSeqAutENR; 

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO988_Ajouter_SeqAut_ENR ( XDY_Basedd,
                                        XZAOT_ConfSeqAutENR,
                                        XDY_ResConf *);



#endif
