/*E*/
/*  Fichier : $Id: xzag08sp.prc,v 1.2 1996/05/07 18:23:25 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/05/07 18:23:25 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag08sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	12/09/95	: Creation	(V 1.1)
* C.T.	03/05/96	: Aucune action à realiser (plus de donnees dans HIS)(V 1.2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Transfere les donnees de niveau du mois dans la base de sauvegarde
* /restauration
* 
* Sequence d'appel
* SP	XZAG08_Transfert_Niveau_Mois
* 
* Arguments en entree
* XDY_Mot	va_Mois_in
* XDY_Mot	va_Annee_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK	: 
* XDC_ARG_INV	: parametre d'entree null ou date demandee trop proche
* 
* Conditions d'utilisation
* - Il faut que la date courante soit au moins superieure a
* au mois specifie plus 7 jours sinon erreur de parametre
* - Le mois vaut de 1 a 12
* - L'annee vaut au moins 1900
* - La periode de sauvegarde est du debut de mois - 7 jours a la
* la fin du mois + 7 jours
* 
* Fonction
* Copier de la base historique dans la base sauvegarde/restauration
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;08
	@va_Mois_in	smallint = null,
	@va_Annee_in	smallint = null
as
#ifdef CI
	return XDC_OK
#else
	return XDC_NOK
#endif
go
