/*E*/
/*  Fichier : $Id: xzas30sp.prc,v 1.11 1999/02/26 11:45:41 gaborit Exp $    Release : $Revision: 1.11 $     Date : $Date: 1999/02/26 11:45:41 $ 
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas30sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Analyse 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.  07/12/94        : Creation      (V 1.1)
* T.F.  02/01/95        : Ajout d'un code de retour comme argument.
* C.T.  01/03/95        : Au CI rechercher les infos dans HIS 
*                         si pas trouvees dans EXP (V 1.3)
* B.G.	04/09/1995	: corrections recette site (1.4)
* B.G.  11/09/1995      : modif = par in dans sous select (1.5)
* C.T.  23/10/1995      : modif code erreur retourne si evt sans localisation (1.6)
* C.T.  19/04/1996      : Correction de test de validite du debit
*                         dans le cas de FMC plus vieille d'un mois (1.7) (837)
* JMG	23/08/1996	: ReCorrection de test de validite du debit
			  dans le cas de FMC plus vieille d'un mois (1.8)
* JMG	23/09/1996	: arrondis revus (1.9)
* PN	29/06/1998	: separation EXP/HIS (1.10)
* PN	05/02/1999	: modif interg dem/1724 v1.11
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Calcul de l'efficacite intrinseque et absolue PMV et NAV pour un evenement.
* Retourne des informations concernant l'evenement.
*
* Sequence d'appel
* SP    XZAS30_Eff_Intrinseque_Absolue_PMV_NAV
*
* Arguments en entree :
* Un evenement est compose d'un numero et d'une cle
* XDY_Entier    @va_Numero_in		: Numero de l'evement
* XDY_Mot	@va_Cle_in		: Valeur de la cle
*
* Arguments en sortie :
* XDY_Date	@va_TE0_out		: Date de debut d'evenement	(TE0)
* XDY_Date	@va_TE1_out		: Date de fin de l'evenement saisie (TE1)
* XDY_Entier    @va_Station_EVNT_out    :Station de cpt au droit de l'evenement
* XDY_PR	@va_PR_EVNT_out		: PR de l'evenement
* XDY_Sens	@va_Sens_out		: Sens
* XDY_Octet     @va_Status_out          : Status retour de la procedure
*
* Liste retournee :
* XDY_Entier 	delai		: Delai d'information
* XDY_Entier	Nbre_concer_int : Nombre d'usagers concernes (intrinseque)
* XDY_Entier	Nbre_inform_int : Nombre d'usagers informes (intrinseque)
* XDY_Entier    Nbre_Bcouvt_int : Nombre d'usagers bien couvert (intrinseque)
* XDY_Mot	TX_couvert_int  : Taux de couverture (intrinseque)
* XDY_Mot 	Fact_credibilite: Facteur de credibilite (intrinseque)
* XDY_Entier 	Nbre_concer_abs : Nombre d'usagers concernes (absolu)
* XDY_Entier	Nbre_Bcouvt_abs : Nombre d'usagers bien couvert (absolu)
* XDY_Mot	TX_couvert_abs  : Taux de couverture (absolu)
*
* Code retour
* XDC_OK		: fin de procedure normale
* XDC_NOK               : probleme d'acces aux donnees 
* XDC_ARG_INV           : parametres d'entree null, ou valeur non autorisee ou
*                         echangeur inexistante
* XDC_PAS_SYN           : pas de resultat
*
* Conditions d'utilisation
* L'evenement doit etre FINI.
*
* Fonction
*
-------------------------------------------------------*/

use PRC
go

create procedure XZAS;30
	@va_Numero_in		int		= null,
	@va_Cle_in		smallint	= null,
	@va_TE0_out             datetime	output,
	@va_TE1_out             datetime	output,
	@va_Station_EVNT_out    int		output,
	@va_PR_EVNT_out         T_PR		output,
	@va_Sens_out            smallint	output,
	@va_Status_out          tinyint         = null output
