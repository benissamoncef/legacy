/*E*/
/*  Fichier : $Id: xzae21sp.prc,v 1.27 2020/11/12 09:27:16 devgtie Exp $      Release : $Revision: 1.27 $        Date : $Date: 2020/11/12 09:27:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae21sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courantes 
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.  08/11/94        : Creation (v1.1)
* B.G.  22/11/94        : Correction (v1.2)
* B.G.	24/10/95	: gestion du sens inverse A57 (1.3)
* C.T.	22/11/95	: Ajout en sortie du site correspondant a la localisation donnee (1.4)
* C.T.	04/04/96	: Rechercher l'echangeur complet dans le cas de sens inconnu (1.5) (795)
* JMG	12/12/97	: ajout PR bifurc amont et aval en sortie (dem/1534) 1.6
* JMG	29/12/97	: ajout des PR debut des bifurcations en sortie (dem/1534) 1.7
* C.T.	23/02/98	: Correction recherche de bifurcation (dem/1534) 1.8
* JMG	05/03/98	: correction recherche tunnel (dem/1558) 1.9
* PN	07 Jul 1998	: modif separation EXP/HIS (1696) V1.10
* JMG	16/05/00	: memoriser la portion de l'evt pour les recherches 1.11
* PNI	29/11/10	: Appel de XZAO;17 avec sens corrigÈ, suppression du ; v1.12 DEM963
* JPL	01/02/12	: Calcul distances bifurcation a la fin; idem precedente  1.13
* JPL	01/02/12	: Recherche bifurcations dans RES_ECH (sortie en sens evenement) (DEM 1011)  1.14
* JPL	08/02/12	: Retour a la recherche bifurcations par portions (config. incomplete) (DEM 1011)  1.15
* JPL	08/02/12	: Bifurcations: recherche autoroutes et PR debut correct (DEM 1011)  1.16
* JPL	09/02/12	: Bifurcations sans bretelle de sortie ignorees (dans le code, absence config.) (DEM 1011)  1.17
* JMG	18/04/12	: correction param de sortie texte pour client V15 linux 1.18
* JPL	25/06/12	: Retour numeros echangeur bruts + noms sorties usager, indicateurs bifurcation (DEM 1033)  1.19
* PNI	05/09/2013	: Correction de la recherche de la portion et district (DEM1080) v1.20
* PNI	19/09/2013	: Correction pour calcul de l'autoroute de bifurcation (DEM1084) v1.21
* PNI	30/09/2013	: Correction pour calcul de l'autoroute de bifurcation (DEM1084) v1.22
* PNI	27/06/2016	: Suppression du cas particulier des bif sur A57 DEM1176 v1.23
* LCL	15/11/2016	: Ajout localisation en viaduc (LOT23) v1.24
* JMG	26/06/2017 	: convergence zone urbaine 1.25
* PNI	03/04/2019	: Suppression du cas particulier des bif sur A57 DEM1176 v1.26
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Localise un point par rapport aux sorties qui l'encadrent
* 
* -------------------------------------------------------
* Sequence d'appel
* SP	XZAE21_Localiser_Evt_PA
* 
* Arguments en entree
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* 
* Arguments en sortie
* XDY_Octet		va_NumSortieAmont_out
* XDY_Nom		va_NomSortieAm_out
* XDY_Abrev		va_AbrevSortieAm_out
* XDY_Sortie		va_SortieAmUsager_out
* XDY_Octet		va_SortieAmontEstBifurc_out
* XDY_Entier		va_DistanceSortieAmont_out
* XDY_Octet		va_NumSortieAval_out
* XDY_Nom		va_NomSortieAv_out
* XDY_Abrev		va_AbrevSortieAv_out
* XDY_Sortie		va_SortieAvUsager_out
* XDY_Octet		va_SortieAvalEstBifurc_out
* XDY_Entier		va_DistanceSortieAval_out
* XDY_Octet		va_NumPeageAmont_out
* XDY_Nom		va_NomPeageAm_out
* XDY_Abrev		va_AbrevPeageAm_out
* XDY_Entier		va_DistancePeageAmont_out
* XDY_Octet		va_NumPeageeAv_out
* XDY_Nom		va_NomPeageAval_out
* XDY_Abrev		va_AbrevPeageAval_out
* XDY_Entier		va_DistancePeageAval_out
* XDY_Entier		va_LongueurPlateforme_out
* XDY_Autoroute		va_NumAutorouteBifAmont_out
* XDY_Entier		va_DistanceBifAmont_out
* XDY_Autoroute		va_NumAutorouteBifAval_out
* XDY_Entier		va_DistanceBifAval_out
* XDY_Nom		va_Descente_out
* XDY_Mot		va_Pente_out
* XDY_Nom		va_Tunnel_out
* XDY_Site		va_Site_out
* XDY_PR		va_PrBifAmont_out 
* XDY_PR		va_PrBifAval_out 
* XDY_PR		va_PrDebut_BifAmont_out
* XDY_PR		va_PrDebut_BifAval_out
* XDY_Nom		va_Viaduc_out
*
* Code retour
* XDC_OK
* XDC_NOK
* <0 erreur sybase 
* 
* Conditions d'utilisation
* 
---------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE21' and type = 'P')
	drop procedure XZAE21
go


create procedure XZAE21
	@va_Autoroute_in			T_AUTOROUTE = null,
	@va_PR_in				T_PR = null,
	@va_Sens_in				tinyint = null,
	@va_NumSortieAmont_out			tinyint = null output,
	@va_DistanceSortieAmont_out		int = null output,
	@va_SortieAmontEstBifurc_out		tinyint = null output,
	@va_NumSortieAval_out			tinyint = null output,
	@va_DistanceSortieAval_out		int = null output,
	@va_SortieAvalEstBifurc_out		tinyint = null output,
	@va_NumPeageAmont_out			tinyint = null output,
	@va_DistancePeageAmont_out		int = null output,
	@va_NumPeageAval_out			tinyint = null output,
	@va_DistancePeageAval_out		int = null output,
	@va_LongueurPlateforme_out		int = null output,
	@va_NumAutorouteBifAmont_out		T_AUTOROUTE = null output,
	@va_DistanceBifAmont_out		int = null output,
	@va_NumAutorouteBifAval_out		T_AUTOROUTE = null output,
	@va_DistanceBifAval_out			int = null output,
	@va_Descente_out			char(25) = XDC_CHAINE_VIDE output,
	@va_Tunnel_out				char(25) = XDC_CHAINE_VIDE output,
	
	@va_NomSortieAm_out			char(25) = XDC_CHAINE_VIDE output,
	@va_AbrevSortieAm_out			char(15) = XDC_CHAINE_VIDE output,
	@va_SortieAmUsager_out			char(20) = XDC_CHAINE_VIDE output,
	@va_NomSortieAv_out			char(25) = XDC_CHAINE_VIDE output,
	@va_AbrevSortieAv_out			char(15) = XDC_CHAINE_VIDE output,
	@va_SortieAvUsager_out			char(20) = XDC_CHAINE_VIDE output,
	@va_NomPeageAm_out			char(25) = XDC_CHAINE_VIDE output,
	@va_AbrevPeageAm_out			char(15) = XDC_CHAINE_VIDE output,
	@va_NomPeageAval_out			char(25) = XDC_CHAINE_VIDE output,
	@va_AbrevPeageAval_out			char(15) = XDC_CHAINE_VIDE output,
	@va_Pente_out				smallint = null output,
	@va_Site_out				T_SITE = null output,
	@va_PrBifAmont_out			T_PR = null output,
	@va_PrBifAval_out			T_PR = null output,
	@va_PrDebut_BifAmont_out		T_PR = null output,
	@va_PrDebut_BifAval_out			T_PR = null output,
	@va_Viaduc_out				char(25) = XDC_CHAINE_VIDE output,
	@va_ZoneUrbaine_out			smallint = null output
as

/*B Positionnement des valeurs par defaut des parametres de sortie */

	select	@va_Descente_out	= XDC_CHAINE_VIDE,
		@va_Tunnel_out		= XDC_CHAINE_VIDE,
		@va_NomSortieAm_out	= XDC_CHAINE_VIDE,
		@va_AbrevSortieAm_out	= XDC_CHAINE_VIDE,
		@va_SortieAmUsager_out	= XDC_CHAINE_VIDE,
		@va_NomSortieAv_out	= XDC_CHAINE_VIDE,
		@va_AbrevSortieAv_out	= XDC_CHAINE_VIDE,
		@va_SortieAvUsager_out	= XDC_CHAINE_VIDE,
		@va_NomPeageAm_out	= XDC_CHAINE_VIDE,
		@va_AbrevPeageAm_out	= XDC_CHAINE_VIDE,
		@va_NomPeageAval_out	= XDC_CHAINE_VIDE,
		@va_AbrevPeageAval_out	= XDC_CHAINE_VIDE,
		@va_Viaduc_out		= XDC_CHAINE_VIDE

	select	@va_SortieAmontEstBifurc_out	= null,
		@va_SortieAvalEstBifurc_out	= null,
		@va_NumAutorouteBifAmont_out	= null,
		@va_DistanceBifAmont_out	= null,
		@va_NumAutorouteBifAval_out	= null,
		@va_DistanceBifAval_out		= null,
		@va_PrBifAmont_out		= null,
		@va_PrBifAval_out		= null,
		@va_PrDebut_BifAmont_out	= null,
		@va_PrDebut_BifAval_out		= null,
		@va_ZoneUrbaine_out		= null


