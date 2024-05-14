/*E*/
/* Fichier : $Id: xzao540sp.prc,v 1.2 2018/01/09 11:59:40 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/01/09 11:59:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao540sp.prc
------------------------------------------------------
* HISTORIQUE :
*
* VR	19/03/12 : Creation  (DEM/1016)
* LCL	05/07/17 : Modif PRA
* GGY	26/09/23 : Ajout district CA (DEM483)
* GGY	17/10/23 : Correction si CFG vide (DEM483)
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao540sp.h" */

/* static char *version = " @(#)xzao540sp.prc	1.4 1.4 02/16/11: xzao540" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une zone d'influence PRV dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO540' and type = 'P')
	drop procedure XZAO540
go


create procedure XZAO540
   @va_Basedd_in        	char(3),
   @va_AutoroutePMV_in		char(5),
   @va_SensPMV_in		tinyint,
   @va_PrPMV_in			T_PR,
   @va_SitePMV_in		char(2),
   @va_DistancePMV_in		int,
   @va_AutorouteZone_in		char(5),
   @va_SensZone_in		tinyint,
   @va_PrDebutZone_in		T_PR,
   @va_PrFinZone_in		T_PR,
   @va_DirectionZone_in		char(15),
   @va_TypeZone_in		tinyint,
   @va_Inhibition_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO540SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   	va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajout d'une zone d'influence en base
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
* Insertion ou modification dans la table ZON_PRV 
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   @vl_Autoroute        tinyint,
   @vl_AutorouteZone    tinyint,
   @vl_Site             tinyint,
   @vl_SiteNom			char(10),
   @vl_Maitre           smallint,
   @vl_NumZone          smallint,
   @vl_Numero           smallint

   select @va_Resultat_out = XDC_ECHEC
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_AutoroutePMV_in = null 
   or @va_SensPMV_in = null
   or @va_PrPMV_in = null 
   or @va_SitePMV_in = null 
   or @va_AutorouteZone_in = null 
   or @va_SensZone_in = null 
   or @va_PrDebutZone_in = null
   or @va_PrFinZone_in = null
   or @va_DirectionZone_in = null
   or @va_TypeZone_in = null
   or @va_Inhibition_in = null
   or @va_DistancePMV_in = null
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
            ** Verification de l'existence de l'autoroute et du PRV comme equipement
            */
         
	    select @vl_Autoroute = null
            select @vl_Autoroute = numero from CFG..RES_AUT
            where nom = @va_AutoroutePMV_in
                        
	    select @vl_AutorouteZone = null
	    select @vl_AutorouteZone = numero from CFG..RES_AUT
	    where nom = @va_AutorouteZone_in

	    select @vl_Site = null
            select @vl_Site = numero from CFG..RES_DIS
            where code = @va_SitePMV_in
                        
	    select @vl_Numero = null
            select @vl_Numero = numero from CFG..EQT_GEN
            where autoroute = @vl_Autoroute
		and PR = @va_PrPMV_in
		and sens = @va_SensPMV_in
                and type = XDC_EQT_PRV
            
            /*A
            ** L'autoroute et le PRV et le site existent
            */
                     
            if @vl_Autoroute <> null and @vl_AutorouteZone <> null and @vl_Numero <> null and @vl_Site <> null
            begin
                
                select @vl_NumZone = numero_zone from CFG..ZON_PRV
                where numero_prv =  @vl_Numero and 
			autoroute_prv = @vl_Autoroute and
			sens_prv = @va_SensPMV_in and
			PR_prv = @va_PrPMV_in and 
			autoroute_zone = @vl_AutorouteZone and
			sens_zone = @va_SensZone_in and
			PR_debut_zone = @va_PrDebutZone_in and
			PR_fin_zone = @va_PrFinZone_in and
			type_zone = @va_TypeZone_in
			
               /*A
               ** La zone d'influence n'existe pas dans ZON_PRV
               */
               
               if @vl_NumZone = null
               begin
               
                  /*A
                  ** Insertion de la zone d'influence dans la table ZON_PRV
                  */
                  
                  select @vl_NumZone = max(numero_zone) from CFG..ZON_PRV

                  if @vl_NumZone = null
                    select @vl_NumZone = 0  
            
                  insert into CFG..ZON_PRV (
			numero_zone,
			numero_prv,
			autoroute_prv,
			sens_prv,
			PR_prv,
			site_prv,
			autoroute_zone,
			sens_zone,
			PR_debut_zone,
			PR_fin_zone,
			direction_zone,
			type_zone,
			inhibition,
			distance
		  )
                  values ( @vl_NumZone + 1, 
                 	   @vl_Numero,
			   @vl_Autoroute,
			   @va_SensPMV_in,
			   @va_PrPMV_in,
			   @vl_Site,
			   @vl_AutorouteZone,
			   @va_SensZone_in,
			   @va_PrDebutZone_in,
			   @va_PrFinZone_in,
			   @va_DirectionZone_in,
			   @va_TypeZone_in,
			   @va_Inhibition_in,
			   @va_DistancePMV_in)

                  /*A
                  ** Si erreur d'insertion
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
		* la zone d'influence existe dans ZON_PRV	
               */
                     
               begin
               
                  /*A
                  ** Modification de la zone d'influence dans la table ZON_PRV
                  */
                  
                  update CFG..ZON_PRV
                  set site_prv= @vl_Site,
		      PR_debut_zone	= @va_PrDebutZone_in, 
		      PR_fin_zone 	= @va_PrFinZone_in,
		      direction_zone 	= @va_DirectionZone_in,
		      type_zone		= @va_TypeZone_in,
                      inhibition	= @va_Inhibition_in,
                      distance		= @va_DistancePMV_in
                  where numero_zone = @vl_NumZone
               
                  /*A
                  ** Si erreur de modification
                  */
                     
                  if @@rowcount <> 1
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
            ** Le serveur ou l'autoroute n'existe pas 
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
            ** Verification de l'existence de l'autoroute et du PRV comme equipement
            */
         
	    select @vl_Autoroute = null
            select @vl_Autoroute = numero from CFT..RES_AUT
              where nom = @va_AutoroutePMV_in
                        
	    select @vl_AutorouteZone = null
	    select @vl_AutorouteZone = numero from CFT..RES_AUT
	      where nom = @va_AutorouteZone_in

	    select @vl_Site = null
            select @vl_Site = numero from CFT..RES_DIS
              where code = @va_SitePMV_in
                        
	    select @vl_Numero = null
            select @vl_Numero = numero from CFT..EQT_GEN
              where autoroute  = @vl_Autoroute
		and PR = @va_PrPMV_in
		and sens = @va_SensPMV_in
                and type = XDC_EQT_PRV
            
            /*A
            ** L'autoroute et le PRV et le site existent
            */
                     
            if @vl_Autoroute <> null and @vl_AutorouteZone <> null and @vl_Numero <> null and @vl_Site <> null
            begin
                
                select @vl_NumZone = numero_zone from CFT..ZON_PRV
                  where numero_prv =  @vl_Numero and 
			autoroute_prv = @vl_Autoroute and
			sens_prv = @va_SensPMV_in and
			PR_prv = @va_PrPMV_in and 
			autoroute_zone = @vl_AutorouteZone and
			sens_zone = @va_SensZone_in  and
			PR_debut_zone = @va_PrDebutZone_in and
			PR_fin_zone = @va_PrFinZone_in and
			type_zone = @va_TypeZone_in
			
               /*A
               ** La zone d'influence n'existe pas dans ZON_PRV
               */
               
               if @vl_NumZone = null
               begin
               
                  /*A
                  ** Insertion de la zone d'influence dans la table ZON_PRV
                  */
                  
                  select @vl_NumZone = max(numero_zone) from CFT..ZON_PRV

                  if @vl_NumZone = null
                    select @vl_NumZone = 0  
            
                  insert into CFT..ZON_PRV (
			numero_zone,
			numero_prv,
			autoroute_prv,
			sens_prv,
			PR_prv,
			site_prv,
			autoroute_zone,
			sens_zone,
			PR_debut_zone,
			PR_fin_zone,
			direction_zone,
			type_zone,
			inhibition,
			distance
		  )
                  values ( @vl_NumZone + 1, 
                 	   @vl_Numero,
			   @vl_Autoroute,
			   @va_SensPMV_in,
			   @va_PrPMV_in,
			   @vl_Site,
			   @vl_AutorouteZone,
			   @va_SensZone_in,
			   @va_PrDebutZone_in,
			   @va_PrFinZone_in,
			   @va_DirectionZone_in,
			   @va_TypeZone_in,
			   @va_Inhibition_in,
			   @va_DistancePMV_in)

                  /*A
                  ** Si erreur d'insertion
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
		* la zone d'influence existe dans ZON_PRV	
               */
                     
               begin
               
                  /*A
                  ** Modification de la zone d'influence dans la table ZON_PRV
                  */
                  
                  update CFT..ZON_PRV
                  set site_prv= @vl_Site,
		      PR_debut_zone	= @va_PrDebutZone_in, 
		      PR_fin_zone 	= @va_PrFinZone_in,
		      direction_zone 	= @va_DirectionZone_in,
		      type_zone 	= @va_TypeZone_in,
                      inhibition	= @va_Inhibition_in,
                      distance		= @va_DistancePMV_in
                  where numero_zone = @vl_NumZone
               
                  /*A
                  ** Si erreur de modification
                  */
                     
                  if @@rowcount <> 1
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
            ** Le serveur ou l'autoroute n'existe pas 
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
            
               select @va_Resultat_out = XDC_ECHEC
               
         end

#endif
      
      end
   end         
     
   return XDC_OK
         
go  
