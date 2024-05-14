/*E*/
/*  Fichier : $Id: xzae61sp.h,v 1.3 2008/02/13 13:24:24 devgtie Exp $      Release : $Revision: 1.3 $        Date : $Date: 2008/02/13 13:24:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae61sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	04/10/94	: Creation	(V 1.1)
* JPL	11/06/07	: Renommage (sans ";") 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche de travaux relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE61_Ecrire_Fiche_Travaux
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Booleen		va_Nature_in
* XDY_Octet		va_Type_in
* XDY_Booleen		va_Continu_in
* XDY_Horodate		va_HoroDebC_in
* XDY_Horodate		va_HoroFinC_in
* XDY_Booleen		va_Mobile_in
* XDY_Autoroute		va_Autoroute_out
* XDY_PR		va_PRqueueM_in
* XDY_PR		va_PRteteM_in
* XDY_Sens		va_Sens_in
* XDY_PR		va_PRtete_in
* XDY_Booleen		va_VoieRet_in
* XDY_Horodate		va_Horodate_in
* XDY_Eqt		va_PosteOperateur_in
* XDY_Octet		va_Previson_Traiter_Clos_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, insert ou update impossible,
*                 poste enrichisseur different
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* la date passee en parametre d'entree doit etre ensuite passee
* a la procedure de validation de la fiche main courante (XZAE11)
* 
* Fonction
* Modifier dans la table TRAVAUX
* Selon le cas correspondant a la valeur de Prevision_Traiter_Clos :
* 	Prevision :	Inserer dans la table TRAVAUX_HISTORIQUE
* 	Traiter :	Inserer ou modifier dans la table TRAVAUX_HISTORIQUE
* 	Clos :		Modifier dans la table TRAVAUX_HISTORIQUE
-------------------------------------------------------*/

 
#define XZAE61_Ecrire_Fiche_Travaux	"XZAE61" 
