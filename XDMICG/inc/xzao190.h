/*E*/
/*  Fichier : $Id: xzao190.h,v 1.1 1996/01/29 11:00:03 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/01/29 11:00:03 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao190.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* description des equipement du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* CT	01 Dec 1995	: Creation
------------------------------------------------------ */

#ifndef xzao190
#define xzao190

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO190_RPC_NAME "XZAO;190"

#define XZAOC_XZAO190_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	NomFax;
	XDY_Nom		NomDistrict;
	
} XZAOT_ConfFax;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO190_Ajouter_Fax (	XDY_Basedd,
					XZAOT_ConfFax,
					XDY_ResConf *);



#endif
