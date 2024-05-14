/*E*/
/*  Fichier : $Id: xzas100sp.prc,v 1.3 2013/03/29 10:54:11 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2013/03/29 10:54:11 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas100sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   PROC POUR TEST TDP RADT
*
------------------------------------------------------
* HISTORIQUE :
*
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Calucl le temps de parcours entre 2 PR d'une autoroute pour un sens
* 
* Sequence d'appel
* SP	XZAS100
* 
* Arguments en entree
* XDY_Autoroute	va_Autoroute_in		: numero d'autoroute
* XDY_PR	va_PRDebut_in		
* XDY_PR	va_PRFin_in		
* XDY_Datetime	va_HorodateDebut_in	: date de debut des mesures multiple de 6mn
* 
* Arguments en sortie
*
* Liste retournee :
* XDY_Horodate	Horodate	: date de validation
* XDY_PR	PRDebut		: PR de debut de bouchon
* XDY_PR	PRFin		: PR de fin de bouchon
* 
* Code retour
* XDC_OK
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee 
* 
* Conditions d'utilisation
*
* Fonction
* 
-------------------------------------------------------*/

use PRC
go
drop proc XZAS100
go

create procedure XZAS100
	@va_Autoroute_in	tinyint		= null,
	@va_PRDebut_in		T_PR		= null,
	@va_PRFin_in		T_PR		= null,
	@va_Sens_in		tinyint		= null,
	@va_Tdp_out		int	output
as
	declare @vl_Type tinyint, @vl_station smallint, @vl_sens T_SENS, @vl_Sens_inv T_SENS,
		@vl_Cause int, @vl_CleCause tinyint,
		@vl_Numero int, @vl_Cle tinyint,
		@vl_debut T_PR, @vl_fin T_PR, @vl_Horodate datetime, @vl_HorodateDebut datetime,
		@vl_HorodateFin datetime, @vl_DateFin datetime, @vl_Status int,
	@vl_temps_partiel float,	@vl_temps int, @vl_distance T_PR,
	@vl_ratio float,
	@vl_pond float,
	@vl_nb_peage int,
	@vl_long int,
	@vl_v tinyint

	/*A controle parametres en entree */
	if @va_Autoroute_in = null or @va_PRDebut_in = null or
	    @va_PRFin_in = null or
	   @va_PRDebut_in = @va_PRFin_in
		return XDC_ARG_INV

	select @vl_temps=0
	select @vl_Horodate=getdate()

	/* Recherches des zones inscrites entre les PR */
	declare zones cursor for
	select station,sens,distance=abs(Z.PR_fin-Z.PR_debut),Z.PR_debut,Z.PR_fin
	from CFG..RES_INF Z, CFG..RES_POR P
	where 	P.numero=Z.portion and
		P.autoroute=@va_Autoroute_in and
		Z.sens=@va_Sens_in and
		 @va_PRDebut_in <= Z.PR_debut  and Z.PR_fin <= @va_PRFin_in

	open zones
	fetch zones into @vl_station, @vl_sens, @vl_distance,@vl_debut,@vl_fin
	while (@@sqlstatus = 0 )
	begin
		select 	@vl_v=0
		select 	@vl_v=V
		from EXP..MES_TRA
		where station=@vl_station and sens=@vl_sens and voie=2
		having station=@vl_station and sens=@vl_sens and voie=2 and (horodate<=@vl_Horodate and horodate > dateadd(minute,-6,@vl_Horodate))

		if @vl_v>0
			select @vl_temps_partiel=((@vl_distance*60*60)/isnull(@vl_v,120))
		else
		begin
			select @vl_v=120
			select @vl_temps_partiel=((@vl_distance*60*60)/120)
		end

