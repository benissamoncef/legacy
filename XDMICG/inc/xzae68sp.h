/*E*/
/*  Fichier : $Id: xzae68sp.h,v 1.1 1994/11/03 14:53:56 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:53:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae68sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	01/09/94	: Creation
* C.T.  06/10/94	: ajout colonne dans LISTE_CONSEQ
*                         pour multi-utilisation de 
*                         la proc	(V 1.4)
* C.T.  18/10/94	: enleve le retour de la liste dans 
*                         le cas d'un appel via une procedure
*                         stockee (pas reteste)	(V 1.5)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE68_ListeConsequences
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Horodate		va_Horodate_in 
* XDY_Octet		va_Effacement_in
* 
* Arguments en sortie
* XDY_Entier		va_NumEvt_out
* XDY_Octet		va_CleEvt_out
* XDY_Octet		va_Type_out
* 
* Service rendu
* Parcoure l'arbre des consequences d'un evenement (en profondeur)
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* Ne pas renseigner le parametre va_Effacement_in, il est utilise 
* en interne pour une autre procedure
* 
* Fonction
* appelle XZAE70 recursivement et construit la liste 
* ordonnee des consequences
------------------------------------------------------*/

 
#define XZAE68_ListeConsequences	"XZAE;68" 
