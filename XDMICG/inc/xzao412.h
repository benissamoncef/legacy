/*E*/
/* Fichier : $Id: xzao412.h,v 1.1 1997/11/06 11:47:57 gesconf Exp $      Release : $Revision: 1.1 $        Date :$Date: 1997/11/06 11:47:57 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao412.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	10 Fev 1997	: Creation (DEM/1395)
------------------------------------------------------ */

#ifndef xzao412
#define xzao412

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO412_RPC_NAME "XZAO412"

#define XZAOC_XZAO412_NB_PARM_RETURN 14

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO412_Lire_Liste_Destination_TDP_SAD (	XDY_Basedd,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
