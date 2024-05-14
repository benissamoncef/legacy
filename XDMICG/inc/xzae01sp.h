/*E*/
/*  Fichier : $Id: xzae01sp.h,v 1.2 2013/03/05 13:47:47 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2013/03/05 13:47:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	30/09/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne la liste des evenements correspondant aux criteres
* 
* Sequence d'appel
* SP	XZAE01_Liste_Evts
* 
* Arguments en entree
* XDY_Autoroute	va_Autoroute_in
* XDY_PR	va_PR1_in
* XDY_PR	va_PR2_in
* XDY_Sens	va_Sens_in
* XDY_District	va_District_in
* XDY_Horodate	va_HorodateDebut_in
* XDY_Horodate	va_HorodateFin_in
* XDY_Octet	va_Evt_Operation_in
* XDY_Octet	va_Type_in
* 
* Arguments en sortie
* XDC_Octet	va_Cle_out		-- arguments retournes 
* XDC_Entier	va_Numero_out		-- dans une liste
* XDC_Octet	va_Type_out
* XDC_Autoroute	va_Autoroute_out
* XDC_PR	va_PR_out
* XDC_Sens	va_Sens_out
* XDC_Horodate	va_Horodate_out
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* l'argument evt_operation est obligatoire.
* lorsqu'une localisation est precisee, seuls les criteres suivants
* au niveau de la localisation sont admis : 
* - district precise, autoroute, PR et sens a null
* - autoroute, PR precises, district a null (le sens est facultatif)
* 
* Fonction
* Recherche des evenements verifiant :
* 	localisation sur Autoroute, entre PR1 et PR2, avec le sens Sens
* ET/OU
* 	localisation dans District
* ET/OU
* 	HorodateDebut	<	heure evenement	<	HorodateFin
* ET/OU
* 	Evenement ou Operation ?
* ET/OU
* 	Type = type de l'evenement
----------------------------------------------------------*/

 
#define XZAE01_Liste_Evts	"XZAE01" 
