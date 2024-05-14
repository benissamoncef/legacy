								/*E*/
/* Fichier : $Id: xzao492sp.prc,v 1.8 2021/10/07 17:45:31 pc2dpdy Exp $        $Revision: 1.8 $        $Date: 2021/10/07 17:45:31 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao492sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Insert IMU en base EQT_GEN
*
------------------------------------------------------
* HISTORIQUE :
*
* CGR	28/07/20 	: Creation IMU 1.1 DEM-SAE155
* ABE	09/02/21	: Ajout SousType DEM-SAE155 1.2
* ABE	07/06/21	: Fix sortie sur ajout, maj DEM-SAE155 1.3
* ABE	07/10/21	: Changement type maitre imu = XDC_EQT_MAC DEM-SAE155 1.4
* ABE	07/10/21	: Changement site_gestion DEM-SAE155 1.7
* ABE	07/10/21	: Changement Type_maitre DEM-SAE155 1.8
* GGY	26/09/23	: Ajout district CA (DEM483)
* GGY	17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------ */

/* fichiers inclus */

/* #include "xzao492sp.h" */

/* static char *version = "@(#)xzao492sp.prc	1.1 1.1 07/20/28 : xzao492" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X
------------------------------------------------------
* SERVICE RENDU : 
* Ajout d'une IMU  dans la base
*
* SEQUENCE D'APPEL :
* SP	XZAO492_Ajouter_IMU
*
* Arguments en entree
* XDY_Basedd	 va_Basedd_in
* XDY_NomEqt	 va_NomIMU_in
* XDY_NomAuto	 va_Autoroute_in
* XDY_PR	 va_PR_in
* XDY_Sens	 va_Sens_in
* XDY_Texte	 va_AdresseEndPointOPCUA_in
* XDY_Texte	 va_UserOPCUA_in	
* XDY_Texte	 va_PasswordOPCUA_in
* XDY_NomEqt	 va_NomBICE_in
* XDY_NomEqt     va_NomServeur_in
* XDY_NomSite    va_NomSite_in
* XDY_Booleen    va_Supprime_in
* XDY_Port	 va_PortMaitre_in
* XDY_Entier	 va_SousType_in
* XDY_NomMachine va_NomMachine_in
*
* Arguments en sortie
* XDC_Entier 	va_Resultat_out    
*
------------------------------------------------------
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO492' and type = 'P')
	drop procedure XZAO492
go

create procedure XZAO492
   @va_Basedd_in        	char(3),
   @va_NomIMU_in		char(10),
   @va_Autoroute_in		char(4),
   @va_PR_in			int,
   @va_Sens_in			tinyint,
   @va_AdresseEndPointOPCUA_in	char(251),
   @va_UserOPCUA_in		char(251),
   @va_PasswordOPCUA_in		char(251),
   @va_NomBICE_in		char(10),
   @va_NomServeur_in		char(10),
   @va_NomSite_in		char(2),
   @va_Supprime_in		bit,
   @va_PortMaitre_in		int,
   @va_SousType_in		int,
   @va_NomMachine_in		char(11),
   @va_Resultat_out		int output
   as 

------------------------------------------------------*/
   /*A
   ** Definition des variables globales
   */

