/*E*/
/*Fichier : $Id: xzao74.h,v 1.5 2018/10/08 14:44:03 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2018/10/08 14:44:03 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao74.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	22 Sep 1994	: Creation
* volcic	14 Oct 1994	: Modification de XDY_FonctionInt et suppr typedef (v1.2)
* volcic	27 Dec 1994	: Modification de NumType (v1.3)
* volcic        27 Dec 1994     : Modification de XZAOT_TypePicto (v1.4)
* JMG		02/08/18	; ajout picto suppression SAGA DEM1306 1.5
------------------------------------------------------ */

#ifndef xzao74
#define xzao74

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO74_RPC_NAME "XZAO74"

#define XZAOC_XZAO74_NB_PARM_RETURN 0

/* definitions de types exportes */
   
typedef struct {

   	XDY_Octet	NumType;
   	XDY_Octet	NombreLignes;
   	XDY_Octet	NombreCaracteres;
   	XDY_Booleen	Flash;
   	XDY_Booleen	Temperature;
   	XDY_Octet	Picto;
   	
} XZAOT_TypePMV;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO74_Lire_Types_PMV (	XDY_Basedd,
					XDY_FonctionInt,
					XDY_ResConf *);

#endif
