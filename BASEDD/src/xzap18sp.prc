/*E*/
/*Fichier : $Id: xzap18sp.prc,v 1.7 2016/09/21 10:39:37 devgfi Exp $      $Revision: 1.7 $        $Date: 2016/09/21 10:39:37 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER xzap18sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	25 Aug 1994	: Creation
* gaborit	05 dec 1994	: resultats output a la place de select
*                                 + numero de fiche annuaire, etc
* gaborit	06 dec 1994	: correction rowcount (1.3)
* guilhou	18 nov 1996	: refonte de la gestion des astreintes (DEM/1306) 1.4
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.5
* JPL		09/10/2014	: Degroupee; prise en compte du sens  (DEM 1100)  1.6
* JPL		20/09/2016	: Support des deux ordres des PR de zones, independamment du sens (DEM 1186)  1.7
* JPL		02/07/21	: Extension du champ "remarque" a 250 caracteres (SAE-287)  1.8
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAP18' and type = 'P')
	drop procedure XZAP18
go


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lit une proposition d'appel telephone ou bip d'un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP18

	@va_NumPA_in		smallint,
	@va_Ordre_in		tinyint,
	@va_Remarque_out	char(250) output,
	@va_Explication_out	char(250) output,
	@va_PR_out		int output,
	@va_Autoroute_out	tinyint output,
	@va_Type_out		tinyint output,
	@va_Horodate_out	datetime output
	/*@va_NumFic_out 		smallint=null output,
	@va_NomFic_out 		char(25)=null output,
	@va_NumCmp_out 		smallint=null output*/
	
	
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		va_NumPA_in
* XDY_Octet 		va_Ordre_in
*
* ARGUMENTS EN SORTIE :
*
* XZAOPT_PropTel 	va_PropTel
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* FONCTION :
*
* Lit une proposition d'appel telephone ou bip d'un plan d'actions
*
* MECANISMES :
*
------------------------------------------------------*/


as 

#ifdef HIST
declare @vl_Status int, @vl_Site tinyint

	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP18
			@va_NumPA_in		,
			@va_Ordre_in		,
			@va_Remarque_out	 output,
			@va_Explication_out	 output,
			@va_PR_out		 output,
			@va_Autoroute_out	 output,
			@va_Type_out		 output,
			@va_Horodate_out	 output

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP18
			@va_NumPA_in		,
			@va_Ordre_in		,
			@va_Remarque_out	 output,
			@va_Explication_out	 output,
			@va_PR_out		 output,
			@va_Autoroute_out	 output,
			@va_Type_out		 output,
			@va_Horodate_out	 output

	if @vl_Status = null
		return XDC_PRC_INC
	else
		return @vl_Status

#else
    declare @vl_Heure datetime, @vl_HeureCar char(9),
   	@vl_Type tinyint,
	@vl_SousType char(25),@vl_NumInt smallint,
	@vl_Num smallint,@vl_NomInt char(25),
	@vl_Compt smallint,
	@vl_Sens tinyint

   /*A
   ** Si une des variables d'entree est nulle alors retourne XDC_ARG_INV
   */
   
   if @va_NumPA_in = null
   or @va_Ordre_in = null
  
      return XDC_ARG_INV
  
   else
   
   /*A
   ** Si les variables d'entree sont non nulles
   ** Lecture d'une Proposition APL d'un plan d'action
   */
   
   begin
      
      select 	@va_Remarque_out=rtrim(remarque),
      		@va_Explication_out=explication,
      		@va_PR_out=PR,
      		@va_Autoroute_out=autoroute,
      		@va_Type_out=type_d_astreinte,
		@vl_Sens=sens
   		
   	from EXP..SAD_APL
   
        where numero = @va_NumPA_in 
          and ordre = @va_Ordre_in

	select 	@va_Horodate_out=horodate
	from EXP..SAD_PLN
	where numero = @va_NumPA_in

