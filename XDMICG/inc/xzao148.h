/*E*/
/*Fichier : $Id: xzao148.h,v 1.4 1994/11/22 21:44:26 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1994/11/22 21:44:26 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao148.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	11 Oct 1994	: Creation
* volcic	16 Oct 1994	: Modif XDY_FonctionInt (v1.2)
* volcic	22 Nov 1994	: Modif XDY_Nom (v1.3)
* volcic	22 Nov 1994	: Modif Entete (v1.4)
------------------------------------------------------ */

#ifndef xzao148
#define xzao148

/* fichiers inclus */

#include "xzao126.h"

/* definitions de constantes exportees */

#define XZAOC_XZAO148_RPC_NAME "XZAO;148"

#define XZAOC_XZAO148_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO148_Liste_Portions_District (	XDY_Basedd,
						XDY_Nom,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
