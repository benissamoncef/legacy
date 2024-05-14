/*E*/
/*Fichier : $Id: xzap41sp.prc,v 1.2 2011/09/12 15:49:00 gesconf Exp $      Release : $Date: 2011/09/12 15:49:00 $        Date : $Date: 2011/09/12 15:49:00 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap41sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	17/10/07	creation
* PCL	15/01/08	on ne retourne que les interlocuteurs 
			qui ont une @mail non nulle V1.2 (REPERE1)
* GGY	26/09/23 : Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lit une proposition d'un envoi MAIL d'un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP41

	@va_NumPA_in		smallint=null,
	@va_Ordre_in		tinyint=null,
	@va_Remarque_out	char(50)=null output,
	@va_Explication_out	char(250)=null output
	
	
/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		va_NumPA_in
* XDY_Octet 		va_Ordre_in
*
* ARGUMENTS EN SORTIE :
*
* XDY_Phrase		va_Remarque_out	
* XDY_Texte		va_Explication_out
*	en select:
* XDY_PR		va_PR_out	
* XDY_Autoroute		va_Autoroute_out
* XDY_Octet 		va_Type_out
* XDY_Texte		va_Explication_out(par destinataire)	
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
* Lit une proposition d'un envoi FAX d'un plan d'actions
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
		exec @vl_Status = SQL_CI.PRC..XZAP41
			@va_NumPA_in		,
			@va_Ordre_in		,
			@va_Remarque_out	output,
			@va_Explication_out	output

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP41
			@va_NumPA_in		,
			@va_Ordre_in		,
			@va_Remarque_out	output,
			@va_Explication_out	output

	if @vl_Status = null
		return XDC_PRC_INC
	else
		return @vl_Status

#else
   /*A
   ** Si une des variables d'entree est nulle alors retourne XDC_ARG_INV
   */
   
   if @va_NumPA_in = null
   or @va_Ordre_in = null
  
      return XDC_ARG_INV
  
   else
   
   /*A
   ** Si les variables d'entree sont non nulles
   ** Lecture d'une Proposition FAX d'un plan d'action
   */
   
   begin
      
      select	@va_Remarque_out 	= remarque,
      		@va_Explication_out 	= explication
      		
   		
   	from EXP..SAD_MAIL
   
        where numero = @va_NumPA_in 
          and ordre = @va_Ordre_in 
        
        
      select 	texte,
      		morceau
   	
      from EXP..SAD_TXT_MAIL
   
        where numero = @va_NumPA_in 
          and ordre = @va_Ordre_in 
        
        order by morceau
       
/*A recherche du nom et mail des interlocuteur d'astreinte */

	declare @vl_PR int,
		@vl_Autoroute tinyint,
		@vl_Type tinyint,
		@vl_Explication char(250),
		@vl_Date datetime,
		@vl_Comp smallint,
		@vl_Inter smallint,
		@vl_NomInt char(25),
		@vl_Mail char(100),
		@vl_Compte int
		
	select  @vl_Date=horodate
		from EXP..SAD_PLN
		where numero = @va_NumPA_in


	create table #RESULTAT (
			PR 		int null,
			autoroute 	tinyint null,
			type tinyint 	null,
			explication 	char(250) null,
			interlocuteur 	smallint null,
			nom char(25) 	null,
			competence 	smallint null,
			mail 		char(100) null
			)
	
	declare COMP cursor for	
		select competence 
		from  	CFG..ANN_AST_CMP CMP,
	      		CFG..ANN_GEO GEO
		where CMP.type_d_astreinte = @vl_Type
	        and (GEO.competence = CMP.numero
	              and (GEO.autoroute = @vl_Autoroute
	                   and GEO.PR_debut <= @vl_PR
	                   and GEO.PR_fin   >= @vl_PR)
	              )

	declare PROP cursor for
		select	PR,
			autoroute,
			type_d_astreinte,
			explication
   		
		from EXP..SAD_DST_MAIL
   
 		where numero = @va_NumPA_in 
          	  and ordre = @va_Ordre_in 
        
	open PROP
	
	fetch PROP into @vl_PR,@vl_Autoroute,@vl_Type,@vl_Explication
	
	while (@@sqlstatus=0)
	begin
		
/*A recherche des competences correspondantes */
        
		select @vl_Comp = null
		
		select @vl_Compte = count(*)
		from  	CFG..ANN_AST_CMP CMP,
	      		CFG..ANN_GEO GEO
		where CMP.type_d_astreinte = @vl_Type
	        and (GEO.competence = CMP.numero
	              and (GEO.autoroute = @vl_Autoroute
	                   and GEO.PR_debut <= @vl_PR
	                   and GEO.PR_fin   >= @vl_PR)
	              )

/*A si on n a pas trouve par les zones geographiques, on cherche sans */

	        if @vl_Compte = 0
		begin
		  select @vl_Comp = CMP.numero
		  from  	CFG..ANN_AST_CMP CMP
		  where CMP.type_d_astreinte = @vl_Type
	          and CMP.numero not in (select competence from CFG..ANN_GEO)

		  if @vl_Comp = null
                    /*A si on n a pas trouve : on retourne 0 */
		    select @vl_Comp=0,@vl_Inter=0,@vl_NomInt=" ",@vl_Mail=" "
		end
            
		else

                  /*A recherche de la fiche annuaire */
	
	        begin
		open COMP
		fetch COMP into @vl_Comp
		while (@@sqlstatus = 0)
		begin
			insert #RESULTAT
		      	select	@vl_PR,
			      	@vl_Autoroute,
			      	@vl_Type,
			      	@vl_Explication,
				interlocuteur,
				nom,
				@vl_Comp,
				adresse_email
			from CFG..ANN_AST_CAL,CFG..ANN_FIC
			where ( CFG..ANN_AST_CAL.competence=@vl_Comp 
				and date_debut<=@vl_Date and date_fin>=@vl_Date
				and CFG..ANN_AST_CAL.interlocuteur=CFG..ANN_FIC.numero)

			if @@rowcount=0
			begin
				select @vl_Inter=0,@vl_NomInt=" ",@vl_Mail=" "
				select @vl_Mail=isnull(@vl_Mail," ")

				insert #RESULTAT
				values(  @vl_PR,
					@vl_Autoroute,
					@vl_Type,
					@vl_Explication,
					@vl_Inter,
					@vl_NomInt,
					@vl_Comp,
					@vl_Mail)
			end
			fetch COMP into @vl_Comp
		    end
		    close COMP
		end
		fetch PROP into @vl_PR,@vl_Autoroute,@vl_Type,@vl_Explication
	end
      
      select * from #RESULTAT 
      /* REPERE1 */
      where mail is not null
      /* FIN DU REPERE1 */
      return XDC_OK
      
   end   
         
#endif
go
