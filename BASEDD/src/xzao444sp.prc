/*E*/
/* Fichier : $Id: xzao444sp.prc,v 1.2 2018/06/07 19:12:21 devgfi Exp $        Release : $Revision: 1.2 $        Date : $Date: 2018/06/07 19:12:21 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzao444sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Recherche des equipements impactes par une perturbation
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	14/12/16 : Creation  1.1
* JPL	07/06/18 : Suppression commentaires imbriques  1.2
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* Service rendu
*
* Si l'evenement est termine, retourne les equipements utilises pour cet evenement ;
* Sinon retourne les equipements a activer (dans la perturbation)
*             et ceux dont le panneau d'alerte est a activer (pour les Travaux)
*             et les equipements a remettre au neutre
*                (hors zone d'influence et utilises pour cet evenement).
*
* RESTRICTIONS :
*  - au type d'evenement concerne (Travaux) ;
*  - au type de BRA supportant la fonction (pour efficacite) ;
*
* -----------------------------------------------------------------------------
* Sequence d'appel
* SP	XZAO444_BRA_En_Perturbation
*
* Arguments en entree
* XDY_Autoroute		va_Autoroute_in
* XDY_Sens		va_Sens_in
* XDY_PR		va_PR_in
* XDY_Horodate		va_Horodate_in
* XDY_PR		va_PR_Tete_in
* XDY_Entier		va_Numero_Fmc_in
* XDY_Octet		va_Cle_Fmc_in
*
* Arguments en sortie
* XDY_Eqt		va_Num_BRA_out
* XDY_District		va_District_out
* XDY_PR		va_PR_out
* XDY_Octet		va_Type_BRA
* XDY_Octet		va_Limite_Vitesse
* XDY_Octet		va_Mise_Au_Neutre
*
* Code retour
* XDC_OK
* XDC_NOK
* <0 erreur sybase
*
* Conditions d'utilisation
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO444' and type = 'P')
	drop procedure XZAO444
go


#ifdef HIST
	#define	BDD_XPL	HIS
#else
	#define	BDD_XPL	EXP
#endif



create procedure XZAO444
	@va_Autoroute_in	T_AUTOROUTE = null,
	@va_Sens_in		tinyint = null,
	@va_PR_in		T_PR = null,
	@va_Horodate_in		datetime = null,
	@va_PR_Tete_in		T_PR = null,
	@va_Numero_Fmc_in	int = null,
	@va_Cle_Fmc_in		tinyint = null
as

declare	@vl_numero_eqt smallint, @vl_district_eqt tinyint, @vl_PR_eqt int, @vl_type_bra tinyint,
	@vl_type_fmc tinyint, @vl_horodate_fin datetime,
	@vl_ordre_PR_autoroute smallint, @vl_ordre_PR_chaussee smallint,
	@vl_vitesse tinyint, @vl_tunnel bit

	/*A Controle des parametres d'entree obligatoire */
	if	@va_PR_in = null or
		@va_Sens_in = null

	return XDC_ARG_INV


	/*A Rechercher les caracteristiques de l'autoroute */
	select	@vl_ordre_PR_autoroute = inversee
	from	CFG..RES_AUT_INV
	where	numero = @va_Autoroute_in

	if @@rowcount = 0
		return XDC_ARG_INV


	/*A Ramener le sens sur section courante s'il s'agit d'une sortie ou d'une entree */
	if	@va_Sens_in = XDC_SENS_SORTIE_NORD or
		@va_Sens_in = XDC_SENS_ENTREE_NORD
	begin
	  select @va_Sens_in = XDC_SENS_NORD
	end

	if	@va_Sens_in=XDC_SENS_SORTIE_SUD or
		@va_Sens_in=XDC_SENS_ENTREE_SUD
	begin
	  select @va_Sens_in = XDC_SENS_SUD
	end

	/*A Si le sens est inconnu, pas d'equipement amont */
	if @va_Sens_in <> XDC_SENS_NORD  and  @va_Sens_in <> XDC_SENS_SUD
	  return XDC_OK


	/*A Determiner l'ordre des PR sur la chaussee */
	select @vl_ordre_PR_chaussee = 1
	if ( (@vl_ordre_PR_autoroute = XDC_PR_NOR)  and  (@va_Sens_in = XDC_SENS_NORD) )  or
	   ( (@vl_ordre_PR_autoroute = XDC_PR_INV)  and  (@va_Sens_in = XDC_SENS_SUD) )
		select @vl_ordre_PR_chaussee = (-1)


	create table #TMP_EQT (
		numero_eqt	smallint	not null,
		district	tinyint		null,
		PR		int		not null,
		type_bra	tinyint		not null,
		limite_vitesse	tinyint		null,
		mise_au_neutre	tinyint		not null
	)


	/* Si un evenement est fourni alors determiner son type et s'il est termine */
	select	@vl_type_fmc = type,
		@vl_horodate_fin = fin
	from	BDD_XPL..FMC_GEN
	where	numero = @va_Numero_Fmc_in  and  cle = @va_Cle_Fmc_in


	/* Selectionner les equipements disponibles sur l'autoroute et dans le sens specifies */
	declare Equipement_En_Pert cursor for
		select 	distinct EQT.numero,
			EQT.PR,
			DSP.sit,
			NAV.type
		from	CFG..EQT_GEN EQT,
			BDD_XPL..EQT_DSP DSP,
			CFG..EQT_NAV NAV
		where	EQT.type	= XDC_EQT_NAV and
			EQT.autoroute	= @va_Autoroute_in and
			EQT.sens	= @va_Sens_in and
			NAV.numero	= EQT.numero and
			DSP.equipement	= EQT.numero and
			DSP.type	= XDC_EQT_NAV and
			DSP.dernier	= 1 and 	-- derniere dispo connue
			DSP.inhibe	= 0 and 	-- non inhibe
			DSP.desactive	= 0 and 	-- non desactive
			DSP.majeure	= 0 and 	-- en panne
			DSP.HS		= 0 		-- non HS

		and	NAV.type        in (7, 9)			-- RESTRICTION aux types supportant la fonction
		and	abs (EQT.PR - @va_PR_in) < 10000		-- se limiter a une zone autour du lieu indique
		-- order by (EQT.PR * @vl_ordre_PR_chaussee) asc		-- dans le sens de circulation

	open Equipement_En_Pert

	fetch Equipement_En_Pert into	@vl_numero_eqt, @vl_PR_eqt, @vl_district_eqt, @vl_type_bra

	while (@@sqlstatus = 0 )
	begin
		/* Identifier les equipements candidats a la remise au neutre */
		/* (utilises pour cet evenement, et hors zone ou evenement termine) */
		if @vl_type_fmc = XZAEC_FMC_Travaux  and
		  ( @vl_horodate_fin is not null  or
		    not ( @vl_PR_eqt between @va_PR_in and @va_PR_Tete_in or
		          @vl_PR_eqt between @va_PR_Tete_in and @va_PR_in
		        )
		  )
		begin
			if exists (
#ifdef HIST
				select 1 from HIS..ACT_GEN where type = XDC_EQT_NAV and equipement = @vl_numero_eqt and
								evenement = @va_Numero_Fmc_in and cle = @va_Cle_Fmc_in and
								heure_succes <= @va_Horodate_in and heure_fin >= @va_Horodate_in
#else
				select 1 from EXP..ACT_GEN where type = XDC_EQT_NAV and equipement = @vl_numero_eqt and
								evenement = @va_Numero_Fmc_in and cle = @va_Cle_Fmc_in and
								heure_lancement < getdate() and heure_succes < getdate() and
								heure_lancement_fin is null and heure_fin is null
#endif
			)
			begin
				insert #TMP_EQT (numero_eqt, district, PR, type_bra, limite_vitesse, mise_au_neutre)
				values (@vl_numero_eqt, @vl_district_eqt, @vl_PR_eqt, @vl_type_bra, 0, XDC_VRAI)
			end
		end


		/* Identifier les equipements dont le panneau d'alerte est activable */
		/* (en zone d'activation et evenement de type concerne en cours) */
		if @vl_horodate_fin is null  and
		   @vl_type_fmc = XZAEC_FMC_Travaux and
		   ( @vl_PR_eqt between @va_PR_in and @va_PR_Tete_in or
		     @vl_PR_eqt between @va_PR_Tete_in and @va_PR_in
		   )
		begin
			/* Determiner la limite de vitesse au PR de l'equipement */
			exec PRC..XZAO464 @va_Autoroute_in, @vl_PR_eqt, @vl_PR_eqt, @va_Sens_in, @vl_vitesse output, @vl_tunnel output
			insert #TMP_EQT (numero_eqt, district, PR, type_bra, limite_vitesse, mise_au_neutre)
			values (@vl_numero_eqt, @vl_district_eqt, @vl_PR_eqt, @vl_type_bra, @vl_vitesse, XDC_FAUX)
		end

		fetch Equipement_En_Pert into	@vl_numero_eqt, @vl_PR_eqt, @vl_district_eqt, @vl_type_bra
	end

	close Equipement_En_Pert

	select	distinct numero_eqt, district, PR, type_bra, limite_vitesse, mise_au_neutre
	from	#TMP_EQT
	order by (PR * @vl_ordre_PR_chaussee) asc		-- dans le sens de circulation

	return XDC_OK

go
