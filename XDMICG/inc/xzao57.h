/*E*/
/*  Fichier : $Id: xzao57.h,v 1.1 1994/10/05 19:13:18 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:13:18 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao57.h
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

#ifndef xzao57
#define xzao57

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO57_RPC_NAME "XZAO;57"

#define XZAOC_XZAO57_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct
{
   	XDY_Nom		NomPortion1;
   	XDY_Nom		NomPortion2;
   	XDY_Booleen	ExtremitePortion1;
   	XDY_Booleen	ExtremitePortion2;
   	XDY_Booleen	Creation;

} XZAOT_ConfNoeud; 

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO57_Ajouter_Noeud ( 	XDY_Basedd,
					XZAOT_ConfNoeud,
					XDY_ResConf *);


#endif
