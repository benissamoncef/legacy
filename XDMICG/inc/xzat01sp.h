/*E*/
/*  Fichier : $Id: xzat01sp.h,v 1.1 1994/11/03 14:54:01 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:54:01 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzat01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des etats d'un equipement 
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	23/09/94	: Creation
* C.T.	29/09/94	: Modif interface de fonction
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* S≈quence d'appel
* SP	XZAT01_Liste_Eqt_Dispo
* 
* Arguments en entr≈e
* XDY_TypeEqt	va_Type_in :		par d≈faut tous les types
* XDY_District	va_District_in :	par d≈faut tous les districts
* 
* XZATT_Dispo	va_AjoutDispo_in :	ou entre les valeurs :
* 				XDC_EQT_MINEUR
* 				XDC_EQT_MAJEUR
* 				XDC_EQT_CRITIQUE
* 				XDC_EQT_HS
* 				XDC_EQT_INHIBE
* 				XDC_EQT_DESACTIVE
* 				XDC_EQT_AREACTIVER
* 				XDC_EQT_ADESACTIVEr
* 
* XZATT_Dispo	va_AjoutNonDispo_in :	ou entre les valeurs :
* 				XDC_EQT_NON_MINEUR
* 				XDC_EQT_NON_MAJEUR
* 				XDC_EQT_NON_CRITIQUE
* 				XDC_EQT_NON_HS
* 				XDC_EQT_NON_INHIBE
* 				XDC_EQT_NON_DESACTIVE
* 				XDC_EQT_NON_AREACTIVER
* 				XDC_EQT_NON_ADESACTIVEr
* XZATT_Dispo	va_RetraitDispo_in :	ou entre les valeurs 
* XZATT_Dispo	va_RetraitNonDispo_in :	ou entre les valeurs 
* 
* Arguments en sortie
* XZATT_Liste		ListeEqts
* 
* Service rendu
* Retourne la liste des ≈quipements du district et du type pr≈cis≈, 
* ayant la disponibilit≈ pr≈cis≈e (plusieurs valeurs possibles).
* 
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* code de ATRP18 si pb a son appel
* 
* Conditions d'utilisation
* Pr≈ciser au moins un crit…re Type ou Dispo pour ne pas avoir une 
* taille trop importante de requete.
* AjoutDispo et AjoutNonDispo permettent de selectionner les
* equipements ayant la disponibilite AjoutDispo ou AjoutNonDispo
* et RetraitDispo et RetraitNonDispo enlevent de la liste formee
* les equipements ayant la disponibilite RetraitDispo ou RetraitNonDispo
* 
* Fonction
* select from EQUIPEMENT where crit…res
------------------------------------------------------------*/

 
#define XZAT01_Liste_Eqt_Dispo	"XZAT;01" 