/*A recherche de la competence correspondante */
       	create table #COM (
		num              smallint null,
		type             tinyint null,
		sous_type        char(25) null,
		district         tinyint null)
		  
	create table #FICHE (
		competence	smallint null,
		interlocuteur	smallint null,
		nom             char(25) null)

	create table #RESULTAT (
		sous_type        char(25) null,
		nom              char(25) null,
		numint           smallint null,
		numero           smallint null)

	insert #COM (num,sous_type)
	select distinct   	CMP.numero,
				CMP.sous_type_d_astreinte
	from  	CFG..ANN_AST_CMP CMP,
      		CFG..ANN_GEO GEO
	where CMP.type_d_astreinte = @va_Type_out
        and (( GEO.competence = CMP.numero
              and (GEO.autoroute = @va_Autoroute_out and
                   (case when GEO.PR_debut <= GEO.PR_fin then GEO.PR_debut else GEO.PR_fin end) <= @va_PR_out and
                   (case when GEO.PR_debut <= GEO.PR_fin then GEO.PR_fin else GEO.PR_debut end) >= @va_PR_out and
                   ( GEO.sens = XDC_LES_2_SENS  or  GEO.sens = @vl_Sens ) )
              )
	      or CMP.numero not in (select competence from CFG..ANN_GEO)
	)

	insert #FICHE
		select CFG..ANN_AST_CAL.competence,interlocuteur,nom
		from CFG..ANN_AST_CAL, #COM, CFG..ANN_FIC
		where date_debut<=@va_Horodate_out
		and date_fin>=@va_Horodate_out
		and CFG..ANN_AST_CAL.competence in (select num from #COM)
		and CFG..ANN_AST_CAL.interlocuteur = CFG..ANN_FIC.numero

	declare COMP cursor for select num, sous_type from #COM
	declare INTERL cursor for select competence,interlocuteur, nom from #FICHE

	open COMP
	fetch COMP into @vl_Num, @vl_SousType

	while (@@sqlstatus=0)
	begin
		select  @vl_NomInt=null,@vl_NumInt=null

		/*si ma competence n'a pas d'interlocuteur dans le calendrier*/
		if not exists (select * from #FICHE where competence=@vl_Num)
		begin
			/*on regarde la fonction*/
			select @vl_NomInt=nom, @vl_NumInt=numero
			from CFG..ANN_FIC
			where competence=@vl_Num

			if @@rowcount != 0
			begin
				insert #RESULTAT
				select  @vl_SousType,
					@vl_NomInt,
					@vl_NumInt,
					@vl_Num
				from CFG..ANN_GEO
				where competence=@vl_Num
				and (autoroute = @va_Autoroute_out and
					(case when PR_debut <= PR_fin then PR_debut else PR_fin end) <= @va_PR_out and
					(case when PR_debut <= PR_fin then PR_fin else PR_debut end) >= @va_PR_out and
                                        ( sens = XDC_LES_2_SENS  or  sens = @vl_Sens ) )

			end
		end
		else
		begin

			open INTERL
			fetch INTERL into @vl_Compt,@vl_NumInt,@vl_NomInt

			while (@@sqlstatus=0)
			begin
				/* si mon interlocuteur a la bonne competence*/
				if @vl_Compt = @vl_Num
				begin
					insert #RESULTAT
					select  @vl_SousType,
						@vl_NomInt,
						@vl_NumInt,
						@vl_Num
					from CFG..ANN_GEO
					where competence=@vl_Num
					and (autoroute = @va_Autoroute_out and
						(case when PR_debut <= PR_fin then PR_debut else PR_fin end) <= @va_PR_out and
						(case when PR_debut <= PR_fin then PR_fin else PR_debut end) >= @va_PR_out and
                                                ( sens = XDC_LES_2_SENS  or  sens = @vl_Sens ) )
				end
/*		if @@rowcount=0
			insert #RESULTAT 
			values (@vl_SousType,@vl_NomInt,@vl_NumInt,@vl_Num)*/
			
				fetch INTERL into @vl_Compt,@vl_NumInt,@vl_NomInt
			end

			close INTERL
		end

		fetch COMP into @vl_Num,@vl_SousType
	end
	select distinct * from #RESULTAT
	order by numero
   end   
   return (XDC_OK)      
#endif
go
