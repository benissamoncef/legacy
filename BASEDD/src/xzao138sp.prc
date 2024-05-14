/*E*/
/* Fichier : $Id: xzao138sp.prc,v 1.6 2018/10/19 15:04:01 devgfi Exp $        $Revision: 1.6 $        $Date: 2018/10/19 15:04:01 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao138sp.prc
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
* JMG   	13/11/07	: ajout site de gestion DEM715  1.3
* LCL		11/07/18	: correction syntaxe from  1.5
* JPL		18/10/18	: Nommage des colonnes sur 'insert'  1.6
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao138sp.h" */

/* static char *version = "@(#)xzao138.c	1.1 20/09/94 : xzao138" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un local technique video dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;138
   @va_Basedd_in        	char(3),
   @va_NomLTVideo_in		char(10),
   @va_NomSystemeVideo_in	char(10),
   @va_Autoroute_in		char(4),
   @va_PR_in			int,
   @va_Sens_in			tinyint,
   @va_LiaisonsFixes_in		tinyint,
   @va_LiaisonsCycles_in	tinyint,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO138SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_NomLTVideo_in
* XDY_NomEqt		va_NomSystemeVideo_in
* XDY_Autoroute		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Octet		va_LiaisonsFixes_in
* XDY_Octet		va_LiaisonsCycles_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un local technique video en base
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
* Insertion ou modification dans la table EQT_LTV (LT_VIDEO)
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
   @vl_NumSysVideo	smallint,
   @vl_Numero		smallint,
   @vl_Site 		char(10)
   
   select @va_Resultat_out = XDC_LU
            
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NomLTVideo_in = null 
   or @va_NomSystemeVideo_in = null 
   or @va_Autoroute_in = null  
   or @va_PR_in = null 
   or @va_Sens_in = null 
   or @va_LiaisonsFixes_in = null 
   or @va_LiaisonsCycles_in = null
   
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
              
            if @vl_Autoroute <> null
            begin
               
               select @vl_NumSysVideo = numero from CFG..EQT_GEN
                 where type = XDC_EQT_SYV
                   and nom  = @va_NomSystemeVideo_in    
            
            
               /*A
               ** Le systeme video existe comme equipement
               */
               
               if @vl_NumSysVideo <> null
               begin
                  
                  select @vl_NumLTVideo = numero from CFG..EQT_GEN 
                    where type        = XDC_EQT_LTV
                      and nom         = @va_NomLTVideo_in
                
                  /*A
                  ** Le LTvideo n'existe pas comme equipement
                  */
               
                  if @vl_NumLTVideo = null
                  begin
         
                     /*A
                     ** Insertion du LTVideo dans la table EQT_GEN
                     */
               
                     select @vl_Numero = max(numero) from  CFG..EQT_GEN
                       where type = XDC_EQT_LTV
                       
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
                                   site_gestion
                                 )
                     values ( XDC_EQT_LTV, @vl_Numero + 1, @vl_NumSysVideo, XDC_MAI_LTV, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomLTVideo_in , null)
                    
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
                     ** Insertion du LTVideo dans la table EQT_LTV
                     */
               
                     insert into CFG..EQT_LTV
                     values ( @vl_Numero + 1, @va_LiaisonsFixes_in, @va_LiaisonsCycles_in )
                  
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
                  ** Le LTvideo existe comme equipement
                  */
               
                  begin
           
                     /*A
                     ** Modification du LTVideo dans la table EQT_GEN
                     */
                
                     update CFG..EQT_GEN
    	             set maitre = @vl_NumSysVideo,
    	                 type_maitre = XDC_MAI_LTV,
    	                 autoroute   = @vl_Autoroute,
    	                 PR          = @va_PR_in,
    	                 sens        = @va_Sens_in
    	             where numero = @vl_NumLTVideo
    	               and type   = XDC_EQT_LTV
    	               and nom    = @va_NomLTVideo_in
                       
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
                     ** Modification du LTVideo dans la table EQT_LTV
                     */
                
                     update CFG..EQT_LTV
    	             set nombre_liaisons_fixes  = @va_LiaisonsFixes_in,
    	                 nombre_liaisons_cycles = @va_LiaisonsCycles_in
    	             where numero = @vl_NumLTVideo
                       
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
               ** Le Systeme Video n'existe pas
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
              
            if @vl_Autoroute <> null
            begin
               
               select @vl_NumSysVideo = numero from CFT..EQT_GEN
                 where type = XDC_EQT_SYV
                   and nom  = @va_NomSystemeVideo_in    
            
            
               /*A
               ** Le systeme video existe comme equipement
               */
               
               if @vl_NumSysVideo <> null
               begin
                  
                  select @vl_NumLTVideo = numero from CFT..EQT_GEN 
                    where type        = XDC_EQT_LTV
                      and nom         = @va_NomLTVideo_in
                
                  /*A
                  ** Le LTvideo n'existe pas comme equipement
                  */
               
                  if @vl_NumLTVideo = null
                  begin
         
                     /*A
                     ** Insertion du LTVideo dans la table EQT_GEN
                     */
               
                     select @vl_Numero = max(numero) from  CFT..EQT_GEN
                       where type = XDC_EQT_LTV
                       
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
                                   site_gestion
                                 )
                     values ( XDC_EQT_LTV, @vl_Numero + 1, @vl_NumSysVideo, XDC_MAI_LTV, @vl_Autoroute, @va_PR_in, @va_Sens_in, @va_NomLTVideo_in , null)
                    
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
                     ** Insertion du LTVideo dans la table EQT_LTV
                     */
               
                     insert into CFT..EQT_LTV
                     values ( @vl_Numero + 1, @va_LiaisonsFixes_in, @va_LiaisonsCycles_in )
                  
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
                  ** Le LTvideo existe comme equipement
                  */
               
                  begin
           
                     /*A
                     ** Modification du LTVideo dans la table EQT_GEN
                     */
                
                     update CFT..EQT_GEN
    	             set maitre = @vl_NumSysVideo,
    	                 type_maitre = XDC_MAI_LTV,
    	                 autoroute   = @vl_Autoroute,
    	                 PR          = @va_PR_in,
    	                 sens        = @va_Sens_in
    	             where numero = @vl_NumLTVideo
    	               and type   = XDC_EQT_LTV
    	               and nom    = @va_NomLTVideo_in
                       
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
                     ** Modification du LTVideo dans la table EQT_LTV
                     */
                
                     update CFT..EQT_LTV
    	             set nombre_liaisons_fixes  = @va_LiaisonsFixes_in,
    	                 nombre_liaisons_cycles = @va_LiaisonsCycles_in
    	             where numero = @vl_NumLTVideo
                       
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
               ** Le Systeme Video n'existe pas
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
