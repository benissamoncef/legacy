/*E*/
/*Fichier : $Id: xzao501.h,v 1.1 2007/08/21 11:33:45 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2007/08/21 11:33:45 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao501.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* PNI	21/08/07	: Creation

------------------------------------------------------ */

#ifndef xzao501
#define xzao501

/* fichiers inclus */

#include "xzao126.h"

/* definitions de constantes exportees */

#define XZAOC_XZAO501_RPC_NAME "XZAO501"

#define XZAOC_XZAO501_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO501_Liste_Portions_District (	XDY_Basedd,
						XDY_Nom,
						XDY_FonctionInt,
						XDY_Entier * );

#endif
