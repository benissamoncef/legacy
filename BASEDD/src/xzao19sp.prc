/*E*/
/*  Fichier : $Id: xzao19sp.prc,v 1.7 2007/03/26 15:43:20 gesconf Exp $      Release : $Revision: 1.7 $        Date : $Date: 2007/03/26 15:43:20 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao19sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de la configuration du reseau 
* d'autoroute 
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.  07/11/94        : Creation (v1.1)
* C.T.  10/01/95        : Ajout une date en entree (v1.2)
*                         pour la formation
* B.G.  19/01/95        : modif curseur (v1.3)
* B.G.	24/10/95	: gestion du sens inverse A57 (1.4)
* Guilhou 07/11/97	: parametre de sortie indique bra voie lente (dem/1459) 1.5
* PN	 29/06/98	: separation EXP/HIS 1.6
* PC	 24/02/05	: au ci on appelle la proc du ci et plus celle de his 1.7 (REPERE1 et 2)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Recherche la "numero" NAV non inhibee en amont (>0) ou
* en aval (<0). 
* -------------------------------------------------------
* Sequence d'appel
* SP	XZAO19_NAV_amont
* 
* Arguments en entree
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Mot		va_Numero_in
* XDY_Horodate		va_Horodate_in
* 
* Arguments en sortie
* XDY_Eqt		va_NumNAV_out
* XDY_District		va_District_out
* XDY_Autoroute		va_Autoroute_out
* XDY_PR		va_PR_out
* XDY_Sens		va_Sens_out
* XDY_Entier		va_Distance_out
* XDY_PR		va_PRInfluence_out
* XDY_Octet		va_Type_out
*
* Code retour
* XDC_OK
* XDC_NOK
* <0 erreur sybase 
* 
* Conditions d'utilisation
* 
---------------------------------------------------------*/

use PRC
go

create procedure XZAO;19
	@va_Autoroute_in	T_AUTOROUTE = null,
	@va_PR_in		T_PR = null,
	@va_Sens_in		tinyint = null,
	@va_Numero_in		smallint = null,
	@va_NumNAV_out		smallint = null output,
	@va_District_out	tinyint = null output,
	@va_Autoroute_out	T_AUTOROUTE = null output,
	@va_PR_out		T_PR = null output,
	@va_Sens_out		tinyint = null output,
	@va_Distance_out	int = null output,
	@va_PRInfluence_out	int = null output,
	@va_Type_out		tinyint = null output,
	@va_Horodate_in		datetime = null
as
declare @vl_NumEqt smallint, @vl_PR T_PR, @vl_Site T_SITE,
	@vl_Horodate datetime, @vl_Type tinyint, @vl_Status int

/*A Controle des parametres d'entree obligatoire */

	if	@va_Autoroute_in = null or
		@va_PR_in = null or
		@va_Sens_in = null or
		@va_Numero_in = null or
		@va_Numero_in = 0

	return XDC_ARG_INV

/*A Declaration des variables locales */

	declare	@vl_cr int

/*A Ram…ne le sens sur section courante s'il s'agit d'une sortie ou d'une entree */

	if	@va_Sens_in=XDC_SENS_SORTIE_NORD or
		@va_Sens_in=XDC_SENS_ENTREE_NORD
	select	@va_Sens_in=XDC_SENS_NORD
	
	if	@va_Sens_in=XDC_SENS_SORTIE_SUD or
		@va_Sens_in=XDC_SENS_ENTREE_SUD
	select	@va_Sens_in=XDC_SENS_SUD

/*A Si le sens est inconnu : retourne null (pas d'equipement amont) */
	
	if	@va_Sens_in<>XDC_SENS_NORD and @va_Sens_in<>XDC_SENS_SUD
	return XDC_OK
	
/*A Si l'autoroute est inversee, change le signe de @va_Numero_in (A57) */

	if	exists (select * from CFG..RES_AUT_INV where numero=@va_Autoroute_in and inversee=XDC_PR_INV)
	select	@va_Numero_in=-@va_Numero_in
	
