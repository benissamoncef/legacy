/*E*/
/*Fichier : $Id: xzan25sp.prc,v 1.6 2016/09/21 10:40:02 devgfi Exp $      Release : $Revision: 1.6 $        Date : $Date: 2016/09/21 10:40:02 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan25sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Oct 1994	: Creation
* Guilhou	14 nov 1996	: categorie devient district_validite (DEM/1306) 1.2
* Guilhou	23 dec 1996	: correction filtrage sur district de validite (DEM/1365) 1.4
* JPL		07/10/2014	: retour du sens des zones geographiques (DEM 1100)  1.5
* JPL		20/09/2016	: Support des deux ordres des PR de zones, independamment du sens (DEM 1186)  1.6
------------------------------------------------------*/

/* fichiers inclus */


/* #include "xzan25sp.h" */

/* static char *version = "@(#)xzan25.c	1.1 23/18/94 : xzan25" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des astreintes
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;25
   @va_Horodate_in			datetime = null,
   @va_NumTypeAstreinte_in		tinyint = null,
   @va_NumDistrict_in			tinyint = null,
   @va_Autoroute_in			tinyint = null,
   @va_PR_in				int = null
   as 

/* 
*
* SP	XZAN25SP
*
* PARAMETRES EN ENTREE :
* 
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des fiches astreinte » une date donn≈e
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/
declare

   /*A
   ** Declaration des variables locales
   */
    
   @vl_Compt 		smallint,
   @vl_District		tinyint,
   @vl_Astreinte	tinyint,
   @vl_Autoroute	tinyint,
   @vl_Categ		smallint,
   @vl_Astr		tinyint,
   @vl_Auto		tinyint,
   @vl_DateMin		datetime,
   @vl_DateMax		datetime,
   @vl_Num		smallint,
   @vl_Type		tinyint,
   @vl_NomAst		char(25),
   @vl_SousType		char(25),
   @vl_NomInt		char(25),
   @vl_NumInt		smallint,
   @vl_Site		tinyint,
   @vl_Horodate 	tinyint

	/*! transfo en param…tres nulls bug applix */
	if @va_NumTypeAstreinte_in = 0 select @va_NumTypeAstreinte_in = null
	if @va_NumDistrict_in = 0 select @va_NumDistrict_in = null
	if @va_Autoroute_in = 0 select @va_Autoroute_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null
       
   /*A
   ** Si les parametres sont non nuls
   */
      
      /*A
      ** Travail dans la base CFG
      */   
      
      select @vl_Astreinte = 0
      
      if @va_NumTypeAstreinte_in = null
       	 
       	 select @vl_Astreinte = 1
       
      else 
      
      begin
         
         select @vl_Astr = numero from CFG..TYP_AST
           where numero = @va_NumTypeAstreinte_in
         
         if @vl_Astr = null
           
            return ( XZANC_PAS_COMP_TYPE )
      end     
      
      
      select @vl_District = 0
      
      if @va_NumDistrict_in = null
       	 
       	 select @vl_District = 1
      
      else 
      
      begin   
         
         select @vl_Categ = district_validite from CFG..ANN_AST_CMP
           where district_validite = @va_NumDistrict_in
         
          if @vl_Categ = null
           
            return ( XZANC_PAS_COMP_DISTRICT )
      end    
         
   	select @vl_Horodate =0
	if @va_Horodate_in=null
		select @vl_Horodate = 1

      select @vl_Autoroute = 0
      
      if @va_Autoroute_in = null
       	 
       	 select @vl_Autoroute = 1
       	 
      else
      
      begin
         
         select @vl_Auto = numero from CFG..RES_AUT
           where numero = @va_Autoroute_in
         
         if @vl_Auto = null
           
            return ( XDC_NOK )
           
         else
         
            if @va_PR_in = XDC_PR_INCONNU
         
               return ( XDC_NOK ) 	 
         
      end
      
      
      /*A
      ** Recherche de la liste des fiches astreintes » une date donnee
      */
      create table #RESULTAT (
               numero		smallint null,
               type		char(25) null,
               sous_type	char(25) null,
	       district         tinyint null,
	       site_reponsable 	tinyint null,
               autoroute	tinyint null,
               PR_debut		int null,
               PR_fin		int null,
               nom		char(25) null,
               numint		smallint null,
               sens		tinyint null)
      
      create table #FICHE (
               competence	smallint null,
               interlocuteur	smallint null,
               date_debut       datetime null,
	       date_fin		datetime null,
	       nom		char(25) null)
             
      create table #COM (
               num		smallint null,
               type		tinyint null,
               sous_type	char(25) null,
	       district		tinyint null,
	       site_responsable tinyint null
               )

	/*A on recupere toutes les competences correspondant
	**  au type a la zone et au district */
      insert #COM (num,type,sous_type,district,site_responsable)
      select distinct 	CMP.numero,
			CMP.type_d_astreinte,
			CMP.sous_type_d_astreinte,
			CMP.district_validite,
			CMP.site_responsable
      from 	CFG..ANN_AST_CMP CMP,
      		CFG..ANN_GEO GEO
      where ( @vl_Astreinte = 1 or CMP.type_d_astreinte = @va_NumTypeAstreinte_in )
        and ( @vl_District = 1 or CMP.district_validite = @va_NumDistrict_in or CMP.district_validite = 0)
        and ((GEO.competence = CMP.numero
              and (@vl_Autoroute = 1 or
                   (GEO.autoroute = @va_Autoroute_in
                    and (case when GEO.PR_debut <= GEO.PR_fin then GEO.PR_debut else GEO.PR_fin end) <= @va_PR_in
                    and (case when GEO.PR_debut <= GEO.PR_fin then GEO.PR_fin else GEO.PR_debut end) >= @va_PR_in)
                  )
              )
        	or CMP.numero not in (select competence from CFG..ANN_GEO)
            )
	order by CMP.numero

	insert #FICHE
	select distinct CFG..ANN_AST_CAL.competence,interlocuteur,date_debut,date_fin,nom
	from CFG..ANN_AST_CAL, #COM, CFG..ANN_FIC
	where (@vl_Horodate=1 or (date_debut<=@va_Horodate_in and date_fin>=@va_Horodate_in))
	and CFG..ANN_AST_CAL.competence in (select num from #COM)
	and CFG..ANN_AST_CAL.interlocuteur = CFG..ANN_FIC.numero
	order by CFG..ANN_AST_CAL.competence

      declare COMP cursor for select num, type,sous_type, district, site_responsable from #COM order by num
      declare INTERL cursor for select competence,interlocuteur, nom from #FICHE order by competence

      open COMP
      fetch COMP into @vl_Num, @vl_Type, @vl_SousType, @vl_District, @vl_Site

      while (@@sqlstatus=0)
      begin
      	select 	@vl_NomAst=nom
      	from 	CFG..TYP_AST
      	where 	numero=@vl_Type
      	
      	select  @vl_NomInt=null,@vl_NumInt=null

	/*si ma competence n'a pas d'interlocuteur dans le calendrier*/
	if not exists (select * from #FICHE where competence=@vl_Num)
	begin
		/*si la competence n'est pas deja dans la table resultat*/
		if not exists (select numero from #RESULTAT where numero = @vl_Num)
		begin
		/*on regarde la fonction*/
		select @vl_NomInt=nom, @vl_NumInt=numero
		from CFG..ANN_FIC
		where competence=@vl_Num

		if @@rowcount=0
			select @vl_NomInt=null,@vl_NumInt=null
	
		insert #RESULTAT
			( numero, type, sous_type, district, site_reponsable, autoroute, PR_debut, PR_fin, nom, numint, sens )
			select  @vl_Num,
				@vl_NomAst,
				@vl_SousType,
				@vl_District,
				@vl_Site,
				autoroute,
				PR_debut,
				PR_fin,
				@vl_NomInt,
				@vl_NumInt,
				sens
			from    CFG..ANN_GEO
			where   competence=@vl_Num
			and (@vl_Autoroute = 1 or
				(autoroute = @va_Autoroute_in
				 and (case when PR_debut <= PR_fin then PR_debut else PR_fin end) <= @va_PR_in
				 and (case when PR_debut <= PR_fin then PR_fin else PR_debut end) >= @va_PR_in)
			    )

		if @@rowcount=0
			insert #RESULTAT
				( numero, type, sous_type, district, site_reponsable, autoroute, PR_debut, PR_fin, nom, numint, sens )
			values (@vl_Num,@vl_NomAst,@vl_SousType,@vl_District,@vl_Site,null,null,null,@vl_NomInt,@vl_NumInt,null)
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
				( numero, type, sous_type, district, site_reponsable, autoroute, PR_debut, PR_fin, nom, numint, sens )
       				select 	@vl_Num,
      					@vl_NomAst,
					@vl_SousType,
					@vl_District,
					@vl_Site,
      					autoroute,
      					PR_debut,
      					PR_fin,
      					@vl_NomInt,
      					@vl_NumInt,
      					sens
      
      				from 	CFG..ANN_GEO
      				where   competence=@vl_Num
       		       		and (@vl_Autoroute = 1 or
					(autoroute = @va_Autoroute_in
					 and (case when PR_debut <= PR_fin then PR_debut else PR_fin end) <= @va_PR_in
					 and (case when PR_debut <= PR_fin then PR_fin else PR_debut end) >= @va_PR_in)
				    )

       			if @@rowcount=0
       				insert #RESULTAT 	
					( numero, type, sous_type, district, site_reponsable, autoroute, PR_debut, PR_fin, nom, numint, sens )
				values (@vl_Num,@vl_NomAst,@vl_SousType,@vl_District,@vl_Site,null,null,null,@vl_NomInt,@vl_NumInt,null)
  		end

		fetch INTERL into @vl_Compt,@vl_NumInt,@vl_NomInt
		end
       		close INTERL
       end

       fetch COMP into @vl_Num, @vl_Type, @vl_SousType, @vl_District, @vl_Site
     end

     select distinct numero, type, sous_type, district, site_reponsable, autoroute, PR_debut, PR_fin, nom, numint, sens
     from #RESULTAT
     order by numero
     
   return ( XDC_OK )
         
go  
