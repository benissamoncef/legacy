/*E*/
/*  Fichier : $Id: xzae55sp.h,v 1.1 1994/11/03 14:53:38 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae55sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	04/10/94	: Creation	(V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Mettre en base une fiche de vehicule lent relative a un 
* evenement existant
* 
* Sequence d'appel
* SP	XZAE55_Ecrire_Fiche_Veh_Lent
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Horodate		va_Horodate_in
* XDY_Mot		va_Longueur_in
* XDY_Octet		va_Vitesse_in
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
* Selon la valeur de Prevision_Traiter_Clos :
* 	Prevision :	Inserer ou modifier dans la table VEHICULES_LENTS_HISTORIQUE
* 	Traiter :	Inserer dans la table VEHICULES_LENTS_HISTORIQUE
* 	Clos :		Modifier dans la table VEHICULES_LENTS_HISTORIQUE
---------------------------------------------------------*/

 
#define XZAE55_Ecrire_Fiche_Veh_Lent	"XZAE;55" 
