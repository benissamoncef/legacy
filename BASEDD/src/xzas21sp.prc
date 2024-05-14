/*E*/
/*  Fichier : $Id: xzas21sp.prc,v 1.6 1998/09/22 13:30:59 gaborit Exp $  Release : $Revision: 1.6 $     Date : $Date: 1998/09/22 13:30:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas21sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Analyse 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.F.  29/11/94        : Creation      (V 1.1)
* C.T.  29/01/95        : Si pas de zone d'influence, les retards
*                         sont 0, si un retard ne peut etre calcule
*                         les autres sont toutefois calcules (V 1.2)
* C.T.  01/03/95        : Recherche infos dans HIS si date de
*                         calcul > un mois pour CI (V 1.3)
* B.G.	06/11/95	: optimisation (vitesse) (1.4)
* L.V.	26/08/96	: optimisation de la precision des retards
*			en multipliant pa 1000 puis en divisant a
*			la fin par 1000 (1.5)
* P.N.	29/06/98	: separation EXP/HIS 1.6
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne le retard de parcours entre 2 echangeurs consecutifs
* et situes sur le meme autoroute.
*
* Sequence d'appel
* SP    XZAS21
*
* Arguments en entree :
* XDY_PR	va_PRDeb_in		: PR du premier echangeur
* XDY_PR        va_PRFin_in             : PR du second echangeur
* XDY_Datetime	va_Horodate_in		: date de calcul (Heure H)
* XDT_Sens	va_Sens_in		: sens de parcours
*
* Arguments en sortie :
* XDY_Mot	va_RetardH_out		: retard de parcours (Heure H)
* XDY_Mot	va_RetardHM1_out        : retard de parcours (Heure H-1)
* XDY_Mot	va_RetardHP1_out        : retard de parcours (Heure H+1)
*		(Temps de reference - temps de parcours)
*		- retard = 0 s'il n'y a pas de retard
*               - retard > 0 s'il y a eu du retard
*               - retard < 0 si le retard n'est pas calculable
*
* Liste retournee :
*
* Code retour
* XDC_OK
* XDC_NOK               : probleme d'acces aux donnees
* XDC_ARG_INV           : parametres d'entree null, ou valeur non autorisee ou
*                         echangeur inexistante
*
* Conditions d'utilisation
* Procedure appele par la procedure xzas20sp.prc
* Dans le cas ou un retard ne peut etre calcule, il est value a null
*
* Fonction
*
-------------------------------------------------------*/

use PRC
go

create table #COEFF(
	indice  int,
	coeff	float
	)
go

create procedure XZAS;21
	@va_PRDeb_in	          int       	 = null,
	@va_PRFin_in              int       	 = null,
	@va_Autoroute_in	  tinyint	 = null,
	@va_Horodate_in    	  datetime       = null,
	@va_Sens_in		  tinyint	 = null,
	@va_RetardH_out		  int output,
	@va_RetardHM1_out         int output,
	@va_RetardHP1_out         int output
as
	declare @vl_CountH int,                 /* Compteur (H, H-1 ou H+1) */
		@vl_Indice int,                 /* Indice des coefficients  */
		@vl_NumStation int,		/* Numero de station */
		@vl_Vitesse float,		/* Vitesse */
		@vl_Vitesse_inter float,       /* Vitesse intermediaire */
		@vl_Vitesse_L float,            /* Vitesse libre */
		@vl_Horodate datetime,		/* Date (multiple de 6mn) */
		@vl_HorodateH datetime,         /* Date de reference H,H-1,H+1*/
		@vl_Erreur bit,
		@vl_Status int,
		@vl_RechercheDsHis bit

	/* Initialisation du retard */
	select @va_RetardH_out = -1
	select @va_RetardHM1_out = -1
	select @va_RetardHP1_out = -1
 
	/* Initialisation de var locale */
	select @vl_RechercheDsHis = XDC_NON

#ifdef CI
	/*A
	** si la date donnee est anterieure d'un mois au moins
	** rechercher les infos dans la base HIS sinon dans la base EXP
	*/
	if @va_Horodate_in < (select dateadd(month, -1, getdate ()))
	begin
		select @vl_RechercheDsHis = XDC_OUI
		exec @vl_Status = SQL_HIS.PRC..XZAS;21
			@va_PRDeb_in	          ,
			@va_PRFin_in              ,
			@va_Autoroute_in	  ,
			@va_Horodate_in    	  ,
			@va_Sens_in		  ,
			@va_RetardH_out		   output,
			@va_RetardHM1_out          output,
			@va_RetardHP1_out          output

		if @vl_Status = null
                       	return XDC_PRC_INC
                else return @vl_Status
	end
#endif

#ifdef HIST
		select @vl_RechercheDsHis = XDC_OUI
