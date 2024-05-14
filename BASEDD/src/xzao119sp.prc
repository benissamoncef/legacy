/*E*/
/*Fichier : $Id: xzao119sp.prc,v 1.6 2018/07/11 09:50:04 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2018/07/11 09:50:04 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao119sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao119sp.h" */

/* static char *version = "@(#)xzao119.c	1.1 20/09/94 : xzao119" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une Troncon dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;119
   @va_Basedd_in        	char(3),
   @va_NomTroncon_in		char(25),
   @va_NomAutoroute_in		char(4),
   @va_PRdebut_in		int,
   @va_PRfin_in			int,
   @va_Creation_in		bit,
   @va_Resultat_out		int output
   as 

/* 
*
* SP	XZAO119SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd		va_Basedd_in
* XDY_Nom		va_NomTroncon_in
* XDY_Nom		va_NomAutoroute_in
* XDY_PR		va_PRdebut_in
* XDY_PR		va_PRfin_in
* XDY_Booleen		va_Creation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf	   va_Resultat_out	
*
* VALEUR RENDUE :
*
* Ajoute un Troncon en base
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
* Insertion ou modification dans la table RES_TRC (VUE_TRONCON)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Sep 1994	: Creation
* volcic	30 Dec 1994	: Modif pour tcgcd dans CFT (v1.3)
* C.T.		18 Fev 1998	: Modif test rowcount sur update suite SQL 11.0.3.2 (1.4)
* LCL		11/07/18	: correction syntaxe from
* GGY		26/09/23	: Ajout district CA (DEM483)
* GGY		17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_NumAutoroute	tinyint,
   @vl_NumTroncon	tinyint,
   @vl_Site			char(10)
   
   select @va_Resultat_out = XDC_ECHEC
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Basedd_in = null  
   or @va_NomTroncon_in = null 
   or  @va_NomAutoroute_in = null 
   or @va_PRdebut_in = null 
   or @va_PRfin_in = null 
   
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
         
            select @vl_NumAutoroute = numero from CFG..RES_AUT 
              where nom = @va_NomAutoroute_in
                
          
            if @vl_NumAutoroute <> null
            begin
         
               /*A 
               ** Insertion ou Modification du Troncon
               **
               ** Verification de leexistence et recherche du Troncon dans RES_TRC
               */
         
               select @vl_NumTroncon = autoroute from CFG..RES_TRC
                 where nom       = @va_NomTroncon_in
                   and autoroute = @vl_NumAutoroute
               
               /*A
               ** Le Troncon n'existe pas dans RES_TRC
               */
            
               if @vl_NumTroncon = null
               begin
              
                  /*A
                  ** Creation positionne =>
                  ** Insertion du Troncon dans la table RES_TRC
                  */
                  
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     insert into CFG..RES_TRC
                     values ( @va_NomTroncon_in, @vl_NumAutoroute, @va_PRdebut_in, @va_PRfin_in )
                  
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
                  ** Creation non positionne =>
                  ** Affectation du parametre de sortie Resultat a XDC_ECHEC
                  */
         
                     select @va_Resultat_out = XDC_ECHEC
               end
               
               else
         
               /*A
               ** Le Troncon existe deja
               */      
            
               begin
           
                  /*A
                  ** Creation positionne =>
                  ** Modification de le Troncon dans la table RES_TRC
                  */
                
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     update CFG..RES_TRC
    	             set PR_debut = @va_PRdebut_in,
    	                 PR_fin   = @va_PRfin_in
    	             where autoroute = @vl_NumAutoroute  
                       and nom       = @va_NomTroncon_in
                     
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
                  ** Marquage de suppression de le Troncon dans la table RES_TRC
                  */
                  
                  begin
                  
                     delete CFG..RES_TRC   
    	             where autoroute = @vl_NumAutoroute  
                       and nom       = @va_NomTroncon_in
                  
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
         
         begin
         
            /*A
            ** Verification de leexistence et recherche du numero de leautoroute
            */
         
            select @vl_NumAutoroute = numero from CFT..RES_AUT 
              where nom = @va_NomAutoroute_in
                
          
            if @vl_NumAutoroute <> null
            begin
         
               /*A 
               ** Insertion ou Modification du Troncon
               **
               ** Verification de leexistence et recherche du Troncon dans RES_TRC
               */
         
               select @vl_NumTroncon = autoroute from CFT..RES_TRC
                 where nom       = @va_NomTroncon_in
                   and autoroute = @vl_NumAutoroute
               
               /*A
               ** Le Troncon n'existe pas dans RES_TRC
               */
            
               if @vl_NumTroncon = null
               begin
              
                  /*A
                  ** Creation positionne =>
                  ** Insertion du Troncon dans la table RES_TRC
                  */
                  
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     insert into CFT..RES_TRC
                     values ( @va_NomTroncon_in, @vl_NumAutoroute, @va_PRdebut_in, @va_PRfin_in )
                  
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
                  ** Creation non positionne =>
                  ** Affectation du parametre de sortie Resultat a XDC_ECHEC
                  */
         
                     select @va_Resultat_out = XDC_ECHEC
               end
               
               else
         
               /*A
               ** Le Troncon existe deja
               */      
            
               begin
           
                  /*A
                  ** Creation positionne =>
                  ** Modification de le Troncon dans la table RES_TRC
                  */
                
                  if @va_Creation_in = XDC_VRAI
                  begin
                  
                     update CFT..RES_TRC
    	             set PR_debut = @va_PRdebut_in,
    	                 PR_fin   = @va_PRfin_in
    	             where autoroute = @vl_NumAutoroute  
                       and nom       = @va_NomTroncon_in
                     
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
                  ** Marquage de suppression de le Troncon dans la table RES_TRC
                  */
                  
                  begin
                  
                     delete CFT..RES_TRC   
    	             where autoroute = @vl_NumAutoroute  
                       and nom       = @va_NomTroncon_in
                  
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
