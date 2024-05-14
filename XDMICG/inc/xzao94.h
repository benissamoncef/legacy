/*E*/
/*  Fichier : $Id: xzao94.h,v 1.1 1994/10/05 19:14:04 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:14:04 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao94.h
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

#ifndef xzao94
#define xzao94

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO94_RPC_NAME "XZAO;94"

#define XZAOC_XZAO94_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_Nom		Nom;
	XDY_Booleen	Creation;
	
} XZAOT_ConfVille;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO94_Ajouter_Ville (	XDY_Basedd,
				 	XZAOT_ConfVille,
					XDY_ResConf *);



#endif
