/*E*/
/* Fichier : $Id: xzao262.h,v 1.1 2018/01/05 21:04:25 devgfi Exp $        Release : $Revision: 1.1 $        Date : $Date: 2018/01/05 21:04:25 $
--------------------------------------------------------------------------------
* ESCOTA * PROJET MIGRAZUR
--------------------------------------------------------------------------------
* SOUS-SYSTEME BASEDD
--------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao262.h
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
--------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL		05/01/17 : Creation  (DEM 1266)  1.1
----------------------------------------------------------------------------- */

#ifndef xzao262
#define xzao262

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO262_RPC_NAME "XZAO262"

#define XZAOC_XZAO262_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

	XDY_NomEqt	Nom_Station_DIT;
	XDY_NomEqt	Nom_Station_RADT;
	XDY_Sens	Sens_RADT;

} XZAOT_Conf_Station_DIT;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO262_Ajouter_Station_DIT (	XDY_Basedd,
						XZAOT_Conf_Station_DIT,
						XDY_ResConf * );


#endif
