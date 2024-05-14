/*E*/
/*  Fichier : $Id: xzae111sp.prc,v 1.13 2019/09/27 18:13:24 devgtie Exp $      Release : $Revision: 1.13 $        Date : $Date: 2019/09/27 18:13:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae111sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	29/10/96	: Creation
* P.N.	10/03/97	: Ajout du type PosteOperateur pour TDP v1.2
* P.N.  04/05/99	: Correction pour execution sur SQL_HIS v1.3
* CHI	18/04/05	: Ajout champ sens pour gestion des zones de decrochage DEM457 V1.4
* JPL	15/05/07	: Recheche PR tete en table meteo pour FMCs Chaussee (DEM 644,646) 1.5
* JPL	05/06/07	: Recheche PR tete en table travaux pour FMC Basculement pour travaux (DEM 647) 1.6
* JPL	05/11/09	: Recheche PR tete en table FMC_BAS pour FMC Basculement 1.7
* JPL	05/11/09	: Ajout champ sens pour FMC PosteOperateur egalement 1.8
* JPL	05/11/09	: Procedure degroupee; lecture localisation en un select, dans FMC_HIS_DER au CI 1.9
* JPL	19/08/10	: Recheche PR tete en table travaux pour FMC Zone de Stockage (DEM 942) 1.10
* PNI	21/08/19	: utilisation FMC_HIS pour eviter deadlock et renvoie d'une localisation par def en cas d'erreur DEM1357 v1.11
* PNI	18/09/19	:  En cas d'erreur renvoyer XDC_OK avec la localisation par défaut DEM1357  1.12
* PNI	27/09/19	:  En cas d'erreur renvoyer XDC_OK avec la localisation par défaut DEM1357  1.13
---------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les PR d'une fiche main 
* courante lors de sa derniere validation
* -----------------------------------------------------
* Sequence d'appel
* SP	XZAE111_Lire_Localisation_FMC
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* 
* Arguments en sortie
*
*
* 
* Code retour
* XDC_OK
* XDC_NOK	: fiche non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* 
* Fonction
----------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE111' and type = 'P')
	drop procedure XZAE111
go


create procedure XZAE111
	@va_NumEvt_in		int = null,
	@va_CleEvt_in		tinyint = null
as
	declare @vl_Horodate		datetime,
		@vl_Type		tinyint,
		@vl_NumCause		int,
		@vl_CleCause		tinyint,
		@vl_NumAutoroute	tinyint,
		@vl_Autoroute		char(4),
		@vl_PR			T_PR,
		@vl_PRtete		T_PR,
		@vl_Sens		tinyint

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvt_in = 0 select @va_NumEvt_in = null
	if @va_CleEvt_in = 0 select @va_CleEvt_in = null

	/*A controle des arguments obligatoires */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

#ifdef CI
	/*A Verifier l'existence de l'evenement */
	select
		@vl_Type		= type,
		@vl_NumCause            = cause,
		@vl_CleCause            = cle_cause
	from  EXP..FMC_GEN
	where	numero = @va_NumEvt_in and
		cle = @va_CleEvt_in

	if @@rowcount = 0
	begin
		select "A0",1,114300,114300
		return XDC_OK
	end

		if @vl_Type=XZAEC_FMC_PosteOperateur
		begin
			select CFG..RES_AUT.nom, Sens = XDC_SENS_INCONNU, min(PR_debut), max(PR_fin)
			from CFG..RES_POR, CFG..RES_AUT 
			where district=@va_CleEvt_in and
			      CFG..RES_AUT.numero=autoroute
			group by CFG..RES_AUT.nom
		end
		else
		begin
			/*A rechercher la localisation de l'evenement dans */
			/*A l'historique a la derniere date de validation  */
			select
				@vl_NumAutoroute	= autoroute,
				@vl_PR			= PR,
				@vl_Sens		= sens,
				@vl_Horodate		= horodate_validation,
				@vl_Autoroute		= nom
			from EXP..FMC_HIS H, CFG..RES_AUT A
			where	H.numero = @va_NumEvt_in
			  and	H.cle = @va_CleEvt_in
			  and	A.numero = H.autoroute
			  group by H.numero,H.cle
			having H.numero = @va_NumEvt_in
			and   H.cle = @va_CleEvt_in
			and   A.numero = H.autoroute
			and   H.horodate_validation=max(H.horodate_validation)

			/* si aucune validation n'a ete effectuee alors erreur */
			if @@rowcount = 0
			begin
				select "A0",1,114300,114300
				return XDC_OK
			end 

			/*print "Derniere validation a %1! : %2!-%3!-%4!", @vl_Horodate, @vl_Autoroute, @vl_Sens, @vl_PR*/


			/*A Recherche la localisation de la tete de l'evenement */
			/*A s'il y a lieu                            */
			if @vl_Type=XZAEC_FMC_QueueBouchon
			begin
				select 
					@vl_PRtete		= PR
				from  EXP..FMC_HIS
				where	numero = @vl_NumCause and 
					cle = @vl_CleCause 
				group by numero,cle
				having
				        horodate_validation = max( horodate_validation )

			end
			else
			if (@vl_Type=XZAEC_FMC_Travaux or
			    @vl_Type=XZAEC_FMC_ZoneStockage or
			    @vl_Type=XZAEC_FMC_BasculTravaux)
			begin
				select 
					@vl_PRtete		= localisation_tete
				from  EXP..FMC_TRH
				where	numero =  @va_NumEvt_in and 
					cle = @va_CleEvt_in and 
					horodate = @vl_Horodate

			end
			else
			if (@vl_Type=XZAEC_FMC_Basculement)
			begin
				select 
					@vl_PRtete		= localisation_PR_fin
				from  EXP..FMC_BAS
				where	numero =  @va_NumEvt_in and 
					cle = @va_CleEvt_in
			end
			else
			if (@vl_Type=XZAEC_FMC_Meteo or
			    @vl_Type=XZAEC_FMC_TraitChaussees or
			    @vl_Type=XZAEC_FMC_ChausseeGliss)
			begin
				select 
					@vl_PRtete		= PR
				from  EXP..FMC_MET
				where	numero =  @va_NumEvt_in and 
					cle = @va_CleEvt_in and 
					horodate = @vl_Horodate
			end
			else
			begin
				select @vl_PRtete = @vl_PR
			end
		
			if (@vl_Autoroute=null) or (@vl_Sens=null) or (@vl_PR=null) or (@vl_PRtete=null)
			begin
				select "A0",1,114300,114300
				return XDC_OK
			end
			else
			begin
				select 	@vl_Autoroute,@vl_Sens,@vl_PR,@vl_PRtete
			end
		end
