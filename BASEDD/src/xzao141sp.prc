/*E*/
/* Fichier : $Id: xzao141sp.prc,v 1.6 2018/10/19 15:06:06 devgfi Exp $        $Revision: 1.6 $        $Date: 2018/10/19 15:06:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao141sp.prc
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

/* #include "xzao141sp.h" */

/* static char *version = "@(#)xzao141.c	1.1 20/09/94 : xzao141" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un magnetoscope dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;141
   @va_Basedd_in        	char(3),
   @va_Nom_in			char(10),
   @va_Code_in			char(3),
   @va_NomSysVideo_in		char(10),
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO141SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_Nom_in
* XDY_CodeEqt		va_Code_in	
* XDY_NomEqt		va_NomSysVideo_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un magnetoscope en base
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
* Insertion ou modification dans la table EQT_MAG (MONITEUR)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumSysVideo	smallint,
   @vl_NumMagneto	smallint,
   @vl_NumMag		smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
            
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_Nom_in = null 
   or @va_Code_in = null 
   or @va_NomSysVideo_in = null
   
      return XDC_ARG_INV
   
   else   
   begin
      
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
            ** Verification de l'existence et recherche du systeme video comme equipement
            */
         
            select @vl_NumSysVideo = numero from CFG..EQT_GEN
              where type = XDC_EQT_SYV
              	and nom  = @va_NomSysVideo_in 
              	    
            /*A
            ** Existence du systeme video et de la machine operateur
            */
              
            if @vl_NumSysVideo <> null
            begin
                  
               select @vl_NumMagneto = numero from CFG..EQT_GEN 
                 where type = XDC_EQT_MAG
              	   and nom  = @va_Nom_in
              	      
               /*A
               ** Le magnetoscope n'existe pas comme equipement 
               */
              
               if @vl_NumMagneto = null
               begin
                    
                  /*A
                  ** Insertion du magnetoscope dans la table EQT_GEN
                  */
               
                  select @vl_Numero = max(numero) from CFG..EQT_GEN
                    where type = XDC_EQT_MAG
              
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
                  values ( XDC_EQT_MAG, @vl_Numero + 1, @vl_NumSysVideo, XDC_MAI_MAG, null, null, null, @va_Nom_in , null)
                       
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
                  ** Insertion du magnetoscope dans la table EQT_MAG
                  */
               
                  insert into CFG..EQT_MAG
                  values ( @vl_Numero + 1, @va_Code_in )
                       
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
               ** Le magnetoscope existe deja dans EQT_MAG
               */
                  
               begin
           
                  /*A
                  ** Modification du magnetoscope dans la table EQT_GEN
                  */
                
                  update CFG..EQT_GEN
    	          set maitre      = @vl_NumSysVideo,
    	              type_maitre = XDC_MAI_MAG
    	          where numero = @vl_NumMagneto
    	            and type   = XDC_EQT_MAG
    	            and nom    = @va_Nom_in
                       
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
                  ** Modification du magnetoscope dans la table EQT_MAG
                  */
                
                  update CFG..EQT_MAG
    	          set code_magnetoscope = @va_Code_in
    	          where numero = @vl_NumMagneto
                       
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
            ** Le systeme video n'existe pas comme equipement
            **
            ** Affectation du parametre de sortie Resultat a XDC_ECHEC
            */
         
               select @va_Resultat_out = XDC_ECHEC
         end
         
#ifdef CI
	 
	 else 
	 
	 /*A
         **  Travail dans la base CFG
         */   
      
         begin
         
            /*A
            ** Verification de l'existence et recherche du systeme video comme equipement
            */
         
            select @vl_NumSysVideo = numero from CFT..EQT_GEN
              where type = XDC_EQT_SYV
              	and nom  = @va_NomSysVideo_in 
              	    
            /*A
            ** Existence du systeme video et de la machine operateur
            */
              
            if @vl_NumSysVideo <> null
            begin
                  
               select @vl_NumMagneto = numero from CFT..EQT_GEN 
                 where type = XDC_EQT_MAG
              	   and nom  = @va_Nom_in
              	      
               /*A
               ** Le magnetoscope n'existe pas comme equipement 
               */
              
               if @vl_NumMagneto = null
               begin
                    
                  /*A
                  ** Insertion du magnetoscope dans la table EQT_GEN
                  */
               
                  select @vl_Numero = max(numero) from CFT..EQT_GEN
                    where type = XDC_EQT_MAG
              
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
                  values ( XDC_EQT_MAG, @vl_Numero + 1, @vl_NumSysVideo, XDC_MAI_MAG, null, null, null, @va_Nom_in , null)
                       
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
                  ** Insertion du magnetoscope dans la table EQT_MAG
                  */
               
                  insert into CFT..EQT_MAG
                  values ( @vl_Numero + 1, @va_Code_in )
                       
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
               ** Le magnetoscope existe deja dans EQT_MAG
               */
                  
               begin
           
                  /*A
                  ** Modification du magnetoscope dans la table EQT_GEN
                  */
                
                  update CFT..EQT_GEN
    	          set maitre      = @vl_NumSysVideo,
    	              type_maitre = XDC_MAI_MAG
    	          where numero = @vl_NumMagneto
    	            and type   = XDC_EQT_MAG
    	            and nom    = @va_Nom_in
                       
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
                  ** Modification du magnetoscope dans la table EQT_MAG
                  */
                
                  update CFT..EQT_MAG
    	          set code_magnetoscope = @va_Code_in
    	          where numero = @vl_NumMagneto
                       
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
            ** Le systeme video n'existe pas comme equipement
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
