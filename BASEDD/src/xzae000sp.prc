
use PRC
go

create procedure XZAE000
as
	declare @vl_horo datetime, @vl_numero int, @vl_cle int


	create table #ACC (numero int, cle tinyint)

	insert into #ACC
	select numero,cle
	from HIS..FMC_ACC
	where  validation is null

	declare Pointeur_fmc_acc cursor
	for select *
	from #ACC

	open Pointeur_fmc_acc
	fetch Pointeur_fmc_acc into @vl_numero, @vl_cle 
	while @@sqlstatus = 0
	begin

		select @vl_horo=horodate_validation
		from HIS..FMC_HIS
		where numero=@vl_numero and cle=@vl_cle
		group by numero,cle
		having horodate_validation = max(horodate_validation)

		select @vl_horo

		update HIS..FMC_ACC set validation=@vl_horo,zz=zz+1
		where numero=@vl_numero and cle=@vl_cle 
	
		select validation from HIS..FMC_ACC where numero=@vl_numero and cle=@vl_cle

		fetch Pointeur_fmc_acc into @vl_numero, @vl_cle 
	end

	close Pointeur_fmc_acc
	
	drop table #ACC

	return XDC_OK
go
