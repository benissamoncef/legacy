/*E*/
/*  Fichier : $Id: xzao70.h,v 1.3 2018/10/08 14:45:30 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2018/10/08 14:45:30 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao70.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	22 Sep 1994	: Creation
* JMG	12/08/18 : ajout picto. suppression SAGA DEM1306 1.3
------------------------------------------------------ */

#ifndef xzao70
#define xzao70

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO70_RPC_NAME "XZAO70"

#define XZAOC_XZAO70_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_Nom		NomType;
   	XDY_Octet	NombreLignes;
   	XDY_Octet	NombreCaracteres;
   	XDY_Booleen	Flash;
   	XDY_Booleen	Temperature;
   	XDY_Octet	Picto;
   	
} XZAOT_ConfTypPMV;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO70_Ajouter_Type_PMV (	XDY_Basedd,
					XZAOT_ConfTypPMV,
					XDY_ResConf *);



#endif
