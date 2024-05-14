/*E*/
/*  Fichier : $Id: xzaf05sp.prc,v 1.4 1995/11/02 11:49:29 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/11/02 11:49:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf05sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm pour la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	11/01/95	: Creation 
* C.T.	02/02/95	: Modif enleve les queue de bouchon dans la 
*                         selection (V 1.2)
* C.T.	30/05/95	: Correction PR des bouchons (V 1.3)
* B.G.	30/10/95	: gestion du sens inverse A57 (1.4)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAF05_Animer_Bouchon
* 
* Arguments en entree
* 
* Arguments en sortie
* 
* Liste retournee :
* XDY_Entier	evenement
* XDY_Octet 	cle
* XDY_Mot	portion
* XDY_Octet 	sens
* XDY_Entier	translation
* XDY_Entier	extension
* XDY_Octet	niveau
*
* Service rendu
* Retourne la liste des bouchons pour
* l'animation IHM pour la formation
*
* Code retour
* XDC_OK
* XDC_NOK	
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
*
---------------------------------------------------*/

use PRC
go

create procedure  XZAF;05
as
/*A D≈claration des variables locales */
declare @vl_NumEvt		int,
	@vl_CleEvt		tinyint,
	@vl_NumTete		int,
	@vl_CleTete		tinyint,
	@vl_Type		smallint,
	@vl_Horodate		datetime,
	@vl_Autoroute		T_AUTOROUTE,
	@vl_PR			T_PR,
	@vl_PRTete		T_PR,
	@vl_PRInter		T_PR,
	@vl_Sens		T_SENS,
	@vl_Niveau		tinyint

	create table #SYN_BOU_POR
	(
		evenement		int,
		cle			tinyint,
		portion			smallint,
		sens			tinyint,
		translation		int,
		extension		int,
		niveau			tinyint	null
	)

	/*A Recherche des evenements en cours a la date precisee */
	declare Pointeur_evt cursor
	for select 
		numero,
		cle,
		type,
		autoroute,
		PR,
		sens,
		horodate_validation
	from EVT_FRM
	where	spid = @@spid and 
		type = XZAEC_FMC_TeteBouchon  and
		sens != XDC_SENS_INCONNU

	open Pointeur_evt
	fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type,
			@vl_Autoroute, @vl_PR, @vl_Sens, @vl_Horodate
	while @@sqlstatus = 0
	begin
		/*B Test des informations nulles */
		if @vl_Autoroute=null or @vl_PR=null or
		   @vl_Sens=null 
			return XDC_NOK
	

		/*B lecture de la queue */
		select	@vl_NumTete=@vl_NumEvt,
			@vl_CleTete=@vl_CleEvt,
			@vl_PRTete=@vl_PR
	
		/*	lecture du numero de la queue */
	
		select	@vl_NumEvt=numero,
			@vl_CleEvt=cle
		from	HIS..FMC_GEN
		where	cause=@vl_NumTete and
			cle_cause=@vl_CleTete and
			type=XZAEC_FMC_QueueBouchon
			
		if @@rowcount<>1 return XDC_NOK -- bouchon sans queue
			
		/*	lecture du PR de la queue */
	
		select	@vl_PR=PR
		from	EVT_FRM
		where	numero=@vl_NumEvt and
			cle=@vl_CleEvt
			
		if @@rowcount<>1 return XDC_NOK -- historique invalide
	
	
		/*B lecture du niveau de bouchon */
	
		select	@vl_Niveau=isnull(type,1)
		from	HIS..FMC_BOU
		where	numero=@vl_NumEvt and
			cle=@vl_CleEvt and
			horodate = @vl_Horodate
	
		if @@rowcount<>1 return XDC_NOK -- historique bouchon invalide
	
		/*B ram…ne le bouchon sur section courante */
	
		if @vl_Sens=XDC_SENS_SORTIE_SUD or
		   @vl_Sens=XDC_SENS_ENTREE_SUD
			select	@vl_Sens=XDC_SENS_SUD
	
		if @vl_Sens=XDC_SENS_SORTIE_NORD or
		   @vl_Sens=XDC_SENS_ENTREE_NORD
			select	@vl_Sens=XDC_SENS_NORD
	
		/*B verifie que le bouchon n'est pas compl…tement sur ≈changeur ou avec un sens inconnu */
	
		if @vl_PR=@vl_PRTete or @vl_Sens=XDC_SENS_INCONNU
			return XDC_OK
	
		/*B si le sens est nord ou sur A57, inverse les PR de d≈but et de fin pour avoir debut<fin */
	
		if @vl_PR>@vl_PRTete
		begin
			select @vl_PRInter=@vl_PR
			select @vl_PR=@vl_PRTete
			select @vl_PRTete=@vl_PRInter
		end
	
		/*B recherche des portions compl…tement recouvertes par le bouchon */
	
		if exists (	select * from CFG..RES_POR
				where	autoroute=@vl_Autoroute and
					PR_debut>=@vl_PR and
					PR_fin<=@vl_PRTete )
	
		insert #SYN_BOU_POR
			(
			evenement,
			cle,
			portion,
			sens,
			translation,
			extension,
			niveau
			)
		select	@vl_NumEvt,
			@vl_CleEvt,
			numero,
			@vl_Sens,
			5000,
			10000,
			@vl_Niveau
		from	CFG..RES_POR
		where	autoroute=@vl_Autoroute and
			PR_debut>=@vl_PR and
			PR_fin<=@vl_PRTete
	
		/*B recherche des portions recouvrant compl…tement le bouchon */
	
		if exists (	select * from CFG..RES_POR
				where	autoroute=@vl_Autoroute and
					PR_debut<@vl_PR and
					PR_fin>@vl_PRTete )
	
		insert #SYN_BOU_POR
			(
			evenement,
			cle,
			portion,
			sens,
			translation,
			extension,
			niveau
			)
		select	@vl_NumEvt,
			@vl_CleEvt,
			numero,
			@vl_Sens,
			10000*((@vl_PR+@vl_PRTete)/2-PR_debut)/(PR_fin-PR_debut),
			10000*(@vl_PRTete-@vl_PR)/(PR_fin-PR_debut),
			@vl_Niveau
		from	CFG..RES_POR
		where	autoroute=@vl_Autoroute and
			PR_debut<@vl_PR and
			PR_fin>@vl_PRTete
	
		/*B recherche des portions extremes comportant un bout de bouchon */
	
		if exists (	select * from CFG..RES_POR
				where	autoroute=@vl_Autoroute and
					PR_debut<@vl_PR and
					PR_fin>@vl_PR and
					not (	PR_debut<@vl_PRTete and
						PR_fin>@vl_PRTete ))
	
		insert #SYN_BOU_POR
			(
			evenement,
			cle,
			portion,
			sens,
			translation,
			extension,
			niveau
			)
		select	@vl_NumEvt,
			@vl_CleEvt,
			numero,
			@vl_Sens,
			10000*((@vl_PR+PR_fin)/2-PR_debut)/(PR_fin-PR_debut),
			10000*(PR_fin-@vl_PR)/(PR_fin-PR_debut),
			@vl_Niveau
		from	CFG..RES_POR
		where	autoroute=@vl_Autoroute and
			PR_debut<@vl_PR and
			PR_fin>@vl_PR and
			not (	PR_debut<@vl_PRTete and
				PR_fin>@vl_PRTete )
	
		if exists (	select * from CFG..RES_POR
				where	autoroute=@vl_Autoroute and
					PR_debut<@vl_PRTete and
					PR_fin>@vl_PRTete and
					not (	PR_debut<@vl_PR and
						PR_fin>@vl_PR ))
	
		insert #SYN_BOU_POR
			(
			evenement,
			cle,
			portion,
			sens,
			translation,
			extension,
			niveau
			)
		select	@vl_NumEvt,
			@vl_CleEvt,
			numero,
			@vl_Sens,
			10000*((@vl_PRTete+PR_debut)/2-PR_debut)/(PR_fin-PR_debut),
			10000*(@vl_PRTete-PR_debut)/(PR_fin-PR_debut),
			@vl_Niveau
		from	CFG..RES_POR
		where	autoroute=@vl_Autoroute and
			PR_debut<@vl_PRTete and
			PR_fin>@vl_PRTete and
			not (	PR_debut<@vl_PR and
				PR_fin>@vl_PR )

		
		/* passage a l'evenement suivant */
		fetch Pointeur_evt into @vl_NumEvt, @vl_CleEvt, @vl_Type,
			@vl_Autoroute, @vl_PR, @vl_Sens
	end
	close Pointeur_evt

	/* retourner la liste des bouchons */
	select 
		evenement,
		cle,
		portion,
		sens,
		translation,
		extension,
		niveau
	from #SYN_BOU_POR

	return XDC_OK
go
