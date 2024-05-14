/*E*/
/*  Fichier : $Id: xzae1720sp.prc,v 1.2 2016/09/26 17:48:14 gesconf Exp $      Release : $Revision: 1.2 $      Date : $Date: 2016/09/26 17:48:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae172sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module NOMADE : Retourne les 10 derniers tdp gdes sections
* Remarque :                                                                   
*
------------------------------------------------------
* HISTORIQUE :
*
* Cluchague     13/01/02  : Creation
* Cluchague     03/09/02  : le tdp calcule peut depasser 999 donc sur 5 digits 1.2
* pni		26/09/16	: ordonnancement 1.3 dem1187
-----------------------------------------------------*/
/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE172
* 
* Arguments en entree
* 
* Arguments en sortie
*
* aucun : 
* 
* Service rendu
*         retourne les temps de parcours grandes sections                             
* 
* Code retour
* XDC_OK
* XDC_NOK	:
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

drop procedure XZAE1720
go

create procedure XZAE1720
as

declare
@vl_horodate varchar(19),
@vl_nom_autoroute varchar(5),
@vl_sens tinyint,
@vl_PR int,
@vl_origine varchar(25),
@vl_destination varchar(25),
@vl_autoroute tinyint, 
@vl_indice tinyint, @vl_min int,
@vl_horostr varchar(30),
@vl_horo datetime, @vl_horo_out datetime,
@vl_nom varchar(50),@vl_index tinyint,
@vl_temps_out varchar(25), @vl_temps_libre_out varchar(25)

   create table #TDP_GDE_SECTION
   (
      zone       varchar(50)  null,
      horodate varchar(25) null,
      temps 	varchar(25) null,
      temps_libre 	varchar(25) null
   )

   create table #TDP_FINAL
   (	autoroute tinyint null,
	sens		tinyint null,
	PR		int null,
	zone       varchar(50)  null,
	temps_libre varchar(15) null,
	temps1 	   varchar(15) null,
	temps2	   varchar(15) null,
	temps3 	   varchar(15) null,
	temps4 	   varchar(15) null,
	temps5 	   varchar(15) null,
	temps6 	   varchar(15) null,
	temps7 	   varchar(15) null,
	temps8 	   varchar(15) null,
	temps9 	   varchar(15) null,
	temps10	   varchar(15) null
   )

select @vl_horodate = getdate()
select @vl_index=0

select @vl_min=datepart(minute,@vl_horodate)%6
if @vl_min<>0
		select @vl_horodate=dateadd(minute,-@vl_min,@vl_horodate)

	/*parcours des zones à exporter*/
declare cursor1 cursor for	select AUT.nom, TFM.sens, TFM.origine, TFM.destination , AUT.numero,D.PR
	from EXP..TFM_TDP TFM,
	     CFG..RES_AUT AUT, CFG..RES_DTP D
        where AUT.numero = TFM.autoroute and TFM.sens=D.sens and TFM.autoroute=D.autoroute and libelle_TFM=TFM.origine
	       order by AUT.numero,TFM.sens,D.PR

open cursor1

	/*pour chaque zone*/
fetch cursor1 into @vl_nom_autoroute, @vl_sens, @vl_origine, @vl_destination, @vl_autoroute,@vl_PR


