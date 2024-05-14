/*E*/
/*  Fichier : $Id: xzao61.h,v 1.2 1994/12/15 15:23:28 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/12/15 15:23:28 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao61.h
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

#ifndef xzao61
#define xzao61

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO61_RPC_NAME "XZAO;61"

#define XZAOC_XZAO61_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_Nom		NomPortion;		
   	XDY_PR		PRdebut;
   	XDY_PR		PRfin;
   	XDY_Mot		Pente;	
   	XDY_Nom		NomPente;		
   	XDY_Booleen	Creation;

} XZAOT_ConfPente;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO61_Ajouter_Pente ( 	XDY_Basedd,
					XZAOT_ConfPente,
					XDY_ResConf *);


#endif
