/*E*/
/*  Fichier : $Id: xzao157.h,v 1.1 1994/10/05 19:12:39 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:12:39 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao157.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Sep 1994	: Creation
------------------------------------------------------ */

#ifndef xzao157
#define xzao157

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO157_RPC_NAME "XZAO;157"

#define XZAOC_XZAO157_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		NomType;
	XDY_Booleen	FlashMod4;
	XDY_Octet	NbBarrieres;

} XZAOT_ConfTypNAV;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO157_Ajouter_Type_NAV (	XDY_Basedd,
					XZAOT_ConfTypNAV,
					XDY_ResConf *);



#endif