/*A Declaration des variables locales */

	declare	@vl_cr int,
		@vl_Sens		T_SENS,
		@vl_ordre_PR		smallint,
		@vl_portionEvt		smallint,
		@vl_portionBifAmont	smallint,
		@vl_portionBifAval	smallint,
		@vl_extremiteAmont	bit,
		@vl_extremiteAval	bit,
		@vl_PR1			T_PR,
		@vl_PR2			T_PR,
		@vl_long1		int,
		@vl_long2		int,
		@vl_NumPea		tinyint,
		@vl_DisPea		int,
		@vl_NumAut		T_AUTOROUTE,
		@vl_DisBif		int,
		@vl_NomPea		char(25),
		@vl_AbrPea		char(15),

		@vl_OK			bit,
		@vl_portion		smallint,
		@vl_portion1		smallint,
		@vl_portion2		smallint,
		@vl_prbifamont		T_PR,
		@vl_prbifaval		T_PR,
		@vl_prdebut_bifamont	T_PR,
		@vl_prdebut_bifaval	T_PR,
		@vl_viaduc		char(25)


/*A Controle des parametres d'entree obligatoire */

	if	@va_Autoroute_in = null or
		@va_PR_in = null or
		@va_Sens_in = null

	return XDC_ARG_INV

	select	@vl_ordre_PR = inversee
	from	CFG..RES_AUT_INV
	where	numero = @va_Autoroute_in

	if @@rowcount<>1
		return XDC_NOK


