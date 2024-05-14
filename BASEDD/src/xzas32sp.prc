/*E*/
/*  Fichier : $Id: xzas32sp.prc,v 1.7 1998/09/22 13:32:00 gaborit Exp $   Release : $Revision: 1.7 $       Date : $Date: 1998/09/22 13:32:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas32sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Analyse 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.  08/12/94        : Creation      (V 1.1)
* T.F.   2/01/95        : Ajout d'un code de retour comme argument.(V 1.2)
* C.T.  01/03/95        : Recherche infos dans HIS si date
*                         > 1 mois      (V 1.3)
* B.G.	05/09/95	: modif recette site (1.4)
* B.G.  11/09/95        : filtre les echangeurs sans vitesse (1.5)
* B.G.	31/10/95	: gestion du sens inverse A57 (1.6)
* P.N.	29/06/98	: separation EXP/HIS (1.7)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne des informations sur les echangeurs concernes par un evenement.
* La liste des echangeurs AMONT s'etant du PR de l'evenement jusqu'a la fin
* de l'autoroute.
*
* Sequence d'appel
* SP   XZAS32_Info_Echangeurs 
*
* Arguments en entree :
* Un evenement est compose d'un numero et d'une cle
* XDY_Entier    @va_Numero_in		: Numero de l'evement
* XDY_Mot	@va_Cle_in		: Valeur de la cle
* XDY_Date	@vl_TE0			: Date de debut d'evenement
* XDY_PR	@vl_PR_EVNT		: PR de l'evenement
*
* Arguments en sortie :
* XDY_Octet     @va_Status_out          : Status retour de la procedure
*
* Liste retournee :
* XDY_Entier	Num_Echangeur		: Numero d'un echangeur  (r)
* XDY_PR	PR			: PR de l'echangeur	PR(r)
* XDY_Mot 	Amont_Aval		: Positionnement de l'echangeur  (j)
* XDY_Entier	Debit_TE0		: Debit a l'instant TE0 (Qe(j,s,TE0))
* XDY_Entier	Vitesse_T0E		: Vitesse a l'instant TE0 (V(j,s,TE0))
*
* Code retour
* XDC_OK		: fin de procedure normale
* XDC_NOK               : probleme d'acces aux donnees 
* XDC_ARG_INV           : parametres d'entree null, ou valeur non autorisee ou
*                         echangeur inexistante
* XDC_PAS_SYN		: pas d'echangeurs amont ou d'echangeur aval
*
* Conditions d'utilisation
* Utilise pour le calcul de l'efficacite intrinseque et absolue RADIO
* Le champ "Amont_Aval" perment de localiser l'echangeur.
*-> Si "Amont_Aval" = 0, il s'agit de l'echangeur situe en aval de l'evenement
*-> Si "Amont_Aval" > 0, il s'agit d'un echangeur situe en amont de l'evenement 
*	(Plus "Amont_Aval" est eleve, plus l'echangeur est loin de l'evenement)
* La liste retournee est triee par "Amont_Aval" decroissant.
*
* Fonction
*
-------------------------------------------------------*/

use PRC
go

create procedure XZAS;32
	@va_Numero_in		int		= null,
	@va_Cle_in		smallint	= null,
	@va_TE0_in              datetime	= null,
	@va_PR_EVNT_in          T_PR		= null,
	@va_Status_out          tinyint         = null output
as
	declare	@vl_Numero		int,	 /* Numero d'action */
		@vl_Station_Troncon	int,	 /* Num station echangeur */
		@vl_Station_Entree	int,     /* Num station entre echgr */
		@vl_Vitesse		int,	 /* Vitesse */
		@vl_Debit		int,	 /* Debit */
		@vl_Debit_Entree	int,     /* Debit */
		@vl_PR			T_PR,	 /* PR echangeur */
		@vl_Autoroute		smallint,/* Autoroute */
		@vl_Sens             	smallint,/* Sens */
		@vl_Sens_inv		smallint,/* Sens inverse pour A57 */
		@vl_Sens_Entree         smallint,/* Sens entrant */
		@vl_Status int,
		@vl_RechercheDsHis	bit

	/* Test Argument APPLIX  */
	if @va_Numero_in = 0    select @va_Numero_in	= null
	if @va_Cle_in = 0	select @va_Cle_in	= null
	if @va_PR_EVNT_in = 0	select @va_PR_EVNT_in	= null	

	/*A 
	** Controle des parametres en entree :
	*/
	if (@va_Numero_in       = null) or
	   (@va_Cle_in		= null) or
	   (@va_PR_EVNT_in	= null)
		return XDC_ARG_INV

	/* Initialisation des arguments de sortie et d'une var locale */
	select	@va_Status_out = XDC_OK,
		@vl_RechercheDsHis = XDC_NON