as
	declare	@vl_NumEqt_PMV		int,	 /* Numero du PMV */
		@vl_NumEqt_NAV		int,     /* Numero NAV */
		@vl_Station_PMV		int,	 /* Station de comptage PMV */	
		@vl_Vitesse_PMV		int,     /* Vitesse au droit du PMV */
		@vl_Debit		int,     /* Debit  */
		@vl_Debit_TE0		int,     /* Debit a l'instant TE0 */
		@vl_Debit_TA0		int,     /* Debit a l'instant TA0  */
		@vl_Nb_Concernes	int,	 /* Usagers concernes */
		@vl_Nb_Concernes_Abs	int,     /* Usagers concernes absolue */
		@vl_Nb_Informes         int,     /* Usagers informes */
		@vl_Nb_Couverts          int,    /* Usagers bien couverts */
		@vl_Tx_Couverture	int,	 /* Taux de couverture */
		@vl_Tx_Couverture_Abs	int,     /* Taux de couverture absolue*/
		@vl_Fact_Cred		int,	 /* Facteur de credibilite */
		@vl_Distance		int,	 /* Distance PMV-Evenement */
		@vl_Retard		int,	 /* Retard de parcours */
		@vl_PR_PMV		T_PR,    /* PR du PMV en amont */
		@vl_PR_NAV		T_PR,    /* PR NAV */
		@vl_PR_Inf_NAV 		T_PR,    /* PR influence NAV */
		@vl_Autoroute		smallint,/* Numero de l'autoroute */
		@vl_Autoroute_Eqt	smallint,/* Numero d'autoroute */
		@vl_Sens_Eqt		smallint,/* Sens equipement */
		@vl_District_Eqt	tinyint, /* District */
		@vl_Code_Retour		tinyint, /* Code retour fonction */
		@vl_TE2                 datetime,/* Date de passage PMV */
		@vl_TA0                 datetime,/* Date de debut d'aff PMV */
		@vl_TA1                 datetime,/* Date de fin d'aff PMV */
		@vl_TN0                 datetime,/* Date de debut NAV */
		@vl_TN1                 datetime,/* Date de fin NAV */
		@vl_Cloture             datetime,/* Date de cloture de l'evt */
		@vl_Status int,
		@vl_RechercheDsHis	bit


	/* Test Argument APPLIX  */
	if @va_Numero_in = 0    select @va_Numero_in	= null
	if @va_Cle_in = 0	select @va_Cle_in	= null

	/*A 
	** Controle des parametres en entree :
	*/
	if (@va_Numero_in       = null) or
	   (@va_Cle_in		= null)
		return XDC_ARG_INV

	/* Initialisation des arguments de sortie */
	select	@va_Status_out = XDC_OK,
		@vl_RechercheDsHis = XDC_NON

	/*A
	** Recheche de l'horodate de debut et de fin d'evenement.
	*/
	/* La date de debut et de fin de la FMC 		*/
	/* l'evenement est fini (date de fin renseignee).	*/

#ifdef CI
	exec @vl_Status = SQL_HIS.PRC..XZAS;30
		@va_Numero_in		,
		@va_Cle_in		,
		@va_TE0_out             ,
		@va_TE1_out             ,
		@va_Station_EVNT_out    output,
		@va_PR_EVNT_out         output,
		@va_Sens_out            output,
		@va_Status_out           output

	if @vl_Status = null
                return XDC_PRC_INC
        else return @vl_Status
	
#else
#ifdef HIST
		select  @va_TE0_out = debut,
			@va_TE1_out = fin,
			@vl_Cloture = cloture
		from HIS..FMC_GEN
		where numero = @va_Numero_in
		and cle = @va_Cle_in
		and fin != null
		and datex=XDC_FAUX
		if @@rowcount = 0
		begin
			select @va_Status_out = XDC_PAS_SYN
			return XDC_OK
		end
#else
	select 	@va_TE0_out = debut,
		@va_TE1_out = fin,
		@vl_Cloture = cloture
	from EXP..FMC_GEN
	where numero = @va_Numero_in
	and cle = @va_Cle_in
	and fin != null
	and datex=XDC_FAUX

	if @@rowcount = 0
	begin
		select @va_Status_out = XDC_PAS_SYN
		return XDC_OK
	end 
