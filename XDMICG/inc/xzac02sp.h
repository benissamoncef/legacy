/*E*/
/*  Fichier : $Id: xzac02sp.h,v 1.1 1994/11/03 14:52:15 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac02sp.prc
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
* SP	XZAC02_Fin_Commande_Manuelle_NAV
* Arguments en entree
* XDY_Entier	NumAction
* XDY_Horodate	HeureSucces
* XDY_Horodate	HeureEchec
* char(4)		Scenario
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
* Doit etre appelee apres XZAC03 ou XZAC51.
* Si l'operateur a choisi d'aboutir a un autre scenario que celui 
* initialement prevu, il peut l'indiquer dans Scenario.
* 
* Fonction
* Verifier l'existence de l'actio
* Update de la table ACTION, colonne heure_succes ou heure_echec 
* ou heure_fin, et eventuellement scenario
* S'il s'agit de heure succes ou heure echec, et si heure fin de 
* l'actio precedente sur le meme equipement n'est pas positionnee, le faire.
-----------------------------------------------------------------------*/

 
#define XZAC02_Fin_Commande_Manuelle_NAV	"XZAC;02" 
