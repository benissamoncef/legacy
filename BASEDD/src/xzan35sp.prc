/*E*/
/*Fichier : $Id: xzan35sp.prc,v 1.5 2016/09/21 10:40:06 devgfi Exp $      Release : $Revision: 1.5 $        Date : $Date: 2016/09/21 10:40:06 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan35sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Oct 1994	: Creation
* gaborit	16 dec 1994	: modif null applix 1.2
* guilhou	14 nov 1996	: categorie devient district_validite (DEM/1306) 1.3
* JPL		07/10/2014	: retour du sens des zones geographiques (DEM 1100)  1.4
* JPL		20/09/2016	: Support des deux ordres des PR de zones, independamment du sens (DEM 1186)  1.5
------------------------------------------------------*/


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des fiches competences
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;35
   @va_NumCategorie_in			smallint = null,
   @va_NumTypeAstreinte_in		tinyint = null,
   @va_Autoroute_in			tinyint = null,
   @va_PR_in				int = null,
   @va_DateLimite_in			datetime = null
   as 

/* 
*
* SP	XZAN35SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Mot		va_NumCategorie_in
* XDY_Octet		va_NumTypeAstreinte_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Horodate		va_DateLimite_in
*
* PARAMETRES EN SORTIE :
*
* XZANT_ListeCal
*
* VALEUR RENDUE :
*
* Retourne la liste des fiches competences dont le calendrier possede des vides
* jusqu'a la date limite
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
   @vl_Categorie	smallint,
   @vl_Astreinte	tinyint,
   @vl_Autoroute	tinyint,
   @vl_Categ		smallint,
   @vl_Astr		tinyint,
   @vl_Auto		tinyint,
   @vl_Num		smallint,
   @vl_Type		tinyint,
   @vl_NomAst		char(25),
   @vl_Date		int
       
   /* bug null applix */
   if @va_NumCategorie_in=0 select  @va_NumCategorie_in= null
   if @va_NumTypeAstreinte_in=0 select @va_NumTypeAstreinte_in = null
   if @va_Autoroute_in=0 select @va_Autoroute_in = null
   if @va_PR_in	=XDC_PR_INCONNU select @va_PR_in = null
   if @va_DateLimite_in	= XDC_DATE_NULLE select @va_DateLimite_in = null
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_DateLimite_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   begin
      
      /*A
      ** Travail dans la base CFG
      */   
      
      select @vl_Date = datediff ( day, getdate(), @va_DateLimite_in )
      
      if @vl_Date < 0
         
         return ( XDC_NOK )
      
      select @vl_Astreinte = 0
      
      if @va_NumTypeAstreinte_in = null
       	 
       	 select @vl_Astreinte = 1
       
      else 
      
      begin   
         
         select @vl_Astr = numero from CFG..TYP_AST
           where numero = @va_NumTypeAstreinte_in
         
         if @vl_Astr = null
           
            return ( XDC_NOK )
           
      end
      
      select @vl_Categorie = 0
      
      if @va_NumCategorie_in = null
       	 
       	 select @vl_Categorie = 1
      
      else 
      
      begin
         
         select @vl_Categ = district_validite from CFG..ANN_AST_CMP
           where district_validite = @va_NumCategorie_in
         
          if @vl_Categ = null
           
            return ( XDC_NOK )
           
      end   
               
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
         
         begin
            
            if @va_PR_in = null
         
               return ( XDC_NOK ) 	 
         end
         
      end
      
      
      /*A
      ** Recherche de la liste des fiches astreintes a une date donnee
      */
         
      create table #RESULTAT (
               numero		smallint null,
               type		char(25) null,
               autoroute	tinyint null,
               PR_debut		int null,
               PR_fin		int null,
               nom		char(25) null,
               numint		smallint null,
               sens		tinyint null)

      create table #COM (
               num		smallint null,
               type		tinyint null,
               heure		datetime null
               )

/*A on recupere toutes les competences correspondant
**  au type a la zone et a la categorie */

      insert #COM
      select distinct CMP.numero,type_d_astreinte,heure_de_debut
      from 	CFG..ANN_AST_CMP CMP,
      		CFG..ANN_GEO GEO
      where ( @vl_Astreinte = 1 or CMP.type_d_astreinte = @va_NumTypeAstreinte_in )
        and ( @vl_Categorie = 1 or CMP.district_validite = @va_NumCategorie_in )
        and ((GEO.competence = CMP.numero
              and (@vl_Autoroute = 1 or
                   (GEO.autoroute = @va_Autoroute_in  and
                    (case when GEO.PR_debut <= GEO.PR_fin then GEO.PR_debut else GEO.PR_fin end) <= @va_PR_in  and
                    (case when GEO.PR_debut <= GEO.PR_fin then GEO.PR_fin else GEO.PR_debut end) >= @va_PR_in)
                  )
              )
        or CMP.numero not in (select competence from CFG..ANN_GEO)
            )


/*A on ajoute a #RESULTAT toutes les fiches n'ayant pas d'interlocuteur  */

      declare COMP cursor for select num,type from #COM
      open COMP
      fetch COMP into @vl_Num, @vl_Type
      
      while (@@sqlstatus=0)
      begin
      	select 	@vl_NomAst=nom
      	from 	CFG..TYP_AST
      	where 	numero=@vl_Type
      	
      	if exists (
      	 	select 	*
      	
      		from 	CFG..ANN_AST_CAL
      	
      		where	competence=@vl_Num
      		and	(interlocuteur=null or interlocuteur=0)
      		and	date_debut<=@va_DateLimite_in
      		  )
      	   and not exists (
      	       	     	select *
         	   	from CFG..ANN_FIC
         	   	where competence=@vl_Num
         	   	   )
      	begin
      		insert #RESULTAT ( numero, type, autoroute, PR_debut, PR_fin, nom, numint, sens )
       			select 	@vl_Num,
      				@vl_NomAst,
      				autoroute,
      				PR_debut,
      				PR_fin,
      				null,
      				null,
      				sens
      
      			from 	CFG..ANN_GEO
      			where   competence=@vl_Num
       		if @@rowcount=0
       		insert #RESULTAT values (@vl_Num, @vl_NomAst, null, null, null, null, null, null)
       	end
      			
       fetch COMP into @vl_Num, @vl_Type
     end

     select distinct numero, type, autoroute, PR_debut, PR_fin, nom, numint, sens
     from #RESULTAT
     order by numero   
      
        
   end
     
   return ( XDC_OK )
         
go  
