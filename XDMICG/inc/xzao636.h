/*E*/
/*Fichier :  $Id: xzao636.h,v 1.1 2008/02/13 13:24:34 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:34 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao636.h
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

#ifndef xzao636
#define xzao636

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO636_RPC_NAME "XZAO;636"

#define XZAOC_XZAO636_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO636_Rech_SYS ( XDY_Autoroute,
                              XDY_PR,
                              XDY_Octet,
                              XDY_FonctionInt,
                              XDY_Horodate,
                              XDY_PR);

#endif
