/*E*/
/* Fichier : $Id: xzao80sp.prc,v 1.9 2021/05/04 13:29:13 pc2dpdy Exp $        $Revision: 1.9 $        $Date: 2021/05/04 13:29:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao80sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	21 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* JMG		13/11/07	: ajout site de gestion DEM715  1.3
* LCL		05/07/17	: Modif PRA  1.4
* LCL		11/07/18	: correction syntaxe from  1.5
* JPL		18/10/18	: Nommage des colonnes sur 'insert'  1.6
* JPL		06/11/18	: Degroupee; Gestion du champ 'Supprime' (DEM 1305)  1.7
* LCL	 	07/09/19	: DAI/IP DEM1333 1.8
* CGR		14/09/20	: Ajout deuxieme adresse IP DEM-SAE175 1.9
* GGY		26/09/23	:	Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao80sp.h" */

/* static char *version = "@(#)xzao80.c	1.1 23/18/94 : xzao80" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une DAI dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO80' and type = 'P')
	drop procedure XZAO80
go

create procedure XZAO80
   @va_Basedd_in        	char(3),
   @va_NomDAI_in		char(10),
   @va_Autoroute_in		char(4),
   @va_PR_in			int,
   @va_Sens_in			tinyint,
   @va_NomServeur_in		char(10),
   @va_LiaisonMaitre_in		tinyint,
   @va_LiaisonEsclave_in	tinyint,
   @va_Rgs_in			char(3),
   @va_NomSite_in		char(2),
   @va_Supprime_in		bit,
   @va_Adresse_IP_in            char(20),
   @va_Port_in                  int,
   @va_Adresse_IPE_in           char(20),
   @va_PortE_in                 int,
   @va_Adresse2_IP_in		char(20),
   @va_Port2_in			int,
   @va_Adresse2_IPE_in		char(20),
   @va_Port2E_in		int,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO80SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in        		
* XDY_NomEqt    va_NomDAI_in
* XDY_Nom       va_Autoroute_in
* XDY_PR        va_PR_in
* XDY_Sens      va_Sens_in
* XDY_NomEqt    va_NomServeur_in
* XDY_Octet     va_LiaisonMaitre_in
* XDY_Octet     va_LiaisonEsclave_in
* XDY_Rgs       va_Rgs_in			
* XDY_NomSite	va_NomSite_in
* XDY_Booleen	va_Supprime_in
* XDY_AdresseIP	va_Adresse_IP_in
* XDY_Port	va_Port_in
* XDY_AdresseIP	va_Adressee_IPE_in
* XDY_Port	va_PortE_in
* XDY_AdresseIP	va_Adresse2_IP_in
* XDY_Port	va_Port2_in
* XDY_AdresseIP	va_Adrresse2_IPE_in
* XDY_Port	va_Port2E_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une DAI en base
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
* Insertion ou modification dans les tables EQT_DAI (DAI) et EQT_GEN (EQUIPEMENT)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_NumDAI		smallint,
   @vl_Serveur		smallint,
   @vl_Numero		smallint,
   @vl_Site			tinyint,
   @vl_SiteNom		char(10)
   
   select @va_Resultat_out = XDC_LU
            
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null
   or @va_NomDAI_in = null 
   or @va_Autoroute_in = null 
   or @va_PR_in = null 
   or @va_Sens_in = null 
   or @va_NomServeur_in = null 
   or @va_LiaisonMaitre_in = null 
   or @va_LiaisonEsclave_in = null 
   or @va_Rgs_in = null   
   or @va_NomSite_in = null
   or @va_Supprime_in = null
   
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
            ** Verification de l'existence et recherche du numero de l'autoroute
            ** et du serveur comme equipement
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

            if @vl_Autoroute <> null and @vl_Serveur <> null
            begin
         
               /*A 
               ** Insertion ou Modification de la DAI
               **
               ** Verification de l'existence et recherche de la DAI dans les equipements
               */
         
               select @vl_NumDAI = numero from CFG..EQT_GEN
                 where type      = XDC_EQT_DAI
                   and nom       = @va_NomDAI_in
              
            
               /*A
               ** La DAI n'existe pas comme equipement
               */
            
               if @vl_NumDAI = null
               begin
              
                  /*A
                  ** Insertion de la DAI dans la table EQT_GEN
                  */
               
                  select @vl_Numero = max(numero) from CFG..EQT_GEN
                    where type      = XDC_EQT_DAI
              
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
                  values ( XDC_EQT_DAI, 
				@vl_Numero + 1, 
				@vl_Serveur,  
				XDC_MAI_DAI, 
				@vl_Autoroute, 
				@va_PR_in, 
				@va_Sens_in, 
				@va_NomDAI_in , 
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
                  ** Insertion de la DAI dans la table EQT_DAI
                  */
               
                  insert into CFG..EQT_DAI
                  values ( @vl_Numero + 1,
		  	   @va_Rgs_in,
			   @va_LiaisonEsclave_in,
			   @va_LiaisonMaitre_in ,
                           @va_Adresse_IP_in,
			   @va_Port_in,
			   @va_Adresse_IPE_in,
			   @va_PortE_in,
			   @va_Adresse2_IP_in,
			   @va_Port2_in,
			   @va_Adresse2_IPE_in,
			   @va_Port2E_in)
               
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
               ** La DAI existe deja comme equipement
               */      
            
               begin
           
                  /*A
                  ** Modification de la DAI dans la table EQT_GEN
                  */
                
                  update CFG..EQT_GEN
    	          set maitre      = @vl_Serveur,
    	              type_maitre = XDC_MAI_DAI,  
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      sens        = @va_Sens_in,
		      site_gestion = @vl_Site,
		      eqt_supprime = @va_Supprime_in

                  where numero = @vl_NumDAI
                    and type   = XDC_EQT_DAI  
    
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
                  ** Modification de la DAI dans la table EQT_DAI
                  */
                
                  update CFG..EQT_DAI
    	          set rgs                   = @va_Rgs_in,
                      liaison_serie_esclave = @va_LiaisonEsclave_in,
                      liaison_serie_maitre  = @va_LiaisonMaitre_in,
                      adresse_ip	    = @va_Adresse_IP_in,
                      port		    = @va_Port_in,
		      adresse_ip_e	    = @va_Adresse_IPE_in,
		      port_e		    = @va_PortE_in,
		      adresse2_ip	    = @va_Adresse2_IP_in,
		      port2		    = @va_Port2_in,
		      adresse2_ip_e	    = @va_Adresse2_IPE_in,
		      port2_e		    = @va_Port2E_in
                  where numero = @vl_NumDAI
                 
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
         
            select @vl_Autoroute = numero from CFT..RES_AUT
              where nom = @va_Autoroute_in
          
            select @vl_Serveur = numero from CFT..EQT_GEN
              where nom  = @va_NomServeur_in
                and type = XDC_EQT_SRV
          
	    select @vl_Site = numero from CFT..RES_DIS
	    where code = @va_NomSite_in

		if @vl_Site = NULL
			return XDC_ARG_INV

            if @vl_Autoroute <> null and @vl_Serveur <> null
            begin
         
               /*A 
               ** Insertion ou Modification de la DAI
               **
               ** Verification de l'existence et recherche de la DAI dans les equipements
               */
         
               select @vl_NumDAI = numero from CFT..EQT_GEN
                 where type      = XDC_EQT_DAI
                   and nom       = @va_NomDAI_in
              
            
               /*A
               ** La DAI n'existe pas comme equipement
               */
            
               if @vl_NumDAI = null
               begin
              
                  /*A
                  ** Insertion de la DAI dans la table EQT_GEN
                  */
               
                  select @vl_Numero = max(numero) from CFT..EQT_GEN
                    where type      = XDC_EQT_DAI
              
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
                  values ( XDC_EQT_DAI, 
			@vl_Numero + 1, 
			@vl_Serveur,  
			XDC_MAI_DAI, 
			@vl_Autoroute, 
			@va_PR_in, 
			@va_Sens_in, 
			@va_NomDAI_in , 
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
                  ** Insertion de la DAI dans la table EQT_DAI
                  */
               
                  insert into CFT..EQT_DAI
                  values ( @vl_Numero + 1,
		  	   @va_Rgs_in,
			   @va_LiaisonEsclave_in,
			   @va_LiaisonMaitre_in ,
                           @va_Adresse_IP_in,
                           @va_Port_in,
			   @va_Adresse_IPE_in,
			   @va_PortE_in,
			   @va_Adresse2_IP_in,
			   @va_Port2_in,
			   @va_Adresse2_IPE_in,
			   @va_Port2E_in)
               
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
               ** La DAI existe deja comme equipement
               */      
            
               begin
           
                  /*A
                  ** Modification de la DAI dans la table EQT_GEN
                  */
                
                  update CFT..EQT_GEN
    	          set maitre      = @vl_Serveur,
    	              type_maitre = XDC_MAI_DAI,  
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      sens        = @va_Sens_in,
		      site_gestion = @vl_Site,
		      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumDAI
                    and type   = XDC_EQT_DAI  
    
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
                  ** Modification de la DAI dans la table EQT_DAI
                  */
                
                  update CFT..EQT_DAI
    	          set rgs                   = @va_Rgs_in,
                      liaison_serie_esclave = @va_LiaisonEsclave_in,
                      liaison_serie_maitre  = @va_LiaisonMaitre_in,
                      adresse_ip	    = @va_Adresse_IP_in,
                      port		    = @va_Port_in,
		      adresse_ip_e	    = @va_Adresse_IPE_in,
		      port_e		    = @va_PortE_in,
		      adresse2_ip	    = @va_Adresse2_IP_in,
		      port2		    = @va_Port2_in,
		      adresse2_ip_e	    = @va_Adresse2_IPE_in,
		      port2_e		    = @va_Port2E_in
                  where numero = @vl_NumDAI
                 
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
          
            else
          
               /*A
               ** L'autoroute n'existe pas
               **
               ** Affectation du parametre de sortie Resultat a XDC_ECHEC
               */
         
               select @va_Resultat_out = XDC_ECHEC
          
         end
         
#endif
         
      commit tran
   end         
     
   return XDC_OK
         
go  
