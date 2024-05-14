/*E*/
/*  Fichier : $Id: xzac07sp.h,v 1.1 1994/11/03 14:52:24 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac07sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/09/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Indique la fin de la commande individuelle par l'operateur
* 
* Sequence d'appel
* SP	XZAC07_Fin_Commande_Manuelle_Echangeur
*
* Arguments en entree
* XDY_Entier	va_NumAction_in
* XDY_Horodate	va_HeureSucces_in
* XDY_Booleen	va_SuccesEchec_in
* XDY_Octet	va_Sequence_in
*
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee, procedure lancee sur serveur non prevu
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Doit etre appelee apres XZAC07 ou XZAC53.
* Si l'operateur a choisi d'aboutir a une autre sequence 
* que celle initialement prevue, il peut l'indiquer dans Sequence.
* 
* Fonction
* Verifier l'existence de l'actio
* Update de la table ACTION, colonne heure_succes ou 
* heure_echec ou heure_fin, et eventuellement sequence.
* S'il s'agit de heure succes ou heure echec, et si heure 
* fin de l'actio precedente sur le meme equipement n'est 
* pas positionnee, le faire.
-----------------------------------------------------------------------*/

 
#define XZAC07_Fin_Commande_Manuelle_Echangeur	"XZAC;07" 