#endif
#endif
	/* Recherche du PR de l'evenement et des informations : autoroute  */
	/* et sens.							   */


	set rowcount 1
#ifdef HIST
		select 	@va_PR_EVNT_out = PR,
			@vl_Autoroute = autoroute,
			@va_Sens_out = sens
		from HIS..FMC_HIS
		where numero = @va_Numero_in
		and cle = @va_Cle_in
		if @@rowcount = 0
		begin
			select @va_Status_out = XDC_PAS_SYN
			return XDC_OK
		end
#else
	select 	@va_PR_EVNT_out = PR,
		@vl_Autoroute = autoroute,
		@va_Sens_out = sens
	from EXP..FMC_HIS
	where numero = @va_Numero_in
	and cle = @va_Cle_in

	if @@rowcount = 0
 	begin
		select @va_Status_out = XDC_PAS_SYN
		return XDC_OK
	end
#endif

	set rowcount 0

	if @vl_Autoroute is null or @va_PR_EVNT_out is null or @va_Sens_out is null or 
	   @va_Sens_out = XDC_SENS_INCONNU
 	begin
		select @va_Status_out = XDC_PAS_SYN
		return XDC_OK
	end

	/* Recherche du PMV le plus proche de l'evenement */
	execute @vl_Code_Retour = PRC..XZAO;15
				@vl_Autoroute,
				@va_PR_EVNT_out,
				@va_Sens_out,
				1,
				@vl_NumEqt_PMV		output,
				@vl_District_Eqt	output,
				@vl_Autoroute_Eqt	output,
				@vl_PR_PMV		output,
				@vl_Sens_Eqt		output,
				@vl_Distance		output

	if @vl_Code_Retour != XDC_OK
		return @vl_Code_Retour


	/* Recherche de NAV le plus proche de l'evenement */
	execute @vl_Code_Retour = PRC..XZAO;19
				@vl_Autoroute,
				@va_PR_EVNT_out,
				@va_Sens_out,
				1,
				@vl_NumEqt_NAV  	output,
				@vl_District_Eqt    	output,
				@vl_Autoroute_Eqt   	output,
				@vl_PR_NAV      	output,
				@vl_Sens_Eqt    	output,
				@vl_Distance    	output,
				@vl_PR_Inf_NAV  	output

	if @vl_Code_Retour != XDC_OK
		return @vl_Code_Retour

#ifdef HIST
	/*A
	** Au CI, si l'evenement est cloture depuis plus d'un mois
	** rechercher dans la base HIS les actions effectuees pour
	** cet evenement
	*/
		select @vl_RechercheDsHis = XDC_OUI
#endif

	/*A
	** Recherche des informations concernant : PMV et NAV.
	*/
	/* Informations PMV (prise en compte de la premiere et derniere action) */
	set rowcount 1

#ifndef HIST
	begin
		select 	@vl_TA0 	= heure_succes
		from EXP..ACT_GEN
		where evenement = @va_Numero_in
		and   cle = @va_Cle_in
		and   type = XDC_ACT_PMV
		and   heure_succes >= @va_TE0_out
		and   equipement = @vl_NumEqt_PMV
		and   heure_succes <> null
		order by heure_succes
	
		select 	@vl_TA1		= heure_fin
		from EXP..ACT_GEN
		where evenement = @va_Numero_in
		and   cle = @va_Cle_in
		and   type = XDC_ACT_PMV
		and   heure_succes >= @va_TE0_out
		and   equipement = @vl_NumEqt_PMV
		and   heure_succes <> null
		and   heure_fin <> null
		order by heure_fin desc
	
		/* Informations NAV (prise en compte de la premiere et derniere action) */
		select 	@vl_TN0		= heure_succes
		from EXP..ACT_GEN
		where evenement = @va_Numero_in
		and   cle = @va_Cle_in
		and   type = XDC_ACT_NAV
		and   heure_succes >= @va_TE0_out
		and   equipement = @vl_NumEqt_NAV
		and   heure_succes <> null
		order by heure_succes

		select 	@vl_TN1		= heure_fin
		from EXP..ACT_GEN
		where evenement = @va_Numero_in
		and   cle = @va_Cle_in
		and   type = XDC_ACT_NAV
		and   heure_succes >= @va_TE0_out
		and   equipement = @vl_NumEqt_NAV
		and   heure_succes <> null
		and   heure_fin <> null
		order by heure_fin desc

	end
