/*E*/
/*  Fichier : @(#)xzae153sp.prc	1.3      Release : 1.3        Date : 10/30/01
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae153sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* strada : Calcul Autoroute,PR,sens a partir du loc RDS
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	24 Dec 1998 : creation
* Niepceron	03 Mar 1999 : correction v1.2
* F.Lizot	06 Oct 2004 : correction SAGA
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE153_Localisation
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
* Select dans RDS et calcul suivant methode
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE153' and type = 'P')
	drop procedure XZAE153
go

create procedure XZAE153
	@va_L1_in		char(4)=null,
	@va_L2_in		char(5)=null,
	@va_L3_in		tinyint=null,
	@va_L4_in		int=null,
	@va_L5_in		char(2)=null,
	@va_L6_in		char(35)=null,
	@va_L7_in		char(70)=null,
	@va_L8_in		int=null,
	@va_L9_in		int=null,
	@va_L10_in		char(35)=null,
	@va_L11_in		int=null,
	@va_L12_in		int=null,
	@va_libelle_out 	char(250)=null output,
	@va_Autoroute_out	tinyint=null output,
	@va_PR_out		T_PR=null output,
	@va_Sens_out		tinyint=null output,
	@va_Autoroutetete_out	tinyint=null output,
	@va_PRtete_out		T_PR=null output
	
as
declare	@vl_inv smallint
	select @va_libelle_out="toto0"
	select @va_Autoroute_out=1
	 
	if @va_L1_in = null or 
	   @va_L2_in = null or 
	   @va_L3_in = null or 
	   @va_L4_in = null  
	   return XDC_ARG_INV
	   
	/* On verifie que l'on est la table RDS de la version specifiee */
	if not exists ( select * from CFG..NUM_RDS where numero=@va_L1_in)
		return XDC_NOK
		
		
	/* On ne traite que les localisation de type LOC */
