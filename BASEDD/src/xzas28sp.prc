/*E*/
/*  Fichier : $Id: xzas28sp.prc,v 1.1 1994/12/29 17:23:34 fournel Exp $    Release : $Revision: 1.1 $     Date : $Date: 1994/12/29 17:23:34 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas28sp.prc
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
* Etat de poste : Consultation.
*
* Sequence d'appel
* SP    XZAS28_Consultation_Appels_Usager
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
* Cette fonction permet au GIT de consulter les appel classes
* par type.
-------------------------------------------------------*/

use PRC
go

create procedure XZAS;28
as

	/* Fin de la procedure */
 	return XDC_OK
go