#else
	begin
	
		select 	@vl_TA0 	= heure_succes
		from HIS..ACT_GEN
		where evenement = @va_Numero_in
		and   cle = @va_Cle_in
		and   type = XDC_ACT_PMV
		and   heure_succes >= @va_TE0_out
		and   equipement = @vl_NumEqt_PMV
		and   heure_succes <> null
		order by heure_succes
	
		select 	@vl_TA1		= heure_fin
		from HIS..ACT_GEN
		where evenement = @va_Numero_in
		and   cle = @va_Cle_in
		and   type = XDC_ACT_PMV
		and   heure_succes >= @va_TE0_out
		and   equipement = @vl_NumEqt_PMV
		and   heure_succes <> null
		and   heure_fin <> null
		order by heure_fin desc
	
		/* Informations NAV (prise en compte de la premiere et derniere action) */
		select 	@vl_TN0		= heure_succes
		from HIS..ACT_GEN
		where evenement = @va_Numero_in
		and   cle = @va_Cle_in
		and   type = XDC_ACT_NAV
		and   heure_succes >= @va_TE0_out
		and   equipement = @vl_NumEqt_NAV
		and   heure_succes <> null
		order by heure_succes

		select 	@vl_TN1		= heure_fin
		from HIS..ACT_GEN
		where evenement = @va_Numero_in
		and   cle = @va_Cle_in
		and   type = XDC_ACT_NAV
		and   heure_succes >= @va_TE0_out
		and   equipement = @vl_NumEqt_NAV
		and   heure_succes <> null
		and   heure_fin <> null
		order by heure_fin desc

	end
#endif
	set rowcount 0

	/* Recherche de la station de comptage proche du PMV	*/
	/* Portion puis ZI qui contiennent le PMV.		*/
	select @vl_Station_PMV = station
	from CFG..RES_INF
	where sens = @va_Sens_out
	and   PR_debut  <= @vl_PR_PMV
	and   PR_fin    >= @vl_PR_PMV
	and   portion in (	select numero
				from CFG..RES_POR
				where autoroute = @vl_Autoroute
				and   PR_debut  <= @vl_PR_PMV
				and   PR_fin	>= @vl_PR_PMV
			)

	/* Recherche de la station de comptage proche de l'evenement 	*/
	/* Portion puis ZI qui contiennent l'evenement.		  	*/
	select @va_Station_EVNT_out = station
	from CFG..RES_INF
	where sens = @va_Sens_out
	and   PR_debut  <= @va_PR_EVNT_out
	and   PR_fin    >= @va_PR_EVNT_out
	and   portion in (       select numero
				from CFG..RES_POR
				where autoroute = @vl_Autoroute
				and   PR_debut  <= @va_PR_EVNT_out
				and   PR_fin    >= @va_PR_EVNT_out
			)

	/* Recherche de la vitesse au droit du PMV a la date TE1 */
#ifdef HIST
		select @vl_Vitesse_PMV = V
		from HIS..MES_TRA
		where station = @vl_Station_PMV
		and	sens  = @va_Sens_out
		and	voie  = XDC_RADT_CHAUSSEE
		and	horodate <= @va_TE1_out
		and 	horodate > dateadd(minute,-6,@va_TE1_out)
#else
	select @vl_Vitesse_PMV = V
	from EXP..MES_TRA
	where station = @vl_Station_PMV
	and	sens  = @va_Sens_out
	and	voie  = XDC_RADT_CHAUSSEE
	and	horodate <= @va_TE1_out
	and 	horodate > dateadd(minute,-6,@va_TE1_out)

