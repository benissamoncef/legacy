/*E*/
/*Fichier : $Id: xzao92sp.prc,v 1.8 2018/07/11 09:46:45 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2018/07/11 09:46:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao92sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
* PN	20 Nov 1996	: Suppression du controle sur la portion 1.5
* LCL	05/07/17	: modif PRA
* LCL           11/07/18        : correction syntaxe from
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao92sp.h" */

/* static char *version = "@(#)xzao92.c	1.1 23/18/94 : xzao92" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un Reseau dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;92
   @va_Basedd_in        	char(3),
   @va_NomReseau_in		char(25),
   @va_TypeReseau_in		tinyint,
   @va_Autoroute_in		char(4),
   @va_PR_in			int,
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO92SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Nom		va_NomReseau_in
* XDY_Octet		va_TypeReseau_in
* XDY_Nom		va_Autoroute_in
* XDY_PR		va_PR_in
* XDY_Booleen		va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un Reseau en base
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
* Insertion ou modification dans la table RES_TRN (RESEAU TRANSVERSAL)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	20 Oct 1994	: Suppr XDC_ECHEC et Ajout #ifdef CI (v1.2)
* volcic	08 Nov 1994	: Modif Ajout Base CFG (v1.3)
* volcic	17 Mai 1995	: Modif Ajout numero Portion (v1.4)
* C.T.		18 Fev 1998	: Modif test sur update suite SQL server 11.0.2.3 (1.6)
* GGY		26/09/23 	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_Autoroute	tinyint,
   @vl_NumReseau	smallint,
   @vl_Numero		smallint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_LU
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NomReseau_in = null 
   or @va_TypeReseau_in = null 
   or @va_Autoroute_in = null 
   or @va_PR_in = null 
   
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
            ** Verification de leexistence et recherche du numero de leautoroute
            */
         
            select @vl_Autoroute = POR.numero from CFG..RES_AUT AUT, CFG..RES_POR POR 
              where AUT.nom = @va_Autoroute_in
                and POR.autoroute = AUT.numero
		and POR.PR_debut <= @va_PR_in
		and POR.PR_fin   >= @va_PR_in
                
          
            if @vl_Autoroute <> null
            begin
         
               /*A 
               ** Insertion ou Modification de le Reseau
               **
               ** Verification de leexistence et recherche de le Reseau dans RES_TRN
               */
         
               select @vl_NumReseau = numero from CFG..RES_TRN
                 where nom     = @va_NomReseau_in
               
               /*A
               ** Le Reseau n'existe pas dans RES_TRN
               */
            
               if @vl_NumReseau = null
               begin
              
                  /*A
                  ** Creation positionne =>
                  ** Insertion de leReseau dans la table RES_TRN
                  */
                  
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     select @vl_Numero = max(numero) from CFG..RES_TRN
               
                     if @vl_Numero = null
                        select @vl_Numero = 0
                        
                     insert into CFG..RES_TRN
                     values ( @vl_Numero + 1, @va_NomReseau_in, @vl_Autoroute, @va_PR_in, @va_TypeReseau_in, NULL )
                  
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
               ** Le Reseau existe deja
               */      
            
               begin
           
                  /*A
                  ** Creation positionne =>
                  ** Modification de le Reseau dans la table RES_TRN
                  */
                
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     update CFG..RES_TRN
    	             set nom  = @va_NomReseau_in,
    	                 PR   = @va_PR_in,
    	                 type = @va_TypeReseau_in,
                        portion  = @vl_Autoroute  
    	             where numero   = @vl_NumReseau
    
                     /*A
                     ** Erreur de modification
                     */
                     
                     if @@rowcount > 1
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
                  
                  else
                  
                  /*A
                  ** Creation non positionne =>
                  ** Marquage de suppression de le Reseau dans la table RES_TRN
                  */
                  
                  begin
                  
                     delete CFG..RES_TRN   
    	             where numero   = @vl_NumReseau
                       and portion  = @vl_Autoroute
                  
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
      
         if @va_Basedd_in = XDC_BASE_CFT
         begin
         
            /*A
            ** Verification de l'existence et recherche du numero de leautoroute
            */
         
            select @vl_Autoroute = POR.numero from CFT..RES_AUT AUT, CFT..RES_POR POR 
              where AUT.nom = @va_Autoroute_in
                and POR.autoroute = AUT.numero
		and POR.PR_debut <= @va_PR_in
		and POR.PR_fin   >= @va_PR_in
                
          
            if @vl_Autoroute <> null
            begin
         
               /*A 
               ** Insertion ou Modification de le Reseau
               **
               ** Verification de leexistence et recherche de le Reseau dans RES_TRN
               */
         
               select @vl_NumReseau = numero from CFT..RES_TRN
                 where nom     = @va_NomReseau_in
               
               /*A
               ** Le Reseau n'existe pas dans RES_TRN
               */
            
               if @vl_NumReseau = null
               begin
              
                  /*A
                  ** Creation positionne =>
                  ** Insertion de leReseau dans la table RES_TRN
                  */
                  
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     select @vl_Numero = max(numero) from CFT..RES_TRN
               
                     if @vl_Numero = null
                        select @vl_Numero = 0
                        
                     insert into CFT..RES_TRN
                     values ( @vl_Numero + 1, @va_NomReseau_in, @vl_Autoroute, @va_PR_in, @va_TypeReseau_in, NULL )
                  
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
               ** Le Reseau existe deja
               */      
            
               begin
           
                  /*A
                  ** Creation positionne =>
                  ** Modification de le Reseau dans la table RES_TRN
                  */
                
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     update CFT..RES_TRN
    	             set nom  = @va_NomReseau_in,
    	                 PR   = @va_PR_in,
    	                 type = @va_TypeReseau_in,
                        portion  = @vl_Autoroute 
    	             where numero   = @vl_NumReseau
    
                     /*A
                     ** Erreur de modification
                     */
                     
                     if @@rowcount > 1
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
                  
                  else
                  
                  /*A
                  ** Creation non positionne =>
                  ** Marquage de suppression de le Reseau dans la table RES_TRN
                  */
                  
                  begin
                  
                     delete CFT..RES_TRN   
    	             where numero   = @vl_NumReseau
                       and portion  = @vl_Autoroute
                  
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
         
      end
   end         
     
   return XDC_OK
         
go  