/*	if @va_L2_in <> "LOC"
		return XDC_NOK
*/	
	select @va_libelle_out=""
	/* Methode 1 */
	if @va_L3_in = 1
	begin
		select @va_PRtete_out=pr,
			@va_Autoroutetete_out=autoroute
		from CFG..RDS
		where numero=@va_L4_in	
		
		select @va_libelle_out=rtrim(@va_L6_in)
		
		if @va_L8_in != null and @va_L8_in !=0
		begin
			select @va_PR_out=pr,
				@va_Autoroute_out=autoroute
			from CFG..RDS
			where numero=(@va_L4_in	+ @va_L8_in)
		end
		else
		begin
			select @va_PR_out=@va_PRtete_out,
				@va_Autoroute_out=@va_Autoroutetete_out
		end
	end
	else
	/* Methode 2 */
	if @va_L3_in = 2
	begin
		select @va_PR_out=pr,
			@va_Autoroute_out=autoroute
		from CFG..RDS
		where numero=@va_L9_in	
		
			
		select @va_PRtete_out=pr,
			@va_Autoroutetete_out=autoroute
		from CFG..RDS
		where numero=@va_L4_in		
		
		select @va_libelle_out= ( rtrim(@va_L6_in) + " " + rtrim(@va_L10_in) )
	end
	else
	/* Methode 3 */
	if @va_L3_in = 3
	begin
		select @va_PRtete_out=pr,
			@va_Autoroutetete_out=autoroute
		from CFG..RDS
		where numero=@va_L4_in	
		
		select @va_libelle_out=rtrim(@va_L6_in)

		if @va_L8_in != null and @va_L8_in !=0
		begin
			select @va_PR_out=pr,
				@va_Autoroute_out=autoroute
			from CFG..RDS
			where numero=(@va_L4_in	+ @va_L8_in)
		end
		else
		begin
			select @va_PR_out=@va_PRtete_out,
				@va_Autoroute_out=@va_Autoroutetete_out
		end
				
		select @vl_inv=inversee
		from CFG..RES_AUT_INV 
		where numero=@va_Autoroute_out

		if @va_L5_in = "P" or ( @va_L5_in = "N" and @vl_inv=XDC_PR_INV)
		begin
			select @va_PRtete_out=@va_PRtete_out-@va_L11_in
			select @va_PR_out=@va_PR_out+@va_L12_in
		end
		else
		if @va_L5_in = "N" or ( @va_L5_in = "P" and @vl_inv=XDC_PR_INV)
		begin
			select @va_PRtete_out=@va_PRtete_out+@va_L11_in
			select @va_PR_out=@va_PR_out-@va_L12_in
		end
		else
		begin
			select @va_PRtete_out=@va_PRtete_out-@va_L11_in
			select @va_PR_out=@va_PR_out+@va_L12_in
		end
	end
	else
	/* Methode 4 */
	if @va_L3_in = 4
	begin
		if @va_L9_in<>0
		begin
			select @va_PR_out=pr,
				@va_Autoroute_out=autoroute
			from CFG..RDS
			where numero=@va_L9_in	
			select @va_PRtete_out=pr,
				@va_Autoroutetete_out=autoroute
			from CFG..RDS
			where numero=@va_L4_in	
		end
		else
		begin
			select @va_PRtete_out=null
			select @va_PR_out=pr,
				@va_Autoroute_out=autoroute
			from CFG..RDS
			where numero=@va_L4_in
			select @va_Autoroutetete_out=@va_Autoroute_out
		end
		
		
		select @vl_inv=inversee
		from CFG..RES_AUT_INV 
		where numero=@va_Autoroute_out
	
		select @va_libelle_out= ( rtrim(@va_L6_in) + " " + rtrim( @va_L10_in) )
		
		/* Modif SAGA */
		if @va_L5_in = "N"
		begin
			if @va_L9_in<>0
			begin
				select @va_PRtete_out=@va_PRtete_out+@va_L11_in
				select @va_PR_out=@va_PR_out-@va_L12_in
                        end
			else
			begin
				select @va_PR_out=@va_PR_out+@va_L11_in
                        end
		end
                else
		begin
			if @va_L9_in<>0
			begin
				select @va_PRtete_out=@va_PRtete_out-@va_L11_in
				select @va_PR_out=@va_PR_out+@va_L12_in
                        end
			else
			begin
				select @va_PR_out=@va_PR_out-@va_L11_in
                        end
		end
                
		/* Supprime : SAGA
		if @va_L5_in = "P" or ( @va_L5_in = "N" and @vl_inv=XDC_PR_INV)
		begin
			if @va_L9_in<>0
			begin
				select @va_PRtete_out=@va_PRtete_out-@va_L11_in
				select @va_PR_out=@va_PR_out+@va_L12_in
			end
			else
			begin
				select @va_PR_out=@va_PR_out+@va_L11_in
			end
		end
		else
		if @va_L5_in = "N" or ( @va_L5_in = "P" and @vl_inv=XDC_PR_INV)
		begin
			if @va_L9_in<>0
			begin
				select @va_PRtete_out=@va_PRtete_out-@va_L11_in
				select @va_PR_out=@va_PR_out+@va_L12_in
			end
			else
			begin
				select @va_PR_out=@va_PR_out+@va_L11_in
			end
		end
		else
		begin
			 if @va_L9_in<>0
			 begin
				 select @va_PRtete_out=@va_PRtete_out-@va_L11_in
				 select @va_PR_out=@va_PR_out+@va_L12_in
			 end
			 else
			 begin
				 select @va_PR_out=@va_PR_out+@va_L11_in
			 end
		end
		*/
		
	end
	else
		return XDC_NOK
		
	if @va_PRtete_out=null
	begin
		select @va_PRtete_out=@va_PR_out
		select @va_PR_out=@va_PR_out
	end
	
	select @vl_inv=inversee
	from CFG..RES_AUT_INV 
	where numero=@va_Autoroute_out
	
	if @va_L5_in = "P"
	begin
		if @vl_inv=XDC_PR_INV
			select @va_Sens_out=XDC_SENS_NORD
		else
			select @va_Sens_out=XDC_SENS_SUD
	end
	else
	if @va_L5_in = "N"
	begin
		if @vl_inv=XDC_PR_INV
			select @va_Sens_out=XDC_SENS_SUD
		else
			select @va_Sens_out=XDC_SENS_NORD
	end
	else
		select @va_Sens_out=XDC_SENS_INCONNU
		
		
	select @va_libelle_out=( @va_libelle_out + ", " + @va_L5_in + ", " + rtrim(@va_L7_in))
	return XDC_OK
go