#endif

	/* Calcul de l'horodate de passage au droit du PMV du vehicule qui */
	/* passe au PR de l'evenement a TE1.				   */
	/* TE2 = TE1 - ( PR(EVNT) - PR(PMV) / VPMV(TE1) )		   */
	/* Cacul du retard de parcours ( PR(EVNT) - PR(PMV) / VPMV(TE1) */
	select @vl_Retard = (36*abs(@vl_PR_PMV - @va_PR_EVNT_out)) /
						@vl_Vitesse_PMV /10
	/* Calcul de TE2  (TE1 - retard) */
	select @vl_TE2 = dateadd(second,-@vl_Retard,@va_TE1_out)

	/* Creation d'une table temporaire pour la sauvegarde des resultats */
	create table #EFFICACITE_RES (
		type		    tinyint	null,
		delai			int	null,
		Nbre_concer_int		int	null,
		Nbre_inform_int		int	null,
		Nbre_Bcouvt_int		int     null,
		TX_couvert_int	   smallint	null,
		Fact_credibilite   smallint     null,
		Nbre_concer_abs		int	null,
		Nbre_Bcouvt_abs		int	null,
		TX_couvert_abs     smallint     null
		)

	/*A
	** Calcul des indicateurs PMV.
	*/

 	/* Recherche du debit au droit du PMV entre la date TE0 et TE2 */
#ifdef HIST
	if @vl_RechercheDsHis = XDC_OUI or @vl_Debit_TE0 is null
	begin
		select @vl_Debit_TE0 = avg(Q)
		from HIS..MES_TRA
		where station = @vl_Station_PMV
		and     sens  = @va_Sens_out
		and     voie  = XDC_RADT_CHAUSSEE
		and     horodate >= @va_TE0_out
		and     horodate < dateadd(minute,6,@vl_TE2)
	end
#else
	select @vl_Debit_TE0 = avg(Q)
	from EXP..MES_TRA
	where station = @vl_Station_PMV
	and     sens  = @va_Sens_out
	and     voie  = XDC_RADT_CHAUSSEE
	and     horodate >= @va_TE0_out
	and     horodate < dateadd(minute,6,@vl_TE2)

#endif

	/*A
	** Calcul du nombre d'usagers concernes intrinseque (PMV)
	*/
	select @vl_Nb_Concernes = @vl_Debit_TE0 * abs(datediff(second,@va_TE0_out,@vl_TE2))/3600

	/* Recherche du debit au droit du PMV entre la date TA0 et TA1 */

#ifdef HIST
		select @vl_Debit_TA0 = avg(Q)
		from HIS..MES_TRA
		where station = @vl_Station_PMV
		and     sens  = @va_Sens_out
		and     voie  = XDC_RADT_CHAUSSEE
		and     horodate >= @vl_TA0
		and     horodate < dateadd(minute,6,@vl_TA1)
#else
	select @vl_Debit_TA0 = avg(Q)
	from EXP..MES_TRA
	where station = @vl_Station_PMV
	and     sens  = @va_Sens_out
	and     voie  = XDC_RADT_CHAUSSEE
	and     horodate >= @vl_TA0
	and     horodate < dateadd(minute,6,@vl_TA1)
#endif


	/*A
	** Calcul du nombre d'usagers informes intrinseque (PMV)
	*/
	select @vl_Nb_Informes = @vl_Debit_TA0 * abs(datediff(second,@vl_TA0,@vl_TA1))/3600

	/*A
	** Calcul du nombre de d'usagers bien couverts intrinseque (PMV)
	*/
	/* Recherche de min(TA1,TE2).	     */
	if @vl_TA1 > @vl_TE2
		select @vl_TA1=@vl_TE2

	/* Condition particuliere pour le calcul */
	if @vl_TA0 > @vl_TA1
		select @vl_Debit = 0
	else
	   begin