/*A Determiner le sens de recherche, ramenÈ sur section courante s'il s'agit d'une bretelle */
/*A Si le sens est inconnu : recherche en sens Nord pour peage, sorties, bifurcations, tunnels */

	select	@vl_Sens = @va_Sens_in

	if (@va_Sens_in=XDC_SENS_SORTIE_NORD) or (@va_Sens_in=XDC_SENS_ENTREE_NORD)
		select	@vl_Sens = XDC_SENS_NORD
	else
	if (@va_Sens_in=XDC_SENS_SORTIE_SUD) or (@va_Sens_in=XDC_SENS_ENTREE_SUD)
		select	@vl_Sens = XDC_SENS_SUD
	else
	if (@va_Sens_in=XDC_SENS_INCONNU)
		select	@vl_Sens = XDC_SENS_NORD

	if @vl_Sens not in (XDC_SENS_NORD, XDC_SENS_SUD)
		return XDC_NOK


/*A Recherche la portion et le site correspondant au PR */

	select 	@vl_portionEvt=numero,
		@va_Site_out = district
	from 	CFG..RES_POR
	where 	autoroute=@va_Autoroute_in and
		PR_debut<=@va_PR_in and
		PR_fin>@va_PR_in


	if @@rowcount = 0 
	begin
		select 	@vl_portionEvt=numero,
			@va_Site_out = district
		from 	CFG..RES_POR
		where 	autoroute=@va_Autoroute_in and
			PR_debut<@va_PR_in and
			PR_fin>=@va_PR_in
	
		if @@rowcount = 0 return XDC_NOK
	end


