/*E*/
/*  Fichier : $Id: xzas27sp.prc,v 1.1 1994/12/29 17:23:27 fournel Exp $     Release : $Revision: 1.1 $       Date : $Date: 1994/12/29 17:23:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas27sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Analyse 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.  19/12/94        : Creation      (V 1.1)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Etat de poste : Saisie.
*
* Sequence d'appel
* SP    XZAS27_Saisie_Appel_Usager
*
* Arguments en entree :
*
* Arguments en sortie :
*
* Liste retournee :
*
* Code retour
* XDC_OK
* XDC_NOK               : probleme d'acces aux donnees 
* XDC_ARG_INV           : parametres d'entree null, ou valeur non autorisee ou
*                         echangeur inexistante
*
* Conditions d'utilisation
*
* Fonction
* Cette fonction permet a l'operateur CI de saisir pour chaque appel usager
* le motif de l'appel (tarif peage, renseignement...)
-------------------------------------------------------*/

use PRC
go

create procedure XZAS;27
as

	/* Fin de la procedure */
 	return XDC_OK
go