#ifdef HIST
			select @vl_Debit = avg(Q)
			from HIS..MES_TRA
			where station = @vl_Station_PMV
			and     sens  = @va_Sens_out
			and     voie  = XDC_RADT_CHAUSSEE
			and     horodate >= @vl_TA0
			and     horodate < dateadd(minute,6,@vl_TA1)
#else
		select @vl_Debit = avg(Q)
		from EXP..MES_TRA
		where station = @vl_Station_PMV
		and     sens  = @va_Sens_out
		and     voie  = XDC_RADT_CHAUSSEE
		and     horodate >= @vl_TA0
		and     horodate < dateadd(minute,6,@vl_TA1)
#endif
	   end
	
	select @vl_Nb_Couverts=@vl_Debit * abs(datediff(second,@vl_TA1,@vl_TA0))/3600

	/*A
	** Calcul du taux de couverture intrinseque (PMV).
	*/
	if (@vl_Nb_Concernes != 0)
		select @vl_Tx_Couverture = (((1000*@vl_Nb_Couverts) /
						@vl_Nb_Concernes) + 5) / 10
	else
		select @vl_Tx_Couverture = 0
	
	/*A
	** Calcul du facteur de credibilite intrinseque (PMV).
	*/
	if (@vl_Nb_Informes != 0)
		select @vl_Fact_Cred = (((1000*@vl_Nb_Couverts) /
						@vl_Nb_Informes)+5)/10
	else
		select @vl_Fact_Cred = 0

	/* Recherche du debit au droit de l'evenement entre la date TE0 et TE1 */
#ifdef HIST
		select @vl_Debit_TE0 = avg(Q)
		from HIS..MES_TRA
		where station = @va_Station_EVNT_out
		and     sens  = @va_Sens_out
		and     voie  = XDC_RADT_CHAUSSEE
		and     horodate >= @va_TE0_out
		and     horodate < dateadd(minute,6,@va_TE1_out)
#else
	select @vl_Debit_TE0 = avg(Q)
	from EXP..MES_TRA
	where station = @va_Station_EVNT_out
	and     sens  = @va_Sens_out
	and     voie  = XDC_RADT_CHAUSSEE
	and     horodate >= @va_TE0_out
	and     horodate < dateadd(minute,6,@va_TE1_out)
#endif
	/*A
	** Calcul du nombre d'usagers concernes absolue  (PMV)
	*/
	select @vl_Nb_Concernes_Abs = @vl_Debit_TE0 * abs(datediff(second,@va_TE0_out,@va_TE1_out))/3600

	/*A
	** Calcul du taux de couverture absolue (PMV).
	*/
	if (@vl_Nb_Concernes_Abs != 0)
		select @vl_Tx_Couverture_Abs =  (((1000*@vl_Nb_Couverts) /
						@vl_Nb_Concernes_Abs) + 5)/10
	else
		select @vl_Tx_Couverture_Abs = 0

	/* Preparation de la liste resultat */
	insert #EFFICACITE_RES  (       type,
					delai,
					Nbre_concer_int,
					Nbre_inform_int,
					Nbre_Bcouvt_int,
					TX_couvert_int,
					Fact_credibilite,
					Nbre_concer_abs,
					Nbre_Bcouvt_abs,
					TX_couvert_abs
				)
	values  (       XDC_EQT_PMV,
			abs(datediff(second,@vl_TA0,@va_TE0_out)),
			@vl_Nb_Concernes,
			@vl_Nb_Informes,
			@vl_Nb_Couverts,
			@vl_Tx_Couverture,
			@vl_Fact_Cred,
			@vl_Nb_Concernes_Abs,
			@vl_Nb_Couverts,
			@vl_Tx_Couverture_Abs
		)

	/*A
	** Calcul des indicateurs NAV.
	*/
	/* Recherche du debit au droit de l'evenement entre la date TN0 et TN1 */

