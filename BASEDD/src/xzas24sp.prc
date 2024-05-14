/*E*/
/*  Fichier : $Id: xzas24sp.prc,v 1.11 2012/12/19 11:31:38 gesconf Exp $     Release : $Revision: 1.11 $     Date : $Revision: 1.11 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas24sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Analyse 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.  06/12/94        : Creation      (V 1.1)
* B.G.  23/01/95        : modif date fin      (V 1.3)
* B.G.  01/02/95        : modif prevus      (V 1.4)
* C.T.  01/03/95        : Recherche infos dans HIS si date de
*                         calcul > 1 mois pour CI (V 1.5)
* C.T.  23/01/96        : Les travaux prevus sont exclus si la date de fin
*                         prevus est depassee (V1.6)
* P.N.  07/01/97	: Recuperation du nom de l'autoroute de la derniere validation (dem/1284) v1.7
* P.N.  29/06/98	: separartion EXP/HIS v1.8
* P.N.  05/02/99	: modif interges dem/1724 1.9
* JPL	11/06/07	: Retourne aussi les Basculements pour Travaux (DEM 647) 1.10
* PNI	19/12/12	: Duplication des #TMP_TRAV pour ne pas altérer le cursur FI 1691 1.11
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne la liste des travaux tries par autoroute et 
* PR croissants. Les travaux retournes peuvent etre fini ou non a une 
* heure donnee.
*
* Sequence d'appel
* SP    XZAS24_Liste_Travaux
*
* Arguments en entree :
* XDY_Octet 	@va_Travaux_Fini_in	: Indicateur permettant de
*					  determiner le critere de recherche
*					  "1" -> Travaux finis
*					  "2" -> Travaux finis ou non
*
* XDY_Datetime	@va_Date_in		: date a laquelle il faut effectuer
*					  la recherche.
* Arguments en sortie :
*
* Liste retournee :
* XDY_Entier	numero			: Numero de FMC
* XDY_Entier	cle			: Cle (numero du district)
* XDY_Octet	autoroute		: Numero de l'autoroute
* XDY_Sens	sens			: Sens de parcours
* XDY_Nom	lib_auto		: Nom de l'autoroute
* XDY_Nom	lib_district		: Code du district
* XDY_Datetime	debut			: Date de debut de travaux
* XDY_Datetime	fin			: Date de fin de travaux
* XDY_PR	PR_deb			: PR du debut de travaux
* XDY_PR	PR_fin			: PR de fin de fin de travaux
* XDY_Datetime	horodate_validation	: Date de validation
*
* Code retour
* XDC_OK
* XDC_NOK               : probleme d'acces aux donnees 
* XDC_ARG_INV           : parametres d'entree null, ou valeur non autorisee ou
*                         echangeur inexistante
*
* Conditions d'utilisation
* La liste retournee comprend des travaux obligatoirement finis ou non
* obligatoirement finis en fonction du choix qui est fait.
*
* Fonction
*
-------------------------------------------------------*/

use PRC
go

create procedure XZAS;24
	@va_Travaux_Fini_in	tinyint	= null,
	@va_Date_in		datetime = null
as
	declare @vl_Numero              int,     /* Numero d'evenement */
		@vl_Cle                 tinyint, /* Numero de cle */
		@vl_NumAutoroute     	tinyint, /* Numero d'autoroute */
		@vl_Sens             	tinyint, /* Sens de parcours */
		@vl_HorodateVal_Max     datetime,/* Date max de validation*/
		@vl_Horodate_Max        datetime,
		@vl_Status int,
		@vl_RechercheDsHis	bit

	/* Test argument d'entre */
	if @va_Travaux_Fini_in = 0	select @va_Travaux_Fini_in = null

	/*A
	** Test de coherence du type d'envenement a traiter.
	*/
	if @va_Travaux_Fini_in = null or
	   @va_Date_in = null or
	   @va_Travaux_Fini_in > 2 or
	   @va_Travaux_Fini_in <1
		return  XDC_ARG_INV


	/* initialisation varible locale */
	select @vl_RechercheDsHis = XDC_NON
#ifdef CI
	/*A
	** Au CI, si la date donnee est anterieure d'un mois au moins
	** rechercher les infos dans la base HIS sinon dans la base EXP
	*/
	if @va_Date_in < (select dateadd(month, -1, getdate ()))
	begin
		select @vl_RechercheDsHis = XDC_OUI
		exec @vl_Status = SQL_HIS.PRC..XZAS;24
			@va_Travaux_Fini_in ,
			@va_Date_in

		if @vl_Status = null
                       	return XDC_PRC_INC
                else return @vl_Status
	end
#endif
#ifdef HIST
		select @vl_RechercheDsHis = XDC_OUI
