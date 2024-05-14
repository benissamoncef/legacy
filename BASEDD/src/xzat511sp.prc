/*E*/
/*  Fichier : @(#)xzat511sp.prc
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzat511.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	13/11/23 	: Creation (DEM483)
------------------------------------------------------ */

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* CL	XZAT511_Utilisation_PIC
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Mot		va_NumEqt_in
* 
* Arguments en sortie
* XDY_Octet			va_DispoPIC_out
* XDY_Bandeau_PIC	va_Bandeau_out
* XDY_Booleen		va_Flash_out
* XDY_Picto_PIC		va_Picto_out
* XDY_Entier		va_NumEvt_out
* XDY_Octet			va_CleEvt_out
* XDY_Octet			va_Priorite_out
* XDY_Entier		va_DistanceEvt_out
* XDY_Entier		va_Action_out
* XDY_Site			va_SiteAction_out
* 
* Service rendu
* Retourne l'utilisation reelle d'un PIC a une horodate donnee.
* 
* Code retour
* XDC_OK
* XDC_NOK
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de recherche des infos de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
* 
* Fonction
* select dans les tables EQUIPEMENT, E_PIC, ACTION et INDISPONIBILITE
* 
------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAT511' and type = 'P')
	drop procedure XZAT511
go

create procedure XZAT511
	@va_Horodate_in			datetime					= null,
	@va_NumEqt_in			T_EQUIPEMENT				= null,
	@va_DispoPIC_out		tinyint						= null	output,
	@va_Bandeau_out 		char(XDC_LGR_Bandeau_PIC)	= null	output,
	@va_Flash_out			bit							= XDC_NON	output,
	@va_Picto_out			char(XDC_LGR_Picto_PIC)		= null	output,
	@va_NumEvt_out			int							= null	output,
	@va_CleEvt_out			tinyint						= null	output,
	@va_Priorite_out		tinyint						= null	output,
	@va_DistanceEvt_out		int							= null	output,
	@va_Action_out			int							= null	output,
	@va_SiteAction_out		T_SITE						= null	output
as
	declare @vl_HS bit, @vl_Critique bit, @vl_Majeure bit, @vl_Inhibe bit,
			@vl_Desactive bit, @vl_Mineure bit, @vl_NbEnregistrements int,
			@vl_TypeEqt tinyint, @vl_Status int

	/*! transformations bugs applix */
	if @va_NumEqt_in = 0 select @va_NumEqt_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

	/*A test les parametres obligatoires */
	if @va_NumEqt_in = null
		return XDC_ARG_INV

	/*A date nulle : date courante */
	if @va_Horodate_in=null select @va_Horodate_in=getdate()

#ifdef HIST
	/*! cas ou la procedure est creee sur HIS*/
		/*B lecture des infos sur la PIC */
		select 
			@va_NumEvt_out		= HIS..ACT_GEN.evenement,
			@va_CleEvt_out		= HIS..ACT_GEN.cle,
			@va_Bandeau_out		= HIS..ACT_PIC.bandeau,
			@va_Flash_out		= HIS..ACT_PIC.flash,
			@va_Picto_out		= HIS..ACT_PIC.pictogramme,
			@va_Priorite_out	= HIS..ACT_GEN.priorite,
			@va_DistanceEvt_out	= HIS..ACT_GEN.distance_evt,
			@va_Action_out		= HIS..ACT_GEN.numero,
			@va_SiteAction_out	= HIS..ACT_GEN.sit
				from HIS..ACT_PIC, HIS..ACT_GEN
					where	HIS..ACT_GEN.equipement		= @va_NumEqt_in			and 
							HIS..ACT_GEN.type			= XDC_ACT_PIC			and
							HIS..ACT_PIC.actio			= HIS..ACT_GEN.numero	and 
							HIS..ACT_PIC.sit			= HIS..ACT_GEN.sit		and
							HIS..ACT_GEN.heure_succes	<= @va_Horodate_in		and
							HIS..ACT_GEN.heure_fin		> @va_Horodate_in
	
		select @vl_NbEnregistrements = @@rowcount
		
		if @vl_NbEnregistrements = 0
		begin
		select 
			@va_NumEvt_out		= HIS..ACT_GEN.evenement,
			@va_CleEvt_out		= HIS..ACT_GEN.cle,
			@va_Bandeau_out		= ltrim(HIS..ACT_PIC.bandeau),
			@va_Flash_out		= HIS..ACT_PIC.flash,
			@va_Picto_out		= HIS..ACT_PIC.pictogramme,
			@va_Priorite_out	= HIS..ACT_GEN.priorite,
			@va_DistanceEvt_out	= HIS..ACT_GEN.distance_evt,
			@va_Action_out		= HIS..ACT_GEN.numero,
			@va_SiteAction_out	= HIS..ACT_GEN.sit
				from HIS..ACT_PIC, HIS..ACT_GEN
					where	HIS..ACT_GEN.equipement		= @va_NumEqt_in			and 
							HIS..ACT_GEN.type			= XDC_ACT_PIC			and
							HIS..ACT_PIC.actio			= HIS..ACT_GEN.numero	and 
							HIS..ACT_PIC.sit			= HIS..ACT_GEN.sit		and
							HIS..ACT_GEN.heure_succes	<= @va_Horodate_in		and
							HIS..ACT_GEN.heure_fin is null
		end

		if @vl_NbEnregistrements > 1
		begin
			/*! eviter une erreur sybase (car les var. de type bit doivent etre valuees ) */
			select	@va_DispoPIC_out = XDC_NON
			/*! erreur dans la base */
			return XDC_NOK
		end
		else if @vl_NbEnregistrements = 0
			/* Si le PIC n'est pas utilise alors positionner toutes */
			/* les chaines de caracteres retournees a " " pour eviter */
			/* les problemes de retour de null, sauf le type du picto */
			/* qui doit etre positionne a picto vide                  */
		begin
			select @va_Bandeau_out	= ""
			select @va_Picto_out	= ""
		end

		if @va_Flash_out = null
			select @va_Flash_out = 0

		/* lecture de la disponibilite de l'equipement */
		select
			@vl_Desactive = desactive,
			@vl_HS = HS,
			@vl_Majeure = majeure,
			@vl_Mineure = mineure,
			@vl_Critique = critique,
			@vl_Inhibe = inhibe
				from HIS..EQT_DSP
               		where	equipement = @va_NumEqt_in and 
							((@vl_TypeEqt is not null and type = @vl_TypeEqt) or
							(@vl_TypeEqt is null and type = XDC_EQT_PIC )) and
							horodate in (   select max(horodate) 
												from HIS..EQT_DSP
													where	equipement = @va_NumEqt_in and
															((@vl_TypeEqt is not null and type = @vl_TypeEqt) or
															(@vl_TypeEqt is null and type = XDC_EQT_PIC )) and
															horodate <= @va_Horodate_in)
