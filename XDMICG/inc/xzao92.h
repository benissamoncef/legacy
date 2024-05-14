/*E*/
/*  Fichier : $Id: xzao92.h,v 1.1 1994/10/05 19:14:02 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:14:02 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao92.h
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

#ifndef xzao92
#define xzao92

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO92_RPC_NAME "XZAO;92"

#define XZAOC_XZAO92_NB_PARM_RETURN 1

/* definitions de types exportes */
	
typedef struct {

   	XDY_Nom		NomReseau;
   	XDY_Octet	TypeReseau;
   	XDY_NomAuto	Autoroute;
   	XDY_PR		PR;
	XDY_Booleen	Creation;
	
} XZAOT_ConfReseau;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO92_Ajouter_Reseau (	XDY_Basedd,
				 	XZAOT_ConfReseau,
					XDY_ResConf *);

#endif