#endif

	/* Creation du table temporaire afin de concerver l'ensemble des */	
	/* fiches main courante de type travaux finis.			 */
	create table #FMC_TRAV (	numero	int		null,
					cle	tinyint 	null,
					autoroute smallint 	null,
					sens	tinyint 	null,
					lib_auto     char(10)   null,
					lib_district char(10)   null,
					debut	datetime	null,
					fin	datetime	null,
					PR_deb  int		null,
					PR_fin  int             null,
					horodate_validation datetime   null)
	create table #FMC_TRAV2 (	numero	int		null,
					cle	tinyint 	null,
					autoroute smallint 	null,
					sens	tinyint 	null,
					lib_auto     char(10)   null,
					lib_district char(10)   null,
					debut	datetime	null,
					fin	datetime	null,
					PR_deb  int		null,
					PR_fin  int             null,
					horodate_validation datetime   null)
	/*A
	** Recherche des fiches main courante de type "Travaux" 
	** dont la date de debut est inferieure a la date recherchee 
	** et dont la date de fin soit renseignee (pour les travaux finis)
	** ou non (pour les travaux finis ou en cours).
	*/
	if @va_Travaux_Fini_in = 1	/* Liste de travaux finis */
	begin
#ifndef HIST
			/* La date de fin doit etre renseignee */
			insert #FMC_TRAV (	numero,
						cle,
						autoroute,
						sens,
						debut,
						fin,
						PR_deb,
						horodate_validation)
			select 	distinct GEN.numero,
				 	GEN.cle,
				 	HIS.autoroute,
				 	HIS.sens,
				 	GEN.debut,
				 	GEN.fin,
				 	HIS.PR,
				 	HIS.horodate_validation
			from EXP..FMC_GEN GEN,EXP..FMC_HIS HIS
			where GEN.type in (XZAEC_FMC_Travaux, XZAEC_FMC_BasculTravaux)
			and   GEN.datex = XDC_FAUX
			and   GEN.debut <= @va_Date_in 
			and   GEN.fin   >= @va_Date_in 
			and   HIS.horodate_validation <= @va_Date_in
			and   HIS.cle = GEN.cle
			and   HIS.numero = GEN.numero
#else
			/* La date de fin doit etre renseignee */
			insert #FMC_TRAV (	numero,
						cle,
						autoroute,
						sens,
						debut,
						fin,
						PR_deb,
						horodate_validation)
			select 	distinct GEN.numero,
				 	GEN.cle,
				 	HIS.autoroute,
				 	HIS.sens,
				 	GEN.debut,
				 	GEN.fin,
				 	HIS.PR,
				 	HIS.horodate_validation
			from HIS..FMC_GEN GEN,HIS..FMC_HIS HIS
			where GEN.type in (XZAEC_FMC_Travaux, XZAEC_FMC_BasculTravaux)
			and   GEN.datex = XDC_FAUX
			and   GEN.debut <= @va_Date_in 
			and   GEN.fin   >= @va_Date_in 
			and   HIS.horodate_validation <= @va_Date_in
			and   HIS.cle = GEN.cle
			and   HIS.numero = GEN.numero
#endif
	end
	else
		/* Liste de travaux finis ou en cours */
		/* Pas de condition sur la date de fin */
#ifndef HIST
			insert #FMC_TRAV (      numero,
						cle,
						autoroute,
						sens,
						debut,
						fin,
						PR_deb,
						horodate_validation)
				select  distinct GEN.numero,
						GEN.cle,
						HIS.autoroute,
						HIS.sens,
						isnull(GEN.debut,GEN.debut_prevu),
						GEN.fin,
						HIS.PR,
						HIS.horodate_validation
				from EXP..FMC_GEN GEN,EXP..FMC_HIS HIS
				where GEN.type in (XZAEC_FMC_Travaux, XZAEC_FMC_BasculTravaux)
				and   GEN.datex = XDC_FAUX
				and   isnull(GEN.debut,GEN.debut_prevu) <= @va_Date_in
				and   (isnull(GEN.fin,GEN.fin_prevue)   >= @va_Date_in
				 or   isnull(GEN.fin,GEN.fin_prevue)    = null)
				and   HIS.horodate_validation <= @va_Date_in
				and   HIS.cle = GEN.cle
				and   HIS.numero = GEN.numero
#else
			insert #FMC_TRAV (      numero,
						cle,
						autoroute,
						sens,
						debut,
						fin,
						PR_deb,
						horodate_validation)
				select  distinct GEN.numero,
						GEN.cle,
						HIS.autoroute,
						HIS.sens,
						isnull(GEN.debut,GEN.debut_prevu),
						GEN.fin,
						HIS.PR,
						HIS.horodate_validation
				from HIS..FMC_GEN GEN,HIS..FMC_HIS HIS
				where GEN.type in (XZAEC_FMC_Travaux, XZAEC_FMC_BasculTravaux)
				and   GEN.datex = XDC_FAUX
				and   isnull(GEN.debut,GEN.debut_prevu) <= @va_Date_in
				and   (isnull(GEN.fin,GEN.fin_prevue)   >= @va_Date_in
				 or   isnull(GEN.fin,GEN.fin_prevue)    = null)
				and   HIS.horodate_validation <= @va_Date_in
				and   HIS.cle = GEN.cle
				and   HIS.numero = GEN.numero