while @@sqlstatus = 0
	begin
		/*traitement de H à H-60*/
		select @vl_horo = @vl_horodate
		select @vl_indice = 0
		while @vl_indice < 10
		begin
			select @vl_horo = dateadd(minute, -6*@vl_indice,@vl_horodate)
			/*appel de la proc XZAO4350 pour recuperer temps, temps_libre, horodate*/
			exec PRC..XZAO4350 @vl_sens,
					@vl_origine,
					@vl_destination,
					 @vl_autoroute,
					 @vl_horo,
					 @vl_horo_out output,
					 @vl_temps_out output,
					 @vl_temps_libre_out output

  			select @vl_horostr = convert (char(10),@vl_horo_out,103) + ' ' + convert(char(5),@vl_horo_out,8)
  			select @vl_horostr = convert(char(5),@vl_horo_out,8)
		
			select @vl_nom = rtrim(@vl_origine) + ' - ' + rtrim(@vl_destination)

			insert into #TDP_GDE_SECTION
			values (@vl_nom, @vl_horostr, @vl_temps_out, @vl_temps_libre_out)

			if @vl_indice = 0
			begin
				if @vl_index = 0
					insert into #TDP_FINAL (autoroute,sens,PR,zone, temps1, temps_libre)
					values (0,0,0,"Zone",@vl_horostr,"Temps libre")

				insert into #TDP_FINAL (autoroute,sens,PR,zone, temps1, temps_libre)
				values (@vl_autoroute,@vl_sens,@vl_PR,@vl_nom, @vl_temps_out, @vl_temps_libre_out)
			end
			else if @vl_indice = 1
			begin
				update #TDP_FINAL set temps2 = @vl_temps_out  where zone = @vl_nom
				if @vl_index = 0
				update #TDP_FINAL set temps2 = @vl_horostr where zone = "Zone"
			end
			else if @vl_indice = 2
			begin
				update #TDP_FINAL set temps3 = @vl_temps_out where zone = @vl_nom
				if @vl_index = 0
				update #TDP_FINAL set temps3 = @vl_horostr where zone = "Zone"
			end
			else if @vl_indice = 3
			begin
				update #TDP_FINAL set temps4 = @vl_temps_out where zone = @vl_nom
				if @vl_index = 0
				update #TDP_FINAL set temps4 = @vl_horostr where zone = "Zone"
			end
			else if @vl_indice = 4
			begin
				update #TDP_FINAL set temps5 = @vl_temps_out where zone = @vl_nom
				if @vl_index = 0
				update #TDP_FINAL set temps5 = @vl_horostr where zone = "Zone"
			end
			else if @vl_indice = 5
			begin
				update #TDP_FINAL set temps6 = @vl_temps_out where zone = @vl_nom
				if @vl_index = 0
				update #TDP_FINAL set temps6 = @vl_horostr where zone = "Zone"
			end
			else if @vl_indice = 6
			begin
				update #TDP_FINAL set temps7 = @vl_temps_out  where zone = @vl_nom
				if @vl_index = 0
				update #TDP_FINAL set temps7 = @vl_horostr where zone = "Zone"
			end
			else if @vl_indice = 7
			begin
				update #TDP_FINAL set temps8 = @vl_temps_out where zone = @vl_nom
				if @vl_index = 0
				update #TDP_FINAL set temps8 = @vl_horostr where zone = "Zone"
			end
			else if @vl_indice = 8
			begin
				update #TDP_FINAL set temps9 = @vl_temps_out  where zone = @vl_nom
				if @vl_index = 0
				update #TDP_FINAL set temps9 = @vl_horostr where zone = "Zone"
			end
			else if @vl_indice = 9
			begin
				update #TDP_FINAL set temps10 = @vl_temps_out where zone = @vl_nom
				if @vl_index = 0
				update #TDP_FINAL set temps10 = @vl_horostr where zone = "Zone"
				select @vl_index = @vl_index +1
			end
			select @vl_indice = @vl_indice +1
		end
	fetch cursor1 into @vl_nom_autoroute, @vl_sens, @vl_origine, @vl_destination, @vl_autoroute,@vl_PR

	end
close cursor1


/*valeurs a retourner*/
select  zone ,
	temps_libre,
	temps1 	   ,
	temps2	 ,
	temps3 	 ,
	temps4 	 ,
	temps5 	 ,
	temps6 	 ,
	temps7 	 ,
	temps8 	 ,
	temps9 	 ,
	temps10 from #TDP_FINAL
order by autoroute,sens,PR
drop table #TDP_GDE_SECTION 
drop table #TDP_FINAL 

return (XDC_OK)
go
