/*E*/
/* Fichier : $Id: xzao137sp.prc,v 1.7 2018/10/19 15:03:53 devgfi Exp $        $Revision: 1.7 $        $Date: 2018/10/19 15:03:53 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao137sp.prc
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
* volcic	09 Nov 1994	: Ajout #endif (v1.3)
* JMG   	13/11/07	: ajout site de gestion DEM715  1.4
* LCL		11/07/18	: correction syntaxe from  1.6
* JPL		18/10/18	: Nommage des colonnes sur 'insert'  1.7
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao137sp.h" */

/* static char *version = "@(#)xzao137.c	1.1 20/09/94 : xzao137" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un systeme video dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;137
   @va_Basedd_in        	char(3),
   @va_NomVideo_in		char(10),
   @va_NomServeur_in		char(10),
   @va_Liaison_in		tinyint,
   @va_RGS_in			char(3),
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO137SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_NomEqt		va_Nom_in
* XDY_NomEqt		va_NomServeur_in
* XDY_Octet		va_Liaison_in
* XDY_Rgs		va_RGS_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un systeme video en base
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
* Insertion ou modification dans la table EQT_SYV (SYSTEME_VIDEO)
*
* MECANISMES :
*
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Serveur		smallint,
   @vl_NumSysVideo	smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
            
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null
   or @va_NomVideo_in = null 
   or @va_NomServeur_in = null 
   or @va_Liaison_in = null
   or @va_RGS_in = null
   
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
         
            select @vl_Serveur = numero from CFG..EQT_GEN 
              where nom  = @va_NomServeur_in
                and type = XDC_EQT_SRV
                
            /*A
            ** Le serveur existe comme equipement
            */
           
            if @vl_Serveur <> null
            begin
         
               /*A 
               ** Verification de l'existence et recherche du Systeme Video dans EQT_SYV
               */
         
               select @vl_NumSysVideo = numero from CFG..EQT_GEN 
                 where nom  = @va_NomVideo_in
                   and type = XDC_EQT_SYV
               
               /*A
               ** Le SysVideo n'existe pas dans EQT_GEN
               */
            
               if @vl_NumSysVideo = null
               begin
              
                  /*A
                  ** Insertion du SysVideo dans la table EQT_GEN
                  */
                  
                  select @vl_Numero = max(numero) from CFG..EQT_GEN 
                    where type = XDC_EQT_SYV
                    
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
                  values ( XDC_EQT_SYV , @vl_Numero + 1, @vl_Serveur, XDC_MAI_SYV, null, null, null, @va_NomVideo_in , null)
                  
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
                  ** Insertion du SysVideo dans la table EQT_SYV
                  */
                  
                  insert into CFG..EQT_SYV
                  values ( @vl_Numero + 1, @va_RGS_in, @va_Liaison_in )
                  
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
               ** Le SysVideo existe deja
               */      
            
               begin
           
                  /*A
                  ** Modification de le SysVideo dans la table EQT_GEN
                  */
                
                  update CFG..EQT_GEN
    	          set maitre      = @vl_Serveur,
    	              type_maitre = XDC_MAI_SYV
    	          where numero = @vl_NumSysVideo
    	            and type   = XDC_EQT_SYV
                       
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
                  ** Modification de le SysVideo dans la table EQT_SYV
                  */
                
                  update CFG..EQT_SYV
    	          set rgs           = @va_RGS_in,
    	              liaison_serie = @va_Liaison_in
    	          where numero = @vl_NumSysVideo
                       
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
      
         else
         
#ifdef CI         
         
         /*A
         **  Travail dans la base CFT
         */   
      
         begin
         
            /*A
            ** Verification de l'existence et recherche du systeme video comme equipement
            */
         
            select @vl_Serveur = numero from CFT..EQT_GEN 
              where nom  = @va_NomServeur_in
                and type = XDC_EQT_SRV
                
            /*A
            ** Le serveur existe comme equipement
            */
           
            if @vl_Serveur <> null
            begin
         
               /*A 
               ** Verification de l'existence et recherche du Systeme Video dans EQT_SYV
               */
         
               select @vl_NumSysVideo = numero from CFT..EQT_GEN 
                 where nom  = @va_NomVideo_in
                   and type = XDC_EQT_SYV
               
               /*A
               ** Le SysVideo n'existe pas dans EQT_GEN
               */
            
               if @vl_NumSysVideo = null
               begin
              
                  /*A
                  ** Insertion du SysVideo dans la table EQT_GEN
                  */
                  
                  select @vl_Numero = max(numero) from CFT..EQT_GEN 
                    where type = XDC_EQT_SYV
                    
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
                  values ( XDC_EQT_SYV , @vl_Numero + 1, @vl_Serveur, XDC_MAI_SYV, null, null, null, @va_NomVideo_in , null)
                  
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
                  ** Insertion du SysVideo dans la table EQT_SYV
                  */
                  
                  insert into CFT..EQT_SYV
                  values ( @vl_Numero + 1, @va_RGS_in, @va_Liaison_in )
                  
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
               ** Le SysVideo existe deja
               */      
            
               begin
           
                  /*A
                  ** Modification de le SysVideo dans la table EQT_GEN
                  */
                
                  update CFT..EQT_GEN
    	          set maitre      = @vl_Serveur,
    	              type_maitre = XDC_MAI_SYV
    	          where numero = @vl_NumSysVideo
    	            and type   = XDC_EQT_SYV
                       
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
                  ** Modification de le SysVideo dans la table EQT_SYV
                  */
                
                  update CFT..EQT_SYV
    	          set rgs           = @va_RGS_in,
    	              liaison_serie = @va_Liaison_in
    	          where numero = @vl_NumSysVideo
                       
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
