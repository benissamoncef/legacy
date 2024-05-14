/*E*/
/*Fichier : $Id: xzao136.h,v 1.4 2009/01/27 11:23:14 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 2009/01/27 11:23:14 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao136.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	18 Oct 1994	: Modif test (v1.2)
* volcic	20 Oct 1994	: Suppr *arg sortie (v1.3)
* Niepceron	27 Jan 2009	: renommage de la proc v1.4 DEM800
------------------------------------------------------ */

#ifndef xzao136
#define xzao136

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO136_RPC_NAME "XZAO136"

#define XZAOC_XZAO136_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO136_Lire_Suppleant (	XDY_Basedd,
					XDY_NomEqt,
					XDY_NomEqt );

#endif
