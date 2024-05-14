/*E*/
/*Fichier : $Id: xzao22.h,v 1.1 1998/01/09 14:12:38 verdier Exp $      Release : $Revision: 1.1 $         Date : $Date: 1998/01/09 14:12:38 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao22.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	16/12/97	: creation (dem/1534)
------------------------------------------------------ */

#ifndef xzao22
#define xzao22

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO22_RPC_NAME "XZAO;22"

#define XZAOC_XZAO22_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO22_Distance (	XDY_Autoroute,
					XDY_PR,
					XDY_Autoroute,
					XDY_PR ,
					XDY_Entier *);

#endif