#endif

	if @@rowcount = 0 
		return XDC_OK
	insert #FMC_TRAV2 select * from #FMC_TRAV

	/*A
	** Parcours de chacun des evenements "travaux" repondant aux criteres
	** requis.
	*/
	declare Travaux cursor for
		select distinct numero,cle,autoroute,sens
		from #FMC_TRAV
		group by numero,cle

	open Travaux

	/* Positionnement sur le premier enregistrement */
	fetch Travaux into 	@vl_Numero,
				@vl_Cle,
				@vl_NumAutoroute,
				@vl_Sens

	/* Tant qu'il exite de evenements de ce type */ 
	while @@sqlstatus = 0
	begin
		/*A
		** Recherche de l'horodate maximale de validation.
		** Ceci correspond a l'etat le plus recent.
		*/
		select @vl_HorodateVal_Max = max(horodate_validation)
		from #FMC_TRAV2
		where numero = @vl_Numero
		and   cle    = @vl_Cle

		/* Pour chaque FMC, conservation de l'etat le plus recent */
		/* uniquement. */
		delete #FMC_TRAV2
		where numero = @vl_Numero
		and   cle    = @vl_Cle
		and horodate_validation != @vl_HorodateVal_Max
		
		select @vl_NumAutoroute=autoroute
		from #FMC_TRAV2
		where numero = @vl_Numero
		and   cle    = @vl_Cle
		and horodate_validation = @vl_HorodateVal_Max
		

#ifndef HIST
		begin
			/*A
			** Recherche de l'horodate maximale (dernier etat).
			*/
			select @vl_Horodate_Max = max(horodate)
			from EXP..FMC_TRH
			where numero = @vl_Numero
			and   cle    = @vl_Cle

			/*A
			** Recherche des informations concernant l'evenement 
			** selectionne.
			*/
	
			/* Recheche du PR de fin de travaux */ 
			update #FMC_TRAV2
			set PR_fin = (	select TRH.localisation_tete
					from EXP..FMC_TRH TRH
					where TRH.numero = @vl_Numero
					and   TRH.cle    = @vl_Cle
					and   TRH.horodate = @vl_Horodate_Max),
			    lib_auto = ( select AUT.nom
					 from CFG..RES_AUT AUT
					 where AUT.numero = @vl_NumAutoroute),
			    lib_district = ( select DIS.code
					     from CFG..RES_DIS DIS
					     where DIS.numero = @vl_Cle)
			where numero = @vl_Numero
			and   cle    = @vl_Cle
			and horodate_validation = @vl_HorodateVal_Max
		end
#else
		begin
			/*A
			** Recherche de l'horodate maximale (dernier etat).
			*/
			select @vl_Horodate_Max = max(horodate)
			from HIS..FMC_TRH
			where numero = @vl_Numero
			and   cle    = @vl_Cle

			/*A
			** Recherche des informations concernant l'evenement 
			** selectionne.
			*/
	
			/* Recheche du PR de fin de travaux */ 
			update #FMC_TRAV2
			set PR_fin = (	select TRH.localisation_tete
					from HIS..FMC_TRH TRH
					where TRH.numero = @vl_Numero
					and   TRH.cle    = @vl_Cle
					and   TRH.horodate = @vl_Horodate_Max),
			    lib_auto = ( select AUT.nom
					 from CFG..RES_AUT AUT
					 where AUT.numero = @vl_NumAutoroute),
			    lib_district = ( select DIS.code
					     from CFG..RES_DIS DIS
					     where DIS.numero = @vl_Cle)
			where numero = @vl_Numero
			and   cle    = @vl_Cle
			and horodate_validation = @vl_HorodateVal_Max
		end
#endif

		/* Passage a l'enregistrement suivant */
		fetch Travaux into      @vl_Numero,
					@vl_Cle,
					@vl_NumAutoroute,
					@vl_Sens
	end
	/* Fermeture du curseur */
	close Travaux

	/*A
	** Affichage des resultats (retour au programme appelant)
	*/
	select * from #FMC_TRAV2
	order by lib_auto,sens,PR_deb

	/* Suppression des tables temporaires */
	drop table #FMC_TRAV 
	drop table #FMC_TRAV2

	/*A
	** Fin de la procedure.
	*/
 	return XDC_OK
go
