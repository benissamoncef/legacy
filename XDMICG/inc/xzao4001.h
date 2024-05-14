/*E*/
/* Fichier : $Id: xzao4001.h,v 1.1 2017/10/10 15:12:19 devgfi Exp $      Release : $Revision: 1.1 $        Date : $Date: 2017/10/10 15:12:19 $
----------------------------------------------------------------------------------------------------
* ESCOTA * PROJET MIGRAZUR
----------------------------------------------------------------------------------------------------
* SOUS-SYSTEME BASEDD
----------------------------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao4001.h
----------------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
----------------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	05/10/17 : Creation d'apres XZAO400 dont elle constitue la suite (DEM 1236)
------------------------------------------------------------------------------------------------- */

#ifndef xzao4001
#define xzao4001

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO4001_RPC_NAME "XZAO4001"

#define XZAOC_XZAO4001_NB_PARM_RETURN 16

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO4001_Liste_Zones_De_Parcours ( XDY_Basedd,
                                              XDY_District,
                                              XDY_FonctionInt,
                                              XDY_Entier * );

#endif
