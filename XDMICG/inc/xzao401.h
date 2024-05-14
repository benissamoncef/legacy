/*E*/
/* Fichier : $Id: xzao401.h,v 1.2 2012/02/14 14:28:38 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2012/02/14 14:28:38 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao401.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	10 Fev 1997	: Creation (DEM/1395)
* JMG	07/02/12	: tdp etendu dem/1014
------------------------------------------------------ */

#ifndef xzao401
#define xzao401

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO401_RPC_NAME "XZAO401"

#define XZAOC_XZAO401_NB_PARM_RETURN 14

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO401_Lire_Liste_Destination_TDP (	XDY_Basedd,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
