/*E*/
/*Fichier : $Id: xzao791.h,v 1.1 2017/02/10 08:57:24 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2017/02/10 08:57:24 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao791.h
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

#ifndef xzao791
#define xzao791

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO791_RPC_NAME "XZAO791"

#define XZAOC_XZAO791_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO791_Rech_Echangeur (	XDY_Eqt,
					XDY_Octet *);

#endif
