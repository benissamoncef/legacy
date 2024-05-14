/*E*/
/*  Fichier : @(#)xzao953.h	1.0      Release : 1.0        Date : 26/06/17
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao953.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL		26 Jun 2017	: Creation (convergence)
------------------------------------------------------ */

#ifndef xzao953
#define xzao953

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO953_RPC_NAME "XZAO953"

#define XZAOC_XZAO953_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_Octet	Numero;
   	XDY_NomAuto	Autoroute;
   	XDY_Sens	Sens;
   	XDY_PR		PRdebut;
	XDY_PR		PRfin;
	
} XZAOT_ConfZoneUrbaine;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO953_Ajouter_ZoneUrbaine (	XDY_Basedd,
					XZAOT_ConfZoneUrbaine,
					XDY_ResConf *);



#endif