#else
#ifdef HIST
	/*A Verifier l'existence de l'evenement */
	select
		@vl_Type		= type,
		@vl_NumCause            = cause,
		@vl_CleCause            = cle_cause
	from  HIS..FMC_GEN
	where	numero = @va_NumEvt_in and
		cle = @va_CleEvt_in

	if @@rowcount = 0
		return XDC_NOK


		if @vl_Type=XZAEC_FMC_PosteOperateur
		begin
			select CFG..RES_AUT.nom, Sens = XDC_SENS_INCONNU, min(PR_debut), max(PR_fin)
			from CFG..RES_POR, CFG..RES_AUT 
			where district=@va_CleEvt_in and
			      CFG..RES_AUT.numero=autoroute
			group by CFG..RES_AUT.nom
		end
		else
		begin
			/*A rechercher la localisation de l'evenement dans */
			/*A l'historique a la derniere date de validation  */
			select
				@vl_NumAutoroute	= autoroute,
				@vl_PR			= PR,
				@vl_Sens		= sens,
				@vl_Horodate		= horodate_validation,
				@vl_Autoroute		= nom
			from HIS..FMC_HIS H, CFG..RES_AUT A
			where	H.numero = @va_NumEvt_in
			  and	H.cle = @va_CleEvt_in
			having	H.numero = @va_NumEvt_in
			   and	H.cle = @va_CleEvt_in
			   and	horodate_validation = max(horodate_validation)
			   and	A.numero = H.autoroute

			/* si aucune validation n'a ete effectuee alors erreur */
			if @@rowcount = 0
				return XDC_NOK

			/*print "Derniere validation a %1! : %2!-%3!-%4!", @vl_Horodate, @vl_Autoroute, @vl_Sens, @vl_PR*/


			/*A Recherche la localisation de la tete de l'evenement */
			/*A s'il y a lieu                            */
			if @vl_Type=XZAEC_FMC_QueueBouchon
			begin
				select 
					@vl_PRtete		= PR
				from  HIS..FMC_HIS
				where	numero = @vl_NumCause and 
					cle = @vl_CleCause 
				group by numero,cle
				having 
					horodate_validation = max( horodate_validation )

			end
			else
			if (@vl_Type=XZAEC_FMC_Travaux or
			    @vl_Type=XZAEC_FMC_ZoneStockage or
			    @vl_Type=XZAEC_FMC_BasculTravaux)
			begin
				select 
					@vl_PRtete		= localisation_tete
				from  HIS..FMC_TRH
				where	numero =  @va_NumEvt_in and 
					cle = @va_CleEvt_in and 
					horodate = @vl_Horodate
			end
			else
			if (@vl_Type=XZAEC_FMC_Basculement)
			begin
				select 
					@vl_PRtete		= localisation_PR_fin
				from  HIS..FMC_BAS
				where	numero =  @va_NumEvt_in and 
					cle = @va_CleEvt_in
			end
			else
			if (@vl_Type=XZAEC_FMC_Meteo or
			    @vl_Type=XZAEC_FMC_TraitChaussees or
			    @vl_Type=XZAEC_FMC_ChausseeGliss)
			begin
				select 
					@vl_PRtete		= PR
				from  HIS..FMC_MET
				where	numero =  @va_NumEvt_in and 
					cle = @va_CleEvt_in and 
					horodate = @vl_Horodate
			end
			else
			begin
				select @vl_PRtete = @vl_PR
			end
			
			select 	@vl_Autoroute,@vl_Sens,@vl_PR,@vl_PRtete

		end
#endif
#endif
	return XDC_OK

go
