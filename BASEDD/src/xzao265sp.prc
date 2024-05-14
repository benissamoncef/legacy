/*E*/
/* Fichier : $Id: xzao265sp.prc,v 1.3 2018/11/07 19:19:36 devgfi Exp $        $Revision: 1.3 $        $Date: 2018/11/07 19:19:36 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao265sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	02/03/18 : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
* JPL	17/10/18 : Nommage des colonnes sur 'insert'  1.2
* JPL	17/10/18 : Ajout du parametre 'supprime' (DEM 1305)  1.3
* GGY	26/09/23 : Ajout district CA (DEM483)
* GGY	17/10/23 : Correction si CFG vide (DEM483)
------------------------------------------------------ */

/* fichiers inclus */

/* #include "xzao265sp.h" */

/* static char *version = "@(#)xzao265sp.prc	1.2 1.2 11/19/07 : xzao265" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X
------------------------------------------------------
* SERVICE RENDU : 
* Ajout d'une CAP dans la base
*
* SEQUENCE D'APPEL :
* SP	XZAO265_Ajouter_CAP
*
* Arguments en entree
* XDY_Basedd	 va_Basedd_in
* XDY_NomEqt	 va_NomCAP_in
* XDY_NomAuto	 va_Autoroute_in
* XDY_PR	 va_PR_in
* XDY_Sens	 va_Sens_in	
* XDY_Entier	 va_Type_in
* XDY_Rgs	 va_Rgs_in
* XDY_AdresseIP	 va_AdresseIP_in
* XDY_Port	 va_PortMaitre_in	
* XDY_Port	 va_PortEsclave_in	
* XDY_Octet 	 va_PointCaracteristique_in
* XDY_NomEqt     va_NomServeur_in
* XDY_NomSite    va_NomSite_in
* XDY_Booleen    va_Supprime_in
*
* Arguments en sortie
* XDC_Entier 	va_Resultat_out    
*
------------------------------------------------------
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO265' and type = 'P')
	drop procedure XZAO265
go

create procedure XZAO265
   @va_Basedd_in        	char(3),
   @va_NomCAP_in		char(10),
   @va_Autoroute_in		char(4),
   @va_PR_in			int,
   @va_Sens_in			tinyint,
   @va_Type_in			int,
   @va_Rgs_in			char(4),
   @va_AdresseIP_in		char(20),
   @va_PortMaitre_in		int,
   @va_PortEsclave_in		int,
   @va_PointCaracteristique_in	tinyint,
   @va_NomServeur_in		char(10),
   @va_NomSite_in		char(2),
   @va_Supprime_in		bit,
   @va_Resultat_out		int output
   as 

------------------------------------------------------*/
   /*A
   ** Definition des variables globales
   */

	declare	@vl_Autoroute	tinyint,
			@vl_AutoMod		tinyint,
			@vl_Type		tinyint,
			@vl_Serveur		smallint,
			@vl_NumCAP		smallint,
			@vl_Numero		smallint,
			@vl_Zone		smallint,
			@vl_Num			smallint,
			@vl_pr_deb 		integer,
			@vl_pr_fin 		integer,
			@vl_sens 		smallint,
			@vl_Site		tinyint,
			@vl_SiteNom		char(10)
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in 			= null
	   or @va_NomCAP_in		= null
	   or @va_Autoroute_in		= null
	   or @va_PR_in			= null
	   or @va_Sens_in		= null
	   or @va_Type_in		= null
	   or @va_Rgs_in		= null
	   or @va_AdresseIP_in		= null
	   or @va_PortMaitre_in		= null
	   or @va_PortEsclave_in	= null
	   or @va_PointCaracteristique_in  = null
	   or @va_NomServeur_in		= null
	   or @va_NomSite_in            = null
	   or @va_Supprime_in           = null
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
            ** Verification de l'existence et recherche de l'autoroute et de la CAP comme equipement
            */
            select @vl_Autoroute = numero from CFG..RES_AUT
              where nom = @va_Autoroute_in
            
            select @vl_Serveur = numero from CFG..EQT_GEN
              where nom  = @va_NomServeur_in
                and type = XDC_EQT_SRV
            
	 select @vl_Site = numero from CFG..RES_DIS
		 where code = @va_NomSite_in

	     if @vl_Site = NULL
	     return XDC_ARG_INV

            /*A
            ** Existence de l'autoroute et du serveur
            */
            
            if @vl_Autoroute != null and @vl_Serveur != null /*and @vl_Type != null*/
            begin
               
               select @vl_NumCAP = numero from CFG..EQT_GEN
                 where type      = XDC_EQT_CAP
                   and nom       = @va_NomCAP_in 
                   
               /*A
               ** La CAP n'existe pas comme equipement
               */
            
               if @vl_NumCAP = null
               begin
              
             
                  /*A
                  ** Insertion de la CAP dans la table EQT_GEN et EQT_CAP
                  */
               
                  select @vl_Numero = max(numero) from CFG..EQT_GEN
                    where type = XDC_EQT_CAP
                    
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
                  values ( XDC_EQT_CAP,
                           @vl_Numero + 1,
                           @vl_Serveur,
                           XDC_MAI_CAP,
                           @vl_Autoroute,
                           @va_PR_in,
                           @va_Sens_in,
                           @va_NomCAP_in,
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
                        
                  insert into CFG..EQT_CAP (numero, type, rgs, adresse_ip, port_maitre, port_esclave, point_caracteristique)
                  values ( @vl_Numero + 1, @va_Type_in, @va_Rgs_in, @va_AdresseIP_in, @va_PortMaitre_in, @va_PortEsclave_in, @va_PointCaracteristique_in)
                  
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
                        
               end
               
               else
              
               /*A
               ** La CAP existe comme equipement
               */
               begin
               
                  /*select @va_NumCAP_out = @vl_NumCAP*/
                  
                  /*A
                  ** Modification de la CAP dans la table EQT_GEN et EQT_CAP
                  */
               
                  update CFG..EQT_GEN
                  set maitre      = @vl_Serveur,
                      type_maitre = XDC_MAI_CAP,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      sens        = @va_Sens_in,
		      site_gestion = @vl_Site,
		      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumCAP
                    and type   = XDC_EQT_CAP    
                
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
                        
                  update CFG..EQT_CAP
                  set type              = @va_Type_in,
		      rgs		= @va_Rgs_in,
		      adresse_ip        = @va_AdresseIP_in,
                      port_maitre	= @va_PortMaitre_in,
                      port_esclave	= @va_PortEsclave_in,
		      point_caracteristique = @va_PointCaracteristique_in
                  where numero = @vl_NumCAP   
 
 
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
            ** Verification de l'existence et recherche de l'autoroute et de la CAP comme equipement
            */
         
            select @vl_Autoroute = numero from CFT..RES_AUT
              where nom = @va_Autoroute_in
            
            select @vl_Serveur = numero from CFT..EQT_GEN
              where nom  = @va_NomServeur_in
                and type = XDC_EQT_SRV
              
	    select @vl_Site = numero from CFT..RES_DIS
		    where code = @va_NomSite_in

	if @vl_Site = NULL
		return XDC_ARG_INV
		
            if @vl_Autoroute != null and @vl_Serveur != null /*and @vl_Type != null*/
            begin
               
               select @vl_NumCAP = numero from CFT..EQT_GEN
                 where type      = XDC_EQT_CAP
                   and nom       = @va_NomCAP_in 
                   
               /*A
               ** La CAP n'existe pas comme equipement
               */
            
               if @vl_NumCAP = null
               begin
                  
                  /*A
                  ** Insertion de la CAP dans la table EQT_GEN et EQT_CAP
                  */
               
                  select @vl_Numero = max(numero) from CFT..EQT_GEN
                    where type = XDC_EQT_CAP
                    
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
                  values ( XDC_EQT_CAP,
                           @vl_Numero + 1,
                           @vl_Serveur,
                           XDC_MAI_CAP,
                           @vl_Autoroute,
                           @va_PR_in,
                           @va_Sens_in,
                           @va_NomCAP_in,
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
                        
                  insert into CFT..EQT_CAP (numero, type, rgs, adresse_ip, port_maitre, port_esclave, point_caracteristique)
                  values ( @vl_Numero + 1, @va_Type_in, @va_Rgs_in, @va_AdresseIP_in, @va_PortMaitre_in, @va_PortEsclave_in, @va_PointCaracteristique_in)
                  
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
                        
               end
               
               else
              
               /*A
               ** La CAP existe comme equipement
               */
            
               begin
               
                  /*A
                  ** Modification de la CAP dans la table EQT_GEN et EQT_CAP
                  */
               
                  update CFT..EQT_GEN
                  set maitre      = @vl_Serveur,
                      type_maitre = XDC_MAI_CAP,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      sens        = @va_Sens_in,
		      site_gestion = @vl_Site,
		      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumCAP
                    and type   = XDC_EQT_CAP    
                
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
                        
                  update CFT..EQT_CAP
                  set type              = @va_Type_in,
		      rgs		= @va_Rgs_in,
		      adresse_ip        = @va_AdresseIP_in,
                      port_maitre	= @va_PortMaitre_in,
                      port_esclave	= @va_PortEsclave_in,
		      point_caracteristique = @va_PointCaracteristique_in
                  where numero = @vl_NumCAP   
 
 
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
