/*E*/
/* Fichier : $Id: xzao139sp.prc,v 1.14 2018/11/07 18:14:44 devgfi Exp $        $Revision: 1.14 $        $Date: 2018/11/07 18:14:44 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao139sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic		19 Sep 1994 : Creation
* volcic		25 Nov 1994 : Modif Update  1.2
* volcic		14 Jan 1995 : Modif Update  1.3
* hilmarcher    24/03/2004	: Modif type champ code_camera pour DEM 270  1.4
* cluchague     14/04/2004	: @va_code_camera char(5) au lieu de char(4)  v1.5
* JMG			13/11/07	: ajout site de gestion DEM715  1.6
* JPL			08/10/10	: Degroupee; Zones camera dans EQT_CAM_POS fait par XZAO193 (DEM 948)  1.7
* JPL			15/10/10	: Ajout du parametre d'adresse IP de la camera (DEM 954)  1.8
* JPL			14/12/10	: Ajout des parametres Protocole, Port et Encoder (DEM 954)  1.9
* LCL			11/07/18	: correction syntaxe from  1.11
* JPL			17/10/18	: Nommage des colonnes sur 'insert'  1.13
* JPL			17/10/18	: Ajout du parametre 'supprime' (DEM 1305)  1.14
* GGY			26/09/23	:	Ajout district CA (DEM483)
* GGY			17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une camera dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO139' and type = 'P')
	drop procedure XZAO139
go


create procedure XZAO139
   @va_Basedd_in        	char(3),
   @va_Nom_in			char(10),
   @va_Code_in			char(5),
   @va_NomLTVideo_in		char(10),
   @va_Autoroute_in		char(4),
   @va_PR_in			int,
   @va_Sens_in			tinyint,
   @va_Cycle_in			bit,
   @va_Mobile_in		bit,
   @va_Positionnable_in		bit,
   @va_AdresseIP_in		char(20),
   @va_Protocole_in		char(10),
   @va_Port_in			smallint,
   @va_Encoder_in		char(20),
   @va_Supprime_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO139SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_Nom_in
* XDY_CodeEqt		va_Code_in	
* XDY_NomEqt		va_NomLTVideo_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Booleen		va_Cycle_in
* XDY_Booleen		va_Mobile_in
* XDY_Booleen		va_Positionnable_in
* XDY_AdresseIP		va_AdresseIP_in
* XDY_Protocole		va_Protocole_in
* XDY_Port		va_Port_in
* XDY_Encoder		va_Encoder_in
* XDY_Booleen		va_Supprime_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute une camera video en base
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
* Insertion ou modification dans la table EQT_CAM (CAMERA)
*
* MECANISMES :
*
------------------------------------------------------*/