#ifdef CI
	/*A
	** si la date donnee est anterieure d'un mois au moins
	** rechercher les infos dans la base HIS sinon dans la base EXP
	*/
		select @vl_RechercheDsHis = XDC_OUI
		exec @vl_Status = SQL_HIS.PRC..XZAS;32
			@va_Numero_in		,
			@va_Cle_in		,
			@va_TE0_in              ,
			@va_PR_EVNT_in          ,
			@va_Status_out          output

		if @vl_Status = null
                       	return XDC_PRC_INC
                else return @vl_Status
#endif
#ifdef HIST
		select @vl_RechercheDsHis = XDC_OUI
#endif

	/* Complement d'info sur l'evemement */
#ifndef HIST
		select 	@vl_Autoroute= autoroute,
			@vl_Sens = sens
		from EXP..FMC_HIS
		where	numero = @va_Numero_in
		and	cle = @va_Cle_in
		and	PR = @va_PR_EVNT_in
#else
		select 	@vl_Autoroute= autoroute,
			@vl_Sens = sens
		from HIS..FMC_HIS
		where	numero = @va_Numero_in
		and	cle = @va_Cle_in
		and	PR = @va_PR_EVNT_in
#endif

	/*A traitement de l'A57 */
	
	if exists (	select * from CFG..RES_AUT_INV
			where	numero=@vl_Autoroute
			  and	inversee=XDC_PR_INV )
	begin
		if @vl_Sens = XDC_SENS_NORD
			select @vl_Sens_inv = XDC_SENS_SUD
		else
			select @vl_Sens_inv = XDC_SENS_NORD
	end
	else	select @vl_Sens_inv=@vl_Sens
	
	
	/* Creation d'un table temporaire pour conserver les informations */
	/* TFM.							    	   */
	/* Amont > 0 / Aval = 0 */
	create table #ECHANG_LISTE	(
				Num_Echangeur	int	null,
				PR		int	null,
				Amont_Aval	tinyint null,
				Debit_TE0	int	null,
				Vitesse_TE0	int     null
				)

	/* Recheche des echangeurs AMONT */

	insert #ECHANG_LISTE (Num_Echangeur,PR)
	select CFG..RES_ECH.numero,CFG..RES_ECH.PR
	from CFG..RES_ECH,CFG..RES_POR
	where CFG..RES_ECH.portion=CFG..RES_POR.numero
	  and CFG..RES_POR.autoroute=@vl_Autoroute
	  and (CFG..RES_ECH.PR <= @va_PR_EVNT_in or @vl_Sens_inv=XDC_SENS_NORD)
	  and (CFG..RES_ECH.PR >= @va_PR_EVNT_in or @vl_Sens_inv=XDC_SENS_SUD)

	/* numerotation des echangeurs amont */
	set rowcount 1

	select @vl_PR=PR
	from #ECHANG_LISTE
	where Amont_Aval=null

	while @@rowcount<>0
	begin

	   select @vl_Numero=isnull(max(Amont_Aval),0)+1
	   from #ECHANG_LISTE

	   if @vl_Sens_inv=XDC_SENS_NORD
		update #ECHANG_LISTE
		set Amont_Aval=@vl_Numero
		where Amont_Aval=null
		and PR in (	select min(PR)
				from #ECHANG_LISTE
				where Amont_Aval=null )
	   else
		update #ECHANG_LISTE
		set Amont_Aval=@vl_Numero
		where Amont_Aval=null
		and PR in (     select max(PR)
				from #ECHANG_LISTE
				where Amont_Aval=null )

	   select @vl_PR=PR
	   from #ECHANG_LISTE
	   where Amont_Aval=null

	end

	/*A
	** Recherche du premier echangeur situe en aval de l'evenement.
	*/
	if @vl_Sens_inv=XDC_SENS_NORD
		insert #ECHANG_LISTE (Num_Echangeur,PR,Amont_Aval)
		select CFG..RES_ECH.numero,CFG..RES_ECH.PR,0
		from CFG..RES_ECH,CFG..RES_POR
		where CFG..RES_ECH.portion=CFG..RES_POR.numero
		  and CFG..RES_POR.autoroute=@vl_Autoroute
		  and CFG..RES_ECH.PR < @va_PR_EVNT_in
		order by CFG..RES_ECH.PR desc
	else
		insert #ECHANG_LISTE (Num_Echangeur,PR,Amont_Aval)
		select CFG..RES_ECH.numero,CFG..RES_ECH.PR,0
		from CFG..RES_ECH,CFG..RES_POR
		where CFG..RES_ECH.portion=CFG..RES_POR.numero
		  and CFG..RES_POR.autoroute=@vl_Autoroute
		  and CFG..RES_ECH.PR > @va_PR_EVNT_in
		order by CFG..RES_ECH.PR

	/* Pas d'echangeur aval */
	if @@rowcount != 1
	begin
		select @va_Status_out = XDC_PAS_SYN
		set rowcount 0
		return XDC_OK
	end

	/* Retour au mode normal */
	set rowcount 0
	/* Duplique la table tempo pour pb curseur update */
	create table #ECHANG_LIST2(
				Num_Echangeur   int     null,
				PR              int     null
				)
	insert #ECHANG_LIST2 (Num_Echangeur,PR)
		select Num_Echangeur,PR from #ECHANG_LISTE

	/* Parcour de la liste des echangeurs afin de completer les */
	/* informations necessaires au calcul.			    */
	declare Echangeur cursor for
		select 	Num_Echangeur,
			PR
		from #ECHANG_LIST2

	open Echangeur

	/* Positionnement sur le premier echangeur */
	fetch Echangeur into 	@vl_Numero,
				@vl_PR

	/* Tant qu'il existe des echangeurs dans la liste */
	while @@sqlstatus = 0
	begin
		/* Debit representatif pour les vehicules entrant sur le */
		/* reseau a l'echangeur "j",dans le sens "s",a l'horodate "t".*/
		/*  Qe(j,s,r) */
		/* Au temps : TEO */
		/* Le sens entrant depend du sens normal */
		if @vl_Sens = XDC_SENS_NORD
			select @vl_Sens_Entree = XDC_SENS_ENTREE_NORD
		if @vl_Sens = XDC_SENS_SUD
			select @vl_Sens_Entree = XDC_SENS_ENTREE_SUD

		/* Vitesse mesure en pleine voie, au niveau de l'echangeur  */
		/* "j" et dans le sens "s", a l'horodate "t". V(j,s,r).     */
		/* Station representative sur le troncon contenant l'echangeur*/
		/* Au temps : TEO */

		/* Recheche de la station de comptage */
		/* Portion puis ZI qui contiennent le PR de l'echangeur.    */
		select @vl_Station_Troncon = station
		from CFG..RES_INF
		where sens = @vl_Sens
		and   PR_debut  <= @vl_PR
		and   PR_fin    >= @vl_PR
		and   portion in (       select numero
	 		from CFG..RES_POR
	 		where autoroute = @vl_Autoroute
	 		)

		select @vl_Station_Entree = null,@vl_Debit=null,@vl_Vitesse=null,@vl_Debit_Entree=null

		select @vl_Station_Entree = numero
		from CFG..EQT_GEN
		where type=XDC_EQT_RAD
		and PR=@vl_PR
		and autoroute=@vl_Autoroute

