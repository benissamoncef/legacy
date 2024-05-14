/*E*/
/*Fichier :  $Id: xzao986.h,v 1.1 2008/02/13 13:24:37 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:37 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao986.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ    27 Sep 2007    : Creation DEM 662
------------------------------------------------------ */

#ifndef xzao986
#define xzao986

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO986_RPC_NAME "XZAO;986"

#define XZAOC_XZAO986_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
    XDY_Nom        NomSequence;
    XDY_Octet      Type;
    XDY_NomEqt     NomVEN;
    XDY_Booleen    Creation;
} XZAOT_ConfSeqAutVEN;        

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO986_Ajouter_SeqAut_VEN ( XDY_Basedd,
                                        XZAOT_ConfSeqAutVEN,
                                        XDY_ResConf *);



#endif
