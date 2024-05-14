/*E*/
/*Fichier : $Id: xzao151sp.prc,v 1.6 2018/07/11 09:59:50 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2018/07/11 09:59:50 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao151sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* PN	20 Nov 1996	: Suppression du controle sur la portion 1.4
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao151sp.h" */

/* static char *version = "@(#)xzao151.c	1.1 20/09/94 : xzao151" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un Portail dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;151
   @va_Basedd_in        	char(3),
   @va_NomPortion_in		char(25),
   @va_PR_in			int,
   @va_Sens_in			tinyint,
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO151SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Nom		va_NomPortion_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* XDY_Booleen		va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un Portail en base
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
* Insertion ou modification dans la table RES_PTL (RES_PORTAIL)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumPortion	smallint,
   @vl_NumPortail	smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NomPortion_in = null
   or @va_PR_in = null 
   or @va_Sens_in = null 
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
            ** Verification de le existence et recherche de la portion
            */
         
            select @vl_NumPortion = numero from CFG..RES_POR
              where nom       = @va_NomPortion_in
                and PR_debut <  @va_PR_in
                and PR_fin   >= @va_PR_in
              
            /*A
            ** Existence de la portion
            */
            
            if @vl_NumPortion <> null
            begin
               
               select @vl_NumPortail = portion from CFG..RES_PTL
                 where PR       = @va_PR_in
                   and sens	= @va_Sens_in
                   
               /*A
               ** Le Portail n'existe pas dans RES_PTL
               */
               
               if @vl_NumPortail = null
               begin
                  
                  /*A
                  ** Creation positionne =>
                  ** Insertion du Portail dans la table RES_PTL
                  */
               
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     insert into CFG..RES_PTL
                     values ( @vl_NumPortion, @va_PR_in, @va_Sens_in )
                  
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
               ** Le portail existe deja dans RES_PTL
               */
               
               begin
               
                  /*A
                  ** Creation non positionne =>
                  ** Suppression du Portail dans la table RES_PTL
                  */
                
                  if @va_Creation_in = XDC_FAUX
                  begin
                  
                     delete CFG..RES_PTL
    	             where portion = @vl_NumPortion
    	               and PR      = @va_PR_in
    	               and sens    = @va_Sens_in
                       
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
                  begin
                  update CFG..RES_PTL
                  set portion=@vl_NumPortion
                 where PR       = @va_PR_in
                   and sens	= @va_Sens_in
                  end
                  
               end 
            end
             
            else
         
            /*A
            ** La portion n'existe pas
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
            ** Verification de le existence et recherche de la portion
            */
         
            select @vl_NumPortion = numero from CFT..RES_POR
              where nom       = @va_NomPortion_in
                and PR_debut <  @va_PR_in
                and PR_fin   >= @va_PR_in
              
            /*A
            ** Existence de la portion
            */
            
            if @vl_NumPortion <> null
            begin
               
               select @vl_NumPortail = portion from CFT..RES_PTL
                 where PR      	= @va_PR_in
                   and sens	= @va_Sens_in
                   
               /*A
               ** Le Portail n'existe pas dans RES_PTL
               */
               
               if @vl_NumPortail = null
               begin
                  
                  /*A
                  ** Creation positionne =>
                  ** Insertion du Portail dans la table RES_PTL
                  */
               
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     insert into CFT..RES_PTL
                     values ( @vl_NumPortion, @va_PR_in, @va_Sens_in )
                  
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
               ** Le portail existe deja dans RES_PTL
               */
               
               begin
               
                  /*A
                  ** Creation non positionne =>
                  ** Suppression du Portail dans la table RES_PTL
                  */
                
                  if @va_Creation_in = XDC_FAUX
                  begin
                  
                     delete CFT..RES_PTL
    	             where portion = @vl_NumPortion
    	               and PR      = @va_PR_in
    	               and sens    = @va_Sens_in
                       
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
                  begin
                  update CFT..RES_PTL
                  set portion=@vl_NumPortion
                 where PR       = @va_PR_in
                   and sens	= @va_Sens_in
                  end
                   
               end 
            end
             
            else
         
            /*A
            ** La portion n'existe pas
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
