/* ======================================================== */
/* TRF_DY_DP_FMC                               							*/
/* ESCOTA : PC N3 N2                             						*/
/* Transfert du site DY dans DP des données FMC base HIS 		*/
/* Création : 04/2007  																			*/
/* ======================================================== */
use HIS
go

if exists (select 1
           from  sysobjects
           where id = object_id('TRF_DY_DP_FMC')
            		 and type = 'P')
   drop procedure TRF_DY_DP_FMC
go

create procedure TRF_DY_DP_FMC
as
begin
	declare @num_max int, 
					@indice_encours int,
					@num_FMC_DY int

	/* Récupération des FMC associées au site XDC_DY (Meyrargues = 23) */
	declare curFmcDy cursor for
		select numero 
		from FMC_GEN
		where cle = 23
		order by numero

	/* Identification du plus grand numéro de FMC utilisé pour le site DP */
	select @num_max = max(numero)
	from FMC_GEN
	where cle = 24
	
	select @indice_encours = @num_max + 1
	
	open curFmcDy
	fetch curFmcDy into @num_FMC_DY
	
	while (@@sqlstatus = 0)
	begin
		/* Mise à jour de la table FMC_GEN */
		update FMC_GEN
		set zz = zz + 1, cle = 24, sit = 24, numero = @indice_encours
		where numero = @num_FMC_DY and cle = 23
		
		/* Mise à jour des tables liées */
		update ACT_GEN
		set evenement = @indice_encours, cle = 24
		where evenement = @num_FMC_DY and cle = 23
		
		update ALT_EXP
		set evenement = @indice_encours, cle = 24
		where evenement = @num_FMC_DY and cle = 23
		
		update FMC_ACC
		set zz = zz + 1, cle = 24, sit = 24, numero = @indice_encours
		where numero = @num_FMC_DY and cle = 23

		/*
		update FMC_ACT
		set numero = @indice_encours, cle = 24
		where numero = @num_FMC_DY and cle = 23
		
		update FMC_ADA
		set evenement = @indice_encours, cle = 24
		where evenement = @num_FMC_DY and cle = 23
	*/	
		update FMC_BAS
		set zz = zz + 1, cle = 24, sit = 24, numero = @indice_encours
		where numero = @num_FMC_DY and cle = 23

		update FMC_BOU
		set cle = 24, sit = 24, numero = @indice_encours
		where numero = @num_FMC_DY and cle = 23

		update FMC_COM
		set evenement = @indice_encours, cle = 24, sit = 24
		where evenement = @num_FMC_DY and cle = 23
		
		update FMC_CNC
		set zz = zz + 1, cle = 24, sit = 24, numero = @indice_encours
		where numero = @num_FMC_DY and cle = 23

		update FMC_DAT
		set cle = 24, sit = 24, numero = @indice_encours
		where numero = @num_FMC_DY and cle = 23

		update FMC_DEL
		set zz = zz + 1, cle = 24, sit = 24, numero = @indice_encours
		where numero = @num_FMC_DY and cle = 23

		update FMC_ECH
		set zz = zz + 1, cle = 24, sit = 24, numero = @indice_encours
		where numero = @num_FMC_DY and cle = 23

		/*
		update FMC_TFM
		set numero = @indice_encours, cle = 24
		where numero = @num_FMC_DY and cle = 23
		
		update FMC_TRS
		set numero = @indice_encours, cle = 24
		where numero = @num_FMC_DY and cle = 23
	*/	
		update FMC_HIS
		set numero = @indice_encours, cle = 24
		where numero = @num_FMC_DY and cle = 23

		update FMC_LIS
		set numero = @indice_encours, cle = 24
		where numero = @num_FMC_DY and cle = 23
		
		update FMC_MET
		set cle = 24, sit = 24, numero = @indice_encours
		where numero = @num_FMC_DY and cle = 23

		update FMC_MNF
		set zz = zz + 1, cle = 24, sit = 24, numero = @indice_encours
		where numero = @num_FMC_DY and cle = 23

		update FMC_TRF
		set zz = zz + 1, cle = 24, sit = 24, numero = @indice_encours
		where numero = @num_FMC_DY and cle = 23

		update FMC_VEH
		set zz = zz + 1, cle = 24, sit = 24, numero = @indice_encours
		where numero = @num_FMC_DY and cle = 23

		update FMC_NAT 
		set cle = 24, sit = 24, numero = @indice_encours 
		where numero = @num_FMC_DY and cle = 23 

		update FMC_OPR
		set cle = 24, sit = 24, numero = @indice_encours
		where numero = @num_FMC_DY and cle = 23

		/*
		update FMC_SAT
		set evenement = @indice_encours, cle = 24
		where evenement = @num_FMC_DY and cle = 23
	*/

		update FMC_TRH
		set cle = 24, sit = 24, numero = @indice_encours
		where numero = @num_FMC_DY and cle = 23

		update FMC_VLN
		set cle = 24, sit = 24, numero = @indice_encours
		where numero = @num_FMC_DY and cle = 23

		select @indice_encours = @indice_encours + 1
		fetch curFmcDy into @num_FMC_DY
	end
	
	close curFmcDy
	deallocate cursor curFmcDy
	
end

GO
