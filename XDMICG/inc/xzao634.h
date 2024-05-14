/*E*/
/*Fichier :  $Id: xzao634.h,v 1.1 2008/02/13 13:24:33 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:33 $
--
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao634.h
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

#ifndef xzao634
#define xzao634

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO634_RPC_NAME "XZAO;634"

#define XZAOC_XZAO634_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO634_Rech_INC ( XDY_Autoroute,
                              XDY_PR,
                              XDY_Octet,
                              XDY_FonctionInt,
                              XDY_Horodate,
                              XDY_PR);

#endif
