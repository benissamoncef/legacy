/*E*/
/*Fichier :  $Id: xzao992.h,v 1.1 2008/02/13 13:24:38 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:38 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao992.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       21 Nov 2007    : Creation DEM 662
------------------------------------------------------ */

#ifndef xzao992
#define xzao992

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO992_RPC_NAME "XZAO992"

#define XZAOC_XZAO992_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
    XDY_Texte      TypeEqt;
    XDY_NomEqt     NomEqt;
    XDY_NomAuto    Autoroute;
    XDY_NomEqt     NomMaitre;
    XDY_PR         PR;
    XDY_Sens       Sens;
    XDY_Voie       Voie;
    XDY_Octet      Inverse;
    XDY_NomEqt     Domaine;
    
} XZAOT_ConfEqt;        

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO992_Ajouter_Equipement ( XDY_Basedd,
                                        XZAOT_ConfEqt,
                                        XDY_ResConf *);

#endif
