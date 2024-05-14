/*E*/
/*  Fichier : 	$Id: xzan36.h,v 1.1 1994/12/19 16:18:24 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/19 16:18:24 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan36.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Oct 1994	: Creation
------------------------------------------------------ */

#ifndef xzan36
#define xzan36

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZANC_XZAN36_RPC_NAME "XZAN;36"

#define XZANC_XZAN36_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAN36_Existence_Astreinte (	XDY_Basedd,
					XDY_Octet,
					XDY_Booleen *);



#endif
