/*E*/
/*  Fichier : $Id: asql_t26_eqt_gen.trg,v 1.3 2017/03/09 15:19:26 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2017/03/09 15:19:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asql_t26_eqt_gen.trg
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* trigger sur la table EQT_GEN pour UPDATE
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	11/11/07	creation
* JMG           02/10/09        : secto DEM887
* JMG   09/03/17 : REGIONALISATION - DEM1220 1.3
* LCL	27/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* met a jour  un enregistrement dans la table EQT_DSP pour chaque
* �quipement mis a jour dans EQT_GEN
*
* Code retour
* 
* n�ant
*
* Conditions d'utilisation
* base CFG
---------------------------------------------------*/

use CFG
go

drop trigger eqt_gen_i_trg2
go

create trigger eqt_gen_i_trg2
on EQT_GEN
for update
as

/*A D�claration des variables locales */

	declare @vl_type 	tinyint,
		@vl_eqt 	smallint,
		@vl_type_mai	tinyint,
		@vl_eqt_mai	smallint,
		@vl_nom		char(10),
		@vl_site_serv	T_SITE,
		@vl_sit 	T_SITE,
		@vl_sit_prec 	T_SITE,
		@vl_site_gestion T_SITE,
		@vl_bidon	smallint,
		@vl_mineure bit,
		@vl_majeure bit,
		@vl_HS bit,
		@vl_critique bit,
		@vl_desactive bit,
		@vl_inhibe bit
	
	select @vl_site_serv=null
		
/*A r�cup�ration des valeurs mises � jour */
		
	select	@vl_type=type,
		@vl_eqt=numero,
		@vl_type_mai=type_maitre,
		@vl_eqt_mai=maitre,
		@vl_nom=nom,
		@vl_site_gestion=site_gestion
		
	from inserted
	
/*A si le maitre n'existe pas */
		
	if @vl_eqt_mai=null or @vl_type_mai=null
	begin

/*B 	si il ne s'agit pas d'une machine : erreur */

		if @vl_type<>XDC_EQT_MAC and @vl_type<>XDC_EQT_FAX  rollback trigger
		
/*B 	recuperation du site pour machine */

		else if @vl_type=XDC_EQT_MAC 
			select @vl_site_serv=district
			from CFG..EQT_MAC
			where numero=@vl_eqt
		else 
			select @vl_site_serv=district
			from CFG..EQT_FAX
			where numero=@vl_eqt
	end

/*A s'il sagit de la base CFG */

	if db_name()="CFG"
	begin

/*A r�cup�ration du site a partir de son maitre */

		select	@vl_sit=sit
		from 	EXP..EQT_DSP DSP
		where	type=@vl_type_mai and
			equipement=@vl_eqt_mai
	
		select	@vl_sit=isnull(@vl_sit,@vl_site_serv)

		if @vl_site_gestion is not null
			select @vl_sit=@vl_site_gestion 

		select @vl_bidon=equipement ,
			@vl_mineure=mineure, @vl_majeure= majeure, @vl_critique=critique, 
			@vl_HS=HS, @vl_inhibe=inhibe, @vl_desactive=desactive,@vl_sit_prec=sit
		from EXP..EQT_DSP where type=@vl_type and equipement=@vl_eqt and dernier =1
	
			if @@rowcount=0
			begin
				if	( @vl_sit=XDC_CI and @@servername="SQL_CI" ) or
					( @vl_sit=XDC_VC and @@servername="SQL_DM" ) or
					( @vl_sit=XDC_CA and @@servername="SQL_CA" ) or
					( @vl_sit=XDC_DP and @@servername="SQL_DS" ) 
				begin
					insert	EXP..EQT_DSP	(
						type,
						equipement,
						horodate,
						mineure,
						majeure,
						critique,
						HS,
						inhibe,
						desactive,
						dernier,
						sit
						)
					values			(
						@vl_type,
						@vl_eqt,
						getdate(),
						XDC_NON,
						XDC_NON,
						XDC_NON,
						XDC_OUI, -- un equipement est cr�� HS
						XDC_NON,
						XDC_OUI, -- et d�sactiv�
						XDC_OUI,
						@vl_sit
						)
				end
			end
			else
			begin
				if @vl_sit != @vl_sit_prec 
				begin
				if @vl_sit is not null
				begin
					update EXP..EQT_DSP
					set dernier=XDC_NON
					where type=@vl_type
					and equipement=@vl_eqt
					and dernier=XDC_OUI
				end

				if ( @vl_sit=XDC_CI and @@servername="SQL_CI" ) or
					( @vl_sit=XDC_VC and @@servername="SQL_DM" ) or
					( @vl_sit=XDC_CA and @@servername="SQL_CA" ) or
					( @vl_sit=XDC_DP and @@servername="SQL_DS" ) 
				begin
					insert	EXP..EQT_DSP	(
						type,
						equipement,
						horodate,
						mineure,
						majeure,
						critique,
						HS,
						inhibe,
						desactive,
						dernier,
						sit
						)
					values			(
						@vl_type,
						@vl_eqt,
						getdate(),
						@vl_mineure,
						@vl_majeure,
						@vl_critique,
						@vl_HS, 
						@vl_inhibe,
						@vl_desactive,
						XDC_OUI,
						@vl_sit
						)
				end
				end
			end
	end
go
