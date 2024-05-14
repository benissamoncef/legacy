/*E*/
/*Fichier :  $Id: xzao0166.h,v 1.1 2008/02/13 13:24:25 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:25 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao0166.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ  	13 Nov 2007	: Creation DEM 662
------------------------------------------------------ */

#ifndef xzao0166
#define xzao0166

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO0166_RPC_NAME "XZAO;1166"

#define XZAOC_XZAO0166_NB_PARM_RETURN 1

/* definitions de types exportes */


typedef struct {
        XDY_NomEqt          Nom;
        XDY_NomMachine      NomMachine;

} XZAOT_ConfGTCNICERENOVEE;	

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO0166_Ajouter_GTC_NICE_RENOVEE ( XDY_Basedd,
                                               XZAOT_ConfGTCNICERENOVEE,
                                               XDY_ResConf *);


#endif
