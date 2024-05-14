/* ======================================================== */
/* TRF_DY_DP_ACTION                               					*/
/* ESCOTA : PC N3 N2                             						*/
/* Transfert du site DY dans DP des données ACTION base HIS */
/* Création : 04/2007  																			*/
/* ======================================================== */
use HIS
go

if exists (select 1
           from  sysobjects
           where id = object_id('TRF_DY_DP_ACTION')
            		 and type = 'P')
   drop procedure TRF_DY_DP_ACTION
go

create procedure TRF_DY_DP_ACTION
as
begin
	declare @num_max int, 
					@indice_encours int,
					@num_ACTION_DY int

	/* Récupération des Actions associées au site XDC_DY (Meyrargues = 23) */
	declare curActionDy cursor for
		select numero 
		from ACT_GEN
		where sit = 23
		order by numero

	/* Identification du plus grand numéro d'ACTION utilisé pour le site DP */
	select @num_max = max(numero)
	from ACT_GEN
	where sit = 24
	
	select @indice_encours = @num_max + 1
	
	open curActionDy
	fetch curActionDy into @num_ACTION_DY
	
	while (@@sqlstatus = 0)
	begin
		/* Mise à jour de la table ACT_GEN */
		update ACT_GEN
		set sit = 24, sit_origine = 24, numero = @indice_encours
		where numero = @num_ACTION_DY and sit = 23
		
		/* Mise à jour des tables liées */
		update ACT_APL
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
		
		update ACT_BAD
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_BAF
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_DEP
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_ECH
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_EXP_ADA
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_FAX
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_FRG
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_GAR
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_INT
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_NAV
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_PMA
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_PMV
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_PMV_ADA
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_RMQ
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_SIS
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_TFM
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_TFM_EXP
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_TUB
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		update ACT_TXT_TFM
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
				
		/*
		update ADA_ACT
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY
		*/		
		/* N'EXISTE PAS ENCORE */
/*		
		update ACT_EXP
		set sit = 24, actio = @indice_encours
		where actio = @num_ACTION_DY and sit = 23
*/				
		select @indice_encours = @indice_encours + 1
		fetch curActionDy into @num_ACTION_DY
	end
	close curActionDy
	deallocate cursor curActionDy
	
end

GO
