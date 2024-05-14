/*E*/
/*  Fichier : @(#)xzae111sp.h	1.1      Release : 1.1        Date : 11/29/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae144sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture  des evenements strada en base
*
------------------------------------------------------
* HISTORIQUE :
*
*  C. Penot	05 nov 1997	: Creation
---------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* -----------------------------------------------------
* Sequence d'appel
* SP	XZAE144_Synthese_Situation_Strad
* 
* Arguments en entree
* 
* Arguments en sortie
*
*
* 
* Code retour
* XDC_OK
* XDC_NOK	: fiche non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* 
* Fonction
* Select dans les tables EVENEMENT, EVENEMENT_HISTORIQUE, 
* ALERTE, COMMENTAIRES where numero evenement and horodate
----------------------------------------------------*/

 
#define XZAE144_Synthese_Situation_Strada	"XZAE144" 
