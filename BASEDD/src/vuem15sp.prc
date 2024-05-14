/*E*/
/* Fichier : $Id: vuem15sp.prc,v 1.9 2017/03/09 15:43:19 pc2dpdy Exp $        Release : $Revision: 1.9 $        Date : $Date: 2017/03/09 15:43:19 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem15_Detail_Depanneursp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	30/08/95	: Creation
* C.T.	11/12/95	: Ajout controle sur date de FMC (1.2)
* L.V.	20/02/97	: retrait test heure_fin is null v1.3 
* P.N.	05/02/99	: Modif interg. dem/1724 v1.5
* P.C.	05/01/00	: on prend en compte les accidents pour les types de vh 1.6
* PNI	28/03/11	: Ajout champ commentaire de FMC_VEH v1.7
* JPL	20/06/16	: Support regions pour criteres geographiques; compatible precedente (DEM 1171)  1.8
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.9
* LCL	28/09/23 	: Adaptations CNA DEM-483	
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM15_Detail_Depanneur
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in
* XDY_Site	va_District_in
* 
* Arguments en sortie
* 
* Service rendu
* Detail des actions de d�pannage par ditrict et par mois
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

if exists (select 1 from sysobjects where name = 'VUEM15' and type = 'P')
	drop procedure VUEM15
go


create procedure VUEM15
	@va_Debut_in	datetime	= null,
	@va_District_in	T_SITE		= null
as
	declare @vl_Fin datetime, @vl_Type smallint, @vl_NomType char(25),
		@vl_Evt int, @vl_CleEvt tinyint, 
		@vl_DebutAct char(19), @vl_FinAct char(19),
		@vl_DebutEvt char(19), @vl_FinEvt char(19),
		@vl_Depanneur char(25), @vl_Sens T_SENS, 
		@vl_Autoroute T_AUTOROUTE, @vl_NomAutoroute char(4), 
		@vl_PR T_PR, @vl_TypeVeh tinyint, @vl_Duree int,
		@vl_PointCar tinyint, @vl_TypeMeteo tinyint, @vl_observation char (150)

	/*A controle des parametres d'entree */
	if @va_Debut_in is null
		return XDC_ARG_INV

	select @vl_Fin = dateadd(month, 1, @va_Debut_in)

	create table #LISTE_DEP (debut_evt char(19), fin_evt char(19),
		debut_act char(19), fin_act char(19),
		duree int null , autoroute char(4) default XDC_CHAINE_VIDE, PR int  null, 
		sens tinyint null, point_caracteristique tinyint null, 
		nom_type char(25), type_veh tinyint null,
		depanneur char(25) null, meteo tinyint null, observation char (150) )


	/* creer et renseigner une table temporaires des sites (regions, secteurs, districts) */
	create table #LISTE_SITES ( numero tinyint null )

	insert #LISTE_SITES ( numero )  values ( @va_District_in )


	/*A rechercher les actions depanneurs sur le mois */
	declare pointeur_action cursor
	for select
		HIS..ACT_DEP.nom,
		convert(char(10),HIS..ACT_GEN.heure_succes, 103)+' '+convert(char(8),HIS..ACT_GEN.heure_succes,8),
		convert(char(10),HIS..ACT_GEN.heure_fin, 103)+' '+convert(char(8), HIS..ACT_GEN.heure_fin, 8),
		convert(char(10),HIS..FMC_GEN.debut, 103)+' '+convert(char(8),HIS..FMC_GEN.debut, 8),
		convert(char(10),HIS..FMC_GEN.fin, 103)+' '+convert(char(8),HIS..FMC_GEN.fin, 8),
		datediff(minute, HIS..FMC_GEN.debut,HIS..FMC_GEN.fin),
		HIS..FMC_GEN.type,
		CFG..TYP_FMC.nom,
		HIS..FMC_GEN.numero,
		HIS..FMC_GEN.cle
	from HIS..ACT_DEP, HIS..ACT_GEN, HIS..FMC_GEN, CFG..TYP_FMC
	where HIS..ACT_GEN.numero = actio and 
		HIS..ACT_GEN.sit = HIS..ACT_DEP.sit and
		((@va_District_in=99 and HIS..FMC_GEN.datex=XDC_VRAI) or
		 (@va_District_in<>99 and HIS..FMC_GEN.datex<>XDC_VRAI)) and
		HIS..ACT_GEN.heure_succes < @vl_Fin and 
		HIS..ACT_GEN.heure_fin >= @va_Debut_in and
		HIS..FMC_GEN.cloture >= @va_Debut_in and
		HIS..FMC_GEN.numero = evenement and
		HIS..FMC_GEN.cle = HIS..ACT_GEN.cle and
		CFG..TYP_FMC.numero = HIS..FMC_GEN.type
	open pointeur_action

	fetch pointeur_action into @vl_Depanneur, @vl_DebutAct, @vl_FinAct,
				@vl_DebutEvt, @vl_FinEvt, @vl_Duree, 
				@vl_Type, @vl_NomType, @vl_Evt, @vl_CleEvt
	while @@sqlstatus = 0
	begin
		/* rechercher la derniere localisation */
		select
			@vl_Autoroute = autoroute,
			@vl_PR = PR,
			@vl_Sens = sens,
			@vl_PointCar = point_caracteristique
		from HIS..FMC_HIS
		where numero =@vl_Evt and cle = @vl_CleEvt 
		group by numero,cle
		having horodate_validation=max(horodate_validation)
		
		/* tester si c'est sur le district */
		select
			@vl_NomAutoroute = CFG..RES_AUT.nom
		from CFG..RES_POR, CFG..RES_AUT
		where autoroute = @vl_Autoroute and
			PR_debut <= @vl_PR and 
			PR_fin > @vl_PR and
			( @va_District_in = 99  or  district in (select numero from #LISTE_SITES) ) and
			CFG..RES_AUT.numero = CFG..RES_POR.autoroute 

		if @@rowcount != 0
		begin	
			select @vl_observation=""
			/* rechercher le type de vehicule depanne */
			if @vl_Type = XZAEC_FMC_VehArrete or
			   @vl_Type = XZAEC_FMC_VehPanne or
			   @vl_Type = XZAEC_FMC_VehFeu or
			   @vl_Type = XZAEC_FMC_Accident or
			   @vl_Type = XZAEC_FMC_Contresens
			begin
				select @vl_TypeVeh = type_de_vehicule,@vl_observation=commentaire
				from HIS..FMC_VEH
				where numero = @vl_Evt and
					cle = @vl_CleEvt 

				if @vl_TypeVeh is null
					select @vl_TypeVeh = 0
			end
			else	select @vl_TypeVeh = 0

			/* rechercher le type de meteo */
			if @vl_Type = XZAEC_FMC_Meteo
			begin
				select @vl_TypeMeteo = type_de_perturbation
				from HIS..FMC_MET
				where numero = @vl_Evt and
					cle = @vl_CleEvt 
			end

			insert #LISTE_DEP ( autoroute, PR, sens,
			point_caracteristique, debut_evt, fin_evt, 
			duree, debut_act, fin_act, nom_type, type_veh, 
			depanneur, meteo, observation)
			values (@vl_NomAutoroute, @vl_PR, @vl_Sens,
			@vl_PointCar, @vl_DebutEvt, @vl_FinEvt, 
			@vl_Duree, @vl_DebutAct,
			@vl_FinAct, @vl_NomType, @vl_TypeVeh,
			@vl_Depanneur, @vl_TypeMeteo,@vl_observation)
		end
		
		fetch pointeur_action into @vl_Depanneur, 
				@vl_DebutAct, @vl_FinAct,
				@vl_DebutEvt, @vl_FinEvt, @vl_Duree, 
				@vl_Type, @vl_NomType, @vl_Evt, @vl_CleEvt
	end
	close pointeur_action

	/*A retourner la liste */
	select
		debut_evt,
		fin_evt,
		duree,
		autoroute,
		PR,
		sens,
		debut_act,
		fin_act,
		depanneur,
		type_veh,
		nom_type,
		point_caracteristique,
		meteo, 
		observation
	from #LISTE_DEP
	order by debut_evt, debut_act

	return XDC_OK
go