/*A Recherche la pente correspondant au PR */

	select	@va_Descente_out=isnull(nom,XDC_CHAINE_VIDE),
		@va_Pente_out=pente
	from	CFG..RES_PNT
	where	portion=@vl_portionEvt and
		PR_debut<@va_PR_in and
		PR_fin>=@va_PR_in


/*A Recherche le tunnel correspondant au PR */

	select	@va_Tunnel_out=isnull(nom_tunnel,XDC_CHAINE_VIDE)
	from	CFG..RES_SEG
	where	portion=@vl_portionEvt and
		PR_debut<=@va_PR_in and
		sens = @vl_Sens
	having	PR_debut=max(PR_debut) and
		portion=@vl_portionEvt and
		PR_debut<=@va_PR_in and
		tunnel=XDC_OUI and
		sens = @vl_Sens

/*A Recherche le viaduc correspondant au PR */

	select	@va_Viaduc_out=isnull(nom,XDC_CHAINE_VIDE)
	from	CFG..RES_VIA
	where   PRdebut<=@va_PR_in  and
		autoroute = @va_Autoroute_in
		and PRfin >= @va_PR_in

	select @va_ZoneUrbaine_out = count(*) 
	from CFG..ZON_URB
	where debut <=@va_PR_in  and
	autoroute = @va_Autoroute_in
	and fin >= @va_PR_in
	and sens = @vl_Sens

/*A Recherche le peage amont */

	select	@va_NumPeageAmont_out=PEA.numero,
		@va_NomPeageAm_out=isnull(PEA.nom,XDC_CHAINE_VIDE),
		@va_AbrevPeageAm_out=isnull(PEA.abreviation,XDC_CHAINE_VIDE),
		@vl_PR1=PEA.PRsens1,
		@vl_PR2=PEA.PRsens2,
		@vl_long1=PEA.longueur_sens1,
		@vl_long2=PEA.longueur_sens2
	from	CFG..RES_PEA PEA,
		CFG..RES_POR POR
	where	PEA.portion=POR.numero and
		POR.autoroute=@va_Autoroute_in and
		PEA.supprime=0 and
		( @vl_Sens<>XDC_SENS_SUD or
			(PEA.longueur_sens1<>0 and PEA.PRsens1<@va_PR_in)) and
		( @vl_Sens<>XDC_SENS_NORD or
			(PEA.longueur_sens2<>0 and PEA.PRsens2>@va_PR_in))
	having	( @vl_Sens<>XDC_SENS_SUD or PEA.PRsens1=max(PEA.PRsens1)) and
		( @vl_Sens<>XDC_SENS_NORD or PEA.PRsens2=min(PEA.PRsens2))
	
	if @vl_Sens=XDC_SENS_NORD
	select  @vl_PR1=@vl_PR2,
		@vl_long1=@vl_long2
	

	/* on a trouv≈ un p≈age */
	if @vl_PR1<>null
	begin
		exec @vl_cr=XZAO;22	@va_Autoroute_in,
					@va_PR_in,
					@va_Autoroute_in,
					@vl_PR1,
					@va_DistancePeageAmont_out output
	
		if @vl_cr<>XDC_OK return XDC_NOK
	end


