/*E*/
/* Fichier : $Id: xzao72sp.prc,v 1.13 2019/01/24 16:23:17 devgfi Exp $        $Revision: 1.13 $        $Date: 2019/01/24 16:23:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao72sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	20 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* volcic	09 Nov 1994	: Ajout #endif (v1.3)
* volcic	15 Nov 1994	: Change va_NomAutoroute_in en va_NomAutoroute_in (v1.4)
* volcic	07 Dec 1995	: Modifs suite rmq recette a blanc (v1.5)
* JMG   13/11/07 : ajout site de gestion DEM715  1.6
* LCL	05/07/17 : Modif PRA  1.7
* LCL	11/07/18 : correction syntaxe from  1.8
* LCL	04/09/18 : Suppression SAGA DEM1306 1.9
* JMG   13/11/07 : ajout site de gestion DEM715  1.6
* LCL	05/07/17 : Modif PRA  1.7
* LCL	11/07/18 : correction syntaxe from  1.8
* LCL	04/09/18 : Suppression SAGA DEM1306 1.9
* JPL	17/10/18 : Nommage des colonnes sur 'insert'  1.10
* LCL	22/10/18 : Ajout Id LCR DEM1306
* LCL	06/11/18 : Ajout Id LCR et type LCR dans EQT_PCT DEM1306 1.12
* JPL	24/01/19 : Ajout du parametre 'supprime' (DEM 1305)  1.13
* GGY	26/09/23 :	Ajout district CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */
/* #include "xzao72sp.h" */
/* static char *version = "@(#)xzao72.c	1.1 23/18/94 : xzao72" ; */
/* declaration de fonctions internes */
/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un PMV et d'un Picto dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO72' and type = 'P')
	drop procedure XZAO72
go

create procedure XZAO72
	@va_Basedd_in       char(3),
	@va_NomPMV_in		char(10),
	@va_NomAutoroute_in	char(4),
	@va_PR_in			int,
	@va_Sens_in			tinyint,
	@va_NomServeur_in	char(25),
	@va_LiaisonPMV_in	tinyint,
	@va_RgsPMV_in		char(3),
	@va_TypePMV_in		char(25),
	@va_LiaisonPicto_in	tinyint,
	@va_RgsPicto_in		char(3),
	@va_TypePicto_in	char(25),
	@va_NomSite_in		char(2),
	@va_AdresseIP_in	char(21),
	@va_PortIP_in		int,
	@va_TypeLCR_in		tinyint,
	@va_IdLCR_in		char(50),
	@va_Supprime_in		bit,
	@va_Resultat_out	int output
   as 

