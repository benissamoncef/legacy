/*E*/
/*Fichier : $Id: xzao147.h,v 1.2 1994/10/20 11:31:18 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/10/20 11:31:18 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao147.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	16 Oct 1994	: Modif XDY_FonctionInt
------------------------------------------------------ */

#ifndef xzao147
#define xzao147

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO147_RPC_NAME "XZAO;147"

#define XZAOC_XZAO147_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Octet	NumeroAutoroute;
	XDY_PR		PR;
	
} XZAOT_ITPC;


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO147_Liste_ITPC (	XDY_Basedd,
				XDY_Nom,
				XDY_Mot,
				XDY_FonctionInt,
				XDY_Entier * );

#endif