#else
#ifdef CI
	/*! cas ou la procedure est creee au CI */
	/*A si la date donnee est anterieure d'un mois au moins */
 	/*A rechercher l'evenement dans la base HIS sinon dans  */
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
	begin	
		exec	@vl_Status = SQL_HIS.PRC..XZAT511
				@va_Horodate_in, 
				@va_NumEqt_in,
				@va_DispoPIC_out	output, 
				@va_Bandeau_out		output,
				@va_Flash_out		output,
				@va_Picto_out		output,
				@va_NumEvt_out		output, 
				@va_CleEvt_out		output,
				@va_Priorite_out	output, 
				@va_DistanceEvt_out	output,
				@va_Action_out		output, 
				@va_SiteAction_out	output
				if @vl_Status = null
				        return XDC_PRC_INC
				else return @vl_Status
	end
	else
	begin
		/* lecture des infos sur le PIC */
		select 
			@va_NumEvt_out		= EXP..ACT_GEN.evenement,
			@va_CleEvt_out		= EXP..ACT_GEN.cle,
			@va_Bandeau_out		= ltrim(EXP..ACT_PIC.bandeau),
			@va_Flash_out		= EXP..ACT_PIC.flash,
			@va_Picto_out		= EXP..ACT_PIC.pictogramme,
			@va_Priorite_out	= EXP..ACT_GEN.priorite,
			@va_DistanceEvt_out	= EXP..ACT_GEN.distance_evt,
			@va_Action_out		= EXP..ACT_GEN.numero,
			@va_SiteAction_out	= EXP..ACT_GEN.sit
				from EXP..ACT_PIC, EXP..ACT_GEN
					where	EXP..ACT_GEN.equipement = @va_NumEqt_in and 
							EXP..ACT_GEN.type = XDC_ACT_PIC and
							EXP..ACT_PIC.actio = EXP..ACT_GEN.numero and 
							EXP..ACT_PIC.sit = EXP..ACT_GEN.sit and
							(EXP..ACT_GEN.heure_succes <= @va_Horodate_in OR EXP..ACT_GEN.heure_lancement <= @va_Horodate_in) and
							(EXP..ACT_GEN.heure_fin is null or EXP..ACT_GEN.heure_fin > @va_Horodate_in)
	
		select @vl_NbEnregistrements = @@rowcount
		if @vl_NbEnregistrements > 1
		begin
			/*! eviter une erreur sybase (car les var. */
			/*! de type bit doivent etre valuees ) */
			select	@va_DispoPIC_out = XDC_NON
			/*! erreur dans la base */
			return XDC_NOK
		end
		else if @vl_NbEnregistrements = 0
		begin
			/*B si le PIC n'est pas utilise alors positionner toutes */
			/*B les chaines de caracteres retournees a " " pour eviter */
			/*B les problemes de retour de null, sauf le type du picto */
			/*B qui doit etre positionne a picto vide                  */
			select @va_Bandeau_out	= ""
			select @va_Picto_out	= ""
		end

		if @va_Flash_out = null
			select @va_Flash_out = 0

		/*B lecture de la disponibilite de l'equipement */
		select
			@vl_Desactive = desactive,
			@vl_HS = HS,
			@vl_Majeure = majeure,
			@vl_Mineure = mineure,
			@vl_Critique = critique,
			@vl_Inhibe = inhibe
		from EXP..EQT_DSP
			where	equipement = @va_NumEqt_in and ((@vl_TypeEqt is not null and type = @vl_TypeEqt) or
					(@vl_TypeEqt is null and type = XDC_EQT_PIC )) and
					horodate in (	select max(horodate)
										from EXP..EQT_DSP
											where	equipement = @va_NumEqt_in and 
													((@vl_TypeEqt is not null and type = @vl_TypeEqt) or
													(@vl_TypeEqt is null and type = XDC_EQT_PIC )) and
													horodate <= @va_Horodate_in)
	end

