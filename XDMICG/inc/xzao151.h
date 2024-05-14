/*E*/
/*  Fichier : $Id: xzao151.h,v 1.1 1994/10/05 19:12:35 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:12:35 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao151.h
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

#ifndef xzao151
#define xzao151

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO151_RPC_NAME "XZAO;151"

#define XZAOC_XZAO151_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_Nom		NomPortion;
   	XDY_PR		PR;
   	XDY_Sens	Sens;
   	XDY_Booleen	Creation;

} XZAOT_ConfPortail;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO151_Ajouter_Portail (	XDY_Basedd,
					XZAOT_ConfPortail,
					XDY_ResConf *);



#endif