declare @vl_Autoroute		tinyint,
		@vl_Serveur			smallint,
		@vl_NumIMU			smallint,
		@vl_Numero			smallint,
		@vl_Site			char(10),
		@vl_Site_gestion	tinyint,
		@vl_Type_maitre		tinyint


   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in 			= null
	   or @va_NomIMU_in		= null
	   or @va_Autoroute_in		= null
	   or @va_PR_in			= null
	   or @va_Sens_in		= null
	   or @va_AdresseEndPointOPCUA_in = null
	   or @va_UserOPCUA_in		= null
	   or @va_PasswordOPCUA_in	= null
	   or @va_NomBICE_in 		= null
	   or @va_NomServeur_in		= null
	   or @va_NomSite_in            = null
	   or @va_Supprime_in           = null
	   or @va_PortMaitre_in		= null
	   or @va_SousType_in		= null
	   or @va_NomMachine_in		= null
         return XDC_ARG_INV
  
  else		
 
   begin
      
      /*A
      ** Verification que le serveur est bien celui du CI
      */
     select @vl_Site_gestion = numero
     from CFG..RES_DIS
     where code = @va_NomSite_in

     select @vl_Type_maitre = type
     from CFG..EQT_GEN
     where nom = @va_NomServeur_in 

		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin tran
 	
         /*A
         **  Travail dans la base CFG
         */   
      
         if @va_Basedd_in = XDC_BASE_CFG
         begin
   
            /*A
            ** Verification de l'existence et recherche de l'autoroute et de l IMU comme equipement
            */
            select @vl_Autoroute = numero from CFG..RES_AUT
              where nom = @va_Autoroute_in
         
            select @vl_Serveur = numero from CFG..EQT_GEN
              where nom  = @va_NomServeur_in
                and type = XDC_EQT_MAC 
	   
	   if @vl_Autoroute = NULL or @vl_Serveur = NULL
	   	return XDC_ARG_INV

            /*A
            ** Existence de l'autoroute et du serveur
            */
           
            if @vl_Autoroute != null and @vl_Serveur != null
            begin
               
               select @vl_NumIMU = numero from CFG..EQT_GEN
                 where type      = XDC_EQT_IMU
                   and nom       = @va_NomIMU_in 
                   
               /*A
               ** L IMU n'existe pas comme equipement
               */
              
               if @vl_NumIMU = null
               begin
              
             
                  /*A
                  ** Insertion de l IMU dans la table EQT_GEN et EQT_IMU
                  */
               
                  select @vl_Numero = max(numero) from CFG..EQT_GEN
                    where type = XDC_EQT_IMU
                    
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
                  values ( XDC_EQT_IMU,
                           @vl_Numero + 1,
                           @vl_Serveur,
                           @vl_Type_maitre,
                           @vl_Autoroute,
                           @va_PR_in,
                           @va_Sens_in,
                           @va_NomIMU_in,
                           @vl_Site_gestion,
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
               
                  insert into CFG..EQT_IMU (numero, adresse_end_point_OPCUA, user_OPCUA, password_OPCUA, nom_BICE, port_maitre, sous_type, nom_machine)
                  values ( @vl_Numero + 1, @va_AdresseEndPointOPCUA_in, @va_UserOPCUA_in, @va_PasswordOPCUA_in, @va_NomBICE_in, @va_PortMaitre_in, @va_SousType_in, @va_NomMachine_in)
                 
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
               ** L IMU existe comme equipement
               */
               begin
                  
                  /*A
                  ** Modification de l IMU dans la table EQT_GEN et EQT_IMU
                  */
               
                  update CFG..EQT_GEN
                  set maitre      = @vl_Serveur,
                      type_maitre = @vl_Type_maitre,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      sens        = @va_Sens_in,
		      site_gestion = @vl_Site_gestion,
		      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumIMU
                    and type   = XDC_EQT_IMU    
                
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
                        
                  update CFG..EQT_IMU
                  set adresse_end_point_OPCUA   = @va_AdresseEndPointOPCUA_in,
                      user_OPCUA		= @va_UserOPCUA_in,
                      password_OPCUA		= @va_PasswordOPCUA_in,
		      nom_BICE 			= @va_NomBICE_in,
		      port_maitre		= @va_PortMaitre_in,
		      sous_type			= @va_SousType_in,
		      nom_machine		= @va_NomMachine_in
                  where numero = @vl_NumIMU   
 		
 
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
            ** Verification de l'existence et recherche de l'autoroute et de l IMU comme equipement
            */
         
            select @vl_Autoroute = numero from CFT..RES_AUT
              where nom = @va_Autoroute_in
            
            select @vl_Serveur = numero from CFT..EQT_GEN
              where nom  = @va_NomServeur_in
                and type = XDC_EQT_MAC
            

            if @vl_Autoroute = null or @vl_Serveur = null
	    	return XDC_ARG_INV
	
	    if @vl_Autoroute != null and @vl_Serveur != null
            begin
               
               select @vl_NumIMU = numero from CFT..EQT_GEN
                 where type      = XDC_EQT_IMU
                   and nom       = @va_NomIMU_in 
                   
               /*A
               ** L IMU n'existe pas comme equipement
               */
            
               if @vl_NumIMU = null
               begin
                  
                  /*A
                  ** Insertion de l IMU dans la table EQT_GEN et EQT_IMU
                  */
               
                  select @vl_Numero = max(numero) from CFT..EQT_GEN
                    where type = XDC_EQT_IMU
                    
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
                  values ( XDC_EQT_IMU,
                           @vl_Numero + 1,
                           @vl_Serveur,
                           @vl_Type_maitre,
                           @vl_Autoroute,
                           @va_PR_in,
                           @va_Sens_in,
                           @va_NomIMU_in,
                           @vl_Site_gestion,
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
                        
                  insert into CFT..EQT_IMU (numero, adresse_end_point_OPCUA, user_OPCUA, password_OPCUA, nom_BICE, port_maitre, sous_type, nom_machine)
                  values ( @vl_Numero + 1, @va_AdresseEndPointOPCUA_in, @va_UserOPCUA_in, @va_PasswordOPCUA_in, @va_NomBICE_in, @va_PortMaitre_in, @va_SousType_in, @va_NomMachine_in)
                 
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
               ** L IMU existe comme equipement
               */
            
               begin
               
                  /*A
                  ** Modification de l IMU dans la table EQT_GEN et EQT_IMU
                  */
               
                  update CFT..EQT_GEN
                  set maitre      = @vl_Serveur,
                      type_maitre = @vl_Type_maitre,
                      autoroute   = @vl_Autoroute,
                      PR          = @va_PR_in,
                      sens        = @va_Sens_in,
		      site_gestion = @vl_Site_gestion,
		      eqt_supprime = @va_Supprime_in
                  where numero = @vl_NumIMU
                    and type   = XDC_EQT_IMU    
                
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
                        
                  update CFT..EQT_IMU
                  set adresse_end_point_OPCUA	= @va_AdresseEndPointOPCUA_in,
                      user_OPCUA		= @va_UserOPCUA_in,
                      password_OPCUA		= @va_PasswordOPCUA_in,
		      nom_BICE 			= @va_NomBICE_in,
		      port_maitre		= @va_PortMaitre_in,
		      sous_type			= @va_SousType_in,
		      nom_machine		= @va_NomMachine_in
                  where numero = @vl_NumIMU   
 		
 
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
