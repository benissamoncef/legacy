use PRC
go
drop proc PREVIA
go

create procedure PREVIA
	@va_mois	datetime = null,
	@va_station	smallint = null,
	@va_sens	tinyint = null,
	@va_voie	tinyint = null
as

declare @vl_fin	datetime

if @va_mois=null or @va_station is null or
	@va_sens is null or @va_voie is null
	return 2 
 
 

/* on calcule la date limite */
select @vl_fin = dateadd(year,1, @va_mois)


/* on recupere les mesure dans une table tempo avant de les traiter (performance) */

select	Q,HORODATE=dateadd(minute,-6,horodate)
into	#tempo
from	HIS..MES_TRA
where	station= 
 @va_station and
	sens=@va_sens and
	voie=@va_voie and
	horodate>@va_mois and
	horodate<=@vl_fin and
	validite_Q != 0 


select	distinct
	@va_station,
	@va_sens,
	journee=datename(weekday,HORODATE),
	jour=datepart(dayofyear,HORODATE),
	heure=datepart(hour, 
 HORODATE),
	debit=avg(Q),
	nb=count(Q)
from	#tempo
group by datepart(dayofyear,HORODATE)*24+datepart(hour,HORODATE)
go
grant execute on PREVIA to INVITE
go
