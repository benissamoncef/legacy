/*E*/
/*Fichier : $Id: xzan01.h,v 1.3 1994/12/19 16:18:20 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1994/12/19 16:18:20 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan01.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* volcic	29 Nov 1994	: Modif nb param resultat (v1.2)
------------------------------------------------------ */

#ifndef xzan01
#define xzan01

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZANC_XZAN01_RPC_NAME "XZAN;01"

#define XZANC_XZAN01_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAN01_Creer_Type_Astreinte (	XDY_Basedd,
				       		XDY_Nom,
				       		XDY_Octet,
				       		XDY_Entier * ); 
				        


#endif
