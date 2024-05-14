/*E*/
/*Fichier :  $Id: xzao637.h,v 1.1 2008/02/13 13:24:34 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:34 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao637.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       24 Septembre 2007  : Creation DEM 662
------------------------------------------------------ */

#ifndef xzao637
#define xzao637

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO637_RPC_NAME "XZAO;637"

#define XZAOC_XZAO637_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO637_Rech_LTN ( XDY_Autoroute,
                              XDY_PR,
                              XDY_Octet,
                              XDY_FonctionInt,
                              XDY_Horodate,
                              XDY_PR);

#endif
