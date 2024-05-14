/*E*/
/*Fichier :  $Id: xzao980.h,v 1.1 2008/02/13 13:24:35 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:35 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao980.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ      15 Nov 2007    : Creation DEM 662
------------------------------------------------------ */

#ifndef xzao980
#define xzao980

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO980_RPC_NAME "XZAO;980"

#define XZAOC_XZAO980_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
    XDY_NomEqt      NomDemiEch;
    XDY_NomAuto     Autoroute;
    XDY_NomEqt      NomServeur;
    XDY_PR          PR;
    XDY_Sens        Sens;
    
} XZAOT_ConfDemiEchRenove;        

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO980_Ajouter_Demi_Ech_Renove ( XDY_Basedd,
                                             XZAOT_ConfDemiEchRenove,
                                             XDY_ResConf *);

#endif
