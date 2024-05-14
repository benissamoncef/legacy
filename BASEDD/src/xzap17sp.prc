/*E*/
/* Fichier : $Id: xzap17sp.prc,v 1.15 2018/10/19 17:41:49 pc2dpdy Exp $        Release : $Revision: 1.15 $        Date : $Date: 2018/10/19 17:41:49 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap17sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	25 Aug 1994	: Creation
* gaborit 	05 Dec 1994	: ajout explication par destinataire (1.3)
* gaborit 	06 Dec 1994	: ajout nom et fax destinataire (1.4)
* gaborit 	06 Dec 1994	: correction @@rowcount (1.5)
* guilhou	22 nov 1996	: refonte de la gestion des astreintes (DEM/1306) 1.6
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.7
* cluchague     22 Jan 2002     : trop long, on optimise V1.8               
* cluchague     22 Jan 2007     : on autorise plusieurs  interlocuteurs pour 1 type d'astreinte (pc ville) V1.9
* JMG	21/11/07	: adresse mail	1.10 DEM/718
* cluchague     15 Jan 2008     : on en retourne que les interlocuteurs qui ont un fax / mail V1.11 
* JPL		09/10/2014	: Degroupee; prise en compte du sens  (DEM 1100)  1.12
* JPL		20/09/2016	: Support des deux ordres des PR de zones, independamment du sens (DEM 1186)  1.13
* PNI		28/04/2017	: Suppression de la partie recup�rant le numero des fax au lieu du mail 1.14
* LCL		07/09/2018	: Ajout SMS LOT25 DEM1301 1.15
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAP17' and type = 'P')
	drop procedure XZAP17
go


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Lit une proposition d'un envoi FAX d'un plan d'actions
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP17

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
declare @vl_Status int,
        @vl_type int, 
		@vl_Site tinyint

	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP17
		@va_NumPA_in		,
		@va_Ordre_in		,
		@va_Remarque_out	output,
		@va_Explication_out	output

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP17
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
   		
   	from EXP..SAD_FAX
   
        where numero = @va_NumPA_in 
          and ordre = @va_Ordre_in 
        
        
      select 	texte,
      		morceau
   	
      from EXP..SAD_TXT_FAX
   
        where numero = @va_NumPA_in 
          and ordre = @va_Ordre_in 
        
        order by morceau
       
/*A recherche du nom et fax des interlocuteur d'astreinte */

	declare @vl_PR int,
		@vl_Autoroute tinyint,
		@vl_Type tinyint,
		@vl_Explication char(250),
		@vl_Date datetime,
		@vl_Comp smallint,
		@vl_Inter smallint,
		@vl_NomInt char(25),
		@vl_Fax char(100),
		@vl_Compte int,
		@vl_Sens tinyint,
		@vl_sms tinyint
		
	select  @vl_Date=horodate
		from EXP..SAD_PLN
		where numero = @va_NumPA_in


	create table #RESULTAT (
			PR 		int null,
			autoroute 	tinyint null,
			sens	 	tinyint null,
			type tinyint 	null,
			explication 	char(250) null,
			interlocuteur 	smallint null,
			nom 		char(25) null,
			competence 	smallint null,
			fax 		char(100) null,
			sms		tinyint null
			)
	
	declare COMP cursor for	
		select competence 
		from  	CFG..ANN_AST_CMP CMP,
	      		CFG..ANN_GEO GEO
		where CMP.type_d_astreinte = @vl_Type
	        and (GEO.competence = CMP.numero
	              and (GEO.autoroute = @vl_Autoroute  and
	                   (case when GEO.PR_debut <= GEO.PR_fin then GEO.PR_debut else GEO.PR_fin end) <= @vl_PR and
	                   (case when GEO.PR_debut <= GEO.PR_fin then GEO.PR_fin else GEO.PR_debut end) >= @vl_PR and
                           ( GEO.sens = XDC_LES_2_SENS  or  GEO.sens = @vl_Sens ) )
	              )

	declare PROP cursor for
		select	PR,
			autoroute,
			sens,
			type_d_astreinte,
			explication,
			sms
   		
		from EXP..SAD_DST_FAX
   
 		where numero = @va_NumPA_in 
          	  and ordre = @va_Ordre_in 
        
	open PROP
	
	fetch PROP into @vl_PR,@vl_Autoroute,@vl_Sens,@vl_Type,@vl_Explication, @vl_sms
	
	while (@@sqlstatus=0)
	begin
		