/*A Recherche le peage aval */

	select	@va_NumPeageAval_out=PEA.numero,
		@va_NomPeageAval_out=isnull(PEA.nom,XDC_CHAINE_VIDE),
		@va_AbrevPeageAval_out=isnull(PEA.abreviation,XDC_CHAINE_VIDE),
		@vl_PR1=PEA.PRsens1,
		@vl_PR2=PEA.PRsens2,
		@va_LongueurPlateforme_out=PEA.longueur_sens1,
		@vl_long2=PEA.longueur_sens2
	from	CFG..RES_PEA PEA,
		CFG..RES_POR POR
	where	PEA.portion=POR.numero and
		POR.autoroute=@va_Autoroute_in and
		PEA.supprime=0 and
		( @vl_Sens<>XDC_SENS_SUD or
			(PEA.longueur_sens1<>0 and PEA.PRsens1>=@va_PR_in)) and
		( @vl_Sens<>XDC_SENS_NORD or
			(PEA.longueur_sens2<>0 and PEA.PRsens2<=@va_PR_in))
	having	( @vl_Sens<>XDC_SENS_SUD or PEA.PRsens1=min(PEA.PRsens1)) and
		( @vl_Sens<>XDC_SENS_NORD or PEA.PRsens2=max(PEA.PRsens2))
	
	if @vl_Sens=XDC_SENS_NORD
	select	@vl_PR1=@vl_PR2,
		@va_LongueurPlateforme_out=@vl_long2
	
	/* on a trouv≈ un p≈age */
	if @vl_PR1<>null
	begin
		exec @vl_cr=XZAO;22	@va_Autoroute_in,
					@va_PR_in,
					@va_Autoroute_in,
					@vl_PR1,
					@va_DistancePeageAval_out output
	
		if @vl_cr<>XDC_OK return XDC_NOK
	end


/*A Recherche la bifurcation amont (sens SUD) ou aval (sens NORD) */

	select  @vl_OK=XDC_NON,
		@vl_portion2=@vl_portionEvt,
		@vl_portion1=null
	
	while @vl_OK=XDC_NON
	begin
		select	@vl_portion1=NEU.portion1
		from	CFG..RES_NEU NEU,
			CFG..RES_POR POR
		where	NEU.portion2=@vl_portion2 and
			NEU.extremite_portion_2=0 and
			NEU.portion1=POR.numero and
			POR.autoroute<>@va_Autoroute_in
		
		if @@rowcount<>0 select @vl_OK=XDC_OUI
		
		else
		begin
			select	@vl_portion=NEU.portion1
			from	CFG..RES_NEU NEU,
				CFG..RES_POR POR
			where	NEU.portion2=@vl_portion2 and
				NEU.extremite_portion_2=0 and
				NEU.portion1=POR.numero and
				POR.autoroute=@va_Autoroute_in
			
			if @@rowcount=0 select @vl_OK=XDC_OUI

			select @vl_portion2 = @vl_portion
		end	
	end
	if @vl_portion1<>null
	begin
		if @vl_Sens=XDC_SENS_SUD
		begin

			select @vl_portionBifAmont = @vl_portion2
			select @vl_extremiteAmont = XDC_FAUX
			select @vl_prbifamont=PR_debut+1
			from   CFG..RES_POR
			where  numero=@vl_portion2
		end
		else
		begin

			select @vl_portionBifAval = @vl_portion2
			select @vl_extremiteAval = XDC_FAUX
			select @vl_prbifaval=PR_debut+1
			from   CFG..RES_POR
			where  numero=@vl_portion2
		end
	end

