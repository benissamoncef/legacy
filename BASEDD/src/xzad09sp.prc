/*E*/
/*  Fichier : $Id: xzad09sp.prc,v 1.4 2010/09/01 19:22:59 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 2010/09/01 19:22:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzad09sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* ajout/retrait de la fmc de la table FRN_TDP.
*
------------------------------------------------------
* HISTORIQUE :
*
* P.N.	24/02/97	: Creation	(dem/tdp) (V 1.1)
* JPL	16/05/07	: Prise en compte fmc traitement chaussees et chaussee glissante (DEM 644,646) 1.2
* JPL	08/06/07	: Prise en compte Fmc Basculement pour Travaux (DEM 647) 1.3
* JPL	27/08/10	: Prise en compte Fmc Zone de stockage (DEM 942) 1.4
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* ajout/retrait de la fmc de la table FRN_TDP.
* 
* Sequence d'appel
* XZAD09_???
* 
* Arguments en entree
* XDY_Mot		va_Numero_in	: numero 
* XDY_Octet		va_Site_in	: site de recherche 
* XDY_Datetime		va_Horodate_in
* 
* Arguments en sortie
* XDY_Mot		va_Temps_out : dernier tdp trouve
* 
* Code retour
* XDC_OK
* XDC_NOK	: insertion ou modification impossible
* XDC_ARG_INV	: parametres avec valeur non autorisee ou
*                 numero inexistante
* 
* Conditions d'utilisation
* 
* Fonction
* Consultation dans la table TDP_6MN
-------------------------------------------------------*/

use PRC
go

drop procedure XZAD09
go


create procedure XZAD09
	@va_Evenement_in	int	= null,
	@va_Cle_in		tinyint = null,
	@va_Type_in		tinyint	= null,
	@va_Autoroute_in	tinyint	= null,
	@va_PR_in		int	= null,
	@va_Sens_in 		tinyint	= null
as
	declare @vl_Retour int,
		@vl_PRtete	T_PR,
		@vl_NumCause	int,
		@vl_CleCause	tinyint,
		@vl_Zdp		smallint,
		@vl_Site	tinyint,
		@vl_Debut	datetime,
		@vl_max_num int

	select @vl_Retour = XDC_OK
	select @vl_Zdp = null

	/*A controle parametres en entree */
	if @va_Evenement_in = null or @va_Cle_in = null 
		return XDC_ARG_INV

	if @va_Type_in=XZAEC_FMC_TeteBouchon
		return XDC_OK
		
	select @vl_max_num=count(*) from EXP..FRN_TDP
	
	if @vl_max_num <> 0
	begin
		select 
			@vl_max_num = max(numero) 
		from 	EXP..FRN_TDP
	end
		
	if @va_Type_in in (XZAEC_FMC_Travaux,
	                   XZAEC_FMC_ZoneStockage,
	                   XZAEC_FMC_BasculTravaux)
	begin
		select 
			@vl_PRtete = localisation_tete
		from  EXP..FMC_TRH
		where	numero =  @va_Evenement_in and 
			cle = @va_Cle_in 
		group by horodate
		having horodate = max(horodate)
	end
	else
	if @va_Type_in in (XZAEC_FMC_Meteo,
	                   XZAEC_FMC_TraitChaussees,
	                   XZAEC_FMC_ChausseeGliss)
	begin
		select 
			@vl_PRtete		= PR
		from  EXP..FMC_MET
		where	numero =  @va_Evenement_in and 
			cle = @va_Cle_in  
		group by horodate
		having horodate = max(horodate)
	end
	else
	begin
		select @vl_PRtete = @va_PR_in
	end
	
	
	declare frn cursor for
	select distinct
		zdp,
		site_zdp,
		debut
	from EXP..FRN_TDP FRN, CFG..EQT_GEN GEN, CFG..EQT_ZDP ZDP
	where
		GEN.numero=ZDP.numero and
		GEN.numero=FRN.zdp and
	     	GEN.type = XDC_EQT_ZDP and
		GEN.autoroute = @va_Autoroute_in and
		(GEN.sens = @va_Sens_in or @va_Sens_in = XDC_SENS_INCONNU ) and
		((GEN.PR <= @va_PR_in and ZDP.PR_fin >= @va_PR_in) or 
		 (GEN.PR <= @vl_PRtete and ZDP.PR_fin >= @vl_PRtete))  and
		FRN.fin = null 
		
	open frn
	
	fetch frn into	@vl_Zdp,
			@vl_Site,
			@vl_Debut
	
	while (@@sqlstatus = 0)
	begin
		if not exists ( select * from EXP..FRN_TDP
				where 	fin=null and 
			      		no_fmc = @va_Evenement_in and
			      		cle_fmc = @va_Cle_in and
			      		zdp = @vl_Zdp and
			      		site_zdp = @vl_Site)
		begin
		  		/*B incremente me dernier numero de franchissement */
				select  @vl_max_num = @vl_max_num + 1
				insert EXP..FRN_TDP (numero,zdp,site_zdp,no_fmc,cle_fmc,debut,fin)
				values (@vl_max_num,@vl_Zdp,@vl_Site,@va_Evenement_in,@va_Cle_in,@vl_Debut,null)
		end
		
	fetch frn into	@vl_Zdp,
			@vl_Site,
			@vl_Debut
	
	end

	delete EXP..FRN_TDP 
	where 	fin=null and 
		no_fmc = @va_Evenement_in and 
		cle_fmc = @va_Cle_in and
		zdp not in ( select zdp from EXP..FRN_TDP FRN, CFG..EQT_GEN GEN, CFG..EQT_ZDP ZDP
				where
					GEN.numero=ZDP.numero and
					GEN.numero=FRN.zdp and
	     				GEN.type = XDC_EQT_ZDP and
					GEN.autoroute = @va_Autoroute_in and
					(GEN.sens = @va_Sens_in or @va_Sens_in = XDC_SENS_INCONNU ) and
					((GEN.PR <= @va_PR_in and ZDP.PR_fin >= @va_PR_in) or 
					 (GEN.PR <= @vl_PRtete and ZDP.PR_fin >= @vl_PRtete))  and
					FRN.fin = null)
	

	close frn
	return @vl_Retour
go
