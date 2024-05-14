/*E*/
/*Fichier : $Id: xzao127.h,v 1.3 1994/11/28 14:22:06 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1994/11/28 14:22:06 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao127.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	29 Nov 1994	: Ajout XZAOT_Troncon (1.3)
------------------------------------------------------ */

#ifndef xzao127
#define xzao127

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO127_RPC_NAME "XZAO;127"

#define XZAOC_XZAO127_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct
{
   
   XDY_Nom	Nom;
   XDY_Entier	PRdebut;
   XDY_Entier	PRfin;

} XZAOT_Troncon;   


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO127_Liste_Troncons (	XDY_Basedd,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
