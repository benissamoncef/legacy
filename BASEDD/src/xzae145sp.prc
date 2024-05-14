/*E*/
/*  Fichier : $Id: xzae145sp.prc,v 1.4 1998/09/30 11:28:31 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 1998/09/30 11:28:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae145sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* archive une situation strada en base
------------------------------------------------------
* EXPTORIQUE :
*
*  Guilhou	05 Nov 1997       : Creation
*  PN		29 Jun 1998       : separation EXP/HIS 1.3
*  PN		30 Sep 1998	  : correction : ADA_FMC est repliquee ver EXP v1.4
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP XZAE145_Archive_Situation_Strada
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture des des infos associees a un evt
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE145' and type = 'P')
	drop procedure XZAE145
go

create procedure XZAE145
	@va_numero_fmc_in	int=null,
	@va_cle_fmc_in		tinyint=null,
	@va_numero_alerte_in	int=null,
	@va_cle_alerte_in	tinyint=null,
	@va_reference_in        char(50)=null
as
	declare @vl_Status int,
		@vl_ref char(50),
		@vl_type char(50),
		@vl_version int,
		@vl_horodate char(30),
		@vl_ligne char(250)

	if @va_reference_in = null
		return XDC_ARG_INV

#ifndef HIST
	/*si on cree une fmc pour cette situation strada*/
	if @va_numero_fmc_in != null
	begin
		/*si la synthese de l'evt strada est deja dans EXP*/
		if exists (select * from EXP..ADA_FMC 
		   	   where reference = @va_reference_in)
		begin
			/*mise a jour des champs numero_fmc et cle_fmc*/
			update EXP..ADA_FMC
			set numero_fmc = @va_numero_fmc_in,
			    cle_fmc = @va_cle_fmc_in
                        where reference = @va_reference_in

			return XDC_OK
		end
		else
		begin
			/*insertion de la synthese*/
			declare Pointeur cursor
			for select reference, version, horodate, ligne, type
			from EXP..ADA_SYN
			where reference = @va_reference_in
			open Pointeur

			/*parcours de la liste*/
			fetch Pointeur into @vl_ref, @vl_version, @vl_horodate, @vl_ligne, @vl_type
			while (@@sqlstatus = 0)
			begin
				insert into EXP..ADA_FMC(reference, version, horodate, ligne, type, numero_fmc, cle_fmc)
				values (@vl_ref,
					@vl_version,
					@vl_horodate,
					@vl_ligne,
					@vl_type,
					@va_numero_fmc_in,
					@va_cle_fmc_in)
				fetch Pointeur into @vl_ref, @vl_version, @vl_horodate, @vl_ligne, @vl_type
			end
			return XDC_OK
		end
	end
	else 
	if @va_numero_alerte_in != null
	begin
                /*si la synthese de l'evt strada est deja dans EXP*/
                if exists (select * from EXP..ADA_FMC
                           where reference = @va_reference_in)
                begin
                        /*mise a jour des champs numero_alerte et cle_alerte*/
                        update EXP..ADA_FMC
                        set numero_alerte = @va_numero_alerte_in,
                            cle_alerte = @va_cle_alerte_in
                        where reference = @va_reference_in

                        return XDC_OK
		end
                else    
		begin
                        /*insertion de la synthese*/
                        declare Pointeur cursor 
                        for select reference, version, horodate, ligne, type
                        from EXP..ADA_SYN
                        where reference = @va_reference_in
                        open Pointeur

                        /*parcours de la liste*/
                        fetch Pointeur into @vl_ref, @vl_version, @vl_horodate, @vl_ligne, @vl_type
                        while (@@sqlstatus = 0)
                        begin
                                insert into EXP..ADA_FMC(reference, version, horodate, ligne, type, numero_alerte, cle_alerte)
                                values (@vl_ref,
                                        @vl_version,
                                        @vl_horodate,
                                        @vl_ligne,
                                        @vl_type,
                                        @va_numero_alerte_in,
                                        @va_cle_alerte_in)
                                fetch Pointeur into @vl_ref, @vl_version, @vl_horodate, @vl_ligne, @vl_type
                        end
                        return XDC_OK
                end
	
	end
	return XDC_OK
#else
	exec @vl_Status = SQL_CI.PRC..XZAE145 
		@va_numero_fmc_in,
		@va_cle_fmc_in	,
		@va_numero_alerte_in,
		@va_cle_alerte_in,
		@va_reference_in 
	if @vl_Status = null
                     return XDC_PRC_INC
        else return @vl_Status
#endif

go