#endif
	/*A Controle des parametres en entree */
	if @va_PRDeb_in = null or
	   @va_PRFin_in = null or
	   @va_Autoroute_in = null or
	   @va_Horodate_in = null or
	   @va_Sens_in = null
		return XDC_ARG_INV

	/*A Recherche des zones d'influences comprises entre les deux 	*/
	/*A echangeurs.							*/


	/* Creation d'une table temporaire pour conserver les informations */
	/* concernant les zones d'influences selectionnees		   */
	create table #PORTION_ZI(
				station		int	null,
	 			longueur	int	null,
				retardH    	int	null,
				retardHM1       int     null,
				retardHP1       int     null,
				pr_zi_deb	int,
				pr_zi_fin	int
				)

	/*A Recherche des zones d'influences */
	insert #PORTION_ZI (
		    station,
		    pr_zi_deb,
		    pr_zi_fin)
	select distinct
		CFG..RES_INF.station,
		CFG..RES_INF.PR_debut,
		CFG..RES_INF.PR_fin
	from	CFG..RES_INF,
		CFG..RES_POR
	where	CFG..RES_INF.portion=CFG..RES_POR.numero
	  and	CFG..RES_POR.autoroute=@va_Autoroute_in
	  and	CFG..RES_INF.sens=@va_Sens_in
	  and	CFG..RES_INF.PR_debut < @va_PRFin_in
	  and	CFG..RES_INF.PR_fin > @va_PRDeb_in

	/*A ramene les ZI au PR des echangeurs pour celles qui depassent */

	update #PORTION_ZI
	set pr_zi_deb=@va_PRDeb_in
	where pr_zi_deb<@va_PRDeb_in

	update #PORTION_ZI
	set pr_zi_fin=@va_PRFin_in
	where pr_zi_fin>@va_PRFin_in

	/*A elimine les ZI de longueur nulle */
	
	delete #PORTION_ZI
	where pr_zi_deb>=pr_zi_fin
	
	/*A calcule la longueur des ZI */

	update #PORTION_ZI
	set longueur=pr_zi_fin-pr_zi_deb

	select @vl_NumStation=null

	set rowcount 1
	select @vl_NumStation=station
	from #PORTION_ZI
	where retardH = null
	set rowcount 0

	while (@vl_NumStation <>null)
	begin

		/*A Recherche de la vitesse libre */
		select @vl_Vitesse_L = VTL
		from CFG..EQT_RAD_VTL
		where station = @vl_NumStation
		and sens = @va_Sens_in

		/*! traitement si une mesure de vitesse en base */
		if @@rowcount != 0
		begin
		   /*A Calcul des retard H, H-1 et H+1 */
		   select @vl_CountH = 0

		   /* Calcul des 3 retards */ 
		   while (@vl_CountH < 3)
		   begin
		      /* Test sur la date a prendre en compte (H, H-1 ou H+1) */
		      /* vl_CountH: 0 = H, 1 = H-1 ou 2 = H+1 */
		      if (@vl_CountH = 1)
		      begin
		        /* Retard d'une heure (H-1) */
		        select @vl_HorodateH = dateadd(hour,-1, @va_Horodate_in)
		      end
		      else
		      begin
		        /* Estimation (H+1)  et heure H */
		        select @vl_HorodateH = @va_Horodate_in
		      end

		      /* Date arrondie aux 6mn inferieures */
#ifndef HIST
		      	select @vl_Horodate = max(horodate)
	  	      	from EXP..MES_TRA
		      	where station = @vl_NumStation
		      	and sens = @va_Sens_in
			and voie = 0 
		      	and horodate <= @vl_HorodateH
#else
		      	select @vl_Horodate = max(horodate)
	  	      	from HIS..MES_TRA
		      	where station = @vl_NumStation
		      	and sens = @va_Sens_in
			and voie = 0 
		      	and horodate <= @vl_HorodateH
#endif

		      if @@rowcount = 0
		   	return XDC_NOK 

		      select @vl_Vitesse = 0

		      /*A Recherche de la vitesse de parcours */
		      if (@vl_CountH = 2) /* Heure H+1 (vitesse estimee) */
		      begin
		        select @vl_Indice = 0

		        /* Debut de l'estimation */
		        select @vl_Erreur = XDC_NON
		        while (@vl_Indice <= 10 and @vl_Erreur = XDC_NON)
		        begin
#ifndef HIST
		        		select @vl_Vitesse_inter = V
		        		from EXP..MES_TRA
		        		where station = @vl_NumStation 
		        		and sens = @va_Sens_in
		        		and voie = 0
		        		and horodate = @vl_Horodate
#else
		        		select @vl_Vitesse_inter = V
		        		from HIS..MES_TRA
		        		where station = @vl_NumStation 
		        		and sens = @va_Sens_in
		        		and voie = 0
		        		and horodate = @vl_Horodate
