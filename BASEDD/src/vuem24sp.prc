/*E*/
/*  Fichier : @(#)vuem24sp.prc	1.1      Release : 1.1        Date : 03/11/97
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem24sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	10/03/97	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM24_MesuresParClasse
* 
* Arguments en entree
* datetime	horodate de debut
* datetime	horodate de fin
* int		Type de mesure (Q, V, T_O, %PL)
* smallint	station
* tinyint	sens
* tinyint	voie
* 
* Arguments en sortie
* Aucun
*
* liste retournee
* datetime	horodate hh:mm de la mesure
* int		valeur de la mesure
* int		validite de la mesure
* 
* Service rendu
* retourne les mesures traffic d'une station entre les 2 horodates demandees
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

create procedure VUEM24
	@va_debut	datetime = null,
	@va_fin		datetime = null,
	@va_type	int = null,
	@va_station	smallint = null,
	@va_sens	tinyint = null,
	@va_voie	tinyint = null
as

declare @vl_fin	datetime

if @va_debut=null or @va_fin is null or @va_type is null or @va_station is null or 
	@va_sens is null or @va_voie is null
	return XDC_ARG_INV


/* on recupere les mesure dans une table tempo avant de les traiter (performance) */

select	Q, validite_Q, V, validite_V, T_O, validite_TO, PL, validite_PL, horodate
into	#tempo
from	HIS..MES_TRA
where	station=@va_station and
	sens=@va_sens and
	voie=@va_voie and
	horodate>=@va_debut and
	horodate<=@va_fin
order by horodate


if (@va_type = XDC_RADT_V)
begin

	select
		V,
		validite_V,
		convert(char(5),horodate,8)
	from	#tempo

end

if (@va_type = XDC_RADT_Q)
begin

	select
		Q,
		validite_Q,
		convert(char(5),horodate,8)
	from	#tempo

end

if (@va_type = XDC_RADT_TO)
begin

	select
		T_O,
		validite_TO,
		convert(char(5),horodate,8)
	from	#tempo

end


if (@va_type = XDC_RADT_PL)
begin

	select
		PL,
		validite_PL,
		convert(char(5),horodate,8)
	from	#tempo

end


return XDC_OK

go