/*A Recherche la bifurcation aval (sens SUD) ou amont (sens NORD) */

	select  @vl_OK=XDC_NON,
		@vl_portion2=@vl_portionEvt,
		@vl_portion1=null
	
	while @vl_OK=XDC_NON
	begin
		select	@vl_portion1=NEU.portion1
		from	CFG..RES_NEU NEU,
			CFG..RES_POR POR
		where	NEU.portion2=@vl_portion2 and
			NEU.extremite_portion_2=1 and
			NEU.portion1=POR.numero and
			POR.autoroute<>@va_Autoroute_in
		
		if @@rowcount<>0 select @vl_OK=XDC_OUI
		
		else
		begin
			select	@vl_portion=NEU.portion1
			from	CFG..RES_NEU NEU,
				CFG..RES_POR POR
			where	NEU.portion2=@vl_portion2 and
				NEU.extremite_portion_2=1 and
				NEU.portion1=POR.numero and
				POR.autoroute=@va_Autoroute_in
			
			if @@rowcount=0 select @vl_OK=XDC_OUI

			select @vl_portion2 = @vl_portion
		end	
	end
	if @vl_portion1<>null
	begin
		if @vl_Sens=XDC_SENS_SUD
		begin

			select @vl_portionBifAval = @vl_portion2
			select @vl_extremiteAval = XDC_VRAI
			select @vl_prbifaval=PR_fin
			from   CFG..RES_POR
			where  numero=@vl_portion2
		end
		else 
		begin

			select @vl_portionBifAmont = @vl_portion2
			select @vl_extremiteAmont = XDC_VRAI
			select @vl_prbifamont=PR_fin
			from   CFG..RES_POR
			where  numero=@vl_portion2
		end
	end

	/*A si des bifurcations sont trouvees alors determiner vers quelle autoroute, et PR de debut */
	if ( @vl_portionBifAmont <> null )
	begin
		
/*DEM1176*/
 		if ( @vl_portionBifAmont = @vl_portionEvt) and ((@va_Autoroute_in in (2,6) and @va_Sens_in=XDC_SENS_SUD) /*or (@va_Autoroute_in in (8) and  @va_Sens_in=XDC_SENS_NORD)*/)
		
		select	@va_NumAutorouteBifAmont_out = POR.autoroute,
			@vl_prdebut_bifamont =
			   case when NEU.extremite_portion_2 = 0 then POR.PR_debut else POR.PR_fin end
		from	CFG..RES_NEU NEU, CFG..RES_POR POR
		where	NEU.portion1 = @vl_portionBifAmont  and
			NEU.extremite_portion_1 = @vl_extremiteAmont  and
			POR.numero= @vl_portionEvt and
			POR.autoroute = @va_Autoroute_in

		else

		select	@va_NumAutorouteBifAmont_out = POR.autoroute,
			@vl_prdebut_bifamont =
			   case when NEU.extremite_portion_2 = 0 then POR.PR_debut else POR.PR_fin end
		from	CFG..RES_NEU NEU, CFG..RES_POR POR
		where	NEU.portion1 = @vl_portionBifAmont  and
			NEU.extremite_portion_1 = @vl_extremiteAmont  and
			NEU.portion2 = POR.numero  and
			POR.autoroute <> @va_Autoroute_in
	end

	if ( @vl_portionBifAval <> null )
	begin
