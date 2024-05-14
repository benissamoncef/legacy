/*E*/
/*  Fichier : $Id: xzac82sp.h,v 1.1 2005/09/13 18:08:30 gesconf Exp $     Release : $Revision: 1.1 $       Date : $Date: 2005/09/13 18:08:30 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac82sp.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	09/06/05	: Creation

-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Action d'affichage PMVA.
* 
* Sequence d'appel
* SP	XZAC82_Commande_PMVA
* 
* Arguments en entree

* 
* Arguments en sortie
* XDY_Entier	va_NumeroAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si l'actio a ete proposee par le plan d'actions, positionner 
* Priorite et DistanceEvt, sinon, les mettre a 0
* 
* Fonction
* Verifier l'existence de l'evenement
* Verifier l'existence d'un equipement non indisponible 
* dans le district ou l'actio est executee.
* 
-----------------------------------------------------*/

 
#define XZAC82_Commande_PMVA	"XZAC82" 
