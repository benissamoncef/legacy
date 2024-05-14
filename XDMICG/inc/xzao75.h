/*E*/
/*Fichier : $Id: xzao75.h,v 1.5 1994/12/27 23:30:59 volcic Exp $      Release : $Revision: 1.5 $       Date : $Date: 1994/12/27 23:30:59 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao75.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	14 Oct 1994	: Modification de XDY_FonctionInt et suppr typedef (v1.2)
* volcic	07 Dec 1994	: Modification de XDY_NomFichier en XDY_Nom (v1.3)
* GGY    	22/06/23	: Ajout parametre va_TypePnc_in et va_FichierPnc_in (DEM-473)
------------------------------------------------------ */

#ifndef xzao75
#define xzao75

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO75_RPC_NAME "XZAO;75"

#define XZAOC_XZAO75_NB_PARM_RETURN 1

/* definitions de types exportes */
   	
/* XDY_NomFich	FichierPanneau; */

typedef struct {

   	XDY_Octet	NumType;
   	XDY_Octet	NombreCaracteres;
   	XDY_Nom		FichierPanneau;
	XDY_Entier	TypePnc;
	XDY_Nom		FichierPnc;
} XZAOT_TypPicto;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO75_Lire_Types_Picto (	XDY_Basedd,
					XDY_FonctionInt,
					XDY_ResConf *);

#endif