/*DEM1176*/
/*if ( @vl_portionBifAval = @vl_portionEvt ) and (@va_Autoroute_in = 8)
		select	@va_NumAutorouteBifAval_out = POR.autoroute,
			@vl_prdebut_bifaval =
			   case when NEU.extremite_portion_2 = 0 then POR.PR_debut else POR.PR_fin end
		from	CFG..RES_NEU NEU, CFG..RES_POR POR
		where	NEU.portion1 = @vl_portionBifAval  and
			NEU.extremite_portion_1 = @vl_extremiteAval  and
			POR.numero= @vl_portionEvt and
			POR.autoroute = @va_Autoroute_in
		

		else
*/
		select	@va_NumAutorouteBifAval_out = POR.autoroute,
			@vl_prdebut_bifaval =
			   case when NEU.extremite_portion_2 = 0 then POR.PR_debut else POR.PR_fin end
		from	CFG..RES_NEU NEU, CFG..RES_POR POR
		where	NEU.portion1 = @vl_portionBifAval  and
			NEU.extremite_portion_1 = @vl_extremiteAval  and
			NEU.portion2 = POR.numero  and
			POR.autoroute <> @va_Autoroute_in
	end

	/*B Ignorer certaines bifurcations (sans bretelle de sortie) */
	if (@va_Autoroute_in = 1) and (@vl_Sens = XDC_SENS_SUD)
	begin
		if (@va_NumAutorouteBifAmont_out = 5)	select @va_NumAutorouteBifAmont_out = null
		if (@va_NumAutorouteBifAval_out = 5)	select @va_NumAutorouteBifAval_out = null
	end
	else
	if (@va_Autoroute_in = 1) and (@vl_Sens = XDC_SENS_NORD)
	begin
		if (@va_NumAutorouteBifAmont_out = 2)	select @va_NumAutorouteBifAmont_out = null
		if (@va_NumAutorouteBifAval_out = 2)	select @va_NumAutorouteBifAval_out = null
	end
	else
	if (@va_Autoroute_in = 3) and (@vl_Sens = XDC_SENS_SUD)
	begin
		if (@va_NumAutorouteBifAmont_out = 6)	select @va_NumAutorouteBifAmont_out = null
		if (@va_NumAutorouteBifAval_out = 6)	select @va_NumAutorouteBifAval_out = null
	end
	else
	if (@va_Autoroute_in = 6) and (@vl_Sens = XDC_SENS_SUD)
	begin
		if (@va_NumAutorouteBifAmont_out = 6)	select @va_NumAutorouteBifAmont_out = null
		if (@va_NumAutorouteBifAmont_out = 7)	select @va_NumAutorouteBifAmont_out = null
		if (@va_NumAutorouteBifAval_out = 7)	select @va_NumAutorouteBifAval_out = null
	end
	else
	if (@va_Autoroute_in = 6) and (@vl_Sens = XDC_SENS_NORD)
	begin
		if (@va_NumAutorouteBifAmont_out = 4)	select @va_NumAutorouteBifAmont_out = null
		if (@va_NumAutorouteBifAval_out = 4)	select @va_NumAutorouteBifAval_out = null
	end

	/*A si des bifurcations sont trouvees alors calculer leur distance */
	if (@va_NumAutorouteBifAmont_out <> null) and (@vl_prbifamont <> null)
	begin
		exec @vl_cr = XZAO;22	@va_Autoroute_in,
					@va_PR_in,
					@va_Autoroute_in,
					@vl_prbifamont,
					@va_DistanceBifAmont_out output
		if @vl_cr <> XDC_OK
			return XDC_NOK
	end

	if (@va_NumAutorouteBifAval_out <> null) and (@vl_prbifaval <> null)
	begin
		exec @vl_cr = XZAO;22	@va_Autoroute_in,
					@va_PR_in,
					@va_Autoroute_in,
					@vl_prbifaval,
					@va_DistanceBifAval_out output
		if @vl_cr <> XDC_OK
			return XDC_NOK
	end

	if (@va_NumAutorouteBifAmont_out <> null)
	begin
		select @va_PrBifAmont_out = @vl_prbifamont
		select @va_PrDebut_BifAmont_out = @vl_prdebut_bifamont
	end
	if (@va_NumAutorouteBifAval_out <> null)
	begin
		select @va_PrBifAval_out = @vl_prbifaval
		select @va_PrDebut_BifAval_out = @vl_prdebut_bifaval
	end