declare

   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_NumLTVideo	smallint,
   @vl_NumPosCam	smallint,
   @vl_NumCamera	smallint,
   @vl_Camera		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_ECHEC
            
   /*A
   ** Test des parametres d'entree :
   **   Si un parametre d'entree obligatoire est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null 
   or @va_Nom_in = null 
   or @va_Code_in = null 
   or @va_NomLTVideo_in = null 
   or @va_Autoroute_in = null  
   or @va_PR_in = null 
   or @va_Sens_in = null 
   or @va_Cycle_in = null 
   or @va_Mobile_in = null
   or @va_Positionnable_in = null 
   or @va_Supprime_in = null 
   
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
         
            /*A
            ** Verification de l'existence et recherche du systeme video comme equipement
            */
         
            select @vl_Autoroute = numero from CFG..RES_AUT
              where nom = @va_Autoroute_in
            
            /*A
            ** Existence de l'autoroute
            */
              
            if @vl_Autoroute <> null
            begin
               
               select @vl_NumLTVideo = numero from CFG..EQT_GEN
                 where  ( type = XDC_EQT_LTV or type = XDC_EQT_SRV )
                   and nom       = @va_NomLTVideo_in    
            
               /*A
               ** Existence du local technique Video
               */
              
               if @vl_NumLTVideo <> null
               begin
                  
                  select @vl_NumCamera = numero from CFG..EQT_GEN 
                    where type        = XDC_EQT_CAM
                      and nom         = @va_Nom_in
                
                  /*A
                  ** La camera n'existe pas comme equipement 
                  */
              
                  if @vl_NumCamera = null
                  begin
                    
                     /*A
                     ** Insertion de la camera dans la table EQT_GEN
                     */
               
                     select @vl_Numero = max(numero) from CFG..EQT_GEN
                       where type = XDC_EQT_CAM
                       
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
                     values ( XDC_EQT_CAM,
                              @vl_Numero + 1,
                              @vl_NumLTVideo,
                              XDC_MAI_CAM,
                              @vl_Autoroute,
                              @va_PR_in,
                              @va_Sens_in,
                              @va_Nom_in,
                              null,
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
                     ** Insertion de la camera dans la table EQT_CAM
                     */
               
                     insert CFG..EQT_CAM ( numero,
                                           code_camera,
                                           cycle_ou_camera,
                                           fixe_ou_mobile,
                                           prepositionnable,
                                           IP,
                                           protocole,
                                           port,
                                           encoder )
                     values ( @vl_Numero + 1,
                              @va_Code_in,
                              @va_Cycle_in,
                              @va_Mobile_in,
                              @va_Positionnable_in,
                              @va_AdresseIP_in,
                              @va_Protocole_in,
                              @va_Port_in,
                              @va_Encoder_in )
                  
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
                  ** La camera existe deja dans EQT_GEN
                  */
                  
                  begin
           
                     /*A
                     ** Modification de la camera dans la table EQT_GEN
                     */
                
                     update CFG..EQT_GEN
    	             set maitre  	= @vl_NumLTVideo,
    	                 autoroute   	= @vl_Autoroute,
    	                 PR 		= @va_PR_in,
    	                 sens		= @va_Sens_in,
    	                 eqt_supprime	= @va_Supprime_in
    	             where numero = @vl_NumCamera
    	               and type   = XDC_EQT_CAM
                       
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
                     ** Modification de la camera dans la table EQT_CAM
                     */
                
                     update CFG..EQT_CAM
    	             set code_camera = @va_Code_in,
			 cycle_ou_camera  = @va_Cycle_in,
    	                 fixe_ou_mobile   = @va_Mobile_in,
    	                 prepositionnable = @va_Positionnable_in,
    	                 IP               = @va_AdresseIP_in,
                         protocole        = @va_Protocole_in,
                         port             = @va_Port_in,
                         encoder          = @va_Encoder_in
    	             where numero = @vl_NumCamera
                       
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
              ** Le LT Video n'existe pas comme equipement
              **
              ** Affectation du parametre de sortie Resultat a XDC_ECHEC
              */
         
                 select @va_Resultat_out = XDC_ECHEC
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
            ** Verification de l'existence et recherche du systeme video comme equipement
            */
         
            select @vl_Autoroute = numero from CFT..RES_AUT
              where nom = @va_Autoroute_in
            
            /*A
            ** Existence de l'autoroute
            */
              
            if @vl_Autoroute <> null
            begin
               
               select @vl_NumLTVideo = numero from CFT..EQT_GEN
                 where  ( type = XDC_EQT_LTV or type = XDC_EQT_SRV )
                   and nom       = @va_NomLTVideo_in    
            
               /*A
               ** Existence du local technique Video
               */
              
               if @vl_NumLTVideo <> null
               begin
                  
                  select @vl_NumCamera = numero from CFT..EQT_GEN 
                    where type        = XDC_EQT_CAM
                      and nom         = @va_Nom_in
                
                  /*A
                  ** La camera n'existe pas comme equipement 
                  */
              
                  if @vl_NumCamera = null
                  begin
                    
                     /*A
                     ** Insertion de la camera dans la table EQT_GEN
                     */
               
                     select @vl_Numero = max(numero) from CFT..EQT_GEN
                       where type = XDC_EQT_CAM
                       
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
                     values ( XDC_EQT_CAM,
                              @vl_Numero + 1,
                              @vl_NumLTVideo,
                              XDC_MAI_CAM,
                              @vl_Autoroute,
                              @va_PR_in,
                              @va_Sens_in,
                              @va_Nom_in,
                              null,
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
                     ** Insertion de la camera dans la table EQT_CAM
                     */
               
                     insert CFT..EQT_CAM ( numero,
                                           code_camera,
                                           cycle_ou_camera,
                                           fixe_ou_mobile,
                                           prepositionnable,
                                           IP,
                                           protocole,
                                           port,
                                           encoder )
                     values ( @vl_Numero + 1,
                              @va_Code_in,
                              @va_Cycle_in,
                              @va_Mobile_in,
                              @va_Positionnable_in,
                              @va_AdresseIP_in,
                              @va_Protocole_in,
                              @va_Port_in,
                              @va_Encoder_in )
                  
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
                  ** La camera existe deja dans EQT_GEN
                  */
                  
                  begin
           
                     /*A
                     ** Modification de la camera dans la table EQT_GEN
                     */
                
                     update CFT..EQT_GEN
    	             set maitre  	= @vl_NumLTVideo,
    	                 autoroute   	= @vl_Autoroute,
    	                 PR 		= @va_PR_in,
    	                 sens		= @va_Sens_in,
    	                 eqt_supprime	= @va_Supprime_in
    	             where numero = @vl_NumCamera
    	               and type   = XDC_EQT_CAM
                       
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
                     ** Modification de la camera dans la table EQT_CAM
                     */
                
                     update CFT..EQT_CAM
    	             set code_camera = @va_Code_in,
			 cycle_ou_camera  = @va_Cycle_in,
    	                 fixe_ou_mobile   = @va_Mobile_in,
    	                 prepositionnable = @va_Positionnable_in,
    	                 IP               = @va_AdresseIP_in,
                         protocole        = @va_Protocole_in,
                         port             = @va_Port_in,
                         encoder          = @va_Encoder_in
    	             where numero = @vl_NumCamera
                       
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
              ** Le LT Video n'existe pas comme equipement
              **
              ** Affectation du parametre de sortie Resultat a XDC_ECHEC
              */
         
                 select @va_Resultat_out = XDC_ECHEC
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
