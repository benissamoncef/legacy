/*E*/
/*  Fichier : $Id: xzao444.h,v 1.1 2017/01/10 11:22:53 devgfi Exp $        Release : $Revision: 1.1 $        Date : $Date: 2017/01/10 11:22:53 $
--------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR / PASTRE
--------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
--------------------------------------------------------------------------------
* FICHIER xzao444.prc
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Recherche des equipements BRA dont le panneau d'alerte est a piloter.
*
--------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	15/12/16 : Creation  1.1
------------------------------------------------------------------------------*/

/*X*/
/*------------------------------------------------------------------------------
* Sequence d'appel
*
* SP	 XZAO444_BRA_En_Perturbation
*
* Arguments en entree
*
* XDY_Autoroute		va_Autoroute_in
* XDY_Sens		va_Sens_in
* XDY_PR		va_PR_in
* XDY_District		va_District_in
* XDY_Horodate		va_Horodate_in
* XDY_PR		va_PR_Tete_in
* XDY_Entier		va_Numero_Fmc_in	Optionnel
* XDY_Octet		va_Cle_Fmc_in		Optionnel
* XDY_FonctionInt	pa_FonctionUtilisateur
*
* Arguments en sortie
* XZATT_Liste		ListeEqts
*
* Service rendu
* Retourne la liste des BRA situes sur une autoroute et dans un sens donnes,
* autour d'un point donne et dont le panneau d'alerte est a activer
* ou a eteindre en cas de fin d'evenement ou sortie de zone d'activation.
*
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
------------------------------------------------------------------------------*/


#ifndef xzao444_H
#define xzao444_H

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"

#include "xdy.h"
#include "asql_com.h"


/* definitions de constantes exportees */

#define	XZAOC_XZAO444_RPC_NAME		"XZAO444"

#define	XZAOC_XZAO444_NB_PARM_RETURN	0


/* definitions de types exportes */


/* declarations de donnees globales */

extern tg_Connexion  *XZAGV_Connexion;
extern tg_Context    * XZAGV_Context;

/* delaration de fonctions externes */

int XZAO444_BRA_En_Perturbation (	XDY_Autoroute	va_Autoroute_in,
					XDY_Sens	va_Sens_in,
					XDY_PR		va_PR_in,
					XDY_District	va_District_in,
					XDY_Horodate	va_Horodate_in,
					XDY_PR		va_PR_Tete_in,
					XDY_Entier	va_Numero_Fmc_in,
					XDY_Octet	va_Cle_Fmc_in,
					XDY_FonctionInt	pa_FonctionUtilisateur );

#endif