#ifdef HIST
		select @vl_Debit_TE0 = avg(Q)
		from HIS..MES_TRA
		where station = @va_Station_EVNT_out
		and     sens  = @va_Sens_out
		and     voie  = XDC_RADT_CHAUSSEE
		and     horodate >= @vl_TN0
		and     horodate < dateadd(minute,6,@vl_TN1)
#else
	select @vl_Debit_TE0 = avg(Q)
	from EXP..MES_TRA
	where station = @va_Station_EVNT_out
	and     sens  = @va_Sens_out
	and     voie  = XDC_RADT_CHAUSSEE
	and     horodate >= @vl_TN0
	and     horodate < dateadd(minute,6,@vl_TN1)
#endif


	/*A
	** Calcul du nombre d'usagers informes intrinseque (NAV)
	*/
	select @vl_Nb_Informes = @vl_Debit_TE0 * abs(datediff(second,@vl_TN0,@vl_TN1))/3600

	/*A
	** Calcul du nombre de d'usagers bien couverts intrinseque (NAV)
	*/
	/* Recherche de min(TN1,TE1).        */
	if @vl_TN1 > @va_TE1_out
		select @vl_TN1 = @va_TE1_out

	/* Condition particuliere pour le calcul */
	if @vl_TN0 > @vl_TN1
		select @vl_Debit = 0
	else
	   begin
#ifdef HIST
			select @vl_Debit = avg(Q)
			from HIS..MES_TRA
			where station = @va_Station_EVNT_out
			and     sens  = @va_Sens_out
			and     voie  = XDC_RADT_CHAUSSEE
			and     horodate >= @vl_TN0
			and     horodate < dateadd(minute,6,@vl_TN1)
#else
		select @vl_Debit = avg(Q)
		from EXP..MES_TRA
		where station = @va_Station_EVNT_out
		and     sens  = @va_Sens_out
		and     voie  = XDC_RADT_CHAUSSEE
		and     horodate >= @vl_TN0
		and     horodate < dateadd(minute,6,@vl_TN1)
#endif
	   end
	
	select @vl_Nb_Couverts=@vl_Debit * abs(datediff(second,@vl_TN1,@vl_TN0))/3600

	/*A
	** Calcul du taux de couverture intrinseque (NAV).
	*/
	if (@vl_Nb_Concernes_Abs != 0)
		select @vl_Tx_Couverture = (((1000*@vl_Nb_Couverts) /
							@vl_Nb_Concernes_Abs)+5)/10
	else
		select @vl_Tx_Couverture = 0

	/*A
	** Calcul du facteur de credibilite intrinseque (PMV).
	*/
	if (@vl_Nb_Informes != 0)
		select @vl_Fact_Cred = (((1000*@vl_Nb_Couverts) /
							@vl_Nb_Informes)+5)/10
	else
		select @vl_Fact_Cred = 0

	insert #EFFICACITE_RES  (       type,
					delai,
					Nbre_concer_int,
					Nbre_inform_int,
					Nbre_Bcouvt_int,
					TX_couvert_int,
					Fact_credibilite,
					Nbre_concer_abs,
					Nbre_Bcouvt_abs,
					TX_couvert_abs
				)	
	values  (       XDC_EQT_NAV,
			abs(datediff(second,@vl_TN0,@va_TE0_out)),
			@vl_Nb_Concernes_Abs,
			@vl_Nb_Informes,
			@vl_Nb_Couverts,
			@vl_Tx_Couverture,
			@vl_Fact_Cred,
			@vl_Nb_Concernes_Abs,
			@vl_Nb_Couverts,
			@vl_Tx_Couverture
		)

	/*A 
	** Affichage des resultats
	*/
	select 	delai,
		Nbre_concer_int,
		Nbre_inform_int,
		Nbre_Bcouvt_int,
		TX_couvert_int,
		Fact_credibilite,
		Nbre_concer_abs,
		Nbre_Bcouvt_abs,
		TX_couvert_abs
	from #EFFICACITE_RES
	order by type desc

	/* Destruction de la table temporaire */
	drop table #EFFICACITE_RES

	/*A
	** Fin de la procedure
	*/
 	return XDC_OK
go
