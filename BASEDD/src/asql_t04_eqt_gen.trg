/*E*/
/*  Fichier : $Id: asql_t04_eqt_gen.trg,v 1.14 2017/03/09 15:18:32 pc2dpdy Exp $      Release : $Revision: 1.14 $        Date : $Date: 2017/03/09 15:18:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER asql_t04_eqt_gen.trg
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
* trigger sur la table EQT_GEN
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.  11/11/94        : Creation
* B.G.  11/11/94        : Modif site pour machine (1.2+1.3)
* B.G.  29/11/94        : Ajout table EQT_ACV (1.4)
* B.G.  29/11/94        : modif select * (1.5)
* C.T.  29/11/95        : pas de rollback sur fax (1.6)
* C.T.  22/04/96        : correction recherche du district pour fax (1.7-1.8)
* PNI	07/11/05	: Suppression de la condition de sit pour le CI DEM365 (1.9)
* PNI	23/11/05	: Suppression de la correction pour la  DEM365 (1.10)
* LCL   09/07/07        : Suppression DY (1.11)
* JMG   13/11/07        : ajout site de gestion DEM715
* JMG           02/10/09        : secto DEM887
* JMG   09/03/17 	: REGIONALISATION - DEM1220 1.14
* JPC   19/07/23 	: Correction script comme en PROD
* LCL	27/09/23 : Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Cr�e un enregistrement dans la table EQT_DSP pour chaque
* �quipement cr��
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

drop trigger eqt_gen_i_trg
go

create trigger eqt_gen_i_trg
on EQT_GEN
for insert
as

/*A D�claration des variables locales */

	declare @vl_type 	tinyint,
		@vl_eqt 	smallint,
		@vl_type_mai	tinyint,
		@vl_eqt_mai	smallint,
		@vl_nom		char(10),
		@vl_site_serv	T_SITE,
		@vl_sit 	T_SITE,
		@vl_site_gestion T_SITE,
		@vl_bidon	smallint
	
	select @vl_site_serv=null
		
/*A r�cup�ration des valeurs ins�r�es */
		
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

		select  @vl_sit=isnull(@vl_sit,@vl_site_gestion)

/*A si on est en local */

		if	( @vl_sit=XDC_CI and @@servername="SQL_CI" ) or
			( @vl_sit=XDC_VC and @@servername="SQL_DM" ) or
			( @vl_sit=XDC_DP and @@servername="SQL_DS" ) or
			( @vl_sit=XDC_CA and @@servername="SQL_CA" ) 

/*A on cr�e la table de disponibilite */

		begin
		select @vl_bidon=equipement from EXP..EQT_DSP where type=@vl_type and equipement=@vl_eqt
	
			if @@rowcount=0
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
/*A on cr�e la table d'activation */

		select @vl_bidon=equipement from EXP..EQT_ACV where type=@vl_type and equipement=@vl_eqt
	
			if @@rowcount=0
			insert	EXP..EQT_ACV	(
						type,
						equipement,
						activable,
						a_desactiver,
						sit
						)
			values			(
						@vl_type,
						@vl_eqt,
						XDC_NON,
						XDC_NON,
						@vl_sit
						)
		end
	end
go
