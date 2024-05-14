/*E*/
/*Fichier :  $Id: xzao317.h,v 1.1 2008/02/13 13:24:30 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:30 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao317.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ    20 Septembre 2007    : Creation DEM 662
------------------------------------------------------ */

#ifndef xzao317
#define xzao317

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO317_RPC_NAME "XZAO;317"

#define XZAOC_XZAO317_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct { XDY_NomEqt     NomEQT;
                 XDY_Eqt        NumeroEQT;
                 XDY_PR         PR;
                 XDY_Sens       Sens;
                 XDY_Octet      NumeroAutoroute;
                 XDY_Octet      Voie;
                 XDY_Booleen    Inverse;

} XZAOT_EQTSyn;    

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO317_Liste_EQT_Synoptique ( XDY_Basedd,
                                          XDY_Nom,
                                          XDY_Booleen,
                                          XDY_FonctionInt,
                                          XDY_Entier * );

#endif
