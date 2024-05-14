/*E*/
/*Fichier : $Id: xzao61sp.prc,v 1.8 2018/07/11 09:31:41 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2018/07/11 09:31:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao61sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao61sp.h" */

/* static char *version = "@(#)xzao61.c	1.1 23/18/94 : xzao61" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un Pente dans la table PENTE
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;61
   @va_Basedd_in        	char(3),
   @va_NomPortion_in		char(5),
   @va_PRdebut_in		int,
   @va_PRfin_in			int,
   @va_Pente_in			smallint,	
   @va_NomPente_in		char(25),
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO61SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_Nom	va_NomPente_in
* XDY_PR	va_PRdebut_in
* XDY_PR	va_PRfin_in	
* XDY_Mot	va_Pente_in
* XDY_Booleen	va_Creation_in			
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un pente a une base
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
* Insertion dans la table RES_PNT (PENTE)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	12 Sep 1994	: Creation
* volcic	18 Oct 1994	: Ajout XDC_SQL_CI (v1.2)
* volcic	18 Oct 1994	: Modif XDC_SQLCI (v1.3)
* volcic	09 Nov 1994	: Ajout #ifdef CI (v1.4)
* volcic	22 Nov 1994	: Modi cade (v1.5)
* C.T.		18 Fev 1998	: Modif test sur update suite SQL server 11.0.2.3
* LCL      	11/07/18	: correction syntaxe from
* GGY		26/09/23	:	Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumPortion	smallint,
   @vl_Portion1		smallint,
   @vl_Portion2		smallint,
   @vl_PRdebut		int,
   @vl_PRfin		int,
   @vl_Autoroute	tinyint,
   @vl_Numero		tinyint,
   @vl_Num			tinyint,
   @vl_Site			char(10)

   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null or @va_NomPente_in = null 
   or @va_PRdebut_in = null or @va_PRfin_in = null or @va_Pente_in = null 
      
      return XDC_ARG_INV
   
   else   
   
   begin

		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin 
         select @vl_NumPortion = numero, @vl_Autoroute = autoroute from CFG..RES_POR
            where nom = @va_NomPortion_in
              
         if @vl_NumPortion <> null
         begin
            
            select @vl_Numero = portion from CFG..RES_PNT
                 where portion  = @vl_NumPortion
                   and PR_debut = @va_PRdebut_in
            
            if @vl_Numero = null
            begin
               
               if @va_Creation_in = 1
               begin
               
                  /*A
                  **  Insertion de la pente argument dans la table CFG..RES_PNT de la base
                  **  passee en argument, si celui ci n'existe pas et Creation positionne
                  */
             
                  select @vl_Num = numero from CFG..RES_POR
                     where numero    = @vl_NumPortion
                       and PR_debut <= @va_PRdebut_in
                       and PR_fin   >= @va_PRfin_in
                       
                  if @vl_Num = null     
                  begin
                     
                     select @vl_Portion1 = numero, @vl_PRfin = PR_fin from CFG..RES_POR
                       where nom = @va_NomPortion_in
                          
                     select @vl_Portion2 = numero, @vl_PRdebut = PR_debut from CFG..RES_POR
                       where autoroute = @vl_Autoroute
                         and PR_debut <= @va_PRfin_in
                         and PR_fin   >= @va_PRfin_in
                          
                     if @vl_Portion2 <> null
                     begin
                     
                        insert into CFG..RES_PNT
                  	values ( @vl_Portion1, @va_PRdebut_in, @vl_PRfin, @va_Pente_in, @va_NomPente_in)
               
                   	insert into CFG..RES_PNT
                  	values ( @vl_Portion2, @vl_PRdebut, @va_PRfin_in, @va_Pente_in, @va_NomPente_in)
               
                     end
                     
                     else
                     
                       return (XDC_NOK)
                  
                  end
                  else
                  begin
                  
                    insert into CFG..RES_PNT
                    values ( @vl_Num, @va_PRdebut_in, @va_PRfin_in, @va_Pente_in, @va_NomPente_in)
               
                    /*A
                    ** Erreur d'insertion
                    */
                     
                    if @@rowcount <> 1
                    begin
                     
                       /*A
                       ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                       */
           
                       select @va_Resultat_out = XDC_AJOUT
                        
                       return XDC_NOK
                    end
                
                    /*A
                    ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                    */
         
                    select @va_Resultat_out = XDC_AJOUT
                
                 end
               
               end
               
               else
                
               /*A
               ** Creation non positionne
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
         
                   select @va_Resultat_out = XDC_ECHEC
                
            end
             
            else
          
            begin
             
                if @va_Creation_in = 1
                begin
               
                  /*A
                  **  Mise a jour de la pente argument dans la table CFG..RES_PNT de la base
                  **  passee en argument, si celui ci n'existe pas et Creation positionne
                  */
               
                  select @vl_Num = numero from CFG..RES_POR
                     where numero    = @vl_NumPortion
                       and PR_debut <= @va_PRdebut_in
                       and PR_fin   >= @va_PRfin_in
                       
                  if @vl_Num = null     
                  begin
                     
                     select @vl_Portion1 = numero, @vl_PRfin = PR_fin from CFG..RES_POR
                       where nom = @va_NomPortion_in
                          
                     select @vl_Portion2 = numero, @vl_PRdebut = PR_debut from CFG..RES_POR
                       where autoroute = @vl_Autoroute
                         and PR_debut <= @va_PRfin_in
                         and PR_fin   >= @va_PRfin_in
                          
                     if @vl_Portion2 <> null
                     begin
                     
                        update CFG..RES_PNT
                  	set PR_fin    = @vl_PRfin,
                            pente     = @va_Pente_in,
                      	    nom       = @va_NomPente_in
                         where portion = @vl_Portion1
                         and PR_debut  = @va_PRdebut_in
                         
                   	update CFG..RES_PNT
                  	set PR_fin    = @va_PRfin_in,
                            pente     = @va_Pente_in,
                      	    nom       = @va_NomPente_in
                         where portion = @vl_Portion2
                         and PR_debut  = @vl_PRdebut
                         
                   	
                     end
                     
                     else
                     
                       return (XDC_NOK)
                  end
                  begin
                  
                     update CFG..RES_PNT
                     set PR_fin    = @va_PRfin_in,
                         pente     = @va_Pente_in,
                         nom       = @va_NomPente_in
                     where portion = @vl_Num
                       and PR_debut  = @va_PRdebut_in
                      
                     /*A
                     ** Erreur de modification
                     */
                     
                     if @@rowcount > 1
                     begin
                     
                        /*A
                        ** Affectation du parametre de sortie Resultat a XDC_MODIF
                        */
         
                        select @va_Resultat_out = XDC_MODIF
                        
                        return XDC_NOK
                     end
                  
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_MODIF
                     */
         
                     select @va_Resultat_out = XDC_MODIF
                
                  end  
               end
               
               else
               
               begin
               
                  /*A
                  **  Suppression de la pente argument dans la table CFG..RES_PNT de la base
                  **  passee en argument, si celui ci n'existe pas et Creation positionne
                  */
               
                  select @vl_Num = numero from CFG..RES_POR
                    where numero    = @vl_Numero
                      and PR_debut <= @va_PRdebut_in
                      and PR_fin   >= @va_PRfin_in
                       
                  if @vl_Num = null     
                  begin
                     
                     select @vl_Portion1 = numero, @vl_PRfin = PR_fin from CFG..RES_POR
                       where nom = @va_NomPortion_in
                          
                     select @vl_Portion2 = numero, @vl_PRdebut = PR_debut from CFG..RES_POR
                       where autoroute = @vl_Autoroute
                         and PR_debut <= @va_PRfin_in
                         and PR_fin   >= @va_PRfin_in
                          
                     if @vl_Portion2 <> null
                     begin
                     
                        delete CFG..RES_PNT
                  	where portion = @vl_Portion1
                        and PR_debut  = @va_PRdebut_in
                         
                   	delete CFG..RES_PNT
                  	where portion = @vl_Portion2
                        and PR_debut  = @vl_PRdebut
                         
                   	
                     end
                     
                     else
                     
                       return (XDC_NOK)
                  end
                  begin
                  
                     delete CFG..RES_PNT
                     where portion  = @vl_Num
                     and PR_debut = @va_PRdebut_in
                
                      /*A
                      ** Erreur de suppression
                      */
                     
                      if @@rowcount <> 1
                      begin
                     
                          /*A
                          ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                          */
         
                          select @va_Resultat_out = XDC_SUPPR
                        
                          return XDC_NOK
                       end
                       
                       /*A
                       ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                       */
         
                         select @va_Resultat_out = XDC_SUPPR
                   end
                end   
             end
         end
         
         else
         
         /*A
         ** L'autoroute ou la portion n'existe pas
         ** Affectation du parametre de sortie Resultat a XDC_ECHEC
         */
            select @va_Resultat_out = XDC_ECHEC
                
      end      
      
