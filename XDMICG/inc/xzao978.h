/*E*/
/*Fichier :  $Id: xzao978.h,v 1.1 2008/02/13 13:24:35 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:35 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao978.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ    15 Nov 2007    : Creation DEM 662
------------------------------------------------------ */

#ifndef xzao978
#define xzao978

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO978_RPC_NAME "XZAO;978"

#define XZAOC_XZAO978_NB_PARM_RETURN 1

/* definitions de types exportes */
    
typedef struct {
        XDY_NomEqt     NomTube;
        XDY_NomAuto    Autoroute;
        XDY_NomEqt     NomServeur;
        XDY_PR         PR;
        XDY_PR         PRInfluence;
        XDY_Sens       Sens;
} XZAOT_ConfTubeRenove;        

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO978_Ajouter_Tube_Renove ( XDY_Basedd,
                                         XZAOT_ConfTubeRenove,
                                         XDY_ResConf *);

#endif
