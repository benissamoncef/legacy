/*E*/
/*  Fichier : $Id: vuem52sp.prc,v 1.3 1999/02/26 11:48:24 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1999/02/26 11:48:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem52sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* fichiers metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	01/07/96	: Creation	(V 1.1)
* P.N.	05/02/99	: Modif interg. dem/1724 v1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* calcul du volume d'un bouchon avec le nombre de voies
* et la longueur maximale avec prise en compte des PR fantomes
* 
* Sequence d'appel
* SP	VUEM52_Volume_Bouchon
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in		
* XDY_Entier	va_Cause_in	: tete de bouchon
* XDY_Octet	va_CleCause_in		
* XDY_Octet	va_Autoroute_in		
* XDY_Sens	va_Sens_in		
* 
* Arguments en sortie
* XDY_Entier	va_Volume_out		
* XDY_Entier	va_Longueur_out		
*
* Code retour
* XDC_OK
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee 
* 
* Conditions d'utilisation
* Attention aucun controle n'est effectue pour verifier que le PR
* de debut et de fin de bouchon correspondent au sens de l'autoroute
* au niveau des fiches evenements : pour le graphe c'est identique
*
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Pas executable aux districts
* 
*
* Fonction
* selectionner dans la table FMC_GEN, FMC_HIS, FMC_BOU
-------------------------------------------------------*/

use PRC
go

create procedure VUEM52
	@va_NumEvt_in		int		= null,
	@va_CleEvt_in		tinyint		= null,
	@va_Cause_in		int		= null,
	@va_CleCause_in		tinyint		= null,
	@va_Autoroute_in	T_AUTOROUTE	= null,
	@va_Sens_in		T_SENS		= null,
	@va_Volume_out		int		= null	output,
	@va_Longueur_out	int		= null	output