select "station=",@vl_station,@vl_temps_partiel,@vl_v,@vl_distance,@vl_debut,@vl_fin
		select @vl_ratio=1
		/* Recherhe si il existe un bouchon ds la zone et hors pt de mesure */
		select @vl_Type=B.type,@vl_Numero=G.numero,@vl_Cle=G.cle,@vl_long=H.longueur
		from EXP..FMC_GEN G, EXP..FMC_BOU B, EXP..FMC_HIS_DER H, EXP..FMC_HIS_DER T, CFG..RES_CHA S, CFG..EQT_GEN E
		where G.fin=null and G.type=XZAEC_FMC_QueueBouchon and (G.numero=B.numero and G.cle=B.cle) and
			(H.numero=G.numero and H.cle=G.cle) and
			(T.numero=G.cause and T.cle=G.cle_cause) and
			E.numero=@vl_station and E.type=XDC_EQT_RAD and E.nom not like "RDP%" and S.station=E.numero and
			H.autoroute=E.autoroute and H.sens=S.sens and 
			/*sens2*/ 
			(((S.sens=XDC_SENS_NORD) and (T.PR>S.PR or H.PR<S.PR) and ((@vl_debut < T.PR and T.PR <= @vl_fin) or ((@vl_debut < H.PR and H.PR <= @vl_fin)))) or
			/*sens1*/
			((S.sens=XDC_SENS_SUD) and (H.PR>S.PR or T.PR<S.PR) and ((@vl_debut < T.PR and T.PR <= @vl_fin) or ((@vl_debut < H.PR and H.PR <= @vl_fin)))))

/*				(((H.PR > T.PR) and T.PR >= @vl_debut) or ((H.PR<T.PR) and H.PR >= @vl_debut)) and
				(((H.PR >T.PR) and H.PR <= @vl_fin) or ((H.PR<T.PR) and T.PR <= @vl_fin)) and /*Dans la zone */
			( (((H.PR>T.PR) and T.PR > S.PR) or ((H.PR<T.PR) and H.PR > S.PR)) or (((H.PR>T.PR) and H.PR < S.PR )or ((H.PR<T.PR) and T.PR < S.PR )))*/ /* hors pt de mesure */

		if @@rowcount != 0
		begin
			if @vl_Type=XZAEC_BOU_Accordeon select @vl_ratio=0.85
			if @vl_Type=XZAEC_BOU_Dense select @vl_ratio=0.75
			if @vl_Type=XZAEC_BOU_Ralenti select @vl_ratio=0.5
			if @vl_Type=XZAEC_BOU_Bouche select @vl_ratio=0.2
			if @vl_Type=XZAEC_BOU_Bloque select @vl_ratio=0.1
			select @vl_pond=(convert(float,@vl_long)/( @vl_fin-@vl_debut))
			if @vl_pond > 1 select @vl_pond=1

			select "prise en cpte du bouchon1 ",@vl_Numero,"/",@vl_Cle,"=> ratio=",@vl_ratio, @vl_long,( @vl_fin-@vl_debut)
			/*select @vl_ratio=@vl_ratio*@vl_pond+(1.0-@vl_pond)*/
			select "prise en cpte du bouchon1 ",@vl_Numero,"/",@vl_Cle,"=> ratio=",@vl_ratio
		end

		select @vl_temps=@vl_temps+(@vl_temps_partiel/@vl_ratio)
select @vl_temps/1000,@vl_temps_partiel/1000,@vl_Horodate,@vl_distance,@vl_debut,@vl_fin,@vl_v,((@vl_distance*60*60)/isnull(@vl_v*1000,120000)/1000), (@vl_temps)/60/1000 , "mn",(@vl_temps/1000)%60,"sec"
select @vl_temps,@vl_temps_partiel

		fetch zones into @vl_station, @vl_sens, @vl_distance,@vl_debut,@vl_fin
	end
	close zones

	/* Recherches de la zone contenant le PR_debut */
	select @vl_station=station, @vl_sens=sens, @vl_distance=abs(Z.PR_fin-@va_PRDebut_in),@vl_debut=Z.PR_debut,@vl_fin=Z.PR_fin
	from CFG..RES_INF Z, CFG..RES_POR P
	where 	P.numero=Z.portion and
		P.autoroute=@va_Autoroute_in and
		Z.sens=@va_Sens_in and
		Z.PR_fin >  @va_PRDebut_in and  @va_PRDebut_in >  Z.PR_debut 

	if @@rowcount != 0
	begin
		select 	@vl_v=0
		select 	@vl_v=V
		from EXP..MES_TRA
		where station=@vl_station and sens=@vl_sens and voie=2
		having station=@vl_station and sens=@vl_sens and voie=2 and horodate=max(horodate)

		if @vl_v>0
			select @vl_temps_partiel=((@vl_distance*60*60)/isnull(@vl_v,120))
		else
		begin
			select @vl_v=120
			select @vl_temps_partiel=((@vl_distance*60*60)/120)
		end



