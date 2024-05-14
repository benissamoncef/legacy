/*E*/
/* Fichier : $Id: xzao427.h,v 1.1 2005/09/13 18:11:40 gesconf Exp $     Release : $Revision: 1.1 $        Date : $Date: 2005/09/13 18:11:40 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao427.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	16/06/05	: Creation 
------------------------------------------------------ */

#ifndef xzao427
#define xzao427

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO427_RPC_NAME "XZAO427"

#define XZAOC_XZAO427_NB_PARM_RETURN 12

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO427_Lire_Liste_Zone_Influence_PMVA (XDY_Basedd,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
