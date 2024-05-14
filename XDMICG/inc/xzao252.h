/*E*/
/*Fichier : $Id: xzao252.h,v 1.1 2012/07/04 10:58:14 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:58:14 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao252.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  VR	21/12/11	: Creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzao252
#define xzao252

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO252_RPC_NAME "XZAO252"

#define XZAOC_XZAO252_NB_PARM_RETURN 12

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO252_Lire_Liste_Zone_Influence_PRV (XDY_Basedd,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