#else
	/*! cas ou la procedure est creee sur le serveur d'un district */
	/*A lecture des infos sur la PIC */
	select 
		@va_NumEvt_out		= EXP..ACT_GEN.evenement,
		@va_CleEvt_out		= EXP..ACT_GEN.cle,
		@va_Bandeau_out		= ltrim(EXP..ACT_PIC.bandeau),
		@va_Flash_out		= EXP..ACT_PIC.flash,
		@va_Picto_out		= EXP..ACT_PIC.pictogramme,
		@va_Priorite_out	= EXP..ACT_GEN.priorite,
		@va_DistanceEvt_out	= EXP..ACT_GEN.distance_evt,
		@va_Action_out		= EXP..ACT_GEN.numero,
		@va_SiteAction_out	= EXP..ACT_GEN.sit
			from EXP..ACT_PIC, EXP..ACT_GEN
				where	EXP..ACT_GEN.equipement = @va_NumEqt_in and 
						EXP..ACT_GEN.type = XDC_ACT_PIC and
						EXP..ACT_PIC.actio = EXP..ACT_GEN.numero and 
						EXP..ACT_PIC.sit = EXP..ACT_GEN.sit and
						EXP..ACT_GEN.heure_succes <= @va_Horodate_in and
						(EXP..ACT_GEN.heure_fin is null or 
						 EXP..ACT_GEN.heure_fin > @va_Horodate_in)

	select @vl_NbEnregistrements = @@rowcount
	if @vl_NbEnregistrements > 1
	begin
		/*! eviter une erreur sybase (car les var. */
		/*! de type bit doivent etre valuees ) */
		select	@va_DispoPIC_out = XDC_NON
		/*! erreur dans la base */
		return XDC_NOK
	end
	else if @vl_NbEnregistrements = 0
	begin
		/*A si le PIC n'est pas utilise alors positionner toutes */
		/*A les chaines de caracteres retournees a " " pour eviter */
		/*A les problemes de retour de null, sauf le type du picto */
		/*A qui doit etre positionne a picto vide                  */
		select @va_Bandeau_out	= ""
		select @va_Picto_out	= ""
	end

	if @va_Flash_out = null
		select @va_Flash_out = 0

	/*A lecture de la disponibilite de l'equipement */
	select
		@vl_Desactive = desactive,
		@vl_HS = HS,
		@vl_Majeure = majeure,
		@vl_Mineure = mineure,
		@vl_Critique = critique,
		@vl_Inhibe = inhibe
			from EXP..EQT_DSP
				where	equipement = @va_NumEqt_in and
						((@vl_TypeEqt is not null and type = @vl_TypeEqt) or
						(@vl_TypeEqt is null and type = XDC_EQT_PIC )) and
						horodate in (	select max(horodate) 
											from EXP..EQT_DSP
												where	equipement = @va_NumEqt_in and 
														((@vl_TypeEqt is not null and type = @vl_TypeEqt) or
														(@vl_TypeEqt is null and type = XDC_EQT_PIC )) and
														horodate <= @va_Horodate_in)
#endif
#endif

	if @@rowcount = 0
	begin
		/*! eviter une erreur sybase (car les var. */
		/*! de type bit doivent etre valuees ) */
		select	@va_DispoPIC_out = XDC_OUI
		/*! erreur dans la base */
		select @va_Bandeau_out	= ""
		select @va_Picto_out	= ""
	end

	if @va_Flash_out = null
		select @va_Flash_out = 0

	/* etat de l'equipement */
	select @va_DispoPIC_out =	@vl_Desactive * XDC_EQT_DESACTIVE + @vl_HS * XDC_EQT_HS +
	   							@vl_Majeure * XDC_EQT_MAJEUR + @vl_Critique * XDC_EQT_CRITIQUE +
	   							@vl_Inhibe * XDC_EQT_INHIBE + @vl_Mineure * XDC_EQT_MINEUR


	select	@va_DispoPIC_out, ltrim(rtrim(@va_Bandeau_out)),
			@va_Flash_out, @va_Picto_out,
			@va_NumEvt_out, @va_CleEvt_out,
			@va_Priorite_out, @va_DistanceEvt_out,
			@va_Action_out, @va_SiteAction_out
	return XDC_OK
go
