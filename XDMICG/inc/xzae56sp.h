/*E*/
/*  Fichier : $Id: xzae56sp.h,v 1.1 1994/11/03 14:53:40 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae56sp.prc
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
* Mettre en base une fiche de meteo relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE56_Ecrire_Fiche_Meteo
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Horodate		va_Horodate_in
* XDY_PR		va_PRFin_in
* XDY_Octet		va_Perturbation_in
* XDY_Mot		va_Visibilite_in
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
* Selon le cas correspondant a la valeur de Prevision_Traiter_Clos :
* 	Prevision :	Inserer ou modifier dans la table METEO_HISTORIQUE
* 	Traiter :	Inserer dans la table METEO_HISTORIQUE
* 	Clos :		Modifier dans la table METEO_HISTORIQUE
---------------------------------------------------------*/

 
#define XZAE56_Ecrire_Fiche_Meteo	"XZAE;56" 
