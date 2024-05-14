/*E*/
/* Fichier : $Id: xzao193.h,v 1.1 2010/10/07 18:37:05 gesconf Exp $        Release : $Revision: 1.1 $        Date : $Date: 2010/10/07 18:37:05 $
--------------------------------------------------------------------------------
* ESCOTA * PROJET MIGRAZUR
--------------------------------------------------------------------------------
* SOUS-SYSTEME BASEDD
--------------------------------------------------------------------------------
* MODULE XZAO * FICHIER xzao193.h
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
--------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL		07/10/2010 : Creation  (DEM 948)  1.1
----------------------------------------------------------------------------- */

#ifndef xzao193
#define xzao193

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO193_RPC_NAME "XZAO193"

#define XZAOC_XZAO193_NB_PARM_RETURN 1

/* definitions de types exportes */


typedef struct {

   	XDY_NomEqt	Nom;
   	XDY_NomAuto	Autoroute;
	XDY_Sens	Sens;
	XDY_PR		PRdebut;
	XDY_PR		PRfin;
	XDY_Octet	Pertinence;

} XZAOT_ConfZoneCamera;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO193_Ajouter_Zon_Camera (	XDY_Basedd,
					XZAOT_ConfZoneCamera,
					XDY_ResConf *);


#endif
