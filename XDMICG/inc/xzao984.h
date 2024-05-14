/*E*/
/*Fichier :  $Id: xzao984.h,v 1.1 2008/02/13 13:24:36 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:36 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao984.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ 27 Sep 2007 : Creation DEM 662
------------------------------------------------------ */

#ifndef xzao984
#define xzao984

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO984_RPC_NAME "XZAO;984"

#define XZAOC_XZAO984_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
    XDY_Nom        NomSequence;
    XDY_Octet      Type;
    XDY_NomEqt     NomECL;
    XDY_Booleen    Creation;
} XZAOT_ConfSeqAutECL;        

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO984_Ajouter_SeqAut_ECL ( XDY_Basedd,
                                        XZAOT_ConfSeqAutECL,
                                        XDY_ResConf *);



#endif
