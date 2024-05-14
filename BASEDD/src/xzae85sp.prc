/*E*/
/*  Fichier : $Id: xzae85sp.prc,v 1.2 2009/04/16 15:30:46 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2009/04/16 15:30:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae85sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courantes
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	06/04/95 : Creation	(V 1.1)
* JPL	16/04/09 : Test des Pr aux bornes de portions selon table portions (DEM 878) 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Controler l'existance du PR sur une autoroute
* 
* Sequence d'appel
* SP	XZAE85_Controler_Localisation
* 
* Arguments en entree
* XDY_Autoroute	va_Autoroute_in		: numero d'autoroute
* XDY_PR	va_PR_in		: pr 
* 
* Arguments en sortie
* XDY_Site	va_Site_in		: Site local
* 
* Code retour
* XDC_OK
* XDC_NOK		: probleme d'acces aux mesures
* XDC_LOC_INC		: localisation incorrecte
* XDC_ARG_INV		: parametres d'entree null
* 
* Conditions d'utilisation
*
* Fonction
-------------------------------------------------------*/

use PRC
go

create procedure XZAE;85
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
	@va_Site_out		T_SITE		= null output
as
	/*A controle parametres en entree */
	if @va_Autoroute_in = null or @va_PR_in = null
		return XDC_ARG_INV

	/*A recherche la localisation */
	select @va_Site_out = district
	from CFG..RES_POR
	where autoroute = @va_Autoroute_in and
		PR_debut <= @va_PR_in and @va_PR_in < PR_fin

	if @@rowcount = 0
		return XDC_LOC_INC

	return XDC_OK
go
