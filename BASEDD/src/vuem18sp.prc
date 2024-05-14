/*E*/
/* Fichier : $Id: vuem18sp.prc,v 1.7 2016/09/16 20:10:26 devgfi Exp $        Release : $Revision: 1.7 $        Date : $Date: 2016/09/16 20:10:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem18sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	19/07/95	: Creation
* C.T.	21/06/96	: Ajout cas tous les districts (V1.2)
*                         et tous les mois de l'annee
* C.P.	06/01/98	: Rectification de la procedure pour
			eviter le doublements d'actions a PR variable
* P.N.	05/02/99	: Modif interg. dem/1724 v1.18
* JMG	21/09/99	: action sur remorque : l'autoroute est dans la table
* JPL	16/09/16	: Cas PMV sur remorque, 'Tous sites' : plus de doublons si PR action hors reseau  1.7
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM18_Affichage_PMV
* 
* Arguments en entree
* XDY_Horodate	va_Debut_in
* XDY_Horodate	va_Fin_in
* XDY_Octet	va_Site_in
* XDY_Octet	va_TypeAffichage_in
* 
* Arguments en sortie
*
* liste retournee
* XDC_NOM	type fmc
* XDC_Entier	nombre
* XDC_Entier	duree
* XDC_Octet	mois
* 
* Service rendu
* Affichage PMV sur remorque ou affichage en gare 
* par mois/par district/par type fmc
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

if exists (select 1 from sysobjects where name = 'VUEM18' and type = 'P')
	drop procedure VUEM18
go


create procedure VUEM18
	@va_Debut_in	datetime	= null,
	@va_Fin_in	datetime	= null,
	@va_Site_in	T_SITE		= null,
	@va_TypeAffichagePMV_in bit	= null
as
   declare @vl_Fin datetime, 
	@vl_Mois tinyint, @vl_NomSite T_NOM, @vl_Nombre int,
	@vl_Duree int, @vl_Autoroute T_AUTOROUTE,
	@vl_PR T_PR, @vl_Type T_NOM

	/*! correction bug applix */
   	if @va_Site_in = 0 select @va_Site_in = null

   	/*A controle des parametres d'entree */
   	if @va_Debut_in is null and @va_Fin_in is null 
		return XDC_ARG_INV

   	create table #RECAP_PMV ( nombre int null,
				duree int null,
				nom_type char(25) null,
				mois tinyint,
				sit tinyint null)

	create table #LISTE_ACTIONS (mois tinyint, duree int, nom_type char(25),numero int null,date datetime null)

	select	@vl_Fin = dateadd(month,1,@va_Fin_in)

	if @va_TypeAffichagePMV_in = XDC_VRAI
		/*B liste des affichages PMV sur un mois */
		insert #LISTE_ACTIONS (mois, duree, nom_type, numero, date)
		select
			datepart(month,heure_succes),
			datediff(minute, HIS..ACT_GEN.heure_succes, HIS..ACT_GEN.heure_fin),
			CFG..TYP_FMC.nom,
			HIS..FMC_GEN.numero,
			heure_succes
		from HIS..FMC_GEN, HIS..ACT_GEN, CFG..TYP_FMC, 
			HIS..ACT_RMQ, CFG..RES_POR, HIS..FMC_HIS
		where	HIS..ACT_GEN.type = XDC_ACT_PMV_REMORQUE and
			HIS..ACT_GEN.numero = HIS..ACT_RMQ.actio and
			HIS..ACT_GEN.sit = HIS..ACT_RMQ.sit and
			heure_succes >= @va_Debut_in and 
			heure_succes < @vl_Fin and
			heure_fin is not null and
			HIS..FMC_GEN.numero = HIS..ACT_GEN.evenement and
			HIS..FMC_GEN.cle = HIS..ACT_GEN.cle and
			((@va_Site_in=99 and HIS..FMC_GEN.datex=XDC_VRAI) or
		 	 (@va_Site_in<>99 and HIS..FMC_GEN.datex<>XDC_VRAI)) and
			HIS..FMC_HIS.numero = HIS..ACT_GEN.evenement and
			HIS..FMC_HIS.cle = HIS..ACT_GEN.cle and
			/*CFG..RES_POR.autoroute = HIS..FMC_HIS.autoroute and*/
			CFG..RES_POR.autoroute = HIS..ACT_RMQ.autoroute and
			CFG..RES_POR.PR_debut <= HIS..ACT_RMQ.PR and
			CFG..RES_POR.PR_fin > HIS..ACT_RMQ.PR and
			(@va_Site_in is null or @va_Site_in = 99 or CFG..RES_POR.district = @va_Site_in) and
			CFG..TYP_FMC.numero = HIS..FMC_GEN.type 
		group by HIS..FMC_GEN.numero, HIS..FMC_GEN.cle,
			HIS..FMC_HIS.numero, HIS..FMC_HIS.cle,
			HIS..ACT_GEN.numero, HIS..ACT_GEN.sit,
			HIS..ACT_GEN.evenement, HIS..ACT_GEN.cle,
			HIS..ACT_RMQ.actio, HIS..ACT_RMQ.sit,
			CFG..TYP_FMC.numero
		having
			HIS..ACT_GEN.type = XDC_ACT_PMV_REMORQUE and
			HIS..ACT_GEN.numero = HIS..ACT_RMQ.actio and
			HIS..ACT_GEN.sit = HIS..ACT_RMQ.sit and
			heure_succes >= @va_Debut_in and heure_succes < @vl_Fin and
			heure_fin is not null and
			HIS..FMC_GEN.numero = HIS..ACT_GEN.evenement and
			HIS..FMC_GEN.cle = HIS..ACT_GEN.cle and
			((@va_Site_in=99 and HIS..FMC_GEN.datex=XDC_VRAI) or
		 	 (@va_Site_in<>99 and HIS..FMC_GEN.datex<>XDC_VRAI)) and
			HIS..FMC_HIS.numero = HIS..ACT_GEN.evenement and
			HIS..FMC_HIS.cle = HIS..ACT_GEN.cle and
			horodate_validation = max(horodate_validation) and
			/*CFG..RES_POR.autoroute = HIS..FMC_HIS.autoroute and*/
			CFG..RES_POR.autoroute = HIS..ACT_RMQ.autoroute and
			CFG..RES_POR.PR_debut <= HIS..ACT_RMQ.PR and
			CFG..RES_POR.PR_fin > HIS..ACT_RMQ.PR and
			(@va_Site_in is null or @va_Site_in = 99 or CFG..RES_POR.district = @va_Site_in) and
			CFG..TYP_FMC.numero = HIS..FMC_GEN.type and
			CFG..RES_POR.numero = min(CFG..RES_POR.numero)
	else
	begin
		/*B liste des affichages en gare sur un mois */
		insert #LISTE_ACTIONS (mois, duree, nom_type,numero,date) 
		select 
			datepart(month,heure_succes),
			datediff(minute, HIS..ACT_GEN.heure_succes, HIS..ACT_GEN.heure_fin),
			CFG..TYP_FMC.nom,
			HIS..FMC_GEN.numero,
			heure_succes
		from HIS..FMC_GEN, HIS..ACT_GEN, CFG..TYP_FMC, 
			CFG..RES_POR, HIS..FMC_HIS
		where	HIS..ACT_GEN.type = XDC_ACT_AFF_GARE and
			heure_succes >= @va_Debut_in and heure_succes < @vl_Fin and
			heure_fin is not null and
			HIS..FMC_GEN.numero = HIS..ACT_GEN.evenement and
			HIS..FMC_GEN.cle = HIS..ACT_GEN.cle and
			HIS..FMC_HIS.numero = HIS..ACT_GEN.evenement and
			HIS..FMC_HIS.cle = HIS..ACT_GEN.cle and
			((@va_Site_in=99 and HIS..FMC_GEN.datex=XDC_VRAI) or
		 	 (@va_Site_in<>99 and HIS..FMC_GEN.datex<>XDC_VRAI)) and
			CFG..RES_POR.autoroute = HIS..FMC_HIS.autoroute and
			CFG..RES_POR.PR_debut <= HIS..FMC_HIS.PR and
			CFG..RES_POR.PR_fin > HIS..FMC_HIS.PR and
			(@va_Site_in is null or @va_Site_in = 99 or CFG..RES_POR.district = @va_Site_in) and
			CFG..TYP_FMC.numero = HIS..FMC_GEN.type 
		group by HIS..FMC_GEN.numero, HIS..FMC_GEN.cle,
			HIS..FMC_HIS.numero, HIS..FMC_HIS.cle,
			HIS..ACT_GEN.numero,  HIS..ACT_GEN.sit,
			HIS..ACT_GEN.evenement, HIS..ACT_GEN.cle,
			CFG..TYP_FMC.numero, 
			CFG..RES_POR.autoroute
		having 
			HIS..ACT_GEN.type = XDC_ACT_AFF_GARE and
			heure_succes >= @va_Debut_in and heure_succes < @vl_Fin and
			heure_fin is not null and
			HIS..FMC_GEN.numero = HIS..ACT_GEN.evenement and
			HIS..FMC_GEN.cle = HIS..ACT_GEN.cle and
			((@va_Site_in=99 and HIS..FMC_GEN.datex=XDC_VRAI) or
		 	 (@va_Site_in<>99 and HIS..FMC_GEN.datex<>XDC_VRAI)) and
			HIS..FMC_HIS.numero = HIS..ACT_GEN.evenement and
			HIS..FMC_HIS.cle = HIS..ACT_GEN.cle and
			horodate_validation = max(horodate_validation) and
			CFG..RES_POR.autoroute = HIS..FMC_HIS.autoroute and
			CFG..RES_POR.PR_debut <= HIS..FMC_HIS.PR and
			CFG..RES_POR.PR_fin > HIS..FMC_HIS.PR and
			(@va_Site_in is null or @va_Site_in = 99 or CFG..RES_POR.district = @va_Site_in) and
			CFG..TYP_FMC.numero = HIS..FMC_GEN.type 
	end


	/*B nombre d'encombrement pour un mois */
	insert #RECAP_PMV(nom_type, nombre, duree, mois)
	select 
		nom_type,
		count(*),
		sum(duree),
		mois
	from #LISTE_ACTIONS
	group by nom_type, mois

	--select * from #LISTE_ACTIONS
	--select * from #RECAP_PMV


	/*A retourner la liste des infos */
	if @va_Debut_in = @va_Fin_in
	begin
		select @vl_Mois = datepart(month,@va_Debut_in)

		select 
			nom,
			isnull(nombre,0),
			isnull(duree,0),
			isnull(mois, @vl_Mois)
		from #RECAP_PMV, CFG..TYP_FMC
		where nom_type=*nom and numero!=XZAEC_FMC_TeteBouchon
		order by nom, mois
	end
	else
	begin
		while (@va_Debut_in <= @va_Fin_in)
		begin
			select @vl_Mois = datepart(month, @va_Debut_in)
			insert #RECAP_PMV(nom_type, nombre, duree, mois)
			select
				nom, 0, 0, @vl_Mois
			from #RECAP_PMV, CFG..TYP_FMC
			where nom_type=*nom and numero!=XZAEC_FMC_TeteBouchon

			select @va_Debut_in = dateadd(month, 1, @va_Debut_in)
		end

		select
			nom_type,
			max(nombre),
			max(duree),
			mois
		from #RECAP_PMV, CFG..TYP_FMC
		group by nom_type, mois
		order by nom_type, mois
	end

	return XDC_OK
go
