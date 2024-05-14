/*  Fichier : xzah14sp.prc
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzah14sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.  02/11/94        : Cr≈ation
* C.T.  09/12/94        : Ajout appel a XZAH;19 (bouchon sur voie inverse)
*                         (V 1.2)
* B.G.  22/12/94        : modif pb having + insertion null
* B.G.	24/10/95	: gestion du sens inverse A57 (1.4)
* C.T.  11/12/96        : Modif appel a XZAH;19 (FMC7) (1.5)
* JPL	15/06/23	: Securisation dans une transaction de la lecture du PR de tete d'un bouchon (SAE_491)  1.6
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (bouchon)
* 
* Sequence d'appel
* SP	XZAH14_Animer_Bouchon
* 
* Arguments en entree
* XDY_Evt		va_NumEvt_in
* XDY_Booleen		va_TeteQueue_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Octet		va_Suppr_in
* 
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: argument invalide
* 
* Conditions d'utilisation
* 
* appel≈ sur trigger par fmc_his_iu (fmc_his.trg)
------------------------------------------------------*/

use PRC
go

create procedure  XZAH;14
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_TeteQueue_in	bit,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_Suppr_in		bit
as

/*A declaration des variables */

	declare	@vl_PRTete	int,
		@vl_NumTete	int,
		@vl_CleTete	int,
		@vl_PR		int,
		@vl_Niveau	tinyint

/*A Test des param…tres d'appel */

/*B	Test des param…tres nuls */

	if	@va_NumEvt_in=null or
		@va_CleEvt_in=null
	return XDC_NOK

/*A S'il s'agit d'une suppression, supprime les bouchons,
    et les bouchons invers≈s */

	update	EXP..SYN_BOU_POR
	set	ordre=XDC_SUPPR
	where	evenement=@va_NumEvt_in and
		cle=@va_CleEvt_in

	update	EXP..SYN_BAS_SEG
	set	ordre=XDC_SUPPR
	where	evenement_bou=@va_NumEvt_in and
		cle_bou=@va_CleEvt_in

	if @va_Suppr_in=XDC_VRAI
		return XDC_OK

/*A Test des param…tres d'appel (suite)*/

/*B	Test des param…tres nuls */

	if	@va_Autoroute_in=null or
		@va_PR_in=null or
		@va_Sens_in=null
	return XDC_NOK

/*A s'il s'agit d'une t¡te, lecture de la queue */

	if	@va_TeteQueue_in=XZAHC_Tete
	begin
		select	@vl_NumTete=@va_NumEvt_in,
			@vl_CleTete=@va_CleEvt_in,
			@vl_PRTete=@va_PR_in

/*B	lecture du numero de la queue */

		select	@va_NumEvt_in=numero,
			@va_CleEvt_in=cle
		from	EXP..FMC_GEN
		where	cause=@vl_NumTete and
			cle_cause=@vl_CleTete and
			type=XZAEC_FMC_QueueBouchon
		
		if @@rowcount<>1 return XDC_NOK -- bouchon sans queue
		
/*B	lecture du PR de la queue */

		select	@va_PR_in=PR
		from	EXP..FMC_HIS
		where	numero=@va_NumEvt_in and
			cle=@va_CleEvt_in
		having	horodate_validation=max(horodate_validation) and
			numero=@va_NumEvt_in and
			cle=@va_CleEvt_in
		
		if @@rowcount<>1 return XDC_NOK -- historique invalide
	end

/*A s'il s'agit d'une queue, lecture de la t¡te */

	else
	begin

/*B	lecture du numero de la tete */

		select  @vl_NumTete=cause,
			@vl_CleTete=cle_cause
		from	EXP..FMC_GEN
		where	numero=@va_NumEvt_in and
			cle=@va_CleEvt_in
		
		if @@rowcount<>1 return XDC_NOK -- bouchon sans tete
		
/*B	lecture du PR de la tete */

		begin transaction

		select	@vl_PRTete=PR
		from	EXP..FMC_HIS
		where	numero=@vl_NumTete and
			cle=@vl_CleTete
		having	horodate_validation=max(horodate_validation) and
			numero=@vl_NumTete and
			cle=@vl_CleTete

		commit transaction

		if @vl_PRTete is null return XDC_NOK -- historique invalide
	end

/*A lecture du niveau de bouchon */

	select	@vl_Niveau=isnull(type,1)
	from	EXP..FMC_BOU
	where	numero=@va_NumEvt_in and
		cle=@va_CleEvt_in
	having	horodate=max(horodate) and
		numero=@va_NumEvt_in and
		cle=@va_CleEvt_in


	if @@rowcount<>1 return XDC_NOK -- historique bouchon invalide

/*A ram…ne le bouchon sur section courante */

	if	@va_Sens_in=XDC_SENS_SORTIE_SUD or
		@va_Sens_in=XDC_SENS_ENTREE_SUD
	select	@va_Sens_in=XDC_SENS_SUD

	if	@va_Sens_in=XDC_SENS_SORTIE_NORD or
		@va_Sens_in=XDC_SENS_ENTREE_NORD
	select	@va_Sens_in=XDC_SENS_NORD