#ifdef CI
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin 
         select @vl_NumPortion = numero, @vl_Autoroute = autoroute from CFT..RES_POR
            where nom = @va_NomPortion_in
              
         if @vl_NumPortion <> null
         begin
            
            select @vl_Numero = portion from CFT..RES_PNT
                 where portion  = @vl_NumPortion
                   and PR_debut = @va_PRdebut_in
            
            if @vl_Numero = null
            begin
               
               if @va_Creation_in = 1
               begin
               
                  /*A
                  **  Insertion de la pente argument dans la table CFT..RES_PNT de la base
                  **  passee en argument, si celui ci n'existe pas et Creation positionne
                  */
             
                  select @vl_Num = numero from CFT..RES_POR
                     where numero    = @vl_NumPortion
                       and PR_debut <= @va_PRdebut_in
                       and PR_fin   >= @va_PRfin_in
                       
                  if @vl_Num = null     
                  begin
                     
                     select @vl_Portion1 = numero, @vl_PRfin = PR_fin from CFT..RES_POR
                       where nom = @va_NomPortion_in
                          
                     select @vl_Portion2 = numero, @vl_PRdebut = PR_debut from CFT..RES_POR
                       where autoroute = @vl_Autoroute
                         and PR_debut <= @va_PRfin_in
                         and PR_fin   >= @va_PRfin_in
                          
                     if @vl_Portion2 <> null
                     begin
                     
                        insert into CFT..RES_PNT
                  	values ( @vl_Portion1, @va_PRdebut_in, @vl_PRfin, @va_Pente_in, @va_NomPente_in)
               
                   	insert into CFT..RES_PNT
                  	values ( @vl_Portion2, @vl_PRdebut, @va_PRfin_in, @va_Pente_in, @va_NomPente_in)
               
                     end
                     
                     else
                     
                       return (XDC_NOK)
                  
                  end
                  else
                  begin
                  
                    insert into CFT..RES_PNT
                    values ( @vl_Num, @va_PRdebut_in, @va_PRfin_in, @va_Pente_in, @va_NomPente_in)
               
                    /*A
                    ** Erreur d'insertion
                    */
                     
                    if @@rowcount <> 1
                    begin
                     
                       /*A
                       ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                       */
           
                       select @va_Resultat_out = XDC_AJOUT
                        
                       return XDC_NOK
                    end
                
                    /*A
                    ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                    */
         
                    select @va_Resultat_out = XDC_AJOUT
                
                 end
               
               end
               
               else
                
               /*A
               ** Creation non positionne
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
         
                   select @va_Resultat_out = XDC_ECHEC
                
            end
             
            else
          
            begin
             
                if @va_Creation_in = 1
                begin
               
                  /*A
                  **  Mise a jour de la pente argument dans la table CFT..RES_PNT de la base
                  **  passee en argument, si celui ci n'existe pas et Creation positionne
                  */
               
                  select @vl_Num = numero from CFT..RES_POR
                     where numero    = @vl_NumPortion
                       and PR_debut <= @va_PRdebut_in
                       and PR_fin   >= @va_PRfin_in
                       
                  if @vl_Num = null     
                  begin
                     
                     select @vl_Portion1 = numero, @vl_PRfin = PR_fin from CFT..RES_POR
                       where nom = @va_NomPortion_in
                          
                     select @vl_Portion2 = numero, @vl_PRdebut = PR_debut from CFT..RES_POR
                       where autoroute = @vl_Autoroute
                         and PR_debut <= @va_PRfin_in
                         and PR_fin   >= @va_PRfin_in
                          
                     if @vl_Portion2 <> null
                     begin
                     
                        update CFT..RES_PNT
                  	set PR_fin    = @vl_PRfin,
                            pente     = @va_Pente_in,
                      	    nom       = @va_NomPente_in
                         where portion = @vl_Portion1
                         and PR_debut  = @va_PRdebut_in
                         
                   	update CFT..RES_PNT
                  	set PR_fin    = @va_PRfin_in,
                            pente     = @va_Pente_in,
                      	    nom       = @va_NomPente_in
                         where portion = @vl_Portion2
                         and PR_debut  = @vl_PRdebut
                         
                   	
                     end
                     
                     else
                     
                       return (XDC_NOK)
                  end
                  begin
                  
                     update CFT..RES_PNT
                     set PR_fin    = @va_PRfin_in,
                         pente     = @va_Pente_in,
                         nom       = @va_NomPente_in
                     where portion = @vl_Num
                       and PR_debut  = @va_PRdebut_in
                      
                     /*A
                     ** Erreur de modification
                     */
                     
                     if @@rowcount > 1
                     begin
                     
                        /*A
                        ** Affectation du parametre de sortie Resultat a XDC_MODIF
                        */
         
                        select @va_Resultat_out = XDC_MODIF
                        
                        return XDC_NOK
                     end
                  
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_MODIF
                     */
         
                     select @va_Resultat_out = XDC_MODIF
                
                  end  
               end
               
               else
               
               begin
               
                  /*A
                  **  Suppression de la pente argument dans la table CFT..RES_PNT de la base
                  **  passee en argument, si celui ci n'existe pas et Creation positionne
                  */
               
                  select @vl_Num = numero from CFT..RES_POR
                    where numero    = @vl_Numero
                      and PR_debut <= @va_PRdebut_in
                      and PR_fin   >= @va_PRfin_in
                       
                  if @vl_Num = null     
                  begin
                     
                     select @vl_Portion1 = numero, @vl_PRfin = PR_fin from CFT..RES_POR
                       where nom = @va_NomPortion_in
                          
                     select @vl_Portion2 = numero, @vl_PRdebut = PR_debut from CFT..RES_POR
                       where autoroute = @vl_Autoroute
                         and PR_debut <= @va_PRfin_in
                         and PR_fin   >= @va_PRfin_in
                          
                     if @vl_Portion2 <> null
                     begin
                     
                        delete CFT..RES_PNT
                  	where portion = @vl_Portion1
                        and PR_debut  = @va_PRdebut_in
                         
                   	delete CFT..RES_PNT
                  	where portion = @vl_Portion2
                        and PR_debut  = @vl_PRdebut
                         
                   	
                     end
                     
                     else
                     
                       return (XDC_NOK)
                  end
                  begin
                  
                     delete CFT..RES_PNT
                     where portion  = @vl_Num
                     and PR_debut = @va_PRdebut_in
                
                      /*A
                      ** Erreur de suppression
                      */
                     
                      if @@rowcount <> 1
                      begin
                     
                          /*A
                          ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                          */
         
                          select @va_Resultat_out = XDC_SUPPR
                        
                          return XDC_NOK
                       end
                       
                       /*A
                       ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                       */
         
                         select @va_Resultat_out = XDC_SUPPR
                   end
                end   
             end
         end
         
         else
         
         /*A
         ** L'autoroute ou la portion n'existe pas
         ** Affectation du parametre de sortie Resultat a XDC_ECHEC
         */
            select @va_Resultat_out = XDC_ECHEC
                
      end


#endif

      end
   end
      
   return XDC_OK
         
go  
