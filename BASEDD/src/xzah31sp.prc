/*E*/
/*  Fichier : $Id: xzah31sp.prc,v 1.1 1996/12/23 14:55:40 gaborit Exp $        Release : $Revision: 1.1 $        Date : $Date: 1996/12/23 14:55:40 $
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
* C.T.  11/12/96        : Cr≈ation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Prepare l'animation IHM (escargot)
* 
* Sequence d'appel
* SP	XZAH31_Animer_Escargot
* 
* Arguments en entree
* XDY_Evt		va_NumEvt_in
* XDY_Entier		va_Longueur_in
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

create procedure  XZAH;31
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_Longueur_in		int		= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_PR_in		T_PR		= null,
	@va_Sens_in		T_SENS		= null,
	@va_Suppr_in		bit
as

/*A declaration des variables */

	declare	@vl_PRTete	int,
		@vl_PR		int,
		@vl_Niveau	tinyint

/*A Test des param…tres d'appel */

/*B	Test des param…tres nuls */

	if	@va_NumEvt_in=null or
		@va_CleEvt_in=null
	return XDC_NOK

/*A S'il s'agit d'une suppression, supprime les escargots,
    et les escargots invers≈s */

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

/*A ram…ne le bouchon sur section courante */

	if	@va_Sens_in=XDC_SENS_SORTIE_SUD or
		@va_Sens_in=XDC_SENS_ENTREE_SUD
	select	@va_Sens_in=XDC_SENS_SUD

	if	@va_Sens_in=XDC_SENS_SORTIE_NORD or
		@va_Sens_in=XDC_SENS_ENTREE_NORD
	select	@va_Sens_in=XDC_SENS_NORD

/*A verifie que le bouchon n'est pas compl…tement sur ≈changeur ou avec un sens inconnu */

	if	@va_Longueur_in=0 or @va_Sens_in=XDC_SENS_INCONNU
	return XDC_OK

/*A si le sens est nord (ou sud sur A57), inverse les PR de d≈but et de fin pour avoir debut<fin */

	if	(@va_Sens_in = XDC_SENS_NORD) or 
		(@va_Sens_in = XDC_SENS_SUD and exists ( select * from CFG..RES_AUT_INV where numero = @va_Autoroute_in and inversee < 0))
	begin
		select @vl_PRTete=@va_PR_in
		select @va_PR_in=@va_PR_in - @va_Longueur_in
	end
	else	select @vl_PRTete=@va_PR_in + @va_Longueur_in

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
		XZAEC_BOU_Escargot

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
		XZAEC_BOU_Escargot

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
		XZAEC_BOU_Escargot

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
		XZAEC_BOU_Escargot

	from	CFG..RES_POR

	where	autoroute=@va_Autoroute_in and
		PR_debut<@vl_PRTete and
		PR_fin>@vl_PRTete and
		not (	PR_debut<@va_PR_in and
			PR_fin>@va_PR_in )

		

/*A puis bouchon invers≈ en cas basculement */

	exec PRC..XZAH;19 @va_NumEvt_in, @va_CleEvt_in, XZAEC_FMC_Escargot, XZAEC_BOU_Escargot,
		@va_Autoroute_in, @va_PR_in, @vl_PRTete, @va_Sens_in,
		null, null, null, null, null, null, null, null, null, null, @va_Suppr_in

	return XDC_OK
go
