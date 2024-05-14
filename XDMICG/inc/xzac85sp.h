/*E*/
/*  Fichier : $Id: xzac85sp.h,v 1.1 2005/09/13 18:09:37 gesconf Exp $     Release : $Revision: 1.1 $       Date : $Date: 2005/09/13 18:09:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac85sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	09/06/05	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture de l'actio d'affichage PMVA
* 
* Sequence d'appel
* SP	XZAC85_Lire_Commande_PMVA
* 
* Arguments en entree
* XDY_Entier	va_NumAction_in
* XDY_Octet	va_Site_in
* 
* Arguments en sortie

* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* Select dans les tables ACTION et E_PMV
-----------------------------------------------------------------------*/

 
#define XZAC85_Lire_Commande_PMVA	"XZAC85" 
