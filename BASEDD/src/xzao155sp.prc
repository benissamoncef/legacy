/*E*/
/*Fichier : $Id: xzao155sp.prc,v 1.5 2018/07/11 10:00:42 pc2dpdy Exp $      Release : $Revision: 1.5 $        Date : $Date: 2018/07/11 10:00:42 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao155sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao155sp.h" */

/* static char *version = "@(#)xzao155.c	1.1 20/09/94 : xzao155" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un Scenario dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;155
   @va_Basedd_in        	char(3),
   @va_Scenario_in		char(4),
   @va_Nom_in			char(25),
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO155SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* char[4]		va_Scenario_in
* XDY_Nom		va_Nom_in
* XDY_Booleen		va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un Scenario en base
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
* Insertion ou modification dans la table EQT_SCN_NAV (SCENARIO NAV)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	20 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumScenario	tinyint,
   @vl_Numero		tinyint,
   @vl_Site			char(10)
   
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_Scenario_in = null 
   or @va_Nom_in = null 
   or @va_Creation_in = null 
   
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
            ** Verification de l'existence du scenario
            */
         
            select @vl_NumScenario = numero from CFG..EQT_SCN_NAV
              where scenario = @va_Scenario_in
                and nom      = @va_Nom_in
                
            /*A
            ** Le Scenario n'existe pas dans EQT_SCN_NAV
            */
               
            if @vl_NumScenario = null
            begin
                  
               /*A
               ** Creation positionne =>
               ** Insertion du Scenario dans la table EQT_SCN_NAV
               */
               
               if @va_Creation_in = XDC_VRAI
               begin
                     
                  select @vl_Numero = max(numero) from CFG..EQT_SCN_NAV
                     
                  if @vl_Numero = null
                     select @vl_Numero = 0
                  
                  insert into CFG..EQT_SCN_NAV
                  values ( @vl_Numero + 1, @va_Scenario_in, @va_Nom_in, XDC_FAUX )
                  
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
                  
            end
               
            else
               
            /*A
            ** Le scenario existe deja dans EQT_SCN_NAV
            */
               
            begin
               
               /*A
               ** Creation non positionne =>
               ** Suppression du Scenario dans la table EQT_SCN_NAV
               */
                
               if @va_Creation_in = XDC_FAUX
               begin
                  
                  update CFG..EQT_SCN_NAV
    	          set supprime = XDC_VRAI
    	          where scenario = @va_Scenario_in
    	            and numero   = @vl_NumScenario
                       
                  /*A
                  ** Erreur de suppression
                  */
                     
                  if @@rowcount <> 1
                  begin
                    
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                     */
         
                     select @va_Resultat_out = XDC_SUPPR
                        
                     return XDC_NOK
                  end
                        
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                  */
         
                  select @va_Resultat_out = XDC_SUPPR
                          
               end
                  
               else
           
               /*A
               ** Creation positionne =>
               ** Suppression du Scenario dans la table EQT_SCN_NAV
               */
         
               begin
                  
                  update CFG..EQT_SCN_NAV
    	          set nom = @va_Nom_in,
    	              supprime = XDC_FAUX
    	          where scenario = @va_Scenario_in
    	            and numero   = @vl_NumScenario
                 
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
      
#ifdef CI

         else
         
         /*A
         **  Travail dans la base CFT
         */   
      
         begin
         
            /*A
            ** Verification de l'existence du scenario
            */
         
            select @vl_NumScenario = numero from CFT..EQT_SCN_NAV
              where scenario = @va_Scenario_in
                and nom      = @va_Nom_in
                
            /*A
            ** Le Scenario n'existe pas dans EQT_SCN_NAV
            */
               
            if @vl_NumScenario = null
            begin
                  
               /*A
               ** Creation positionne =>
               ** Insertion du Scenario dans la table EQT_SCN_NAV
               */
               
               if @va_Creation_in = XDC_VRAI
               begin
                     
                  select @vl_Numero = max(numero) from CFT..EQT_SCN_NAV
                     
                  if @vl_Numero = null
                     select @vl_Numero = 0
                  
                  insert into CFT..EQT_SCN_NAV
                  values ( @vl_Numero + 1, @va_Scenario_in, @va_Nom_in, XDC_FAUX )
                  
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
                  
            end
               
            else
               
            /*A
            ** Le scenario existe deja dans EQT_SCN_NAV
            */
               
            begin
               
               /*A
               ** Creation non positionne =>
               ** Suppression du Scenario dans la table EQT_SCN_NAV
               */
                
               if @va_Creation_in = XDC_FAUX
               begin
                  
                  update CFT..EQT_SCN_NAV
    	          set supprime = XDC_VRAI
    	          where scenario = @va_Scenario_in
    	            and numero   = @vl_NumScenario
                       
                  /*A
                  ** Erreur de suppression
                  */
                     
                  if @@rowcount <> 1
                  begin
                    
                     /*A
                     ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                     */
         
                     select @va_Resultat_out = XDC_SUPPR
                        
                     return XDC_NOK
                  end
                        
                  /*A
                  ** Affectation du parametre de sortie Resultat a XDC_SUPPR
                  */
         
                  select @va_Resultat_out = XDC_SUPPR
                          
               end
                  
               else
           
               /*A
               ** Creation positionne =>
               ** Suppression du Scenario dans la table EQT_SCN_NAV
               */
         
               begin
                  
                  update CFT..EQT_SCN_NAV
    	          set nom = @va_Nom_in,
    	              supprime = XDC_FAUX
    	          where scenario = @va_Scenario_in
    	            and numero   = @vl_NumScenario
                 
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


#endif         
         
         
      
      end
   end         
     
   return XDC_OK
         
go  
