/*E*/
/*Fichier : $Id: xzao21.h,v 1.3 1995/01/17 14:38:09 torregrossa Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/01/17 14:38:09 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao21.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	17 Oct 1994	: Creation
* C.T.          13 Jan 1995     : Ajout de la date en entree (V1.2)
------------------------------------------------------ */

#ifndef xzao21
#define xzao21

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO21_RPC_NAME "XZAO;21"

#define XZAOC_XZAO21_NB_PARM_RETURN 2

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO21_Rech_Echangeur (	XDY_Autoroute,
					XDY_PR,
					XDY_Octet,
					XDY_Mot *,
					XDY_District *,
					XDY_Horodate);

#endif
