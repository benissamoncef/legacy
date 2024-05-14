/*E*/
/*  Fichier : @(#)vuemb15sp.prc	1.6      Release : 1.6        Date : 02/20/97
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuemb15sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	30/08/95	: Creation
* C.T.	26/09/95	: Correction recherche d'appels (V 1.2)
* C.T.	02/10/95	: Correction recherche d'appels (V 1.3)
* P.N.  08/10/96	: Modification des noms d'autoroutes (dem/1230) v1.4
* L.V.	12/02/97	: Renvoie des donnees par un selecy distinct v1.5
* L.V.	20/02/97	: Test homogene avec vuem15 sur les dates v1.6
* C.T.	24/02/98	: Modif suite pb defualt suite passage en SQL 11.0.3.2 (v1.8)
* P.N.	05/02/99	: Modif interg. dem/1724 v1.9
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEMb15_NbAppels
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in
* 
* Arguments en sortie
* 
* Service rendu
* Nombre d'appels a depanneurs par mois
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

create procedure VUEMb15
	@va_Debut_in	datetime	= null,
	@va_Datex_in	tinyint		= null
as
	declare @vl_Fin datetime, @vl_Evt int, @vl_CleEvt tinyint, 
		@vl_Autoroute T_AUTOROUTE, @vl_PR T_PR, @vl_Site T_SITE

	/*A controle des parametres d'entree */
	if @va_Debut_in is null
		return XDC_ARG_INV

	select @vl_Fin = dateadd(month, 1, @va_Debut_in)

	create table #RECAP_DEP (sit tinyint, autoroute tinyint, nombre int default 0)

	insert #RECAP_DEP (sit, autoroute, nombre)
	select distinct district, autoroute, 0
	from CFG..RES_POR


	/*A rechercher les actions depanneurs sur le mois */
	declare pointeur_action cursor
	for select
		HIS..ACT_GEN.evenement,
		HIS..ACT_GEN.cle
	from HIS..ACT_GEN, HIS..FMC_GEN
	where HIS..ACT_GEN.type = XDC_ACT_DEPANNEUR and 
		heure_succes < @vl_Fin and 
		heure_fin >= @va_Debut_in and
		HIS..FMC_GEN.cloture >= @va_Debut_in and
		HIS..FMC_GEN.numero = evenement and
		HIS..FMC_GEN.cle = HIS..ACT_GEN.cle and
		((@va_Datex_in=XDC_VRAI and HIS..FMC_GEN.datex=XDC_VRAI) or
		 (@va_Datex_in<>XDC_VRAI and HIS..FMC_GEN.datex<>XDC_VRAI)) 
	open pointeur_action

	fetch pointeur_action into @vl_Evt, @vl_CleEvt
	while @@sqlstatus = 0
	begin
		/* rechercher la derniere localisation */
		select
			@vl_Autoroute = autoroute,
			@vl_PR = PR
		from HIS..FMC_HIS
		where numero =@vl_Evt and cle = @vl_CleEvt 
		group by numero,cle,horodate_validation
		having horodate_validation=max(horodate_validation)
		
		/* rechercher le district */
		select
			@vl_Site = district
		from CFG..RES_POR
		where autoroute = @vl_Autoroute and
			PR_debut <= @vl_PR and 
			PR_fin > @vl_PR

		if @@rowcount = 1
		begin
			update #RECAP_DEP set  nombre = nombre + 1
			where autoroute = @vl_Autoroute and sit = @vl_Site
		end
		
		fetch pointeur_action into @vl_Evt, @vl_CleEvt
	end
	close pointeur_action


	/* ordonner suivant A8 puis par district */
	create table #LISTE (nom char(4), district char(25), nombre int)
	insert #LISTE (nom , district, nombre )
	select
		CFG..RES_AUT.nom,
		CFG..RES_DIS.nom,
		nombre
	from #RECAP_DEP, CFG..RES_AUT, CFG..RES_DIS
	where CFG..RES_AUT.numero = autoroute and
		CFG..RES_DIS.numero = #RECAP_DEP.sit and
		(CFG..RES_AUT.nom ="A8" or CFG..RES_AUT.nom ="A500" or
		CFG..RES_AUT.nom ="A57")
	order by autoroute, CFG..RES_DIS.nom

	insert into #LISTE
	select
		CFG..RES_AUT.nom,
		CFG..RES_DIS.nom,
		nombre
	from #RECAP_DEP, CFG..RES_AUT, CFG..RES_DIS
	where CFG..RES_AUT.numero = autoroute and
		CFG..RES_DIS.numero = #RECAP_DEP.sit and
		CFG..RES_AUT.nom !="A8" and CFG..RES_AUT.nom !="A51" and
		CFG..RES_AUT.nom !="A500" and CFG..RES_AUT.nom !="A57"
	order by CFG..RES_DIS.nom, autoroute

	insert into #LISTE
	select
		CFG..RES_AUT.nom,
		CFG..RES_DIS.nom,
		nombre
	from #RECAP_DEP, CFG..RES_AUT, CFG..RES_DIS
	where CFG..RES_AUT.numero = autoroute and
		CFG..RES_DIS.numero = #RECAP_DEP.sit and
		CFG..RES_AUT.nom ="A51"
	order by CFG..RES_DIS.nom, autoroute

	/*A retourner la liste */
	select nom,district,nombre
	from #LISTE

	return XDC_OK
go
