/*E*/
/*  Fichier : @(#)vuem03sp.prc	1.3      Release : 1.3        Date : 06/28/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem03sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.	28/07/95	: Creation
* B.G.  02/10/95	: modif pour compter les heures de 06 a 00 et non de 00 a 54 (1.2)
* C.T.  14/06/96	: modif interfaces E/S , ajout nom 1er jour, nb jours, nb de mesures par heures (1.3)
* JMG	01/12/97	: pas de donnees extrapolees 1.5
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM03_DebitHoraire
* 
* Arguments en entree
* datetime	premier jour du mois a traiter
* smallint	station
* tinyint	sens
* tinyint	voie
* 
* Arguments en sortie
* char(8)	nom du premier jour du mois a traiter
* XDC_Octet	nombre de jours du mois
*
* liste retournee
* CHAR(8)	jour de la semaine
* int		jour du mois
* int		heure de la journee
* int		debit horaire
* int		nombre de mesures par heure
* 
* Service rendu
* retourne le debit horaire sur un mois
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
drop proc VUEM03
go
create procedure VUEM03
	@va_mois	datetime = null,
	@va_station	smallint = null,
	@va_sens	tinyint = null,
	@va_voie	tinyint = null,
	@va_PremierJour	char(8) output,
	@va_NbJours	tinyint output
as

declare @vl_fin	datetime,@jour int,@heure int,@debit int,
	@debit1 int,
	@debit2 int,
	@debit3 int,
	@debit_rep int,
	@valeur_rep tinyint

if @va_mois=null or @va_station is null or 
	@va_sens is null or @va_voie is null
	return XDC_ARG_INV

/* recherche le nom du premier jour */
select @va_PremierJour = datename(weekday, @va_mois)

/* on calcule la date limite */
select @vl_fin = dateadd(month,1, @va_mois)

/* recherche le nombre de jours */
select @va_NbJours = datediff(day, @va_mois, @vl_fin )

/* on recupere les mesure dans une table tempo avant de les traiter (performance) */

select	Q,HORODATE=dateadd(minute,-6,horodate)
into	#tempo
from	HIS..MES_TRA
where	station=@va_station and
	sens=@va_sens and
	voie=@va_voie and
	horodate>@va_mois and
	horodate<=@vl_fin and
	validite_Q != XDC_RADT_RECONST

select	distinct
	journee=datename(weekday,HORODATE),
	jour=datepart(day,HORODATE),
	heure=datepart(hour,HORODATE),
	debit=avg(Q),
	nb=count(Q)
into	#tempo2
from	#tempo
group by datepart(day,HORODATE)*24+datepart(hour,HORODATE)


select jour=jour,heure=heure,debit=debit 
into	#tempo3
from	#tempo2

	declare pointeur cursor
	for select 
		jour,
		heure,
		debit
	from #tempo3
	

	select @valeur_rep=0,@debit_rep=0


	open pointeur
	fetch pointeur into @jour ,@heure,@debit
	while @@sqlstatus = 0
	begin
		select 	@debit1=-1,@debit2=-1,@debit3=-1

		select 	@debit1=debit from #tempo2 where jour=@jour and heure=@heure+1

		select 	@debit2=debit from #tempo2 where jour=@jour and heure=@heure+2
		select 	@debit3=debit from #tempo2 where jour=@jour and heure=@heure+3
		
--select @debit_rep,@debit,@debit1,@debit2,@debit3,@valeur_rep
		if (@debit1=@debit) AND (@debit2=@debit) AND  (@debit3=@debit) 
		begin
			select @valeur_rep=1,@debit_rep=@debit
			delete #tempo2 where jour=@jour and heure=@heure+1
		end
--select @jour ,@heure,@debit_rep,@debit,@valeur_rep
		if @debit_rep=@debit and @valeur_rep=1
		begin
			delete #tempo2 where jour=@jour and heure=@heure
		end
		else
		begin
			select @valeur_rep=0,@debit_rep=0
		end
		
	fetch pointeur into @jour ,@heure,@debit

	end

	select * from #tempo2
go
grant EXECUTE on VUEM03 to public
go
