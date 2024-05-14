/*E*/
/*  Fichier : $Id: xzae67sp.h,v 1.2 2019/04/08 17:28:04 devgfi Exp $        $Revision: 1.2 $        $Date: 2019/04/08 17:28:04 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae67sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	03/10/94	: Creation	(V 1.1)
* JPL	04/04/19	: Procédure dégroupée  1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer un commentaire relatif a un evenement existant
* 
* Sequence d'appel
* SP	XZAE67_Ecrire_Fiche_Commentaire
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Entier		va_Numero_in
* XDY_Horodate		va_Horodate_in
* XDY_Horodate		va_HoroPerime_in
* XDY_Commentaire	va_Texte_in
* XDY_Eqt		va_PosteOperateur_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, insertion ou update impossible,
*                 poste enrichisseur different, va_Horodate_in null
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si le parametre va_Numero_in est null alors insertion sinon modification
* Le parametre va_Horodate_in doit etre non null dans le cas d'une
* insertion d'un nouveau commentaire
* 
* Fonction
* Verifier droit d'enrichir
* Inserer ou modifier dans la table COMMENTAIRES
----------------------------------------------------------*/

 
#define XZAE67_Ecrire_Fiche_Commentaire	"XZAE67" 