select "station=2",@vl_station,@vl_temps_partiel,@vl_v,@vl_distance,@vl_debut,@vl_fin
		select @vl_ratio=1
		/* Recherhe si il existe un bouchon ds la zone et hors pt de mesure */
		select @vl_Type=B.type,@vl_Numero=G.numero,@vl_Cle=G.cle,@vl_long=H.longueur
		from EXP..FMC_GEN G, EXP..FMC_BOU B, EXP..FMC_HIS_DER H, EXP..FMC_HIS_DER T, CFG..RES_CHA S, CFG..EQT_GEN E
		where G.fin=null and G.type=XZAEC_FMC_QueueBouchon and (G.numero=B.numero and G.cle=B.cle) and
			(H.numero=G.numero and H.cle=G.cle) and
			(T.numero=G.cause and T.cle=G.cle_cause) and
			E.numero=@vl_station and E.type=XDC_EQT_RAD and E.nom not like "RDP%" and S.station=E.numero and 
			H.autoroute=E.autoroute and H.sens=S.sens and 
			/*sens2*/ 
			(((S.sens=XDC_SENS_NORD) and (T.PR>S.PR or H.PR<S.PR) and ((@vl_debut < T.PR and T.PR <= @vl_fin) or ((@vl_debut < H.PR and H.PR <= @vl_fin)))) or
			/*sens1*/
			((S.sens=XDC_SENS_SUD) and (H.PR>S.PR or T.PR<S.PR) and ((@vl_debut < T.PR and T.PR <= @vl_fin) or ((@vl_debut < H.PR and H.PR <= @vl_fin)))))
/*				(((H.PR > T.PR) and T.PR >= @vl_debut) or ((H.PR<T.PR) and H.PR >= @vl_debut)) and
				(((H.PR >T.PR) and H.PR <= @vl_fin) or ((H.PR<T.PR) and T.PR <= @vl_fin)) and /*Dans la zone */
			( (((H.PR>T.PR) and T.PR > S.PR) or ((H.PR<T.PR) and H.PR > S.PR)) or (((H.PR>T.PR) and H.PR < S.PR )or ((H.PR<T.PR) and T.PR < S.PR )))*/ /* hors pt de mesure */

		if @@rowcount != 0
		begin
			if @vl_Type=XZAEC_BOU_Accordeon select @vl_ratio=0.85
			if @vl_Type=XZAEC_BOU_Dense select @vl_ratio=0.75
			if @vl_Type=XZAEC_BOU_Ralenti select @vl_ratio=0.5
			if @vl_Type=XZAEC_BOU_Bouche select @vl_ratio=0.2
			if @vl_Type=XZAEC_BOU_Bloque select @vl_ratio=0.1
			select "prise en cpte du bouchon2 ",@vl_Numero,"/",@vl_Cle,"=> ratio=",@vl_ratio
		end

		select @vl_temps=@vl_temps+(@vl_temps_partiel/@vl_ratio)
--select @vl_temps,@vl_temps_partiel,@vl_distance
		
	end

	/* Recherches de la zone contenant le PRfin */
	select @vl_station=station,@vl_sens=sens,@vl_distance=abs(Z.PR_debut-@va_PRFin_in),@vl_debut=Z.PR_debut,@vl_fin=Z.PR_fin
	from CFG..RES_INF Z, CFG..RES_POR P
	where 	P.numero=Z.portion and
		P.autoroute=@va_Autoroute_in and
		Z.sens=@va_Sens_in and
		Z.PR_fin >  @va_PRFin_in and  @va_PRFin_in >  Z.PR_debut 

	if @@rowcount != 0
	begin
		select 	@vl_v=0
		select 	@vl_v=V
		from EXP..MES_TRA
		where station=@vl_station and sens=@vl_sens and voie=2
		having station=@vl_station and sens=@vl_sens and voie=2 and horodate=max(horodate)


		if @vl_v>0
			select @vl_temps_partiel=((@vl_distance*60*60)/isnull(@vl_v,120))
		else
		begin
			select @vl_v=120
			select @vl_temps_partiel=((@vl_distance*60*60)/120)
		end

		select @vl_ratio=1
		/* Recherhe si il existe un bouchon ds la zone et hors pt de mesure */
