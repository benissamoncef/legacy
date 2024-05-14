/*E*/
/* Fichier : $Id: xzao450sp.prc,v 1.9 2020/05/19 14:06:43 pc2dpdy Exp $        $Revision: 1.9 $        $Date: 2020/05/19 14:06:43 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao450sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	: Creation
* JMG   13/11/07	: ajout site de gestion DEM715
* LCL	05/07/17	: Modif PRA
* LCL	11/07/18	: correction syntaxe from  1.4
* JPL	17/10/18	: Nommage des colonnes sur 'insert'  1.7
* JPL	17/10/18	: Ajout du parametre 'supprime' (DEM 1305)  1.8
* LCL	07/06/19	: BAF/IP DEM1332 1.9
* GGY	26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------ */

/* fichiers inclus */

/* #include "xzao450sp.h" */

/* static char *version = "$Id: xzao450sp.prc,v 1.9 2020/05/19 14:06:43 pc2dpdy Exp $ $Revision: 1.9 $ $Date: 2020/05/19 14:06:43 $ : xzao450" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO450' and type = 'P')
	drop procedure XZAO450
go


create procedure XZAO450
   @va_Basedd_in        	char(3),
   @va_NomBAF_in		char(10),
   @va_Autoroute_in		char(4),
   @va_PR_in			int,
   @va_Sens_in			tinyint,
   @va_NomServeur_in		char(10),
   @va_Liaison_in		tinyint,
   @va_Rgs_in			char(3),
   @va_Type_in			tinyint,
   @va_NomMod_in		char(10),
   @va_CodeMod_in		char(2),
   @va_AutoMod_in		char(4),
   @va_PRMod_in			int,
   @va_SensMod_in		tinyint,
   @va_NomSite_in		char(2),
   @va_Supprime_in		bit,
   @va_Adresse_IP_in 		char(20),
   @va_Port_in			int,
   @va_Resultat_out		int output
   as 

/* 
* SERVICE RENDU : 
*
* Ajout d'une BAF dans la base
*
* SEQUENCE D'APPEL :
*
* SP	XZAO450_Ajouter_BAF
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_NomBAF_in
* XDY_NomAuto		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_NomEqt		va_Serveur_in
* XDY_Octet		va_Liaison_in
* XDY_Rgs		va_Rgs_in
* XDY_Octet		va_Type_in
* XDY_NomEqt		va_NomMod_in
* char(2)		va_CodeMod_in
* XDY_NomAuto		va_AutoMod_in
* XDY_PR		va_PRMod_in
* XDY_Sens		va_SensMod_in
* XDY_NomSite		va_NomSite_in
* XDY_Booleen		va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Mot	   va_NumBAF_out
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une BAF en base
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
* Insertion ou modification dans la table EQT_BAF
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_AutoMod		tinyint,
   @vl_Type			tinyint,
   @vl_Serveur		smallint,
   @vl_NumBAF		smallint,
   @vl_Numero		smallint,
   @vl_Num			smallint,
   @vl_NumModule	smallint,
   @vl_NumMod		smallint,
   @vl_Site			tinyint,
   @vl_SiteNom		char(10)

   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NomBAF_in = null 
   or @va_Autoroute_in = null 
   or @va_PR_in = null 
   or @va_Sens_in = null 
   or @va_NomServeur_in = null
   or @va_Liaison_in = null 
   or @va_Rgs_in = null
   or @va_Type_in = null
   or @va_NomMod_in = null
   or @va_CodeMod_in = null
   or @va_AutoMod_in = null
   or @va_PRMod_in = null
   or @va_SensMod_in = null 
   or @va_NomSite_in = null
   or @va_Supprime_in = null
   or @va_Adresse_IP_in = null
   or @va_Port_in = null
   
      return XDC_ARG_INV
   
   else   
   begin
      
		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_SiteNom = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_SiteNom=XDC_SQLCI or @vl_SiteNom=XDC_SQLCA
		begin tran
      
      
         /*A
         **  Travail dans la base CFG
         */   
      
         if @va_Basedd_in = XDC_BASE_CFG
         begin
         
            /*A
            ** Verification de l'existence et recherche de l'autoroute et de la BAF comme equipement
            */
         
            select @vl_Autoroute = numero from CFG..RES_AUT
              where nom = @va_Autoroute_in
            
            select @vl_AutoMod = numero from CFG..RES_AUT
              where nom = @va_AutoMod_in
            
            select @vl_Serveur = numero from CFG..EQT_GEN
              where nom  = @va_NomServeur_in
                and type = XDC_EQT_SRV
              
 /*           select @vl_Type = numero from CFG..TYP_BAF
              where nom  = @va_Type_in*/
              
	 select @vl_Site = numero from CFG..RES_DIS
		 where code = @va_NomSite_in

	     if @vl_Site = NULL
	     return XDC_ARG_INV

            /*A
            ** Existence de l'autoroute et du serveur
            */
            
            if @vl_Autoroute != null and @vl_AutoMod != null and @vl_Serveur != null /*and @vl_Type != null*/
            begin
               
               select @vl_NumBAF = numero from CFG..EQT_GEN
                 where type      = XDC_EQT_BAF
                   and nom       = @va_NomBAF_in 
                   
               /*A
               ** La BAF n'existe pas comme equipement
               */
            
               if @vl_NumBAF = null
               begin
                  
                  /*A
                  ** Insertion de la BAF dans la table EQT_GEN et EQT_BAF
                  */
               
                  select @vl_Numero = max(numero) from CFG..EQT_GEN
                    where type = XDC_EQT_BAF
                    
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
                  values ( XDC_EQT_BAF,
                           @vl_Numero + 1,
                           @vl_Serveur,
                           XDC_MAI_BAF,
                           @vl_Autoroute,
                           @va_PR_in,
                           @va_Sens_in,
                           @va_NomBAF_in,
                           @vl_Site,
                           @va_Supprime_in
                         )
                  
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                     */
         
                     select @va_Resultat_out = XDC_AJOUT
                        
                     return XDC_NOK
                  end   
                        
                  insert into CFG..EQT_BAF
                  values ( @vl_Numero + 1, @va_Type_in, @va_Liaison_in, @va_Rgs_in , @va_Adresse_IP_in, @va_Port_in)
                  
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
                  ** Stockage des modules BAF
                  */
                  
                  select @vl_NumMod = max(numero) from CFG..EQT_GEN
                    where type = XDC_EQT_MODBAF
                    
                  if @vl_NumMod = null
                     select @vl_NumMod = 0  
                  
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
                  values ( XDC_EQT_MODBAF,
                           @vl_NumMod + 1,
                           @vl_Numero + 1,
                           XDC_MAI_MODBAF,
                           @vl_AutoMod,
                           @va_PRMod_in,
                           @va_SensMod_in,
                           @va_NomMod_in,
                           @vl_Site,
                           XDC_FAUX
                         )
                  
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
                        
                  insert into CFG..EQT_MODBAF
                  values ( @vl_NumMod + 1, @va_CodeMod_in )
                  
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
               ** La BAF existe comme equipement
               */
            
               begin
               
                  /*select @va_NumBAF_out = @vl_NumBAF*/
                  
                  /*A
                  ** Modification de la BAF dans la table EQT_GEN et EQT_BAF
                  */
               
                  update CFG..EQT_GEN
                  set maitre      = @vl_Serveur,
                      type_maitre = XDC_MAI_BAF,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      sens        = @va_Sens_in,
		      site_gestion = @vl_Site,
		      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumBAF
                    and type   = XDC_EQT_BAF    
                
                  /*A
                  ** Erreur de modification
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_MODIF
                     */
         
                     select @va_Resultat_out = XDC_MODIF
                        
                     return XDC_NOK
                  end   
                        
                  update CFG..EQT_BAF
                  set type          = @va_Type_in,
                      liaison_serie = @va_Liaison_in,
                      rgs           = @va_Rgs_in,
		      adresse_ip   = @va_Adresse_IP_in,
		      port         = @va_Port_in
                  where numero = @vl_NumBAF   
 
 
                  /*A
                  ** Erreur de modification
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
                  ** Insertion ou Update du module de la BAF
                  */
                  
                  select @vl_NumModule = numero from CFG..EQT_GEN
                    where type 	 	= XDC_EQT_MODBAF
                      and nom    	= @va_NomMod_in
                      
                      
                  
                  /*A
                  ** Le Module BAF n'existe pas comme equipement
                  ** Insertion dans EQT_GEN et EQT_MODBAF
                  */
                  
                  if @vl_NumModule = null
                  begin
                  
                     select @vl_Num = max(numero) from CFG..EQT_GEN
                       where type = XDC_EQT_MODBAF
                    
                     if @vl_Num = null
                        select @vl_Num = 0  
                  
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
                     values ( XDC_EQT_MODBAF,
                              @vl_Num + 1,
                              @vl_NumBAF,
                              XDC_MAI_MODBAF,
                              @vl_AutoMod,
                              @va_PRMod_in,
                              @va_SensMod_in,
                              @va_NomMod_in,
                              @vl_Site,
                              XDC_FAUX
                            )
                  
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
                        
                     insert into CFG..EQT_MODBAF
                     values ( @vl_Num + 1, @va_CodeMod_in )
                  
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
                  
                  begin
                  
                  /*A
                  ** Le Module BAF existe comme equipement
                  ** Update EQT_GEN et EQT_MOD
                  */
                  
                     update CFG..EQT_GEN
                     set autoroute   = @vl_AutoMod,
                         PR          = @va_PRMod_in,
                         sens        = @va_SensMod_in,
			 maitre      = @vl_NumBAF,
			 type_maitre = XDC_MAI_MODBAF,
			 site_gestion = @vl_Site
                     where type 	= XDC_EQT_MODBAF
                       and numero 	= @vl_NumModule
                      
                     /*A
                     ** Erreur de modification
                     */
                     
                     if @@rowcount <> 1
                     begin
                     
                        /*A
                        ** Affectation du parametre de sortie Resultat a XDC_MODIF
                        */
         
                        select @va_Resultat_out = XDC_MODIF
                        
                        return XDC_NOK
                     end   
                        
                     
                     update CFG..EQT_MODBAF
                     set code     = @va_CodeMod_in
                     where numero = @vl_NumModule   
 
                     /*A
                     ** Erreur de modification
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
            end
            
            else
            
            /*A
            ** L'autoroute ou le serveur n'existe pas
            **
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
               select @va_Resultat_out = XDC_ECHEC
         
         end

#ifdef CI
         
         else
         
         begin
         
         /*A
         **  Travail dans la base CFT
         */   
      
            /*A
            ** Verification de l'existence et recherche de l'autoroute et de la BAF comme equipement
            */
         
            select @vl_Autoroute = numero from CFT..RES_AUT
              where nom = @va_Autoroute_in
            
            select @vl_AutoMod = numero from CFT..RES_AUT
              where nom = @va_AutoMod_in
            
            select @vl_Serveur = numero from CFT..EQT_GEN
              where nom  = @va_NomServeur_in
                and type = XDC_EQT_SRV
              
    /*        select @vl_Type = numero from CFT..TYP_BAF
              where nom  = @va_Type_in*/
              
	    select @vl_Site = numero from CFT..RES_DIS
		    where code = @va_NomSite_in

	if @vl_Site = NULL
		return XDC_ARG_INV

            /*A
            ** Existence de l'autoroute et du serveur
            */
            
            if @vl_Autoroute != null and @vl_AutoMod != null and @vl_Serveur != null /*and @vl_Type != null*/
            begin
               
               select @vl_NumBAF = numero from CFT..EQT_GEN
                 where type      = XDC_EQT_BAF
                   and nom       = @va_NomBAF_in 
                   
               select @vl_NumMod = numero from CFT..EQT_GEN
                 where type      = XDC_EQT_MODBAF
                   and nom       = @va_NomMod_in 
                   
               /*A
               ** La BAF n'existe pas comme equipement
               */
            
               if @vl_NumBAF = null
               begin
                  
                  /*A
                  ** Insertion de la BAF dans la table EQT_GEN et EQT_BAF
                  */
               
                  select @vl_Numero = max(numero) from CFT..EQT_GEN
                    where type = XDC_EQT_BAF
                    
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
                  values ( XDC_EQT_BAF,
                           @vl_Numero + 1,
                           @vl_Serveur,
                           XDC_MAI_BAF,
                           @vl_Autoroute,
                           @va_PR_in,
                           @va_Sens_in,
                           @va_NomBAF_in,
                           @vl_Site,
                           @va_Supprime_in
                         )
                  
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_AJOUT
                     */
         
                     select @va_Resultat_out = XDC_AJOUT
                        
                     return XDC_NOK
                  end   
                        
                  insert into CFT..EQT_BAF (numero, type, liaison_serie,rgs, adresse_ip, port)
                  values ( @vl_Numero + 1, @va_Type_in, @va_Liaison_in, @va_Rgs_in , @va_Adresse_IP_in, @va_Port_in)
                  
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
                  ** Stockage des modules BAF
                  */
                  
                  select @vl_NumMod = max(numero) from CFT..EQT_GEN
                    where type = XDC_EQT_MODBAF
                    
                  if @vl_NumMod = null
                     select @vl_NumMod = 0  
                  
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
                  values ( XDC_EQT_MODBAF,
                           @vl_NumMod + 1,
                           @vl_Numero + 1,
                           XDC_MAI_MODBAF,
                           @vl_AutoMod,
                           @va_PRMod_in,
                           @va_SensMod_in,
                           @va_NomMod_in,
                           @vl_Site,
                           XDC_FAUX
                         )
                  
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
                        
                  insert into CFT..EQT_MODBAF
                  values ( @vl_NumMod + 1, @va_CodeMod_in )
                  
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
               ** La BAF existe comme equipement
               */
            
               begin
               
                  /*select @va_NumBAF_out = @vl_NumBAF*/
                  
                  /*A
                  ** Modification de la BAF dans la table EQT_GEN et EQT_BAF
                  */
               
                  update CFT..EQT_GEN
                  set maitre      = @vl_Serveur,
                      type_maitre = XDC_MAI_BAF,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      sens        = @va_Sens_in,
		      site_gestion = @vl_Site,
		      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumBAF
                    and type   = XDC_EQT_BAF    
                
                  /*A
                  ** Erreur de modification
                  */
                     
                  if @@rowcount <> 1
                  begin
                     
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_MODIF
                     */
         
                     select @va_Resultat_out = XDC_MODIF
                        
                     return XDC_NOK
                  end   
                        
                  update CFT..EQT_BAF
                  set type          = @va_Type_in,
                      liaison_serie = @va_Liaison_in,
                      rgs           = @va_Rgs_in,
		      adresse_ip   = @va_Adresse_IP_in,
		      port         = @va_Port_in
                  where numero = @vl_NumBAF   
 
 
                  /*A
                  ** Erreur de modification
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
                  ** Insertion ou Update du module de la BAF
                  */
                  
                  select @vl_NumModule = numero from CFT..EQT_GEN
                    where type 	 	= XDC_EQT_MODBAF
                      and nom    	= @va_NomMod_in
                      
                      
                  
                  /*A
                  ** Le Module BAF n'existe pas comme equipement
                  ** Insertion dans EQT_GEN et EQT_MOD
                  */
                  
                  if @vl_NumModule = null
                  begin
                  
                     select @vl_Num = max(numero) from CFT..EQT_GEN
                       where type = XDC_EQT_MODBAF
                    
                     if @vl_Num = null
                        select @vl_Num = 0  
                  
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
                     values ( XDC_EQT_MODBAF,
                              @vl_Num + 1,
                              @vl_NumBAF,
                              XDC_MAI_MODBAF,
                              @vl_AutoMod,
                              @va_PRMod_in,
                              @va_SensMod_in,
                              @va_NomMod_in,
                              @vl_Site,
                              XDC_FAUX
                            )
                  
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
                        
                     insert into CFT..EQT_MODBAF
                     values ( @vl_Num + 1, @va_CodeMod_in )
                  
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
                  
                  begin
                  
                  /*A
                  ** Le Module BAF existe comme equipement
                  ** Update EQT_GEN et EQT_MOD
                  */
                  
                     update CFT..EQT_GEN
                     set autoroute   = @vl_AutoMod,
                         PR          = @va_PRMod_in,
                         sens        = @va_SensMod_in,
			 maitre       = @vl_NumBAF,
			 type_maitre  = XDC_MAI_MODBAF,
			 site_gestion = @vl_Site
                     where type 	= XDC_EQT_MODBAF
                       and numero 	= @vl_NumModule
                      
                     /*A
                     ** Erreur de modification
                     */
                     
                     if @@rowcount <> 1
                     begin
                     
                        /*A
                        ** Affectation du parametre de sortie Resultat a XDC_MODIF
                        */
         
                        select @va_Resultat_out = XDC_MODIF
                        
                        return XDC_NOK
                     end   
                        
                     
                     update CFT..EQT_MODBAF
                     set code     = @va_CodeMod_in
                     where numero = @vl_NumModule   
 
                     /*A
                     ** Erreur de modification
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
            end
            
            else
            
            /*A
            ** L'autoroute ou le serveur n'existe pas
            **
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
               select @va_Resultat_out = XDC_ECHEC
         
         
         end

#endif

      commit
   end         
     
   return XDC_OK
         
go  
