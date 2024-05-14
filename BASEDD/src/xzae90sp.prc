/*E*/
/* Fichier : $Id: xzae90sp.prc,v 1.4 2012/03/07 14:01:28 gesconf Exp $        Release : $Revision: 1.4 $        Date : $Date: 2012/03/07 14:01:28 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae90sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Lecture des commentaires d'une FMC
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	28/09/95	: Creation
* C.T.	04/10/95	: recherche au pc sur EXP (1.2)
* PN	29/06/95	: separation EXP/HIS 1.3
* JPL	02/03/12	: Retour toutes infos (site, dates) et tri par horodate (pour DEM 1024)  1.4
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Liste des commentaires d'une fmc
*
* Sequence d'appel
* SP    XZAE90_Liste_Commentaires
*
* Arguments en entree
* XDY_Entier	va_Evt_in
* XDY_Octet	va_CleEvt_in
*
* Arguments en sortie
*
* Liste retournee :
*
* Code retour
* XDC_OK
* <0 erreur sybase
*
* Conditions d'utilisation
*
* Fonction
---------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE90' and type = 'P')
	drop procedure XZAE90
go


create procedure XZAE90
	@va_Evt_in	int = null,
	@va_CleEvt_in	tinyint = null
as
	if @va_Evt_in is null or @va_CleEvt_in is null
		return XDC_NOK

	/*A Retourner les commentaires de la fiche main courante */
	select 
		numero,
		sit,
		horodate_validation,
		horodate_fin,
		texte
#ifdef HIST
	from HIS..FMC_COM
#else
	from EXP..FMC_COM
#endif
	where	evenement = @va_Evt_in and
		cle = @va_CleEvt_in
	order by horodate_validation, numero

	return XDC_OK
go