as
	declare @vl_PRDebut T_PR, @vl_PRFin T_PR, @vl_Surface int,
		@vl_HoroDeb datetime, @vl_HoroFin datetime

	/*A controle parametres en entree */
	if @va_NumEvt_in = null or @va_CleEvt_in = null
		return XDC_ARG_INV

	select @va_Volume_out = null, @va_Longueur_out = null

	/*! creation d'une table de stockage des PR de tete et */
	/*! queue de bouchon */
	create table #BOU_PR (numero int, cle tinyint, 
	horodate datetime null, PR int null, cause int null, 
	cle_cause tinyint null)

	if not exists (select * from EXP..FMC_GEN
			where numero = @va_NumEvt_in and cle = @va_CleEvt_in and datex<>XDC_VRAI)
	begin
		/*B lecture des PR de debut de bouchon */
		insert #BOU_PR (numero, cle, horodate, PR)
		select 
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			HIS..FMC_HIS.horodate_validation,
			HIS..FMC_HIS.PR
		from HIS..FMC_GEN, HIS..FMC_HIS
		where	HIS..FMC_GEN.numero = @va_Cause_in and
			HIS..FMC_GEN.cle = @va_CleCause_in and
			HIS..FMC_GEN.datex<>XDC_VRAI and
			HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle = HIS..FMC_GEN.cle 
		
		if @@rowcount = 0
			return XDC_OK

		/*B lecture des PR de fin de bouchon */
		insert #BOU_PR (numero, cle, horodate, PR, cause, cle_cause)
		select 
			HIS..FMC_HIS.numero,
			HIS..FMC_HIS.cle,
			HIS..FMC_HIS.horodate_validation,
			HIS..FMC_HIS.PR,
			HIS..FMC_GEN.cause,
			HIS..FMC_GEN.cle_cause
		from HIS..FMC_GEN, HIS..FMC_HIS
		where	HIS..FMC_GEN.numero = @va_NumEvt_in and
			HIS..FMC_GEN.cle = @va_CleEvt_in and
			HIS..FMC_GEN.datex<>XDC_VRAI and
			HIS..FMC_HIS.numero = HIS..FMC_GEN.numero and
			HIS..FMC_HIS.cle = HIS..FMC_GEN.cle

		if @@rowcount = 0
			return XDC_OK
	end
	else
	begin
		/*B lecture des PR de debut de bouchon */
		insert #BOU_PR (numero, cle, horodate, PR)
		select 
			EXP..FMC_HIS.numero,
			EXP..FMC_HIS.cle,
			EXP..FMC_HIS.horodate_validation,
			EXP..FMC_HIS.PR
		from EXP..FMC_GEN, EXP..FMC_HIS
		where	EXP..FMC_GEN.numero = @va_Cause_in and
			EXP..FMC_GEN.cle = @va_CleCause_in and
			EXP..FMC_GEN.datex<>XDC_VRAI and
			EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
			EXP..FMC_HIS.cle = EXP..FMC_GEN.cle 

		if @@rowcount = 0
			return XDC_OK

		/*B lecture des PR de fin de bouchon */
		insert #BOU_PR (numero, cle, horodate, PR, cause, cle_cause)
		select 
			EXP..FMC_HIS.numero,
			EXP..FMC_HIS.cle,
			EXP..FMC_HIS.horodate_validation,
			EXP..FMC_HIS.PR,
			EXP..FMC_GEN.cause,
			EXP..FMC_GEN.cle_cause
		from EXP..FMC_GEN, EXP..FMC_HIS
		where	EXP..FMC_GEN.numero = @va_NumEvt_in and
			EXP..FMC_GEN.cle = @va_CleEvt_in and
			EXP..FMC_GEN.datex<>XDC_VRAI and
			EXP..FMC_HIS.numero = EXP..FMC_GEN.numero and
			EXP..FMC_HIS.cle = EXP..FMC_GEN.cle

		if @@rowcount = 0
			return XDC_OK
	end


	/*! creation d'une table de travail de stockage du graphe de bouchon */
	create table #GRA_BOU ( horodate datetime , horodate_fin datetime null, PR_debut int , PR_fin int)

	insert #GRA_BOU (horodate, horodate_fin, PR_debut, PR_fin)
	select distinct
		Q.horodate,
		null,
		T.PR,
		Q.PR
	from #BOU_PR T, #BOU_PR Q
	where	T.numero = Q.cause and
		T.cle = Q.cle_cause and
		T.horodate <= Q.horodate
	group by T.numero,T.cle, Q.numero,Q.cle,Q.horodate
	having	T.numero = Q.cause and
		T.cle = Q.cle_cause and
		T.horodate <= Q.horodate and
		T.horodate = max(T.horodate)

	insert #GRA_BOU (horodate, horodate_fin, PR_debut, PR_fin)
	select distinct
		T.horodate,
		null,
		T.PR,
		Q.PR
	from #BOU_PR T, #BOU_PR Q
	where	Q.cause = T.numero and
		Q.cle_cause = T.cle and
		Q.horodate <= T.horodate
	group by 
		Q.numero,Q.cle,
		T.numero,T.cle,T.horodate
	having	Q.cause = T.numero and
		Q.cle_cause = T.cle and
		Q.horodate <= T.horodate and
		Q.horodate = max(Q.horodate)

	select
		horodate = g2.horodate,
		horodate_fin = g1.horodate,
		g2.PR_debut,
		g2.PR_fin
	into #GRAPHE
	from #GRA_BOU g1, #GRA_BOU g2
	where g1.horodate > g2.horodate
	group by g2.horodate
	having g1.horodate > g2.horodate and
		g1.horodate = min(g1.horodate)

	/*A Calcul du volume d'encombrement */
	select distinct 
		horodate,
		horodate_fin,
		PR_debut,
		PR_fin,
		volume = @va_Longueur_out,
		longueur = abs(PR_debut - PR_fin)
	into #VOLUME
	from #GRAPHE

	/*A calcul du volume par validation */
	select @va_Volume_out = 0
	declare pointeur_zone cursor
	for select
		horodate,
		horodate_fin,
		PR_debut,
		PR_fin
	from #VOLUME

	open pointeur_zone
	fetch pointeur_zone into @vl_HoroDeb, @vl_HoroFin, @vl_PRDebut, @vl_PRFin
	while @@sqlstatus = 0
	begin
		/* calcul de la surface : longueur * nombre de voies */
		exec VUEM51 @va_Autoroute_in, @vl_PRDebut, @vl_PRFin, @va_Sens_in, @vl_Surface output

		select @va_Volume_out = @va_Volume_out + datediff(minute, @vl_HoroDeb, @vl_HoroFin) * @vl_Surface

		fetch pointeur_zone into @vl_HoroDeb, @vl_HoroFin, @vl_PRDebut, @vl_PRFin
	end
	close pointeur_zone

	/*A calcul de la longueur maximale du bouchon avec prise en compte des PR fantomes */
	select 
		@vl_PRDebut = PR_debut,
		@vl_PRFin = PR_fin
	from #VOLUME
	having longueur = max(longueur)
	exec XZAO;22 @va_Autoroute_in, @vl_PRDebut, @va_Autoroute_in, @vl_PRFin, @va_Longueur_out output

	return XDC_OK
go