/*A recherche des competences correspondantes */
        
		select @vl_Comp = null
		
		select @vl_Compte = count(*)
		from  	CFG..ANN_AST_CMP CMP,
	      		CFG..ANN_GEO GEO
		where CMP.type_d_astreinte = @vl_Type
	        and (GEO.competence = CMP.numero
	              and (GEO.autoroute = @vl_Autoroute  and
	                   (case when GEO.PR_debut <= GEO.PR_fin then GEO.PR_debut else GEO.PR_fin end) <= @vl_PR and
	                   (case when GEO.PR_debut <= GEO.PR_fin then GEO.PR_fin else GEO.PR_debut end) >= @vl_PR and
                           ( GEO.sens = XDC_LES_2_SENS  or  GEO.sens = @vl_Sens ) )
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
		    select @vl_Comp=0,@vl_Inter=0,@vl_NomInt=" ",@vl_Fax=" "
		end
            
		else

                  /*A recherche de la fiche annuaire */
	
	        begin
		open COMP
		fetch COMP into @vl_Comp
		while (@@sqlstatus = 0)
		begin
			if @vl_Explication="SMS" 
			begin
			insert #RESULTAT
		      	select	@vl_PR,
			      	@vl_Autoroute,
				@vl_Sens,
			      	@vl_Type,
			      	@vl_Explication,
				interlocuteur,
				nom,
				@vl_Comp,
				tel1,
				@vl_sms
			from CFG..ANN_AST_CAL,CFG..ANN_FIC
			where ( CFG..ANN_AST_CAL.competence=@vl_Comp 
				and date_debut<=@vl_Date and date_fin>=@vl_Date
				and CFG..ANN_AST_CAL.interlocuteur=CFG..ANN_FIC.numero
				and upper(CFG..ANN_FIC.explication_tel1) like '%PORTABLE%')
			insert #RESULTAT
		      	select	@vl_PR,
			      	@vl_Autoroute,
				@vl_Sens,
			      	@vl_Type,
			      	@vl_Explication,
				interlocuteur,
				nom,
				@vl_Comp,
				tel2,
				@vl_sms
			from CFG..ANN_AST_CAL,CFG..ANN_FIC
			where ( CFG..ANN_AST_CAL.competence=@vl_Comp 
				and date_debut<=@vl_Date and date_fin>=@vl_Date
				and CFG..ANN_AST_CAL.interlocuteur=CFG..ANN_FIC.numero
				and upper(CFG..ANN_FIC.explication_tel2) like '%PORTABLE%')
			insert #RESULTAT
		      	select	@vl_PR,
			      	@vl_Autoroute,
				@vl_Sens,
			      	@vl_Type,
			      	@vl_Explication,
				interlocuteur,
				nom,
				@vl_Comp,
				tel3,
				@vl_sms
			from CFG..ANN_AST_CAL,CFG..ANN_FIC
			where ( CFG..ANN_AST_CAL.competence=@vl_Comp 
				and date_debut<=@vl_Date and date_fin>=@vl_Date
				and CFG..ANN_AST_CAL.interlocuteur=CFG..ANN_FIC.numero
				and upper(CFG..ANN_FIC.explication_tel3) like '%PORTABLE%')
			end
			else if @vl_Explication<>"mail" and @vl_Explication<>"Fax special police"
			begin
			insert #RESULTAT
		      	select	@vl_PR,
			      	@vl_Autoroute,
				@vl_Sens,
			      	@vl_Type,
			      	@vl_Explication,
				interlocuteur,
				nom,
				@vl_Comp,
				fax,
				@vl_sms
			from CFG..ANN_AST_CAL,CFG..ANN_FIC
			where ( CFG..ANN_AST_CAL.competence=@vl_Comp 
				and date_debut<=@vl_Date and date_fin>=@vl_Date
				and CFG..ANN_AST_CAL.interlocuteur=CFG..ANN_FIC.numero)
			end
			else if @vl_Explication="mail" or @vl_Explication="Fax special police"
			begin
			insert #RESULTAT
			select  @vl_PR,
				@vl_Autoroute,
				@vl_Sens,
				@vl_Type,
				@vl_Explication,
				interlocuteur,
				nom,
				@vl_Comp,
				adresse_email,
				@vl_sms
			from CFG..ANN_AST_CAL,CFG..ANN_FIC
			where ( CFG..ANN_AST_CAL.competence=@vl_Comp
				and date_debut<=@vl_Date and date_fin>=@vl_Date
				and CFG..ANN_AST_CAL.interlocuteur=CFG..ANN_FIC.numero)
			end

			if @@rowcount=0
			begin
				select @vl_Inter=0,@vl_NomInt=" ",@vl_Fax=" "
				select @vl_Fax=isnull(@vl_Fax," ")

				insert #RESULTAT
				values(  @vl_PR,
					@vl_Autoroute,
					@vl_Sens,
					@vl_Type,
					@vl_Explication,
					@vl_Inter,
					@vl_NomInt,
					@vl_Comp,
					@vl_Fax,
					@vl_sms)
			end
			fetch COMP into @vl_Comp
		    end
		    close COMP
		end
	fetch PROP into @vl_PR,@vl_Autoroute,@vl_Sens,@vl_Type,@vl_Explication, @vl_sms

	end
      
      select PR,
	     autoroute,
	     type,
	     explication,
	     interlocuteur,
	     nom,
	     competence,
	     fax,
	     sens,
	     sms
      from #RESULTAT 
      /* REPERE1 */
      where fax is not null and fax != " "
      /* FIN DU REPERE1 */
      return XDC_OK
      
   end   
         
#endif
go
