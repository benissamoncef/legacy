/*E*/
/*  Fichier : $Id: vuem09sp.prc,v 1.10 2011/03/16 19:43:52 gesconf Exp $        Release : $Revision: 1.10 $        Date : $Date: 2011/03/16 19:43:52 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem09sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	02/04/97	: Creation
* JMG	01/12/97	: pas de donnees extrapolees 1.3
* C.P.	15/06/1998	: Modif cd #tempo liée à SYBASE 11 V1.5
* JMG	10/09/99	: traitement depassement arithmetique ds certains cas et correction propre 
* PCL	05/12/08	: les vitesses =0 ne doivent plus rentrer les moyennes donc les remplacer par null REPERE1 v1.8
* JPL	15/03/11	: Retour des mesures dans l'ordre des dates (DEM 977)  1.9
* JPL	16/03/11	: Interface IHM : utilisation d'autres constantes  1.10
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM09_DonneesRADT
* 
* Arguments en entree
* datetime	date de debut de recherche de la forme jj/mm/AAAA hh:mm
* datetime	date de fin de recherche de la forme jj/mm/AAAA hh:mm
* smallint	station
* tinyint	sens
* tinyint	voie
* tinyint	type de mesure
* tinyint	type de periode
* 
* Arguments en sortie
*
* liste retournee
* CHAR(8)	jour de la semaine
* datetime	horodate du jour sous la forme jj/mm/AAAA
* datetime	horo de debut de periode de la forme hh:mm
* datetime	horo de fin de periode de la forme hh:mm
* int		moyenne des mesures sur la periode demandee
* 
* Service rendu
* retourne les moyennes (jounalieres, horaires, 30mn, 12mn, 6mn)
* des mesures sur la periode demandee
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

if exists (select * from sysobjects where name = 'VUEM09' and type = 'P')
	drop procedure VUEM09
go


create procedure VUEM09
	@va_debut	datetime = null,
	@va_fin		datetime = null,
	@va_periode	smallint = null,
	@va_type_mesure	tinyint = null,
	@va_station	smallint = null,
	@va_sens	tinyint = null,
	@va_voie	tinyint = null
as

declare @vl_date datetime,
	@vl_debut datetime,
	@vl_ecart int,
	@vl_moyenne int,
	@vl_fin	datetime,
	@vl_date_fin datetime

/* Controle des aprametres d'entree */
if @va_debut=null or @va_fin is null or 
	@va_sens is null or @va_voie is null or @va_station is null
	return XDC_ARG_INV

if @va_periode not in (1440, 60, 30, 12, 6)
	return XDC_ARG_INV


/* on recupere les mesure dans une table tempo avant de les traiter (performance) */
create table #tempo
	(donnee int null,
	horodate datetime)

/* on forme une table resultat */
create table #resultat
	(debut datetime,
	fin datetime,
	mesure int null)


select @vl_date = @va_debut,
	@vl_ecart = @va_periode

/* On remplit le tableau resultat */
while (@vl_date<@va_fin)
begin
	select @vl_date_fin = dateadd(minute, @vl_ecart, @vl_date)
	
	insert #resultat
	values(@vl_date, @vl_date_fin, null)
	
	select @vl_date = @vl_date_fin
	
end

	/* Cas ou on recupere le debit total */
if (@va_type_mesure=XDC_RADT_Q)
begin	

	insert #tempo (donnee, horodate)
	select	Q,
		horodate
	from	HIS..MES_TRA
	where	station=@va_station and
		sens=@va_sens and
		voie=@va_voie and
		horodate>=@va_debut and
		horodate<@va_fin and
		validite_Q!=XDC_RADT_RECONST

end


	/* Cas ou on recupere le debit PL */
if (@va_type_mesure=XDC_RADT_Q_PL)
begin	

	/*ATTENTION : l'ordre Q/100*PL est obligatoire*/
	/* pour ne pas avoir un depassement arithmetique*/

	insert #tempo (donnee, horodate) 
	select	convert(smallint,((Q/10)*PL)/10),
		horodate
	from	HIS..MES_TRA
	where	station=@va_station and
		sens=@va_sens and
		voie=@va_voie and
		horodate>=@va_debut and
		horodate<@va_fin and
		validite_PL!=XDC_RADT_RECONST

end

	/* Cas ou on recupere le debit VL */
if (@va_type_mesure=XDC_RADT_Q_VL)
begin	

	/*ATTENTION : idem precedent*/

	insert #tempo (donnee, horodate) 
	select	convert(smallint,((Q/10) * (100-PL))/10),
		horodate
	from	HIS..MES_TRA
	where	station=@va_station and
		sens=@va_sens and
		voie=@va_voie and
		horodate>=@va_debut and
		horodate<@va_fin and
		validite_PL!=XDC_RADT_RECONST

end


	/* Cas ou on recupere la vitesse */
if (@va_type_mesure=XDC_RADT_V)
begin	

	insert #tempo (donnee, horodate)
	select	V,
		horodate
	from	HIS..MES_TRA
	where	station=@va_station and
		sens=@va_sens and
		voie=@va_voie and
		horodate>=@va_debut and
		horodate<@va_fin and
		validite_V!=XDC_RADT_RECONST

	/* REPERE1 */
	update #tempo set donnee=NULL where donnee=0
	/* fin de REPERE1 */
end


	/* Cas ou on recupere le taux d'occupation */
if (@va_type_mesure=XDC_RADT_TO)
begin	

	insert #tempo (donnee, horodate)
	select	T_O,
		horodate
	from	HIS..MES_TRA
	where	station=@va_station and
		sens=@va_sens and
		voie=@va_voie and
		horodate>=@va_debut and
		horodate<@va_fin and
		validite_TO!=XDC_RADT_RECONST

end


/* On met les mesures trouvees dans la table resultat2 */
select * into #resultat2 from #resultat

declare pointeur_periode cursor for select #resultat.debut, #resultat.fin from #resultat

open pointeur_periode

fetch pointeur_periode into	@vl_debut,
				@vl_fin 

while (@@sqlstatus = 0)
begin
	/* calcul de la moyenne pour chacune des periodes */
	select @vl_moyenne = avg(donnee)
	from #tempo
	where
	horodate>=@vl_debut and
	horodate<@vl_fin

	/* on met la moyenne ds #resultat2 */
	update #resultat2
	set mesure = @vl_moyenne
	where debut=@vl_debut
	
	
	fetch pointeur_periode into	@vl_debut,
					@vl_fin 

end

close pointeur_periode

select 
	jour=datename(weekday,debut),
	horo=convert(char(10),debut,103),
	horo_debut=convert(char(5),debut,8),
	horo_fin=convert(char(5),fin,8),
	mesure
from #resultat2
order by debut

go

