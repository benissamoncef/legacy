/*E*/
/* Fichier : $Id: xzao257sp.prc,v 1.6 2018/11/07 18:39:51 devgfi Exp $        $Revision: 1.6 $        $Date: 2018/11/07 18:39:51 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao257sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR  14/12/11   : Creation (DEM/1016)
* RG  10/10/2018 : DEM1307  1.4
* JPL	17/10/18 : Nommage des colonnes sur 'insert'  1.5
* JPL	17/10/18 : Ajout du parametre 'supprime' (DEM 1305)  1.6
* ABE	31/05/21 : PRV sur IP DEM-SAE283 1.7
* GGY	26/09/23 : Ajout district CA (DEM483)
* GGY	17/10/23 : Correction si CFG vide (DEM483)
------------------------------------------------------ */

/* fichiers inclus */

/* #include "xzao257sp.h" */

/* static char *version = "@(#)xzao257sp.prc	1.2 1.2 11/19/07 : xzao257" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO257' and type = 'P')
	drop procedure XZAO257
go

create procedure XZAO257
   @va_Basedd_in          char(3),
   @va_NomPRV_in          char(10),
   @va_Autoroute_in       char(4),
   @va_PR_in              int,
   @va_Sens_in            tinyint,
   @va_NomServeur_in      char(10),
   @va_Liaison_in         tinyint,
   @va_Rgs_in             char(3),
   @va_Type_in            tinyint,
   @va_NomSite_in         char(2),
   @va_VitesseNominale_in smallint,
   @va_NomZone_in         char(50),
   @va_Critique_in        bit,
   @va_BretelleEntree_in  bit,
   @va_RappelNominal_in   bit,
   @va_Supprime_in		bit,
   @va_Adresse_IP_in	char(20),
   @va_Port_in		int,
   @va_Resultat_out       int output
   as 

declare @vl_Autoroute	tinyint,
        @vl_AutoMod		tinyint,
        @vl_Type		tinyint,
        @vl_Serveur		smallint,
        @vl_NumPRV		smallint,
        @vl_Numero		smallint,
        @vl_Zone		smallint,
        @vl_Num			smallint,
        @vl_Site		tinyint,
		@vl_SiteNom		char(10),
        @vl_pr_deb		integer,
        @vl_pr_fin		integer,
        @vl_sens		smallint
   
   select @va_Resultat_out = XDC_LU
   
   /*
   ** Test des parametres d'entree :
   ** Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
  
  if @va_Basedd_in             = null
     or @va_NomPRV_in          = null
     or @va_Autoroute_in       = null 
     or @va_PR_in              = null
     or @va_Sens_in            = null
     or @va_NomServeur_in      = null
     or @va_Liaison_in         = null
     or @va_Rgs_in             = null
     or @va_Type_in            = null
     or @va_NomSite_in         = null
     or @va_VitesseNominale_in = null
     or @va_NomZone_in         = null
     or @va_Critique_in        = null
     or @va_BretelleEntree_in  = null
     or @va_RappelNominal_in   = null
     or @va_Supprime_in        = null
    return XDC_ARG_INV
  
  else		
	   
    begin

		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_SiteNom = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_SiteNom=XDC_SQLCI or @vl_SiteNom=XDC_SQLCA
		begin tran
 
      /* Travail dans la base CFG */
      if @va_Basedd_in = XDC_BASE_CFG
      begin
   
      /* Verification de l'existence et recherche de l'autoroute et de la PRV comme equipement */
      select @vl_Autoroute = numero 
      from CFG..RES_AUT
      where nom = @va_Autoroute_in
            
      select @vl_Serveur = numero 
      from CFG..EQT_GEN
      where nom  = @va_NomServeur_in
      and type = XDC_EQT_SRV
            
      select @vl_Zone = numero 
      from CFG..ZON_REG
      where nom = @va_NomZone_in

      /* DEM1307 - 10/10/2018 - RG */
      if @vl_Zone = NULL
        set @vl_Zone = 0

      select @vl_Site = numero 
      from CFG..RES_DIS
      where code = @va_NomSite_in

      if @vl_Site = NULL
        return XDC_ARG_INV
 
      if @va_Type_in != XDC_PRV_PLEINE_VOIE and @va_Type_in != XDC_PRV_ACCOTEMENT and @va_Type_in != XDC_PRV_BPV
        return XDC_ARG_INV
   		
      /* Verification de la valeur de la vitesse */ 
      if @va_VitesseNominale_in != 130 and
         @va_VitesseNominale_in != 110 and
         @va_VitesseNominale_in != 90  and 
         @va_VitesseNominale_in != 70
        return XDC_ARG_INV

      /* verification de l'equipement par rapport a sa zone */
      select @vl_pr_deb = debut, @vl_pr_fin = fin, @vl_sens = sens 
      from CFG..ZON_REG
      where nom = @va_NomZone_in
           
      if @@rowcount=1
      begin
	if @vl_pr_deb > @va_PR_in or
         @va_PR_in  > @vl_pr_fin
        	return XDC_ARG_INV
      end

      /* Existence de l'autoroute et du serveur */
      if @vl_Autoroute != null and 
         @vl_Serveur   != null /*and @vl_Type != null*/
        begin
               
        select @vl_NumPRV = numero 
        from CFG..EQT_GEN
        where type = XDC_EQT_PRV and 
              nom  = @va_NomPRV_in 
                   
        /* La PRV n'existe pas comme equipement */
        if @vl_NumPRV = null
          begin
                 
            /* Insertion de la PRV dans la table EQT_GEN et EQT_PRV */
            select @vl_Numero = max(numero) 
            from CFG..EQT_GEN
            where type = XDC_EQT_PRV
                    
            if @vl_Numero = null
              select @vl_Numero = 0  
                  
            insert into CFG..EQT_GEN (
                          type,
                          numero,
                          maitre,
                          type_maitre,
                          autoroute,
                          PR,
                          sens,
                          nom,
                          site_gestion,
                          eqt_supprime
                        )
            values ( XDC_EQT_PRV, 
                     @vl_Numero + 1, 
                     @vl_Serveur, 
                     XDC_MAI_PRV, 
                     @vl_Autoroute, 
                     @va_PR_in, 
                     @va_Sens_in, 
                     @va_NomPRV_in,
                     @vl_Site,
                     @va_Supprime_in
                   )

            if @@rowcount <> 1
              begin
                     
                /* Affectation du parametre de sortie Resultat a XDC_AJOUT */
                select @va_Resultat_out = XDC_AJOUT
                return XDC_NOK
              
              end   
                        
            insert into CFG..EQT_PRV 
            ( numero, 
              type, 
              liaison_serie,rgs,
              vitesse_nominale, 
              critique, 
              zone, 
              bretelle_entree, 
              rappel_nominal,
              adresse_ip,
              port )
              values ( @vl_Numero + 1, 
                       @va_Type_in, 
                       @va_Liaison_in, 
                       @va_Rgs_in ,
                       @va_VitesseNominale_in, 
                       @va_Critique_in, 
                       @vl_Zone,
                       @va_BretelleEntree_in,
                       @va_RappelNominal_in,
                       @va_Adresse_IP_in,
                       @va_Port_in)
                  
            /* Erreur d'insertion */
            if @@rowcount <> 1
              begin
                     
              /* Affectation du parametre de sortie Resultat a XDC_AJOUT */
              select @va_Resultat_out = XDC_AJOUT 
              return XDC_NOK

            end   
                        
          end
               
            else
              
              /* La PRV existe comme equipement */
              begin
               
              /*select @va_NumPRV_out = @vl_NumPRV*/
              /* Modification de la PRV dans la table EQT_GEN et EQT_PRV */
              update CFG..EQT_GEN
              set maitre       = @vl_Serveur,
                  type_maitre  = XDC_MAI_PRV,
                  autoroute    = @vl_Autoroute,
                  PR           = @va_PR_in,
                  sens         = @va_Sens_in,
                  site_gestion = @vl_Site,
                  eqt_supprime	= @va_Supprime_in
              where numero = @vl_NumPRV and 
                    type   = XDC_EQT_PRV    
                
              /* Erreur de modification */
              if @@rowcount <> 1
                begin
                     
                /* Affectation du parametre de sortie Resultat a XDC_MODIF */
                select @va_Resultat_out = XDC_MODIF   
                return XDC_NOK
              end   
                        
              update CFG..EQT_PRV
              set type             = @va_Type_in,
                  liaison_serie    = @va_Liaison_in,
                  rgs              = @va_Rgs_in,
                  vitesse_nominale = @va_VitesseNominale_in,
                  critique         = @va_Critique_in,
                  zone             = @vl_Zone,
                  bretelle_entree  = @va_BretelleEntree_in,
                  rappel_nominal   = @va_RappelNominal_in,
                  adresse_ip = @va_Adresse_IP_in,
                  port = @va_Port_in
                where numero = @vl_NumPRV   
 
                /* Erreur de modification */
                if @@rowcount <> 1
                  begin
                     
                  /* Affectation du parametre de sortie Resultat a XDC_MODIF */
                  select @va_Resultat_out = XDC_MODIF
                  return XDC_NOK

                end 
              end
            end
            
            else
            
            /* L'autoroute ou le serveur n'existe pas
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
            select @va_Resultat_out = XDC_ECHEC
         
          end

#ifdef CI
         
         else
         
           begin
         
         /* Travail dans la base CFT */   
         /* Verification de l'existence et recherche de l'autoroute et de la PRV comme equipement */
         
         select @vl_Autoroute = numero 
         from CFT..RES_AUT
         where nom = @va_Autoroute_in
            
         select @vl_Serveur = numero 
         from CFT..EQT_GEN
         where nom  = @va_NomServeur_in and
               type = XDC_EQT_SRV
              
         /* select @vl_Type = numero 
         from CFT..TYP_PRV
         where nom  = @va_Type_in */
	      
         select @vl_Zone = numero 
         from CFT..ZON_REG
         where nom = @va_NomZone_in

         /* DEM 1307 - 10/10/2018 - RG */
         if @vl_Zone = NULL
           set @vl_Zone = 0
              
	       select @vl_Site = numero 
         from CFT..RES_DIS
		     where code = @va_NomSite_in

	       if @vl_Site = NULL
		       return XDC_ARG_INV
		
         if @va_Type_in !=XDC_PRV_PLEINE_VOIE and 
            @va_Type_in != XDC_PRV_ACCOTEMENT and
	    @va_Type_in != XDC_PRV_BPV
   		     return XDC_ARG_INV
   		
         /* Verification de la valeur de la vitesse */ 
	       if @va_VitesseNominale_in != 130 and
		        @va_VitesseNominale_in != 110 and
		        @va_VitesseNominale_in != 90  and 
		        @va_VitesseNominale_in != 70
			     return XDC_ARG_INV

         /* verification de l'equipement par rapport a sa zone */
         select @vl_pr_deb = debut, @vl_pr_fin = fin, @vl_sens = sens 
         from CFT..ZON_REG
         where nom = @va_NomZone_in
          
	 if @@rowcount=1
	 begin
	   	   if @vl_pr_deb > @va_PR_in or
	   	      @va_PR_in > @vl_pr_fin
	   		   return XDC_ARG_INV
	end         
         /* Existence de l'autoroute et du serveur */
         if @vl_Autoroute != null and @vl_Serveur != null /*and @vl_Type != null*/
           begin
               
           select @vl_NumPRV = numero 
           from CFT..EQT_GEN
           where type = XDC_EQT_PRV and 
                 nom  = @va_NomPRV_in 
                   
           /* La PRV n'existe pas comme equipement */
           if @vl_NumPRV = null
             begin
                  
             /* Insertion de la PRV dans la table EQT_GEN et EQT_PRV */
             select @vl_Numero = max(numero) 
             from CFT..EQT_GEN
             where type = XDC_EQT_PRV
                    
             if @vl_Numero = null
               select @vl_Numero = 0  
                  
             insert into CFT..EQT_GEN (
                           type,
                           numero,
                           maitre,
                           type_maitre,
                           autoroute,
                           PR,
                           sens,
                           nom,
                           site_gestion,
                           eqt_supprime
                         )
             values ( XDC_EQT_PRV, 
                      @vl_Numero + 1, 
                      @vl_Serveur, 
                      XDC_MAI_PRV, 
                      @vl_Autoroute, 
                      @va_PR_in, 
                      @va_Sens_in, 
                      @va_NomPRV_in , 
                      @vl_Site,
                      @va_Supprime_in
                    )
                  
             if @@rowcount <> 1
               begin
                     
               /* Affectation du parametre de sortie Resultat a XDC_AJOUT */
               select @va_Resultat_out = XDC_AJOUT         
               return XDC_NOK
             
             end   
                        
                  insert into CFT..EQT_PRV 
                  ( numero, 
                    type, 
                    liaison_serie,
                    rgs, 
                    vitesse_nominale, 
                    critique, 
                    zone,
                    bretelle_entree,
                    rappel_nominal,
                    adresse_ip,
                    port)
                  values ( @vl_Numero + 1, 
                           @va_Type_in, 
                           @va_Liaison_in, 
                           @va_Rgs_in ,
					                 @va_VitesseNominale_in, 
                           @va_Critique_in, 
                           @vl_Zone,
                           @va_BretelleEntree_in,
                           @va_RappelNominal_in,
                           @va_Adresse_IP_in,
                           @va_Port_in)
                  
                 /* Erreur d'insertion */                     
                  if @@rowcount <> 1
                    begin
                     
                     /* Affectation du parametre de sortie Resultat a XDC_AJOUT */
                     select @va_Resultat_out = XDC_AJOUT
                     return XDC_NOK

                  end   
                        
               end
               
               else
              
               /* La PRV existe comme equipement */
               begin
               
                 /*select @va_NumPRV_out = @vl_NumPRV*/
                  
                 /* Modification de la PRV dans la table EQT_GEN et EQT_PRV */
                 update CFT..EQT_GEN
                 set maitre       = @vl_Serveur,
                     type_maitre  = XDC_MAI_PRV,
                     autoroute    = @vl_Autoroute,
                     PR           = @va_PR_in,
                     sens         = @va_Sens_in,
		                 site_gestion = @vl_Site,
                     eqt_supprime = @va_Supprime_in
                 where numero = @vl_NumPRV and 
                       type   = XDC_EQT_PRV    
                
                 /* Erreur de modification */    
                 if @@rowcount <> 1
                 begin
                     
                   /* Affectation du parametre de sortie Resultat a XDC_MODIF */
                     select @va_Resultat_out = XDC_MODIF
                     return XDC_NOK

                  end   
                        
                  update CFT..EQT_PRV
                  set type             = @va_Type_in,
                      liaison_serie    = @va_Liaison_in,
                      rgs              = @va_Rgs_in,
		                  vitesse_nominale = @va_VitesseNominale_in,
		                  critique         = @va_Critique_in,
			                zone             = @vl_Zone,
			                bretelle_entree  = @va_BretelleEntree_in,
			                rappel_nominal   = @va_RappelNominal_in,
                      adresse_ip = @va_Adresse_IP_in,
                      port = @va_Port_in
                  where numero = @vl_NumPRV   
 
 
                  /* Erreur de modification */
                  if @@rowcount <> 1
                  begin
                    /* Affectation du parametre de sortie Resultat a XDC_MODIF */
                    select @va_Resultat_out = XDC_MODIF    
                    return XDC_NOK

                  end               
                end
              end
            
            else
            
            /* L'autoroute ou le serveur n'existe pas
               Affectation du parametre de sortie Resultat a XDC_ECHEC */
         
            select @va_Resultat_out = XDC_ECHEC
         
         end

#endif

      commit

   end         
     
   return XDC_OK
         
go  