#ifdef HIST
		begin
			select @vl_Debit_Entree = Q
			from HIS..MES_TRA
			where station = @vl_Station_Entree
			and     sens  = @vl_Sens_Entree
			and     voie  = XDC_RADT_CHAUSSEE
			and     horodate > @va_TE0_in
			and     horodate <= dateadd(minute,6,@va_TE0_in)
	
	
			select @vl_Vitesse = V,@vl_Debit = Q
			from HIS..MES_TRA
			where station = @vl_Station_Troncon
			and     sens  = @vl_Sens
			and     voie  = XDC_RADT_CHAUSSEE
			and     horodate > @va_TE0_in
			and     horodate <= dateadd(minute,6,@va_TE0_in)
		end
#else
		select @vl_Debit_Entree = Q
		from EXP..MES_TRA
		where station = @vl_Station_Entree
		and     sens  = @vl_Sens_Entree
		and     voie  = XDC_RADT_CHAUSSEE
		and     horodate > @va_TE0_in
		and     horodate <= dateadd(minute,6,@va_TE0_in)


		select @vl_Vitesse = V,@vl_Debit = Q
		from EXP..MES_TRA
		where station = @vl_Station_Troncon
		and     sens  = @vl_Sens
		and     voie  = XDC_RADT_CHAUSSEE
		and     horodate > @va_TE0_in
		and     horodate <= dateadd(minute,6,@va_TE0_in)
#endif

		/* Mise a jour des donnees dans la tables resultat */
		update #ECHANG_LISTE
		set 	Debit_TE0 = isnull(@vl_Debit_Entree,@vl_Debit),
			Vitesse_TE0 = @vl_Vitesse
		where  Num_Echangeur = @vl_Numero


		/* Passage a l'echangeur suivant */
		fetch Echangeur into    @vl_Numero,
					@vl_PR

	end     /* Fin de parcours de la liste des echangeurs */

	close Echangeur

	/*A
	** Suppression des echangeurs sans vitesse sauf l'aval
	*/
	delete #ECHANG_LISTE
	where ( Vitesse_TE0=null or Vitesse_TE0=0 ) and Amont_Aval<>0

	/*A
	** Affichage du resultat
	*/
	select * from #ECHANG_LISTE
	order by Amont_Aval desc

	/* Destruction de la table temporaire */
	drop table #ECHANG_LISTE 

	/*A
	** Fin de la procedure
	*/
 	return XDC_OK
go
