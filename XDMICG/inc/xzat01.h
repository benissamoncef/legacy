/*E*/
/*  Fichier : $Id: xzat01.h,v 1.1 2013/02/22 14:43:09 devgfi Exp $        Release : $Revision: 1.1 $        Date : $Date: 2013/02/22 14:43:09 $
------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR / PASTRE
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat01.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement
*
------------------------------------------------------
* HISTORIQUE :
*
* JPL	22/02/13 : Creation 1.1
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAT01_Liste_Eqt_Dispo
*
* Arguments en entree
*
* XDY_TypeEqt	va_Type_in :		par defaut tous les types
* XDY_District	va_District_in :	par defaut tous les districts
*
* XDY_Etat_Eqt	va_AjoutDispo_in :	ou entre les valeurs :
* 				XDC_EQT_MINEUR
* 				XDC_EQT_MAJEUR
* 				XDC_EQT_CRITIQUE
* 				XDC_EQT_HS
* 				XDC_EQT_INHIBE
* 				XDC_EQT_DESACTIVE
* 				XDC_EQT_A_DESACTIVER
* 				XDC_EQT_A_REACTIVER
*
* XDY_Etat_Eqt	va_AjoutNonDispo_in :	ou entre les valeurs :
* 				XDC_EQT_NON_MINEUR
* 				XDC_EQT_NON_MAJEUR
* 				XDC_EQT_NON_CRITIQUE
* 				XDC_EQT_NON_HS
* 				XDC_EQT_NON_INHIBE
* 				XDC_EQT_NON_DESACTIVE
* 				XDC_EQT_A_DESACTIVER
* 				XDC_EQT_A_REACTIVER
* XDY_Etat_Eqt	va_RetraitDispo_in :	ou entre les valeurs precedentes
* XDY_Etat_Eqt	va_RetraitNonDispo_in :	ou entre les valeurs
*
* Arguments en sortie
* XZATT_Liste		ListeEqts
*
* Service rendu
* Retourne la liste des equipements du district et du type precise,
* ayant la disponibilite precisee (plusieurs valeurs possibles).
*
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* code de ATRP18 si pb a son appel
*
* Conditions d'utilisation
* Preciser au moins un critere Type ou Dispo pour ne pas avoir une
* taille trop importante de requete.
* AjoutDispo et AjoutNonDispo permettent de selectionner les
* equipements ayant la disponibilite AjoutDispo ou AjoutNonDispo
* et RetraitDispo et RetraitNonDispo enlevent de la liste formee
* les equipements ayant la disponibilite RetraitDispo ou RetraitNonDispo
*
* Fonction
* select from EQUIPEMENT where criteres
------------------------------------------------------------*/


#ifndef xzat01_H
#define xzat01_H

/* fichiers inclus */


/* definitions de constantes exportees */

#define	XZATC_XZAT01_RPC_NAME		"XZAT;01"

#define	XZATC_XZAT01_NB_PARM_RETURN	0


/* definitions de types exportes */
   	
typedef struct {

	XDY_TypeEqt		TypeEqt;
	XDY_Eqt			NumeroEqt;
	XDY_NomEqt		NomEqt;
	XDY_Autoroute		NumAuto;
	XDY_PR			PR;
	XDY_Sens		Sens;
	XDY_Etat_Eqt		EtatEqt;
	XDY_Horodate		DateEtat;
	XDY_District		SiteGestion;   	
} XZATT_EtatEqt;


/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAT01_Liste_Eqt_Dispo (	XDY_TypeEqt	va_Type_in,
					XDY_District	va_District_in,
					XDY_Etat_Eqt	va_AjoutDispo_in,
					XDY_Etat_Eqt	va_AjoutNonDispo_in,
					XDY_Etat_Eqt	va_RetraitDispo_in,
					XDY_Etat_Eqt	va_RetraitNonDispo_in,
					XDY_Horodate	va_Horodate_in,
					XDY_FonctionInt	va_FonctionUtilisateur_in );

#endif
