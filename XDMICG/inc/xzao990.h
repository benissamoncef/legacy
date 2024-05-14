/*E*/
/*Fichier :  $Id: xzao990.h,v 1.1 2008/02/13 13:24:38 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:38 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao990.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ    27 Sep 2007 : Creation DEM 662
------------------------------------------------------ */

#ifndef xzao990
#define xzao990

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO990_RPC_NAME "XZAO;990"

#define XZAOC_XZAO990_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
    XDY_Nom        NomSequence;
    XDY_Octet      Type;
    XDY_NomEqt     NomINC;
    XDY_Booleen    Creation;
} XZAOT_ConfSeqAutINC;        

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO990_Ajouter_SeqAut_INC ( XDY_Basedd,
                                        XZAOT_ConfSeqAutINC,
                                        XDY_ResConf *);



#endif