/*A Suivant le sens et le signe de @va_Numero_in, declare un curseur asc ou desc */
/*REPERE1#ifdef CI
	exec @vl_Status = SQL_HIS.PRC..XZAO;19
	@va_Autoroute_in,
	@va_PR_in	,
	@va_Sens_in	,
	@va_Numero_in	,
	@va_NumNAV_out	output,
	@va_District_out output,
	@va_Autoroute_out output,
	@va_PR_out	output,
	@va_Sens_out	output,
	@va_Distance_out output,
	@va_PRInfluence_out output,
	@va_Type_out	output,
	@va_Horodate_in	

	if @vl_Status = null
                 return XDC_PRC_INC
        else return @vl_Status
        
#elseFIN REPERE1*/
#ifdef HIST

		create table #EQT_NAV (numero smallint, PR int, sit tinyint, type tinyint)
	
		if	( @va_Sens_in=XDC_SENS_SUD and @va_Numero_in>0 ) or
			( @va_Sens_in=XDC_SENS_NORD and @va_Numero_in<0 )
		begin
			declare Eqt cursor for
				select 	EQT.numero,
					EQT.PR,
					NAV.type
				from	CFG..EQT_GEN EQT,
					CFG..EQT_NAV NAV
				where	EQT.type=XDC_EQT_NAV and
					EQT.autoroute=@va_Autoroute_in and
					EQT.sens=@va_Sens_in and
					NAV.numero=EQT.numero and 
					EQT.PR<@va_PR_in  	-- en amont (SUD) ou aval (NORD)
				order by EQT.PR desc 		-- le plus proche en premier
		end
		else
		begin
			declare Eqt cursor for
				select 	EQT.numero,
					EQT.PR,
					NAV.type
				from	CFG..EQT_GEN EQT,
					CFG..EQT_NAV NAV
				where	EQT.type=XDC_EQT_NAV and
					EQT.autoroute=@va_Autoroute_in and
					EQT.sens=@va_Sens_in and
					NAV.numero=EQT.numero and
					EQT.PR>@va_PR_in  	-- en aval (SUD) ou amont (NORD)
				order by EQT.PR asc 		-- le plus proche en premier
		end
	
		open Eqt
		fetch Eqt into @vl_NumEqt, @vl_PR, @vl_Type
		while (@@sqlstatus = 0)
		begin
			/* recherche la derniere date avant la date donnee */
			select
				@vl_Horodate = max(horodate) 
			from HIS..EQT_DSP DSP
			where	DSP.equipement = @vl_NumEqt and
				DSP.type = XDC_EQT_NAV and
				DSP.horodate <= @va_Horodate_in
	
			/* tester si l'equipement est disponible   */
			/* a la derniere date avant la date donnee */
			select distinct
				@vl_Site = DSP.sit
			from HIS..EQT_DSP DSP
			where	DSP.horodate = @vl_Horodate and
				DSP.equipement = @vl_NumEqt and
				DSP.type = XDC_EQT_NAV and
				DSP.inhibe=0 and 	-- non inhibe
				DSP.desactive=0 and 	-- non desactive
				DSP.HS=0 		-- non HS
	
			if @@rowcount != 0
				insert #EQT_NAV	
				values(@vl_NumEqt, @vl_PR, @vl_Site, @vl_Type)
	
			/* passage a l'equipement suivant */
			fetch Eqt into @vl_NumEqt, @vl_PR, @vl_Type
		end
		close Eqt
	
		if	( @va_Sens_in=XDC_SENS_SUD and @va_Numero_in>0 ) or
			( @va_Sens_in=XDC_SENS_NORD and @va_Numero_in<0 )
			declare Equipement cursor for
			select numero, PR, sit, type from #EQT_NAV
			order by #EQT_NAV.PR desc 		-- le plus proche en premier
		else
			declare Equipement cursor for
			select numero, PR, sit, type from #EQT_NAV
			order by #EQT_NAV.PR asc 		-- le plus proche en premier
