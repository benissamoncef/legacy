/*E*/
/*Fichier : $Id: xzao133.h,v 1.2 1994/10/20 11:31:12 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/10/20 11:31:12 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao133.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	14 Oct 1994	: Modif XDY_FonctionInt (v1.2)
------------------------------------------------------ */

#ifndef xzao133
#define xzao133

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO133_RPC_NAME "XZAO;133"

#define XZAOC_XZAO133_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO133_Liste_Villes (	XDY_Basedd,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
