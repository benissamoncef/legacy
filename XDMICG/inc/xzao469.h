/*E*/
/* Fichier : $Id: xzao469.h,v 1.1 2012/11/08 09:54:25 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/11/08 09:54:25 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao469.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	05 Juin 2012	: Creation (DEM/1014 PMA)
------------------------------------------------------ */

#ifndef xzao469
#define xzao469

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO469_RPC_NAME "XZAO469"

#define XZAOC_XZAO469_NB_PARM_RETURN 14

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO469_Lire_Liste_Destination_PMVA (	XDY_Basedd,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