#else
/*REPERE2	if @va_Horodate_in is null or @@servername<>XDC_SQLCI FIN REPERE2*/
	if @va_Horodate_in is null or @va_Horodate_in is not null
	begin
		if	( @va_Sens_in=XDC_SENS_SUD and @va_Numero_in>0 ) or
			( @va_Sens_in=XDC_SENS_NORD and @va_Numero_in<0 )
		begin
			declare Equipement cursor for
				select 	EQT.numero,
					EQT.PR,
					DSP.sit,
					NAV.type
				from	CFG..EQT_GEN EQT,
					EXP..EQT_DSP DSP,
					CFG..EQT_NAV NAV
				where	EQT.type=XDC_EQT_NAV and
					EQT.autoroute=@va_Autoroute_in and
					EQT.sens=@va_Sens_in and
					EQT.PR<@va_PR_in and 	-- en amont (SUD) ou aval (NORD)
					NAV.numero=EQT.numero and
					DSP.equipement=EQT.numero and
					DSP.type=XDC_EQT_NAV and
					DSP.dernier=1 and 	-- derniere dispo connue
					DSP.inhibe=0 and 	-- non inhibe
					DSP.desactive=0 and 	-- non desactive
					DSP.HS=0 		-- non HS
				order by EQT.PR desc 		-- le plus proche en premier
		end
		else
		begin
			declare Equipement cursor for
				select 	EQT.numero,
					EQT.PR,
					DSP.sit,
					NAV.type
				from	CFG..EQT_GEN EQT,
					EXP..EQT_DSP DSP,
					CFG..EQT_NAV NAV
				where	EQT.type=XDC_EQT_NAV and
					EQT.autoroute=@va_Autoroute_in and
					EQT.sens=@va_Sens_in and
					EQT.PR>@va_PR_in and 	-- en aval (SUD) ou amont (NORD)
					NAV.numero = EQT.numero and 
					DSP.equipement=EQT.numero and
					DSP.type=XDC_EQT_NAV and
					DSP.dernier=1 and 	-- derniere dispo connue
					DSP.inhibe=0 and 	-- non inhibe
					DSP.desactive=0 and 	-- non desactive
					DSP.HS=0 		-- non HS
				order by EQT.PR asc 		-- le plus proche en premier
		end
	end

#endif
	
	select @va_Numero_in=abs(@va_Numero_in)

	open Equipement

	fetch Equipement into	@va_NumNAV_out,
				@va_PR_out,
				@va_District_out,
				@va_Type_out

	while @@sqlstatus = 0 and @va_Numero_in > 1
	begin
		fetch Equipement into	@va_NumNAV_out,
					@va_PR_out,
					@va_District_out,
					@va_Type_out

		select @va_Numero_in=@va_Numero_in-1
	end
	close Equipement
	
/*A  si on a trouv≈ un equipement : recherche distance, autoroute, sens et PRInfluence */

	if @@sqlstatus = 0
	begin
		select 	@va_Autoroute_out=@va_Autoroute_in,
			@va_Sens_out=@va_Sens_in

		exec @vl_cr=XZAO;22 	@va_Autoroute_out,
					@va_PR_out,
					@va_Autoroute_in,
					@va_PR_in,
					@va_Distance_out output

		if @vl_cr<>XDC_OK return XDC_NOK
		select  @va_PRInfluence_out=PR_influence
		from CFG..EQT_NAV
		where numero=@va_NumNAV_out
	end
	
/*A  sinon on positionne les param…tres retour » null */

	else
	select 	@va_NumNAV_out=null,
		@va_PR_out=null,
		@va_District_out=null,
		@va_Type_out=null

	return XDC_OK

go
