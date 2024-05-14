/*E*/
/*  Fichier : @(#)vuem07sp.prc	1.4      Release : 1.4        Date : 10/28/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem07sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	19/07/95	: Creation
* C.T.	08/12/95	: Suppression des null en retour (V 1.2)
*                         et optimisation
* C.T.	08/06/96	: Optimisation (V 1.3)
* V.L.	23/10/96	: recherche sur tous les districts (V 1.4)
* C.P.	04/02/98	: Modification du au pb FMC tete de bouchon (V 1.6)
* P.N.	05/02/99	: Modif interg. dem/1724 v1.7
* P.N.	09/02/99	: Suppression des PMV sur fourg , rem ... v1.8
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM07_Affichage_PMV
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in
* XDY_Horodate	va_Fin_in
* XDY_Site	va_Site_in
* 
* Arguments en sortie
*
* liste retournee
* XDC_NOM	district
* XDC_Entier	nombre
* XDC_Entier	duree
* XDC_Octet	mois
* 
* Service rendu
* Affichage PMV par mois/par district/par type fmc
* en nombre
* 
* Code retour
* XDC_OK
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

create procedure VUEM07
	@va_Debut_in	datetime	= null,
	@va_Fin_in	datetime	= null,
	@va_Site_in	T_SITE		= null
as
   declare @vl_Fin datetime, @vl_Debut datetime, 
	@vl_Mois tinyint, @vl_NomSite T_NOM,
	@vl_DebutSuivant datetime, @vl_Nombre int

   	/*A controle des parametres d'entree */
   	if @va_Debut_in is null  or @va_Fin_in is null
		return XDC_ARG_INV

	if @va_Site_in = 0 select @va_Site_in = null

   	create table #RECAP_PMV ( nombre int null,
				duree int null,
				nom_type char(25) null,
				mois tinyint null)

	create table #LISTE_ACTIONS (duree int, nom_type char(25), district tinyint, mois tinyint)

	select	@vl_Debut = @va_Debut_in,
		@vl_Fin = dateadd(month,1,@va_Fin_in),
		@vl_Mois = datepart(month,@va_Debut_in)

	/*B liste des PMV sur un mois */
	insert #LISTE_ACTIONS (duree, nom_type,district, mois)
	select 
		datediff(minute, HIS..ACT_GEN.heure_succes, HIS..ACT_GEN.heure_fin),
		CFG..TYP_FMC.nom,
		CFG..RES_POR.district,
		datepart(month,heure_succes)
	from HIS..FMC_GEN, HIS..ACT_GEN, CFG..TYP_FMC, 
		CFG..RES_POR, CFG..EQT_GEN, CFG..RES_AUT
	where	HIS..ACT_GEN.type = XDC_ACT_PMV and
		heure_succes >= @vl_Debut and heure_succes < @vl_Fin and
		heure_fin is not null and
		HIS..FMC_GEN.numero = HIS..ACT_GEN.evenement and
		HIS..FMC_GEN.cle = HIS..ACT_GEN.cle and
		CFG..EQT_GEN.type = HIS..ACT_GEN.type and
		CFG..EQT_GEN.numero = HIS..ACT_GEN.equipement and
		CFG..RES_POR.autoroute = CFG..EQT_GEN.autoroute and
		CFG..RES_POR.PR_debut <= CFG..EQT_GEN.PR and
		CFG..RES_POR.PR_fin > CFG..EQT_GEN.PR and
		(@va_Site_in is null or @va_Site_in=99 or CFG..RES_POR.district = @va_Site_in) and
		CFG..TYP_FMC.numero = HIS..FMC_GEN.type and
		HIS..FMC_GEN.type != XZAEC_FMC_TeteBouchon and
		((@va_Site_in=99 and HIS..FMC_GEN.datex=XDC_VRAI) or
		 (@va_Site_in<>99 and HIS..FMC_GEN.datex<>XDC_VRAI)) and
		CFG..RES_AUT.numero = CFG..EQT_GEN.autoroute

	/*B nombre d'encombrement pour un mois */
	insert #RECAP_PMV(nom_type, nombre, duree, mois)
	select 
		nom_type,
		count(*),
		sum(duree),
		mois
	from #LISTE_ACTIONS
	group by nom_type, mois


	/*A retourner la liste des infos */
	if @va_Debut_in = @va_Fin_in
		select 
			nom,
			isnull(nombre,0),
			isnull(duree,0),
			datepart(month, @va_Debut_in)
		from #RECAP_PMV, CFG..TYP_FMC
		where nom_type=*nom and numero!=XZAEC_FMC_TeteBouchon
		order by nom, mois
	else
	begin
		while (@va_Debut_in <= @va_Fin_in)
		begin
			select @vl_Mois = datepart(month, @va_Debut_in)

			insert #RECAP_PMV(nom_type, nombre, duree, mois)
			select 
				nom, 0, 0, @vl_Mois
			from #RECAP_PMV, CFG..TYP_FMC
			where nom_type =* nom and mois = @vl_Mois and numero!=XZAEC_FMC_TeteBouchon

			select @va_Debut_in = dateadd(month, 1, @va_Debut_in)
		end

		select
			nom_type,
			max(nombre),
			max(duree),
			mois
		from #RECAP_PMV
		group by nom_type, mois
		order by nom_type, mois
	end

	return XDC_OK
go