/*A recherche les sorties amont et aval */

	exec @vl_cr=XZAO17	@va_Autoroute_in,
				@va_PR_in,
				@vl_Sens,
				1,
				@va_NumSortieAmont_out output,
				@va_NomSortieAm_out output,
				@va_AbrevSortieAm_out output,
				@va_SortieAmUsager_out output,
				@va_SortieAmontEstBifurc_out output,
				null,
				null,
				null,
				@va_DistanceSortieAmont_out output
	
	if @vl_cr<>XDC_OK return XDC_NOK


	exec @vl_cr=XZAO17	@va_Autoroute_in,
				@va_PR_in,
				@vl_Sens,
				-1,
				@va_NumSortieAval_out output,
				@va_NomSortieAv_out output,
				@va_AbrevSortieAv_out output,
				@va_SortieAvUsager_out output,
				@va_SortieAvalEstBifurc_out output,
				null,
				null,
				null,
				@va_DistanceSortieAval_out output
	
	if @vl_cr<>XDC_OK return XDC_NOK

	select @va_NomSortieAm_out=isnull(@va_NomSortieAm_out,XDC_CHAINE_VIDE),
	       @va_AbrevSortieAm_out=isnull(@va_AbrevSortieAm_out,XDC_CHAINE_VIDE),
	       @va_SortieAmUsager_out=isnull(@va_SortieAmUsager_out,XDC_CHAINE_VIDE),
	       @va_NomSortieAv_out=isnull(@va_NomSortieAv_out,XDC_CHAINE_VIDE),
	       @va_AbrevSortieAv_out=isnull(@va_AbrevSortieAv_out,XDC_CHAINE_VIDE),
	       @va_SortieAvUsager_out=isnull(@va_SortieAvUsager_out,XDC_CHAINE_VIDE)


/*A si on se trouve sur une autoroute inversee, 
**  on echange amont et aval pour peage et bifurcation
**  (pour le reste, c est fait dans les sous procs)
*/

	if (@vl_ordre_PR = XDC_PR_INV)
	begin
		select	@vl_NumPea=@va_NumPeageAmont_out,
			@vl_DisPea=@va_DistancePeageAmont_out,
			@vl_NomPea=@va_NomPeageAm_out,
			@vl_AbrPea=@va_AbrevPeageAm_out

		select	@va_NumPeageAmont_out=@va_NumPeageAval_out,
			@va_DistancePeageAmont_out=@va_DistancePeageAval_out,
			@va_NomPeageAm_out=@va_NomPeageAval_out,
			@va_AbrevPeageAm_out=@va_AbrevPeageAval_out
		
		select	@va_NumPeageAval_out=@vl_NumPea,
			@va_DistancePeageAval_out=@vl_DisPea,
			@va_NomPeageAval_out=@vl_NomPea,
			@va_AbrevPeageAval_out=@vl_AbrPea

		select	@va_LongueurPlateforme_out=@vl_long1


		select	@vl_NumAut = @va_NumAutorouteBifAmont_out,
			@vl_DisBif = @va_DistanceBifAmont_out,
			@vl_PR1    = @va_PrBifAmont_out,
			@vl_PR2    = @va_PrDebut_BifAmont_out

		select	@va_NumAutorouteBifAmont_out = @va_NumAutorouteBifAval_out,
			@va_DistanceBifAmont_out     = @va_DistanceBifAval_out,
			@va_PrBifAmont_out           = @va_PrBifAval_out,
			@va_PrDebut_BifAmont_out     = @va_PrDebut_BifAval_out

		select	@va_NumAutorouteBifAval_out = @vl_NumAut,
			@va_DistanceBifAval_out     = @vl_DisBif,
			@va_PrBifAval_out           = @vl_PR1,
			@va_PrDebut_BifAval_out     = @vl_PR2
	end
	
	select @va_Descente_out, @va_Tunnel_out, @va_NomSortieAm_out,
			@va_AbrevSortieAm_out, @va_NomSortieAv_out, @va_AbrevSortieAv_out,
			@va_NomPeageAm_out, @va_AbrevPeageAm_out, @va_NomPeageAval_out,
			@va_AbrevPeageAval_out, @va_SortieAmUsager_out, @va_SortieAvUsager_out,
			@va_Viaduc_out



	return XDC_OK
go

