/*E*/
/*Fichier : $Id: xzao650sp.prc,v 1.3 2018/07/11 11:32:39 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2018/07/11 11:32:39 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao650sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL		15/11/16	: Creation (LOT23)
* LCL		05/07/17	: Modif PRA
* LCL           11/07/18        : correction syntaxe from
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao650sp.h" */

/* static char *version = "@(#)xzao650.c	1.1 23/18/94 : xzao650" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une aire dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

drop procedure XZAO650
go

create procedure XZAO650
   @va_Basedd_in        	char(3),
   @va_NomViaduc_in		char(25),
   @va_Autoroute_in		char(4),
   @va_Sens_in			tinyint,
   @va_PRdebut_in		int,
   @va_PRfin_in	   		int,
   @va_Site_in			char(2),
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO86SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Nom		va_NomViaduc_in
* XDY_Nom		va_Autoroute_in
* XDY_Sens		va_Sens_in
* XDY_PR		va_PRdebut
* XDY_PR		va_PRfin
* XDY_NomSite		va_Site_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un viaduc en base
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* Insertion ou modification dans la table RES_VIA (VIADUC)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	14 Nov 2016	: Creation
* GGY	26/09/23	: Ajout district CA (DEM483)
* GGY	17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumAutoroute	tinyint,
   @vl_Site			tinyint,
   @vl_SiteNom		char(10),
   @vl_NomViaduc	varchar(25),
   @vl_Numero		smallint
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null 
   or @va_NomViaduc_in = null 
   or @va_Autoroute_in = null 
   or @va_PRdebut_in = null 
   or @va_PRfin_in = null
   
      return XDC_ARG_INV
   
   else   
   begin
      
		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_SiteNom = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_SiteNom=XDC_SQLCI or @vl_SiteNom=XDC_SQLCA
		begin
      
         /*A
         **  Travail dans la base CFG
         */   
      
         if @va_Basedd_in = XDC_BASE_CFG
         begin
         
            /*A
            ** Verification de l'existence et recherche du numero de l'autoroute
            */
         
            select @vl_NumAutoroute = numero from CFG..RES_AUT 
              where nom = @va_Autoroute_in
            
	    select @vl_Site = numero from CFG..RES_DIS
	    where code = @va_Site_in and type = XDC_TYPEM_SD
         
		if  @vl_Site = null
			select  @vl_Site = XDC_CI

            if @vl_NumAutoroute <> null and @vl_Site <> null
            begin
         
               /*A 
               ** Insertion ou Modification du viaduc
               **
               ** Verification de l'existence et recherche de l'aire dans RES_VIA
               */
         
               select @vl_Numero = numero from CFG..RES_VIA
                 where nom      = @va_NomViaduc_in
		 
               
               /*A
               ** Le viaduc n'existe pas dans RES_VIA
               */
            
               if @vl_Numero = null
               begin
              
		     select @vl_Numero = max(numero)+1 from CFG..RES_VIA

		     if  @vl_Numero = NULL
			select @vl_Numero = 1

                     insert into CFG..RES_VIA
                     values ( @vl_Numero, @va_NomViaduc_in,  
				@vl_NumAutoroute, @va_PRdebut_in, @va_PRfin_in, 
				@va_Sens_in, @vl_Site)
                  
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
               
               else
         
               /*A
               ** Le viaduc existe deja
               */      
            
               begin
           
                     update CFG..RES_VIA
    	             set nom         = @va_NomViaduc_in, 
    	                 PRdebut     = @va_PRdebut_in, 
		         PRfin       = @va_PRfin_in,
		         sens        = @va_Sens_in,
		         site_gestion        = @vl_Site   
    	             where numero   = @vl_Numero  
    
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
          
               /*A
               ** L'autoroute n'existe pas
               **
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
         
               select @va_Resultat_out = XDC_ECHEC
          
         end
         
#ifdef CI
         else
         
         /*A
         **  Travail dans la base CFT
         */   
      
         begin
         
            /*A
            ** Verification de l'existence et recherche du numero de l'autoroute
            */
         
            select @vl_NumAutoroute = numero from CFT..RES_AUT 
              where nom = @va_Autoroute_in

	    select @vl_Site = numero from CFT..RES_DIS
	      where code = @va_Site_in and type = XDC_TYPEM_SD

		if  @vl_Site = null
			select  @vl_Site = XDC_CI
            
            if @vl_NumAutoroute <> null
            begin
         
               /*A 
               ** Insertion ou Modification du viaduc
               **
               ** Verification de l'existence et recherche du viaduc dans RES_VIA
               */
         
               select @vl_Numero = numero from CFT..RES_VIA
                 where nom      = @va_NomViaduc_in
               
               /*A
               ** L'aire n'existe pas dans RES_VIA
               */
            
               if @vl_Numero = null
               begin
             
		     select @vl_Numero = max(numero)+1 from CFT..RES_VIA

		     if  @vl_Numero = NULL
			select @vl_Numero = 1

                     insert into CFT..RES_VIA
                     values ( @vl_Numero, @va_NomViaduc_in,
			@vl_NumAutoroute, @va_PRdebut_in, @va_PRfin_in, 
			@va_Sens_in, @vl_Site)
                  
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
               
               else
         
               /*A
               ** Le viaduc existe deja
               */      
            
               begin
           
                     update CFT..RES_VIA
    	             set nom         = @va_NomViaduc_in, 
    	                 PRdebut     = @va_PRdebut_in, 
		         PRfin       = @va_PRfin_in,
		         sens        = @va_Sens_in,
			 site_gestion	     = @vl_Site
    	             where numero   = @vl_Numero 
    
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
          
               /*A
               ** L'autoroute n'existe pas
               **
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
         
               select @va_Resultat_out = XDC_ECHEC
          
         end

#endif
         
      end
   end         
     
   return XDC_OK
         
go  
