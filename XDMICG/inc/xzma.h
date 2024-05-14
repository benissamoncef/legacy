/*E*/
/*Fichier : $Id: xzma.h,v 1.1 1995/12/28 16:03:38 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/12/28 16:03:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  MAINTE
------------------------------------------------------
* MODULE XZMA * FICHIER xzma.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Le module xzma sert � mettre � jour les compteurs de
* marche des �quipements pour alimenter la base de donn�es
* de la GMAO.
*
* Les compteurs de marche peuvent etre de deux types :
*       - en nombre d'utilisation (ex : d�ploiement BRA)
*       - en dur�e d'utilisation (ex : affichage PMV)
*
* Les primitives d'interface XZMA01 et XZMA02 permettent
* au taches d'EQUEXT de mettre � jour les valeurs de ces
* compteurs dans des fichiers duree_xxxx ou nombre_xxxx.
*
* Un callback est automatiquement arm� au premier appel 
* d'une primitive du module, et sert � relever les
* compteurs dans un fichier transfert_xxxx.
*
* Ce fichier sera rapatri� sur POCMA (poste de maintenance)
* par la tache tmcpt.x qui appelle le script xzma_transfert.ksh.
*
------------------------------------------------------
* HISTORIQUE :
*
* B.GABORIT	26 dec 1995 :	Cr�ation	(1.1)
------------------------------------------------------*/

#ifndef XZMA
#define XZMA

/* fichiers inclus */

#include "xdy.h"
#include "xdm.h"
#include "xdg.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsm.h"

/* definitions de constantes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : Met � jour le compteur de marche(dur�e) d'un �quipement
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	XZMA01_Maj_duree_marche(
	XDY_Eqt		va_numero_in,
	XDY_TypeEqt	va_type_in,
	XDY_Entier	va_compter_in,
	XDY_NomTache	va_tache_in,
	XDY_NomMachine	va_machine_in
          );

/*
* ARGUMENTS EN ENTREE :
*	XDY_Eqt		va_numero_in
*	XDY_TypeEqt	va_type_in
*	XDY_Entier	va_compter_in (XDC_VRAI pour d�marrage et XDC_FAUX pour arret)
*	XDY_NomTache	va_tache_in
*	XDY_NomMachine	va_machine_in (nom de la machine trait�e, qui peut etre <> de la machine locale)
*
* ARGUMENTS EN SORTIE : aucun
*
*
* CODE RETOUR : 
*
* 	XDC_OK 		si ex�cution correcte de la primitive.
*	XDC_NOK 	si erreur d'ex�cution de la primitive
*
* CONDITION D'UTILISATION
*
* appel dans le cas d'un �quipement dont on compte la dur�e d'utilisation (pour le nombre, cf XZMA02).
* Le nom de machine est celui de la machine pour qui l'on traite (en cas de reprise par une autre machine)
*
* FONCTION 
*
* Mise � jour ou cr�ation du fichier duree_tache_machine_type_numero
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : Met � jour le compteur de marche (nb utilisations) d'un �quipement
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	XZMA02_Maj_nb_utilisations(
	XDY_Eqt		va_numero_in,
	XDY_TypeEqt	va_type_in,
	XDY_NomTache	va_tache_in,
	XDY_NomMachine	va_machine_in
          );

/*
* ARGUMENTS EN ENTREE :
*	XDY_Eqt		va_numero_in
*	XDY_TypeEqt	va_type_in
*	XDY_NomTache	va_tache_in
*	XDY_NomMachine	va_machine_in (nom de la machine trait�e, qui peut etre <> de la machine locale)
*
* ARGUMENTS EN SORTIE : aucun
*
*
* CODE RETOUR : 
*
* 	XDC_OK 		si ex�cution correcte de la primitive.
*	XDC_NOK 	si erreur d'ex�cution de la primitive
*
* CONDITION D'UTILISATION
*
* appel dans le cas d'un �quipement dont on compte le nombre d'utilisation (pour la dur�e, cf XZMA01).
* Le nom de machine est celui de la machine pour qui l'on traite (en cas de reprise par une autre machine)
*
* FONCTION 
*
* Mise � jour ou cr�ation du fichier nombre_tache_machine_type_numero
*
------------------------------------------------------*/
#endif

