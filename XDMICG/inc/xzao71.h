/*E*/
/*  Fichier : $Id: xzao71.h,v 1.2 1994/10/20 11:31:49 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/10/20 11:31:49 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao71.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	22 Sep 1994	: Creation
* GGY	27/04/23	:	Ajout colonnes TypePnc et FichierPnc dans TYP_PCT (DEM-473)
------------------------------------------------------ */

#ifndef xzao71
#define xzao71

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO71_RPC_NAME "XZAO;71"

#define XZAOC_XZAO71_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_Nom		NomType;
   	XDY_Octet	NombreCaracteres;
   	XDY_NomFich	FichierPanneau;
	XDY_Octet	TypePnc;
	XDY_NomFich	FichierPnc;   	
} XZAOT_ConfTypPicto;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO71_Ajouter_Type_Picto (	XDY_Basedd,
					XZAOT_ConfTypPicto,
					XDY_ResConf *);


#endif
