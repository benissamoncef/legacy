/*E*/
/*  Fichier : $Id: xzao68.h,v 1.1 1994/10/05 19:13:37 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:13:37 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao68.h
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

#ifndef xzao68
#define xzao68

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO68_RPC_NAME "XZAO;68"

#define XZAOC_XZAO68_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {

   	XDY_NomEqt	NomStation;  	
   	XDY_Sens	Sens;
	XDY_PR		PR;

} XZAOT_ConfChau;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO68_Ajouter_Chaussee (	XDY_Basedd,
					XZAOT_ConfChau,
					XDY_ResConf *);


#endif
