/*E*/
/*  Fichier : $Id: xzae71sp.h,v 1.1 1994/11/03 14:53:57 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae71sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	05/10/94	: Creation		V 1.1
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE71_Historique_Exploitation
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Horodate		va_HoroDebut_in
* XDY_Horodate		va_HoroFin_in
* XDY_Entier		va_Districts_in
* XDY_Entier		va_TypesAction_in
* XDY_Entier		va_TypesEvt1_in
* XDY_Entier		va_TypesEvt2_in
* 
* Arguments en sortie
* Liste des actions, Evenements ou operations trouves :
* XDY_Entier		va_Numero_in
* XDY_Octet		va_Cle_in
* XDY_Mot		va_Type_in
* XDY_Entier		va_Cause_in
* XDY_Octet		va_CleCause_in
* XDY_Horodate		va_HoroDebut_in
* XDY_Horodate		va_HoroFin_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Booleen		va_Categorie_in (XDC_HIS_EXP_EVT ou XDC_HIS_EXP_ACT)
* 
* Service rendu
* Retourne l'historique de l'exploitation
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* HorodateFini est obligatoire.
* Tous les autres arguments peuvent etre ou non values.
* Tous les resultats sont retournes dans une liste 
* 
* Fonction
---------------------------------------------------------------*/

 
#define XZAE71_Historique_Exploitation	"XZAE;71" 
