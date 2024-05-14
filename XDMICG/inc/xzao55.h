/*E*/
/*  Fichier : $Id: xzao55.h,v 1.1 1994/10/05 19:13:14 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:13:14 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao55.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	22 Sep 1994	: Creation
------------------------------------------------------ */

#ifndef xzao55
#define xzao55

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO55_RPC_NAME "XZAO;55"

#define XZAOC_XZAO55_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct
{
   	XDY_Nom		NomPortion;
   	XDY_NomAuto	NomAutoroute;
   	XDY_NomSite	NomDistrict;
   	XDY_NomSite	NomDistrictRecouvrement;
   	XDY_PR		PRdebut;
   	XDY_PR		PRfin;

} XZAOT_ConfPortion;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO55_Ajouter_Portion ( 	XDY_Basedd,
					XZAOT_ConfPortion,
					XDY_ResConf *);

#endif