/* 
*
* SP	XZAO72SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt    va_NomPMV_in
* XDY_Nom       va_NomAutoroute_in
* XDY_PR        va_PR_in
* XDY_Sens      va_Sens_in
* XDY_NomEqt    va_NomServeur_in
* XDY_Octet     va_LiaisonPMV_in
* XDY_Rgs       va_RgsPMV_in
* XDY_Nom       va_TypePMV_in
* XDY_Octet     va_LiaisonPicto_in
* XDY_Rgs       va_RgsPicto_in
* XDY_Nom       va_TypePicto_in			
* XDY_NomSite   va_NomSite_in			
* XDY_AdresseIP va_AdresseIP_in
* XDY_Entier    va_PortIP_in
* XDY_Octet		va_TypeLCR_in
* char			va_IdLCR_in[50]
* XDY_Booleen	va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un PMV et un Picto en base
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
* Insertion ou modification dans les tables EQT_PMV (C_PMV), EQT_PCT (C_PICTO) et EQT_GEN (EQUIPEMENT)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute		tinyint,
   @vl_Serveur			smallint,
   @vl_Machine			smallint,
   @vl_NumPMV			smallint,
   @vl_TypePMV			tinyint,
   @vl_NumPicto			smallint,
   @vl_TypePicto		tinyint,
   @vl_Numero			smallint,
   @vl_NumPct			smallint,
   @vl_Site				tinyint,
   @vl_SiteNom			char(10),
   @vl_AdresseIP_in		char(20),
   @vl_presence_picto	tinyint
   
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null
   or @va_NomPMV_in = null 
   or @va_NomAutoroute_in = null 
   or @va_PR_in = null 
   or @va_Sens_in = null 
   or @va_NomServeur_in = null 
   or @va_LiaisonPMV_in = null 
   or @va_RgsPMV_in = null 
   or @va_TypePMV_in = null
   or @va_LiaisonPicto_in = null 
   or @va_RgsPicto_in = null 
   or @va_TypePicto_in = null  
   or @va_NomSite_in = null
   or @va_AdresseIP_in = null
   or @va_PortIP_in = null
   or @va_TypeLCR_in = null
   or @va_IdLCR_in = null
   or @va_Supprime_in = null

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
         ** Mise a jour de la table EQT_GEN 
         */
         
         /*B
         ** Verification de l'existence et recherche du numero de l'autoroute ainsi
         ** que du serveur associe au PMV
         */
         
         select @vl_Autoroute = numero from CFG..RES_AUT
           where nom = @va_NomAutoroute_in
           
         select @vl_Serveur = numero from CFG..EQT_GEN
           where nom  = @va_NomServeur_in
             and type = XDC_EQT_SRV
          
	    select @vl_Site = numero from CFG..RES_DIS
    where code = @va_NomSite_in

	if @vl_Site = NULL
		return XDC_ARG_INV

         /*A
         ** l'autoroute et le serveur associe au PMV existent
         */
         
         if @vl_Autoroute <> null and @vl_Serveur <> null
         begin
         
            /*A 
            ** Insertion ou Modification du PMV
            */
            
            /*B
            ** Verification de l'existence et recherche du type PMV
            */
         
            select @vl_TypePMV = numero, @vl_presence_picto= picto from CFG..TYP_PMV
              where nom = @va_TypePMV_in
              
            if @vl_TypePMV <> null
            begin
              
                  /*B
                  ** Verification de l'existence et Recherche de l'equipement PMV
                  */
         
                  select @vl_NumPMV = numero from CFG..EQT_GEN
                    where type	= XDC_EQT_PMV
                      and nom	= @va_NomPMV_in
         
              
                  /*A
                  ** Le PMV n'existe pas
                  */
         
                  if @vl_NumPMV = null
                  begin
            
                     /*A
                     ** Insertion dans la table equipement du nouveau PMV
                     */
            
                     select @vl_Numero = max(numero) from CFG..EQT_GEN
                       where type = XDC_EQT_PMV
              
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
                     values ( XDC_EQT_PMV,
                              @vl_Numero + 1,
                              @vl_Serveur,
                              XDC_MAI_PMV,
                              @vl_Autoroute,
                              @va_PR_in,
                              @va_Sens_in,
                              @va_NomPMV_in,
                              @vl_Site,
                              @va_Supprime_in
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
               
                     /*A
                     ** Insertion dans la table PMV du nouveau PMV
                     */
            
                     insert into CFG..EQT_PMV (numero, type, liaison_serie, rgs, adresse_ip, port, type_lcr, id_lcr)
                     values ( @vl_Numero + 1, @vl_TypePMV, @va_LiaisonPMV_in, @va_RgsPMV_in, @va_AdresseIP_in, @va_PortIP_in, @va_TypeLCR_in, @va_IdLCR_in ) 
            
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
                  end /* Fin Le PMV n'existe pas */
                  
                  else
         
                  /*A
                  ** Le PMV existe deja
                  */ 
              
                  begin
           
                    /*A
                    ** Modification dans la table du PMV
                    */
            
                    update CFG..EQT_GEN
    	            set maitre      = @vl_Serveur,
    	                type_maitre = XDC_MAI_PMV,
                        autoroute   = @vl_Autoroute,
                        PR          = @va_PR_in,
                        sens        = @va_Sens_in,
                        nom         = @va_NomPMV_in,
			site_gestion = @vl_Site,
			eqt_supprime = @va_Supprime_in
                    where numero = @vl_NumPMV
                      and type   = XDC_EQT_PMV 
    
                    /*A
                    ** Erreur de modificaion
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
                    ** Modification dans la table du PMV
                    */
            
                    update CFG..EQT_PMV
    	            set rgs     	= @va_RgsPMV_in,
    	                liaison_serie 	= @va_LiaisonPMV_in,
    	                type            = @vl_TypePMV,
			adresse_ip	= @va_AdresseIP_in,
			port		= @va_PortIP_in,
			type_lcr	= @va_TypeLCR_in,
			id_lcr		= @va_IdLCR_in
                    where numero = @vl_NumPMV
    
                    /*A
                    ** Erreur de modificaion
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
               
                 end /* Fin Le PMV existe deja */
             end /* Fin Le type de PMV existe */
         
             else   
             
             /*A
             ** Le Type de PMV n'existe pas
             ** Affectation du parametre de sortie Resultat a XDC_ECHEC
             */
             begin
         

                select @va_Resultat_out = XDC_ECHEC
	        return XDC_OK
             end
         
             /*A 
             ** Insertion ou Modification du Picto
             */
            
             /*B
             ** Verification de l'existence et recherche du type PMV
             */
         
	     /*ne pas inserer un picto pour un PMV sans picto*/
	     if @vl_presence_picto <> 1
	     	return  XDC_OK

             select @vl_TypePicto = numero from CFG..TYP_PCT
               where nom = @va_TypePicto_in
              
             if @vl_TypePicto <> null
             begin
              
               /*B
               ** Verification de l'existence et Recherche de l'equipement Picto
               */
         
               select @vl_NumPicto = numero from CFG..EQT_GEN
                 where type      = XDC_EQT_PCT
                   and nom       = @va_NomPMV_in
       
              
                /*A
                ** Le Picto n'existe pas
                */
         
                if @vl_NumPicto = null
                begin
            
                   /*A
                   ** Insertion dans la table equipement du nouveau Picto
                   */
            
                   select @vl_NumPct = max(numero) from CFG..EQT_GEN
                     where type = XDC_EQT_PCT
                        
                   if @vl_NumPct = null  
                      select @vl_NumPct = 0
                      
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
                   values ( XDC_EQT_PCT,
                            @vl_NumPct + 1,
                            @vl_Serveur,
                            XDC_MAI_PCT,
                            @vl_Autoroute,
                            @va_PR_in,
                            @va_Sens_in,
                            @va_NomPMV_in,
                            @vl_Site,
                            @va_Supprime_in
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
               
                   /*A
                   ** Insertion dans la table Picto du nouveau Picto
                   */
            
                     insert into CFG..EQT_PCT (numero, type, liaison_serie, rgs, adresse_ip, port, type_lcr, id_lcr)
                     values ( @vl_NumPct + 1, @vl_TypePicto, @va_LiaisonPicto_in, @va_RgsPicto_in, @va_AdresseIP_in, @va_PortIP_in, @va_TypeLCR_in, @va_IdLCR_in ) 
            
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
                   
                end /* Fin Le Picto n'existe pas */
                  
                else
         
                /*A
                ** Le Picto existe deja
                */ 
              
                begin
           
                   /*A
                   ** Modification dans la table du Picto
                   */
            
                   update CFG..EQT_GEN
    	           set maitre      = @vl_Serveur,
    	               type_maitre = XDC_MAI_PCT,
                       autoroute = @vl_Autoroute,
                       PR        = @va_PR_in,
                       sens      = @va_Sens_in,
                       nom       = @va_NomPMV_in,
                       site_gestion = @vl_Site,
                       eqt_supprime = @va_Supprime_in
                   where numero = @vl_NumPicto
                     and type   = XDC_EQT_PCT  
    
                   /*A
                   ** Erreur de modificaion
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
                    ** Modification dans la table du PMV
                    */
            
                    update CFG..EQT_PCT
    	            set rgs     	= @va_RgsPicto_in,
    	                liaison_serie 	= @va_LiaisonPicto_in,
                        type   		= @vl_TypePicto,
			adresse_ip	= @va_AdresseIP_in,
			port		= @va_PortIP_in,
			type_lcr	= @va_TypeLCR_in,
			id_lcr		= @va_IdLCR_in
                    where numero = @vl_NumPicto
    
                    /*A
                    ** Erreur de modificaion
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
             
                end /* Fin Le Picto existe deja */
            end
            else
            /*A Le type de picto n'existe pas */
            begin
               select @va_Resultat_out = XDC_ECHEC
               return (XDC_OK) 
            end /* Fin Le type de Picto n'existe pas */
         
         end /* Fin test sur autoroute et serveur */
         else   
         /*A
         ** L'autoroute ou le serveur n'existe pas
         ** Affectation du parametre de sortie Resultat a XDC_ECHEC
         */
         begin
         
           select @va_Resultat_out = XDC_ECHEC
	   return XDC_OK
	 end
      
      end /* Fin test base CFG */

#ifdef CI
      
       else   
      /*A
      **  Travail dans la base CFT
      */   
      
      begin
         
         /*A
         ** Mise a jour de la table EQT_GEN 
         */
         
         /*B
         ** Verification de l'existence et recherche du numero de l'autoroute ainsi
         ** que du serveur associe au PMV
         */
         
         select @vl_Autoroute = numero from CFT..RES_AUT
           where nom = @va_NomAutoroute_in
           
         select @vl_Serveur = numero from CFT..EQT_GEN
           where nom  = @va_NomServeur_in
             and type = XDC_EQT_SRV
          
     select @vl_Site = numero from CFT..RES_DIS
	     where code = @va_NomSite_in

	 if @vl_Site = NULL
		 return XDC_ARG_INV

         /*A
         ** l'autoroute et le serveur associe au PMV existent
         */
         
         if @vl_Autoroute <> null and @vl_Serveur <> null
         begin
         
            /*A 
            ** Insertion ou Modification du PMV
            */
            
            /*B
            ** Verification de l'existence et recherche du type PMV
            */
         
            select @vl_TypePMV = numero, @vl_presence_picto = picto from CFT..TYP_PMV
              where nom = @va_TypePMV_in
              
            if @vl_TypePMV <> null
            begin
              
                  /*B
                  ** Verification de l'existence et Recherche de l'equipement PMV
                  */
         
                  select @vl_NumPMV = numero from CFT..EQT_GEN
                    where type	= XDC_EQT_PMV
                      and nom	= @va_NomPMV_in
         
              
                  /*A
                  ** Le PMV n'existe pas
                  */
         
                  if @vl_NumPMV = null
                  begin
            
                     /*A
                     ** Insertion dans la table equipement du nouveau PMV
                     */
            
                     select @vl_Numero = max(numero) from CFT..EQT_GEN
                       where type = XDC_EQT_PMV
              
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
                     values ( XDC_EQT_PMV,
                              @vl_Numero + 1,
                              @vl_Serveur,
                              XDC_MAI_PMV,
                              @vl_Autoroute,
                              @va_PR_in,
                              @va_Sens_in,
                              @va_NomPMV_in,
                              @vl_Site,
                              @va_Supprime_in
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
               
                     /*A
                     ** Insertion dans la table PMV du nouveau PMV
                     */
            
                     insert into CFT..EQT_PMV (numero, type, liaison_serie, rgs, adresse_ip, port, type_lcr, id_lcr)
                     values ( @vl_Numero + 1, @vl_TypePMV, @va_LiaisonPMV_in, @va_RgsPMV_in, @va_AdresseIP_in, @va_PortIP_in, @va_TypeLCR_in, @va_IdLCR_in ) 
            
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
                  end /* Fin Le PMV n'existe pas */
                  
                  else
         
                  /*A
                  ** Le PMV existe deja
                  */ 
              
                  begin
           
                    /*A
                    ** Modification dans la table du PMV
                    */
            
                    update CFT..EQT_GEN
    	            set maitre      = @vl_Serveur,
    	                type_maitre = XDC_MAI_PMV,
                        autoroute   = @vl_Autoroute,
                        PR          = @va_PR_in,
                        sens        = @va_Sens_in,
                        nom         = @va_NomPMV_in,
                        site_gestion = @vl_Site,
                        eqt_supprime = @va_Supprime_in
                    where numero = @vl_NumPMV
                      and type   = XDC_EQT_PMV 
    
                    /*A
                    ** Erreur de modificaion
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
                    ** Modification dans la table du PMV
                    */
            
                    update CFT..EQT_PMV
    	            set rgs     	= @va_RgsPMV_in,
    	                liaison_serie 	= @va_LiaisonPMV_in,
    	                type            = @vl_TypePMV,
			adresse_ip	= @va_AdresseIP_in,
			port		= @va_PortIP_in,
			type_lcr	= @va_TypeLCR_in,
			id_lcr		= @va_IdLCR_in
                    where numero = @vl_NumPMV
    
                    /*A
                    ** Erreur de modificaion
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
               
                 end /* Fin Le PMV existe deja */
             end /* Fin Le type de PMV existe */
         
             else   
             
             /*A
             ** Le Type de PMV n'existe pas
             ** Affectation du parametre de sortie Resultat a XDC_ECHEC
             */
             begin
         

                select @va_Resultat_out = XDC_ECHEC
	        return XDC_OK
             end
         
             /*A 
             ** Insertion ou Modification du Picto
             */
            
             /*B
             ** Verification de l'existence et recherche du type PMV
             */
	     /*ne pas inserer un picto pour un PMV sans picto*/
	     if @vl_presence_picto <> 1
	     	return  XDC_OK
         
             select @vl_TypePicto = numero from CFT..TYP_PCT
               where nom = @va_TypePicto_in
              
             if @vl_TypePicto <> null
             begin
              
               /*B
               ** Verification de l'existence et Recherche de l'equipement Picto
               */
         
               select @vl_NumPicto = numero from CFT..EQT_GEN
                 where type      = XDC_EQT_PCT
                   and nom       = @va_NomPMV_in
       
              
                /*A
                ** Le Picto n'existe pas
                */
         
                if @vl_NumPicto = null
                begin
            
                   /*A
                   ** Insertion dans la table equipement du nouveau Picto
                   */
            
                   select @vl_NumPct = max(numero) from CFT..EQT_GEN
                     where type = XDC_EQT_PCT
                        
                   if @vl_NumPct = null  
                      select @vl_NumPct = 0
                      
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
                   values ( XDC_EQT_PCT,
                            @vl_NumPct + 1,
                            @vl_Serveur,
                            XDC_MAI_PCT,
                            @vl_Autoroute,
                            @va_PR_in,
                            @va_Sens_in,
                            @va_NomPMV_in,
                            @vl_Site,
                            @va_Supprime_in
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
               
                   /*A
                   ** Insertion dans la table Picto du nouveau Picto
                   */
            
                     insert into CFT..EQT_PCT (numero, type, liaison_serie, rgs, adresse_ip,port, type_lcr, id_lcr)
                     values ( @vl_NumPct + 1, @vl_TypePicto, @va_LiaisonPicto_in, @va_RgsPicto_in, @va_AdresseIP_in, @va_PortIP_in, @va_TypeLCR_in, @va_IdLCR_in ) 
            
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
                   
                end /* Fin Le Picto n'existe pas */
                  
                else
         
                /*A
                ** Le Picto existe deja
                */ 
              
                begin
           
                   /*A
                   ** Modification dans la table du Picto
                   */
            
                   update CFT..EQT_GEN
    	           set maitre      = @vl_Serveur,
    	               type_maitre = XDC_MAI_PCT,
                       autoroute = @vl_Autoroute,
                       PR        = @va_PR_in,
                       sens      = @va_Sens_in,
                       nom       = @va_NomPMV_in,
                       site_gestion = @vl_Site,
		       eqt_supprime = @va_Supprime_in
                   where numero = @vl_NumPicto
                     and type   = XDC_EQT_PCT  
    
                   /*A
                   ** Erreur de modificaion
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
                    ** Modification dans la table du PMV
                    */
            
                    update CFT..EQT_PCT
    	            set rgs     	= @va_RgsPicto_in,
    	                liaison_serie 	= @va_LiaisonPicto_in,
                        type  		 = @vl_TypePicto,
			adresse_ip	= @va_AdresseIP_in,
			port		= @va_PortIP_in,
			type_lcr	= @va_TypeLCR_in,
			id_lcr		= @va_IdLCR_in
    	            where numero = @vl_NumPicto
    
                    /*A
                    ** Erreur de modificaion
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
             
                end /* Fin Le Picto existe deja */
            end
            else
            /*A Le type de picto n'existe pas */
            begin
               select @va_Resultat_out = XDC_ECHEC
               return (XDC_OK) 
            end /* Fin Le type de Picto n'existe pas */
         
         end /* Fin test sur autoroute et serveur */
         else   
         /*A
         ** L'autoroute ou le serveur n'existe pas
         ** Affectation du parametre de sortie Resultat a XDC_ECHEC
         */
         begin
           select @va_Resultat_out = XDC_ECHEC
	   return XDC_OK
	 end
         
      end /* fin test base CFT */
#endif
       
    end /* fin verification base CI */
  end  /* fin arguments valides */ 
  return XDC_OK
         
go  
