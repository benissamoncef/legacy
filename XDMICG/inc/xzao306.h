/*E*/
/*Fichier : $Id: xzao306.h,v 1.3 2016/06/22 18:02:58 devgfi Exp $      Release : $Revision: 1.3 $        Date : $Date: 2016/06/22 18:02:58 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao306.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	09 Mar 1995	: Creation
* C.T.	10 Mar 1995	: Modif include fichier (V 1.2)
* JPL	22/06/2016	: Appel procedure degroupee  1.3
------------------------------------------------------ */

#ifndef xzao306
#define xzao306

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO306_RPC_NAME "XZAO306"

#define XZAOC_XZAO306_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	
	XDY_Eqt		NumeroStation;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Octet	NumeroAutoroute;

} XZAOT_Station;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO306_Liste_RADT (	XDY_Basedd,
				XDY_Nom,
				XDY_Mot,
				XDY_FonctionInt,
				XDY_Entier *);

extern tg_Connexion  *XZAGV_Connexion;
extern tg_Context    *XZAGV_Context;

#endif