#endif

		   		if @@rowcount = 0
		   		select	@vl_Erreur = XDC_OUI, 
		   			@vl_Vitesse = -1 

		   		select @vl_Vitesse_inter = @vl_Vitesse_inter *
		   		(select coeff from #COEFF
		   		 where indice = @vl_Indice)

		        	/* Changement de date (H-6mn) */ 
		        	select @vl_Horodate =
		   			dateadd(minute,-6, @vl_Horodate)

		        	select @vl_Vitesse = @vl_Vitesse + @vl_Vitesse_inter

		   		/* Passage au coefficient suivant */
		   		select @vl_Indice = @vl_Indice +1
		        end /* Fin de l'estimation */

		      end
		      else		/* Heures H et H-1 (vitesse reelle ) */
		      begin
#ifndef HIST
		      		select @vl_Vitesse = V
		      		from EXP..MES_TRA
		      		where station = @vl_NumStation
		      		and sens = @va_Sens_in
		      		and voie = 0
		      		and horodate = @vl_Horodate 
#else
		      		select @vl_Vitesse = V
		      		from HIS..MES_TRA
		      		where station = @vl_NumStation
		      		and sens = @va_Sens_in
		      		and voie = 0
		      		and horodate = @vl_Horodate 
#endif

		   	if @@rowcount = 0
		   		select @vl_Vitesse = -1

		      end

		      /* Le retard est calcule en seconde et les vitesse */
		      /* recueillies sont en Km/h (conversion en m/s)    */

		      /*A Calcul du retard pour une ZI */
		      if ((@vl_Vitesse > 0) and (@vl_Vitesse_L > 0))
		      begin
		      	/* vl_CountH: 0 = H, 1 = H-1 ou 2 = H+1 */
		      	if (@vl_CountH = 1)
		      	begin
		   		/* Retard d'une heure */
				update #PORTION_ZI
				set retardHM1 = (3600*longueur)/@vl_Vitesse - (3600*longueur)/@vl_Vitesse_L
				where station = @vl_NumStation
		      	end
		      	else if (@vl_CountH = 2)
	  	      	begin
		   		/* Estimation */
				update #PORTION_ZI
				set retardHP1 = (3600*longueur)/@vl_Vitesse - (3600*longueur)/@vl_Vitesse_L
				where station = @vl_NumStation
		      	end
		      	else
		      	begin
		   		/* Heure H */
				update #PORTION_ZI
				set retardH = (3600*longueur)/@vl_Vitesse - (3600*longueur)/@vl_Vitesse_L
				where station = @vl_NumStation
		      	end
		      end
		      else
		      begin
		      	/* vl_CountH: 0 = H, 1 = H-1 ou 2 = H+1 */
		      	if (@vl_CountH = 1)
		      	begin
		   		/* Retard d'une heure */
				update #PORTION_ZI
				set retardHM1 = -1
				where station = @vl_NumStation
		      	end
		      	else if (@vl_CountH = 2)
	  	      	begin
		   		/* Estimation */
				update #PORTION_ZI
				set retardHP1 = -1
				where station = @vl_NumStation
		      	end
		      	else
		      	begin
		   		/* Heure H */
				update #PORTION_ZI
				set retardH = -1
				where station = @vl_NumStation
		      	end
		      end


		      select @vl_CountH = @vl_CountH + 1
		   end
		   /* Calcul des retard H, H-1 et H+1 */

		end
		else /* pas de vtl en base pour cette station */
			update #PORTION_ZI
			set retardHM1 = -1,
			    retardHP1 = -1,
			    retardH = -1
			where station = @vl_NumStation

		update #PORTION_ZI
		set retardH = 0
		where station = @vl_NumStation
		  and retardH=null

		select @vl_NumStation=null
		
		set rowcount 1
		select @vl_NumStation=station
		from #PORTION_ZI
		where retardH = null
		set rowcount 0
	end
	/* Fin de parcours ZI */

	/*A Calcul du retard  total (temps de reference - temps de parcours) */ 
	/*B Le retard est toujours superieur ou egale a zero */

	update #PORTION_ZI
	set retardH = 0
	where retardH < 0 or retardH = null

	update #PORTION_ZI
	set retardHM1 = 0
	where retardHM1 < 0 or retardHM1 = null

	update #PORTION_ZI
	set retardHP1 = 0
	where retardHP1 < 0 or retardHP1 = null

	/* Le retard global entre les deux echangeurs = somme des retards */
	/* calcule pour chacune des ZI */
	/* heure H */
	begin
		select @va_RetardH_out = sum(retardH)/1000 
		from #PORTION_ZI
		where retardH >= 0
	
		/* heure H moins 1 */
		select @va_RetardHM1_out = sum(retardHM1)/1000
		from #PORTION_ZI
		where retardHM1 >= 0
	
		/* heure H plus 1 */
		select @va_RetardHP1_out = sum(retardHP1)/1000
		from #PORTION_ZI
		where retardHP1 >= 0

		if @va_RetardH_out is null
			select @va_RetardH_out = 0
		if @va_RetardHM1_out is null
			select @va_RetardHM1_out = 0
		if @va_RetardHP1_out is null
			select @va_RetardHP1_out = 0
	end

	/* Destruction de la table temporaire, une fois les calculs termines */
	drop table #PORTION_ZI

 	return XDC_OK
go
drop table #COEFF
go
