/*E*/
/*Fichier : $Id: xzao131.h,v 1.3 1994/11/29 10:26:17 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1994/11/29 10:26:17 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao131.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
------------------------------------------------------ */

#ifndef xzao131
#define xzao131

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO131_RPC_NAME "XZAO;131"

#define XZAOC_XZAO131_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		NomPente;
	XDY_PR		PRdebut;
	XDY_PR		PRfin;
	XDY_Octet	NumeroAutoroute;
	XDY_Mot		Pente;

} XZAOT_Pente;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO131_Liste_Pentes (	XDY_Basedd,
					XDY_Nom,
					XDY_Mot,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