/*A verifie que le bouchon n'est pas compl…tement sur ≈changeur ou avec un sens inconnu */

	if	@va_PR_in=@vl_PRTete or @va_Sens_in=XDC_SENS_INCONNU
	return XDC_OK

/*A si le sens est nord (ou sud sur A57), inverse les PR de d≈but et de fin pour avoir debut<fin */

	if	@va_PR_in>@vl_PRTete
	begin
		select @vl_PR=@va_PR_in
		select @va_PR_in=@vl_PRTete
		select @vl_PRTete=@vl_PR
	end

/*A recherche des portions compl…tement recouvertes par le bouchon */

	if exists (	select * from CFG..RES_POR
			where	autoroute=@va_Autoroute_in and
				PR_debut>=@va_PR_in and
				PR_fin<=@vl_PRTete )

	insert EXP..SYN_BOU_POR
		(
		evenement,
		cle,
		portion,
		ordre,
		sens,
		translation,
		extension,
		niveau
		)
	select	@va_NumEvt_in,
		@va_CleEvt_in,
		numero,
		XDC_AJOUT,
		@va_Sens_in,
		5000,
		10000,
		@vl_Niveau

	from	CFG..RES_POR

	where	autoroute=@va_Autoroute_in and
		PR_debut>=@va_PR_in and
		PR_fin<=@vl_PRTete

/*A recherche des portions recouvrant compl…tement le bouchon */

	if exists (	select * from CFG..RES_POR
			where	autoroute=@va_Autoroute_in and
				PR_debut<@va_PR_in and
				PR_fin>@vl_PRTete )

	insert EXP..SYN_BOU_POR
		(
		evenement,
		cle,
		portion,
		ordre,
		sens,
		translation,
		extension,
		niveau
		)
	select	@va_NumEvt_in,
		@va_CleEvt_in,
		numero,
		XDC_AJOUT,
		@va_Sens_in,
		10000*((@va_PR_in+@vl_PRTete)/2-PR_debut)/(PR_fin-PR_debut),
		10000*(@vl_PRTete-@va_PR_in)/(PR_fin-PR_debut),
		@vl_Niveau

	from	CFG..RES_POR

	where	autoroute=@va_Autoroute_in and
		PR_debut<@va_PR_in and
		PR_fin>@vl_PRTete

/*A recherche des portions extremes comportant un bout de bouchon */

	if exists (	select * from CFG..RES_POR
			where	autoroute=@va_Autoroute_in and
				PR_debut<@va_PR_in and
				PR_fin>@va_PR_in and
				not (	PR_debut<@vl_PRTete and
					PR_fin>@vl_PRTete ))

	insert EXP..SYN_BOU_POR
		(
		evenement,
		cle,
		portion,
		ordre,
		sens,
		translation,
		extension,
		niveau
		)
	select	@va_NumEvt_in,
		@va_CleEvt_in,
		numero,
		XDC_AJOUT,
		@va_Sens_in,
		10000*((@va_PR_in+PR_fin)/2-PR_debut)/(PR_fin-PR_debut),
		10000*(PR_fin-@va_PR_in)/(PR_fin-PR_debut),
		@vl_Niveau

	from	CFG..RES_POR

	where	autoroute=@va_Autoroute_in and
		PR_debut<@va_PR_in and
		PR_fin>@va_PR_in and
		not (	PR_debut<@vl_PRTete and
			PR_fin>@vl_PRTete )

	if exists (	select * from CFG..RES_POR
			where	autoroute=@va_Autoroute_in and
				PR_debut<@vl_PRTete and
				PR_fin>@vl_PRTete and
				not (	PR_debut<@va_PR_in and
					PR_fin>@va_PR_in ))

	insert EXP..SYN_BOU_POR
		(
		evenement,
		cle,
		portion,
		ordre,
		sens,
		translation,
		extension,
		niveau
		)
	select	@va_NumEvt_in,
		@va_CleEvt_in,
		numero,
		XDC_AJOUT,
		@va_Sens_in,
		10000*((@vl_PRTete+PR_debut)/2-PR_debut)/(PR_fin-PR_debut),
		10000*(@vl_PRTete-PR_debut)/(PR_fin-PR_debut),
		@vl_Niveau

	from	CFG..RES_POR

	where	autoroute=@va_Autoroute_in and
		PR_debut<@vl_PRTete and
		PR_fin>@vl_PRTete and
		not (	PR_debut<@va_PR_in and
			PR_fin>@va_PR_in )

		

/*A puis bouchon invers≈ en cas basculement */

	exec PRC..XZAH;19 @va_NumEvt_in, @va_CleEvt_in, XZAEC_FMC_QueueBouchon, @vl_Niveau,
		@va_Autoroute_in, @va_PR_in, @vl_PRTete, @va_Sens_in,
		null, null, null, null, null, null, null, null, null, null, @va_Suppr_in

	return XDC_OK
go
