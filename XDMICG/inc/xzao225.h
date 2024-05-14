/*E*/
/*Fichier :  $Id: xzao225.h,v 1.2 2008/02/13 13:24:29 devgtie Exp $      Release : $Revision: 1.2 $        Date : $Date: 2008/02/13 13:24:29 $
------------------------------------------------------
* ESCOTA * PROJET PASTRE
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao225.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JPL	28/06/07: Creation  1.1
------------------------------------------------------ */

#ifndef xzao225
#define xzao225

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO225_RPC_NAME "XZAO225"

#define XZAOC_XZAO225_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {

	XDY_Autoroute	NumeroAutoroute;
	XDY_PR		PR_Debut;
	XDY_Sens	Sens;
} XZAOT_Canton;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO225_Liste_Cantons_Par_Portion (	XDY_Basedd,
						XDY_Nom,
						XDY_Mot,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
