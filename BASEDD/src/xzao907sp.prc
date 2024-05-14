/*E*/
/* Fichier : $Id: xzao907sp.prc,v 1.2 2021/06/03 13:08:32 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2021/06/03 13:08:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao907sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Retourne le numéro d'un PAU pour un secteur donnée
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	12/04/21 	: Creation PAU SONO 1.1 DEM-SAE244
* LCL	03/06/21	: inversion PR si PR deb > PR fin
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourn numero PAU
* 
* Sequence d'appel
* SP	XZAO907_Trouve_PAU
* 
* Arguments en entree
* XDY_Site	va_Site_in
* XDY_Texte	va_Autoroute_in
* XDY_Entier	va_Sens_in
* XDY_Entier	va_PR_debut_in
* XDY_Entier	va_PR_fin_in
* XDY_Site	va_site_origine_in
* XDY_Entier	va_DistanceEvt_in
*
* Arguments en sortie
* XDY_Entier	va_NumEqt_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* XDC_REV_PB_DCL: probleme pour declencher le reveil
* XDC_PRC_INC	: ARRP03 inexistante
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si l'action a ete proposee par le plan d'actions, positionner 
* Priorite et DistanceEvt, sinon, mettre DistanceEvt a XDC_DISTANCE_INCONNUE
* (Priorite sera mise automatiquement a null)
* 
* Fonction
* Retourne le numéro de PAU le plus proche en amont dans la tranche de PR donnée 
* 
-----------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO907' and type = 'P')
	drop procedure XZAO907
go

create procedure XZAO907
	@va_Site_in		tinyint,								
	@va_Autoroute_in	char(8),											
	@va_Sens_in		int,
	@va_PR_debut_in		int,
	@va_PR_fin_in		int,
	@va_site_origine_in	T_SITE,
	@va_NumEqt_out		int = null output,
	@va_distant_in 		int = XDC_NON
as
	declare @vl_Site tinyint,
		@vl_HeureSucces datetime,
		@vl_HeureEchec datetime,
		@vl_EtatIndispo int,
		@vl_EtatPbMineur int,
		@vl_Autoroute T_AUTOROUTE, 
		@vl_NumEqt T_EQUIPEMENT, 
		@vl_Status int, 
		@vl_serveur_pra char(6),
		@vl_A57 tinyint,
		@vl_PR_swap int

	/* récupération valeur A57*/
	select @vl_A57 = numero from CFG..RES_AUT where nom="A57" 

	/* Init equipement a 0 pour sortie status OK si equipement pas trouvé */
	select @va_NumEqt_out = 0

	/* recherche le site local */
        /* MOVIS */
	if @va_site_origine_in = null
		select  @vl_Site = numero
			from CFG..RES_DIS
			where serveur = @@servername
	else
        begin
                if @va_distant_in = XDC_NON
                begin
                    select @vl_Site = @va_site_origine_in
                end
                else
                begin
                    select @vl_Site = @va_Site_in
                end
        end				

        if @vl_Site is null
                return XDC_NOK

	if @va_PR_debut_in > @va_PR_fin_in
	begin
		select @vl_PR_swap = @va_PR_debut_in
		select @va_PR_debut_in = @va_PR_fin_in
		select @va_PR_fin_in = @vl_PR_swap
	end

	/* Init serveur PRA */
	select @vl_serveur_pra=serveur_pra from CFG..RES_PRA where numero=@va_Site_in
	/*A On est au CI et on pilote un autre district */
	if @vl_Site != @va_Site_in and @vl_Site = XDC_CI and @va_distant_in = XDC_NON		
	begin
		if @vl_serveur_pra=XDC_SQLDP	
			exec @vl_Status = SQL_DS.PRC..XZAO907 @va_Site_in, 
					@va_Autoroute_in,
					@va_Sens_in,
					@va_PR_debut_in,
					@va_PR_fin_in,
					@va_site_origine_in,
					@vl_NumEqt output,
					XDC_OUI
		else if @vl_serveur_pra=XDC_SQLVC
			exec @vl_Status = SQL_DM.PRC..XZAO907 @va_Site_in, 
					@va_Autoroute_in,
					@va_Sens_in,
					@va_PR_debut_in,
					@va_PR_fin_in,
					@va_site_origine_in,
					@vl_NumEqt output,
					XDC_OUI
		else if @vl_serveur_pra=XDC_SQLCI	
			exec @vl_Status = SQL_CI.PRC..XZAO907 @va_Site_in, 
					@va_Autoroute_in,
					@va_Sens_in,
					@va_PR_debut_in,
					@va_PR_fin_in,
					@va_site_origine_in,
					@vl_NumEqt output,
					XDC_OUI
		else return XDC_NOK

		return 10												
	end
				
	/*A controle des parametres obligatoires */
	if @va_Autoroute_in  = null 
		or @va_Sens_in  = null 
		or @va_PR_debut_in  = null 
		or @va_PR_fin_in = null 
		return XDC_ARG_INV


	/* Convertie va_Autoroute_in en int, si ça fonctionne on ne cherche pas dans la table res_aut */								
	select @vl_Autoroute=0
	if isnumeric(@va_Autoroute_in)=1
	begin
		select @vl_Autoroute = convert( integer, @va_Autoroute_in)
	end
	else				
	begin
		/* Trouver le numéro d'autoroute */
		select 	@vl_Autoroute = numero
		from CFG..RES_AUT
		where nom   = @va_Autoroute_in
	end

		/* Cherchez l'equipement en fonction de l'autoroute, des pr et du sens */
		if (@vl_Autoroute!=@vl_A57) /* On cherche le premier equipement dans le sens de la circulation qui correspond au param */
		begin
			if (@va_Sens_in = XDC_SENS_SUD or @va_Sens_in = XDC_SENS_ENTREE_SUD)
			begin
				select top 1 @vl_NumEqt=numero from CFG..EQT_GEN 
				where autoroute = @vl_Autoroute
				and sens = @va_Sens_in
				and PR >= @va_PR_debut_in and PR <= @va_PR_fin_in
				and type = XDC_EQT_PAU
				ORDER BY PR
			end
			else
			begin
				select top 1 @vl_NumEqt=numero from CFG..EQT_GEN 
				where autoroute = @vl_Autoroute
				and sens = @va_Sens_in
				and PR >= @va_PR_debut_in and PR <= @va_PR_fin_in
				and type = XDC_EQT_PAU
				ORDER BY PR DESC
			end
		end
		else /* Pour l'A57 on prend le premier equipement dans le sens de la circulation*/
		begin
			if(@va_Sens_in = XDC_SENS_NORD or @va_Sens_in = XDC_SENS_ENTREE_NORD)
			begin
				select top 1 @vl_NumEqt=numero from CFG..EQT_GEN 
				where autoroute = @vl_Autoroute
				and sens = @va_Sens_in
				and PR >= @va_PR_debut_in and PR <= @va_PR_fin_in
				and type = XDC_EQT_PAU
			ORDER BY numero	DESC
			end
			else
			begin
				select top 1 @vl_NumEqt=numero from CFG..EQT_GEN
				where autoroute = @vl_Autoroute
				and sens = @va_Sens_in
				and PR >= @va_PR_debut_in and PR <= @va_PR_fin_in
				and type = XDC_EQT_PAU
		        	ORDER BY numero
			end
		end
										
		/*A Verifier l'existence de l'equipement sur le site et determiner sa disponibilite */
		select	@vl_EtatPbMineur = mineure,
			@vl_EtatIndispo = (majeure | critique | HS | inhibe | desactive)
		from	EXP..EQT_DSP
		where	type = XDC_EQT_PAU  and  equipement =  @vl_NumEqt  and
			sit = @vl_Site  and
			dernier = 1

		if @@rowcount = 0
			return XDC_OK
		if @vl_EtatIndispo <> 0
			return XDC_OK
									
		select @va_NumEqt_out = @vl_NumEqt
		
		return XDC_OK												
go