select "station3=",@vl_station,@vl_temps_partiel,@vl_v,@vl_distance,@vl_debut,@vl_fin
		select @vl_Type=B.type,@vl_Numero=G.numero,@vl_Cle=G.cle,@vl_long=H.longueur
		from EXP..FMC_GEN G, EXP..FMC_BOU B, EXP..FMC_HIS_DER H, EXP..FMC_HIS_DER T, CFG..RES_CHA S, CFG..EQT_GEN E
		where G.fin=null and G.type=XZAEC_FMC_QueueBouchon and (G.numero=B.numero and G.cle=B.cle) and
			(H.numero=G.numero and H.cle=G.cle) and
			(T.numero=G.cause and T.cle=G.cle_cause) and
			E.numero=@vl_station and E.type=XDC_EQT_RAD and  E.nom not like "RDP%" and S.station=E.numero and
			H.autoroute=E.autoroute and H.sens=S.sens and 
			/*sens2*/ 
			(((S.sens=XDC_SENS_NORD) and (T.PR>S.PR or H.PR<S.PR) and ((@vl_debut < T.PR and T.PR <= @vl_fin) or ((@vl_debut < H.PR and H.PR <= @vl_fin)))) or
			/*sens1*/
			((S.sens=XDC_SENS_SUD) and (H.PR>S.PR or T.PR<S.PR) and ((@vl_debut < T.PR and T.PR <= @vl_fin) or ((@vl_debut < H.PR and H.PR <= @vl_fin)))))

/*				(((H.PR > T.PR) and T.PR >= @vl_debut) or ((H.PR<T.PR) and H.PR >= @vl_debut)) and
				(((H.PR >T.PR) and H.PR <= @vl_fin) or ((H.PR<T.PR) and T.PR <= @vl_fin)) and */ 
/*			( (((H.PR>T.PR) and T.PR > S.PR) or ((H.PR<T.PR) and H.PR > S.PR)) or (((H.PR>T.PR) and H.PR < S.PR )or ((H.PR<T.PR) and T.PR < S.PR )))*/ /* hors pt de mesure */


		if @@rowcount != 0
		begin
			select "Bouchon rencontr� de type ", @vl_Type
			if @vl_Type=XZAEC_BOU_Accordeon select @vl_ratio=0.85
			if @vl_Type=XZAEC_BOU_Dense select @vl_ratio=0.75
			if @vl_Type=XZAEC_BOU_Ralenti select @vl_ratio=0.5
			if @vl_Type=XZAEC_BOU_Bouche select @vl_ratio=0.2
			if @vl_Type=XZAEC_BOU_Bloque select @vl_ratio=0.1
--			select @vl_ratio=@vl_ratio/convert(float,(convert(float,@vl_long)/( @vl_fin-@vl_debut)))
			select "prise en cpte du bouchon3 ",@vl_Numero,"/",@vl_Cle,"=> ratio=",@vl_ratio

		end

		select @vl_temps=@vl_temps+(@vl_temps_partiel/@vl_ratio)
--select @vl_temps,@vl_temps_partiel,@vl_distance
		
	end
	select @vl_temps=@vl_temps/1000


	/* Ajout du temps pass� au peage : 1mn par p�age */
	select @vl_nb_peage=count(*) from CFG..RES_PEA PEA, CFG..RES_POR POR
	where 	POR.numero=PEA.portion and autoroute=@va_Autoroute_in and
		( (@va_PRDebut_in < PRsens1 and @va_PRFin_in > PRsens1 and @va_Sens_in=XDC_SENS_SUD) or (@va_PRDebut_in < PRsens2 and @va_PRFin_in > PRsens2 and @va_Sens_in=XDC_SENS_NORD)) and
		supprime=XDC_FAUX
	
	select "Nb de peages rencontres : " , 	@vl_nb_peage
	select @vl_temps=@vl_temps+(@vl_nb_peage*60)
		
--	select "RDT:",@vl_temps, @vl_temps/60 , "mn",@vl_temps%60,"sec"
	select @va_Tdp_out=@vl_temps

	return XDC_OK
go
